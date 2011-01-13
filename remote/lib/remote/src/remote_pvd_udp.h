/* 
 * Proview   $Id: remote_pvd_udp.h,v 1.2 2007-03-20 12:36:38 claes Exp $
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
 **/

#ifndef remote_pvd_udp_h
#define remote_pvd_udp_h

#include "pwr.h"

#if defined __cplusplus
extern "C" {
#endif

typedef struct {
  int cid;
  pwr_tOix oix;
  pwr_tOix fthoix;
  pwr_tOix bwsoix;
  pwr_tOix fwsoix;
  pwr_tOix fchoix;
  pwr_tOix lchoix;
  char name[32];
} rpvd_sObject;

typedef enum {
  rpvd_eMsg_Oid,
  rpvd_eMsg_ObjectName,
  rpvd_eMsg_Object,
  rpvd_eMsg_ReadAttribute,
  rpvd_eMsg_WriteAttribute,
  rpvd_eMsg_Attribute,
  rpvd_eMsg_Status,
  rpvd_eMsg_SubAdd,
  rpvd_eMsg_SubRemove,
  rpvd_eMsg_SubSend,
  rpvd_eMsg_SubRequest,
  rpvd_eMsg_NodeUp
} rpvd_eMsg;

typedef struct {
  int 		Type;
  int		Id;
  int		Status;
} rpvd_sMsgAny;

typedef struct {
  int 		Type;
  int		Id;
  int		Status;
  pwr_tOid 	Oid;
} rpvd_sMsgOid;

typedef struct {
  int 		Type;
  int		Id;
  int		Status;
  pwr_tOName 	Name;
  pwr_tOid	POid;
} rpvd_sMsgObjectName;

typedef struct {
  int 		Type;
  int		Id;
  int		Status;
  pwr_tOid	Oid;
  int		OSize;
  rpvd_sObject  o[136];
} rpvd_sMsgObject;

typedef struct {
  int 		Type;
  int		Id;
  int		Status;
  pwr_tOid	Oid;
  pwr_tOName   	Attribute;
} rpvd_sMsgReadAttribute;

typedef struct {
  int 		Type;
  int		Id;
  int		Status;
  pwr_tOid	Oid;
  pwr_tOName   	Attribute;
  int		Size;
  pwr_tTypeId   Tid;
  char  	Value[1200];
} rpvd_sMsgAttribute;

typedef struct {
  int 		Type;
  int		Id;
  int		Status;
  pwr_tOid	Oid;
  pwr_tOName   	Attribute;
  int		Size;
  pwr_tTypeId   Tid;
  char  	Value[1200];
} rpvd_sMsgWriteAttribute;

typedef struct {
  int 		Type;
  int		Id;
  int		Status;
  pwr_tOid	Oid;
  pwr_tOName   	Attribute;
  int		Size;
  pwr_tTypeId   Tid;
  int		Rix;
} rpvd_sMsgSubAdd;

typedef struct {
  int 		Type;
  int		Id;
  int		Status;
  int		Rix;
} rpvd_sMsgSubRemove;

typedef struct {
  int 		Type;
  int		Id;
  int		Status;
  int		More;
  char		Data[8184];
} rpvd_sMsgSubSend;

typedef union {
  rpvd_sMsgAny 		Any;
  rpvd_sMsgOid 		Oid;
  rpvd_sMsgObjectName  	ObjectName;
  rpvd_sMsgObject 	Object;
  rpvd_sMsgReadAttribute ReadAttribute;
  rpvd_sMsgWriteAttribute WriteAttribute;
  rpvd_sMsgAttribute 	Attribute;
  rpvd_sMsgSubAdd 	SubAdd;
  rpvd_sMsgSubRemove 	SubRemove;
  rpvd_sMsgSubSend 	SubSend;
} rpvd_sMsg;

void udp_Disable();

void udp_Enable();

pwr_tStatus udp_Send( char *buf, int buf_size);

pwr_tStatus udp_Receive( char **buff, int tmo);

pwr_tStatus udp_Request( char *sendbuf, int sendbuf_size, 
			 char **rcvbuf);

pwr_tStatus udp_Init( char *remote_address, char *remote_host_name, int port);

void udp_LinkFailure();
pwr_tStatus udp_CheckLink();

#if defined __cplusplus
}
#endif
#endif
