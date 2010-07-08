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
DEFINES
******************************************************************************/

#undef PN_FALSE
#undef PN_TRUE

#define PN_FALSE          0x00u
#define PN_TRUE           0xFFu

typedef unsigned char     PN_U8;
typedef unsigned short    PN_U16;
typedef unsigned long     PN_U32;

typedef signed char       PN_S8;
typedef signed short      PN_S16;
typedef signed long       PN_S32;

typedef unsigned char     PN_BOOL;

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

#define PROFINET_IO_CR_MIN_SEND_CLOCK                       0x0020u
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
#define PROFINET_IO_SUBMODULE_MAX_SUB_SLOT_NUMBER           0x7FFFu
#define PROFINET_IO_SUBMODULE_MAX_SUB_SLOT_NUMBER_SLOT_0    0x8FFFu

#define PROFINET_SUBSLOT_INTERFACE                          0x8000
#define PROFINET_SUBSLOT_PORT_BASE                          0x8001

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

#define PROFINET_MRP_RT_MODE_OFF                            0x00000000uL
#define PROFINET_MRP_RT_MODE_CLASS1_CLASS2_ON               0x00000001uL
#define PROFINET_MRP_RT_MODE_CLASS3_ON                      0x00000002uL

/*---------------------------------------------------------------------------*/

#define PROFINET_MRP_CHECK_MANAGER_OFF                      0x00000000uL
#define PROFINET_MRP_CHECK_MANAGER_ON                       0x00000001uL
#define PROFINET_MRP_CHECK_DOMAIN_ON                        0x00000002uL

/*===========================================================================*/

typedef struct _T_PN_MAC_ADDRESS
{
  PN_U8   HighAndVersionHighByte;
  PN_U8   HighAndVersionLowByte;

  PN_U8   MidHighByte;
  PN_U8   MidLowByte;

  PN_U8   LowHighByte;
  PN_U8   LowLowByte;

} PACK_WORD_ALIGNMENT (T_PN_MAC_ADDRESS);

/*===========================================================================*/

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

#define PN_SERVICE_CLASS_CMCTL_CMDEV                  (PN_U8) _PN_GET_SERVICE_CLASS (PN_SERVICE_CLASS1)
#define PN_SERVICE_GET_DEVICE_STATE                   (PN_U8) (PN_SERVICE_CLASS1 | 0x00u)
#define PN_SERVICE_CONNECT                            (PN_U8) (PN_SERVICE_CLASS1 | 0x01u)
#define PN_SERVICE_WRITE                              (PN_U8) (PN_SERVICE_CLASS1 | 0x02u)
#define PN_SERVICE_READ                               (PN_U8) (PN_SERVICE_CLASS1 | 0x03u)
#define PN_SERVICE_CONTROL                            (PN_U8) (PN_SERVICE_CLASS1 | 0x04u)
#define PN_SERVICE_CLASS_CMCTL_CMDEV_MAX_SERVICE      PN_SERVICE_CONTROL

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
#define PN_SERVICE_CLASS_CONFIG_MAX_SERVICE           PN_SERVICE_SET_IDENTIFICATION

#define PN_SERVICE_CLASS_SUPERVISOR                   (PN_U8) _PN_GET_SERVICE_CLASS (PN_SERVICE_CLASS1)
#define PN_SERVICE_GET_LIST_OF_STATION                (PN_U8) (PN_SERVICE_CLASS1 | 0x00u)
#define PN_SERVICE_SET_DEVICE_NAME                    (PN_U8) (PN_SERVICE_CLASS1 | 0x01u)
#define PN_SERVICE_SET_IP_SETTINGS                    (PN_U8) (PN_SERVICE_CLASS1 | 0x02u)
#define PN_SERVICE_DCP_SET                            (PN_U8) (PN_SERVICE_CLASS1 | 0x03u)
#define PN_SERVICE_DCP_GET                            (PN_U8) (PN_SERVICE_CLASS1 | 0x04u)
#define PN_SERVICE_IMPLICIT_READ                      (PN_U8) (PN_SERVICE_CLASS1 | 0x05u)
#define PN_SERVICE_LOOKUP                             (PN_U8) (PN_SERVICE_CLASS1 | 0x06u)
#define PN_SERVICE_CLASS_SUPERVISOR_MAX_SERVICE       PN_SERVICE_LOOKUP

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

/*---------------------------------------------------------------------------*/

typedef struct _T_PN_SERVICE_ERROR_CON
{
  PN_U8    Code;
  PN_U8    Detail;

  PN_U8    AdditionalDetail;
  PN_U8    AreaCode;

} PACK_WORD_ALIGNMENT (T_PN_SERVICE_ERROR_CON);

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

} PACK_WORD_ALIGNMENT (T_PN_MCR_PROVIDER);

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
} PACK_WORD_ALIGNMENT (T_PN_MCR_CONSUMER);

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

} PACK_WORD_ALIGNMENT (T_PN_ALARM_CR);

/*---------------------------------------------------------------------------*/

typedef struct _T_PN_REFERENCE
{
  PN_U8   ReferenceHighByte;
  PN_U8   ReferenceLowByte;

} PACK_WORD_ALIGNMENT (T_PN_REFERENCE);

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

} PACK_WORD_ALIGNMENT (T_PN_SUBMODULE);

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

} PACK_WORD_ALIGNMENT (T_PN_MODULE);

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

} PACK_WORD_ALIGNMENT (T_PN_DATA_RECORD);

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

} PACK_WORD_ALIGNMENT (T_PN_API);

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

} PACK_WORD_ALIGNMENT (T_PN_IOCR);

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

#define PN_SERVICE_DOWNLOAD_NUMBER_API                            0x0002u

#define PN_SERVICE_DOWNLOAD_MIN_NUMBER_MODULES                    0x0001u

#define PN_SERVICE_DOWNLOAD_FLAG_ACTIVATE                         0x01u
#define PN_SERVICE_DOWNLOAD_FLAG_AUTO_CONNECT_RES                 0x02u
#define PN_SERVICE_DOWNLOAD_FLAG_AUTO_WRITE_RES                   0x04u
#define PN_SERVICE_DOWNLOAD_FLAG_AUTO_READ_RES                    0x08u
#define PN_SERVICE_DOWNLOAD_FLAG_AUTO_CONTROL_RES                 0x10u
#define PN_SERVICE_DOWNLOAD_FLAG_FULL_APPLICATION_IDENT_SUPPORT   0x20u
#define PN_SERVICE_DOWNLOAD_FLAG_CERTIFICATION_ACTIVE             0x80u

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
  PN_U8               Alignment;

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

  T_PN_ALARM_CR       PACK_WORD_ALIGNMENT (AlarmCRBlock);

/*
  T_PN_IOCR           IOCR [NumberOfIOCR];
  T_PN_API            Api [NumberOfAPIs];
  T_PN_MODULE         Module [NumberOfModules];
  T_PN_DATA_RECORD    DataRecords [NumberOfDataRecords];
*/

} PACK_WORD_ALIGNMENT (T_PN_SERVICE_DOWNLOAD_REQ);

/*---------------------------------------------------------------------------*/

typedef struct _T_PN_DATA_INFO
{
  PN_U8               SlotNumberHighByte;
  PN_U8               SlotNumberLowByte;

  PN_U8               SubSlotNumberHighByte;
  PN_U8               SubSlotNumberLowByte;

  PN_U8               OffsetHighByte;
  PN_U8               OffsetLowByte;

} PACK_WORD_ALIGNMENT (T_PN_DATA_INFO);

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

} PACK_WORD_ALIGNMENT (T_PN_API_INFO);

/*---------------------------------------------------------------------------*/

#define PN_IOCR_IDENTIFIER_DATA_PROVIDER              0x2000u
#define PN_IOCR_DEVICE_REF_MASK                       0x00FFu
#define PN_IOCR_NUMBER_MASK                           0x0300u
#define PN_IOCR_CONNECTION_MASK                       0x1C00u
#define PN_IOCR_IDENTIFIER_MASK                       (PN_IOCR_DEVICE_REF_MASK | \
                                                      PN_IOCR_NUMBER_MASK     | \
                                                      PN_IOCR_CONNECTION_MASK)

#define _PN_IOCR_NUMBER_TO_DEVICE_REF(IOCRIdentfier)  (IOCRIdentfier & PN_IOCR_DEVICE_REF_MASK)
#define _PN_IOCR_NUMBER_TO_INDEX(IOCRIdentfier)       ((IOCRIdentfier & PN_IOCR_NUMBER_MASK) >> 8u)
#define _PN_IOCR_CONNECTION_TO_INDEX(IOCRIdentfier)   ((IOCRIdentfier & PN_IOCR_CONNECTION_MASK) >> 10u)

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

} PACK_WORD_ALIGNMENT (T_PN_IOCR_INFO);

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

} PACK_WORD_ALIGNMENT (T_PN_SERVICE_DOWNLOAD_CON);

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

} PACK_WORD_ALIGNMENT (T_PN_SERVICE_SET_IDENTIFICATION_REQ);

/*---------------------------------------------------------------------------*/

typedef struct _T_PN_SERVICE_SET_IDENTIFICATION_CON
{
  PN_U16   DummyForCompiler;

} PACK_WORD_ALIGNMENT (T_PN_SERVICE_SET_IDENTIFICATION_CON);

/*===========================================================================*/

#define PN_SERVICE_ALARM_PRIO_LOW                               (PN_U8) 0x01u
#define PN_SERVICE_ALARM_PRIO_HIGH                              (PN_U8) 0x02u
#define PN_SERVICE_ALARM_PRIO_MASK                              (PN_SERVICE_ALARM_PRIO_LOW | \
                                                                 PN_SERVICE_ALARM_PRIO_HIGH)

#define PROFINET_ALARM_DIAGNOSIS_APPEARS                        (PN_U16) 0x0001u
#define PROFINET_ALARM_PROCESS                                  (PN_U16) 0x0002u
#define PROFINET_ALARM_PULL                                     (PN_U16) 0x0003u
#define PROFINET_ALARM_PLUG                                     (PN_U16) 0x0004u
#define PROFINET_ALARM_STATUS                                   (PN_U16) 0x0005u
#define PROFINET_ALARM_UPDATE                                   (PN_U16) 0x0006u
#define PROFINET_ALARM_REDUNDANCY                               (PN_U16) 0x0007u
#define PROFINET_ALARM_CONTROLLED_BY_SUPERVISOR                 (PN_U16) 0x0008u
#define PROFINET_ALARM_RELEASED                                 (PN_U16) 0x0009u
#define PROFINET_ALARM_PLUG_WRONG_SUBMODULE                     (PN_U16) 0x000Au
#define PROFINET_ALARM_RETURN_SUBMODULE                         (PN_U16) 0x000Bu
#define PROFINET_ALARM_DIAGNOSIS_DISAPPEARS                     (PN_U16) 0x000Cu
#define PROFINET_ALARM_MC_MISMATCH                              (PN_U16) 0x000Du
#define PROFINET_ALARM_PORT_CHANGE_NOTIFY                       (PN_U16) 0x000Eu
#define PROFINET_ALARM_SYNC_CHANGE_NOTIFY                       (PN_U16) 0x000Fu
#define PROFINET_ALARM_ISO_PROBLEM                              (PN_U16) 0x0010u
#define PROFINET_ALARM_NETWORK_COMPONENT_PROBLEM                (PN_U16) 0x0011u
#define PROFINET_ALARM_TIME_DATA_CHANGED                        (PN_U16) 0x0012u
#define PROFINET_ALARM_UPLOAD_RETRIEVAL_NOTIFICATION            (PN_U16) 0x001Eu
#define PROFINET_ALARM_PULL_MODULE                              (PN_U16) 0x001Fu
#define PROFINET_ALARM_MANUFACTURER_LOWER_LIMIT                 (PN_U16) 0x0020u
#define PROFINET_ALARM_MANUFACTURER_UPPER_LIMIT                 (PN_U16) 0x007Fu
#define PROFINET_ALARM_PROFILE_LOWER_LIMIT                      (PN_U16) 0x0080u
#define PROFINET_ALARM_PROFILE_UPPER_LIMIT                      (PN_U16) 0x00FFu

#define PROFINET_ALARM_SPECIFIER_SEQUENCE_MASK                  (PN_U16) 0x07FFu
#define PROFINET_ALARM_SPECIFIER_CHANNEL_DIAGNOSIS              (PN_U16) 0x0800u
#define PROFINET_ALARM_SPECIFIER_MANUFACTURER_DIAGNOSIS         (PN_U16) 0x1000u
#define PROFINET_ALARM_SPECIFIER_SUBMODULE_DIAGNOSIS_STATE      (PN_U16) 0x2000u
#define PROFINET_ALARM_SPECIFIER_AR_DIAGNOSIS_STATE             (PN_U16) 0x8000u
#define PROFINET_ALARM_SPECIFIER_MASK                           (PROFINET_ALARM_SPECIFIER_SEQUENCE_MASK            | \
                                                                PROFINET_ALARM_SPECIFIER_CHANNEL_DIAGNOSIS         | \
                                                                PROFINET_ALARM_SPECIFIER_MANUFACTURER_DIAGNOSIS    | \
                                                                PROFINET_ALARM_SPECIFIER_SUBMODULE_DIAGNOSIS_STATE | \
                                                                PROFINET_ALARM_SPECIFIER_AR_DIAGNOSIS_STATE)

#define PROFINET_ALARM_USER_STRUCTURE_IDENT_CHANNEL_DIAG        (PN_U16) 0x8000u
#define PROFINET_ALARM_USER_STRUCTURE_IDENT_MULTIPLE            (PN_U16) 0x8001u
#define PROFINET_ALARM_USER_STRUCTURE_IDENT_EXT_CHANNEL_DIAG    (PN_U16) 0x8002u
#define PROFINET_ALARM_USER_STRUCTURE_IDENT_QUAL_CHANNEL_DIAG   (PN_U16) 0x8003u
#define PROFINET_ALARM_USER_STRUCTURE_IDENT_MAINTENANCE         (PN_U16) 0x8100u
#define PROFINET_ALARM_USER_STRUCTURE_IDENT_UPLOAD_RETRIEVAL    (PN_U16) 0x8200u
#define PROFINET_ALARM_USER_STRUCTURE_IDENT_IPARAMETER          (PN_U16) 0x8201u

#define PROFINET_ALARM_CHANNEL_DIAG_CHANNEL_NUMBER_SUBSLOT      (PN_U16) 0x8000u

typedef struct _T_PN_SERVICE_GET_ALARM_REQ
{
  PN_U8   AlarmRefHighByte;
  PN_U8   AlarmRefLowByte;

} PACK_WORD_ALIGNMENT (T_PN_SERVICE_GET_ALARM_REQ);

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

} PACK_WORD_ALIGNMENT (T_PN_SERVICE_GET_ALARM_CON);

/*===========================================================================*/

typedef struct _T_PN_SERVICE_ALARM_ACK_REQ
{
  PN_U8   AlarmRefHighByte;
  PN_U8   AlarmRefLowByte;

  PN_U8   AlarmPriority;
  PN_U8   Alignment;

} PACK_WORD_ALIGNMENT (T_PN_SERVICE_ALARM_ACK_REQ);

/*---------------------------------------------------------------------------*/

typedef struct _T_PN_SERVICE_ALARM_ACK_CON
{
  PN_U8   AlarmRefHighByte;
  PN_U8   AlarmRefLowByte;

} PACK_WORD_ALIGNMENT (T_PN_SERVICE_ALARM_ACK_CON);

/*===========================================================================*/

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

} PACK_WORD_ALIGNMENT (T_PN_SERVICE_ALARM_REQ);

/*---------------------------------------------------------------------------*/

typedef struct _T_PN_SERVICE_ALARM_CON
{
  PN_U8   AlarmRefHighByte;
  PN_U8   AlarmRefLowByte;

} PACK_WORD_ALIGNMENT (T_PN_SERVICE_ALARM_CON);

/*===========================================================================*/

typedef struct _T_PN_SERVICE_GET_ALARM_ACK_REQ
{
  PN_U8   AlarmRefHighByte;
  PN_U8   AlarmRefLowByte;

} PACK_WORD_ALIGNMENT (T_PN_SERVICE_GET_ALARM_ACK_REQ);

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

} PACK_WORD_ALIGNMENT (T_PN_SERVICE_GET_ALARM_ACK_CON);

/*===========================================================================*/

typedef struct _T_PN_SERVICE_GET_DEVICE_STATE_REQ
{
  PN_U16   DummyForCompiler;

} PACK_WORD_ALIGNMENT (T_PN_SERVICE_GET_DEVICE_STATE_REQ);

/*---------------------------------------------------------------------------*/

typedef struct _T_PN_ERROR_CODE
{
  PN_U8   Reason;
  PN_U8   Decode;

  PN_U8   Code1;
  PN_U8   Code2;

} PACK_WORD_ALIGNMENT (T_PN_ERROR_CODE);

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

} PACK_WORD_ALIGNMENT (T_PN_DIFF_MODULE_API);

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

} PACK_WORD_ALIGNMENT (T_PN_DIFF_MODULE_SLOT);

/*---------------------------------------------------------------------------*/

#define PROFINET_SUBMODULE_STATE_DETAIL_NO_MODULE                     0x0000u
#define PROFINET_SUBMODULE_STATE_DETAIL_WRONG_MODULE                  0x0001u
#define PROFINET_SUBMODULE_STATE_DETAIL_LOCKED_BY_CONTROLLER          0x0002u
#define PROFINET_SUBMODULE_STATE_DETAIL_APPL_READY_PENDING            0x0004u
#define PROFINET_SUBMODULE_STATE_DETAIL_SUBSTITUTE_MODULE             0x0007u

#define PROFINET_SUBMODULE_STATE_FORMAT_INDICATOR                     0x8000u

#define PROFINET_SUBMODULE_STATE_ADD_INFO_NO                          (PROFINET_SUBMODULE_STATE_FORMAT_INDICATOR | 0x0000u)
#define PROFINET_SUBMODULE_STATE_ADD_INFO_TAKEOVER_NOT_ALLOWED        (PROFINET_SUBMODULE_STATE_FORMAT_INDICATOR | 0x0001u)
#define PROFINET_SUBMODULE_STATE_ADD_INFO_RESERVED                    (PROFINET_SUBMODULE_STATE_FORMAT_INDICATOR | 0x000Eu)
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

} PACK_WORD_ALIGNMENT (T_PN_DIFF_MODULE_SUBSLOT);

/*---------------------------------------------------------------------------*/

typedef struct _T_PN_DIFF_MODULE
{
  PN_U8                   NumberOfAPIsHighByte;
  PN_U8                   NumberOfAPIsLowByte;

/*
  T_PN_DIFF_MODULE_API    Api [NumberOfAPIs];
*/

} PACK_WORD_ALIGNMENT (T_PN_DIFF_MODULE);

/*---------------------------------------------------------------------------*/

typedef struct _T_PN_SERVICE_GET_DEVICE_STATE_CON
{
  PN_U8               StateHighByte;
  PN_U8               StateLowByte;

  T_PN_ERROR_CODE     PACK_WORD_ALIGNMENT (Error);

  PN_U8               NumberOfDiffModulesHighByte;
  PN_U8               NumberOfDiffModulesLowByte;

/*
  T_PN_DIFF_MODULE    DiffModule [NumberOfModules];
*/

} PACK_WORD_ALIGNMENT (T_PN_SERVICE_GET_DEVICE_STATE_CON);

/*===========================================================================*/

typedef struct _T_PN_SERVICE_GET_LIST_OF_STATION_REQ
{
  PN_U16   DummyForCompiler;

} PACK_WORD_ALIGNMENT (T_PN_SERVICE_GET_LIST_OF_STATION_REQ);

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

} PACK_WORD_ALIGNMENT (T_PN_DEVICE_IP_INFO);

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

} PACK_WORD_ALIGNMENT (T_PN_DEVICE_PROPERTY_INFO);

/*---------------------------------------------------------------------------*/

typedef struct _T_PN_DEVICE_INFO
{
  T_PN_MAC_ADDRESS            PACK_WORD_ALIGNMENT (MacAddress);
  T_PN_DEVICE_IP_INFO         PACK_WORD_ALIGNMENT (Ip);
  T_PN_DEVICE_PROPERTY_INFO   PACK_WORD_ALIGNMENT (Property);

  PN_U8                       DeviceNameLengthHighByte;
  PN_U8                       DeviceNameLengthLowByte;

/*
  char                        DeviceName [DeviceNameLength];
*/

} PACK_WORD_ALIGNMENT (T_PN_DEVICE_INFO);

/*---------------------------------------------------------------------------*/

typedef struct _T_PN_SERVICE_GET_LIST_OF_STATION_CON
{
  PN_U8               NumberOfDevicesHighByte;
  PN_U8               NumberOfDevicesLowByte;

/*
  T_PN_DEVICE_INFO    Device [NumberOfDevices];
*/

} PACK_WORD_ALIGNMENT (T_PN_SERVICE_GET_LIST_OF_STATION_CON);

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

} PACK_WORD_ALIGNMENT (T_PN_SERVICE_SET_NAME_REQ);

/*---------------------------------------------------------------------------*/

typedef struct _T_PN_SERVICE_SET_NAME_CON
{
  PN_U16   DummyForCompiler;

} PACK_WORD_ALIGNMENT (T_PN_SERVICE_SET_NAME_CON);

/*===========================================================================*/

typedef struct _T_PN_SERVICE_SET_IP_SETTINGS_REQ
{
  T_PN_MAC_ADDRESS    PACK_WORD_ALIGNMENT (DestMacAddress);

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

} PACK_WORD_ALIGNMENT (T_PN_SERVICE_SET_IP_SETTINGS_REQ);

/*---------------------------------------------------------------------------*/

typedef struct _T_PN_SERVICE_SET_IP_SETTINGS_CON
{
  PN_U16   DummyForCompiler;

} PACK_WORD_ALIGNMENT (T_PN_SERVICE_SET_IP_SETTINGS_CON);

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
#define DCP_OPTION_CONTROL_SUB_RESET_TO_FACTORY_SETTING   (PN_U8) 5u

#define DCP_OPTION_DEVICE_INITIATIVE_SUB_DEVICE           (PN_U8) 1u

#define DCP_OPTION_ALL_SUB_ALL                            (PN_U8) 255u

#define DCP_MAX_NUMBER_SUBOPTIONS                         (PN_U8) 8u

#define DCP_SUBOPTION_SOFTING_PROFILE_INFO                (PN_U8) 1u
#define DCP_SUBOPTION_SOFTING_UPDATE_FW                   (PN_U8) 2u
#define DCP_SUBOPTION_SOFTING_SET_ACTIVE_CONFIGURATION    (PN_U8) 3u

#define DCP_OPTION_SOFTING_PROFILE_INFO_ADD               (PN_U8) 1u
#define DCP_OPTION_SOFTING_PROFILE_INFO_DELETE            (PN_U8) 2u

#define PROFILE_INFO_TASK_PROFILING                       (PN_U8) 255u

typedef struct _T_PN_PROFILE_INFO
{
  PN_U8   Action;
  PN_U8   ModuleId;

} PACK_WORD_ALIGNMENT (T_PN_PROFILE_INFO);

/*---------------------------------------------------------------------------*/

#define PN_SERVICE_DCP_SET_RESPONSE_TIMEOUT       PN_SERVICE_SET_RESPONSE_TIMEOUT

typedef struct _T_PN_SERVICE_DCP_SET_REQ
{
  T_PN_MAC_ADDRESS    PACK_WORD_ALIGNMENT (DestMacAddress);

  PN_U8               Option;
  PN_U8               Suboption;

  PN_U8               LengthHighByte;
  PN_U8               LengthLowByte;

/*
  char                Data [Length];
*/
} PACK_WORD_ALIGNMENT (T_PN_SERVICE_DCP_SET_REQ);

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

} PACK_WORD_ALIGNMENT (T_PN_SERVICE_DCP_SET_CON);

/*===========================================================================*/

#define PN_SERVICE_DCP_GET_RESPONSE_TIMEOUT       PN_SERVICE_SET_RESPONSE_TIMEOUT

typedef struct _T_PN_SERVICE_DCP_GET_REQ
{
  T_PN_MAC_ADDRESS    PACK_WORD_ALIGNMENT (DestMacAddress);

  PN_U8               Option;
  PN_U8               Suboption;

} PACK_WORD_ALIGNMENT (T_PN_SERVICE_DCP_GET_REQ);

/*---------------------------------------------------------------------------*/

typedef struct _T_PN_SYS_TIME
{
  PN_U32    HighPart;
  PN_U32    LowPart;

} PACK_WORD_ALIGNMENT (T_PN_SYS_TIME);

typedef struct _T_PN_PROFILE_ENTRY
{
  PN_U32          Identifier;
  T_PN_SYS_TIME   Time;

} PACK_WORD_ALIGNMENT (T_PN_PROFILE_ENTRY);

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
} PACK_WORD_ALIGNMENT (T_PN_PROFILE_INFO_HEADER);

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

} PACK_WORD_ALIGNMENT (T_PN_SERVICE_DCP_GET_CON);

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
#define APPLICATION_DATA_OPTION_CONTROL_SUB_RESET_TO_FACTORY_SETTING    (PN_U8) 5u

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

} PACK_WORD_ALIGNMENT (T_PN_APPLICATION_OPTION);

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
} PACK_WORD_ALIGNMENT (T_PN_APPLICATION_OPTION_DATA);

/*---------------------------------------------------------------------------*/

typedef struct _T_PN_APPLICATION_OPTION_RESULT
{
  PN_U8   Option;
  PN_U8   Suboption;

  PN_U8   Result;

} PACK_WORD_ALIGNMENT (T_PN_APPLICATION_OPTION_RESULT);

/*---------------------------------------------------------------------------*/

typedef T_PN_APPLICATION_OPTION     T_PN_APPLICATION_OPTION_GET;

typedef struct _T_PN_SERVICE_APPLICATION_DATA_GET_IND
{
  PN_U8                         NumberOptionsHighByte;
  PN_U8                         NumberOptionsLowByte;

/*
  T_PN_APPLICATION_OPTION_GET   Data [NumberOptions];
*/
} PACK_WORD_ALIGNMENT (T_PN_SERVICE_APPLICATION_DATA_GET_IND);

/*---------------------------------------------------------------------------*/

typedef T_PN_APPLICATION_OPTION_DATA    T_PN_APPLICATION_OPTION_DATA_GET;

typedef struct _T_PN_SERVICE_APPLICATION_DATA_GET_RES
{
  PN_U8                               NumberOptionsHighByte;
  PN_U8                               NumberOptionsLowByte;

/*
  T_PN_APPLICATION_OPTION_DATA_GET    Data [NumberOptions];
*/
} PACK_WORD_ALIGNMENT (T_PN_SERVICE_APPLICATION_DATA_GET_RES);

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
} PACK_WORD_ALIGNMENT (T_PN_SERVICE_APPLICATION_DATA_SET_IND);

/*---------------------------------------------------------------------------*/

typedef T_PN_APPLICATION_OPTION_RESULT    T_PN_APPLICATION_OPTION_SET;

typedef struct _T_PN_SERVICE_APPLICATION_DATA_SET_RES
{
  PN_U8                         NumberOptionsHighByte;
  PN_U8                         NumberOptionsLowByte;

/*
  T_PN_APPLICATION_OPTION_SET   Data [NumberOptions];
*/
} PACK_WORD_ALIGNMENT (T_PN_SERVICE_APPLICATION_DATA_SET_RES);

/*===========================================================================*/

typedef struct _T_PN_SERVICE_GET_ETHERNET_DATA_REQ
{
  PN_U16   DummyForCompiler;

} PACK_WORD_ALIGNMENT (T_PN_SERVICE_GET_ETHERNET_DATA_REQ);

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

#define PROFINET_MAU_TYPE_UNKNOWN               (PN_U16) 1u
#define PROFINET_MAU_TYPE_10BASE_TXHD           (PN_U16) 10u
#define PROFINET_MAU_TYPE_10BASE_TXFD           (PN_U16) 11u
#define PROFINET_MAU_TYPE_100BASE_TXHD          (PN_U16) 15u
#define PROFINET_MAU_TYPE_100BASE_TXFD          (PN_U16) 16u
#define PROFINET_MAU_TYPE_100BASE_FXHD          (PN_U16) 17u
#define PROFINET_MAU_TYPE_100BASE_FXFD          (PN_U16) 18u
#define PROFINET_MAU_TYPE_1000BASE_TXHD         (PN_U16) 29u
#define PROFINET_MAU_TYPE_1000BASE_TXFD         (PN_U16) 30u

#define PN_LINE_LENGTH_UNKNOWN                  (PN_U16) 0xFFFFu
#define PN_DELAY_UNKNOWN                        (PN_U16) 0u

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

  PN_U8   RxDelayHighByte;
  PN_U8   RxDelayLowByte;

  PN_U8   TxDelayHighByte;
  PN_U8   TxDelayLowByte;

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

} PACK_WORD_ALIGNMENT (T_PN_PORT_DATA);

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
} PACK_WORD_ALIGNMENT (T_PN_SERVICE_GET_ETHERNET_DATA_CON);

/*===========================================================================*/

typedef struct _T_PN_CONNECT_IO_LOCATION
{
  PN_U8   SlotNumberHighByte;
  PN_U8   SlotNumberLowByte;

  PN_U8   SubSlotNumberHighByte;
  PN_U8   SubSlotNumberLowByte;

  PN_U8   OffsetHighByte;
  PN_U8   OffsetLowByte;

} PACK_WORD_ALIGNMENT (T_PN_CONNECT_IO_LOCATION);

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

} PACK_WORD_ALIGNMENT (T_PN_CONNECT_IOCR_API);

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

} PACK_WORD_ALIGNMENT (T_PN_CONNECT_IOCR);

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

  PN_U8   ConsumerStatusLength;
  PN_U8   ProviderStatusLength;

} PACK_WORD_ALIGNMENT (T_PN_CONNECT_SUBMODULE);

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

} PACK_WORD_ALIGNMENT (T_PN_CONNECT_MODULE_API);

/*---------------------------------------------------------------------------*/

typedef struct _T_PN_CONNECT_MODULE
{
  PN_U8                     NumberOfAPIsHighByte;
  PN_U8                     NumberOfAPIsLowByte;

/*
  T_PN_CONNECT_MODULE_API   Api [NumberOfAPIs];
*/

} PACK_WORD_ALIGNMENT (T_PN_CONNECT_MODULE);

/*---------------------------------------------------------------------------*/

#define PROFINET_AR_TYPE_IOCARSINGLE            (PN_U16) 0x0001u
#define PROFINET_AR_TYPE_IOCAR_CONFIG_IN_RUN    (PN_U16) 0x0003u
#define PROFINET_AR_TYPE_IOCAR_RED_CONTROLLER   (PN_U16) 0x0004u
#define PROFINET_AR_TYPE_IOCAR_RED_DEVICE       (PN_U16) 0x0005u
#define PROFINET_AR_TYPE_IOSAR                  (PN_U16) 0x0006u

typedef struct _T_PN_SERVICE_CONNECT_IND
{
  PN_U8                 ARIdentifierHighByte;
  PN_U8                 ARIdentifierLowByte;

  PN_U8                 ARTypeHighByte;
  PN_U8                 ARTypeLowByte;

  PN_U8                 ARPropertiesHighWordHighByte;
  PN_U8                 ARPropertiesHighWordLowByte;
  PN_U8                 ARPropertiesLowWordHighByte;
  PN_U8                 ARPropertiesLowWordLowByte;

  PN_U8                 AlarmRefHighByte;
  PN_U8                 AlarmRefLowByte;

  PN_U8                 NumberOfIOCRHighByte;
  PN_U8                 NumberOfIOCRLowByte;

  PN_U8                 NumberOfModuleHighByte;
  PN_U8                 NumberOfModuleLowByte;

/*
  T_PN_CONNECT_IOCR     IOCR [NumberOfIOCR];
  T_PN_CONNECT_MODULE   Module [NumberOfModule];
*/

} PACK_WORD_ALIGNMENT (T_PN_SERVICE_CONNECT_IND);

/*---------------------------------------------------------------------------*/

#define ERROR_CODE1_OK                                0u
#define ERROR_CODE2_OK                                0u

#define CONNECT_RES_ERROR_CODE1_OK                    ERROR_CODE1_OK
#define CONNECT_RES_ERROR_CODE2_OK                    ERROR_CODE2_OK
#define CONNECT_RES_ERROR_CODE1_AR_OUT_OF_RESOURCE    0x40u
#define CONNECT_RES_ERROR_CODE2_AR_OUT_OF_RESOURCE    0x04u

typedef struct _T_PN_SERVICE_CONNECT_RES
{
  PN_U8               ARIdentifierHighByte;
  PN_U8               ARIdentifierLowByte;

  PN_U8               ErrorCode1;
  PN_U8               ErrorCode2;

  PN_U8               NumberOfDiffModulesHighByte;
  PN_U8               NumberOfDiffModulesLowByte;

/*
  T_PN_DIFF_MODULE    DiffModule [NumberOfModules];
*/

} PACK_WORD_ALIGNMENT (T_PN_SERVICE_CONNECT_RES);

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

} PACK_WORD_ALIGNMENT (T_PN_SERVICE_WRITE_IND);

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

} PACK_WORD_ALIGNMENT (T_PN_SERVICE_WRITE_REQ);

/*---------------------------------------------------------------------------*/

#define WRITE_RES_ERROR_CODE1_OK                          ERROR_CODE1_OK
#define WRITE_RES_ERROR_CODE2_OK                          ERROR_CODE2_OK

#define WRITE_RES_ERROR_CODE1_INVALID_LENGTH              0x08u
#define WRITE_RES_ERROR_CODE2_INVALID_LENGTH              0x0Bu
#define WRITE_RES_ERROR_CODE1_RESSOURCE_BUSY              0xC2u
#define WRITE_RES_ERROR_CODE2_RESSOURCE_BUSY              0xC2u
#define WRITE_RES_ERROR_CODE1_INVALID_INDEX               0xB0u
#define WRITE_RES_ERROR_CODE2_INVALID_INDEX               10u
#define WRITE_RES_ERROR_CODE1_INVALID_SLOT_SUBSLOT        0xB2u
#define WRITE_RES_ERROR_CODE2_INVALID_SLOT_SUBSLOT        7u

typedef struct _T_PN_SERVICE_WRITE_RES
{
  PN_U8   ARIdentifierHighByte;
  PN_U8   ARIdentifierLowByte;

  PN_U8   ErrorCode1;
  PN_U8   ErrorCode2;

} PACK_WORD_ALIGNMENT (T_PN_SERVICE_WRITE_RES);

/*---------------------------------------------------------------------------*/

typedef struct _T_PN_SERVICE_WRITE_CON
{
  PN_U16   DummyForCompiler;

} PACK_WORD_ALIGNMENT (T_PN_SERVICE_WRITE_CON);

/*===========================================================================*/

#define PROFINET_INDEX_SUBSLOT_EXPECTED_IDENTIFICATION      (PN_U16) 0x8000u
#define PROFINET_INDEX_SUBSLOT_REAL_IDENTIFICATION          (PN_U16) 0x8001u
#define PROFINET_INDEX_SUBSLOT_DIAGNOSIS_START              (PN_U16) 0x800Au
#define PROFINET_INDEX_SUBSLOT_DIAGNOSIS_END                (PN_U16) 0x800Cu
#define PROFINET_INDEX_SUBSLOT_MAINTENANCE_START            (PN_U16) 0x8010u
#define PROFINET_INDEX_SUBSLOT_MAINTENANCE_END              (PN_U16) 0x8013u
#define PROFINET_INDEX_SUBSLOT_SUBSTITUTE                   (PN_U16) 0x801Eu
#define PROFINET_INDEX_SUBSLOT_INPUT_DATA                   (PN_U16) 0x8028u
#define PROFINET_INDEX_SUBSLOT_OUTPUT_DATA                  (PN_U16) 0x8029u
#define PROFINET_INDEX_SUBSLOT_PD_PORT_DATA_REAL            (PN_U16) 0x802Au
#define PROFINET_INDEX_SUBSLOT_PDIR_DATA                    (PN_U16) 0x802Cu
#define PROFINET_INDEX_SUBSLOT_PD_INTERFACE_MRP_DATA_REAL   (PN_U16) 0x8050u
#define PROFINET_INDEX_SUBSLOT_PD_PORT_MRP_DATA_REAL        (PN_U16) 0x8054u
#define PROFINET_INDEX_SUBSLOT_PD_PORT_FO_DATA_REAL         (PN_U16) 0x8060u
#define PROFINET_INDEX_SUBSLOT_PD_PORT_FO_DATA_ADJUST       (PN_U16) 0x8062u
#define PROFINET_INDEX_SUBSLOT_PD_NC_DATA_CHECK             (PN_U16) 0x8070u
#define PROFINET_INDEX_SUBSLOT_PD_INTERFACE_DATA_REAL       (PN_U16) 0x8080u
#define PROFINET_INDEX_SUBSLOT_PD_INTERFACE_FSU_ADJUST      (PN_U16) 0x8090u
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
#define PROFINET_INDEX_SLOT_MAINTENANCE_START               (PN_U16) 0xC010u
#define PROFINET_INDEX_SLOT_MAINTENANCE_END                 (PN_U16) 0xC013u
#define PROFINET_INDEX_SLOT_PROFILE_RESERVED_START          (PN_U16) 0xD000u
#define PROFINET_INDEX_SLOT_PROFILE_RESERVED_END            (PN_U16) 0xDFFFu

#define PROFINET_INDEX_AR_EXPECTED_IDENTIFICATION           (PN_U16) 0xE000u
#define PROFINET_INDEX_AR_REAL_IDENTIFICATION               (PN_U16) 0xE001u
#define PROFINET_INDEX_AR_MODULE_DIFF_BLOCK                 (PN_U16) 0xE002u

#define PROFINET_INDEX_API_REAL_IDENTIFICATION              (PN_U16) 0xF000u
#define PROFINET_INDEX_API_DIAGNOSIS_START                  (PN_U16) 0xF00Au
#define PROFINET_INDEX_API_DIAGNOSIS_END                    (PN_U16) 0xF00Cu
#define PROFINET_INDEX_API_MAINTENANCE_START                (PN_U16) 0xF010u
#define PROFINET_INDEX_API_MAINTENANCE_END                  (PN_U16) 0xF013u
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

} PACK_WORD_ALIGNMENT (T_PN_SERVICE_READ_IND);

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

} PACK_WORD_ALIGNMENT (T_PN_SERVICE_READ_REQ);

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

} PACK_WORD_ALIGNMENT (T_PN_SERVICE_READ_CON);

/*---------------------------------------------------------------------------*/

#define READ_RES_ERROR_CODE1_OK                           ERROR_CODE1_OK
#define READ_RES_ERROR_CODE2_OK                           ERROR_CODE2_OK

#define READ_RES_ERROR_CODE1_INVALID_LENGTH               WRITE_RES_ERROR_CODE1_INVALID_LENGTH
#define READ_RES_ERROR_CODE2_INVALID_LENGTH               WRITE_RES_ERROR_CODE2_INVALID_LENGTH
#define READ_RES_ERROR_CODE1_RESSOURCE_BUSY               WRITE_RES_ERROR_CODE1_RESSOURCE_BUSY
#define READ_RES_ERROR_CODE2_RESSOURCE_BUSY               WRITE_RES_ERROR_CODE2_RESSOURCE_BUSY
#define READ_RES_ERROR_CODE1_INVALID_INDEX                WRITE_RES_ERROR_CODE1_INVALID_INDEX
#define READ_RES_ERROR_CODE2_INVALID_INDEX                WRITE_RES_ERROR_CODE2_INVALID_INDEX
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

} PACK_WORD_ALIGNMENT (T_PN_SERVICE_READ_RES);

/*===========================================================================*/

#define PROFINET_BLOCK_TYPE_READ_REQ                      (PN_U16) 0x0009u
#define PROFINET_BLOCK_TYPE_READ_RES                      (PN_U16) 0x8009u
#define PROFINET_BLOCK_TYPE_DIAGNOSIS                     (PN_U16) 0x0010u
#define PROFINET_BLOCK_TYPE_EXPECTED_IDENT                (PN_U16) 0x0012u
#define PROFINET_BLOCK_TYPE_REAL_IDENT                    (PN_U16) 0x0013u
#define PROFINET_BLOCK_TYPE_SUBSTITUTE_VALUE              (PN_U16) 0x0014u
#define PROFINET_BLOCK_TYPE_INPUT_DATA                    (PN_U16) 0x0015u
#define PROFINET_BLOCK_TYPE_OUTPUT_DATA                   (PN_U16) 0x0016u
#define PROFINET_BLOCK_TYPE_AR_DATA                       (PN_U16) 0x0018u
#define PROFINET_BLOCK_TYPE_LOG_DATA                      (PN_U16) 0x0019u
#define PROFINET_BLOCK_TYPE_API_DATA                      (PN_U16) 0x001Au
#define PROFINET_BLOCK_TYPE_IDENT_AND_MAINTENANCE_0       (PN_U16) 0x0020u
#define PROFINET_BLOCK_TYPE_IDENT_AND_MAINTENANCE_1       (PN_U16) 0x0021u
#define PROFINET_BLOCK_TYPE_IDENT_AND_MAINTENANCE_2       (PN_U16) 0x0022u
#define PROFINET_BLOCK_TYPE_IDENT_AND_MAINTENANCE_3       (PN_U16) 0x0023u
#define PROFINET_BLOCK_TYPE_IDENT_AND_MAINTENANCE_4       (PN_U16) 0x0024u
#define PROFINET_BLOCK_TYPE_IM0_FILTER_SUBMODULE          (PN_U16) 0x0030u
#define PROFINET_BLOCK_TYPE_IM0_FILTER_MODULE             (PN_U16) 0x0031u
#define PROFINET_BLOCK_TYPE_IM0_FILTER_DEVICE             (PN_U16) 0x0032u
#define PROFINET_BLOCK_TYPE_MODULE_DIFF                   (PN_U16) 0x8104u
#define PROFINET_BLOCK_TYPE_PDEV_DATA                     (PN_U16) 0x0201u
#define PROFINET_BLOCK_TYPE_PDIR_DATA                     (PN_U16) 0x0205u
#define PROFINET_BLOCK_TYPE_PDIR_GLOBAL_DATA              (PN_U16) 0x0206u
#define PROFINET_BLOCK_TYPE_PDIR_FRAME_DATA               (PN_U16) 0x0207u
#define PROFINET_BLOCK_TYPE_PDIR_BEGIN_END_DATA           (PN_U16) 0x0208u
#define PROFINET_BLOCK_TYPE_PD_PORT_REAL_DATA             (PN_U16) 0x020Fu
#define PROFINET_BLOCK_TYPE_PD_INTERFACE_MRP_REAL         (PN_U16) 0x0212u
#define PROFINET_BLOCK_TYPE_PD_PORT_MRP_REAL              (PN_U16) 0x0215u
#define PROFINET_BLOCK_TYPE_MRP_CLIENT_PARAMS             (PN_U16) 0x0217u
#define PROFINET_BLOCK_TYPE_MRP_RING_STATE                (PN_U16) 0x0219u
#define PROFINET_BLOCK_TYPE_MRP_RT_STATE                  (PN_U16) 0x021Au
#define PROFINET_BLOCK_TYPE_MRP_RT_CLIENT_DATA            (PN_U16) 0x021Du
#define PROFINET_BLOCK_TYPE_PD_NC_CHECK                   (PN_U16) 0x0230u
#define PROFINET_BLOCK_TYPE_PD_INTERFACE_REAL             (PN_U16) 0x0240u
#define PROFINET_BLOCK_TYPE_MULTIPLE_BLOCKS               (PN_U16) 0x0400u

#define PROFINET_INDEX_USER_SPECIFIC_BASE                 (PN_U16) 0x0000u
#define PROFINET_INDEX_USER_SPECIFIC_SIZE                 (PN_U16) 0x7FFFu
#define PROFINET_INDEX_SUBSLOT_SPECIFIC_BASE              (PN_U16) 0x8000u
#define PROFINET_INDEX_SUBSLOT_SPECIFIC_SIZE              (PN_U16) 0x3FFFu
#define PROFINET_INDEX_SLOT_SPECIFIC_BASE                 (PN_U16) 0xC000u
#define PROFINET_INDEX_SLOT_SPECIFIC_SIZE                 (PN_U16) 0x1FFFu
#define PROFINET_INDEX_AR_SPECIFIC_BASE                   (PN_U16) 0xE000u
#define PROFINET_INDEX_AR_SPECIFIC_SIZE                   (PN_U16) 0x0FFFu
#define PROFINET_INDEX_API_SPECIFIC_BASE                  (PN_U16) 0xF000u
#define PROFINET_INDEX_API_SPECIFIC_SIZE                  (PN_U16) 0x07FFu
#define PROFINET_INDEX_DEVICE_SPECIFIC_BASE               (PN_U16) 0xF800u
#define PROFINET_INDEX_DEVICE_SPECIFIC_SIZE               (PN_U16) 0x07FFu

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

} PACK_WORD_ALIGNMENT (T_PROFINET_UUID);

/*---------------------------------------------------------------------------*/

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

} PACK_WORD_ALIGNMENT (T_PN_SERVICE_IMPLICIT_READ_REQ);

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

} PACK_WORD_ALIGNMENT (T_PN_SERVICE_IMPLICIT_READ_CON);

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

} PACK_WORD_ALIGNMENT (T_PN_SERVICE_LOOKUP_REQ);

/*---------------------------------------------------------------------------*/

typedef struct _T_PN_SERVICE_LOOKUP_CON
{
  PN_U8             InstanceHighByte;
  PN_U8             InstanceLowByte;

  PN_U8             DeviceIdHighByte;
  PN_U8             DeviceIdLowByte;

  PN_U8             VendorIdHighByte;
  PN_U8             VendorIdLowByte;

} PACK_WORD_ALIGNMENT (T_PN_SERVICE_LOOKUP_CON);

/*===========================================================================*/

typedef struct _T_PROFINET_BLOCK_HEADER
{
  PN_U8   TypeHighByte;
  PN_U8   TypeLowByte;

  PN_U8   LengthHighByte;
  PN_U8   LengthLowByte;

  PN_U8   VersionHighByte;
  PN_U8   VersionLowByte;

} PACK_WORD_ALIGNMENT (T_PROFINET_BLOCK_HEADER);

/*---------------------------------------------------------------------------*/

typedef struct _T_PROFINET_IOCR
{
  PN_U8   IOCRTypeHighByte;
  PN_U8   IOCRTypeLowByte;

  PN_U8   PropertiesHighWordHighByte;
  PN_U8   PropertiesHighWordLowByte;
  PN_U8   PropertiesLowWordHighByte;
  PN_U8   PropertiesLowWordLowByte;

  PN_U8   FrameIdHighByte;
  PN_U8   FrameIdLowByte;

  PN_U8   CycleCounterHighByte;
  PN_U8   CycleCounterLowByte;

  PN_U8   DataStatus;
  PN_U8   TransferStatus;

  PN_U8   InitiatorPortHighByte;
  PN_U8   InitiatorPortLowByte;

  PN_U8   ResponderPortHighByte;
  PN_U8   ResponderPortLowByte;

} PACK_WORD_ALIGNMENT (T_PROFINET_IOCR);

/*---------------------------------------------------------------------------*/

typedef struct _T_PROFINET_API
{
  PN_U8   APIHighWordHighByte;
  PN_U8   APIHighWordLowByte;
  PN_U8   APILowWordHighByte;
  PN_U8   APILowWordLowByte;

} PACK_WORD_ALIGNMENT (T_PROFINET_API);

/*---------------------------------------------------------------------------*/

typedef struct _T_PROFINET_AR
{
  T_PROFINET_UUID   PACK_WORD_ALIGNMENT (ARUUID);

  PN_U8             ARTypeHighByte;
  PN_U8             ARTypeLowByte;

  PN_U8             PropertiesHighWordHighByte;
  PN_U8             PropertiesHighWordLowByte;
  PN_U8             PropertiesLowWordHighByte;
  PN_U8             PropertiesLowWordLowByte;

  T_PROFINET_UUID   PACK_WORD_ALIGNMENT (InitiatorUUID);

  PN_U8             InitiatorNameLengthHighByte;
  PN_U8             InitiatorNameLengthLowByte;

/*
  PN_U8             InitiatorName [InitiatorNameLength];

  PN_U8             NumberOfIOCRHighByte;
  PN_U8             NumberOfIOCRLowByte;

  T_PROFINET_IOCR   IOCR [NumberOfIOCR];

  PN_U8             AlarmTypeHighByte;
  PN_U8             AlarmTypeLowByte;

  PN_U8             LocalAlarmRefHighByte;
  PN_U8             LocalAlarmRefLowByte;

  PN_U8             RemoteAlarmRefHighByte;
  PN_U8             RemoteAlarmRefLowByte;

  T_PROFINET_UUID   ParameterServerUUID;

  PN_U8             ParameterServerNameLengthHighByte;
  PN_U8             ParameterServerNameLengthLowByte;

  PN_U8             ParameterServerName [ParameterServerNameLength];

  PN_U8             NumberOfAPIHighByte;
  PN_U8             NumberOfAPILowByte;

  T_PROFINET_API    API [NumberOfAPI];
*/

} PACK_WORD_ALIGNMENT (T_PROFINET_AR);

/*---------------------------------------------------------------------------*/

typedef struct _T_PROFINET_AR_DATA
{
  T_PROFINET_BLOCK_HEADER   PACK_WORD_ALIGNMENT (Header);

  PN_U8                     NumberOfARsHighByte;
  PN_U8                     NumberOfARsLowByte;

/*
  T_PROFINET_AR             AR [NumberOfARs];
*/

} PACK_WORD_ALIGNMENT (T_PROFINET_AR_DATA);

/*===========================================================================*/

typedef struct _T_PROFINET_IM0_FILTER_SUBMODULE
{
  PN_U8             SubSlotNumberHighByte;
  PN_U8             SubSlotNumberLowByte;

  PN_U8             IdentNumberHighWordHighByte;
  PN_U8             IdentNumberHighWordLowByte;
  PN_U8             IdentNumberLowWordHighByte;
  PN_U8             IdentNumberLowWordLowByte;

} PACK_WORD_ALIGNMENT (T_PROFINET_IM0_FILTER_SUBMODULE);

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

} PACK_WORD_ALIGNMENT (T_PROFINET_IM0_FILTER_MODULE);

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

} PACK_WORD_ALIGNMENT (T_PROFINET_IM0_FILTER_API);

typedef T_PROFINET_IM0_FILTER_API   T_PROFINET_REAL_IDENT_API;
typedef T_PROFINET_IM0_FILTER_API   T_PROFINET_EXPECTED_IDENT_API;

/*---------------------------------------------------------------------------*/

typedef struct _T_PROFINET_IM0_FILTER_DATA
{
  T_PROFINET_BLOCK_HEADER   PACK_WORD_ALIGNMENT (Header);

  PN_U8                     NumberOfAPIsHighByte;
  PN_U8                     NumberOfAPIsLowByte;

/*
  T_PROFINET_API            API [NumberOfAPIs];
*/

} PACK_WORD_ALIGNMENT (T_PROFINET_IM0_FILTER_DATA);

/*===========================================================================*/

typedef struct _T_PROFINET_API_DATA
{
  T_PROFINET_BLOCK_HEADER   PACK_WORD_ALIGNMENT (Header);

  PN_U8                     NumberOfAPIsHighByte;
  PN_U8                     NumberOfAPIsLowByte;

/*
  T_PROFINET_API            API [NumberOfAPIs];
*/

} PACK_WORD_ALIGNMENT (T_PROFINET_API_DATA);

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

} PACK_WORD_ALIGNMENT (T_PROFINET_SUBMODULE_DATA);

/*---------------------------------------------------------------------------*/

typedef struct _T_PROFINET_SUBMODULE_IO_DATA
{
  PN_U8    DataDescriptionHighByte;
  PN_U8    DataDescriptionLowByte;

  PN_U8    IODataLengthHighByte;
  PN_U8    IODataLengthLowByte;

  PN_U8    ConsumerStatusLength;
  PN_U8    ProviderStatusLength;

} PACK_WORD_ALIGNMENT (T_PROFINET_SUBMODULE_IO_DATA);

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

} PACK_WORD_ALIGNMENT (T_PROFINET_MODULE_API);

/*---------------------------------------------------------------------------*/

typedef struct _T_PROFINET_MODULE_REQ
{
  T_PROFINET_BLOCK_HEADER   PACK_WORD_ALIGNMENT (Header);

  PN_U8                     NumberOfAPIsHighByte;
  PN_U8                     NumberOfAPIsLowByte;

/*
  T_PROFINET_MODULE_API     Api [NumberOfAPIs];
*/

} PACK_WORD_ALIGNMENT (T_PROFINET_MODULE_REQ);

typedef T_PROFINET_MODULE_REQ   T_PROFINET_REAL_IDENT_DATA;
typedef T_PROFINET_MODULE_REQ   T_PROFINET_EXPECTED_IDENT_DATA;
typedef T_PROFINET_MODULE_REQ   T_PROFINET_MODULE_DIFF_DATA;

/*---------------------------------------------------------------------------*/

typedef struct _T_PROFINET_RECORD_INPUT_DATA
{
  T_PROFINET_BLOCK_HEADER   PACK_WORD_ALIGNMENT (Header);

  PN_U8                     ConsumerStatusLength;
  PN_U8                     ConsumerStatus;

  PN_U8                     ProviderStatusLength;
  PN_U8                     ProviderStatus;

  PN_U8                     DataLengthHighByte;
  PN_U8                     DataLengthLowByte;

/*
  PN_U8                     Data [DataLength];
*/

} PACK_WORD_ALIGNMENT (T_PROFINET_RECORD_INPUT_DATA);

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

typedef struct _T_PROFINET_SUBSTITUTE_VALUE
{
  T_PROFINET_BLOCK_HEADER       PACK_WORD_ALIGNMENT (Header);

  PN_U8                         SubstitutionModeHighByte;
  PN_U8                         SubstitutionModeLowByte;

/*
  T_PROFINET_OUTPUT_DATA_ITEM   SubstituteData;
*/

} PACK_WORD_ALIGNMENT (T_PROFINET_SUBSTITUTE_VALUE);

/*---------------------------------------------------------------------------*/

typedef struct _T_PROFINET_RECORD_OUTPUT_DATA
{
  T_PROFINET_BLOCK_HEADER       PACK_WORD_ALIGNMENT (Header);

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

} PACK_WORD_ALIGNMENT (T_PROFINET_RECORD_OUTPUT_DATA);

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

} PACK_WORD_ALIGNMENT (T_PROFINET_TIMESTAMP);

/*---------------------------------------------------------------------------*/

typedef struct _T_PROFINET_LOG_ENTRY
{
  T_PROFINET_TIMESTAMP    PACK_WORD_ALIGNMENT (TimeStamp);

  T_PROFINET_UUID         PACK_WORD_ALIGNMENT (ARUUID);
  T_PN_ERROR_CODE         PACK_WORD_ALIGNMENT (Status);

  PN_U8                   DetailsHighWordHighByte;
  PN_U8                   DetailsHighWordLowByte;
  PN_U8                   DetailsLowWordHighByte;
  PN_U8                   DetailsLowWordLowByte;

} PACK_WORD_ALIGNMENT (T_PROFINET_LOG_ENTRY);

/*---------------------------------------------------------------------------*/

typedef struct _T_PROFINET_LOG_DATA
{
  T_PROFINET_BLOCK_HEADER   PACK_WORD_ALIGNMENT (Header);

  T_PROFINET_TIMESTAMP      PACK_WORD_ALIGNMENT (CurrentTimeStamp);

  PN_U8                     NumberOfEntriesHighByte;
  PN_U8                     NumberOfEntriesLowByte;

/*
  T_PROFINET_LOG_ENTRY      Data [NumberOfEntries];
*/

} PACK_WORD_ALIGNMENT (T_PROFINET_LOG_DATA);

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

typedef struct _T_PROFINET_PD_LINE_DELAY
{
  PN_U8   DelayHighWordHighByte;
  PN_U8   DelayHighWordLowByte;
  PN_U8   DelayLowWordHighByte;
  PN_U8   DelayLowWordLowByte;

} PACK_WORD_ALIGNMENT (T_PROFINET_PD_LINE_DELAY);

/*---------------------------------------------------------------------------*/

typedef struct _T_PROFINET_PD_PEER_REAL
{
  T_PROFINET_PD_PEER_PORT     PeerPort;
/*
  T_PROFINET_PEER_CHASSIS     PeerChassis;

  T_PROFINET_PD_LINE_DELAY    Line;
  T_PN_MAC_ADDRESS            MacAddress;
*/

} PACK_BYTE_ALIGNMENT (T_PROFINET_PD_PEER_REAL);

/*---------------------------------------------------------------------------*/

typedef struct _T_PROFINET_PD_MAU_TYPE
{
  PN_U8   TypeHighByte;
  PN_U8   TypeLowByte;

  PN_U8   Padding [2]; /* align to 32-Bit */

} PACK_WORD_ALIGNMENT (T_PROFINET_PD_MAU_TYPE);

/*---------------------------------------------------------------------------*/

typedef struct _T_PROFINET_PD_PORT_SETTINGS
{
  PN_U8   DomainBoundary [4];
  PN_U8   MulticastBoundary [4];

  PN_U8   PortStateHighByte;
  PN_U8   PortStateLowByte;

  PN_U8   Padding [2]; /* align to 32-Bit */

} PACK_WORD_ALIGNMENT (T_PROFINET_PD_PORT_SETTINGS);

/*---------------------------------------------------------------------------*/

typedef struct _T_PROFINET_PD_MEDIA_TYPE
{
  PN_U8   TypeHighWordHighByte;
  PN_U8   TypeHighWordLowByte;
  PN_U8   TypeLowWordHighByte;
  PN_U8   TypeLowWordLowByte;

} PACK_WORD_ALIGNMENT (T_PROFINET_PD_MEDIA_TYPE);

/*---------------------------------------------------------------------------*/

typedef struct _T_CM_PD_PORT_REAL
{
  T_PROFINET_BLOCK_HEADER       PACK_WORD_ALIGNMENT (Header);

  PN_U8                         Padding [2];

  PN_U8                         SlotNumberHighByte;
  PN_U8                         SlotNumberLowByte;

  PN_U8                         SubSlotNumberHighByte;
  PN_U8                         SubSlotNumberLowByte;

  PN_U8                         LengthPortId;
/*
  PN_U8                         PortId [LengthPortId];

  PN_U8                         NumberOfPeers;
  PN_U8                         Padding []; align to 32-Bit

  T_PROFINET_PD_PEER_REAL       PeerReal [NumberOfPeers];
  PN_U8                         Padding []; align to 32-Bit

  T_PROFINET_PD_MAU_TYPE        Mau;
  T_PROFINET_PD_PORT_SETTINGS   PortSettings;
  T_PROFINET_PD_MEDIA_TYPE      Media;

*/

} PACK_BYTE_ALIGNMENT (T_PROFINET_PD_PORT_REAL);

/*===========================================================================*/

typedef struct _T_PROFINET_PD_NC_CHECK
{
  T_PROFINET_BLOCK_HEADER   PACK_WORD_ALIGNMENT (Header);

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

} PACK_WORD_ALIGNMENT (T_PROFINET_PD_NC_CHECK);

/*===========================================================================*/

typedef struct _T_PROFINET_MULTIPLE_HEADER
{
  T_PROFINET_BLOCK_HEADER   PACK_WORD_ALIGNMENT (Header);

  PN_U8                     Padding [2];

  PN_U8                     APIHighWordHighByte;
  PN_U8                     APIHighWordLowByte;
  PN_U8                     APILowWordHighByte;
  PN_U8                     APILowWordLowByte;

  PN_U8                     SlotNumberHighByte;
  PN_U8                     SlotNumberLowByte;

  PN_U8                     SubSlotNumberHighByte;
  PN_U8                     SubSlotNumberLowByte;

} PACK_WORD_ALIGNMENT (T_PROFINET_MULTIPLE_HEADER);

typedef T_PROFINET_MULTIPLE_HEADER    T_PROFINET_PD_REAL_DATA;
typedef T_PROFINET_MULTIPLE_HEADER    T_PROFINET_PD_EXPECTED_DATA;

/*===========================================================================*/

#define C_CONTROL_COMMAND_PRM_END             0x0001u
#define C_CONTROL_COMMAND_APPLICATION_READY   0x0002u
#define C_CONTROL_COMMAND_RELEASE             0x0004u

typedef struct _T_PN_SERVICE_CONTROL_IND
{
  PN_U8   ARIdentifierHighByte;
  PN_U8   ARIdentifierLowByte;

  PN_U8   CommandHighByte;
  PN_U8   CommandLowByte;

} PACK_WORD_ALIGNMENT (T_PN_SERVICE_CONTROL_IND);

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

} PACK_WORD_ALIGNMENT (T_PN_SERVICE_CONTROL_RES);

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

#endif /* __PROFINET_H__ */

/*****************************************************************************/
/* 
vim:nu ts=2 et nowrap ~
*/

