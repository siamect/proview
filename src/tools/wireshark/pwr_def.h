
/* From rt_qmon.c */

typedef enum {
  eEvent__ = 0 ,
  eEvent_connect,
  eEvent_down,
  eEvent_user,
  eEvent_ack,
  eEvent_
} eEvent;


/* From rt_qcom.h */

#define qcom_cIloopBack		(1<<31 | 0)
#define qcom_cInetEvent		(1<<31 | 100)
#define qcom_cIapplEvent	(1<<31 | 101)
#define qcom_cImhAllHandlers	(1<<31 | 102)
#define qcom_cImhAllOutunits	(1<<31 | 103)
#define qcom_cIhdServer		(1<<31 | 104)
#define qcom_cIhdClient		(1<<31 | 105)
#define qcom_cInacp		(1<<31 | 106)
#define qcom_cIini		(1<<31 | 107)

typedef enum {
  qcom_eBtype__ = 0,
  qcom_eBtype_system,
  qcom_eBtype_qcom,
  qcom_eBtype_event,
  qcom_eBtype_ = 200
} qcom_eBtype;

typedef enum {
  qcom_eStype__ = 0,
  qcom_eStype_linkConnect,
  qcom_eStype_linkDisconnect,
  qcom_eStype_linkActive,
  qcom_eStype_linkStalled,
  qcom_eStype_applConnect,
  qcom_eStype_applDisconnect,
  qcom_eStype_ = 32
} qcom_eStype;


/* From rt_mh_net.x */
#define net_cMsgClass		200

enum net_eMsg {
  net_eMsg__ = -1,
  net_eMsg_error = 0,		/* A network error was detected */

  net_eMsg_id,			/* Nethandler identification */
  net_eMsg_idAck,		/* Nethandler identification acknowlege */
  net_eMsg_idAck2,		/* Nethandler idAck acknowlege */

  net_eMsg_volumes,		/*  */
  net_eMsg_volumesR,		/*  */

  net_eMsg_subAdd,		/* Add subscription */
  net_eMsg_subRemove,		/* Remove subscription */
  net_eMsg_subData,		/* Subscription data transfer */

  net_eMsg_sanAdd,		/* Add subscription */
  net_eMsg_sanRemove,		/* Remove subscription */
  net_eMsg_sanUpdate,		/* Subscription data transfer */

  net_eMsg_nameToObject,	/* Cache inquiry, fetch name */
  net_eMsg_oidToObject,		/* Cache inquiry, fetch objid */
  net_eMsg_objectR,		/* response */

  net_eMsg_getObjectInfo,
  net_eMsg_getObjectInfoR,
  net_eMsg_setObjectInfo,
  net_eMsg_setObjectInfoR,

  net_eMsg_flush,		/* Cache flush request */

  net_eMsg_createObject,
  net_eMsg_deleteObject,
  net_eMsg_moveObject,
  net_eMsg_renameObject,

  net_eMsg_nodeUp,		/* A node is up */
  net_eMsg_nodeDown,		/* A node is up */

  net_eMsg_getCclass,		/* Cached class info*/
  net_eMsg_getCclassR,

  net_eMsg_getGclass,		/* The whole class */
  net_eMsg_getGclassR,

  net_eMsg_serverConnect,       /* New server */

  net_eMsg_fileList,
  net_eMsg_fileListR,

  net_eMsg_,			/* Not a valid message */
  
  net_eMsg_volumes7,            /* Version 7. Internal only */

  net_eMsg_end
};

/* From rt_mh_net.x */
#define mh_cMsgClass      	201

enum mh_eMsg {
  mh_eMsg__			= 0,
  mh_eMsg_ApplConnect		= 1,
  mh_eMsg_ApplDisconnect	= 2,
  mh_eMsg_ApplGetMsgInfo	= 3,
  mh_eMsg_ApplMessage		= 4,
  mh_eMsg_ApplReply		= 5,
  mh_eMsg_ApplReturn		= 6,

  mh_eMsg_Event			= 8,

  mh_eMsg_HandlerDisconnect	= 10,
  mh_eMsg_HandlerHello		= 11,
  mh_eMsg_HandlerSync		= 12,

  mh_eMsg_OutunitAck		= 15,
  mh_eMsg_OutunitBlock		= 16,
  mh_eMsg_OutunitDisconnect	= 18,
  mh_eMsg_OutunitHello		= 19,
  mh_eMsg_OutunitInfo		= 20,
  mh_eMsg_OutunitSync		= 21,

  mh_eMsg_ProcDown		= 24,

  mh_eMsg_StopScanSup		= 25,
  mh_eMsg_StartScanSup		= 26,

  mh_eMsg_Sync			= 27,

  mh_eMsg_                      = 28
};

enum mh_eEvent {
  mh_eEvent__		= 0,    
  mh_eEvent_Ack		= 1,
  mh_eEvent_Block	= 2,
  mh_eEvent_Cancel	= 3,
  mh_eEvent_CancelBlock	= 4,
  mh_eEvent_Missing	= 5,
  mh_eEvent_Reblock	= 6,
  mh_eEvent_Return	= 7,
  mh_eEvent_Unblock	= 8,
  mh_eEvent_Info	= 32,
  mh_eEvent_Alarm	= 64,
  mh_eEvent_   		= 65
};

enum mh_eSupType {
  mh_eSupType__		= 0,
  mh_eSupType_Digital	= 1, 
  mh_eSupType_Analog	= 2,
  mh_eSupType_None	= 3,
  mh_eSupType_Link	= 4,
  mh_eSupType_Cycle	= 4,
  mh_eSupType_		= 5
};

/* From rt_sev_net.h */
#define sev_cMsgClass 202

typedef enum {
  sev_eMsgType_NodeUp,
  sev_eMsgType_HistItemsRequest,
  sev_eMsgType_HistItems,
  sev_eMsgType_HistDataStore,
  sev_eMsgType_HistDataGetRequest,
  sev_eMsgType_HistDataGet,
  sev_eMsgType_HistItemDelete,
  sev_eMsgType_HistItemStatus,
  sev_eMsgType_ServerStatusRequest,
  sev_eMsgType_ServerStatus,
  sev_eMsgType_HistObjectDataGetRequest,
  sev_eMsgType_HistObjectDataGet
} sev_eMsgType;
