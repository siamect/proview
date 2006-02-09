/**/
/******************************************************************************/
/**                                                                          **/
/**  Copyright (c) 1998                                                      **/
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
 
/*** MODULE PAMS_C_TYPE_CLASS IDENT V5.0-01 ***/
/**/
#ifndef _P_TYPECL_H
#define _P_TYPECL_H 1
/**/
/*                                                                          */
/*                                                                          */
/*                                                                          */
/***********************************************************************    */
/*  Function:                                                         **    */
/*    This include file defines all the message definitions           **    */
/*    used within the MessageQ system.                                **    */
/*                                                                    **    */
/*  NOTE:                                                             **    */
/*    The following are reserved to BEA:                              **    */
/*                                                                    **    */
/*        Msg Classes: 28, 29, 62 & 30,000 thru 32,767                **    */
/*        Msg Types:   -1 thru -5,000                                 **    */
/***********************************************************************    */
/*                                                                          */
/*                                                                          */
/*                                                                          */
/***********************************************************************    */
/*                                                                          */
/*      MRS message CLASS and TYPE codes                                    */
/*                                                                          */
/***********************************************************************    */
/*                                                                          */
/*                                                                          */
/* define MRS class **                                                      */
#define MSG_CLAS_MRS 28
/*                                                                          */
#define MSG_TYPE_MRS_ACK -801
#define MSG_TYPE_MRS_DQF_SET -704
#define MSG_TYPE_MRS_DQF_SET_REP -705
#define MSG_TYPE_MRS_DQF_TRANSFER -700
#define MSG_TYPE_MRS_DQF_TRANSFER_ACK -701
#define MSG_TYPE_MRS_DQF_TRANSFER_REP -702
#define MSG_TYPE_MRS_JRN_DISABLE -871
#define MSG_TYPE_MRS_JRN_DISABLE_REP -872
#define MSG_TYPE_MRS_JRN_ENABLE -873
#define MSG_TYPE_MRS_JRN_ENABLE_REP -874
#define MSG_TYPE_MRS_SAF_SET -706
#define MSG_TYPE_MRS_SAF_SET_REP -707
#define MSG_TYPE_MRS_SAF_TRANSFER -805
#define MSG_TYPE_MRS_SAF_TRANSFER_ACK -806
#define MSG_TYPE_MRS_SAF_TRANSFER_REP -807
#define MSG_TYPE_MRS_SET_PCJ -880
#define MSG_TYPE_MRS_SET_PCJ_REP -881
#define MSG_TYPE_MRS_SET_DLJ -882
#define MSG_TYPE_MRS_SET_DLJ_REP -883
/*                                                                          */
/*                                                                          */
/***********************************************************************    */
/*                                                                          */
/*      AVAIL message TYPE codes                                            */
/*                                                                          */
/***********************************************************************    */
/*                                                                          */
/*                                                                          */
#define MSG_TYPE_AVAIL_REG -1180
#define MSG_TYPE_AVAIL_DEREG -1181
#define MSG_TYPE_AVAIL_REG_REPLY -1182
#define MSG_TYPE_AVAIL -1183
#define MSG_TYPE_UNAVAIL -1184
/*                                                                          */
/*                                                                          */
/***********************************************************************    */
/*                                                                          */
/*      SBS message TYPE codes   SBS message CLASS = 29                     */
/*                                                                          */
/***********************************************************************    */
/*                                                                          */
/* V3.2 and prior API                                                       */
#define MSG_TYPE_SBS_DEREG -1174
#define MSG_TYPE_SBS_DEREG_ACK -1155
#define MSG_TYPE_SBS_DEREG_BY_ID -1154
#define MSG_TYPE_SBS_REG -1150
#define MSG_TYPE_SBS_REG_EZ_REPLY -1153
#define MSG_TYPE_SBS_REG_EZ -1173
#define MSG_TYPE_SBS_REG_REPLY -1152
#define MSG_TYPE_SBS_BS_SEQGAP -1166
/* Extended API                                                             */
#define MSG_TYPE_SBS_REGISTER_REQ -1196
#define MSG_TYPE_SBS_REGISTER_RESP -1197
#define MSG_TYPE_SBS_DEREGISTER_REQ -1170
#define MSG_TYPE_SBS_DEREGISTER_RESP -1172
#define MSG_TYPE_SBS_STATUS_REQ -1177
#define MSG_TYPE_SBS_STATUS_RESP -1178
#define MSG_TYPE_SBS_SEQUENCE_GAP -1199
/*                                                                          */
/*                                                                          */
/***********************************************************************    */
/*                                                                          */
/*      PAMS message CLASS and TYPE codes                                   */
/*                                                                          */
/***********************************************************************    */
/*                                                                          */
/*                                                                          */
/* define PAMS class **                                                     */
#define MSG_CLAS_PAMS 29
/*                                                                          */
/*                                                                          */
#define MSG_TYPE_DISABLE_NOTIFY -991
#define MSG_TYPE_DISABLE_Q_NOTIFY_REQ -964
#define MSG_TYPE_DISABLE_Q_NOTIFY_RESP -965
#define MSG_TYPE_ENABLE_NOTIFY -990
#define MSG_TYPE_ENABLE_Q_NOTIFY_REQ -962
#define MSG_TYPE_ENABLE_Q_NOTIFY_RESP -963
#define MSG_TYPE_LINKMGT_REQ -975
#define MSG_TYPE_LINKMGT_RESP -976
#define MSG_TYPE_LINK_COMPLETE -999
#define MSG_TYPE_LINK_LOST -998
/* same as MSG_TYPE_LINK_LOST                                               */
#define MSG_TYPE_LINK_LOSS -998
#define MSG_TYPE_LIST_ALL_CONNECTIONS -996
#define MSG_TYPE_LIST_ALL_ENTRIES -995
/* same as LIST_ALL_ENTRIES                                                 */
#define MSG_TYPE_LIST_ALL_ENTRYS -995
#define MSG_TYPE_LIST_ALL_GROUPS -997
#define MSG_TYPE_LIST_ALL_Q_REQ -960
#define MSG_TYPE_LIST_ALL_Q_RESP -961
#define MSG_TYPE_LOCATE_Q_REP -972
#define MSG_TYPE_Q_UPDATE -966
#define MSG_TYPE_TIMER_EXPIRED -900
#define MSG_TYPE_UNDECLARE_SQ -981
/*                                                                          */
/*                                                                          */
/***********************************************************************    */
/*                                                                          */
/*        DEFINE LU62 type codes for LU6.2 UCB                              */
/*                                                                          */
/***********************************************************************    */
/*                                                                          */
/* define LU6.2 class                                                       */
#define MSG_CLAS_APPC 62
/*                                                                          */
#define MSG_TYPE_LU62_INIT -6200
#define MSG_TYPE_LU62_DEFINE_LU -6201
#define MSG_TYPE_LU62_ALLOCATE -6202
#define MSG_TYPE_LU62_SEND_DATA -6203
#define MSG_TYPE_LU62_REQ_CONFIRM -6204
#define MSG_TYPE_LU62_SEND_CONFIRM -6205
#define MSG_TYPE_LU62_CONFIRM_RECV -6206
#define MSG_TYPE_LU62_DEALLOCATE -6207
#define MSG_TYPE_LU62_SEND_ERROR -6208
#define MSG_TYPE_LU62_DELETE_LU -6209
#define MSG_TYPE_LU62_RECV_DATA -6210
#define MSG_TYPE_LU62_CONFIRM_REQ -6211
#define MSG_TYPE_LU62_CONFIRM_SEND -6212
#define MSG_TYPE_LU62_CONFIRMED -6213
#define MSG_TYPE_LU62_ERROR -6214
#define MSG_TYPE_LU62_DEALLOCATED -6215
/*                                                                          */
/***********************************************************************    */
/*                                                                          */
/* The following message types are for 2.1 functionality                    */
/*                                                                          */
/***********************************************************************    */
/*                                                                          */
#define MSG_TYPE_LU62_REQ_SEND -6220
#define MSG_TYPE_LU62_CONNECTED -6221
#define MSG_TYPE_LU62_DEFINE_TP -6222
#define MSG_TYPE_LU62_ACTIVATE -6223
#define MSG_TYPE_LU62_OK_SEND -6224
#define MSG_TYPE_LU62_FLUSH -6225
/*                                                                          */
/***********************************************************************    */
/*                                                                          */
/* The following message types are for the Generic Port Server              */
/*                                                                          */
/***********************************************************************    */
/*                                                                          */
/*                                                                          */
#define MSG_TYPE_REGISTER_TARGET -6280
#define MSG_TYPE_CONNECT_REQUEST -6281
#define MSG_TYPE_CONNECT_ACCEPT -6282
#define MSG_TYPE_CONNECT_REJECT -6283
#define MSG_TYPE_CONNECTION_TERMINATED -6284
#define MSG_TYPE_CHANGE_DIRECTION -6285
#define MSG_TYPE_DATA_MESSAGE -6286
#define MSG_TYPE_LOG_EVENT -6287
#define MSG_TYPE_LU62_ADD_LU -6288
#define MSG_TYPE_LU62_ADD_TARGET -6289
#define MSG_TYPE_LU62_REPORT_STATUS -6290
#define MSG_TYPE_LU62_SHUTDOWN -6299
/*                                                                          */
/***********************************************************************    */
/*                                                                          */
/*       ETHERNET   Class and Type codes (1000-1010)                        */
/*                                                                          */
/***********************************************************************    */
/*                                                                          */
#define MSG_CLAS_ETHERNET 100
/*                                                                          */
#define MSG_TYPE_E_CONNECT -1000
#define MSG_TYPE_E_DISCONNECT -1001
#define MSG_TYPE_E_INITIALIZE -1002
#define MSG_TYPE_E_CONNECT_COMPLETE -1003
#define MSG_TYPE_E_CONNECT_REJECTED -1004
#define MSG_TYPE_E_PARTNER_DISC -1005
#define MSG_TYPE_E_IO_ERROR -1006
#define MSG_TYPE_E_PROTOCOL_ERROR -1007
#define MSG_TYPE_E_LISTEN_TIMEOUT -1008
#define MSG_TYPE_E_MESSAGES_LOST -1009
#define MSG_TYPE_E_RUNTIME_ERROR -1010
/*                                                                          */
/***********************************************************************    */
/*                                                                          */
/*       Generic UCB Types and Classes                                      */
/*                                                                          */
/***********************************************************************    */
/*                                                                          */
#define MSG_CLAS_UCB 102
/*                                                                          */
#define MSG_TYPE_UCB_CONNECT -1200
#define MSG_TYPE_UCB_DISCONNECT -1201
#define MSG_TYPE_UCB_INITIALIZE -1202
#define MSG_TYPE_UCB_CONNECT_COMPLETE -1203
#define MSG_TYPE_UCB_PARTNER_DISC -1204
#define MSG_TYPE_UCB_MESSAGES_LOST -1205
#define MSG_TYPE_UCB_IO_ERROR -1206
#define MSG_TYPE_UCB_RCV_DATA -1207
/*                                                                          */
/***********************************************************************    */
/*                                                                          */
/*      ADM message CLASS and TYPE codes                                    */
/*                                                                          */
/***********************************************************************    */
/*                                                                          */
/* define ADM class **                                                      */
#define MSG_CLAS_ADM 31000
/*                                                                          */
/***********************************************************************    */
/*                                                                          */
/*      TUXEDO message CLASS and TYPE codes                                 */
/*                                                                          */
/***********************************************************************    */
/*                                                                          */
/* define TUXEDO class **                                                   */
#define MSG_CLAS_TUXEDO 31001
#define MSG_CLAS_TUXEDO_TPFAIL 31002
#define MSG_CLAS_TUXEDO_TPSUCCESS 31003
/*                                                                          */
/***********************************************************************    */
/*                                                                          */
/*      QMB message CLASS and TYPE codes                                    */
/*                                                                          */
/***********************************************************************    */
/*                                                                          */
/* define QMB class **                                                      */
#define MSG_CLAS_QMB 32000
#define MSG_CLAS_QMB_REPLY_CANCEL 31999
/*                                                                          */
#define MSG_TYPE_DUMP_QTABLES -5002
#define MSG_TYPE_PURGE_CI -5004
#define MSG_TYPE_PURGE_CI_ALL -5005
#define MSG_TYPE_NEW_LOG -5006
#define MSG_TYPE_LOAD_CONFIG -5007
#define MSG_TYPE_RSQ_REGISTER -5008
#define MSG_TYPE_DATAGRAM -5009
#define MSG_TYPE_REQUEST -5010
#define MSG_TYPE_REPLY -5011
#define MSG_TYPE_RTS_ERROR -5012
#define MSG_TYPE_EVENT_LOG -5013
#define MSG_TYPE_TRACE_LOG -5014
#define MSG_TYPE_QMB_TERMINATE -5099
/*                                                                          */
/******************* End of P_TYPECL ***********************************    */
/**/
#endif	/* end P_TYPECL_H */
