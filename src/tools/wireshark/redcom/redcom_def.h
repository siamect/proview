
/* From rt_redcom.c */

typedef enum {
  eAction__ = 0,
  eAction_export,
  eAction_import,
  eAction_tmo,
  eAction_
} eAction;

typedef enum {
  eEvent__ = 0 ,
  eEvent_connect,
  eEvent_down,
  eEvent_user,
  eEvent_ack,
  eEvent_set_active,
  eEvent_set_passive,
  eEvent_set_off,
  eEvent_
} eEvent;

#define pwr_Bit(b) (1<<b)
#define mSeg__		0
#define mSeg_first	pwr_Bit(3)
#define mSeg_middle	pwr_Bit(4)
#define mSeg_last	pwr_Bit(5)
#define mSeg_single	(mSeg_first|mSeg_middle|mSeg_last)
#define mSeg_sequence	(mSeg_first|mSeg_middle|mSeg_last)
#define mSeg_bcast	pwr_Bit(8)

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


/* From rt_redu.h */
#define redu_cPort 56001
#define redu_cQixExport ((1<<31) | 4)
#define redu_cQixPrio1 ((1<<31) | 5)
#define redu_cQixPrio2 ((1<<31) | 6)
#define redu_cQixPrio3 ((1<<31) | 7)
#define redu_cQixPrio4 ((1<<31) | 8)
#define redu_cQixPrio5 ((1<<31) | 9)
#define redu_cQixPrio6 ((1<<31) | 10)
#define redu_cQixPrio7 ((1<<31) | 11)
#define redu_cQixPrio8 ((1<<31) | 12)
#define redu_cQixPrio9 ((1<<31) | 13)
#define redu_cQixPrio10 ((1<<31) | 14)

typedef enum {
  redu_eMsgType_Table,
  redu_eMsgType_Cyclic,
  redu_eMsgType_TableRequest,
  redu_eMsgType_TableVersionRequest,
  redu_eMsgType_TableVersion
} redu_eMsgType;

typedef enum {
  redu_ePrio_0 = 0,
  redu_ePrio_1 = 1,
  redu_ePrio_2 = 2,
  redu_ePrio_3 = 3,
  redu_ePrio_4 = 4,
  redu_ePrio_5 = 5,
  redu_ePrio_6 = 6,
  redu_ePrio_7 = 7,
  redu_ePrio_8 = 8,
  redu_ePrio_9 = 9,
  redu_ePrio_10 = 10,
  redu_ePrio__ = 11,
} redu_ePrio;

