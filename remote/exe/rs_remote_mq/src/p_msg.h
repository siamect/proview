/**/
/******************************************************************************/
/**                                                                          **/
/**  Copyright (c) 1997, 1998                                                **/
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
 
/*** MODULE PAMS_MSG IDENT V4.0-08 ***/
/*                                                                            */
/*                                                                            */
/*--                                                                          */
/*                                                                            */
#ifndef _P_MSG_H
#define _P_MSG_H 1
#include <p_entry.h>
 
 
#ifdef __DECC
#pragma member_alignment __save
#pragma nomember_alignment
#endif
 
#if defined(_WIN32) || defined(__osf__)
#pragma pack(1)
#endif
 
 
typedef struct _AVAIL {
    q_address target_q;
    } AVAIL;
 
 
typedef struct _AVAIL_DEREG {
    int16 version;
    int16 filler;
    q_address target_q;
    q_address distribution_q;
    char req_ack;
    } AVAIL_DEREG;
 
 
typedef struct _AVAIL_REG {
    int16 version;
    int16 filler;
    q_address target_q;
    q_address distribution_q;
    int32 timeout;
    } AVAIL_REG;
 
 
typedef struct _AVAIL_REG_REPLY {
    int16 status;
    uint16 reg_id;
    int16 number_reg;
    } AVAIL_REG_REPLY;
 
 
/* NOTE: DISABLE_NOTIFY uses the same structure as ENABLE_NOTIFY            */
/*       DISABLE_Q_NOTIFY_REQ is the same structure as LIST_ALL_Q_REQ       */
/*       DISABLE_Q_NOTIFY_RESP is the same structure as Q_NOTIFY_RESP       */
 
 
typedef struct _ENABLE_NOTIFY {
    char reserved;
    char connection_flag;
    } ENABLE_NOTIFY;
 
 
/* NOTE: ENABLE_Q_NOTIFY_REQ is the same structure as LIST_ALL_Q_REQ        */
/*       ENABLE_Q_NOTIFY_RESP is the same structure as Q_NOTIFY_RESP        */
 
 
typedef struct _TADDRESS {
    int32 len;
    char str [16];
    } TADDRESS;
typedef struct _NODENAME {
    int32 len;
    char str [256];
    } NODENAME;
 
 
typedef struct _LINKMGT_REQ {
    int32 version;
    int32 user_tag;
    int32 function_code;
    int32 group_number;
    int32 connect_type;
    int32 reconnect_timer;
    int32 window_size;
    int32 window_delay;
    int32 reserved_space [10];
    TADDRESS transport_addr;
    NODENAME node_name;
    } LINKMGT_REQ;
 
 
typedef struct _LINKMGT_RESP {
    int32 version;
    int32 user_tag;
    int32 status;
    int32 group_number;
    int32 in_link_state;
    int32 out_link_state;
    int32 connect_type;
    int32 platform_id;
    int32 reconnect_timer;
    int32 window_size;
    int32 window_delay;
    int32 reserved_space [10];
    TADDRESS transport_addr;
    NODENAME node_name;
    } LINKMGT_RESP;
 
 
/* NOTE: LINK_COMPLETE uses the LINK_NOTIFICATION structure                 */
 
typedef struct _LINK_NOTIFICATION {
    int16 group_number;
    int16 filler1;
    char os_type;
    char filler2;
    } LINK_NOTIFICATION;
 
 
/* NOTE: LINK_LOST uses the LINK_NOTIFICATION structure                     */
/*       LIST_ALL_CONNECTIONS uses the GROUP_RECORD structure               */
/*       LIST_ALL_GROUPS uses the GROUP_RECORD structure                    */ 

typedef struct _GROUP_RECORD {
    int16 group_number;
    char group_name [4];
    char uic [3];
    char os_type;
    char node [6];
    char state;
    char reserved [3];
    } GROUP_RECORD;
 
 
/* NOTE: LIST_ALL_ENTRIES uses the QLIST_RECORD structure                   */
 
typedef struct _QLIST_RECORD {
    char q_name [20];
    int16 q_number;
    char attach_flag;
    char reserved;
    } QLIST_RECORD;
 
 
/* NOTE: LIST_ALL_RESP has the same format as GROUP_RECORD                  */
 
typedef struct _LIST_ALL_RESP {
    int16 group_number;
    char group_name [4];
    char uic_number [3];
    char operating_system;
    char decnet_node [6];
    char connection_state;
    char reserved1 [3];
    } LIST_ALL_RESP;
 
 
/* NOTE: LIST_ALL_Q_REQ, DISABLE_Q_NOTIFY_REQ, and                          */
/*       ENABLE_Q_NOTIFY_REQ use the same structure                         */
 
typedef struct _Q_NOTIFY_REQ {
    int32 version;
    int32 user_tag;
    } Q_NOTIFY_REQ;
 
 
/* NOTE: LIST_ALL_Q_RESP uses the Q_NOTIFY_RESP structure                   */
 
typedef struct _LOCATE_Q_REP {
    int32 version;
    int32 search_loc;
    int32 object_handle;
    int32 status;
    int32 trans_id;
    char q_name [256];
    } LOCATE_Q_REP;
 
 
/* ACTION VALUES FOR MRS_DQF_SET message                                    */
#define DQF_SET_OPEN 1
#define DQF_SET_CLOSE 2
#define DQF_SET_FAILOVER 3
 
 
/* STATUS VALUES FOR MRS_DQF_SET message                                    */
#define DQF_SET_ERROR 0
#define DQF_SET_SUCCESS 1
#define DQF_SET_REFUSED 2
 
 
typedef struct _MRS_DQF_SET {
    int16 version;
    int16 action;
    int32 status;
    q_address original_target;
    q_address new_target;
    int32 original_mrs_area_len;
    char original_mrs_area [256];
    } MRS_DQF_SET;
 
 
/* NOTE: MRS_DQF_SET_REP returns the same struture as MRS_DQF_SET           */
 
typedef struct _MRS_DQF_TRANSFER {
    int16 version;
    int32 user_tag;
    int16 status;
    int32 send_count;
    int16 from_dqf_len;
    char from_dqf_file [256];
    int16 to_q;
    } MRS_DQF_TRANSFER;
 
 
/* NOTE: MRS_DQF_TRANSFER_ACK returns same structure as MRS_DQF_TRANSFER    */
/*       MRS_DQF_TRANSFER_REP returns same structure as MRS_DQF_TRANSFER    */
/*       MRS_JRN_DISABLE uses the same structure as MRS_JRN_ENABLE          */
 
/* STATUS VALUES FOR JRN_ENABLE message                                     */
#define JRN_SET_ERROR 0
#define JRN_SET_SUCCESS 1
#define JRN_SET_REFUSED 2
#define JRN_SET_ALREADY_DISABLED 3
#define JRN_SET_ALREADY_ENABLED 4
#define JRN_SET_SERVER_NOTUP 5
 
 
/* NOTE: MRS_JRN_DISABLE_REP return the same structure as MRS_JRN_ENABLE    */
/*       MRS_JRN_ENABLE uses the MRS_JRN_SET_ALL structure                  */
 
typedef struct _MRS_JRN_SET_ALL {
    int32 version;
    int32 dqf_status;
    int32 saf_status;
    int32 pcj_status;
    int32 dlj_status;
    } MRS_JRN_SET_ALL;
 
 
/* NOTE: MRS_JRN_ENABLE_REP return the same structure as MRS_JRN_ENABLE     */
 
 
/* ACTION VALUES FOR MRS_SAF_SET message                                    */
#define SAF_SET_OPEN 4
#define SAF_SET_CLOSE 5
#define SAF_SET_FAILOVER 6
 
/* STATUS VALUES FOR MRS_SAF_SET message                                    */
 
#define SAF_SET_ERROR 0
#define SAF_SET_SUCCESS 1
#define SAF_SET_REFUSED 2
 
 
typedef struct _MRS_SAF_SET {
    int16 version;
    int16 action;
    int32 status;
    q_address original_target;
    q_address new_target;
    int32 original_mrs_area_len;
    char original_mrs_area [256];
    int16 original_owner_group;
    int16 new_owner_group;
    } MRS_SAF_SET;
 
 
/* NOTE: MRS_SAF_SET_REP return the same structure as MRS_SAF_SET           */
/*       MRS_SET_DLJ, MRS_SET_DLJ_REP use the same MRS_SET_DLJ structure    */
 
typedef struct _MRS_SET_DLJ {
    int16 version;
    int32 user_tag;
    int32 status;
    char dlj_file [64];
    } MRS_SET_DLJ;
 
 
/* NOTE: MRS_SET_PCJ, MRS_SET_PCJ_REP use the same MRS_SET_PCJ structure    */
 
typedef struct _MRS_SET_PCJ {
    int16 version;
    int32 user_tag;
    int32 force_j;
    int32 status;
    char dlj_file [64];
    } MRS_SET_PCJ;
 
 
/* NOTE: Q_UPDATE, DISABLE_Q_NOTIFY_RESP, ENABLE_Q_NOTIFY_RESP,             */
/*       and LIST_ALL_Q_RESP use the same Q_NOTIFY_RESP structure           */
 
#define MAX_NUMBER_Q_RECS 50
typedef struct _Q_NOTIFY_RESP {
    int32 version;
    int32 user_tag;
    int32 status_code;
    int32 last_block_flag;
    int32 number_q_recs;
    struct  {
        q_address q_num;
        q_address q_owner;
        int32 q_type;
        int32 q_active_flag;
        int32 q_attached_flag;
        int32 q_owner_pid;
        } q_rec [50];
    } Q_NOTIFY_RESP;
 
 
typedef struct _SBS_REGISTER_HEAD {
    int32 version;
    int32 user_tag;
    int32 mot;
    q_address distribution_q;
    int32 req_ack;
    int32 seq_gap_notify;
    int32 auto_dereg;
    int32 rule_count;
    int32 rule_conjunct;
    } SBS_REGISTER_HEAD;
 
 
typedef struct _SBS_REGISTER_RULE {
    int32 offset;
    int32 data_operator;
    int32 length;
    int32 operand;
    } SBS_REGISTER_RULE;
 
 
#define MAX_SEL_RULES 256
typedef struct _SBS_REGISTER_REQ {
    SBS_REGISTER_HEAD head;
    SBS_REGISTER_RULE rule [256];
    } SBS_REGISTER_REQ;
 
 
typedef struct _SBS_REGISTER_RESP {
    int32 version;
    int32 user_tag;
    int32 status;
    int32 reg_id;
    int32 number_reg;
    } SBS_REGISTER_RESP;
 
 
typedef struct _SBS_DEREGISTER_REQ {
    int32 version;
    int32 user_tag;
    int32 mot;
    q_address distribution_q;
    int32 reg_id;
    int32 req_ack;
    } SBS_DEREGISTER_REQ;
 
 
typedef struct _SBS_DEREGISTER_RESP {
    int32 version;
    int32 status;
    int32 user_tag;
    int32 number_reg;
    } SBS_DEREGISTER_RESP;
 
 
typedef struct _SBS_STATUS_REQ {
    int32 version;
    int32 user_tag;
    int32 start_mot;
    int32 end_mot;
    int32 reset;
    } SBS_STATUS_REQ;
 
 
typedef struct _SBS_STATUS_RESP {
    int32 version;
    int32 user_tag;
    int32 status;
    int32 num_rec;
    int32 last_block;
    char data [31988];
    } SBS_STATUS_RESP;
 
 
typedef struct _SBS_STATUS_RESP_MOT {
    int32 mot;
    union  {
        struct  {
            union  {
                struct  {
                    char s_b1;
                    char s_b2;
                    char s_b3;
                    char s_b4;
                    } S_un_b;
                struct  {
                    uint16 s_w1;
                    uint16 s_w2;
                    } S_un_w;
                uint32 S_addr;
                } inet_addr;
            uint16 inet_port;
            } udp;
        struct  {
            char mca_addr [12];
            char protocol [4];
            } eth;
        struct  {
            char unused [20];
            } dmq;
        int32 filler [5];
        } transport;
    int32 heartbeat_timer;
    int32 xmit_silo;
    int32 rcv_silo;
    int32 rcv_silo_max;
    int32 num_reg;
    int32 complete_rcvd;
    int32 complete_bytes;
    int32 seq_gaps;
    int32 whole_msg_gaps;
    int32 whole_silo_gap;
    struct  {
        char device_name [16];
        struct  {
            uint32 tv_sec;
            uint32 tv_usec;
            } fail_tod;
        int32 msgs_sent;
        int32 bytes_sent;
        int32 pkts_sent;
        int32 pkts_rcvd;
        int32 dupl_pkts_disc;
        } rail [2];
    } SBS_STATUS_RESP_MOT;
 
 
typedef struct _SBS_STATUS_REP_REG_Q {
    q_address reg_q;
    } SBS_STATUS_REP_REG_Q;
 
 
typedef struct _SBS_STATUS_REP_NUM_GROUPS {
    int32 num_groups;
    } SBS_STATUS_REP_NUM_GROUPS;
 
 
typedef struct _SBS_STATUS_RESP_GROUP {
    int32 group;
    int32 rexmit_reqs_to_remote;
    int32 rexmit_sat_by_remote;
    int32 late_rexmit;
    int32 rexmit_reqs_from_remote;
    int32 rexmit_sat_by_local;
    } SBS_STATUS_RESP_GROUP;
 
 
typedef struct _SBS_SEQUENCE_GAP {
    int32 num_msgs_missing;
    int32 sender_group;
    int32 mot;
    int32 channel;
    } SBS_SEQUENCE_GAP;
 
 
typedef struct _TIMER_EXPIRED {
    int32 timer_id;
    char reserved [20];
    } TIMER_EXPIRED;
 
 
typedef struct _UNAVAIL {
    q_address target_q;                 /* Q that is now un-available       */
    } UNAVAIL;
 
 
/*                                                                          */
/*                                                                          */
/************************************************************************** */
/*   Obsolete DmQ Message API                                               */
/************************************************************************** */
/*                                                                          */
/*                                                                          */
 
 
typedef struct _AVAIL_REG_V20 {
    int16 version;
    q_address target_q;
    q_address distribution_q;
    } AVAIL_REG_V20;
 
 
typedef struct _AVAIL_DEREG_V20 {
    int16 version;
    q_address target_q;
    q_address distribution_q;
    char req_ack;
    } AVAIL_DEREG_V20;
 
 
typedef struct _SBS_REG {
    int16 version;
    uint16 mot;
    q_address distribution_q;
    int16 offset;
    char data_operator;
    int16 length;
    uint32 operand;
    char req_ack;
    char req_seqgap;
    char req_autodereg;
    } SBS_REG;
 
 
typedef struct _SBS_REG_REPLY {
    int16 status;
    uint16 reg_id;
    int16 number_reg;
    } SBS_REG_REPLY;
 
 
typedef struct _SBS_REG_EZ {
    int16 version;
    int16 mot;
    q_address distribution_q;
    } SBS_REG_EZ;
 
 
typedef struct _SBS_REG_EZ_REPLY {
    int16 status;
    uint16 reg_id;
    int16 number_reg;
    } SBS_REG_EZ_REPLY;
 
 
typedef struct _SBS_DEREG {
    int16 version;
    uint16 mot;
    q_address distribution_q;
    char req_ack;
    } SBS_DEREG;
 
 
typedef struct _SBS_DEREG_ACK {
    int16 status;
    int16 number_reg;
    } SBS_DEREG_ACK;
 
 
typedef struct _SBS_BS_SEQGAP {
    int32 num_msgs_missing;
    uint16 sender_group;
    uint16 mot;
    uint16 channel;
    } SBS_BS_SEQGAP;
 
 
#if defined(_WIN32) || defined(__osf__)
#pragma pack(8)     /* restore alignment back to the defaults */
#endif
/*                                                                          */
/*                                                                          */
/*                                                                          */
#ifdef __DECC
#pragma member_alignment __restore
#endif
#endif  /* end P_MSG */
/* End of P_MSG                                                             */
