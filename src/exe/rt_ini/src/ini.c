#include <stdio.h>
#include <ctype.h>

#if defined OS_LYNX
# include <sys/wait.h>
#elif defined OS_LINUX
# include <sys/wait.h>
# include <sys/types.h>
#endif

#include "pwr.h"
#include "pwr_baseclasses.h"
#include "co_dbs.h"
#include "co_tree.h"
#include "co_time.h"
#include "co_errno.h"
#include "rt_gdh.h"
#include "rt_qcom.h"
#include "ini.h"
#if defined OS_ELN
# include "ini_time.h"
#endif
#include "rt_ini_alias.h"
#include "rt_ini_msg.h"
#include "rt_lst.h"
#include "rt_errh.h"
#include "rt_syi.h"

#if defined OS_VMS
# define cPrio_neth		8
# define cPrio_qmon		9
# define cPrio_neth_acp		6
# define cPrio_io_comm		7
# define cPrio_tmon		6
# define cPrio_emon		7
# define cPrio_alimserver	6
# define cPrio_bck		5
# define cPrio_linksup		5
# define cPrio_fast		5
# define cPrio_trend		5
# define cPrio_plc_init		5
#elif defined OS_ELN
# define cPrio_neth		7
# define cPrio_qmon		6
# define cPrio_neth_acp		8
# define cPrio_io_comm		6
# define cPrio_tmon		8
# define cPrio_emon		8
# define cPrio_alimserver	8
# define cPrio_bck		10
# define cPrio_linksup		10
# define cPrio_fast		10
# define cPrio_trend		10
# define cPrio_plc_init		16
#elif defined OS_LYNX
# define cPrio_neth		18
# define cPrio_qmon		19
# define cPrio_neth_acp		17
# define cPrio_io_comm		17
# define cPrio_tmon		17
# define cPrio_emon		17
# define cPrio_alimserver	17
# define cPrio_bck		16
# define cPrio_linksup		16
# define cPrio_fast		15
# define cPrio_trend		15
# define cPrio_plc_init		5
#elif defined OS_LINUX
# define cPrio_base		0
# define cPrio_neth		(cPrio_base + 18)
# define cPrio_qmon		(cPrio_base + 19)
# define cPrio_neth_acp		(cPrio_base + 17)
# define cPrio_io_comm		(cPrio_base + 17)
# define cPrio_tmon		(cPrio_base + 17)
# define cPrio_emon		(cPrio_base + 17)
# define cPrio_alimserver	(cPrio_base + 17)
# define cPrio_bck		(cPrio_base + 16)
# define cPrio_linksup		(cPrio_base + 16)
# define cPrio_fast		(cPrio_base + 15)
# define cPrio_trend		(cPrio_base + 15)
# define cPrio_webmon		(cPrio_base + 15)
# define cPrio_elog		(cPrio_base + 15)
# define cPrio_plc_init		(cPrio_base + 5)
#endif

static pwr_tBoolean	checkSect (pwr_tStatus*, ini_sContext*, int, int);
static gdb_sObject*	oidToObject (pwr_tObjid);
static pwr_tBoolean	loadSectObject (pwr_tStatus*, ini_sContext*, ivol_sVolume*);
static pwr_tBoolean	loadSectRbody (pwr_tStatus*, ini_sContext*, ivol_sVolume*);
static pwr_tBoolean     loadSectScObject (pwr_tStatus*, ini_sContext*, ivol_sVolume*);
static pwr_tBoolean	loadSectVolume (pwr_tStatus*, ini_sContext*, ivol_sVolume*);
static pwr_tBoolean	readSectFile (pwr_tStatus*, ini_sContext*);
static pwr_tBoolean	readSectVolRef (pwr_tStatus*, ini_sContext*);
static pwr_tBoolean	readSectVolume (pwr_tStatus*, ini_sContext*, ivol_sVolume*);
static gdb_sObject*	reloadObject (pwr_tStatus*, ini_sContext*, ivol_sVolume*, dbs_sObject*);
static pwr_tBoolean	reloadSectObject (pwr_tStatus*, ini_sContext*, ivol_sVolume*);
static pwr_tBoolean	reloadSectRbody (pwr_tStatus*, ini_sContext*, ivol_sVolume*);
static pwr_tBoolean	reloadSectVolume (pwr_tStatus*, ini_sContext*, ivol_sVolume*);
static char		*strsav (char*);
static void		create_active_io ();
static void		delete_old_io ();
static void		rename_active_io (ini_sContext*);

static char *
strsav (
  char *s
)
{
  int	  len;
  char	  *t = NULL;

  if (s == NULL)
    return NULL;

  len = strlen(s);
  if (len > 0) {
    t = malloc(len + 1);
    strcpy(t, s);
  }

  return t;
}

static pwr_tBoolean
checkSect (
  pwr_tStatus	*status,
  ini_sContext	*cp,
  int		sects,
  int		version
)
{
  pwr_dStatus(sts, status, INI__SUCCESS);

  if (cp->sect.version != version) {
    *sts = INI__SECTVERSION;
    errh_LogError(&cp->log, "Section header Versions differ: %d != %d", cp->sect.version, version);
    cp->errors++;
  } else if (((1<<cp->sect.type) & sects) != 0) {
    *sts = INI__MULTSECT;
    errh_LogError(&cp->log, "Section type allready read, %d", 1<<cp->sect.type);
    cp->errors++;
  }

  return ODD(*sts);
}

static gdb_sObject*
reloadObject (
  pwr_tStatus		*status,
  ini_sContext		*cp,
  ivol_sVolume		*vp,
  dbs_sObject		*oh
)
{
  gdb_sObject		*op;
  ivol_sObject		*iop;
  gdb_mChange		flags;

  pwr_dStatus(sts, status, INI__SUCCESS);

  flags.m = gdb_mChange__;

  gdb_ScopeLock {

    op = oidToObject(oh->oid);
    if (op != NULL) {
      op->u.n.flags.b.swapDelete = 0;

      flags.b.father = op->g.f.poid.oix != oh->poid.oix;
      flags.b.name   = strcmp(op->g.f.name.orig, oh->name) != 0;
      flags.b.server = op->g.flags.m & net_mGo_isClient && cdh_ObjidIsNotEqual(op->g.soid, oh->soid);
      flags.b.classid  = op->g.cid != oh->cid;
      flags.b.size   = !op->g.flags.b.isAliasClient && op->g.size != oh->rbody.size;
      flags.b.flags  = op->u.n.lflags.m != oh->flags.m;
      flags.b.body   = time_Acomp(&op->u.n.time, &oh->rbody.time) != 0;

    } else {
      flags.b.created   = 1;
      op = ivol_LoadObject(sts, vp, oh, vol_mLink_swapLoad);
      flags.b.body = op->g.size > 0;
    }

  } gdb_ScopeUnlock;

  if (!flags.m)
    return op;

  iop = tree_Insert(sts, vp->oid_t, &oh->oid.oix);
  if (iop == NULL)
    return NULL;

  iop->flags.m = flags.m;
  iop->oh = *oh;
  iop->op = op;
  iop->cp = hash_Search(sts, gdbroot->cid_ht, &op->g.cid);
  if (iop->cp == NULL)
    errh_Bugcheck(2, "class definition missing");

  if (flags.b.created) {
    lst_InsertPred(NULL, &vp->cre_lh, &iop->obj_ll, iop);
  } else {
    if (oh->flags.b.io)
      lst_InsertPred(NULL, &vp->upd_io_lh, &iop->obj_ll, iop);
    else
      lst_InsertPred(NULL, &vp->upd_lh, &iop->obj_ll, iop);

    iop->unlink.b.goAdopt	= iop->flags.b.father;
    iop->unlink.b.loFamilyTab	= iop->flags.b.father | iop->flags.b.name;
    iop->unlink.b.loParentRef	= iop->flags.b.father;
    iop->unlink.b.noSibList	= iop->flags.b.father;
    iop->unlink.b.noCidList	= iop->flags.b.classid;
    iop->link.m = iop->unlink.m;
    if (iop->upd_body != NULL) {
      printf("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!error");
    }
    if (flags.b.body && op->g.size > 0) {
	iop->upd_body = calloc(1, op->g.size);
    }
  }
        
  return op;

}

static gdb_sObject*
oidToObject (
  pwr_tObjid	oid
)
{
  gdb_sObject *op;

  gdb_AssumeLocked;

  op = hash_Search(NULL, gdbroot->oid_ht, &oid);

  return op;
}


static pwr_tBoolean
readSectFile (
  pwr_tStatus		*status,
  ini_sContext		*cp
)
{
  char			timbuf[32];
 
  pwr_dStatus(sts, status, INI__SUCCESS);

  if (cp->dbs.file.version != dbs_cVersionFile) {
    *sts = INI__SECTVERSION;
    errh_LogError(&cp->log, "dbs_cVersionFile differ: %d != %d", 
                  cp->dbs.file.version, dbs_cVersionFile);
    cp->errors++;
  }

  time_AtoAscii(&cp->dbs.file.time, time_eFormat_DateAndTime, timbuf, sizeof(timbuf));

  errh_LogInfo(&cp->log, "Created %s", timbuf);

#if 1
  printf("Comment in file is not OK. fix when dbs_File is OK\n");
#else
  if (strlen (cp->dbs.file.comment) > 0) {
    errh_LogInfo(&cp->log, "%s", cp->dbs.file.comment);
  }
#endif
  return ODD(*sts);
}


static pwr_tBoolean
readSectVolRef (
  pwr_tStatus		*status,
  ini_sContext		*cp
)
{
  dbs_sVolRef		volRef;
  ivol_sVolume		*vp;
  int			i;
  PDR			pdrs;

  pwr_dStatus(sts, status, INI__SUCCESS);

#if 1

  if (fseek(cp->dbs.f, cp->sect.offset, SEEK_SET) != 0)
    pwr_Return(NO, sts, errno_GetStatus());


  /* Read the section */

  for (i = 0; i < cp->sect.size; i += dbs_dAlign(sizeof(volRef))) {
    if (!dbs_AlignedRead(sts, &volRef, sizeof(volRef), &cp->dbs))
      return NO;
    
    if (cp->dbs.file.format.m != cp->format.m) {      
      pdrmem_create(&pdrs, &volRef, sizeof(volRef), PDR_DECODE, 
                    cp->dbs.file.format, cp->format);

      if(!pdr_dbs_sVolRef(&pdrs, &volRef))
        pwr_Return(NO, sts, INI__XDR);
    }
    
    /* Try to find the volume in the volume list.  */

    vp = tree_Find(sts, cp->vid_t, &volRef.vid);
    if (vp == NULL) {
      /* This volume is not in the volume list.  */

      vp = tree_Insert(sts, cp->vid_t, &volRef.vid);
      lst_InsertPred(NULL, &cp->vol_lh, &vp->ll, vp);
      vp->time = volRef.time;
      strcpy(vp->name, volRef.name);
      if (volRef.cid != pwr_eClass_ClassVolume)
        vp->isVolRef = 1;
      vp->volRef = volRef;
      vp->oid_t = tree_CreateTable(sts, sizeof(pwr_tObjectIx),
                                   offsetof(ivol_sObject, oix), 
                                   sizeof(ivol_sObject), 10,
                                   tree_Comp_oix);
      lst_Init(NULL, &vp->upd_lh, NULL);
      lst_Init(NULL, &vp->upd_io_lh, NULL);
      lst_Init(NULL, &vp->cre_lh, NULL);
#if 0
//      lst_Init(NULL, &vp->del_lh, NULL);
#endif

#if 0

      np->cacheSize += VolRef.CacheMaxCount;
#endif
    } else {
      /* Do some checks.  */
      if (time_Acomp(&vp->time, &volRef.time) != 0) {
        char timbuf1[32];
        char timbuf2[32];

        time_AtoAscii(&vp->time, time_eFormat_DateAndTime, timbuf1, sizeof(timbuf1));
        time_AtoAscii(&volRef.time, time_eFormat_DateAndTime, timbuf2, sizeof(timbuf2));

	errh_LogWarning(&cp->log, "Version missmatch for volume: %s, %s != %s\n",
                        vp->volume.name, timbuf1, timbuf2);
	cp->warnings++;
      }
    }
  }
#endif
  return ODD(*sts);
}

static pwr_tBoolean
readSectVolume (
  pwr_tStatus		*status,
  ini_sContext		*cp,
  ivol_sVolume		*vp
)
{
  PDR			pdrs;

  pwr_dStatus(sts, status, INI__SUCCESS);

  if (fseek(cp->dbs.f, cp->sect.offset, SEEK_SET) != 0)
    pwr_Return(NO, sts, errno_GetStatus());

  if (!dbs_AlignedRead(sts, &vp->volume, cp->sect.size, &cp->dbs))
    return NO;

  if (cp->dbs.file.format.m != cp->format.m) {
    pdrmem_create(&pdrs, &vp->volume, sizeof(vp->volume), PDR_DECODE,
                  cp->dbs.file.format, cp->format);
    if(!pdr_dbs_sVolume(&pdrs, &vp->volume))
      pwr_Return(NO, sts, INI__XDR);
  }
  
  cp->node.cardinality	+= vp->volume.cardinality;
  cp->node.bodySize	+= vp->volume.rbodySize;

  return ODD(*sts);
}

static pwr_tBoolean
loadSectRbody (
  pwr_tStatus		*sts,
  ini_sContext		*cp,
  ivol_sVolume		*vp
)
{
  pwr_tInt32		i;
  dbs_sBody		ob;
  pwr_tAddress		body;
  pwr_tUInt32		nextpos;
  ivol_sBody		*bp = NULL;
  PDR			pdrs;

  /* Read the objdata entries one by one */

  i = cp->sect.size;	/* Remaining # of bytes in section */
  nextpos = cp->sect.offset;
  while (i > 0) {
    fseek(cp->dbs.f, nextpos, SEEK_SET);
    if (!dbs_AlignedRead(sts, &ob, sizeof(ob), &cp->dbs)) {
      errh_LogFatal(&cp->log, "loadSectRbody, fread, %m", *sts);
      return NO;
    }

    if (cp->dbs.file.format.m != cp->format.m) {      
      pdrmem_create(&pdrs, &ob, sizeof(ob), PDR_DECODE,
                    cp->dbs.file.format, cp->format);
      if(!pdr_dbs_sBody(&pdrs, &ob))
        pwr_Return(NO, sts, INI__XDR);
    }
  
    i -= dbs_dAlign(sizeof(ob)) + dbs_dAlign(ob.size);
    nextpos = ftell(cp->dbs.f) + dbs_dAlign(ob.size);

    bp = ivol_GetBody(sts, ob.oid, NULL);
    if (bp == NULL) {
      errh_LogError(&cp->log, "Cannot find body of object %s\n%m", cdh_ObjidToString(NULL, ob.oid, 0), *sts);
    } else {
      if (dbs_dAlign(bp->size) < ob.size) {
        errh_LogError(&cp->log, "Data beyond size of body, Objid %s", cdh_ObjidToString(NULL, ob.oid, 0));
      } else if (bp->size > ob.size) {
        errh_LogError(&cp->log, "Data is smaller than size of body, Objid %s", cdh_ObjidToString(NULL, ob.oid, 0));
      } else {
	body = bp->body;
	if (fread(body, bp->size, 1, cp->dbs.f) == 0)
	  pwr_Return(NO, sts, errno_GetStatus());
      }
    }
  }

  return INI__SUCCESS;
}

static pwr_tBoolean
reloadSectRbody (
  pwr_tStatus		*sts,
  ini_sContext		*cp,
  ivol_sVolume		*vp
)
{
#if 0
  *sts = INI__ERROR;
  return NO;
  
#else
  
  pwr_tInt32		i;
  dbs_sBody		ob;
  void			*body;
  pwr_tUInt32		nextpos;
  PDR			pdrs;
  ivol_sObject		*iop;

  /* Read the objdata entries one by one */

  i = cp->sect.size;	/* Remaining # of bytes in section */
  nextpos = cp->sect.offset;

  while (i > 0) {
    if (fseek(cp->dbs.f, nextpos, SEEK_SET) != 0) {
      *sts = errno_GetStatus();
      errh_LogFatal(&cp->log, "reloadSectRbody, fseek, %m", *sts);
      break;
    }
    if (!dbs_AlignedRead(sts, &ob, sizeof(ob), &cp->dbs)) {
      errh_LogFatal(&cp->log, "reloadSectRbody, fread, %m", *sts);
      break;
    }

    if (cp->dbs.file.format.m != cp->format.m) {
      pdrmem_create(&pdrs, &ob, sizeof(ob), PDR_DECODE,
                    cp->dbs.file.format, cp->format);
      if(!pdr_dbs_sBody(&pdrs, &ob))
        pwr_Return(NO, sts, INI__XDR);
    }
  
    i -= dbs_dAlign(sizeof(ob)) + dbs_dAlign(ob.size);
    nextpos = ftell(cp->dbs.f) + dbs_dAlign(ob.size);

    iop = tree_Find(sts, vp->oid_t, &ob.oid.oix);
    if (iop == NULL)
      continue;
 
    if (iop->body == NULL) {
      iop->size = iop->op->g.size;
      iop->body = pool_Address(NULL, gdbroot->rtdb, iop->op->u.n.body);
    }

    if (iop->flags.b.created && iop->flags.b.body) {
      body = iop->body;
    } else if (iop->flags.b.body) {

      if (iop->upd_body == NULL) {
	printf("iop->upd_body == NULL\n");
      }
      body = iop->upd_body;
    } else {
      continue;
    }

    if (iop->body == NULL) {
      errh_LogError(&cp->log, "Cannot find body of object %s\n%m", cdh_ObjidToString(NULL, ob.oid, 0), sts);
    } else {
      if (dbs_dAlign(iop->op->g.size) < ob.size) {
	errh_LogError(&cp->log, "Data beyond size of body, Objid %s", cdh_ObjidToString(NULL, ob.oid, 0));
      } else if (iop->op->g.size > ob.size) {
        errh_LogError(&cp->log, "Data is smaller than size of body, Objid %s", cdh_ObjidToString(NULL, ob.oid, 0));
      } else {
	if (fread(body, iop->op->g.size, 1, cp->dbs.f) == 0)
	  pwr_Return(NO, sts, errno_GetStatus());
      }
    }
  }

  return ODD(*sts);

#endif
}

static pwr_tBoolean
loadSectObject (
  pwr_tStatus		*status,
  ini_sContext		*cp,
  ivol_sVolume		*vp
)
{
  pwr_tUInt32		i;
  dbs_sObject		oh;
  gdb_sObject		*op;
  PDR			pdrs;

  pwr_dStatus(sts, status, INI__SUCCESS);

  /* Read the section header by header */

  if (fseek(cp->dbs.f, cp->sect.offset, SEEK_SET) != 0)
    pwr_Return(NO, sts, errno_GetStatus());

  for (i = 0; i < cp->sect.size; i += dbs_dAlign(sizeof(oh))) {
    if (!dbs_AlignedRead(sts, &oh, sizeof(oh), &cp->dbs)) {
      errh_LogFatal(&cp->log, "loadSectObject, fread, %m", *sts);
      break;
    }

    if (cp->dbs.file.format.m != cp->format.m) {
      pdrmem_create(&pdrs, &oh, sizeof(oh), PDR_DECODE,
                    cp->dbs.file.format, cp->format);
      if(!pdr_dbs_sObject(&pdrs, &oh))
        pwr_Return(NO, sts, INI__XDR);
    }
  
    if (oh.cid == pwr_eClass_Node && oh.oid.vid == cp->node.nid)
      cp->node.nod_oid = oh.oid;  /* This is the node object.  */

    op = ivol_LoadObject(sts, vp, &oh, vol_mLink_load);
    if (op == NULL) {
      errh_LogError(&cp->log, "Loading object %s, %s, parent %s\n%m",
	cdh_ObjidToString(NULL, oh.oid, 0), oh.name, cdh_ObjidToString(NULL, oh.poid, 0), *sts);
      return NO;
    }
  }

  return YES;
}

static pwr_tBoolean
reloadSectObject (
  pwr_tStatus		*status,
  ini_sContext		*cp,
  ivol_sVolume		*vp
)
{
  pwr_tUInt32		i;
  dbs_sObject		oh;
  gdb_sObject		*op;
  PDR			pdrs;

  pwr_dStatus(sts, status, INI__SUCCESS);

  ivol_InitiateVolumeUpdate(sts, vp);

  /* Read the section header by header */

  if (fseek(cp->dbs.f, cp->sect.offset, SEEK_SET) != 0)
    pwr_Return(NO, sts, errno_GetStatus());

  for (i = 0; i < cp->sect.size; i += dbs_dAlign(sizeof(oh))) {
    if (!dbs_AlignedRead(sts, &oh, sizeof(oh), &cp->dbs)) {
      errh_LogFatal(&cp->log, "reloadSectObject, fread, %m", *sts);
      return NO;
    }

    if (cp->dbs.file.format.m != cp->format.m) {
      pdrmem_create(&pdrs, &oh, sizeof(oh), PDR_DECODE,
                    cp->dbs.file.format, cp->format);
      if(!pdr_dbs_sObject(&pdrs, &oh))
        pwr_Return(NO, sts, INI__XDR);
    
    }
  
    if (oh.cid == pwr_eClass_Node && oh.oid.vid == cp->node.nid)
      cp->node.nod_oid = oh.oid;  /* This is the node object.  */

    op = reloadObject(sts, cp, vp, &oh);
    if (op == NULL) {
      errh_LogError(&cp->log, "Loading object %s, %s, parent %s\n%m",
	cdh_ObjidToString(NULL, oh.oid, 0), oh.name, cdh_ObjidToString(NULL, oh.poid, 0), sts);
      return NO;
    }
  }

  return ODD(*sts);

}

static pwr_tBoolean
loadSectScObject (
  pwr_tStatus		*status,
  ini_sContext		*cp,
  ivol_sVolume		*vp
)
{
  pwr_tUInt32		i;
  dbs_sScObject	        sc;
  gdb_sScObject	        *scp;
  PDR			pdrs;

  pwr_dStatus(sts, status, INI__SUCCESS);

  /* Read the section header by header */

  if (fseek(cp->dbs.f, cp->sect.offset, SEEK_SET) != 0)
    pwr_Return(NO, sts, errno_GetStatus());

  for (i = 0; i < cp->sect.size; i += dbs_dAlign(sizeof(sc))) {
    if (!dbs_AlignedRead(sts, &sc, sizeof(sc), &cp->dbs)) {
      errh_LogFatal(&cp->log, "loadSectScObject, fread, %m", *sts);
      break;
    }

    if (cp->dbs.file.format.m != cp->format.m) {
      pdrmem_create(&pdrs, &sc, sizeof(sc), PDR_DECODE,
                    cp->dbs.file.format, cp->format);
      if(!pdr_dbs_sScObject(&pdrs, &sc))
        pwr_Return(NO, sts, INI__XDR);
    }

    scp = ivol_LoadScObject(sts, vp, &sc, vol_mLinkSc_load);
    if (scp == NULL) {
      errh_LogError(&cp->log, "Loading object %s, parent %s\n%m",
	cdh_ObjidToString(NULL, sc.oid, 0), cdh_ObjidToString(NULL, sc.poid, 0), *sts);
      return NO;
    }
  }

  return YES;
}

static pwr_tBoolean
loadSectVolume (
  pwr_tStatus		*status,
  ini_sContext		*cp,
  ivol_sVolume		*vp
)
{
  PDR			pdrs;

  pwr_dStatus(sts, status, INI__SUCCESS);

  /* Read the section */

  if (fseek(cp->dbs.f, cp->sect.offset, SEEK_SET) != 0)
    pwr_Return(NO, sts, errno_GetStatus());

  if (!dbs_AlignedRead(sts, &vp->volume, cp->sect.size, &cp->dbs)) {
    errh_LogFatal(&cp->log, "loadSectVolume, fread, %m", *sts);
    return NO;
  }

  if (cp->dbs.file.format.m != cp->format.m) {
    pdrmem_create(&pdrs, &vp->volume, sizeof(vp->volume), PDR_DECODE,
                  cp->dbs.file.format, cp->format);
    if(!pdr_dbs_sVolume(&pdrs, &vp->volume))
      pwr_Return(NO, sts, INI__XDR);
  }

  errh_LogInfo(&cp->log, "Loading volume %s (%s)...", vp->volume.name, vp->volume.className);

  vp->vp = ivol_LoadVolume(sts, &vp->volume, &cp->dbs.file.format);

  return ODD(*sts);
}

static pwr_tBoolean
reloadSectVolume (
  pwr_tStatus		*status,
  ini_sContext		*cp,
  ivol_sVolume		*vp
)
{
  PDR			pdrs;

  pwr_dStatus(sts, status, INI__SUCCESS);

  /* Read the section */

  if (fseek(cp->dbs.f, cp->sect.offset, SEEK_SET) != 0)
    pwr_Return(NO, sts, errno_GetStatus());

  if (!dbs_AlignedRead(sts, &vp->volume, cp->sect.size, &cp->dbs)) {
    errh_LogFatal(&cp->log, "reloadSectVolume, fread, %m", *sts);
    return NO;
  }

  if (cp->dbs.file.format.m != cp->format.m) {
    pdrmem_create(&pdrs, &vp->volume, sizeof(vp->volume), PDR_DECODE,
                  cp->dbs.file.format, cp->format);
    if(!pdr_dbs_sVolume(&pdrs, &vp->volume))
      pwr_Return(NO, sts, INI__XDR);
  }

  errh_LogInfo(&cp->log, "Loading volume %s (%s)...", vp->volume.name, vp->volume.className);

  gdb_Lock;

    vp->vp = hash_Search(sts, gdbroot->vid_ht, &vp->vid);

  gdb_Unlock;

  return ODD(*sts);
}

ini_sContext *
ini_CheckContext (
  pwr_tStatus	*status,
  ini_sContext	*cp
)
{
  pwr_dStatus(sts, status, INI__SUCCESS);

  ini_LoadDirectory(sts, cp);

  if (!cp->flags.b.aliasfile)
    sprintf(cp->aliasfile.name, dbs_cNameAlias, cp->dir);
  cdh_ToLower(cp->aliasfile.name, cp->aliasfile.name);
  cp->nodefile.errcount = NULL;
  cp->nodefile.logOpenFail = errh_LogInfo;

  if (!cp->flags.b.applfile)
    sprintf(cp->applfile.name, dbs_cNameAppl, cp->dir, cp->nodename, cp->busid);
  cdh_ToLower(cp->applfile.name, cp->applfile.name);
  cp->applfile.errcount = NULL;
  cp->applfile.logOpenFail = errh_LogInfo;

  if (!cp->flags.b.bootfile)
    sprintf(cp->bootfile.name, dbs_cNameBoot, cp->dir, cp->nodename, cp->busid);
  cdh_ToLower(cp->bootfile.name, cp->bootfile.name);
  cp->bootfile.errcount = &cp->fatals;
  cp->bootfile.logOpenFail = errh_LogFatal;

  if (!cp->flags.b.nodefile)
    sprintf(cp->nodefile.name, dbs_cNameNode, cp->dir, cp->busid);
  cdh_ToLower(cp->nodefile.name, cp->nodefile.name);
  cp->nodefile.errcount = &cp->fatals;
  cp->nodefile.logOpenFail = errh_LogFatal;

  cdh_ToLower(cp->plcfile.name, cp->plcfile.name);
  cp->plcfile.errcount = NULL;
  cp->plcfile.logOpenFail = errh_LogInfo;

  return cp;  
}

ini_sContext *
ini_CreateContext (
  pwr_tStatus	*status
)
{
  ini_sContext *cp;

  pwr_dStatus(sts, status, INI__SUCCESS);

  cp = calloc(1, sizeof(*cp));
  if (cp == NULL)
    return NULL;

  cp->nid_t = tree_CreateTable(sts, sizeof(pwr_tNodeId),
    offsetof(qini_sNode, nid), sizeof(qini_sNode), 10,
    tree_Comp_nid);

#if 0
  cp->vname_t = tree_CreateTable(sizeof(pwr_tObjName),
    offsetof(ivol_sVolume, name), sizeof(ivol_sVolume), 10,
    tree_eComp_strncmp, NULL);
#else
  cp->vid_t = tree_CreateTable(sts, sizeof(pwr_tVolumeId),
    offsetof(ivol_sVolume, vid), sizeof(ivol_sVolume), 10,
    tree_Comp_vid);
#endif

  cp->proc_t = tree_CreateTable(sts, sizeof(((ini_sProc *)0)->id),
    offsetof(ini_sProc, id), sizeof(ini_sProc), 10,
    tree_Comp_strncmp);

  lst_Init(NULL, &cp->vol_lh, NULL);
  lst_Init(NULL, &cp->proc_lh, NULL);

  co_GetOwnFormat(&cp->format);

  if (cp->vid_t == NULL) {
    free(cp);
    return NULL;
  }

  return cp;  
}

FILE *
ini_OpenFile (
  pwr_tStatus	*status,
  ini_sContext	*cp,
  ini_sFile	*fp

)
{
  FILE *f;

  pwr_dStatus(sts, status, INI__SUCCESS);

  f = fopen(fp->name, "r");
  if (f == NULL) {
    if (fp->errcount)
      (*fp->errcount)++;
    if (fp->logOpenFail)
      fp->logOpenFail(&cp->log, "Could not open file %s", fp->name);
    return NULL;
  }

  if (cp->flags.b.verbose) errh_LogInfo(&cp->log, "Opened file %s", fp->name);

  return f;
}

char *
ini_LoadDirectory (
  pwr_tStatus	*status,
  ini_sContext *cp
)
{
  pwr_dStatus(sts, status, INI__SUCCESS);

  if (!cp->flags.b.nodename)
    syi_NodeName(sts, cp->nodename, sizeof(cp->nodename));
  if (!cp->flags.b.hostname)
    syi_HostName(sts, cp->hostname, sizeof(cp->hostname));

  syi_NodeSpec(sts, cp->nodespec, sizeof(cp->nodespec));
  syi_HostSpec(sts, cp->hostspec, sizeof(cp->hostspec));
  syi_BootDisk(sts, cp->bootdisk, sizeof(cp->bootdisk));

#if defined(OS_ELN)

  if (syi_LocalBoot(sts)) {
    sprintf(cp->dir, "%s[sys0.sysexe]", cp->bootdisk);
    if (cp->flags.b.restart) {
      cp->busid = qcom_MyBus(sts);
    }
  } else {
    if (!cp->flags.b.restart) {
      if(!ini_Time(sts, &cp->busid))
	exit(*sts);
    } else {
      cp->busid = qcom_MyBus(sts);
    }
    sprintf(cp->dir, "%s::%s", cp->hostspec, dbs_cNameDirectory);
    sprintf(cp->bdir, "%s::%s", cp->hostspec, dbs_cNameBaseDirectory);
  }

#elif defined(OS_VMS)
  {
    char *s;

    if (!cp->flags.b.busid) {
      s = getenv(pwr_dEnvBusId);
      if (s != NULL)
	cp->busid = atoi(s);
    }
    sprintf(cp->bdir, "%s", dbs_cNameBaseDirectory);
    sprintf(cp->dir, "%s", dbs_cNameDirectory);
  }
#elif defined(OS_LYNX) || defined(OS_LINUX)
  {
    char *s;

    if ((s = getenv(dbs_cNameBaseDirectory)) != NULL)
      sprintf(cp->bdir, "%s/", s);
    else
      errh_LogError(&cp->log, "Environment variable '%s' is not defined", dbs_cNameBaseDirectory);

    if ((s = getenv(dbs_cNameDirectory)) != NULL)
      sprintf(cp->dir, "%s/", s);
    else
      errh_LogError(&cp->log, "Environment variable '%s' is not defined", dbs_cNameDirectory);

    if (!cp->flags.b.busid) {
      s = getenv(pwr_dEnvBusId);
      if (s != NULL)

	cp->busid = atoi(s);
    }
  }
#else
# error "NYI for this OS"
#endif

  *sts = ini_GetAlias(cp->aliasfile.name, cp->nodename, cp->alias);
  if (ODD(*sts)) {
    errh_LogInfo(&cp->log, "Alias defined to %s for this node %s", cp->alias, cp->nodename);
    strcpy(cp->nodename, cp->alias);
  }

  return cp->dir;
}

#if 0
ini_UpdateSystemInfo (ini_sContext *cp)
{
    /* Certain fields in the node and system objects must be filled in.  */

    node_bp = ivol_GetBody(&sts, cp->node.nod_oid, NULL);
    if (node_bp == NULL) {
      errh_LogFatal(&cp->log, "Can not find a node object!");
      return INI__ERROR;
    }

    Node = (pwr_sNode *) node_bp->body;

    Node->BootVersion    = cp->node.rtVersion;
    Node->BootCreTime    = cp->node.rtCreTime;
    Node->CurVersion     = cp->node.rtVersion;
    Node->CurCreTime     = cp->node.rtCreTime;
    Node->BootPLCVersion = cp->node.plcVersion;
    Node->CurPLCVersion  = cp->node.plcVersion;
    clock_gettime(CLOCK_REALTIME, &Node->LastChgTime);
#if 0
//    Node->Swaps++;
//    Node->LastCriticalDuration = ;
//    Node->MeanCriticalDuration = ;
#endif
    

    gdb_Unlock;

    {
      /* Set SystemName to project name given in boot file.  */
      pwr_tStatus	  sts;
      int		  size = MIN(sizeof(System.SystemName) - 1, strlen(cp->proj));
      int		  gsize = MIN(sizeof(System.SystemGroup) - 1, strlen(cp->group));

      sts = gdh_SetObjectInfo("pwrNode-System.SystemName", cp->proj, size);
      sts = gdh_SetObjectInfo("pwrNode-System.SystemGroup", cp->group, gsize);
    }
}
#endif

void
ini_ReadBootFile (
  pwr_tStatus	*status,
  ini_sContext	*cp
)
{
  char			day[80];
  char			time[80];
  char			buffer[256];
  char			*s;
  int			i;
  int			n;
  int			nvol;

#if 1
  pwr_tObjName          vname;
  char			vids[80];
  pwr_tVolumeId		vid;
#else
  char			vids[80];
  pwr_tVolumeId		vid;
#endif

  /*  pwr_tProjVersion	ver;*/
  FILE *f;
  ivol_sVolume		*vp;

  pwr_dStatus(sts, status, INI__SUCCESS);

  f = ini_OpenFile(sts, cp, &cp->bootfile);

  if (f == NULL)
    return;

  errh_LogInfo(&cp->log, "Reading Boot file %s", cp->bootfile.name);

  for (
    i = 0, nvol = 0, s = fgets(buffer, sizeof(buffer) - 1, f);
    s != NULL;
    s = fgets(buffer, sizeof(buffer) - 1, f)
  ) {
    if (*s == '!') {
      s++;
      continue;
    }

    switch (i) {
    case 0:	/* Creation Date.  */
      i++;
      time[0] = day[0] = '\0';
      n = sscanf(s, "%s %s", day, time);
      break;
    case 1:
      i++;
      cp->proj[0] = '\0';
      n = sscanf(s, "%s", cp->proj);
      errh_LogInfo(&cp->log, "Created at %s %s for project: %s", day, time, cp->proj);
      break;
    case 2:
      i++;
      cp->group[0] = '\0';
      n = sscanf(s, "%s", cp->group);
      break;
    case 3: {	/* Find PLC version.  */
      char *t;

      i++;

      n = sscanf(s, "%s %s", vname, vids);
      if (n < 2) {
	errh_LogError(&cp->log, "Bootfile corrupt, error in plc data");
	cp->errors++;
	continue;
      }

      t = strrchr( vname, '_');
      n = sscanf(t+1, "%d", &cp->node.plcVersion);
      if (n < 1) {
	errh_LogError(&cp->log, "Bootfile corrupt, error in plc data");
	cp->errors++;
	continue;
      }

      if (!cp->flags.b.plcfile) {
#if defined OS_LYNX || defined OS_LINUX
	sprintf(cp->plcfile.name, dbs_cNamePlc, "", cp->nodename, cp->busid, cp->node.plcVersion);
#elif defined OS_VMS
	sprintf(cp->plcfile.name, dbs_cNamePlc, "pwrp_exe:", cp->nodename, cp->busid, cp->node.plcVersion);
#else
	sprintf(cp->plcfile.name, dbs_cNamePlc, cp->dir, cp->nodename, cp->busid, cp->node.plcVersion);
#endif
      }
#if 0
  char			tempname[256];
#if defined OS_LYNX || defined OS_LINUX
     /* The path should be defined by the PATH variable
      * We want short path names because LYNX's ps command only
      * displays the first 31 characters of the file name.
      */
      sprintf(cp->plcfile.name, "%s", tempname);
#else
      sprintf(cp->plcfile.name, "%s%s", cp->hostspec, tempname);
#endif
#endif
      cdh_ToLower(cp->plcfile.name, cp->plcfile.name);
      errh_LogInfo(&cp->log, "This node vill run PLC file: %s", cp->plcfile.name);
      break;
    }
    case 4:	/* Find root volume.  */
      i++;
      n = sscanf(s, "%s %s", vname, vids);
      if (n < 2) {
	errh_LogError(&cp->log, "Found no root volume");
	cp->errors++;
	continue;
      } else {
#if 1
	*sts = cdh_StringToVolumeId(vids, &vid);
	if (EVEN(*sts)) {
	  errh_LogError(&cp->log, "Found no root volume");
	  cp->errors++;
	  continue;
	}
	errh_LogInfo(&cp->log, "This node has node identity %s (%d)", cdh_NodeIdToString(NULL, vid, 0, 0), vid);
	cp->node.nid = vid;
#endif
      }
      vp = tree_Insert(sts, cp->vid_t, &vid);
      strcpy(vp->name, vname);
      lst_InsertPred(NULL, &cp->vol_lh, &vp->ll, vp);
      vp->isVolRef = 0;
      vp->oid_t = tree_CreateTable(sts, sizeof(pwr_tObjectIx),
	offsetof(ivol_sObject, oix), sizeof(ivol_sObject), 10,
	tree_Comp_oix);
      lst_Init(NULL, &vp->upd_lh, NULL);
      lst_Init(NULL, &vp->upd_io_lh, NULL);
      lst_Init(NULL, &vp->cre_lh, NULL);
#if 0
//      lst_Init(NULL, &vp->del_lh, NULL);
#endif

      break;
    case 5:	/* Find other volume.  */
#if 1
      n = sscanf(s, "%s %s", vname, vids);
#else
      n = sscanf(s, "%s", vids);
#endif
      if (n < 2) {
	errh_LogError(&cp->log, "Error in line, skip to next line\n%s", s);
	cp->errors++;
	continue;
      } else {
#if 1
	*sts = cdh_StringToVolumeId(vids, &vid);
	if (EVEN(*sts)) {
	  errh_LogError(&cp->log, "Error in line, skip to next line\n%s\n%m", s, *sts);
	  cp->errors++;
	  continue;
	}
#endif
      }
      /* Do we have this volume?  */
      vp = tree_Find(sts, cp->vid_t, &vid);
      if (vp != NULL) {
	errh_LogInfo(&cp->log, "Volume already defined: %s", s);
	vp->isVolRef = 0;
	/* todo !!! check versions */
      } else {
	vp = tree_Insert(sts, cp->vid_t, &vid);
        strcpy(vp->name, vname);
	lst_InsertPred(NULL, &cp->vol_lh, &vp->ll, vp);
	vp->oid_t = tree_CreateTable(sts, sizeof(pwr_tObjectIx),
	  offsetof(ivol_sObject, oix), sizeof(ivol_sObject), 10,
	  tree_Comp_oix);
	lst_Init(NULL, &vp->upd_lh, NULL);
	lst_Init(NULL, &vp->upd_io_lh, NULL);
	lst_Init(NULL, &vp->cre_lh, NULL);
#if 0
//	lst_Init(NULL, &vp->del_lh, NULL);
#endif
      }
      break;
    }
  }

  fclose(f);
}

pwr_tBoolean
ini_IterVolumes (
  pwr_tStatus *sts,
  ini_sContext *cp,
  pwr_tBoolean (*func)(pwr_tStatus*, ini_sContext*, ivol_sVolume*)
)
{
  ivol_sVolume		*vp;
  lst_sEntry		*vl;
  
  for (vp = lst_Succ(NULL, &cp->vol_lh, &vl); vp != NULL; vp = lst_Succ(NULL, vl, &vl)) {
    sprintf(vp->filename, dbs_cNameVolume, cp->bdir, vp->name);
    cdh_ToLower(vp->filename, vp->filename);

    dbs_Open(sts, &cp->dbs, vp->filename);
    if (*sts == ERRNO__NOENT) { /* Give pwrp a chance */
        sprintf(vp->filename, dbs_cNameVolume, cp->dir, vp->name);
        cdh_ToLower(vp->filename, vp->filename);
        dbs_Open(sts, &cp->dbs, vp->filename);
    }

    if (EVEN(*sts)) {
      errh_LogError(&cp->log, "Open file %s, %m", vp->filename, *sts);
      cp->errors++;
      continue;
    }
    
    errh_LogInfo(&cp->log, "Reading volume file %s", vp->filename);
    
    func(sts, cp, vp);

    dbs_Close(sts, &cp->dbs);

    if (EVEN(*sts)) {
      errh_LogError(&cp->log, "Error while reading volume file %s, %m", vp->filename, *sts);
      cp->errors++;
      break;
    }
  }
  return ODD(*sts);
}


/* read all load file headers to get sizes */
pwr_tBoolean
ini_CheckVolumeFile (
  pwr_tStatus		*status,
  ini_sContext		*cp,
  ivol_sVolume		*vp
)
{
  int reqmask = 0;
  int sects = 0;
  unsigned int nextpos;
  PDR pdrs;

  pwr_dStatus(sts, status, INI__SUCCESS);

  nextpos = dbs_dAlign(sizeof(dbs_sFile));
  do {
    if (fseek(cp->dbs.f, nextpos, SEEK_SET) != 0) {
      *sts = errno_GetStatus();
      errh_LogFatal(&cp->log, "ini_CheckVolumeFile, fseek, %m", *sts);
      return NO;
    }
    if (!dbs_AlignedRead(sts, &cp->sect, sizeof(cp->sect), &cp->dbs)) {
      errh_LogFatal(&cp->log, "ini_CheckVolumeFile, fread, %m", *sts);
      return NO;
    }

    if (cp->dbs.file.format.m != cp->format.m) {
      pdrmem_create(&pdrs, &cp->sect, sizeof(cp->sect), PDR_DECODE,
                    cp->dbs.file.format, cp->format);
      if(!pdr_dbs_sSect(&pdrs, &cp->sect)) {
        *sts = INI__XDR;
        errh_LogFatal(&cp->log, "ini_CheckVolumeFile, pdr, %m", *sts);
        return NO;
      }
    }

    nextpos = ftell(cp->dbs.f);

    switch (cp->sect.type) {

    case dbs_eSect_dir:
      if (checkSect(sts, cp, sects, dbs_cVersionDirectory))
	readSectFile(sts, cp);
      break;
    case dbs_eSect_volume:
      if (checkSect(sts, cp, sects, dbs_cVersionVolume))
	readSectVolume(sts, cp, vp);
      break;
    case dbs_eSect_volref:
      if (checkSect(sts, cp, sects, dbs_cVersionVolRef))
	readSectVolRef(sts, cp);
      break;
    case dbs_eSect_oid:
      checkSect(sts, cp, sects, dbs_cVersionOid);
      break;
    case dbs_eSect_object:
      checkSect(sts, cp, sects, dbs_cVersionObject);
      break;
    case dbs_eSect_rbody:
      checkSect(sts, cp, sects, dbs_cVersionRbody);
      break;
    default:
      *sts = INI__BADSECT;	/* Successful return status; means that */
      break;			/* we can continue after unknown sections */
    }
    if (EVEN(*sts))
      break;	/* Panic exit */

    sects |= 1<<cp->sect.type;

  } while (cp->sect.type != dbs_eSect_rbody);

  reqmask = (1<<dbs_eSect_dir) | (1<<dbs_eSect_volume) | (1<<dbs_eSect_volref)
               | (1<<dbs_eSect_oid) | (1<<dbs_eSect_object) | (1<<dbs_eSect_rbody);
  if ((sects & reqmask) != reqmask) {
    *sts = INI__SECTORDER;
    errh_LogError(&cp->log, "Section order");
    cp->errors++;
  }
  
  return ODD(*sts);
}

pwr_tBoolean
ini_LoadVolume (
  pwr_tStatus		*status,
  ini_sContext		*cp,
  ivol_sVolume		*vp
)
{
  pwr_tUInt32		reqmask = 0;
  pwr_tUInt32		sects = 0;
  pwr_tUInt32		nextpos;
  PDR			pdrs;

  pwr_dStatus(sts, status, INI__SUCCESS);

  nextpos = dbs_dAlign(sizeof(dbs_sFile));
  do {
    if (fseek(cp->dbs.f, nextpos, SEEK_SET) != 0) {
      *sts = errno_GetStatus();
      errh_LogFatal(&cp->log, "ini_LoadVolumeFile, fseek, %m", *sts);
      return NO;
    }
    if (!dbs_AlignedRead(sts, &cp->sect, sizeof(cp->sect), &cp->dbs)) {
      errh_LogFatal(&cp->log, "ini_LoadVolumeFile, fread, %m", *sts);
      return NO;
    }

    if (cp->dbs.file.format.m != cp->format.m) {
      pdrmem_create(&pdrs, &cp->sect, sizeof(cp->sect), PDR_DECODE,
                    cp->dbs.file.format, cp->format);
      if(!pdr_dbs_sSect(&pdrs, &cp->sect)) {
        *sts = INI__XDR;
        errh_LogFatal(&cp->log, "ini_LoadVolumeFile, pdr, %m", *sts);
        return NO;
      }
    }  

    if (cp->sect.version != dbs_cVersionDirectory) {
      *sts = INI__HEADVERSION;
      break;
    }

    nextpos = ftell(cp->dbs.f);

    switch (cp->sect.type) {
    case dbs_eSect_dir:
      checkSect(sts, cp, sects, dbs_cVersionDirectory);
      break;
    case dbs_eSect_volume:
      if (checkSect(sts, cp, sects, dbs_cVersionVolume))
	loadSectVolume(sts, cp, vp);
      break;
    case dbs_eSect_volref:
      checkSect(sts, cp, sects, dbs_cVersionVolRef);
      break;
    case dbs_eSect_oid:
      checkSect(sts, cp, sects, dbs_cVersionOid);
      break;
    case dbs_eSect_object:
      if (checkSect(sts, cp, sects, dbs_cVersionObject))
	loadSectObject(sts, cp, vp);
      break;
    case dbs_eSect_rbody:
      if (checkSect(sts, cp, sects, dbs_cVersionRbody))
	loadSectRbody(sts, cp, vp);
      break;
    default:
    case dbs_eSect_scobject:
      if (checkSect(sts, cp, sects, dbs_cVersionScObject))
	loadSectScObject(sts, cp, vp);
      break;
      *sts = INI__BADSECT;	/* Successful return status; means that */
      break;			/* we can continue after unknown sections */
    }
    if (EVEN(*sts))
      break;	/* Panic exit */

    sects |= 1<<cp->sect.type;

  } while (cp->sect.type != dbs_eSect_rbody);


  /** @todo add dbs_eSect_scobject to reqmask */
  reqmask = (1<<dbs_eSect_dir) | (1<<dbs_eSect_volume) | (1<<dbs_eSect_volref)
               | (1<<dbs_eSect_oid) | (1<<dbs_eSect_object) | (1<<dbs_eSect_rbody);

  if ((sects & reqmask) != reqmask) {
    *sts = INI__SECTORDER;
    errh_LogError(&cp->log, "Section order");
    cp->errors++;
  }

  return ODD(*sts);
}

pwr_tBoolean
ini_ReloadVolume (
  pwr_tStatus		*status,
  ini_sContext		*cp,
  ivol_sVolume		*vp
)
{
  pwr_tUInt32		reqmask = 0;
  pwr_tUInt32		sects = 0;
  pwr_tUInt32		nextpos;
  PDR			pdrs;

  pwr_dStatus(sts, status, INI__SUCCESS);

  nextpos = dbs_dAlign(sizeof(dbs_sFile));
  do {
    if (fseek(cp->dbs.f, nextpos, SEEK_SET) != 0) {
      *sts = errno_GetStatus();
      errh_LogFatal(&cp->log, "ini_ReloadVolumeFile, fseek, %m", *sts);
      return NO;
    }
    if (!dbs_AlignedRead(sts, &cp->sect, sizeof(cp->sect), &cp->dbs)) {
      errh_LogFatal(&cp->log, "ini_ReloadVolumeFile, fread, %m", *sts);
      return NO;
    }

    if (cp->dbs.file.format.m != cp->format.m) {
      pdrmem_create(&pdrs, &cp->sect, sizeof(cp->sect), PDR_DECODE,
                    cp->dbs.file.format, cp->format);
      if(!pdr_dbs_sSect(&pdrs, &cp->sect)) {
        *sts = INI__XDR;
        errh_LogFatal(&cp->log, "ini_ReloadVolumeFile, pdr, %m", *sts);
        return NO;
      }
    }
  
    if (cp->sect.version != dbs_cVersionDirectory) {
      *sts = INI__HEADVERSION;
      break;
    }

    nextpos = ftell(cp->dbs.f);

    switch (cp->sect.type) {
    case dbs_eSect_dir:
      checkSect(sts, cp, sects, dbs_cVersionDirectory);
      break;
    case dbs_eSect_volume:
      if (checkSect(sts, cp, sects, dbs_cVersionVolume))
	reloadSectVolume(sts, cp, vp);
      break;
    case dbs_eSect_volref:
      checkSect(sts, cp, sects, dbs_cVersionVolRef);
      break;
    case dbs_eSect_oid:
      checkSect(sts, cp, sects, dbs_cVersionOid);
      break;
    case dbs_eSect_object:
      if (checkSect(sts, cp, sects, dbs_cVersionObject))
	reloadSectObject(sts, cp, vp);
      break;
    case dbs_eSect_rbody:
      if (checkSect(sts, cp, sects, dbs_cVersionRbody))
	reloadSectRbody(sts, cp, vp);
      break;
    default:
      *sts = INI__BADSECT;	/* Successful return status; means that */
      break;			/* we can continue after unknown sections */
    }
    if (EVEN(*sts))
      break;	/* Panic exit */

    sects |= 1<<cp->sect.type;

  } while (cp->sect.type != dbs_eSect_rbody);

  reqmask = (1<<dbs_eSect_dir) | (1<<dbs_eSect_volume) | (1<<dbs_eSect_volref)
               | (1<<dbs_eSect_oid) | (1<<dbs_eSect_object) | (1<<dbs_eSect_rbody);

  if ((sects & reqmask) != reqmask) {
    *sts = INI__SECTORDER;
    errh_LogError(&cp->log, "Section order");
    cp->errors++;
  }

  return ODD(*sts);
}

pwr_tBoolean
ini_CheckNode (
  pwr_tStatus		*status,
  ini_sContext		*cp
)
{

  pwr_dStatus(sts, status, INI__SUCCESS);

  return ini_IterVolumes(sts, cp, ini_CheckVolumeFile);
}

pwr_tBoolean
ini_LoadNode (
  pwr_tStatus		*status,
  ini_sContext		*cp
)
{
  pwr_tBoolean result;
  pwr_dStatus(sts, status, INI__SUCCESS);

  gdb_ScopeLock {
    result = ini_IterVolumes(sts, cp, ini_LoadVolume);
  } gdb_ScopeUnlock;

  return result;
}

pwr_tBoolean
ini_ReloadNode (
  pwr_tStatus		*status,
  ini_sContext		*cp
)
{
  pwr_dStatus(sts, status, INI__SUCCESS);

  return ini_IterVolumes(sts, cp, ini_ReloadVolume);
}

pwr_tBoolean
ini_UpdateBodies (
  pwr_tStatus	*status,
  ini_sContext	*cp,
  pwr_tBoolean	io
)
{
  ivol_sVolume	*vp;
  lst_sEntry	*vl;
  ivol_sObject	*iop;
  lst_sEntry	*iol;
  lst_sEntry	*bl;
  
  pwr_dStatus(sts, status, INI__SUCCESS);

  for (vp = lst_Succ(NULL, &cp->vol_lh, &vl); vp != NULL; vp = lst_Succ(NULL, vl, &vl)) {
    if (io)
      bl = &vp->upd_io_lh;
    else
      bl = &vp->upd_lh;

    for (iop = lst_Succ(NULL, bl, &iol); iop != NULL; iop = lst_Succ(NULL, iol, &iol)) {
      if (iop->body == NULL || iop->upd_body == NULL) 
        continue;
      ivol_CopyBody(sts, iop->upd_body, iop->body, iop->cp);
    }
  }

  return TRUE;
}

pwr_tBoolean
ini_FreeBodies (
  pwr_tStatus	*status,
  ini_sContext	*cp,
  pwr_tBoolean	io
)
{
  ivol_sVolume	*vp;
  lst_sEntry	*vl;
  ivol_sObject	*iop;
  lst_sEntry	*iol;
  lst_sEntry	*bl;
  
  pwr_dStatus(sts, status, INI__SUCCESS);

  for (vp = lst_Succ(NULL, &cp->vol_lh, &vl); vp != NULL; vp = lst_Succ(NULL, vl, &vl)) {
    if (io)
      bl = &vp->upd_io_lh;
    else
      bl = &vp->upd_lh;

    for (iop = lst_Succ(NULL, bl, &iol); iop != NULL; iop = lst_Succ(NULL, iol, &iol)) {
      if (iop->upd_body == NULL) continue;
      free(iop->upd_body);
      iop->upd_body = NULL;
    }
  }

  return YES;
}

pwr_tBoolean
ini_DecodeBodies (
  pwr_tStatus	*status,
  ini_sContext	*cp,
  pwr_tBoolean	io
)
{
  ivol_sVolume	*vp;
  lst_sEntry	*vl;
  ivol_sObject	*iop;
  lst_sEntry	*iol;
  lst_sEntry	*bl;
  
  pwr_dStatus(sts, status, INI__SUCCESS);

  for (vp = lst_Succ(NULL, &cp->vol_lh, &vl); vp != NULL; vp = lst_Succ(NULL, vl, &vl)) {
    if (io)
      bl = &vp->upd_io_lh;
    else
      bl = &vp->upd_lh;

    for (iop = lst_Succ(NULL, bl, &iol); iop != NULL; iop = lst_Succ(NULL, iol, &iol)) {
      if (iop->body == NULL || iop->upd_body == NULL) continue;
      if (!ivol_DecodeBody(sts, iop->upd_body, iop->cp))
	return FALSE;
    }
  }

  return TRUE;
}

pwr_tBoolean
ini_UpdateDatabase (
  pwr_tStatus	*status,
  ini_sContext	*cp
)
{
  ivol_sVolume	*vp;
  lst_sEntry	*vl;
  
  pwr_dStatus(sts, status, INI__SUCCESS);

  for (vp = lst_Succ(NULL, &cp->vol_lh, &vl); vp != NULL; vp = lst_Succ(NULL, vl, &vl)) {
  }

  return YES;
}

pwr_tBoolean
ini_BuildNode (
  pwr_tStatus	*status,
  ini_sContext	*cp
)
{
  pwr_sNode	*Node;
  ivol_sBody	*node_bp = NULL;
  pwr_tStatus	tsts;
  pwr_tObjid	oid = pwr_cNObjid;
  pwr_sSystem	System;
  int		size = MIN(sizeof(System.SystemName) - 1, strlen(cp->proj));
  int		gsize = MIN(sizeof(System.SystemGroup) - 1, strlen(cp->group));

  pwr_dStatus(sts, status, INI__SUCCESS);

  gdb_ScopeLock {

    ivol_BuildNode(sts, &cp->node, &cp->dbs.file.format);
    if (EVEN(*sts)) {
      errh_LogFatal(&cp->log, "Building runtime database, %m", *sts);
      break;
    }

    node_bp = ivol_GetBody(sts, cp->node.nod_oid, NULL);
    if (node_bp == NULL) {
      errh_LogFatal(&cp->log, "Can not find a node object!");
      break;
    }

    Node = (pwr_sNode *) node_bp->body;

    Node->BootVersion    = cp->node.rtVersion;
    Node->BootCreTime    = cp->node.rtCreTime;
    Node->CurVersion     = cp->node.rtVersion;
    Node->CurCreTime     = cp->node.rtCreTime;

    cp->np = Node;
#if 0
    Node->BootPLCVersion = cp->node.plcVersion;
    Node->CurPLCVersion  = cp->node.plcVersion;
    clock_gettime(CLOCK_REALTIME, &Node->LastChgTime);
#endif

  } gdb_ScopeUnlock;

  if (EVEN(*sts)) {
    if (cp->flags.b.ignoreFatal)
      return NO;
    exit(INI__ERROR);
  }

  /* Create a $System object and set SystemName to project name given in boot file.  */

  tsts = gdh_CreateObject("pwrNode-System", pwr_eClass_System, sizeof(pwr_sSystem),
    &oid, pwr_cNObjid, 0, pwr_cNObjid);
  if (ODD(tsts)) {
    gdh_SetObjectInfo("pwrNode-System.SystemName", cp->proj, size); 
    gdh_SetObjectInfo("pwrNode-System.SystemGroup", cp->group, gsize); 
  }

  create_active_io();

  return ODD(*sts);
}

pwr_tBoolean
ini_RebuildNode (
  pwr_tStatus	*status,
  ini_sContext	*cp
)
{
  ivol_sVolume	*vp;
  lst_sEntry	*vl;

  pwr_dStatus(sts, status, INI__SUCCESS);

  if (cp->flags.b.verbose)
    errh_LogInfo(&cp->log, "Rebuild database...");

  gdb_ScopeExcl {

    for (vp = lst_Succ(NULL, &cp->vol_lh, &vl); vp != NULL; vp = lst_Succ(NULL, vl, &vl)) {
      if (cp->flags.b.verbose)
	errh_LogInfo(&cp->log, "Rebuild volume: %s", vp->volume.name);
      ivol_RebuildVolume(sts, vp, &cp->dbs.file.format);
    }

  } gdb_ScopeUnexcl;

  if (cp->flags.b.verbose)
    errh_LogInfo(&cp->log, "Database rebuilt");

  if (cp->flags.b.verbose)
    errh_LogInfo(&cp->log, "Create io areas...");
  rename_active_io(cp);
  create_active_io();
  if (cp->flags.b.verbose)
    errh_LogInfo(&cp->log, "Io areas created");

  return YES;
}

pwr_tBoolean
ini_CreateDb (
  pwr_tStatus	*status,
  ini_sContext	*cp
)
{
  gdb_sLocal		*lgdb = NULL;

  pwr_dStatus(sts, status, INI__SUCCESS);

  memset(&cp->gdbInit, 0, sizeof(cp->gdbInit));

  cp->gdbInit.nid         = cp->node.nid;
  cp->gdbInit.objects     = cp->node.cardinality;
  cp->gdbInit.rtdb_isize  = cp->node.bodySize;

#if defined(OS_ELN) 
  cp->gdbInit.cvol_max = MAX(200, cp->node.cvolMaxCount);
  cp->gdbInit.cvol_min = MAX(100, cp->node.cvolMinCount);
#else
  cp->gdbInit.cvol_max = MAX(10000, cp->node.cvolMaxCount);
  cp->gdbInit.cvol_min = MAX(900, cp->node.cvolMinCount);
#endif

  lgdb = gdb_CreateDb(sts, &cp->gdbInit);

  return lgdb != NULL;
}

pwr_tBoolean
ini_ReadNodeFile (
  pwr_tStatus	*status,
  ini_sContext	*cp
)
{
  FILE *f;

  pwr_dStatus(sts, status, INI__SUCCESS);

  f = ini_OpenFile(sts, cp, &cp->nodefile);

  if (f == NULL)
    return NO;

  errh_LogInfo(&cp->log, "Reading Node file %s", cp->nodefile.name);

  qini_ParseFile(f, cp->nid_t, &cp->warnings, &cp->errors, &cp->fatals);
  fclose(f);

  return YES;
}

ini_sProc *
ini_ProcInsert (
  pwr_tStatus	*status,
  ini_sContext	*cp,
  char		*id,
  char		*name,
  int		load,
  int		run,
  char		*file,
  int		prio,
  int		debug,
  char		*arg
)
{
  ini_sProc	*pp;
  char		buf[255];

  pwr_dStatus(sts, status, INI__SUCCESS);

  pp = tree_Insert(sts, cp->proc_t, id);
  if (pp == NULL) return NULL;

  if (name != NULL && name[0] != '\0' && strcmp(name, "\"\"")) {
    if (pp->proc.name != NULL) free(pp->proc.name);
    sprintf(buf, name, cp->busid);
    pp->proc.name = strsav(buf);
  }
  if (load != -1) pp->proc.flags.b.load = load != 0;
  if (run != -1) pp->flags.b.run = run != 0;
  if (file != NULL && file[0] != '\0' && strcmp(file, "\"\"")) {
    if (pp->proc.file != NULL) free(pp->proc.file);
    pp->proc.file = strsav(file);
  }
  if (arg != NULL && arg[0] != '\0' && strcmp(arg, "\"\"")) {
    if (pp->proc.arg != NULL) free(pp->proc.arg);
    pp->proc.arg = strsav(arg);
  }
  if (prio != -1) pp->proc.p_prio = prio;
  if (debug != -1) pp->proc.flags.b.debug = debug != 0;
  if (!lst_IsLinked(NULL, &pp->proc_ll)) {
    lst_InsertPred(NULL, &cp->proc_lh, &pp->proc_ll, pp);
  }

  return pp;
}

void 
ini_ProcLoad (
  pwr_tStatus	*status,
  ini_sContext	*cp,
  ini_sProc	*pp
)  
{

  pwr_dStatus(sts, status, INI__SUCCESS);

#if !defined(OS_ELN)
  return;
#else
  if (pp->flags.b.running) 
    return;
  
  if (pp->proc.flags.b.load) {
    errh_LogInfo(&cp->log, "Loading %s, file: %s, prio: %d", pp->id, pp->proc.file, pp->proc.p_prio);
  } else {
    errh_LogInfo(&cp->log, "%s, file: %s, prio: %d, will not be loaded.", pp->id, pp->proc.file, pp->proc.p_prio);
    return;
  }

  *sts = proc_Load(&pp->proc);
  if (EVEN(*sts))
    errh_Error(&cp->log, "Error loading %s, %m", pp->id, *sts);
#endif
}

void 
ini_ProcStart (
  pwr_tStatus	*status,
  ini_sContext	*cp,
  ini_sProc	*pp
)  
{

  pwr_dStatus(sts, status, INI__SUCCESS);

  if (pp->flags.b.running) 
    return;
  
  if (pp->flags.b.run) {
    errh_LogInfo(&cp->log, "Starting %s, file: %s, prio: %d", pp->id, pp->proc.file, pp->proc.p_prio);
  } else {
    errh_LogInfo(&cp->log, "%s, file: %s, prio: %d, will not be started.", pp->id, pp->proc.file, pp->proc.p_prio);
    return;
  }

  *sts = proc_Start(&pp->proc);
  if (EVEN(*sts))
    errh_LogError(&cp->log, "Error starting %s, %m", pp->id, *sts);
}

void
ini_ProcIter (
  pwr_tStatus	*status,
  ini_sContext	*cp,
  int		mask,
  void		(*func)(pwr_tStatus*, ini_sContext*, ini_sProc*)
)
{
  lst_sEntry	*pl;
  ini_sProc	*pp;

  pwr_dStatus(sts, status, INI__SUCCESS);

  for (pp = lst_Succ(NULL, &cp->proc_lh, &pl); pp != NULL; pp = lst_Succ(NULL, pl, &pl)) {
    if (pp->proc.flags.m & mask)
      func(sts, cp, pp);
  }
}

void
ini_ProcTable (
  pwr_tStatus	*status,
  ini_sContext	*cp
)
{
  FILE		*f;
  pwr_sAppl	*ap;
  pwr_tObjid	oid;
  ini_sProc	*pp;
  char		*s;
  char		buffer[256];

  pwr_dStatus(sts, status, INI__SUCCESS);

  pp = ini_ProcInsert(sts, cp, "pwr_neth", "pwr_neth_%d", 0, 1, "rt_neth", cPrio_neth, 0, "");
  pp->flags.b.neth = 1;
  pp->proc.flags.b.system = 1;

  pp = ini_ProcInsert(sts, cp, "pwr_qmon", "pwr_qmon_%d", 0, 1, "rt_qmon", cPrio_qmon, 0, "");
  pp->flags.b.qmon = 1;
  pp->proc.flags.b.system = 1;

  pp = ini_ProcInsert(sts, cp, "pwr_nacp", "pwr_nacp_%d", 0, 1, "rt_neth_acp", cPrio_neth_acp, 0, "");
  pp->proc.flags.b.system = 1;

  pp = ini_ProcInsert(sts, cp, "pwr_io", "pwr_io_%d", 0, 1, "rt_io_comm", cPrio_io_comm, 0, "");
  pp->proc.flags.b.system = 1;

  pp = ini_ProcInsert(sts, cp, "pwr_tmon", "pwr_tmon_%d", 0, 1, "rt_tmon", cPrio_tmon, 0, "");
  pp->proc.flags.b.system = 1;

  pp = ini_ProcInsert(sts, cp, "pwr_emon", "pwr_emon_%d", 0, 1, "rt_emon", cPrio_emon, 0, "");
  pp->proc.flags.b.system = 1;

  pp = ini_ProcInsert(sts, cp, "pwr_alim", "pwr_alim_%d", 0, 1, "rt_alimserver", cPrio_alimserver, 0, "");
  pp->proc.flags.b.system = 1;

  pp = ini_ProcInsert(sts, cp, "pwr_bck", "pwr_bck_%d", 0, 1, "rt_bck", cPrio_bck, 0, "");
  pp->proc.flags.b.system = 1;

  pp = ini_ProcInsert(sts, cp, "pwr_linksup", "pwr_linksup_%d", 0, 1, "rt_linksup", cPrio_linksup, 0, "");
  pp->proc.flags.b.system = 1;

  pp = ini_ProcInsert(sts, cp, "pwr_trend", "pwr_trend_%d", 0, 1, "rt_trend", cPrio_trend, 0, "");
  pp->proc.flags.b.system = 1;

  pp = ini_ProcInsert(sts, cp, "pwr_fast", "pwr_fast_%d", 0, 1, "rt_fast", cPrio_fast, 0, "");
  pp->proc.flags.b.system = 1;

#if defined OS_LINUX
  pp = ini_ProcInsert(sts, cp, "pwr_elog", "pwr_elog_%d", 0, 1, "rt_elog", cPrio_elog, 0, "");
  pp->proc.flags.b.system = 1;

  pp = ini_ProcInsert(sts, cp, "pwr_webmon", "pwr_webmon_%d", 0, 1, "rt_webmon.sh", cPrio_fast, 0, "");
  pp->proc.flags.b.system = 1;
#endif

  do {
    pwr_sClass_PlcProcess *plc;
    pwr_tObjid oid;
    char p_name[40];

    *sts = gdh_GetClassList(pwr_cClass_PlcProcess, &oid);
    if (EVEN(*sts)) {
      errh_LogError(&cp->log, "Found no PlcProcess-object\n%m", sts);
      break;
    }

    *sts = gdh_ObjidToPointer(oid, (void *)&plc);
    if (EVEN(*sts)) break;

    cp->PlcProcess = plc;
    sprintf(p_name, "pwr_plc_%%d_%d", plc->ChgCount++ % 10); 
    pp = ini_ProcInsert(sts, cp, "pwr_plc", p_name, 1, 1, cp->plcfile.name, cPrio_plc_init, plc->StartWithDebug, "");
    pp->flags.b.plc = 1;
    cp->plc = pp;
    pp->proc.flags.b.user = 1;
    pp->proc.flags.b.k_mode = 1;
    pp->proc.k_size = 30;
  } while (0);

  for (
    *sts = gdh_GetClassList(pwr_eClass_Appl, &oid);
    ODD(*sts);
    *sts = gdh_GetNextObject(oid, &oid)
  ) {
    char name[256];

    gdh_ObjidToName(oid, name, sizeof(name) - 1, cdh_mName_object);
    /* A workaround untill ObjidToName works */
    s = strrchr(name, '-');
    if (s != NULL) {
      s++;
    } else {
      s = strrchr(name, ':');
      if (s != NULL)
	s++;
      else
	s = name;
    }

    if (ODD(*sts = gdh_ObjidToPointer(oid, (pwr_tAddress *)&ap))) {
      pp = ini_ProcInsert(sts, cp, s, ap->ProgramName, ap->Load, ap->Run,
	ap->FileName, ap->JobPriority, ap->StartWithDebug, ap->Arg);
      pp->proc.flags.b.user = 1;
    }
  }
  
  f = ini_OpenFile(sts, cp, &cp->applfile);
  if (f != NULL) {    
    if (cp->flags.b.verbose) errh_LogInfo(&cp->log, "Reading Application file %s\n", cp->applfile.name);
    for (;;) {
      char *nl;

      s = fgets(buffer, sizeof(buffer) - 1, f);
      if (s == NULL) break;
      nl = strchr(s, '\n');
      if (nl != NULL) *nl = '\0';

      if (cp->flags.b.verbose) errh_LogInfo(&cp->log, "   %s", buffer);
      if (buffer[0] == '#') continue;
	
      do {
	int i_load = - 1;
	int i_run = - 1;
	int i_debug = -1;
	int i_prio = -1;
	char *id = NULL;
	char *name = NULL;
	char *load = NULL;
	char *run = NULL;
	char *file = NULL;
	char *prio = NULL;
	char *debug = NULL;
	char *arg = NULL;

	id	= strtok(s, ",");	if (id == NULL) break;
	name	= strtok(NULL, ",");	if (name == NULL) break;
	load	= strtok(NULL, ",");	if (load == NULL) break;
	run	= strtok(NULL, ",");	if (run == NULL) break;
	file	= strtok(NULL, ",");	if (file == NULL) break;
	prio	= strtok(NULL, ",");	if (prio == NULL) break;
	debug	= strtok(NULL, ",");	if (debug == NULL) break;
	arg	= strtok(NULL, ",");	if (arg == NULL) break;

	while (isspace(*id)) id++;
	while (isspace(*name)) name++;
	while (isspace(*load)) load++;
	while (isspace(*run)) run++;
	while (isspace(*file)) file++;
	while (isspace(*prio)) prio++;
	while (isspace(*debug)) debug++;
	while (isspace(*arg)) arg++;

	if (id[0] == '\0') break;
	if (strstr(load, "no"))
	  i_load = 0;
	else if (strstr(load, "load"))
	  i_load = 1;

	if (strstr(run, "no"))
	  i_run = 0;
	else if (strstr(run, "run"))
	  i_run = 1;

	if (strstr(debug, "no"))
	  i_debug = 0;
	else if (strstr(debug, "debug"))
	  i_debug = 1;

	if (strcspn(prio, "0123456789") > 0)
	  i_prio = -1;
	else
	  i_prio = atoi(prio);

	pp = ini_ProcInsert(sts, cp, id, name, i_load, i_run, file, i_prio, i_debug, arg);
	if (!pp->proc.flags.b.system && !pp->proc.flags.b.base)
	  pp->proc.flags.b.user = 1;
      } while (0);

    }
    fclose(f);
  }
}

void
ini_ProcWait (
  pwr_tStatus	*status,
  ini_sContext	*cp
)
{
  lst_sEntry	*pl;
  ini_sProc	*pp;
  pid_t		pid;
  pid_t		last_pid = 1;
  qcom_sGet get;
  int tmo = 1000;

  pwr_dStatus(sts, status, INI__SUCCESS);

#if defined OS_LYNX || defined OS_LINUX
  for (;;) {
    int status;

    get.data = NULL;
    qcom_Get(sts, &cp->eventQ, &get, tmo);
    if (*sts == QCOM__TMO || *sts == QCOM__QEMPTY) {
      errh_Info("Timeout");
    } else if (get.data == NULL) {
      errh_Info("Nulldata");
    } else {
      errh_Info("%d %s", get.size, get.data);
      qcom_Free(NULL, get.data);
    }
    if (lst_Succ(NULL, &cp->proc_lh, &pl) == NULL) break;
    pid = waitpid(-1, &status, WNOHANG|WUNTRACED);
    if (pid == 0) continue;
    if (pid == last_pid) break;
    
    for (pp = lst_Succ(NULL, &cp->proc_lh, &pl); pp != NULL; pp = lst_Succ(NULL, pl, &pl)) {
      if (pp->proc.pid == pid) {
	errh_Info("Process %s exited with status %d", pp->proc.name, status);
	break;
      }
    }
  }
#else
  sleep(100000);
#endif
  errh_Info("Ich sterbe!!");
}

static void
create_active_io ()
{
  pwr_tStatus sts;
  pwr_tObjid oid;

  sts = gdh_CreateObject("pwrNode-active", pwr_eClass_NodeHier, 0,
    &oid, pwr_cNObjid, 0, pwr_cNObjid);
  sts = gdh_CreateObject("pwrNode-active-io", pwr_eClass_NodeHier, 0,
    &oid, pwr_cNObjid, 0, pwr_cNObjid);
  sts = gdh_CreateObject("pwrNode-active-io-ai", pwr_cClass_AiArea,
    4000 * sizeof(((pwr_sClass_AiArea*)0)->Value[0]),
    &oid, pwr_cNObjid, 0, pwr_cNObjid);
  sts = gdh_CreateObject("pwrNode-active-io-ao", pwr_cClass_AoArea,
    4000 * sizeof(((pwr_sClass_AoArea*)0)->Value[0]),
    &oid, pwr_cNObjid, 0, pwr_cNObjid);
  sts = gdh_CreateObject("pwrNode-active-io-av", pwr_cClass_AvArea,
    4000 * sizeof(((pwr_sClass_AvArea*)0)->Value[0]),
    &oid, pwr_cNObjid, 0, pwr_cNObjid);
  sts = gdh_CreateObject("pwrNode-active-io-ca", pwr_cClass_CaArea,
    4000 * sizeof(((pwr_sClass_CaArea*)0)->Value[0]),
    &oid, pwr_cNObjid, 0, pwr_cNObjid);
  sts = gdh_CreateObject("pwrNode-active-io-co", pwr_cClass_CoArea,
    4000 * sizeof(((pwr_sClass_CoArea*)0)->Value[0]),
    &oid, pwr_cNObjid, 0, pwr_cNObjid);
  sts = gdh_CreateObject("pwrNode-active-io-di", pwr_cClass_DiArea,
    5000 * sizeof(((pwr_sClass_DiArea*)0)->Value[0]),
    &oid, pwr_cNObjid, 0, pwr_cNObjid);
  sts = gdh_CreateObject("pwrNode-active-io-do", pwr_cClass_DoArea,
    4000 * sizeof(((pwr_sClass_DoArea*)0)->Value[0]),
    &oid, pwr_cNObjid, 0, pwr_cNObjid);
  sts = gdh_CreateObject("pwrNode-active-io-dv", pwr_cClass_DvArea,
    4000 * sizeof(((pwr_sClass_DvArea*)0)->Value[0]),
    &oid, pwr_cNObjid, 0, pwr_cNObjid);
  sts = gdh_CreateObject("pwrNode-active-io-dv_init", pwr_cClass_IvArea,
    4000 * sizeof(((pwr_sClass_IvArea*)0)->Value[0]),
    &oid, pwr_cNObjid, 0, pwr_cNObjid);
  sts = gdh_CreateObject("pwrNode-active-io-av_init", pwr_cClass_IvArea,
    4000 * sizeof(((pwr_sClass_IvArea*)0)->Value[0]),
    &oid, pwr_cNObjid, 0, pwr_cNObjid);
}

static void
rename_active_io (
  ini_sContext *cp
)
{
  pwr_tStatus sts;
  pwr_tObjid oid;

  sts = gdh_NameToObjid("pwrNode-old", &oid);
  if (ODD(sts)) delete_old_io ();

  sts = gdh_NameToObjid("pwrNode-active", &oid);
  if (EVEN(sts)) {
    errh_LogError(&cp->log, "gdh_NameToObjid(pwrNode-active, &oid), %m", sts);
    return;
  }
  sts = gdh_RenameObject(oid, "old");
  if (EVEN(sts)) {
    errh_LogError(&cp->log, "gdh_RenameObject(oid, pwrNode-old), %m", sts);
    return;
  }
}

static void
delete_old_io ()
{
  pwr_tStatus sts;
  pwr_tObjid oid;

  sts = gdh_NameToObjid("pwrNode-old-io-ai", &oid);
  if(ODD(sts)) gdh_DeleteObject(oid);
  sts = gdh_NameToObjid("pwrNode-old-io-ao", &oid);
  if(ODD(sts)) gdh_DeleteObject(oid);
  sts = gdh_NameToObjid("pwrNode-old-io-av", &oid);
  if(ODD(sts)) gdh_DeleteObject(oid);
  sts = gdh_NameToObjid("pwrNode-old-io-ca", &oid);
  if(ODD(sts)) gdh_DeleteObject(oid);
  sts = gdh_NameToObjid("pwrNode-old-io-co", &oid);
  if(ODD(sts)) gdh_DeleteObject(oid);
  sts = gdh_NameToObjid("pwrNode-old-io-di", &oid);
  if(ODD(sts)) gdh_DeleteObject(oid);
  sts = gdh_NameToObjid("pwrNode-old-io-do", &oid);
  if(ODD(sts)) gdh_DeleteObject(oid);
  sts = gdh_NameToObjid("pwrNode-old-io-dv", &oid);
  if(ODD(sts)) gdh_DeleteObject(oid);
  sts = gdh_NameToObjid("pwrNode-old-io-dv_init", &oid);
  if(ODD(sts)) gdh_DeleteObject(oid);
  sts = gdh_NameToObjid("pwrNode-old-io-av_init", &oid);
  if(ODD(sts)) gdh_DeleteObject(oid);
  sts = gdh_NameToObjid("pwrNode-old-io", &oid);
  if(ODD(sts)) gdh_DeleteObject(oid);
  sts = gdh_NameToObjid("pwrNode-old", &oid);
  if(ODD(sts)) gdh_DeleteObject(oid);
}
