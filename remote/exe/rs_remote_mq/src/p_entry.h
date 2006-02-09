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
 
/* $RCSfile: p_entry.h,v $, $Revision: 1.1 $, $Date: 2006-02-09 09:51:05 $ */
/*                                                                          */
/************************************************************************** */
/*   Define PAMS structures used in calls                                   */
/************************************************************************** */
/*                                                                          */
/*                                                                          */
 
#ifndef _P_ENTRY_H
#define _P_ENTRY_H 1
 
#include <sys/types.h>

#ifndef __INTS_LOADED
 
#if defined(__DECC) && defined(__VMS)
#include <ints.h>
#ifndef __ALPHA
typedef struct { long l[2]; } int64;
typedef struct { unsigned long l[2]; } uint64;
#endif
#else
#if defined(__BORLANDC__)
#include <systypes.h>
#else
#define __INTS_LOADED 1
 
#if !(defined(_IBMR2) && defined(_H_INTTYPES) && defined(_ALL_SOURCE))
typedef char int8;
typedef short int16;
#endif	/* _IBMR2 */

typedef unsigned char  uint8;
typedef unsigned short uint16;
 
#if defined(__alpha) && defined(__osf__)
#if !(defined(_IBMR2) && defined(_H_INTTYPES) && defined(_ALL_SOURCE))
typedef	int int32;
typedef long int64;
#endif	/* _IBMR2 */
typedef	unsigned int uint32;
typedef unsigned long uint64;
#else
#if !(defined(_IBMR2) && defined(_H_INTTYPES) && defined(_ALL_SOURCE))
typedef long int32;
typedef struct { long l[2]; } int64;
#endif	/* _IBMR2 */
typedef unsigned long uint32;
typedef struct { unsigned long l[2]; } uint64;
#endif /* __alpha && __osf__ */
 
#endif /* __BORLANDC__       */
#endif /* __DECC             */
#endif /* __INTS_LOADED      */
 
 
#if ! defined(PAMSAPI)
#if defined(_WIN32)
#define PAMSAPI __stdcall
#define PAMSEXPORT __declspec(dllexport)
#define PAMSIMPORT extern __declspec(dllexport)
#else
#if defined(_WINDOWS) || defined(_Windows)
#define PAMSAPI FAR pascal _export
#define PAMSEXPORT
#define PAMSIMPORT extern
#else
#if defined(__OS2__) && defined(__BORLANDC__)
#define PAMSAPI __syscall
#define PAMSEXPORT
#define PAMSIMPORT extern
#else
#if defined(__OS2__) && defined(__WATCOM__)
#define PAMSAPI _System
#define PAMSEXPORT
#define PAMSIMPORT extern
#else
#define PAMSAPI
#define PAMSEXPORT
#define PAMSIMPORT extern
#endif /* __WATCOM__   */
#endif /* __BORLAND__C */
#endif /* _WINDOWS     */
#endif /* _WIN32       */
#endif /* PAMSAPI      */
  
#ifndef	GEN_PAMS_V1
#define	GEN_PAMS_V1 0			/* Generate current structures      */
#endif
 
#ifndef DMQ_GENERIC_VERSION
#define	DMQ_GENERIC_VERSION	"5.0"
#endif
 
#define	PSB		psb
 
#ifdef __DECC
#pragma member_alignment __save
#pragma member_alignment
#endif
 
/*                                                                          */
#ifndef PAMS_ADDRESS_DEFINED
#define PAMS_ADDRESS_DEFINED 1
 
/*                                                                          */
/*   Define the queue address record data type                              */
/*                                                                          */
typedef int32 all;
typedef struct _group_queue {
    int16 queue;
    int16 group;
    } group_queue;
typedef union _q_address {
    int32 all;
    group_queue au;
    } q_address;
/*                                                                          */
/* pams_address and Pams_Address and PAMS_ADDRESS                           */
/* to resolve to the MessageQ q_address structure                           */
/*                                                                          */
 
#ifndef pams_address
#define pams_address q_address
#endif
 
#ifndef Pams_Address
#define Pams_Address q_address
#endif
 
#ifndef PAMS_ADDRESS
#define PAMS_ADDRESS q_address
#endif
 
#endif
 
#define PAMS_HANDLE_SIZE 16
typedef char pams_handle[PAMS_HANDLE_SIZE];
#define PAMS_CORRELATION_ID_SIZE 32
typedef char pams_correlation_id[PAMS_CORRELATION_ID_SIZE]; 
typedef uint32 pams_sequence_number[2];
#define PAMS_SEQUENCE_NUMBER_SIZE 8

/*                                                                          */
/*   Define the PAMS Show Buffer structure                                  */
/*   Used to return further information from PAMS_GET_MSG, PAMS_GET_MSGW,   */
/*   and PAMS_GET_MSGA calls.                                               */
/*                                                                          */
struct show_buffer {
    int32 version;
    int32 transfer_status;
    int32 transfer_size;
    int32 flags;
    int32 urcode;		/* used by Tuxedo forwarding agent in 5.0 */
    char reserved [20];
    int32 target;
    int32 original_target;
    int32 source;
    int32 original_source;
    int32 delivery;
    int32 priority;
    int32 endian;
    pams_correlation_id correlation_id;
    } ;
/*                                                                          */
/*   Define the PAMS Status Block (PSB) structure                           */
/*   Used for extended return status' from PAMS and to obtain               */
/*   the sequence number for later calls to PAMS_CONFIRM_MSG                */
/*                                                                          */
 
#if GEN_PAMS_V1
struct PSB {
   u_short  type;
   u_short  call_depend;
   uint32   status;
   uint32   msg_seq_number[2];
   uint32   uma_status;
   char     filler[12];
} ;
#else
struct PSB {
   short    type_of_psb;
   short    call_dependent;
   int32    del_psb_status;
   uint32   seq_number[2];
   int32    uma_psb_status;
   short    psb_reserved[6];
} ;
#endif
struct PSB2 {
   short    type_of_psb;
   short    call_dependent;
   int32    del_psb_status;
   uint32   seq_number[2];
   int32    uma_psb_status;
   int32    function_status;
   short    psb_reserved[4];
};
 
#define psb2 PSB2
 
/*                                                                          */
/*                                                                          */
/*                                                                          */
/*   Define the Compound Selection data type                                */
/*                                                                          */

typedef struct _selection_array_component {
    int32 queue;
    int32 priority;
    int32 key_1_offset;
    int32 key_1_size;
    int32 key_1_value;
    int32 key_1_oper;
    int32 key_2_offset;
    int32 key_2_size;
    int32 key_2_value;
    int32 key_2_oper;
    int32 order_offset;
    int32 order_size;
    int32 order_order;

    union {
       pams_correlation_id  correlation_id;
       pams_sequence_number sequence_number;
    } extended_key;
} selection_array_component;

/*                                                                          */
/*                                                                          */
/************************************************************************** */
/*      Entry points for PAMS subroutines.                                  */
/************************************************************************** */
/*                                                                          */
/*                                                                          */
#ifdef __cplusplus
extern "C" {
#endif
 
PAMSIMPORT int32 PAMSAPI pams_attach_q (
   int32 *,               /* ptr to attach mode                  */
   q_address *,           /* ptr to queue attached               */
   int32 *,               /* ptr to queue type                   */
   char *,                /* ptr to queue name to attach         */
   int32 *,               /* ptr to queue name length            */
   int32 *,               /* ptr to name space list	         */
   int32 *,               /* ptr to name space list length       */
   int32 *,               /* ptr to timeout value                */
   char *,                /* reserved - must be NULL             */
   char *                 /* reserved - must be NULL             */
);
 
PAMSIMPORT int32 PAMSAPI pams_bind_q (
   q_address *,           /* ptr to address to be bound          */
   char *,                /* ptr to queue name to be bound       */
   int32 *,               /* ptr to queue name length            */
   int32 *,               /* ptr to name space list              */
   int32 *,               /* ptr to name space list length       */
   int32 *,               /* ptr to timeout value                */
   char *                 /* reserved - must be NULL             */
);
 
PAMSIMPORT int32 PAMSAPI pams_detach_q (
   q_address *,           /* ptr to queue to detach              */
   int32 *,               /* ptr detach option list              */
   int32 *,               /* ptr to option list length           */
   int32 *                /* ptr to count of msgs flushed        */
);
 
PAMSIMPORT int32 PAMSAPI pams_locate_q (
   char *,                /* ptr to queue name to locate         */
   int32 *,               /* ptr to queue name length            */
   q_address *,           /* ptr to queue number located         */
   int32 *,               /* ptr to locate wait mode             */
   int32 *,               /* ptr to locate req id                */
   int32 *,               /* ptr to locate resp queue            */
   int32 *,               /* ptr to name space list              */
   int32 *,               /* ptr to name space list length       */
   int32 *                /* ptr to timeout value                */
);
 
PAMSIMPORT int32 PAMSAPI pams_put_msg (
   char          *,       /* ptr to char buffer                  */
   char          *,       /* ptr to priority                     */
   q_address     *,       /* ptr to target                       */
   short         *,       /* ptr to class                        */
   short         *,       /* ptr to type                         */
   char          *,       /* ptr to delivery byte                */
   short         *,       /* ptr to output length                */
   int32         *,       /* ptr to timeout value                */
   struct PSB    *,       /* ptr to PSB structure                */
   char          *,       /* ptr to UMA                          */
   q_address     *,       /* ptr to response queue               */
   int32         *,       /* ptr to large input length           */
   pams_correlation_id,   /* correlation identifier string       */
   char          *        /* reserved - must be NULL             */
);
 
PAMSIMPORT int32 PAMSAPI pams_get_msg (
   char          *,       /* ptr to char buffer                  */
   char          *,       /* ptr to priority                     */
   q_address     *,       /* ptr to source                       */
   short         *,       /* ptr to class                        */
   short         *,       /* ptr to type                         */
   short         *,       /* ptr to input length                 */
   short         *,       /* ptr to output length                */
   int32         *,       /* ptr to selection address            */
   struct PSB    *,       /* ptr to PSB structure                */
   struct show_buffer *,  /* ptr to show buffer                  */
   int32         *,       /* ptr to show buffer size             */
   int32         *,       /* ptr to large input length           */
   int32         *,       /* ptr to large output length          */
   char          *        /* reserved - must be NULL             */
);
 
PAMSIMPORT int32 PAMSAPI pams_get_msgw (
   char          *,       /* ptr to char buffer                  */
   char          *,       /* ptr to priority                     */
   q_address     *,       /* ptr to source                       */
   short         *,       /* ptr to class                        */
   short         *,       /* ptr to type                         */
   short         *,       /* ptr to input length                 */
   short         *,       /* ptr to output length                */
   int32         *,       /* ptr to timeout value                */
   int32         *,       /* ptr to selection address            */
   struct PSB    *,       /* ptr to PSB structure                */
   struct show_buffer *,  /* ptr to show buffer                  */
   int32         *,       /* ptr to show buffer size             */
   int32         *,       /* ptr to large input length           */
   int32         *,       /* ptr to large output length          */
   char          *        /* reserved - must be NULL             */
);
 
PAMSIMPORT int32 PAMSAPI pams_get_msga (
   char          *,       /* ptr to char buffer                  */
   char          *,       /* ptr to priority                     */
   q_address     *,       /* ptr to source                       */
   short         *,       /* ptr to class                        */
   short         *,       /* ptr to type                         */
   short         *,       /* ptr to input length                 */
   short         *,       /* ptr to output length                */
   int32         *,       /* ptr to selection address            */
   struct PSB    *,       /* ptr to PSB structure                */
   struct show_buffer *,  /* ptr to show buffer                  */
   int32         *,       /* ptr to show buffer size             */
   int32         *,       /* ptr to large input length           */
   int32         *,       /* ptr to large output length          */
   void (*func) (int32*), /* ptr to actrtn                       */
   int32         *,       /* ptr to actparam                     */
   int32         *,       /* ptr to flag_id                      */
   char          *        /* reserved - must be NULL             */
);
 
PAMSIMPORT int32 PAMSAPI pams_cancel_get (
   int32         *        /* ptr to sel_filter                   */
); 

PAMSIMPORT int32 PAMSAPI pams_confirm_msg (
   uint32        *,       /* msg seq num                         */
   int32         *,       /* confirm status                      */
   char          *        /* post-conf journal param             */
);
 
PAMSIMPORT int32 PAMSAPI pams_show_msg (
   char          **,      /* ptr to ptr to char buffer           */
   struct show_buffer *   /* ptr to SHOWMSG structure            */
);
 
PAMSIMPORT int32 PAMSAPI pams_exit (void);
 
PAMSIMPORT int32 PAMSAPI pams_open_jrn ( char 	*,
   int16	*, int32	*);
 
PAMSIMPORT int32 PAMSAPI pams_read_jrn ( int32 	*, char *, char	 	*,
   int32 	*, int16 	*, int16	*, int16 *, int16 	*,
   int32 	*, uint32 	*, int32 	*, uint32 *, int32 	*,
   int32        *, int32        *, char         *);
 
PAMSIMPORT int32 PAMSAPI pams_close_jrn ( int32 	*);
 
PAMSIMPORT int32 PAMSAPI pams_print_sts (
   char          *,	  /* ptr to string process name          */
   char          *,       /* ptr to string message               */
   int	                  /* status return                       */
);
 
PAMSIMPORT int32 PAMSAPI putil_print_sts (
   char          *,       /* ptr to string process name          */
   char          *,       /* ptr to string message               */
   int	                  /* status return                       */
);
 
PAMSIMPORT int32 PAMSAPI putil_print_status (
   char          *,       /* ptr to string process name          */
   char          *,       /* ptr to string message               */
   int	                  /* status return                       */
);
 
PAMSIMPORT int32 PAMSAPI putil_show_pending (
   int32         *,       /* count - ptr to number of queues     */
   short         *,       /* ptr to array of queues              */
   int32         *        /* ptr to array of pending msg counts  */
);
 
PAMSIMPORT int32 PAMSAPI pams_set_timer (
   int32         *,       /* ptr to timer id                     */
   char          *,       /* ptr to timer format                 */
   int32         *,       /* ptr to timer tick count             */
   int32         *        /* ptr to int32 array                  */
);                                
 
PAMSIMPORT int32 PAMSAPI pams_cancel_timer (
   int32         *        /* ptr to timer id                     */
);
 
PAMSIMPORT int32 PAMSAPI pams_set_select (
   selection_array_component *, /* selection array               */
   short         *,       /* number of select masks in array     */
   int32         *        /* returned select-var for get         */
); 
 
PAMSIMPORT int32 PAMSAPI pams_cancel_select (
   int32         *        /* select var from set select          */
);
 
PAMSIMPORT int32 PAMSAPI pams_status_text (
   int32	 *,       /* error code                          */
   int32	 *,       /* returned severity of error          */
   char          *,       /* ptr to buffer for error message     */
   int32	 *,       /* length of buffer for return message */
   int32         *        /* length of message returned          */
);
 
PAMSIMPORT int32 PAMSAPI pams_create_handle (
    pams_handle,          /* a pams_handle                       */
    int32 *               /* the type of handle                  */
);
 
PAMSIMPORT int32 PAMSAPI pams_delete_handle (
    pams_handle           /* a pams_handle                       */
);
 
PAMSIMPORT int32 PAMSAPI pams_msg_length (
   pams_handle,           /* a pams_handle                       */
   uint32 *               /* length of message returned          */
);
 
PAMSIMPORT int32 PAMSAPI pams_insert_buffer (
    pams_handle,          /* message handle                      */
    char *,               /* pointer to buffer                   */
    uint32 *              /* length of buffer                    */
);
 
PAMSIMPORT int32 PAMSAPI pams_extract_buffer (
    pams_handle,          /* message handle                      */
    char        *,        /* pointer to a buffer                 */
    uint32      *,        /* length of the buffer                */
    uint32      *         /* length of xtracted message          */
);
 
PAMSIMPORT int32 PAMSAPI pams_encode_int8 (
    pams_handle,          /* message handle                      */
    int32 *,              /* tag                                 */
    int8 *                /* value                               */
);
 
PAMSIMPORT int32 PAMSAPI pams_encode_uint8 (
    pams_handle,          /* message handle                      */
    int32 *,              /* field tag                           */
    uint8 *               /* field value                         */
);
 
PAMSIMPORT int32 PAMSAPI pams_encode_int16 (
    pams_handle,          /* message handle                      */
    int32 *,              /* field tag                           */
    int16 *               /* field value                         */
);
 
PAMSIMPORT int32 PAMSAPI pams_encode_uint16 (
    pams_handle,          /* message handle                      */
    int32 *,              /* field tag                           */
    uint16 *              /* field value                         */
);
 
PAMSIMPORT int32 PAMSAPI pams_encode_int32 (
    pams_handle,          /* message handle                      */
    int32 *,              /* field tag                           */
    int32 *               /* field value                         */
);
 
PAMSIMPORT int32 PAMSAPI pams_encode_uint32 (
    pams_handle,          /* message handle                      */
    int32 *,              /* field tag                           */
    uint32  *             /* field value                         */
);
 
PAMSIMPORT int32 PAMSAPI pams_encode_qid (
    pams_handle,          /* message handle                      */
    int32 *,              /* field tag                           */
    q_address  *          /* field value                         */
);
 
PAMSIMPORT int32 PAMSAPI pams_encode_int64 (
    pams_handle,          /* message handle                      */
    int32 *,              /* field tag                           */
    int64 *               /* field value                         */
);
 
PAMSIMPORT int32 PAMSAPI pams_encode_uint64 (
    pams_handle,          /* message handle                      */
    int32 *,              /* field tag                           */
    uint64  *             /* field value                         */
);
 
PAMSIMPORT int32 PAMSAPI pams_encode_float (
    pams_handle,          /* message handle                      */
    int32 *,              /* field tag                           */
    float *               /* field value                         */
);
 
PAMSIMPORT int32 PAMSAPI pams_encode_double (
    pams_handle,          /* message handle                      */
    int32 *,              /* field tag                           */
    double *              /* field value                         */
);
 
PAMSIMPORT int32 PAMSAPI pams_encode_string (
    pams_handle,           /* message handle                     */
    int32 *,               /* field tag                          */
    char *,                /* field value                        */
    int32 *                /* length of string                   */
);
 
PAMSIMPORT int32 PAMSAPI pams_encode_array (
    pams_handle,           /* message handle                      */
    int32      *,          /* field tag                           */
    void       *,          /* pointer to array                    */
    int32      *           /* number of elments in array          */
);
 
PAMSIMPORT int32 PAMSAPI pams_remove_encoding (
    pams_handle,           /* message handle                      */
    int32 *,               /* field tag                           */
    int32 *                /* flags                               */
);
 
PAMSIMPORT int32 PAMSAPI pams_next_msg_field (
    pams_handle,           /* message handle                      */
    int32      *,          /* field tag                           */
    int32      *           /* length of field                     */
);
 
PAMSIMPORT int32 PAMSAPI pams_decode_int8 (
    pams_handle,            /* message handle                     */
    int32 *,                /* field tag                          */
    int8 *                  /* field value                        */
);
 
PAMSIMPORT int32 PAMSAPI pams_decode_uint8 (
    pams_handle,            /* message handle                     */
    int32 *,                /* field tag                          */
    uint8 *                 /* field value                        */
);
 
PAMSIMPORT int32 PAMSAPI pams_decode_int16 (
    pams_handle,            /* message handle                     */
    int32 *,                /* field tag                          */
    int16 *                 /* field value                        */
);
 
PAMSIMPORT int32 PAMSAPI pams_decode_uint16 (
    pams_handle,            /* message handle                     */
    int32 *,                /* field tag                          */
    uint16 *                /* field value                        */
);
 
PAMSIMPORT int32 PAMSAPI pams_decode_int32 (
    pams_handle,            /* message handle                     */
    int32 *,                /* field tag                          */
    int32  *                /* field value                        */
);
 
PAMSIMPORT int32 PAMSAPI pams_decode_uint32 (
    pams_handle,           /* message handle                      */
    int32 *,               /* field tag                           */
    uint32 *               /* field value                         */
);
 
PAMSIMPORT int32 PAMSAPI pams_decode_qid (
    pams_handle,          /* message handle                      */
    int32 *,              /* field tag                           */
    q_address  *          /* field value                         */
);
 
PAMSIMPORT int32 PAMSAPI pams_decode_int64 (
    pams_handle,           /* message handle                      */
    int32 *,               /* field tag                           */
    int64 *                /* field value                         */
);
 
PAMSIMPORT int32 PAMSAPI pams_decode_uint64 (
    pams_handle,           /* message handle                      */
    int32 *,               /* field tag                           */
    uint64 *               /* field value                         */
);
 
PAMSIMPORT int32 PAMSAPI pams_decode_float (
    pams_handle,           /* message handle                      */
    int32 *,               /* field tag                           */
    float *                /* field value                         */
);
 
PAMSIMPORT int32 PAMSAPI pams_decode_double (
    pams_handle,           /* message handle                      */
    int32 *,               /* field tag                           */
    double *               /* field value                         */
);
 
PAMSIMPORT int32 PAMSAPI pams_decode_string (
    pams_handle,           /* message handle                      */
    int32 *,               /* field tag                           */
    char *,                /* buffer                              */
    int32 *,               /* length of buffer                    */
    int32 *                /* length of string                    */
);
 
PAMSIMPORT int32 PAMSAPI pams_decode_array (
    pams_handle,           /* message handle                      */
    int32 *,               /* field tag                           */
    void *,                /* pointer to array                    */
    int32 *,               /* array size in elements              */
    int32 *                /* number of elements returned         */
);
 
PAMSIMPORT int32 PAMSAPI pams_set_msg_position (
    pams_handle,           /* message handle                      */
    int32      *,          /* field tag                           */
    int32      *           /* flags                               */
);
    
#ifdef __DECC
#pragma member_alignment __restore
#endif
#ifdef __cplusplus
}
#endif
#endif  /* if already included */
