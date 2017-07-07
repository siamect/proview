/******************************************************************************

SOFTING Industrial Automation GmbH
Richard-Reitzner-Allee 6
D-85540 Haar
Phone: ++49-89-4 56 56-0
Fax: ++49-89-4 56 56-3 99
http://www.softing.com

Copyright (C) SOFTING Industrial Automation GmbH 2005-2013. All Rights Reserved

Version: 2.01.00

******************************************************************************/

#ifndef __PROFINET_H__
#define __PROFINET_H__

/******************************************************************************

! ! !  A T T E N T I O N  ! ! !

       This header file needs  B Y T E  structure alignment.

       All data structures need M O T O R O L A format.

******************************************************************************/

#if defined WIN32 || defined _WIN32
  #pragma warning (disable : 4103)     /* used #pragma pack to change alignment */
  #pragma pack (push, 1)
  #pragma warning (default : 4103)

  #ifndef PACK_WORD_ALIGNMENT
    #define PACK_WORD_ALIGNMENT(Struct)   Struct
  #endif
  #ifndef PACK_BYTE_ALIGNMENT
    #define PACK_BYTE_ALIGNMENT(Struct)   Struct
  #endif

  typedef unsigned char     PN_U8;
  typedef unsigned short    PN_U16;
  typedef unsigned long     PN_U32;

  typedef signed char       PN_S8;
  typedef signed short      PN_S16;
  typedef signed long       PN_S32;

  typedef unsigned char     PN_BOOL;
#elif defined __GNUC__
  #ifndef PACK_WORD_ALIGNMENT
    #define PACK_WORD_ALIGNMENT(Struct)   __attribute__ ((packed, aligned (2))) Struct
  #endif

  #ifndef PACK_BYTE_ALIGNMENT
    #define PACK_BYTE_ALIGNMENT(Struct)   __attribute__ ((packed, aligned (1))) Struct
  #endif

  typedef unsigned char     PN_U8;
  typedef unsigned short    PN_U16;

  typedef signed char       PN_S8;
  typedef signed short      PN_S16;

  #if defined __x86_64__
  typedef unsigned int      PN_U32;
  typedef signed int        PN_S32;
  #else
  typedef unsigned long     PN_U32;
  typedef signed long       PN_S32;
  #endif

  typedef unsigned char     PN_BOOL;
#elif defined _OS9000

  #ifndef PACK_WORD_ALIGNMENT
    #define PACK_WORD_ALIGNMENT(Struct)   Struct
  #endif
  #ifndef PACK_BYTE_ALIGNMENT
    #define PACK_BYTE_ALIGNMENT(Struct)   Struct
  #endif

  typedef unsigned char     PN_U8;
  typedef unsigned short    PN_U16;
  typedef unsigned long     PN_U32;

  typedef signed char       PN_S8;
  typedef signed short      PN_S16;
  typedef signed long       PN_S32;

  typedef unsigned char     PN_BOOL;
#else
  #error CAUTION: set word structure alignment
  typedef unsigned char     PN_U8;
  typedef unsigned short    PN_U16;
  typedef unsigned long     PN_U32;

  typedef signed char       PN_S8;
  typedef signed short      PN_S16;
  typedef signed long       PN_S32;

  typedef unsigned char     PN_BOOL;
#endif

/******************************************************************************
DEFINES
******************************************************************************/

#undef PN_FALSE
#undef PN_TRUE

#define PN_FALSE          0x00u
#define PN_TRUE           0xFFu

#define _PN_ALIGN2(Type,Value)    ((Type) ((((Type)(Value)) + (Type) 1) & ((Type)(-2))))
#define _PN_ALIGN4(Type,Value)    ((Type) ((((Type)(Value)) + (Type) 3) & ((Type)(-4))))

/*===========================================================================*/

#define PROFINET_IDENT_MAINTENANCE_ORDER_ID_LENGTH          20u
#define PROFINET_IDENT_MAINTENANCE_SR_NUMBER_LENGTH         16u
#define PROFINET_IDENT_MAINTENANCE_FUNCTION_LENGTH          32u
#define PROFINET_IDENT_MAINTENANCE_LOCATION_LENGTH          22u
#define PROFINET_IDENT_MAINTENANCE_DATE_LENGTH              16u
#define PROFINET_IDENT_MAINTENANCE_DESCRIPTION_LENGTH       54u
#define PROFINET_IDENT_MAINTENANCE_SIGNATURE_LENGTH         54u

#define PROFINET_MIN_ETHERNET_FRAME_LENGTH                  60u
#define PROFINET_MIN_ETHERNET_VLAN_FRAME_LENGTH             64u
#define PROFINET_MAX_ETHERNET_FRAME_LENGTH                  1518u
#define PROFINET_MAX_RPC_FRAME_LENGTH                       1464u

/*===========================================================================*/

#define PROFINET_AR_PROPERTY_STATE_BACKUP                   0x00000000uL
#define PROFINET_AR_PROPERTY_STATE_PRIMARY                  0x00000001uL
#define PROFINET_AR_PROPERTY_STATE_MASK                     (PROFINET_AR_PROPERTY_STATE_BACKUP | \
                                                             PROFINET_AR_PROPERTY_STATE_PRIMARY)
#define PROFINET_AR_PROPERTY_TAKEOVER_NOT_ALLOWED           0x00000000uL
#define PROFINET_AR_PROPERTY_TAKEOVER_ALLOWED               0x00000008uL
#define PROFINET_AR_PROPERTY_PARAMETER_SERVER_EXT           0x00000000uL
#define PROFINET_AR_PROPERTY_PARAMETER_SERVER_CM            0x00000010uL
#define PROFINET_AR_PROPERTY_DATA_RATE_AT_LEAST_100MBIT     0x00000000uL
#define PROFINET_AR_PROPERTY_DATA_RATE_100MBIT              0x00000020uL
#define PROFINET_AR_PROPERTY_DATA_RATE_1GBIT                0x00000040uL
#define PROFINET_AR_PROPERTY_DATA_RATE_10GBIT               0x00000060uL
#define PROFINET_AR_PROPERTY_DATA_RATE_MASK                 (PROFINET_AR_PROPERTY_DATA_RATE_AT_LEAST_100MBIT | \
                                                             PROFINET_AR_PROPERTY_DATA_RATE_100MBIT          | \
                                                             PROFINET_AR_PROPERTY_DATA_RATE_1GBIT            | \
                                                             PROFINET_AR_PROPERTY_DATA_RATE_10GBIT)
#define PROFINET_AR_PROPERTY_ACCESS_BY_EXP_SUB_BLOCK        0x00000000uL
#define PROFINET_AR_PROPERTY_ACCESS_BY_DEVICE               0x00000100uL
#define PROFINET_AR_PROPERTY_COMPANION_SINGLE_AR            0x00000000uL
#define PROFINET_AR_PROPERTY_FIRST_COMPANION_AR             0x00000200uL
#define PROFINET_AR_PROPERTY_COMPANION_AR                   0x00000400uL
#define PROFINET_AR_PROPERTY_COMPANION_AR_WITHOUT_ACK       0x00000000uL
#define PROFINET_AR_PROPERTY_COMPANION_AR_WITH_ACK          0x00000800uL
#define PROFINET_AR_PROPERTY_STARTUP_MODE_LEGACY            0x00000000uL
#define PROFINET_AR_PROPERTY_STARTUP_MODE_ADVANCED          0x40000000uL
#define PROFINET_AR_PROPERTY_STARTUP_MODE_MASK              (PROFINET_AR_PROPERTY_STARTUP_MODE_LEGACY | \
                                                             PROFINET_AR_PROPERTY_STARTUP_MODE_ADVANCED)
#define PROFINET_AR_PROPERTY_PULL_MODULE_ALARM_ALLOWED      0x80000000uL
#define PROFINET_AR_PROPERTY_MASK                           (PROFINET_AR_PROPERTY_STATE_MASK               | \
                                                             PROFINET_AR_PROPERTY_TAKEOVER_NOT_ALLOWED     | \
                                                             PROFINET_AR_PROPERTY_TAKEOVER_ALLOWED         | \
                                                             PROFINET_AR_PROPERTY_PARAMETER_SERVER_EXT     | \
                                                             PROFINET_AR_PROPERTY_PARAMETER_SERVER_CM      | \
                                                             PROFINET_AR_PROPERTY_DATA_RATE_MASK           | \
                                                             PROFINET_AR_PROPERTY_ACCESS_BY_EXP_SUB_BLOCK  | \
                                                             PROFINET_AR_PROPERTY_ACCESS_BY_DEVICE         | \
                                                             PROFINET_AR_PROPERTY_COMPANION_SINGLE_AR      | \
                                                             PROFINET_AR_PROPERTY_COMPANION_AR_WITHOUT_ACK | \
                                                             PROFINET_AR_PROPERTY_STARTUP_MODE_MASK        | \
                                                             PROFINET_AR_PROPERTY_PULL_MODULE_ALARM_ALLOWED)

/*===========================================================================*/

#define PROFINET_IO_CR_TYPE_INPUT                           0x0001u
#define PROFINET_IO_CR_TYPE_OUTPUT                          0x0002u
#define PROFINET_IO_CR_TYPE_MULTICAST_PROVIDER              0x0003u
#define PROFINET_IO_CR_TYPE_MULTICAST_CONSUMER              0x0004u

/*--- IO CR Properties ------------------------------------------------------*/

#define PROFINET_IO_CR_RT_CLASS1                            0x00000001uL
#define PROFINET_IO_CR_RT_CLASS2                            0x00000002uL
#define PROFINET_IO_CR_RT_CLASS3                            0x00000003uL
#define PROFINET_IO_CR_RT_CLASS_UDP                         0x00000004uL
#define PROFINET_IO_CR_RT_CLASS_MASK                        (PROFINET_IO_CR_RT_CLASS1 | \
                                                             PROFINET_IO_CR_RT_CLASS2 | \
                                                             PROFINET_IO_CR_RT_CLASS3 | \
                                                             PROFINET_IO_CR_RT_CLASS_UDP)
#define PROFINET_IO_CR_INTERFACE_MASK                       0x00000018uL
#define PROFINET_IO_CR_CLOCK_SYNC_MASK                      0x000000E0uL
#define PROFINET_IO_CR_ADDRESS_RESOLUTION_MASK              0x00000300uL
#define PROFINET_IO_CR_NO_MEDIUM_REDUNDANCY                 0x00000000uL
#define PROFINET_IO_CR_MEDIUM_REDUNDANCY                    0x00000400uL
#define PROFINET_IO_CR_MEDIUM_REDUNDANCY_MASK               0x00000400uL
#define PROFINET_IO_CR_PROPERTIES_RESERVED_MASK             0xFFFFF800uL

/*---------------------------------------------------------------------------*/

#define PROFINET_IO_CR_MIN_SEND_CLOCK                       0x0008u
#define PROFINET_IO_CR_SEND_CLOCK_250US                     0x0008u
#define PROFINET_IO_CR_SEND_CLOCK_500US                     0x0010u
#define PROFINET_IO_CR_SEND_CLOCK_1MS                       0x0020u
#define PROFINET_IO_CR_SEND_CLOCK_2MS                       0x0040u
#define PROFINET_IO_CR_SEND_CLOCK_3MS                       0x0060u
#define PROFINET_IO_CR_SEND_CLOCK_4MS                       0x0080u
#define PROFINET_IO_CR_MAX_SEND_CLOCK                       0x0080u

#define PROFINET_IO_CR_REDUCTION_RATIO1                     0x0001u
#define PROFINET_IO_CR_REDUCTION_RATIO2                     0x0002u
#define PROFINET_IO_CR_REDUCTION_RATIO4                     0x0004u
#define PROFINET_IO_CR_REDUCTION_RATIO8                     0x0008u
#define PROFINET_IO_CR_REDUCTION_RATIO16                    0x0010u
#define PROFINET_IO_CR_REDUCTION_RATIO32                    0x0020u
#define PROFINET_IO_CR_REDUCTION_RATIO64                    0x0040u
#define PROFINET_IO_CR_REDUCTION_RATIO128                   0x0080u
#define PROFINET_IO_CR_REDUCTION_RATIO256                   0x0100u
#define PROFINET_IO_CR_REDUCTION_RATIO512                   0x0200u
#define PROFINET_IO_CR_REDUCTION_RATIO1024                  0x0400u
#define PROFINET_IO_CR_REDUCTION_RATIO2048                  0x0800u
#define PROFINET_IO_CR_REDUCTION_RATIO4096                  0x1000u
#define PROFINET_IO_CR_REDUCTION_RATIO8192                  0x2000u
#define PROFINET_IO_CR_REDUCTION_RATIO16384                 0x4000u

#define PROFINET_IO_CR_MIN_PHASE                            PROFINET_IO_CR_REDUCTION_RATIO1
#define PROFINET_IO_CR_MAX_PHASE                            PROFINET_IO_CR_REDUCTION_RATIO16384

#define PROFINET_IO_CR_TAG_NO_VLAN                          0x0000uL
#define PROFINET_IO_CR_TAG_DEFAULT_VLAN                     0x0001uL
#define PROFINET_IO_CR_TAG_USER_PRIORITY                    0xC000uL
#define PROFINET_IO_CR_TAG_MASK                             (PROFINET_IO_CR_TAG_NO_VLAN      | \
                                                             PROFINET_IO_CR_TAG_DEFAULT_VLAN | \
                                                             PROFINET_IO_CR_TAG_USER_PRIORITY)

#define PROFINET_IO_CR_MIN_FRAME_SEND_OFFSET                0x00000000uL
#define PROFINET_IO_CR_MAX_FRAME_SEND_OFFSET                0x003D08FFuL
#define PROFINET_IO_CR_FRAME_SEND_AS_FAST_AS_POSSIBLE       0xFFFFFFFFuL

#define PROFINET_IO_CR_MIN_WATCHDOG_FACTOR                  0x0001u
#define PROFINET_IO_CR_MAX_WATCHDOG_FACTOR                  0x1E00u

#define PROFINET_IO_CR_MIN_DATA_HOLD_FACTOR                 0x0001u
#define PROFINET_IO_CR_MAX_DATA_HOLD_FACTOR                 0x1E00u

/*===========================================================================*/

#define PROFINET_DEFAULT_API                                0x00000000uL
#define PROFINET_DRIVE_API                                  0x00003A00uL
#define PROFINET_ENCODER_API                                0x00003D00uL
#define PROFINET_INTELLIGENT_PUMP_API                       0x00005D00uL

/*===========================================================================*/

#define PROFINET_IO_MODULE_MIN_SLOT_NUMBER                  0x0000u
#define PROFINET_IO_MODULE_MAX_SLOT_NUMBER                  0x7FFFu

/*---------------------------------------------------------------------------*/

#define PROFINET_MIN_MODULE_IDENTNUMBER                     0x00000001u

/*---------------------------------------------------------------------------*/

#define PROFINET_MODULE_STATE_NO_MODULE                     0x0000u
#define PROFINET_MODULE_STATE_WRONG_MODULE                  0x0001u
#define PROFINET_MODULE_STATE_PROPER_MODULE                 0x0002u
#define PROFINET_MODULE_STATE_SUBSTITUTE_MODULE             0x0003u
#define PROFINET_MODULE_STATE_MASK                          (PROFINET_MODULE_STATE_NO_MODULE     | \
                                                             PROFINET_MODULE_STATE_WRONG_MODULE  | \
                                                             PROFINET_MODULE_STATE_PROPER_MODULE | \
                                                             PROFINET_MODULE_STATE_SUBSTITUTE_MODULE)

/*===========================================================================*/

#define PROFINET_IO_SUBMODULE_MIN_SUB_SLOT_NUMBER           0x0000u
#define PROFINET_IO_SUBMODULE_MAX_SUB_SLOT_NUMBER           0x8FFFu
#define PROFINET_IO_SUBMODULE_MAX_SUB_SLOT_NUMBER_SLOT_0    0x8FFFu

#define PROFINET_SUBSLOT_INTERFACE                          0x8000
#define PROFINET_SUBSLOT_INTERFACE_MASK                     0x8F00
#define PROFINET_SUBSLOT_PORT_BASE                          0x8001
#define PROFINET_SUBSLOT_PORT_MASK                          0x80FF

/*---------------------------------------------------------------------------*/

#define PROFINET_MIN_SUBMODULE_IDENTNUMBER                  0x00000001u

/*---------------------------------------------------------------------------*/

#define PROFINET_IO_SUBMODULE_TYPE_NO_INPUT_NO_OUTPUT       0x0000u
#define PROFINET_IO_SUBMODULE_TYPE_INPUT                    0x0001u
#define PROFINET_IO_SUBMODULE_TYPE_OUTPUT                   0x0002u
#define PROFINET_IO_SUBMODULE_TYPE_INPUT_AND_OUTPUT         0x0003u
#define PROFINET_IO_SUBMODULE_TYPE_IO_MASK                  (PROFINET_IO_SUBMODULE_TYPE_NO_INPUT_NO_OUTPUT | \
                                                             PROFINET_IO_SUBMODULE_TYPE_INPUT              | \
                                                             PROFINET_IO_SUBMODULE_TYPE_OUTPUT             | \
                                                             PROFINET_IO_SUBMODULE_TYPE_INPUT_AND_OUTPUT)

#define PROFINET_IO_SUBMODULE_TYPE_SHARED_INPUT             0x0004u
#define PROFINET_IO_SUBMODULE_TYPE_NORMAL_INPUT             0x0000u
#define PROFINET_IO_SUBMODULE_TYPE_REDUCED_INPUT            0x0008u
#define PROFINET_IO_SUBMODULE_TYPE_NORMAL_OUTPUT            0x0000u
#define PROFINET_IO_SUBMODULE_TYPE_REDUCED_OUTPUT           0x0010u
#define PROFINET_IO_SUBMODULE_TYPE_NORMAL_IOXS              0x0000u
#define PROFINET_IO_SUBMODULE_TYPE_DISCARD_IOXS             0x0020u

#define PROFINET_IO_SUBMODULE_TYPE_MASK                     (PROFINET_IO_SUBMODULE_TYPE_IO_MASK        | \
                                                             PROFINET_IO_SUBMODULE_TYPE_SHARED_INPUT   | \
                                                             PROFINET_IO_SUBMODULE_TYPE_NORMAL_INPUT   | \
                                                             PROFINET_IO_SUBMODULE_TYPE_REDUCED_INPUT  | \
                                                             PROFINET_IO_SUBMODULE_TYPE_NORMAL_OUTPUT  | \
                                                             PROFINET_IO_SUBMODULE_TYPE_REDUCED_OUTPUT | \
                                                             PROFINET_IO_SUBMODULE_TYPE_NORMAL_IOXS    | \
                                                             PROFINET_IO_SUBMODULE_TYPE_DISCARD_IOXS)

/*===========================================================================*/

#define PROFINET_IO_DATA_MIN_LENGTH                         40u
#define PROFINET_IO_DATA_MAX_LENGTH                         1440u
#define PROFINET_UDP_IO_DATA_MIN_LENGTH                     12u
#define PROFINET_UDP_IO_DATA_MAX_LENGTH                     1412u

/*---------------------------------------------------------------------------*/

#define CYCLIC_DATA_STATUS_STATE_PRIMARY                    0x01u
#define CYCLIC_DATA_STATUS_STATE_BACKUP                     0x00u
#define CYCLIC_DATA_STATUS_DATA_VALID                       0x04u
#define CYCLIC_DATA_STATUS_DATA_INVALID                     0x00u
#define CYCLIC_DATA_STATUS_STATE_RUN                        0x10u
#define CYCLIC_DATA_STATUS_STATE_STOP                       0x00u
#define CYCLIC_DATA_STATUS_NORMAL_OPERATION                 0x20u
#define CYCLIC_DATA_STATUS_PROBLEM_DETECTED                 0x00u
#define CYCLIC_DATA_STATUS_MASK                             (CYCLIC_DATA_STATUS_STATE_PRIMARY   | \
                                                            CYCLIC_DATA_STATUS_STATE_BACKUP     | \
                                                            CYCLIC_DATA_STATUS_DATA_VALID       | \
                                                            CYCLIC_DATA_STATUS_DATA_INVALID     | \
                                                            CYCLIC_DATA_STATUS_STATE_RUN        | \
                                                            CYCLIC_DATA_STATUS_STATE_STOP       | \
                                                            CYCLIC_DATA_STATUS_NORMAL_OPERATION | \
                                                            CYCLIC_DATA_STATUS_PROBLEM_DETECTED)

/*===========================================================================*/

#define PROFINET_ALARM_CR_TYPE                              0x0001u

/*---------------------------------------------------------------------------*/

#define PROFINET_MIN_RTA_ALARM_FACTOR                       1u
#define PROFINET_MIN_RTA_ALARM_RETRY                        3u
#define PROFINET_MAX_RTA_ALARM_RETRY                        15u

/*---------------------------------------------------------------------------*/

#define PROFINET_ALARM_CR_PRIORITY_DEFAULT                  0x00000000uL
#define PROFINET_ALARM_CR_PRIORITY_LOW                      0x00000001uL
#define PROFINET_ALARM_CR_PRIORITY_MASK                     (PROFINET_ALARM_CR_PRIORITY_DEFAULT | \
                                                             PROFINET_ALARM_CR_PRIORITY_LOW)

#define PROFINET_ALARM_CR_USE_DATA_RTA                      0x00000000uL
#define PROFINET_ALARM_CR_USE_UDP_RTA                       0x00000002uL
#define PROFINET_ALARM_CR_TRANSPORT_MASK                    (PROFINET_ALARM_CR_USE_DATA_RTA | \
                                                             PROFINET_ALARM_CR_USE_UDP_RTA)

#define PROFINET_ALARM_CR_MASK                              (PROFINET_ALARM_CR_PRIORITY_MASK | \
                                                             PROFINET_ALARM_CR_TRANSPORT_MASK)

#define PROFINET_ALARM_CR_TAG_VLAN_MASK                     0x0FFFuL
#define PROFINET_ALARM_CR_TAG_HIGH_PRIORITY_HIGH            0xC000uL
#define PROFINET_ALARM_CR_TAG_HIGH_MASK                     (PROFINET_ALARM_CR_TAG_VLAN_MASK | \
                                                             PROFINET_ALARM_CR_TAG_HIGH_PRIORITY_HIGH)
#define PROFINET_ALARM_CR_TAG_LOW_PRIORITY_LOW              0xA000uL
#define PROFINET_ALARM_CR_TAG_LOW_MASK                      (PROFINET_ALARM_CR_TAG_VLAN_MASK | \
                                                             PROFINET_ALARM_CR_TAG_LOW_PRIORITY_LOW)

/*===========================================================================*/

#define PROFINET_ALARM_DATA_MIN_LENGTH                      200u
#define PROFINET_ALARM_DATA_MAX_LENGTH                      1432u

/*===========================================================================*/

#define PROFINET_MC_CR_ADDR_RESOLUTION_DNS                  0x00000001uL
#define PROFINET_MC_CR_ADDR_RESOLUTION_DCP                  0x00000002uL
#define PROFINET_MC_CR_ADDR_RESOLUTION_MASK                 (PROFINET_MC_CR_ADDR_RESOLUTION_DNS | \
                                                             PROFINET_MC_CR_ADDR_RESOLUTION_DCP)

#define PROFINET_MC_CR_ADDR_RESOLUTION_MIN_FACTOR           0x00010000uL
#define PROFINET_MC_CR_ADDR_RESOLUTION_MAX_FACTOR           0xFFFF0000uL
#define PROFINET_MC_CR_ADDR_RESOLUTION_FACTOR_MASK          (PROFINET_MC_CR_ADDR_RESOLUTION_MIN_FACTOR | \
                                                             PROFINET_MC_CR_ADDR_RESOLUTION_MAX_FACTOR)

#define PROFINET_MC_CR_MASK                                 (PROFINET_MC_CR_ADDR_RESOLUTION_MASK | \
                                                             PROFINET_MC_CR_ADDR_RESOLUTION_FACTOR_MASK)

/*---------------------------------------------------------------------------*/

#define PROFINET_MIN_MCI_TIMEOUT                            0u
#define PROFINET_MAX_MCI_TIMEOUT                            100u

/*===========================================================================*/

#define PROFINET_MIN_INDEX_NUMBER                           0x0000u
#define PROFINET_MAX_INDEX_NUMBER                           0x7FFFu

/*===========================================================================*/

#define PROFINET_DCP_DATA_BLOCK_MAX_LENGTH                  1460u

/*===========================================================================*/

#define PROFINET_MRP_ROLE_DISABLED                          0x0000u
#define PROFINET_MRP_ROLE_CLIENT                            0x0001u
#define PROFINET_MRP_ROLE_MANAGER                           0x0002u

/*---------------------------------------------------------------------------*/

#define PROFINET_MRP_DEFAULT_LINK_DOWN_TIME                 20u
#define PROFINET_MRP_DEFAULT_LINK_UP_TIME                   20u
#define PROFINET_MRP_DEFAULT_LINK_RETRIES                   4u

/*---------------------------------------------------------------------------*/

#define PROFINET_MRP_RING_OPEN                              0x0000u
#define PROFINET_MRP_RING_CLOSED                            0x0001u

/*---------------------------------------------------------------------------*/

#define PROFINET_MRP_REDUNDANCY_LOST                        0x0000u
#define PROFINET_MRP_REDUNDANCY_OK                          0x0001u

/*---------------------------------------------------------------------------*/

#define PROFINET_MRP_CHECK_MANAGER_OFF                      0x00000000uL
#define PROFINET_MRP_CHECK_MANAGER_ON                       0x00000001uL
#define PROFINET_MRP_CHECK_DOMAIN_ON                        0x00000002uL

/*===========================================================================*/

#define PNIO_ERROR_CODE1_APPLICATION                        (PN_U8) 0xA0u
#define PNIO_ERROR_CODE1_ACCESS                             (PN_U8) 0xB0u
#define PNIO_ERROR_CODE1_RESOURCE                           (PN_U8) 0xC0u

#define PNIO_ERROR_CODE1_ACCESS_INVALID_INDEX               (PN_U8) (PNIO_ERROR_CODE1_ACCESS | 0x00u)
#define PNIO_ERROR_CODE1_ACCESS_INVALID_LENGTH              (PN_U8) (PNIO_ERROR_CODE1_ACCESS | 0x01u)
#define PNIO_ERROR_CODE1_ACCESS_INVALID_SLOT_SUBSLOT        (PN_U8) (PNIO_ERROR_CODE1_ACCESS | 0x02u)
#define PNIO_ERROR_CODE1_ACCESS_TYPE_CONFLICT               (PN_U8) (PNIO_ERROR_CODE1_ACCESS | 0x03u)
#define PNIO_ERROR_CODE1_ACCESS_INVALID_API                 (PN_U8) (PNIO_ERROR_CODE1_ACCESS | 0x04u)
#define PNIO_ERROR_CODE1_ACCESS_STATE_CONFLICT              (PN_U8) (PNIO_ERROR_CODE1_ACCESS | 0x05u)
#define PNIO_ERROR_CODE1_ACCESS_DENIED                      (PN_U8) (PNIO_ERROR_CODE1_ACCESS | 0x06u)
#define PNIO_ERROR_CODE1_ACCESS_INVALID_RANGE               (PN_U8) (PNIO_ERROR_CODE1_ACCESS | 0x07u)
#define PNIO_ERROR_CODE1_ACCESS_INVALID_PARAMETER           (PN_U8) (PNIO_ERROR_CODE1_ACCESS | 0x08u)
#define PNIO_ERROR_CODE1_ACCESS_INVALID_TYPE                (PN_U8) (PNIO_ERROR_CODE1_ACCESS | 0x09u)
#define PNIO_ERROR_CODE1_ACCESS_BACKUP                      (PN_U8) (PNIO_ERROR_CODE1_ACCESS | 0x0Au)

/*===========================================================================*/

typedef struct _T_PN_MAC_ADDRESS
{
  PN_U8   HighAndVersionHighByte;
  PN_U8   HighAndVersionLowByte;

  PN_U8   MidHighByte;
  PN_U8   MidLowByte;

  PN_U8   LowHighByte;
  PN_U8   LowLowByte;

} PACK_BYTE_ALIGNMENT (T_PN_MAC_ADDRESS);

/*---------------------------------------------------------------------------*/

typedef struct _T_PROFINET_UUID
{
  PN_U8   TimeLowHighWordHighByte;
  PN_U8   TimeLowHighWordLowByte;
  PN_U8   TimeLowLowWordHighByte;
  PN_U8   TimeLowLowWordLowByte;

  PN_U8   TimeMidHighByte;
  PN_U8   TimeMidLowByte;

  PN_U8   TimeHighAndVersionHighByte;
  PN_U8   TimeHighAndVersionLowByte;

  PN_U8   Clock [2];
  PN_U8   Device [6];

} PACK_BYTE_ALIGNMENT (T_PROFINET_UUID);

/*===========================================================================*/

#define MAX_NUMBER_SUPPORTED_DEVICES                  (PN_U16) 256u

#define PN_DEVICE_REFERENCE_THIS_STATION              (PN_U16) 0x0000u

/*===========================================================================*/

#define PN                                            (PN_U8) 0u
#define PN_CONTROLLER                                 (PN_U8) 1u
#define PN_DEVICE                                     (PN_U8) 2u
#define PN_SUPERVISOR                                 (PN_U8) 3u

#define PN_NUMBER_INSTANCES                           (PN_U8) (PN_SUPERVISOR + 1)

/*---------------------------------------------------------------------------*/

#define PN_SERVICE_MASK                               0xF0u
#define PN_SERVICE_CLASS_MASK                         0x0Fu
#define PN_NUMBER_SERVICES_PER_CLASS                  16u
#define PN_NUMBER_SERVICE_CLASSES                     16u

#define _PN_GET_SERVICE_CLASS(ServiceClass)           ((PN_U8) ((ServiceClass) >> 4u))

#define PN_SERVICE_CLASS1                             0x00u
#define PN_SERVICE_CLASS2                             0x10u
#define PN_SERVICE_CLASS3                             0x20u
#define PN_SERVICE_CLASS4                             0x30u
#define PN_SERVICE_CLASS5                             0x40u
#define PN_SERVICE_CLASS6                             0x80u

#define PN_SERVICE_CLASS_CMCTL_CMDEV                  (PN_U8) _PN_GET_SERVICE_CLASS (PN_SERVICE_CLASS1)
#define PN_SERVICE_GET_DEVICE_STATE                   (PN_U8) (PN_SERVICE_CLASS1 | 0x00u)
#define PN_SERVICE_CONNECT                            (PN_U8) (PN_SERVICE_CLASS1 | 0x01u)
#define PN_SERVICE_WRITE                              (PN_U8) (PN_SERVICE_CLASS1 | 0x02u)
#define PN_SERVICE_READ                               (PN_U8) (PN_SERVICE_CLASS1 | 0x03u)
#define PN_SERVICE_CONTROL                            (PN_U8) (PN_SERVICE_CLASS1 | 0x04u)
#define PN_SERVICE_WRITE_MULTIPLE                     (PN_U8) (PN_SERVICE_CLASS1 | 0x05u)
#define PN_SERVICE_CLASS_CMCTL_CMDEV_MAX_SERVICE      PN_SERVICE_WRITE_MULTIPLE

#define PN_SERVICE_CLASS_ALARM                        (PN_U8) _PN_GET_SERVICE_CLASS (PN_SERVICE_CLASS2)
#define PN_SERVICE_GET_ALARM                          (PN_U8) (PN_SERVICE_CLASS2 | 0x00u)
#define PN_SERVICE_ALARM_ACK                          (PN_U8) (PN_SERVICE_CLASS2 | 0x01u)
#define PN_SERVICE_ALARM                              (PN_U8) (PN_SERVICE_CLASS2 | 0x02u)
#define PN_SERVICE_GET_ALARM_ACK                      (PN_U8) (PN_SERVICE_CLASS2 | 0x03u)
#define PN_SERVICE_CLASS_ALARM_MAX_SERVICE            PN_SERVICE_GET_ALARM_ACK

#define PN_SERVICE_CLASS_DCP                          (PN_U8) _PN_GET_SERVICE_CLASS (PN_SERVICE_CLASS3)
#define PN_SERVICE_APPLICATION_DATA_GET               (PN_U8) (PN_SERVICE_CLASS3 | 0x00u)
#define PN_SERVICE_APPLICATION_DATA_SET               (PN_U8) (PN_SERVICE_CLASS3 | 0x01u)
#define PN_SERVICE_APPLICATION_IMPLICIT_READ          (PN_U8) (PN_SERVICE_CLASS3 | 0x02u)
#define PN_SERVICE_GET_ETHERNET_DATA                  (PN_U8) (PN_SERVICE_CLASS3 | 0x03u)
#define PN_SERVICE_CLASS_DCP_MAX_SERVICE              PN_SERVICE_GET_ETHERNET_DATA

#define PN_SERVICE_CLASS_CONFIG                       (PN_U8) _PN_GET_SERVICE_CLASS (PN_SERVICE_CLASS4)
#define PN_SERVICE_DOWNLOAD                           (PN_U8) (PN_SERVICE_CLASS4 | 0x00u)
#define PN_SERVICE_SET_IDENTIFICATION                 (PN_U8) (PN_SERVICE_CLASS4 | 0x01u)
#define PN_SERVICE_CONFIGURE_WATCHDOG                 (PN_U8) (PN_SERVICE_CLASS4 | 0x02u)
#define PN_SERVICE_DOWNLOAD_NEIGHBOUR_DATA            (PN_U8) (PN_SERVICE_CLASS4 | 0x03u)
#define PN_SERVICE_CONFIGURE_LICENCE                  (PN_U8) (PN_SERVICE_CLASS4 | 0x04u)
#define PN_SERVICE_DOWNLOAD_IRT_DOMAIN                (PN_U8) (PN_SERVICE_CLASS4 | 0x05u)
#define PN_SERVICE_AR_VENDOR_DATA                     (PN_U8) (PN_SERVICE_CLASS4 | 0x06u)
#define PN_SERVICE_CONFIGURE_REDUNDANCY               (PN_U8) (PN_SERVICE_CLASS4 | 0x07u)
#define PN_SERVICE_CLASS_CONFIG_MAX_SERVICE           PN_SERVICE_CONFIGURE_LICENCE

#define PN_SERVICE_CLASS_TOPOLOGY                     (PN_U8) _PN_GET_SERVICE_CLASS (PN_SERVICE_CLASS5)
#define PN_SERVICE_LOCAL_AND_PEER_CHECK               (PN_U8) (PN_SERVICE_CLASS5 | 0x00u)
#define PN_SERVICE_CLASS_TOPOLOGY_MAX_SERVICE         PN_SERVICE_LOCAL_AND_PEER_CHECK

#define PN_SERVICE_CLASS_SUPERVISOR                   (PN_U8) _PN_GET_SERVICE_CLASS (PN_SERVICE_CLASS1)
#define PN_SERVICE_GET_LIST_OF_STATION                (PN_U8) (PN_SERVICE_CLASS1 | 0x00u)
#define PN_SERVICE_SET_DEVICE_NAME                    (PN_U8) (PN_SERVICE_CLASS1 | 0x01u)
#define PN_SERVICE_SET_IP_SETTINGS                    (PN_U8) (PN_SERVICE_CLASS1 | 0x02u)
#define PN_SERVICE_DCP_SET                            (PN_U8) (PN_SERVICE_CLASS1 | 0x03u)
#define PN_SERVICE_DCP_GET                            (PN_U8) (PN_SERVICE_CLASS1 | 0x04u)
#define PN_SERVICE_IMPLICIT_READ                      (PN_U8) (PN_SERVICE_CLASS1 | 0x05u)
#define PN_SERVICE_LOOKUP                             (PN_U8) (PN_SERVICE_CLASS1 | 0x06u)
#define PN_SERVICE_DISCOVER                           (PN_U8) (PN_SERVICE_CLASS1 | 0x07u)
#define PN_SERVICE_CLASS_SUPERVISOR_MAX_SERVICE       PN_SERVICE_DISCOVER

/*---------------------------------------------------------------------------*/

#define PN_SERVICE_ERROR_CODE_LOCAL                   (PN_U8) 0x00u
#define PN_SERVICE_ERROR_CODE_STACK                   (PN_U8) 0x01u
#define PN_SERVICE_ERROR_CODE_REMOTE                  (PN_U8) 0x02u

/*--- local errors ----------------------------------------------------------*/

#define PN_SERVICE_OK                                 (PN_S16) 0
#define PN_SERVICE_ERROR_INVALID_NUMBER_OF_SERVICES   (PN_S16) 1
#define PN_SERVICE_ERROR_INVALID_SERVICE_LENGTH       (PN_S16) 2
#define PN_SERVICE_ERROR_INVALID_SERVICE_OFFSET       (PN_S16) 3
#define PN_SERVICE_ERROR_INVALID_INSTANCE             (PN_S16) 4
#define PN_SERVICE_ERROR_INVALID_SERVICE              (PN_S16) 5
#define PN_SERVICE_ERROR_NOT_IMPLEMENTED              (PN_S16) 6
#define PN_SERVICE_ERROR_NO_PARALLEL_SERVICES         (PN_S16) 7
#define PN_SERVICE_ERROR_TEMPORARILY_NOT_EXECUTABLE   (PN_S16) 8
#define PN_SERVICE_ERROR_NO_RESOURCE                  (PN_S16) 9

#define PN_SERVICE_ERROR_INVALID_PARAMETER            (PN_S16) 16
#define PN_SERVICE_ERROR_WRONG_STATE                  (PN_S16) 17
#define PN_SERVICE_ERROR_INVALID_DEVICE_REF           (PN_S16) 18
#define PN_SERVICE_ERROR_WRONG_MODE                   (PN_S16) 19

/*---------------------------------------------------------------------------*/

typedef struct _T_PN_SERVICE_ERROR_CON
{
  PN_U8    Code;
  PN_U8    Detail;

  PN_U8    AdditionalDetail;
  PN_U8    AreaCode;

} PACK_BYTE_ALIGNMENT (T_PN_SERVICE_ERROR_CON);

/*===========================================================================*/

#define PROFINET_IO_MCR_FRAME_ID_RT_CLASS1_BASE               (PN_U16) 0xF800u
#define PROFINET_IO_MCR_FRAME_ID_RT_CLASS1_NUMBER_IDS         (PN_U16) 0x03FFu
#define PROFINET_IO_MCR_FRAME_ID_RT_CLASS1_HIGHEST_FRAME_ID   (PN_U16) (PROFINET_IO_MCR_FRAME_ID_RT_CLASS1_BASE + \
                                                                        PROFINET_IO_MCR_FRAME_ID_RT_CLASS1_NUMBER_IDS)
#define PROFINET_IO_MCR_FRAME_ID_RT_CLASS2_BASE               (PN_U16) 0xBC00u
#define PROFINET_IO_MCR_FRAME_ID_RT_CLASS2_NUMBER_IDS         (PN_U16) 0x03FFu
#define PROFINET_IO_MCR_FRAME_ID_RT_CLASS2_HIGHEST_FRAME_ID   (PN_U16) (PROFINET_IO_MCR_FRAME_ID_RT_CLASS2_BASE + \
                                                                        PROFINET_IO_MCR_FRAME_ID_RT_CLASS2_NUMBER_IDS)

typedef struct _T_PN_MCR_PROVIDER
{
  PN_U8   FrameIdHighByte;
  PN_U8   FrameIdLowByte;

} PACK_BYTE_ALIGNMENT (T_PN_MCR_PROVIDER);

/*---------------------------------------------------------------------------*/

typedef struct _T_PN_MCR_CONSUMER
{
  PN_U8   FrameIdHighByte;
  PN_U8   FrameIdLowByte;

  PN_U8   PropertiesHighWordHighByte;
  PN_U8   PropertiesHighWordLowByte;
  PN_U8   PropertiesLowWordHighByte;
  PN_U8   PropertiesLowWordLowByte;

  PN_U8   TimeoutHighByte;
  PN_U8   TimeoutLowByte;

  PN_U8   NameLengthHighByte;
  PN_U8   NameLengthLowByte;

/*
  char    DeviceName [NameLength];
*/
} PACK_BYTE_ALIGNMENT (T_PN_MCR_CONSUMER);

/*---------------------------------------------------------------------------*/

typedef struct _T_PN_ALARM_CR
{
  PN_U8   VersionHighByte;
  PN_U8   VersionLowByte;

  PN_U8   RTATimeoutFactorHighByte;
  PN_U8   RTATimeoutFactorLowByte;

  PN_U8   RTARetryHighByte;
  PN_U8   RTARetryLowByte;

  PN_U8   PropertiesHighWordHighByte;
  PN_U8   PropertiesHighWordLowByte;
  PN_U8   PropertiesLowWordHighByte;
  PN_U8   PropertiesLowWordLowByte;

  PN_U8   MaxAlarmLengthHighByte;
  PN_U8   MaxAlarmLengthLowByte;

  PN_U8   TagHeaderHighHighByte;
  PN_U8   TagHeaderHighLowByte;

  PN_U8   TagHeaderLowHighByte;
  PN_U8   TagHeaderLowLowByte;

} PACK_BYTE_ALIGNMENT (T_PN_ALARM_CR);

/*---------------------------------------------------------------------------*/

typedef struct _T_PN_REFERENCE
{
  PN_U8   ReferenceHighByte;
  PN_U8   ReferenceLowByte;

} PACK_BYTE_ALIGNMENT (T_PN_REFERENCE);

/*---------------------------------------------------------------------------*/

#define PROFINET_IO_CR_IO_STATUS_LENGTH     0x0001u

typedef struct _T_PN_SUBMODULE
{
  PN_U8             SubSlotNumberHighByte;
  PN_U8             SubSlotNumberLowByte;

  PN_U8             IdentNumberHighWordHighByte;
  PN_U8             IdentNumberHighWordLowByte;
  PN_U8             IdentNumberLowWordHighByte;
  PN_U8             IdentNumberLowWordLowByte;

  PN_U8             PropertiesHighByte;
  PN_U8             PropertiesLowByte;

  PN_U8             InputDataLengthHighByte;
  PN_U8             InputDataLengthLowByte;

  PN_U8             OutputDataLengthHighByte;
  PN_U8             OutputDataLengthLowByte;

  PN_U8             ConsumerStatusLength;
  PN_U8             ProviderStatusLength;

  PN_U8             NumberOfDataRecordsHighByte;
  PN_U8             NumberOfDataRecordsLowByte;

/*
  T_PN_REFERENCE    DataRecords [NumberOfDataRecords];
*/

} PACK_BYTE_ALIGNMENT (T_PN_SUBMODULE);

/*---------------------------------------------------------------------------*/

typedef struct _T_PN_MODULE
{
  PN_U8             VersionHighByte;
  PN_U8             VersionLowByte;

  PN_U8             SlotNumberHighByte;
  PN_U8             SlotNumberLowByte;

  PN_U8             IdentNumberHighWordHighByte;
  PN_U8             IdentNumberHighWordLowByte;
  PN_U8             IdentNumberLowWordHighByte;
  PN_U8             IdentNumberLowWordLowByte;

  PN_U8             PropertiesHighByte;
  PN_U8             PropertiesLowByte;

  PN_U8             NumberOfSubmodulesHighByte;
  PN_U8             NumberOfSubmodulesLowByte;

/*
  T_PN_SUBMODULE    Submodule [NumberOfSubmodules];
*/

} PACK_BYTE_ALIGNMENT (T_PN_MODULE);

/*---------------------------------------------------------------------------*/

typedef struct _T_PN_DATA_RECORD
{
  PN_U8   VersionHighByte;
  PN_U8   VersionLowByte;

  PN_U8   SequenceHighByte;
  PN_U8   SequenceLowByte;

  PN_U8   APIHighWordHighByte;
  PN_U8   APIHighWordLowByte;
  PN_U8   APILowWordHighByte;
  PN_U8   APILowWordLowByte;

  PN_U8   IndexHighByte;
  PN_U8   IndexLowByte;

  PN_U8   LengthHighByte;
  PN_U8   LengthLowByte;

/*
  PN_U8   Data [Length];
*/

} PACK_BYTE_ALIGNMENT (T_PN_DATA_RECORD);

/*---------------------------------------------------------------------------*/

typedef struct _T_PN_API
{
  PN_U8             APIHighWordHighByte;
  PN_U8             APIHighWordLowByte;
  PN_U8             APILowWordHighByte;
  PN_U8             APILowWordLowByte;

  PN_U8             NumberOfModulesHighByte;
  PN_U8             NumberOfModulesLowByte;

/*
  T_PN_REFERENCE    Module [NumberOfModules];
*/

} PACK_BYTE_ALIGNMENT (T_PN_API);

/*---------------------------------------------------------------------------*/

typedef struct _T_PN_IOCR
{
  PN_U8               VersionHighByte;
  PN_U8               VersionLowByte;

  PN_U8               TypeHighByte;
  PN_U8               TypeLowByte;

  PN_U8               PropertiesHighWordHighByte;
  PN_U8               PropertiesHighWordLowByte;
  PN_U8               PropertiesLowWordHighByte;
  PN_U8               PropertiesLowWordLowByte;

  PN_U8               SendClockFactorHighByte;
  PN_U8               SendClockFactorLowByte;

  PN_U8               ReductionRatioHighByte;
  PN_U8               ReductionRatioLowByte;

  PN_U8               PhaseHighByte;
  PN_U8               PhaseLowByte;

  PN_U8               SequenceHighByte;
  PN_U8               SequenceLowByte;

  PN_U8               WatchdogFactorHighByte;
  PN_U8               WatchdogFactorLowByte;

  PN_U8               DataHoldFactorHighByte;
  PN_U8               DataHoldFactorLowByte;

  PN_U8               FrameSendOffsetHighWordHighByte;
  PN_U8               FrameSendOffsetHighWordLowByte;
  PN_U8               FrameSendOffsetLowWordHighByte;
  PN_U8               FrameSendOffsetLowWordLowByte;

  PN_U8               TagHeaderHighByte;
  PN_U8               TagHeaderLowByte;

  T_PN_MAC_ADDRESS    MulticastAddr;

  PN_U8               NumberOfAPIsHighByte;
  PN_U8               NumberOfAPIsLowByte;

/*
  T_PN_REFERENCE      Api [NumberOfAPIs];
  the following is only available for IOCR type == PROFINET_IO_CR_TYPE_MULTICAST_CONSUMER
  or for IOCR type == PROFINET_IO_CR_TYPE_MULTICAST_PROVIDER

  union
  {
  T_PN_MCR_CONSUMER   MCRConsumer;
  T_PN_MCR_PROVIDER   MCRProvider;
  }
*/

} PACK_BYTE_ALIGNMENT (T_PN_IOCR);

/*---------------------------------------------------------------------------*/

#define PN_SERVICE_DOWNLOAD_INVALID_NUMBER_IOCRS                  0x01u
#define PN_SERVICE_DOWNLOAD_INVALID_NUMBER_API                    0x02u
#define PN_SERVICE_DOWNLOAD_INVALID_NUMBER_MODULES                0x03u
#define PN_SERVICE_DOWNLOAD_INVALID_NUMBER_DATA_RECORDS           0x04u
#define PN_SERVICE_DOWNLOAD_INVALID_DEVICE_NAME                   0x05u
#define PN_SERVICE_DOWNLOAD_INVALID_INTERFACE_NAME_LENGTH         0x06u
#define PN_SERVICE_DOWNLOAD_INVALID_RTA_TIMEOUT                   0x07u
#define PN_SERVICE_DOWNLOAD_INVALID_RTA_RETRY                     0x08u
#define PN_SERVICE_DOWNLOAD_INVALID_ALARM_LENGTH                  0x09u
#define PN_SERVICE_DOWNLOAD_INVALID_IO_CR_TYPE                    0x0Au
#define PN_SERVICE_DOWNLOAD_INVALID_SEND_CLOCK                    0x0Bu
#define PN_SERVICE_DOWNLOAD_INVALID_REDUCTION_RATIO               0x0Cu
#define PN_SERVICE_DOWNLOAD_INVALID_PHASE                         0x0Du
#define PN_SERVICE_DOWNLOAD_INVALID_FRAME_SEND_OFFSET             0x0Eu
#define PN_SERVICE_DOWNLOAD_INVALID_WATCHDOG_FACTOR               0x0Fu
#define PN_SERVICE_DOWNLOAD_INVALID_DATA_HOLD_FACTOR              0x10u
#define PN_SERVICE_DOWNLOAD_INVALID_MULTICAST_ADDR                0x11u
#define PN_SERVICE_DOWNLOAD_INVALID_API_REFERENCE                 0x12u
#define PN_SERVICE_DOWNLOAD_INVALID_API                           0x13u
#define PN_SERVICE_DOWNLOAD_INVALID_MODULE_REFERENCE              0x14u
#define PN_SERVICE_DOWNLOAD_INVALID_DATA_RECORD_REFERENCE         0x15u
#define PN_SERVICE_DOWNLOAD_INVALID_SLOT_NUMBER                   0x16u
#define PN_SERVICE_DOWNLOAD_INVALID_NUMBER_SUBMODULE              0x17u
#define PN_SERVICE_DOWNLOAD_INVALID_IO_DATA_LENGTH                0x18u
#define PN_SERVICE_DOWNLOAD_INVALID_STATUS_LENGTH                 0x19u
#define PN_SERVICE_DOWNLOAD_INVALID_SEQUENCE                      0x1Au
#define PN_SERVICE_DOWNLOAD_INVALID_INDEX                         0x1Bu
#define PN_SERVICE_DOWNLOAD_INVALID_RECORD_DATA_LENGTH            0x1Cu
#define PN_SERVICE_DOWNLOAD_INVALID_SUBNETMASK                    0x1Du
#define PN_SERVICE_DOWNLOAD_INVALID_IP_ADDRESS                    0x1Eu
#define PN_SERVICE_DOWNLOAD_INVALID_ROUTER_ADDRESS                0x1Fu
#define PN_SERVICE_DOWNLOAD_IP_ADDRESS_AND_ROUTER_MISMATCH        0x20u
#define PN_SERVICE_DOWNLOAD_INVALID_LENGTH                        0x21u
#define PN_SERVICE_DOWNLOAD_INVALID_MCR_FRAME_ID                  0x22u
#define PN_SERVICE_DOWNLOAD_INVALID_MCR_PROPERTIES                0x23u
#define PN_SERVICE_DOWNLOAD_INVALID_MCR_TIMEOUT                   0x24u
#define PN_SERVICE_DOWNLOAD_INVALID_MCR_NAME_LENGTH               0x25u
#define PN_SERVICE_DOWNLOAD_INVALID_MCR_NAME                      0x26u

#define PN_SERVICE_DOWNLOAD_NO_DEVICE_PARA_SET_RESOURCE           0x01u
#define PN_SERVICE_DOWNLOAD_NO_ALARM_RESOURCE                     0x02u
#define PN_SERVICE_DOWNLOAD_NO_IO_DATA_RESOURCE                   0x03u
#define PN_SERVICE_DOWNLOAD_NO_IO_DATA_APPL_RESOURCE              0x04u
#define PN_SERVICE_DOWNLOAD_NO_CYCLIC_SM_OBJECT_RESOURCE          0x05u
#define PN_SERVICE_DOWNLOAD_NO_APB_RESOURCE                       0x06u

#define PN_MAX_INTERFACE_NAME_LENGTH                              64
#define PN_MAX_DEVICE_NAME_LENGTH                                 240
#define PN_MAX_LABEL_LENGTH                                       64

#define PN_SERVICE_DOWNLOAD_SUPERVISOR_NUMBER_IOCRS               0x0000u
#define PN_SERVICE_DOWNLOAD_DEVICE_NUMBER_IOCRS                   0x0000u
#define PN_SERVICE_DOWNLOAD_DEVICE_NUMBER_DATA_RECORDS            0x0000u
#define PN_SERVICE_DOWNLOAD_MIN_NUMBER_IOCRS                      0x0002u
#define PN_SERVICE_DOWNLOAD_MAX_NUMBER_IOCRS                      0x0004u

#define PN_SERVICE_DOWNLOAD_NUMBER_API                            0x0010u

#define PN_SERVICE_DOWNLOAD_MIN_NUMBER_MODULES                    0x0001u

#define PN_SERVICE_DOWNLOAD_FLAG_ACTIVATE                         0x01u
#define PN_SERVICE_DOWNLOAD_FLAG_AUTO_CONNECT_RES                 0x02u
#define PN_SERVICE_DOWNLOAD_FLAG_AUTO_WRITE_RES                   0x04u
#define PN_SERVICE_DOWNLOAD_FLAG_AUTO_READ_RES                    0x08u
#define PN_SERVICE_DOWNLOAD_FLAG_AUTO_CONTROL_RES                 0x10u
#define PN_SERVICE_DOWNLOAD_FLAG_FULL_APPLICATION_IDENT_SUPPORT   0x20u
#define PN_SERVICE_DOWNLOAD_FLAG_DISABLE_DCP_HELLO                0x40u
#define PN_SERVICE_DOWNLOAD_FLAG_CERTIFICATION_ACTIVE             0x80u

#define PN_SERVICE_DOWNLOAD_ADD_FLAG_SKIP_IP_ASSIGNMENT           0x01u

#define PN_SERVICE_DOWNLOAD_VERSION                               2u
#define PN_SERVICE_DOWNLOAD_LITTLE_ENDIAN                         0u
#define PN_SERVICE_DOWNLOAD_BIG_ENDIAN                            1u

typedef struct _T_PN_SERVICE_DOWNLOAD_REQ
{
  PN_U8               IpAddressHighWordHighByte;
  PN_U8               IpAddressHighWordLowByte;
  PN_U8               IpAddressLowWordHighByte;
  PN_U8               IpAddressLowWordLowByte;

  PN_U8               SubnetMaskHighWordHighByte;
  PN_U8               SubnetMaskHighWordLowByte;
  PN_U8               SubnetMaskLowWordHighByte;
  PN_U8               SubnetMaskLowWordLowByte;

  PN_U8               DefaultRouterHighWordHighByte;
  PN_U8               DefaultRouterHighWordLowByte;
  PN_U8               DefaultRouterLowWordHighByte;
  PN_U8               DefaultRouterLowWordLowByte;

  char                InterfaceName [PN_MAX_INTERFACE_NAME_LENGTH];

  char                DeviceName [PN_MAX_DEVICE_NAME_LENGTH];

  PN_U8               Flag;
  PN_U8               AdditionalFlag;

  PN_U8               VersionHighByte;
  PN_U8               VersionLowByte;

  PN_U8               ARPropertiesHighWordHighByte;
  PN_U8               ARPropertiesHighWordLowByte;
  PN_U8               ARPropertiesLowWordHighByte;
  PN_U8               ARPropertiesLowWordLowByte;

  PN_U8               InstanceHighByte;
  PN_U8               InstanceLowByte;

  PN_U8               DeviceIdHighByte;
  PN_U8               DeviceIdLowByte;

  PN_U8               VendorIdHighByte;
  PN_U8               VendorIdLowByte;

  PN_U8               NumberOfIOCRHighByte;
  PN_U8               NumberOfIOCRLowByte;

  PN_U8               NumberOfAPIsHighByte;
  PN_U8               NumberOfAPIsLowByte;

  PN_U8               NumberOfModulesHighByte;
  PN_U8               NumberOfModulesLowByte;

  PN_U8               NumberOfDataRecordsHighByte;
  PN_U8               NumberOfDataRecordsLowByte;

  T_PN_ALARM_CR       AlarmCRBlock;

/*
  T_PN_IOCR           IOCR [NumberOfIOCR];
  T_PN_API            Api [NumberOfAPIs];
  T_PN_MODULE         Module [NumberOfModules];
  T_PN_DATA_RECORD    DataRecords [NumberOfDataRecords];
*/

} PACK_BYTE_ALIGNMENT (T_PN_SERVICE_DOWNLOAD_REQ);

/*---------------------------------------------------------------------------*/

typedef struct _T_PN_DATA_INFO
{
  PN_U8               SlotNumberHighByte;
  PN_U8               SlotNumberLowByte;

  PN_U8               SubSlotNumberHighByte;
  PN_U8               SubSlotNumberLowByte;

  PN_U8               OffsetHighByte;
  PN_U8               OffsetLowByte;

} PACK_BYTE_ALIGNMENT (T_PN_DATA_INFO);

/*---------------------------------------------------------------------------*/

typedef struct _T_PN_API_INFO
{
  PN_U8               APIHighWordHighByte;
  PN_U8               APIHighWordLowByte;
  PN_U8               APILowWordHighByte;
  PN_U8               APILowWordLowByte;

  PN_U8               NumberOfIODataHighByte;
  PN_U8               NumberOfIODataLowByte;

  PN_U8               NumberOfIOStatusHighByte;
  PN_U8               NumberOfIOStatusLowByte;

/*
  T_PN_DATA_INFO      Data [NumberOfIOData];
  T_PN_DATA_INFO      Status [NumberOfIOStatus];
*/

} PACK_BYTE_ALIGNMENT (T_PN_API_INFO);

/*---------------------------------------------------------------------------*/

#define PN_IOCR_IDENTIFIER_DATA_PROVIDER                0x8000u
#define PN_IOCR_CMDEV_MASK                              0x0100u
#define PN_IOCR_DEVICE_REF_MASK                         0x00FFu
#define PN_IOCR_CMDEV_AR_MASK                           0x00FFu
#define PN_IOCR_NUMBER_MASK                             0x7E00u
#define PN_IOCR_IDENTIFIER_MASK                         (PN_IOCR_DEVICE_REF_MASK | \
                                                        PN_IOCR_CMDEV_AR_MASK    | \
                                                        PN_IOCR_NUMBER_MASK      | \
                                                        PN_IOCR_CMDEV_MASK)

#define _PN_IOCR_NUMBER_TO_DEVICE_REF(IOCRIdentifier)   (IOCRIdentifier & PN_IOCR_DEVICE_REF_MASK)
#define _PN_IOCR_CMDEV_AR_TO_INDEX(IOCRIdentifier)      (IOCRIdentifier & PN_IOCR_CMDEV_AR_MASK)
#define _PN_IOCR_NUMBER_TO_INDEX(IOCRIdentifier)        ((IOCRIdentifier & PN_IOCR_NUMBER_MASK) >> 9u)
#define _PN_IOCR_CMDEV(IOCRIdentifier)                  (IOCRIdentifier & PN_IOCR_CMDEV_MASK)

typedef struct _T_PN_IOCR_INFO
{
  PN_U8               IOCRIdentifierHighByte;
  PN_U8               IOCRIdentifierLowByte;

  PN_U8               TypeHighByte;
  PN_U8               TypeLowByte;

  PN_U8               IODataLengthHighByte;
  PN_U8               IODataLengthLowByte;

  PN_U8               NumberOfAPIsHighByte;
  PN_U8               NumberOfAPIsLowByte;

/*
  T_PN_API_INFO       Api [NumberOfAPIs];
*/

} PACK_BYTE_ALIGNMENT (T_PN_IOCR_INFO);

/*---------------------------------------------------------------------------*/

typedef struct _T_PN_SERVICE_DOWNLOAD_CON
{
  PN_U8               AlarmRefHighByte;
  PN_U8               AlarmRefLowByte;

  PN_U8               NumberOfIOCRHighByte;
  PN_U8               NumberOfIOCRLowByte;

/*
  T_PN_IOCR_INFO      IOCR [NumberOfIOCR];
*/

} PACK_BYTE_ALIGNMENT (T_PN_SERVICE_DOWNLOAD_CON);

/*===========================================================================*/

#define PN_SERVICE_SET_IDENTIFICATION_INVALID_TYPE_OF_STATION   0x01u
#define PN_SERVICE_SET_IDENTIFICATION_INVALID_ORDER_ID          0x02u
#define PN_SERVICE_SET_IDENTIFICATION_INVALID_SERIAL_NUMBER     0x03u

#define SERVICE_SET_IDENTIFICATION_TYPE_OF_STATION_LENGTH       50u

#define PN_IDENT_MAINTENANCE_1_SUPPORTED                        (PN_U16) 0x0002u
#define PN_IDENT_MAINTENANCE_2_SUPPORTED                        (PN_U16) 0x0004u
#define PN_IDENT_MAINTENANCE_3_SUPPORTED                        (PN_U16) 0x0008u
#define PN_IDENT_MAINTENANCE_4_SUPPORTED                        (PN_U16) 0x0010u

typedef struct _T_PN_SERVICE_SET_IDENTIFICATION_REQ
{
  PN_U8   TypeOfStation [SERVICE_SET_IDENTIFICATION_TYPE_OF_STATION_LENGTH];
  PN_U8   OrderId [PROFINET_IDENT_MAINTENANCE_ORDER_ID_LENGTH];
  PN_U8   SerialNumber [PROFINET_IDENT_MAINTENANCE_SR_NUMBER_LENGTH];

  PN_U8   HwRevisionHighByte;
  PN_U8   HwRevisionLowByte;

  PN_U8   SwRevisionHighWordHighByte;
  PN_U8   SwRevisionHighWordLowByte;
  PN_U8   SwRevisionLowWordHighByte;
  PN_U8   SwRevisionLowWordLowByte;

  PN_U8   RevisionCounterHighByte;
  PN_U8   RevisionCounterLowByte;

  PN_U8   ProfileIdHighByte;
  PN_U8   ProfileIdLowByte;

  PN_U8   ProfileTypeHighByte;
  PN_U8   ProfileTypeLowByte;

  PN_U8   IdentAndMaintenanceVersionHighByte;
  PN_U8   IdentAndMaintenanceVersionLowByte;

  PN_U8   IdentAndMaintenanceSupportedHighByte;
  PN_U8   IdentAndMaintenanceSupportedLowByte;

} PACK_BYTE_ALIGNMENT (T_PN_SERVICE_SET_IDENTIFICATION_REQ);

/*---------------------------------------------------------------------------*/

typedef struct _T_PN_SERVICE_SET_IDENTIFICATION_CON
{
  PN_U16   DummyForCompiler;

} PACK_BYTE_ALIGNMENT (T_PN_SERVICE_SET_IDENTIFICATION_CON);

/*===========================================================================*/

#define PN_SERVICE_CONFIGURE_WATCHDOG_INVALID_ACTION            0x01u
#define PN_SERVICE_CONFIGURE_WATCHDOG_INVALID_INTERVAL          0x02u
#define PN_SERVICE_CONFIGURE_WATCHDOG_ACTIVATED                 0x03u

#define WATCHDOG_EXPIRATION_ACTION_GO_OFFLINE                   (PN_U8) 1u
#define WATCHDOG_EXPIRATION_ACTION_REBOOT                       (PN_U8) 2u

#define MIN_WATCHDOG_INTERVAL                                   (PN_U32) 3uL
#define MAX_WATCHDOG_INTERVAL                                   (PN_U32) 60000uL

typedef struct _T_PN_SERVICE_CONFIGURE_WATCHDOG_REQ
{
  PN_U8   WatchdogExpirationAction;
  PN_U8   Alignment;

  PN_U8   WatchdogIntervalHighWordHighByte;
  PN_U8   WatchdogIntervalHighWordLowByte;
  PN_U8   WatchdogIntervalLowWordHighByte;
  PN_U8   WatchdogIntervalLowWordLowByte;

} PACK_BYTE_ALIGNMENT (T_PN_SERVICE_CONFIGURE_WATCHDOG_REQ);

/*---------------------------------------------------------------------------*/

typedef struct _T_PN_SERVICE_CONFIGURE_WATCHDOG_CON
{
  PN_U16   DummyForCompiler;

} PACK_BYTE_ALIGNMENT (T_PN_SERVICE_CONFIGURE_WATCHDOG_CON);

/*===========================================================================*/

#define PN_SERVICE_DOWNLOAD_NEIGHBOUR_DATA_DUPLICATE_SUBSLOT        (PN_U8) 0x01u
#define PN_SERVICE_DOWNLOAD_NEIGHBOUR_DATA_INVALID_SLOT_NUMBER      (PN_U8) 0x02u
#define PN_SERVICE_DOWNLOAD_NEIGHBOUR_DATA_INVALID_SUBSLOT_NUMBER   (PN_U8) 0x03u
#define PN_SERVICE_DOWNLOAD_NEIGHBOUR_DATA_INVALID_NAME_LENGTH      (PN_U8) 0x04u
#define PN_SERVICE_DOWNLOAD_NEIGHBOUR_DATA_INVALID_NAME             (PN_U8) 0x05u
#define PN_SERVICE_DOWNLOAD_NEIGHBOUR_DATA_INVALID_PORT_ID_LENGTH   (PN_U8) 0x06u
#define PN_SERVICE_DOWNLOAD_NEIGHBOUR_DATA_INVALID_PORT_ID          (PN_U8) 0x07u
#define PN_SERVICE_DOWNLOAD_NEIGHBOUR_DATA_INVALID_LINK_STATE_LINK  (PN_U8) 0x08u
#define PN_SERVICE_DOWNLOAD_NEIGHBOUR_DATA_INVALID_PORT_STATE       (PN_U8) 0x09u

#define REPLACE_NAME_OF_STATION_BY_NAME_OF_INTERFACE                (PN_U8) 0x01u
#define ENABLE_LINE_DELAY_CHECK                                     (PN_U8) 0x02u
#define ENABLE_MAU_TYPE_CHECK                                       (PN_U8) 0x04u
#define ENABLE_LINK_STATE_CHECK                                     (PN_U8) 0x08u
#define ENABLE_SYNC_MODE_CHECK                                      (PN_U8) 0x10u
#define ENABLE_MAU_TYPE_MODE_CHECK                                  (PN_U8) 0x20u

#define PROFINET_MAX_PORT_ID_STRING_LENGTH                          (PN_U8) 14u
#define PROFINET_MIN_PORT_ID_STRING_LENGTH                          (PN_U8) 8u

#define PROFINET_USE_LINE_DELAY_AS_LINE_DELAY                       (PN_U32) 0x00000000uL
#define PROFINET_USE_LINE_DELAY_AS_CABLE_DELAY                      (PN_U32) 0x80000000uL

#define PROFINET_MAU_TYPE_UNKNOWN                                   (PN_U16) 0u
#define PROFINET_MAU_TYPE_10BASE_TXHD                               (PN_U16) 10u
#define PROFINET_MAU_TYPE_10BASE_TXFD                               (PN_U16) 11u
#define PROFINET_MAU_TYPE_10BASE_FLHD                               (PN_U16) 12u
#define PROFINET_MAU_TYPE_10BASE_FLFD                               (PN_U16) 13u
#define PROFINET_MAU_TYPE_100BASE_TXHD                              (PN_U16) 15u
#define PROFINET_MAU_TYPE_100BASE_TXFD                              (PN_U16) 16u
#define PROFINET_MAU_TYPE_100BASE_FXHD                              (PN_U16) 17u
#define PROFINET_MAU_TYPE_100BASE_FXFD                              (PN_U16) 18u
#define PROFINET_MAU_TYPE_1000BASE_XHD                              (PN_U16) 21u
#define PROFINET_MAU_TYPE_1000BASE_XFD                              (PN_U16) 22u
#define PROFINET_MAU_TYPE_1000BASE_LXHD                             (PN_U16) 23u
#define PROFINET_MAU_TYPE_1000BASE_LXFD                             (PN_U16) 24u
#define PROFINET_MAU_TYPE_1000BASE_SXHD                             (PN_U16) 25u
#define PROFINET_MAU_TYPE_1000BASE_SXFD                             (PN_U16) 26u
#define PROFINET_MAU_TYPE_1000BASE_THD                              (PN_U16) 29u
#define PROFINET_MAU_TYPE_1000BASE_TFD                              (PN_U16) 30u
#define PROFINET_MAU_TYPE_10G_BASE_FX                               (PN_U16) 31u
#define PROFINET_MAU_TYPE_100BASE_LX10                              (PN_U16) 46u
#define PROFINET_MAU_TYPE_100BASE_PXFD                              (PN_U16) 54u

#define PROFINET_LINK_STATE_LINK_UP                                 (PN_U16) 0x0001u
#define PROFINET_LINK_STATE_LINK_DOWN                               (PN_U16) 0x0002u
#define PROFINET_LINK_STATE_LINK_TESTING                            (PN_U16) 0x0003u
#define PROFINET_LINK_STATE_LINK_UNKNOWN                            (PN_U16) 0x0004u
#define PROFINET_LINK_STATE_LINK_DORMANT                            (PN_U16) 0x0005u
#define PROFINET_LINK_STATE_LINK_NOTPRESENT                         (PN_U16) 0x0006u
#define PROFINET_LINK_STATE_LINK_LOWERDOWN                          (PN_U16) 0x0007u
#define PROFINET_LINK_STATE_PORT_STATE_UNKNOWN                      (PN_U16) 0x0000u
#define PROFINET_LINK_STATE_PORT_STATE_DISABLED                     (PN_U16) 0x0100u
#define PROFINET_LINK_STATE_PORT_STATE_BLOCKING                     (PN_U16) 0x0200u
#define PROFINET_LINK_STATE_PORT_STATE_LISTENING                    (PN_U16) 0x0300u
#define PROFINET_LINK_STATE_PORT_STATE_LEARNING                     (PN_U16) 0x0400u
#define PROFINET_LINK_STATE_PORT_STATE_FORWARDING                   (PN_U16) 0x0500u
#define PROFINET_LINK_STATE_PORT_STATE_BROKEN                       (PN_U16) 0x0600u
#define PROFINET_LINK_STATE_PORT_MASK                               (PN_U16) 0xFF00u

#define PROFINET_SYNC_MODE_CABLE_DELAY_CHECK_ON                     (PN_U16) 0x0001u
#define PROFINET_SYNC_MODE_SYNC_MASTER_CHECK_ON                     (PN_U16) 0x0002u
#define PROFINET_SYNC_MODE_MASK                                     (PN_U16) (PROFINET_SYNC_MODE_CABLE_DELAY_CHECK_ON | \
                                                                             PROFINET_SYNC_MODE_SYNC_MASTER_CHECK_ON)

#define PROFINET_MAU_TYPE_MODE_DIFFERENCE_CHECK_ON                  (PN_U16) 0x0001u
#define PROFINET_MAU_TYPE_MODE_MASK                                 (PN_U16) (PROFINET_MAU_TYPE_MODE_DIFFERENCE_CHECK_ON)

typedef struct _T_PN_NEIGHBOUR_PORT_DATA
{
  PN_U8   Flags;
  PN_U8   Alignment;

  PN_U8   SlotNumberHighByte;
  PN_U8   SlotNumberLowByte;

  PN_U8   SubSlotNumberHighByte;
  PN_U8   SubSlotNumberLowByte;

  PN_U8   DeviceNameLength;
  char    DeviceName [PN_MAX_DEVICE_NAME_LENGTH];

  PN_U8   PortIdLength;
  char    PortId [PROFINET_MAX_PORT_ID_STRING_LENGTH];

  PN_U8   LineDelayHighWordHighByte;
  PN_U8   LineDelayHighWordLowByte;
  PN_U8   LineDelayLowWordHighByte;
  PN_U8   LineDelayLowWordLowByte;

  PN_U8   MAUTypeHighByte;
  PN_U8   MAUTypeLowByte;

  PN_U8   LinkStateHighByte;
  PN_U8   LinkStateLowByte;

  PN_U8   SyncModeHighByte;
  PN_U8   SyncModeLowByte;

  PN_U8   MAUTypeModeHighByte;
  PN_U8   MAUTypeModeLowByte;

} PACK_BYTE_ALIGNMENT (T_PN_NEIGHBOUR_PORT_DATA);


/*---------------------------------------------------------------------------*/

typedef struct _T_PN_SERVICE_DOWNLOAD_NEIGHBOUR_DATA_REQ
{
  PN_U8   NumberPorts;
  PN_U8   Alignment;

/*T_PN_NEIGHBOUR_PORT_DATA    NeighbourData [NumberPorts];*/

} PACK_BYTE_ALIGNMENT (T_PN_SERVICE_DOWNLOAD_NEIGHBOUR_DATA_REQ);

/*---------------------------------------------------------------------------*/

typedef struct _T_PN_SERVICE_DOWNLOAD_NEIGHBOUR_DATA_CON
{
  PN_U16   DummyForCompiler;

} PACK_BYTE_ALIGNMENT (T_PN_SERVICE_DOWNLOAD_NEIGHBOUR_DATA_CON);

/*===========================================================================*/

#define PN_SERVICE_CONFIGURE_LICENCE_INVALID_LENGTH           0x01u
#define PN_SERVICE_CONFIGURE_LICENCE_FAILED                   0x02u

#define MAX_LICENCE_DATA_LENGTH                               (PN_U16) 256u

typedef struct _T_PN_SERVICE_CONFIGURE_LICENCE_REQ
{
  PN_U8   DataLengthHighByte;
  PN_U8   DataLengthLowByte;

  PN_U8   Data [MAX_LICENCE_DATA_LENGTH];

} PACK_BYTE_ALIGNMENT (T_PN_SERVICE_CONFIGURE_LICENCE_REQ);

/*---------------------------------------------------------------------------*/

typedef struct _T_PN_SERVICE_CONFIGURE_LICENCE_CON
{
  PN_U16   DummyForCompiler;

} PACK_BYTE_ALIGNMENT (T_PN_SERVICE_CONFIGURE_LICENCE_CON);

/*===========================================================================*/

#define PN_SYNC_DOMAIN_PRIORITY1_PRIMARY_MASTER                 0x01u
#define PN_SYNC_DOMAIN_PRIORITY1_SECONDARY_MASTER               0x02u
#define PN_SYNC_DOMAIN_PRIORITY1_LEVEL0                         0x00u
#define PN_SYNC_DOMAIN_PRIORITY1_LEVEL1                         0x08u
#define PN_SYNC_DOMAIN_PRIORITY1_LEVEL2                         0x10u
#define PN_SYNC_DOMAIN_PRIORITY1_LEVEL3                         0x18u
#define PN_SYNC_DOMAIN_PRIORITY1_LEVEL4                         0x20u
#define PN_SYNC_DOMAIN_PRIORITY1_LEVEL5                         0x28u
#define PN_SYNC_DOMAIN_PRIORITY1_LEVEL6                         0x30u
#define PN_SYNC_DOMAIN_PRIORITY1_LEVEL7                         0x38u
#define PN_SYNC_DOMAIN_PRIORITY1_MASK                           (PN_SYNC_DOMAIN_PRIORITY1_PRIMARY_MASTER  | \
                                                                PN_SYNC_DOMAIN_PRIORITY1_SECONDARY_MASTER | \
                                                                PN_SYNC_DOMAIN_PRIORITY1_LEVEL0           | \
                                                                PN_SYNC_DOMAIN_PRIORITY1_LEVEL1           | \
                                                                PN_SYNC_DOMAIN_PRIORITY1_LEVEL2           | \
                                                                PN_SYNC_DOMAIN_PRIORITY1_LEVEL3           | \
                                                                PN_SYNC_DOMAIN_PRIORITY1_LEVEL4           | \
                                                                PN_SYNC_DOMAIN_PRIORITY1_LEVEL5           | \
                                                                PN_SYNC_DOMAIN_PRIORITY1_LEVEL6           | \
                                                                PN_SYNC_DOMAIN_PRIORITY1_LEVEL7)

#define PN_SYNC_DOMAIN_PRIORITY2_DEFAULT                        0xFFu

#define PN_SYNC_DOMAIN_CLOCK_CLASS_SLAVE_ONLY                   0xFFu

#define PN_SYNC_DOMAIN_CLOCK_ACCURACY_25NS                      0x20u
#define PN_SYNC_DOMAIN_CLOCK_ACCURACY_100NS                     0x21u
#define PN_SYNC_DOMAIN_CLOCK_ACCURACY_250NS                     0x22u
#define PN_SYNC_DOMAIN_CLOCK_ACCURACY_1000NS                    0x23u
#define PN_SYNC_DOMAIN_CLOCK_ACCURACY_2500NS                    0x24u
#define PN_SYNC_DOMAIN_CLOCK_ACCURACY_10US                      0x25u
#define PN_SYNC_DOMAIN_CLOCK_ACCURACY_25US                      0x26u
#define PN_SYNC_DOMAIN_CLOCK_ACCURACY_100US                     0x27u
#define PN_SYNC_DOMAIN_CLOCK_ACCURACY_250US                     0x28u
#define PN_SYNC_DOMAIN_CLOCK_ACCURACY_1MS                       0x29u
#define PN_SYNC_DOMAIN_CLOCK_ACCURACY_UNKNOWN                   0xFEu

#define PN_SYNC_DOMAIN_CLOCK_VARIANCE_DEFAULT                   0x0000u

/*---------------------------------------------------------------------------*/

#define PN_SERVICE_DOWNLOAD_IRT_DOMAIN_INVALID_PRIORITY1        0x01u
#define PN_SERVICE_DOWNLOAD_IRT_DOMAIN_INVALID_PRIORITY2        0x02u
#define PN_SERVICE_DOWNLOAD_IRT_DOMAIN_INVALID_CLOCK_CLASS      0x03u
#define PN_SERVICE_DOWNLOAD_IRT_DOMAIN_INVALID_CLOCK_ACCURACY   0x04u
#define PN_SERVICE_DOWNLOAD_IRT_DOMAIN_INVALID_CLOCK_VARIANCE   0x05u
#define PN_SERVICE_DOWNLOAD_IRT_DOMAIN_INVALID_CLOCK_CYCLE      0x06u
#define PN_SERVICE_DOWNLOAD_IRT_DOMAIN_INVALID_PHASE            0x07u
#define PN_SERVICE_DOWNLOAD_IRT_DOMAIN_INVALID_RED_BEGIN        0x08u
#define PN_SERVICE_DOWNLOAD_IRT_DOMAIN_INVALID_GREEN_BEGIN      0x09u
#define PN_SERVICE_DOWNLOAD_IRT_DOMAIN_INVALID_SF_BEGIN         0x0Au
#define PN_SERVICE_DOWNLOAD_IRT_DOMAIN_INVALID_YELLOW_BEGIN     0x0Bu
#define PN_SERVICE_DOWNLOAD_IRT_DOMAIN_INVALID_FRAME_ID         0x0Cu
#define PN_SERVICE_DOWNLOAD_IRT_DOMAIN_INVALID_PORT             0x0Du
#define PN_SERVICE_DOWNLOAD_IRT_DOMAIN_INVALID_SEND_OFFSET      0x0Eu

typedef struct _T_PN_SERVICE_SYNC_DOMAIN
{
  T_PROFINET_UUID   SubDomainUUID;

  PN_U8             Priority1;
  PN_U8             Priority2;

  PN_U8             ClockClass;
  PN_U8             ClockAccuracy;

  PN_U8             ClockVarianceHighByte;
  PN_U8             ClockVarianceLowByte;

} PACK_BYTE_ALIGNMENT (T_PN_SERVICE_SYNC_DOMAIN);

/*---------------------------------------------------------------------------*/

typedef struct _T_PN_SERVICE_PHASE_ASSIGNMENTS
{
  PN_U8   RxRedPeriodBeginHighWordHighByte;
  PN_U8   RxRedPeriodBeginHighWordLowByte;
  PN_U8   RxRedPeriodBeginLowWordHighByte;
  PN_U8   RxRedPeriodBeginLowWordLowByte;

  PN_U8   RxGreenPeriodBeginHighWordHighByte;
  PN_U8   RxGreenPeriodBeginHighWordLowByte;
  PN_U8   RxGreenPeriodBeginLowWordHighByte;
  PN_U8   RxGreenPeriodBeginLowWordLowByte;

  PN_U8   TxRedPeriodBeginHighWordHighByte;
  PN_U8   TxRedPeriodBeginHighWordLowByte;
  PN_U8   TxRedPeriodBeginLowWordHighByte;
  PN_U8   TxRedPeriodBeginLowWordLowByte;

  PN_U8   TxGreenPeriodBeginHighWordHighByte;
  PN_U8   TxGreenPeriodBeginHighWordLowByte;
  PN_U8   TxGreenPeriodBeginLowWordHighByte;
  PN_U8   TxGreenPeriodBeginLowWordLowByte;

  PN_U8   TxStoreForwardPeriodBeginHighWordHighByte;
  PN_U8   TxStoreForwardPeriodBeginHighWordLowByte;
  PN_U8   TxStoreForwardPeriodBeginLowWordHighByte;
  PN_U8   TxStoreForwardPeriodBeginLowWordLowByte;

  PN_U8   TxYellowPeriodBeginHighWordHighByte;
  PN_U8   TxYellowPeriodBeginHighWordLowByte;
  PN_U8   TxYellowPeriodBeginLowWordHighByte;
  PN_U8   TxYellowPeriodBeginLowWordLowByte;

} PACK_BYTE_ALIGNMENT (T_PN_SERVICE_PHASE_ASSIGNMENTS);

/*---------------------------------------------------------------------------*/

#define MAX_NUMBER_PDIR_BEGIN_END_PHASES              0x00000010uL

typedef struct _T_PN_SERVICE_PHASE_DATA_ADJUST
{
  PN_U8                             ClockCycle;
  PN_U8                             Alignment;

  /* 2 IRT ports possible, otherwise an absolute forwarder is required */
  PN_U8                             Phase [2];
  T_PN_SERVICE_PHASE_ASSIGNMENTS    Assignments [2] [MAX_NUMBER_PDIR_BEGIN_END_PHASES];

} PACK_BYTE_ALIGNMENT (T_PN_SERVICE_PHASE_DATA_ADJUST);

/*---------------------------------------------------------------------------*/

typedef struct _T_PN_SERVICE_PROVIDER_SEND_OFFSETS
{
  PN_U8   FrameIdHighByte;
  PN_U8   FrameIdLowByte;

  PN_U8   DestinationPort;
  PN_U8   Alignment;

  PN_U8   FrameSendOffsetHighWordHighByte;
  PN_U8   FrameSendOffsetHighWordLowByte;
  PN_U8   FrameSendOffsetLowWordHighByte;
  PN_U8   FrameSendOffsetLowWordLowByte;

} PACK_BYTE_ALIGNMENT (T_PN_SERVICE_PROVIDER_SEND_OFFSETS);

/*---------------------------------------------------------------------------*/

typedef struct _T_PN_SERVICE_DOWNLOAD_LOCAL_IRT_DOMAIN
{
  T_PN_SERVICE_SYNC_DOMAIN              SyncDomain;

  T_PN_SERVICE_PHASE_DATA_ADJUST        PhaseAssignments;

  T_PN_SERVICE_PROVIDER_SEND_OFFSETS    Provider [2u * MAX_NUMBER_SUPPORTED_DEVICES];

} PACK_BYTE_ALIGNMENT (T_PN_SERVICE_DOWNLOAD_LOCAL_IRT_DOMAIN);

/*---------------------------------------------------------------------------*/

typedef struct _T_PN_SERVICE_DOWNLOAD_IRT_DOMAIN_REQ
{
  union
  {
    T_PN_SERVICE_DOWNLOAD_LOCAL_IRT_DOMAIN    Local;

  } UseAs;

} PACK_BYTE_ALIGNMENT (T_PN_SERVICE_DOWNLOAD_IRT_DOMAIN_REQ);

/*---------------------------------------------------------------------------*/

typedef struct _T_PN_SERVICE_DOWNLOAD_IRT_DOMAIN_CON
{
  PN_U16   DummyForCompiler;

} PACK_BYTE_ALIGNMENT (T_PN_SERVICE_DOWNLOAD_IRT_DOMAIN_CON);

/*===========================================================================*/

#define PN_SERVICE_ALARM_PRIO_LOW                                       (PN_U8) 0x01u
#define PN_SERVICE_ALARM_PRIO_HIGH                                      (PN_U8) 0x02u
#define PN_SERVICE_ALARM_PRIO_MASK                                      (PN_SERVICE_ALARM_PRIO_LOW | \
                                                                         PN_SERVICE_ALARM_PRIO_HIGH)

#define PROFINET_ALARM_DIAGNOSIS_APPEARS                                (PN_U16) 0x0001u
#define PROFINET_ALARM_PROCESS                                          (PN_U16) 0x0002u
#define PROFINET_ALARM_PULL                                             (PN_U16) 0x0003u
#define PROFINET_ALARM_PLUG                                             (PN_U16) 0x0004u
#define PROFINET_ALARM_STATUS                                           (PN_U16) 0x0005u
#define PROFINET_ALARM_UPDATE                                           (PN_U16) 0x0006u
#define PROFINET_ALARM_REDUNDANCY                                       (PN_U16) 0x0007u
#define PROFINET_ALARM_CONTROLLED_BY_SUPERVISOR                         (PN_U16) 0x0008u
#define PROFINET_ALARM_RELEASED                                         (PN_U16) 0x0009u
#define PROFINET_ALARM_PLUG_WRONG_SUBMODULE                             (PN_U16) 0x000Au
#define PROFINET_ALARM_RETURN_SUBMODULE                                 (PN_U16) 0x000Bu
#define PROFINET_ALARM_DIAGNOSIS_DISAPPEARS                             (PN_U16) 0x000Cu
#define PROFINET_ALARM_MC_MISMATCH                                      (PN_U16) 0x000Du
#define PROFINET_ALARM_PORT_CHANGE_NOTIFY                               (PN_U16) 0x000Eu
#define PROFINET_ALARM_SYNC_CHANGE_NOTIFY                               (PN_U16) 0x000Fu
#define PROFINET_ALARM_ISO_PROBLEM                                      (PN_U16) 0x0010u
#define PROFINET_ALARM_NETWORK_COMPONENT_PROBLEM                        (PN_U16) 0x0011u
#define PROFINET_ALARM_TIME_DATA_CHANGED                                (PN_U16) 0x0012u
#define PROFINET_ALARM_DFP_PROBLEM                                      (PN_U16) 0x0013u
#define PROFINET_ALARM_MRPD_PROBLEM                                     (PN_U16) 0x0014u
#define PROFINET_ALARM_SYSTEM_REDUNDANCY_NOTIFICATION                   (PN_U16) 0x0015u
#define PROFINET_ALARM_MULTIPLE_INTERFACE_MISMATCH                      (PN_U16) 0x0016u
#define PROFINET_ALARM_UPLOAD_RETRIEVAL_NOTIFICATION                    (PN_U16) 0x001Eu
#define PROFINET_ALARM_PULL_MODULE                                      (PN_U16) 0x001Fu
#define PROFINET_ALARM_MANUFACTURER_LOWER_LIMIT                         (PN_U16) 0x0020u
#define PROFINET_ALARM_MANUFACTURER_UPPER_LIMIT                         (PN_U16) 0x007Fu
#define PROFINET_ALARM_PROFILE_LOWER_LIMIT                              (PN_U16) 0x0080u
#define PROFINET_ALARM_PROFILE_UPPER_LIMIT                              (PN_U16) 0x00FFu

#define PROFINET_ALARM_SPECIFIER_SEQUENCE_MASK                          (PN_U16) 0x07FFu
#define PROFINET_ALARM_SPECIFIER_CHANNEL_DIAGNOSIS                      (PN_U16) 0x0800u
#define PROFINET_ALARM_SPECIFIER_MANUFACTURER_DIAGNOSIS                 (PN_U16) 0x1000u
#define PROFINET_ALARM_SPECIFIER_SUBMODULE_DIAGNOSIS_STATE              (PN_U16) 0x2000u
#define PROFINET_ALARM_SPECIFIER_AR_DIAGNOSIS_STATE                     (PN_U16) 0x8000u
#define PROFINET_ALARM_SPECIFIER_MASK                                   (PROFINET_ALARM_SPECIFIER_SEQUENCE_MASK            | \
                                                                        PROFINET_ALARM_SPECIFIER_CHANNEL_DIAGNOSIS         | \
                                                                        PROFINET_ALARM_SPECIFIER_MANUFACTURER_DIAGNOSIS    | \
                                                                        PROFINET_ALARM_SPECIFIER_SUBMODULE_DIAGNOSIS_STATE | \
                                                                        PROFINET_ALARM_SPECIFIER_AR_DIAGNOSIS_STATE)

#define PROFINET_ALARM_USER_STRUCTURE_IDENT_MANUFACTURER_LOWER_LIMIT    (PN_U16) 0x0000u
#define PROFINET_ALARM_USER_STRUCTURE_IDENT_MANUFACTURER_UPPER_LIMIT    (PN_U16) 0x7FFFu
#define PROFINET_ALARM_USER_STRUCTURE_IDENT_CHANNEL_DIAG                (PN_U16) 0x8000u
#define PROFINET_ALARM_USER_STRUCTURE_IDENT_MULTIPLE                    (PN_U16) 0x8001u
#define PROFINET_ALARM_USER_STRUCTURE_IDENT_EXT_CHANNEL_DIAG            (PN_U16) 0x8002u
#define PROFINET_ALARM_USER_STRUCTURE_IDENT_QUAL_CHANNEL_DIAG           (PN_U16) 0x8003u
#define PROFINET_ALARM_USER_STRUCTURE_IDENT_MAINTENANCE                 (PN_U16) 0x8100u
#define PROFINET_ALARM_USER_STRUCTURE_IDENT_UPLOAD_RETRIEVAL            (PN_U16) 0x8200u
#define PROFINET_ALARM_USER_STRUCTURE_IDENT_IPARAMETER                  (PN_U16) 0x8201u
#define PROFINET_ALARM_USER_STRUCTURE_IDENT_SOE                         (PN_U16) 0x8300u
#define PROFINET_ALARM_USER_STRUCTURE_IDENT_PROFILE_LOWER_LIMIT         (PN_U16) 0x9000u
#define PROFINET_ALARM_USER_STRUCTURE_IDENT_PROFILE_UPPER_LIMIT         (PN_U16) 0x9FFFu

#define PROFINET_ALARM_DIAG_MAINTENANCE_REQUIRED                        (PN_U32) 0x00000001u
#define PROFINET_ALARM_DIAG_MAINTENANCE_DEMANDED                        (PN_U32) 0x00000002u

#define PROFINET_ALARM_CHANNEL_DIAG_CHANNEL_NUMBER_SUBSLOT              (PN_U16) 0x8000u

#define PROFINET_ALARM_CHANNEL_PROPERTIES_ACCUMULATIVE                  (PN_U16) 0x0100u
#define PROFINET_ALARM_CHANNEL_PROPERTIES_MAINTENANCE_DIAGNOSIS         (PN_U16) 0x0000u
#define PROFINET_ALARM_CHANNEL_PROPERTIES_MAINTENANCE_REQUIRED          (PN_U16) 0x0200u
#define PROFINET_ALARM_CHANNEL_PROPERTIES_MAINTENANCE_DEMANDED          (PN_U16) 0x0400u
#define PROFINET_ALARM_CHANNEL_PROPERTIES_MAINTENANCE_QUALIFIED         (PN_U16) 0x0600u
#define PROFINET_ALARM_CHANNEL_PROPERTIES_DIAG_ALL_DISAPPEAR            (PN_U16) 0x0000u
#define PROFINET_ALARM_CHANNEL_PROPERTIES_DIAG_APPEAR                   (PN_U16) 0x0800u
#define PROFINET_ALARM_CHANNEL_PROPERTIES_DIAG_DISAPPEAR                (PN_U16) 0x1000u
#define PROFINET_ALARM_CHANNEL_PROPERTIES_DIAG_DISAPPEAR_REMAIN         (PN_U16) 0x1800u
#define PROFINET_ALARM_CHANNEL_PROPERTIES_DIRECTION_MANUFACTURER        (PN_U16) 0x0000u
#define PROFINET_ALARM_CHANNEL_PROPERTIES_DIRECTION_IN                  (PN_U16) 0x2000u
#define PROFINET_ALARM_CHANNEL_PROPERTIES_DIRECTION_OUT                 (PN_U16) 0x4000u
#define PROFINET_ALARM_CHANNEL_PROPERTIES_DIRECTION_IN_OUT              (PROFINET_ALARM_CHANNEL_PROPERTIES_DIRECTION_IN | \
                                                                        PROFINET_ALARM_CHANNEL_PROPERTIES_DIRECTION_OUT)

#define PROFINET_CH_ERR_TYPE_SHORT_CIRCUIT                              (PN_U16) 0x0001u
#define PROFINET_CH_ERR_TYPE_UNDERVOLTAGE                               (PN_U16) 0x0002u
#define PROFINET_CH_ERR_TYPE_OVERVOLTAGE                                (PN_U16) 0x0003u
#define PROFINET_CH_ERR_TYPE_OVERLOAD                                   (PN_U16) 0x0004u
#define PROFINET_CH_ERR_TYPE_OVERTEMPERATURE                            (PN_U16) 0x0005u
#define PROFINET_CH_ERR_TYPE_LINE_BREAK                                 (PN_U16) 0x0006u
#define PROFINET_CH_ERR_TYPE_UPPER_LIMIT_EXCEEDED                       (PN_U16) 0x0007u
#define PROFINET_CH_ERR_TYPE_LOWER_LIMIT_EXCEEDED                       (PN_U16) 0x0008u
#define PROFINET_CH_ERR_TYPE_GENERIC                                    (PN_U16) 0x0009u
#define PROFINET_CH_ERR_TYPE_SIMULATION_ACTIVE                          (PN_U16) 0x000Au
#define PROFINET_CH_ERR_TYPE_PRM_MISSING                                (PN_U16) 0x000Fu
#define PROFINET_CH_ERR_TYPE_PRM_FAULT                                  (PN_U16) 0x0010u
#define PROFINET_CH_ERR_TYPE_POWER_SUPPLY_FAULT                         (PN_U16) 0x0011u
#define PROFINET_CH_ERR_TYPE_FUSE_BLOWN                                 (PN_U16) 0x0012u
#define PROFINET_CH_ERR_TYPE_COMMUNICATION_FAULT                        (PN_U16) 0x0013u
#define PROFINET_CH_ERR_TYPE_GROUND_FAULT                               (PN_U16) 0x0014u
#define PROFINET_CH_ERR_TYPE_REFERENCE_POINT_LOST                       (PN_U16) 0x0015u
#define PROFINET_CH_ERR_TYPE_SAMPLING_ERROR                             (PN_U16) 0x0016u
#define PROFINET_CH_ERR_TYPE_THRESHOLD_WARNING                          (PN_U16) 0x0017u
#define PROFINET_CH_ERR_TYPE_OUTPUT_DISABLED                            (PN_U16) 0x0018u
#define PROFINET_CH_ERR_TYPE_SAFETY_EVENT                               (PN_U16) 0x0019u
#define PROFINET_CH_ERR_TYPE_EXTERNAL_FAULT                             (PN_U16) 0x001Au
#define PROFINET_CH_ERR_TYPE_MANUFACTURER1_LOWER_LIMIT                  (PN_U16) 0x001Bu
#define PROFINET_CH_ERR_TYPE_MANUFACTURER1_UPPER_LIMIT                  (PN_U16) 0x001Eu
#define PROFINET_CH_ERR_TYPE_TEMPORARY_FAULT                            (PN_U16) 0x001Fu
#define PROFINET_CH_ERR_TYPE_COMMON_PROFILE_LOWER_LIMIT                 (PN_U16) 0x0020u
#define PROFINET_CH_ERR_TYPE_COMMON_PROFILE_UPPER_LIMIT                 (PN_U16) 0x00FFu
#define PROFINET_CH_ERR_TYPE_MANUFACTURER2_LOWER_LIMIT                  (PN_U16) 0x0100u
#define PROFINET_CH_ERR_TYPE_MANUFACTURER2_UPPER_LIMIT                  (PN_U16) 0x7FFFu
#define PROFINET_CH_ERR_TYPE_DATA_TRANSMISSION_IMPOSSIBLE               (PN_U16) 0x8000u
#define PROFINET_CH_ERR_TYPE_REMOTE_MISMATCH                            (PN_U16) 0x8001u
#define PROFINET_CH_ERR_TYPE_MEDIA_REDUNDANCY_MISMATCH                  (PN_U16) 0x8002u
#define PROFINET_CH_ERR_TYPE_SYNC_MISMATCH                              (PN_U16) 0x8003u
#define PROFINET_CH_ERR_TYPE_ISOCHRONOUS_MODE_MISMATCH                  (PN_U16) 0x8004u
#define PROFINET_CH_ERR_TYPE_MCR_MISMATCH                               (PN_U16) 0x8005u
#define PROFINET_CH_ERR_TYPE_FIBER_OPTIC_MISMATCH                       (PN_U16) 0x8007u
#define PROFINET_CH_ERR_TYPE_NETWORK_COMPONENT_MISMATCH                 (PN_U16) 0x8008u
#define PROFINET_CH_ERR_TYPE_TIME_MISMATCH                              (PN_U16) 0x8009u
#define PROFINET_CH_ERR_TYPE_DFP_MISMATCH                               (PN_U16) 0x800Au
#define PROFINET_CH_ERR_TYPE_MRPD_MISMATCH                              (PN_U16) 0x800Bu
#define PROFINET_CH_ERR_TYPE_SYSTEM_REDUNDANCY_MISMATCH                 (PN_U16) 0x800Cu
#define PROFINET_CH_ERR_TYPE_MULTIPLE_INTERFACE_MISMATCH                (PN_U16) 0x800Du
#define PROFINET_CH_ERR_TYPE_NESTED_DIAGNOSIS_INDICATION                (PN_U16) 0x800Eu
#define PROFINET_CH_ERR_TYPE_PROFILE_LOWER_LIMIT                        (PN_U16) 0x9000u
#define PROFINET_CH_ERR_TYPE_PROFILE_UPPER_LIMIT                        (PN_U16) 0x9FFFu

/* general for all channel error types */
#define PROFINET_EXT_CH_ERR_TYPE_MANUFACTURER_LOWER_LIMIT               (PN_U16) 0x0001u
#define PROFINET_EXT_CH_ERR_TYPE_MANUFACTURER_UPPER_LIMIT               (PN_U16) 0x7FFFu
#define PROFINET_EXT_CH_ERR_TYPE_PROFILE_LOWER_LIMIT                    (PN_U16) 0x9000u
#define PROFINET_EXT_CH_ERR_TYPE_PROFILE_UPPER_LIMIT                    (PN_U16) 0x9FFFu

/* general for channel error type 0 - 0x7FFF */
#define PROFINET_EXT_CH_ERR_TYPE_ACCUMULATIVE_INFO                      (PN_U16) 0x8000u

/* used with channel error type 0x8000 */
#define MAX_NUMBER_LOCAL_MISMATCH_ENTRIES                               3u
#define PROFINET_EXT_CH_ERR_TYPE_LINK_STATE_MISMATCH                    (PN_U16) 0x8000u
#define PROFINET_EXT_CH_ERR_TYPE_MAU_TYPE_MISMATCH                      (PN_U16) 0x8001u
#define PROFINET_EXT_CH_ERR_TYPE_LINE_DELAY_MISMATCH                    (PN_U16) 0x8002u

/* used with channel error type 0x8001 */
#define MAX_NUMBER_PEER_MISMATCH_ENTRIES                                32u
#define PROFINET_EXT_CH_ERR_TYPE_PEER_CHASSIS_ID_MISMATCH               (PN_U16) 0x8000u
#define PROFINET_EXT_CH_ERR_TYPE_PEER_PORT_ID_MISMATCH                  (PN_U16) 0x8001u
#define PROFINET_EXT_CH_ERR_TYPE_PEER_RT_CLASS3_MISMATCH                (PN_U16) 0x8002u
#define PROFINET_EXT_CH_ERR_TYPE_PEER_MAU_TYPE_MISMATCH                 (PN_U16) 0x8003u
#define PROFINET_EXT_CH_ERR_TYPE_PEER_MRP_DOMAIN_MISMATCH               (PN_U16) 0x8004u
#define PROFINET_EXT_CH_ERR_TYPE_NO_PEER_DETECTED                       (PN_U16) 0x8005u
#define PROFINET_EXT_CH_ERR_TYPE_PEER_LINE_DELAY_MISMATCH               (PN_U16) 0x8007u
#define PROFINET_EXT_CH_ERR_TYPE_PEER_PTCP_MISMATCH                     (PN_U16) 0x8008u
#define PROFINET_EXT_CH_ERR_TYPE_PEER_PREAMBLE_LENGTH_MISMATCH          (PN_U16) 0x8009u
#define PROFINET_EXT_CH_ERR_TYPE_PEER_FRAGMENTATION_MISMATCH            (PN_U16) 0x800Au

/* used with channel error type 0x8002 */
#define PROFINET_EXT_CH_ERR_TYPE_MRP_MANAGER_ROLE_FAIL                  (PN_U16) 0x8000u
#define PROFINET_EXT_CH_ERR_TYPE_MRP_RING_OPEN                          (PN_U16) 0x8001u
#define PROFINET_EXT_CH_ERR_TYPE_MRP_MULTIPLE_MANAGER                   (PN_U16) 0x8003u

/* used with channel error type 0x8003, 0x8009 */
#define PROFINET_EXT_CH_ERR_TYPE_NO_SYNC_RECEIVED                       (PN_U16) 0x8000u
#define PROFINET_EXT_CH_ERR_TYPE_JITTER_OUT_OF_BOUND                    (PN_U16) 0x8003u

/* used with channel error type 0x8004 */
#define PROFINET_EXT_CH_ERR_TYPE_OUTPUT_TIME_FAILURE                    (PN_U16) 0x8000u
#define PROFINET_EXT_CH_ERR_TYPE_INPUT_TIME_FAILURE                     (PN_U16) 0x8001u
#define PROFINET_EXT_CH_ERR_TYPE_MASTER_LIFE_SIGN_FAILURE               (PN_U16) 0x8002u

/* used with channel error type 0x8005 */
#define PROFINET_EXT_CH_ERR_TYPE_MCR_CONSUMER_TIMEOUT                   (PN_U16) 0x8000u
#define PROFINET_EXT_CH_ERR_TYPE_MCR_ADDR_RESOLUTION_FAILURE            (PN_U16) 0x8001u

/* used with channel error type 0x8007 */
#define PROFINET_EXT_CH_ERR_TYPE_FO_POWER_BUDGET                        (PN_U16) 0x8000u

/* used with channel error type 0x8008 */
#define PROFINET_EXT_CH_ERR_TYPE_NC_FRAME_DROPPED                       (PN_U16) 0x8000u

/* used with channel error type 0x800A */
#define PROFINET_EXT_CH_ERR_TYPE_DFP_FRAME_LATE_LOWER_LIMIT             (PN_U16) 0x8100u
#define PROFINET_EXT_CH_ERR_TYPE_DFP_FRAME_LATE_UPPER_LIMIT             (PN_U16) 0x8FFFu

/* used with channel error type 0x800B */
#define PROFINET_EXT_CH_ERR_TYPE_MRPD_FRAME_DUPLICATE_LOWER_LIMIT       (PN_U16) 0x8700u
#define PROFINET_EXT_CH_ERR_TYPE_MRPD_FRAME_DUPLICATE_UPPER_LIMIT       (PN_U16) 0x8FFFu

/* used with channel error type 0x800C */
#define PROFINET_EXT_CH_ERR_TYPE_SYSTEM_REDUNDANCY_EVENT                (PN_U16) 0x8000u

/* used with channel error type 0x800D */
#define PROFINET_EXT_CH_ERR_TYPE_STANDARD_GATEWAY_MISMATCH              (PN_U16) 0x8000u
#define PROFINET_EXT_CH_ERR_TYPE_AMBIGUOUS_NAME                         (PN_U16) 0x8001u
#define PROFINET_EXT_CH_ERR_TYPE_AMBIGUOUS_IP_ADDRESS                   (PN_U16) 0x8002u
#define PROFINET_EXT_CH_ERR_TYPE_MODE_CONFLICT                          (PN_U16) 0x8003u

/* used with channel error type 0x800E */
#define PROFINET_EXT_CH_ERR_TYPE_SUBORDINATED_ENTITY_INFO               (PN_U16) 0x8000u

typedef struct _T_PN_SERVICE_GET_ALARM_REQ
{
  PN_U8   AlarmRefHighByte;
  PN_U8   AlarmRefLowByte;

} PACK_BYTE_ALIGNMENT (T_PN_SERVICE_GET_ALARM_REQ);

/*---------------------------------------------------------------------------*/

typedef struct _T_PN_SERVICE_GET_ALARM_CON
{
  PN_U8     AlarmRefHighByte;
  PN_U8     AlarmRefLowByte;

  PN_U8     AlarmPriority;
  PN_BOOL   RemainingAlarms;

  PN_U8     AlarmTypeHighByte;
  PN_U8     AlarmTypeLowByte;

  PN_U8     APIHighWordHighByte;
  PN_U8     APIHighWordLowByte;
  PN_U8     APILowWordHighByte;
  PN_U8     APILowWordLowByte;

  PN_U8     SlotNumberHighByte;
  PN_U8     SlotNumberLowByte;

  PN_U8     SubSlotNumberHighByte;
  PN_U8     SubSlotNumberLowByte;

  PN_U8     ModuleIdentNumberHighWordHighByte;
  PN_U8     ModuleIdentNumberHighWordLowByte;
  PN_U8     ModuleIdentNumberLowWordHighByte;
  PN_U8     ModuleIdentNumberLowWordLowByte;

  PN_U8     SubmoduleIdentNumberHighWordHighByte;
  PN_U8     SubmoduleIdentNumberHighWordLowByte;
  PN_U8     SubmoduleIdentNumberLowWordHighByte;
  PN_U8     SubmoduleIdentNumberLowWordLowByte;

  PN_U8     SpecifierHighByte;
  PN_U8     SpecifierLowByte;

  PN_U8     LengthHighByte;
  PN_U8     LengthLowByte;

/*
  PN_U8     Data [Length];  manufacturer specific data
*/

} PACK_BYTE_ALIGNMENT (T_PN_SERVICE_GET_ALARM_CON);

/*===========================================================================*/

typedef struct _T_PN_SERVICE_ALARM_ACK_REQ
{
  PN_U8   AlarmRefHighByte;
  PN_U8   AlarmRefLowByte;

  PN_U8   AlarmPriority;
  PN_U8   Alignment;

} PACK_BYTE_ALIGNMENT (T_PN_SERVICE_ALARM_ACK_REQ);

/*---------------------------------------------------------------------------*/

typedef struct _T_PN_SERVICE_ALARM_ACK_CON
{
  PN_U8   AlarmRefHighByte;
  PN_U8   AlarmRefLowByte;

} PACK_BYTE_ALIGNMENT (T_PN_SERVICE_ALARM_ACK_CON);

/*===========================================================================*/

typedef struct _T_PROFINET_BLOCK_HEADER
{
  PN_U8   TypeHighByte;
  PN_U8   TypeLowByte;

  PN_U8   LengthHighByte;
  PN_U8   LengthLowByte;

  PN_U8   VersionHighByte;
  PN_U8   VersionLowByte;

} PACK_BYTE_ALIGNMENT (T_PROFINET_BLOCK_HEADER);

/*---------------------------------------------------------------------------*/

typedef struct _T_PROFINET_MAINTENANCE_ITEM
{
  PN_U8                     UserStructureIdentifierHighByte;
  PN_U8                     UserStructureIdentifierLowByte;

  T_PROFINET_BLOCK_HEADER   Header;

  PN_U8                     Padding [2];

  PN_U8                     StatusHighWordHighByte;
  PN_U8                     StatusHighWordLowByte;
  PN_U8                     StatusLowWordHighByte;
  PN_U8                     StatusLowWordLowByte;

} PACK_BYTE_ALIGNMENT (T_PROFINET_MAINTENANCE_ITEM);

/*---------------------------------------------------------------------------*/

typedef struct _T_PROFINET_UPLOAD_RETRIEVAL_ITEM
{
  PN_U8                     UserStructureIdentifierHighByte;
  PN_U8                     UserStructureIdentifierLowByte;

  T_PROFINET_BLOCK_HEADER   Header;

  PN_U8                     Padding [2];

/*
  PN_U8                     URRecordIndexHighWordHighByte;
  PN_U8                     URRecordIndexHighWordLowByte;
  PN_U8                     URRecordIndexLowWordHighByte;
  PN_U8                     URRecordIndexLowWordLowByte;

  PN_U8                     URRecordLengthHighWordHighByte;
  PN_U8                     URRecordLengthHighWordLowByte;
  PN_U8                     URRecordLengthLowWordHighByte;
  PN_U8                     URRecordLengthLowWordLowByte;
*/

} PACK_BYTE_ALIGNMENT (T_PROFINET_UPLOAD_RETRIEVAL_ITEM);

/*---------------------------------------------------------------------------*/

typedef struct _T_PROFINET_IPARAMETER_ITEM
{
  PN_U8                     UserStructureIdentifierHighByte;
  PN_U8                     UserStructureIdentifierLowByte;

  T_PROFINET_BLOCK_HEADER   Header;

  PN_U8                     Padding [2];

  PN_U8                     RequestHeaderHighWordHighByte;
  PN_U8                     RequestHeaderHighWordLowByte;
  PN_U8                     RequestHeaderLowWordHighByte;
  PN_U8                     RequestHeaderLowWordLowByte;

  PN_U8                     MaxSegmentSizeHighWordHighByte;
  PN_U8                     MaxSegmentSizeHighWordLowByte;
  PN_U8                     MaxSegmentSizeLowWordHighByte;
  PN_U8                     MaxSegmentSizeLowWordLowByte;

  PN_U8                     TransferIndexHighWordHighByte;
  PN_U8                     TransferIndexHighWordLowByte;
  PN_U8                     TransferIndexLowWordHighByte;
  PN_U8                     TransferIndexLowWordLowByte;

  PN_U8                     TotalSizeHighWordHighByte;
  PN_U8                     TotalSizeHighWordLowByte;
  PN_U8                     TotalSizeLowWordHighByte;
  PN_U8                     TotalSizeLowWordLowByte;

} PACK_BYTE_ALIGNMENT (T_PROFINET_IPARAMETER_ITEM);

/*---------------------------------------------------------------------------*/

typedef struct _T_PROFINET_CHANNEL_DIAG
{
  PN_U8   UserStructureIdentifierHighByte;
  PN_U8   UserStructureIdentifierLowByte;

  PN_U8   ChannelNumberHighByte;
  PN_U8   ChannelNumberLowByte;

  PN_U8   PropertiesHighByte;
  PN_U8   PropertiesLowByte;

  PN_U8   ErrorTypeHighByte;
  PN_U8   ErrorTypeLowByte;

} PACK_BYTE_ALIGNMENT (T_PROFINET_CHANNEL_DIAG);

/*---------------------------------------------------------------------------*/

typedef struct _T_PROFINET_EXT_CHANNEL_DIAG
{
  PN_U8   UserStructureIdentifierHighByte;
  PN_U8   UserStructureIdentifierLowByte;

  PN_U8   ChannelNumberHighByte;
  PN_U8   ChannelNumberLowByte;

  PN_U8   PropertiesHighByte;
  PN_U8   PropertiesLowByte;

  PN_U8   ErrorTypeHighByte;
  PN_U8   ErrorTypeLowByte;

  PN_U8   ExtErrorTypeHighByte;
  PN_U8   ExtErrorTypeLowByte;

  PN_U8   ExtAdditionalValueHighWordHighByte;
  PN_U8   ExtAdditionalValueHighWordLowByte;
  PN_U8   ExtAdditionalValueLowWordHighByte;
  PN_U8   ExtAdditionalValueLowWordLowByte;

} PACK_BYTE_ALIGNMENT (T_PROFINET_EXT_CHANNEL_DIAG);

/*---------------------------------------------------------------------------*/

typedef struct _T_PROFINET_QUALIFIED_CHANNEL_DIAG
{
  PN_U8   UserStructureIdentifierHighByte;
  PN_U8   UserStructureIdentifierLowByte;

  PN_U8   ChannelNumberHighByte;
  PN_U8   ChannelNumberLowByte;

  PN_U8   PropertiesHighByte;
  PN_U8   PropertiesLowByte;

  PN_U8   ErrorTypeHighByte;
  PN_U8   ErrorTypeLowByte;

  PN_U8   ExtErrorTypeHighByte;
  PN_U8   ExtErrorTypeLowByte;

  PN_U8   ExtAdditionalValueHighWordHighByte;
  PN_U8   ExtAdditionalValueHighWordLowByte;
  PN_U8   ExtAdditionalValueLowWordHighByte;
  PN_U8   ExtAdditionalValueLowWordLowByte;

  PN_U8   QualifierHighWordHighByte;
  PN_U8   QualifierHighWordLowByte;
  PN_U8   QualifierLowWordHighByte;
  PN_U8   QualifierLowWordLowByte;

} PACK_BYTE_ALIGNMENT (T_PROFINET_QUALIFIED_CHANNEL_DIAG);

/*---------------------------------------------------------------------------*/

#define PN_SERVICE_ALARM_INVALID_ALARM_TYPE               0x01u
#define PN_SERVICE_ALARM_INVALID_API                      0x02u
#define PN_SERVICE_ALARM_INVALID_SLOT_NUMBER              0x03u
#define PN_SERVICE_ALARM_INVALID_SUBSLOT_NUMBER           0x04u
#define PN_SERVICE_ALARM_INVALID_SPECIFIER                0x05u
#define PN_SERVICE_ALARM_INVALID_LENGTH                   0x06u

typedef struct _T_PN_SERVICE_ALARM_REQ
{
  PN_U8   AlarmRefHighByte;
  PN_U8   AlarmRefLowByte;

  PN_U8   AlarmPriority;
  PN_U8   Alignment;

  PN_U8   AlarmTypeHighByte;
  PN_U8   AlarmTypeLowByte;

  PN_U8   APIHighWordHighByte;
  PN_U8   APIHighWordLowByte;
  PN_U8   APILowWordHighByte;
  PN_U8   APILowWordLowByte;

  PN_U8   SlotNumberHighByte;
  PN_U8   SlotNumberLowByte;

  PN_U8   SubSlotNumberHighByte;
  PN_U8   SubSlotNumberLowByte;

  PN_U8   ModuleIdentNumberHighWordHighByte;
  PN_U8   ModuleIdentNumberHighWordLowByte;
  PN_U8   ModuleIdentNumberLowWordHighByte;
  PN_U8   ModuleIdentNumberLowWordLowByte;

  PN_U8   SubmoduleIdentNumberHighWordHighByte;
  PN_U8   SubmoduleIdentNumberHighWordLowByte;
  PN_U8   SubmoduleIdentNumberLowWordHighByte;
  PN_U8   SubmoduleIdentNumberLowWordLowByte;

  PN_U8   SpecifierHighByte;
  PN_U8   SpecifierLowByte;

  PN_U8   LengthHighByte;
  PN_U8   LengthLowByte;

/*
  PN_U8   Data [Length];  manufacturer specific data
*/

} PACK_BYTE_ALIGNMENT (T_PN_SERVICE_ALARM_REQ);

/*---------------------------------------------------------------------------*/

typedef struct _T_PN_SERVICE_ALARM_CON
{
  PN_U8   AlarmRefHighByte;
  PN_U8   AlarmRefLowByte;

} PACK_BYTE_ALIGNMENT (T_PN_SERVICE_ALARM_CON);

/*===========================================================================*/

typedef struct _T_PN_SERVICE_GET_ALARM_ACK_REQ
{
  PN_U8   AlarmRefHighByte;
  PN_U8   AlarmRefLowByte;

} PACK_BYTE_ALIGNMENT (T_PN_SERVICE_GET_ALARM_ACK_REQ);

/*---------------------------------------------------------------------------*/

typedef struct _T_PN_SERVICE_GET_ALARM_ACK_CON
{
  PN_U8     AlarmRefHighByte;
  PN_U8     AlarmRefLowByte;

  PN_U8     AlarmPriority;
  PN_BOOL   RemainingAlarmAcks;

  PN_U8     AlarmTypeHighByte;
  PN_U8     AlarmTypeLowByte;

  PN_U8     APIHighWordHighByte;
  PN_U8     APIHighWordLowByte;
  PN_U8     APILowWordHighByte;
  PN_U8     APILowWordLowByte;

  PN_U8     SlotNumberHighByte;
  PN_U8     SlotNumberLowByte;

  PN_U8     SubSlotNumberHighByte;
  PN_U8     SubSlotNumberLowByte;

  PN_U8     SpecifierHighByte;
  PN_U8     SpecifierLowByte;

  PN_U8     ErrorCode;
  PN_U8     ErrorDecode;

  PN_U8     ErrorCode1;
  PN_U8     ErrorCode2;

} PACK_BYTE_ALIGNMENT (T_PN_SERVICE_GET_ALARM_ACK_CON);

/*===========================================================================*/

typedef struct _T_PN_SERVICE_GET_DEVICE_STATE_REQ
{
  PN_U16   DummyForCompiler;

} PACK_BYTE_ALIGNMENT (T_PN_SERVICE_GET_DEVICE_STATE_REQ);

/*---------------------------------------------------------------------------*/

typedef struct _T_PN_ERROR_CODE
{
  PN_U8   Reason;
  PN_U8   Decode;

  PN_U8   Code1;
  PN_U8   Code2;

} PACK_BYTE_ALIGNMENT (T_PN_ERROR_CODE);

typedef T_PN_ERROR_CODE           T_PROFINET_ERROR_CODE;

/*---------------------------------------------------------------------------*/

#define PROFINET_STATUS_OK                    0uL

typedef struct _T_PROFINET_STATUS
{
  union
  {
    PN_U32                  Status;
    PN_U32                  RpcError;
    PN_U32                  NdrError;
    T_PROFINET_ERROR_CODE   Error;

  } PACK_WORD_ALIGNMENT (UseAs);

} PACK_WORD_ALIGNMENT (T_PROFINET_STATUS);

/*---------------------------------------------------------------------------*/

typedef struct _T_PN_DIFF_MODULE_API
{
  PN_U8                   APIHighWordHighByte;
  PN_U8                   APIHighWordLowByte;
  PN_U8                   APILowWordHighByte;
  PN_U8                   APILowWordLowByte;

  PN_U8                   NumberOfModulesHighByte;
  PN_U8                   NumberOfModulesLowByte;

/*
  T_PN_DIFF_MODULE_SLOT   Module [NumberOfModules];
*/

} PACK_BYTE_ALIGNMENT (T_PN_DIFF_MODULE_API);

/*---------------------------------------------------------------------------*/

#define PROFINET_MODULE_STATE_NO_MODULE                     0x0000u
#define PROFINET_MODULE_STATE_WRONG_MODULE                  0x0001u
#define PROFINET_MODULE_STATE_PROPER_MODULE                 0x0002u
#define PROFINET_MODULE_STATE_SUBSTITUTE_MODULE             0x0003u
#define PROFINET_MODULE_STATE_MASK                          (PROFINET_MODULE_STATE_NO_MODULE    | \
                                                            PROFINET_MODULE_STATE_WRONG_MODULE  | \
                                                            PROFINET_MODULE_STATE_PROPER_MODULE | \
                                                            PROFINET_MODULE_STATE_SUBSTITUTE_MODULE)

typedef struct _T_PN_DIFF_MODULE_SLOT
{
  PN_U8                       SlotNumberHighByte;
  PN_U8                       SlotNumberLowByte;

  PN_U8                       IdentNumberHighWordHighByte;
  PN_U8                       IdentNumberHighWordLowByte;
  PN_U8                       IdentNumberLowWordHighByte;
  PN_U8                       IdentNumberLowWordLowByte;

  PN_U8                       StateHighByte;
  PN_U8                       StateLowByte;

  PN_U8                       NumberOfSubmodulesHighByte;
  PN_U8                       NumberOfSubmodulesLowByte;

/*
  T_PN_DIFF_MODULE_SUBSLOT    Submodule [NumberOfSubmodules];
*/

} PACK_BYTE_ALIGNMENT (T_PN_DIFF_MODULE_SLOT);

/*---------------------------------------------------------------------------*/

#define PROFINET_SUBMODULE_STATE_DETAIL_NO_MODULE                     0x0000u
#define PROFINET_SUBMODULE_STATE_DETAIL_WRONG_MODULE                  0x0001u
#define PROFINET_SUBMODULE_STATE_DETAIL_LOCKED_BY_CONTROLLER          0x0002u
#define PROFINET_SUBMODULE_STATE_DETAIL_APPL_READY_PENDING            0x0004u
#define PROFINET_SUBMODULE_STATE_DETAIL_SUBSTITUTE_MODULE             0x0007u

#define PROFINET_SUBMODULE_STATE_FORMAT_INDICATOR                     0x8000u

#define PROFINET_SUBMODULE_STATE_ADD_INFO_NO                          (PROFINET_SUBMODULE_STATE_FORMAT_INDICATOR | 0x0000u)
#define PROFINET_SUBMODULE_STATE_ADD_INFO_TAKEOVER_NOT_ALLOWED        (PROFINET_SUBMODULE_STATE_FORMAT_INDICATOR | 0x0001u)
#define PROFINET_SUBMODULE_STATE_ADD_INFO_RESERVED                    (PROFINET_SUBMODULE_STATE_FORMAT_INDICATOR | 0x0006u)
#define PROFINET_SUBMODULE_STATE_QUALIFIED_INFO                       (PROFINET_SUBMODULE_STATE_FORMAT_INDICATOR | 0x0008u)
#define PROFINET_SUBMODULE_STATE_MAINTANANCE_REQUIRED                 (PROFINET_SUBMODULE_STATE_FORMAT_INDICATOR | 0x0010u)
#define PROFINET_SUBMODULE_STATE_MAINTANANCE_DEMANDED                 (PROFINET_SUBMODULE_STATE_FORMAT_INDICATOR | 0x0020u)
#define PROFINET_SUBMODULE_STATE_DIAG_INFO_NO                         (PROFINET_SUBMODULE_STATE_FORMAT_INDICATOR | 0x0000u)
#define PROFINET_SUBMODULE_STATE_DIAG_INFO_DIAG_AVAILABLE             (PROFINET_SUBMODULE_STATE_FORMAT_INDICATOR | 0x0040u)
#define PROFINET_SUBMODULE_STATE_AR_INFO_NO                           (PROFINET_SUBMODULE_STATE_FORMAT_INDICATOR | 0x0000u)
#define PROFINET_SUBMODULE_STATE_AR_INFO_APPL_READY_PENDING           (PROFINET_SUBMODULE_STATE_FORMAT_INDICATOR | 0x0080u)
#define PROFINET_SUBMODULE_STATE_AR_INFO_SUPERORDINATED_LOCK          (PROFINET_SUBMODULE_STATE_FORMAT_INDICATOR | 0x0100u)
#define PROFINET_SUBMODULE_STATE_AR_INFO_LOCKED_BY_CONTROLLER         (PROFINET_SUBMODULE_STATE_FORMAT_INDICATOR | 0x0180u)
#define PROFINET_SUBMODULE_STATE_AR_INFO_LOCKED_BY_SUPERVISOR         (PROFINET_SUBMODULE_STATE_FORMAT_INDICATOR | 0x0200u)
#define PROFINET_SUBMODULE_STATE_AR_INFO_MASK                         (PROFINET_SUBMODULE_STATE_FORMAT_INDICATOR | 0x0680u)
#define PROFINET_SUBMODULE_STATE_IDENT_INFO_OK                        (PROFINET_SUBMODULE_STATE_FORMAT_INDICATOR | 0x0000u)
#define PROFINET_SUBMODULE_STATE_IDENT_INFO_SUBSTITUTE                (PROFINET_SUBMODULE_STATE_FORMAT_INDICATOR | 0x0800u)
#define PROFINET_SUBMODULE_STATE_IDENT_INFO_WRONG                     (PROFINET_SUBMODULE_STATE_FORMAT_INDICATOR | 0x1000u)
#define PROFINET_SUBMODULE_STATE_IDENT_INFO_NO_SUBMODULE              (PROFINET_SUBMODULE_STATE_FORMAT_INDICATOR | 0x1800u)
#define PROFINET_SUBMODULE_STATE_IDENT_INFO_RESRERVED                 (PROFINET_SUBMODULE_STATE_FORMAT_INDICATOR | 0x6000u)

typedef struct _T_PN_DIFF_MODULE_SUBSLOT
{
  PN_U8   SubSlotNumberHighByte;
  PN_U8   SubSlotNumberLowByte;

  PN_U8   IdentNumberHighWordHighByte;
  PN_U8   IdentNumberHighWordLowByte;
  PN_U8   IdentNumberLowWordHighByte;
  PN_U8   IdentNumberLowWordLowByte;

  PN_U8   StateHighByte;
  PN_U8   StateLowByte;

} PACK_BYTE_ALIGNMENT (T_PN_DIFF_MODULE_SUBSLOT);

/*---------------------------------------------------------------------------*/

typedef struct _T_PN_DIFF_MODULE
{
  PN_U8                   NumberOfAPIsHighByte;
  PN_U8                   NumberOfAPIsLowByte;

/*
  T_PN_DIFF_MODULE_API    Api [NumberOfAPIs];
*/

} PACK_BYTE_ALIGNMENT (T_PN_DIFF_MODULE);

/*---------------------------------------------------------------------------*/

typedef struct _T_PN_SERVICE_GET_DEVICE_STATE_CON
{
  PN_U8               StateHighByte;
  PN_U8               StateLowByte;

  T_PN_ERROR_CODE     Error;
  T_PN_MAC_ADDRESS    MacAddress;

  PN_U8               NumberOfDiffModulesHighByte;
  PN_U8               NumberOfDiffModulesLowByte;

/*
  T_PN_DIFF_MODULE    DiffModule [NumberOfModules];
*/

} PACK_BYTE_ALIGNMENT (T_PN_SERVICE_GET_DEVICE_STATE_CON);

/*===========================================================================*/

typedef struct _T_PN_SERVICE_GET_LIST_OF_STATION_REQ
{
  PN_U16   DummyForCompiler;

} PACK_BYTE_ALIGNMENT (T_PN_SERVICE_GET_LIST_OF_STATION_REQ);

/*---------------------------------------------------------------------------*/

typedef struct _T_PN_DEVICE_IP_INFO
{
  PN_U8               AddressHighWordHighByte;
  PN_U8               AddressHighWordLowByte;
  PN_U8               AddressLowWordHighByte;
  PN_U8               AddressLowWordLowByte;

  PN_U8               SubnetMaskHighWordHighByte;
  PN_U8               SubnetMaskHighWordLowByte;
  PN_U8               SubnetMaskLowWordHighByte;
  PN_U8               SubnetMaskLowWordLowByte;

  PN_U8               DefaultRouterHighWordHighByte;
  PN_U8               DefaultRouterHighWordLowByte;
  PN_U8               DefaultRouterLowWordHighByte;
  PN_U8               DefaultRouterLowWordLowByte;

} PACK_BYTE_ALIGNMENT (T_PN_DEVICE_IP_INFO);

/*---------------------------------------------------------------------------*/

#define DEVICE_TYPE_IO_SINGLE_DEVICE      0x01u
#define DEVICE_TYPE_IO_CONTROLLER         0x02u
#define DEVICE_TYPE_IO_MULTI_DEVICE       0x04u
#define DEVICE_TYPE_IO_SUPERVISOR         0x08u

typedef struct _T_PN_DEVICE_PROPERTY_INFO
{
  PN_U8               DeviceIdHighByte;
  PN_U8               DeviceIdLowByte;

  PN_U8               VendorIdHighByte;
  PN_U8               VendorIdLowByte;

  PN_U8               DeviceType;
  PN_U8               Alignment;

} PACK_BYTE_ALIGNMENT (T_PN_DEVICE_PROPERTY_INFO);

/*---------------------------------------------------------------------------*/

typedef struct _T_PN_DEVICE_INFO
{
  T_PN_MAC_ADDRESS            MacAddress;
  T_PN_DEVICE_IP_INFO         Ip;
  T_PN_DEVICE_PROPERTY_INFO   Property;

  PN_U8                       DeviceNameLengthHighByte;
  PN_U8                       DeviceNameLengthLowByte;

/*
  char                        DeviceName [DeviceNameLength];
*/

} PACK_BYTE_ALIGNMENT (T_PN_DEVICE_INFO);

/*---------------------------------------------------------------------------*/

typedef struct _T_PN_SERVICE_GET_LIST_OF_STATION_CON
{
  PN_U8               NumberOfDevicesHighByte;
  PN_U8               NumberOfDevicesLowByte;

/*
  T_PN_DEVICE_INFO    Device [NumberOfDevices];
*/

} PACK_BYTE_ALIGNMENT (T_PN_SERVICE_GET_LIST_OF_STATION_CON);

/*===========================================================================*/

#define PN_SERVICE_SET_INVALID_RESPONSE_LENGTH                0x01u
#define PN_SERVICE_SET_INVALID_RESPONSE                       0x02u
#define PN_SERVICE_SET_RESPONSE_TIMEOUT                       0x03u
#define PN_SERVICE_INVALID_MAC_ADDRESS                        0x04u
#define PN_SERVICE_SET_INVALID_SUBNETMASK                     0x05u
#define PN_SERVICE_SET_INVALID_IP_ADDRESS                     0x06u
#define PN_SERVICE_SET_INVALID_ROUTER_ADDRESS                 0x07u
#define PN_SERVICE_SET_IP_ADDRESS_AND_ROUTER_MISMATCH         0x08u
#define PN_SERVICE_SET_INVALID_DEVICE_NAME                    0x09u
#define PN_SERVICE_INVALID_DCP_OPTION                         0x0Au
#define PN_SERVICE_INVALID_DCP_LENGTH                         0x0Bu

#define PN_SERVICE_SET_NAME_INVALID_RESPONSE_LENGTH           PN_SERVICE_SET_INVALID_RESPONSE_LENGTH
#define PN_SERVICE_SET_NAME_INVALID_RESPONSE                  PN_SERVICE_SET_INVALID_RESPONSE
#define PN_SERVICE_SET_NAME_RESPONSE_TIMEOUT                  PN_SERVICE_SET_RESPONSE_TIMEOUT
#define PN_SERVICE_SET_IP_SETTINGS_INVALID_RESPONSE_LENGTH    PN_SERVICE_SET_INVALID_RESPONSE_LENGTH
#define PN_SERVICE_SET_IP_SETTINGS_INVALID_RESPONSE           PN_SERVICE_SET_INVALID_RESPONSE
#define PN_SERVICE_SET_IP_SETTINGS_RESPONSE_TIMEOUT           PN_SERVICE_SET_RESPONSE_TIMEOUT

typedef struct _T_PN_SERVICE_SET_NAME_REQ
{
  T_PN_MAC_ADDRESS    DestMacAddress;

  PN_U8               NameLengthHighByte;
  PN_U8               NameLengthLowByte;

  char                DeviceName [PN_MAX_DEVICE_NAME_LENGTH];

} PACK_BYTE_ALIGNMENT (T_PN_SERVICE_SET_NAME_REQ);

/*---------------------------------------------------------------------------*/

typedef struct _T_PN_SERVICE_SET_NAME_CON
{
  PN_U16   DummyForCompiler;

} PACK_BYTE_ALIGNMENT (T_PN_SERVICE_SET_NAME_CON);

/*===========================================================================*/

typedef struct _T_PN_SERVICE_SET_IP_SETTINGS_REQ
{
  T_PN_MAC_ADDRESS    DestMacAddress;

  PN_U8               AddressHighWordHighByte;
  PN_U8               AddressHighWordLowByte;
  PN_U8               AddressLowWordHighByte;
  PN_U8               AddressLowWordLowByte;

  PN_U8               SubnetMaskHighWordHighByte;
  PN_U8               SubnetMaskHighWordLowByte;
  PN_U8               SubnetMaskLowWordHighByte;
  PN_U8               SubnetMaskLowWordLowByte;

  PN_U8               DefaultRouterHighWordHighByte;
  PN_U8               DefaultRouterHighWordLowByte;
  PN_U8               DefaultRouterLowWordHighByte;
  PN_U8               DefaultRouterLowWordLowByte;

} PACK_BYTE_ALIGNMENT (T_PN_SERVICE_SET_IP_SETTINGS_REQ);

/*---------------------------------------------------------------------------*/

typedef struct _T_PN_SERVICE_SET_IP_SETTINGS_CON
{
  PN_U16   DummyForCompiler;

} PACK_BYTE_ALIGNMENT (T_PN_SERVICE_SET_IP_SETTINGS_CON);

/*===========================================================================*/

#define DCP_OPTION_IP                                     (PN_U8) 1u
#define DCP_OPTION_DEVICE_PROPERTIES                      (PN_U8) 2u
#define DCP_OPTION_DHCP                                   (PN_U8) 3u
#define DCP_OPTION_CONTROL                                (PN_U8) 5u
#define DCP_OPTION_DEVICE_INITIATIVE                      (PN_U8) 6u
#define DCP_OPTION_MANUFACTURER_BASE                      (PN_U8) 128u
#define DCP_OPTION_MANUFACTURER_NUMBER                    (PN_U8) 126u
#define DCP_OPTION_SOFTING_STANDARD_EXTENTION             (PN_U8) 254u
#define DCP_OPTION_ALL                                    (PN_U8) 255u

#define DCP_MAX_NUMBER_OPTIONS                            (PN_U8) 8u

#define DCP_OPTION_IP_SUB_MAC_ADDRESS                     (PN_U8) 1u
#define DCP_OPTION_IP_SUB_IP_ADDRESS                      (PN_U8) 2u

#define DCP_OPTION_DEVICE_PROPERTIES_SUB_MANUFACTURER     (PN_U8) 1u
#define DCP_OPTION_DEVICE_PROPERTIES_SUB_NAMEOFSTATION    (PN_U8) 2u
#define DCP_OPTION_DEVICE_PROPERTIES_SUB_DEVICEID         (PN_U8) 3u
#define DCP_OPTION_DEVICE_PROPERTIES_SUB_DEVICEROLE       (PN_U8) 4u
#define DCP_OPTION_DEVICE_PROPERTIES_SUB_DEVICEOPTIONS    (PN_U8) 5u
#define DCP_OPTION_DEVICE_PROPERTIES_SUB_ALIAS            (PN_U8) 6u
#define DCP_OPTION_DEVICE_PROPERTIES_SUB_INSTANCE         (PN_U8) 7u

#define DCP_OPTION_DHCP_SUB_HOST_NAME                     (PN_U8) 12u
#define DCP_OPTION_DHCP_SUB_VENDOR_INFO                   (PN_U8) 43u
#define DCP_OPTION_DHCP_SUB_SERVER_IDENT                  (PN_U8) 54u
#define DCP_OPTION_DHCP_SUB_PARAMETER_LIST                (PN_U8) 55u
#define DCP_OPTION_DHCP_SUB_CLASS_ID                      (PN_U8) 60u
#define DCP_OPTION_DHCP_SUB_CLIENT_ID                     (PN_U8) 61u
#define DCP_OPTION_DHCP_SUB_DOMAIN_NAME                   (PN_U8) 81u
#define DCP_OPTION_DHCP_SUB_UUID                          (PN_U8) 97u
#define DCP_OPTION_DHCP_SUB_CONTROL                       (PN_U8) 255u

#define DCP_OPTION_CONTROL_SUB_START_TRANSACTION          (PN_U8) 1u
#define DCP_OPTION_CONTROL_SUB_END_TRANSACTION            (PN_U8) 2u
#define DCP_OPTION_CONTROL_SUB_SIGNAL                     (PN_U8) 3u
#define DCP_OPTION_CONTROL_SUB_RESPONSE                   (PN_U8) 4u
#define DCP_OPTION_CONTROL_SUB_FACTORY_RESET              (PN_U8) 5u
#define DCP_OPTION_CONTROL_SUB_RESET_TO_FACTORY_SETTING   (PN_U8) 6u

#define DCP_OPTION_DEVICE_INITIATIVE_SUB_DEVICE           (PN_U8) 1u

#define DCP_OPTION_ALL_SUB_ALL                            (PN_U8) 255u

#define DCP_MAX_NUMBER_SUBOPTIONS                         (PN_U8) 8u

#define DCP_SUBOPTION_SOFTING_PROFILE_INFO                (PN_U8) 1u
#define DCP_SUBOPTION_SOFTING_UPDATE_FW                   (PN_U8) 2u
#define DCP_SUBOPTION_SOFTING_SET_ACTIVE_CONFIGURATION    (PN_U8) 3u
#define DCP_SUBOPTION_SOFTING_STORE_CONFIGURATION         (PN_U8) 4u
#define DCP_SUBOPTION_SOFTING_IP_INFO                     (PN_U8) 5u

#define SOFTING_CONFIGURATION_LAST_FRAGMENT               (PN_U32) 0x80000000uL

typedef struct _T_PN_STORE_SOFTING_CONFIGURATION
{
  PN_U8   ConfigLengthHighWordHighByte;
  PN_U8   ConfigLengthHighWordLowByte;
  PN_U8   ConfigLengthLowWordHighByte;
  PN_U8   ConfigLengthLowWordLowByte;

  PN_U8   FragmentNumberHighWordHighByte;
  PN_U8   FragmentNumberHighWordLowByte;
  PN_U8   FragmentNumberLowWordHighByte;
  PN_U8   FragmentNumberLowWordLowByte;

  PN_U8   FragmentLengthHighByte;
  PN_U8   FragmentLengthLowByte;
  PN_U8   ChecksumHighByte;
  PN_U8   ChecksumLowByte;

/*
  PN_U8   ConfigDataFragment [FragmentLength];
*/
} PACK_BYTE_ALIGNMENT (T_PN_STORE_SOFTING_CONFIGURATION);

#define DCP_OPTION_SOFTING_PROFILE_INFO_ADD               (PN_U8) 1u
#define DCP_OPTION_SOFTING_PROFILE_INFO_DELETE            (PN_U8) 2u

#define PROFILE_INFO_TASK_PROFILING                       (PN_U8) 255u

typedef struct _T_PN_PROFILE_INFO
{
  PN_U8   Action;
  PN_U8   ModuleId;

} PACK_BYTE_ALIGNMENT (T_PN_PROFILE_INFO);

/*---------------------------------------------------------------------------*/

#define PN_SERVICE_DCP_SET_RESPONSE_TIMEOUT       PN_SERVICE_SET_RESPONSE_TIMEOUT

typedef struct _T_PN_SERVICE_DCP_SET_REQ
{
  T_PN_MAC_ADDRESS    DestMacAddress;

  PN_U8               Option;
  PN_U8               Suboption;

  PN_U8               LengthHighByte;
  PN_U8               LengthLowByte;

/*
  char                Data [Length];
*/
} PACK_BYTE_ALIGNMENT (T_PN_SERVICE_DCP_SET_REQ);

/*---------------------------------------------------------------------------*/

#define DCP_RESULT_OK                                     (PN_U8) 0u
#define DCP_RESULT_OPTION_NOT_SUPPORTED                   (PN_U8) 1u
#define DCP_RESULT_SUBOPTION_NOT_SUPPORTED                (PN_U8) 2u
#define DCP_RESULT_SUBOPTION_NOT_SET                      (PN_U8) 3u
#define DCP_RESULT_RESOURCE_ERROR                         (PN_U8) 4u
#define DCP_RESULT_SET_NOT_POSSIBLE                       (PN_U8) 5u

typedef struct _T_PN_SERVICE_DCP_SET_CON
{
  PN_U16   DummyForCompiler;

} PACK_BYTE_ALIGNMENT (T_PN_SERVICE_DCP_SET_CON);

/*===========================================================================*/

#define PN_SERVICE_DCP_GET_RESPONSE_TIMEOUT       PN_SERVICE_SET_RESPONSE_TIMEOUT

typedef struct _T_PN_SERVICE_DCP_GET_REQ
{
  T_PN_MAC_ADDRESS    DestMacAddress;

  PN_U8               Option;
  PN_U8               Suboption;

} PACK_BYTE_ALIGNMENT (T_PN_SERVICE_DCP_GET_REQ);

/*---------------------------------------------------------------------------*/

typedef struct _T_PN_SYS_TIME
{
  PN_U32    HighPart;
  PN_U32    LowPart;

} PACK_BYTE_ALIGNMENT (T_PN_SYS_TIME);

typedef struct _T_PN_PROFILE_ENTRY
{
  PN_U32          Identifier;
  T_PN_SYS_TIME   Time;

} PACK_BYTE_ALIGNMENT (T_PN_PROFILE_ENTRY);

/*---------------------------------------------------------------------------*/

#define PROFILE_DATA_ENCODING_BIG_ENDIAN              0x00u
#define PROFILE_DATA_ENCODING_LITTLE_ENDIAN           0x01u

typedef struct _T_PN_PROFILE_INFO_HEADER
{
  PN_U32                Version;

  PN_U32                CountsPer1ms;

  PN_U16                NumberEntries;
  PN_U8                 DataRepresentation;
  PN_U8                 Alignment;

/*
  T_PN_PROFILE_ENTRY    Data [NumberEntries];
*/
} PACK_BYTE_ALIGNMENT (T_PN_PROFILE_INFO_HEADER);

/*---------------------------------------------------------------------------*/

typedef struct _T_PROFINET_DCP_BLOCK_INFO
{
  PN_U8   BlockInfoHighByte;
  PN_U8   BlockInfoLowByte;

} PACK_BYTE_ALIGNMENT (T_PROFINET_DCP_BLOCK_INFO);

/*---------------------------------------------------------------------------*/

#define STORE_CONFIGURATION_RESULT_OK                  0x00u
#define STORE_CONFIGURATION_RESULT_BUSY                0xFFu

typedef struct _T_PN_STORE_SOFTING_CONFIGURATION_RESULT
{
  PN_U8   Result;
  PN_U8   Alignment;

} PACK_BYTE_ALIGNMENT (T_PN_STORE_SOFTING_CONFIGURATION_RESULT);

/*---------------------------------------------------------------------------*/

typedef struct _T_PN_SERVICE_DCP_GET_CON
{
  PN_U8               Option;
  PN_U8               Suboption;

  PN_U8               LengthHighByte;
  PN_U8               LengthLowByte;

/*
  char                Data [Length];
*/

} PACK_BYTE_ALIGNMENT (T_PN_SERVICE_DCP_GET_CON);

/*===========================================================================*/

#define APPLICATION_DATA_OPTION_IP                                      (PN_U8) 1u
#define APPLICATION_DATA_OPTION_DEVICE_PROPERTIES                       (PN_U8) 2u
#define APPLICATION_DATA_OPTION_CONTROL                                 (PN_U8) 5u
#define APPLICATION_DATA_OPTION_MANUFACTURER_BASE                       (PN_U8) 128u
#define APPLICATION_DATA_OPTION_MANUFACTURER_SIZE                       (PN_U8) 126u

#define APPLICATION_DATA_OPTION_IP_SUB_IP_ADDRESS                       (PN_U8) 2u

#define APPLICATION_DATA_OPTION_DEVICE_PROPERTIES_SUB_MANUFACTURER      (PN_U8) 1u
#define APPLICATION_DATA_OPTION_DEVICE_PROPERTIES_SUB_NAMEOFSTATION     (PN_U8) 2u

#define APPLICATION_DATA_OPTION_CONTROL_SUB_START                       (PN_U8) 1u
#define APPLICATION_DATA_OPTION_CONTROL_SUB_STOP                        (PN_U8) 2u
#define APPLICATION_DATA_OPTION_CONTROL_SUB_SIGNAL                      (PN_U8) 3u
#define APPLICATION_DATA_OPTION_CONTROL_SUB_FACTORY_RESET               (PN_U8) 5u
#define APPLICATION_DATA_OPTION_CONTROL_SUB_RESET_TO_FACTORY_SETTING    (PN_U8) 6u

#define APPLICATION_DATA_OPTION_RESULT_OK                               (PN_U8) 0u
#define APPLICATION_DATA_OPTION_RESULT_OPTION_NOT_SUPPORTED             (PN_U8) 1u
#define APPLICATION_DATA_OPTION_RESULT_SUBOPTION_NOT_SUPPORTED          (PN_U8) 2u
#define APPLICATION_DATA_OPTION_RESULT_SUBOPTION_NOT_SET                (PN_U8) 3u
#define APPLICATION_DATA_OPTION_RESULT_RESOURCE_ERROR                   (PN_U8) 4u
#define APPLICATION_DATA_OPTION_RESULT_SET_NOT_POSSIBLE                 (PN_U8) 5u

typedef struct _T_PN_APPLICATION_OPTION
{
  PN_U8   Option;
  PN_U8   Suboption;

} PACK_BYTE_ALIGNMENT (T_PN_APPLICATION_OPTION);

/*---------------------------------------------------------------------------*/

typedef struct _T_PN_APPLICATION_OPTION_DATA
{
  PN_U8   Option;
  PN_U8   Suboption;

  PN_U8   LengthHighByte;
  PN_U8   LengthLowByte;

/*
  PN_U8   Data [Length];
*/
} PACK_BYTE_ALIGNMENT (T_PN_APPLICATION_OPTION_DATA);

/*---------------------------------------------------------------------------*/

typedef struct _T_PN_APPLICATION_OPTION_RESULT
{
  PN_U8   Option;
  PN_U8   Suboption;

  PN_U8   Result;

} PACK_BYTE_ALIGNMENT (T_PN_APPLICATION_OPTION_RESULT);

/*---------------------------------------------------------------------------*/

typedef T_PN_APPLICATION_OPTION     T_PN_APPLICATION_OPTION_GET;

typedef struct _T_PN_SERVICE_APPLICATION_DATA_GET_IND
{
  PN_U8                         NumberOptionsHighByte;
  PN_U8                         NumberOptionsLowByte;

/*
  T_PN_APPLICATION_OPTION_GET   Data [NumberOptions];
*/
} PACK_BYTE_ALIGNMENT (T_PN_SERVICE_APPLICATION_DATA_GET_IND);

/*---------------------------------------------------------------------------*/

typedef T_PN_APPLICATION_OPTION_DATA    T_PN_APPLICATION_OPTION_DATA_GET;

typedef struct _T_PN_SERVICE_APPLICATION_DATA_GET_RES
{
  PN_U8                               NumberOptionsHighByte;
  PN_U8                               NumberOptionsLowByte;

/*
  T_PN_APPLICATION_OPTION_DATA_GET    Data [NumberOptions];
*/
} PACK_BYTE_ALIGNMENT (T_PN_SERVICE_APPLICATION_DATA_GET_RES);

/*===========================================================================*/

typedef T_PN_APPLICATION_OPTION_DATA    T_PN_APPLICATION_OPTION_DATA_SET;

/*---------------------------------------------------------------------------*/

typedef struct _T_PN_SERVICE_APPLICATION_DATA_SET_IND
{
  PN_U8                               NumberOptionsHighByte;
  PN_U8                               NumberOptionsLowByte;

/*
  T_PN_APPLICATION_OPTION_DATA_SET    Data [NumberOptions];
*/
} PACK_BYTE_ALIGNMENT (T_PN_SERVICE_APPLICATION_DATA_SET_IND);

/*---------------------------------------------------------------------------*/

typedef T_PN_APPLICATION_OPTION_RESULT    T_PN_APPLICATION_OPTION_SET;

typedef struct _T_PN_SERVICE_APPLICATION_DATA_SET_RES
{
  PN_U8                         NumberOptionsHighByte;
  PN_U8                         NumberOptionsLowByte;

/*
  T_PN_APPLICATION_OPTION_SET   Data [NumberOptions];
*/
} PACK_BYTE_ALIGNMENT (T_PN_SERVICE_APPLICATION_DATA_SET_RES);

/*===========================================================================*/

typedef struct _T_PN_SERVICE_GET_ETHERNET_DATA_REQ
{
  PN_U16   DummyForCompiler;

} PACK_BYTE_ALIGNMENT (T_PN_SERVICE_GET_ETHERNET_DATA_REQ);

/*---------------------------------------------------------------------------*/

#define PN_PORT_STATE_LINK_DOWN                 (PN_U8) 0u
#define PN_PORT_STATE_LINK_UP                   (PN_U8) 1u

#define PROFINET_AUTONEGOTIATION_SUPPORTED      (PN_U8) 0x01u
#define PROFINET_AUTONEGOTIATION_ENABLED        (PN_U8) 0x02u

#define PROFINET_CAPABILITIES_UNKNOWN           (PN_U16) 0x0001u
#define PROFINET_CAPABILITIES_10_BASE_T         (PN_U16) 0x0002u
#define PROFINET_CAPABILITIES_10_BASE_TFD       (PN_U16) 0x0004u
#define PROFINET_CAPABILITIES_100_BASE_T4       (PN_U16) 0x0008u
#define PROFINET_CAPABILITIES_100_BASE_TX       (PN_U16) 0x0010u
#define PROFINET_CAPABILITIES_100_BASE_TXFD     (PN_U16) 0x0020u
#define PROFINET_CAPABILITIES_100_BASE_T2       (PN_U16) 0x0040u
#define PROFINET_CAPABILITIES_100_BASE_T2FD     (PN_U16) 0x0080u
#define PROFINET_CAPABILITIES_FDX_PAUSE         (PN_U16) 0x0100u
#define PROFINET_CAPABILITIES_FDX_A_PAUSE       (PN_U16) 0x0200u
#define PROFINET_CAPABILITIES_FDX_S_PAUSE       (PN_U16) 0x0400u
#define PROFINET_CAPABILITIES_FDX_B_PAUSE       (PN_U16) 0x0800u
#define PROFINET_CAPABILITIES_1000_BASE_X       (PN_U16) 0x1000u
#define PROFINET_CAPABILITIES_1000_BASE_XFD     (PN_U16) 0x2000u
#define PROFINET_CAPABILITIES_1000_BASE_T       (PN_U16) 0x4000u
#define PROFINET_CAPABILITIES_1000_BASE_TFD     (PN_U16) 0x8000u

#define PN_LINE_LENGTH_UNKNOWN                  (PN_U16) 0xFFFFu
#define PN_DELAY_UNKNOWN                        (PN_U32) 0uL

typedef struct _T_PN_PORT_DATA
{
  PN_U8   LinkState;
  PN_U8   Autonegotiation;

  PN_U8   CapabilitiesHighByte;
  PN_U8   CapabilitiesLowByte;

  PN_U8   MAUTypeHighByte;
  PN_U8   MAUTypeLowByte;

  PN_U8   LineLengthHighByte;
  PN_U8   LineLengthLowByte;

  PN_U8   RxDelayHighWordHighByte;
  PN_U8   RxDelayHighWordLowByte;
  PN_U8   RxDelayLowWordHighByte;
  PN_U8   RxDelayLowWordLowByte;

  PN_U8   TxDelayHighWordHighByte;
  PN_U8   TxDelayHighWordLowByte;
  PN_U8   TxDelayLowWordHighByte;
  PN_U8   TxDelayLowWordLowByte;

  PN_U8   NumberRxFramesHighWordHighByte;
  PN_U8   NumberRxFramesHighWordLowByte;
  PN_U8   NumberRxFramesLowWordHighByte;
  PN_U8   NumberRxFramesLowWordLowByte;

  PN_U8   NumberTxFramesHighWordHighByte;
  PN_U8   NumberTxFramesHighWordLowByte;
  PN_U8   NumberTxFramesLowWordHighByte;
  PN_U8   NumberTxFramesLowWordLowByte;

  PN_U8   NumberRxBytesHighWordHighByte;
  PN_U8   NumberRxBytesHighWordLowByte;
  PN_U8   NumberRxBytesLowWordHighByte;
  PN_U8   NumberRxBytesLowWordLowByte;

  PN_U8   NumberTxBytesHighWordHighByte;
  PN_U8   NumberTxBytesHighWordLowByte;
  PN_U8   NumberTxBytesLowWordHighByte;
  PN_U8   NumberTxBytesLowWordLowByte;

  PN_U8   NumberRxErrorsHighWordHighByte;
  PN_U8   NumberRxErrorsHighWordLowByte;
  PN_U8   NumberRxErrorsLowWordHighByte;
  PN_U8   NumberRxErrorsLowWordLowByte;

  PN_U8   NumberTxErrorsHighWordHighByte;
  PN_U8   NumberTxErrorsHighWordLowByte;
  PN_U8   NumberTxErrorsLowWordHighByte;
  PN_U8   NumberTxErrorsLowWordLowByte;

} PACK_BYTE_ALIGNMENT (T_PN_PORT_DATA);

/*---------------------------------------------------------------------------*/

typedef struct _T_PN_SERVICE_GET_ETHERNET_DATA_CON
{
  PN_U8             Flags;
  PN_U8             AdditionalFlags;

  char              DeviceName [PN_MAX_DEVICE_NAME_LENGTH];

  PN_U8             IpAddressHighWordHighByte;
  PN_U8             IpAddressHighWordLowByte;
  PN_U8             IpAddressLowWordHighByte;
  PN_U8             IpAddressLowWordLowByte;

  PN_U8             SubnetMaskHighWordHighByte;
  PN_U8             SubnetMaskHighWordLowByte;
  PN_U8             SubnetMaskLowWordHighByte;
  PN_U8             SubnetMaskLowWordLowByte;

  PN_U8             DefaultRouterHighWordHighByte;
  PN_U8             DefaultRouterHighWordLowByte;
  PN_U8             DefaultRouterLowWordHighByte;
  PN_U8             DefaultRouterLowWordLowByte;

  PN_U8             NumberPortsHighByte;
  PN_U8             NumberPortsLowByte;

/*
  T_PN_PORT_DATA    Data [NumberPorts];
*/
} PACK_BYTE_ALIGNMENT (T_PN_SERVICE_GET_ETHERNET_DATA_CON);

/*===========================================================================*/

typedef struct _T_PN_CONNECT_IO_LOCATION
{
  PN_U8   SlotNumberHighByte;
  PN_U8   SlotNumberLowByte;

  PN_U8   SubSlotNumberHighByte;
  PN_U8   SubSlotNumberLowByte;

  PN_U8   OffsetHighByte;
  PN_U8   OffsetLowByte;

} PACK_BYTE_ALIGNMENT (T_PN_CONNECT_IO_LOCATION);

/*---------------------------------------------------------------------------*/

typedef struct _T_PN_CONNECT_IOCR_API
{
  PN_U8                       APIHighWordHighByte;
  PN_U8                       APIHighWordLowByte;
  PN_U8                       APILowWordHighByte;
  PN_U8                       APILowWordLowByte;

  PN_U8                       NumberOfIODataHighByte;
  PN_U8                       NumberOfIODataLowByte;

  PN_U8                       NumberOfIOStatusHighByte;
  PN_U8                       NumberOfIOStatusLowByte;

/*
  T_PN_CONNECT_IO_LOCATION    Data [NumberOfIOData];
  T_PN_CONNECT_IO_LOCATION    Status [NumberOfIOStatus];
*/

} PACK_BYTE_ALIGNMENT (T_PN_CONNECT_IOCR_API);

/*---------------------------------------------------------------------------*/

typedef struct _T_PN_CONNECT_IOCR
{
  PN_U8                   IOCRIdentifierHighByte;
  PN_U8                   IOCRIdentifierLowByte;

  PN_U8                   TypeHighByte;
  PN_U8                   TypeLowByte;

  PN_U8                   CycleTimeHighByte;
  PN_U8                   CycleTimeLowByte;

  PN_U8                   DataHoldFactorHighByte;
  PN_U8                   DataHoldFactorLowByte;

  PN_U8                   IODataLengthHighByte;
  PN_U8                   IODataLengthLowByte;

  PN_U8                   NumberOfAPIsHighByte;
  PN_U8                   NumberOfAPIsLowByte;

/*
  T_PN_CONNECT_IOCR_API   Api [NumberOfAPIs];
*/

} PACK_BYTE_ALIGNMENT (T_PN_CONNECT_IOCR);

/*---------------------------------------------------------------------------*/

typedef struct _T_PN_CONNECT_SUBMODULE
{
  PN_U8   SubSlotNumberHighByte;
  PN_U8   SubSlotNumberLowByte;

  PN_U8   IdentNumberHighWordHighByte;
  PN_U8   IdentNumberHighWordLowByte;
  PN_U8   IdentNumberLowWordHighByte;
  PN_U8   IdentNumberLowWordLowByte;

  PN_U8   ConsumerDataLengthHighByte;
  PN_U8   ConsumerDataLengthLowByte;

  PN_U8   ProviderDataLengthHighByte;
  PN_U8   ProviderDataLengthLowByte;

  PN_U8   PropertiesHighByte;
  PN_U8   PropertiesLowByte;

  PN_U8   ConsumerStatusLength;
  PN_U8   ProviderStatusLength;

} PACK_BYTE_ALIGNMENT (T_PN_CONNECT_SUBMODULE);

/*---------------------------------------------------------------------------*/

typedef struct _T_PN_CONNECT_MODULE_API
{
  PN_U8                     APIHighWordHighByte;
  PN_U8                     APIHighWordLowByte;
  PN_U8                     APILowWordHighByte;
  PN_U8                     APILowWordLowByte;

  PN_U8                     SlotNumberHighByte;
  PN_U8                     SlotNumberLowByte;

  PN_U8                     IdentNumberHighWordHighByte;
  PN_U8                     IdentNumberHighWordLowByte;
  PN_U8                     IdentNumberLowWordHighByte;
  PN_U8                     IdentNumberLowWordLowByte;

  PN_U8                     NumberOfSubmodulesHighByte;
  PN_U8                     NumberOfSubmodulesLowByte;
/*
  T_PN_CONNECT_SUBMODULE    Submodule [NumberOfSubmodules];
*/

} PACK_BYTE_ALIGNMENT (T_PN_CONNECT_MODULE_API);

/*---------------------------------------------------------------------------*/

typedef struct _T_PN_CONNECT_MODULE
{
  PN_U8                     NumberOfAPIsHighByte;
  PN_U8                     NumberOfAPIsLowByte;

/*
  T_PN_CONNECT_MODULE_API   Api [NumberOfAPIs];
*/

} PACK_BYTE_ALIGNMENT (T_PN_CONNECT_MODULE);

/*---------------------------------------------------------------------------*/

typedef struct _T_PN_CONNECT_AR_VENDOR_BLOCK
{
  PN_U8   VersionHighByte;
  PN_U8   VersionLowByte;

  PN_U8   LengthHighByte;
  PN_U8   LengthLowByte;

  PN_U8   APIHighWordHighByte;
  PN_U8   APIHighWordLowByte;
  PN_U8   APILowWordHighByte;
  PN_U8   APILowWordLowByte;

/*
  PN_U8   Data [Length];
*/

} PACK_BYTE_ALIGNMENT (T_PN_CONNECT_AR_VENDOR_BLOCK);

/*---------------------------------------------------------------------------*/

typedef struct _T_PN_CONNECT_REDUNDANCY_BLOCK
{
  PN_U8   VersionHighByte;
  PN_U8   VersionLowByte;

  PN_U8   DHFactorHighByte;
  PN_U8   DHFactorLowByte;

  PN_U8   PropertiesHighWordHighByte;
  PN_U8   PropertiesHighWordLowByte;
  PN_U8   PropertiesLowWordHighByte;
  PN_U8   PropertiesLowWordLowByte;

} PACK_BYTE_ALIGNMENT (T_PN_CONNECT_REDUNDANCY_BLOCK);

/*---------------------------------------------------------------------------*/

#define PROFINET_AR_TYPE_IOCARSINGLE            (PN_U16) 0x0001u
#define PROFINET_AR_TYPE_IOCAR_CONFIG_IN_RUN    (PN_U16) 0x0003u
#define PROFINET_AR_TYPE_IOCAR_RED_CONTROLLER   (PN_U16) 0x0004u
#define PROFINET_AR_TYPE_IOCAR_RED_DEVICE       (PN_U16) 0x0005u
#define PROFINET_AR_TYPE_IOSAR                  (PN_U16) 0x0006u
#define PROFINET_AR_TYPE_IOCARSINGLE_RT3        (PN_U16) 0x0010u

typedef struct _T_PN_SERVICE_CONNECT_IND
{
  PN_U8                           ARIdentifierHighByte;
  PN_U8                           ARIdentifierLowByte;

  PN_U8                           ARTypeHighByte;
  PN_U8                           ARTypeLowByte;

  PN_U8                           ARPropertiesHighWordHighByte;
  PN_U8                           ARPropertiesHighWordLowByte;
  PN_U8                           ARPropertiesLowWordHighByte;
  PN_U8                           ARPropertiesLowWordLowByte;

  PN_U8                           AlarmRefHighByte;
  PN_U8                           AlarmRefLowByte;

  PN_U8                           NumberOfIOCRHighByte;
  PN_U8                           NumberOfIOCRLowByte;

  PN_U8                           NumberOfModuleHighByte;
  PN_U8                           NumberOfModuleLowByte;

  PN_U8                           NumberOfARVendorHighByte;
  PN_U8                           NumberOfARVendorLowByte;

  PN_U8                           NumberOfRedundancyHighByte;
  PN_U8                           NumberOfRedundancyLowByte;

/*
  T_PN_CONNECT_IOCR               IOCR [NumberOfIOCR];
  T_PN_CONNECT_MODULE             Module [NumberOfModule];
  T_PN_CONNECT_AR_VENDOR_BLOCK    ARVendor [NumberOfARVendor];
  T_PN_CONNECT_REDUNDANCY_BLOCK   Redundancy [NumberOfRedundancy];
*/

} PACK_BYTE_ALIGNMENT (T_PN_SERVICE_CONNECT_IND);

/*---------------------------------------------------------------------------*/

#define ERROR_CODE1_OK                                0u
#define ERROR_CODE2_OK                                0u

#define CONNECT_RES_ERROR_CODE1_OK                    ERROR_CODE1_OK
#define CONNECT_RES_ERROR_CODE2_OK                    ERROR_CODE2_OK
#define CONNECT_RES_ERROR_CODE1_AR_OUT_OF_RESOURCE    0x40u
#define CONNECT_RES_ERROR_CODE2_AR_OUT_OF_RESOURCE    0x04u

typedef struct _T_PN_SERVICE_CONNECT_RES
{
  PN_U8                         ARIdentifierHighByte;
  PN_U8                         ARIdentifierLowByte;

  PN_U8                         ErrorCode1;
  PN_U8                         ErrorCode2;

  PN_U8                         NumberOfDiffModulesHighByte;
  PN_U8                         NumberOfDiffModulesLowByte;

  PN_U8                         NumberOfARVendorHighByte;
  PN_U8                         NumberOfARVendorLowByte;

/*
  T_PN_DIFF_MODULE              DiffModule [NumberOfModules];
  T_PN_CONNECT_AR_VENDOR_BLOCK  VendorBlock [NumberOfARVendor];
*/

} PACK_BYTE_ALIGNMENT (T_PN_SERVICE_CONNECT_RES);

/*===========================================================================*/

typedef struct _T_PN_SERVICE_WRITE_IND
{
  PN_U8   ARIdentifierHighByte;
  PN_U8   ARIdentifierLowByte;

  PN_U8   APIHighWordHighByte;
  PN_U8   APIHighWordLowByte;
  PN_U8   APILowWordHighByte;
  PN_U8   APILowWordLowByte;

  PN_U8   SlotNumberHighByte;
  PN_U8   SlotNumberLowByte;

  PN_U8   SubSlotNumberHighByte;
  PN_U8   SubSlotNumberLowByte;

  PN_U8   IndexHighByte;
  PN_U8   IndexLowByte;

  PN_U8   LengthHighByte;
  PN_U8   LengthLowByte;

/*
  PN_U8   Data [Length];
*/

} PACK_BYTE_ALIGNMENT (T_PN_SERVICE_WRITE_IND);

/*---------------------------------------------------------------------------*/

#define PN_SERVICE_WRITE_INVALID_API                    0x01u
#define PN_SERVICE_WRITE_INVALID_SLOT                   0x02u
#define PN_SERVICE_WRITE_INVALID_SUBSLOT                0x03u
#define PN_SERVICE_WRITE_INVALID_INDEX                  0x04u
#define PN_SERVICE_WRITE_INVALID_LENGTH                 0x05u
#define PN_SERVICE_WRITE_RESPONSE_TIMEOUT               0x06u
#define PN_SERVICE_WRITE_INVALID_SEQUENCE_NUMBER        0x07u
#define PN_SERVICE_WRITE_INVALID_AR_UUID                0x08u

typedef struct _T_PN_SERVICE_WRITE_REQ
{
  PN_U8   VersionHighByte;
  PN_U8   VersionLowByte;

  PN_U8   APIHighWordHighByte;
  PN_U8   APIHighWordLowByte;
  PN_U8   APILowWordHighByte;
  PN_U8   APILowWordLowByte;

  PN_U8   SlotNumberHighByte;
  PN_U8   SlotNumberLowByte;

  PN_U8   SubSlotNumberHighByte;
  PN_U8   SubSlotNumberLowByte;

  PN_U8   IndexHighByte;
  PN_U8   IndexLowByte;

  PN_U8   LengthHighByte;
  PN_U8   LengthLowByte;

/*
  PN_U8   Data [Length];
*/

} PACK_BYTE_ALIGNMENT (T_PN_SERVICE_WRITE_REQ);

/*---------------------------------------------------------------------------*/

#define WRITE_RES_ERROR_CODE1_OK                          ERROR_CODE1_OK
#define WRITE_RES_ERROR_CODE2_OK                          ERROR_CODE2_OK

#define WRITE_RES_ERROR_CODE1_INVALID_LENGTH              0x08u
#define WRITE_RES_ERROR_CODE2_INVALID_LENGTH              0x0Bu
#define WRITE_RES_ERROR_CODE1_RESSOURCE_BUSY              0xC2u
#define WRITE_RES_ERROR_CODE2_RESSOURCE_BUSY              0xC2u
#define WRITE_RES_ERROR_CODE1_INVALID_INDEX               0xB0u
#define WRITE_RES_ERROR_CODE2_INVALID_INDEX               10u
#define WRITE_RES_ERROR_CODE1_INVALID_API                 0xB4u
#define WRITE_RES_ERROR_CODE2_INVALID_API                 6u
#define WRITE_RES_ERROR_CODE1_INVALID_SLOT_SUBSLOT        0xB2u
#define WRITE_RES_ERROR_CODE2_INVALID_SLOT_SUBSLOT        7u

typedef struct _T_PN_SERVICE_WRITE_RES
{
  PN_U8   ARIdentifierHighByte;
  PN_U8   ARIdentifierLowByte;

  PN_U8   ErrorCode1;
  PN_U8   ErrorCode2;

} PACK_BYTE_ALIGNMENT (T_PN_SERVICE_WRITE_RES);

/*---------------------------------------------------------------------------*/

typedef struct _T_PN_SERVICE_WRITE_CON
{
  PN_U16   DummyForCompiler;

} PACK_BYTE_ALIGNMENT (T_PN_SERVICE_WRITE_CON);

/*===========================================================================*/

#define PROFINET_INDEX_SUBSLOT_EXPECTED_IDENTIFICATION      (PN_U16) 0x8000u
#define PROFINET_INDEX_SUBSLOT_REAL_IDENTIFICATION          (PN_U16) 0x8001u
#define PROFINET_INDEX_SUBSLOT_DIAGNOSIS_START              (PN_U16) 0x800Au
#define PROFINET_INDEX_SUBSLOT_DIAGNOSIS_END                (PN_U16) 0x800Cu
#define PROFINET_INDEX_SUBSLOT_DIAGNOSIS_CHA_CODING         (PN_U16) 0x800Au
#define PROFINET_INDEX_SUBSLOT_DIAGNOSIS_ALL_CODING         (PN_U16) 0x800Bu
#define PROFINET_INDEX_SUBSLOT_DIAGNOSIS_D_M_Q_S            (PN_U16) 0x800Cu
#define PROFINET_INDEX_SUBSLOT_MAINTENANCE_START            (PN_U16) 0x8010u
#define PROFINET_INDEX_SUBSLOT_MAINTENANCE_END              (PN_U16) 0x8013u
#define PROFINET_INDEX_SUBSLOT_MAINTENANCE_REQ_CHA_CODING   (PN_U16) 0x8010u
#define PROFINET_INDEX_SUBSLOT_MAINTENANCE_DEM_CHA_CODING   (PN_U16) 0x8011u
#define PROFINET_INDEX_SUBSLOT_MAINTENANCE_REQ_ALL_CODING   (PN_U16) 0x8012u
#define PROFINET_INDEX_SUBSLOT_MAINTENANCE_DEM_ALL_CODING   (PN_U16) 0x8013u
#define PROFINET_INDEX_SUBSLOT_SUBSTITUTE                   (PN_U16) 0x801Eu
#define PROFINET_INDEX_SUBSLOT_PDIR_SUBFRAME_DATA           (PN_U16) 0x8020u
#define PROFINET_INDEX_SUBSLOT_INPUT_DATA                   (PN_U16) 0x8028u
#define PROFINET_INDEX_SUBSLOT_OUTPUT_DATA                  (PN_U16) 0x8029u
#define PROFINET_INDEX_SUBSLOT_PD_PORT_DATA_REAL            (PN_U16) 0x802Au
#define PROFINET_INDEX_SUBSLOT_PD_PORT_DATA_CHECK           (PN_U16) 0x802Bu
#define PROFINET_INDEX_SUBSLOT_PDIR_DATA                    (PN_U16) 0x802Cu
#define PROFINET_INDEX_SUBSLOT_EXPECTED_PD_SYNC_DATA        (PN_U16) 0x802Du
#define PROFINET_INDEX_SUBSLOT_PD_PORT_DATA_ADJUST          (PN_U16) 0x802Fu
#define PROFINET_INDEX_SUBSLOT_ISOCHRONOUS_MODE_DATA        (PN_U16) 0x8030u
#define PROFINET_INDEX_SUBSLOT_EXPECTED_PD_TIME_DATA        (PN_U16) 0x8031u
#define PROFINET_INDEX_SUBSLOT_PD_INTERFACE_MRP_DATA_REAL   (PN_U16) 0x8050u
#define PROFINET_INDEX_SUBSLOT_PD_INTERFACE_MRP_DATA_CHECK  (PN_U16) 0x8051u
#define PROFINET_INDEX_SUBSLOT_PD_INTERFACE_MRP_DATA_ADJUST (PN_U16) 0x8052u
#define PROFINET_INDEX_SUBSLOT_PD_PORT_MRP_DATA_ADJUST      (PN_U16) 0x8053u
#define PROFINET_INDEX_SUBSLOT_PD_PORT_MRP_DATA_REAL        (PN_U16) 0x8054u
#define PROFINET_INDEX_SUBSLOT_PD_PORT_FO_DATA_REAL         (PN_U16) 0x8060u
#define PROFINET_INDEX_SUBSLOT_PD_PORT_FO_DATA_CHECK        (PN_U16) 0x8061u
#define PROFINET_INDEX_SUBSLOT_PD_PORT_FO_DATA_ADJUST       (PN_U16) 0x8062u
#define PROFINET_INDEX_SUBSLOT_NC_DATA_CHECK                (PN_U16) 0x8070u
#define PROFINET_INDEX_SUBSLOT_PD_INTERFACE_DATA_ADJUST     (PN_U16) 0x8071u
#define PROFINET_INDEX_SUBSLOT_PD_PORT_STATISTIC            (PN_U16) 0x8072u
#define PROFINET_INDEX_SUBSLOT_PD_INTERFACE_DATA_REAL       (PN_U16) 0x8080u
#define PROFINET_INDEX_SUBSLOT_PD_INTERFACE_FSU_ADJUST      (PN_U16) 0x8090u
#define PROFINET_INDEX_SUBSLOT_ENERGY_START                 (PN_U16) 0x80A0u
#define PROFINET_INDEX_SUBSLOT_ENERGY_END                   (PN_U16) 0x80AFu
#define PROFINET_INDEX_IDENT_AND_MAINTENANCE_0              (PN_U16) 0xAFF0u
#define PROFINET_INDEX_IDENT_AND_MAINTENANCE_1              (PN_U16) 0xAFF1u
#define PROFINET_INDEX_IDENT_AND_MAINTENANCE_2              (PN_U16) 0xAFF2u
#define PROFINET_INDEX_IDENT_AND_MAINTENANCE_3              (PN_U16) 0xAFF3u
#define PROFINET_INDEX_IDENT_AND_MAINTENANCE_4              (PN_U16) 0xAFF4u
#define PROFINET_INDEX_IDENT_AND_MAINTENANCE_5              (PN_U16) 0xAFF5u
#define PROFINET_INDEX_IDENT_AND_MAINTENANCE_6              (PN_U16) 0xAFF6u
#define PROFINET_INDEX_IDENT_AND_MAINTENANCE_7              (PN_U16) 0xAFF7u
#define PROFINET_INDEX_IDENT_AND_MAINTENANCE_8              (PN_U16) 0xAFF8u
#define PROFINET_INDEX_IDENT_AND_MAINTENANCE_9              (PN_U16) 0xAFF9u
#define PROFINET_INDEX_IDENT_AND_MAINTENANCE_10             (PN_U16) 0xAFFAu
#define PROFINET_INDEX_IDENT_AND_MAINTENANCE_11             (PN_U16) 0xAFFBu
#define PROFINET_INDEX_IDENT_AND_MAINTENANCE_12             (PN_U16) 0xAFFCu
#define PROFINET_INDEX_IDENT_AND_MAINTENANCE_13             (PN_U16) 0xAFFDu
#define PROFINET_INDEX_IDENT_AND_MAINTENANCE_14             (PN_U16) 0xAFFEu
#define PROFINET_INDEX_IDENT_AND_MAINTENANCE_15             (PN_U16) 0xAFFFu
#define PROFINET_INDEX_SUBSLOT_PROFILE_RESERVED_START       (PN_U16) 0xB000u
#define PROFINET_INDEX_SUBSLOT_PROFILE_RESERVED_END         (PN_U16) 0xBFFFu

#define PROFINET_INDEX_SLOT_EXPECTED_IDENTIFICATION         (PN_U16) 0xC000u
#define PROFINET_INDEX_SLOT_REAL_IDENTIFICATION             (PN_U16) 0xC001u
#define PROFINET_INDEX_SLOT_DIAGNOSIS_START                 (PN_U16) 0xC00Au
#define PROFINET_INDEX_SLOT_DIAGNOSIS_END                   (PN_U16) 0xC00Cu
#define PROFINET_INDEX_SLOT_DIAGNOSIS_CHA_CODING            (PN_U16) 0xC00Au
#define PROFINET_INDEX_SLOT_DIAGNOSIS_ALL_CODING            (PN_U16) 0xC00Bu
#define PROFINET_INDEX_SLOT_DIAGNOSIS_D_M_Q_S               (PN_U16) 0xC00Cu
#define PROFINET_INDEX_SLOT_MAINTENANCE_START               (PN_U16) 0xC010u
#define PROFINET_INDEX_SLOT_MAINTENANCE_END                 (PN_U16) 0xC013u
#define PROFINET_INDEX_SLOT_MAINTENANCE_REQ_CHA_CODING      (PN_U16) 0xC010u
#define PROFINET_INDEX_SLOT_MAINTENANCE_DEM_CHA_CODING      (PN_U16) 0xC011u
#define PROFINET_INDEX_SLOT_MAINTENANCE_REQ_ALL_CODING      (PN_U16) 0xC012u
#define PROFINET_INDEX_SLOT_MAINTENANCE_DEM_ALL_CODING      (PN_U16) 0xC013u
#define PROFINET_INDEX_SLOT_PROFILE_RESERVED_START          (PN_U16) 0xD000u
#define PROFINET_INDEX_SLOT_PROFILE_RESERVED_END            (PN_U16) 0xDFFFu

#define PROFINET_INDEX_AR_EXPECTED_IDENTIFICATION           (PN_U16) 0xE000u
#define PROFINET_INDEX_AR_REAL_IDENTIFICATION               (PN_U16) 0xE001u
#define PROFINET_INDEX_AR_MODULE_DIFF_BLOCK                 (PN_U16) 0xE002u
#define PROFINET_INDEX_AR_DIAGNOSIS_START                   (PN_U16) 0xE00Au
#define PROFINET_INDEX_AR_DIAGNOSIS_END                     (PN_U16) 0xE00Cu
#define PROFINET_INDEX_AR_DIAGNOSIS_CHA_CODING              (PN_U16) 0xE00Au
#define PROFINET_INDEX_AR_DIAGNOSIS_ALL_CODING              (PN_U16) 0xE00Bu
#define PROFINET_INDEX_AR_DIAGNOSIS_D_M_Q_S                 (PN_U16) 0xE00Cu
#define PROFINET_INDEX_AR_MAINTENANCE_START                 (PN_U16) 0xE010u
#define PROFINET_INDEX_AR_MAINTENANCE_END                   (PN_U16) 0xE013u
#define PROFINET_INDEX_AR_MAINTENANCE_REQ_CHA_CODING        (PN_U16) 0xE010u
#define PROFINET_INDEX_AR_MAINTENANCE_DEM_CHA_CODING        (PN_U16) 0xE011u
#define PROFINET_INDEX_AR_MAINTENANCE_REQ_ALL_CODING        (PN_U16) 0xE012u
#define PROFINET_INDEX_AR_MAINTENANCE_DEM_ALL_CODING        (PN_U16) 0xE013u
#define PROFINET_INDEX_AR_MULTIPLE_WRITE                    (PN_U16) 0xE040u
#define PROFINET_INDEX_AR_FSU_PARAMETER_START               (PN_U16) 0xE050u
#define PROFINET_INDEX_AR_FSU_PARAMETER_END                 (PN_U16) 0xE05Fu
#define PROFINET_INDEX_AR_PROFILE_RESERVED_START            (PN_U16) 0xEC00u
#define PROFINET_INDEX_AR_PROFILE_RESERVED_END              (PN_U16) 0xEFFFu

#define PROFINET_INDEX_API_REAL_IDENTIFICATION              (PN_U16) 0xF000u
#define PROFINET_INDEX_API_DIAGNOSIS_START                  (PN_U16) 0xF00Au
#define PROFINET_INDEX_API_DIAGNOSIS_END                    (PN_U16) 0xF00Cu
#define PROFINET_INDEX_API_DIAGNOSIS_CHA_CODING             (PN_U16) 0xF00Au
#define PROFINET_INDEX_API_DIAGNOSIS_ALL_CODING             (PN_U16) 0xF00Bu
#define PROFINET_INDEX_API_DIAGNOSIS_D_M_Q_S                (PN_U16) 0xF00Cu
#define PROFINET_INDEX_API_MAINTENANCE_START                (PN_U16) 0xF010u
#define PROFINET_INDEX_API_MAINTENANCE_END                  (PN_U16) 0xF013u
#define PROFINET_INDEX_API_MAINTENANCE_REQ_CHA_CODING       (PN_U16) 0xF010u
#define PROFINET_INDEX_API_MAINTENANCE_DEM_CHA_CODING       (PN_U16) 0xF011u
#define PROFINET_INDEX_API_MAINTENANCE_REQ_ALL_CODING       (PN_U16) 0xF012u
#define PROFINET_INDEX_API_MAINTENANCE_DEM_ALL_CODING       (PN_U16) 0xF013u
#define PROFINET_INDEX_API_AR_DATA                          (PN_U16) 0xF020u
#define PROFINET_INDEX_API_PROFILE_RESERVED_START           (PN_U16) 0xF400u
#define PROFINET_INDEX_API_PROFILE_RESERVED_END             (PN_U16) 0xF7FFu

#define PROFINET_INDEX_DEVICE_MAINTENANCE                   (PN_U16) 0xF80Cu
#define PROFINET_INDEX_AR_DATA                              (PN_U16) 0xF820u
#define PROFINET_INDEX_API_DATA                             (PN_U16) 0xF821u
#define PROFINET_INDEX_LOG_DATA                             (PN_U16) 0xF830u
#define PROFINET_INDEX_PDEV_DATA                            (PN_U16) 0xF831u
#define PROFINET_INDEX_IDENT_AND_MAINTENANCE_0_FILTER       (PN_U16) 0xF840u
#define PROFINET_INDEX_PD_REAL_DATA                         (PN_U16) 0xF841u
#define PROFINET_INDEX_PD_EXPECTED_DATA                     (PN_U16) 0xF842u
#define PROFINET_INDEX_AUTOCONFIGURATION                    (PN_U16) 0xF850u
#define PROFINET_INDEX_TRIGGER_CONNECTION_MONITOR           (PN_U16) 0xFBFFu
#define PROFINET_INDEX_PROFILE_RESERVED_START               (PN_U16) 0xFC00u
#define PROFINET_INDEX_PROFILE_RESERVED_END                 (PN_U16) 0xFFFFu

#define PN_SERVICE_READ_INVALID_API                         PN_SERVICE_WRITE_INVALID_API
#define PN_SERVICE_READ_INVALID_SLOT                        PN_SERVICE_WRITE_INVALID_SLOT
#define PN_SERVICE_READ_INVALID_SUBSLOT                     PN_SERVICE_WRITE_INVALID_SUBSLOT
#define PN_SERVICE_READ_INVALID_INDEX                       PN_SERVICE_WRITE_INVALID_INDEX
#define PN_SERVICE_READ_RESPONSE_TIMEOUT                    PN_SERVICE_WRITE_RESPONSE_TIMEOUT
#define PN_SERVICE_READ_INVALID_SEQUENCE_NUMBER             PN_SERVICE_WRITE_INVALID_SEQUENCE_NUMBER
#define PN_SERVICE_READ_INVALID_AR_UUID                     PN_SERVICE_WRITE_INVALID_AR_UUID

typedef struct _T_PN_SERVICE_READ_IND
{
  PN_U8   ARIdentifierHighByte;
  PN_U8   ARIdentifierLowByte;

  PN_U8   APIHighWordHighByte;
  PN_U8   APIHighWordLowByte;
  PN_U8   APILowWordHighByte;
  PN_U8   APILowWordLowByte;

  PN_U8   SlotNumberHighByte;
  PN_U8   SlotNumberLowByte;

  PN_U8   SubSlotNumberHighByte;
  PN_U8   SubSlotNumberLowByte;

  PN_U8   IndexHighByte;
  PN_U8   IndexLowByte;

  PN_U8   LengthHighByte;
  PN_U8   LengthLowByte;

} PACK_BYTE_ALIGNMENT (T_PN_SERVICE_READ_IND);

/*---------------------------------------------------------------------------*/

typedef struct _T_PN_SERVICE_READ_REQ
{
  PN_U8   VersionHighByte;
  PN_U8   VersionLowByte;

  PN_U8   APIHighWordHighByte;
  PN_U8   APIHighWordLowByte;
  PN_U8   APILowWordHighByte;
  PN_U8   APILowWordLowByte;

  PN_U8   SlotNumberHighByte;
  PN_U8   SlotNumberLowByte;

  PN_U8   SubSlotNumberHighByte;
  PN_U8   SubSlotNumberLowByte;

  PN_U8   IndexHighByte;
  PN_U8   IndexLowByte;

  PN_U8   LengthHighByte;
  PN_U8   LengthLowByte;

} PACK_BYTE_ALIGNMENT (T_PN_SERVICE_READ_REQ);

/*---------------------------------------------------------------------------*/

typedef struct _T_PN_SERVICE_READ_CON
{
  PN_U8   APIHighWordHighByte;
  PN_U8   APIHighWordLowByte;
  PN_U8   APILowWordHighByte;
  PN_U8   APILowWordLowByte;

  PN_U8   SlotNumberHighByte;
  PN_U8   SlotNumberLowByte;

  PN_U8   SubSlotNumberHighByte;
  PN_U8   SubSlotNumberLowByte;

  PN_U8   IndexHighByte;
  PN_U8   IndexLowByte;

  PN_U8   LengthHighByte;
  PN_U8   LengthLowByte;

/*
  PN_U8   Data [Length];
*/

} PACK_BYTE_ALIGNMENT (T_PN_SERVICE_READ_CON);

/*---------------------------------------------------------------------------*/

#define READ_RES_ERROR_CODE1_OK                           ERROR_CODE1_OK
#define READ_RES_ERROR_CODE2_OK                           ERROR_CODE2_OK

#define READ_RES_ERROR_CODE1_INVALID_LENGTH               WRITE_RES_ERROR_CODE1_INVALID_LENGTH
#define READ_RES_ERROR_CODE2_INVALID_LENGTH               WRITE_RES_ERROR_CODE2_INVALID_LENGTH
#define READ_RES_ERROR_CODE1_RESSOURCE_BUSY               WRITE_RES_ERROR_CODE1_RESSOURCE_BUSY
#define READ_RES_ERROR_CODE2_RESSOURCE_BUSY               WRITE_RES_ERROR_CODE2_RESSOURCE_BUSY
#define READ_RES_ERROR_CODE1_INVALID_INDEX                WRITE_RES_ERROR_CODE1_INVALID_INDEX
#define READ_RES_ERROR_CODE2_INVALID_INDEX                WRITE_RES_ERROR_CODE2_INVALID_INDEX
#define READ_RES_ERROR_CODE1_INVALID_API                  WRITE_RES_ERROR_CODE1_INVALID_API
#define READ_RES_ERROR_CODE2_INVALID_API                  WRITE_RES_ERROR_CODE2_INVALID_API
#define READ_RES_ERROR_CODE1_INVALID_SLOT_SUBSLOT         WRITE_RES_ERROR_CODE1_INVALID_SLOT_SUBSLOT
#define READ_RES_ERROR_CODE2_INVALID_SLOT_SUBSLOT         WRITE_RES_ERROR_CODE2_INVALID_SLOT_SUBSLOT

typedef struct _T_PN_SERVICE_READ_RES
{
  PN_U8   ARIdentifierHighByte;
  PN_U8   ARIdentifierLowByte;

  PN_U8   ErrorCode1;
  PN_U8   ErrorCode2;

  PN_U8   LengthHighByte;
  PN_U8   LengthLowByte;

/*
  PN_U8   Data [Length];
*/

} PACK_BYTE_ALIGNMENT (T_PN_SERVICE_READ_RES);

/*===========================================================================*/

#define PROFINET_BLOCK_TYPE_READ_REQ                          (PN_U16) 0x0009u
#define PROFINET_BLOCK_TYPE_READ_RES                          (PN_U16) 0x8009u
#define PROFINET_BLOCK_TYPE_DIAGNOSIS                         (PN_U16) 0x0010u
#define PROFINET_BLOCK_TYPE_EXPECTED_IDENT                    (PN_U16) 0x0012u
#define PROFINET_BLOCK_TYPE_REAL_IDENT                        (PN_U16) 0x0013u
#define PROFINET_BLOCK_TYPE_SUBSTITUTE_VALUE                  (PN_U16) 0x0014u
#define PROFINET_BLOCK_TYPE_INPUT_DATA                        (PN_U16) 0x0015u
#define PROFINET_BLOCK_TYPE_OUTPUT_DATA                       (PN_U16) 0x0016u
#define PROFINET_BLOCK_TYPE_AR_DATA                           (PN_U16) 0x0018u
#define PROFINET_BLOCK_TYPE_LOG_DATA                          (PN_U16) 0x0019u
#define PROFINET_BLOCK_TYPE_API_DATA                          (PN_U16) 0x001Au
#define PROFINET_BLOCK_TYPE_SRL_DATA                          (PN_U16) 0x001Bu
#define PROFINET_BLOCK_TYPE_IDENT_AND_MAINTENANCE_0           (PN_U16) 0x0020u
#define PROFINET_BLOCK_TYPE_IDENT_AND_MAINTENANCE_1           (PN_U16) 0x0021u
#define PROFINET_BLOCK_TYPE_IDENT_AND_MAINTENANCE_2           (PN_U16) 0x0022u
#define PROFINET_BLOCK_TYPE_IDENT_AND_MAINTENANCE_3           (PN_U16) 0x0023u
#define PROFINET_BLOCK_TYPE_IDENT_AND_MAINTENANCE_4           (PN_U16) 0x0024u
#define PROFINET_BLOCK_TYPE_IM0_FILTER_SUBMODULE              (PN_U16) 0x0030u
#define PROFINET_BLOCK_TYPE_IM0_FILTER_MODULE                 (PN_U16) 0x0031u
#define PROFINET_BLOCK_TYPE_IM0_FILTER_DEVICE                 (PN_U16) 0x0032u
#define PROFINET_BLOCK_TYPE_MODULE_DIFF                       (PN_U16) 0x8104u
#define PROFINET_BLOCK_TYPE_PRM_SERVER                        (PN_U16) 0x0105u
#define PROFINET_BLOCK_TYPE_AR_VENDOR_REQ                     (PN_U16) 0x0108u
#define PROFINET_BLOCK_TYPE_AR_VENDOR_RES                     (PN_U16) 0x8108u
#define PROFINET_BLOCK_TYPE_IR_INFO                           (PN_U16) 0x0109u
#define PROFINET_BLOCK_TYPE_SR_INFO                           (PN_U16) 0x010Au
#define PROFINET_BLOCK_TYPE_AR_FSU                            (PN_U16) 0x010Bu
#define PROFINET_BLOCK_TYPE_PD_PORT_DATA_CHECK                (PN_U16) 0x0200u
#define PROFINET_BLOCK_TYPE_PDEV_DATA                         (PN_U16) 0x0201u
#define PROFINET_BLOCK_TYPE_PD_PORT_DATA_ADJUST               (PN_U16) 0x0202u
#define PROFINET_BLOCK_TYPE_PD_SYNC_DATA                      (PN_U16) 0x0203u
#define PROFINET_BLOCK_TYPE_ISOCHRONOUS_MODE_DATA             (PN_U16) 0x0204u
#define PROFINET_BLOCK_TYPE_PDIR_DATA                         (PN_U16) 0x0205u
#define PROFINET_BLOCK_TYPE_PDIR_GLOBAL_DATA                  (PN_U16) 0x0206u
#define PROFINET_BLOCK_TYPE_PDIR_FRAME_DATA                   (PN_U16) 0x0207u
#define PROFINET_BLOCK_TYPE_PDIR_BEGIN_END_DATA               (PN_U16) 0x0208u
#define PROFINET_BLOCK_TYPE_PD_PORT_DOMAIN_BOUNDARY_ADJUST    (PN_U16) 0x0209u
#define PROFINET_BLOCK_TYPE_PD_PORT_PEER_CHECK                (PN_U16) 0x020Au
#define PROFINET_BLOCK_TYPE_PD_PORT_LINE_DELAY_CHECK          (PN_U16) 0x020Bu
#define PROFINET_BLOCK_TYPE_PD_PORT_MAU_TYPE_CHECK            (PN_U16) 0x020Cu
#define PROFINET_BLOCK_TYPE_PD_PORT_MAU_TYPE_ADJUST           (PN_U16) 0x020Eu
#define PROFINET_BLOCK_TYPE_PD_PORT_DATA_REAL                 (PN_U16) 0x020Fu
#define PROFINET_BLOCK_TYPE_PD_PORT_MULTICAST_BOUNDARY_ADJUST (PN_U16) 0x0210u
#define PROFINET_BLOCK_TYPE_PD_INTERFACE_MRP_ADJUST           (PN_U16) 0x0211u
#define PROFINET_BLOCK_TYPE_PD_INTERFACE_MRP_REAL             (PN_U16) 0x0212u
#define PROFINET_BLOCK_TYPE_PD_INTERFACE_MRP_CHECK            (PN_U16) 0x0213u
#define PROFINET_BLOCK_TYPE_PD_PORT_MRP_ADJUST                (PN_U16) 0x0214u
#define PROFINET_BLOCK_TYPE_PD_PORT_MRP_REAL                  (PN_U16) 0x0215u
#define PROFINET_BLOCK_TYPE_MRP_MANAGER_PARAMS                (PN_U16) 0x0216u
#define PROFINET_BLOCK_TYPE_MRP_CLIENT_PARAMS                 (PN_U16) 0x0217u
#define PROFINET_BLOCK_TYPE_MRP_RING_STATE                    (PN_U16) 0x0219u
#define PROFINET_BLOCK_TYPE_PD_PORT_LINK_STATE_ADJUST         (PN_U16) 0x021Bu
#define PROFINET_BLOCK_TYPE_PD_PORT_LINK_STATE_CHECK          (PN_U16) 0x021Cu
#define PROFINET_BLOCK_TYPE_PD_PORT_SYNC_DIFF_CHECK           (PN_U16) 0x021Eu
#define PROFINET_BLOCK_TYPE_PD_PORT_MAU_TYPE_DIFF_CHECK       (PN_U16) 0x021Fu
#define PROFINET_BLOCK_TYPE_PD_PORT_FO_REAL                   (PN_U16) 0x0220u
#define PROFINET_BLOCK_TYPE_PD_PORT_FO_REAL_VENDOR            (PN_U16) 0x0221u
#define PROFINET_BLOCK_TYPE_PD_PORT_FO_DATA_CHECK             (PN_U16) 0x0222u
#define PROFINET_BLOCK_TYPE_PD_PORT_FO_DATA_ADJUST            (PN_U16) 0x0223u
#define PROFINET_BLOCK_TYPE_PD_PORT_P2P_BOUNDARY_ADJUST       (PN_U16) 0x0224u
#define PROFINET_BLOCK_TYPE_PD_PORT_DCP_BOUNDARY_ADJUST       (PN_U16) 0x0225u
#define PROFINET_BLOCK_TYPE_PD_PORT_PREAMBLE_LENGTH_ADJUST    (PN_U16) 0x0226u
#define PROFINET_BLOCK_TYPE_FO_DIAGNOSIS_INFO                 (PN_U16) 0x0228u
#define PROFINET_BLOCK_TYPE_PDIR_SUBFRAME_DATA                (PN_U16) 0x022Au
#define PROFINET_BLOCK_TYPE_SUBFRAME_BLOCK                    (PN_U16) 0x022Bu
#define PROFINET_BLOCK_TYPE_NC_DATA_CHECK                     (PN_U16) 0x0230u
#define PROFINET_BLOCK_TYPE_PD_INTERFACE_REAL                 (PN_U16) 0x0240u
#define PROFINET_BLOCK_TYPE_PD_INTERFACE_ADJUST               (PN_U16) 0x0250u
#define PROFINET_BLOCK_TYPE_PD_PORT_STATISTIC                 (PN_U16) 0x0251u
#define PROFINET_BLOCK_TYPE_MULTIPLE_BLOCKS                   (PN_U16) 0x0400u
#define PROFINET_BLOCK_TYPE_READ_QUERY                        (PN_U16) 0x0500u
#define PROFINET_BLOCK_TYPE_FS_HELLO                          (PN_U16) 0x0600u
#define PROFINET_BLOCK_TYPE_FS_PARAMETER                      (PN_U16) 0x0601u
#define PROFINET_BLOCK_TYPE_FSU_1_BEGIN                       (PN_U16) 0x0602u
#define PROFINET_BLOCK_TYPE_FSU_1_END                         (PN_U16) 0x0607u
#define PROFINET_BLOCK_TYPE_PD_INTERFACE_FSU_ADJUST           (PN_U16) 0x0608u
#define PROFINET_BLOCK_TYPE_AR_FSU_ADJUST                     (PN_U16) 0x0609u
#define PROFINET_BLOCK_TYPE_FSU_2_BEGIN                       (PN_U16) 0x060Au
#define PROFINET_BLOCK_TYPE_FSU_2_END                         (PN_U16) 0x060Fu
#define PROFINET_BLOCK_TYPE_AC_DATA                           (PN_U16) 0x0700u
#define PROFINET_BLOCK_TYPE_AC_COMMUNICATION_DATA             (PN_U16) 0x0701u
#define PROFINET_BLOCK_TYPE_AC_CONFIGURATION_DATA             (PN_U16) 0x0702u
#define PROFINET_BLOCK_TYPE_AC_ISOCHRONOUS_DATA               (PN_U16) 0x0703u
#define PROFINET_BLOCK_TYPE_ENERGY_START                      (PN_U16) 0x0800u
#define PROFINET_BLOCK_TYPE_ENERGY_END                        (PN_U16) 0x08FFu
#define PROFINET_BLOCK_TYPE_MAINTENANCE_ITEM                  (PN_U16) 0x0F00u
#define PROFINET_BLOCK_TYPE_UPLOAD_SELECTED_RECORD            (PN_U16) 0x0F01u
#define PROFINET_BLOCK_TYPE_I_PARAMETER_ITEM                  (PN_U16) 0x0F02u
#define PROFINET_BLOCK_TYPE_RETRIEVE_SELECTED_RECORD          (PN_U16) 0x0F03u
#define PROFINET_BLOCK_TYPE_RETRIEVE_ALL_RECORDS              (PN_U16) 0x0F04u

#define PROFINET_INDEX_USER_SPECIFIC_BASE                     (PN_U16) 0x0000u
#define PROFINET_INDEX_USER_SPECIFIC_SIZE                     (PN_U16) 0x7FFFu
#define PROFINET_INDEX_SUBSLOT_SPECIFIC_BASE                  (PN_U16) 0x8000u
#define PROFINET_INDEX_SUBSLOT_SPECIFIC_SIZE                  (PN_U16) 0x3FFFu
#define PROFINET_INDEX_SLOT_SPECIFIC_BASE                     (PN_U16) 0xC000u
#define PROFINET_INDEX_SLOT_SPECIFIC_SIZE                     (PN_U16) 0x1FFFu
#define PROFINET_INDEX_AR_SPECIFIC_BASE                       (PN_U16) 0xE000u
#define PROFINET_INDEX_AR_SPECIFIC_SIZE                       (PN_U16) 0x0FFFu
#define PROFINET_INDEX_API_SPECIFIC_BASE                      (PN_U16) 0xF000u
#define PROFINET_INDEX_API_SPECIFIC_SIZE                      (PN_U16) 0x07FFu
#define PROFINET_INDEX_DEVICE_SPECIFIC_BASE                   (PN_U16) 0xF800u
#define PROFINET_INDEX_DEVICE_SPECIFIC_SIZE                   (PN_U16) 0x07FFu

typedef T_PN_SERVICE_READ_IND   T_PN_SERVICE_IMPLICIT_READ_IND;

/*---------------------------------------------------------------------------*/

#define PN_SERVICE_IMPLICIT_READ_RESPONSE_TIMEOUT       PN_SERVICE_SET_RESPONSE_TIMEOUT
#define PN_SERVICE_IMPLICIT_READ_INVALID_IP_ADDRESS     (PN_SERVICE_IMPLICIT_READ_RESPONSE_TIMEOUT + 0x01u)
#define PN_SERVICE_IMPLICIT_READ_INVALID_BLOCK_TYPE     (PN_SERVICE_IMPLICIT_READ_RESPONSE_TIMEOUT + 0x02u)
#define PN_SERVICE_IMPLICIT_READ_INVALID_API            (PN_SERVICE_IMPLICIT_READ_RESPONSE_TIMEOUT + 0x03u)
#define PN_SERVICE_IMPLICIT_READ_INVALID_SLOT           (PN_SERVICE_IMPLICIT_READ_RESPONSE_TIMEOUT + 0x04u)
#define PN_SERVICE_IMPLICIT_READ_INVALID_SUBSLOT        (PN_SERVICE_IMPLICIT_READ_RESPONSE_TIMEOUT + 0x05u)
#define PN_SERVICE_IMPLICIT_READ_INVALID_TARGET_UUID    (PN_SERVICE_IMPLICIT_READ_RESPONSE_TIMEOUT + 0x06u)

typedef struct _T_PN_SERVICE_IMPLICIT_READ_REQ
{
  PN_U8             IpAddressHighWordHighByte;
  PN_U8             IpAddressHighWordLowByte;
  PN_U8             IpAddressLowWordHighByte;
  PN_U8             IpAddressLowWordLowByte;

  PN_U8             TypeHighByte;
  PN_U8             TypeLowByte;

  PN_U8             APIHighWordHighByte;
  PN_U8             APIHighWordLowByte;
  PN_U8             APILowWordHighByte;
  PN_U8             APILowWordLowByte;

  PN_U8             SlotNumberHighByte;
  PN_U8             SlotNumberLowByte;

  PN_U8             SubSlotNumberHighByte;
  PN_U8             SubSlotNumberLowByte;

  PN_U8             IndexHighByte;
  PN_U8             IndexLowByte;

  PN_U8             InstanceHighByte;
  PN_U8             InstanceLowByte;

  PN_U8             DeviceIdHighByte;
  PN_U8             DeviceIdLowByte;

  PN_U8             VendorIdHighByte;
  PN_U8             VendorIdLowByte;

/*
  T_PROFINET_UUID   TargetUUID;
*/

} PACK_BYTE_ALIGNMENT (T_PN_SERVICE_IMPLICIT_READ_REQ);

/*---------------------------------------------------------------------------*/

typedef struct _T_PN_SERVICE_IMIPLICIT_READ_CON
{
  PN_U8   TypeHighByte;
  PN_U8   TypeLowByte;

  PN_U8   APIHighWordHighByte;
  PN_U8   APIHighWordLowByte;
  PN_U8   APILowWordHighByte;
  PN_U8   APILowWordLowByte;

  PN_U8   SlotNumberHighByte;
  PN_U8   SlotNumberLowByte;

  PN_U8   SubSlotNumberHighByte;
  PN_U8   SubSlotNumberLowByte;

  PN_U8   IndexHighByte;
  PN_U8   IndexLowByte;

  PN_U8   LengthHighByte;
  PN_U8   LengthLowByte;

/*
  PN_U8   Data [Length];
*/

} PACK_BYTE_ALIGNMENT (T_PN_SERVICE_IMPLICIT_READ_CON);

/*---------------------------------------------------------------------------*/

typedef T_PN_SERVICE_READ_RES   T_PN_SERVICE_IMPLICIT_READ_RES;

/*===========================================================================*/

#define PN_SERVICE_LOOKUP_RESPONSE_TIMEOUT        PN_SERVICE_SET_RESPONSE_TIMEOUT
#define PN_SERVICE_LOOKUP_INVALID_IP_ADDRESS      (PN_SERVICE_LOOKUP_RESPONSE_TIMEOUT + 0x01u)
#define PN_SERVICE_LOOKUP_INVALID_RESPONSE        (PN_SERVICE_LOOKUP_RESPONSE_TIMEOUT + 0x02u)

typedef struct _T_PN_SERVICE_LOOKUP_REQ
{
  PN_U8             IpAddressHighWordHighByte;
  PN_U8             IpAddressHighWordLowByte;
  PN_U8             IpAddressLowWordHighByte;
  PN_U8             IpAddressLowWordLowByte;

} PACK_BYTE_ALIGNMENT (T_PN_SERVICE_LOOKUP_REQ);

/*---------------------------------------------------------------------------*/

typedef struct _T_PN_SERVICE_LOOKUP_CON
{
  PN_U8             InstanceHighByte;
  PN_U8             InstanceLowByte;

  PN_U8             DeviceIdHighByte;
  PN_U8             DeviceIdLowByte;

  PN_U8             VendorIdHighByte;
  PN_U8             VendorIdLowByte;

} PACK_BYTE_ALIGNMENT (T_PN_SERVICE_LOOKUP_CON);

/*===========================================================================*/

#define DISCOVER_STANDARD_MODE                    0x00u
#define DISCOVER_FORCED_PROFINET_MODE             0x01u

#define IP_ADDRESS_LOCAL_NODE                     0x7F000001uL

#define PN_MAX_COMMUNITY_STRING_LENGTH            128u

#define PN_SERVICE_DISCOVER_INVALID_MAC_ADDRESS   0x01u
#define PN_SERVICE_DISCOVER_DCP_TIMEOUT           0x02u

typedef struct _T_PN_SERVICE_DISCOVER_REQ
{
  PN_U8               Flags;
  PN_U8               Alignment;

  T_PN_MAC_ADDRESS    MacAddress;

  PN_U8               IpAddressHighWordHighByte;
  PN_U8               IpAddressHighWordLowByte;
  PN_U8               IpAddressLowWordHighByte;
  PN_U8               IpAddressLowWordLowByte;

  PN_U8               CommunityString [PN_MAX_COMMUNITY_STRING_LENGTH];
  PN_U8               Reserved [32];

} PACK_BYTE_ALIGNMENT (T_PN_SERVICE_DISCOVER_REQ);

/*---------------------------------------------------------------------------*/

#define PN_DISCOVER_BLOCK_TYPE_NODE_LOCAL_DATA      0x0001u
#define PN_DISCOVER_BLOCK_TYPE_NODE_REMOTE_DATA     0x0002u

typedef struct _T_PN_DISCOVER_BLOCK_HEADER
{
  PN_U8   TypeHighByte;
  PN_U8   TypeLowByte;

  PN_U8   LengthHighByte;
  PN_U8   LengthLowByte;

  PN_U8   VersionHighByte;
  PN_U8   VersionLowByte;

} PACK_BYTE_ALIGNMENT (T_PN_DISCOVER_BLOCK_HEADER);

/*---------------------------------------------------------------------------*/

typedef T_PROFINET_UUID       T_MRP_UUID;

typedef struct _T_PN_DISCOVER_NODE_LOCAL_IF_DATA_V10
{
  PN_U8                 InterfaceNumber;
  PN_U8                 Alignment;

  PN_U8                 NameOfStationLength;
  PN_U8                 MrpDomainNameLength;

  PN_U8                 NameOfStation [PN_MAX_DEVICE_NAME_LENGTH];

  T_PN_MAC_ADDRESS      MacAddress;
  T_PN_DEVICE_IP_INFO   IpInfo;

  T_MRP_UUID            MrpDomainUUID;

  PN_U8                 MrpRoleHighByte;
  PN_U8                 MrpRoleLowByte;

  PN_U8                 MrpDomainName [PN_MAX_DEVICE_NAME_LENGTH];

} PACK_BYTE_ALIGNMENT (T_PN_DISCOVER_NODE_LOCAL_IF_DATA_V10);

/*---------------------------------------------------------------------------*/

#define PN_CHASSIS_ID_TYPE_UNKNOWN                  0u
#define PN_CHASSIS_ID_TYPE_MAC_ADDRESS              4u
#define PN_CHASSIS_ID_TYPE_IP_ADDRESS               5u
#define PN_CHASSIS_ID_TYPE_IF_NAME                  6u
#define PN_CHASSIS_ID_TYPE_CHASSIS_ID               7u

/*---------------------------------------------------------------------------*/

#define PN_PORT_ID_TYPE_UNKNOWN                     0u
#define PN_PORT_ID_TYPE_MAC_ADDRESS                 3u
#define PN_PORT_ID_TYPE_IP_ADDRESS                  4u
#define PN_PORT_ID_TYPE_IF_NAME                     5u
#define PN_PORT_ID_TYPE_AGENT_ID                    6u
#define PN_PORT_ID_TYPE_PORT_ID                     7u

/*---------------------------------------------------------------------------*/

#define PN_FEATURE_OPTIMIZED_RTC3_MODE_ENABLED      0x00000001uL
#define PN_FEATURE_OPTIMIZED_RTC3_MODE_SUPPORTED    0x00000002uL
#define PN_FEATURE_PREAMBLE_SHORT_MODE_ENABLED      0x00000004uL
#define PN_FEATURE_PREAMBLE_SHORT_MODE_SUPPORTED    0x00000008uL
#define PN_FEATURE_FRAGMENTATION_ENABLED            0x00000010uL
#define PN_FEATURE_FRAGMENTATION_SUPPORTED          0x00000020uL

typedef struct _T_PN_DISCOVER_NODE_LOCAL_PORT_DATA_V10
{
  PN_U8                 InterfaceNumber;
  PN_U8                 PortNumber;

  PN_U8                 PortIdTypeHighByte;
  PN_U8                 PortIdTypeLowByte;

  PN_U8                 PortIdLength;
  PN_U8                 PortDescriptionLength;

  PN_U8                 PortId [256];
  PN_U8                 PortDescription [256];

  PN_U8                 LineDelayHighWordHighByte;
  PN_U8                 LineDelayHighWordLowByte;
  PN_U8                 LineDelayLowWordHighByte;
  PN_U8                 LineDelayLowWordLowByte;

  PN_U8                 RxDelayHighWordHighByte;
  PN_U8                 RxDelayHighWordLowByte;
  PN_U8                 RxDelayLowWordHighByte;
  PN_U8                 RxDelayLowWordLowByte;

  PN_U8                 TxDelayHighWordHighByte;
  PN_U8                 TxDelayHighWordLowByte;
  PN_U8                 TxDelayLowWordHighByte;
  PN_U8                 TxDelayLowWordLowByte;

  T_PN_MAC_ADDRESS      PtcpMasterAddress;

  PN_U8                 RTC3Status;
  PN_U8                 MRPDStatus;

  T_PROFINET_UUID       PtcpDomainUUID;
  T_PROFINET_UUID       IRDataUUID;

  PN_U8                 FeatureHighWordHighByte;
  PN_U8                 FeatureHighWordLowByte;
  PN_U8                 FeatureLowWordHighByte;
  PN_U8                 FeatureLowWordLowByte;

  PN_U8                 MauTypeHighByte;
  PN_U8                 MauTypeLowByte;

} PACK_BYTE_ALIGNMENT (T_PN_DISCOVER_NODE_LOCAL_PORT_DATA_V10);

/*---------------------------------------------------------------------------*/

typedef struct _T_PN_DISCOVER_NODE_LOCAL_DATA_V10
{
  T_PN_DISCOVER_BLOCK_HEADER                Header;

  PN_U8                                     ChassisIdTypeHighByte;
  PN_U8                                     ChassisIdTypeLowByte;

  PN_U8                                     ChassisIdLengthHighByte;
  PN_U8                                     ChassisIdLengthLowByte;

  PN_U8                                     ChassisId [256];

  PN_U8                                     NumberOfIfHighByte;
  PN_U8                                     NumberOfIfLowByte;

  PN_U8                                     NumberOfPortHighByte;
  PN_U8                                     NumberOfPortLowByte;

/*T_PN_DISCOVER_NODE_LOCAL_IF_DATA_V10      NodeLocalIfData [NumberOfIf]; */
/*T_PN_DISCOVER_NODE_LOCAL_PORT_DATA_V10    NodeLocalPortData [NumberOfPort]; */

} PACK_BYTE_ALIGNMENT (T_PN_DISCOVER_NODE_LOCAL_DATA_V10);

/*---------------------------------------------------------------------------*/

typedef struct _T_PN_DISCOVER_NODE_REMOTE_PEER_DATA_V10
{
  PN_U8                 LocalInterfaceNumber;
  PN_U8                 LocalPortNumber;

  PN_U8                 ChassisIdTypeHighByte;
  PN_U8                 ChassisIdTypeLowByte;

  PN_U8                 ChassisIdLengthHighByte;
  PN_U8                 ChassisIdLengthLowByte;

  PN_U8                 ChassisId [256];

  PN_U8                 PortIdTypeHighByte;
  PN_U8                 PortIdTypeLowByte;

  PN_U8                 PortIdLength;
  PN_U8                 PortDescriptionLength;

  PN_U8                 PortId [256];
  PN_U8                 PortDescription [256];

  PN_U8                 NameOfStationLength;
  PN_U8                 Alignment;

  PN_U8                 NameOfStation [PN_MAX_DEVICE_NAME_LENGTH];

  T_PN_MAC_ADDRESS      MacAddress;
  T_PN_DEVICE_IP_INFO   IpInfo;

} PACK_BYTE_ALIGNMENT (T_PN_DISCOVER_NODE_REMOTE_PEER_DATA_V10);

/*---------------------------------------------------------------------------*/

typedef struct _T_PN_DISCOVER_NODE_REMOTE_DATA_V10
{
  T_PN_DISCOVER_BLOCK_HEADER                Header;

  PN_U8                                     NumberOfPeerHighByte;
  PN_U8                                     NumberOfPeerLowByte;

/*T_PN_DISCOVER_NODE_REMOTE_PEER_DATA_V10   NodeRemotePeerData [NumberOfPeer]; */

} PACK_BYTE_ALIGNMENT (T_PN_DISCOVER_NODE_REMOTE_DATA_V10);

/*---------------------------------------------------------------------------*/

typedef struct _T_PN_SERVICE_DISCOVER_CON
{
  PN_U8                             RemainingLengthHighByte;
  PN_U8                             RemainingLengthLowByte;

  PN_U8                             Reserved [6];

/*T_PN_DISCOVER_NODE_LOCAL_DATA     NodeLocalData; */
/*T_PN_DISCOVER_NODE_REMOTE_DATA    NodeRemoteData; */

} PACK_BYTE_ALIGNMENT (T_PN_SERVICE_DISCOVER_CON);

/*===========================================================================*/

typedef struct _T_PROFINET_IOCR
{
  PN_U8   PropertiesHighWordHighByte;
  PN_U8   PropertiesHighWordLowByte;
  PN_U8   PropertiesLowWordHighByte;
  PN_U8   PropertiesLowWordLowByte;

  PN_U8   IOCRTypeHighByte;
  PN_U8   IOCRTypeLowByte;

  PN_U8   FrameIdHighByte;
  PN_U8   FrameIdLowByte;

  PN_U8   CycleCounterHighByte;
  PN_U8   CycleCounterLowByte;

  PN_U8   DataStatus;
  PN_U8   TransferStatus;

} PACK_BYTE_ALIGNMENT (T_PROFINET_IOCR);

/*---------------------------------------------------------------------------*/

typedef struct _T_PROFINET_API
{
  PN_U8   APIHighWordHighByte;
  PN_U8   APIHighWordLowByte;
  PN_U8   APILowWordHighByte;
  PN_U8   APILowWordLowByte;

} PACK_BYTE_ALIGNMENT (T_PROFINET_API);

/*---------------------------------------------------------------------------*/

typedef struct _T_PROFINET_IOCR_DFP
{
  PN_U8   IOCRReferenceHighByte;
  PN_U8   IOCRReferenceLowByte;

  PN_U8   SubframeOffsetHighByte;
  PN_U8   SubframeOffsetLowByte;

  PN_U8   SubframeDataHighWordHighByte;
  PN_U8   SubframeDataHighWordLowByte;
  PN_U8   SubframeDataLowWordHighByte;
  PN_U8   SubframeDataLowWordLowByte;

} PACK_BYTE_ALIGNMENT (T_PROFINET_IOCR_DFP);

/*---------------------------------------------------------------------------*/

typedef struct _T_PROFINET_IR_INFO
{
  T_PROFINET_BLOCK_HEADER   Header;

  PN_U8                     Padding1 [2];

  T_PROFINET_UUID           IRDataUUID;
  PN_U8                     Padding2 [2];

  PN_U8                     NumberOfIOCRHighByte;
  PN_U8                     NumberOfIOCRLowByte;

/*
  T_PROFINET_IOCR_DFP       IOCRDFP [NumberOfIOCR];
*/

} PACK_BYTE_ALIGNMENT (T_PROFINET_IR_INFO);

/*---------------------------------------------------------------------------*/

typedef struct _T_PROFINET_SR_INFO
{
  T_PROFINET_BLOCK_HEADER   Header;

  PN_U8                     DataHoldFactorHighByte;
  PN_U8                     DataHoldFactorLowByte;

  PN_U8                     PropertiesHighWordHighByte;
  PN_U8                     PropertiesHighWordLowByte;
  PN_U8                     PropertiesLowWordHighByte;
  PN_U8                     PropertiesLowWordLowByte;

} PACK_BYTE_ALIGNMENT (T_PROFINET_SR_INFO);

/*---------------------------------------------------------------------------*/

#define PROFINET_FSU_HELLO_MODE_OFF               0x00000000uL
#define PROFINET_FSU_HELLO_ON_LINK_UP             0x00000001uL
#define PROFINET_FSU_HELLO_DELAYED_ON_LINK_UP     0x00000002uL
#define PROFINET_FSU_HELLO_MODE_MASK              (PROFINET_FSU_HELLO_MODE_OFF  | \
                                                  PROFINET_FSU_HELLO_ON_LINK_UP | \
                                                  PROFINET_FSU_HELLO_DELAYED_ON_LINK_UP)

typedef struct _T_PROFINET_FS_HELLO
{
  T_PROFINET_BLOCK_HEADER   Header;

  PN_U8                     Padding [2];

  PN_U8                     ModeHighWordHighByte;
  PN_U8                     ModeHighWordLowByte;
  PN_U8                     ModeLowWordHighByte;
  PN_U8                     ModeLowWordLowByte;

  PN_U8                     IntervalHighWordHighByte;
  PN_U8                     IntervalHighWordLowByte;
  PN_U8                     IntervalLowWordHighByte;
  PN_U8                     IntervalLowWordLowByte;

  PN_U8                     RetryHighWordHighByte;
  PN_U8                     RetryHighWordLowByte;
  PN_U8                     RetryLowWordHighByte;
  PN_U8                     RetryLowWordLowByte;

  PN_U8                     DelayHighWordHighByte;
  PN_U8                     DelayHighWordLowByte;
  PN_U8                     DelayLowWordHighByte;
  PN_U8                     DelayLowWordLowByte;

} PACK_BYTE_ALIGNMENT (T_PROFINET_FS_HELLO);

/*---------------------------------------------------------------------------*/

typedef struct _T_PROFINET_FS_PRM
{
  T_PROFINET_BLOCK_HEADER   Header;

  PN_U8                     Padding1 [2];

  PN_U8                     ModeHighWordHighByte;
  PN_U8                     ModeHighWordLowByte;
  PN_U8                     ModeLowWordHighByte;
  PN_U8                     ModeLowWordLowByte;

  T_PROFINET_UUID           FSPrmUUID;
  PN_U8                     Padding2 [2];

} PACK_BYTE_ALIGNMENT (T_PROFINET_FS_PRM);

/*---------------------------------------------------------------------------*/

typedef struct _T_PROFINET_FS
{
  T_PROFINET_BLOCK_HEADER   Header;

  PN_U8                     Padding [2];

/*
  PN_U8                     Data [];
  PN_U8                     Padding []; if necessary to align to 32-Bit
*/

} PACK_BYTE_ALIGNMENT (T_PROFINET_FS);

/*---------------------------------------------------------------------------*/

typedef struct _T_PROFINET_AR_FSU_ADJUST
{
  T_PROFINET_BLOCK_HEADER   Header;

  PN_U8                     Padding [2];

/*
  T_PROFINET_FS_PRM         FSParameter;
  T_PROFINET_FS             FastStartup;
*/

} PACK_BYTE_ALIGNMENT (T_PROFINET_AR_FSU_ADJUST);

typedef T_PROFINET_AR_FSU_ADJUST      T_PROFINET_AR_FSU;

/*---------------------------------------------------------------------------*/

typedef struct _T_PROFINET_INTERFACE_FSU_ADJUST
{
  T_PROFINET_BLOCK_HEADER   Header;

  PN_U8                     Padding [2];

/*
  T_PROFINET_FS_HELLO       FSHello;
  T_PROFINET_FS             FastStartup;
*/

} PACK_BYTE_ALIGNMENT (T_PROFINET_INTERFACE_FSU_ADJUST);

/*---------------------------------------------------------------------------*/

typedef struct _T_PROFINET_SRL_DATA
{
  T_PROFINET_BLOCK_HEADER   Header;

  PN_U8                     RedundancyInfoHighByte;
  PN_U8                     RedundancyInfoLowByte;

} PACK_BYTE_ALIGNMENT (T_PROFINET_SRL_DATA);

/*---------------------------------------------------------------------------*/

typedef struct _T_PROFINET_AR_DATA_INFO
{
  PN_U8                 NumberEntriesHighByte;
  PN_U8                 NumberEntriesLowByte;


  PN_U8                 Padding [2];
/*
  T_PROFINET_IR_INFO    IRInfo;
  T_PROFINET_SR_INFO    SRInfo;
  T_PROFINET_AR_FSU     ARFSU;
  T_PROFINET_SRL_DATA   SRLData;

*/

} PACK_BYTE_ALIGNMENT (T_PROFINET_AR_DATA_INFO);

/*---------------------------------------------------------------------------*/

typedef struct _T_PROFINET_AR
{
  T_PROFINET_UUID           ARUUID;
  T_PROFINET_UUID           InitiatorUUID;
  T_PROFINET_UUID           PrmServerUUID;

  PN_U8                     PropertiesHighWordHighByte;
  PN_U8                     PropertiesHighWordLowByte;
  PN_U8                     PropertiesLowWordHighByte;
  PN_U8                     PropertiesLowWordLowByte;

  PN_U8                     ARTypeHighByte;
  PN_U8                     ARTypeLowByte;

  PN_U8                     AlarmCRTypeHighByte;
  PN_U8                     AlarmCRTypeLowByte;

  PN_U8                     LocalAlarmRefHighByte;
  PN_U8                     LocalAlarmRefLowByte;

  PN_U8                     RemoteAlarmRefHighByte;
  PN_U8                     RemoteAlarmRefLowByte;

  PN_U8                     InitiatorPortHighByte;
  PN_U8                     InitiatorPortLowByte;

  PN_U8                     ResponderPortHighByte;
  PN_U8                     ResponderPortLowByte;

  PN_U8                     InitiatorNameLengthHighByte;
  PN_U8                     InitiatorNameLengthLowByte;

/*
  PN_U8                     InitiatorName [InitiatorNameLength];
  PN_U8                     Padding []; if necessary to align to 32-Bit

  PN_U8                     PrmServerNameLengthHighByte;
  PN_U8                     PrmServerNameLengthLowByte;
  PN_U8                     PrmServerName [PrmServerNameLength];
  PN_U8                     Padding []; if necessary to align to 32-Bit

  PN_U8                     NumberOfIOCRHighByte;
  PN_U8                     NumberOfIOCRLowByte;
  PN_U8                     Padding []; if necessary to align to 32-Bit

  T_PROFINET_IOCR           IOCRData [NumberOfIOCRs];

  PN_U8                     NumberOfAPIHighByte;
  PN_U8                     NumberOfAPILowByte;
  PN_U8                     Padding []; if necessary to align to 32-Bit

  T_PROFINET_API            API [NumberOfAPI];

  T_PROFINET_AR_DATA_INFO   ARDataInfo;
*/

} PACK_BYTE_ALIGNMENT (T_PROFINET_AR);

/*---------------------------------------------------------------------------*/

typedef struct _T_PROFINET_AR_DATA
{
  T_PROFINET_BLOCK_HEADER   Header;

  PN_U8                     NumberOfARsHighByte;
  PN_U8                     NumberOfARsLowByte;

/*
  T_PROFINET_AR             AR [NumberOfARs];
*/

} PACK_BYTE_ALIGNMENT (T_PROFINET_AR_DATA);

/*===========================================================================*/

typedef struct _T_PROFINET_IM0_FILTER_SUBMODULE
{
  PN_U8             SubSlotNumberHighByte;
  PN_U8             SubSlotNumberLowByte;

  PN_U8             IdentNumberHighWordHighByte;
  PN_U8             IdentNumberHighWordLowByte;
  PN_U8             IdentNumberLowWordHighByte;
  PN_U8             IdentNumberLowWordLowByte;

} PACK_BYTE_ALIGNMENT (T_PROFINET_IM0_FILTER_SUBMODULE);

typedef T_PROFINET_IM0_FILTER_SUBMODULE   T_PROFINET_REAL_IDENT_SUBMODULE;
typedef T_PROFINET_IM0_FILTER_SUBMODULE   T_PROFINET_EXPECTED_IDENT_SUBMODULE;

/*---------------------------------------------------------------------------*/

typedef struct _T_PROFINET_IM0_FILTER_MODULE
{
  PN_U8                             SlotNumberHighByte;
  PN_U8                             SlotNumberLowByte;

  PN_U8                             IdentNumberHighWordHighByte;
  PN_U8                             IdentNumberHighWordLowByte;
  PN_U8                             IdentNumberLowWordHighByte;
  PN_U8                             IdentNumberLowWordLowByte;

  PN_U8                             NumberOfSubmodulesHighByte;
  PN_U8                             NumberOfSubmodulesLowByte;

/*
  T_PROFINET_IM0_FILTER_SUBMODULE   Submodule [NumberOfSubmodules];
*/

} PACK_BYTE_ALIGNMENT (T_PROFINET_IM0_FILTER_MODULE);

typedef T_PROFINET_IM0_FILTER_MODULE    T_PROFINET_REAL_IDENT_MODULE;
typedef T_PROFINET_IM0_FILTER_MODULE    T_PROFINET_EXPECTED_IDENT_MODULE;

/*---------------------------------------------------------------------------*/

typedef struct _T_PROFINET_IM0_FILTER_API
{
  PN_U8                           APIHighWordHighByte;
  PN_U8                           APIHighWordLowByte;
  PN_U8                           APILowWordHighByte;
  PN_U8                           APILowWordLowByte;

  PN_U8                           NumberOfModulesHighByte;
  PN_U8                           NumberOfModulesLowByte;

/*
  T_PROFINET_IM0_FILTER_MODULE    Module [NumberOfModules];
*/

} PACK_BYTE_ALIGNMENT (T_PROFINET_IM0_FILTER_API);

typedef T_PROFINET_IM0_FILTER_API   T_PROFINET_REAL_IDENT_API;
typedef T_PROFINET_IM0_FILTER_API   T_PROFINET_EXPECTED_IDENT_API;

/*---------------------------------------------------------------------------*/

typedef struct _T_PROFINET_IM0_FILTER_DATA
{
  T_PROFINET_BLOCK_HEADER   Header;

  PN_U8                     NumberOfAPIsHighByte;
  PN_U8                     NumberOfAPIsLowByte;

/*
  T_PROFINET_API            API [NumberOfAPIs];
*/

} PACK_BYTE_ALIGNMENT (T_PROFINET_IM0_FILTER_DATA);

/*===========================================================================*/

typedef struct _T_PROFINET_API_DATA
{
  T_PROFINET_BLOCK_HEADER   Header;

  PN_U8                     NumberOfAPIsHighByte;
  PN_U8                     NumberOfAPIsLowByte;

/*
  T_PROFINET_API            API [NumberOfAPIs];
*/

} PACK_BYTE_ALIGNMENT (T_PROFINET_API_DATA);

/*===========================================================================*/

#define PROFINET_SUBMODULE_DATA_DESCRIPTION_INPUT     (PN_U16) 0x0001u
#define PROFINET_SUBMODULE_DATA_DESCRIPTION_OUTPUT    (PN_U16) 0x0002u

typedef struct _T_PROFINET_SUBMODULE_DATA
{
  PN_U8    SubSlotNumberHighByte;
  PN_U8    SubSlotNumberLowByte;

  PN_U8    IdentNumberHighWordHighByte;
  PN_U8    IdentNumberHighWordLowByte;
  PN_U8    IdentNumberLowWordHighByte;
  PN_U8    IdentNumberLowWordLowByte;

  PN_U8    PropertiesHighByte;
  PN_U8    PropertiesLowByte;

} PACK_BYTE_ALIGNMENT (T_PROFINET_SUBMODULE_DATA);

/*---------------------------------------------------------------------------*/

typedef struct _T_PROFINET_SUBMODULE_IO_DATA
{
  PN_U8    DataDescriptionHighByte;
  PN_U8    DataDescriptionLowByte;

  PN_U8    IODataLengthHighByte;
  PN_U8    IODataLengthLowByte;

  PN_U8    ConsumerStatusLength;
  PN_U8    ProviderStatusLength;

} PACK_BYTE_ALIGNMENT (T_PROFINET_SUBMODULE_IO_DATA);

/*===========================================================================*/

typedef struct _T_PROFINET_MODULE_API
{
  PN_U8                           APIHighWordHighByte;
  PN_U8                           APIHighWordLowByte;
  PN_U8                           APILowWordHighByte;
  PN_U8                           APILowWordLowByte;

  PN_U8                           SlotNumberHighByte;
  PN_U8                           SlotNumberLowByte;

  PN_U8                           IdentNumberHighWordHighByte;
  PN_U8                           IdentNumberHighWordLowByte;
  PN_U8                           IdentNumberLowWordHighByte;
  PN_U8                           IdentNumberLowWordLowByte;

  PN_U8                           PropertiesHighByte;
  PN_U8                           PropertiesLowByte;

  PN_U8                           NumberOfSubmodulesHighByte;
  PN_U8                           NumberOfSubmodulesLowByte;

/*
  T_PROFINET_SUBMODULE_DATA       SubModuleData [NumberOfSubmodules];
  T_PROFINET_SUBMODULE_IO_DATA    SubModuleIOData [NumberOfSubmodules];
*/

} PACK_BYTE_ALIGNMENT (T_PROFINET_MODULE_API);

/*---------------------------------------------------------------------------*/

typedef struct _T_PROFINET_MODULE_REQ
{
  T_PROFINET_BLOCK_HEADER   Header;

  PN_U8                     NumberOfAPIsHighByte;
  PN_U8                     NumberOfAPIsLowByte;

/*
  T_PROFINET_MODULE_API     Api [NumberOfAPIs];
*/

} PACK_BYTE_ALIGNMENT (T_PROFINET_MODULE_REQ);

typedef T_PROFINET_MODULE_REQ   T_PROFINET_REAL_IDENT_DATA;
typedef T_PROFINET_MODULE_REQ   T_PROFINET_EXPECTED_IDENT_DATA;
typedef T_PROFINET_MODULE_REQ   T_PROFINET_MODULE_DIFF_DATA;

/*---------------------------------------------------------------------------*/

typedef struct _T_PROFINET_RECORD_INPUT_DATA
{
  T_PROFINET_BLOCK_HEADER   Header;

  PN_U8                     ConsumerStatusLength;
  PN_U8                     ConsumerStatus;

  PN_U8                     ProviderStatusLength;
  PN_U8                     ProviderStatus;

  PN_U8                     DataLengthHighByte;
  PN_U8                     DataLengthLowByte;

/*
  PN_U8                     Data [DataLength];
*/

} PACK_BYTE_ALIGNMENT (T_PROFINET_RECORD_INPUT_DATA);

/*---------------------------------------------------------------------------*/

typedef struct _T_PROFINET_OUTPUT_DATA_ITEM
{
  PN_U8                         ConsumerStatus;
/*
  PN_U8                         Data [DataLength];
  PN_U8                         ProviderStatus;
*/

} PACK_BYTE_ALIGNMENT (T_PROFINET_OUTPUT_DATA_ITEM);

/*---------------------------------------------------------------------------*/

#define PROFINET_SUBSTITUTION_MODE_ZERO           (PN_U16) 0u
#define PROFINET_SUBSTITUTION_MODE_LAST_VALUE     (PN_U16) 1u
#define PROFINET_SUBSTITUTION_MODE_REPLACEMENT    (PN_U16) 2u

#define PROFINET_FAIL_SAFE_SUBSTITUTION_ACTIVE    (PN_U16) 0x0001u

typedef struct _T_PROFINET_SUBSTITUTE_VALUE
{
  T_PROFINET_BLOCK_HEADER       Header;

  PN_U8                         SubstitutionModeHighByte;
  PN_U8                         SubstitutionModeLowByte;

/*
  T_PROFINET_OUTPUT_DATA_ITEM   SubstituteData;
*/

} PACK_BYTE_ALIGNMENT (T_PROFINET_SUBSTITUTE_VALUE);

/*---------------------------------------------------------------------------*/

typedef struct _T_PROFINET_RECORD_OUTPUT_DATA
{
  T_PROFINET_BLOCK_HEADER       Header;

  PN_U8                         SubstituteActiveHighByte;
  PN_U8                         SubstituteActiveLowByte;

  PN_U8                         ConsumerStatusLength;
  PN_U8                         ProviderStatusLength;

  PN_U8                         DataLengthHighByte;
  PN_U8                         DataLengthLowByte;

/*
  T_PROFINET_OUTPUT_DATA_ITEM   Data;
  T_PROFINET_SUBSTITUTE_VALUE   SubstituteValue;
*/

} PACK_BYTE_ALIGNMENT (T_PROFINET_RECORD_OUTPUT_DATA);

/*===========================================================================*/

typedef struct _T_PROFINET_TIMESTAMP
{
  PN_U8   HighPartHighWordHighByte;
  PN_U8   HighPartHighWordLowByte;
  PN_U8   HighPartLowWordHighByte;
  PN_U8   HighPartLowWordLowByte;

  PN_U8   LowPartHighWordHighByte;
  PN_U8   LowPartHighWordLowByte;
  PN_U8   LowPartLowWordHighByte;
  PN_U8   LowPartLowWordLowByte;

} PACK_BYTE_ALIGNMENT (T_PROFINET_TIMESTAMP);

/*---------------------------------------------------------------------------*/

typedef struct _T_PROFINET_LOG_ENTRY
{
  T_PROFINET_TIMESTAMP    TimeStamp;

  T_PROFINET_UUID         ARUUID;
  T_PN_ERROR_CODE         Status;

  PN_U8                   DetailsHighWordHighByte;
  PN_U8                   DetailsHighWordLowByte;
  PN_U8                   DetailsLowWordHighByte;
  PN_U8                   DetailsLowWordLowByte;

} PACK_BYTE_ALIGNMENT (T_PROFINET_LOG_ENTRY);

/*---------------------------------------------------------------------------*/

typedef struct _T_PROFINET_LOG_DATA
{
  T_PROFINET_BLOCK_HEADER   Header;

  T_PROFINET_TIMESTAMP      CurrentTimeStamp;

  PN_U8                     NumberOfEntriesHighByte;
  PN_U8                     NumberOfEntriesLowByte;

/*
  T_PROFINET_LOG_ENTRY      Data [NumberOfEntries];
*/

} PACK_BYTE_ALIGNMENT (T_PROFINET_LOG_DATA);

/*===========================================================================*/

typedef struct _T_PROFINET_PD_PEER_PORT
{
  PN_U8   LengthPeerPortId;
/*
  PN_U8   PeerPortId [LengthPeerPortId];
*/

} PACK_BYTE_ALIGNMENT (T_PROFINET_PD_PEER_PORT);

/*---------------------------------------------------------------------------*/

typedef struct _T_PROFINET_PD_PEER_CHASSIS
{
  PN_U8   LengthPeerChassisId;
/*
  PN_U8   PeerChassisId [LengthPeerChassisId];
*/

} PACK_BYTE_ALIGNMENT (T_PROFINET_PD_PEER_CHASSIS);

/*---------------------------------------------------------------------------*/

#define PROFINET_CABLE_DELAY_FORMAT_INDICATOR       0x80000000uL

typedef struct _T_PROFINET_PD_LINE_DELAY
{
  PN_U8   DelayHighWordHighByte;
  PN_U8   DelayHighWordLowByte;
  PN_U8   DelayLowWordHighByte;
  PN_U8   DelayLowWordLowByte;

} PACK_BYTE_ALIGNMENT (T_PROFINET_PD_LINE_DELAY);

/*---------------------------------------------------------------------------*/

typedef struct _T_PROFINET_PD_PEER_REAL
{
  T_PROFINET_PD_PEER_PORT       PeerPort;
/*
  T_PROFINET_PD_PEER_CHASSIS    PeerChassis;

  T_PROFINET_PD_LINE_DELAY      Line;
  T_PN_MAC_ADDRESS              MacAddress;
*/

} PACK_BYTE_ALIGNMENT (T_PROFINET_PD_PEER_REAL);

/*---------------------------------------------------------------------------*/

typedef struct _T_PROFINET_PD_PEERS_REAL
{
  PN_U8                     NumberOfPeers;
/*
  PN_U8                     Padding []; align to 32-Bit

  T_PROFINET_PD_PEER_REAL   PeerReal [NumberOfPeers];
  PN_U8                     Padding []; align to 32-Bit
*/

} PACK_BYTE_ALIGNMENT (T_PROFINET_PD_PEERS_REAL);

/*---------------------------------------------------------------------------*/

typedef struct _T_PROFINET_PD_MAU_TYPE
{
  PN_U8   TypeHighByte;
  PN_U8   TypeLowByte;

} PACK_BYTE_ALIGNMENT (T_PROFINET_PD_MAU_TYPE);

/*---------------------------------------------------------------------------*/

typedef struct _T_PROFINET_PD_LINK_STATE
{
  PN_U8   StateHighByte;
  PN_U8   StateLowByte;

} PACK_BYTE_ALIGNMENT (T_PROFINET_PD_LINK_STATE);

/*---------------------------------------------------------------------------*/

#define PROFINET_MEDIA_TYPE_UNKNOWN             (PN_U32) 0x00000000uL
#define PROFINET_MEDIA_TYPE_COPPER_CABLE        (PN_U32) 0x00000001uL
#define PROFINET_MEDIA_TYPE_FIBER_OPTIC_CABLE   (PN_U32) 0x00000002uL
#define PROFINET_MEDIA_TYPE_RADIO               (PN_U32) 0x00000003uL

typedef struct _T_PROFINET_PD_MEDIA_TYPE
{
  PN_U8   TypeHighWordHighByte;
  PN_U8   TypeHighWordLowByte;
  PN_U8   TypeLowWordHighByte;
  PN_U8   TypeLowWordLowByte;

} PACK_BYTE_ALIGNMENT (T_PROFINET_PD_MEDIA_TYPE);

/*---------------------------------------------------------------------------*/

#define PROFINET_RTC3_STATUS_OFF                (PN_U16) 0x0000uL
#define PROFINET_RTC3_STATUS_UP                 (PN_U16) 0x0002uL
#define PROFINET_RTC3_STATUS_RUN                (PN_U16) 0x0004uL
#define PROFINET_RTC3_STATUS_MASK_STATE         (PN_U16) (PROFINET_RTC3_STATUS_OFF | \
                                                         PROFINET_RTC3_STATUS_UP   | \
                                                         PROFINET_RTC3_STATUS_RUN)
#define PROFINET_RTC3_STATUS_FRAG_OFF           (PN_U16) 0x0000uL
#define PROFINET_RTC3_STATUS_FRAG_ON            (PN_U16) 0x1000uL
#define PROFINET_RTC3_STATUS_SHORTEN_PREAMBLE   (PN_U16) 0x2000uL
#define PROFINET_RTC3_STATUS_OPTIMIZED          (PN_U16) 0x8000uL

typedef struct _T_PROFINET_PD_PORT_SETTINGS
{
  T_PROFINET_PD_MAU_TYPE      MAU;
  PN_U8                       Padding1 [2]; /* align to 32-Bit */

  PN_U8                       Reserved [2];
  PN_U8                       RTC3StatusHighByte;
  PN_U8                       RTC3StatusLowByte;

  PN_U8                       MulticastBoundaryHighWordHighByte;
  PN_U8                       MulticastBoundaryHighWordLowByte;
  PN_U8                       MulticastBoundaryLowWordHighByte;
  PN_U8                       MulticastBoundaryLowWordLowByte;

  T_PROFINET_PD_LINK_STATE    Link;

  PN_U8                       Padding2 [2]; /* align to 32-Bit */

  T_PROFINET_PD_MEDIA_TYPE    Media;

} PACK_BYTE_ALIGNMENT (T_PROFINET_PD_PORT_SETTINGS);

/*---------------------------------------------------------------------------*/

typedef struct _T_PROFINET_PD_PORT_DATA_REAL
{
  T_PROFINET_BLOCK_HEADER       Header;

  PN_U8                         Padding [2];

  PN_U8                         SlotNumberHighByte;
  PN_U8                         SlotNumberLowByte;

  PN_U8                         SubSlotNumberHighByte;
  PN_U8                         SubSlotNumberLowByte;

  PN_U8                         LengthPortId;
/*
  PN_U8                         PortId [LengthPortId];

  T_PROFINET_PD_PEERS_REAL      Peers;

  T_PROFINET_PD_PORT_SETTINGS   PortSettings;
*/

} PACK_BYTE_ALIGNMENT (T_PROFINET_PD_PORT_DATA_REAL);

/*===========================================================================*/

typedef T_PN_DEVICE_IP_INFO       T_PROFINET_IP_PARAMETER;

typedef struct _T_PROFINET_PD_INTERFACE_DATA_REAL
{
  T_PROFINET_BLOCK_HEADER   Header;

  PN_U8                     LengthChassisId;
/*
  PN_U8                     ChassisId [LengthChassisId];
  PN_U8                     Padding []; align to 32-Bit

  T_PN_MAC_ADDRESS          MACAddress;
  PN_U8                     Padding []; align to 32-Bit

  T_PROFINET_IP_PARAMETER   IPParameter;
  PN_U8                     Padding []; align to 32-Bit
*/

} PACK_BYTE_ALIGNMENT (T_PROFINET_PD_INTERFACE_DATA_REAL);

/*===========================================================================*/

typedef struct _T_PROFINET_PD_PEER_CHECK_DATA
{
  T_PROFINET_PD_PEER_PORT       PeerPort;
/*
  T_PROFINET_PD_PEER_CHASSIS    PeerChassis;
*/

} PACK_BYTE_ALIGNMENT (T_PROFINET_PD_PEER_CHECK_DATA);

/*---------------------------------------------------------------------------*/

typedef struct _T_PROFINET_PD_PEER_CHECK
{
  T_PROFINET_BLOCK_HEADER         Header;

  PN_U8                           NumberOfPeers;

/*
  T_PROFINET_PD_PEER_CHECK_DATA   PeerCheckData [NumberOfPeers];

  PN_U8                           Alignment []; align to 32-Bit
*/
} PACK_BYTE_ALIGNMENT (T_PROFINET_PD_PEER_CHECK);

/*---------------------------------------------------------------------------*/

typedef struct _T_PROFINET_PD_LINE_DELAY_CHECK
{
  T_PROFINET_BLOCK_HEADER     Header;

  PN_U8                       Padding [2];

  T_PROFINET_PD_LINE_DELAY    Line;

} PACK_BYTE_ALIGNMENT (T_PROFINET_PD_LINE_DELAY_CHECK);

/*---------------------------------------------------------------------------*/

typedef struct _T_PROFINET_PD_MAU_TYPE_CHECK
{
  T_PROFINET_BLOCK_HEADER   Header;

  T_PROFINET_PD_MAU_TYPE    MAUType;

} PACK_BYTE_ALIGNMENT (T_PROFINET_PD_MAU_TYPE_CHECK);

/*---------------------------------------------------------------------------*/

typedef struct _T_PROFINET_PD_LINK_STATE_CHECK
{
  T_PROFINET_BLOCK_HEADER   Header;

 T_PROFINET_PD_LINK_STATE   Link;

} PACK_BYTE_ALIGNMENT (T_PROFINET_PD_LINK_STATE_CHECK);

/*---------------------------------------------------------------------------*/

#define PROFINET_DISABLE_CABLE_DELAY_CHECK    (PN_U16) 0x0000u
#define PROFINET_ENABLE_CABLE_DELAY_CHECK     (PN_U16) 0x0001u
#define PROFINET_DISABLE_SYNC_MASTER_CHECK    (PN_U16) 0x0000u
#define PROFINET_ENABLE_SYNC_MASTER_CHECK     (PN_U16) 0x0002u

typedef struct _T_PROFINET_PD_SYNC_DIFF_CHECK
{
  T_PROFINET_BLOCK_HEADER   Header;

  PN_U8                     SyncModeHighByte;
  PN_U8                     SyncModeLowByte;

} PACK_BYTE_ALIGNMENT (T_PROFINET_PD_SYNC_DIFF_CHECK);

/*---------------------------------------------------------------------------*/

#define PROFINET_DISABLE_MAU_TYPE_CHECK   (PN_U16) 0x0000u
#define PROFINET_ENABLE_MAU_TYPE_CHECK    (PN_U16) 0x0001u

typedef struct _T_PROFINET_PD_MAU_TYPE_DIFF_CHECK
{
  T_PROFINET_BLOCK_HEADER   Header;

  PN_U8                     MAUTypeModeHighByte;
  PN_U8                     MAUTypeModeLowByte;

} PACK_BYTE_ALIGNMENT (T_PROFINET_PD_MAU_TYPE_DIFF_CHECK);

/*---------------------------------------------------------------------------*/

typedef struct _T_PROFINET_PD_PORT_DATA_CHECK
{
  T_PROFINET_BLOCK_HEADER             Header;

  PN_U8                               Padding [2];

  PN_U8                               SlotNumberHighByte;
  PN_U8                               SlotNumberLowByte;

  PN_U8                               SubSlotNumberHighByte;
  PN_U8                               SubSlotNumberLowByte;

/*
  T_PROFINET_PD_PEER_CHECK            PeerCheck;
  T_PROFINET_PD_LINE_DELAY_CHECK      LineDelayCheck;
  T_PROFINET_PD_MAU_TYPE_CHECK        MAUTypeCheck;
  T_PROFINET_PD_LINK_STATE_CHECK      LinkStateCheck;
  T_PROFINET_PD_SYNC_DIFF_CHECK       SyncDiffCheck;
  T_PROFINET_PD_MAU_TYPE_DIFF_CHECK   MAUTypeDiffCheck;
*/

} PACK_BYTE_ALIGNMENT (T_PROFINET_PD_PORT_DATA_CHECK);

/*===========================================================================*/

typedef struct _T_PROFINET_PD_PORT_STATISTIC
{
  T_PROFINET_BLOCK_HEADER   Header;

  PN_U8                     Padding [2];

  PN_U8                     RxBytesHighWordHighByte;
  PN_U8                     RxBytesHighWordLowByte;
  PN_U8                     RxBytesLowWordHighByte;
  PN_U8                     RxBytesLowWordLowByte;

  PN_U8                     TxBytesHighWordHighByte;
  PN_U8                     TxBytesHighWordLowByte;
  PN_U8                     TxBytesLowWordHighByte;
  PN_U8                     TxBytesLowWordLowByte;

  PN_U8                     RxDiscardsHighWordHighByte;
  PN_U8                     RxDiscardsHighWordLowByte;
  PN_U8                     RxDiscardsLowWordHighByte;
  PN_U8                     RxDiscardsLowWordLowByte;

  PN_U8                     TxDiscardsHighWordHighByte;
  PN_U8                     TxDiscardsHighWordLowByte;
  PN_U8                     TxDiscardsLowWordHighByte;
  PN_U8                     TxDiscardsLowWordLowByte;

  PN_U8                     RxErrorsHighWordHighByte;
  PN_U8                     RxErrorsHighWordLowByte;
  PN_U8                     RxErrorsLowWordHighByte;
  PN_U8                     RxErrorsLowWordLowByte;

  PN_U8                     TxErrorsHighWordHighByte;
  PN_U8                     TxErrorsHighWordLowByte;
  PN_U8                     TxErrorsLowWordHighByte;
  PN_U8                     TxErrorsLowWordLowByte;

} PACK_BYTE_ALIGNMENT (T_PROFINET_PD_PORT_STATISTIC);

/*===========================================================================*/

typedef struct _T_PROFINET_NC_DATA_CHECK
{
  T_PROFINET_BLOCK_HEADER   Header;

  PN_U8                     Padding [2];

  PN_U8                     RequiredDropBudgetHighWordHighByte;
  PN_U8                     RequiredDropBudgetHighWordLowByte;
  PN_U8                     RequiredDropBudgetLowWordHighByte;
  PN_U8                     RequiredDropBudgetLowWordLowByte;

  PN_U8                     DemandedDropBudgetHighWordHighByte;
  PN_U8                     DemandedDropBudgetHighWordLowByte;
  PN_U8                     DemandedDropBudgetLowWordHighByte;
  PN_U8                     DemandedDropBudgetLowWordLowByte;

  PN_U8                     ErrorDropBudgetHighWordHighByte;
  PN_U8                     ErrorDropBudgetHighWordLowByte;
  PN_U8                     ErrorDropBudgetLowWordHighByte;
  PN_U8                     ErrorDropBudgetLowWordLowByte;

} PACK_BYTE_ALIGNMENT (T_PROFINET_NC_DATA_CHECK);

/*===========================================================================*/

#define MIN_MRP_LINK_DOWNTIME                           (PN_U16) 1u
#define DEFAULT_MRP_LINK_DOWNTIME                       (PN_U16) 20u
#define MAX_MRP_LINK_DOWNTIME                           (PN_U16) 1000u
#define MIN_MRP_LINK_UPTIME                             (PN_U16) 1u
#define DEFAULT_MRP_LINK_UPTIME                         (PN_U16) 20u
#define MAX_MRP_LINK_UPTIME                             (PN_U16) 1000u
#define MIN_MRP_LINK_CHANGE_COUNT                       (PN_U16) 1u
#define DEFAULT_MRP_LINK_CHANGE_COUNT                   (PN_U16) 4u
#define MAX_MRP_LINK_CHANGE_COUNT                       (PN_U16) 5u

typedef struct _T_PROFINET_MRP_CLIENT_PARAMS
{
  T_PROFINET_BLOCK_HEADER   Header;

  PN_U8                     LinkDownTimeHighByte;
  PN_U8                     LinkDownTimeLowByte;

  PN_U8                     LinkUpTimeHighByte;
  PN_U8                     LinkUpTimeLowByte;

  PN_U8                     LinkChangeCountHighByte;
  PN_U8                     LinkChangeCountLowByte;

} PACK_BYTE_ALIGNMENT (T_PROFINET_MRP_CLIENT_PARAMS);

/*---------------------------------------------------------------------------*/

typedef struct _T_PROFINET_MRP_MANAGER_PARAMS
{
  T_PROFINET_BLOCK_HEADER   Header;

  PN_U8                     PriorityHighByte;
  PN_U8                     PriorityLowByte;

  PN_U8                     TopologyChangeTimeHighByte;
  PN_U8                     TopologyChangeTimeLowByte;

  PN_U8                     LinkChangeCountHighByte;
  PN_U8                     LinkChangeCountLowByte;

  PN_U8                     SendTestFrameShortTimeHighByte;
  PN_U8                     SendTestFrameShortTimeLowByte;

  PN_U8                     SendTestFrameDefaultTimeHighByte;
  PN_U8                     SendTestFrameDefaultTimeLowByte;

  PN_U8                     TestFrameRetryHighByte;
  PN_U8                     TestFrameRetryLowByte;

} PACK_BYTE_ALIGNMENT (T_PROFINET_MRP_MANAGER_PARAMS);

/*---------------------------------------------------------------------------*/

typedef struct _T_PROFINET_MRP_PARAMS
{
  union
  {
    T_PROFINET_MRP_CLIENT_PARAMS    Client;
    T_PROFINET_MRP_MANAGER_PARAMS   Manager;

  } UseAs;

} PACK_BYTE_ALIGNMENT (T_PROFINET_MRP_PARAMS);

/*===========================================================================*/

#define MULTIPLE_INTERFACE_MODE_LEGACY                  0x00000000uL
#define MULTIPLE_INTERFACE_MODE_LOCAL_CHASSIS_ID        0x00000001uL

typedef struct _T_PROFINET_PD_INTERFACE_DATA_ADJUST
{
  T_PROFINET_BLOCK_HEADER   Header;

  PN_U8                     Padding [2];

  PN_U8                     MultipleInterfaceModeHighWordHighByte;
  PN_U8                     MultipleInterfaceModeHighWordLowByte;
  PN_U8                     MultipleInterfaceModeLowWordHighByte;
  PN_U8                     MultipleInterfaceModeLowWordLowByte;


} PACK_BYTE_ALIGNMENT (T_PROFINET_PD_INTERFACE_DATA_ADJUST);

/*===========================================================================*/

typedef struct _T_PROFINET_PD_PORT_FO_ADJUST
{
  T_PROFINET_BLOCK_HEADER   Header;

  /* TODO: define structure */

} PACK_BYTE_ALIGNMENT (T_PROFINET_PD_PORT_FO_ADJUST);

/*===========================================================================*/

typedef struct _T_PROFINET_DOMAIN_BOUNDARY_ADJUST
{
  T_PROFINET_BLOCK_HEADER   Header;

  PN_U8                     Padding1 [2];

  PN_U8                     IngressBoundaryHighWordHighByte;
  PN_U8                     IngressBoundaryHighWordLowByte;
  PN_U8                     IngressBoundaryLowWordHighByte;
  PN_U8                     IngressBoundaryLowWordLowByte;

  PN_U8                     EgressBoundaryHighWordHighByte;
  PN_U8                     EgressBoundaryHighWordLowByte;
  PN_U8                     EgressBoundaryLowWordHighByte;
  PN_U8                     EgressBoundaryLowWordLowByte;

  PN_U8                     PropertiesHighByte;
  PN_U8                     PropertiesLowByte;

  PN_U8                     Padding2 [2];

} T_PROFINET_DOMAIN_BOUNDARY_ADJUST;

/*---------------------------------------------------------------------------*/

typedef struct _T_PROFINET_MULTICAST_BOUNDARY_ADJUST
{
  T_PROFINET_BLOCK_HEADER   Header;

  PN_U8                     Padding1 [2];

  PN_U8                     BoundaryHighWordHighByte;
  PN_U8                     BoundaryHighWordLowByte;
  PN_U8                     BoundaryLowWordHighByte;
  PN_U8                     BoundaryLowWordLowByte;

  PN_U8                     PropertiesHighByte;
  PN_U8                     PropertiesLowByte;

  PN_U8                     Padding2 [2];

} T_PROFINET_MULTICAST_BOUNDARY_ADJUST;

typedef T_PROFINET_MULTICAST_BOUNDARY_ADJUST   T_PROFINET_PEER2PEER_BOUNDARY_ADJUST;
typedef T_PROFINET_MULTICAST_BOUNDARY_ADJUST   T_PROFINET_DCP_BOUNDARY_ADJUST;

/*---------------------------------------------------------------------------*/

typedef struct _T_PROFINET_MAU_TYPE_ADJUST
{
  T_PROFINET_BLOCK_HEADER   Header;

  PN_U8                     Padding [2];

  PN_U8                     TypeHighByte;
  PN_U8                     TypeLowByte;

  PN_U8                     PropertiesHighByte;
  PN_U8                     PropertiesLowByte;

} T_PROFINET_MAU_TYPE_ADJUST;

/*---------------------------------------------------------------------------*/

typedef struct _T_PROFINET_LINK_STATE_ADJUST
{
  T_PROFINET_BLOCK_HEADER   Header;

  PN_U8                     Padding [2];

  PN_U8                     StateHighByte;
  PN_U8                     StateLowByte;

  PN_U8                     PropertiesHighByte;
  PN_U8                     PropertiesLowByte;

} T_PROFINET_LINK_STATE_ADJUST;

/*---------------------------------------------------------------------------*/

#define PROFINET_ADJUST_SHORTEN_PREAMBLE        (PN_U16) 0x0001uL

typedef struct _T_PROFINET_PREAMBLE_LENGTH_ADJUST
{
  T_PROFINET_BLOCK_HEADER   Header;

  PN_U8                     Padding [2];

  PN_U8                     LengthHighByte;
  PN_U8                     LengthLowByte;

  PN_U8                     PropertiesHighByte;
  PN_U8                     PropertiesLowByte;

} T_PROFINET_PREAMBLE_LENGTH_ADJUST;

/*---------------------------------------------------------------------------*/

typedef struct _T_PROFINET_PD_PORT_DATA_ADJUST
{
  T_PROFINET_BLOCK_HEADER                 Header;

  PN_U8                                   Padding [2];

  PN_U8                                   SlotNumberHighByte;
  PN_U8                                   SlotNumberLowByte;

  PN_U8                                   SubSlotNumberHighByte;
  PN_U8                                   SubSlotNumberLowByte;

/*
  T_PROFINET_DOMAIN_BOUNDARY_ADJUST       Domain;
  T_PROFINET_MULTICAST_BOUNDARY_ADJUST    Multicast;
  T_PROFINET_MAU_TYPE_ADJUST              MAU;
  T_PROFINET_LINK_STATE_ADJUST            Link;
  T_PROFINET_PEER2PEER_BOUNDARY_ADJUST    Peer2Peer;
  T_PROFINET_DCP_BOUNDARY_ADJUST          DCP;
  T_PROFINET_PREAMBLE_LENGTH_ADJUST       Preamble;
*/

} PACK_BYTE_ALIGNMENT (T_PROFINET_PD_PORT_DATA_ADJUST);

/*===========================================================================*/

#define MAX_MRP_DOMAIN_NAME_LENGTH                    PN_MAX_DEVICE_NAME_LENGTH

typedef struct _T_PROFINET_PD_INTERFACE_MRP_DATA_ADJUST
{
  T_PROFINET_BLOCK_HEADER     Header;

  PN_U8                       Padding1 [2];

  T_MRP_UUID                  DomainUUID;

  PN_U8                       RoleHighByte;
  PN_U8                       RoleLowByte;

  PN_U8                       Padding2 [2];

  PN_U8                       NameLength;
/*
  PN_U8                       MrpDomainName [NameLength];
  PN_U8                       Alignment [];
  T_PROFINET_MRP_PARAMS       Parameter;
*/

} PACK_BYTE_ALIGNMENT (T_PROFINET_PD_INTERFACE_MRP_DATA_ADJUST);

/*===========================================================================*/

typedef struct _T_PROFINET_MRP_RING_STATE
{
  T_PROFINET_BLOCK_HEADER   Header;

  PN_U8                     StateHighByte;
  PN_U8                     StateLowByte;

} PACK_BYTE_ALIGNMENT (T_PROFINET_MRP_RING_STATE);

/*===========================================================================*/

typedef struct _T_PROFINET_PD_INTERFACE_MRP_DATA_REAL
{
  T_PROFINET_BLOCK_HEADER     Header;

  PN_U8                       Padding [2];

  T_MRP_UUID                  DomainUUID;

  PN_U8                       RoleHighByte;
  PN_U8                       RoleLowByte;

  PN_U8                       VersionHighByte;
  PN_U8                       VersionLowByte;

  PN_U8                       NameLength;
/*
  PN_U8                       MrpDomainName [NameLength];
  PN_U8                       Alignment [];
  T_PROFINET_MRP_PARAMETER    Parameter;
  T_PROFINET_MRP_RING_STATE   Ring;
*/

} PACK_BYTE_ALIGNMENT (T_PROFINET_PD_INTERFACE_MRP_DATA_REAL);

/*===========================================================================*/

#define MRP_CHECK_REDUNDANCY_MANAGER_DISABLE            (PN_U32) 0x00000000
#define MRP_CHECK_REDUNDANCY_MANAGER_ENABLE             (PN_U32) 0x00000001
#define MRP_CHECK_DOMAIN_UUID_DISABLE                   (PN_U32) 0x00000000
#define MRP_CHECK_DOMAIN_UUID_ENABLE                    (PN_U32) 0x00000002

typedef struct _T_PROFINET_PD_INTERFACE_MRP_DATA_CHECK
{
  T_PROFINET_BLOCK_HEADER   Header;

  PN_U8                     Padding [2];

  T_MRP_UUID                DomainUUID;

  PN_U8                     CheckHighWordHighByte;
  PN_U8                     CheckHighWordLowByte;
  PN_U8                     CheckLowWordHighByte;
  PN_U8                     CheckLowWordLowByte;

} PACK_BYTE_ALIGNMENT (T_PROFINET_PD_INTERFACE_MRP_DATA_CHECK);

/*===========================================================================*/

typedef struct _T_PROFINET_PD_PORT_MRP_DATA_ADJUST
{
  T_PROFINET_BLOCK_HEADER   Header;

  PN_U8                     Padding [2];

  T_MRP_UUID                DomainUUID;

} PACK_BYTE_ALIGNMENT (T_PROFINET_PD_PORT_MRP_DATA_ADJUST);

typedef T_PROFINET_PD_PORT_MRP_DATA_ADJUST    T_PROFINET_PD_PORT_MRP_DATA_REAL;

/*===========================================================================*/

#define PROFINET_MAX_LINE_RX_DELAY_UNKNOWN      0uL
#define PROFINET_YELLOW_TIME_DISABLE            0uL

typedef struct _T_PROFINET_PORT_GLOBAL_DATA_V1_1
{
  PN_U8   MaxTxDelayHighWordHighByte;
  PN_U8   MaxTxDelayHighWordLowByte;
  PN_U8   MaxTxDelayLowWordHighByte;
  PN_U8   MaxTxDelayLowWordLowByte;

  PN_U8   MaxRxDelayHighWordHighByte;
  PN_U8   MaxRxDelayHighWordLowByte;
  PN_U8   MaxRxDelayLowWordHighByte;
  PN_U8   MaxRxDelayLowWordLowByte;

} PACK_BYTE_ALIGNMENT (T_PROFINET_PORT_GLOBAL_DATA_V1_1);

/*---------------------------------------------------------------------------*/

#define PROFINET_DEFAULT_YELLOW_TIME            0x0001E848uL

typedef struct _T_PROFINET_PORT_GLOBAL_DATA_V1_2
{
  PN_U8   MaxTxDelayHighWordHighByte;
  PN_U8   MaxTxDelayHighWordLowByte;
  PN_U8   MaxTxDelayLowWordHighByte;
  PN_U8   MaxTxDelayLowWordLowByte;

  PN_U8   MaxRxDelayHighWordHighByte;
  PN_U8   MaxRxDelayHighWordLowByte;
  PN_U8   MaxRxDelayLowWordHighByte;
  PN_U8   MaxRxDelayLowWordLowByte;

  PN_U8   MaxLineRxDelayHighWordHighByte;
  PN_U8   MaxLineRxDelayHighWordLowByte;
  PN_U8   MaxLineRxDelayLowWordHighByte;
  PN_U8   MaxLineRxDelayLowWordLowByte;

  PN_U8   YellowTimeHighWordHighByte;
  PN_U8   YellowTimeHighWordLowByte;
  PN_U8   YellowTimeLowWordHighByte;
  PN_U8   YellowTimeLowWordLowByte;

} PACK_BYTE_ALIGNMENT (T_PROFINET_PORT_GLOBAL_DATA_V1_2);

/*---------------------------------------------------------------------------*/

typedef struct _T_PROFINET_PDIR_GLOBAL_DATA
{
  T_PROFINET_BLOCK_HEADER             Header;

  PN_U8                               Padding [2];

  T_PROFINET_UUID                     IRDataUUID;

  PN_U8                               MaxBridgeDelayHighWordHighByte;
  PN_U8                               MaxBridgeDelayHighWordLowByte;
  PN_U8                               MaxBridgeDelayLowWordHighByte;
  PN_U8                               MaxBridgeDelayLowWordLowByte;

  PN_U8                               NumberOfPortsHighWordHighByte;
  PN_U8                               NumberOfPortsHighWordLowByte;
  PN_U8                               NumberOfPortsLowWordHighByte;
  PN_U8                               NumberOfPortsLowWordLowByte;

 /*
  T_PROFINET_PORT_GLOBAL_DATA_V1_2    PortGlobalData [];
  or
  T_PROFINET_PORT_GLOBAL_DATA_V1_1    PortGlobalData [];
*/
} PACK_BYTE_ALIGNMENT (T_PROFINET_PDIR_GLOBAL_DATA);

/*===========================================================================*/

typedef struct _T_PROFINET_PDIR_FRAME_DATA_DETAIL
{
  PN_U8   FrameSendOffsetHighWordHighByte;
  PN_U8   FrameSendOffsetHighWordLowByte;
  PN_U8   FrameSendOffsetLowWordHighByte;
  PN_U8   FrameSendOffsetLowWordLowByte;

  PN_U8   DataLengthHighByte;
  PN_U8   DataLengthLowByte;

  PN_U8   ReductionRatioHighByte;
  PN_U8   ReductionRatioLowByte;

  PN_U8   PhaseHighByte;
  PN_U8   PhaseLowByte;

  PN_U8   FrameIdHighByte;
  PN_U8   FrameIdLowByte;

  PN_U8   EtherTypeHighByte;
  PN_U8   EtherTypeLowByte;

  PN_U8   RxPort;
  PN_U8   FrameDetails;

  PN_U8   TxPortGroup;
/*
  PN_U8   TxPortGroupArray [(NUMBER_PORTS / 8) + 1];

  PN_U8   Alignment []; align to 32-Bit
*/

} PACK_BYTE_ALIGNMENT (T_PROFINET_PDIR_FRAME_DATA_DETAIL);

/*---------------------------------------------------------------------------*/

#define PROFINET_ABSOLUTE_FORWARDING_MODE         0x00000000uL
#define PROFINET_RELATIVE_FORWARDING_MODE         0x00000001uL
#define PROFINET_FORWARDING_MODE_MASK             (PROFINET_ABSOLUTE_FORWARDING_MODE | \
                                                  PROFINET_RELATIVE_FORWARDING_MODE)
#define PROFINET_LEGACY_MULTICAST_ADDRESS         0x00000000uL
#define PROFINET_RTC3_MULTICAST_ADDRESS           0x00000002uL
#define PROFINET_FF_MULTICAST_ADDRESS             0x00000004uL
#define PROFINET_MULTICAST_ADDRESS_MASK           (PROFINET_LEGACY_MULTICAST_ADDRESS | \
                                                  PROFINET_RTC3_MULTICAST_ADDRESS    | \
                                                  PROFINET_FF_MULTICAST_ADDRESS)
#define PROFINET_FRAGMENTATION_DISABLED           0x00000000uL
#define PROFINET_MAX_128_BYTE_FRAGMENTS           0x00000008uL
#define PROFINET_MAX_256_BYTE_FRAGMENTS           0x00000010uL
#define PROFINET_FRAGMENTATION_MASK               (PROFINET_FRAGMENTATION_DISABLED | \
                                                  PROFINET_MAX_128_BYTE_FRAGMENTS | \
                                                  PROFINET_MAX_256_BYTE_FRAGMENTS)
#define PROFINET_PDIR_FRAME_PROPERTY_MASK         0x0000FFFFuL

/*---------------------------------------------------------------------------*/

typedef struct _T_PROFINET_PDIR_FRAME_DATA_V1_0
{
  T_PROFINET_BLOCK_HEADER             Header;

  PN_U8                               Padding [2];

 /*
  T_PROFINET_PDIR_FRAME_DATA_DETAIL   Detail [];
 */

} PACK_BYTE_ALIGNMENT (T_PROFINET_PDIR_FRAME_DATA_V1_0);

/*---------------------------------------------------------------------------*/

typedef struct _T_PROFINET_PDIR_FRAME_DATA_V1_1
{
  T_PROFINET_BLOCK_HEADER             Header;

  PN_U8                               Padding [2];

  PN_U8                               PropertiesHighWordHighByte;
  PN_U8                               PropertiesHighWordLowByte;
  PN_U8                               PropertiesLowWordHighByte;
  PN_U8                               PropertiesLowWordLowByte;

 /*
  T_PROFINET_PDIR_FRAME_DATA_DETAIL   Detail [];
 */

} PACK_BYTE_ALIGNMENT (T_PROFINET_PDIR_FRAME_DATA_V1_1);

/*===========================================================================*/

typedef struct _T_PROFINET_RED_GUARD
{
  PN_U8   StartOfRedFrameIdHighByte;
  PN_U8   StartOfRedFrameIdLowByte;

  PN_U8   EndOfRedFrameIdHighByte;
  PN_U8   EndOfRedFrameIdLowByte;

} PACK_BYTE_ALIGNMENT (T_PROFINET_RED_GUARD);

/*---------------------------------------------------------------------------*/

typedef struct _T_PROFINET_PDIR_PERIOD_ASSIGNMENT
{
  PN_U8   RedOrangeHighWordHighByte;
  PN_U8   RedOrangeHighWordLowByte;
  PN_U8   RedOrangeLowWordHighByte;
  PN_U8   RedOrangeLowWordLowByte;

  PN_U8   OrangeHighWordHighByte;
  PN_U8   OrangeHighWordLowByte;
  PN_U8   OrangeLowWordHighByte;
  PN_U8   OrangeLowWordLowByte;

  PN_U8   GreenHighWordHighByte;
  PN_U8   GreenHighWordLowByte;
  PN_U8   GreenLowWordHighByte;
  PN_U8   GreenLowWordLowByte;

} PACK_BYTE_ALIGNMENT (T_PROFINET_PDIR_PERIOD_ASSIGNMENT);

/*---------------------------------------------------------------------------*/

typedef struct _T_PROFINET_PDIR_PHASE
{
  PN_U8   AssignmentHighByte;
  PN_U8   AssignmentLowByte;

} PACK_BYTE_ALIGNMENT (T_PROFINET_PDIR_PHASE);

/*---------------------------------------------------------------------------*/

typedef struct _T_PROFINET_PDIR_PHASE_ASSIGNMENT
{
  T_PROFINET_PDIR_PHASE   Tx;
  T_PROFINET_PDIR_PHASE   Rx;

} PACK_BYTE_ALIGNMENT (T_PROFINET_PDIR_PHASE_ASSIGNMENT);

/*---------------------------------------------------------------------------*/

typedef struct _T_PROFINET_PDIR_PORT_PHASE_DATA
{
  PN_U8                               NumberOfPhasesHighWordHighByte;
  PN_U8                               NumberOfPhasesHighWordLowByte;
  PN_U8                               NumberOfPhasesLowWordHighByte;
  PN_U8                               NumberOfPhasesLowWordLowByte;

 /*
  T_PROFINET_PDIR_PHASE_ASSIGNMENT    Phase [NumberOfPhases];
 */

} PACK_BYTE_ALIGNMENT (T_PROFINET_PDIR_PORT_PHASE_DATA);

/*---------------------------------------------------------------------------*/

typedef struct _T_PROFINET_PDIR_BEGIN_END_ASSIGNMENT
{
  T_PROFINET_PDIR_PERIOD_ASSIGNMENT   Tx;
  T_PROFINET_PDIR_PERIOD_ASSIGNMENT   Rx;

} PACK_BYTE_ALIGNMENT (T_PROFINET_PDIR_BEGIN_END_ASSIGNMENT);

/*---------------------------------------------------------------------------*/

#define MAX_NUMBER_PDIR_BEGIN_END_ASSIGNMENTS         0x00000010uL

typedef struct _T_PROFINET_PDIR_PORT_BEGIN_END_DATA
{
  PN_U8                                   NumberOfAssignmentsHighWordHighByte;
  PN_U8                                   NumberOfAssignmentsHighWordLowByte;
  PN_U8                                   NumberOfAssignmentsLowWordHighByte;
  PN_U8                                   NumberOfAssignmentsLowWordLowByte;

 /*
  T_PROFINET_PDIR_BEGIN_END_ASSIGNMENT    Period [NumberOfAssignments];
 */

} PACK_BYTE_ALIGNMENT (T_PROFINET_PDIR_PORT_BEGIN_END_DATA);

/*---------------------------------------------------------------------------*/

typedef struct _T_PROFINET_PDIR_BEGIN_END_DATA
{
  T_PROFINET_BLOCK_HEADER               Header;

  PN_U8                                 Padding [2];

  T_PROFINET_RED_GUARD                  RedGuard;

  PN_U8                                 NumberOfPortsHighWordHighByte;
  PN_U8                                 NumberOfPortsHighWordLowByte;
  PN_U8                                 NumberOfPortsLowWordHighByte;
  PN_U8                                 NumberOfPortsLowWordLowByte;

 /*
  T_PROFINET_PDIR_PORT_BEGIN_END_DATA   PortAssignment [NumberOfPorts];
  T_PROFINET_PDIR_PORT_PHASE_DATA       PortPhase [NumberOfPorts];
 */

} PACK_BYTE_ALIGNMENT (T_PROFINET_PDIR_BEGIN_END_DATA);

/*===========================================================================*/

typedef struct _T_PROFINET_PDIR_DATA
{
  T_PROFINET_BLOCK_HEADER           Header;

  PN_U8                             Padding [2];

  PN_U8                             SlotNumberHighByte;
  PN_U8                             SlotNumberLowByte;

  PN_U8                             SubSlotNumberHighByte;
  PN_U8                             SubSlotNumberLowByte;

/*
  T_PROFINET_PDIR_GLOBAL_DATA       PDIRGlobalData;
  T_PROFINET_PDIR_FRAME_DATA        PDIRFrameData;
  T_PROFINET_PDIR_BEGIN_END_DATA    PDIRBeginEndData;
*/

} PACK_BYTE_ALIGNMENT (T_PROFINET_PDIR_DATA);

/*===========================================================================*/

#define PROFINET_PLL_WINDOW_1_US        1000uL
#define PROFINET_PLL_WINDOW_10_US       10000uL
#define PROFINET_PLL_WINDOW_1_MS        1000000uL
#define PROFINET_PLL_WINDOW_10_MS       10000000uL

#define PROFINET_DEFAULT_PLL_WINDOW     PROFINET_PLL_WINDOW_1_US

typedef struct _T_PROFINET_PD_SYNC_DATA
{
  T_PROFINET_BLOCK_HEADER   Header;

  PN_U8                     Padding [2];

  T_PROFINET_UUID           PTCPSubDomainUUID;

  PN_U8                     ReservedIntervalBeginHighWordHighByte;
  PN_U8                     ReservedIntervalBeginHighWordLowByte;
  PN_U8                     ReservedIntervalBeginLowWordHighByte;
  PN_U8                     ReservedIntervalBeginLowWordLowByte;

  PN_U8                     ReservedIntervalEndHighWordHighByte;
  PN_U8                     ReservedIntervalEndHighWordLowByte;
  PN_U8                     ReservedIntervalEndLowWordHighByte;
  PN_U8                     ReservedIntervalEndLowWordLowByte;

  PN_U8                     PLLWindowHighWordHighByte;
  PN_U8                     PLLWindowHighWordLowByte;
  PN_U8                     PLLWindowLowWordHighByte;
  PN_U8                     PLLWindowLowWordLowByte;

  PN_U8                     SyncSendFactorHighWordHighByte;
  PN_U8                     SyncSendFactorHighWordLowByte;
  PN_U8                     SyncSendFactorLowWordHighByte;
  PN_U8                     SyncSendFactorLowWordLowByte;

  PN_U8                     SendClockFactorHighByte;
  PN_U8                     SendClockFactorLowByte;

  PN_U8                     PTCPTimeoutFactorHighByte;
  PN_U8                     PTCPTimeoutFactorLowByte;

  PN_U8                     PTCPTakeoverFactorHighByte;
  PN_U8                     PTCPTakeoverFactorLowByte;

  PN_U8                     PTCPMasterStartupTimeHighByte;
  PN_U8                     PTCPMasterStartupTimeLowByte;

  PN_U8                     SyncPropertiesHighByte;
  PN_U8                     SyncPropertiesLowByte;

  PN_U8                     PTCPMasterPrio1;
  PN_U8                     PTCPMasterPrio2;

  PN_U8                     PTCPSubDomainNameLength;

/*
  PN_U8                     PTCPSubDomainName [PTCPSubDomainNameLength];
  PN_U8                     Alignment []; align to 32-Bit
*/

} PACK_BYTE_ALIGNMENT (T_PROFINET_PD_SYNC_DATA);

/*===========================================================================*/

typedef struct _T_PROFINET_SUBFRAME_BLOCK
{
  T_PROFINET_BLOCK_HEADER   Header;

  PN_U8                     FrameIdHighByte;
  PN_U8                     FrameIdLowByte;

  PN_U8                     SFIOCRPropertiesHighWordHighByte;
  PN_U8                     SFIOCRPropertiesHighWordLowByte;
  PN_U8                     SFIOCRPropertiesLowWordHighByte;
  PN_U8                     SFIOCRPropertiesLowWordLowByte;

  PN_U8                     SubframeDataHighWordHighByte;
  PN_U8                     SubframeDataHighWordLowByte;
  PN_U8                     SubframeDataLowWordHighByte;
  PN_U8                     SubframeDataLowWordLowByte;

} PACK_BYTE_ALIGNMENT (T_PROFINET_SUBFRAME_BLOCK);

/*---------------------------------------------------------------------------*/

#define PROFINET_MAX_NUMBER_SUBFRAME_BLOCKS         255u

typedef struct _T_PROFINET_PDIR_SUBFRAME_DATA
{
  T_PROFINET_BLOCK_HEADER     Header;

  PN_U8                       NumberOfBlocksHighByte;
  PN_U8                       NumberOfBlocksLowByte;

/*
  T_PROFINET_SUBFRAME_BLOCK   Block [NumberOfBlocks];
*/

} PACK_BYTE_ALIGNMENT (T_PROFINET_PDIR_SUBFRAME_DATA);

/*===========================================================================*/

typedef struct _T_PROFINET_PDEV_DATA
{
  T_PROFINET_BLOCK_HEADER         Header;

  PN_U8                           Padding [2];

/*
  T_PROFINET_PDIR_DATA            PDIRData;
  T_PROFINET_PD_SYNC_DATA         PDSyncData;
  T_PROFINET_PDIR_SUBFRAME_DATA   PDIRSubframeData;
*/


} PACK_BYTE_ALIGNMENT (T_PROFINET_PDEV_DATA);

/*===========================================================================*/

typedef struct _T_PROFINET_IDENT_MAINTENANCE
{
  T_PROFINET_BLOCK_HEADER   Header;

  PN_U8                     VendorIdHighByte;
  PN_U8                     VendorIdLowByte;

  PN_U8                     OrderId [PROFINET_IDENT_MAINTENANCE_ORDER_ID_LENGTH];
  PN_U8                     SerialNumber [PROFINET_IDENT_MAINTENANCE_SR_NUMBER_LENGTH];

  PN_U8                     HwRevisionHighByte;
  PN_U8                     HwRevisionLowByte;

  PN_U8                     SwRevisionHighWordHighByte;
  PN_U8                     SwRevisionHighWordLowByte;
  PN_U8                     SwRevisionLowWordHighByte;
  PN_U8                     SwRevisionLowWordLowByte;

  PN_U8                     RevisionCounterHighByte;
  PN_U8                     RevisionCounterLowByte;

  PN_U8                     ProfileIdHighByte;
  PN_U8                     ProfileIdLowByte;

  PN_U8                     ProfileTypeHighByte;
  PN_U8                     ProfileTypeLowByte;

  PN_U8                     VersionHighByte;
  PN_U8                     VersionLowByte;

  PN_U8                     SupportedHighByte;
  PN_U8                     SupportedLowByte;

} PACK_BYTE_ALIGNMENT (T_PROFINET_IDENT_MAINTENANCE);

/*---------------------------------------------------------------------------*/

typedef struct _T_PROFINET_IDENT_MAINTENANCE_1
{
  T_PROFINET_BLOCK_HEADER   Header;

  PN_U8                     Function [PROFINET_IDENT_MAINTENANCE_FUNCTION_LENGTH];
  PN_U8                     Location [PROFINET_IDENT_MAINTENANCE_LOCATION_LENGTH];

} PACK_BYTE_ALIGNMENT (T_PROFINET_IDENT_MAINTENANCE_1);

/*---------------------------------------------------------------------------*/

typedef struct _T_PROFINET_IDENT_MAINTENANCE_2
{
  T_PROFINET_BLOCK_HEADER   Header;

  PN_U8                     Date [PROFINET_IDENT_MAINTENANCE_DATE_LENGTH];

} PACK_BYTE_ALIGNMENT (T_PROFINET_IDENT_MAINTENANCE_2);

/*---------------------------------------------------------------------------*/

typedef struct _T_PROFINET_IDENT_MAINTENANCE_3
{
  T_PROFINET_BLOCK_HEADER   Header;

  PN_U8                     Description [PROFINET_IDENT_MAINTENANCE_DESCRIPTION_LENGTH];

} PACK_BYTE_ALIGNMENT (T_PROFINET_IDENT_MAINTENANCE_3);

/*---------------------------------------------------------------------------*/

typedef struct _T_PROFINET_IDENT_MAINTENANCE_4
{
  T_PROFINET_BLOCK_HEADER   Header;

  PN_U8                     Signature [PROFINET_IDENT_MAINTENANCE_SIGNATURE_LENGTH];

} PACK_BYTE_ALIGNMENT (T_PROFINET_IDENT_MAINTENANCE_4);

/*===========================================================================*/

typedef struct _T_PROFINET_MULTIPLE_HEADER
{
  T_PROFINET_BLOCK_HEADER   Header;

  PN_U8                     Padding [2];

  PN_U8                     APIHighWordHighByte;
  PN_U8                     APIHighWordLowByte;
  PN_U8                     APILowWordHighByte;
  PN_U8                     APILowWordLowByte;

  PN_U8                     SlotNumberHighByte;
  PN_U8                     SlotNumberLowByte;

  PN_U8                     SubSlotNumberHighByte;
  PN_U8                     SubSlotNumberLowByte;

} PACK_BYTE_ALIGNMENT (T_PROFINET_MULTIPLE_HEADER);

typedef T_PROFINET_MULTIPLE_HEADER    T_PROFINET_PD_REAL_DATA;
typedef T_PROFINET_MULTIPLE_HEADER    T_PROFINET_PD_EXPECTED_DATA;

/*===========================================================================*/

typedef struct _T_PROFINET_RECORD_DIAGNOSIS
{
  T_PROFINET_BLOCK_HEADER       Header;

  PN_U8                         APIHighWordHighByte;
  PN_U8                         APIHighWordLowByte;
  PN_U8                         APILowWordHighByte;
  PN_U8                         APILowWordLowByte;

  PN_U8                         SlotNumberHighByte;
  PN_U8                         SlotNumberLowByte;

  PN_U8                         SubSlotNumberHighByte;
  PN_U8                         SubSlotNumberLowByte;

  PN_U8                         ChannelNumberHighByte;
  PN_U8                         ChannelNumberLowByte;

  PN_U8                         PropertiesHighByte;
  PN_U8                         PropertiesLowByte;

/*
  union
  {
    T_PROFINET_CHANNEL_DIAG             ChannelDiag;
    T_PROFINET_EXT_CHANNEL_DIAG         ExtChannelDiag;
    T_PROFINET_QUALIFIED_CHANNEL_DIAG   QualifiedChannelDiag;
    PN_U8                               ManufacturerDiag;
  }
*/

} PACK_BYTE_ALIGNMENT (T_PROFINET_RECORD_DIAGNOSIS);

/*===========================================================================*/

#define C_CONTROL_COMMAND_PRM_END             0x0001u
#define C_CONTROL_COMMAND_APPLICATION_READY   0x0002u
#define C_CONTROL_COMMAND_RELEASE             0x0004u
#define C_CONTROL_COMMAND_READYFORCOMPANION   0x0010u
#define C_CONTROL_COMMAND_READYFORRTCLASS3    0x0020u

typedef struct _T_PN_SERVICE_CONTROL_IND
{
  PN_U8   ARIdentifierHighByte;
  PN_U8   ARIdentifierLowByte;

  PN_U8   CommandHighByte;
  PN_U8   CommandLowByte;

} PACK_BYTE_ALIGNMENT (T_PN_SERVICE_CONTROL_IND);

/*---------------------------------------------------------------------------*/

typedef struct _T_PN_SERVICE_CONTROL_RES
{
  PN_U8               ARIdentifierHighByte;
  PN_U8               ARIdentifierLowByte;

  PN_U8               CommandHighByte;
  PN_U8               CommandLowByte;

  PN_U8               NumberOfDiffModulesHighByte;
  PN_U8               NumberOfDiffModulesLowByte;

/*
  T_PN_DIFF_MODULE    DiffModule [NumberOfModules];
*/

} PACK_BYTE_ALIGNMENT (T_PN_SERVICE_CONTROL_RES);

/*===========================================================================*/

typedef struct _T_PN_SERVICE_WRITE_MULTIPLE_IND
{
  PN_U8                     ARIdentifierHighByte;
  PN_U8                     ARIdentifierLowByte;

  PN_U8                     NumberOfBlocksHighByte;
  PN_U8                     NumberOfBlocksLowByte;

/*
  T_PN_SERVICE_WRITE_IND    Write [NumberOfBlocks];
*/

} PACK_BYTE_ALIGNMENT (T_PN_SERVICE_WRITE_MULTIPLE_IND);

/*---------------------------------------------------------------------------*/

typedef struct _T_PN_SERVICE_WRITE_MULTIPLE_REQ
{
  PN_U8                     NumberOfBlocksHighByte;
  PN_U8                     NumberOfBlocksLowByte;

/*
  T_PN_SERVICE_WRITE_REQ    Write [NumberOfBlocks];
*/

} PACK_BYTE_ALIGNMENT (T_PN_SERVICE_WRITE_MULTIPLE_REQ);

/*---------------------------------------------------------------------------*/

typedef struct _T_WRITE_MULTIPLE_RES
{
  PN_U8   ARIdentifierHighByte;
  PN_U8   ARIdentifierLowByte;

  PN_U8   APIHighWordHighByte;
  PN_U8   APIHighWordLowByte;
  PN_U8   APILowWordHighByte;
  PN_U8   APILowWordLowByte;

  PN_U8   SlotNumberHighByte;
  PN_U8   SlotNumberLowByte;

  PN_U8   SubSlotNumberHighByte;
  PN_U8   SubSlotNumberLowByte;

  PN_U8   IndexHighByte;
  PN_U8   IndexLowByte;

  PN_U8   ErrorCode1;
  PN_U8   ErrorCode2;

} PACK_BYTE_ALIGNMENT (T_WRITE_MULTIPLE_RES);

/*---------------------------------------------------------------------------*/

typedef struct _T_PN_SERVICE_WRITE_MULTIPLE_RES
{
  PN_U8                   ARIdentifierHighByte;
  PN_U8                   ARIdentifierLowByte;

  PN_U8                   NumberOfBlocksHighByte;
  PN_U8                   NumberOfBlocksLowByte;

/*
  T_WRITE_MULTIPLE_RES    Write [NumberOfBlocks];
*/

} PACK_BYTE_ALIGNMENT (T_PN_SERVICE_WRITE_MULTIPLE_RES);

/*---------------------------------------------------------------------------*/

typedef T_WRITE_MULTIPLE_RES        T_WRITE_MULTIPLE_CON;

/*---------------------------------------------------------------------------*/

typedef struct _T_PN_SERVICE_WRITE_MULTIPLE_CON
{
  PN_U8                   NumberOfBlocksHighByte;
  PN_U8                   NumberOfBlocksLowByte;

/*
  T_WRITE_MULTIPLE_CON    Write [NumberOfBlocks];
*/

} PACK_BYTE_ALIGNMENT (T_PN_SERVICE_WRITE_MULTIPLE_CON);

/*===========================================================================*/

#define MAX_NUMBER_MISMATCH_ENTRIES     ((MAX_NUMBER_LOCAL_MISMATCH_ENTRIES > MAX_NUMBER_PEER_MISMATCH_ENTRIES) ? \
                                         MAX_NUMBER_LOCAL_MISMATCH_ENTRIES : MAX_NUMBER_PEER_MISMATCH_ENTRIES)

typedef struct _T_PN_SERVICE_LOCAL_AND_PEER_CHECK_IND
{
  PN_U8                         ARIdentifierHighByte;
  PN_U8                         ARIdentifierLowByte;

  PN_U8                         AlarmTypeHighByte;
  PN_U8                         AlarmTypeLowByte;

  PN_U8                         SlotNumberHighByte;
  PN_U8                         SlotNumberLowByte;

  PN_U8                         SubSlotNumberHighByte;
  PN_U8                         SubSlotNumberLowByte;

  PN_U8                         NumberEntriesHighByte;
  PN_U8                         NumberEntriesLowByte;

  T_PROFINET_EXT_CHANNEL_DIAG   ExtChannelDiag [MAX_NUMBER_MISMATCH_ENTRIES];

} PACK_BYTE_ALIGNMENT (T_PN_SERVICE_LOCAL_AND_PEER_CHECK_IND);

/*****************************************************************************/

#if defined WIN32 || defined _WIN32
  #pragma warning (disable : 4103)     /* used #pragma pack to reset alignment */
  #pragma pack(pop)
  #pragma warning (default : 4103)

#elif defined __GNUC__
    /* nothing to do */

#elif defined _OS9000
    /* nothing to do */

#else
  #error CAUTION: set default structure alignment
#endif

/*****************************************************************************/

#endif /* __PROFINET_H__ */

/*****************************************************************************/
/* vim: set nu ts=2 et nowrap: */