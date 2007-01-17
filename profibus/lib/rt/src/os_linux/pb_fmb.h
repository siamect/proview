/* 
 * Proview   $Id: pb_fmb.h,v 1.4 2007-01-17 12:40:30 claes Exp $
 * 
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
 */
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
/*                  FMB-USER-INTERFACE DEFINES and TYPES                     */
/*                                                                           */
/*  Filename    : PB_FMB.H                                                   */
/*  Version     : 5.21.0.00.release                                          */
/*  Date        : 27-February-1998                                           */
/*  Author      : SOFTING AG                                                 */
/*                                                                           */
/*                                                                           */
/*  Description : This file contains the types and defines of the Fieldbus-  */
/*                Basic Management User-Interface                            */
/*****************************************************************************/

#ifndef __PB_FMB__
#define __PB_FMB__

#if defined (WIN32) || defined (_WIN32) || defined (WIN16) || defined (_WIN16)
#pragma warning (disable : 4103)     /* used #pragma pack to change alignment */
#ifdef WIN32
#pragma pack(push,2)
#else
#pragma pack(2)
#endif
#pragma warning (default : 4103)
#endif


/*****************************************************************************/
/***     FMB service identifier                                            ***/
/*****************************************************************************/
#define FMB_SET_VALUE             15
#define FMB_READ_VALUE            16
#define FMB_LSAP_STATUS           17
#define FMB_FM2_EVENT             19
#define FMB_RESET                 20
#define FMB_EXIT                  21
#define FMB_SET_BUSPARAMETER      22
#define FMB_READ_BUSPARAMETER     24
#define FMB_VALIDATE_MASTER       25
#define FMB_GET_LIVE_LIST         26
#define FMB_SET_CONFIGURATION     27

#define FMB_EXCEPTION             28

/*****************************************************************************/
/***     FMB data structures                                               ***/
/*****************************************************************************/

/*****************************************************************************/
/* FMB-Configuration-Management                                              */
/*****************************************************************************/

/* VFD-Configuration ------------------------------------------------------- */
typedef struct _T_FMB_CONFIG_VFD
{
  USIGN16     max_no_vfds;                           /* max. number of VFD's */
  USIGN16     max_no_obj_descr;            /* max. number of OD object descr.*/
  USIGN8      max_obj_name_length;            /* max. size of OD object name */
  USIGN8      max_obj_ext_length;        /* max. size of OD object extension */
} T_FMB_CONFIG_VFD;


/* --- CRL-Configuration --------------------------------------------------- */
typedef struct _T_FMB_CONFIG_CRL
{
  USIGN16     max_no_fal_sdbs;                    /* max. number of FAL-SDBs */
  USIGN16     max_no_fdl_sdbs;                    /* max. number of FDL-SDBs */
  USIGN16     max_no_data_buffer;              /* max. number of PDU buffers */
  USIGN16     max_no_api_buffer;               /* max. number of API buffers */
  USIGN16     max_no_poll_entries;       /* max. number of poll list entries */
  USIGN16     max_no_subscr_entries;   /* max. number of subscr.list entries */
  PB_BOOL     resrc_check;                                 /* for future use */
  USIGN8      max_no_parallel_req;                         /* for future use */
  USIGN8      max_no_parallel_ind;                         /* for future use */
  USIGN8      dummy;                                       /* alignment byte */
} T_FMB_CONFIG_CRL;

/* Notes on T_FMB_CONFIG_CRL: ------------------------------------------------

component 'max_no_subscr_entries' is of significance for PROFIBUS-PA only
component 'resrc_check' is introduced for future use - for version 5.10
           set "resrc_check = PB_TRUE;"
component 'max_no_parallel_req' is introduced for future use - for
           version 5.10 set "max_no_parallel_req = 0;"
component 'max_no_parallel_ind' is introduced for future use - for
           version 5.10 set "max_no_parallel_ind = 0;"

end of notes on T_FMB_CONFIG_CRL ------------------------------------------ */


/* DP-Configuration -------------------------------------------------------- */
typedef struct _T_FMB_CONFIG_DP
{
  USIGN8      max_number_slaves;       /* maximum number DP Slaves supported */
  USIGN8      max_slave_output_len;      /* max. length of slave output data */
  USIGN8      max_slave_input_len;        /* max. length of slave input data */
  USIGN8      max_slave_diag_len;           /* max. length of one diag entry */
  USIGN16     max_slave_diag_entries;  /* max. number entries in diag buffer */
  USIGN16     max_bus_para_len;          /* max. length of bus parameter set */
  USIGN16     max_slave_para_len;      /* max. length of slave parameter set */
} T_FMB_CONFIG_DP;


/* FDLIF-Configuration ----------------------------------------------------- */
typedef struct _T_FMB_CONFIG_FDLIF
{
  USIGN8      send_req_credits;  /* max. number of send credits for SDA and SDN services */
  USIGN8      srd_req_credits;   /* max. number of send credits for SRD services */
  USIGN8      receive_credits;             /* max. number of receive credits */
  USIGN8      max_no_resp_saps;         /* max. number of FDL responder SAPs */
} T_FMB_CONFIG_FDLIF;


/* SM7-Configuration ------------------------------------------------------- */
typedef struct _T_FMB_CONFIG_SM7
{
   USIGN16    reserved;
} T_FMB_CONFIG_SM7;


/* Fieldbus-Basic-Management-Configuration --------------------------------- */
typedef struct _T_FMB_SET_CONFIGURATION_REQ
{
  PB_BOOL             fms_active;         /* FMS and FM7 services are usable */
  PB_BOOL             dp_active;                   /* DP services are usable */
  PB_BOOL             fdlif_active;             /* FDLIF services are usable */
  PB_BOOL             sm7_active;                 /* SM7 services are usable */
  USIGN16             fdl_evt_receiver;  /* receiver of FDL events:
                                            FMB_USR,FM7_USR,DP_USR,FDLIF_USR */
  USIGN16             data_buffer_length;         /* max. size of PDU buffer */
  T_FMB_CONFIG_VFD    vfd;                   /* VFD configuration parameters */
  T_FMB_CONFIG_CRL    crl;                   /* CRL configuration parameters */
  T_FMB_CONFIG_DP     dp;                      /* DP configuration parameter */
  T_FMB_CONFIG_FDLIF  fdlif;                /* FDLIF configuration parameter */
  T_FMB_CONFIG_SM7    sm7;                   /* SM7 configuration parameters */
} T_FMB_SET_CONFIGURATION_REQ;


/*****************************************************************************/
/* FMB-Set-FDL-Busparameter                                                  */
/*****************************************************************************/

/* Baudrate ---------------------------------------------------------------- */
#define KBAUD_9_6                  0x00
#define KBAUD_19_2                 0x01
#define KBAUD_45_45                0x0B
#define KBAUD_93_75                0x02
#define KBAUD_187_5                0x03
#define KBAUD_500                  0x04
#define KBAUD_750                  0x05
#define MBAUD_1_5                  0x06
#define MBAUD_3                    0x07
#define MBAUD_6                    0x08
#define MBAUD_12                   0x09


/* Medium-Redundancy ------------------------------------------------------- */
#define NO_REDUNDANCY              0x00
#define BUS_A_HIGHPRIOR            0x01
#define BUS_B_HIGHPRIOR            0x02
#define REDUNDANCY                 0x03

/* In-Ring-Desired --------------------------------------------------------- */
#define IN_RING_DESIRED            PB_TRUE
#define NOT_IN_RING_DESIRED        PB_FALSE


typedef struct _T_FMB_SET_BUSPARAMETER_REQ
{
  USIGN8         loc_add;                                   /* local station */
  USIGN8         loc_segm;                                  /* local segment */
  USIGN8         baud_rate;                                     /* baud rate */
  USIGN8         medium_red;                            /* medium redundancy */
  USIGN16        tsl;                                           /* slot time */
  USIGN16        min_tsdr;                  /* min. station delay time resp. */
  USIGN16        max_tsdr;                  /* max. station delay time resp. */
  USIGN8         tqui;                                         /* quiet time */
  USIGN8         tset;                                         /* setup time */
  USIGN32        ttr;                          /* target token rotation time */
  USIGN8         g;                                     /* gap update factor */
  PB_BOOL        in_ring_desired;               /* active or passive station */
  USIGN8         hsa;                             /* highest station address */
  USIGN8         max_retry_limit;                        /* max. retry limit */
} T_FMB_SET_BUSPARAMETER_REQ;


/*****************************************************************************/
/* FMB-Read-FDL-Busparameter                                                 */
/*****************************************************************************/

typedef struct _T_FMB_READ_BUSPARAMETER_CNF
{
  USIGN8         loc_add;                                   /* local station */
  USIGN8         loc_segm;                                  /* local segment */
  USIGN8         baud_rate;                                     /* baud rate */
  USIGN8         medium_red;                            /* medium redundancy */
  USIGN16        tsl;                                           /* slot time */
  USIGN16        min_tsdr;                  /* min. station delay time resp. */
  USIGN16        max_tsdr;                  /* max. station delay time resp. */
  USIGN8         tqui;                                         /* quiet time */
  USIGN8         tset;                                         /* setup time */
  USIGN32        ttr;                          /* target token rotation time */
  USIGN8         g;                                     /* gap update factor */
  PB_BOOL        in_ring_desired;               /* active or passive station */
  USIGN8         hsa;                             /* highest station address */
  USIGN8         max_retry_limit;                        /* max. retry limit */
  USIGN16        reserved;                                       /* not used */
  USIGN8         ident[202];                             /* FDL-Ident-String */
} T_FMB_READ_BUSPARAMETER_CNF;


/*****************************************************************************/
/* FMB-Set-Value Service                                                     */
/*****************************************************************************/
#if !defined (__PB_FM7__)

/* FDL-Variable-Identifiers ------------------------------------------------ */
#define ID_TS                      1
#define ID_BAUD_RATE               2
#define ID_MEDIUM_RED              3
#define ID_HW_RELEASE              4
#define ID_SW_RELEASE              5
#define ID_TSL                     6
#define ID_MIN_TSDR                7
#define ID_MAX_TSDR                8
#define ID_TQUI                    9
#define ID_TSET                   10
#define ID_TTR                    11
#define ID_G                      12
#define ID_IN_RING_DESIRED        13
#define ID_HSA                    14
#define ID_MAX_RETRY_LIMIT        15
#define ID_TRR                    16
#define ID_LAS                    17
#define ID_GAPL                   18

/* Statistic-Counter-Identifiers ------------------------------------------- */
#define ID_FRAME_SENT_COUNT       20
#define ID_RETRY_COUNT            21
#define ID_SD_COUNT               22
#define ID_SD_ERROR_COUNT         23

#endif

typedef struct _T_FMB_SET_VALUE_REQ
{
   USIGN8   id;                                         /* value identifier */
   USIGN8   length;                                  /* # of values in byte */
/* USIGN8   value[length];                                  list of values  */
} T_FMB_SET_VALUE_REQ;


/*****************************************************************************/
/* FMB-Read-Value Service                                                    */
/*****************************************************************************/

typedef struct _T_FMB_READ_VALUE_REQ
{
  USIGN8   id;                                          /* value identifier */
  USIGN8   dummy;                                              /* alignment */
} T_FMB_READ_VALUE_REQ;


typedef struct _T_FMB_READ_VALUE_CNF
{
   USIGN8   id;                                         /* value identifier */
   USIGN8   length;                                  /* # of values in byte */
/* USIGN8   value[length];                                   list of values */
} T_FMB_READ_VALUE_CNF;


/*****************************************************************************/
/* FMB-LSAP-Status Service                                                   */
/*****************************************************************************/

typedef struct _T_FMB_LSAP_STATUS_REQ
{
  USIGN8    lsap;                                            /* desired LSAP */
  USIGN8    dummy;                                         /* alignment byte */
} T_FMB_LSAP_STATUS_REQ;


typedef struct _T_FMB_LSAP_STATUS_CNF
{
  USIGN8    access;                                /* station address or all */
  USIGN8    addr_extension;                                /* segment number */
  USIGN8    sda;                                                      /* SDA */
  USIGN8    sdn;                                                      /* SDN */
  USIGN8    srd;                                                      /* SRD */
  USIGN8    csrd;                                                    /* CSRD */
} T_FMB_LSAP_STATUS_CNF;


/*****************************************************************************/
/* FMB-Validate-Master Service                                               */
/*****************************************************************************/

typedef struct _T_FMB_VALIDATE_MASTER_REQ
{
  USIGN8    rem_add;                               /* desired remote station */
  USIGN8    dummy;                                         /* alignment byte */
} T_FMB_VALIDATE_MASTER_REQ;


/*****************************************************************************/
/* FMB-Get-Live-List Service                                                 */
/*****************************************************************************/
/* Status of Stations in Live-List ----------------------------------------- */
#define PASSIVE               0x00
#define ACTIVE_NOT_READY      0x01
#define ACTIVE_READY          0x02
#define ACTIVE_IN_RING        0x03

typedef struct _T_FMB_LIVE_LIST
{
  USIGN8     station;                                     /* station number */
  USIGN8     status;                          /* current station of station */
} T_FMB_LIVE_LIST;

typedef struct _T_FMB_GET_LIVE_LIST_CNF
{
  USIGN8           dummy;                                       /* alignment */
  USIGN8           no_of_elements;                /* # of live list elements */
/* T_FMB_LIVE_LIST live_list[no_of_elements];     list of live list elements */
} T_FMB_GET_LIVE_LIST_CNF;


/****************************************************************************/
/* FMB-Event-Management                                                     */
/****************************************************************************/

#define FM2_FAULT_ADDRESS          0x01     /* duplicate address recognized */
#define FM2_FAULT_PHY              0x02     /* phys.layer is malfunctioning */
#define FM2_FAULT_TTO              0x03         /* time out on bus detected */
#define FM2_FAULT_SYN              0x04      /* no receiver synchronization */
#define FM2_FAULT_OUT_OF_RING      0x05              /* station out of ring */
#define FM2_GAP_EVENT              0x06              /* new station in ring */

/* Additional FM2-Events (Error messages from ASPC2) ---------------------- */
#define FM2_MAC_ERROR              0x13                  /* fatal MAC error */
#define FM2_HW_ERROR               0x14                   /* fatal HW error */

typedef struct _T_FMB_FM2_EVENT_IND
{
  USIGN16    reason;                                         /* reason code */
} T_FMB_FM2_EVENT_IND;


#if defined (WIN32) || defined (_WIN32) || defined (WIN16) || defined (_WIN16)
#pragma warning (disable : 4103)     /* used #pragma pack to reset alignment */
#ifdef WIN32
#pragma pack(pop)
#else
#pragma pack()
#endif
#pragma warning (default : 4103)
#endif

#endif  /* __PB_FMB__ */
