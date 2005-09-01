/* 
 * Proview   $Id: rt_qini.h,v 1.2 2005-09-01 14:57:56 claes Exp $
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

#ifndef rt_qini_h
#define rt_qini_h

/* rt_qini.h -- Initiate the Qcom database. */

#if defined(OS_ELN)
# include stdlib
# include string
# include time
# include <in.h>
# include <inet.h>
#else
# include <stdlib.h>
# include <string.h>
# include <time.h>
#endif

#if defined(OS_VMS)
# include <lib$routines.h>
# include <in.h>
# include <inet.h>
# include <ucx$inetdef.h>
#elif defined(OS_LYNX) || defined(OS_LINUX)
# include <netinet/in.h>
# include <arpa/inet.h>
#endif

#include "pwr.h"
#include "rt_qcom.h"
#include "rt_qdb.h"
#include "rt_qdb_msg.h"

#if defined(OS_LINUX)
# include <sys/socket.h>
#else
# include <socket.h>
#endif

#include "pwr.h"
#include "co_tree.h"
#include "co_cdh.h"

typedef union {
  pwr_tBitMask m;
  pwr_32Bits (
    pwr_Bits( name	, 1),
    pwr_Bits( naddr	, 1),
    pwr_Bits( port	, 1),
    pwr_Bits( flags	, 1),
    pwr_Bits( fill_0	, 4),,,,

    pwr_Bits( fill_1	, 8),,,,,,,,
    pwr_Bits( idx	, 8),,,,,,,,
    pwr_Bits( type	, 8),,,,,,,
  ) b;

#define qini_mNode__		0
#define qini_mNode_name		pwr_Bit(0)
#define qini_mNode_naddr	pwr_Bit(1)
#define qini_mNode_last		pwr_Bit(2)
#define qini_mNode_sequence	(qini_mSeg_first|qini_mSeg_middle|qini_mSeg_last)

#define qini_mNode_		(~qini_mNode__)

} qini_mNode;


typedef struct {
  tree_sNode		tree;
  pwr_tNodeId		nid;
  qini_mNode		flags;
  char			name[80];
  struct in_addr	naddr;
  int			port;
  pwr_tBoolean		connect;
} qini_sNode;    

int			qini_ParseFile (FILE*, tree_sTable*, int*, int*, int*);
pwr_tBoolean		qini_BuildDb (pwr_tStatus*, tree_sTable*, qini_sNode*, void*, qcom_tBus);

#endif
