/* 
 * Proview   $Id: rt_san.h,v 1.2 2005-09-01 14:57:56 claes Exp $
 * Copyright (C) 2005 SSAB Oxelösund AB.
 *
 * This program is free software; you can redistribute it and/or 
 * modify it under the terms of the GNU General Public License as 
 * published by the Free Software Foundation, either version 2 of 
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful 
 * but WITHOUT ANY WARRANTY; without even the implied warranty of 
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the 
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License 
 * along with the program, if not, write to the Free Software 
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#ifndef rt_san_h
#define rt_san_h

/* rt_san.h -- Subscribed alarm (and block) notification.  */

#ifndef pwr_h
# include "pwr.h"
#endif

#ifndef rt_net_h
# include "rt_net.h"
#endif

#ifndef rt_gdb_h
# include "rt_gdb.h"
#endif

typedef union {
  pwr_tBitMask m;
  pwr_32Bits (
    pwr_Bits( sansAct	, 1 ),
    pwr_Bits( sansUpd	, 1 ),

    pwr_Bits( fill	, 30 ),,,,,,,,,,,,,,,,,,,,,,,,,,,,,
  ) b;

#define san_mServer__		0
#define san_mServer_sansAct	pwr_Bit(0)
#define san_mServer_sansUpd	pwr_Bit(1)
#define san_mServer_		(~san_mServer__)
} san_mServer;

typedef struct {
  net_sSanEntry		sane;		/* San entry.  */
  pool_sQlink		sans_htl;	/* Subscription server hash table link.  */
  pool_sQlink		sansAct_ll;	/* List of servers for one node.  */
  pool_sQlink		sansUpd_ll;	/* List of servers with pending update for one node.  */
  pwr_tUInt32		count;		/* Update count */

  net_sAlarm		al;
  gdb_sObject		*op;		/* May only be used by tmon */
  san_mServer		flags;
} san_sServer;

#endif
