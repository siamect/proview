/* rt_gdh.c -- <short description>

   PROVIEW/R
   Copyright (C) 1996-98 by Mandator AB.

   This module contains the access routines to the Global Data
   Handler. Those routines are callable from application level.  */

#if defined(OS_ELN)
# include $vaxelnc
# include stdio
# include string
#else
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
#endif


#include "pwr.h"              
#include "co_cdh.h"
#include "co_time.h"
#include "rt_gdh_msg.h"
#include "rt_hash_msg.h"
#include "rt_pwr_msg.h"
#include "pwr_class.h"
#include "rt_errh.h"
#include "rt_gdb.h"
#include "rt_gdh.h"
#include "rt_vol.h"
#include "rt_mvol.h"
#include "rt_dvol.h"
#include "rt_cmvolc.h"
#include "rt_cvol.h"
#include "rt_cvolc.h"
#include "rt_ndc.h"
#include "rt_net.h"
#include "rt_sub.h"
#include "rt_subc.h"
#include "rt_sanc.h"
#include "rt_dl.h"

#if defined(OS_ELN)
  /* For ELN and Lynx , the 'gdh_Lock' code also takes out a mutex lock 
     to prevent 'gdh' from being called simultaneously from more than 
     1 process/thread in a job. 
     If this happens, there is a potential risk of deadlocking
     in the 'net' interface.  */

# define gdh_Lock	ELN$LOCK_MUTEX(gdbroot->thread_lock); gdb_Lock
# define gdh_Unlock	gdb_Unlock; ELN$UNLOCK_MUTEX(gdbroot->thread_lock)

#elif defined (OS_LYNX) || defined(OS_LINUX)
# define gdh_Lock	pthread_mutex_lock(&gdbroot->thread_lock); gdb_Lock
# define gdh_Unlock	gdb_Unlock; pthread_mutex_unlock(&gdbroot->thread_lock)

#else
# define gdh_Lock	gdb_Lock
# define gdh_Unlock	gdb_Unlock
#endif

#define gdh_ScopeLock	 gdh_Lock;do
#define gdh_ScopeUnlock  while(0);gdh_Unlock

#define touchObject(op)  if (op != NULL && op->l.flags.b.isCached) cvolc_TouchObject(op)



#define STS_M_SEVERITY 0x7
#define STS_K_WARNING 0	    	/**< WARNING                          */
#define STS_K_SUCCESS 1        	/**< SUCCESSFUL COMPLETION            */
#define STS_K_ERROR 2          	/**< ERROR                            */
#define STS_K_INFO 3           	/**< INFORMATION                      */
#define STS_K_SEVERE 4        	/**< SEVERE ERROR                     */


/**
 * @brief This routine fetches the minimum, not visible alarmlevel for a 
 * certain object. 
 *
 * It inspects all its parents and
 * maximizes the blocking levels. 
 */
static void
getAlarmVisibility (
  pwr_tStatus		*status,   /**< ZZZ */
  gdb_sObject		*op,       /**< ZZZ */
  pwr_tUInt32 		*avis      /**< ZZZ */
) 
{
  gdb_sObject		*pop;
  pwr_dStatus(sts, status, GDH__SUCCESS);


  *avis = op->l.al.b;
  pop = op;    
  while (pop->l.por != pool_cNRef) {
    pop = pool_Address(sts, gdbroot->pool, pop->l.por);
    if (EVEN (*sts)) return;

    *avis = MAX(*avis, pop->l.al.b);
  } 
}
/**
 * @brief This routine translates an attribute reference structure 
 * into the name of the attribute that it denominates.
 * @return pwr_tStatus
 */
pwr_tStatus
gdh_AttrrefToName (
  pwr_sAttrRef		*arp,     /**< Supplies the attribute reference structure 
                                       that defines an attribute. */
  char			*name,    /**< Receives the name of the form <object name>,
				       <attributename>. */
  unsigned int		size,     /**< Supplies the maximun length of the buffer */
  pwr_tBitMask		nametype  /**< ZZZ */
)
{
  pwr_tStatus		sts = GDH__SUCCESS;
  mvol_sAttribute	Attribute;
  mvol_sAttribute	*ap;
  unsigned int		lsize;
  char			string[512];
  char			*s = NULL;
  cdh_mName		lnametype;

  if (arp  == NULL) return GDH__BADARG;
  if (name == NULL) return GDH__BADARG;

  memset(&Attribute, 0, sizeof(Attribute));

  if (nametype == cdh_mNName)
    lnametype.m = cdh_mName_pathStrict;
  else
    lnametype.m = nametype;

  gdh_ScopeLock {

    ap = vol_ArefToAttribute(&sts, &Attribute, arp, gdb_mLo_global, vol_mTrans_all);
    if (ap == NULL) break;

    touchObject(ap->op);
    s = vol_AttributeToName(&sts, ap, lnametype.m, string); 

  } gdh_ScopeUnlock;

  if (ap == NULL && lnametype.b.fallback != cdh_mName_eFallback_strict) {
    sts = GDH__SUCCESS;
    s = cdh_ArefToString(string, arp, 1);
  }

  if (s != NULL) {
    lsize = strlen(s);
    if (lsize >= size)
      sts = GDH__NAMEBUF;
    strncpy(name, s, lsize + 1);
  }

  return sts;
}

/** 
 * @brief Converts a class & attribute to attrref format.
 *
 * The Objid of the attrref is left untouched.  
 * @return pwr_tStatus 
 */

pwr_tStatus
gdh_ClassAttrToAttrref (
  pwr_tClassId		cid,    /**< The class identity whose attribute we want to examin */
  char			*name,  /**< The name of the attribute we want */
  pwr_sAttrRef		*arp    /**< Receives the attribute description. */
)
{
  pwr_tStatus		sts = GDH__SUCCESS;
  mvol_sAttribute	Attribute;
  mvol_sAttribute	*ap;
  cdh_sParseName	ParseName;
  cdh_sParseName	*pn = NULL;
  char			buff[512] = ".";

  if (arp  == NULL) return GDH__BADARG;

  /* If no attribute name is given, Assume whole body.  */

  if (name != NULL && *name != '\0') {
    pn = cdh_ParseName(&sts, &ParseName, pwr_cNObjid, name, 0);
    if (pn == NULL) {
      strcat(buff, name);
      pn = cdh_ParseName(&sts, &ParseName, pwr_cNObjid, buff, 0);
    }
    if (pn == NULL)
      return GDH__BADNAME;
  }    
  
  memset(&Attribute, 0, sizeof(Attribute));

  gdh_ScopeLock {

    ap = mvol_AnameToAttribute(&sts, &Attribute, cid, pn);
    if (ap == NULL) break;
    mvol_AttributeToAref(&sts, ap, arp);

  } gdh_ScopeUnlock;

  return sts;
}

/** 
 * @brief Get the class identity corresponding to a class
 * with name 'classname'.
 * @return pwr_tStatus   
 */
pwr_tStatus
gdh_ClassNameToId (
  char			*name, /**< ZZZ */
  pwr_tClassId		*cid   /**< ZZZ */
)
{
  pwr_tStatus		sts = GDH__SUCCESS;
  gdb_sClass		*cp;
  
  if (name == NULL) return GDH__BADARG;

  gdh_ScopeLock {

    cp = mvol_NameToClass(&sts, name);
    if (cp != NULL && cid != NULL)
      *cid = cp->cid;

  } gdh_ScopeUnlock;

  return sts;
}

/**
 * @brief Create and initialize a new local object.
 *
 * Space is allocated for the object which must not exist. 
 * This call is possible only if the object is created
 * in a local volume, i.e a volume owned by the local node.
 * The volume must furthermore be of class $DynamicVolume or $SystemVolume.
 *
 * All reachable nodes, who have mounted the volume in question,
 * are notified about this new object.   
 * It is possible to require a certain objid, but the the objid must not
 * exist in the requested volume.
 *
 * If the volume identity part of the required objid (objid.vid) is zero
 * the the volume id indicated by the given name is used.
 * If the objid.vid != zero, then it must match the volume id indicated by
 * the name.  
 * @return pwr_tStatus
*/

pwr_tStatus
gdh_CreateObject (
  char			*name,		/**< Null terminated object name string.  */
  pwr_tClassId		cid,		/**< What class should the object be of?  */
  unsigned int		size,		/**< Size of object body.  */
  pwr_tObjid		*oid,		/**< Address of a pwr_tObjid where
					   the object identity is to be stored.  */
  pwr_tObjid		req_oid,	/**< Requested objid, */
  pwr_tBitMask		flags,		/**< Alias client or mount client?  */
  pwr_tObjid		soid		/**< Server objid.  */
)
{
  pwr_tStatus		sts = GDH__SUCCESS;
  gdb_sObject		*op;
  cdh_sParseName	ParseName;
  cdh_sParseName	*pn;

  if (name == NULL) return GDH__BADARG;
  if (oid == NULL)  return GDH__BADARG;

  pn = cdh_ParseName(&sts, &ParseName, pwr_cNObjid, name, 0);
  if (pn == NULL) return sts;
  if (pn->flags.b.idString) return GDH__NOSUCHOBJ;

  gdh_ScopeLock {

    op = dvol_CreateObject(&sts, pn, cid, size, req_oid);

  } gdh_ScopeUnlock;

  if (op != NULL)
    *oid = op->g.oid;

  return sts;
}

/**
 * @brief Remove a local object.
 *
 * Object header and the associated body are removed.
 * This call is possible only if the object is
 * in a local volume, i.e a volume owned by the local node.
 * The volume must be of class $DynamicVolume or $SystemVolume.
 * The object must not be parent to any other object.
 *
 * All reachable nodes, who have mounted the volume in question,
 * are notified about the removal of this object.
 * @return pwr_tStatus 
 */

pwr_tStatus
gdh_DeleteObject (
  pwr_tObjid		oid  /**< The identity of the object to delete. */
)
{
  pwr_tStatus		sts = GDH__SUCCESS;

  if (cdh_ObjidIsNull(oid))
    return GDH__NOSUCHOBJ;

  gdh_ScopeLock {

    dvol_DeleteObject(&sts, oid);

  } gdh_ScopeUnlock;

  return sts;
}

/** 
 * @brief Request a direct link to the data denoted by an attribute reference.
 * 
 * The object owning the data must be owned by the local node.
 * The caller can change the data without restriction. 
 * @return pwr_tStatus 
 */
pwr_tStatus
gdh_DLRefObjectInfoAttrref (
  pwr_sAttrRef		*arp,      /**< ZZZ */
  void			**infop,   /**< ZZZ */
  pwr_tDlid		*dlid      /**< ZZZ */
)
{
  pwr_tStatus		sts = GDH__SUCCESS;
  dl_sLink		*dp;
  mvol_sAttribute	Attribute;
  mvol_sAttribute	*ap;
  void			*p;

  if (arp == NULL) return GDH__BADARG;

  memset(&Attribute, 0, sizeof(Attribute));

  gdh_ScopeLock {

    ap = vol_ArefToAttribute(&sts, &Attribute, arp, gdb_mLo_native, vol_mTrans_all);
    if (ap == NULL) break;
    p = vol_AttributeToAddress(&sts, ap);    
    if (p == NULL) break;

    dp = dl_Create(&sts, ap, arp);
    if (dp == NULL) break;

    if (infop != NULL) *infop = p;
    if (dlid  != NULL) *dlid = dp->dlid;

  } gdh_ScopeUnlock;
    
  return sts;
}

/** 
 * @brief Terminate direct linking of an object or an object parameter.
 * 
 * The pointer returned by DLRefObjectInfo can become invalid
 * after this call.
 * @return pwr_tStatus  
 */

pwr_tStatus
gdh_DLUnrefObjectInfo (
  pwr_tDlid		dlid  /** <The id of the direct link we want to break */
)
{
  pwr_tStatus		sts = GDH__SUCCESS;

  gdh_ScopeLock {

    dl_Cancel(&sts, dlid);

  } gdh_ScopeUnlock;

  return sts;

}

/** 
 * @brief Terminates all direct links set up by this user (job/process).  
 */

void
gdh_DLUnrefObjectInfoAll ()
{

  gdh_ScopeLock {

    dl_CancelUser(gdbroot->my_pid);

  } gdh_ScopeUnlock;

}

/**  
 * @brief Get the alarm status for a certain object.  
 * @return pwr_tStatus
 */

pwr_tStatus
gdh_GetAlarmInfo (
  pwr_tObjid		oid,   /**< Object identity */
  pwr_tUInt32		*a,    /**< Receives the alarm status. Can be NULL */
  pwr_tUInt32		*maxa, /**< Receives the highest alarm level of the object. Can be NULL */
  pwr_tUInt32		*b,    /**< Receives the blocking level of the object. CAn be NULL */        
  pwr_tUInt32		*maxb, /**< Receives the highest existing blocking level of the object. Can be NULL */ 
  pwr_tUInt32		*alarmvisibility /**< ZZZ */
)
{
  pwr_tStatus		sts = GDH__SUCCESS;
  gdb_sObject		*op;

  gdh_ScopeLock {

    op = vol_OidToObject(&sts, oid, gdb_mLo_global, vol_mTrans_none, cvol_eHint_none);
    if (op == NULL) break;

    touchObject(op);

    if (!op->l.flags.b.isNative) sanc_Subscribe(NULL, op);

    if (a != NULL) *a = op->l.al.a;

    if (maxa != NULL) {
#if 0
      *maxa = 0;
#else
      *maxa = op->l.al.maxa;
      if (op->l.flags.b.isNative) 
	*maxa = MAX(*maxa, op->u.n.ral.maxa);
#endif
    }


#if 0
    if (b != NULL) *b = 0;
#else
    if (b != NULL) *b = op->l.al.b;
#endif

    if (maxb != NULL) {
#if 0
      *maxb = 0;
#else
      *maxb = op->l.al.maxb;
      if (op->l.flags.b.isNative) 
	*maxb = MAX(*maxb, op->u.n.ral.maxb);
#endif
    }

    if (alarmvisibility != NULL) {
#if 0
       *alarmvisibility = 0;
#else
      getAlarmVisibility(&sts, op, alarmvisibility);
#endif
    }

  } gdh_ScopeUnlock;

  return sts;
}

/** 
 * @brief Get the node identity of the local node.  
 * @return pwr_tStatus
 */
pwr_tStatus
gdh_GetNodeIndex (
  pwr_tNodeId		*nid /**< Receives node index of the local node */
)
{

  if (nid == NULL) return GDH__BADARG;

  gdh_ScopeLock {

    *nid = gdbroot->my_node->nid;

  } gdh_ScopeUnlock;

  return GDH__SUCCESS;
}

/**
 * @brief Get the node identity of the owner node of an object.  
 * @return pwr_tStatus
 */

pwr_tStatus
gdh_GetObjectNodeIndex (
  pwr_tObjid		oid, /**< Object identity. */
  pwr_tNodeId		*nid /**< Receives the node index. */
)
{
  pwr_tStatus		sts = GDH__SUCCESS;
  gdb_sObject		*op;
  gdb_sVolume		*vp;

  if (nid == NULL) return GDH__BADARG;

  gdh_ScopeLock {

    op = vol_OidToObject(&sts, oid, gdb_mLo_global, vol_mTrans_all, cvol_eHint_none);
    if (op == NULL) break;

    touchObject(op);

    vp = pool_Address(NULL, gdbroot->pool, op->l.vr);
    *nid = vp->g.nid;

  } gdh_ScopeUnlock;

  return sts;;
}

/** 
 * @brief Fetch data from an object or an attribute.
 *
 * The caller gets blocked until the information is available.
 * The caller is responsible for allocating a buffer
 * where the information can be stored. If that buffer
 * is to small, the information will be truncated.  
 * @return pwr_tStatus
 */

pwr_tStatus
gdh_GetObjectInfo (
  char			*name,		/**<  Name of object or object.parameter.	 */
  pwr_tAddress		bufp,		/**<  Pointer to a buffer of 'bufsize' bytes
					      to be filled with requested information.  */
  pwr_tUInt32		bufsize		/**<  Size of the 'bufp' buffer.	*/
)
{
  pwr_tStatus		sts = GDH__SUCCESS;
  cdh_sParseName	parseName;
  cdh_sParseName	*pn;
  gdb_sVolume		*vp;
  gdb_sNode		*np = NULL;
  mvol_sAttribute	attribute;
  mvol_sAttribute	*ap;
  void			*p;
  pwr_sAttrRef		aref;
  pwr_sAttrRef		*arp;
  pwr_sAttrRef		raref;
  pwr_sAttrRef		*rarp = NULL;
  gdb_sCclass		*ccp;
  gdb_sCclass		*ccpLocked = NULL;
  pwr_tBoolean		equal;
  pwr_tBoolean		fetched;
  pwr_tUInt32		ridx = ULONG_MAX;
  

  if (name == NULL) return GDH__BADARG;

  pn = cdh_ParseName(&sts, &parseName, pwr_cNObjid, name, 0);
  if (pn == NULL) return sts;

  memset(&attribute, 0, sizeof(attribute));

  gdh_Lock;

  do {
    ap = vol_NameToAttribute(&sts, &attribute, pn, gdb_mLo_global, vol_mTrans_all);
    if (ap == NULL) break;
    if (ap->op == NULL) break;
    
    touchObject(ap->op);

    p = vol_AttributeToAddress(&sts, ap);
    if (p != NULL) {
      memcpy(bufp, p, MIN(ap->size, bufsize));
      break;
    } else if (ap->op->l.flags.m & gdb_mLo_native) {
      break;
    }

    /* Try remote.  */

    arp = mvol_AttributeToAref(&sts, ap, &aref);
    if (arp == NULL) break;

    vp = pool_Address(NULL, gdbroot->pool, ap->op->l.vr);
    np = hash_Search(&sts, gdbroot->nid_ht, &vp->g.nid);
    if (np == NULL)
      break;

    ccp = NULL;
    /* Get cached class if needed */
    if (!ap->op->u.c.flags.b.classChecked || !ap->op->u.c.flags.b.classEqual) {
      ccp = cmvolc_GetCachedClass(&sts, np, vp, ap, &equal, &fetched);
      if (EVEN(sts)) {
        np = NULL;
        break;
      }
      if (equal)
        ccp = NULL;
        
      if (ccpLocked == NULL && ccp != NULL && !equal) {
        cmvolc_LockClass(NULL, ccp);
        ccpLocked = ccp;
      }
          

      /* If gdb has been unlocked, refresh pointers */
      /** @todo Check if we can do it more efficient, eg. vol_ArefToAttribute */
      if (fetched) {
        memset(&attribute, 0, sizeof(attribute));
        np = NULL;
        continue;
      }

      if (equal)
        break;

      rarp = ndc_NarefToRaref(&sts, ap, arp, ccp, &ridx, &raref, &equal);        
    }
    break;    
  } while (1);

  gdh_Unlock;


  if (np != NULL && ODD(sts)) {
    if (equal)
      cvolc_GetObjectInfo(&sts, np, arp, NULL, NULL, ULONG_MAX, ap, bufp, bufsize);
    else
      cvolc_GetObjectInfo(&sts, np, arp, ccp, rarp, ridx, ap, bufp, bufsize);
  }
  

  if (ccpLocked) {
    gdb_Lock;
    cmvolc_UnlockClass(NULL, ccpLocked);
    gdb_Unlock;
  }
  

  return sts;
}

/** 
 * @brief Fetch the data of an attribute or a whole object body.
 *   
 * The function blocks until the information is available.
 * The caller is responsible for allocating a buffer where
 * the information can be stored. If that buffer is to small
 * the information is truncated.   
 * @return pwr_tStatus  
*/

pwr_tStatus
gdh_GetObjectInfoAttrref (
  pwr_sAttrRef		*arp,     /**< Attribute reference descriptor that defines an object or an object reference */
  void			*bufp,    /**< Receives the requested information */
  unsigned int		bufsize   /**< The size in bytes of the data buffer */
)
{
  pwr_tStatus		sts = GDH__SUCCESS;
  gdb_sVolume		*vp;
  gdb_sNode		*np = NULL;
  mvol_sAttribute	attribute;
  mvol_sAttribute	*ap;
  void			*p;
  gdb_sCclass		*ccp;
  gdb_sCclass		*ccpLocked = NULL;
  pwr_sAttrRef		raref;
  pwr_sAttrRef		*rarp = NULL;
  pwr_tBoolean		equal;
  pwr_tBoolean		fetched;
  pwr_tUInt32		ridx = ULONG_MAX;

  memset(&attribute, 0, sizeof(attribute));

  gdh_Lock;
  
  do {
    
    ap = vol_ArefToAttribute(&sts, &attribute, arp, gdb_mLo_global, vol_mTrans_all);
    if (ap == NULL || ap->op == NULL) break;

    touchObject(ap->op);

    p = vol_AttributeToAddress(&sts, ap);
    if (p != NULL) {
      memcpy(bufp, p, MIN(ap->size, bufsize));
      break;
    } else if ((ap->op->l.flags.m & gdb_mLo_native) != 0) {
      break;
    }

    /* Try remote.  */

    vp = pool_Address(NULL, gdbroot->pool, ap->op->l.vr);
    np = hash_Search(&sts, gdbroot->nid_ht, &vp->g.nid);

    if (np == NULL)
      break;

    ccp = NULL;
    /* Get cached class if needed */
    if (!ap->op->u.c.flags.b.classChecked || !ap->op->u.c.flags.b.classEqual) {
      ccp = cmvolc_GetCachedClass(&sts, np, vp, ap, &equal, &fetched);
      if (EVEN(sts)) {
        np = NULL;
        break;
      }
      if (equal)
        ccp = NULL;
        
      if (ccpLocked == NULL && ccp != NULL && !equal) {
        cmvolc_LockClass(NULL, ccp);
        ccpLocked = ccp;
      }

      /* If gdb has been unlocked, refresh pointers */
      if (fetched) {
        memset(&attribute, 0, sizeof(attribute));
        np = NULL;
        continue;
      }

      if (equal)
        break;
        
      rarp = ndc_NarefToRaref(&sts, ap, arp, ccp, &ridx, &raref, &equal);
    }

    break;    
  } while (1);

  gdh_Unlock;


  if (np != NULL && ODD(sts)) {
    if (equal)
      cvolc_GetObjectInfo(&sts, np, arp, NULL, NULL, ULONG_MAX, ap, bufp, bufsize);
    else
      cvolc_GetObjectInfo(&sts, np, arp, ccp, rarp, ridx, ap, bufp, bufsize);
  }
  

  if (ccpLocked) {
    gdb_Lock;
    cmvolc_UnlockClass(NULL, ccpLocked);
    gdb_Unlock;
  }

  return sts;
}

/**
 * @brief Get the size of the body of an object. 
 *
 * This information
 * can be retrieved both for local and remote objects.  
 * @return pwr_tStatus
 */

pwr_tStatus
gdh_GetObjectSize (
  pwr_tObjid		oid,   /**< The object identity. */ 
  pwr_tUInt32		*size  /**< Receives the size in bytes of the object. */
)
{
  pwr_tStatus		sts = GDH__SUCCESS;
  gdb_sObject		*op;

  if (size == NULL) return GDH__BADARG;

  gdh_ScopeLock {

    op = vol_OidToObject(&sts, oid, gdb_mLo_global, vol_mTrans_none, cvol_eHint_none);
    if (op != NULL) {
      touchObject(op);
      *size = op->g.size;
    }

  } gdh_ScopeUnlock;

  return sts;
}

/** 
 * @brief Get the class identifier of an object. 
 *
 * This information
 * can be retrieved both for local and remote objects.  
 * @return pwr_tStatus
 */
pwr_tStatus
gdh_GetObjectClass (
  pwr_tObjid		oid,   /**< The identity of the object. */
  pwr_tClassId		*cid   /**< Receives the object class identity. */
)
{
  pwr_tStatus		sts = GDH__SUCCESS;
  gdb_sObject		*op;
  gdb_sClass		*cp;
  pwr_tStatus		lsts;

  if (cid == NULL) return GDH__BADARG;

  gdh_ScopeLock {

    op = vol_OidToObject(&sts, oid, gdb_mLo_global, vol_mTrans_all, cvol_eHint_none);
    if (op != NULL) {
      touchObject(op);
      *cid = op->g.cid;

      if (op->l.flags.b.isCached) {
        cp = hash_Search(&lsts, gdbroot->cid_ht, cid);
        if (cp == NULL) {
          cmvolc_GetNonExistingClass(&lsts, op);
        }
      }

    }
    

  } gdh_ScopeUnlock;

  return sts;
}

/** 
 * @brief Get the type or class identifier of an attribute reference. 
 * @return pwr_tStatus
 */
pwr_tStatus
gdh_GetAttrRefTid (
  pwr_sAttrRef		*arp,  /**< The attribute reference. */
  pwr_tTid		*tid   /**< Receives the aref type or class identity. */
)
{
  pwr_tStatus		sts = GDH__SUCCESS;
  mvol_sAttribute	*ap;
  mvol_sAttribute	attribute;

  if ( arp->Flags.b.Object && arp->Body) {
    *tid = arp->Body & ~7;
    return GDH__SUCCESS;
  }
  if ( arp->Body == 0 && arp->Offset == 0)
    return gdh_GetObjectClass( arp->Objid, tid);

  gdh_ScopeLock {

    ap = vol_ArefToAttribute(&sts, &attribute, arp, gdb_mLo_global, vol_mTrans_all);
    if (ap != NULL) touchObject(ap->op);
    
  } gdh_ScopeUnlock;

  if (ap != NULL)
    *tid  = ap->adef->TypeRef;

  return sts;
}

/** 
 * @brief Return the location of an object.
 *
 * The argument 'location' will get the
 * value TRUE if the object is local,
 * i.e. the object is in a volume owned by the local node,
 * else the value FALSE.  
 * @return pwr_tStatus 
 */

pwr_tStatus
gdh_GetObjectLocation (
  pwr_tObjid		oid,       /**< The identity of the object.*/
  pwr_tBoolean		*location  /**< Receives the location.
                                        TRUE means local and FALSE remote.*/
)
{
  pwr_tStatus		sts = GDH__SUCCESS;
  gdb_sObject		*op;

  if (location == NULL) return GDH__BADARG;

  gdh_ScopeLock {

    op = vol_OidToObject(&sts, oid, gdb_mLo_global, vol_mTrans_all, cvol_eHint_none);
    if (op != NULL) {
      touchObject(op);
      *location = (op->l.flags.m & gdb_mLo_native) != 0;
    }

  } gdh_ScopeUnlock;

  return sts;
}


/** 
 * @brief Gets the direct link count of an object.
 * 
 * This information is can be fetched only for local objects.
 * @return pwr_tStatus
 */
pwr_tStatus
gdh_GetObjectDLCount (
  pwr_tObjid		 oid,    /**< The object identity. */
  pwr_tUInt32		*count  /**< Receives the value of the direct access counter of the object.*/
)
{
  pwr_tStatus		sts = GDH__SUCCESS;
  gdb_sObject		*op;

  if (count == NULL) return GDH__BADARG;

  gdh_ScopeLock {

    op = vol_OidToObject(&sts, oid, gdb_mLo_native, vol_mTrans_all, cvol_eHint_none);
    if (op != NULL) {
      *count = op->u.n.dlcount;
    }

  } gdh_ScopeUnlock;

  return sts;
}

/** 
 * @brief Get the object identity of the parent of an object.  
 * @return pwr_tStatus
 */ 

pwr_tStatus
gdh_GetParent (
  pwr_tObjid		oid,          /**< The identity of the object. */
  pwr_tObjid		*new_oid      /**< Receives the object identity of the parent. */
)
{
  pwr_tStatus		sts = GDH__SUCCESS;
  gdb_sObject		*op;

  if (new_oid == NULL) return GDH__BADARG;

  gdh_ScopeLock {

    op = vol_OidToObject(&sts, oid, gdb_mLo_global, vol_mTrans_all, cvol_eHint_none);
    if (op != NULL) {
      touchObject(op);
      if (op->g.f.poid.oix == pwr_cNObjectIx) {
	*new_oid = pwr_cNObjid;
	sts = GDH__NO_PARENT;
      } else {
	*new_oid = op->g.f.poid;
      }
    }

  } gdh_ScopeUnlock;

  return sts;
}


/** 
 * @brief Get the object identity of the local parent of an object.  
 * @return pwr_tStatus
 */ 

pwr_tStatus
gdh_GetLocalParent (
  pwr_tObjid		oid,       /**< The identity of the object. */
  pwr_tObjid		*new_oid   /**< Receives the object identity of local parent. */
)
{
  pwr_tStatus		sts = GDH__SUCCESS;
  gdb_sObject		*op;

  if (new_oid == NULL) return GDH__BADARG;

  gdh_ScopeLock {

    op = vol_OidToObject(&sts, oid, gdb_mLo_global, vol_mTrans_none, cvol_eHint_none);
    if (op != NULL) {
      touchObject(op);
      if (op->g.f.poid.oix == pwr_cNObjectIx) {
	*new_oid = pwr_cNObjid;
	sts = GDH__NO_PARENT;
      } else {
	*new_oid = op->g.f.poid;
      }
    }

  } gdh_ScopeUnlock;

  return sts;
}


/** 
 * @brief Checks the oldness of a certain subscription.
 *
 * Data gets `old' after a certain timeout-time,
 * when no updates are refreshing the data. 
 * @sa gdh_examples.c
 * 
 * @return pwr_tStatus
 */
pwr_tStatus
gdh_GetSubscriptionOldness (
  pwr_tSubid		sid,		/**<  Subscription to examine.  */
  pwr_tBoolean		*old,		/**<  Pointer to where the oldness
                                              flag is stored or NULL.  */
  pwr_tTime		*lastupdate,	/**<  Pointer to where the 64-bit
                                              time value of last update is
                                              stored or NULL.  */
  pwr_tStatus		*status		/**<  Status of last data transfer.  */
)
{
  pwr_tStatus		sts = GDH__SUCCESS;
  sub_sClient		*cp;
  cdh_uRefId		rid;
  void			*p;

  rid.pwr = sid;

  gdh_ScopeLock {

    p = hash_Search(&sts, gdbroot->subc_ht, &sid);
    if (p == NULL) break;

    if (rid.r.vid_3 == cdh_eVid3_subid) {
      cp = (sub_sClient *) p;

      if (old	      != NULL) *old = cp->old;
      if (lastupdate  != NULL) memcpy(lastupdate, &cp->lastupdate, sizeof(cp->lastupdate));
      if (status      != NULL) *status = cp->sts;
    } else if (rid.r.vid_3 == cdh_eVid3_dlid) {
      if (old	      != NULL) *old = FALSE;
      if (lastupdate  != NULL) clock_gettime(CLOCK_REALTIME, lastupdate);
      if (status      != NULL) *status = GDH__SUCCESS;
    } else {
      sts = GDH__WEIRD;
    }

  } gdh_ScopeUnlock;

  return sts;
}

/** 
 * @brief Get the object identity of the first child of an object.  
 * @return pwr_tStatus
 */

pwr_tStatus
gdh_GetChild (
  pwr_tObjid		oid,        /**< The object identity. */
  pwr_tObjid		*new_oid    /**< Recevies the object identity of the first child. */
)
{
  pwr_tStatus		sts = GDH__SUCCESS;
  gdb_sObject		*op;

  if (new_oid == NULL) return GDH__BADARG;

  gdh_ScopeLock {

    op = vol_OidToObject(&sts, oid, gdb_mLo_global, vol_mTrans_all, cvol_eHint_child);
    if (op != NULL) {
      touchObject(op);
      if (op->g.flags.b.isParent) {
	*new_oid = op->g.soid;
      } else {
	sts = GDH__NO_CHILD;
      }
    }

  } gdh_ScopeUnlock;

  return sts;
}

/**
 * @brief Get the object identity of the next sibling (i.e.
 * the objects have the parent in common). 
 * @return pwr_tStatus
 */
pwr_tStatus
gdh_GetNextSibling (
  pwr_tObjid		oid,      /**< The object identity. */
  pwr_tObjid		*new_oid  /** Receives the object identity of the next sibling.*/
)
{
  pwr_tStatus		sts = GDH__SUCCESS;
  gdb_sObject		*op;
  gdb_sObject		*pop;
  pwr_tObjid		noid;

  if (new_oid == NULL) return GDH__BADARG;

  gdh_ScopeLock {

    op = vol_OidToObject(&sts, oid, gdb_mLo_global, vol_mTrans_none, cvol_eHint_next);
    if (op != NULL) {
      touchObject(op);
      noid.vid = op->g.oid.vid;
      noid.oix = op->g.sib.flink;
      pop = pool_Address(NULL, gdbroot->pool, op->l.por);
    }

  } gdh_ScopeUnlock;

  if (op == NULL) return sts;

  if (pop == NULL || noid.oix == pop->g.soid.oix) {
    sts = GDH__NO_SIBLING;
  } else {
    *new_oid = noid;
  }

  return sts;
}

/** 
 * @brief Get the object identity of the previous sibling (i.e.
 * the previous object with the same parent).  
 * @return pwr_tStatus
*/

pwr_tStatus
gdh_GetPreviousSibling (
  pwr_tObjid		oid,        /**< The object identity. */
  pwr_tObjid		*new_oid    /**< Receievs the object identity for the previous sibling. */
)
{
  pwr_tStatus		sts = GDH__SUCCESS;
  gdb_sObject		*op;
  gdb_sObject		*pop;
  pwr_tObjid		noid;

  if (new_oid == NULL) return GDH__BADARG;

  gdh_ScopeLock {

    op = vol_OidToObject(&sts, oid, gdb_mLo_global, vol_mTrans_none, cvol_eHint_prev);
    if (op != NULL) {
      touchObject(op);
      noid.vid = op->g.oid.vid;
      noid.oix = op->g.sib.blink;
      pop = pool_Address(NULL, gdbroot->pool, op->l.por);
    }

  } gdh_ScopeUnlock;

  if (op == NULL) return sts;

  if (pop == NULL || oid.oix == pop->g.soid.oix) {
    sts = GDH__NO_SIBLING;
  } else {
    *new_oid = noid;
  }

  return sts;

}

/** 
 * @brief Get the object identity for the first object not
 * having a parent, i.e. the top level of the
 * naming hierarchy.  
 * @return pwr_tStatus 
 */
pwr_tStatus
gdh_GetRootList (
  pwr_tObjid		*oid /**< Receives the object identity. */
)
{
  pwr_tStatus		sts = GDH__SUCCESS;
  gdb_sObject		*op;

  /*  Check arguments.  */
  if (oid == NULL) return GDH__BADARG;

  gdh_ScopeLock {

    /*  Get the first child of the rootvolume.  */

    op = hash_Search(&sts, gdbroot->oid_ht, &gdbroot->my_volume->g.oid);
    if (op != NULL) {
      if (op->g.flags.b.isParent) {
	*oid = op->g.soid;
      } else {
	sts = GDH__NOSUCHOBJ;
      }
    }

  } gdh_ScopeUnlock;

  return sts;
}

/** 
 * @brief Get the object identity of the first object
 * in the list of objects of a specified class.
 *
 * The call shows only private volumes
 * (of class $RootVolume, $SubVolume,
 * $DynamicVolume, $ClassVolume and $SystemVolume),
 * and shared volumes.  
 * @return pwr_tStatus 
*/

pwr_tStatus
gdh_GetClassList (
  pwr_tClassId		cid,        /**< The class identity. */
  pwr_tObjid		*oid        /**< Receives the object identity. */
)
{
  pwr_tStatus		sts = GDH__SUCCESS;
  gdb_sObject		*op = NULL;

  gdh_ScopeLock {

    op = mvol_ClassList(&sts, cid, pwr_cNObjid, mvol_eList_first);
    if (op != NULL) {
      *oid = op->g.oid;
    }

  } gdh_ScopeUnlock;

  return sts;
}

/**
 * @brief Get the object identity of the next object
 * in the list of objects of a certain class.
 *
 * The visibility of objects in the class list are
 * restricted to objects in private volumes,
 * (of class $RootVolume, $SubVolume,
 * $DynamicVolume, $ClassVolume and $SystemVolume),
 * and in shared volumes.  
 * @return pwr_tStatus
 */

pwr_tStatus
gdh_GetNextObject (
  pwr_tObjid		oid,         /**< The object identity. */
  pwr_tObjid		*new_oid     /**< Receives the object identity */
)
{
  pwr_tStatus		sts = GDH__SUCCESS;
  gdb_sObject		*op;

  /* Check arguments. */
  if (new_oid == NULL) return GDH__BADARG;

  gdh_ScopeLock {

    op = mvol_ClassList(&sts, pwr_cNClassId, oid, mvol_eList_next);
    if (op != NULL) {
      *new_oid = op->g.oid;
    }

  } gdh_ScopeUnlock;

  return sts;
}

/** 
 * @brief Get the attribute reference of the first object
 * in the list of objects of a specified class.
 * The object can be a solitary object or an attribute object.
 *
 * The call shows only private volumes
 * (of class $RootVolume, $SubVolume,
 * $DynamicVolume, $ClassVolume and $SystemVolume),
 * and shared volumes.  
 * @return pwr_tStatus 
*/

pwr_tStatus
gdh_GetClassListAttrRef (
  pwr_tClassId		cid,        /**< The class identity. */
  pwr_sAttrRef		*arp        /**< Receives the attribute reference. */
)
{
  pwr_tStatus		sts = GDH__SUCCESS;

  gdh_ScopeLock {

    mvol_ClassListAttrRef(&sts, cid, NULL, arp, mvol_eList_first);

  } gdh_ScopeUnlock;

  return sts;
}

/**
 * @brief Get the attrref of the next object
 * in the list of objects of a certain class.
 * The object can be a solitary object or an attribute object.
 *
 * The visibility of objects in the class list are
 * restricted to objects in private volumes,
 * (of class $RootVolume, $SubVolume,
 * $DynamicVolume, $ClassVolume and $SystemVolume),
 * and in shared volumes.  
 * @return pwr_tStatus
 */

pwr_tStatus
gdh_GetNextAttrRef (
  pwr_tClassId		cid,          /**< The class identity. */
  pwr_sAttrRef		*arp,         /**< The attribute reference. */
  pwr_sAttrRef		*new_arp      /**< Receives the attribute reference */
)
{
  pwr_tStatus		sts = GDH__SUCCESS;

  /* Check arguments. */
  if (new_arp == NULL) return GDH__BADARG;

  gdh_ScopeLock {

    mvol_ClassListAttrRef(&sts, cid, arp, new_arp, mvol_eList_next);

  } gdh_ScopeUnlock;

  return sts;
}

/** 
 * @brief Get the attribute reference of the first attribute object
 * of a specified class in the specified object.
 * @return pwr_tStatus 
*/

pwr_tStatus
gdh_GetObjectClassList (
  pwr_tCid		cid,        /**< The class identity. */
  pwr_tOid		oid,	    /**< Host object. */
  pwr_sAttrRef		*arp        /**< Receives the attribute reference. */
)
{
  pwr_tStatus		sts = GDH__SUCCESS;
  pwr_sAttrRef		ar;

  gdh_ScopeLock {

    ar.Objid = oid;
    mvol_ClassListAttrRef(&sts, cid, &ar, arp, mvol_eList_objectfirst);

  } gdh_ScopeUnlock;

  return sts;
}

/**
 * @brief Get the attrref of the next attribute object
 * of a specified class in a specified object.
 * @return pwr_tStatus
 */

pwr_tStatus
gdh_GetNextObjectAttrRef (
  pwr_tClassId		cid,          /**< The class identity. */
  pwr_sAttrRef		*arp,         /**< The attribute reference. */
  pwr_sAttrRef		*new_arp      /**< Receives the attribute reference */
)
{
  pwr_tStatus		sts = GDH__SUCCESS;

  /* Check arguments. */
  if (new_arp == NULL) return GDH__BADARG;

  gdh_ScopeLock {

    mvol_ClassListAttrRef(&sts, cid, arp, new_arp, mvol_eList_objectnext);

  } gdh_ScopeUnlock;

  return sts;
}

/** 
 * @brief Return the objid of the node object for a node identified by a
 * node identifier (of type pwr_tNodeId).
 *  
 * If the node idenetity is specified as zero,
 * the node object for the local node is returned. 
 * @return pwr_tStatus  
 */

pwr_tStatus
gdh_GetNodeObject (
  pwr_tNodeId		nid,   /**< The node index. */
  pwr_tObjid		*oid   /**< Receive the object identity. */
)
{
  pwr_tStatus		sts = GDH__SUCCESS;
  gdb_sNode		*np;

  if (oid == NULL) return GDH__BADARG;

  gdh_ScopeLock {

    if (nid == pwr_cNNodeId) {
      *oid = gdbroot->db->nod_oid;
    } else {
      np = hash_Search(&sts, gdbroot->nid_ht, &nid);
      if (np != NULL) {
	*oid = np->nod_oid;
      }
    }

  } gdh_ScopeUnlock;

  return sts;
}

/** 
 * @brief Get the characteristics of an attribute, given
 * its name and the class it belongs to. 
 *
 * The output arguments
 * can have an actual value of NULL if they are not wanted.  
 * @return pwr_tStatus
 */

pwr_tStatus
gdh_GetAttributeCharacteristics (
  char			*name,		/**< String containing attribute name;
                                             object.attribute or
                                             object.attribute[index].  */
  pwr_tTypeId		*tid,		/**< Address to an integer where
					     the type is stored.  */
  pwr_tUInt32		*size,		/**< Address to an integer where
					     the size is stored.  */
  pwr_tUInt32		*offs,		/**< Address to an integer where
					    the offset is stored.  */
  pwr_tUInt32		*elem		/**< Address to an integer where
					    the # of elements is stored.  */
)
{
  pwr_tStatus		sts = GDH__SUCCESS;
  cdh_sParseName	parseName;
  cdh_sParseName	*pn;
  mvol_sAttribute	attribute;
  mvol_sAttribute	*ap;

  pn = cdh_ParseName(&sts, &parseName, pwr_cNObjid, name, 0);
  if (pn == NULL) return sts;

  memset(&attribute, 0, sizeof(attribute));

  gdh_ScopeLock {

    ap = vol_NameToAttribute(&sts, &attribute, pn, gdb_mLo_global, vol_mTrans_all);
    if (ap != NULL) touchObject(ap->op);
  
  } gdh_ScopeUnlock;

  if (ap != NULL) {
    if (size != NULL) *size = ap->size;
    if (offs != NULL) *offs = ap->offs;
    if (tid  != NULL) *tid  = ap->tid;
    if (elem != NULL) *elem = ap->elem;
  }

  return sts;

}

/** 
 * @brief Get the characteristics of an attribute.
 *  
 * The output arguments
 * can be NULL if they are not wanted.  
 * @return pwr_tStatus
 */

pwr_tStatus
gdh_GetAttributeCharAttrref (
  pwr_sAttrRef		*arp,  /**< The attribute of which to fetch the charactertistics. */
  pwr_tTypeId		*tid,  /**< Receives the attribute type. */
  unsigned int		*size, /**< Receives the size in bytes */
  unsigned int		*offs, /**< Receives the offset of the attribute from the 
                                    beginning of the object .*/
  unsigned int		*elem  /**< Receives the number of elements. */
)
{
  pwr_tStatus		sts = GDH__SUCCESS;
  mvol_sAttribute	attribute;
  mvol_sAttribute	*ap;

  memset(&attribute, 0, sizeof(attribute));

  gdh_ScopeLock {
  
    ap = vol_ArefToAttribute(&sts, &attribute, arp, gdb_mLo_global, vol_mTrans_all);
    if (ap != NULL) touchObject(ap->op);
    
  } gdh_ScopeUnlock;

  if (ap != NULL) {
    if (size != NULL) *size = ap->size;
    if (offs != NULL) *offs = ap->offs;
    if (tid  != NULL) *tid  = ap->tid;
    if (elem != NULL) *elem = ap->elem;
  } 

  return sts;
}

/** 
 * @brief Get the object identity of the previous object
 * in the list of objects of a certain class.
 *
 * The visibility of objects in the class list are
 * restricted to objects in private volumes,
 * (of class $RootVolume, $SubVolume,
 * $DynamicVolume, $ClassVolume and $SystemVolume),
 * and in shared volumes.  
 * @return pwr_tStatus 
 */
pwr_tStatus
gdh_GetPreviousObject (
  pwr_tObjid		oid,        /**< The object identity.*/ 
  pwr_tObjid		*new_oid    /**< Receives the identity of the previos object.*/
)
{
  pwr_tStatus		sts = GDH__SUCCESS;
  gdb_sObject		*op;

  if (new_oid == NULL) return GDH__BADARG;

  gdh_ScopeLock {

    op = mvol_ClassList(&sts, pwr_cNClassId, oid, mvol_eList_prev);
    if (op != NULL) {
      *new_oid = op->g.oid;
    }

  } gdh_ScopeUnlock;

  return sts;
}

/** 
 * Map the object and node database and start communications.
 *   
 * If the GDH user wants to make use of PAMS, then he should
 * call PAMS_DCL_PROCESS before calling gdh_Init, and supply
 * the actual PAMS process number as parameter to gdh_Init.
 * If the user does NOT use PAMS, a zero process_num must be
 * passed as argument. 
 */

pwr_tStatus
gdh_Init (
  char		*name /**< ZZZ */
)
{
  pwr_tStatus		sts = GDH__SUCCESS;
               
  /* Map the database.  */

  gdbroot = gdb_MapDb(&sts, NULL, name);

  return sts;
}

/** 
 * @brief Fetch the meta data of an attribute, given its attrref or its name.
 *
 * If meta data is wanted for a class use the class parameter.  
 * The output arguments can be NULL if they are not wanted.  
 * @return pwr_tStatus
 */
pwr_tStatus
gdh_MDAttribute (
  pwr_tClassId		cid,		/**< Class. Use pwr_cNClassId if meta
					    data for an object and not for
					    a class is wanted.  */
  pwr_sAttrRef		*arp,		/**< Use NULL if attrname should be used
					    instead of the attribute reference.  */
  char			*aname,		/**< String containing
					    object.attribute or
					    object.attribute[index]
					    If class data then name of attribute
					    must start with a dot.  */
  pwr_tTypeId		*tid,		/**< Address of an type id where the
					    attribute type will be stored
					    or NULL if not wanted  */
  unsigned int		*size,		/**< Address of an integer where the
					    attribute size will be stored
					    or NULL if not wanted.  */
  unsigned int		*offs,		/**< Address of an integer where the
					    attribute offset will be stored
					    or NULL if not wanted.  */
  unsigned int		*elem		/**< Address of an integer where the
					    number of elements will be stored
					    or NULL if not wanted.  */
)
{
  pwr_tStatus		sts = GDH__SUCCESS;
  cdh_sParseName	parseName;
  cdh_sParseName	*pn;
  mvol_sAttribute	attribute;
  mvol_sAttribute	*ap = NULL;
  
  if (arp == NULL) {
    pn = cdh_ParseName(&sts, &parseName, pwr_cNObjid, aname, 0);
    if (pn == NULL) return sts;
  }
  
  memset(&attribute, 0, sizeof(attribute));

  gdh_ScopeLock {

    if (arp != NULL) {
      ap = vol_ArefToAttribute(&sts, &attribute, arp, gdb_mLo_global, vol_mTrans_all);
    } else if (cid != pwr_cNClassId) {
      ap = mvol_AnameToAttribute(&sts, ap, cid, pn);
    } else {
      ap = vol_NameToAttribute(&sts, ap, pn, gdb_mLo_global, vol_mTrans_all);
    }

    if (ap != NULL) touchObject(ap->op);

  } gdh_ScopeUnlock;

  if (ap != NULL) {
    if (size != NULL) *size = ap->size;
    if (offs != NULL) *offs = ap->offs;
    if (tid  != NULL) *tid  = ap->tid;
    if (elem != NULL) *elem = ap->elem;
  }

  return sts;
}

/**
 * @brief  Move a local object.
 *
 * This call is possible only if the object is
 * in a local volume, i.e a volume owned by the local node.
 * The volume must be of class $DynamicVolume or $SystemVolume.
 * All reachable nodes, who have mounted the volume in question,
 * are notified about the move of this object. 
 * @return pwr_tStatus
 */
pwr_tStatus
gdh_MoveObject (
  pwr_tObjid		oid, /**< The object that should be moved. */
  pwr_tObjid		poid /**< The object that should become the newe parent. */
)
{
  pwr_tStatus		sts = GDH__SUCCESS;
  
  if (cdh_ObjidIsNull(oid))	    return GDH__NOSUCHOBJ;
  if (oid.oix == pwr_cNObjectIx)    return GDH__NOTMOVABLE;
  if (poid.vid == pwr_cNVolumeId)   poid.vid = oid.vid;
  if (oid.vid != poid.vid)	    return GDH__SAMEVOLUME;
  if (oid.oix == poid.oix)	    return GDH__CHILDSELF;

  gdh_ScopeLock {

    dvol_MoveObject(&sts, oid, poid);

  } gdh_ScopeUnlock;

  return sts;
}

/** 
 * @brief Get the object identity of an object with name 'name'.  
 * @return pwr_tStatus
 */
pwr_tStatus
gdh_NameToObjid (
  char			*name, /**< The object name. */
  pwr_tObjid		*oid   /**< receives the identity of the object. */
)
{
  pwr_tStatus		sts = GDH__SUCCESS;
  cdh_sParseName	parseName;
  cdh_sParseName	*pn;
  gdb_sObject		*op = NULL;

  if (name == NULL) return GDH__BADARG;
  if (oid  == NULL) return GDH__BADARG;

  pn = cdh_ParseName(&sts, &parseName, pwr_cNObjid, name, 0);
  if (pn == NULL) return sts;

  gdh_ScopeLock {

    op = vol_NameToObject(&sts, pn, gdb_mLo_global, vol_mTrans_all);
    if (op != NULL) {
      touchObject(op);
      *oid = op->g.oid;
    }

  } gdh_ScopeUnlock;

  return sts;
}

/** 
 * @brief Get the address of the data of an object,
 * given its name.
 *
 * Only local objects are handled.
 * @return pwr_tStatus  
 */
pwr_tStatus
gdh_NameToPointer (
  char			*name, /**< The name of the object. */ 
  void			**p    /**< Receives a pointer to the object. */
)
{
  pwr_tStatus		sts = GDH__SUCCESS;
  cdh_sParseName	parseName;
  cdh_sParseName	*pn;
  gdb_sObject		*op = NULL;

  if (name == NULL) return GDH__BADARG;
  if (p    == NULL) return GDH__BADARG;

  pn = cdh_ParseName(&sts, &parseName, pwr_cNObjid, name, 0);
  if (pn == NULL) return sts;

  gdh_ScopeLock {

    op = vol_NameToObject(&sts, pn, gdb_mLo_global, vol_mTrans_all);
    if (op != NULL)
      *p = vol_ObjectToAddress(&sts, op);

  } gdh_ScopeUnlock;

  return sts;
}

/** 
 *@brief Check if the Nethandler has started.  
 *@return pwr_tStatus
 */

pwr_tStatus
gdh_NethandlerRunning (
)
{
  pwr_tStatus		sts = GDH__SUCCESS;

  gdh_ScopeLock {

    if (gdbroot->db->nethandler.qix != 0)
      sts = GDH__SUCCESS;
    else
      sts = GDH__NONETHANDLER;

  } gdh_ScopeUnlock;

  return sts;
}

/** 
 * @brief Converts from name string to attribute reference. 
 * @return pwr_tStatus
 */
pwr_tStatus
gdh_NameToAttrref (
  pwr_tObjid		poid,   /**< This object is added to the Name argument to
                                     form the complete name of the object/attribute.
				     It should be specified as pwr_cNobjid if it is not used.*/
  char			*name,  /**< The argument is added to the parent to form the
                                     complete item name. If the parent argument is supplied as 
                                     pwr_cNobjid, this argument is considered to decsribe the 
                                     full name. */ 
  pwr_sAttrRef		*arp    /**< The resulting attribute reference decsriptor. */
)
{
  pwr_tStatus		sts = GDH__SUCCESS;
  cdh_sParseName	parseName;
  cdh_sParseName	*pn;
  mvol_sAttribute	attribute;
  mvol_sAttribute	*ap;
 
  if (arp == NULL) return GDH__BADARG;

  pn = cdh_ParseName(&sts, &parseName, poid, name, 0);
  if (pn == NULL) return sts;

  memset(&attribute, 0, sizeof(attribute));

  gdh_ScopeLock {
  
    ap = vol_NameToAttribute(&sts, &attribute, pn, gdb_mLo_global, vol_mTrans_all);
    if (ap == NULL) break;

    touchObject(ap->op);
    mvol_AttributeToAref(&sts, ap, arp);

  } gdh_ScopeUnlock;

  return sts;
}

/** 
 * @brief Get the name of an object identified by its object identity.
 * 
 * The caller is responsible for supplying a sufficient buffer. 
 * @return pwr_tStatus
 */ 
pwr_tStatus
gdh_ObjidToName (
  pwr_tObjid		oid,		/**<  Object id of an object.  */
  char			*namebuf,	/**<  A buffer where the name can be put.  */
  pwr_tUInt32		size,		/**<  Size of namebuf.  */
  pwr_tBitMask		nametype        /**< ZZZ */
)
{
  pwr_tStatus		sts = GDH__SUCCESS;
  char			name[512];
  char			*s = NULL;
  pwr_tInt32		len;
  cdh_mName		lnametype;
  gdb_sObject		*op;

  if (namebuf == NULL) return GDH__BADARG;

  if (nametype == cdh_mNName)
    lnametype.m = cdh_mName_pathStrict;
  else
    lnametype.m = nametype;

  if (cdh_ObjidIsNull(oid))
    return GDH__NOSUCHOBJ;

  gdh_ScopeLock {

    op = vol_OidToObject(&sts, oid, gdb_mLo_global, vol_mTrans_none, cvol_eHint_none);
    if (op == NULL)
      break;

    touchObject(op);
    s = vol_ObjectToName(&sts, op, lnametype.m, name);

  } gdh_ScopeUnlock;

  if (op == NULL && lnametype.b.fallback != cdh_mName_eFallback_strict) {
    sts = GDH__SUCCESS;
    s = cdh_ObjidToString(name, oid, 1);
  }

  if (s != NULL) {
    len = strlen(s);
    if (len >= size)
      sts = GDH__NAMEBUF;
    strncpy(namebuf, s, len + 1);
  }

  return sts;
}

/** 
 * @brief Gets the address of the data of an object, given its
 * object identity.
 *  
 * Nota bene ! Only local objects can be referenced.  
 *
 * @return pwr_tStatus
 */
pwr_tStatus
gdh_ObjidToPointer (
  pwr_tObjid		oid,   /**< The object identity. */
  void			**p    /**< Reveives a pointer to the object. */
)
{
  pwr_tStatus		sts = GDH__SUCCESS;
  gdb_sObject		*op;

  if (p == NULL) return GDH__BADARG;

  gdh_ScopeLock {

    op = vol_OidToObject(&sts, oid, gdb_mLo_native, vol_mTrans_all, cvol_eHint_none);
    if (op != NULL) 
      *p = vol_ObjectToAddress(&sts, op);

  } gdh_ScopeUnlock;

  return sts;
}

/** 
 * @brief Gets the address of the data of an attribute reference.
 *  
 * Nota bene ! Only local objects can be referenced.  
 *
 * @return pwr_tStatus
 */
pwr_tStatus
gdh_AttrRefToPointer (
  pwr_sAttrRef		*arp,   /**< Attribute reference. */
  void			**p    /**< Reveives a pointer to the object. */
)
{
  pwr_tStatus		sts = GDH__SUCCESS;
  gdb_sObject		*op;

  if (p == NULL) return GDH__BADARG;

  gdh_ScopeLock {

    op = vol_OidToObject(&sts, arp->Objid, gdb_mLo_native, vol_mTrans_all, cvol_eHint_none);
    if (op != NULL) {
      *p = vol_ObjectToAddress(&sts, op);
      *p = (char *)*p + arp->Offset;
    }
  } gdh_ScopeUnlock;

  return sts;
}

/** 
 * @brief Rename a local object.
 *
 * This call is possible only if the object is
 * in a local volume, i.e a volume owned by the local node.
 * The volume must be of class $DynamicVolume or $SystemVolume.
 *  All reachable nodes, who have mounted the volume in question,
 * are notified about the renaming of this object.  
 * @return pwr_tStatus
 */

pwr_tStatus
gdh_RenameObject (
  pwr_tObjid		oid,    /**< The object identity that should be renamed.*/
  char			*name   /**< The new name. */
)
{
  pwr_tStatus		sts = GDH__SUCCESS;
  cdh_sParseName	parseName;
  cdh_sParseName	*pn;

  if (name == NULL) return GDH__BADARG;

  pn = cdh_ParseName(&sts, &parseName, pwr_cNObjid, name, 0);
  if (pn == NULL || pn->nObject != 1 || pn->flags.b.idString)
    return GDH__BADNAME;

  gdh_ScopeLock {

    dvol_RenameObject(&sts, oid, pn);

  } gdh_ScopeUnlock;

  return sts;
}

/** 
 * @brief Change the value of an attribute or the whole body of an object.
 * 
 * The routine stalls until the operation is performed.
 * The caller is responsible for the information in the buffer.
 * If the buffer is shorter then what is required,
 * then a partial update takes place.  
 * @return pwr_tStatus 
 */

pwr_tStatus
gdh_SetObjectInfo (
  char			*name,      /**< The name of the object or object attribute.*/
  void			*bufp,      /**< Pointer to the data. */
  unsigned int		bufsize     /**< The size in bytes of the data buffer. */
)
{
  pwr_tStatus		sts = GDH__SUCCESS;
  cdh_sParseName	parseName;
  cdh_sParseName	*pn;
  mvol_sAttribute	attribute;
  mvol_sAttribute	*ap;
  gdb_sVolume		*vp;
  gdb_sNode		*np = NULL;
  pwr_sAttrRef		aref;
  pwr_sAttrRef		*arp;
  void			*p;
  pwr_sAttrRef		raref;
  pwr_sAttrRef		*rarp = NULL;
  gdb_sCclass		*ccp;
  gdb_sCclass		*ccpLocked = NULL;
  pwr_tBoolean		equal;
  pwr_tBoolean		fetched;
  pwr_tUInt32		ridx = ULONG_MAX;

  if (name == NULL) return GDH__BADARG;
  if (bufp == NULL) return GDH__BADARG;

  pn = cdh_ParseName(&sts, &parseName, pwr_cNObjid, name, 0);
  if (pn == NULL) return sts;

  memset(&attribute, 0, sizeof(attribute));

  gdh_Lock;
 
  do {

    ap = vol_NameToAttribute(&sts, &attribute, pn, gdb_mLo_global, vol_mTrans_all);
    if (ap == NULL || ap->op == NULL) break;
    
    touchObject(ap->op);
    p = vol_AttributeToAddress(&sts, ap);
    if (p != NULL) {
      memcpy(p, bufp, MIN(ap->size, bufsize));
      break;
    } else if ((ap->op->l.flags.m & gdb_mLo_native) != 0) {
      break;
    }
    
    /* Try remote.  */

    memset(&aref, 0, sizeof(aref));
    arp = mvol_AttributeToAref(&sts, ap, &aref);
    if (arp == NULL) break;

    vp = pool_Address(NULL, gdbroot->pool, ap->op->l.vr);
    np = pool_Address(NULL, gdbroot->pool, vp->l.nr);
    if (np == NULL)
      break;

    ccp = NULL;
    /* Get cached class if needed */
    if (!ap->op->u.c.flags.b.classChecked || !ap->op->u.c.flags.b.classEqual) {
      ccp = cmvolc_GetCachedClass(&sts, np, vp, ap, &equal, &fetched);
      if (EVEN(sts)) {
        np = NULL;
        break;
      }
      if (equal)
        ccp = NULL;
        
      if (ccpLocked == NULL && ccp != NULL && !equal) {
        cmvolc_LockClass(NULL, ccp);
        ccpLocked = ccp;
      }
          

      /* If gdb has been unlocked, refresh pointers */
      /** @todo Check if we can do it more efficient, eg. vol_ArefToAttribute */
      if (fetched) {
        memset(&attribute, 0, sizeof(attribute));
        np = NULL;
        continue;
      }

      if (equal)
        break;

      rarp = ndc_NarefToRaref(&sts, ap, arp, ccp, &ridx, &raref, &equal);        
      
    }
    break;        
  } while(1);

  gdh_Unlock;

  if (np != NULL && ODD(sts)) {
    if (equal)
      cvolc_SetObjectInfo(&sts, np, arp, NULL, NULL, ULONG_MAX, ap, bufp, bufsize);
    else
      cvolc_SetObjectInfo(&sts, np, arp, ccp, rarp, ridx, ap, bufp, bufsize);
  }
  

  if (ccpLocked) {
    gdb_Lock;
    cmvolc_UnlockClass(NULL, ccpLocked);
    gdb_Unlock;
  }

  return sts;
}

/**
 * @brief Change the data of an object or an attribute.
 * 
 * The routine stalls until the operation is performed. The caller
 * is responsible for the information in the buffer. If the buffer
 * is shorter then what is required, then a partial update takes place.  
 * @return pwr_tStatus
 */

pwr_tStatus
gdh_SetObjectInfoAttrref (
  pwr_sAttrRef		*arp,    /**< Supplies the attribute reference decriptor that defines
                                      an object or an object and attribute. */
  void			*bufp,   /**< Pointer to the data. */
  unsigned int		bufsize  /**< The size in bytes of the data buffer. */
)
{
  pwr_tStatus		sts = GDH__SUCCESS;
  mvol_sAttribute	attribute;
  mvol_sAttribute	*ap;
  gdb_sVolume		*vp;
  gdb_sNode		*np = NULL;
  void			*p;
  gdb_sCclass		*ccp;
  gdb_sCclass		*ccpLocked = NULL;
  pwr_sAttrRef		raref;
  pwr_sAttrRef		*rarp = NULL;
  pwr_tBoolean		equal;
  pwr_tBoolean		fetched;
  pwr_tUInt32		ridx = ULONG_MAX;


  if (arp  == NULL) return GDH__BADARG;
  if (bufp == NULL) return GDH__BADARG;

  memset(&attribute, 0, sizeof(attribute));

  gdh_Lock; 
  do {

    ap = vol_ArefToAttribute(&sts, &attribute, arp, gdb_mLo_global, vol_mTrans_all);
    if (ap == NULL || ap->op == NULL) break;

    touchObject(ap->op);
    p = vol_AttributeToAddress(&sts, ap);
    if (p != NULL) {
      memcpy(p, bufp, MIN(ap->size, bufsize));
      break;
    } else if ((ap->op->l.flags.m & gdb_mLo_native) != 0) {
      break;
    }

    /* Try remote.  */

    vp = pool_Address(NULL, gdbroot->pool, ap->op->l.vr);
    np = pool_Address(NULL, gdbroot->pool, vp->l.nr);

    if (np == NULL)
      break;

    ccp = NULL;
    /* Get cached class if needed */
    if (!ap->op->u.c.flags.b.classChecked || !ap->op->u.c.flags.b.classEqual) {
      ccp = cmvolc_GetCachedClass(&sts, np, vp, ap, &equal, &fetched);
      if (EVEN(sts)) {
        np = NULL;
        break;
      }
      if (equal)
        ccp = NULL;
        
      if (ccpLocked == NULL && ccp != NULL && !equal) {
        cmvolc_LockClass(NULL, ccp);
        ccpLocked = ccp;
      }

      /* If gdb has been unlocked, refresh pointers */
      if (fetched) {
        memset(&attribute, 0, sizeof(attribute));
        np = NULL;
        continue;
      }

      if (equal)
        break;
        
      rarp = ndc_NarefToRaref(&sts, ap, arp, ccp, &ridx, &raref, &equal);
    }

    break;

  } while (1);
  
  gdh_Unlock;

  if (np != NULL && ODD(sts)) {
    if (equal)
      cvolc_SetObjectInfo(&sts, np, arp, NULL, NULL, ULONG_MAX, ap, bufp, bufsize);
    else
      cvolc_SetObjectInfo(&sts, np, arp, ccp, rarp, ridx, ap, bufp, bufsize);
  }
  

  if (ccpLocked) {
    gdb_Lock;
    cmvolc_UnlockClass(NULL, ccpLocked);
    gdb_Unlock;
  }

  return sts;
}

/**
 * @brief Sets up a list of subscriptions to an object/attribute.
 *
 * Much more efficient way to establish a number of subscriptions than 
 * calling gdh_SubRefObjectInfoAttrref or gdh_SubRefObjectInfoName 
 * @return pwr_tStatus
 */

pwr_tStatus
gdh_SubRefObjectInfoList (
  unsigned int		nentries,	/**< Number of entries in the object[],
					    attrref[] and subid[] arrays.  */

  void			*object[],	/**< The address of an array with
					    either the address of a name string
					    or the address of an pwr_sAttrRef
					    identifying the objects or parameters
					    to establish subscriptions for.  */

  pwr_tBoolean		is_aref [],	/**< True if the corresponding object[]
					    entry is an attrref. Otherwise it
					    is regarded as a name string.  */

  pwr_tSubid		subid[]		/**< Returned handles with index matching
					    the object argument. The array must be
					    declared by the caller.  */
)
{
  pwr_tStatus		sts = GDH__SUCCESS;
  sub_sClient		*cp;
  pool_sQlink		*lh;
  pwr_tUInt32		i;
  char			*s;
  pwr_sAttrRef		*arp;

  gdh_ScopeLock {

    /* Allocate a temporary list header */

    lh = pool_Qalloc(NULL, gdbroot->pool);

    for (i = 0; i < nentries; i++) {

      /* Build subcli structure */

      if (is_aref[i]) {
	s = NULL;
	arp = object[i];
      } else {
	s = object[i];
	arp = NULL;
      }

      cp = subc_Create(s, arp, lh);

      subid[i] = cp->sid;
    }

    /* Try to get it running */

    subc_ActivateList(lh, pwr_cNObjid);

    /* Get rid of temporary root */

    pool_Free(NULL, gdbroot->pool, lh);

  } gdh_ScopeUnlock;

  return sts;
}

/** 
 * @brief This routine returns a subid to the information that is
 * denoted by 'aref'. The name can be either an object or an 
 * object parameter.
 *
 * Since this routine operates on a single data item, it is
 * pretty slow. For faster setup of many subscriptions in one
 * call, use gdh_SubRefObjectInfoList.  
 * @return pwr_tStatus
 */

pwr_tStatus
gdh_SubRefObjectInfoAttrref (
  pwr_sAttrRef		*aref,		/**< Reference to an object.attribute.  */
  pwr_tSubid		*sid		/**< Returned handle.  */
)
{
  pwr_tStatus		sts = GDH__SUCCESS;
  pwr_tBoolean		true_flag = TRUE;

  if (aref == NULL) return GDH__BADARG;
  if (sid  == NULL) return GDH__BADARG;

  sts = gdh_SubRefObjectInfoList(1, (void *)&aref, &true_flag, sid);

  return sts;
}

/**
 * @brief Get the address of the data that is denoted by 'name'.
 *
 * The name can be either a whole object or an object attribute.
 *
 * Since this routine operates on a single data item, it is
 * pretty slow. For faster setup of many subscriptions in one
 * call, use gdh_SubRefObjectInfoList.  
*/
pwr_tStatus
gdh_SubRefObjectInfoName (
  char			*name,		/**< Name of an object or object.attribute. */
  pwr_tSubid		*sid		/**< Returned handle. */
)
{
  pwr_tStatus		sts = GDH__SUCCESS;
  pwr_tBoolean		false_flag = FALSE;

  if (name == NULL) return GDH__BADARG;
  if (sid  == NULL) return GDH__BADARG;

  sts = gdh_SubRefObjectInfoList(1, (void *)&name, &false_flag, sid);

  return sts;
}

/**
 * @brief Terminates subscriptions. All invalid pwr_tSubids remain in the
 * list. The valid pwr_tSubids are zeroed out!  
 *
 * @return pwr_tStatus
 */
pwr_tStatus
gdh_SubUnrefObjectInfoList (
  unsigned int		nentries,	/**< # of pwr_tSubids in the array.  */
  pwr_tSubid		*sid		/**< Array of subscriptions to terminate.  */
)
{
  pwr_tStatus		retsts = GDH__SUCCESS;
  pwr_tStatus		sts = GDH__SUCCESS;
  sub_sClient		*cp;
  pool_sQlink		*lh;
  pwr_tUInt32		i;  
  cdh_uRefId		rid;

  if (sid == NULL) return GDH__BADARG;

  gdh_ScopeLock {

    /* Allocate a temporary root */

    lh = pool_Qalloc(NULL, gdbroot->pool);

    for (i=0; i < nentries; i++) {

      rid.pwr = sid[i];

      if (rid.r.vid_3 != cdh_eVid3_subid)	/* Verify Subid.  */
	sts = GDH__NOSUBCLI;
      else
	cp = hash_Search(&sts, gdbroot->subc_ht, &sid[i]);

      if (EVEN (sts)) {
	retsts = (sts & ~STS_M_SEVERITY) | STS_K_INFO;
      } else {

	/* Move the subscription client to the temporary list.  */

	pool_Qremove(NULL, gdbroot->pool, &cp->subc_ll);
	pool_QinsertPred(NULL, gdbroot->pool, &cp->subc_ll, lh);

	/* Tell the caller this was a valid subid by zeroing it
	   leaving the invalid ones in the list...  */

	sid[i] = pwr_cNSubid;
      }
    }

    /* Cancel all clients in the temporary list.  */

    subc_CancelList(lh);

    /* Get rid of temporary root.  */

    pool_Free(NULL, gdbroot->pool, lh);

  } gdh_ScopeUnlock;

  return retsts;
}

/**
 *@brief Terminates subscription of an object or an attribute.  
 * 
 *@return pwr_tStatus 
 */

pwr_tStatus
gdh_SubUnrefObjectInfo (
  pwr_tSubid		sid		/**< Subscription to terminate.  */
)
{
  pwr_tStatus		sts = GDH__SUCCESS;
  pwr_tSubid		sidlst = sid;

  sts = gdh_SubUnrefObjectInfoList (1, &sidlst);
  if (cdh_SubidIsNotNull(sidlst))
    sts = (sts & ~STS_M_SEVERITY) | STS_K_ERROR;

  return sts;
}

/** 
 * @brief Terminates all subscriptions set up by this user (job/process).  
 *
 */

void
gdh_SubUnrefObjectInfoAll ()
{

  gdh_ScopeLock {

    subc_CancelUser(gdbroot->my_pid);

  } gdh_ScopeUnlock;

}

/** 
 * @brief Fetch the data associated with a subscription.  
 */
pwr_tStatus
gdh_SubData (
  pwr_tSubid		sid,		/**<  Subscription to fetch data from.  */
  void			*bp,		/**<  User supplied buffer where data is put.  */
  unsigned int		bsize		/**<  Size in bytes of user buffer.  */
)
{
  pwr_tStatus		sts = GDH__SUCCESS;
  sub_sClient		*cp;
  const void		*p;
  net_sSubData		*dp;
  mvol_sAttribute	attribute;
  mvol_sAttribute	*ap;

  if (bp == NULL) return GDH__BADARG;
  memset(&attribute, 0, sizeof(attribute));

  gdh_ScopeLock {

    cp = hash_Search(&sts, gdbroot->subc_ht, &sid);
    if (cp == NULL) break;

    if (EVEN(cp->sts)) {
      sts = cp->sts;
      break;
    }

    if (
      (cp->nid == gdbroot->db->nid) &&
      (cp->userdata == pool_cNRef)
    ) {
      /*  Handle local object references without user buffers.  */

      ap = vol_ArefToAttribute(&sts, &attribute, &cp->aref, gdb_mLo_global, vol_mTrans_all);
      if (ap ==NULL) break;

      p = vol_AttributeToAddress(&sts, ap);
    } else {
      /*  Handle all other object references.  */

      if (cp->old) {
	sts = GDH__SUBOLD;
      } else {
	dp = pool_Address(NULL, gdbroot->pool, cp->subdata);
	if (dp == NULL) errh_Bugcheck(GDH__WEIRD, "gdh_SubData");
	p = &dp->data[0];
      }
    }

    /*  If all is fine, copy data to user buffer.  */

    if (ODD(sts)) {
      if (cp->cclass != pool_cNRef) {
        gdb_sCclass 		*ccp;
        ndc_sRemoteToNative	*tbl;

        ccp = pool_Address(NULL, gdbroot->pool, cp->cclass);
        if (ccp == NULL) errh_Bugcheck(GDH__WEIRD, "gdh_SubData, get cached class address");
        
        if (!ccp->flags.b.rnConv) errh_Bugcheck(GDH__WEIRD, "gdh_SubData, no conversion table");

        tbl = pool_Address(NULL, gdbroot->pool, ccp->rnConv);
        if (tbl == NULL)errh_Bugcheck(GDH__WEIRD, "gdh_SubData, get cached class address");

        ndc_ConvertRemoteToNativeTable(&sts, ccp, tbl, &cp->raref, &cp->aref, bp, p, MIN(bsize, cp->aref.Size));

      } else    
        memcpy(bp, p, MIN(bsize, cp->aref.Size));
    }

  } gdh_ScopeUnlock;

  return sts;
}

/** 
 * @brief Fetch the data size associated with a subscription.  
 */
pwr_tStatus
gdh_SubSize (
  pwr_tSubid		sid,		/**<  Subscription referenced.  */
  unsigned int		*size		/**<  Subscription size in bytes.  */
)
{
  pwr_tStatus		sts = GDH__SUCCESS;
  sub_sClient		*cp;

  if (size == NULL) return GDH__BADARG;

  gdh_ScopeLock {

    cp = hash_Search(&sts, gdbroot->subc_ht, &sid);
    if (cp == NULL) break;

    if (EVEN(cp->sts)) {
      sts = cp->sts;
    } else {
      *size = cp->aref.Size;
    }

  } gdh_ScopeUnlock;

  return sts;
}

/**
 *@brief Associate a buffer with the subscription.
 * 
 * If a buffer already is associated, that buffer
 * will be returned, if compatible in size.
 * Otherwise an error will be returned.  
 * @return pwr_tStatus
 */

pwr_tStatus
gdh_SubAssociateBuffer (
  pwr_tSubid		sid,		/**< Subscription referenced.  */
  void			**buffer,	/**< Address of buffer that gets allocated
					     for the caller.  */
  unsigned int		buffersize	/**< Requested size in bytes of user buffer.  */
)
{
  pwr_tStatus		sts = GDH__SUCCESS;
  sub_sClient		*cp;

  if (buffer == NULL) return GDH__BADARG;

  gdh_ScopeLock {

    cp = hash_Search(&sts, gdbroot->subc_ht, &sid);
    if (cp ==  NULL) break;

    if (cp->userdata == pool_cNRef) {
      cp->userdata = pool_RefAlloc(NULL, gdbroot->rtdb, buffersize);
      cp->usersize = buffersize;
      *buffer = pool_Address(NULL, gdbroot->rtdb, cp->userdata);
    } else {
      if (cp->usersize >= buffersize) {	/** @todo !!! Try to understand this code!, LW?  
                                           I think it shall be usersize instead of userdata, ML.
                                           Please verify.*/ 
	*buffer = pool_Address(NULL, gdbroot->rtdb, cp->userdata);
      } else {
	sts = GDH__SUBALRBUF;
      }
    } /* Previous buffer */

  } gdh_ScopeUnlock;

  return sts;

}

/** 
 * @brief Sets default subscription timers for future subscription
 * requests. 
 *
 * Supply negative values for no change.
 * A value of zero resets the default values.  
 */

pwr_tStatus
gdh_SetSubscriptionDefaults (
  pwr_tInt32		dt,		/**<  Default update time for subscription
					    requests to come, in ms.  */
  pwr_tInt32		tmo		/**<  Default timeout (i.e. time until
					    data gets flagged as old), in ms.  */
)
{

  subc_SetDefaults(dt, tmo);
  return GDH__SUCCESS;
}

/**
 * @brief  This routine returns addresses to the information that is
 * denoted by "objref[].fullname". 
 *
 * The name can be either an
 * object or an object parameter. Optionally, the subid array
 * can be supplied. If there is such an argument, it is assumed
 * to have n elements. This can be used to retrieve further info
 * about a particular subscription.
 * If a NULL address is received in an objref[].adrs,
 * that particular subscription never was requested due
 * to some error.
 *
 * This routine remains only because of compatibility with
 * applications that uses the GDH V1 interface. The logic is
 * emulated using the new, GDH V2 interface. The only internal
 * knowledge is about Subids, if the high byte is 0 then it
 * is a Dlid, otherwise a Subid!  
 */

pwr_tStatus
gdh_RefObjectInfoList (
  unsigned int		n,		/**<  # of entries in objref and subid.  */
  gdh_sObjRef		*objref,	/**<  Input:
					      Name list: objects or object.attributes
					      Output:
					      Address list.  */
  pwr_tSubid		*sid		/**<  Subids corresponding to objrefs.
					      If supplied as NULL, it is ignored.  */
)
{
  pwr_tStatus		sts = GDH__SUCCESS;
  pwr_tUInt32		i;
  OBJREF_STRUCT		*objrefp;
  pwr_tSubid		*subidarr;
  pwr_tUInt32		*xrefarr;
  pwr_sAttrRef		aref;
  char			**object;
  char			**objp;
  pwr_tUInt32		nsub;
  pwr_tBoolean		dl;
  pwr_tDlid		dlid;
  pwr_tBoolean		*isattrref;

  /* Allocate space for the object array and the subid array
     needed by SubRefObjectInfoList. If we assume that
     all entries in objref will be subscriptions the logic becomes simple!  */

  object    = calloc(n, sizeof(*object));
  xrefarr   = calloc(n, sizeof(pwr_tUInt32));
  subidarr  = calloc(n, sizeof(pwr_tSubid));
  isattrref = calloc(n, sizeof(pwr_tBoolean));

  /* Go through the objrefs. Set up direct links if local reference,
     accumulate in object[] array for a later call to set up subscriptions
     if this is a remote object.  */

  nsub = 0;	/* Keep track of subscription count */

  objrefp = &objref[0];
  objp = &object[0];
  for (i=0; i < n; i++, objrefp++) {
    sts = gdh_NameToAttrref(pwr_cNObjid, objrefp->fullname, &aref);
    if (EVEN (sts)) {
      dl = FALSE;
    } else {
      gdh_GetObjectLocation(aref.Objid, &dl);
    }

    /*  If dl == TRUE, then direct link, else subscribe!  */

    if (dl) {
      dlid = pwr_cNDlid;
      objrefp->adrs = NULL;
      gdh_DLRefObjectInfoAttrref(&aref, &objrefp->adrs, &dlid);
      if (sid != NULL) sid[i] = dlid;
    } else {
      *objp = (char *)&objrefp->fullname;
      isattrref[nsub] = FALSE;
      xrefarr[nsub++] = i;
      objp++;
    }
  } /* For all objref struct entries */

  /* Establish subscriptions.  */

  if (nsub > 0) {
    gdh_SubRefObjectInfoList(nsub, (void *)object, isattrref, subidarr);

    /* Associate buffers and return pwr_tSubid if requested.  */

    for (i=0; i<nsub; i++) {
      objrefp = &objref[xrefarr[i]];
      gdh_SubAssociateBuffer(subidarr[i], &objrefp->adrs, objrefp->bufsize);
      if (sid != NULL) sid[xrefarr[i]] = subidarr[i];
    }
  }

  /* Free all resources.   */

  free(subidarr);
  free(xrefarr);
  free(isattrref);
  free(object);

  return GDH__SUCCESS;
}

/**
 * @brief The routine returns an address to the data that is denoted by Name. 
 * The name can be either an object or an object attribute. 
 *
 * If the object is known and is local, this call will result in a gdh_DLRefObjectInfo,
 * otherwise it will result in a gdh_SubRefObjectInfo. 
 *
 * Some important considerations:<BR> 
 * This call is preferable to gdh_NameToPointer or gdh_ObjidToPointer since it prevents
 * the data from being removed until you dereference it. 
 * If you need data repeatedly use the gdh_RefObject routines, for a single request use
 * gdh_GetObjectInfo. 
 * You may read and write data on the local node. But if the data exists on another 
 * node writing will have no effect, since the data will be overwritten by the next 
 * subscription update. If you need to write data on another node use gdh_SetObjectInfo.
 * If you want to issue several requests at the same time use gdh_RefObjectInfoList, 
 * this is much more efficient than repeated calls to gdh_RefObjectInfo. 
 * @sa Examples
 * @return pwr_tStatus
 */
pwr_tStatus
gdh_RefObjectInfo (
  char			*name,   /**<Supplies the name of an object or object and attribute.*/
  void			**infop, /**<Receives a pointer to the requested information.  */
  pwr_tSubid		*sid,    /**<Receives the subscription identity. */
  unsigned int		size     /**<Size of the subscribed data. */
)
{
  pwr_tStatus		sts = GDH__SUCCESS;
  gdh_sObjRef		objref;


  if (infop == NULL) return GDH__BADARG;
  if (name  == NULL) return GDH__BADARG;
  if (sid   == NULL) return GDH__BADARG;
  if (strlen(name) >= sizeof(objref.fullname)) return GDH__NAMELEN;

  objref.bufsize = size;

  strncpy((char *)&objref.fullname, name, sizeof(objref.fullname));

  sts = gdh_RefObjectInfoList(1, &objref, sid);
  if (ODD (sts) && objref.adrs == NULL) return GDH__RTDBNULL;
  if (ODD (sts)) *infop = objref.adrs;

  return sts;
}

/**
 *@brief Terminates the subscription defined by the 
 * subscription identity.
 *
 * The pointer returned by for example gdh_RefObjectInfo 
 * is invalid after this call.  
 * N.B.! The user is responsible for removing all subscriptions 
 * before terminating a process, the system will not do it for you. 
 * This call will result in either a gdh_DLUnrefObjectInfo or a 
 * gdh_SubUnrefObjectInfo.
 * @sa Examples
 * @return pwr_tStatus
 */ 
pwr_tStatus
gdh_UnrefObjectInfo (
  pwr_tSubid		sid   /**<The subscription to terminate. */
)
{
  pwr_tStatus		sts = GDH__SUCCESS;
  cdh_uRefId		refid;

  refid.pwr = sid;

  if (refid.r.vid_3 == cdh_eVid3_dlid) {
    sts = gdh_DLUnrefObjectInfo(sid);
  } else if (refid.r.vid_3 == cdh_eVid3_subid) {
    sts = gdh_SubUnrefObjectInfo(sid);
  } else {
    sts = GDH__NOSUBCLI;
  }

  return sts;
}

/**
 *@brief Terminates all subscriptions set up by this user (job/process).
 * 
 * This call will result in either a gdh_DLUnrefObjectInfoAll or a 
 * gdh_SubUnrefObjectInfoAll. 
 * @return pwr_tStatus
 */
pwr_tStatus
gdh_UnrefObjectInfoAll ()
{

  gdh_DLUnrefObjectInfoAll();
  gdh_SubUnrefObjectInfoAll();

  return GDH__SUCCESS;
}

/**  
 * @brief This routine sets the alarm level for a certain object.
 *
 * The alarm level is propagated upwards to its parents as
 * long as the new alarm level is higher than the old.  
 * @return pwr_tStatus
 */

pwr_tStatus
gdh_SetAlarmLevel (
  pwr_tObjid		oid,           /**< ZZZ */
  pwr_tUInt32		alarmlevel     /**< ZZZ */
) 
{
  pwr_tStatus		sts = GDH__SUCCESS;
  gdb_sObject		*op;

  gdh_ScopeLock {

    op = vol_OidToObject(&sts, oid, gdb_mLo_owned, vol_mTrans_none, cvol_eHint_none);
    if (op == NULL) break;

    touchObject(op);

    vol_SetAlarmLevel(&sts, op, alarmlevel);

  } gdh_ScopeUnlock;

  return sts;
}

/**
 *@brief  Sets the alarm block level for a certain object.
 *
 * The block level is propagated upwards to its parents as
 * long as the new alarm level is higher than the old. 
 * @return pwr_tStatus
 */
pwr_tStatus
gdh_SetAlarmBlockLevel (
  pwr_tObjid		oid,             /**< ZZZ */
  pwr_tUInt32		blocklevel       /**< ZZZ */
)
{
  pwr_tStatus		sts = GDH__SUCCESS;
  gdb_sObject		*op;

  gdh_ScopeLock {

    op = vol_OidToObject(&sts, oid, gdb_mLo_owned, vol_mTrans_none, cvol_eHint_none);
    if (op == NULL) break;

    touchObject(op);

    vol_SetBlockLevel(&sts, op, blocklevel);

  } gdh_ScopeUnlock;

  return sts;
}

void
gdh_StoreRtdbPointer (
  pwr_tUInt32		*rp,
  void			*p
)
{
  pool_tRef		r;

  if (rp == NULL) return;

  if (p == NULL) {
    r = pool_cNRef;
  } else {  
    r = pool_Reference(NULL, gdbroot->rtdb, p);
    if (r == pool_cNRef) errh_Bugcheck(GDH__RTDBNULL, "gdh_StoreRtdbPointer");
  }
  *rp = r;
}

void *
gdh_TranslateRtdbPointer (
  pwr_tUInt32		r
)
{

  return pool_Address(NULL, gdbroot->rtdb, r);
}

pwr_tStatus
gdh_IsAlias (
  pwr_tObjid		oid,
  pwr_tBoolean		*is_alias
)
{
  pwr_tStatus		sts = GDH__SUCCESS;
  gdb_sObject		*op;

  if (is_alias == NULL) return GDH__BADARG;

  gdh_ScopeLock {

    op = vol_OidToObject(&sts, oid, gdb_mLo_global, vol_mTrans_none, cvol_eHint_none);
    if (op != NULL) {
      touchObject(op);
      *is_alias = (pwr_tBoolean) op->g.flags.b.isAliasClient;
    }

  } gdh_ScopeUnlock;

  return sts;

}

pwr_tStatus
gdh_IsMountClean (
  pwr_tObjid		oid,
  pwr_tBoolean		*is_mount_clean
)
{
  pwr_tStatus		sts = GDH__SUCCESS;
  gdb_sObject		*op;

  if (is_mount_clean == NULL) return GDH__BADARG;

  gdh_ScopeLock {

    op = vol_OidToObject(&sts, oid, gdb_mLo_global, vol_mTrans_none, cvol_eHint_none);
    if (op != NULL) {
      touchObject(op);
      if (!op->l.flags.b.isOwned) {
         *is_mount_clean = 1;
	 sts = GDH__REMOTE;
	 break;
      }
      *is_mount_clean = (pwr_tBoolean) op->u.n.flags.b.isMountClean;
    }

  } gdh_ScopeUnlock;

  return sts;

}

pwr_tStatus
gdh_SetCache (
  pwr_tUInt32		activation_level,	/*  Level when trimming starts.  */
  pwr_tUInt32		trimmed_level		/*  Trimmed cache level.  */
)
{
/*
  pwr_tUInt32 actlvl;
*/
  return GDH__OBSOLETE;

#if 0
  gdh_ScopeLock {

    actlvl = MAX (activation_level, 100);      
    gdbroot->nodedb->gdhpar.cache_activationlevel = actlvl;

    if (trimmed_level >= actlvl - 50)
      gdbroot->nodedb->gdhpar.cache_trimmedlevel = actlvl - 50;
    else
      gdbroot->nodedb->gdhpar.cache_trimmedlevel = trimmed_level;

  } gdh_ScopeUnlock;

  return GDH__SUCCESS;
#endif
  
}

pwr_tStatus
gdh_GetVolumeList( 
  pwr_tVid *vid
)
{
  pool_sQlink		*vl;
  gdb_sVolume		*vp;

  gdh_ScopeLock {
    vl = pool_Qsucc(NULL, gdbroot->pool, &gdbroot->db->vol_lh);
    if (vl == NULL || vl == &gdbroot->db->vol_lh)
      return GDH__NOSUCHVOL;

    vp = pool_Qitem(vl, gdb_sVolume, l.vol_ll);

    *vid = vp->g.vid;
  } gdh_ScopeUnlock;
  return GDH__SUCCESS;
}

pwr_tStatus
gdh_GetNextVolume( 
  pwr_tVid pvid,
  pwr_tVid *vid
)
{
  pool_sQlink		*vl;
  gdb_sVolume		*vp;
  pwr_tStatus 		sts;
  pwr_tStatus		rsts = GDH__NOSUCHVOL;

  gdh_ScopeLock {
    for ( vl = pool_Qsucc(NULL, gdbroot->pool, &gdbroot->db->vol_lh);
	  vl != &gdbroot->db->vol_lh;
	  vl = pool_Qsucc(&sts, gdbroot->pool, vl)) {

      vp = pool_Qitem(vl, gdb_sVolume, l.vol_ll);
      if ( vp->g.vid == pvid) {
	for ( vl = pool_Qsucc(&sts, gdbroot->pool, vl);
	      vl != &gdbroot->db->vol_lh;
	      vl = pool_Qsucc(&sts, gdbroot->pool, vl)) {
	  vp = pool_Qitem(vl, gdb_sVolume, l.vol_ll);
	  if ( vp->l.flags.b.isLoaded || vp->l.flags.b.isCached) {
	    *vid = vp->g.vid;
	    rsts = GDH__SUCCESS;
	    break;
	  }
	}
	if ( rsts == GDH__SUCCESS || vl == &gdbroot->db->vol_lh)
	  break;
      }
    }    
  } gdh_ScopeUnlock;

  return rsts;
}

pwr_tStatus
gdh_VolumeIdToName(
  pwr_tVid vid,
  char *name,
  int size
)
{
  gdb_sVolume *vp;
  pwr_tStatus sts;
  pwr_tStatus rsts = GDH__SUCCESS;

  gdh_ScopeLock {
    vp = hash_Search(&sts, gdbroot->vid_ht, &vid); 
    if (vp != NULL) {
      if (strlen(vp->g.name.orig) >= size)
	rsts = GDH__NAMEBUF;
      strncpy( name, vp->g.name.orig, size);
    }
    else
      rsts = GDH__NOSUCHVOL;
  } gdh_ScopeUnlock;

  return rsts;
}

pwr_tStatus
gdh_GetVolumeInfo(
  pwr_tVid vid,
  gdh_sVolumeInfo *info
)
{
  gdb_sVolume *vp;
  pwr_tStatus sts;
  pwr_tStatus rsts = GDH__SUCCESS;

  gdh_ScopeLock {
    vp = hash_Search(&sts, gdbroot->vid_ht, &vid); 
    if (vp != NULL) {
      info->isCached = vp->l.flags.b.isCached ? 1 : 0;
      info->isLoaded = vp->l.flags.b.isLoaded ? 1 : 0;
      info->isMounted = vp->l.flags.b.isMounted ? 1 : 0;
      info->time = vp->g.time;
      info->cid = vp->g.cid;
      info->nid = vp->g.nid;
      strcpy( info->name, vp->g.name.orig);
    }
    else
      rsts = GDH__NOSUCHVOL;
  } gdh_ScopeUnlock;

  return rsts;
}

pwr_tStatus
gdh_GetObjectBodyDef(
  pwr_tCid cid,
  gdh_sAttrDef **bodydef,
  int *rows
)
{
  gdb_sClass *cp;
  gdb_sObject *bop;
  gdb_sObject *aop;
  pwr_sParam *adef;
  pwr_sObjBodyDef *bdef;
  int acnt = 0;
  gdh_sAttrDef *bd;
  pwr_tStatus sts;
  pwr_tStatus rsts = GDH__SUCCESS;
  pwr_tOid noid;
  gdb_sObject *a_super[20];
  int scnt = 0;
  int i, j;
  
  gdh_ScopeLock {
    cp = hash_Search(&sts, gdbroot->cid_ht, &cid);
    if ( cp == 0) { rsts = GDH__NOSUCHCLASS; goto error_sts;}

    bop = pool_Address(&sts, gdbroot->pool, cp->bor);
    if (bop == NULL) { rsts = GDH__ATTRIBUTE; goto error_sts;}

    bdef = pool_Address(NULL, gdbroot->rtdb, bop->u.n.body);
    acnt += bdef->NumOfParams;

    /* Count attributes */
    a_super[scnt++] = vol_OidToObject(&sts, bop->g.soid, gdb_mLo_local, vol_mTrans_none, cvol_eHint_none);
    while ( a_super[scnt-1]) {
      adef = pool_Address(NULL, gdbroot->rtdb, a_super[scnt-1]->u.n.body);

      if ( strcmp( a_super[scnt-1]->g.f.name.orig, "Super") == 0 && cdh_tidIsCid(adef->TypeRef)) {
	cp = hash_Search(&sts, gdbroot->cid_ht, &adef->TypeRef);
	if ( cp == 0) { rsts = GDH__NOSUCHCLASS; goto error_sts;}
	    
	bop = pool_Address(&sts, gdbroot->pool, cp->bor);
	if (bop == NULL) { rsts = GDH__ATTRIBUTE; goto error_sts;}
	
	bdef = pool_Address(NULL, gdbroot->rtdb, bop->u.n.body);
	acnt += bdef->NumOfParams - 1;
	
	a_super[scnt++] = vol_OidToObject(&sts, bop->g.soid, gdb_mLo_local, vol_mTrans_none, cvol_eHint_none);
      }
      else
	break;
    }
	
    /* Allocate buffer */
    bd = (gdh_sAttrDef *)calloc( acnt, sizeof(gdh_sAttrDef));

    acnt = 0;
    for ( i = scnt - 1; i >= 0; i--) {
      aop = a_super[i];
      while ( aop) {
	adef = pool_Address(NULL, gdbroot->rtdb, aop->u.n.body);
	if ( !(aop == a_super[i] && strcmp( aop->g.f.name.orig, "Super") == 0)) {
	  for ( j = 0; j < i; j++)
	    strcat( bd[acnt].attrName, "Super.");
	  strcat( bd[acnt].attrName, aop->g.f.name.orig);
	  bd[acnt].attrClass = aop->g.cid;
	  bd[acnt].attr = (pwr_uParDef *)adef;
	  if ( i > 0)
	    bd[acnt].flags = gdh_mAttrDef_Super;
	  acnt++;
	}	  
	noid.vid = aop->g.oid.vid;
	noid.oix = aop->g.sib.flink;
	aop = vol_OidToObject(&sts, noid, gdb_mLo_local, vol_mTrans_none, cvol_eHint_next);
	if ( aop && aop == a_super[i])
	  break;
      }
    }

    if ( scnt > 1) {
      /* Detect shadowed attributes */
      for ( j = 0; j < acnt - 1; j++) {
	char *s1 = strrchr( bd[j].attrName, '.');
	if ( !s1)
	  s1 = bd[j].attrName;
	else
	  s1++;
	for ( i = j + 1; i < acnt; i++) {
	  char *s2 = strrchr( bd[i].attrName, '.');
	  if ( !s2)
	    s2 = bd[i].attrName;
	  else
	    s2++;
	  if ( cdh_NoCaseStrcmp( s1, s2) == 0) {
	    bd[j].flags |= gdh_mAttrDef_Shadowed;
	    break;
	  }
	}
      }
    }
    *bodydef = bd;
    *rows = acnt;

  error_sts: {
    }
  } gdh_ScopeUnlock;
  return rsts;
}

pwr_tStatus
gdh_GetTrueObjectBodyDef(
  pwr_tCid cid,
  gdh_sAttrDef **bodydef,
  int *rows
)
{
  gdb_sClass *cp;
  gdb_sObject *bop;
  gdb_sObject *aop;
  pwr_sParam *adef;
  pwr_sObjBodyDef *bdef;
  int acnt = 0;
  gdh_sAttrDef *bd;
  pwr_tStatus sts;
  pwr_tStatus rsts = GDH__SUCCESS;
  pwr_tOid noid;
  
  gdh_ScopeLock {
    cp = hash_Search(&sts, gdbroot->cid_ht, &cid);
    if ( cp == 0) { rsts = GDH__NOSUCHCLASS; goto error_sts;}

    bop = pool_Address(&sts, gdbroot->pool, cp->bor);
    if (bop == NULL) { rsts = GDH__ATTRIBUTE; goto error_sts;}

    bdef = pool_Address(NULL, gdbroot->rtdb, bop->u.n.body);
    acnt = bdef->NumOfParams;

    /* Allocate buffer */
    bd = (gdh_sAttrDef *)calloc( acnt, sizeof(gdh_sAttrDef));

    acnt = 0;
    aop = vol_OidToObject(&sts, bop->g.soid, gdb_mLo_local, vol_mTrans_none, cvol_eHint_none);
    while ( aop) {
      adef = pool_Address(NULL, gdbroot->rtdb, aop->u.n.body);
      strcpy( bd[acnt].attrName, aop->g.f.name.orig);
      bd[acnt].attrClass = aop->g.cid;
      bd[acnt].attr = (pwr_uParDef *)adef;
      acnt++;
	  
      noid.vid = aop->g.oid.vid;
      noid.oix = aop->g.sib.flink;
      aop = vol_OidToObject(&sts, noid, gdb_mLo_local, vol_mTrans_none, cvol_eHint_next);
      if ( aop->g.oid.oix == bop->g.soid.oix)
	break;
    }
    *bodydef = bd;
    *rows = acnt;

  error_sts: {
    }
  } gdh_ScopeUnlock;
  return rsts;
}

pwr_tStatus
gdh_GetAttrRefAdef(
  pwr_sAttrRef *arp,
  gdh_sAttrDef *attrdef
)
{
  pwr_sParam 		*adef;
  mvol_sAttribute	Attribute;
  mvol_sAttribute	*ap;
  pwr_tStatus 		sts;
  pwr_tStatus 		rsts = GDH__SUCCESS;
  
  gdh_ScopeLock {

    ap = vol_ArefToAttribute(&sts, &Attribute, arp, gdb_mLo_global, vol_mTrans_all);
    if (ap == NULL)
      rsts = GDH__ATTRIBUTE;
    else {
      touchObject(ap->op);

      adef = pool_Address(NULL, gdbroot->rtdb, ap->aop->u.n.body);
      strcpy( attrdef->attrName, ap->aop->g.f.name.orig);
      attrdef->attrClass = ap->aop->g.cid;
      attrdef->attr = (pwr_uParDef *)adef;
    }
  } gdh_ScopeUnlock;
  return rsts;
}

pwr_tStatus 
gdh_GetSuperClass( 
  pwr_tCid cid,
  pwr_tCid *supercid
)
{
  gdb_sClass *cp;
  pwr_tStatus sts = GDH__SUCCESS;

  gdh_ScopeLock {
    /* TODO get cashed class... */
    cp = hash_Search(&sts, gdbroot->cid_ht, &cid);
    if ( cp) {
      if ( !(cp->attr[0].flags.m & PWR_MASK_SUPERCLASS))
	sts = GDH__NOSUCHCLASS;
      else
	*supercid = cp->attr[0].tid;
    }
  } gdh_ScopeUnlock;

  return sts;
}

pwr_tStatus
gdh_GetEnumValueDef( pwr_tTid tid, gdh_sValueDef **vd, int *rows)
{
  gdb_sObject *top;
  gdb_sObject *vop;
  pwr_tOid valoid;
  int vcnt;
  pwr_tStatus sts = GDH__SUCCESS;
  pwr_sValue *valp;
  
  gdh_ScopeLock {

    top = vol_OidToObject(&sts, cdh_TypeIdToObjid( tid), gdb_mLo_local, vol_mTrans_none, 
			 cvol_eHint_none);
    if ( top == NULL) goto error_sts;
    if ( !top->g.flags.b.isParent) {
      sts = GDH__NOSUCHOBJ;
      goto error_sts;
    }

    vcnt = 0;
    valoid = top->g.soid;    
    vop = vol_OidToObject(&sts, valoid, gdb_mLo_local, vol_mTrans_none, cvol_eHint_none);
    if (vop == NULL) goto error_sts;

    while (1) {
      valoid.oix = vop->g.sib.blink;

      if ( valoid.oix == top->g.soid.oix)
	break;

      vop = vol_OidToObject(&sts, valoid, gdb_mLo_local, vol_mTrans_none, cvol_eHint_none);
      if (vop == NULL) goto error_sts;

      if ( vop->g.cid == pwr_eClass_Value)
	vcnt++;
    }
	
    /* Allocate buffer */
    *vd = (gdh_sValueDef *)calloc( vcnt, sizeof(gdh_sValueDef));
    *rows = vcnt;

    vcnt = 0;
    valoid = top->g.soid;    
    vop = vol_OidToObject(&sts, valoid, gdb_mLo_local, vol_mTrans_none, cvol_eHint_none);
    if (vop == NULL) goto error_sts;

    while (1) {
      valoid.oix = vop->g.sib.blink;

      if ( valoid.oix == top->g.soid.oix)
	break;

      vop = vol_OidToObject(&sts, valoid, gdb_mLo_local, vol_mTrans_none, cvol_eHint_none);
      if (vop == NULL) goto error_sts;

      if ( vop->g.cid == pwr_eClass_Value) {
	valp = (pwr_sValue *) vol_ObjectToAddress(&sts, vop);
	if (valp == NULL) goto error_sts;

	strcpy( (*vd)[vcnt].Name, vop->g.f.name.orig);
	(*vd)[vcnt].Value = valp;
	vcnt++;
      }
    }
  error_sts: {
    }
  } gdh_ScopeUnlock;

  return sts;
}

pwr_tStatus
gdh_GetMaskBitDef( pwr_tTid tid, gdh_sBitDef **bd, int *rows)
{
  gdb_sObject *top;
  gdb_sObject *bop;
  pwr_tOid boid;
  int bcnt;
  pwr_tStatus sts = GDH__SUCCESS;
  pwr_sBit *bp;
  
  gdh_ScopeLock {

    top = vol_OidToObject(&sts, cdh_TypeIdToObjid( tid), gdb_mLo_local, vol_mTrans_none, 
			 cvol_eHint_none);
    if ( top == NULL) goto error_sts;
    if ( !top->g.flags.b.isParent) {
      sts = GDH__NOSUCHOBJ;
      goto error_sts;
    }

    bcnt = 0;
    boid = top->g.soid;    
    bop = vol_OidToObject(&sts, boid, gdb_mLo_local, vol_mTrans_none, cvol_eHint_none);
    if (bop == NULL) goto error_sts;

    while (1) {
      boid.oix = bop->g.sib.blink;

      if ( boid.oix == top->g.soid.oix)
	break;

      bop = vol_OidToObject(&sts, boid, gdb_mLo_local, vol_mTrans_none, cvol_eHint_none);
      if (bop == NULL) goto error_sts;

      if ( bop->g.cid == pwr_eClass_Bit)
	bcnt++;
    }
	
    /* Allocate buffer */
    *bd = (gdh_sBitDef *)calloc( bcnt, sizeof(gdh_sBitDef));
    *rows = bcnt;

    bcnt = 0;
    boid = top->g.soid;    
    bop = vol_OidToObject(&sts, boid, gdb_mLo_local, vol_mTrans_none, cvol_eHint_none);
    if (bop == NULL) goto error_sts;

    while (1) {
      boid.oix = bop->g.sib.blink;

      if ( boid.oix == top->g.soid.oix)
	break;

      bop = vol_OidToObject(&sts, boid, gdb_mLo_local, vol_mTrans_none, cvol_eHint_none);
      if (bop == NULL) goto error_sts;

      if ( bop->g.cid == pwr_eClass_Bit) {
	bp = (pwr_sBit *) vol_ObjectToAddress(&sts, bop);
	if (bp == NULL) goto error_sts;

	strcpy( (*bd)[bcnt].Name, bop->g.f.name.orig);
	(*bd)[bcnt].Bit = bp;
	bcnt++;
      }
    }
  error_sts: {
    }
  } gdh_ScopeUnlock;

  return sts;
}







