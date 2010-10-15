/******************************************************************************

SOFTING AG
Richard-Reitzner-Allee 6
D-85540 Haar
Phone: ++49-89-4 56 56-0
Fax: ++49-89-4 56 56-3 99
http://www.softing.com

Copyright (C) SOFTING AG 2005-2009. All Rights Reserved

Version: 1.60.0.00.release

******************************************************************************/

#ifndef __PNAK_H__
#define __PNAK_H__

/*****************************************************************************/

#if defined WIN32 || defined _WIN32
  #pragma warning (disable : 4103)     /* used #pragma pack to change alignment */
  #pragma pack (push, 2)
  #pragma warning (default : 4103)

  #ifndef PACK_WORD_ALIGNMENT
    #define PACK_WORD_ALIGNMENT(Struct)   Struct
  #endif
  #ifndef PACK_BYTE_ALIGNMENT
    #define PACK_BYTE_ALIGNMENT(Struct)   Struct
  #endif

#else
  #ifdef __GNUC__
    #ifndef PACK_WORD_ALIGNMENT
      #define PACK_WORD_ALIGNMENT(Struct)   __attribute__ ((packed, aligned (2))) Struct
    #endif

    #ifndef PACK_BYTE_ALIGNMENT
      #define PACK_BYTE_ALIGNMENT(Struct)   __attribute__ ((packed, aligned (1))) Struct
    #endif
  #else
    #error CAUTION: set word structure alignment
  #endif
#endif

/******************************************************************************
ACCESS KIT USER INTERFACE
******************************************************************************/

#define MAX_NUMBER_SUPPORTED_DEVICES        (PN_U16) 256u

#define _BITSET256_RESET_BIT(pBS,BitNr)     (((pBS)) [(BitNr) >> 3] &= ~(1u << ((BitNr) & 7u)))
#define _BITSET256_SET_BIT(pBS,BitNr)       (((pBS)) [(BitNr) >> 3] |=   1u << ((BitNr) & 7u) )
#define _BITSET256_IS_BIT_SET(pBS,BitNr)    ((((pBS)) [(BitNr) >> 3]) &  (1u << ((BitNr) & 7u) ))

/*---------------------------------------------------------------------------*/

#ifndef _MAX
  #define _MAX(A,B)                             ((A) > (B) ? (A) : (B))
#endif

#ifndef _MIN
  #define _MIN(A,B)                             ((A) < (B) ? (A) : (B))
#endif

/*---------------------------------------------------------------------------*/

typedef enum _T_PNAK_RESULT
{
  PNAK_OK                                     = 0,

  /*-------------------------------------------------------------------------*/

  PNAK_NO_EXCEPTION_THROWN,
  PNAK_EXCEPTION_THROWN,

  PNAK_NO_NOTIFICATION_RECEIVED,
  PNAK_NOTIFICATION_RECEIVED,

  /*-------------------------------------------------------------------------*/

  PNAK_ERR_FATAL_ERROR,
  PNAK_ERR_INVALID_CHANNEL_ID,
  PNAK_ERR_CHANNEL_ALREADY_IN_USE,
  PNAK_ERR_CHANNEL_NOT_IN_USE,
  PNAK_ERR_INVALID_POINTER_VALUE,
  PNAK_ERR_SERVICE_IF_ALREADY_BUSY,
  PNAK_ERR_INVALID_DATA_SIZE,
  PNAK_ERR_INVALID_NUMBER_OF_SERVICES,
  PNAK_ERR_INVALID_SERVICE_LENGTH,
  PNAK_ERR_INVALID_SERVICE_OFFSET,
  PNAK_ERR_INVALID_PARAMETER,
  PNAK_ERR_MODE_DOES_NOT_SUPPORT_FUNCTION,
  PNAK_ERR_STARTUP_FAILES,
  PNAK_ERR_INVALID_IOCR,
  PNAK_ERR_IOCR_ACCESS_DENIED,
  PNAK_ERR_NOT_SUPPORTED,

  /*--- errors for host-controller-systems ----------------------------------*/

  PNAK_ERR_MEM_ALLOC_FAIL,
  PNAK_ERR_NO_PROTOCOL_SOFTWARE_RESPONSE,
  PNAK_ERR_INTERFACE_VERSION_CONFLICT,

  /*--- errors for snmp agents ----------------------------------------------*/

  PNAK_ERR_SNMP_TOO_BIG,
  PNAK_ERR_SNMP_NO_SUCH_NAME,
  PNAK_ERR_SNMP_BAD_VALUE,
  PNAK_ERR_SNMP_READ_ONLY,
  PNAK_ERR_SNMP_UNSPECIFIC,
  PNAK_ERR_SNMP_NO_ACCESS,
  PNAK_ERR_SNMP_WRONG_TYPE,
  PNAK_ERR_SNMP_WRONG_LENGTH,
  PNAK_ERR_SNMP_WRONG_ENCODING,
  PNAK_ERR_SNMP_WRONG_VALUE,
  PNAK_ERR_SNMP_NO_CREATION,
  PNAK_ERR_SNMP_INCONSISTENT_VALUE,
  PNAK_ERR_SNMP_RESOURCE_UNAVAILABLE,
  PNAK_ERR_SNMP_COMMIT_FAILED,
  PNAK_ERR_SNMP_UNDO_FAILED,
  PNAK_ERR_SNMP_AUTHORIZATION_ERROR,
  PNAK_ERR_SNMP_NOT_WRITABLE,
  PNAK_ERR_SNMP_INCONSISTENT_NAME,
  PNAK_ERR_SNMP_CONTEXT_NO_SUCH_OBJECT,
  PNAK_ERR_SNMP_CONTEXT_NO_SUCH_INSTANCE

} T_PNAK_RESULT;

/*---------------------------------------------------------------------------*/

typedef unsigned char     T_BITSET_256 [32];

/*---------------------------------------------------------------------------*/

typedef unsigned short    T_PNAK_EXCEPTION_SOURCE;

#define PNAK_EXCEPTION_SOURCE_IS_STACK        0u
#define PNAK_EXCEPTION_SOURCE_IS_PNAK         1u
#define PNAK_EXCEPTION_SOURCE_IS_APPL         2u

typedef struct _T_PNAK_EXCEPTION
{
  T_PNAK_EXCEPTION_SOURCE   Source;
  unsigned short            ChannelId;

  unsigned short            ModuleId;
  unsigned short            FileId;
  unsigned short            ErrorId;

  unsigned long             UserSpecificParameter;
  char                      UserString [22];

} PACK_WORD_ALIGNMENT (T_PNAK_EXCEPTION);

/*---------------------------------------------------------------------------*/

typedef unsigned long     T_PNAK_OID;

/*---------------------------------------------------------------------------*/

typedef unsigned short    T_PNAK_MODE_ID;

#define PNAK_UNINITIALIZED_MODE    0
#define PNAK_CONTROLLER_MODE       1
#define PNAK_DEVICE_MODE           2
#define PNAK_SUPERVISOR_MODE       3

/*---------------------------------------------------------------------------*/

#define PNAK_VERSION_STRING_LENGTH   42

typedef struct _T_PNAK_VERSION
{
  unsigned short    IfId;
  unsigned short    HostIfId;
  unsigned short    HostConfigIfId;
  unsigned short    ExceptionIfId;
  unsigned short    ServiceIfId;
  unsigned short    EventIfId;
  unsigned short    StatisticIfId;
  unsigned short    DataIfId;

  char              VersionString [PNAK_VERSION_STRING_LENGTH];

} PACK_WORD_ALIGNMENT (T_PNAK_VERSION);

typedef struct _T_PNAK_VERSIONS
{
  T_PNAK_VERSION    Firmware;
  T_PNAK_VERSION    Pnak;

} PACK_WORD_ALIGNMENT (T_PNAK_VERSIONS);

/*---------------------------------------------------------------------------*/

#define PNAK_INFINITE_TIMEOUT                         0xFFFFFFFFuL
#define PNAK_MAX_NUMBER_WAIT_OBJECTS                  32uL

typedef unsigned long                                 T_PNAK_TIMEOUT;
typedef unsigned long                                 T_PNAK_WAIT_OBJECT;

#define PNAK_WAIT_OBJECT_EXCEPTION                    ((T_PNAK_WAIT_OBJECT) 0x00000001uL)
#define PNAK_WAIT_OBJECT_STATE_CHANGED                ((T_PNAK_WAIT_OBJECT) 0x00000002uL)
#define PNAK_WAIT_OBJECT_ALARM                        ((T_PNAK_WAIT_OBJECT) 0x00000004uL)
#define PNAK_WAIT_OBJECT_ALARM_ACK                    ((T_PNAK_WAIT_OBJECT) 0x00000008uL)
#define PNAK_WAIT_OBJECT_DEVICE_STATE_CHANGED         ((T_PNAK_WAIT_OBJECT) 0x00000010uL)
#define PNAK_WAIT_OBJECT_ETHERNET_STATE_CHANGED       ((T_PNAK_WAIT_OBJECT) 0x00000020uL)
#define PNAK_WAIT_OBJECT_PROVIDER_DATA_UPDATED        ((T_PNAK_WAIT_OBJECT) 0x00000040uL)
#define PNAK_WAIT_OBJECT_CONSUMER_DATA_CHANGED        ((T_PNAK_WAIT_OBJECT) 0x00000080uL)
#define PNAK_WAIT_OBJECT_SERVICE_IND                  ((T_PNAK_WAIT_OBJECT) 0x00000100uL)
#define PNAK_WAIT_OBJECT_SERVICE_CON                  ((T_PNAK_WAIT_OBJECT) 0x00000200uL)
#define PNAK_WAIT_OBJECT_SERVICE_REQ_RES_HANDLED      ((T_PNAK_WAIT_OBJECT) 0x00000400uL)
#define PNAK_WAIT_OBJECT_CHANNEL_CLOSED               ((T_PNAK_WAIT_OBJECT) 0x00200000uL)
#define PNAK_WAIT_OBJECT_INTERRUPTED                  ((T_PNAK_WAIT_OBJECT) 0x00400000uL)
#define PNAK_USER_WAIT_OBJECT_1                       ((T_PNAK_WAIT_OBJECT) 0x00800000uL)
#define PNAK_USER_WAIT_OBJECT_2                       ((T_PNAK_WAIT_OBJECT) 0x01000000uL)
#define PNAK_USER_WAIT_OBJECT_3                       ((T_PNAK_WAIT_OBJECT) 0x02000000uL)
#define PNAK_USER_WAIT_OBJECT_4                       ((T_PNAK_WAIT_OBJECT) 0x04000000uL)
#define PNAK_USER_WAIT_OBJECT_5                       ((T_PNAK_WAIT_OBJECT) 0x08000000uL)
#define PNAK_USER_WAIT_OBJECT_6                       ((T_PNAK_WAIT_OBJECT) 0x10000000uL)
#define PNAK_USER_WAIT_OBJECT_7                       ((T_PNAK_WAIT_OBJECT) 0x20000000uL)
#define PNAK_USER_WAIT_OBJECT_8                       ((T_PNAK_WAIT_OBJECT) 0x40000000uL)
#define PNAK_WAIT_TIMEOUT                             ((T_PNAK_WAIT_OBJECT) 0x80000000uL)

#define PNAK_WAIT_OBJECTS_SERVICE                     (PNAK_WAIT_OBJECT_SERVICE_CON              | \
                                                       PNAK_WAIT_OBJECT_SERVICE_IND              | \
                                                       PNAK_WAIT_OBJECT_SERVICE_REQ_RES_HANDLED)
#define PNAK_WAIT_OBJECTS_EVENT_IND                   (PNAK_WAIT_OBJECT_STATE_CHANGED            | \
                                                       PNAK_WAIT_OBJECT_ALARM                    | \
                                                       PNAK_WAIT_OBJECT_ALARM_ACK                | \
                                                       PNAK_WAIT_OBJECT_DEVICE_STATE_CHANGED     | \
                                                       PNAK_WAIT_OBJECT_ETHERNET_STATE_CHANGED)
#define PNAK_WAIT_OBJECTS_DATA_IND                    (PNAK_WAIT_OBJECT_PROVIDER_DATA_UPDATED    |\
                                                       PNAK_WAIT_OBJECT_CONSUMER_DATA_CHANGED)
#define PNAK_WAIT_OBJECTS_OTHER                       (PNAK_WAIT_OBJECT_EXCEPTION                | \
                                                       PNAK_WAIT_OBJECT_CHANNEL_CLOSED           | \
                                                       PNAK_WAIT_OBJECT_INTERRUPTED)
#define PNAK_WAIT_OBJECTS_USER                        (PNAK_USER_WAIT_OBJECT_1                   | \
                                                       PNAK_USER_WAIT_OBJECT_2                   | \
                                                       PNAK_USER_WAIT_OBJECT_3                   | \
                                                       PNAK_USER_WAIT_OBJECT_4                   | \
                                                       PNAK_USER_WAIT_OBJECT_5                   | \
                                                       PNAK_USER_WAIT_OBJECT_6                   | \
                                                       PNAK_USER_WAIT_OBJECT_7                   | \
                                                       PNAK_USER_WAIT_OBJECT_8)
#define PNAK_WAIT_OBJECTS_ALL                         (PNAK_WAIT_OBJECTS_OTHER                   | \
                                                       PNAK_WAIT_OBJECTS_EVENT_IND               | \
                                                       PNAK_WAIT_OBJECTS_DATA_IND                | \
                                                       PNAK_WAIT_OBJECTS_SERVICE                 | \
                                                       PNAK_WAIT_OBJECTS_USER                    | \
                                                       PNAK_WAIT_TIMEOUT)

/*=============================================================================
SERVICE INTERFACE
=============================================================================*/

typedef unsigned char     T_PNAK_SERVICE_PRIMITIVE;

#define PNAK_SERVICE_REQ       (T_PNAK_SERVICE_PRIMITIVE) 0x01u
#define PNAK_SERVICE_RES       (T_PNAK_SERVICE_PRIMITIVE) 0x02u
#define PNAK_SERVICE_CON       (T_PNAK_SERVICE_PRIMITIVE) 0x03u
#define PNAK_SERVICE_IND       (T_PNAK_SERVICE_PRIMITIVE) 0x04u

typedef unsigned char     T_PNAK_SERVICE_RESULT;

#define PNAK_RESULT_POS        (T_PNAK_SERVICE_RESULT) 0x00u
#define PNAK_RESULT_NEG        (T_PNAK_SERVICE_RESULT) 0x01u

typedef struct _T_PNAK_SERVICE_DESCRIPTION
{
  unsigned short              DeviceRef;

  unsigned char               Instance;
  unsigned char               Service;

  T_PNAK_SERVICE_PRIMITIVE    Primitive;
  T_PNAK_SERVICE_RESULT       Result;

  unsigned short              ClientId;
  unsigned short              InvokeId;

  unsigned short              DataLength;

} PACK_WORD_ALIGNMENT (T_PNAK_SERVICE_DESCRIPTION);


/*---------------------------------------------------------------------------*/

#define PNAK_MAX_NUMBER_REQ_SERVICES           52u
#define PNAK_MAX_NUMBER_CON_SERVICES           52u
#define PNAK_MAX_NUMBER_IND_SERVICES           52u

#define PNAK_SERVICE_REQ_CHANNEL_SIZE          (PN_U16) 8086u
#define PNAK_SERVICE_CON_CHANNEL_SIZE          (PN_U16) 8086u
#define PNAK_SERVICE_IND_CHANNEL_SIZE          (PN_U16) 8086u

/*=== SERVICE REQUEST/RESPONSE ==============================================*/

typedef struct _T_PNAK_SERVICE_REQ_RES_ENTRY
{
  unsigned short    ServiceOffset;

} PACK_WORD_ALIGNMENT (T_PNAK_SERVICE_REQ_RES_ENTRY);

/*---------------------------------------------------------------------------*/

typedef struct _T_PNAK_SERVICE_REQ_RES
{
  unsigned short                  NumberEntries;

  T_PNAK_SERVICE_REQ_RES_ENTRY    ServiceEntry [PNAK_MAX_NUMBER_REQ_SERVICES];
  unsigned char                   ServiceChannel [PNAK_SERVICE_REQ_CHANNEL_SIZE];

} PACK_WORD_ALIGNMENT (T_PNAK_SERVICE_REQ_RES);

/*=== SERVICE CONFIRMATION ==================================================*/

typedef struct _T_PNAK_SERVICE_CON_ENTRY
{
  unsigned short    ServiceOffset;

} PACK_WORD_ALIGNMENT (T_PNAK_SERVICE_CON_ENTRY);

/*---------------------------------------------------------------------------*/

typedef struct _T_PNAK_SERVICE_CON
{
  unsigned short              NumberEntries;

  T_PNAK_SERVICE_CON_ENTRY    ServiceEntry [PNAK_MAX_NUMBER_CON_SERVICES];
  unsigned char               ServiceChannel [PNAK_SERVICE_CON_CHANNEL_SIZE];

} PACK_WORD_ALIGNMENT (T_PNAK_SERVICE_CON);

/*=== SERVICE INDICATION ====================================================*/

typedef struct _T_PNAK_SERVICE_IND_ENTRY
{
  unsigned short    ServiceOffset;

} PACK_WORD_ALIGNMENT (T_PNAK_SERVICE_IND_ENTRY);

/*---------------------------------------------------------------------------*/

typedef struct _T_PNAK_SERVICE_IND
{
  unsigned short              NumberEntries;

  T_PNAK_SERVICE_IND_ENTRY    ServiceEntry [PNAK_MAX_NUMBER_IND_SERVICES];
  unsigned char               ServiceChannel [PNAK_SERVICE_IND_CHANNEL_SIZE];

} PACK_WORD_ALIGNMENT (T_PNAK_SERVICE_IND);

/*=============================================================================
EVENT INTERFACE
=============================================================================*/

typedef unsigned short            T_PNAK_MODE;

#define PNAK_MODE_OFFLINE         (T_PNAK_MODE) 0u
#define PNAK_MODE_ONLINE          (T_PNAK_MODE) 1u

/*=== EVENT REQUEST ==========================================================*/

typedef struct _T_PNAK_EVENT_SET_MODE
{
  T_PNAK_MODE   Mode;

} PACK_WORD_ALIGNMENT (T_PNAK_EVENT_SET_MODE);

/*---------------------------------------------------------------------------*/

typedef struct _T_PNAK_EVENT_SET_DEVICE_STATE
{
  T_BITSET_256   ActivateDeviceReference;
  T_BITSET_256   DeactivateDeviceReference;

} PACK_WORD_ALIGNMENT (T_PNAK_EVENT_SET_DEVICE_STATE);

/*=== EVENT INDICATION ======================================================*/

typedef enum _T_PNAK_STATE
{
  PNAK_STATE_OK = 0,
  PNAK_STATE_NO_PARAMETER,
  PNAK_STATE_DOUBLE_ADDRESS_ERROR,
  PNAK_STATE_HARDWARE_ERROR,
  PNAK_STATE_APPL_WATCHDOG_EXPIRED,
  PNAK_STATE_ETH_FAILURE,
  PNAK_STATE_UDP_FAILURE,
  PNAK_STATE_DUPLICATE_NAME_ERROR

} T_PNAK_STATE;

/*---------------------------------------------------------------------------*/

typedef struct _T_PNAK_EVENT_STATE
{
  T_PNAK_MODE     Mode;
  T_PNAK_STATE    State;

} PACK_WORD_ALIGNMENT (T_PNAK_EVENT_STATE);

/*---------------------------------------------------------------------------*/

typedef struct _T_PNAK_EVENT_ALARM
{
  T_BITSET_256   DeviceReference;

} PACK_WORD_ALIGNMENT (T_PNAK_EVENT_ALARM);

/*---------------------------------------------------------------------------*/

typedef struct _T_PNAK_EVENT_ALARM_ACK
{
  T_BITSET_256   DeviceReference;

} PACK_WORD_ALIGNMENT (T_PNAK_EVENT_ALARM_ACK);

/*---------------------------------------------------------------------------*/

#define PNAK_DEVICE_STATE_DEACTIVATED                 0x00u
#define PNAK_DEVICE_STATE_FIND_DEVICE                 0x01u
#define PNAK_DEVICE_STATE_CONNECTION_ESTABLISHMENT    0x02u
#define PNAK_DEVICE_STATE_CONNECTED                   0x04u
#define PNAK_DEVICE_STATE_CONNECT_ERROR               0x10u
#define PNAK_DEVICE_STATE_WRITE_ERROR                 0x20u
#define PNAK_DEVICE_STATE_CONTROL_ERROR               0x40u
#define PNAK_DEVICE_STATE_CLOSE_PENDING               0x80u
#define PNAK_DEVICE_STATE_ERROR_MASK                  (PNAK_DEVICE_STATE_CONNECT_ERROR | \
                                                      PNAK_DEVICE_STATE_WRITE_ERROR    | \
                                                      PNAK_DEVICE_STATE_CONTROL_ERROR)

typedef struct _T_PNAK_EVENT_DEVICE_STATE
{
  unsigned char   State [MAX_NUMBER_SUPPORTED_DEVICES];

} PACK_WORD_ALIGNMENT (T_PNAK_EVENT_DEVICE_STATE);

/*=== EVENT INDICATION MASK =================================================*/

#define PNAK_EVENT_STATE_CHANGED              0x00000001uL
#define PNAK_EVENT_DEVICE_STATE_CHANGED       0x00000002uL
#define PNAK_EVENT_ALARM                      0x00000004uL
#define PNAK_EVENT_MASK                       (PNAK_EVENT_STATE_CHANGED | \
                                              PNAK_EVENT_ALARM          | \
                                              PNAK_EVENT_DEVICE_STATE_CHANGED)
typedef unsigned long   T_PNAK_EVENT_REG;

typedef struct _T_PNAK_EVENT_IND_MASK
{
  T_PNAK_EVENT_REG   EventMaskReg;

} PACK_WORD_ALIGNMENT (T_PNAK_EVENT_IND_MASK);

/*=============================================================================
DATA INTERFACE
=============================================================================*/

/*=== DATA INDICATION =======================================================*/

typedef struct _T_PNAK_DATA_CONSUMER_DATA_CHANGED
{
  T_BITSET_256    DeviceReference;

} PACK_WORD_ALIGNMENT (T_PNAK_DATA_CONSUMER_DATA_CHANGED);

/*---------------------------------------------------------------------------*/

typedef struct _T_PNAK_DATA_PROVIDER_DATA_UPDATED
{
  T_BITSET_256    DeviceReference;

} PACK_WORD_ALIGNMENT (T_PNAK_DATA_PROVIDER_DATA_UPDATED);

/*=== DATA INDICATION MASK ==================================================*/

#define PNAK_DATA_EVENT_CYCLIC_TRANSFER_FINISHED    0x00000001uL
#define PNAK_DATA_EVENT_CONSUMER_DATA_CHANGED       0x00000002uL
#define PNAK_DATA_EVENT_PROVIDER_DATA_UPDATED       0x00000004uL

typedef unsigned long   T_PNAK_DATA_EVENT_REG;

typedef struct _T_PNAK_DATA_IND_MASK
{
  T_PNAK_DATA_EVENT_REG    EventMaskReg;

} PACK_WORD_ALIGNMENT (T_PNAK_DATA_IND_MASK);

/*---------------------------------------------------------------------------*/

#define PNAK_IOXS_STATUS_NO_EXTENTION_FOLLOWS     0x00u
#define PNAK_IOXS_STATUS_DETECTED_BY_SUBSLOT      0x00u
#define PNAK_IOXS_STATUS_DETECTED_BY_SLOT         0x20u
#define PNAK_IOXS_STATUS_DETECTED_BY_DEVICE       0x40u
#define PNAK_IOXS_STATUS_DETECTED_BY_CONTROLLER   0x60u
#define PNAK_IOXS_STATUS_DATA_BAD                 0x00u
#define PNAK_IOXS_STATUS_DATA_GOOD                0x80u

/*---------------------------------------------------------------------------*/

/* now we support only one controller-device and
 * one supervisor-device connection, if we want later
 * also support further connections we must increase
 * the max number and add specific statemachines in the
 * object data structure
 */
#define MAX_NUMBER_CMDEV_CONNECTIONS    2u

/******************************************************************************
FUNCTION DECLARATION
******************************************************************************/

#ifdef __cplusplus
  extern "C" {
#endif

/*--- initialization/termination functions ----------------------------------*/

extern void             pnak_init                                 (void);
extern void             pnak_term                                 (void);

extern T_PNAK_RESULT    pnak_start_profistack                     (unsigned short, const T_PNAK_MODE_ID);
extern T_PNAK_RESULT    pnak_stop_profistack                      (unsigned short);

/*--- General interface functions -------------------------------------------*/

extern T_PNAK_RESULT    pnak_wait_for_multiple_objects            (unsigned short, T_PNAK_WAIT_OBJECT*, T_PNAK_TIMEOUT);
extern T_PNAK_RESULT    pnak_set_multiple_objects                 (unsigned short, const T_PNAK_WAIT_OBJECT);
extern T_PNAK_RESULT    pnak_mask_multiple_objects                (unsigned short, const T_PNAK_WAIT_OBJECT);

extern T_PNAK_RESULT    pnak_get_exception                        (unsigned short, T_PNAK_EXCEPTION*);
extern T_PNAK_RESULT    pnak_get_version                          (unsigned short, T_PNAK_VERSIONS*);

extern T_PNAK_RESULT    pnak_snmp_get_data                        (unsigned short, T_PNAK_OID*, void*, unsigned long*, unsigned long*, unsigned char, PN_BOOL);
extern T_PNAK_RESULT    pnak_snmp_check_data                      (unsigned short, T_PNAK_OID*, void*, unsigned long, unsigned long, unsigned char);
extern T_PNAK_RESULT    pnak_snmp_set_data                        (unsigned short, T_PNAK_OID*, void*, unsigned long, unsigned long);

/*--- Service interface functions -------------------------------------------*/

extern T_PNAK_RESULT    pnak_send_service_req_res                 (unsigned short, const T_PNAK_SERVICE_REQ_RES*);
extern T_PNAK_RESULT    pnak_get_service_ind                      (unsigned short, T_PNAK_SERVICE_IND*);
extern T_PNAK_RESULT    pnak_get_service_con                      (unsigned short, T_PNAK_SERVICE_CON*);

/*--- Event interface functions ---------------------------------------------*/

extern T_PNAK_RESULT    pnak_set_mode                             (unsigned short, const T_PNAK_EVENT_SET_MODE*);
extern T_PNAK_RESULT    pnak_set_device_state                     (unsigned short, const T_PNAK_EVENT_SET_DEVICE_STATE*);

extern T_PNAK_RESULT    pnak_get_state                            (unsigned short, T_PNAK_EVENT_STATE*);
extern T_PNAK_RESULT    pnak_get_device_state_ind                 (unsigned short, T_PNAK_EVENT_DEVICE_STATE*);
extern T_PNAK_RESULT    pnak_get_alarm_ind                        (unsigned short, T_PNAK_EVENT_ALARM*);
extern T_PNAK_RESULT    pnak_get_alarm_ack_ind                    (unsigned short, T_PNAK_EVENT_ALARM_ACK*);

/*--- Data interface functions ----------------------------------------------*/

extern T_PNAK_RESULT    pnak_set_iocr_data                        (unsigned short, unsigned short, const unsigned char*, unsigned short);

extern T_PNAK_RESULT    pnak_get_iocr_data                        (unsigned short, unsigned short, unsigned char*, unsigned short*, unsigned char*);
extern T_PNAK_RESULT    pnak_get_consumer_data_changed_ind        (unsigned short, T_PNAK_DATA_CONSUMER_DATA_CHANGED*);
extern T_PNAK_RESULT    pnak_get_provider_data_updated            (unsigned short, T_PNAK_DATA_PROVIDER_DATA_UPDATED*);

/*=============================================================================
CALLBACK INTERFACE
=============================================================================*/

typedef int             (*T_PNAK_WAIT_OBJECT_CALLBACK)            (unsigned short, T_PNAK_WAIT_OBJECT);
typedef int             (*T_PNAK_HW_START_CALLBACK)               (unsigned short, T_PNAK_MODE_ID, void*);

typedef void            (*T_PROVIDER_CALLBACK)                    (unsigned short, unsigned short, unsigned char*, unsigned short);
typedef void            (*T_CONSUMER_CALLBACK)                    (unsigned short, unsigned short, const unsigned char*, unsigned short, unsigned char);

extern T_PNAK_RESULT    pnak_register_provider_callback           (unsigned short, T_PROVIDER_CALLBACK);
extern T_PNAK_RESULT    pnak_unregister_provider_callback         (unsigned short);

extern T_PNAK_RESULT    pnak_register_consumer_callback           (unsigned short, T_CONSUMER_CALLBACK);
extern T_PNAK_RESULT    pnak_unregister_consumer_callback         (unsigned short);

#ifdef __cplusplus
  }
#endif

/*****************************************************************************/

#if defined WIN32 || defined _WIN32
  #pragma warning (disable : 4103)     /* used #pragma pack to reset alignment */
  #pragma pack(pop)
  #pragma warning (default : 4103)
#else
  #ifdef __GNUC__
    /* nothing to do */
  #else
    #error CAUTION: set default structure alignment
  #endif
#endif

/*****************************************************************************/

#endif /* __PNAK_H__ */

/*****************************************************************************/
/* vim: set nu ts=2 et nowrap: */
