/* 
 * Proview   $Id: wb_vext.h,v 1.6 2005-09-20 13:14:28 claes Exp $
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

#ifndef wb_vext_h
#define wb_vext_h


typedef enum {
  vext_eMsgType_Status,
  vext_eMsgType_Object,
  vext_eMsgType_ObjectOid,
  vext_eMsgType_ObjectName,
  vext_eMsgType_ObjectBody,
  vext_eMsgType_CreateObject,
  vext_eMsgType_MoveObject,
  vext_eMsgType_CopyObject,
  vext_eMsgType_DeleteObject,
  vext_eMsgType_DeleteFamily,
  vext_eMsgType_RenameObject,
  vext_eMsgType_WriteAttr,
  vext_eMsgType_Commit,
  vext_eMsgType_Abort
} vext_eMsgType;

// Question message types

typedef struct {
  unsigned long 	message_type;
  vext_eMsgType		Type;
} vext_sQMsgAny;

typedef struct {
  unsigned long 	message_type;
  vext_eMsgType		Type;
  pwr_tOix		Oix;
} vext_sQMsgOid;

typedef struct {
  unsigned long 	message_type;
  vext_eMsgType		Type;
  pwr_tOix		Oix;
} vext_sQMsgDeleteObject;

typedef struct {
  unsigned long 	message_type;
  vext_eMsgType		Type;
  pwr_tOix		Oix;
} vext_sQMsgDeleteFamily;

typedef struct {
  unsigned long 	message_type;
  vext_eMsgType		Type;
  char			Name[120];
} vext_sQMsgObjectName;

typedef struct {
  unsigned long 	message_type;
  vext_eMsgType		Type;
  pwr_tOix		DestOix;
  int			DestType;
  pwr_tCid		Cid;
  char			Name[120];
} vext_sQMsgCreateObject;

typedef struct {
  unsigned long 	message_type;
  vext_eMsgType		Type;
  pwr_tOix		Oix;
  pwr_tOix		DestOix;
  int			DestType;
} vext_sQMsgMoveObject;

typedef struct {
  unsigned long 	message_type;
  vext_eMsgType		Type;
  pwr_tOix		Oix;
  pwr_tOix		DestOix;
  int			DestType;
  char 			Name[120];
} vext_sQMsgCopyObject;

typedef struct {
  unsigned long 	message_type;
  vext_eMsgType		Type;
  pwr_tOix		Oix;
  char			Name[120];
} vext_sQMsgRenameObject;

typedef struct {
  unsigned long 	message_type;
  vext_eMsgType		Type;
  pwr_tOix		Oix;
  unsigned int	       	Offset;
  unsigned int	       	Size;
  char			Buffer[512];
} vext_sQMsgWriteAttr;

typedef union {
  vext_sQMsgAny		Any;
  vext_sQMsgOid		Oid;
  vext_sQMsgObjectName	ObjectName;
  vext_sQMsgCreateObject CreateObject;
  vext_sQMsgMoveObject 	MoveObject;
  vext_sQMsgCopyObject 	CopyObject;
  vext_sQMsgDeleteObject DeleteObject;
  vext_sQMsgDeleteFamily DeleteFamily;
  vext_sQMsgRenameObject RenameObject;
  vext_sQMsgWriteAttr 	WriteAttr;
} vext_sQMsg;

// Answer message types

typedef struct {
  unsigned long 	message_type;
  vext_eMsgType		Type;
  pwr_tStatus		Status;
} vext_sAMsgAny;

typedef struct {
  unsigned long 	message_type;
  vext_eMsgType		Type;
  pwr_tStatus		Status;
  pwr_tOix		oix;
  pwr_tOix		fthoix;
  pwr_tOix		bwsoix;
  pwr_tOix		fwsoix;
  pwr_tOix		fchoix;
  pwr_tOix		lchoix;
  pwr_tCid		cid;
  char			name[32];
  char			longname[120];
} vext_sAMsgObject;

typedef struct {
  unsigned long 	message_type;
  vext_eMsgType		Type;
  pwr_tStatus		Status;
  pwr_tOix		oix;
  int			size;
  char			body[500];
} vext_sAMsgObjectBody;

typedef union {
  vext_sAMsgAny		Any;
  vext_sAMsgObject	Object;
  vext_sAMsgObjectBody	ObjectBody;
} vext_sAMsg;

#endif




