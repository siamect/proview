/******************************************************************************

SOFTING Industrial Automation GmbH
Richard-Reitzner-Allee 6
D-85540 Haar
Phone: ++49-89-4 56 56-0
Fax: ++49-89-4 56 56-3 99
http://www.softing.com

Copyright (C) SOFTING Industrial Automation GmbH 2005-2016. All Rights Reserved

Version: 2.22.00

******************************************************************************/

#ifdef __OVERRIDE_GLOBAL_CONFIG_HEADER__
#include __OVERRIDE_GLOBAL_CONFIG_HEADER__
#endif

#ifndef __PNAK_H__
#define __PNAK_H__

/*****************************************************************************/

#if defined WIN32 || defined _WIN32
#pragma warning(disable : 4103) /* used #pragma pack to change alignment */
#pragma pack(push, 2)
#pragma warning(default : 4103)

#ifndef PACK_WORD_ALIGNMENT
#define PACK_WORD_ALIGNMENT(Struct) Struct
#endif
#ifndef PACK_BYTE_ALIGNMENT
#define PACK_BYTE_ALIGNMENT(Struct) Struct
#endif

#elif defined __GNUC__
#ifndef PACK_WORD_ALIGNMENT
#define PACK_WORD_ALIGNMENT(Struct) __attribute__((packed, aligned(2))) Struct
#endif

#ifndef PACK_BYTE_ALIGNMENT
#define PACK_BYTE_ALIGNMENT(Struct) __attribute__((packed, aligned(1))) Struct
#endif

#elif defined _OS9000

#ifndef PACK_WORD_ALIGNMENT
#define PACK_WORD_ALIGNMENT(Struct) Struct
#endif
#ifndef PACK_BYTE_ALIGNMENT
#define PACK_BYTE_ALIGNMENT(Struct) Struct
#endif

#else
#error CAUTION: set word structure alignment
#endif

/******************************************************************************
ACCESS KIT USER INTERFACE
******************************************************************************/

#define _BITSET256_RESET_BIT(pBS, BitNr)                                       \
  (((pBS))[(BitNr) >> 3] &= ~(1u << ((BitNr)&7u)))
#define _BITSET256_SET_BIT(pBS, BitNr)                                         \
  (((pBS))[(BitNr) >> 3] |= 1u << ((BitNr)&7u))
#define _BITSET256_IS_BIT_SET(pBS, BitNr)                                      \
  ((((pBS))[(BitNr) >> 3]) & (1u << ((BitNr)&7u)))

#define _BITSET64_RESET_BIT(pBS, BitNr)                                        \
  (((pBS))[(BitNr) >> 3] &= ~(1u << ((BitNr)&7u)))
#define _BITSET64_SET_BIT(pBS, BitNr)                                          \
  (((pBS))[(BitNr) >> 3] |= 1u << ((BitNr)&7u))
#define _BITSET64_IS_BIT_SET(pBS, BitNr)                                       \
  ((((pBS))[(BitNr) >> 3]) & (1u << ((BitNr)&7u)))

/*---------------------------------------------------------------------------*/

#ifndef _MAX
#define _MAX(A, B) ((A) > (B) ? (A) : (B))
#endif

#ifndef _MIN
#define _MIN(A, B) ((A) < (B) ? (A) : (B))
#endif

/*---------------------------------------------------------------------------*/

typedef enum _T_PNAK_RESULT {
  PNAK_OK = 0,

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
  PNAK_ERR_SNMP_CONTEXT_NO_SUCH_INSTANCE,

  /*--- errors for watchdog handling ----------------------------------------*/

  PNAK_ERR_WATCHDOG_EXPIRED,
  PNAK_ERR_WATCHDOG_ACCESS_DENIED,
  PNAK_ERR_INVALID_WATCHDOG_UPDATE_COUNT,

  /*--- errors of hardware checks -------------------------------------------*/

  PNAK_ERR_HW_FLASH_CKECK_FAILED,
  PNAK_ERR_HW_RAM_CKECK_FAILED,

  /*--- errors loading library    -------------------------------------------*/
  PNAK_ERR_LIBRARY_NOT_LOADED,

} T_PNAK_RESULT;

/*---------------------------------------------------------------------------*/

typedef unsigned char T_BITSET_256[32];
typedef unsigned char T_BITSET_64[8];

/*---------------------------------------------------------------------------*/

typedef unsigned short T_PNAK_EXCEPTION_SOURCE;

#define PNAK_EXCEPTION_SOURCE_IS_STACK 0u
#define PNAK_EXCEPTION_SOURCE_IS_PNAK 1u
#define PNAK_EXCEPTION_SOURCE_IS_APPL 2u

typedef struct _T_PNAK_EXCEPTION {
  T_PNAK_EXCEPTION_SOURCE Source;
  unsigned short ChannelId;

  unsigned short ModuleId;
  unsigned short FileId;
  unsigned short ErrorId;

  PN_U32 UserSpecificParameter;
  char UserString[22];

} PACK_WORD_ALIGNMENT(T_PNAK_EXCEPTION);

/*---------------------------------------------------------------------------*/

typedef unsigned long T_PNAK_OID;

/*---------------------------------------------------------------------------*/

typedef unsigned short T_PNAK_MODE_ID;

#define PNAK_UNINITIALIZED_MODE 0
#define PNAK_CONTROLLER_MODE 1
#define PNAK_DEVICE_MODE 2
#define PNAK_SUPERVISOR_MODE 3

/*---------------------------------------------------------------------------*/

#define PNAK_VERSION_STRING_LENGTH 42

typedef struct _T_PNAK_VERSION {
  unsigned short IfId;
  unsigned short HostIfId;
  unsigned short HostConfigIfId;
  unsigned short ExceptionIfId;
  unsigned short ServiceIfId;
  unsigned short EventIfId;
  unsigned short StatisticIfId;
  unsigned short DataIfId;

  char VersionString[PNAK_VERSION_STRING_LENGTH];

} PACK_WORD_ALIGNMENT(T_PNAK_VERSION);

typedef struct _T_PNAK_VERSIONS {
  T_PNAK_VERSION Firmware;
  T_PNAK_VERSION Pnak;

} PACK_WORD_ALIGNMENT(T_PNAK_VERSIONS);

/*---------------------------------------------------------------------------*/

#define PNAK_INFINITE_TIMEOUT 0xFFFFFFFFuL
#define PNAK_MAX_NUMBER_WAIT_OBJECTS 32uL

typedef PN_U32 T_PNAK_TIMEOUT;
typedef PN_U32 T_PNAK_WAIT_OBJECT;

#define PNAK_WAIT_OBJECT_EXCEPTION ((T_PNAK_WAIT_OBJECT)0x00000001uL)
#define PNAK_WAIT_OBJECT_STATE_CHANGED ((T_PNAK_WAIT_OBJECT)0x00000002uL)
#define PNAK_WAIT_OBJECT_ALARM ((T_PNAK_WAIT_OBJECT)0x00000004uL)
#define PNAK_WAIT_OBJECT_ALARM_ACK ((T_PNAK_WAIT_OBJECT)0x00000008uL)
#define PNAK_WAIT_OBJECT_DEVICE_STATE_CHANGED ((T_PNAK_WAIT_OBJECT)0x00000010uL)
#define PNAK_WAIT_OBJECT_ETHERNET_STATE_CHANGED                                \
  ((T_PNAK_WAIT_OBJECT)0x00000020uL)
#define PNAK_WAIT_OBJECT_PROVIDER_DATA_UPDATED                                 \
  ((T_PNAK_WAIT_OBJECT)0x00000040uL)
#define PNAK_WAIT_OBJECT_CONSUMER_DATA_CHANGED                                 \
  ((T_PNAK_WAIT_OBJECT)0x00000080uL)
#define PNAK_WAIT_OBJECT_SERVICE_IND ((T_PNAK_WAIT_OBJECT)0x00000100uL)
#define PNAK_WAIT_OBJECT_SERVICE_CON ((T_PNAK_WAIT_OBJECT)0x00000200uL)
#define PNAK_WAIT_OBJECT_SERVICE_REQ_RES_HANDLED                               \
  ((T_PNAK_WAIT_OBJECT)0x00000400uL)
#define PNAK_WAIT_OBJECT_PTCP ((T_PNAK_WAIT_OBJECT)0x00000800uL)
#define PNAK_WAIT_OBJECT_SOCKET_STATE_CHANGED ((T_PNAK_WAIT_OBJECT)0x00001000uL)
#define PNAK_WAIT_OBJECT_SOCKET_DATA_RECEIVED ((T_PNAK_WAIT_OBJECT)0x00002000uL)
#define PNAK_WAIT_OBJECT_MRPD ((T_PNAK_WAIT_OBJECT)0x00004000uL)
#define PNAK_WAIT_OBJECT_CHANNEL_CLOSED ((T_PNAK_WAIT_OBJECT)0x00200000uL)
#define PNAK_WAIT_OBJECT_INTERRUPTED ((T_PNAK_WAIT_OBJECT)0x00400000uL)
#define PNAK_USER_WAIT_OBJECT_1 ((T_PNAK_WAIT_OBJECT)0x00800000uL)
#define PNAK_USER_WAIT_OBJECT_2 ((T_PNAK_WAIT_OBJECT)0x01000000uL)
#define PNAK_USER_WAIT_OBJECT_3 ((T_PNAK_WAIT_OBJECT)0x02000000uL)
#define PNAK_USER_WAIT_OBJECT_4 ((T_PNAK_WAIT_OBJECT)0x04000000uL)
#define PNAK_USER_WAIT_OBJECT_5 ((T_PNAK_WAIT_OBJECT)0x08000000uL)
#define PNAK_USER_WAIT_OBJECT_6 ((T_PNAK_WAIT_OBJECT)0x10000000uL)
#define PNAK_USER_WAIT_OBJECT_7 ((T_PNAK_WAIT_OBJECT)0x20000000uL)
#define PNAK_USER_WAIT_OBJECT_8 ((T_PNAK_WAIT_OBJECT)0x40000000uL)
#define PNAK_WAIT_TIMEOUT ((T_PNAK_WAIT_OBJECT)0x80000000uL)

#define PNAK_WAIT_OBJECTS_SERVICE                                              \
  (PNAK_WAIT_OBJECT_SERVICE_CON | PNAK_WAIT_OBJECT_SERVICE_IND                 \
      | PNAK_WAIT_OBJECT_SERVICE_REQ_RES_HANDLED)
#define PNAK_WAIT_OBJECTS_EVENT_IND                                            \
  (PNAK_WAIT_OBJECT_STATE_CHANGED | PNAK_WAIT_OBJECT_ALARM                     \
      | PNAK_WAIT_OBJECT_ALARM_ACK | PNAK_WAIT_OBJECT_DEVICE_STATE_CHANGED     \
      | PNAK_WAIT_OBJECT_ETHERNET_STATE_CHANGED | PNAK_WAIT_OBJECT_PTCP        \
      | PNAK_WAIT_OBJECT_MRPD)
#define PNAK_WAIT_OBJECTS_DATA_IND                                             \
  (PNAK_WAIT_OBJECT_PROVIDER_DATA_UPDATED                                      \
      | PNAK_WAIT_OBJECT_CONSUMER_DATA_CHANGED)
#define PNAK_WAIT_OBJECTS_SOCKET_IND                                           \
  (PNAK_WAIT_OBJECT_SOCKET_STATE_CHANGED                                       \
      | PNAK_WAIT_OBJECT_SOCKET_DATA_RECEIVED)
#define PNAK_WAIT_OBJECTS_OTHER                                                \
  (PNAK_WAIT_OBJECT_EXCEPTION | PNAK_WAIT_OBJECT_CHANNEL_CLOSED                \
      | PNAK_WAIT_OBJECT_INTERRUPTED)
#define PNAK_WAIT_OBJECTS_USER                                                 \
  (PNAK_USER_WAIT_OBJECT_1 | PNAK_USER_WAIT_OBJECT_2 | PNAK_USER_WAIT_OBJECT_3 \
      | PNAK_USER_WAIT_OBJECT_4 | PNAK_USER_WAIT_OBJECT_5                      \
      | PNAK_USER_WAIT_OBJECT_6 | PNAK_USER_WAIT_OBJECT_7                      \
      | PNAK_USER_WAIT_OBJECT_8)
#define PNAK_WAIT_OBJECTS_ALL                                                  \
  (PNAK_WAIT_OBJECTS_OTHER | PNAK_WAIT_OBJECTS_EVENT_IND                       \
      | PNAK_WAIT_OBJECTS_DATA_IND | PNAK_WAIT_OBJECTS_SERVICE                 \
      | PNAK_WAIT_OBJECTS_SOCKET_IND | PNAK_WAIT_OBJECTS_USER                  \
      | PNAK_WAIT_TIMEOUT)

/*---------------------------------------------------------------------------*/

/* now we support only 2 controller-device, 1 configuration in run and
 * 1 supervisor-device connection, if we want later
 * also support further connections we must increase
 * the max number and add specific statemachines in the
 * object data structure
 */
#define MAX_NUMBER_CMDEV_CONNECTIONS 4u

/*=============================================================================
SERVICE INTERFACE
=============================================================================*/

typedef unsigned char T_PNAK_SERVICE_PRIMITIVE;

#define PNAK_SERVICE_REQ (T_PNAK_SERVICE_PRIMITIVE)0x01u
#define PNAK_SERVICE_RES (T_PNAK_SERVICE_PRIMITIVE)0x02u
#define PNAK_SERVICE_CON (T_PNAK_SERVICE_PRIMITIVE)0x03u
#define PNAK_SERVICE_IND (T_PNAK_SERVICE_PRIMITIVE)0x04u

typedef unsigned char T_PNAK_SERVICE_RESULT;

#define PNAK_RESULT_POS (T_PNAK_SERVICE_RESULT)0x00u
#define PNAK_RESULT_NEG (T_PNAK_SERVICE_RESULT)0x01u

typedef struct _T_PNAK_SERVICE_DESCRIPTION {
  unsigned short DeviceRef;

  unsigned char Instance;
  unsigned char Service;

  T_PNAK_SERVICE_PRIMITIVE Primitive;
  T_PNAK_SERVICE_RESULT Result;

  unsigned short ClientId;
  unsigned short InvokeId;

  unsigned short DataLength;

} PACK_WORD_ALIGNMENT(T_PNAK_SERVICE_DESCRIPTION);

/*---------------------------------------------------------------------------*/

#define PNAK_MAX_NUMBER_REQ_SERVICES 52u
#define PNAK_MAX_NUMBER_CON_SERVICES 52u
#define PNAK_MAX_NUMBER_IND_SERVICES 52u

#ifndef PNAK_SERVICE_REQ_CHANNEL_SIZE
#define PNAK_SERVICE_REQ_CHANNEL_SIZE (PN_U16)8086u
#endif

#ifndef PNAK_SERVICE_CON_CHANNEL_SIZE
#define PNAK_SERVICE_CON_CHANNEL_SIZE (PN_U16)8086u
#endif

#ifndef PNAK_SERVICE_IND_CHANNEL_SIZE
#define PNAK_SERVICE_IND_CHANNEL_SIZE (PN_U16)8086u
#endif

/*=== SERVICE REQUEST/RESPONSE ==============================================*/

typedef struct _T_PNAK_SERVICE_REQ_RES_ENTRY {
  unsigned short ServiceOffset;

} PACK_WORD_ALIGNMENT(T_PNAK_SERVICE_REQ_RES_ENTRY);

/*---------------------------------------------------------------------------*/

typedef struct _T_PNAK_SERVICE_REQ_RES {
  unsigned short NumberEntries;

  T_PNAK_SERVICE_REQ_RES_ENTRY ServiceEntry[PNAK_MAX_NUMBER_REQ_SERVICES];
  unsigned char ServiceChannel[PNAK_SERVICE_REQ_CHANNEL_SIZE];

} PACK_WORD_ALIGNMENT(T_PNAK_SERVICE_REQ_RES);

/*=== SERVICE CONFIRMATION ==================================================*/

typedef struct _T_PNAK_SERVICE_CON_ENTRY {
  unsigned short ServiceOffset;

} PACK_WORD_ALIGNMENT(T_PNAK_SERVICE_CON_ENTRY);

/*---------------------------------------------------------------------------*/

typedef struct _T_PNAK_SERVICE_CON {
  unsigned short NumberEntries;

  T_PNAK_SERVICE_CON_ENTRY ServiceEntry[PNAK_MAX_NUMBER_CON_SERVICES];
  unsigned char ServiceChannel[PNAK_SERVICE_CON_CHANNEL_SIZE];

} PACK_WORD_ALIGNMENT(T_PNAK_SERVICE_CON);

/*=== SERVICE INDICATION ====================================================*/

typedef struct _T_PNAK_SERVICE_IND_ENTRY {
  unsigned short ServiceOffset;

} PACK_WORD_ALIGNMENT(T_PNAK_SERVICE_IND_ENTRY);

/*---------------------------------------------------------------------------*/

typedef struct _T_PNAK_SERVICE_IND {
  unsigned short NumberEntries;

  T_PNAK_SERVICE_IND_ENTRY ServiceEntry[PNAK_MAX_NUMBER_IND_SERVICES];
  unsigned char ServiceChannel[PNAK_SERVICE_IND_CHANNEL_SIZE];

} PACK_WORD_ALIGNMENT(T_PNAK_SERVICE_IND);

/*=============================================================================
EVENT INTERFACE
=============================================================================*/

typedef unsigned short T_PNAK_MODE;

#define PNAK_MODE_OFFLINE (T_PNAK_MODE)0u
#define PNAK_MODE_ONLINE (T_PNAK_MODE)1u

/*=== EVENT REQUEST ==========================================================*/

typedef struct _T_PNAK_EVENT_SET_MODE {
  T_PNAK_MODE Mode;

} PACK_WORD_ALIGNMENT(T_PNAK_EVENT_SET_MODE);

/*---------------------------------------------------------------------------*/

typedef struct _T_PNAK_EVENT_SET_DEVICE_STATE {
  T_BITSET_256 ActivateDeviceReference;
  T_BITSET_256 DeactivateDeviceReference;

} PACK_WORD_ALIGNMENT(T_PNAK_EVENT_SET_DEVICE_STATE);

/*=== EVENT INDICATION ======================================================*/

typedef enum _T_PNAK_STATE {
  PNAK_STATE_OK = 0,
  PNAK_STATE_NO_PARAMETER,
  PNAK_STATE_DOUBLE_ADDRESS_ERROR,
  PNAK_STATE_HARDWARE_ERROR,
  PNAK_STATE_APPL_WATCHDOG_EXPIRED,
  PNAK_STATE_ETH_FAILURE,
  PNAK_STATE_UDP_FAILURE,
  PNAK_STATE_DUPLICATE_NAME_ERROR,
  PNAK_STATE_SPEED_OR_DUPLEXITY_PROBLEM,
  PNAK_STATE_NON_VOLATILE_STORAGE_PROBLEM

} T_PNAK_STATE;

/*---------------------------------------------------------------------------*/

typedef struct _T_PNAK_EVENT_STATE {
  T_PNAK_MODE Mode;
  T_PNAK_STATE State;

} PACK_WORD_ALIGNMENT(T_PNAK_EVENT_STATE);

/*---------------------------------------------------------------------------*/

typedef struct _T_PNAK_EVENT_ALARM {
  T_BITSET_256 DeviceReference;

} PACK_WORD_ALIGNMENT(T_PNAK_EVENT_ALARM);

/*---------------------------------------------------------------------------*/

typedef struct _T_PNAK_EVENT_ALARM_ACK {
  T_BITSET_256 DeviceReference;

} PACK_WORD_ALIGNMENT(T_PNAK_EVENT_ALARM_ACK);

/*---------------------------------------------------------------------------*/

#define PNAK_DEVICE_STATE_DEACTIVATED 0x00u
#define PNAK_DEVICE_STATE_FIND_DEVICE 0x01u
#define PNAK_DEVICE_STATE_CONNECTION_ESTABLISHMENT 0x02u
#define PNAK_DEVICE_STATE_CONNECTED 0x04u
#define PNAK_DEVICE_STATE_CONNECT_ERROR 0x10u
#define PNAK_DEVICE_STATE_WRITE_ERROR 0x20u
#define PNAK_DEVICE_STATE_CONTROL_ERROR 0x40u
#define PNAK_DEVICE_STATE_CLOSE_PENDING 0x80u
#define PNAK_DEVICE_STATE_MASK                                                 \
  (PNAK_DEVICE_STATE_DEACTIVATED | PNAK_DEVICE_STATE_FIND_DEVICE               \
      | PNAK_DEVICE_STATE_CONNECTION_ESTABLISHMENT                             \
      | PNAK_DEVICE_STATE_CONNECTED)
#define PNAK_DEVICE_STATE_MASK_CONNECTING                                      \
  (PNAK_DEVICE_STATE_FIND_DEVICE | PNAK_DEVICE_STATE_CONNECTION_ESTABLISHMENT)
#define PNAK_DEVICE_STATE_ERROR_MASK                                           \
  (PNAK_DEVICE_STATE_CONNECT_ERROR | PNAK_DEVICE_STATE_WRITE_ERROR             \
      | PNAK_DEVICE_STATE_CONTROL_ERROR)

typedef struct _T_PNAK_EVENT_DEVICE_STATE {
  unsigned char State[MAX_NUMBER_SUPPORTED_DEVICES];

} PACK_WORD_ALIGNMENT(T_PNAK_EVENT_DEVICE_STATE);

/*---------------------------------------------------------------------------*/

#define PNAK_PTCP_IN_SNYC 0x00000001uL
#define PNAK_PTCP_NO_SYNC_MESSAGE 0x00000002uL
#define PNAK_PTCP_JITTER_OUT_OF_BOUNDARY 0x00000004uL

/*---------------------------------------------------------------------------*/

#define PNAK_MRPD_MISSING_APPEARS 0x8000u
#define PNAK_MRPD_MISSING_DISAPPEARS 0x0000u

typedef struct _T_PNAK_EVENT_MRPD_IOCR {
  PN_U16 FrameId[PN_SERVICE_DOWNLOAD_MAX_NUMBER_IOCRS];

} PACK_WORD_ALIGNMENT(T_PNAK_EVENT_MRPD_IOCR);

typedef struct _T_PNAK_EVENT_MRPD {
  T_PNAK_EVENT_MRPD_IOCR AR[MAX_NUMBER_CMDEV_CONNECTIONS];

} PACK_WORD_ALIGNMENT(T_PNAK_EVENT_MRPD);

/*=== EVENT INDICATION MASK =================================================*/

#define PNAK_EVENT_STATE_CHANGED 0x00000001uL
#define PNAK_EVENT_DEVICE_STATE_CHANGED 0x00000002uL
#define PNAK_EVENT_ALARM 0x00000004uL
#define PNAK_EVENT_ALARM_ACK 0x00000008uL
#define PNAK_EVENT_ETHERNET_STATE_CHANGED 0x00000010uL
#define PNAK_EVENT_PTCP 0x00000020uL
#define PNAK_EVENT_MRPD 0x00000040uL
#define PNAK_EVENT_MASK                                                        \
  (PNAK_EVENT_STATE_CHANGED | PNAK_EVENT_DEVICE_STATE_CHANGED                  \
      | PNAK_EVENT_ALARM | PNAK_EVENT_ALARM_ACK                                \
      | PNAK_EVENT_ETHERNET_STATE_CHANGED | PNAK_EVENT_PTCP | PNAK_EVENT_MRPD)

typedef PN_U32 T_PNAK_EVENT_REG;

typedef struct _T_PNAK_EVENT_IND_MASK {
  T_PNAK_EVENT_REG EventMaskReg;

} PACK_WORD_ALIGNMENT(T_PNAK_EVENT_IND_MASK);

/*=============================================================================
STATISTIC INTERFACE
=============================================================================*/

/* Attention: keep in sync with data provided by stack, see controller/device
 * context management */
#define PNAK_CMCTL_VENDOR_ID_MISMATCH 0x00000001uL
#define PNAK_CMCTL_DEVICE_ID_MISMATCH 0x00000002uL
#define PNAK_CMCTL_DUPLICATE_NAME 0x00000004uL
#define PNAK_CMCTL_DUPLICATE_IP_ADDRESS 0x00000008uL
#define PNAK_CMCTL_NO_EMPTY_NAME 0x00000010uL
#define PNAK_CMCTL_RPC_RESPONSE_MISSING 0x00000020uL
#define PNAK_CMCTL_MODULE_MISMATCH 0x00000040uL

#define PNIO_STATUS_CODING_RPC 0x00000001uL
#define PNIO_STATUS_CODING_NDR 0x00000002uL
#define PNIO_STATUS_CODING_PROFINET 0x00000003uL

#define PNIO_STATUS_ENCODING_BIG_ENDIAN 0x00u
#define PNIO_STATUS_ENCODING_LITTLE_ENDIAN 0x10u
#define PNIO_STATUS_ENCODING_MASK 0xF0u

typedef struct _T_PNAK_CONNECTION_STATISTIC {
  PN_U32 Coding;

  PN_U8 Encoding;
  PN_U8 Reserved[3];

  T_PROFINET_STATUS PNIOStatus;

} T_PNAK_CONNECTION_STATISTIC;

typedef struct _T_PNAK_CMCTL_STATISTIC {
  PN_U32 NumberDevicesInStartup;
  PN_U32 NumberDevicesConnected;

  T_PN_MAC_ADDRESS MacAddress[MAX_NUMBER_SUPPORTED_DEVICES];

  T_PNAK_CONNECTION_STATISTIC Cmctl[MAX_NUMBER_SUPPORTED_DEVICES];
  PN_U32 Status[MAX_NUMBER_SUPPORTED_DEVICES];

} T_PNAK_CMCTL_STATISTIC;

/*---------------------------------------------------------------------------*/

typedef struct _T_PNAK_CMDEV_STATISTIC {
  T_PNAK_CONNECTION_STATISTIC Cmdev[MAX_NUMBER_CMDEV_CONNECTIONS];

} T_PNAK_CMDEV_STATISTIC;

/*=============================================================================
DATA INTERFACE
=============================================================================*/

/*=== DATA INDICATION =======================================================*/

typedef struct _T_PNAK_DATA_CONSUMER_DATA_CHANGED {
  T_BITSET_256 DeviceReference;

} PACK_WORD_ALIGNMENT(T_PNAK_DATA_CONSUMER_DATA_CHANGED);

/*---------------------------------------------------------------------------*/

typedef struct _T_PNAK_DATA_PROVIDER_DATA_UPDATED {
  T_BITSET_256 DeviceReference;

} PACK_WORD_ALIGNMENT(T_PNAK_DATA_PROVIDER_DATA_UPDATED);

/*=== DATA INDICATION MASK ==================================================*/

#define PNAK_DATA_EVENT_CYCLIC_TRANSFER_FINISHED 0x00000001uL
#define PNAK_DATA_EVENT_CONSUMER_DATA_CHANGED 0x00000002uL
#define PNAK_DATA_EVENT_PROVIDER_DATA_UPDATED 0x00000004uL

typedef PN_U32 T_PNAK_DATA_EVENT_REG;

typedef struct _T_PNAK_DATA_IND_MASK {
  T_PNAK_DATA_EVENT_REG EventMaskReg;

} PACK_WORD_ALIGNMENT(T_PNAK_DATA_IND_MASK);

/*---------------------------------------------------------------------------*/

#define PNAK_IOXS_STATUS_NO_EXTENTION_FOLLOWS 0x00u
#define PNAK_IOXS_STATUS_DETECTED_BY_SUBSLOT 0x00u
#define PNAK_IOXS_STATUS_DETECTED_BY_SLOT 0x20u
#define PNAK_IOXS_STATUS_DETECTED_BY_DEVICE 0x40u
#define PNAK_IOXS_STATUS_DETECTED_BY_CONTROLLER 0x60u
#define PNAK_IOXS_STATUS_DATA_BAD 0x00u
#define PNAK_IOXS_STATUS_DATA_GOOD 0x80u

/******************************************************************************
FUNCTION DECLARATION
******************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

#ifndef __DOXYGEN__

/*--- initialization/termination functions ----------------------------------*/

extern void pnak_init(void);
extern void pnak_term(void);

#define pnak_start_profistack pnak_start_stack
#define pnak_stop_profistack pnak_stop_stack

extern T_PNAK_RESULT pnak_start_stack(unsigned short, const T_PNAK_MODE_ID);
extern T_PNAK_RESULT pnak_stop_stack(unsigned short);

/*--- General interface functions -------------------------------------------*/

extern T_PNAK_RESULT pnak_wait_for_multiple_objects(
    unsigned short, T_PNAK_WAIT_OBJECT*, T_PNAK_TIMEOUT);
extern T_PNAK_RESULT pnak_set_multiple_objects(
    unsigned short, const T_PNAK_WAIT_OBJECT);
extern T_PNAK_RESULT pnak_mask_multiple_objects(
    unsigned short, const T_PNAK_WAIT_OBJECT);

extern T_PNAK_RESULT pnak_get_exception(unsigned short, T_PNAK_EXCEPTION*);
extern T_PNAK_RESULT pnak_get_version(unsigned short, T_PNAK_VERSIONS*);

extern T_PNAK_RESULT pnak_snmp_get_data(unsigned short, T_PNAK_OID*, void*,
    PN_U32*, PN_U32*, unsigned char, PN_BOOL);
extern T_PNAK_RESULT pnak_snmp_check_data(
    unsigned short, T_PNAK_OID*, void*, PN_U32, PN_U32, unsigned char);
extern T_PNAK_RESULT pnak_snmp_set_data(
    unsigned short, T_PNAK_OID*, void*, PN_U32, PN_U32);

extern T_PNAK_RESULT pnak_retrigger_watchdog(unsigned short, PN_U32);

/*--- Service interface functions -------------------------------------------*/

extern T_PNAK_RESULT pnak_limit_number_parallel_services(
    unsigned short, unsigned short, unsigned short);

extern T_PNAK_RESULT pnak_send_service_req_res(
    unsigned short, const T_PNAK_SERVICE_REQ_RES*);
extern T_PNAK_RESULT pnak_get_service_ind(unsigned short, T_PNAK_SERVICE_IND*);
extern T_PNAK_RESULT pnak_get_service_con(unsigned short, T_PNAK_SERVICE_CON*);

/*--- Event interface functions ---------------------------------------------*/

extern T_PNAK_RESULT pnak_set_mode(
    unsigned short, const T_PNAK_EVENT_SET_MODE*);
extern T_PNAK_RESULT pnak_set_device_state(
    unsigned short, const T_PNAK_EVENT_SET_DEVICE_STATE*);

extern T_PNAK_RESULT pnak_get_state(unsigned short, T_PNAK_EVENT_STATE*);
extern T_PNAK_RESULT pnak_get_device_state_ind(
    unsigned short, T_PNAK_EVENT_DEVICE_STATE*);
extern T_PNAK_RESULT pnak_get_alarm_ind(unsigned short, T_PNAK_EVENT_ALARM*);
extern T_PNAK_RESULT pnak_get_alarm_ack_ind(
    unsigned short, T_PNAK_EVENT_ALARM_ACK*);
extern T_PNAK_RESULT pnak_get_ptcp_ind(unsigned short, PN_U32*);
extern T_PNAK_RESULT pnak_get_mrpd_ind(unsigned short, T_PNAK_EVENT_MRPD*);

/*--- Data interface functions ----------------------------------------------*/

extern T_PNAK_RESULT pnak_set_iocr_data(unsigned short, unsigned short,
    const unsigned char*, unsigned short, unsigned char);

extern T_PNAK_RESULT pnak_get_iocr_data(unsigned short, unsigned short,
    unsigned char*, unsigned short*, unsigned char*, unsigned char*);
extern T_PNAK_RESULT pnak_get_consumer_data_changed_ind(
    unsigned short, T_PNAK_DATA_CONSUMER_DATA_CHANGED*);
extern T_PNAK_RESULT pnak_get_provider_data_updated(
    unsigned short, T_PNAK_DATA_PROVIDER_DATA_UPDATED*);

/*=============================================================================
CALLBACK INTERFACE
=============================================================================*/

typedef int (*T_PNAK_WAIT_OBJECT_CALLBACK)(unsigned short, T_PNAK_WAIT_OBJECT);
typedef int (*T_PNAK_HW_START_CALLBACK)(unsigned short, T_PNAK_MODE_ID, void*);

typedef void (*T_PROVIDER_CALLBACK)(unsigned short, unsigned short,
    unsigned char*, unsigned short, unsigned char*);
typedef void (*T_CONSUMER_CALLBACK)(unsigned short, unsigned short,
    const unsigned char*, unsigned short, unsigned char, unsigned char);

extern T_PNAK_RESULT pnak_register_provider_callback(
    unsigned short, T_PROVIDER_CALLBACK);
extern T_PNAK_RESULT pnak_unregister_provider_callback(unsigned short);

extern T_PNAK_RESULT pnak_register_consumer_callback(
    unsigned short, T_CONSUMER_CALLBACK);
extern T_PNAK_RESULT pnak_unregister_consumer_callback(unsigned short);

extern T_PNAK_RESULT pnak_register_ethernet_provider_hook(
    unsigned short, T_PROVIDER_CALLBACK);
extern T_PNAK_RESULT pnak_unregister_ethernet_provider_hook(unsigned short);

/*---------------------------------------------------------------------------*/

typedef void (*T_PNAK_CMCTL_STATISTIC_CALLBACK)(
    unsigned short, T_PNAK_CMCTL_STATISTIC*);

extern T_PNAK_RESULT pnak_register_cmctl_statistic_callback(
    unsigned short, T_PNAK_CMCTL_STATISTIC_CALLBACK);
extern T_PNAK_RESULT pnak_unregister_cmctl_statistic_callback(unsigned short);

typedef void (*T_PNAK_CMDEV_STATISTIC_CALLBACK)(
    unsigned short, T_PNAK_CMDEV_STATISTIC*);

extern T_PNAK_RESULT pnak_register_cmdev_statistic_callback(
    unsigned short, T_PNAK_CMDEV_STATISTIC_CALLBACK);
extern T_PNAK_RESULT pnak_unregister_cmdev_statistic_callback(unsigned short);

#endif /* __DOXYGEN__ */

#ifdef __cplusplus
}
#endif

/* the scai/sdai socket functions are derived from the basic implementation of
 * the PROFINET stack and use the same data structures and error codes
 * so we have to disable the original definitions
 */

#if !defined(SCAI_INCLUDE_SOCKET) && !defined(SDAI_INCLUDE_SOCKET)

/******************************************************************************
SOCKET USER INTERFACE
******************************************************************************/

/**
 * @socketmainpage
 *
 * @section Introduction
 * In addition to the realtime ethernet communication via the PROFINET
 * Application Interface (PNAK) Softing offers a universal Socket Interface for
 * implementation of any TCP or UDP based protocol. It enables the integration
 * of various industrial IT functions into the device:
 *
 * - Embedded Webserver for freely designed websites
 * - FTP-Server for updating the device firmware or for file downloads
 * - E-Mail client for sending E-Mails from the application program
 * - Telnet for virtual terminal connections
 * - Implementation of manufacturer specific application protocols
 *
 * The Socket Interface offers a comfortable application interface which
 * disburdens the user from a lot of tasks and provides a lot of automatisms.
 * The special characteristics of each TCP/IP stack will be hidden from the
 * user. Establishment and management of TCP connections are handled completely
 * by the socket interface. The user can focus on the implementation of the
 * desired IT functionality. This allows an easy implementation of the socket
 * application. The resources needed by the socket interface (see also \ref
 * SocketResources) can be easily adpated to the available memory provided by
 * the used the hardware platform.
 *
 * The communication via the Socket Interface can be used in parallel and
 * without influencing the realtime communication via the used realtime ethernet
 * protocol. Therefore the realtime ethernet communication will be processed
 * with a higher priority than the communication via the Socket Interface. The
 * network interface is shared between the Socket Interface and the realtime
 * ethernet protocol. Thus a high communication load to be processed by the
 * realtime ethernet protocol limits the available bandwith and the performance
 * of the Socket Interface communication.
 *
 * @section Application Interface Description
 * The following figure shows the structure of Softings Socket Interface
 * implementation.
 * @image html Socket_Structure.png
 * A detailed description of the Socket Application Interface can be found in
 * the following chapters:
 * - \ref sock_defines
 * - \ref sock_structures
 * - \ref sock_functions
 */

/**
 * @defgroup sock_defines Defines
 * @ingroup sock
 */

/**
 * @defgroup sock_structures Data Structures
 * @ingroup sock
 */

/**
 * @defgroup sock_functions Functions
 * @ingroup sock
 */

/** \addtogroup sock_defines
 * @{
 */

#define MAX_NUMBER_SUPPORTED_SOCKETS (PN_U16)64u

/*--- Socket options --------------------------------------------------------*/

/** \anchor SocketFlags @name Socket Flags
 * Definitions for Socket Flags
 * @{
 */

#define SOCK_FLAG_TYPE_UDP 0x00000000uL /**< Socket type TCP */
#define SOCK_FLAG_TYPE_TCP 0x00008000uL /**< Socket type UDP */
#define SOCK_FLAG_ENABLE_KEEPALIVE                                             \
  (SOCK_FLAG_TYPE_TCP | 0x00000001uL) /**< Enables keep-alive packets to       \
                                         supervise a TCP connection */
#define SOCK_FLAG_ENABLE_BROADCAST                                             \
  (SOCK_FLAG_TYPE_UDP                                                          \
      | 0x00000002uL) /**< Enables the transmission of broadcast messages over \
                         a UDP socket */
#define SOCK_FLAG_ENABLE_MULTICAST                                             \
  (SOCK_FLAG_TYPE_UDP                                                          \
      | 0x00000004uL) /**< Enables the transmission of multicast messages over \
                         a UDP socket */
#define SOCK_FLAG_USER_MASK                                                    \
  0x0000FFFFuL /**< Specifies the bits wich are valid for the socket           \
                  application */

/** @} */

/*--- Socket error codes ----------------------------------------------------*/

/** \anchor ErrorCode @name Socket Interface Error Codes
 * Definitions for Socket Interface Error Codes
 * @{
 */

typedef enum _T_SOCKET_RESULT {
  SOCK_SUCCESS = 0u, /**< General success return value */
  SOCK_ERR_INVALID_ARGUMENT, /**< Passed argument is invalid */
  SOCK_ERR_API_ALREADY_INITIALIZED, /**< Socket Interface already initialized */
  SOCK_ERR_API_NOT_INITIALIZED, /**< Socket Interface not initialized */
  SOCK_ERR_INVALID_SOCKET_ID, /**< The passed socket ID is invalid */
  SOCK_ERR_OUT_OF_SOCKETS, /**< No free socket available */
  SOCK_ERR_SOCKET_NOT_CREATED, /**< The specified socket has not been created.
                                */
  SOCK_ERR_SOCKET_OFFLINE, /**< The local port associated with the socket is not
                              open. Sending/Receiving of messages is not
                              possible. */
  SOCK_ERR_SOCKET_BUSY, /**< A send request is rejected because a previous send
                           request on the specified socket is not completed */
  SOCK_ERR_SOCKET_NOT_CONNECTED, /**< The request is rejected because the
                                    specified TCP connection or no TCP
                                    connection has been established for the
                                    specified socket. */
  SOCK_ERR_SOCKET_CONNECTED, /**< The request is rejected because the specified
                                TCP connection has established a connection
                                before the filter was activated. */
  SOCK_ERR_ADDR_IN_USE, /**< The passed local address is already in use by
                           another socket. */
  SOCK_ERR_INVALID_LOCAL_ADDR, /**< The passed local IP address is invalid. It
                                  is not possible to bind the socket to this
                                  address. Which IP addresses are allowed
                                  depends on the socket configuration (e.g.
                                  socket type UDP or TCP) and the network
                                  settings adjusted by calling the function
                                  sock_init(). */
  SOCK_ERR_INVALID_REMOTE_ADDR, /**< The passed remote IP address is invalid. It
                                   is not possible to send or connect to this
                                   address. Which IP addresses are allowed
                                   depends on the socket configuration (e.g.
                                   socket type UDP or TCP) and the network
                                   settings adjusted by calling the function
                                   sock_init(). */
  SOCK_ERR_NO_DATA_RECEIVED, /**< No data has been received on the specfied
                                socket or TCP connection. */
  SOCK_ERR_NOT_SUPPORTED, /**< The requested action is not supported on the
                             specified socket. */
  SOCK_ERR_TEMPORARY_NOT_EXECUTABLE, /**< The requested action is temporary not
                                        executable on the specified socket due a
                                        concurrent pending request. */
  SOCK_ERR_FATAL_ERROR, /**< A fatal error is reported during execution of
                           function. */
  SOCK_ERR_LIBRARY_NOT_LOADED /**< Can not load socket library */
} T_SOCKET_RESULT;

/** @} */

/*---------------------------------------------------------------------------*/

#define SOCK_IP_ADDR_ANY                                                       \
  0x00000000uL /**< Can be used as local IP address when creating a socket to  \
                  bind the socket to all the IP addresses (If the system has   \
                    multiple IP addresses) or if the local IP address is       \
                  unknown. */
#define SOCK_PORT_ANY                                                          \
  0u /**< Can be used as local port when creating a TCP Client socket to let   \
        the TCP/IP stack select a unique local port from the dynamic client                                                                \
        port range. */
/** @} */

/*===========================================================================*/

/** \addtogroup sock_structures Data Structures
 * @{
 * \brief Specifies a transport IP address and port.
 * All of the structure data must be specified in network-byte-order
 * (big-endian).
 */

typedef struct _T_PNAK_SOCK_ADDR {
  PN_U32 IpAddress; /**< IP address */

  unsigned short Port; /**< port number */

  unsigned char Alignment[2];

} PACK_WORD_ALIGNMENT(T_PNAK_SOCK_ADDR);

/** @} */

/*---------------------------------------------------------------------------*/

/** \addtogroup sock_structures Data Structures
 * @{
 * \brief Configurable Options for the socket interface
 */
typedef struct _T_PNAK_SOCK_CONFIG_OPTIONS {
  PN_U32
      Timeout; /**< Keep-Alive Timeout in milliseconds. The Keep-Alive Timeout
                  will be used for the supervision of TCP connections. The
                  connection will be aborted if the remote connection partner
                  did not show activity within this timeout. The supervision of
                  TCP connections can be enabled/disabled socket-specific when
                    creating the socket by calling sock_create(). For UDP
                  sockets the application needs to perform a supervision if
                  necessary. If the timeout value is set to 0 and
                  #SOCK_FLAG_ENABLE_KEEPALIVE is set when creating a socket, a
                  default value for the Keep-Alive Timeout will be used. */

} PACK_WORD_ALIGNMENT(T_PNAK_SOCK_CONFIG_OPTIONS);

/** @} */

/*---------------------------------------------------------------------------*/

/** \addtogroup sock_structures Data Structures
 * @{
 * \brief Network specific parameter
 * All of the structure data must be specified in network-byte-order
 * (big-endian).
 */
typedef struct _T_PNAK_SOCK_NETWORK_SETTINGS {
  PN_U32 IpAddress; /**< The IP-Address of the Device. For future use only. */
  PN_U32 SubnetMask; /**< The Netmask of the Device. For future use only. */
  PN_U32 GatewayAddress; /**< The Gateway of the Device. For future use only. */

  unsigned char
      MacAddress[6]; /**< The MAC address of the device. For future use only. */
  unsigned char Alignment[2];

} PACK_WORD_ALIGNMENT(T_PNAK_SOCK_NETWORK_SETTINGS);

/** @} */

/*---------------------------------------------------------------------------*/

/** \addtogroup sock_structures Data Structures
 * @{
 * \brief This structure holds the initialization data for the socket interface
 */
typedef struct _T_PNAK_SOCK_INIT {
  T_PNAK_SOCK_NETWORK_SETTINGS
      NetworkSettings; /**< Network specific parameter */
  T_PNAK_SOCK_CONFIG_OPTIONS
      ConfigOptions; /**< Configurable Options for the socket interface */

} PACK_WORD_ALIGNMENT(T_PNAK_SOCK_INIT);
/** @} */

/*---------------------------------------------------------------------------*/

/** \addtogroup sock_defines
 * @{
 * \anchor ConnectionStatus @name TCP Connection Status
 * Status of a TCP connection
 * @{
 */
#define SOCK_TCP_CONNECTION_STATUS_UNCONNECTED                                 \
  0x00000001uL /**< TCP connection not established */
#define SOCK_TCP_CONNECTION_STATUS_CONNECTING                                  \
  0x00000002uL /**< A TCP connection establishment is in progress. This status \
                  is only possible for a TCP client socket. */
#define SOCK_TCP_CONNECTION_STATUS_CONNECTED                                   \
  0x00000003uL /**< TCP connection established */
/** @} */

/** \anchor ConnectionStatusCode @name TCP Connection Status Codes
 * Error Codes of a TCP connection
 * @{
 */
#define SOCK_TCP_CONNECTION_STATUS_CODE_NO_ERROR                               \
  0x00000000uL /**< No error detected */
#define SOCK_TCP_CONNECTION_STATUS_CODE_INTERNAL_ERROR                         \
  0x00000001uL /**< An internal error occurred in the socket interface causing \
                  the connection to abort. */
#define SOCK_TCP_CONNECTION_STATUS_CODE_CLOSED_REMOTELY                        \
  0x00000002uL /**< The connection has been orderly closed by the remote       \
                  connection partner (TCP FIN received). */
#define SOCK_TCP_CONNECTION_STATUS_CODE_ABORTED_REMOTELY                       \
  0x00000003uL /**< The connection has been aborted by the remote connection   \
                  partner (TCP RST received), e.g. because                     \
                    the remote partner has detected an error or temporarily    \
                  crashed. */
#define SOCK_TCP_CONNECTION_STATUS_CODE_TIMEOUT                                \
  0x00000004uL /**< The connection has been aborted because of a network       \
                  failure or because the remote connection partner failed to                                                                            \
                  respond within the configured supervision timeout (see also  \
                  T_PNAK_SOCK_CONFIG_OPTIONS). */
#define SOCK_TCP_CONNECTION_STATUS_CODE_CONNECT_REJECTED                       \
  0x00000005uL /**< It was not possible to establish a connection because the  \
                  destination host rejected the connect request, e.g. because                                                                  \
                  the remote port is closed. This error only occurs for a TCP  \
                  client socket. To retry connection establishment the                                                             \
                  application has to close the socket and re-create the socket \
                  again. */
/** @} */
/** @} */

/** \addtogroup sock_structures Data Structures
 * @{
 * \brief This structure holds status informations about a single TCP connection
 */
typedef struct _T_PNAK_SOCK_TCP_CONNECTION_LOCAL_STATUS {
  PN_U32 Status; /**< Local status of the TCP connection. Valid values: \ref
                    ConnectionStatus */
  PN_U32 StatusCode; /**< Indicates the last error of the TCP connection. Valid
                        values: \ref ConnectionStatusCode */

  T_PNAK_SOCK_ADDR
      RemoteAddr; /**< Address information of the remote connection partner */

} PACK_WORD_ALIGNMENT(T_PNAK_SOCK_TCP_CONNECTION_LOCAL_STATUS);

/** @} */

/** \addtogroup sock_structures Data Structures
 * @{
 * \brief This structure holds status informations about all TCP connections of
 * a socket
 */
typedef struct _T_PNAK_SOCK_TCP_LOCAL_STATUS_DATA {
  PN_U32 NumberConnections; /**< Number of TCP connection for which status info
                               is provided */

  /*T_PNAK_SOCK_TCP_CONNECTION_LOCAL_STATUS   Connection [NumberConnections];*/ /**< TCP connection status information */

} PACK_WORD_ALIGNMENT(T_PNAK_SOCK_TCP_LOCAL_STATUS_DATA);

/** @} */

/** \addtogroup sock_defines
 * @{
 * \anchor SocketStatus @name Socket Status
 * Status of a socket
 * @{
 */
#define SOCK_LOCAL_STATUS_CLOSED 0x00000001uL /**< The local socket is closed  \
                                               */
#define SOCK_LOCAL_STATUS_OFFLINE                                              \
  0x00000002uL /**< The local port associated with the socket is not open.     \
                  Sending/Receiving of messages is not possible. */
#define SOCK_LOCAL_STATUS_ONLINE                                               \
  0x00000003uL /**< The local port associated with the socket is open.         \
                  Sending/Receiving of messages is possible. */
/** @} */

/** \anchor SocketStatusCode @name Socket Status Codes
 * Error Codes of a socket
 * @{
 */
#define SOCK_LOCAL_STATUS_CODE_NO_ERROR 0x00000000uL /**< No error detected */
#define SOCK_LOCAL_STATUS_CODE_ADDR_IN_USE                                     \
  0x00000001uL /**< The local address passed with sock_create() is already in  \
                  use by another socket. */
#define SOCK_LOCAL_STATUS_CODE_INTERNAL_ERROR                                  \
  0x00000002uL /**< An internal error occurred in the socket interface causing \
                  the socket to close. */
/** @} */
/** @} */

/** \addtogroup sock_structures Data Structures
 * @{
 * \brief This structure holds status informations about a socket
 */
typedef struct _T_PNAK_SOCK_IOC_GET_LOCAL_STATUS {
  PN_U32 Status; /**< Common socket status information. Valid values: \ref
                    SocketStatus */
  PN_U32 StatusCode; /**< Common socket error code. Valid values: \ref
                        SocketStatusCode */

  union {
    T_PNAK_SOCK_TCP_LOCAL_STATUS_DATA
        Tcp; /**< TCP specific socket status information */
    /* UDP specific socket status information not existing */

  } UseAs;

} PACK_WORD_ALIGNMENT(T_PNAK_SOCK_IOC_GET_LOCAL_STATUS);

/** @} */

/*---------------------------------------------------------------------------*/

/** \addtogroup sock_defines
 * @{
 * \anchor SendStatus @name Send Status
 * Send Status of a socket
 * @{
 */
#define SOCK_SEND_STATUS_NOT_BUSY                                              \
  0x00000001uL /**< No send request produced by the application */
#define SOCK_SEND_STATUS_BUSY                                                  \
  0x00000002uL /**< The send request is in progress. */
/** @} */

/** \anchor SendStatusCode @name Send Status Codes
 * Send Error Codes
 * @{
 */
#define SOCK_SEND_STATUS_CODE_NO_ERROR 0x00000000uL /**< No error detected */
#define SOCK_SEND_STATUS_CODE_NO_BUFFERS                                       \
  0x00000001uL /**< The TCP/IP stack was unable to allocate an internal buffer \
                  or the output queue is full. */
#define SOCK_SEND_STATUS_CODE_HOST_UNREACHABLE                                 \
  0x00000002uL /**< The destination host is unreachable, e.g. the destination  \
                  host is down */
#define SOCK_SEND_STATUS_CODE_REJECTED                                         \
  0x00000003uL /**< The destination host rejected the message, e.g. remote     \
                  port is closed. This error can only be returned by TCP                                        \
                  sockets. */
#define SOCK_SEND_STATUS_CODE_SEND_ERROR                                       \
  0x00000004uL /**< The TCP/IP stack reported a general problem sending the    \
                  message. */
/** @} */
/** @} */

/** \addtogroup sock_structures Data Structures
 * @{
 * \brief This structure holds TCP specific send status informations
 */
typedef struct _T_PNAK_SOCK_TCP_SEND_STATUS_DATA {
  T_PNAK_SOCK_ADDR
      RemoteAddr; /**< Address information of the remote connection partner */

} PACK_WORD_ALIGNMENT(T_PNAK_SOCK_TCP_SEND_STATUS_DATA);

/** @} */

/** \addtogroup sock_structures Data Structures
 * @{
 * \brief This structure holds send status informations
 */
typedef struct _T_PNAK_SOCK_IOC_GET_SEND_STATUS {
  PN_U32 Status; /**< Common send status information. Valid values: \ref
                    SendStatus  */
  PN_U32 StatusCode; /**< Common send error code. Valid values: \ref
                        SendStatusCode */

  union {
    T_PNAK_SOCK_TCP_SEND_STATUS_DATA
        Tcp; /**< TCP specific send status information */
    /* UDP specific send status information not existing */

  } UseAs;

} PACK_WORD_ALIGNMENT(T_PNAK_SOCK_IOC_GET_SEND_STATUS);

/** @} */

/*---------------------------------------------------------------------------*/

/** \addtogroup sock_defines
 * @{
 * \anchor ReceiveStatus @name Receive Status
 * Receive Status of a socket
 * @{
 */
#define SOCK_RECEIVE_STATUS_NO_DATA_RECEIVED                                   \
  0x00000000uL /**< No data on the specfied socket or TCP connection has been  \
                  received. */
#define SOCK_RECEIVE_STATUS_DATA_RECEIVED                                      \
  0x00000001uL /**< New data has been received on the specfied socket or TCP   \
                  connection. */
/** @} */

/** \anchor ReceiveStatusCode @name Receive Status Codes
 * Receive Error Codes
 * @{
 */
#define SOCK_RECEIVE_STATUS_CODE_NO_ERROR 0x00000000uL /**< No error detected  \
                                                        */
/** @} */
/** @} */

/** \addtogroup sock_structures Data Structures
 * @{
 * \brief This structure holds TCP specific receive status informations
 */
typedef struct _T_PNAK_SOCK_TCP_RECEIVE_STATUS_DATA {
  T_PNAK_SOCK_ADDR
      RemoteAddr; /**< Address information of the remote connection partner */

} PACK_WORD_ALIGNMENT(T_PNAK_SOCK_TCP_RECEIVE_STATUS_DATA);

/** @} */

/** \addtogroup sock_structures Data Structures
 * @{
 * \brief This structure holds receive status informations
 */
typedef struct _T_PNAK_SOCK_IOC_GET_RECEIVE_STATUS {
  PN_U32 Status; /**< Common receive status information. Valid values: \ref
                    ReceiveStatus  */
  PN_U32 StatusCode; /**< Common receive error code. Valid values: \ref
                        ReceiveStatusCode */

  union {
    T_PNAK_SOCK_TCP_RECEIVE_STATUS_DATA
        Tcp; /**< TCP specific receive status information */
    /* UDP specific send status information not existing */

  } UseAs;

} PACK_WORD_ALIGNMENT(T_PNAK_SOCK_IOC_GET_RECEIVE_STATUS);

/** @} */

/*---------------------------------------------------------------------------*/

/** \addtogroup sock_structures Data Structures
 * @{
 * \brief This structure specifies the TCP connection which shall be aborted
 */
typedef struct _T_PNAK_SOCK_IOC_CLOSE_TCP_CONNECTION {
  T_PNAK_SOCK_ADDR
      RemoteAddr; /**< Address information of the remote connection partner */

} PACK_WORD_ALIGNMENT(T_PNAK_SOCK_IOC_CLOSE_TCP_CONNECTION);

/** @} */

/*---------------------------------------------------------------------------*/

#define SOCK_MAX_NUMBER_FILTER_ENTRIES 4u

/** \addtogroup sock_structures Data Structures
 * @{
 * \brief This structure specifies the IP addresses from which a connection
 * establishment shall be accepted
 */
typedef struct _T_PNAK_SOCK_IOC_TCP_ACCEPT_FILTER {
  PN_U16 NumberEntries; /**< number of filter entries */
  PN_U16 Alignment;

  PN_U32 Filter[SOCK_MAX_NUMBER_FILTER_ENTRIES]; /**< IP address */

} PACK_WORD_ALIGNMENT(T_PNAK_SOCK_IOC_TCP_ACCEPT_FILTER);

/** @} */

/*---------------------------------------------------------------------------*/

/** \addtogroup sock_structures Data Structures
 * @{
 * \brief This structure specifies the IP addresses of the multicast group the
 * application wants to join or leave
 */
typedef struct _T_PNAK_SOCK_IOC_MULTICAST {
  PN_U32 IpAddress;

} PACK_WORD_ALIGNMENT(T_PNAK_SOCK_IOC_MULTICAST);

/*---------------------------------------------------------------------------*/

/** \addtogroup sock_defines
 * @{
 * \anchor IoctlCommand @name IO Control Commands
 * IO Control Commands for a socket
 * @{
 */
#define SOCK_IOC_GET_LOCAL_STATUS                                              \
  0x00000001uL /**< Get the local socket status */
#define SOCK_IOC_GET_SEND_STATUS                                               \
  0x00000002uL /**< Get the status of a send request */
#define SOCK_IOC_GET_RECEIVE_STATUS                                            \
  0x00000003uL /**< Checks if new data has been received */
#define SOCK_IOC_CLOSE_TCP_CONNECTION                                          \
  0x00000004uL /**< Closes a TCP connection */
#define SOCK_IOC_TCP_ACCEPT_FILTER                                             \
  0x00000005uL /**< Defines which IP Addresses are accepted by the socket */
#define SOCK_IOC_UDP_ADD_MULTICAST 0x00000006uL /**< Join a multicast group */
#define SOCK_IOC_UDP_DEL_MULTICAST 0x00000007uL /**< Leave a multicast group   \
                                                 */
/** @} */
/** @} */

/** \addtogroup sock_structures Data Structures
 * @{
 * \brief This structure holds the data for an IO Control Command
 */
typedef struct _T_PNAK_SOCK_IO_CONTROL {
  PN_U32 Command; /**< Specifies the IO Control Command. Valid values: \ref
                     IoctlCommand */

  union {
    T_PNAK_SOCK_IOC_GET_LOCAL_STATUS
        GetLocalStatus; /**< Local Socket Status informations */
    T_PNAK_SOCK_IOC_GET_SEND_STATUS
        GetSendStatus; /**< Send Status informations */
    T_PNAK_SOCK_IOC_GET_RECEIVE_STATUS
        GetReceiveStatus; /**< Receive Status informations */
    T_PNAK_SOCK_IOC_CLOSE_TCP_CONNECTION
        CloseTcpConnection; /**< TCP connection which shall be closed */
    T_PNAK_SOCK_IOC_TCP_ACCEPT_FILTER
        TcpAcceptFilter; /**< IP Address which shall be able to connect to the
                            server */
    T_PNAK_SOCK_IOC_MULTICAST AddMulticast; /**< Multicast group to join */
    T_PNAK_SOCK_IOC_MULTICAST DelMulticast; /**< Multicast group to leave */

  } UseAs;

} PACK_WORD_ALIGNMENT(T_PNAK_SOCK_IO_CONTROL);

/** @} */

/******************************************************************************
FUNCTION DECLARATION
******************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

#ifndef __DOXYGEN__

/*--- initialization/termination functions ----------------------------------*/

extern T_SOCKET_RESULT pnak_sock_init(unsigned short, T_PNAK_SOCK_INIT*);
extern T_SOCKET_RESULT pnak_sock_term(unsigned short);

/*--- socket interface functions --------------------------------------------*/

extern T_SOCKET_RESULT pnak_sock_create(unsigned short, PN_U32,
    T_PNAK_SOCK_ADDR*, T_PNAK_SOCK_ADDR*, unsigned short*);
extern T_SOCKET_RESULT pnak_sock_close(unsigned short, unsigned short);
extern T_SOCKET_RESULT pnak_sock_send(unsigned short, unsigned short,
    T_PNAK_SOCK_ADDR*, PN_U16, const unsigned char*);
extern T_SOCKET_RESULT pnak_sock_receive(
    unsigned short, unsigned short, T_PNAK_SOCK_ADDR*, PN_U16*, unsigned char*);
extern T_SOCKET_RESULT pnak_sock_ioctl(
    unsigned short, unsigned short, T_PNAK_SOCK_IO_CONTROL*);

/*--- socket interface helper functions -------------------------------------*/

extern T_SOCKET_RESULT pnak_sock_get_receive_status(
    unsigned short, T_BITSET_64*);

#endif /* __DOXYGEN__ */

#ifdef __cplusplus
}
#endif

#endif /* SCAI_INCLUDE_SOCKET || SDAI_INCLUDE_SOCKET */

/*****************************************************************************/

#if defined WIN32 || defined _WIN32
#pragma warning(disable : 4103) /* used #pragma pack to reset alignment */
#pragma pack(pop)
#pragma warning(default : 4103)

#elif defined __GNUC__
/* nothing to do */

#elif defined _OS9000
/* nothing to do */

#else
#error CAUTION: set default structure alignment
#endif

/*****************************************************************************/

#endif /* __PNAK_H__ */

/*****************************************************************************/
/* vim: set nu ts=2 et nowrap: */
