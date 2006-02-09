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
 
/* $RCSfile: p_symbol.h,v $, $Revision: 1.1 $, $Date: 2006-02-09 09:51:05 $ */
/*                                                                          */
#ifndef _P_SYMBOL_H
#define _P_SYMBOL_H 1
/*                                                                          */
/************************************************************************** */
/* Define the Selective Receive symbols                                     */
/************************************************************************** */
/*                                                                          */


#define PSEL_SOURCE -1
#define PSEL_CLASS -2
#define PSEL_TYPE -3
#define PSEL_CORRELATION_ID -4
#define PSEL_SEQUENCE_NUMBER -5

#define PSEL_PRI_P0 -6
#define PSEL_PRI_P1 -7
#define PSEL_PRI_ANY -8

/*                                                                          */
/************************************************************************** */
/* Define the Select-by-Queue macros                                        */
/************************************************************************** */
/*                                                                          */
#define PSEL_BY_MASK -1
#define PSEL_PQ -2
#define PSEL_AQ -3
#define PSEL_PQ_AQ -4
#define PSEL_AQ_PQ -5
#define PSEL_PQ_TYPE -6
#define PSEL_PQ_CLASS -7

#define PSEL_UCB -9
#define PSEL_TQ_PQ -10
#define PSEL_TQ_PQ_AQ -11
#define PSEL_PQ_PRI -12

/*                                                                          */
/************************************************************************** */
/* Define the Selective Operators                                           */
/************************************************************************** */
/*                                                                          */

#define PSEL_OPER_ANY     0
#define PSEL_OPER_EQ     -1
#define PSEL_OPER_NEQ    -2
#define PSEL_OPER_GTR    -3
#define PSEL_OPER_LT     -4
#define PSEL_OPER_GTRE   -5
#define PSEL_OPER_LTE    -6
/*                                                                          */
/************************************************************************** */
/* Define the Selective Order                                               */
/************************************************************************** */
/*                                                                          */
#define PSEL_ORDER_FIFO   0
#define PSEL_ORDER_MIN   -1
#define PSEL_ORDER_MAX   -2
/*                                                                          */
/************************************************************************** */
/* Define Delivery Mode symbols                                             */
/************************************************************************** */
/*                                                                          */
#define PDEL_MODE_WF_SAF 25
#define PDEL_MODE_WF_DQF 26
#define PDEL_MODE_WF_NET 27
#define PDEL_MODE_WF_RCM 28
#define PDEL_MODE_WF_MEM 29
#define PDEL_MODE_AK_SAF 30
#define PDEL_MODE_AK_DQF 31
#define PDEL_MODE_AK_NET 32
#define PDEL_MODE_AK_RCM 33
#define PDEL_MODE_AK_MEM 34
#define PDEL_MODE_NN_SAF 35
#define PDEL_MODE_NN_DQF 36
#define PDEL_MODE_NN_NET 37
#define PDEL_MODE_NN_RCM 38
#define PDEL_MODE_NN_MEM 39
#define PDEL_MODE_WF_DEQ 40
#define PDEL_MODE_AK_DEQ 41
#define PDEL_MODE_WF_CONF 42
#define PDEL_MODE_AK_CONF 43
#define PDEL_MODE_WF_ACK 44
#define PDEL_MODE_AK_ACK 45
/*                                                                          */
/************************************************************************** */
/* Define Undeliverable Message Action symbols                              */
/************************************************************************** */
/*                                                                          */
#define PDEL_UMA_RTS 1
#define PDEL_UMA_DLJ 2
#define PDEL_UMA_DLQ 3
#define PDEL_UMA_SAF 4
#define PDEL_UMA_DISC 5
#define PDEL_UMA_DISCL 6
/*                                                                          */
/************************************************************************** */
/* Define PAMS_CONFIRM_MSG force journal symbols                            */
/************************************************************************** */
/*                                                                          */
#define PDEL_DEFAULT_JRN 0
#define PDEL_FORCE_JRN 1
#define PDEL_NO_JRN 2
/*                                                                          */
/************************************************************************** */
/* Define PAMS_LOCATE_Q symbols                                             */
/************************************************************************** */
/*                                                                          */
#define PSEL_TBL_PROC -50
#define PSEL_TBL_GRP -51
#define PSEL_TBL_DNS_CACHE -52
#define PSEL_TBL_BUS -52
#define PSEL_TBL_DNS_LOW -53
#define PSEL_TBL_BUS_LOW -53
#define PSEL_TBL_DNS_MED -54
#define PSEL_TBL_BUS_MED -54
#define PSEL_TBL_DNS_HIGH -55
#define PSEL_TBL_BUS_HIGH -55
#define PSYM_WF_RESP -70
#define PSYM_AK_RESP -71
/*                                                                          */
/************************************************************************** */
/* Define PAMS_ATTACH_Q symbols                                             */
/************************************************************************** */
/*                                                                          */
#define PSYM_ATTACH_PQ -200
#define PSYM_ATTACH_SQ -201
#define PSYM_ATTACH_MRQ -207
#define PSYM_ATTACH_BY_NAME -210
#define PSYM_ATTACH_BY_NUMBER -211
#define PSYM_ATTACH_TEMPORARY -212
/*                                                                          */
/************************************************************************** */
/* Define PAMS_DCL_PROCESS symbols                                          */
/************************************************************************** */
/*                                                                          */
#define PSYM_DCL_PQ -200
#define PSYM_DCL_SQ -201
#define PSYM_SCOPE_LOCAL -202
#define PSYM_SCOPE_GLOBAL -203
/*                                                                          */
/************************************************************************** */
/* Define PAMS_WAKE symbols                                                 */
/************************************************************************** */
/*                                                                          */
#define PSYM_IGNORE -230
#define PSYM_SET_UCB_PRI_P0 -231
#define PSYM_SET_UCB_PRI_P1 -232
#define PSYM_SET_UCB_A -240
#define PSYM_SET_UCB_B -241
#define PSYM_SET_UCB_C -242
#define PSYM_SET_UCB_D -243
#define PSYM_SET_UCB_E -244
#define PSYM_SET_UCB_F -245
#define PSYM_SET_UCB_G -246
#define PSYM_SET_UCB_H -247
#define PSYM_SET_UCB_I -248
#define PSYM_SET_UCB_J -249
/*                                                                          */
/************************************************************************** */
/* Define Data Representation symbols                                       */
/************************************************************************** */
/*                                                                          */
#define PSYM_LITTLE_ENDIAN 0
#define PSYM_VAX_BYTE_ORDER 0
#define PSYM_BIG_ENDIAN 1
#define PSYM_NETWORK_BYTE_ORDER 1
#define PSYM_ASN1 2
#define PSYM_SDM 2
#define PSYM_UNKNOWN 3
#define PSYM_FML 4

/*                                                                          */
/************************************************************************** */
/* Define LINK MANAGEMENT symbols                                           */
/************************************************************************** */
/*                                                                          */
/*                                                                          */
/* Define platform IDs                                                      */
/*                                                                          */
#define PSYM_PLATFORM_VMS_VAX 0
#define PSYM_PLATFORM_ULTRIX_VAX 1
#define PSYM_PLATFORM_ULTRIX_MIPS 2
#define PSYM_PLATFORM_HPUX_HPPA 3
#define PSYM_PLATFORM_SYSV_M88K 4
#define PSYM_PLATFORM_SUNOS_SPARC 5
#define PSYM_PLATFORM_AIX_RS6000 6
#define PSYM_PLATFORM_OS2_X86 7
#define PSYM_PLATFORM_MSDOS_X86 8
#define PSYM_PLATFORM_LINUX 9
#define PSYM_PLATFORM_ELN_VAX 10
#define PSYM_PLATFORM_MACINTOSH_M68K 11
#define PSYM_PLATFORM_SCO_X86 12
#define PSYM_PLATFORM_SYSV_M68K 13
#define PSYM_PLATFORM_VMS_AXP 14
#define PSYM_PLATFORM_UNIX 15
#define PSYM_PLATFORM_NT_X86 16
#define PSYM_PLATFORM_OSF1_AXP 17
#define PSYM_PLATFORM_SOLARIS_SPARC 18
#define PSYM_PLATFORM_NT_AXP 19
#define PSYM_PLATFORM_NT_POWERPC 20
#define PSYM_PLATFORM_OSF1_RS6000 21
#define PSYM_PLATFORM_OSF1_HPPA 22
#define PSYM_PLATFORM_SOLARIS_X86 23
#define PSYM_PLATFORM_MACINTOSH_POWERPC 24
#define PSYM_PLATFORM_OS2_POWERPC 25
#define PSYM_PLATFORM_HPUX_M68K 26
#define PSYM_PLATFORM_IRIX_MIPS 27
#define PSYM_PLATFORM_SYSV_X86 28
#define PSYM_PLATFORM_DYNIX_X86 29
#define PSYM_PLATFORM_UNKNOWN 99
/*                                                                          */
/* Define reconnect timer values                                            */
/*                                                                          */
#define PSYM_LINKMGT_USE_PREVIOUS -1
#define PSYM_LINKMGT_NO_TIMER 0
#define PSYM_LINKMGT_MAX_NODE_LEN 255
#define PSYM_LINKMGT_MAX_TRNSPRT_LEN 16
#define PSYM_LINKMGT_REQ_VERS 10
#define PSYM_LINKMGT_RESP_VERS 10
/*                                                                          */
/* Define group number values                                               */
/*                                                                          */
#define PSYM_LINKMGT_ALL_GROUPS -1
/*                                                                          */
/* success status codes                                                     */
/*                                                                          */
#define PSYM_LINKMGT_SUCCESS 1
/*                                                                          */
/* error status codes                                                       */
/*                                                                          */
#define PSYM_LINKMGT_MSGFMT -2
#define PSYM_LINKMGT_MSGCONTENT -4
#define PSYM_LINKMGT_ALREADYUP -6
#define PSYM_LINKMGT_NOPRIV -8
#define PSYM_LINKMGT_NOGROUP -10
#define PSYM_LINKMGT_NOTSUPPORTED -12
#define PSYM_LINKMGT_NOTRANSPORT -14
#define PSYM_LINKMGT_OPERATIONFAIL -16
/*                                                                          */
/* Command codes                                                            */
/*                                                                          */
#define PSYM_LINKMGT_CMD_INQUIRY 101
#define PSYM_LINKMGT_CMD_CONNECT 102
#define PSYM_LINKMGT_CMD_DISCONNECT 103
#define PSYM_LINKMGT_CMD_ENABLE 104
#define PSYM_LINKMGT_CMD_DISABLE 105
/*                                                                          */
/* Transport type codes                                                     */
/*                                                                          */
#define PSYM_LINKMGT_LOCAL 201
#define PSYM_LINKMGT_DECNET 202
#define PSYM_LINKMGT_TCPIP 203
#define PSYM_LINKMGT_ALL_TRANSPORTS 204
/*                                                                          */
/* Link state codes                                                         */
/*                                                                          */
#define PSYM_LINKMGT_UNKNOWN 301
#define PSYM_LINKMGT_NOCNT 302
#define PSYM_LINKMGT_CONNECTED 303
#define PSYM_LINKMGT_DISABLED 304
/*                                                                          */
/************************************************************************** */
/*    Define the LIST_ALL_GROUPS & LIST_ALL_CONNECTIONS OS Types            */
/************************************************************************** */
/*                                                                          */
#define PSYM_OS_TYPE_AIX 'I'
#define PSYM_OS_TYPE_VAXELN 'E'
#define PSYM_OS_TYPE_MACINTOSH 'A'
#define PSYM_OS_TYPE_HPUX 'H'
#define PSYM_OS_TYPE_IRIX 'G'
#define PSYM_OS_TYPE_DYNIX 'd'
#define PSYM_OS_TYPE_MSDOS 'D'
#define PSYM_OS_TYPE_NT 'N'
#define PSYM_OS_TYPE_OS2 'O'
#define PSYM_OS_TYPE_OSF1 '1'
#define PSYM_OS_TYPE_LINUX 'M'
#define PSYM_OS_TYPE_SCO 'C'
#define PSYM_OS_TYPE_SOLARIS 'L'
#define PSYM_OS_TYPE_SUNOS 'S'
#define PSYM_OS_TYPE_SYSV '5'
#define PSYM_OS_TYPE_ULTRIX_VAX 'X'
#define PSYM_OS_TYPE_ULTRIX_MIPS 'Y'
#define PSYM_OS_TYPE_UNIX 'U'
#define PSYM_OS_TYPE_VMS 'V'
#define PSYM_OS_TYPE_UNKNOWN '*'
/*                                                                          */
/************************************************************************** */
/*    Define the Queue Type symbols                                         */
/************************************************************************** */
/*                                                                          */
#define PSYM_QTYPE_PQ 1
#define PSYM_QTYPE_SQ 2
#define PSYM_QTYPE_MRQ 3
#define PSYM_QTYPE_UCB 4
/*                                                                          */
/************************************************************************** */
/*     define PAMS_DETACH_Q symbols                                         */
/************************************************************************** */
/*                                                                          */
#define PSYM_NOFLUSH_Q 1
#define PSYM_DETACH_ALL 2
#define PSYM_CANCEL_SEL_MASK 3
/*                                                                          */
/************************************************************************** */
/*     Define status return symbols for que notify services.                */
/************************************************************************** */
/*                                                                          */
#define PSYM_QNOT_SUCCESS 1
#define PSYM_QNOT_FAIL 0
#define PSYM_QNOT_BADPARAM -2
/*                                                                          */
/************************************************************************** */
/*     Define symbols special to SBS                                        */
/************************************************************************** */
/*                                                                          */
#define PSEL_SBS_MAX_SEL_RULES 256
#define PSEL_SBSE_MAX_RULES 10
/*                                                                          */
#define PSYM_SBS_RAIL_A 0
#define PSYM_SBS_RAIL_B 1
/*                                                                          */
#define PSYM_SBS_SUCCESS 1
#define PSYM_SBS_BADPARAM 20
#define PSYM_SBS_RESRCFAIL 292
#define PSYM_SBS_NOMATCH 1012
/*                                                                          */
/*                                                                          */
#define PSEL_ALL_RULES 1
#define PSEL_ANY_RULE 2
#define PSEL_OPER_AND 7
/*                                                                          */
/************************************************************************** */
/*     Define symbols for SDM tags                                          */
/************************************************************************** */
/*                                                                          */
#define PSDM_NULL_TAG -2146435073
#define PSDM_BOOLEAN -2130706432
#define PSDM_INT8 -1996488704
#define PSDM_UINT8 -1862270976
#define PSDM_CHAR -1728053248
#define PSDM_INT16 -2113929216
#define PSDM_UINT16 -1979711488
#define PSDM_INT32 -2080374784
#define PSDM_UINT32 -1946157056
#define PSDM_TIME -1811939328
#define PSDM_FLOAT -2097152000
#define PSDM_QID -1962934272
#define PSDM_INT64 -2063597568
#define PSDM_UINT64 -1929379840
#define PSDM_DOUBLE -1795162112
#define PSDM_STRING -2013265920
#define PSDM_OCTET -1879048192
#define PSDM_ARRAY 1073741824
#define PSDM_FIRST 1
#define PSDM_LAST 2
#define PSDM_ANY 4
#define PSDM_PREVIOUS 1024
#define PSDM_NEXT 2048
#define PSDM_BEFORE 4096
#define PSDM_AFTER 8192
#define PSDM_AT 16384
/*                                                                          */
/************************************************************************** */
/*   Define symbols for large, FML, doubly-reference, and handled messages  */
/************************************************************************** */
/*                                                                          */
#define PSYM_MSG_HANDLE -1
#define PSYM_MSG_LARGE -2
#define PSYM_MSG_FML -3
#define PSYM_MSG_BUFFER_PTR -4

/*                                                                          */
/************************************************************************** */
/*     Symbols for psb flags field indicating what psb data is valid        */
/************************************************************************** */
/*                                                                          */

#define PSYM_SHOW_VERSION               (1 <<  0)
#define PSYM_SHOW_STATUS                (1 <<  1)
#define PSYM_SHOW_SIZE                  (1 <<  2)
#define PSYM_SHOW_FLAGS                 (1 <<  3)
#define PSYM_SHOW_URCODE                (1 <<  5)
#define PSYM_SHOW_TARGET                (1 << 10)
#define PSYM_SHOW_ORIGINAL_TARGET       (1 << 11)
#define PSYM_SHOW_SOURCE                (1 << 12)
#define PSYM_SHOW_ORIGINAL_SOURCE       (1 << 13)
#define PSYM_SHOW_DELIVERY              (1 << 14)
#define PSYM_SHOW_PRIORITY              (1 << 15)
#define PSYM_SHOW_ENDIAN                (1 << 16)
#define PSYM_SHOW_CORRELATION_ID        (1 << 17)

/*                                                                          */
/************************************************************************** */
/* NOTE: The following symbols are OBSOLETE and may be removed at any time  */
/************************************************************************** */
/************************************************************************** */
/* OBSOLETE non-MRS Delivery Mode symbols                                   */
/************************************************************************** */
/*                                                                          */
#define PDEL_MODE_DG_LOG 0
#define PDEL_MODE_DG 1
#define PDEL_MODE_RTS 10
#define PDEL_MODE_WFQ 20
/*                                                                          */
/************************************************************************** */
/* OBSOLETE platform IDs                                                    */
/************************************************************************** */
/*                                                                          */
#define PSYM_PLATFORM_VAX_VMS 0
#define PSYM_PLATFORM_VAX_ULTRIX 1
#define PSYM_PLATFORM_RISC_ULTRIX 2
#define PSYM_PLATFORM_HP9000_HPUX 3
#define PSYM_PLATFORM_MOTOROLA_VR32 4
#define PSYM_PLATFORM_SPARC_SUNOS 5
#define PSYM_PLATFORM_IBM_RS6000_AIX 6
#define PSYM_PLATFORM_OS2 7
#define PSYM_PLATFORM_MSDOS 8
#define PSYM_PLATFORM_RSX_PDP11 9
#define PSYM_PLATFORM_PDP11_RSX 9
#define PSYM_PLATFORM_VAXELN 10
#define PSYM_PLATFORM_MACINTOSH 11
#define PSYM_PLATFORM_SCO_UNIX 12
#define PSYM_PLATFORM_M68K 13
#define PSYM_PLATFORM_WINDOWSNT 16
/*									    */
/************************************************************************** */
/* OBSOLETE platform IDs                                                    */
/************************************************************************** */
/*                                                                          */
#define PSYM_OS_TYPE_RSX 'M'
/*                                                                          */
/************************************************************************** */
/* OBSOLETE MRS recovery mode                                               */
/************************************************************************** */
/*                                                                          */
#define PDEL_MODE_ST_RECOVER 22
/*                                                                          */
/************************************************************************** */
/* OBSOLETE name space list                                                 */
/************************************************************************** */
/*                                                                          */
#define PSEL_TBL_QMA -56
/*                                                                          */
/* --------------- End of PAMS symbol file ------------------------------   */
/*									*/
#endif
