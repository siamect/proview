/* rt_gdb.c -- Global Database

   PROVIEW/R
   Copyright (C) 1996-98 by Mandator AB.

   .  */

#if defined(OS_ELN)
# include $vaxelnc
# include stdarg
# include stsdef
#else
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
#endif

#if defined (OS_LYNX)
# include <sys/mman.h>
#elif	defined(OS_LINUX)
# include <sys/file.h>
# include <sys/stat.h>
#endif

#include "pwr.h"
#include "pwr_class.h"
#include "rt_gdh_msg.h"
#include "rt_gdb_msg.h"
#include "rt_hash_msg.h"
#include "rt_errh.h"
#include "rt_gdb.h"
#include "rt_pool.h"
#include "rt_hash.h"
#include "rt_dl.h"
#include "rt_sub.h"
#include "rt_san.h"
#include "rt_net.h"

#if defined (OS_ELN)
#if 0
  noshare gdb_sLocal *gdbroot = NULL;
#endif
#else
  gdb_sLocal *gdbroot = NULL;
#endif


static void
evaluateInit (
  gdb_sInit		*ip
)
{

  ip->volumes	    = MAX(ip->volumes,	    gdb_cMin_volumes);
  ip->classes	    = MAX(ip->classes,	    gdb_cMin_classes);
  ip->nodes	    = MAX(ip->nodes,	    gdb_cMin_nodes);
  ip->ccvolumes	    = MAX(ip->ccvolumes,    gdb_cMin_ccvolumes);
  ip->cclasses	    = MAX(ip->cclasses,     gdb_cMin_cclasses);
  ip->mountServers  = MAX(ip->mountServers, gdb_cMin_mountServers);
  ip->aliasServers  = MAX(ip->aliasServers, gdb_cMin_aliasServers);
  ip->subServers    = MAX(ip->subServers,   gdb_cMin_subServers);
  ip->subClients    = MAX(ip->subClients,   gdb_cMin_subClients);
  ip->sanServers    = MAX(ip->sanServers,   gdb_cMin_sanServers);
  ip->cvol_max	    = MAX(ip->cvol_max,	    gdb_cMin_cvol_max);
  ip->cvol_min	    = MAX(ip->cvol_min,	    gdb_cMin_cvol_min);

  /* Allocate primary hash table with room for 1.5 times the number of
     objects in the load files.  */

  ip->objects += ip->cvol_max;
  ip->objects += ip->objects / 2;
  ip->objects = MAX(ip->objects, gdb_cMin_objects);


  /* The 'pool' pool contains object headers, subscription structures and
     other gdb internal structures. Make it at least 600k to begin with,
     or 1.5 times the size of the object headers we are initially populating
     the database with.
     The 'pool' pool will extend in segments of 1/8th of the initial segment,
     i.e. no segment will be smaller than 60k bytes. There is a limit
     on the number of pool segments of 256, so the worst case pool
     (a minimal system that need to extend the pool heavily) is
     600k + 255*60k = 15.9M.  */

  errh_Info("Objects      : %d : %d bytes", ip->objects, ip->objects * sizeof(gdb_sObject));
  errh_Info("Volumes      : %d : %d bytes", ip->volumes, ip->volumes * sizeof(gdb_sVolume));
  errh_Info("Classes      : %d : %d bytes", ip->classes, ip->classes * sizeof(gdb_sClass));
  errh_Info("Nodes        : %d : %d bytes", ip->nodes, ip->nodes * sizeof(gdb_sNode));
  errh_Info("Mount servers: %d : %d bytes", ip->mountServers, ip->mountServers * sizeof(gdb_sMountServer));
  errh_Info("Alias servers: %d : %d bytes", ip->aliasServers, ip->aliasServers * sizeof(gdb_sAliasServer));
  errh_Info("Sub servers  : %d : %d bytes", ip->subServers, ip->subServers * sizeof(sub_sServer));
  errh_Info("Sub clients  : %d : %d bytes", ip->subClients, ip->subClients * sizeof(sub_sClient));
  errh_Info("San servers  : %d : %d bytes", ip->sanServers, ip->sanServers * sizeof(san_sServer));
  errh_Info("Cache	  : %d", ip->cvol_max);

/** @todo Better values for cached classes and volumes to the initial pool */
  errh_Info("Cached classes: %d : %d bytes", ip->cclasses, ip->cclasses * (sizeof(gdb_sCclass) + 20 * sizeof(gdb_sCattribute)));
  errh_Info("Cached class volumes: %d : %d bytes", ip->ccvolumes, ip->ccvolumes * sizeof(gdb_sCclassVolume));


  ip->pool_isize =
    ip->objects	      * sizeof(gdb_sObject) +
    ip->volumes	      * sizeof(gdb_sVolume) +
    ip->classes	      * sizeof(gdb_sClass) +
    ip->nodes	      * sizeof(gdb_sNode) +
    ip->ccvolumes     * sizeof(gdb_sCclassVolume) +
    ip->cclasses      * (sizeof(gdb_sCclass) + 20 * sizeof(gdb_sCattribute)) +
    ip->mountServers  * sizeof(gdb_sMountServer) +
    ip->aliasServers  * sizeof(gdb_sAliasServer) +
    ip->subServers    * sizeof(sub_sServer) +
    ip->subClients    * sizeof(sub_sClient) +
    ip->sanServers    * sizeof(san_sServer);

  ip->pool_isize = MAX(ip->pool_isize, gdb_cMin_pool_isize);
  ip->pool_esize = ip->pool_isize / 8;

  errh_Info("Pool isize   : %d, esize: %d", ip->pool_isize, ip->pool_esize);

  /* The 'rtdb' pool contains object bodies, and the gdh_SubAssociateBuffer
     buffers. A good initial segment fits all object bodies we get
     populated with, plus some extra, or at least 600k.
     The extent is set to 1/8th of the size of the initial segment.  */

  ip->rtdb_isize += ip->rtdb_isize / 2;
  ip->rtdb_isize = MAX(ip->rtdb_isize, gdb_cMin_rtdb_isize);
  ip->rtdb_esize = ip->rtdb_isize / 8;

  errh_Info("Rtdb isize   : %d, esize: %d", ip->rtdb_isize, ip->rtdb_esize);

}

static gdb_sLocal *
mapLocalDb (
  pwr_tStatus		*sts
)
{

#if defined OS_LYNX || defined OS_LINUX
  pthread_mutexattr_t	mattr;
#endif

  gdb_AssumeLocked;

  if (gdbroot->db->version != gdb_cVersion)
    pwr_Return(NULL, sts, GDH__REVLEVEL);

#ifdef	OS_ELN
  ELN$CREATE_MUTEX(gdbroot->thread_lock, NULL);

#elif defined(OS_LINUX) || defined(OS_LYNX) && !defined(PWR_LYNX_30)
  pthread_mutexattr_init(&mattr);
  if (pthread_mutex_init(&gdbroot->thread_lock, &mattr) == -1) {
    perror("mapLocalDb: pthread_mutex_init, ");
    pwr_Return(NULL, sts, GDB__MUTEXINIT);
  }

#elif defined OS_LYNX
  pthread_mutexattr_create(&mattr);
  if (pthread_mutex_init(&gdbroot->thread_lock, mattr) == -1) {
    perror("mapLocalDb: pthread_mutex_init, ");
    pwr_Return(NULL, sts, GDB__MUTEXINIT);
  }
#endif

  /* Map hash tables.  */

  gdbroot->oid_ht = hash_Create(sts, gdbroot->pool, &gdbroot->h.oid_ht, &gdbroot->db->h.oid_ht, NULL, NULL);
  if (gdbroot->oid_ht == NULL) errh_Bugcheck(*sts, "initiating oid hash table");

  gdbroot->vid_ht = hash_Create(sts, gdbroot->pool, &gdbroot->h.vid_ht, &gdbroot->db->h.vid_ht, NULL, NULL);
  if (gdbroot->vid_ht == NULL) errh_Bugcheck(*sts, "initiating vid hash table");

  gdbroot->vn_ht = hash_Create(sts, gdbroot->pool, &gdbroot->h.vn_ht, &gdbroot->db->h.vn_ht, NULL, NULL);
  if (gdbroot->vn_ht == NULL) errh_Bugcheck(*sts, "initiating vn hash table");

  gdbroot->nid_ht = hash_Create(sts, gdbroot->pool, &gdbroot->h.nid_ht, &gdbroot->db->h.nid_ht, NULL, NULL);
  if (gdbroot->nid_ht == NULL) errh_Bugcheck(*sts, "initiating nid hash table");

  gdbroot->cid_ht = hash_Create(sts, gdbroot->pool, &gdbroot->h.cid_ht, &gdbroot->db->h.cid_ht, NULL, NULL);
  if (gdbroot->cid_ht == NULL) errh_Bugcheck(*sts, "initiating cid hash table");

  gdbroot->tid_ht = hash_Create(sts, gdbroot->pool, &gdbroot->h.tid_ht, &gdbroot->db->h.tid_ht, NULL, NULL);
  if (gdbroot->tid_ht == NULL) errh_Bugcheck(*sts, "initiating tid hash table");

  gdbroot->family_ht = hash_Create(sts, gdbroot->pool, &gdbroot->h.family_ht, &gdbroot->db->h.family_ht, NULL, NULL);
  if (gdbroot->family_ht == NULL) errh_Bugcheck(*sts, "initiating family hash table");

  gdbroot->ms_ht = hash_Create(sts, gdbroot->pool, &gdbroot->h.ms_ht, &gdbroot->db->h.ms_ht, NULL, NULL);
  if (gdbroot->ms_ht == NULL) errh_Bugcheck(*sts, "initiating mount server hash table");

  gdbroot->as_ht = hash_Create(sts, gdbroot->pool, &gdbroot->h.as_ht, &gdbroot->db->h.as_ht, NULL, NULL);
  if (gdbroot->as_ht == NULL) errh_Bugcheck(*sts, "initiating alias server hash table");

  gdbroot->subc_ht = hash_Create(sts, gdbroot->pool, &gdbroot->h.subc_ht, &gdbroot->db->h.subc_ht, NULL, NULL);
  if (gdbroot->subc_ht == NULL) errh_Bugcheck(*sts, "initiating subscription client hash table");

  gdbroot->subs_ht = hash_Create(sts, gdbroot->pool, &gdbroot->h.subs_ht, &gdbroot->db->h.subs_ht, NULL, NULL);
  if (gdbroot->subs_ht == NULL) errh_Bugcheck(*sts, "initiating subscription server hash table");

  gdbroot->sans_ht = hash_Create(sts, gdbroot->pool, &gdbroot->h.sans_ht, &gdbroot->db->h.sans_ht, NULL, NULL);
  if (gdbroot->sans_ht == NULL) errh_Bugcheck(*sts, "initiating subscribed alarm notification server hash table");

  gdbroot->ccvol_ht = hash_Create(sts, gdbroot->pool, &gdbroot->h.ccvol_ht, &gdbroot->db->h.ccvol_ht, NULL, NULL);
  if (gdbroot->ccvol_ht == NULL) errh_Bugcheck(*sts, "initiating cached class volume hash table");

  gdbroot->cclass_ht = hash_Create(sts, gdbroot->pool, &gdbroot->h.cclass_ht, &gdbroot->db->h.cclass_ht, NULL, NULL);
  if (gdbroot->cclass_ht == NULL) errh_Bugcheck(*sts, "initiating cached class hash table");


  if (offsetof(sub_sClient, sid) != offsetof(sub_sServer, sid))
    errh_Bugcheck(GDH__WEIRD, "offset id: client - server");
  if (offsetof(sub_sClient, sid) != offsetof(dl_sLink, dlid))
    errh_Bugcheck(GDH__WEIRD, "offset id: client - dlink");
  if (offsetof(sub_sClient, subc_htl) != offsetof(sub_sServer, subs_htl))
    errh_Bugcheck(GDH__WEIRD, "offset htl: client - server");
  if (offsetof(sub_sClient, subc_htl) != offsetof(dl_sLink, subc_htl))
    errh_Bugcheck(GDH__WEIRD, "offset htl: client - dlink");

  return gdbroot;  
}

#if defined OS_LYNX || defined OS_LINUX
/*
 * A fix which unlinks all segments for the given name.
 * I don't know where to put this routine, maybe it fits better in rt_pool.c.
 */
static void
unlinkPool(
  const char *name
){
  int 	 i;
  int    fd;                                              
  int    flags = O_RDWR;              
  mode_t mode  = S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP; 

  char   *str = getenv(pwr_dEnvBusId);
  char   segname[128];
  char	 busid[8];

  strncpy(busid, (str ? str : "XXX"), 3);
  busid[3] = '\0';

  sprintf(segname, "%s_%.3s", name, busid);

  /* This is the only way I know to find out if the memory is created, ML */
#if defined OS_LYNX
  fd = shm_open(segname, flags, mode); 
#else
  fd = open(segname, flags, mode); 
#endif
  if (fd != -1) { 
    close(fd);

#if defined OS_LYNX
      shm_unlink(segname);
#else
      unlink(segname);
#endif

    for (i = 1; TRUE; i++) {
      sprintf(segname, "%s%04d_%.3s", name, i, busid);
#if defined OS_LYNX
      fd = shm_open(segname, flags, mode); 
#else
      fd = open(segname, flags, mode); 
#endif

      if (fd == -1)
        break;

      close(fd);

#if defined OS_LYNX
      shm_unlink(segname);
#else
      unlink(segname);
#endif

    }
  }
}

#endif

gdb_sAliasServer *
gdb_AddAliasServer (
  pwr_tStatus		*sts,
  pwr_tObjid		soid,
  pwr_tBitMask		flags
)
{
  gdb_sAliasServer	*asp;
  gdb_sObject		*op = NULL;

  gdb_AssumeLocked;

  pwr_Assert(cdh_ObjidIsNotNull(soid));

  asp = hash_Search(sts, gdbroot->as_ht, &soid);
  if (asp != NULL) {
    if (flags & gdb_mAdd_failIfAdded) 
      pwr_Return(NULL, sts, GDB__DUPLADD);
    else
      pwr_Return(asp, sts, GDB__ALRADD);
  }

  asp = pool_Alloc(sts, gdbroot->pool, sizeof(*asp));
  if (asp == NULL) return NULL;

  asp->oid = soid;

  pool_Qinit(NULL, gdbroot->pool, &asp->as_htl);
  pool_Qinit(NULL, gdbroot->pool, &asp->as_ll);
  pool_Qinit(NULL, gdbroot->pool, &asp->cli_lh);

  asp = hash_Insert(sts, gdbroot->as_ht, asp);
  if (asp == NULL) errh_Bugcheck(GDH__WEIRD, "adding new alias server");

  pool_QinsertPred(NULL, gdbroot->pool, &asp->as_ll, &gdbroot->db->as_lh);

  op = hash_Search(sts, gdbroot->oid_ht, &soid);
  if (op == NULL) errh_Bugcheck(GDH__WEIRD, "getting alias server object");
  
  op->g.flags.b.isAliasServer = 1;

  return asp;
}

gdb_sClass *
gdb_AddClass (
  pwr_tStatus		*sts,
  pwr_tClassId		cid,
  pwr_tBitMask		flags
)
{
  gdb_sClass		*cp;

#if 0
  pwr_Assert(cid != pwr_cNClassId);
#endif

  gdb_AssumeLocked;

  cp = hash_Search(sts, gdbroot->cid_ht, &cid);
  if (cp != NULL) {
    if (flags & gdb_mAdd_failIfAdded)
      pwr_Return(NULL, sts, GDB__DUPLADD);
    else
      pwr_Return(cp, sts, GDB__ALRADD);
  }

  cp = pool_Alloc(sts, gdbroot->pool, sizeof(*cp));
  if (cp == NULL) return NULL;

  cp->cid = cid;

  pool_Qinit(NULL, gdbroot->pool, &cp->cid_htl);
  pool_Qinit(NULL, gdbroot->pool, &cp->cid_lh);
  pool_Qinit(NULL, gdbroot->pool, &cp->class_ll);

  cp = hash_Insert(sts, gdbroot->cid_ht, cp);
  if (cp == NULL) errh_Bugcheck(GDH__WEIRD, "adding new class");

  pool_QinsertPred(NULL, gdbroot->pool, &cp->class_ll, &gdbroot->db->class_lh);

  return cp;
}

gdb_sMountServer *
gdb_AddMountServer (
  pwr_tStatus		*sts,
  pwr_tObjid		soid,
  pwr_tBitMask		flags
)
{
  gdb_sMountServer	*msp;

  gdb_AssumeLocked;

  pwr_Assert(cdh_ObjidIsNotNull(soid));

  msp = hash_Search(sts, gdbroot->ms_ht, &soid);
  if (msp != NULL) {
    if (flags & gdb_mAdd_failIfAdded)
      pwr_Return(NULL, sts, GDB__DUPLADD);
    else
      pwr_Return(msp, sts, GDB__ALRADD);
  }

  msp = pool_Alloc(sts, gdbroot->pool, sizeof(*msp));
  if (msp == NULL) return NULL;

  msp->oid = soid;

  pool_Qinit(NULL, gdbroot->pool, &msp->ms_htl);
  pool_Qinit(NULL, gdbroot->pool, &msp->ms_ll);
  pool_Qinit(NULL, gdbroot->pool, &msp->nodms_ll);
  pool_Qinit(NULL, gdbroot->pool, &msp->volms_ll);
  pool_Qinit(NULL, gdbroot->pool, &msp->cli_lh);

  msp = hash_Insert(sts, gdbroot->ms_ht, msp);
  if (msp == NULL) errh_Bugcheck(GDH__WEIRD, "adding new mount server");

  pool_QinsertPred(NULL, gdbroot->pool, &msp->ms_ll, &gdbroot->db->ms_lh);

  return msp;
}

gdb_sNode *
gdb_AddNode (
  pwr_tStatus		*sts,
  pwr_tNodeId		nid,  
  pwr_tBitMask		flags
)
{
  gdb_sNode		*np;

  gdb_AssumeLocked;

  np = hash_Search(sts, gdbroot->nid_ht, &nid);
  if (np != NULL) return np;
  if (np != NULL) {
    if (flags & gdb_mAdd_failIfAdded)
      pwr_Return(NULL, sts, GDB__DUPLADD);
    else
      pwr_Return(np, sts, GDB__ALRADD);
  }

  np = pool_Alloc(sts, gdbroot->pool, sizeof(*np));
  if (np == NULL) return NULL;

  np->nid = nid;

  pool_Qinit(NULL, gdbroot->pool, &np->nid_htl);
  pool_Qinit(NULL, gdbroot->pool, &np->nod_ll);
  pool_Qinit(NULL, gdbroot->pool, &np->own_lh);
  pool_Qinit(NULL, gdbroot->pool, &np->ccvol_lh);
  pool_Qinit(NULL, gdbroot->pool, &np->nodms_lh);
  pool_Qinit(NULL, gdbroot->pool, &np->nodmo_lh);
  pool_Qinit(NULL, gdbroot->pool, &np->subc_lh);
  pool_Qinit(NULL, gdbroot->pool, &np->nodsubs_lh);
  pool_Qinit(NULL, gdbroot->pool, &np->nodsubb_lh);
  pool_Qinit(NULL, gdbroot->pool, &np->sansAct_lh);
  pool_Qinit(NULL, gdbroot->pool, &np->sansUpd_lh);
  pool_Qinit(NULL, gdbroot->pool, &np->sancAdd_lh);
  pool_Qinit(NULL, gdbroot->pool, &np->sancRem_lh);
  pool_Qinit(NULL, gdbroot->pool, &np->sancAct_lh);

  pool_Qinit(NULL, gdbroot->pool, &np->cacheNode.lh);
#if defined OS_ELN
  np->cacheNode.lc_max = 100;
#else
  np->cacheNode.lc_max = 200;
#endif

  np->cacheNode.flags.b.cacheNode = 1;
  np->cacheNode.next = pool_Reference(NULL, gdbroot->pool, &gdbroot->db->cacheCom);

  np = hash_Insert(sts, gdbroot->nid_ht, np);
  if (np == NULL) errh_Bugcheck(GDH__WEIRD, "adding new node");

  pool_QinsertPred(NULL, gdbroot->pool, &np->nod_ll, &gdbroot->db->nod_lh);

  return np;
}

/* Add a volume */

gdb_sVolume *
gdb_AddVolume (
  pwr_tStatus		*sts,
  pwr_tVolumeId		vid,
  pwr_tBitMask		flags
)
{
  gdb_sVolume		*vp;

  gdb_AssumeLocked;

  vp = hash_Search(sts, gdbroot->vid_ht, &vid);
  if (vp != NULL) {
    if (flags & gdb_mAdd_failIfAdded)
      pwr_Return(NULL, sts, GDB__DUPLADD);
    else
      pwr_Return(vp, sts, GDB__ALRADD);
  }

  vp = pool_Alloc(sts, gdbroot->pool, sizeof(*vp));
  if (vp == NULL) return NULL;

  vp->g.vid	= vid;
  vp->g.oid.vid	= vid;

  pool_Qinit(NULL, gdbroot->pool, &vp->l.vid_htl);
  pool_Qinit(NULL, gdbroot->pool, &vp->l.vn_htl);
  pool_Qinit(NULL, gdbroot->pool, &vp->l.vol_ll);
  pool_Qinit(NULL, gdbroot->pool, &vp->l.own_ll);
  pool_Qinit(NULL, gdbroot->pool, &vp->l.obj_lh);
  pool_Qinit(NULL, gdbroot->pool, &vp->l.volms_lh);

  hash_Insert(sts, gdbroot->vid_ht, vp);
  vp->l.flags.b.inVidTable = 1;

  pool_QinsertPred(sts, gdbroot->pool, &vp->l.vol_ll, &gdbroot->db->vol_lh);
  vp->l.flags.b.inVolList = 1;
  
  return vp;
}

/* Rebuild tables out of the database contents.
   This should be done after initialization of the DB, before
   starting the nethandler.  */

void
gdb_BuildGlobalDatabase (
  pwr_tObjid		systemobject,
  pwr_tObjid		nodeobject
)
{

#if 0
  gdhi_ScopeLock {

    /* Make sure the nethandler is not running.  */

    if (gdbroot->db->nethandler.all != 0) lib$signal(GDH__NODERUNNING);
    else {
      gdbroot->this_node->oid = nodeobject;

      gdh_load_fixup ();
    }

  } gdhi_ScopeUnlock;
#endif
}

/* This routine maps the object and node database.
   It should only be called by the init program.  */

gdb_sLocal *
gdb_CreateDb (
  pwr_tStatus		*sts,
  gdb_sInit		*ip
)
{
  pwr_tBoolean		created;
  gdb_sLocal		*lp;
  cdh_uRefId		rid;
  gdb_sTouchQ		*tqp;
  gdb_sObject		*op;
  int			i;

  /* Create job-local structure.  */

  pwr_Assert(sts != NULL);

  *sts = GDB__SUCCESS;

  gdbroot = calloc(1, sizeof(*gdbroot));
  if (gdbroot == NULL) errh_Bugcheck(GDH__INSVIRMEM, "initiating local database");

  gdbroot->my_pid = getpid();

  /* Create lock section.  */

  gdbroot->lock = sect_Alloc(sts, &created, &gdbroot->h.lock, sizeof(sect_sMutex), gdb_cNameDbLock);
  if (gdbroot->lock == NULL) errh_Bugcheck(*sts, "creating database lock");
  if (!created) errh_Bugcheck(GDH__WEIRD, "database lock was allready created");

  /* Create pools.  */

  evaluateInit(ip);

#if defined OS_LYNX || defined OS_LINUX
  unlinkPool(gdb_cNamePool);
  unlinkPool(gdb_cNameRtdb);
#endif

  gdbroot->pool = pool_Create(sts, &gdbroot->h.pool, gdb_cNamePool, ip->pool_isize, ip->pool_esize);
  if (gdbroot->pool == NULL) errh_Bugcheck(*sts, "initating pool");

  gdbroot->rtdb = pool_Create(sts, &gdbroot->h.rtdb, gdb_cNameRtdb, ip->rtdb_isize, ip->rtdb_esize);
  if (gdbroot->rtdb == NULL) errh_Bugcheck(*sts, "initiating rtdb");

  gdbroot->db = pool_AllocNamedSegment(sts, gdbroot->pool, sizeof(*gdbroot->db), gdb_cNameDatabase);
  if (gdbroot->db == NULL) errh_Bugcheck(*sts, "database directory");

  gdbroot->db->version = gdb_cVersion;

  gdbroot->db->eval_init = *ip;

  sect_InitLock(sts, gdbroot->lock, &gdbroot->db->lock);

#if 0
  pool_AllocLookasideSegment(sts, gdbroot->pool, ip->objects, sizeof(gdb_sObject));
#endif

  gdb_ScopeLock {

    /* Initiate all list headers.  */

    pool_Qinit(NULL, gdbroot->pool, &gdbroot->db->vol_lh);
    pool_Qinit(NULL, gdbroot->pool, &gdbroot->db->nod_lh);
    pool_Qinit(NULL, gdbroot->pool, &gdbroot->db->dl_lh);
    pool_Qinit(NULL, gdbroot->pool, &gdbroot->db->ms_lh);
    pool_Qinit(NULL, gdbroot->pool, &gdbroot->db->as_lh);
    pool_Qinit(NULL, gdbroot->pool, &gdbroot->db->class_lh);
    pool_Qinit(NULL, gdbroot->pool, &gdbroot->db->subt_lh);
    pool_Qinit(NULL, gdbroot->pool, &gdbroot->db->subs_lh);
    pool_Qinit(NULL, gdbroot->pool, &gdbroot->db->subb_lh);
    pool_Qinit(NULL, gdbroot->pool, &gdbroot->db->subm_lh);
    pool_Qinit(NULL, gdbroot->pool, &gdbroot->db->tmonq_lh);

    gdbroot->db->tmotime = 10;	/* 1 second */
    gdbroot->db->tmolap  = 100;	/* 10 seconds */


    /* Set up our node identity.  */

    gdbroot->db->nid	      = ip->nid;
    gdbroot->db->nod_oid      = ip->nod_oid;
    gdbroot->db->vid	      = ip->nid;
    gdbroot->db->vol_oid      = pwr_cNObjid;
    gdbroot->db->vol_oid.vid  = ip->nid;

    /* Initiate subscription identity.  */

    rid.pwr.nid = gdbroot->db->nid;
    rid.r.vid_3 = cdh_eVid3_subid;
    rid.pwr.rix = 1;

    gdbroot->db->subcid = rid.pwr;
    gdbroot->db->sancid = rid.pwr;

    /* Initiate direct link identity.  */

    rid.pwr.nid = gdbroot->db->nid;
    rid.r.vid_3 = cdh_eVid3_dlid;
    rid.pwr.rix = 1;

    gdbroot->db->dlid = rid.pwr;

    /* Hash tables.  */

    hash_Init(&gdbroot->db->h.oid_ht, ip->objects, sizeof(pwr_tObjid), sizeof(gdb_sObject),
      offsetof(gdb_sObject, g.oid), offsetof(gdb_sObject, l.oid_htl), hash_eKey_oid);

    hash_Init(&gdbroot->db->h.vid_ht, ip->volumes, sizeof(pwr_tVolumeId), sizeof(gdb_sVolume),
      offsetof(gdb_sVolume, g.vid), offsetof(gdb_sVolume, l.vid_htl), hash_eKey_vid);

    hash_Init(&gdbroot->db->h.vn_ht, ip->volumes, sizeof(cdh_sObjName), sizeof(gdb_sVolume),
      offsetof(gdb_sVolume, g.name), offsetof(gdb_sVolume, l.vn_htl), hash_eKey_objName);

    hash_Init(&gdbroot->db->h.cid_ht, ip->classes, sizeof(pwr_tClassId), sizeof(gdb_sClass),
      offsetof(gdb_sClass, cid), offsetof(gdb_sClass, cid_htl), hash_eKey_cid);

    hash_Init(&gdbroot->db->h.nid_ht, ip->nodes, sizeof(pwr_tNodeId), sizeof(gdb_sNode),
      offsetof(gdb_sNode, nid), offsetof(gdb_sNode, nid_htl), hash_eKey_nid);

    hash_Init(&gdbroot->db->h.family_ht, ip->objects, sizeof(cdh_sFamily), sizeof(gdb_sObject),
      offsetof(gdb_sObject, g.f), offsetof(gdb_sObject, l.family_htl), hash_eKey_family);

    hash_Init(&gdbroot->db->h.ms_ht, ip->mountServers, sizeof(pwr_tObjid), sizeof(gdb_sMountServer),
      offsetof(gdb_sMountServer, oid), offsetof(gdb_sMountServer, ms_htl), hash_eKey_oid);

    hash_Init(&gdbroot->db->h.as_ht, ip->aliasServers, sizeof(pwr_tObjid), sizeof(gdb_sAliasServer),
      offsetof(gdb_sAliasServer, oid), offsetof(gdb_sAliasServer, as_htl), hash_eKey_oid);

    hash_Init(&gdbroot->db->h.subc_ht, ip->subClients, sizeof(pwr_tSubid), sizeof(sub_sClient),
      offsetof(sub_sClient, sid), offsetof(sub_sClient, subc_htl), hash_eKey_oid);

    hash_Init(&gdbroot->db->h.subs_ht, ip->subServers, sizeof(pwr_tSubid), sizeof(sub_sServer),
      offsetof(sub_sServer, sid), offsetof(sub_sServer, subs_htl), hash_eKey_oid);

    hash_Init(&gdbroot->db->h.sans_ht, ip->sanServers, sizeof(pwr_tSubid), sizeof(san_sServer),
      offsetof(san_sServer, sane.sid), offsetof(san_sServer, sans_htl), hash_eKey_oid);

    hash_Init(&gdbroot->db->h.ccvol_ht, ip->ccvolumes, sizeof(pwr_tObjid), sizeof(gdb_sCclassVolume),
      offsetof(gdb_sCclassVolume, key), offsetof(gdb_sCclassVolume, ccvol_htl), hash_eKey_oid);

    hash_Init(&gdbroot->db->h.cclass_ht, ip->cclasses, sizeof(gdb_sCclassKey), sizeof(gdb_sCclass),
      offsetof(gdb_sCclass, key), offsetof(gdb_sCclass, cclass_htl), hash_eKey_memcmp);

    lp = mapLocalDb(sts);
    if (lp == NULL) break;

#if 0
    /* Start communications.  */

    gdbroot->my_qid = qcom_cNQid;
    net_Connect(sts, &gdbroot->my_aid, &gdbroot->my_qid, NULL);
    if (EVEN(*sts)) break;

    if ((gdbroot->my_qid.nid == 0) || (gdbroot->my_qid.qix == 0)) {
      *sts = GDH__QCOM;
      break;
    }
#endif

    gdbroot->my_volume = gdb_AddVolume(sts, gdbroot->db->vid, gdb_mAdd_failIfAdded);
    if (gdbroot->my_volume == NULL) errh_Bugcheck(*sts, "creating root volume");

    gdbroot->no_volume = gdb_AddVolume(sts, pwr_cNVolumeId, gdb_mAdd_failIfAdded);
    if (gdbroot->no_volume == NULL) errh_Bugcheck(*sts, "creating the null volume");

    gdbroot->my_node = gdb_AddNode(sts, gdbroot->db->nid, gdb_mAdd_failIfAdded);
    if (gdbroot->my_node == NULL) errh_Bugcheck(*sts, "creating local node");
    gdbroot->my_node->nod_oid = gdbroot->db->nod_oid;

    gdbroot->no_node = gdb_AddNode(sts, 0, gdb_mAdd_failIfAdded);
    if (gdbroot->no_node == NULL) errh_Bugcheck(*sts, "creating the unknown node");
    strcpy(gdbroot->no_node->name, "******");

    /* Touch queues.  */

    tqp = &gdbroot->db->cacheCom;
    pool_Qinit(NULL, gdbroot->pool, &tqp->lh);
#if defined OS_ELN
    tqp->lc_max = 100;
#else
    tqp->lc_max = 2000;
#endif
    tqp->flags.b.cacheCom = 1;
    tqp->next = pool_Reference(NULL, gdbroot->pool, &gdbroot->db->cacheOld);

    tqp = &gdbroot->db->cacheNew;
    pool_Qinit(NULL, gdbroot->pool, &tqp->lh);
#if defined OS_ELN
    tqp->lc_max = 100;
#else
    tqp->lc_max = 400;
#endif
    tqp->flags.b.cacheNew = 1;
    tqp->next = pool_Reference(NULL, gdbroot->pool, &gdbroot->db->cacheOld);

    tqp = &gdbroot->db->cachePend;
    pool_Qinit(NULL, gdbroot->pool, &tqp->lh);
    tqp->lc_max = 100;
    tqp->flags.b.cachePend = 1;
    tqp->next = pool_cNRef;

    tqp = &gdbroot->db->cacheOld;
    pool_Qinit(NULL, gdbroot->pool, &tqp->lh);
    tqp->lc_max = 100;
    tqp->flags.b.cacheOld = 1;
    tqp->next = pool_cNRef;

    tqp = &gdbroot->db->cacheFree;
    pool_Qinit(NULL, gdbroot->pool, &tqp->lh);
    tqp->lc_max = ip->cvol_max;
    tqp->lc_min = 100;
    tqp->flags.b.cacheFree = 1;
    tqp->next = pool_cNRef;
    
    for (i = 0; i < tqp->lc_max; i++) {
      op = pool_Alloc(NULL, gdbroot->pool, sizeof(*op));
      pool_QinsertPred(NULL, gdbroot->pool, &op->u.c.cache_ll, &gdbroot->db->cacheFree.lh);
      op->u.c.flags.b.cacheFree = 1;
    }

    gdbroot->db->cacheFree.lc = tqp->lc_max;



    tqp = &gdbroot->db->cacheCclass;
    pool_Qinit(NULL, gdbroot->pool, &tqp->lh);
#if defined OS_ELN
    tqp->lc_max = 100;
#else
    tqp->lc_max = 1000; /** @todo What is the max number of cached classes? */
#endif
    tqp->flags.m = 0;
    tqp->next = pool_cNRef;


  } gdb_ScopeUnlock;

  if (EVEN(*sts) || lp == NULL)
    gdbroot = NULL;

  return gdbroot;
}

/* Allocate an object header and initiate
   the global part of the header.  */

gdb_sObject *
gdb_AddObject (
  pwr_tStatus		*sts,
  const char		*name,
  pwr_tObjid		oid,
  pwr_tClassId		cid,
  pwr_tUInt32		size,
  pwr_tObjid		poid,
  pwr_tBitMask		iflags,
  pwr_tObjid		soid
)
{
  gdb_sObject		*op;  
  net_sGobject		*gop;

  op = pool_Alloc(sts, gdbroot->pool, sizeof(*op));
  if (op == NULL) return NULL;

  gop = &op->g;
  gop->oid = oid;
  gop->cid = cid;
  cdh_Family(&gop->f, name, poid);
  gop->soid = soid;
  gop->flags.m = iflags;
  if (!gop->flags.b.isAliasClient)
    gop->size = size;
  else
    gop->size = 0;
    
  return op;
}

/* Load a volume */

gdb_sVolume *
gdb_LoadVolume (
  pwr_tStatus		*sts,
  pwr_tVolumeId		vid,
  const char		*name,
  pwr_tClassId		cid,
  pwr_tNodeId		nid,
  pwr_tTime		time,
  pwr_tBitMask		iload,
  const co_mFormat      *format
)
{
  gdb_sVolume		*vp;
  pwr_tObjName		volname;
  gdb_mLoad		load;
  cdh_uVolumeId		lvid;
  char			string[256];

  load.m = iload;

  gdb_AssumeLocked;

  vp = gdb_AddVolume(sts, vid, gdb_mAdd__);
  if (vp == NULL) return NULL;

  vp->g.cid	= cid;
  if (name == NULL || *name == '\0') {
    lvid.pwr = vid;
    sprintf(volname, "%u_%u_%u_%u", lvid.v.vid_3, lvid.v.vid_2, lvid.v.vid_1, lvid.v.vid_0);
    name = volname;
  }
  cdh_ObjName(&vp->g.name, name);
  vp = hash_Insert(sts, gdbroot->vn_ht, vp);
  sprintf(string, "adding volume: %s (%s)", cdh_VolumeIdToString(NULL, vid, 0, 0), name);
  if (vp == NULL) errh_Bugcheck(*sts, string);

  vp->g.time = time;
  

  /* Local flags.  */
  
#if 0
  vp->l.flags.b.isMounted	= 0;
#endif
  vp->l.flags.b.isConnected	= 0;
  vp->l.flags.b.transAlias	= 1;
  vp->l.flags.b.transMount	= vid == gdbroot->db->vid;

  vp->l.flags.b.inVidTable	= 1;
  vp->l.flags.b.inVnTable	= 0;
  vp->l.flags.b.inVolList	= 1;
  vp->l.flags.b.inOwnList	= 0;

  vp->l.flags.b.isNative	= load.b.native;
  vp->l.flags.b.isCached	= load.b.netCached | load.b.fileCached;
  vp->l.flags.b.isLoaded	= load.b.loaded;

  vp->l.flags.b.privat		= load.b.privat;
  vp->l.flags.b.root		= vid == gdbroot->db->vid;
  vp->l.flags.b.sub		= load.b.native && cid == pwr_eClass_SubVolume;
  vp->l.flags.b.system		= load.b.native && cid == pwr_eClass_SystemVolume;
  vp->l.flags.b.dynamic		= vp->l.flags.b.system | (load.b.native && cid == pwr_eClass_DynamicVolume);
  vp->l.flags.b.shared		= cid == pwr_eClass_DynamicVolume;
  vp->l.flags.b.classvol	= cid == pwr_eClass_ClassVolume;
  vp->l.flags.b.netCached	= load.b.netCached;
  vp->l.flags.b.fileCached	= load.b.fileCached;
  vp->l.flags.b.remote		= load.b.netCached | load.b.fileCached;

  vp->l.flags.b.isOwned		= nid == gdbroot->db->nid && (vp->l.flags.b.root | vp->l.flags.b.sub
				| vp->l.flags.b.system | vp->l.flags.b.dynamic |vp->l.flags.b.classvol);

  if (vp->l.flags.b.isOwned) {
    vp->g.nid = nid;

    if (vp->l.flags.b.classvol) 
        pool_QinsertSucc(NULL, gdbroot->pool, &vp->l.own_ll, &gdbroot->my_node->own_lh);
    else
        pool_QinsertPred(NULL, gdbroot->pool, &vp->l.own_ll, &gdbroot->my_node->own_lh);
    vp->l.flags.b.inOwnList = 1;
  }
  

  /* Native/Cache part.  */

  if (vp->l.flags.b.isNative) {
    pool_Qinit(NULL, gdbroot->pool, &vp->u.n.volmo_lh);
    vp->u.n.next_oid.vid = vid;
    vp->u.n.format = *format;
    if (cid != pwr_eClass_DynamicVolume)
      vp->u.n.next_oid.oix = 0x80000000;
  } else if (vp->l.flags.b.isCached) {
  }

  return vp;
}

/* Lock an object for direct linking.  */

gdb_sObject *
gdb_LockObject (
  pwr_tStatus		*sts,
  gdb_sObject		*op
)
{

  gdb_AssumeLocked;

  pwr_Assert(op->l.flags.b.isNative);

  op->u.n.dlcount++;

  return op;
}

gdb_sLocal *
gdb_MapDb (
  pwr_tStatus		*sts,
  qcom_sQid		*qid,
  const char		*name
)
{
  pwr_tStatus		lsts;
  pwr_tBoolean		created;
  gdb_sLocal		*lp;
  pwr_tNodeId		no_nid = pwr_cNNodeId;
  pwr_tVolumeId		no_vid = pwr_cNVolumeId;

  gdbroot = calloc(1, sizeof(*gdbroot));
  if (gdbroot == NULL) errh_Bugcheck(GDH__INSVIRMEM, "initiating local database");

  gdbroot->my_pid = getpid();

  /* Map lock sections.  */

  gdbroot->lock = sect_Alloc(sts, &created, &gdbroot->h.lock, sizeof(sect_sMutex), gdb_cNameDbLock);
  if (gdbroot->lock == NULL) errh_Bugcheck(*sts, "mapping db lock");
  if (created) {
    sect_Free(&lsts, gdbroot->lock); 
    pwr_Return(NULL, sts, GDH__RTNOTSTARTED);
  }

  /* Map 'pool' & 'rtdb' pools.  */

  gdbroot->pool = pool_Create(sts, &gdbroot->h.pool, gdb_cNamePool, 0, 0);
  if (gdbroot->pool == NULL) errh_Bugcheck(*sts, "initating pool");

  gdbroot->rtdb = pool_Create(sts, &gdbroot->h.rtdb, gdb_cNameRtdb, 0, 0);
  if (gdbroot->rtdb == NULL) errh_Bugcheck(*sts, "initiating rtdb");

  gdbroot->db = pool_AllocNamedSegment(sts, gdbroot->pool, sizeof(*gdbroot->db), gdb_cNameDatabase);
  if (gdbroot->db == NULL) errh_Bugcheck(*sts, "database directory");

  if (qid != NULL && qcom_QidIsNotNull(qid))
    gdbroot->my_qid = *qid;

  gdb_ScopeLock {

    lp = mapLocalDb(sts);
    if (lp == NULL) return NULL;
       
    /* Start communications.  */

    net_Connect(&lsts, &gdbroot->my_aid, &gdbroot->my_qid, NULL, name);
    if (EVEN(lsts)) pwr_Return(NULL, sts, lsts);

    if ((gdbroot->my_qid.nid == 0) || (gdbroot->my_qid.qix == 0))
      pwr_Return(NULL, sts, GDH__QCOM);
      
    gdbroot->my_volume	= hash_Search(sts, gdbroot->vid_ht, &gdbroot->db->vid);
    gdbroot->no_volume	= hash_Search(sts, gdbroot->vid_ht, &no_vid);
    gdbroot->my_node	= hash_Search(sts, gdbroot->nid_ht, &gdbroot->db->nid);
    gdbroot->no_node	= hash_Search(sts, gdbroot->nid_ht, &no_nid);

  } gdb_ScopeUnlock;

  if (qid != NULL) *qid = gdbroot->my_qid;

  return gdbroot;
}

gdb_sClass *
gdb_ReAddClass (
  pwr_tStatus		*sts,
  gdb_sClass		*cp,
  unsigned int		acount  
)
{
  gdb_sClass		*new_cp;

  gdb_AssumeLocked;

  new_cp = pool_Alloc(sts, gdbroot->pool, sizeof(*cp) + (acount == 0 ? 0 : acount - 1) * sizeof(gdb_sAttribute));
  if (new_cp == NULL) return NULL;

  memcpy(new_cp, cp, sizeof(*cp));
  
  pool_Qinit(NULL, gdbroot->pool, &new_cp->cid_htl);
  pool_Qinit(NULL, gdbroot->pool, &new_cp->cid_lh);
  pool_Qinit(NULL, gdbroot->pool, &new_cp->class_ll);

  hash_Remove(sts, gdbroot->cid_ht, cp);
  new_cp = hash_Insert(sts, gdbroot->cid_ht, new_cp);
  if (new_cp == NULL) errh_Bugcheck(GDH__WEIRD, "adding new class");

  pool_Qremove(NULL, gdbroot->pool, &cp->class_ll);
  pool_QinsertSucc(NULL, gdbroot->pool, &new_cp->class_ll, &gdbroot->db->class_lh);

  pool_Qmove(sts, gdbroot->pool, &cp->cid_lh, &new_cp->cid_lh);
  pool_Free(NULL, gdbroot->pool, cp);

  return new_cp;
}

/* Remove an object.
   If object is direct liked postpone the
   remove and set the pending delete flag.  */

void
gdb_RemoveObject (
  pwr_tStatus	*sts,
  gdb_sObject	*op
)
{

  gdb_AssumeLocked;

  pwr_Assert(op != NULL);
  pwr_Assert(op->l.flags.b.isNative);
  pwr_Assert(pool_QisUnlinked(&op->l.obj_ll));
  pwr_Assert(pool_QisUnlinked(&op->l.oid_htl));
  pwr_Assert(pool_QisUnlinked(&op->l.family_htl));
  pwr_Assert(pool_QisUnlinked(&op->u.n.cid_ll));
  pwr_Assert(pool_QisUnlinked(&op->u.n.cli_ll));
  pwr_Assert(pool_QisUnlinked(&op->u.n.sib_ll));

  if (op->u.n.dlcount == 0 && !pool_QisLinked(NULL, gdbroot->pool, &op->u.n.sib_lh)) {
    if (op->u.n.body != pool_cNRef) {
      pwr_Assert(op->g.size > 0);
      pool_FreeReference(sts, gdbroot->rtdb, op->u.n.body);
    }
    pool_Free(NULL, gdbroot->pool, op);
  } else {
    op->u.n.flags.b.pendingDelete = 1;
  }
}

/* Unlock an object for direct linking.
   The OH becomes non-directlinked. If dlcount becomes zero,
   then the object is `unlocked'. If the DELPEND flag is set,
   then this OH is deleted.
   The database should be locked when this routine is called.
   
   Delete the object and its body.
   When the 'PendingDelete' flag is set, we now
   (for a local object) that the object is removed
   from all links.  */

pwr_tBoolean
gdb_UnlockObject (
  pwr_tStatus		*sts,
  gdb_sObject		*op
)
{

  gdb_AssumeLocked;

  pwr_Assert(op->l.flags.b.isNative);

  if (--op->u.n.dlcount == 0 && op->u.n.flags.b.pendingDelete)
    gdb_RemoveObject(sts, op);

  return YES;
}
