/* 
 * Proview   $Id: rt_sev_net.h,v 1.4 2008-09-18 15:06:13 claes Exp $
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

#ifndef rt_sev_net_h
#define rt_sev_net_h

#include "pwr.h"

#ifdef __cplusplus
extern "C"
{
#endif

#define sev_eProcSevClient 121
#define sev_eProcSevServer 122
#define sev_cMsgClass	   201

typedef enum {
  sev_eMsgType_NodeUp,
  sev_eMsgType_HistItemsRequest,
  sev_eMsgType_HistItems,
  sev_eMsgType_HistDataStore,
  sev_eMsgType_HistDataGetRequest,
  sev_eMsgType_HistDataGet,
  sev_eMsgType_HistItemDelete,
  sev_eMsgType_HistItemStatus
} sev_eMsgType;  

typedef struct {
  pwr_tOName aname;
  pwr_eType type;
  unsigned int size;
  unsigned int elem;
  pwr_tString16 unit;
} sev_sHistAttr;

typedef struct {
  pwr_tOid oid;
  pwr_tOName oname;
  pwr_tDeltaTime storagetime;
  pwr_tTime creatime;
  pwr_tTime modtime;
  pwr_tRefId sevid;
  pwr_tString80 description;
  pwr_tFloat32 scantime;
  pwr_tFloat32 deadband;
  pwr_tMask options;
  unsigned int attrnum;
  sev_sHistAttr attr[1];
} sev_sHistItem;

typedef struct {
  pwr_tRefId sevid;
  pwr_eType type;
  unsigned int size;
  int data[1];
} sev_sHistData;

// Message types

typedef struct {
  sev_eMsgType         Type;
} sev_sMsgAny;

typedef struct {
  sev_eMsgType         Type;
  pwr_tStatus	       Status;
  sev_sHistItem	       Items[1];
} sev_sMsgHistItems;

typedef struct {
  sev_eMsgType         Type;
  pwr_tTime	       Time;
  int	       	       Data[1];	
} sev_sMsgHistDataStore;

typedef struct {
  sev_eMsgType         Type;
  pwr_tOid	       Oid;
  pwr_tOName	       AName;
  pwr_tTime	       StartTime;
  pwr_tTime	       EndTime;
  int		       NumPoints;
} sev_sMsgHistDataGetRequest;

typedef struct {
  sev_eMsgType         Type;
  pwr_tOid	       Oid;
  pwr_tOName	       AName;
  pwr_tStatus	       Status;
  int		       NumPoints;
  pwr_eType	       VType;
  unsigned int	       VSize;
  int	       	       Data[1];	
} sev_sMsgHistDataGet;

typedef struct {
  sev_eMsgType         Type;
  pwr_tOid	       Oid;
  pwr_tOName	       AName;
} sev_sMsgHistItemDelete;

typedef struct {
  sev_eMsgType         Type;
  pwr_tOid	       Oid;
  pwr_tOName	       AName;
  pwr_tStatus	       Status;
} sev_sMsgHistItemStatus;

#ifdef __cplusplus
}
#endif

#endif
