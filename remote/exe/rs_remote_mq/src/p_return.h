/* "@(#) messageq/src/PORTABLE/pams/p_return.h $Revision: 1.1 $" */

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
 
/*** MODULE P_RETURN_DEF IDENT V4.0-09 ***/
/**/
#ifndef _P_RETURN_H
#define _P_RETURN_H 1
/**/
/*                                                                          */
/*                                                                          */
#define PAMS_LAST_FAIL -382
#define PAMS_LAST_SUCCESS 45
#define PAMS__ABORT -82
#define PAMS__ACKTMO -16
#define PAMS__AREATOSMALL -124
#define PAMS__ATTACHED -360
#define PAMS__BADARGLIST -226
#define PAMS__BADASSIGN -84
#define PAMS__BADASTPARM -140
#define PAMS__BADCMD -258
#define PAMS__BADDECLARE -10
#define PAMS__BADDELIVERY -60
#define PAMS__BADFREE -12
#define PAMS__BADHANDLE -322
#define PAMS__BADINITFILE -282
#define PAMS__BADJOURNAL -178
#define PAMS__BADLOGIC -238
#define PAMS__BADMSGBUF -308
#define PAMS__BADNAME -254
#define PAMS__BADPARAM -186
#define PAMS__BADPRIORITY -58
#define PAMS__BADPROCNUM -62
#define PAMS__BADQTYPE -288
#define PAMS__BADRECEIVE -26
#define PAMS__BADRESPQ -224
#define PAMS__BADSCRIPT -102
#define PAMS__BADSELIDX -182
#define PAMS__BADSEQ -208
#define PAMS__BADSYNCHNUM -66
#define PAMS__BADTAG -248
#define PAMS__BADTBQHANDLE -250
#define PAMS__BADTIME -110
#define PAMS__BADTMPPROC -64
#define PAMS__BADTMPSYNCH -68
#define PAMS__BADUMA -222
#define PAMS__BIGBLKSIZE -24
#define PAMS__BIGMSG -120
#define PAMS__BOUND -350
#define PAMS__BUFFEROVF -174
#define PAMS__BUSNOTSET -290
#define PAMS__CANCEL -168
#define PAMS__CIRACT -78
#define PAMS__COMMERR -180
#define PAMS__CONFIRMREQ 37
#define PAMS__CONFLICT -298
#define PAMS__CREATEFAIL -134
#define PAMS__DCLTMPFAIL -136
#define PAMS__DECLARED -106
#define PAMS__DETACHED 41
#define PAMS__DISCL_FAILED -192
#define PAMS__DISCL_SUCCESS 13
#define PAMS__DISC_FAILED -190
#define PAMS__DISC_SUCCESS 11
#define PAMS__DLJ_FAILED -194
#define PAMS__DLJ_SUCCESS 15
#define PAMS__DLQ_FAILED -196
#define PAMS__DLQ_SUCCESS 17
#define PAMS__DNSCLASSBAD -310
#define PAMS__DNSDIRFAIL -312
#define PAMS__DNSFMTBAD -314
#define PAMS__DQF_DEVICE_FAIL -198
#define PAMS__DQF_FULL -202
#define PAMS__DUPLQNAME -244
#define PAMS__ENDOFSOURCE -362
#define PAMS__ENQUEUED 33
#define PAMS__EXCEEDQUOTA -56
#define PAMS__EXCMAXUNCONF -170
#define PAMS__EXHAUSTBLKS -108
#define PAMS__EXPIRED -296
#define PAMS__EX_Q_LEN -156
#define PAMS__FAILED -320
#define PAMS__FATAL -272
#define PAMS__FMLERROR -380
#define PAMS__GROUPNOTSET -292
#define PAMS__IDXTBLFULL -184
#define PAMS__INSQUEFAIL -132
#define PAMS__INTERNAL -262
#define PAMS__INVACCESS -252
#define PAMS__INVALIDID -36
#define PAMS__INVALIDNUM -52
#define PAMS__INVBUFFPTR -176
#define PAMS__INVFORMAT -50
#define PAMS__INVJH -204
#define PAMS__INVUCBCNTRL -74
#define PAMS__INVUMA -200
#define PAMS__JOURNAL_FAIL -286
#define PAMS__JOURNAL_FULL -284
#define PAMS__JOURNAL_ON 5
#define PAMS__LINK_DOWN -206
#define PAMS__LINK_UP 3
#define PAMS__LOCKED -364
#define PAMS__LOGNAME -90
#define PAMS__LOGNAME2 -92
#define PAMS__LOGNAME3 -94
#define PAMS__LOGNAME4 -96
#define PAMS__LOGNAME5 -98
#define PAMS__MRQTBLFULL -164
#define PAMS__MRS_RES_EXH -212
#define PAMS__MSG -366
#define PAMS__MSGACT -8
#define PAMS__MSGTOBIG -22
#define PAMS__MSGTOSMALL -122
#define PAMS__MSGUNDEL -18
#define PAMS__NAMETOOLONG -256
#define PAMS__NEED_BUFFER_PTR -378
#define PAMS__NETERROR -276
#define PAMS__NETLINKLOST -280
#define PAMS__NETNOLINK -278
#define PAMS__NOACCESS -302
#define PAMS__NOACL -300
#define PAMS__NOCANSEND -126
#define PAMS__NOLINK -76
#define PAMS__NOMEMORY -352
#define PAMS__NOMOREJH -214
#define PAMS__NOMOREMSG 7
#define PAMS__NOMORETAG -354
#define PAMS__NOMRQRESRC -242
#define PAMS__NOMRS -188
#define PAMS__NONEXT -368
#define PAMS__NOOBJECT -166
#define PAMS__NOOPEN -100
#define PAMS__NOPRIV -316
#define PAMS__NOQUOTA -264
#define PAMS__NOSEND -2
#define PAMS__NOSUCHPCJ -218
#define PAMS__NOSUCHTAG -324
#define PAMS__NOTACTIVE -54
#define PAMS__NOTALLOCATE -20
#define PAMS__NOTBOUND -328
#define PAMS__NOTDCL -70
#define PAMS__NOTFLD -376
#define PAMS__NOTJRN -210
#define PAMS__NOTMRQ -370
#define PAMS__NOTPRIMARYQ -266
#define PAMS__NOTSECONDARYQ -270
#define PAMS__NOTSUPPORTED -268
#define PAMS__NO_DQF -228
#define PAMS__NO_SAF -230
#define PAMS__NO_UMA 25
#define PAMS__NSACCESSERR -344
#define PAMS__NSBADATTR -342
#define PAMS__NSBADCLASS -334
#define PAMS__NSBADNAME -330
#define PAMS__NSBADPATH -332
#define PAMS__NSENTRYEXISTS -338
#define PAMS__NSNOTAVAIL -348
#define PAMS__NSUNKNOWNATTR -340
#define PAMS__NSUNKNOWNENTRY -336
#define PAMS__NSWRITERR -346
#define PAMS__OBJNOTACTIVE -172
#define PAMS__PAMSDOWN -138
#define PAMS__PNUMNOEXIST -54
#define PAMS__POSSDUPL -158
#define PAMS__PREVCALLBUSY -294
#define PAMS__PROPAGATE 39
#define PAMS__PROTOCOL -80
#define PAMS__QUECORRUPT -128
#define PAMS__RECOVERMODE 23
#define PAMS__REJECTED -216
#define PAMS__REMQUEFAIL -130
#define PAMS__RESPQREQ -260
#define PAMS__RESRCFAIL -246
#define PAMS__RTS_FAILED -232
#define PAMS__RTS_SUCCESS 19
#define PAMS__SAF_DEVICE_FAIL -234
#define PAMS__SAF_FAILED -236
#define PAMS__SAF_FORCED 43
#define PAMS__SAF_SUCCESS 21
#define PAMS__SELACTIVE -372
#define PAMS__SELRCVACT -240
#define PAMS__SENDER_TMO_EXPIRED -162
#define PAMS__STALE -304
#define PAMS__STATECHANGE -72
#define PAMS__STOPPED -358
#define PAMS__STORED 31
#define PAMS__STUB -160
#define PAMS__SUCCESS 1
#define PAMS__TIMEOUT -14
#define PAMS__TIMERACT -6
#define PAMS__TRACEBACK 29
#define PAMS__TRUNCATED 45
#define PAMS__UCBERROR -220
#define PAMS__UMA_NA 27
#define PAMS__UNATTACHEDQ 35
#define PAMS__UNBINDING -356
#define PAMS__WAKEFAIL -4
#define PAMS__WRONGDOS -274
/*                                                                          */
/*                                                                          */
/**/
#endif  /* end P_RETURN_H */
