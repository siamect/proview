/*****************************************************************************/
/*                                                                           */
/*                                SOFTING AG                                 */
/*                        Richard-Reitzner-Allee 6                           */
/*                              D-85540 Haar                                 */
/*                      Phone: (++49)-(0)89-45656-0                          */
/*                      Fax:   (++49)-(0)89-45656-399                        */
/*                                                                           */
/*                    Copyright (C) SOFTING AG 1995-2003                     */
/*                            All Rights Reserved                            */
/*                                                                           */
/*****************************************************************************/
/*****************************************************************************/
/*                        DP GLOBAL DEFINES and TYPES                        */
/*                                                                           */
/*  Filename    : PB_DP.H                                                    */
/*  Version     : 5.22.0.00.release                                          */
/*  Date        : 26-February-1999                                           */
/*  Author      : SOFTING AG                                                 */
/*                                                                           */
/*  Description : This file contains the global defines and types of         */
/*                component DP                                               */
/*                                                                           */
/*****************************************************************************/

#ifndef __PB_DP__
#define __PB_DP__


#if defined (WIN32) || defined (_WIN32) || defined (WIN16) || defined (_WIN16)
#pragma warning (disable : 4103)     /* used #pragma pack to change alignment */
#ifdef WIN32
#pragma pack(push,1)
#else
#pragma pack(1)
#endif
#pragma warning (default : 4103)
#endif


/****************************************************************************/
/*** DEFINES ****************************************************************/
/****************************************************************************/

/*--- COMMON DP CONSTANTS --------------------------------------------------*/

#define DP_MAX_NUMBER_STATIONS      127
#define DP_MAX_NUMBER_SLAVES        125

#define DP_GLOBAL_STATION_ADDRESS   127
#define DP_DEFAULT_SLAVE_ADDRESS    126
#define DP_NO_MASTER_ADDRESS        255
#define DP_MAX_SLAVE_ADDRESS        125

#define DP_MAX_TELEGRAM_LEN         244                /* 246 not supported */
#define DP_STATUS_INFO_LEN          128

#define DP_MASTER_USER_DATA_LEN     34
#define DP_MASTER_CLASS2_NAME_LEN   32

#define DP_MIN_SLAVE_DIAG_LEN       6     /* min/max length of service data */
#define DP_MIN_PRM_DATA_LEN         9
#define DP_MIN_CFG_DATA_LEN         3
#define DP_MIN_SLAVE_PARA_LEN       32
#define DP_MIN_BUS_PARA_LEN         66

#define DP_MAX_SLAVE_DIAG_DATA_LEN  244   /* SLAVE- != MASTER- MAX_DIAG_LEN */
#define DP_MAX_MASTER_DIAG_DATA_LEN 242
#define DP_MAX_EXT_DIAG_DATA_LEN    238
#define DP_MAX_OUTPUT_DATA_LEN      244
#define DP_MAX_INPUT_DATA_LEN       244
#define DP_MAX_PRM_DATA_LEN         244
#define DP_MAX_CFG_DATA_LEN         244
#define DP_MAX_AAT_DATA_LEN         492
#define DP_MAX_USER_PRM_DATA_LEN    234
#define DP_MAX_DOWNLOAD_DATA_LEN    240
#define DP_MAX_UPLOAD_DATA_LEN      240
#define DP_MAX_REM_SLAVE_DATA_LEN   238
#define DP_MAX_SLAVE_USER_DATA_LEN  65470
#define DP_MAX_MASTER_USER_DATA_LEN 65470
#define DP_MAX_SET_BUSPARAMETER_LEN 196
#define DP_MAX_MSAC2_DATA_LEN       220

/****************************************************************************/
/* DP USER SERVICE CODES                                                    */
/****************************************************************************/

/*--- DP USER SERVICE IDENTIFIERS ------------------------------------------*/

#define DP_SERVICE_USIF             0x80
#define DP_SERVICE_DDLM             0x00

#define DP_SERVICE_SERV             (DP_SERVICE_USIF | 0x60)
#define DP_SERVICE_SCHED            (DP_SERVICE_USIF | 0x40)
#define DP_SERVICE_SLAVE            (DP_SERVICE_USIF | 0x20)

#define DP_SERVICE_MM               (DP_SERVICE_DDLM | 0x60)
#define DP_SERVICE_MSAC2            (DP_SERVICE_DDLM | 0x40)

/*--- DP SERVICE HANDLER SERVICES ------------------------------------------*/

#define DP_DOWNLOAD_LOC             (DP_SERVICE_SERV | 0x01)
#define DP_UPLOAD_LOC               (DP_SERVICE_SERV | 0x02)
#define DP_START_SEQ_LOC            (DP_SERVICE_SERV | 0x03)
#define DP_END_SEQ_LOC              (DP_SERVICE_SERV | 0x04)
#define DP_GET_SLAVE_DIAG           (DP_SERVICE_SERV | 0x05)
#define DP_SET_PRM_LOC              (DP_SERVICE_SERV | 0x06)
#define DP_GET_MASTER_DIAG_LOC      (DP_SERVICE_SERV | 0x07)
#define DP_GET_SLAVE_PARAM          (DP_SERVICE_SERV | 0x08)
#define DP_SET_BUSPARAMETER         (DP_SERVICE_SERV | 0x09)
#define DP_SET_MASTER_PARAM         (DP_SERVICE_SERV | 0x0A)

/*--- DP SCHEDULER SERVICES ------------------------------------------------*/

#define DP_INIT_MASTER              (DP_SERVICE_SCHED | 0x01)
#define DP_ACT_PARAM_LOC            (DP_SERVICE_SCHED | 0x02)
#define DP_DATA_TRANSFER            (DP_SERVICE_SCHED | 0x03)
#define DP_EXIT_MASTER              (DP_SERVICE_SCHED | 0x04)
#define DP_CONFIGURATION            (DP_SERVICE_SCHED | 0x05)
#define DP_TERMINATION              (DP_SERVICE_SCHED | 0x06)

/*--- DP DDLM SERVICES -----------------------------------------------------*/

#define DP_SET_PRM                  (DP_SERVICE_DDLM | 0x00)    /* SRD: M/S */
#define DP_CHK_CFG                  (DP_SERVICE_DDLM | 0x01)
#define DP_GET_CFG                  (DP_SERVICE_DDLM | 0x02)
#define DP_SLAVE_DIAG               (DP_SERVICE_DDLM | 0x03)
#define DP_RD_INP                   (DP_SERVICE_DDLM | 0x04)
#define DP_RD_OUTP                  (DP_SERVICE_DDLM | 0x05)
#define DP_DATA_EXCHANGE            (DP_SERVICE_DDLM | 0x06)
#define DP_SET_SLAVE_ADD            (DP_SERVICE_DDLM | 0x07)

#define DP_DOWNLOAD                 (DP_SERVICE_MM   | 0x08)    /* SRD: M/M */
#define DP_UPLOAD                   (DP_SERVICE_MM   | 0x09)
#define DP_START_SEQ                (DP_SERVICE_MM   | 0x0A)
#define DP_END_SEQ                  (DP_SERVICE_MM   | 0x0B)
#define DP_ACT_PARAM                (DP_SERVICE_MM   | 0x0C)
#define DP_GET_MASTER_DIAG          (DP_SERVICE_MM   | 0x0D)

#define DP_GLOBAL_CONTROL           (DP_SERVICE_DDLM | 0x0E)    /* SDN: M/S */
#define DP_ACT_PARA_BRCT            (DP_SERVICE_MM   | 0x0F)    /* SDN: M/M */

#define DP_FAULT                    (DP_SERVICE_DDLM | 0x10)       /* local */

/*--- DP DDLM SERVICE FOR DP/V1 --------------------------------------------*/

#define DP_INITIATE                 (DP_SERVICE_MSAC2 | 0x00)
#define DP_ABORT                    (DP_SERVICE_MSAC2 | 0x01)
#define DP_READ                     (DP_SERVICE_MSAC2 | 0x02)
#define DP_WRITE                    (DP_SERVICE_MSAC2 | 0x03)
#define DP_DATA_TRANSPORT           (DP_SERVICE_MSAC2 | 0x04)

/****************************************************************************/
/* DP SERVICE CONSTANTS                                                     */
/****************************************************************************/

/*--- MASTER AUTOMATIC REMOTE SERVICES -------------------------------------*/

#define DP_AUTO_GET_MASTER_DIAG     0x80
#define DP_AUTO_UPLOAD_DOWNLOAD_SEQ 0x40
#define DP_AUTO_ACT_PARAM           0x20

#define DP_AUTO_REMOTE_SERVICES     ( DP_AUTO_GET_MASTER_DIAG     |          \
                                      DP_AUTO_UPLOAD_DOWNLOAD_SEQ |          \
                                      DP_AUTO_ACT_PARAM )

#define DP_USER_REMOTE_SERVICES     0x00

/*--- SLAVE DPRAM ADDRESS ASSIGNMENT MODES ---------------------------------*/

#define DP_AAM_ARRAY                0x00
#define DP_AAM_DEFINED              0x01
#define DP_AAM_COMPACT              0x02
#define DP_AAM_IO_BLOCKS            0x03

/*--- DP SLAVE FLAGS -------------------------------------------------------*/

#define DP_SL_ACTIVE                0x80
#define DP_SL_NEW_PRM               0x40
#define DP_SL_FAIL_SAFE             0x20

#define DP_SL_FLAGS                 ( DP_SL_ACTIVE    |                      \
                                      DP_SL_NEW_PRM   |                      \
                                      DP_SL_FAIL_SAFE )

/*--- DP BUS PARAMETER FLAGS -----------------------------------------------*/

#define DP_BP_ERROR_ACTION          0x80

#define DP_BP_FLAGS                 (DP_BP_ERROR_ACTION)

/*--- DP SLAVE TYPES -------------------------------------------------------*/

#define DP_SLAVE_TYPE_DP            0x00           /* default DP Slave type */

/*--- DP SLAVE PARAMETERIZATION STATION STATUS -----------------------------*/

#define DP_PRM_LOCK_REQ             0x80
#define DP_PRM_UNLOCK_REQ           0x40
#define DP_PRM_SYNC_REQ             0x20
#define DP_PRM_FREEZE_REQ           0x10
#define DP_PRM_WD_ON                0x08

#define DP_PRM_STATION_STATUS       ( DP_PRM_LOCK_REQ   |                    \
                                      DP_PRM_UNLOCK_REQ |                    \
                                      DP_PRM_SYNC_REQ   |                    \
                                      DP_PRM_FREEZE_REQ |                    \
                                      DP_PRM_WD_ON )

/*--- DP SLAVE GLOBAL CONTROL BITS -----------------------------------------*/

#define DP_CONTROL_SYNC             0x20
#define DP_CONTROL_UNSYNC           0x10
#define DP_CONTROL_FREEZE           0x08
#define DP_CONTROL_UNFREEZE         0x04
#define DP_CONTROL_CLEAR_DATA       0x02

#define DP_CONTROL_COMMAND          ( DP_CONTROL_SYNC       |               \
                                      DP_CONTROL_UNSYNC     |               \
                                      DP_CONTROL_FREEZE     |               \
                                      DP_CONTROL_UNFREEZE   |               \
                                      DP_CONTROL_CLEAR_DATA )

/*--- DP AREA CODES --------------------------------------------------------*/

#define DP_AREA_SLAVE_PARAM         0x00
#define DP_AREA_BUS_PARAM           0x7F
#define DP_AREA_SET_MODE            0x80
#define DP_AREA_STAT_COUNT          0x81
#define DP_AREA_NO_PROTECTION       0xFF

/*--- DP AREA / ACTIVATION CODES -------------------------------------------*/

#define DP_SLAVE_ACTIVATE           0x80  /* values are area code dependent */
#define DP_SLAVE_DEACTIVATE         0x00

#define DP_BUS_PAR_ACTIVATE         0xFF

#define DP_OP_MODE_OFFLINE          0x00
#define DP_OP_MODE_STOP             0x40
#define DP_OP_MODE_CLEAR            0x80
#define DP_OP_MODE_RED_CLEAR        0x81            /* new redundancy state */
#define DP_OP_MODE_OPERATE          0xC0
#define DP_OP_MODE_RED_OPERATE      0xC1            /* new redundancy state */

/*--- DP MASTER DIAG IDENTIFIERS -------------------------------------------*/

#define DP_DIAG_SLAVE_DATA          0x00
#define DP_DIAG_SYSTEM_DIAGNOSTIC   0x7E
#define DP_DIAG_MASTER_STATUS       0x7F
#define DP_DIAG_DATA_TRANSFER_LIST  0x80

/*--- DP SLAVE PARAM IDENTIFIERS -------------------------------------------*/

#define DP_SLAVE_PARAM_HEADER       0x01
#define DP_SLAVE_PARAM_PRM_DATA     0x02
#define DP_SLAVE_PARAM_CFG_DATA     0x03
#define DP_SLAVE_PARAM_AAT_DATA     0x04
#define DP_SLAVE_PARAM_USER_DATA    0x05
#define DP_SLAVE_PARAM_SLAVE_INFO   0x06
#define DP_SLAVE_PARAM_SYS_INFO     0x07
#define DP_SLAVE_PARAM_FLUSH_DIAG   0x08

/*--- DP SET MASTER PARAM IDENTIFIERS --------------------------------------*/

#define DP_SET_IDENT_NUMBER         0x00

/*--- DP SLAVE DIAG BITS ---------------------------------------------------*/

#define DP_DIAG_1_MASTER_LOCK           0x80     /* influenced by DP Master */
#define DP_DIAG_1_PRM_FAULT             0x40
#define DP_DIAG_1_INVALID_SLAVE_RES     0x20     /* influenced by DP Master */
#define DP_DIAG_1_NOT_SUPPORTED         0x10
#define DP_DIAG_1_EXT_DIAG              0x08
#define DP_DIAG_1_CFG_FAULT             0x04
#define DP_DIAG_1_STATION_NOT_READY     0x02
#define DP_DIAG_1_STATION_NON_EXISTENT  0x01     /* influenced by DP Master */

#define DP_DIAG_1_STATUS                0xFF

#define DP_DIAG_2_DEACTIVATED           0x80     /* influenced by DP Master */
#define DP_DIAG_2_SYNC_MODE             0x20
#define DP_DIAG_2_FREEZE_MODE           0x10
#define DP_DIAG_2_WD_ON                 0x08
#define DP_DIAG_2_DEFAULT               0x04
#define DP_DIAG_2_STAT_DIAG             0x02
#define DP_DIAG_2_PRM_REQ               0x01

#define DP_DIAG_2_STATUS                ( DP_DIAG_2_DEACTIVATED |            \
                                          DP_DIAG_2_SYNC_MODE   |            \
                                          DP_DIAG_2_FREEZE_MODE |            \
                                          DP_DIAG_2_WD_ON       |            \
                                          DP_DIAG_2_PRM_REQ )

#define DP_DIAG_3_EXT_DIAG_OVERFLOW     0x80

#define DP_DIAG_3_STATUS                ( DP_DIAG_3_EXT_DIAG_OVERFLOW )

#define DP_SLAVE_DIAG_OVERFLOW          (-1)
#define DP_STATION_NON_EXISTENT         0x0100
#define DP_INVALID_SLAVE_RESPONSE       0x2000

/*--- DP STATION STATUS ----------------------------------------------------*/

#define DP_STATE_STATION_TYPE           0x80         /* station status bits */

#define DP_STATE_MASTER_STATION         0x80
#define DP_STATE_SLAVE_STATION          0x00

#define DP_STATE_SLAVE_DIAG_DATA        0x02
#define DP_STATE_MASTER_DIAG_DATA       0x02

#define DP_STATE_SLAVE_ERROR            0x01
#define DP_STATE_MASTER_UP_DOWN_LOAD    0x01

/*--- DP BAUDRATES ---------------------------------------------------------*/

#define DP_KBAUD_9_6                0x00
#define DP_KBAUD_19_2               0x01
#define DP_KBAUD_45_45              0x0B
#define DP_KBAUD_93_75              0x02
#define DP_KBAUD_187_5              0x03
#define DP_KBAUD_500                0x04
#define DP_KBAUD_750                0x05
#define DP_KBAUD_1500               0x06
#define DP_MBAUD_1_5                0x06
#define DP_KBAUD_3000               0x07
#define DP_MBAUD_3                  0x07
#define DP_KBAUD_6000               0x08
#define DP_MBAUD_6                  0x08
#define DP_KBAUD_12000              0x09
#define DP_MBAUD_12                 0x09

/****************************************************************************/
/* DP DATA STRUCTURE DEFINITIONS                                            */
/****************************************************************************/
/* PCI: Protocol Control Information                                        */
/* SDU: Service Data Unit                                                   */
/* PDU: Protocol Data Unit = PCI + SDU                                      */
/*                                                                          */
/* !!! DATA STRUCTURES: MOTOROLA BYTE ORDERING (HIGH, LOW) !!!              */
/****************************************************************************/

/*--- DP SLAVE PARAMETERIZATION DATA ---------------------------------------*/

typedef struct _T_DP_PRM_DATA
{
   USIGN16    prm_data_len;                       /* 9..DP_MAX_PRM_DATA_LEN */
   OCTET      station_status;                                 /* DP_PRM_xxx */
   OCTET      wd_fact_1;                          /* watch dog time factors */
   OCTET      wd_fact_2;
   OCTET      min_tsdr;               /* delay of DP-Slaves before response */
   USIGN16    ident_number;                             /* PNO ident number */
   OCTET      group_ident;                     /* groups for GLOBAL_CONTROL */
/* OCTET      user_prm_data [DP_MAX_USER_PRM_DATA_LEN];                     */

} T_DP_PRM_DATA;

/*--------------------------------------------------------------------------*/

typedef struct _T_DP_CFG_DATA
{
   USIGN16    cfg_data_len;                                       /* 2..244 */
/* OCTET      cfg_data [cfg_data_len - 2];                                  */

} T_DP_CFG_DATA;

/*--------------------------------------------------------------------------*/

typedef struct _T_DP_AAT_DATA
{
   USIGN16    aat_data_len;                                   /* 2..2^16-31 */
   USIGN8     number_inputs;                                      /* 0..244 */
   USIGN8     number_outputs;                                     /* 0..244 */
/* USIGN16    offset_input  [0..244];                                       */
/* USIGN16    offset_output [0..244];                                       */

} T_DP_AAT_DATA;

/*--------------------------------------------------------------------------*/

typedef struct _T_DP_SLAVE_USER_DATA
{
   USIGN16    slave_user_data_len;                            /* 2..2^16-31 */
/* OCTET      slave_user_data [slave_user_data_len - 2];                    */

} T_DP_SLAVE_USER_DATA;

/*--------------------------------------------------------------------------*/

typedef struct _T_DP_SLAVE_PARA_SET
{
   USIGN16          slave_para_len;                /* 0..MAX_SLAVE_PARA_LEN */
   USIGN8           sl_flag;            /* DP_SL_ACTIVE, DP_SL_FLAG_NEW_PRM */
   USIGN8           slave_type;                         /* DP_SLAVE_TYPE_DP */
   OCTET            reserved [12];
/* T_DP_PRM_DATA    prm_data;                                               */
/* T_DP_CFG_DATA    cfg_data;                                               */
/* T_DP_AAT_DATA    aat_data;                                               */
/* T_DP_USER_DATA   user_data;                                              */

} T_DP_SLAVE_PARA_SET;

/*==========================================================================*/

typedef struct _T_DP_BUS_PARA_SET
{
   USIGN16     bus_para_len;                                  /* 66..2^16-1 */
   USIGN8      fdl_add;                                           /* 0..125 */
   USIGN8      baud_rate;                                   /* DP_xBAUD_xxx */
   USIGN16     tsl;                          /* see DIN 19245-1 for details */
   USIGN16     min_tsdr;
   USIGN16     max_tsdr;
   USIGN8      tqui;
   USIGN8      tset;
   USIGN32     ttr;
   USIGN8      g;
   USIGN8      hsa;
   USIGN8      max_retry_limit;
   USIGN8      bp_flag;                               /* DP_BP_ERROR_ACTION */
   USIGN16     min_slave_interval;                /* 1..2^16-1 [100 micros] */
   USIGN16     poll_timeout;                            /* 1..2^16-1 [1 ms] */
   USIGN16     data_control_time;                      /* 1..2^16-1 [10 ms] */
   OCTET       reserved [6];
   USIGN16     master_user_data_len;                         /* 34..2^16-33 */
   STRINGV     master_class2_name [32];  /* DP Master (class 2) vendor name */
/* OCTET       master_user_data [DP_MASTER_USER_DATA_LEN - 34];             */

} T_DP_BUS_PARA_SET;

/*==========================================================================*/

typedef struct _T_DP_DIAG_DATA
{
   OCTET      station_status_1;                            /* DP_DIAG_1_xxx */
   OCTET      station_status_2;                            /* DP_DIAG_2_xxx */
   OCTET      station_status_3;                            /* DP_DIAG_3_xxx */
   USIGN8     master_add;     /* DP Master which parameterized the DP Slave */
   USIGN16    ident_number;             /* PNO ident number of the DP Slave */
/* OCTET      ext_diag_data [DP_MAX_EXT_DIAG_DATA_LEN];                     */

} T_DP_DIAG_DATA;

/*--------------------------------------------------------------------------*/

typedef struct _T_DP_MASTER_STATUS
{
  OCTET     usif_state;                                   /* DP_OP_MODE_xxx */
  USIGN8    ident_number_high;      /* alignment problem in DIN 19245-3 !!! */
  USIGN8    ident_number_low;    /* USIGN16 ident_number at odd address !!! */
  OCTET     dp_hardware_version;           /* DDLM/USIF release information */
  OCTET     dp_firmware_version;
  OCTET     user_hardware_version;              /* USER release information */
  OCTET     user_firmware_version;
  OCTET     reserved [9];

} T_DP_MASTER_STATUS;

/*--------------------------------------------------------------------------*/

typedef struct _T_DP_SLAVE_PARAM_SLAVE_INFO
{
  USIGN16   diag_entries;                        /* available DIAG messages */
  USIGN16   offset_inputs;        /* offset of input area within I/O memory */
  USIGN16   offset_outputs;      /* offset of output area within I/O memory */
  USIGN8    number_inputs;                          /* inputs of I/O memory */
  USIGN8    number_outputs;                        /* outputs of I/O memory */
  USIGN8    sl_flag;                    /* DP_SL_ACTIVE, DP_SL_FLAG_NEW_PRM */
  USIGN8    slave_type;                                 /* DP_SLAVE_TYPE_DP */

} T_DP_SLAVE_PARAM_SLAVE_INFO;

/*--------------------------------------------------------------------------*/

typedef struct _T_DP_SLAVE_PARAM_SYS_INFO
{
  USIGN8    loaded_slaves;                      /* number downloaded slaves */
  USIGN8    active_slaves;                          /* number active slaves */
  INT16     diag_entries;                 /* number available DIAG messages */
  USIGN16   slave_io_image_len;               /* length of slave I/O memory */

} T_DP_SLAVE_PARAM_SYS_INFO;

/*--------------------------------------------------------------------------*/

typedef struct _T_DP_SET_IDENT_NUMBER
{
  USIGN16   ident_number;                        /* new master ident_number */
  OCTET     dp_hardware_version;     /* vendor specific release information */
  OCTET     dp_firmware_version;
  OCTET     user_hardware_version;
  OCTET     user_firmware_version;

} T_DP_SET_IDENT_NUMBER;

/*--------------------------------------------------------------------------*/
/*--- DP USIF SERVICE STRUCTURES -------------------------------------------*/
/*--------------------------------------------------------------------------*/

typedef struct _T_DP_INIT_MASTER_REQ
{
  USIGN8    master_default_address;       /* station address for default BP */
  PB_BOOL   master_class2;              /* use M2 requester functionality ? */
  STRINGV   master_class2_name [32];                   /* vendor name of M2 */

  USIGN8    lowest_slave_address;      /* station address of first DP Slave */

  USIGN8    slave_io_address_mode;  /* Address Assignment Mode of slave I/O */

  PB_BOOL   clear_outputs;      /* clear output areas in DP_OP_MODE_CLEAR ? */
  USIGN8    auto_remote_services;                            /* DP_AUTO_xxx */
  PB_BOOL   cyclic_data_transfer;    /* no DATA_TRANSFER service requests ? */

  USIGN8    dummy;                                        /* alignment byte */

} T_DP_INIT_MASTER_REQ;

/*--------------------------------------------------------------------------*/

typedef struct _T_DP_INIT_MASTER_CON
{
  USIGN16   status;                                           /* OK, IV, NO */

} T_DP_INIT_MASTER_CON;

/*==========================================================================*/

typedef struct _T_DP_EXIT_MASTER_CON
{
  USIGN16   status;                                               /* OK, NO */

} T_DP_EXIT_MASTER_CON;

/*==========================================================================*/

typedef struct _T_DP_START_SEQ_REQ          /* MM: function_num --> rem_add */
{
  USIGN8    rem_add;                 /* station address DP Master (class 1) */
  USIGN8    area_code;                                       /* DP_AREA_xxx */
  USIGN16   timeout;                              /* download timeout [1ms] */

} T_DP_START_SEQ_REQ;

/*--------------------------------------------------------------------------*/

typedef struct _T_DP_START_SEQ_IND          /* MM: req_add --> function_num */
{
  USIGN8    req_add;                 /* station address DP Master (class 2) */
  USIGN8    area_code;                                       /* DP_AREA_xxx */
  USIGN16   timeout;                              /* download timeout [1ms] */

} T_DP_START_SEQ_IND;

/*--------------------------------------------------------------------------*/

typedef struct _T_DP_START_SEQ_RES_CON
{
  USIGN16   status;                     /* status of start sequence service */
  USIGN8    max_len_data_unit;                                    /* 1..240 */
  USIGN8    dummy;                                    /* SDU alignment byte */

} T_DP_START_SEQ_RES_CON;

/*==========================================================================*/

typedef struct _T_DP_END_SEQ_REQ            /* MM: function_num --> rem_add */
{
  USIGN8    rem_add;                 /* station address DP Master (class 1) */
  USIGN8    dummy;                                    /* SDU alignment byte */

} T_DP_END_SEQ_REQ;

/*--------------------------------------------------------------------------*/

typedef struct _T_DP_END_SEQ_IND            /* MM: req_add --> function_num */
{
  USIGN8    req_add;                 /* station address DP Master (class 2) */
  USIGN8    dummy;                                    /* SDU alignment byte */

} T_DP_END_SEQ_IND;

/*--------------------------------------------------------------------------*/

typedef struct _T_DP_END_SEQ_RES_CON
{
  USIGN16   status;                  /* OK, DS, NA, RS, RR, UE, TO, FE, RE, */
                                          /* NE, AD, IP, NI, SE, SC, EA, LE */

} T_DP_END_SEQ_RES_CON;

/*--------------------------------------------------------------------------*/

typedef struct _T_DP_END_SEQ_LOC_IND
{
  USIGN16   status;                                                   /* TO */

} T_DP_END_SEQ_LOC_IND;

/*==========================================================================*/

typedef struct _T_DP_DOWNLOAD_REQ           /* MM: function_num --> rem_add */
{
   USIGN16  data_len;                        /* 0..DP_MAX_DOWNLOAD_DATA_LEN */
   USIGN8   rem_add;                 /* station address DP Master (class 1) */
   USIGN8   area_code;              /* destination area of download service */
   USIGN16  add_offset;                 /* starting offset within this area */
/* OCTET    data [data_len];                                  download data */

} T_DP_DOWNLOAD_REQ;

/*--------------------------------------------------------------------------*/

typedef struct _T_DP_DOWNLOAD_IND           /* MM: req_add --> function_num */
{
   USIGN16  data_len;                        /* 0..DP_MAX_DOWNLOAD_DATA_LEN */
   USIGN8   req_add;                 /* station address DP Master (class 2) */
   USIGN8   area_code;              /* destination area of download service */
   USIGN16  add_offset;                 /* starting offset within this area */
/* OCTET    data [data_len];                                  download data */

} T_DP_DOWNLOAD_IND;

/*--------------------------------------------------------------------------*/

typedef struct _T_DP_DOWNLOAD_RES_CON
{
  USIGN16   status;                      /* OK, DS, NA, RS, RR, UE, TO, FE, */
                                              /* RE, NE, AD, EA, SC, NI, LE */

} T_DP_DOWNLOAD_RES_CON;

/*==========================================================================*/

typedef struct _T_DP_UPLOAD_REQ             /* MM: function_num --> rem_add */
{
  USIGN8   rem_add;                  /* station address DP Master (class 1) */
  USIGN8   area_code;               /* destination area of download service */
  USIGN16  add_offset;                  /* starting offset within this area */
  USIGN8   data_len;                                              /* 1..240 */
  USIGN8   dummy;                                     /* SDU alignment byte */

} T_DP_UPLOAD_REQ;

/*--------------------------------------------------------------------------*/

typedef struct _T_DP_UPLOAD_IND             /* MM: req_add --> function_num */
{
  USIGN8   req_add;                  /* station address DP Master (class 2) */
  USIGN8   area_code;               /* destination area of download service */
  USIGN16  add_offset;                  /* starting offset within this area */
  USIGN8   data_len;                                              /* 1..240 */
  USIGN8   dummy;                                     /* SDU alignment byte */

} T_DP_UPLOAD_IND;

/*--------------------------------------------------------------------------*/

typedef struct _T_DP_UPLOAD_RES_CON
{
   USIGN16   status;                     /* OK, DS, NA, RS, RR, UE, TO, FE, */
                                              /* RE, NE, AD, EA, SC, NI, LE */
   USIGN16   data_len;                         /* 1..DP_MAX_UPLOAD_DATA_LEN */
/* OCTET     data [data_len];                                   upload data */

} T_DP_UPLOAD_RES_CON;

/*==========================================================================*/

typedef struct _T_DP_ACT_PARAM_REQ          /* MM: function_num --> rem_add */
{
  USIGN8    rem_add;                 /* station address DP Master (class 1) */
  USIGN8    area_code;                                       /* DP_AREA_xxx */
  USIGN8    activate;  /* DP_SLAVE_xxx, DP_BUS_PAR_ACTIVATE, DP_OP_MODE_xxx */
  USIGN8    dummy;                                    /* SDU alignment byte */

} T_DP_ACT_PARAM_REQ;

/*--------------------------------------------------------------------------*/

typedef struct _T_DP_ACT_PARAM_IND          /* MM: req_add --> function_num */
{
  USIGN8    req_add;                 /* station address DP Master (class 2) */
  USIGN8    area_code;                                       /* DP_AREA_xxx */
  USIGN8    activate;  /* DP_SLAVE_xxx, DP_BUS_PAR_ACTIVATE, DP_OP_MODE_xxx */
  USIGN8    dummy;                                    /* SDU alignment byte */

} T_DP_ACT_PARAM_IND;

/*--------------------------------------------------------------------------*/

typedef struct _T_DP_ACT_PARAM_RES_CON
{
  USIGN16   status;                  /* OK, DS, NA, RS, RR, UE, TO, FE, RE, */
                                          /* NE, AD, IP, SC, NI, DI, EA, LE */

} T_DP_ACT_PARAM_RES_CON;

/*==========================================================================*/

typedef struct _T_DP_GET_MASTER_DIAG_REQ    /* MM: function_num --> rem_add */
{
  USIGN8    rem_add;                 /* station address DP Master (class 1) */
  USIGN8    identifier;                                      /* DP_DIAG_xxx */

} T_DP_GET_MASTER_DIAG_REQ;

/*--------------------------------------------------------------------------*/

typedef struct _T_DP_GET_MASTER_DIAG_IND    /* MM: req_add --> function_num */
{
  USIGN8    req_add;                 /* station address DP Master (class 2) */
  USIGN8    identifier;                                      /* DP_DIAG_xxx */

} T_DP_GET_MASTER_DIAG_IND;

/*--------------------------------------------------------------------------*/

typedef struct _T_DP_GET_MASTER_DIAG_RES_CON
{
   USIGN16   status;                         /* OK, DS, NA, RS, RR, UE, TO, */
                                              /* FE, RE, NE, AD, IP, EA, LE */
   USIGN16   data_len;                    /* 1..DP_MAX_MASTER_DIAG_DATA_LEN */
/* OCTET     diagnostic_data [data_len];                                    */

} T_DP_GET_MASTER_DIAG_RES_CON;

/*==========================================================================*/

typedef VOID T_DP_GET_SLAVE_DIAG_REQ;

/*--------------------------------------------------------------------------*/

typedef struct _T_DP_GET_SLAVE_DIAG_CON
{
   USIGN16          status;                                       /* OK, NO */
   USIGN8           rem_add;                                      /* 0..126 */
   USIGN8           dummy;                            /* SDU alignment byte */
   INT16            diag_entries;          /* -1..DP_MAX_SLAVE_DIAG_ENTRIES */
   USIGN16          diag_data_len;         /* 0..DP_MAX_SLAVE_DIAG_DATA_LEN */
/* T_DP_DIAG_DATA   diag_data;                                diag_data_len */

} T_DP_GET_SLAVE_DIAG_CON;

/*--------------------------------------------------------------------------*/

typedef T_DP_GET_SLAVE_DIAG_CON    T_DP_GET_SLAVE_DIAG_IND;         /* dito */

/*==========================================================================*/

typedef VOID T_DP_DATA_TRANSFER_REQ;                              /* no SDU */

/*--------------------------------------------------------------------------*/

typedef struct _T_DP_DATA_TRANSFER_CON
{
  USIGN16   status;                                               /* OK, NO */
  INT16     diag_entries;                  /* -1..DP_MAX_SLAVE_DIAG_ENTRIES */

} T_DP_DATA_TRANSFER_CON;

/*==========================================================================*/

typedef struct _T_DP_GET_SLAVE_PARAM_REQ
{
  USIGN8    identifier;                               /* DP_SLAVE_PARAM_xxx */
  USIGN8    rem_add;                                              /* 0..126 */

} T_DP_GET_SLAVE_PARAM_REQ;

/*--------------------------------------------------------------------------*/

typedef struct _T_DP_GET_SLAVE_PARAM_CON
{
   USIGN16   status;                                      /* OK, NO, IV, NE */
   USIGN16   data_len;                                            /* 0..244 */
/* OCTET     slave_param_data [data_len];                          data_len */

} T_DP_GET_SLAVE_PARAM_CON;

/*==========================================================================*/

typedef struct _T_DP_SET_BUSPARAMETER_REQ
{
   USIGN8     bp_flag;                                /* DP_BP_ERROR_ACTION */
   USIGN8     dummy;                                      /* alignment byte */
   USIGN16    min_slave_interval;                 /* 1..2^16-1 [100 micros] */
   USIGN16    poll_timeout;                             /* 1..2^16-1 [1 ms] */
   USIGN16    data_control_time;                       /* 1..2^16-1 [10 ms] */
   USIGN16    master_user_data_len;      /* 34..DP_MAX_SET_BUSPARAMETER_LEN */
   STRINGV    master_class2_name [32];   /* DP Master (class 2) vendor name */
/* OCTET      master_user_data [0..DP_MAX_SET_BUSPARAMETER_LEN - 34];       */

} T_DP_SET_BUSPARAMETER_REQ;

/*--------------------------------------------------------------------------*/

typedef struct _T_DP_SET_BUSPARAMETER_CON
{
  USIGN16   status;                                           /* OK, IV, NO */

} T_DP_SET_BUSPARAMETER_CON;

/*==========================================================================*/

typedef struct _T_DP_SET_MASTER_PARAM_REQ
{
   USIGN8   identifier;                                       /* DP_SET_xxx */
   USIGN8   data_len;                             /* 0..DP_MAX_TELEGRAM_LEN */
/* OCTET    data [data_len];                   data dependent on identifier */

} T_DP_SET_MASTER_PARAM_REQ;

/*--------------------------------------------------------------------------*/

typedef struct _T_DP_SET_MASTER_PARAM_CON
{
  USIGN16    status;                                          /* OK, IV, NO */

} T_DP_SET_MASTER_PARAM_CON;

/*--------------------------------------------------------------------------*/
/*--- DP DDLM SERVICE STRUCTURES -------------------------------------------*/
/*--------------------------------------------------------------------------*/

typedef struct _T_DP_SLAVE_DIAG_REQ
{
  USIGN8    rem_add;                                              /* 0..126 */
  USIGN8    dummy;                                    /* SDU alignment byte */

} T_DP_SLAVE_DIAG_REQ;

/*--------------------------------------------------------------------------*/

typedef struct _T_DP_SLAVE_DIAG_CON
{
   USIGN16   status;                          /* OK, DS, NA, RS, UE, NR, RE */
   USIGN8    rem_add;                                             /* 0..126 */
   USIGN8    dummy;                                   /* PCI alignment byte */
   USIGN16   diag_data_len;                /* 0..DP_MAX_SLAVE_DIAG_DATA_LEN */
/* OCTET     diag_data [diag_data_len];                                     */

} T_DP_SLAVE_DIAG_CON;

/*==========================================================================*/

typedef struct _T_DP_DATA_EXCHANGE_REQ
{
   USIGN8    rem_add;                                             /* 0..126 */
   USIGN8    dummy;                                   /* PCI alignment byte */
   USIGN16   outp_data_len;                    /* 0..DP_MAX_OUTPUT_DATA_LEN */
/* OCTET     outp_data [outp_data_len];                                     */

} T_DP_DATA_EXCHANGE_REQ;

/*--------------------------------------------------------------------------*/

typedef struct _T_DP_DATA_EXCHANGE_CON
{
   USIGN16   status;                          /* OK, DS, NA, RS, RR, UE, RE */
   USIGN8    rem_add;                                             /* 0..126 */
   PB_BOOL   diag_flag;      /* DP_TRUE: DP Slave diagnostic data available */
   USIGN16   inp_data_len;                      /* 0..DP_MAX_INPUT_DATA_LEN */
/* OCTET     inp_data [inp_data_len];                                       */

} T_DP_DATA_EXCHANGE_CON;

/*==========================================================================*/

typedef struct _T_DP_RD_INP_REQ
{
  USIGN8    rem_add;                                              /* 0..126 */
  USIGN8    dummy;                                    /* SDU alignment byte */

} T_DP_RD_INP_REQ;

/*--------------------------------------------------------------------------*/

typedef struct _T_DP_RD_INP_CON
{
   USIGN16   status;                          /* OK, DS, NA, RS, UE, NR, RE */
   USIGN8    rem_add;                                             /* 0..126 */
   USIGN8    dummy;                                   /* PCI alignment byte */
   USIGN16   inp_data_len;                      /* 0..DP_MAX_INPUT_DATA_LEN */
/* OCTET     inp_data [inp_data_len];                                       */

} T_DP_RD_INP_CON;

/*==========================================================================*/

typedef struct _T_DP_RD_OUTP_REQ
{
  USIGN8    rem_add;                                              /* 0..126 */
  USIGN8    dummy;                                    /* SDU alignment byte */

} T_DP_RD_OUTP_REQ;

/*--------------------------------------------------------------------------*/

typedef struct _T_DP_RD_OUTP_CON
{
   USIGN16   status;                          /* OK, DS, NA, RS, UE, NR, RE */
   USIGN8    rem_add;                                             /* 0..126 */
   USIGN8    dummy;                                   /* PCI alignment byte */
   USIGN16   outp_data_len;                    /* 0..DP_MAX_OUTPUT_DATA_LEN */
/* OCTET     outp_data [outp_data_len];                                     */

} T_DP_RD_OUTP_CON;

/*==========================================================================*/

typedef struct _T_DP_SET_PRM_REQ
{
  USIGN8          rem_add;                                        /* 0..126 */
  USIGN8          dummy;                              /* PCI alignment byte */
  T_DP_PRM_DATA   prm_data;                       /* 9..DP_MAX_PRM_DATA_LEN */

} T_DP_SET_PRM_REQ;

/*--------------------------------------------------------------------------*/

typedef struct _T_DP_SET_PRM_CON
{
  USIGN16   status;                           /* OK, DS, NA, RS, RR, UE, RE */

} T_DP_SET_PRM_CON;

/*==========================================================================*/

typedef struct _T_DP_CHK_CFG_REQ
{
  USIGN8           rem_add;                                       /* 0..126 */
  USIGN8           dummy;                             /* PCI alignment byte */
  T_DP_CFG_DATA    cfg_data;                      /* 3..DP_MAX_CFG_DATA_LEN */

} T_DP_CHK_CFG_REQ;

/*--------------------------------------------------------------------------*/

typedef struct _T_DP_CHK_CFG_CON
{
  USIGN16   status;                           /* OK, DS, NA, RS, RR, UE, RE */

} T_DP_CHK_CFG_CON;

/*==========================================================================*/

typedef struct _T_DP_GET_CFG_REQ
{
  USIGN8    rem_add;                                              /* 0..126 */
  USIGN8    dummy;                                    /* SDU alignment byte */

} T_DP_GET_CFG_REQ;

/*--------------------------------------------------------------------------*/

typedef struct _T_DP_GET_CFG_CON
{
  USIGN16         status;                     /* OK, DS, NA, RS, UE, NR, RE */
  USIGN8          rem_add;                                        /* 0..126 */
  USIGN8          dummy;                              /* PCI alignment byte */
  T_DP_CFG_DATA   real_cfg_data;                  /* 3..DP_MAX_CFG_DATA_LEN */

} T_DP_GET_CFG_CON;

/*==========================================================================*/

typedef struct _T_DP_GLOBAL_CONTROL_REQ
{
  USIGN8    rem_add;               /* 0..126, 127 DP_GLOBAL_STATION_ADDRESS */
  USIGN8    dummy;                                    /* PCI alignment byte */
  USIGN8    control_command;                              /* DP_CONTROL_xxx */
  USIGN8    group_select;                  /* group_ident see T_DP_PRM_DATA */

} T_DP_GLOBAL_CONTROL_REQ;

/*--------------------------------------------------------------------------*/

typedef struct _T_DP_GLOBAL_CONTROL_CON
{
  USIGN16   status;                                       /* OK, DS, NO, IV */
  USIGN8    rem_add;               /* 0..126, 127 DP_GLOBAL_STATION_ADDRESS */
  USIGN8    dummy;                                    /* SDU alignment byte */

} T_DP_GLOBAL_CONTROL_CON;

/*==========================================================================*/

typedef struct _T_DP_SET_SLAVE_ADD_REQ
{
   USIGN8    rem_add;               /* 0..125, 126 DP_DEFAULT_SLAVE_ADDRESS */
   USIGN8    dummy;                                   /* PCI alignment byte */
   USIGN16   rem_slave_data_len;            /* 0..DP_MAX_REM_SLAVE_DATA_LEN */
   USIGN8    new_slave_add;                                       /* 0..125 */
   USIGN8    ident_number_high;     /* alignment problem in DIN 19245-3 !!! */
   USIGN8    ident_number_low;   /* USIGN16 ident_number at odd address !!! */
   PB_BOOL   no_add_chg;        /* DP_TRUE: address change after reset only */
/* OCTET     rem_slave_data [rem_slave_data_len];                           */

} T_DP_SET_SLAVE_ADD_REQ;

/*--------------------------------------------------------------------------*/

typedef struct _T_DP_SET_SLAVE_ADD_CON
{
  USIGN16   status;                           /* OK, DS, NA, RS, RR, UE, RE */
  USIGN8    rem_add;                /* 0..125, 126 DP_DEFAULT_SLAVE_ADDRESS */
  USIGN8    dummy;                                    /* SDU alignment byte */

} T_DP_SET_SLAVE_ADD_CON;

/*==========================================================================*/

typedef struct _T_DP_ACT_PARA_BRCT_REQ      /* MM: function_num --> rem_add */
{
  USIGN8    rem_add;               /* 0..125, 127 DP_GLOBAL_STATION_ADDRESS */
  USIGN8    area_code;                                 /* DP_AREA_BUS_PARAM */

} T_DP_ACT_PARA_BRCT_REQ;

/*--------------------------------------------------------------------------*/

typedef struct _T_DP_ACT_PARA_BRCT_CON
{
  USIGN16   status;                                               /* OK, DS */

} T_DP_ACT_PARA_BRCT_CON;

/*==========================================================================*/

typedef struct _T_DP_FM2_EVENT_IND
{
  USIGN16   reason;                                    /* FM2 event message */

} T_DP_FM2_EVENT_IND;

/*--------------------------------------------------------------------------*/
/*--- DPV1 DATA STRUCTURES -------------------------------------------------*/
/*--------------------------------------------------------------------------*/

typedef struct _T_ADDR
{
   USIGN8   api;                                       /* default: set to 0 */
   USIGN8   scl;                                       /* default: set to 0 */
   OCTET    network_address[6];     /* use only when [d|s]_type is set to 1 */
/* OCTET    mac_address[x_len-8]; *//* use only when [d|s]_type is set to 1 */

} T_ADDR;

/*--------------------------------------------------------------------------*/

typedef struct _T_ADD_ADDR
{
   USIGN8   s_type;                                    /* default: set to 0 */
   USIGN8   s_len;                                     /* default: set to 2 */
   USIGN8   d_type;                                    /* default: set to 0 */
   USIGN8   d_len;                                     /* default: set to 2 */
/* USIGN8   s_addr[s_len];                                                  */
/* USIGN8   d_addr[d_len];                                                  */

} T_ADD_ADDR;

/*--------------------------------------------------------------------------*/

typedef struct _T_DP_INITIATE_REQ
{
  USIGN8        rem_add;                                          /* 0..126 */
  USIGN8        reserved [3];
  USIGN16       send_timeout;                          /* 1..2^16-1 [10 ms] */
  OCTET         features_supported [2];                       /* 0x01, 0x00 */
  OCTET         profile_features_supported [2];         /* profile specific */
  USIGN16       profile_ident_number;                   /* profile specific */
  T_ADD_ADDR    add_addr_param;           /* additional address information */

} T_DP_INITIATE_REQ;

/*--------------------------------------------------------------------------*/

#define DP_INITIATE_S_ADDR(x)     ((T_ADDR FAR*)(((USIGN8 FAR*)&((x)->add_addr_param))+sizeof(T_ADD_ADDR)))
#define DP_INITIATE_D_ADDR(x)     ((T_ADDR FAR*)(((USIGN8 FAR*)&((x)->add_addr_param))+sizeof(T_ADD_ADDR)+(x)->add_addr_param.s_len))

/*--------------------------------------------------------------------------*/

typedef struct _T_DP_INITIATE_CON
{
  USIGN16       status;
  USIGN8        rem_add;                                          /* 0..126 */
  USIGN8        max_len_data_unit;                  /* 0..DP_MSAC2_DATA_LEN */
  OCTET         features_supported [2];                       /* 0x01, 0x00 */
  OCTET         profile_features_supported [2];         /* profile specific */
  USIGN16       profile_ident_number;                   /* profile specific */
  T_ADD_ADDR    add_addr_param;           /* additional address information */

} T_DP_INITIATE_CON;

/*--------------------------------------------------------------------------*/

typedef struct _T_DP_READ_REQ
{
  USIGN8    rem_add;               /* 0..126, interpreted only when MSAC_C1 */
  USIGN8    slot_number;                         /* 0..254, 255 is reserved */
  USIGN8    index;                               /* 0..254, 255 is reserved */
  USIGN8    length;                                 /* 0..DP_MSAC2_DATA_LEN */

} T_DP_READ_REQ;

/*--------------------------------------------------------------------------*/

typedef struct _T_DP_READ_CON
{
   USIGN16    status;
   USIGN8     rem_add;                                            /* 0..126 */
   USIGN8     slot_number;                       /* 0..254, 255 is reserved */
   USIGN8     index;                             /* 0..254, 255 is reserved */
   USIGN8     length;                               /* 0..DP_MSAC2_DATA_LEN */
/* OCTET      data [length]                                                 */

} T_DP_READ_CON;

/*--------------------------------------------------------------------------*/

typedef struct _T_DP_WRITE_REQ
{
   USIGN8   rem_add;               /* 0..126, interpreted only when MSAC_C1 */
   USIGN8   slot_number;                         /* 0..254, 255 is reserved */
   USIGN8   index;                               /* 0..254, 255 is reserved */
   USIGN8   length;                                 /* 0..DP_MSAC2_DATA_LEN */
   /* OCTET    data [length]                                                   */

} T_DP_WRITE_REQ;

/*--------------------------------------------------------------------------*/

typedef struct _T_DP_WRITE_CON
{
  USIGN16   status;
  USIGN8    rem_add;                                              /* 0..126 */
  USIGN8    slot_number;                         /* 0..254, 255 is reserved */
  USIGN8    index;                               /* 0..254, 255 is reserved */
  USIGN8    length;                                 /* 0..DP_MSAC2_DATA_LEN */

} T_DP_WRITE_CON;

/*--------------------------------------------------------------------------*/
typedef struct _T_DP_DATA_TRANSPORT_REQ
{
   USIGN8   rem_add;                                              /* 0..126 */
   USIGN8   slot_number;                         /* 0..254, 255 is reserved */
   USIGN8   index;                               /* 0..254, 255 is reserved */
   USIGN8   length;                                 /* 0..DP_MSAC2_DATA_LEN */
/* OCTET    data [length]                                                   */

} T_DP_DATA_TRANSPORT_REQ;

/*--------------------------------------------------------------------------*/

typedef struct _T_DP_DATA_TRANSPORT_CON
{
   USIGN16   status;
   USIGN8    rem_add;                                             /* 0..126 */
   USIGN8    slot_number;                        /* 0..254, 255 is reserved */
   USIGN8    index;                              /* 0..254, 255 is reserved */
   USIGN8    length;                                /* 0..DP_MSAC2_DATA_LEN */
/* OCTET    data [length]                                                   */

} T_DP_DATA_TRANSPORT_CON;

/*--------------------------------------------------------------------------*/

typedef struct _T_DP_ABORT_REQ
{
  USIGN8    subnet;                             /* 0, 1, 2, others reserved */
  USIGN8    reason;                            /* user reason code: 0..0x3F */

} T_DP_ABORT_REQ;

/*--------------------------------------------------------------------------*/

typedef struct _T_DP_ABORT_IND
{
  PB_BOOL   locally_generated;
  USIGN8    subnet;                             /* 0, 1, 2, others reserved */
  USIGN8    reason;                 /* reason codes: see DPV1 specification */
  USIGN8    dummy;                                        /* alignment byte */
  USIGN16   additional_detail;              /* received timeout with RM-REQ */

} T_DP_ABORT_IND;

/*--------------------------------------------------------------------------*/

typedef struct _T_DP_ERROR_CON
{
  USIGN16   status;
  USIGN8    rem_add;
  USIGN8    error_decode;
  USIGN8    error_code_1;
  USIGN8    error_code_2;

} T_DP_ERROR_CON;

/****************************************************************************/

#if defined (WIN32) || defined (_WIN32) || defined (WIN16) || defined (_WIN16)
#pragma warning (disable : 4103)     /* used #pragma pack to reset alignment */
#ifdef WIN32
#pragma pack(pop)
#else
#pragma pack()
#endif
#pragma warning (default : 4103)
#endif

#endif /* __PB_DP__ */
