#ifndef rt_sub_h
#define rt_sub_h

/* rt_sub.h -- <short description>

   PROVIEW/R
   Copyright (C) 1996 by Comator Process AB.

   Subscription data

   The subscription database is split in the following parts:
	SUBCLI	Subscription client data
	SUBSRV	Subscription server data
	SUBBUF	Subscription buffer data, gruoups SUBSRVs
	SUBMSG	Subscription update data, the actual message
	SUBDATA	The actual update data for one SUBCLI, inside SUBMSG  */

#if !defined(OS_ELN)
# include <sys/types.h>
#endif

#ifndef pwr_h
#include "pwr.h"
#endif

#ifndef rt_pool_h
#include "rt_pool.h"
#endif

#ifndef rt_gdb_h
#include "rt_gdb.h"
#endif

#ifndef rt_net_h
#include "rt_net.h"
#endif

#define	sub_cHashSize	799
#define	sub_cMsgSize	9900		/* Target (nominal) size of buffers */

typedef struct {
  pwr_tSubid		sid;		/* Must be at same offset in sub_sClient & sub_sServer */
  pool_sQlink		subc_htl;	/* Subscription client hash table link.  */
  pool_sQlink		subc_ll;	/* Link in list of all clients of one node.  */

  /* The following are only used for remote SUBCLIs */

  pool_sQlink		subt_ll;	/* Linkage for timeout watching */
  pool_tRef		submsg;		/* SUBMSG reference */
  pool_tRef		subdata;	/* SUBDATA segment for last update */

  pool_tRef		userdata;	/* User requested buffer, or pool_cNullRef */
  pwr_tUInt32		usersize;	/* Size of user buffer */

  pool_tRef		cclass;		/* != pool_cNullRef if different class versions */
  pwr_sAttrRef		raref;		/* If different class versions */

  /* Status information */

  pwr_tStatus		sts;		/* Last update status */
  pwr_tVolumeId		vid;		/* Volume that owns the object.  */
  pwr_tNodeId		nid;		/* Node identity of data. pwr_cNNodeId = unknown*/
  pwr_tBoolean		old;		/* Old, timed out */
  pwr_tBoolean		tmoactive;	/* SUBCLI is in tmo list */

  /* Statistics */

  pwr_tTime		lastupdate;	/* Time for last update */
  pwr_tUInt32		count;		/* Update count */

  /* The following are set up when the subscription is established: */

  pid_t			subscriber;	/* Who requested subscription */
  pwr_tUInt32		dt;		/* Update time in 10ths of seconds */
  pwr_tUInt32		tmo;		/* Requested timeout time */

  pwr_tBoolean		sub_by_name;	/* name or attrref selector */
  pwr_sAttrRef		aref;		/* Subscription by attribute ref or name */
  char			name [1];	/* Subscription by name (dyn len) */

} sub_sClient;

typedef struct {
  pwr_tSubid		sid;		/* Must be at same offset in sub_sClient & sub_sServer */
  pool_sQlink		subs_htl;	/* Subscription server hash table link.  */
  pool_sQlink		subs_ll;	/* List of all SUBSRVs */
  pool_sQlink		nodsubs_ll;	/* List of all SUBSRVs for one node.  */
  pool_sQlink		bufsubs_ll;	/* List of SUBBUF members */
  pool_tRef		br;		/* Assigned SUBBUF (pool reference).*/

  pwr_tStatus		sts;		/* Status of last operation */
  pwr_tUInt32		count;		/* Update count */

  pwr_tNodeId		nid;		/* node where SUBCLI resides */

  pwr_tUInt32		dt;		/* Requested update time */

  pwr_tBoolean		sub_by_name;	/* name or attrref selector */
  pwr_sAttrRef		aref;		/* Subscription by attribute ref or name */
  pool_tRef		data;		/* Pool reference of actual data.  */
} sub_sServer;


typedef struct {
  pool_sQlink		subb_ll;	/* List of all SUBBUFs */
  pool_sQlink		nodsubb_ll;	/* List of all SUBBUFs for one node.  */
  pool_sQlink		bufsubs_lh;	/* Root for attached SUBSRVs */
  pwr_tUInt32		bufsubs_lc;	/* # of subsrvs in this subbuf */
  gdb_sTmonQlink	tmonq;		/* List of items to be registered by tmon.  */

  pwr_tNodeId		nid;		/* Node to receive update */
  pwr_tUInt32		totsize;	/* Total (accum) data size of buffer */
  pwr_tInt32		dt;		/* Update time in milli seconds.  */

  pwr_tTime		lastupdate;	/* Time for last update */
  pwr_tStatus		sts;		/* Last operation status */
  pwr_tBoolean		queued;		/* Buffer is queued, ie timer active */
} sub_sBuffer;

/* Subscription data transfer.  */

typedef struct {
  pool_sQlink		subm_ll;	/* Linkage when on client side */
  net_sSubMessage	msg;
} sub_sMessage;

void
sub_InitLocal ();

void
sub_UnlinkObject (
  gdb_sObject		*op
);

#endif
