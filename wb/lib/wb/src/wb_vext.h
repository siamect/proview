#ifndef wb_vext_h
#define wb_vext_h


typedef enum {
  vext_eMsgType_Object,
  vext_eMsgType_ObjectOid,
  vext_eMsgType_ObjectName,
  vext_eMsgType_ObjectBody
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
  char			Name[120];
} vext_sQMsgObjectName;

typedef union {
  vext_sQMsgAny		Any;
  vext_sQMsgOid		Oid;
  vext_sQMsgObjectName	ObjectName;
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




