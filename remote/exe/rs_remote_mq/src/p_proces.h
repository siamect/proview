/**/
/******************************************************************************/
/**                                                                          **/
/**  Copyright (c) 1997                                                      **/
/**  by BEA Systems, Inc.  All rights reserved.                              **/
/**                                                                          **/
/**  Unpublished rights reserved under the  copyright laws of the            **/
/**  United States.                                                          **/
/**                                                                          **/
/**  The software  contained on this  media is proprietary to and            **/
/**  embodies the  confidential technology  of  BEA Systems, Inc.            **/
/**  The possession, use,   duplication  or  dissemination of the            **/
/**  software  and  media is  authorized only pursuant to a valid            **/
/**  written license from BEA Systems, Inc.                                  **/
/**                                                                          **/
/**  RESTRICTED RIGHTS LEGEND Use,  duplication, or disclosure by            **/
/**  the U.S.  Government is subject to restrictions as set forth            **/
/**  in Subparagraph (c)(1)(ii) of  DFARS 252.227-7013, or in FAR            **/
/**  52.227-19, as applicable.                                               **/
/**                                                                          **/
/******************************************************************************/
 
/*** MODULE PAMS_C_PROCESS IDENT V4.0-03 ***/
/**/
#ifndef _P_PROCES_H
#define _P_PROCES_H 1
/**/
/*                                                                          */
/*                                                                          */
/*   Sample Queues                                                          */
/*                                                                          */
#define PAMS_QUEUE_1 1
#define PAMS_QUEUE_2 2
#define PAMS_QUEUE_3 3
#define PAMS_QUEUE_4 4
#define PAMS_QUEUE_5 5
#define PAMS_QUEUE_6 6
#define PAMS_QUEUE_7 7
#define PAMS_QUEUE_8 8
#define PAMS_QUEUE_9 9
#define PAMS_QUEUE_10 10
/*                                                                          */
/*                                                                          */
/*  SBS_EXAMPLE.EXE uses the following UCB numbers for Optimized Delivery   */
/*                                                                          */
#define PAMS_SBS_ETH_CONTROL 74
#define PAMS_SBS_ETH_CHAN1 75
#define PAMS_SBS_ETH_CHAN2 76
/*                                                                          */
/*                                                                          */
/*  Processes 90-100 & 150-199 are reserved for PAMS utilities              */
/*                                                                          */
#define PAMS_SCREEN_PROCESS 0
#define PAMS_TEMPORARY_Q 0
#define PAMS_SPARE1 90
#define PAMS_ALL_UCBS 91
#define PAMS_SPARE2 92
#define PAMS_TIMER_QUEUE 92
#define PAMS_NULL 93
#define PAMS_INTERNAL1 94
#define PAMS_NA_SERVER 94
#define PAMS_QTRANSFER_SERVER 95
#define PAMS_INTERNAL2 96
#define PAMS_DEAD_LETTER_QUEUE 96
#define PAMS_APPLICATION_ERROR_QUEUE 97
#define PAMS_MRS_SERVER 98
#define PAMS_AVAIL_SERVER 99
#define PAMS_SBS_SERVER 99
#define PAMS_DECLARE_SERVER 100
#define PAMS_CONNECT_SERVER 100
#define PAMS_QUEUE_SERVER 100
#define PAMS_TRANSPORT 100
#define PAMS_COM_SERVER 100
#define PAMS_DMQ_LOADER 150
#define PAMS_DCL_BY_Q_NAME 151
#define PAMS_TCPIP_LD 152
#define PAMS_DECNET_LD 153
#define PAMS_EVENT_LOGGER 155
#define PAMS_JRN_SERVER 156
#define PAMS_MRS_FAILOVER 157
#define PAMS_DMQ_FULLTEST_PQ 191
#define PAMS_DMQ_FULLTEST_SQ 192
#define PAMS_EXAMPLE_Q_1 193
#define PAMS_EXAMPLE_Q_2 194
#define PAMS_IVP_UNOWNED_SQ 195
#define PAMS_IVP_PRIVATE_MOT1 4999
#define PAMS_IVP_UNIVERSAL_MOT1 5001
/*                                                                          */
/*                                                                          */
/**/
#endif	/* end P_PROCES_H */
