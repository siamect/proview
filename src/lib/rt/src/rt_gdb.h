#ifndef rt_gdb_h
#define rt_gdb_h

/* rt_gdb.h -- Global Database internal stuff

   PROVIEW/R
   Copyright (C) 1996 by Comator Process AB.

   This include file contains the internal datastructures and
   data entities in GDH.  */

#if defined(OS_VMS) || defined(OS_ELN)
#pragma builtins
#endif

#ifdef	OS_ELN
# include $vaxelnc
# include $mutex
# include unistd
#elif defined OS_LYNX || defined OS_LINUX
# include <pthread.h>
#endif

#ifndef	OS_ELN
#include <unistd.h>
#endif

#ifndef rt_gdb_msg_h
# include "rt_gdb_msg.h"
#endif

#ifndef pwr_h
# include "pwr.h"
#endif

#ifndef pwr_class_h
# include "pwr_class.h"
#endif

#ifndef co_dbs_h
#include "co_dbs.h"
#endif

#ifndef rt_qcom_h
#include "rt_qcom.h"
#endif

#ifndef co_cdh_h
#include "co_cdh.h"
#endif

#ifndef rt_errh_h
#include "rt_errh.h"
#endif

#ifndef rt_sect_h
#include "rt_sect.h"
#endif

#ifndef rt_pool_h
#include "rt_pool.h"
#endif

#ifndef rt_hash_h
#include "rt_hash.h"
#endif

#ifndef rt_net_h
#include "rt_net.h"
#endif

/* Version of the runtime database datastructures. Changing this
   makes gdh_Init bugcheck if the structure versions
   does not match!  */

#define	gdb_cVersion 7

#if defined OS_VMS || defined OS_ELN
#  define	gdb_cNameDatabase	"pwr_gdb"
#  define	gdb_cNamePool		"pwr_pool"
#  define	gdb_cNameRtdb		"pwr_rtdb"
#  define	gdb_cNameDbLock		"pwr_db_lock"
#elif defined OS_LYNX
#  define	gdb_cNameDatabase	"/pwr_gdb"
#  define	gdb_cNamePool		"/pwr_pool"
#  define	gdb_cNameRtdb		"/pwr_rtdb"
#  define	gdb_cNameDbLock		"/pwr_db_lock"
#elif defined OS_LINUX
#  define	gdb_cNameDatabase	"/tmp/pwr_gdb"
#  define	gdb_cNamePool		"/tmp/pwr_pool"
#  define	gdb_cNameRtdb		"/tmp/pwr_rtdb"
#  define	gdb_cNameDbLock		"/tmp/pwr_db_lock"
#endif

#define gdb_cMin_objects	3000
#define gdb_cMin_volumes	30
#define gdb_cMin_classes	300
#define gdb_cMin_nodes		10
#define gdb_cMin_mountServers	100
#define gdb_cMin_aliasServers	50

/** @todo Set better initial values */
# define gdb_cMin_ccvolumes	150
# define gdb_cMin_cclasses	300
# define gdb_cMin_scObjects	300


#if defined(OS_ELN)
# define gdb_cMin_cvol_max	200
# define gdb_cMin_cvol_min	100
# define gdb_cMin_subServers	200
# define gdb_cMin_subClients	2000
# define gdb_cMin_sanServers	200
#elif defined(OS_VMS)
# define gdb_cMin_cvol_max	2000
# define gdb_cMin_cvol_min	1900
# define gdb_cMin_subServers	500
# define gdb_cMin_subClients	2000
# define gdb_cMin_sanServers	200
#elif defined(OS_LINUX)
# define gdb_cMin_cvol_max	2000
# define gdb_cMin_cvol_min	1900
# define gdb_cMin_subServers	500
# define gdb_cMin_subClients	2000
# define gdb_cMin_sanServers	200
#elif defined (OS_LYNX)
# define gdb_cMin_cvol_max	500
# define gdb_cMin_cvol_min	400
# define gdb_cMin_subServers	500
# define gdb_cMin_subClients	100
# define gdb_cMin_sanServers	200
#endif

#define gdb_cMin_pool_isize	600000
#define gdb_cMin_rtdb_isize	600000


/* General database lock to keep consistency.
 
   After a 'gdb_Lock', the database is guaranteed to be locked, regardless
   of the previous locking state. After a 'gdb_Unlock' the database is
   unlocked, also regardless of the previous state!  */

#define	gdb_LockOwned (gdbroot->db->lock_owner == gdbroot->my_pid)
#define	gdb_ExclOwned (gdbroot->db->excl_owner == gdbroot->my_pid)

#define	gdb_Lock\
	{\
	  if (gdb_LockOwned) {\
	    if (gdb_ExclOwned)\
	      gdbroot->lock_count++;\
	    else\
	      errh_Bugcheck(GDB__LOCKCHECK, "gdb_Lock: lock was allready taken");\
	  } else {\
	    sect_Lock(NULL, gdbroot->lock, &gdbroot->db->lock);\
	    gdbroot->db->lock_owner = gdbroot->my_pid;\
	  }\
	}

#define	gdb_Excl\
	{\
	  pwr_Assert(!gdb_ExclOwned && !gdb_LockOwned && gdbroot->lock_count == 0);\
	  sect_Lock(NULL, gdbroot->lock,&gdbroot->db->lock);\
	  gdbroot->lock_count++;\
	  gdbroot->db->lock_owner = gdbroot->my_pid;\
	  gdbroot->db->excl_owner = gdbroot->my_pid;\
	}

#define gdb_Unlock\
	{\
	  pwr_Assert(gdb_LockOwned);\
	  if (gdb_ExclOwned) {\
	    pwr_Assert(gdbroot->lock_count > 1);\
	    gdbroot->lock_count--;\
	  } else {\
	    pwr_Assert(gdbroot->lock_count == 0);\
	    gdbroot->db->lock_owner = 0;\
	    sect_Unlock(NULL, gdbroot->lock, &gdbroot->db->lock);\
	  }\
	}

#define gdb_Unexcl\
	{\
	  pwr_Assert(gdb_LockOwned && gdb_ExclOwned && gdbroot->lock_count == 1);\
	  gdbroot->lock_count--;\
	  gdbroot->db->lock_owner = 0;\
	  gdbroot->db->excl_owner = 0;\
	  sect_Unlock(NULL, gdbroot->lock, &gdbroot->db->lock);\
	}

#define	gdb_AssumeLocked pwr_Assert(gdb_LockOwned)
#define	gdb_AssumeExcled pwr_Assert(gdb_LockOwned && gdb_ExclOwned)
#define	gdb_AssumeUnlocked pwr_Assert(!gdb_LockOwned)
#define	gdb_AssumeUnexcled pwr_Assert(!gdb_LockOwned && !gdb_ExclOwned)
#define gdb_ScopeLock	gdb_Lock;do
#define gdb_ScopeUnlock	while(0);gdb_Unlock
#define gdb_ScopeExcl	gdb_Excl;do
#define gdb_ScopeUnexcl	while(0);gdb_Unexcl

/* What is changed in an object. Used when reloading the database. */

typedef union {
  pwr_tBitMask m;
  pwr_32Bits (
    pwr_Bits( created	, 1),
    pwr_Bits( deleted	, 1),
    pwr_Bits( father	, 1),
    pwr_Bits( name	, 1),
    pwr_Bits( body	, 1),
    pwr_Bits( server	, 1),
    pwr_Bits( flags	, 1),
    pwr_Bits( classid	, 1),

    pwr_Bits( size	, 1),
    pwr_Bits( fill1	, 7),,,,,,,
    pwr_Bits( fill2	, 8),,,,,,,,
    pwr_Bits( fill3	, 8),,,,,,,
  ) b;

# define gdb_mChange__	(0)
# define gdb_mChange_created	pwr_Bit(0)
# define gdb_mChange_deleted	pwr_Bit(1)
# define gdb_mChange_father	pwr_Bit(2)
# define gdb_mChange_name	pwr_Bit(3)
# define gdb_mChange_body	pwr_Bit(4)
# define gdb_mChange_server	pwr_Bit(5)
# define gdb_mChange_flags	pwr_Bit(6)
# define gdb_mChange_class	pwr_Bit(7)
# define gdb_mChange_size	pwr_Bit(8)
# define gdb_mChange_head	(gdb_mChange_created|gdb_mChange_father|gdb_mChange_name|gdb_mChange_body|\
				 gdb_mChange_server|gdb_mChange_flags|gdb_mChange_class|gdb_mChange_size)
# define gdb_mChange_family	(gdb_mChange_father|gdb_mChange_name)
# define gdb_mChange_		(~gdb_mChange__)

} gdb_mChange;


/* Object: Cached part.  */

typedef union {
  pwr_tBitMask m;
  pwr_32Bits (
    pwr_Bits( cacheLock	, 1),
    pwr_Bits( cacheVol	, 1),
    pwr_Bits( cacheNode	, 1),
    pwr_Bits( cacheCom	, 1),
    pwr_Bits( cacheNew	, 1),
    pwr_Bits( cacheOld	, 1),
    pwr_Bits( cachePend	, 1),
    pwr_Bits( cacheFree	, 1),

    pwr_Bits( isParent	, 1),
    pwr_Bits( classChecked, 1), /**< set if we have checked if it has the same class version */
    pwr_Bits( classEqual, 1),   /**< set if native and remote class is equal */ 
    pwr_Bits( fill_1	, 5),,,,,

    pwr_Bits( sancAdd	, 1),
    pwr_Bits( sancAct	, 1),
    pwr_Bits( sancRem	, 1),
    pwr_Bits( fill_2	, 5),,,,,

    pwr_Bits( fill_3	, 8),,,,,,,
  ) b;

#define gdb_mCo__		0
#define gdb_mCo_cacheLock	pwr_Bit(0)
#define gdb_mCo_cacheVol	pwr_Bit(1)
#define gdb_mCo_cacheNode	pwr_Bit(2)
#define gdb_mCo_cacheCom	pwr_Bit(3)
#define gdb_mCo_cacheNew	pwr_Bit(4)
#define gdb_mCo_cacheOld	pwr_Bit(5)
#define gdb_mCo_cachePend	pwr_Bit(6)
#define gdb_mCo_cacheFree	pwr_Bit(7)

#define gdb_mCo_isParent	pwr_Bit(8) /**< Is probably not needed!  */
#define gdb_mCo_classChecked	pwr_Bit(9)	
#define gdb_mCo_classEqual	pwr_Bit(10)	

#define gdb_mCo_sancAdd		pwr_Bit(16)
#define gdb_mCo_sancAct		pwr_Bit(17)
#define gdb_mCo_sancRem		pwr_Bit(18)
#define gdb_mCo_		(~gdb_mCo__)

#define gdb_mCo_inTouchList	(gdb_mCo_cacheLock|gdb_mCo_cacheVol |gdb_mCo_cacheNode|gdb_mCo_cacheCom|\
				 gdb_mCo_cacheNew |gdb_mCo_cacheOld |gdb_mCo_cachePend|gdb_mCo_cacheFree)
#define gdb_mCo_inSancList	(gdb_mCo_sancAdd|gdb_mCo_sancAct|gdb_mCo_sancRem)

} gdb_mCo;

typedef union {
  pwr_tBitMask m;
  pwr_32Bits (
    pwr_Bits( failIfAdded	, 1),

    pwr_Bits( fill		, 31),,,,,,,,,,,,,,,,,,,,,,,,,,,,,,
  ) b;

#define gdb_mAdd__		0
#define gdb_mAdd_failIfAdded	pwr_Bit(0)
#define gdb_mAdd_		(~gdb_mAdd__)

} gdb_mAdd;

typedef union {
  pwr_tBitMask m;
  pwr_32Bits (
    pwr_Bits( native		, 1 ),
    pwr_Bits( netCached		, 1 ),
    pwr_Bits( fileCached	, 1 ),
    pwr_Bits( privat		, 1 ),
    pwr_Bits( loaded		, 1 ),
    pwr_Bits( fill_0		, 3 ),,,

    pwr_Bits( fill_1		, 24 ),,,,,,,,,,,,,,,,,,,,,,,
  ) b;

#define gdb_mLoad__		0
#define gdb_mLoad_native	pwr_Bit(0)
#define gdb_mLoad_netCached	pwr_Bit(1)
#define gdb_mLoad_fileCached	pwr_Bit(2)
#define gdb_mLoad_private	pwr_Bit(3)
#define gdb_mLoad_loaded	pwr_Bit(4)
#define gdb_mLoad_		(~gdb_mLoad__)

#define gdb_mLoad_build		(gdb_mLoad_native|gdb_mLoad_loaded)
} gdb_mLoad;

typedef union {
  pwr_tBitMask m;
  pwr_32Bits (
    pwr_Bits( connected	, 1),
    pwr_Bits( active	, 1),
    pwr_Bits( up	, 1),
    pwr_Bits( fill_0	, 5),,,,,

    pwr_Bits( fill_1	, 8),,,,,,,,
    pwr_Bits( fill_2	, 8),,,,,,,,
    pwr_Bits( fill_3	, 8),,,,,,,
  ) b;

#define gdb_mNode__		0
#define gdb_mNode_connected	pwr_Bit(0)
#define gdb_mNode_active	pwr_Bit(1)
#define gdb_mNode_up		pwr_Bit(2)
#define gdb_mNode_		(~gdb_mNode__)

} gdb_mNode;

/* Volume definitions.  */

typedef union {
  pwr_tBitMask	m;
  pwr_32Bits (
    pwr_Bits( isMountedOn	, 1),

    pwr_Bits( fill		, 31),,,,,,,,,,,,,,,,,,,,,,,,,,,,,,
  ) b;

#define gdb_mNv__		0
#define gdb_mNv_isMountedOn	pwr_Bit(0)
#define gdb_mNv_		(~gdb_mNv__)

} gdb_mNv;

typedef struct {
  pool_sQlink		volmo_lh; /**< List of 'mounted on' in this volume.  */
  pool_sQlink		sc_lh;/**< List of sub class objects in this volume.  */
  gdb_mNv		flags;
  pwr_tObjid		next_oid;
  co_mFormat            format;
} gdb_sNvolume;

typedef union {
  pwr_tBitMask m;
  pwr_32Bits (
    pwr_Bits( isMounted		,1 ),
    pwr_Bits( isConnected	,1 ),
    pwr_Bits( transAlias	,1 ),
    pwr_Bits( transMount	,1 ),
    pwr_Bits( fill_0		,4 ),,,,

    pwr_Bits( inVidTable	,1 ),
    pwr_Bits( inVnTable		,1 ),
    pwr_Bits( inVolList		,1 ),
    pwr_Bits( inOwnList		,1 ),
    pwr_Bits( fill_1		,4 ),,,,

    pwr_Bits( isOwned		,1 ),
    pwr_Bits( isNative		,1 ),
    pwr_Bits( isCached		,1 ),
    pwr_Bits( isLoaded		,1 ),
    pwr_Bits( fill_2		,2 ),,
    pwr_Bits( privat		,1 ),
    pwr_Bits( root		,1 ),

    pwr_Bits( sub		,1 ),
    pwr_Bits( system		,1 ),
    pwr_Bits( dynamic		,1 ),
    pwr_Bits( shared		,1 ),
    pwr_Bits( classvol		,1 ),
    pwr_Bits( netCached		,1 ),
    pwr_Bits( fileCached	,1 ),
    pwr_Bits( remote		,1 )
  ) b;

#define gdb_mLv__		0
#define gdb_mLv_isMounted	pwr_Bit(0)
#define gdb_mLv_isConnected	pwr_Bit(1)
#define gdb_mLv_transAlias	pwr_Bit(2)
#define gdb_mLv_transMount	pwr_Bit(3)

#define gdb_mLv_inVidTable	pwr_Bit(8)
#define gdb_mLv_inVnTable	pwr_Bit(9)
#define gdb_mLv_inVolList	pwr_Bit(10)
#define gdb_mLv_inOwnList	pwr_Bit(11)

#define gdb_mLv_isOwned		pwr_Bit(16)
#define gdb_mLv_isNative	pwr_Bit(17)
#define gdb_mLv_isCached	pwr_Bit(18)
#define gdb_mLv_isLoaded	pwr_Bit(19)
#define gdb_mLv_private		pwr_Bit(22)
#define gdb_mLv_root		pwr_Bit(23)

#define gdb_mLv_sub		pwr_Bit(24)
#define gdb_mLv_system		pwr_Bit(25)
#define gdb_mLv_dynamic		pwr_Bit(26)
#define gdb_mLv_shared		pwr_Bit(27)
#define gdb_mLv_class		pwr_Bit(28)
#define gdb_mLv_netCached	pwr_Bit(29)
#define gdb_mLv_fileCached	pwr_Bit(30)
#define gdb_mLv_remote		pwr_Bit(31)
#define gdb_mLv_		(~gdb_mLv__)

#define gdb_mLv_public		(gdb_mLv_root|gdb_mLv_sub|gdb_mLv_system|gdb_mLv_dynamic)
#define gdb_mLv_owned		(gdb_mLv_private|gdb_mLv_public)
#define gdb_mLv_common		(gdb_mLv_shared|gdb_mLv_class)
#define gdb_mLv_cached		(gdb_mLv_netCached|gdb_mLv_fileCached)
#define gdb_mLv_native		(gdb_mLv_owned|gdb_mLv_common)
#define gdb_mLv_local		(gdb_mLv_native|gdb_mLv_cached)
#define gdb_mLv_global		(gdb_mLv_local|gdb_mLv_remote)

#define gdb_mLv_objectFlags	(0xffff0000)
} gdb_mLv;

typedef struct {
  pool_sQlink		vid_htl;  /**< Link in the vid-to-volume hash table.  */
  pool_sQlink		vn_htl;   /**< Link in the name-to-volume hash table.  */
  pool_sQlink		vol_ll;   /**< Link in the list of volumes known in one node.  */
  pool_sQlink		own_ll;   /**< Link in the list of volumes owned by one node.  */
  pool_sQlink		obj_lh;   /**< List of objects in this volume.  */
  pool_sQlink		volms_lh; /**< List of mount servers in this volume.  */
  pool_tRef		nr;       /**< Reference to node.  */
  gdb_mLv		flags;
} gdb_sLvolume;
  
typedef struct {
  pool_sQlink		lh;
  pwr_tUInt32		lc;
  pwr_tUInt32		lc_max;
  pwr_tUInt32		lc_min;
  gdb_mCo		flags;
  pool_tRef		next;
} gdb_sTouchQ;

typedef struct {
  gdb_sTouchQ		cacheLock;
  gdb_sTouchQ		cacheVol;
  pwr_tBoolean		equalClasses; /** @todo NYI. Set if referenced class volumes have the same versions as the local */
} gdb_sCvolume;

typedef struct {
  pwr_uVolume		v;
  gdb_sLvolume		l;
  net_sGvolume		g;
  union {
    gdb_sNvolume	n;
    gdb_sCvolume	c;
  } u;
} gdb_sVolume;




/** Cached Class Volume
 */

typedef struct {
  pwr_tNodeId		nid;       /**< Node id */
  pwr_tVolumeId         vid;       /**< Volume id */  
} gdb_sCcVolKey;


typedef struct {
  pool_sQlink           ccvol_htl; /**< Entry in cached class volumes hash table */
  pool_sQlink           ccvol_ll;  /**< Entry in cached class volumes hash table */
  gdb_sCcVolKey		key;       /**< Hash table key */
  pwr_tTime             time;      /**< Time for the class volume */ 
  pwr_tBoolean          equalClasses; /**< True if the cached volume has the same versions as the local */
} gdb_sCclassVolume;


/** Cached Attribute
 */

typedef struct {
  net_sCattribute g;
} gdb_sCattribute;


/** Cached class bit mask
 */
typedef union {
  pwr_tBitMask m;
  pwr_32Bits (
    pwr_Bits( equal		, 1 ), /**< The cached class is equal to the native class */
    pwr_Bits( nrConv		, 1 ), /**< Native to remote conversion data exist */
    pwr_Bits( rnConv		, 1 ), /**< Remote to native conversion data exist */
    pwr_Bits( cacheLock		, 1 ), /**< Class is locked */
    pwr_Bits( fill_0		, 4 ),,,,

    pwr_Bits( fill_1		, 24 ),,,,,,,,,,,,,,,,,,,,,,,
  ) b;

#define gdb_mCclass__		0
#define gdb_mCclass_equal	pwr_Bit(0)
#define gdb_mCclass_nrConv	pwr_Bit(1)
#define gdb_mCclass_rnConv	pwr_Bit(2)
#define gdb_mCclass_cacheLock	pwr_Bit(3)
#define gdb_mCclass_		(~gdb_mCclass__)

} gdb_mCclass;




/** Cached Class
 */

typedef struct 
{
  pwr_tClassId		cid;        /**< Class Id */
  pwr_tTime             ccvoltime;  /**< Cached class volume modification time */
} gdb_sCclassKey;



typedef struct {
  pool_sQlink		cclass_htl; /**< Entry in cached class hash table */
  pool_sQlink		cache_ll;   /**< Cached class list */
  pool_sQlink		subc_lh;    /**< Head of subscriptions that uses this class */
  gdb_sCclassKey	key;        /**< Hash table key */
  pwr_tTime		time;       /**< Class modification time */
  gdb_mCclass           flags;
  pwr_tUInt32		lockCnt;    /**< Lock counter */
  pwr_tUInt32           size;       /**< Size of the body. Is this needed? */
  pool_tRef             nrConv;     /**< Native to remote conversion info */ 
  pool_tRef             rnConv;     /**< Remote to native conversion info */ 
  pwr_tUInt32           acount;     /**< Number of attributes in attr */
  net_sCattribute	attr[1];    /**< Dynamic size */
} gdb_sCclass;


  

/** Object.

   An object is represented by different parts.

   Local part
   Global part

   Native or Cached part.  */

typedef union {
  pwr_tBitMask m;
  pwr_32Bits (
    pwr_Bits( isMountServer	, 1),
    pwr_Bits( fill_0		, 7),,,,,,,

    pwr_Bits( inObjList		, 1),
    pwr_Bits( inOidTab		, 1),
    pwr_Bits( inFamilyTab	, 1),
    pwr_Bits( fill_1		, 5),,,,,

    pwr_Bits( isOwned		, 1),
    pwr_Bits( isNative		, 1),
    pwr_Bits( isCached		, 1),
    pwr_Bits( isLoaded		, 1),
    pwr_Bits( fill_2		, 2),,
    pwr_Bits( privat		, 1),
    pwr_Bits( root		, 1),

    pwr_Bits( sub		, 1),
    pwr_Bits( system		, 1),
    pwr_Bits( dynamic		, 1),
    pwr_Bits( shared		, 1),
    pwr_Bits( classvol		, 1),
    pwr_Bits( netCached		, 1),
    pwr_Bits( fileCached	, 1),
    pwr_Bits( remote		, 1)
  ) b;

#define gdb_mLo__		0
#define gdb_mLo_isMountServer	pwr_Bit(0)

#define gdb_mLo_inObjList	pwr_Bit(8)
#define gdb_mLo_inOidTab	pwr_Bit(9)
#define gdb_mLo_inFamilyTab	pwr_Bit(10)

#define gdb_mLo_isOwned		gdb_mLv_isOwned
#define gdb_mLo_isNative	gdb_mLv_isNative
#define gdb_mLo_isCached	gdb_mLv_isCached
#define gdb_mLo_private		gdb_mLv_private		
#define gdb_mLo_root		gdb_mLv_root

#define gdb_mLo_sub		gdb_mLv_sub
#define gdb_mLo_system		gdb_mLv_system
#define gdb_mLo_dynamic		gdb_mLv_dynamic
#define gdb_mLo_shared		gdb_mLv_shared
#define gdb_mLo_class		gdb_mLv_class
#define gdb_mLo_netCached	gdb_mLv_netCached
#define gdb_mLo_fileCached	gdb_mLv_fileCached
#define gdb_mLo_remote		gdb_mLv_remote
#define gdb_mLo_		(~gdb_mLo__)

#define gdb_mLo_public		gdb_mLv_public
#define gdb_mLo_owned		gdb_mLv_owned
#define gdb_mLo_common		gdb_mLv_common
#define gdb_mLo_cached		gdb_mLv_cached
#define gdb_mLo_native		gdb_mLv_native
#define gdb_mLo_local		gdb_mLv_local
#define gdb_mLo_global		gdb_mLv_global
} gdb_mLo;

typedef struct {
  pwr_tUInt32		maxa;           /**< maximized remote alarm level */
  pwr_tUInt32		maxb;		/**< maximized remote block level */
  pwr_tUInt32		idx;		/**< Alarm block level index.  */
} gdb_sRalarm;
  
typedef struct {
  pool_sQlink		obj_ll;		/**< List of objects in one volume.  */
  pool_sQlink		oid_htl;	/**< Oid hash table entry.  */
  pool_sQlink		family_htl;	/**< Family table entry.  */
  pool_tRef		por;		/**< The parent object.  */
  pool_tRef		vr;		/**< Reference to the volume.  */
  net_sAlarm		al;		/**< Alarm.  */
  gdb_mLo		flags;
} gdb_sLobject;

/* Object: Native part.  */

typedef union {
  pwr_tBitMask m;
  pwr_32Bits (
    pwr_Bits( inSibList			, 1 ),
    pwr_Bits( hasChild			, 1 ),
    pwr_Bits( inCidList			, 1 ),
    pwr_Bits( inAliasClientList		, 1 ),
    pwr_Bits( inMountClientList		, 1 ),
    pwr_Bits( pendingDelete		, 1 ),
    pwr_Bits( isMountClean		, 1 ),
    pwr_Bits( fill_0			, 1 ),

    pwr_Bits( swapDelete		, 1 ),
    pwr_Bits( fill_1			, 7 ),,,,,,,

    pwr_Bits( isSc                      , 1 ), /* MUST be same as gdb_mSc */
    pwr_Bits( hasSc                     , 1 ),
    pwr_Bits( fill_2			, 6 ),,,,,,

    pwr_Bits( bodyDecoded		, 1 ),
    pwr_Bits( systemCreated		, 1 ),
    pwr_Bits( fill_3			, 6 ),,,,,
  ) b;

#define gdb_mNo__			0
#define gdb_mNo_inSibList		pwr_Bit(0)
#define gdb_mNo_hasChild		pwr_Bit(1)
#define gdb_mNo_inCidList		pwr_Bit(2)
#define gdb_mNo_inAliasClientList	pwr_Bit(3)
#define gdb_mNo_inMountClientList	pwr_Bit(4)
#define gdb_mNo_pendingDelete		pwr_Bit(5)
#define gdb_mNo_isMountClean		pwr_Bit(6)

#define gdb_mNo_swapDelete		pwr_Bit(8)

#define gdb_mNo_isSc		        pwr_Bit(16)
#define gdb_mNo_hasSc		        pwr_Bit(17)

#define gdb_mNo_bodyDecoded		pwr_Bit(24)
#define gdb_mNo_systemCreated		pwr_Bit(25)
#define gdb_mNo_			(~gdb_mNo__)

#define gdb_mNo_inClientList		(gdb_mNo_inAliasClientList | gdb_mNo_inMountClientList)

#define gdb_mNo_swap			pwr_SetByte(1, 0xff)

} gdb_mNo;

typedef struct {
  pool_sQlink		cid_ll;		/**< Next/prv object of same class */
  gdb_mNo		flags;          /**< NOTE! Must be placed directly after cid_ll 
                                             the same position is needed in gdb_sScObject */
  pool_sQlink		cli_ll;		/**< Mount/Alias client list. */
  pool_sQlink		sib_lh;		/**< Head of children sibling list. */
  pool_sQlink		sib_ll;		/**< Sibling list.  */
  pool_tRef		body;		/**< Address of actual body in rtdbpool.  */
  pwr_tTime             time;
  pwr_tUInt32		dlcount;
  pwr_tUInt32		subcount;
  pwr_tUInt32		sancount;
  gdb_sRalarm		ral;		/**< Remote alarm.  */
  dbs_mFlags		lflags;
  pool_sQlink	 	sc_lh;        /**< Head of children sub class sibling list */
} gdb_sNobject;

typedef struct {
  pool_sQlink		cache_ll;	/**< Cache list */
  pool_sQlink		sanc_ll;	/**< Subscribed alarm notification client list.  */
  pwr_tRefId		sanid;		/**< Subscribed alarm notification identity.  */
  pwr_tUInt32		sanexp;		/**< Expiration time for san, san-scan-index format.  */
  pwr_tUInt32		nChild;		/**< Number of children in cache.  */
  gdb_mCo		flags;
} gdb_sCobject;

typedef struct {
  gdb_sLobject		l;		/**< Local part.  */
  net_sGobject		g;		/**< Global part.  */
  union {
    gdb_sNobject	n;		/**< Native part,  */
    gdb_sCobject	c;		/**< Cached part.  */
  } u;
} gdb_sObject;


/** The Sub Class object. It's not a complete object it's just needed
 *  when the class list is traversed.
 */ 

typedef union {
  pwr_tBitMask m;
  pwr_32Bits (
    pwr_Bits( inScList			, 1 ),
    pwr_Bits( inScTab			, 1 ),
    pwr_Bits( inSibList			, 1 ),
    pwr_Bits( inCidList			, 1 ),
    pwr_Bits( fill_0			, 4 ),,,,

    pwr_Bits( fill_1			, 8 ),,,,,,,,

    pwr_Bits( isSc                      , 1 ), /* MUST be same as gdb_mNo */
    pwr_Bits( hasSc                     , 1 ),
    pwr_Bits( isParentSc                , 1 ),    
    pwr_Bits( isArrayElem               , 1 ),    
    pwr_Bits( fill_2			, 4 ),,,,

    pwr_Bits( fill_3			, 8 ),,,,,,,
  ) b;

#define gdb_mSc__			0
#define gdb_mSc_inScList		pwr_Bit(0)
#define gdb_mSc_inScTab			pwr_Bit(1)
#define gdb_mSc_inSibList		pwr_Bit(2)
#define gdb_mSc_inCidList		pwr_Bit(3)

#define gdb_mSc_isSc		        pwr_Bit(16)
#define gdb_mSc_hasSc		        pwr_Bit(17)
#define gdb_mSc_isParentSc      	pwr_Bit(18)
#define gdb_mSc_isArrayElem	        pwr_Bit(19)
#define gdb_mSc_			(~gdb_mSc__)

} gdb_mSc;

typedef struct {
  pwr_tObjid            oid;            /**< Object Id */
  pwr_tClassId          cid;            /**< Class Id */
  pool_sQlink		sc_htl;	        /**< Sub Class hash table entry. */
  pool_sQlink		sc_ll;	        /**< List of sub class objects in one volume. */
  pool_tRef             vr;             /**< Volume reference */
  pool_tRef             o_r;            /**< The "real" object. or is a reserved word in C++ */
  pwr_tObjid            poid;
  pool_tRef             por;            /**< Sub Class or Object Parent*/
  pool_tRef             cr;             /**< Reference to gdb_sClass */
  pwr_tUInt32           aidx;           /**< Attribute index in gdb_sClass */
  pwr_tUInt32           elem;           /**< Index if array element, else ULONG_MAX */
  pool_sQlink           cid_ll;         /**< Next/prv object of same class */
  gdb_mSc               flags;          /**< NOTE! Must be placed directly after cid_ll 
                                             the same position is needed in gdb_sNobject */
  dbs_mFlags		lflags;
  pool_sQlink           sib_ll;	        /**< Sibling list. */
  pool_sQlink	 	sib_lh;         /**< Head of children sub class sibling list */
  pwr_tUInt32           offset;         /**< Offset in parent body */
  pwr_tUInt32           size;           /**< Size of body */
  pool_tRef             body;	        /**< Address of actual body in the "real" object's
                                             body. Must not be freed  */
} gdb_sScObject; 


typedef struct {
  pool_tRef		aor;		/**< Attribute object header reference.  */
  pool_tRef		abr;		/**< Attribute object body reference.  */
  pwr_mAdef		flags;
  pwr_eType		type;
  pwr_tUInt32		offs;
  pwr_tUInt32		size;
  pwr_tUInt32		elem;
  pwr_tUInt32		moffset;	/**< Attribute maximum offset within body.  */
  pwr_tAix		aix;
  pwr_tClassId          cid;            /**< If class, Class Id */
  pool_tRef             cr;             /**< If class, gdb_sClass reference */
} gdb_sAttribute;

typedef struct {
  pool_sQlink		cid_htl;	/**< Entry in class hash table.  */
  pwr_tClassId		cid;		/**< Class identity of class.  */
  pool_sQlink		class_ll;	/**< Entry in list of classes on node.  */
  pool_sQlink		cid_lh;		/**< Head of instance list of this class,
					     Template excluded.  */
  pool_tRef		cor;		/**< Class object header reference.  */
  pool_tRef		cbr;		/**< Class object body reference.  */
  pool_tRef		bor;		/**< ObjBodyDef object header reference.  */
  pool_tRef		bbr;		/**< ObjBodyDef object body reference.  */
  pwr_tUInt32		size;		/**< Size of objects body.  */
  pwr_tBoolean          hasSc;          /**< At least one attribute is a class */
  pwr_tUInt32		acount;		/**< Number of attributes.  */
  gdb_sAttribute	attr[1];
} gdb_sClass;


typedef struct {
  pool_sQlink		volmo_ll;	/**< Volume mounted on list.  */
  pool_sQlink		nodmo_ll;	/**< Node mounted on list.  */
  pwr_tNodeId		nid;
  pwr_tVolumeId		vid;
  pool_tRef		vr;		/**< Pool reference of the volume.  */
  pool_tRef		nr;		/**< Pool reference of the node.  */
} gdb_sMountedOn;

typedef struct {
  pwr_tObjid		oid;		/**< Object identity of server object.  */
  pool_sQlink		ms_htl;		/**< Entry in server hash table.  */
  pool_sQlink		ms_ll;		/**< Entry in list of all mount servers */
  pool_sQlink		nodms_ll;	/**< Entry in server node's mount server list.  */
  pool_sQlink		volms_ll;	/**< Entry in server volume's mount server list.  */
  pool_tRef		msor;		/**< Object head of mount server.  */
  pool_tRef		vr;		/**< Volume.  */
  pool_sQlink		cli_lh;		/**< Header of mount client list.  */
} gdb_sMountServer;

typedef struct {
  pwr_tObjid		oid;		/**< Object identity of server object.  */
  pool_sQlink		as_htl;		/**< Entry in Alias server hash table.  */
  pool_sQlink		as_ll;		/**< Entry in list of all alias servers */
  pool_tRef		asor;		/**< Server oh.  */
  pool_sQlink		cli_lh;		/**< Alias client object header list.  */
} gdb_sAliasServer;

typedef struct {
  pwr_tNodeId	nid;		/**< Node identity (!= qcom nix #) */
  pool_sQlink	nid_htl;	/**< Link in nid-to-node hash table.  */
  pool_sQlink	nod_ll;		/**< Link in list of all nodes.  */
  pool_sQlink	own_lh;		/**< Header of list of volumes owned by this node.  */
  pwr_tUInt32	own_lc;		/**< Number of owned volumes.  */
  char		name[32];	/**< Ascii name of node (nul-terminated)
					   zero-length means empty slot.  */
  pwr_tObjid	nod_oid;	/**< Node object for this node. */
  pwr_tObjid	vol_oid;	/**< Root volume object of this node.  */
  gdb_mNode	flags;  
  pwr_tUInt32	upcnt;		/**< # of times up */
  pwr_tTime	timeup;		/**< Most recent time link came up */
  pwr_tTime	timedown;	/**< Most recent time link went down */

  co_eOS	os;
  co_eHW	hw;
  co_mFormat    fm;
    

  pool_sQlink	nodms_lh;	/**< Header of list of mountservers for this node.  Client  */

  pool_sQlink	nodmo_lh;	/**< Header of list of 'mounted on' for this node.  Server  */

  /* Cache.  */

  gdb_sTouchQ	cacheNode;	/**< Header of cache list for this node.  */

  pool_sQlink   ccvol_lh;       /**< List of cached class volumes */

  /* Subscriptions.  */

  pool_sQlink	subc_lh;	/**< Header of list of clients served by this node.  */
  pwr_tUInt32	subc_lc;	/**< Number of clients in list above.  */

  pool_sQlink	nodsubs_lh;	/**< Header of list servers for this node.  */
  pool_sQlink	nodsubb_lh;	/**< Header of list of buffer for this node.  */

  /* Subscribed alarm and block notification.  */

  pool_sQlink	sansAct_lh;	/**< Header of list of servers.  */
  pwr_tUInt32	sansAct_lc;	/**< Number of servers.  */
  pool_sQlink	sansUpd_lh;	/**< Header of list of servers to send to client.  */
  pwr_tUInt32	sansUpd_lc;	/**< Number of servers.  */
  pwr_tUInt32 	sans_gen;	/**< Incremented when a sans is added or removed */

  pool_sQlink	sancAdd_lh;	/**< Clients to be added.  */
  pwr_tUInt32	sancAdd_lc;
  pool_sQlink	sancAct_lh;	/**< Active clients.  */
  pwr_tUInt32	sancAct_lc;
  pool_sQlink	sancRem_lh;	/**< Clients to be removed.  */
  pwr_tUInt32	sancRem_lc;


  /* Supported functionality. Please consider a bitmask if you add more */

  pwr_tUInt32	netver;         /**< Net protocol version */
  pwr_tBoolean	cclassSupport;  /**< Cached Class Support */


  /* Receive information counters, Nethandler class */
  
  pwr_tUInt32	rxmsg[net_eMsg_];

  /* Transmit information counters, Nethandler class */

  pwr_tUInt32	txmsg[net_eMsg_];


} gdb_sNode;

typedef struct {
  pwr_tNodeId		nid;			/**< Node index for this node */
  pwr_tObjid		nod_oid;		/**< Object identifier for the node object.  */
  pwr_tUInt32		objects;		/**< Number of  */
  pwr_tUInt32		scObjects;		/**< Number of  */
  pwr_tUInt32		volumes;		/**< Number of  */
  pwr_tUInt32		classes;		/**< Number of  */
  pwr_tUInt32		nodes;			/**< Number of  */
  pwr_tUInt32		ccvolumes;		/**< Number of cached class volumes */
  pwr_tUInt32		cclasses;		/**< Number of cached classes */
  pwr_tUInt32		mountServers;		/**< Number of  */
  pwr_tUInt32		aliasServers;		/**< Number of  */
  pwr_tUInt32		subServers;		/**< Number of  */
  pwr_tUInt32		subClients;		/**< Number of  */
  pwr_tUInt32		sanServers;		/**< Number of  */
  pwr_tUInt32		pool_isize;		/**< GDHpool initial size */
  pwr_tUInt32		pool_esize;		/**< GDHpool extendsize */
  pwr_tUInt32		rtdb_isize;		/**< rtdb initial size */
  pwr_tUInt32		rtdb_esize;		/**< rtdb extendsize  */
  pwr_tUInt32		cvol_max;		/**< cache trimmer activation level */
  pwr_tUInt32		cvol_min;		/**< cahce trimmer end criteria */
} gdb_sInit;

/* The global database header.  */

typedef struct {
  sect_sMutex		lock;		/**< Database lock */
  pid_t			excl_owner;	/**< Owner of the excl lock */
  pid_t			lock_owner;	/**< Owner of db lock */
  pwr_tUInt32		version;	/**< Gdb structure revision.  */

  struct {
    hash_sGtable	subc_ht;	/**< sid -> client hash table.  */
    hash_sGtable	subs_ht;	/**< sid -> server hash table.  */
    hash_sGtable	sans_ht;	/**< sanid -> server hash table.  */
    hash_sGtable	oid_ht;		/**< oid -> object hash table.  */
    hash_sGtable	vid_ht;		/**< vid -> volume hash table.  */
    hash_sGtable	vn_ht;		/**< volume name -> volume hash table.  */
    hash_sGtable	cid_ht;		/**< cid -> class  hash table.  */
    hash_sGtable	tid_ht;		/**< tid -> type  hash table.  */
    hash_sGtable	nid_ht;		/**< nid -> node   hash table.  */
    hash_sGtable	family_ht;	/**< Family (poid + name) -> object hash table.  */
    hash_sGtable	ms_ht;		/**< mount soid -> mount server hash table.  */
    hash_sGtable	as_ht;		/**< mount soid -> alias server hash table.  */
    hash_sGtable	ccvol_ht;	/**< nid + vid -> cached class volume */
    hash_sGtable	cclass_ht;	/**< cid + cached voltime -> cached class */
    hash_sGtable	sc_ht;	        /**< oid -> Sub Class object hash table*/
  } h;

  qcom_sQid		nethandler;	/**< local nethandler */
  qcom_sQid		neth_acp;	/**< local neth acp */
  qcom_sQid		tmon;		/**< local neth acp */

  gdb_sTouchQ		cacheCom;	/**< Root of touched list */
  gdb_sTouchQ		cacheNew;	/**< Root of new list */
  gdb_sTouchQ		cacheOld;	/**< Root of old list */
  gdb_sTouchQ		cachePend;	/**< Root of touched parent list */
  gdb_sTouchQ		cacheFree;	/**< Root of free list */

  gdb_sTouchQ		cacheCclass;	/**< Root of cached class list */

  pwr_tUInt32		rqgen;		/**< Request generation number */

  pwr_tVolumeId		vid;		/**< Vid of root volume. */
  pwr_tNodeId		nid;		/**< Nid of this node. */
  pwr_tObjid		vol_oid;	/**< Objid of root volume object. */
  pwr_tObjid		nod_oid;	/**< Objid of node object of root volume object. */

  pool_sQlink		vol_lh;		/**< Root of volume list */
  pool_sQlink		nod_lh;		/**< Root of node list */
  pool_sQlink		ms_lh;		/**< Root of mount server list */
  pool_sQlink		as_lh;		/**< Root of alias server list */
  pool_sQlink		class_lh;	/**< Root of class list */

  gdb_sInit		orig_init;	/**< Original initialization parameters */
  gdb_sInit		eval_init;	/**< Evaluated initialization parameters */

  net_mLog		log;		/**< Mask that controls logging */

  pool_sQlink		dl_lh;		/**< root of DL list */
  pwr_tUInt32		dl_lc;		/**< length of DL list */
  pwr_tDlid		dlid;		/**< Next direct link id. */

  pwr_tRefId		sancid;		/**< Next san client id. */

  /* The following items are used by subscriptions */

  pwr_tSubid		subcid;		/**< Next subscription client id */

  pwr_tUInt32		tmocnt;		/**< # tmo checks run */
  pwr_tUInt32		tmotime;	/**< tmo chk timer, 0.1s units */
  pwr_tUInt32		tmolap;		/**< time for 1 lap through all remote clients.  */
  pool_sQlink		subt_lh;	/**< List clients watched for timeout */
  pwr_tUInt32		subt_lc;	/**< length of timeout list */

  pool_sQlink		subs_lh;	/**< List header of subscription servers.  */
  pwr_tUInt32		subs_lc;	/**< Number of subscription servers.  */
  pool_sQlink		subb_lh;	/**< List header of subscription buffers.  */
  pwr_tUInt32		subb_lc;	/**< length of buffer list */
  pool_sQlink		subm_lh;	/**< Root of subscription messages list */
  pwr_tUInt32		subm_lc;	/**< length of message list */

  pool_sQlink		tmonq_lh;	/**< Root of timer registration list */

  pwr_tUInt32		al_idx;		/**< Alarm index, set every time alarm or block changes.  */

  pwr_tUInt32		cache_trim_int;	/**< Cache trim interval, ms */
  pwr_tUInt32		sanc_add_int;	/**< Sanc check add interval, ms */  
  pwr_tUInt32		sanc_exp_int;	/**< Sanc check expired interval, ms */  
  pwr_tUInt32		sans_chk_int;	/**< Sans check interval, ms */  
  pwr_tUInt32		subc_chk_int;	/**< Subc check interval, ms */  
} gdb_sGlobal;


/* Job local GDH data, pointed to by gdhi_gLocal, the root of all tables.  */

typedef struct {
#ifdef	OS_ELN
  MUTEX			thread_lock;	/**< ELN lock only */
#elif defined OS_LYNX || defined OS_LINUX
  pthread_mutex_t	thread_lock;	/**< LYNX and LINUX lock only */
#endif
  struct {
    sect_sHead		sect;		/**< Section header for global database.  */
    sect_sHead		lock;		/**< Section header for .  */
    pool_sHead		pool;		/**< Pool for database */
    pool_sHead		rtdb;		/**< Pool for object bodies */
    hash_sTable		subc_ht;	/**< sid -> client hash table.  */
    hash_sTable		subs_ht;	/**< sid -> server hash table.  */
    hash_sTable		sans_ht;	/**< sanid -> server hash table.  */
    hash_sTable		oid_ht;		/**< oid -> object hash table.  */
    hash_sTable		vid_ht;		/**< vid -> volume hash table.  */
    hash_sTable		vn_ht;		/**< volume name -> volume hash table.  */
    hash_sTable		cid_ht;		/**< cid -> class  hash table.  */
    hash_sTable		tid_ht;		/**< tid -> type  hash table.  */
    hash_sTable		nid_ht;		/**< nid -> node   hash table.  */
    hash_sTable		family_ht;	/**< Family (poid + name) -> object hash table.  */
    hash_sTable		ms_ht;		/**< mount soid -> mount server hash table.  */
    hash_sTable		as_ht;		/**< mount soid -> alias server hash table.  */
    hash_sTable		ccvol_ht;	/**< nid + vid -> cached class volume hash table*/
    hash_sTable		cclass_ht;	/**< cid + cached voltime -> cached class hash table */
    hash_sTable		sc_ht;	        /**< oid -> sub class object hash table  */
  } h;
  gdb_sGlobal		*db;		/**< Database Root, (in db_lock section) */
  sect_sHead		*sect;		/**< Section header for global database.  */
  sect_sHead		*lock;		/**< Section header for .  */
  pool_sHead		*pool;		/**< Internal pool for database */
  pool_sHead		*rtdb;		/**< Internal pool for object bodies */
  hash_sTable		*subc_ht;	/**< pwr_tSubid to subcli hash table */
  hash_sTable		*subs_ht;	/**< pwr_tSubid to subsrv hash table */
  hash_sTable		*sans_ht;	/**< sanid -> server hash table.  */
  hash_sTable		*oid_ht;	/**< Object identity table.  */
  hash_sTable		*vid_ht;	/**< Volume table.  */
  hash_sTable		*vn_ht;		/**< volume name -> volume hash table.  */
  hash_sTable		*cid_ht;	/**< Class table.  */
  hash_sTable		*tid_ht;	/**< tid -> type  hash table.  */
  hash_sTable		*nid_ht;	/**< Node table.  */
  hash_sTable		*family_ht;	/**< Family table.  */
  hash_sTable		*ms_ht;		/**< mount soid -> mount server hash table.  */
  hash_sTable		*as_ht;		/**< mount soid -> alias server hash table.  */
  hash_sTable		*ccvol_ht;	/**< nid + vid -> cached class volume hash table */
  hash_sTable		*cclass_ht;	/**< cid + cached voltime -> cached class hash table */
  hash_sTable		*sc_ht;	        /**< oid -> sub class object hash table  */

  gdb_sVolume		*my_volume;	/**< The local root volume.  */
  gdb_sVolume		*no_volume;	/**< The unknown volume with vid = 0.0.0.0.  */
  gdb_sNode		*my_node;	/**< Pointer to NodeHead of local node.  */
  gdb_sNode		*no_node;	/**< Pointer to NodeHead of the unknown node.  */

  pwr_tUInt32		lock_count;	/**< # of times this job locked the DB */
  qcom_sQid		my_qid;		/**< The QueId of this job */
  qcom_sAid		my_aid;		/**< The QueId of this job */
  pid_t			my_pid;		/**< The process id of this job */
  pwr_tBoolean		is_tmon;	/**< Set if tmon */
} gdb_sLocal;

/* The root of all data, the only `global' variable...  */

#if defined (OS_ELN)
  extern noshare gdb_sLocal	*gdbroot;	/**< root of all data in database */
#else
  extern gdb_sLocal	*gdbroot;	/**< root of all data in database */
#endif

typedef enum {
  gdb_eTmon__ = 0,
  gdb_eTmon_subbCheck,
  gdb_eTmon_
} gdb_eTmon;

typedef struct {
  pool_sQlink		ll;
  gdb_eTmon		type;
  pwr_tUInt32		dt;		/**< Update time in milli seconds */
} gdb_sTmonQlink;

/* Function prototypes.  */

gdb_sAliasServer *
gdb_AddAliasServer (
  pwr_tStatus		*sts,
  pwr_tObjid		soid,
  pwr_tBitMask		flags
);

gdb_sClass *
gdb_AddClass (
  pwr_tStatus		*sts,
  pwr_tClassId		cid,
  pwr_tBitMask		flags
);

gdb_sMountServer *
gdb_AddMountServer (
  pwr_tStatus		*sts,
  pwr_tObjid		soid,
  pwr_tBitMask		flags
);

gdb_sNode *
gdb_AddNode (
  pwr_tStatus		*sts,
  pwr_tNodeId		nid,
  pwr_tBitMask		flags
);

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
);

gdb_sScObject *
gdb_AddScObject (
  pwr_tStatus		*sts,
  pwr_tObjid		oid,
  pwr_tClassId		cid,
  pwr_tUInt32		size,
  pwr_tObjid		poid,
  pwr_tUInt32           aidx,
  pwr_tUInt32           elem,
  gdb_mSc		flags
);

gdb_sVolume *
gdb_AddVolume (
  pwr_tStatus		*sts,
  pwr_tVolumeId		vid,
  pwr_tBitMask		flags
);

gdb_sLocal *
gdb_CreateDb (
  pwr_tStatus		*sts,
  gdb_sInit		*ip
);

void
gdb_UnlinkDb (
);

gdb_sObject *
gdb_LoadObject (
  pwr_tStatus		*sts,
  gdb_sVolume		*vp,
  const char		*name,
  pwr_tObjid		oid,
  pwr_tClassId		cid,
  pwr_tUInt32		size,
  pwr_tObjid		poid,
  pwr_tBitMask		iflags,
  pwr_tObjid		soid,
  void			*bodyp
);

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
);

gdb_sObject *
gdb_LockObject (
  pwr_tStatus		*sts,
  gdb_sObject		*op
);

gdb_sLocal *
gdb_MapDb (
  pwr_tStatus		*sts,
  qcom_sQid		*qid,
  const char		*name
);

gdb_sClass *
gdb_ReAddClass (
  pwr_tStatus		*sts,
  gdb_sClass		*cp,
  unsigned int		acount  
);

void
gdb_RemoveObject (
  pwr_tStatus	*sts,
  gdb_sObject	*op
);

pwr_tBoolean
gdb_UnlockObject (
  pwr_tStatus		*sts,
  gdb_sObject		*op
);
#endif
