/* 
 * Proview   $Id: rs_nmps_trans.c,v 1.8 2006-01-12 05:57:43 claes Exp $
 * Copyright (C) 2005 SSAB Oxelösund AB.
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

/*************************************************************************
*
* 	PROGRAM		rs_nmps_trans
*
*       Modifierad
*		960522	Claes Sjöfors	Skapad
*
*
*	Funktion:	Hanterar transar i NMps.
*
**************************************************************************/

/*_Include filer_________________________________________________________*/

#ifdef OS_VMS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <float.h>
#include <libdef.h>
#include <starlet.h>
#include <lib$routines.h>
#endif

#if defined OS_LYNX || defined OS_LINUX
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <float.h>
#endif

#ifdef OS_ELN
#include stdio
#include stdlib
#include string
#include math
#include float
#include libdef
#include starlet
#include lib$routines
#endif

#include "pwr.h"
#include "pwr_baseclasses.h"
#include "pwr_nmpsclasses.h"
#include "pwr_remoteclasses.h"
#include "co_cdh.h"
#include "co_time.h"
#include "rt_gdh.h"
#include "rt_errh.h"
#include "rt_qcom.h"
#include "rt_ini_event.h"
#include "rt_qcom_msg.h"
#include "rt_mh_msg.h"
#include "rt_mh_net.h"
#include "rt_mh_appl.h"
#include "rt_plc_utl.h"
#include "rt_gdh_msg.h"
#include "rt_hash_msg.h"
#include "nmps_cnv.h"
#include "rs_nmps_msg.h"

/*_Globala variabler______________________________________________________*/

/*_Local functions________________________________________________________*/
/* Global functions________________________________________________________*/

#define	NMPS_REQUESTFUNC_DISPLAYOBJECT	1
#define	NMPS_REQUESTFUNC_ACCEPT		2
#define	NMPS_REQUESTFUNC_CELLINSERT	4
#define	NMPS_REQUESTFUNC_CHANGE		8
#define	NMPS_REQUESTFUNC_ACCEPTBOTH	16
#define	NMPS_REQUESTFUNC_DISPLAYRESET	32
#define	NMPS_REQUESTFUNC_INTKEY		64
#define	NMPS_REQUESTFUNC_CHECKSUM	128

#define	NMPS_DATARCVFUNC_DISPLAYOBJECT	1
#define	NMPS_DATARCVFUNC_ACCEPT		2
#define	NMPS_DATARCVFUNC_CELLINSERT	4
#define	NMPS_DATARCVFUNC_ACK		8
#define	NMPS_DATARCVFUNC_ACCEPTBOTH	16
#define	NMPS_DATARCVFUNC_DISPLAYRESET	32
#define NMPS_DATARCVFUNC_HEADER		64
#define	NMPS_DATARCVFUNC_ACKOTHER	128

#define	NMPS_DATASENDFUNC_ACK		8
#define NMPS_DATASENDFUNC_HEADER	64
#define	NMPS_DATASENDFUNC_ACKOTHER	128

#define	NMPS_TRANSTYPE_LOC		0
#define	NMPS_TRANSTYPE_REM		1

#define ODD(a)	(((int)(a) & 1) != 0)
#define EVEN(a)	(((int)(a) & 1) == 0)
#define max(Dragon,Eagle) ((Dragon) > (Eagle) ? (Dragon) : (Eagle))
#define min(Dragon,Eagle) ((Dragon) < (Eagle) ? (Dragon) : (Eagle))
#ifndef __ALPHA
#define abs(Dragon) ((Dragon) >= 0 ? (Dragon) : (-(Dragon)))
#endif
#define	LogAndExit( status) \
{\
  errh_CErrLog(NMPS__BCKEXIT, errh_ErrArgMsg(status), NULL);\
  exit( status);\
}

#define	LogAndReturn( status1, status2) \
{\
  errh_CErrLog(status1, errh_ErrArgMsg(status2), NULL);\
  return status2;\
}

#define	Log( status1, status2) \
{\
  errh_CErrLog(status1, errh_ErrArgMsg(status2), NULL);\
}

typedef struct {
	char			Key[40];
	pwr_tStatus		Status;
	} nmpstrans_t_header;

typedef struct s_translist{
	char			Key[40];
	int			Userdata;
	pwr_tTime		InsertTime;
	float			TimeoutTime;
	struct s_translist	*next_ptr;
	struct s_translist	*prev_ptr;
	} translist_t_entry;

typedef struct {
	pwr_tObjid		objid;
	pwr_sClass_DataRequest	*req;
	gdh_tDlid		subid;
	pwr_tInt32		old_key;
	char			old_key_str[40];
	pwr_tInt32		function;
	void			*display_object_ptr;
	gdh_tDlid		display_object_subid;
	unsigned int		display_object_size;
	pwr_sClass_RemTrans	*send_remtrans_ptr;
	pwr_sClass_LocTrans	*send_loctrans_ptr;
	int			send_remtrans_type;
	gdh_tDlid		send_remtrans_subid;
	void			*send_remtransbuff_ptr;
	gdh_tDlid		send_remtransbuff_subid;
	pwr_sClass_RemTrans	*rcv_remtrans_ptr;
	pwr_sClass_LocTrans	*rcv_loctrans_ptr;
	int			rcv_remtrans_type;
	gdh_tDlid		rcv_remtrans_subid;
	void			*rcv_remtransbuff_ptr;
	gdh_tDlid		rcv_remtransbuff_subid;
	pwr_sClass_NMpsCell	*cell_ptr;
	gdh_tDlid		cell_subid;
	cnv_t_conv_table	*conv_table;
	int			conv_table_count;
	pwr_tOName     		parent_name;
	int			wait_for_accept;
	char			display_object_key[40];
	int			cell_full_msg_sent;
	int			fatal_error;
	} nmpstrans_t_req_list;

typedef struct {
	pwr_tObjid		objid;
	pwr_sClass_DataRcv	*rcv;
	gdh_tDlid		subid;
	pwr_tInt32		function;
	void			*display_object_ptr;
	gdh_tDlid		display_object_subid;
	unsigned int		display_object_size;
	pwr_sClass_RemTrans	*send_remtrans_ptr;
	pwr_sClass_LocTrans	*send_loctrans_ptr;
	int			send_remtrans_type;
	gdh_tDlid		send_remtrans_subid;
	void			*send_remtransbuff_ptr;
	gdh_tDlid		send_remtransbuff_subid;
	pwr_sClass_RemTrans	*rcv_remtrans_ptr;
	pwr_sClass_LocTrans	*rcv_loctrans_ptr;
	int			rcv_remtrans_type;
	gdh_tDlid		rcv_remtrans_subid;
	void			*rcv_remtransbuff_ptr;
	gdh_tDlid		rcv_remtransbuff_subid;
	pwr_sClass_NMpsCell	*cell_ptr;
	gdh_tDlid		cell_subid;
	cnv_t_conv_table	*conv_table;
	int			conv_table_count;
	pwr_tOName     		parent_name;
	int			wait_for_accept;
	char			display_object_key[40];
	int			cell_full_msg_sent;
	int			fatal_error;
	} nmpstrans_t_rcv_list;

typedef struct {
	pwr_tObjid		objid;
	pwr_sClass_DataSend	*snd;
	gdh_tDlid		subid;
	pwr_tInt32		function;
	pwr_sClass_RemTrans	*send_remtrans_ptr;
	pwr_sClass_LocTrans	*send_loctrans_ptr;
	int			send_remtrans_type;
	gdh_tDlid		send_remtrans_subid;
	void			*send_remtransbuff_ptr;
	gdh_tDlid		send_remtransbuff_subid;
	pwr_sClass_RemTrans	*rcv_remtrans_ptr;
	pwr_sClass_LocTrans	*rcv_loctrans_ptr;
	int			rcv_remtrans_type;
	gdh_tDlid		rcv_remtrans_subid;
	void			*rcv_remtransbuff_ptr;
	gdh_tDlid		rcv_remtransbuff_subid;
	cnv_t_conv_table	*conv_table;
	int			conv_table_count;
	int			rtdb_offset;
	int			fatal_error;
	} nmpstrans_t_snd_list;


typedef struct {
	int				RequestObjectCount;
	pwr_tUInt32			LoopCount;
	pwr_sClass_NMpsTransConfig	*transconfig;
	gdh_tDlid			transconfig_dlid;
	nmpstrans_t_req_list		*reqlist;
	int				req_count;
	nmpstrans_t_rcv_list		*rcvlist;
	int				rcv_count;
	nmpstrans_t_snd_list		*sndlist;
	int				snd_count;
	translist_t_entry		*req_translist;
	translist_t_entry		*rcv_translist;
	translist_t_entry		*snd_translist;
	} *trans_ctx;

static pwr_tStatus	translist_delete(
			translist_t_entry	**translist_list,
			translist_t_entry	*translist_ptr);
static pwr_tStatus translist_insert(
			translist_t_entry	**translist_list,
			char			*key,
			int			userdata,
			float			timeout_time,
			translist_t_entry	**translist_ptr);
static pwr_tStatus	translist_find(
			translist_t_entry	*translist_list,
			char			*key,
			translist_t_entry	**found_translist_ptr);
static int	translist_timeout( 	pwr_tTime 	*insert_time, 
					pwr_tTime	*current_time,
					float		timeout);
static pwr_tStatus	translist_timeout_check(
			trans_ctx		transctx,
			translist_t_entry	**translist_list,
			void			(* action_func) ());
static void	nmpstrans_request_timeout(	
					trans_ctx		tranctx,
					char			*key,
					int			userdata);
static void	nmpstrans_datasend_timeout(	
					trans_ctx		transctx,
					char			*key,
					int			userdata);
static int	nmpstrans_request_data( trans_ctx	transctx,
				char			*key,
				int			position,
				translist_t_entry	**translist_list,
				nmpstrans_t_req_list	*req_ptr);
static void	nmpstrans_reqlist_close( trans_ctx transctx);
static void	nmpstrans_rcvlist_close( trans_ctx transctx);
static void	nmpstrans_sndlist_close( trans_ctx transctx);
static pwr_tStatus	nmpstrans_reqlist_add(
			trans_ctx		transctx,
			pwr_tObjid 		objid,
			nmpstrans_t_req_list 	**reqlist,
			int			*reqlist_count);
static pwr_tStatus	nmpstrans_rcvlist_add(
			trans_ctx		transctx,
			pwr_tObjid 		objid,
			nmpstrans_t_rcv_list 	**rcvlist,
			int			*rcvlist_count);
static pwr_tStatus	nmpstrans_sndlist_add(
			trans_ctx		transctx,
			pwr_tObjid 		objid,
			nmpstrans_t_snd_list 	**sndlist,
			int			*sndlist_count);
static pwr_tStatus	nmps_get_transconfig( trans_ctx	transctx);
static pwr_tStatus	nmpstrans_datareq_init( trans_ctx	transctx);
static pwr_tStatus	nmpstrans_datarcv_init( trans_ctx	transctx);
static pwr_tStatus	nmpstrans_datasend_init( trans_ctx	transctx);
static int	nmpstrans_send_datasend( trans_ctx	transctx,
				int			position,
				translist_t_entry	**translist_list,
				nmpstrans_t_snd_list	*snd_ptr);
static pwr_tStatus	nmpstrans_trans_handler( trans_ctx	transctx);
static int	nmpstrans_connect_alarm();
static int	nmpstrans_alarm_send(
			char	*alarm_text,
			char	*alarm_name,
			int	alarm_prio);
static int	nmpstrans_req_receive_data( trans_ctx	transctx,
				translist_t_entry	**translist_list,
				nmpstrans_t_req_list	*req_ptr);
static int	nmpstrans_req_accept_detected( trans_ctx	transctx,
				nmpstrans_t_req_list	*req_ptr);
static int	nmpstrans_req_reset_detected( trans_ctx	transctx,
				nmpstrans_t_req_list	*req_ptr);
static int	ssabutil_lopnr_check( pwr_tInt32	lopnr);


int nmpstrans_check_checksum( int key)
{
  return ssabutil_lopnr_check( key);
}

/****************************************************************************
* Name:		nmpstrans_alarm_connect()
*
* Type		int
*
* Type		Parameter	IOGF	Description
*
* Description:
*	Connect to alarm handler.
*
**************************************************************************/
static int	nmpstrans_connect_alarm() 
{
	static int	alarm_connected = 0;
	int		sts ; 
	mh_eEvent	AbortEventType = mh_eEvent_Alarm;
	mh_eEventPrio   AbortEventPrio = mh_eEventPrio_A;
	pwr_tUInt32	NoOfActMessages ;

        if ( alarm_connected)
	  /* We are already connected */
	  return NMPS__SUCCESS;

	sts = mh_ApplConnect(
		pwr_cNObjid,
		0,
		"AbortEventName",
		AbortEventType,
		AbortEventPrio,
		mh_mEventFlags_Bell | mh_mEventFlags_Ack | mh_mEventFlags_Return ,
		"AbortEventText",
		&NoOfActMessages
		);
	if( EVEN(sts)) return sts;

	alarm_connected = 1;
	return NMPS__SUCCESS;    
}

/*************************************************************************
*
* Name:		nmpstrans_alarm_send()
*
* Type		int
*
* Type		Parameter	IOGF	Description
*
* Description:
*	Sends an alarm with the specified text.
*
**************************************************************************/

static int	nmpstrans_alarm_send(
			char	*alarm_text,
			char	*alarm_name,
			int	alarm_prio ) 
{
	mh_sApplMessage 	mh_msg ;
	pwr_tUInt32	  	mh_id ; 
	int			sts ;
  
	sts = nmpstrans_connect_alarm();
	if ( EVEN(sts)) return sts;	
	mh_msg.Object = pwr_cNObjid;
	mh_msg.EventFlags = mh_mEventFlags_Returned |
			 mh_mEventFlags_NoObject | 
			 mh_mEventFlags_Bell;
	clock_gettime( CLOCK_REALTIME, &mh_msg.EventTime);

	mh_msg.SupObject = pwr_cNObjid;
	mh_msg.Outunit = pwr_cNObjid;
	strcpy ( mh_msg.EventName , alarm_name);
	strcpy ( mh_msg.EventText , alarm_text);
	mh_msg.EventType = mh_eEvent_Alarm;
	mh_msg.SupInfo.SupType = mh_eSupType_None; 
	switch ( alarm_prio)
	{
	  case 'A':
	    mh_msg.EventPrio = mh_eEventPrio_A;
	    break ;
	  case 'B':
	    mh_msg.EventPrio = mh_eEventPrio_B;
	    break; 
	  case 'C':
	    mh_msg.EventPrio = mh_eEventPrio_C;
	    break; 
	  case 'D':
	    mh_msg.EventPrio = mh_eEventPrio_D;
	    break; 
	  default:
	    mh_msg.EventPrio = mh_eEventPrio_A;
	    break; 
	}
	sts = mh_ApplMessage ( &mh_id , &mh_msg ) ;
	if( EVEN(sts) ) return sts;
  
	return NMPS__SUCCESS;
} 


/****************************************************************************
* Name:		translist_delete()
*
* Type		pwr_tStatus
*
* Type		Parameter	IOGF	Description
*
* Description:
*		Delete an entry in the translist.
*
**************************************************************************/
static pwr_tStatus	translist_delete( 
			translist_t_entry	**translist_list, 
			translist_t_entry	*translist_ptr)
{
	if ( translist_ptr == *translist_list)
	  /* Change the root */
	  *translist_list = translist_ptr->next_ptr;

	if ( translist_ptr->prev_ptr )
	  translist_ptr->prev_ptr->next_ptr = translist_ptr->next_ptr;
	if ( translist_ptr->next_ptr )
	  translist_ptr->next_ptr->prev_ptr = translist_ptr->prev_ptr;

	free( translist_ptr);

	return NMPS__SUCCESS;
}


/****************************************************************************
* Name:		nmpsmir_translist_insert()
*
* Type		pwr_tStatus
*
* Type		Parameter	IOGF	Description
*
* Description:
*		Insert an entry in the translist.
*
**************************************************************************/
static pwr_tStatus translist_insert(  
			translist_t_entry	**translist_list,
			char			*key,
			int			userdata,
			float			timeout_time,
			translist_t_entry	**translist_ptr)
{
	translist_t_entry	*next_ptr;

	*translist_ptr = calloc( 1, sizeof( translist_t_entry));
	if ( *translist_ptr == 0) return NMPS__NOMEMORY;

	/* Insert first in list */
	next_ptr = *translist_list;
	if ( next_ptr != NULL)
	  next_ptr->prev_ptr = *translist_ptr;
	(*translist_ptr)->next_ptr = next_ptr;

	*translist_list = *translist_ptr;
	strncpy( (*translist_ptr)->Key, key,
			sizeof((*translist_ptr)->Key));
	(*translist_ptr)->Userdata = userdata;
	(*translist_ptr)->TimeoutTime = timeout_time;
	clock_gettime( CLOCK_REALTIME, &(*translist_ptr)->InsertTime);

	return NMPS__SUCCESS;
}


/****************************************************************************
* Name:		translist_find()
*
* Type		pwr_tStatus
*
* Type		Parameter	IOGF	Description
*
* Description:
*		Find an entry in the translist.
*
**************************************************************************/
static pwr_tStatus	translist_find(  
			translist_t_entry	*translist_list,
			char			*key,
			translist_t_entry	**found_translist_ptr)
{
	translist_t_entry	*translist_ptr;
	int			found;

	/* Loop through the list */
	found = 0;
	translist_ptr = translist_list;
	while ( translist_ptr != NULL)
	{
	  if ( !strcmp( translist_ptr->Key, key))
	  {
	    *found_translist_ptr = translist_ptr;
	    found = 1;
	    break;
	  }
	  translist_ptr = translist_ptr->next_ptr;
	}
	
	if ( !found)
	  return NMPS__ENTRYNOTFOUND;

	return NMPS__SUCCESS;
}


/****************************************************************************
* Name:		translist_timeout()
*
* Type		pwr_tStatus
*
* Type		Parameter	IOGF	Description
*
* Description:
*		Calculate if its time to remove an entry in translist.
*		If the time since insert is greater than timeout, it's time.
*
**************************************************************************/
static int	translist_timeout( 	pwr_tTime 	*insert_time, 
					pwr_tTime	*current_time,
					float		timeout)
{
/**************
	pwr_tTime	diff_time;
	float		time;

	lib$subx( insert_time, current_time, &diff_time);
	time = - diff_time.low/10000000.;
	return ( time > timeout);
**************/
	pwr_tDeltaTime	diff_time;

	time_Adiff( &diff_time, current_time, insert_time);
	if ( diff_time.tv_sec > timeout)
	  return 1;
	else
	  return 0;
}


/****************************************************************************
* Name:		translist_timeout_check()
*
* Type		pwr_tStatus
*
* Type		Parameter	IOGF	Description
*
* Description:
*		Find an entry in the translist.
*
**************************************************************************/
static pwr_tStatus	translist_timeout_check(
			trans_ctx		transctx,
			translist_t_entry	**translist_list,
			void			(* action_func) ())
{
	translist_t_entry	*translist_ptr;
	translist_t_entry	*deletetrans_ptr;
	int			found;
	pwr_tTime		current_time;

	clock_gettime( CLOCK_REALTIME, &current_time);

	/* Loop through the list */
	found = 0;
	translist_ptr = *translist_list;

	while ( translist_ptr != NULL)
	{
	  /* Check time since remove */
	  if ( translist_timeout( 
		&translist_ptr->InsertTime, &current_time, 
		translist_ptr->TimeoutTime))
	  {
	    if ( action_func != NULL)
	      (action_func) ( transctx, translist_ptr->Key, 
			translist_ptr->Userdata);
	    deletetrans_ptr = translist_ptr;
	    translist_ptr = translist_ptr->next_ptr;
	    translist_delete( translist_list, deletetrans_ptr);
	    continue;
	  }
	  translist_ptr = translist_ptr->next_ptr;
	}

	return NMPS__SUCCESS;
}


/*************************************************************************
*
* Name:		nmpstrans_request_timeout
*
* Typ		int
*
* Typ		Parameter	IOGF	Beskrivning
*
* Beskrivning: 
*	This function is called when a timeout is detected.
*
**************************************************************************/

static void	nmpstrans_request_timeout(	
					trans_ctx		transctx,
					char			*key,
					int			userdata)
{
	nmpstrans_t_req_list	*req_ptr;
	char			alarm_text[80];
	char			alarm_name[80];
	int			sts;

	req_ptr = transctx->reqlist + userdata;

	/* Send an alarm, alarmtext idx 0, timeout */
	if ( req_ptr->req->AlarmText[0][0] != 0)
	{
	  sprintf( alarm_text, "%s %s", req_ptr->req->AlarmText[0], key);
	  strcpy( alarm_name, "NMpsTrans");
	  sts = nmpstrans_alarm_send( alarm_text, 
		alarm_name, 'B');

	}

	/* Set error flag in request object */
	req_ptr->req->ErrorDetected = 1;
}


/*************************************************************************
*
* Name:		nmpstrans_datasend_timeout
*
* Typ		int
*
* Typ		Parameter	IOGF	Beskrivning
*
* Beskrivning: 
*	This function is called when a timeout is detected.
*
**************************************************************************/

static void	nmpstrans_datasend_timeout(	
					trans_ctx		transctx,
					char			*key,
					int			userdata)
{
	nmpstrans_t_snd_list	*snd_ptr;
	char			alarm_text[80];
	char			alarm_name[80];
	int			sts;

	snd_ptr = transctx->sndlist + userdata;

	/* Send an alarm, alarmtext idx 0, timeout */
	if ( snd_ptr->snd->AlarmText[0][0] != 0)
	{
	  sprintf( alarm_text, "%s %s", snd_ptr->snd->AlarmText[0], key);
	  strcpy( alarm_name, "NMpsTrans");
	  sts = nmpstrans_alarm_send( alarm_text, 
		alarm_name, 'B');

	}

	/* Set error flag in datasend object */
	snd_ptr->snd->ErrorDetected = 1;
}


/*************************************************************************
*
* Name:		nmpstrans_request_data
*
* Typ		int
*
* Typ		Parameter	IOGF	Beskrivning
*
* Beskrivning: 
*	Send a request.
*
**************************************************************************/

static int	nmpstrans_request_data( trans_ctx	transctx,
				char			*key,
				int			position,
				translist_t_entry	**translist_list,
				nmpstrans_t_req_list	*req_ptr)
{
	translist_t_entry	*translist_ptr;
	pwr_tStatus		sts;

	if ( req_ptr->send_remtrans_type == NMPS_TRANSTYPE_REM)
	{
	  if ( req_ptr->send_remtrans_ptr->DataValid == 1)
	  {
	    /* Remtranscell is busy */
	    return NMPS__REMTRANSBUSY;
	  }
	}
	else
	{
	  if ( req_ptr->send_loctrans_ptr->DataValid == 1)
	  {
	    /* Remtranscell is busy */
	    return NMPS__REMTRANSBUSY;
	  }
	}

	/* Fill the RemTrans buffer with data */
	strcpy( req_ptr->send_remtransbuff_ptr, key);

	if ( req_ptr->send_remtrans_type == NMPS_TRANSTYPE_REM)
	  req_ptr->send_remtrans_ptr->DataValid = 1;
	else
	  req_ptr->send_loctrans_ptr->DataValid = 1;
/*...	transctx->transconfig->RequestSent++;*/

	/* Insert trans in translist */
	sts = translist_insert( translist_list,
			key, position, req_ptr->req->TimeoutTime,
			&translist_ptr);

	return NMPS__SUCCESS;
}


/*************************************************************************
*
* Name:		nmpstrans_rcv_send_ack
*
* Typ		int
*
* Typ		Parameter	IOGF	Beskrivning
*
* Beskrivning: 
*	Send a request.
*
**************************************************************************/

static int	nmpstrans_rcv_send_ack( trans_ctx	transctx,
				char			*key,
				nmpstrans_t_rcv_list	*rcv_ptr)
{

	if ( rcv_ptr->send_remtrans_type == NMPS_TRANSTYPE_REM)
	{
	  if ( rcv_ptr->send_remtrans_ptr->DataValid == 1)
	  {
	    /* Remtranscell is busy */
	    return NMPS__REMTRANSBUSY;
	  }
	}
	else
	{
	  if ( rcv_ptr->send_loctrans_ptr->DataValid == 1)
	  {
	    /* Remtranscell is busy */
	    return NMPS__REMTRANSBUSY;
	  }
	}

	/* Fill the RemTrans buffer with data */
	strcpy( rcv_ptr->send_remtransbuff_ptr, key);

	if ( rcv_ptr->send_remtrans_type == NMPS_TRANSTYPE_REM)
	  rcv_ptr->send_remtrans_ptr->DataValid = 1;
	else
	  rcv_ptr->send_loctrans_ptr->DataValid = 1;
/*...	transctx->transconfig->DataRcvAckSent++;*/

	return NMPS__SUCCESS;
}


/*************************************************************************
*
* Name:		nmpstrans_send_datasend
*
* Typ		int
*
* Typ		Parameter	IOGF	Beskrivning
*
* Beskrivning: 
*	Send from a DataSend object.
*
**************************************************************************/

static int	nmpstrans_send_datasend( trans_ctx	transctx,
				int			position,
				translist_t_entry	**translist_list,
				nmpstrans_t_snd_list	*snd_ptr)
{
	translist_t_entry	*translist_ptr;
	pwr_tStatus		sts;
	char			*data_ptr;
	char			key[40];
	pwr_tObjid		*objid_ptr;
	char			*s;
	pwr_tOName     		name;

	if ( snd_ptr->send_remtrans_type == NMPS_TRANSTYPE_REM)
	{
	  if ( snd_ptr->send_remtrans_ptr->DataValid == 1)
	  {
	    /* Remtranscell is busy */
	    return NMPS__REMTRANSBUSY;
	  }
	}
	else
	{
	  if ( snd_ptr->send_loctrans_ptr->DataValid == 1)
	  {
	    /* Remtranscell is busy */
	    return NMPS__REMTRANSBUSY;
	  }
	}


	data_ptr = (char *)(snd_ptr->snd->DataInP);
	data_ptr += snd_ptr->rtdb_offset;

	objid_ptr = (pwr_tObjid *)(data_ptr + sizeof(data_ptr));

	data_ptr = * (char **) data_ptr;
	if ( data_ptr == 0)
	  return NMPS__SNDNODATA;

	/* Get the key from the name */
	sts = gdh_ObjidToName ( *objid_ptr, name, sizeof(name), 
		cdh_mNName);
	if ( EVEN(sts)) return NMPS__SNDNODATA;

	s = strrchr( name, '-');
	if ( !s)
	  s = name;
	else
	  s++;

	/* Translate to integer */
	strncpy( s, key, sizeof( key));

	data_ptr += snd_ptr->rtdb_offset;
	
	if ( snd_ptr->function & NMPS_DATASENDFUNC_HEADER)
	{
	  sts = cnv_ConvertData(
	  		snd_ptr->conv_table,
			snd_ptr->conv_table_count,
			(char *) data_ptr,
			(char *) snd_ptr->send_remtransbuff_ptr + 
				sizeof(nmpstrans_t_header));
	  strcpy( ((nmpstrans_t_header *) snd_ptr->send_remtransbuff_ptr)->Key, 
			key);
	  ((nmpstrans_t_header *) snd_ptr->send_remtransbuff_ptr)->Status = 1;
	}
	else
	{
	  /* Convert data and fill the RemTrans buffer */
	  sts = cnv_ConvertData(
	  		snd_ptr->conv_table,
			snd_ptr->conv_table_count,
			(char *) data_ptr,
			(char *) snd_ptr->send_remtransbuff_ptr);
	}

	if ( snd_ptr->send_remtrans_type == NMPS_TRANSTYPE_REM)
	  snd_ptr->send_remtrans_ptr->DataValid = 1;
	else
	  snd_ptr->send_loctrans_ptr->DataValid = 1;

/*...	transctx->transconfig->DataSendCount++;*/

	if ( snd_ptr->function & NMPS_DATASENDFUNC_ACK ||
	     snd_ptr->function & NMPS_DATASENDFUNC_ACKOTHER )
	{
	  /* Insert trans in translist */
	  sts = translist_insert( translist_list,
			key, position, snd_ptr->snd->TimeoutTime,
			&translist_ptr);
	}

	if ( !(snd_ptr->function & NMPS_DATASENDFUNC_ACK))
	  snd_ptr->snd->DataSentDetected = 1;

	return NMPS__SUCCESS;
}

static int	nmpstrans_req_create_data( 	char		*key,
					nmpstrans_t_req_list	*req_ptr,
					pwr_tObjid		*objid,
					void			*objectp)
{
	char		objname[80];
	int		sts;

	/* Use the key to create an object name */
	strcpy( objname, req_ptr->parent_name);
	if ( objname[strlen(objname)-1] != ':')
	  strcat( objname, "-");
	strcat( objname, key);
	  
	sts = gdh_CreateObject( objname, 
		cdh_ClassObjidToId( req_ptr->req->DataClass), 0, objid, pwr_cNObjid, 0, pwr_cNObjid);
	if ( EVEN(sts)) return sts;

	sts = gdh_ObjidToPointer( *objid, objectp);
	if ( EVEN(sts)) return sts;

	return NMPS__SUCCESS;
}

static int	nmpstrans_rcv_create_data( 	char		*key,
					nmpstrans_t_rcv_list	*rcv_ptr,
					pwr_tObjid		*objid,
					void			*objectp)
{
	char		objname[80];
	int		sts;

	/* Use the key to create an object name */
	strcpy( objname, rcv_ptr->parent_name);
	if ( objname[strlen(objname)-1] != ':')
	  strcat( objname, "-");
	strcat( objname, key);
	  
	sts = gdh_CreateObject( objname, 
		cdh_ClassObjidToId( rcv_ptr->rcv->DataClass), 0, objid, pwr_cNObjid, 0, pwr_cNObjid);
	if ( EVEN(sts)) return sts;

	sts = gdh_ObjidToPointer( *objid, objectp);
	if ( EVEN(sts)) return sts;

	return NMPS__SUCCESS;
}

static int	nmpstrans_req_receive_data( trans_ctx	transctx,
				translist_t_entry	**translist_list,
				nmpstrans_t_req_list	*req_ptr)
{
	void		*objectp;
	pwr_tStatus	sts, sts2;
	translist_t_entry	*translist_ptr;
	char		key[40];
	void		*msg;
	pwr_tObjid	objid;
	char		alarm_text[80];
	char		alarm_name[80];
	int		data_valid;
	int		i;
	int		return_status_ok;

	
	if ( req_ptr->rcv_remtrans_type == NMPS_TRANSTYPE_REM)
	  data_valid = req_ptr->rcv_remtrans_ptr->DataValid;
	else
	  data_valid = req_ptr->rcv_loctrans_ptr->DataValid;

	if ( data_valid)
	{
	  if ( req_ptr->function & NMPS_REQUESTFUNC_CELLINSERT &&
	     !(req_ptr->function & NMPS_REQUESTFUNC_DISPLAYOBJECT))
	  {
	    /* Check that cell is not full */
	    if ( req_ptr->cell_ptr->CellFull)
	      return NMPS__TRANSCELLFULL;

	    if ( req_ptr->cell_ptr->ExternFlag)
	      /* Cell is busy, wait until next scan */
	      return NMPS__TRANSCELLBUSY;
	  }

	  msg = req_ptr->rcv_remtransbuff_ptr;

	  strncpy( key, ((nmpstrans_t_header *) msg)->Key, sizeof(key));
	  sts = ((nmpstrans_t_header *) msg)->Status;
	  if ( EVEN(sts))
	  {
	    req_ptr->req->ErrorDetected = 1;

	    /* Error message received */
	    return_status_ok = 0;
	    for ( i = 0; i < 10; i++)
	    {
	      if ( req_ptr->req->ReturnStatus[i] != 0 &&
	           sts == req_ptr->req->ReturnStatus[i])
	      {
	        sprintf( alarm_text, "%s %s", req_ptr->req->ReturnStatusText[i], key);
	        strcpy( alarm_name, "NMpsTrans");
	        sts2 = nmpstrans_alarm_send( alarm_text, 
				alarm_name, 'B');
	        break;
              }
            }
	  }
	  else
	    return_status_ok = 1;

	  sts = translist_find( *translist_list, key, &translist_ptr);
	  if ( EVEN(sts))
	  {
	    if ( req_ptr->rcv_remtrans_type == NMPS_TRANSTYPE_REM)
	      req_ptr->rcv_remtrans_ptr->DataValid = 0;
	    else
	      req_ptr->rcv_loctrans_ptr->DataValid = 0;
	    errh_CErrLog(NMPS__TRANSUNEXPECT, NULL);
	    return NMPS__TRANSUNEXPECT;
	  }

	  if ( req_ptr->function & NMPS_REQUESTFUNC_DISPLAYOBJECT)
	  {
	    if ( return_status_ok)
	    {
	      /* Copy data to display object */
	      sts = cnv_ConvertData(
	  		req_ptr->conv_table,
			req_ptr->conv_table_count,
			(char *) msg + sizeof( nmpstrans_t_header),
			(char *) req_ptr->display_object_ptr);
	      if ( req_ptr->function & NMPS_REQUESTFUNC_ACCEPT)
	      {
	        req_ptr->wait_for_accept = 1;
	        strcpy( req_ptr->display_object_key, key);
	      }
	      req_ptr->req->DataRcvDetected = 1;
	    }
            else
	      memset( req_ptr->display_object_ptr, 0,
			req_ptr->display_object_size);
	  }
	  else
	  {
	    if ( return_status_ok)
	    {
	      /* Create an object and copy data into the object */
	      sts = nmpstrans_req_create_data( key, req_ptr, &objid, &objectp);
	      if ( sts == GDH__DUPLNAME)
	      {
		/* Object already exists */
		/* Send an alarm, alarmtext idx 4, object already exists */
		if ( req_ptr->req->AlarmText[4][0] != 0)
	        {
		  sprintf( alarm_text, "%s %s", req_ptr->req->AlarmText[4], key);
		  strcpy( alarm_name, "NMpsTrans");
		  sts2 = nmpstrans_alarm_send( alarm_text, 
				alarm_name, 'B');
	        }
	        req_ptr->req->ErrorDetected = 1;

		if ( req_ptr->rcv_remtrans_type == NMPS_TRANSTYPE_REM)
		  req_ptr->rcv_remtrans_ptr->DataValid = 0;
		else
		  req_ptr->rcv_loctrans_ptr->DataValid = 0;
/*...	        transctx->transconfig->RequestReceived++;*/
	        LogAndReturn(NMPS__TRANSCREAOBJ, sts);
	      }
	      else if ( EVEN(sts))
	      {
		if ( req_ptr->rcv_remtrans_type == NMPS_TRANSTYPE_REM)
		  req_ptr->rcv_remtrans_ptr->DataValid = 0;
		else
		  req_ptr->rcv_loctrans_ptr->DataValid = 0;
/*...	        transctx->transconfig->RequestReceived++;*/
	        req_ptr->fatal_error = 1;
	        LogAndReturn(NMPS__TRANSCREAOBJ, sts);
	      }
	      /* Copy data with cnv */
	      sts = cnv_ConvertData(
	  		req_ptr->conv_table,
			req_ptr->conv_table_count,
			(char *) msg + sizeof( nmpstrans_t_header),
			(char *) objectp);

	      /* Put the data object into the cell */
	      if ( req_ptr->function & NMPS_REQUESTFUNC_CELLINSERT)
	      {
		/* Check that the cell is not busy... */
		req_ptr->cell_ptr->ExternObjId = objid;
		req_ptr->cell_ptr->ExternOpType = 0;
		req_ptr->cell_ptr->ExternFlag = 1;
	      }
	      req_ptr->req->DataRcvDetected = 1;
	    }
	  }
	  translist_delete( translist_list, translist_ptr);
	  if ( req_ptr->rcv_remtrans_type == NMPS_TRANSTYPE_REM)
	    req_ptr->rcv_remtrans_ptr->DataValid = 0;
	  else
	    req_ptr->rcv_loctrans_ptr->DataValid = 0;
/*...	  transctx->transconfig->RequestReceived++; */
	}

	return NMPS__SUCCESS;
}


static int	nmpstrans_rcv_receive_data( trans_ctx	transctx,
				translist_t_entry	**translist_list,
				nmpstrans_t_rcv_list	*rcv_ptr)
{
	void		*objectp;
	pwr_tStatus	sts, sts2;
	translist_t_entry	*translist_ptr;
	char		key[40];
	void		*msg;
	pwr_tObjid	objid;
	char		alarm_text[80];
	char		alarm_name[80];
	int		data_valid;

	if ( rcv_ptr->rcv_remtrans_type == NMPS_TRANSTYPE_REM)
	  data_valid = rcv_ptr->rcv_remtrans_ptr->DataValid;
	else
	  data_valid = rcv_ptr->rcv_loctrans_ptr->DataValid;

	if ( data_valid)
	{
	  if ( rcv_ptr->function & NMPS_DATARCVFUNC_CELLINSERT &&
	     !(rcv_ptr->function & NMPS_DATARCVFUNC_DISPLAYOBJECT))
	  {
	    /* Check that cell is not full */
	    if ( rcv_ptr->cell_ptr->CellFull)
	      return NMPS__TRANSCELLFULL;

	    if ( rcv_ptr->cell_ptr->ExternFlag)
	      /* Cell is busy, wait until next scan */
	      return NMPS__TRANSCELLBUSY;
	  }

	  msg = rcv_ptr->rcv_remtransbuff_ptr;

	  /* The first word of the message is the key */
	  if ( rcv_ptr->function & NMPS_DATARCVFUNC_HEADER)
	  {
	    strncpy( key, 
		((nmpstrans_t_header *) msg)->Key, 
		sizeof(key));
	    sts = ((nmpstrans_t_header *) msg)->Status;
	  }

	  if ( rcv_ptr->function & NMPS_DATARCVFUNC_DISPLAYOBJECT)
	  {
	    /* Copy data to display object */
	    if ( rcv_ptr->function & NMPS_DATARCVFUNC_HEADER)
	      sts = cnv_ConvertData(
	  		rcv_ptr->conv_table,
			rcv_ptr->conv_table_count,
			(char *) msg + sizeof(nmpstrans_t_header),
			(char *) rcv_ptr->display_object_ptr);
	    else
	      sts = cnv_ConvertData(
	  		rcv_ptr->conv_table,
			rcv_ptr->conv_table_count,
			(char *) msg,
			(char *) rcv_ptr->display_object_ptr);

	    if ( rcv_ptr->function & NMPS_DATARCVFUNC_ACCEPT)
	    {
	      rcv_ptr->wait_for_accept = 1;
	      strcpy( rcv_ptr->display_object_key, key);
	    }
	    rcv_ptr->rcv->DataRcvDetected = 1;
	  }
	  else
	  {
	    /* Create an object and copy data into the object */
	    sts = nmpstrans_rcv_create_data( key, rcv_ptr, &objid, &objectp);
	    if ( sts == GDH__DUPLNAME)
	    {
	      /* Object already exists */
	      /* Send an alarm, alarmtext idx 4, object already exists */
	      if ( rcv_ptr->rcv->AlarmText[4][0] != 0)
	      {
	        sprintf( alarm_text, "%s %s", rcv_ptr->rcv->AlarmText[4], key);
	        strcpy( alarm_name, "NMpsTrans");
	        sts2 = nmpstrans_alarm_send( alarm_text, 
				alarm_name, 'B');
	      }
	      rcv_ptr->rcv->ErrorDetected = 1;

	      if ( rcv_ptr->rcv_remtrans_type == NMPS_TRANSTYPE_REM)
	        rcv_ptr->rcv_remtrans_ptr->DataValid = 0;
	      else
	        rcv_ptr->rcv_loctrans_ptr->DataValid = 0;
/*...	      transctx->transconfig->DataRcvReceived++;*/
	      LogAndReturn(NMPS__TRANSCREAOBJ, sts);
	    }
	    else if ( EVEN(sts))
	    {
	      if ( rcv_ptr->rcv_remtrans_type == NMPS_TRANSTYPE_REM)
	        rcv_ptr->rcv_remtrans_ptr->DataValid = 0;
	      else
	        rcv_ptr->rcv_loctrans_ptr->DataValid = 0;
/*...	      transctx->transconfig->DataRcvReceived++;*/
	      rcv_ptr->fatal_error = 1;
	      LogAndReturn(NMPS__TRANSCREAOBJ, sts);
	    }
	    /* Copy data with cnv */
	    if ( rcv_ptr->function & NMPS_DATARCVFUNC_HEADER)
	      sts = cnv_ConvertData(
	  		rcv_ptr->conv_table,
			rcv_ptr->conv_table_count,
			(char *) msg + sizeof(nmpstrans_t_header),
			(char *) objectp);
	    else
	      sts = cnv_ConvertData(
	  		rcv_ptr->conv_table,
			rcv_ptr->conv_table_count,
			(char *) msg,
			(char *) objectp);

	    /* Put the data object into the cell */
	    if ( rcv_ptr->function & NMPS_DATARCVFUNC_CELLINSERT)
	    {
	      /* Check that the cell is not busy... */
	      rcv_ptr->cell_ptr->ExternObjId = objid;
	      rcv_ptr->cell_ptr->ExternOpType = 0;
	      rcv_ptr->cell_ptr->ExternFlag = 1;
	    }
	    rcv_ptr->rcv->DataRcvDetected = 1;
	  }

	  if ( rcv_ptr->rcv_remtrans_type == NMPS_TRANSTYPE_REM)
	    rcv_ptr->rcv_remtrans_ptr->DataValid = 0;
	  else
	    rcv_ptr->rcv_loctrans_ptr->DataValid = 0;
/*...	  transctx->transconfig->DataRcvReceived++; */

	  /* Send an acknowledgement */
	  if (rcv_ptr->function & NMPS_DATARCVFUNC_ACK)
	  {
	    sts = nmpstrans_rcv_send_ack( transctx, key, rcv_ptr);
	    if ( sts == NMPS__REMTRANSBUSY)
	    {
	      /* Wait till next lap */
	    }
	  }
	  if (rcv_ptr->function & NMPS_DATARCVFUNC_ACKOTHER)
	  {
	    /* This is also an aknowlegement from a DataSend */
	    /* Search for a stored trans in the send translist */
	    sts = translist_find( transctx->snd_translist, key, &translist_ptr);
	    if ( EVEN(sts))
	    {
	      if ( rcv_ptr->rcv_remtrans_type == NMPS_TRANSTYPE_REM)
	        rcv_ptr->rcv_remtrans_ptr->DataValid = 0;
	      else
	        rcv_ptr->rcv_loctrans_ptr->DataValid = 0;
	      errh_CErrLog(NMPS__TRANSUNEXPECT, NULL);
	    }
	    translist_delete( &transctx->snd_translist, translist_ptr);
	  }
	}

	return NMPS__SUCCESS;
}

static int	nmpstrans_datasend_ack( trans_ctx	transctx,
				translist_t_entry	**translist_list,
				nmpstrans_t_snd_list	*snd_ptr)
{
	pwr_tStatus	sts;
	translist_t_entry	*translist_ptr;
	char			key[40];
	nmpstrans_t_header	*msg;
	int		data_valid;

	if ( !(snd_ptr->function & NMPS_DATASENDFUNC_ACK))
	  return NMPS__SUCCESS;


	if ( snd_ptr->rcv_remtrans_type == NMPS_TRANSTYPE_REM)
	  data_valid = snd_ptr->rcv_remtrans_ptr->DataValid;
	else
	  data_valid = snd_ptr->rcv_loctrans_ptr->DataValid;

	if ( data_valid)
	{
	  msg = snd_ptr->rcv_remtransbuff_ptr;

	  strncpy( key, msg->Key, sizeof( key));
	  sts = translist_find( *translist_list, key, &translist_ptr);
	  if ( EVEN(sts))
	  {
	    if ( snd_ptr->rcv_remtrans_type == NMPS_TRANSTYPE_REM)
	      snd_ptr->rcv_remtrans_ptr->DataValid = 0;
	    else
	      snd_ptr->rcv_loctrans_ptr->DataValid = 0;
	    errh_CErrLog(NMPS__TRANSUNEXPECT, NULL);
	    return NMPS__TRANSUNEXPECT;
	  }

	  snd_ptr->snd->DataSentDetected = 1;

	  translist_delete( translist_list, translist_ptr);
	  if ( snd_ptr->rcv_remtrans_type == NMPS_TRANSTYPE_REM)
	    snd_ptr->rcv_remtrans_ptr->DataValid = 0;
	  else
	    snd_ptr->rcv_loctrans_ptr->DataValid = 0;

	  sts = msg->Status;
	  if ( EVEN(sts))
	  {
	    /* Error status received ... */
	  }
/*...	  transctx->transconfig->DataSendAcknowledged++; */
	}

	return NMPS__SUCCESS;
}


static int	nmpstrans_req_accept_detected( trans_ctx	transctx,
				nmpstrans_t_req_list	*req_ptr)
{
	void		*objectp;
	pwr_tStatus	sts, sts2;
	pwr_tObjid	objid;
	char		alarm_text[80];
	char		alarm_name[80];

	/* Are we waiting for an accept */
	if ( !req_ptr->wait_for_accept)
	  return NMPS__NOWAITACCEPT;

	if ( req_ptr->function & NMPS_REQUESTFUNC_CELLINSERT)
	{
	  /* Check that cell is not full */
	  if ( req_ptr->cell_ptr->CellFull)
	  {
	    if ( !req_ptr->cell_full_msg_sent)
	    {
	      req_ptr->cell_full_msg_sent = 1;

	      /* Send an alarm, alarmtext idx 1, cell full */
	      if ( req_ptr->req->AlarmText[1][0] != 0)
	      {
	        strcpy( alarm_text, req_ptr->req->AlarmText[1]);
	        strcpy( alarm_name, "NMpsTrans");
	        sts = nmpstrans_alarm_send( alarm_text, 
				alarm_name, 'B');
	      }
	      req_ptr->req->ErrorDetected = 1;
	    }
	    return NMPS__TRANSCELLFULL;
	  }
	  else
	    req_ptr->cell_full_msg_sent = 0;

	  if ( req_ptr->cell_ptr->ExternFlag)
	  {
	    /* Cell is busy, wait until next scan */
	    return NMPS__TRANSCELLBUSY;
	  }
	}

	/* Create an object and copy data into the object */
	sts = nmpstrans_req_create_data( req_ptr->display_object_key, req_ptr,
		&objid, &objectp);
	if ( sts == GDH__DUPLNAME)
	{
	  /* Object already exists */
	  /* Send an alarm, alarmtext idx 4, object already exists */
	  if ( req_ptr->req->AlarmText[4][0] != 0)
	  {
	    sprintf( alarm_text, "%s %s", req_ptr->req->AlarmText[4], 
		req_ptr->display_object_key);
	    strcpy( alarm_name, "NMpsTrans");
	    sts2 = nmpstrans_alarm_send( alarm_text, 
			alarm_name, 'B');
	  }
	  req_ptr->req->ErrorDetected = 1;

	  if ( req_ptr->rcv_remtrans_type == NMPS_TRANSTYPE_REM)
	    req_ptr->rcv_remtrans_ptr->DataValid = 0;
	  else
	    req_ptr->rcv_loctrans_ptr->DataValid = 0;
	  return NMPS__TRANSCREAOBJ;
	}
	if ( EVEN(sts))
	{
	  if ( req_ptr->rcv_remtrans_type == NMPS_TRANSTYPE_REM)
	    req_ptr->rcv_remtrans_ptr->DataValid = 0;
	  else
	    req_ptr->rcv_loctrans_ptr->DataValid = 0;
	  req_ptr->fatal_error = 1;
	  LogAndReturn(NMPS__TRANSCREAOBJ, sts);
	}

	/* Copy data from display object */
	if ( req_ptr->function & NMPS_REQUESTFUNC_DISPLAYOBJECT)
	  memcpy( objectp, req_ptr->display_object_ptr, 
			req_ptr->display_object_size);

	/* Put the data object into the cell */
	if ( req_ptr->function & NMPS_REQUESTFUNC_CELLINSERT )
	{
	  /* Check that the cell is not busy... */
	  req_ptr->cell_ptr->ExternObjId = objid;
	  req_ptr->cell_ptr->ExternOpType = 0;
	  req_ptr->cell_ptr->ExternFlag = 1;
	}
	if ( req_ptr->function & NMPS_REQUESTFUNC_DISPLAYRESET &&
	     req_ptr->function & NMPS_REQUESTFUNC_DISPLAYOBJECT)
	  memset( req_ptr->display_object_ptr, 0, req_ptr->display_object_size);
	req_ptr->wait_for_accept = 0;
	if ( req_ptr->function & NMPS_REQUESTFUNC_INTKEY)
	  req_ptr->req->Key = 0;
	else
	  memset( req_ptr->req->KeyStr, 0, sizeof(req_ptr->req->KeyStr));

	return NMPS__SUCCESS;
}

static int	nmpstrans_rcv_accept_detected( trans_ctx	transctx,
				nmpstrans_t_rcv_list	*rcv_ptr)
{
	void		*objectp;
	pwr_tStatus	sts, sts2;
	pwr_tObjid	objid;
	char		alarm_text[80];
	char		alarm_name[80];

	/* Are we waiting for an accept */
	if ( !rcv_ptr->wait_for_accept)
	  return NMPS__NOWAITACCEPT;

	if ( rcv_ptr->function & NMPS_DATARCVFUNC_CELLINSERT)
	{
	  /* Check that cell is not full */
	  if ( rcv_ptr->cell_ptr->CellFull)
	  {
	    if ( !rcv_ptr->cell_full_msg_sent)
	    {
	      rcv_ptr->cell_full_msg_sent = 1;

	      /* Send an alarm, alarmtext idx 1, cell full */
	      if ( rcv_ptr->rcv->AlarmText[1][0] != 0)
	      {
	        strcpy( alarm_text, rcv_ptr->rcv->AlarmText[1]);
	        strcpy( alarm_name, "NMpsTrans");
	        sts = nmpstrans_alarm_send( alarm_text, 
				alarm_name, 'B');
	      }
	      rcv_ptr->rcv->ErrorDetected = 1;
	    }
	    return NMPS__TRANSCELLFULL;
	  }
	  else
	    rcv_ptr->cell_full_msg_sent = 0;

	  if ( rcv_ptr->cell_ptr->ExternFlag)
	  {
	    /* Cell is busy, wait until next scan */
	    return NMPS__TRANSCELLBUSY;
	  }
	}

	/* Create an object and copy data into the object */
	sts = nmpstrans_rcv_create_data( rcv_ptr->display_object_key, rcv_ptr,
		&objid, &objectp);
	if ( sts == GDH__DUPLNAME)
	{
	  /* Object already exists */
	  /* Send an alarm, alarmtext idx 4, object already exists */
	  if ( rcv_ptr->rcv->AlarmText[4][0] != 0)
	  {
	    sprintf( alarm_text, "%s %s", rcv_ptr->rcv->AlarmText[4], 
		rcv_ptr->display_object_key);
	    strcpy( alarm_name, "NMpsTrans");
	    sts2 = nmpstrans_alarm_send( alarm_text, 
			alarm_name, 'B');
	  }
	  rcv_ptr->rcv->ErrorDetected = 1;

	  if ( rcv_ptr->rcv_remtrans_type == NMPS_TRANSTYPE_REM)
	    rcv_ptr->rcv_remtrans_ptr->DataValid = 0;
	  else
	    rcv_ptr->rcv_loctrans_ptr->DataValid = 0;
	  return NMPS__TRANSCREAOBJ;
	}
	if ( EVEN(sts))
	{
	  if ( rcv_ptr->rcv_remtrans_type == NMPS_TRANSTYPE_REM)
	    rcv_ptr->rcv_remtrans_ptr->DataValid = 0;
	  else
	    rcv_ptr->rcv_loctrans_ptr->DataValid = 0;
	  rcv_ptr->fatal_error = 1;
	  LogAndReturn(NMPS__TRANSCREAOBJ, sts);
	}

	/* Copy data from display object */
	if ( rcv_ptr->function & NMPS_DATARCVFUNC_DISPLAYOBJECT)
	  memcpy( objectp, rcv_ptr->display_object_ptr, 
			rcv_ptr->display_object_size);

	/* Put the data object into the cell */
	if ( rcv_ptr->function & NMPS_DATARCVFUNC_CELLINSERT )
	{
	  /* Check that the cell is not busy... */
	  rcv_ptr->cell_ptr->ExternObjId = objid;
	  rcv_ptr->cell_ptr->ExternOpType = 0;
	  rcv_ptr->cell_ptr->ExternFlag = 1;
	}
	if ( rcv_ptr->function & NMPS_DATARCVFUNC_DISPLAYRESET &&
	     rcv_ptr->function & NMPS_DATARCVFUNC_DISPLAYOBJECT)
	  memset( rcv_ptr->display_object_ptr, 0, rcv_ptr->display_object_size);
	rcv_ptr->wait_for_accept = 0;

	return NMPS__SUCCESS;
}

static int	nmpstrans_req_reset_detected( trans_ctx	transctx,
				nmpstrans_t_req_list	*req_ptr)
{
	if ( req_ptr->function & NMPS_REQUESTFUNC_DISPLAYRESET &&
	     req_ptr->function & NMPS_REQUESTFUNC_DISPLAYOBJECT)
	  memset( req_ptr->display_object_ptr, 0, req_ptr->display_object_size);
	req_ptr->wait_for_accept = 0;
	if ( req_ptr->function & NMPS_REQUESTFUNC_INTKEY)
	  req_ptr->req->Key = 0;
	else
	  memset( req_ptr->req->KeyStr, 0, sizeof(req_ptr->req->KeyStr));

	return NMPS__SUCCESS;
}


static int	nmpstrans_rcv_reset_detected( trans_ctx	transctx,
				nmpstrans_t_rcv_list	*rcv_ptr)
{
	if ( rcv_ptr->function & NMPS_DATARCVFUNC_DISPLAYRESET &&
	     rcv_ptr->function & NMPS_DATARCVFUNC_DISPLAYOBJECT)
	  memset( rcv_ptr->display_object_ptr, 0, rcv_ptr->display_object_size);
	rcv_ptr->wait_for_accept = 0;

	return NMPS__SUCCESS;
}




/****************************************************************************
* Name:		nmpstrans_reqlist_close()
*
* Type		void
*
* Type		Parameter	IOGF	Description
*
* Description:
*		Unref and free reqlist.
*
**************************************************************************/
static void	nmpstrans_reqlist_close( trans_ctx transctx)
{
	nmpstrans_t_req_list	*reqlist_ptr;
	int i;
	pwr_tStatus sts;

	reqlist_ptr = transctx->reqlist;
	for ( i = 0; i < transctx->req_count; i++) {
	  sts = gdh_DLUnrefObjectInfo( reqlist_ptr->subid);
	  if ( reqlist_ptr->req->Function & NMPS_REQUESTFUNC_DISPLAYOBJECT)
	    sts = gdh_DLUnrefObjectInfo( reqlist_ptr->display_object_subid);
	  sts = gdh_DLUnrefObjectInfo( reqlist_ptr->send_remtrans_subid);
	  sts = gdh_DLUnrefObjectInfo( reqlist_ptr->send_remtransbuff_subid);
	  sts = gdh_DLUnrefObjectInfo( reqlist_ptr->rcv_remtrans_subid);
	  sts = gdh_DLUnrefObjectInfo( reqlist_ptr->rcv_remtransbuff_subid);
	  if ( reqlist_ptr->req->Function & NMPS_REQUESTFUNC_CELLINSERT)
	    sts = gdh_DLUnrefObjectInfo( reqlist_ptr->cell_subid);	  
	  free( reqlist_ptr->conv_table);
	  reqlist_ptr->conv_table = 0;
	  reqlist_ptr++;
	}
	free( transctx->reqlist);
	transctx->reqlist = 0;
	transctx->req_count = 0;
}

/****************************************************************************
* Name:		nmpstrans_reqlist_add()
*
* Type		pwr_tStatus
*
* Type		Parameter	IOGF	Description
*
* Description:
*		Add a request object to the reqlist.
*
**************************************************************************/
static pwr_tStatus	nmpstrans_reqlist_add(
			trans_ctx		transctx,
			pwr_tObjid 		objid,
			nmpstrans_t_req_list 	**reqlist,
			int			*reqlist_count)
{
	nmpstrans_t_req_list	*reqlist_ptr;
	nmpstrans_t_req_list	*new_reqlist;
	pwr_sAttrRef		attrref;
	pwr_tStatus		sts;
	pwr_tClassId		class;
	char			*objectp;
	pwr_tObjid		buff_objid;

	sts = gdh_ObjidToPointer ( objid, (pwr_tAddress *) &objectp);
	if (EVEN(sts)) LogAndReturn(NMPS__REQOBJECT, sts);

	if ( *reqlist_count == 0)
	{
	  *reqlist = calloc( 1, sizeof(nmpstrans_t_req_list));
	  if ( *reqlist == 0)
	    return NMPS__NOMEMORY;
	}
	else
	{
	  new_reqlist = calloc( *reqlist_count + 1,
			sizeof(nmpstrans_t_req_list));
	  if ( new_reqlist == 0)
	    return NMPS__NOMEMORY;
	  memcpy( new_reqlist, *reqlist,
			*reqlist_count * sizeof(nmpstrans_t_req_list));
	  free( *reqlist);
	  *reqlist = new_reqlist;
	}
	reqlist_ptr = *reqlist + *reqlist_count;

	reqlist_ptr->objid = objid;

	/* Direct link to the request object */
	memset( &attrref, 0, sizeof(attrref));
	attrref.Objid = objid;
	sts = gdh_DLRefObjectInfoAttrref ( &attrref,
		(pwr_tAddress *) &reqlist_ptr->req,
		&reqlist_ptr->subid);
	if (EVEN(sts)) LogAndReturn(NMPS__BCKCELL, sts);

	if ( reqlist_ptr->req->Function & NMPS_REQUESTFUNC_DISPLAYOBJECT)
	{
	  /* Direct link to the  display object */
	  sts = gdh_GetObjectSize ( reqlist_ptr->req->DisplayObject, 
			&reqlist_ptr->display_object_size);
	  if ( EVEN(sts)) return NMPS__REQDISPLAYOBJECT;

	  memset( &attrref, 0, sizeof(attrref));
	  attrref.Objid = reqlist_ptr->req->DisplayObject;
	  sts = gdh_DLRefObjectInfoAttrref ( &attrref,
		(pwr_tAddress *) &reqlist_ptr->display_object_ptr,
		&reqlist_ptr->display_object_subid);
	  if (EVEN(sts)) LogAndReturn(NMPS__REQDISPLAYOBJECT, sts);
	}

	/* Direct link to the SendRemTrans object */
	memset( &attrref, 0, sizeof(attrref));
	attrref.Objid = reqlist_ptr->req->SendRemTrans;

	sts = gdh_GetObjectClass ( attrref.Objid, &class);
	if (EVEN(sts)) LogAndReturn(NMPS__REQSENDREMTRANS, sts);
	if ( class == pwr_cClass_RemTrans)
	{
	  reqlist_ptr->send_remtrans_type = NMPS_TRANSTYPE_REM;
	  sts = gdh_DLRefObjectInfoAttrref ( &attrref,
		(pwr_tAddress *) &reqlist_ptr->send_remtrans_ptr,
		&reqlist_ptr->send_remtrans_subid);
	  if (EVEN(sts)) LogAndReturn(NMPS__REQSENDREMTRANS, sts);
	}
	else if ( class == pwr_cClass_LocTrans)
	{
	  reqlist_ptr->send_remtrans_type = NMPS_TRANSTYPE_LOC;
	  sts = gdh_DLRefObjectInfoAttrref ( &attrref,
		(pwr_tAddress *) &reqlist_ptr->send_loctrans_ptr,
		&reqlist_ptr->send_remtrans_subid);
	  if (EVEN(sts)) LogAndReturn(NMPS__REQSENDREMTRANS, sts);
	}
	else
	  LogAndReturn(NMPS__REQSENDREMTRANS, sts);

	/* Direct link to the SendRemTrans buffer object */
	sts = gdh_GetChild( reqlist_ptr->req->SendRemTrans, &buff_objid);
	if ( EVEN(sts)) return NMPS__REQSENDREMTRANSBUF;
	memset( &attrref, 0, sizeof(attrref));
	attrref.Objid = buff_objid;
	sts = gdh_DLRefObjectInfoAttrref ( &attrref,
		(pwr_tAddress *) &reqlist_ptr->send_remtransbuff_ptr,
		&reqlist_ptr->send_remtransbuff_subid);
	if (EVEN(sts)) LogAndReturn(NMPS__REQSENDREMTRANSBUF, sts);

	/* Direct link to the RcvRemTrans object */
	memset( &attrref, 0, sizeof(attrref));
	attrref.Objid = reqlist_ptr->req->RcvRemTrans;

	sts = gdh_GetObjectClass ( attrref.Objid, &class);
	if (EVEN(sts)) LogAndReturn(NMPS__REQRCVREMTRANS, sts);
	if ( class == pwr_cClass_RemTrans)
	{
	  reqlist_ptr->rcv_remtrans_type = NMPS_TRANSTYPE_REM;
	  sts = gdh_DLRefObjectInfoAttrref ( &attrref,
		(pwr_tAddress *) &reqlist_ptr->rcv_remtrans_ptr,
		&reqlist_ptr->rcv_remtrans_subid);
	  if (EVEN(sts)) LogAndReturn(NMPS__REQRCVREMTRANS, sts);
	}
	else if ( class == pwr_cClass_LocTrans)
	{
	  reqlist_ptr->rcv_remtrans_type = NMPS_TRANSTYPE_LOC;
	  sts = gdh_DLRefObjectInfoAttrref ( &attrref,
		(pwr_tAddress *) &reqlist_ptr->rcv_loctrans_ptr,
		&reqlist_ptr->rcv_remtrans_subid);
	  if (EVEN(sts)) LogAndReturn(NMPS__REQRCVREMTRANS, sts);
	}
	else
	  LogAndReturn(NMPS__REQRCVREMTRANS, sts);

	/* Direct link to the RcvRemTrans buffer object */
	sts = gdh_GetChild( reqlist_ptr->req->RcvRemTrans, &buff_objid);
	if ( EVEN(sts)) return NMPS__REQRCVREMTRANSBUF;
	memset( &attrref, 0, sizeof(attrref));
	attrref.Objid = buff_objid;
	sts = gdh_DLRefObjectInfoAttrref ( &attrref,
		(pwr_tAddress *) &reqlist_ptr->rcv_remtransbuff_ptr,
		&reqlist_ptr->rcv_remtransbuff_subid);
	if (EVEN(sts)) LogAndReturn(NMPS__REQRCVREMTRANSBUF, sts);

	if ( reqlist_ptr->req->Function & NMPS_REQUESTFUNC_CELLINSERT)
	{
	  /* Direct link to the Cell object */
	  memset( &attrref, 0, sizeof(attrref));
	  attrref.Objid = reqlist_ptr->req->CellObject;
	  sts = gdh_DLRefObjectInfoAttrref ( &attrref,
		(pwr_tAddress *) &reqlist_ptr->cell_ptr,
		&reqlist_ptr->cell_subid);
	  if (EVEN(sts)) LogAndReturn(NMPS__REQINCELL, sts);
	}

	/* Get the name of the data parent object */
	sts = gdh_ObjidToName ( reqlist_ptr->req->DataParent,
		reqlist_ptr->parent_name, sizeof(reqlist_ptr->parent_name), 
		cdh_mName_volumeStrict);
	if (EVEN(sts)) LogAndReturn(NMPS__REQDATAPARENT, sts);

	/* Check DataClass */
	sts = gdh_GetObjectClass ( reqlist_ptr->req->DataClass, &class);
	if (EVEN(sts)) LogAndReturn(NMPS__REQDATACLASS, sts);
	
	if ( class != pwr_eClass_ClassDef)
	  LogAndReturn(NMPS__REQDATACLASS, sts);

	/* Create a conversion table */
	sts = cvn_ConvInit( reqlist_ptr->req->ToConvdefType,
			    reqlist_ptr->req->ToConvdef,
	 	   	    reqlist_ptr->req->ToConvdefFile,
			    reqlist_ptr->req->FromConvdefType,
			    reqlist_ptr->req->FromConvdef,
	 	   	    reqlist_ptr->req->FromConvdefFile,
			    &reqlist_ptr->conv_table_count,
			    1,
			    (char **) &reqlist_ptr->conv_table);
	if (EVEN(sts)) LogAndReturn(NMPS__REQCONVTABLE, sts);

	reqlist_ptr->function = reqlist_ptr->req->Function;
	if ( reqlist_ptr->function & NMPS_REQUESTFUNC_INTKEY)
	  reqlist_ptr->old_key = reqlist_ptr->req->Key;
	else
	  strncpy( reqlist_ptr->old_key_str, reqlist_ptr->req->KeyStr,
			sizeof(reqlist_ptr->req->KeyStr));

	(*reqlist_count)++;
	return NMPS__SUCCESS;
}


/****************************************************************************
* Name:		nmpstrans_rcvlist_close()
*
* Type		pwr_tStatus
*
* Type		Parameter	IOGF	Description
*
* Description:
*		Unref and free the rcvlist.
*
**************************************************************************/
static void	nmpstrans_rcvlist_close( trans_ctx transctx)
{
	nmpstrans_t_rcv_list	*rcvlist_ptr;
	int i;
	pwr_tStatus sts;

	rcvlist_ptr = transctx->rcvlist;
	for ( i = 0; i < transctx->rcv_count; i++) {

	  sts = gdh_DLUnrefObjectInfo( rcvlist_ptr->subid);
	  if ( rcvlist_ptr->rcv->Function & NMPS_DATARCVFUNC_DISPLAYOBJECT)
	    sts = gdh_DLUnrefObjectInfo( rcvlist_ptr->display_object_subid);
	  if ( rcvlist_ptr->rcv->Function & NMPS_DATARCVFUNC_ACK) {
	    sts = gdh_DLUnrefObjectInfo( rcvlist_ptr->send_remtrans_subid);
	    sts = gdh_DLUnrefObjectInfo( rcvlist_ptr->send_remtransbuff_subid);
	  }
	  sts = gdh_DLUnrefObjectInfo( rcvlist_ptr->rcv_remtrans_subid);
	  sts = gdh_DLUnrefObjectInfo( rcvlist_ptr->rcv_remtransbuff_subid);
	  if ( rcvlist_ptr->rcv->Function & NMPS_DATARCVFUNC_CELLINSERT)
	    sts = gdh_DLUnrefObjectInfo( rcvlist_ptr->cell_subid);
	  free( rcvlist_ptr->conv_table);
	  rcvlist_ptr->conv_table = 0;
	  rcvlist_ptr++;
	}
	free( transctx->rcvlist);
	transctx->rcvlist = 0;
	transctx->rcv_count = 0;
}

/****************************************************************************
* Name:		nmpstrans_rcvlist_add()
*
* Type		pwr_tStatus
*
* Type		Parameter	IOGF	Description
*
* Description:
*		Add a DataRcv object to the rcvlist.
*
**************************************************************************/
static pwr_tStatus	nmpstrans_rcvlist_add(
			trans_ctx		transctx,
			pwr_tObjid 		objid,
			nmpstrans_t_rcv_list 	**rcvlist,
			int			*rcvlist_count)
{
	nmpstrans_t_rcv_list	*rcvlist_ptr;
	nmpstrans_t_rcv_list	*new_rcvlist;
	pwr_sAttrRef		attrref;
	pwr_tStatus		sts;
	pwr_tClassId		class;
	char			*objectp;
	pwr_tObjid		buff_objid;

	sts = gdh_ObjidToPointer ( objid, (pwr_tAddress *) &objectp);
	if (EVEN(sts)) LogAndReturn(NMPS__RCVOBJECT, sts);

	if ( *rcvlist_count == 0)
	{
	  *rcvlist = calloc( 1, sizeof(nmpstrans_t_rcv_list));
	  if ( *rcvlist == 0)
	    return NMPS__NOMEMORY;
	}
	else
	{
	  new_rcvlist = calloc( *rcvlist_count + 1,
			sizeof(nmpstrans_t_rcv_list));
	  if ( new_rcvlist == 0)
	    return NMPS__NOMEMORY;
	  memcpy( new_rcvlist, *rcvlist,
			*rcvlist_count * sizeof(nmpstrans_t_rcv_list));
	  free( *rcvlist);
	  *rcvlist = new_rcvlist;
	}
	rcvlist_ptr = *rcvlist + *rcvlist_count;

	rcvlist_ptr->objid = objid;

	/* Direct link to the DataRcv object */
	memset( &attrref, 0, sizeof(attrref));
	attrref.Objid = objid;
	sts = gdh_DLRefObjectInfoAttrref ( &attrref,
		(pwr_tAddress *) &rcvlist_ptr->rcv,
		&rcvlist_ptr->subid);
	if (EVEN(sts)) LogAndReturn(NMPS__BCKCELL, sts);

	if ( rcvlist_ptr->rcv->Function & NMPS_DATARCVFUNC_DISPLAYOBJECT)
	{
	  /* Direct link to the  display object */
	  sts = gdh_GetObjectSize ( rcvlist_ptr->rcv->DisplayObject, 
			&rcvlist_ptr->display_object_size);
	  if ( EVEN(sts)) return NMPS__RCVDISPLAYOBJECT;

	  memset( &attrref, 0, sizeof(attrref));
	  attrref.Objid = rcvlist_ptr->rcv->DisplayObject;
	  sts = gdh_DLRefObjectInfoAttrref ( &attrref,
		(pwr_tAddress *) &rcvlist_ptr->display_object_ptr,
		&rcvlist_ptr->display_object_subid);
	  if (EVEN(sts)) LogAndReturn(NMPS__RCVDISPLAYOBJECT, sts);
	}

	if ( rcvlist_ptr->rcv->Function & NMPS_DATARCVFUNC_ACK)
	{
	  /* Direct link to the SendRemTrans object */
	  memset( &attrref, 0, sizeof(attrref));
	  attrref.Objid = rcvlist_ptr->rcv->SendRemTrans;

	  sts = gdh_GetObjectClass ( attrref.Objid, &class);
	  if (EVEN(sts)) LogAndReturn(NMPS__RCVSENDREMTRANS, sts);
	  if ( class == pwr_cClass_RemTrans)
	  {
	    rcvlist_ptr->send_remtrans_type = NMPS_TRANSTYPE_REM;
	    sts = gdh_DLRefObjectInfoAttrref ( &attrref,
		(pwr_tAddress *) &rcvlist_ptr->send_remtrans_ptr,
		&rcvlist_ptr->send_remtrans_subid);
	    if (EVEN(sts)) LogAndReturn(NMPS__RCVSENDREMTRANS, sts);
	  }
	  else if ( class == pwr_cClass_LocTrans)
	  {
	    rcvlist_ptr->send_remtrans_type = NMPS_TRANSTYPE_LOC;
	    sts = gdh_DLRefObjectInfoAttrref ( &attrref,
		(pwr_tAddress *) &rcvlist_ptr->send_loctrans_ptr,
		&rcvlist_ptr->send_remtrans_subid);
	    if (EVEN(sts)) LogAndReturn(NMPS__RCVSENDREMTRANS, sts);
	  }
	  else
	    LogAndReturn(NMPS__RCVSENDREMTRANS, sts);

	  /* Direct link to the SendRemTrans buffer object */
	  sts = gdh_GetChild( rcvlist_ptr->rcv->SendRemTrans, &buff_objid);
	  if ( EVEN(sts)) return NMPS__RCVSENDREMTRANSBUF;
	  memset( &attrref, 0, sizeof(attrref));
	  attrref.Objid = buff_objid;
	  sts = gdh_DLRefObjectInfoAttrref ( &attrref,
		(pwr_tAddress *) &rcvlist_ptr->send_remtransbuff_ptr,
		&rcvlist_ptr->send_remtransbuff_subid);
	  if (EVEN(sts)) LogAndReturn(NMPS__RCVSENDREMTRANSBUF, sts);
	}

	/* Direct link to the RcvRemTrans object */
	memset( &attrref, 0, sizeof(attrref));
	attrref.Objid = rcvlist_ptr->rcv->RcvRemTrans;

	sts = gdh_GetObjectClass ( attrref.Objid, &class);
	if (EVEN(sts)) LogAndReturn(NMPS__RCVRCVREMTRANS, sts);
	if ( class == pwr_cClass_RemTrans)
	{
	  rcvlist_ptr->rcv_remtrans_type = NMPS_TRANSTYPE_REM;
	  sts = gdh_DLRefObjectInfoAttrref ( &attrref,
		(pwr_tAddress *) &rcvlist_ptr->rcv_remtrans_ptr,
		&rcvlist_ptr->rcv_remtrans_subid);
	  if (EVEN(sts)) LogAndReturn(NMPS__RCVRCVREMTRANS, sts);
	}
	else if ( class == pwr_cClass_LocTrans)
	{
	  rcvlist_ptr->rcv_remtrans_type = NMPS_TRANSTYPE_LOC;
	  sts = gdh_DLRefObjectInfoAttrref ( &attrref,
		(pwr_tAddress *) &rcvlist_ptr->rcv_loctrans_ptr,
		&rcvlist_ptr->rcv_remtrans_subid);
	  if (EVEN(sts)) LogAndReturn(NMPS__RCVRCVREMTRANS, sts);
	}
	else
	  LogAndReturn(NMPS__RCVRCVREMTRANS, sts);

	/* Direct link to the RcvRemTrans buffer object */
	sts = gdh_GetChild( rcvlist_ptr->rcv->RcvRemTrans, &buff_objid);
	if ( EVEN(sts)) return NMPS__RCVRCVREMTRANSBUF;
	memset( &attrref, 0, sizeof(attrref));
	attrref.Objid = buff_objid;
	sts = gdh_DLRefObjectInfoAttrref ( &attrref,
		(pwr_tAddress *) &rcvlist_ptr->rcv_remtransbuff_ptr,
		&rcvlist_ptr->rcv_remtransbuff_subid);
	if (EVEN(sts)) LogAndReturn(NMPS__RCVRCVREMTRANSBUF, sts);

	if ( rcvlist_ptr->rcv->Function & NMPS_DATARCVFUNC_CELLINSERT)
	{
	  /* Direct link to the Cell object */
	  memset( &attrref, 0, sizeof(attrref));
	  attrref.Objid = rcvlist_ptr->rcv->CellObject;
	  sts = gdh_DLRefObjectInfoAttrref ( &attrref,
		(pwr_tAddress *) &rcvlist_ptr->cell_ptr,
		&rcvlist_ptr->cell_subid);
	  if (EVEN(sts)) LogAndReturn(NMPS__RCVINCELL, sts);
	}

	/* Get the name of the data parent object */
	sts = gdh_ObjidToName ( rcvlist_ptr->rcv->DataParent,
		rcvlist_ptr->parent_name, sizeof(rcvlist_ptr->parent_name), 
		cdh_mName_volumeStrict);
	if (EVEN(sts)) LogAndReturn(NMPS__RCVDATAPARENT, sts);

	/* Check DataClass */
	sts = gdh_GetObjectClass ( rcvlist_ptr->rcv->DataClass, &class);
	if (EVEN(sts)) LogAndReturn(NMPS__RCVDATACLASS, sts);
	
	if ( class != pwr_eClass_ClassDef)
	  LogAndReturn(NMPS__RCVDATACLASS, sts);

	/* Create a conversion table */
	sts = cvn_ConvInit( rcvlist_ptr->rcv->ToConvdefType,
			    rcvlist_ptr->rcv->ToConvdef,
	 	   	    rcvlist_ptr->rcv->ToConvdefFile,
			    rcvlist_ptr->rcv->FromConvdefType,
			    rcvlist_ptr->rcv->FromConvdef,
	 	   	    rcvlist_ptr->rcv->FromConvdefFile,
			    &rcvlist_ptr->conv_table_count,
			    1,
			    (char **) &rcvlist_ptr->conv_table);
	if (EVEN(sts)) LogAndReturn(NMPS__RCVCONVTABLE, sts);

	rcvlist_ptr->function = rcvlist_ptr->rcv->Function;

	(*rcvlist_count)++;
	return NMPS__SUCCESS;
}


/****************************************************************************
* Name:		nmpstrans_sndlist_close()
*
* Type		void
*
* Type		Parameter	IOGF	Description
*
* Description:
*		Unref and free the sndlist.
*
**************************************************************************/
static void	nmpstrans_sndlist_close( trans_ctx		transctx)
{
	nmpstrans_t_snd_list	*sndlist_ptr;
	int i;
	pwr_tStatus sts;

	sndlist_ptr = transctx->sndlist;
	for ( i = 0; i < transctx->snd_count; i++) {
	  sts = gdh_DLUnrefObjectInfo( sndlist_ptr->subid);
	  sts = gdh_DLUnrefObjectInfo( sndlist_ptr->send_remtrans_subid);
	  sts = gdh_DLUnrefObjectInfo( sndlist_ptr->send_remtransbuff_subid);
	  if ( sndlist_ptr->snd->Function & NMPS_DATASENDFUNC_ACK) {
	    sts = gdh_DLUnrefObjectInfo( sndlist_ptr->rcv_remtrans_subid);
	    sts = gdh_DLUnrefObjectInfo( sndlist_ptr->rcv_remtransbuff_subid);
	  }	  
	  free( sndlist_ptr->conv_table);

	  sndlist_ptr++;
	}
	free( transctx->sndlist);
	transctx->sndlist = 0;
	transctx->snd_count = 0;
}


/****************************************************************************
* Name:		nmpstrans_sndlist_add()
*
* Type		pwr_tStatus
*
* Type		Parameter	IOGF	Description
*
* Description:
*		Add a request object to the reqlist.
*
**************************************************************************/
static pwr_tStatus	nmpstrans_sndlist_add(
			trans_ctx		transctx,
			pwr_tObjid 		objid,
			nmpstrans_t_snd_list 	**sndlist,
			int			*sndlist_count)
{
	nmpstrans_t_snd_list	*sndlist_ptr;
	nmpstrans_t_snd_list	*new_sndlist;
	pwr_sAttrRef		attrref;
	pwr_tStatus		sts;
	pwr_tClassId		class;
	char			*objectp;
	pwr_tObjid		buff_objid;

	sts = gdh_ObjidToPointer ( objid, (pwr_tAddress *) &objectp);
	if (EVEN(sts)) LogAndReturn(NMPS__SNDOBJECT, sts);

	if ( *sndlist_count == 0)
	{
	  *sndlist = calloc( 1, sizeof(nmpstrans_t_snd_list));
	  if ( *sndlist == 0)
	    return NMPS__NOMEMORY;
	}
	else
	{
	  new_sndlist = calloc( *sndlist_count + 1,
			sizeof(nmpstrans_t_snd_list));
	  if ( new_sndlist == 0)
	    return NMPS__NOMEMORY;
	  memcpy( new_sndlist, *sndlist,
			*sndlist_count * sizeof(nmpstrans_t_snd_list));
	  free( *sndlist);
	  *sndlist = new_sndlist;
	}
	sndlist_ptr = *sndlist + *sndlist_count;

	sndlist_ptr->objid = objid;

	/* Direct link to the DataSend object */
	memset( &attrref, 0, sizeof(attrref));
	attrref.Objid = objid;
	sts = gdh_DLRefObjectInfoAttrref ( &attrref,
		(pwr_tAddress *) &sndlist_ptr->snd,
		&sndlist_ptr->subid);
	if (EVEN(sts)) LogAndReturn(NMPS__BCKCELL, sts);

	/* Direct link to the SendRemTrans object */
	memset( &attrref, 0, sizeof(attrref));
	attrref.Objid = sndlist_ptr->snd->SendRemTrans;

	sts = gdh_GetObjectClass ( attrref.Objid, &class);
	if (EVEN(sts)) LogAndReturn(NMPS__SNDSENDREMTRANS, sts);
	if ( class == pwr_cClass_RemTrans)
	{
	  sndlist_ptr->send_remtrans_type = NMPS_TRANSTYPE_REM;
	  sts = gdh_DLRefObjectInfoAttrref ( &attrref,
		(pwr_tAddress *) &sndlist_ptr->send_remtrans_ptr,
		&sndlist_ptr->send_remtrans_subid);
	  if (EVEN(sts)) LogAndReturn(NMPS__SNDSENDREMTRANS, sts);
	}
	else if ( class == pwr_cClass_LocTrans)
	{
	  sndlist_ptr->send_remtrans_type = NMPS_TRANSTYPE_LOC;
	  sts = gdh_DLRefObjectInfoAttrref ( &attrref,
		(pwr_tAddress *) &sndlist_ptr->send_loctrans_ptr,
		&sndlist_ptr->send_remtrans_subid);
	  if (EVEN(sts)) LogAndReturn(NMPS__SNDSENDREMTRANS, sts);
	}
	else
	  LogAndReturn(NMPS__SNDSENDREMTRANS, sts);

	/* Direct link to the SendRemTrans buffer object */
	sts = gdh_GetChild( sndlist_ptr->snd->SendRemTrans, &buff_objid);
	if ( EVEN(sts)) return NMPS__SNDSENDREMTRANSBUF;
	memset( &attrref, 0, sizeof(attrref));
	attrref.Objid = buff_objid;
	sts = gdh_DLRefObjectInfoAttrref ( &attrref,
		(pwr_tAddress *) &sndlist_ptr->send_remtransbuff_ptr,
		&sndlist_ptr->send_remtransbuff_subid);
	if (EVEN(sts)) LogAndReturn(NMPS__SNDSENDREMTRANSBUF, sts);

	if ( sndlist_ptr->snd->Function & NMPS_DATASENDFUNC_ACK)
	{
	  /* Direct link to the RcvRemTrans object */
	  memset( &attrref, 0, sizeof(attrref));
	  attrref.Objid = sndlist_ptr->snd->RcvRemTrans;

	  sts = gdh_GetObjectClass ( attrref.Objid, &class);
	  if (EVEN(sts)) LogAndReturn(NMPS__SNDRCVREMTRANS, sts);
	  if ( class == pwr_cClass_RemTrans)
	  {
	    sndlist_ptr->rcv_remtrans_type = NMPS_TRANSTYPE_REM;
	    sts = gdh_DLRefObjectInfoAttrref ( &attrref,
		(pwr_tAddress *) &sndlist_ptr->rcv_remtrans_ptr,
		&sndlist_ptr->rcv_remtrans_subid);
	    if (EVEN(sts)) LogAndReturn(NMPS__SNDRCVREMTRANS, sts);
	  }
	  else if ( class == pwr_cClass_LocTrans)
	  {
	    sndlist_ptr->rcv_remtrans_type = NMPS_TRANSTYPE_LOC;
	    sts = gdh_DLRefObjectInfoAttrref ( &attrref,
		(pwr_tAddress *) &sndlist_ptr->rcv_loctrans_ptr,
		&sndlist_ptr->rcv_remtrans_subid);
	    if (EVEN(sts)) LogAndReturn(NMPS__SNDRCVREMTRANS, sts);
	  }
	  else
	    LogAndReturn(NMPS__SNDRCVREMTRANS, sts);

	  /* Direct link to the RcvRemTrans buffer object */
	  sts = gdh_GetChild( sndlist_ptr->snd->RcvRemTrans, &buff_objid);
	  if ( EVEN(sts)) return NMPS__SNDRCVREMTRANSBUF;
	  memset( &attrref, 0, sizeof(attrref));
	  attrref.Objid = buff_objid;
	  sts = gdh_DLRefObjectInfoAttrref ( &attrref,
		(pwr_tAddress *) &sndlist_ptr->rcv_remtransbuff_ptr,
		&sndlist_ptr->rcv_remtransbuff_subid);
	  if (EVEN(sts)) LogAndReturn(NMPS__SNDRCVREMTRANSBUF, sts);
	}

	/* Create a conversion table */
	sts = cvn_ConvInit( sndlist_ptr->snd->ToConvdefType,
			    sndlist_ptr->snd->ToConvdef,
	 	   	    sndlist_ptr->snd->ToConvdefFile,
			    sndlist_ptr->snd->FromConvdefType,
			    sndlist_ptr->snd->FromConvdef,
	 	   	    sndlist_ptr->snd->FromConvdefFile,
			    &sndlist_ptr->conv_table_count,
			    1,
			    (char **) &sndlist_ptr->conv_table);
	if (EVEN(sts)) LogAndReturn(NMPS__SNDCONVTABLE, sts);

	sndlist_ptr->function = sndlist_ptr->snd->Function;
	
	/* Get the difference in offset to rtdb, to make it possible
	   to get the Da pointer */
	sndlist_ptr->rtdb_offset = 
		(char *) &sndlist_ptr->snd->Dummy -
		(char *) sndlist_ptr->snd->DummyP;

	(*sndlist_count)++;
	return NMPS__SUCCESS;
}


/*************************************************************************
*
* Name:		nmps_get_transconfig
*
* Typ		int
*
* Typ		Parameter	IOGF	Beskrivning
*
* Beskrivning: 
*	Hämta pekare till backup konfig objektet.
*
**************************************************************************/

static pwr_tStatus	nmps_get_transconfig( trans_ctx	transctx)
{
	pwr_tStatus	sts;
	pwr_tObjid	objid;
	pwr_sAttrRef	attrref;

	/* Get pointer to the transconfig object */
	sts = gdh_GetClassList ( pwr_cClass_NMpsTransConfig, &objid);
	if (EVEN(sts)) return NMPS__TRANSCONFIG;

	/* Direct link to the cell */
	memset( &attrref, 0, sizeof(attrref));
	attrref.Objid = objid;
	sts = gdh_DLRefObjectInfoAttrref ( &attrref,
		(pwr_tAddress *) &transctx->transconfig,
		&transctx->transconfig_dlid);
	if ( EVEN(sts)) return sts;
	return NMPS__SUCCESS;
}


/*************************************************************************
*
* Name:		nmpstrans_datareq_init
*
* Typ		int
*
* Typ		Parameter	IOGF	Beskrivning
*
* Beskrivning: 
*	Initiering av request funktionen.
*
**************************************************************************/

static pwr_tStatus	nmpstrans_datareq_init( trans_ctx	transctx)
{
	pwr_tStatus	sts;
	pwr_tObjid	objid;

	/* Get the cell objects on this node */
	sts = gdh_GetClassList ( pwr_cClass_DataRequest, &objid);
	while ( ODD(sts))
	{
          /* Store and direct link the cells */
          sts = nmpstrans_reqlist_add( transctx, objid, &transctx->reqlist, 
		&transctx->req_count);
	  if ( EVEN(sts)) return sts;

	  sts = gdh_GetNextObject( objid, &objid);
	}

	transctx->transconfig->RequestObjects = transctx->req_count;

	return NMPS__SUCCESS;
}


/*************************************************************************
*
* Name:		nmps_datarcv_init
*
* Typ		int
*
* Typ		Parameter	IOGF	Beskrivning
*
* Beskrivning: 
*	Initiering av request funktionen.
*
**************************************************************************/

static pwr_tStatus	nmpstrans_datarcv_init( trans_ctx	transctx)
{
	pwr_tStatus	sts;
	pwr_tObjid	objid;

	/* Get the cell objects on this node */
	sts = gdh_GetClassList ( pwr_cClass_DataRcv, &objid);
	while ( ODD(sts))
	{
          /* Store and direct link the cells */
          sts = nmpstrans_rcvlist_add( transctx, objid, &transctx->rcvlist, 
		&transctx->rcv_count);
	  if ( EVEN(sts)) return sts;

	  sts = gdh_GetNextObject( objid, &objid);
	}

/*	transctx->transconfig->DataRcvObjects = transctx->rcv_count;
*/
	return NMPS__SUCCESS;
}



/*************************************************************************
*
* Name:		nmpstrans_datasend_init
*
* Typ		int
*
* Typ		Parameter	IOGF	Beskrivning
*
* Beskrivning: 
*	Initialization of datasend object.
*
**************************************************************************/

static pwr_tStatus	nmpstrans_datasend_init( trans_ctx	transctx)
{
	pwr_tStatus	sts;
	pwr_tObjid	objid;

	/* Get the cell objects on this node */
	sts = gdh_GetClassList ( pwr_cClass_DataSend, &objid);
	while ( ODD(sts))
	{
          /* Store and direct link the cells */
          sts = nmpstrans_sndlist_add( transctx, objid, &transctx->sndlist, 
		&transctx->snd_count);
	  if ( EVEN(sts)) return sts;

	  sts = gdh_GetNextObject( objid, &objid);
	}

/*
	transctx->transconfig->DataSendCount = transctx->datasend_count;
*/
	return NMPS__SUCCESS;
}


/*************************************************************************
*
* Name:		nmpstrans_trans_handler
*
* Typ		int
*
* Typ		Parameter	IOGF	Beskrivning
*
* Beskrivning: 
*	This is the trans handler.
*
**************************************************************************/

static pwr_tStatus	nmpstrans_trans_handler( trans_ctx	transctx)
{
	pwr_tStatus		sts;
	nmpstrans_t_snd_list	*snd_ptr;
	nmpstrans_t_req_list	*req_ptr;
	nmpstrans_t_rcv_list	*rcv_ptr;
	int			i;
	int			trigg_change;
	int			trigg_detect;
	char			alarm_text[80];
	char			alarm_name[80];
	char			key[40];
        int                     reset_trigg;
        int                     keep_key;

	/* Loop through the req objects */
	req_ptr = transctx->reqlist;
	for ( i = 0; i < transctx->req_count; i++)
	{
	  trigg_change = 0;
	  trigg_detect = 0;
	  /* Is there a trigg or a change in key value */
	  if ( req_ptr->function & NMPS_REQUESTFUNC_CHANGE)
	  {
	    if ( req_ptr->function & NMPS_REQUESTFUNC_INTKEY)
	    {
	      if ( req_ptr->old_key != req_ptr->req->Key)
	      {
	        if ( req_ptr->req->Key != 0)
	        {
	          trigg_change = 1;
	          sprintf( key, "%d", req_ptr->req->Key);
	        }
	        else
	          req_ptr->old_key = req_ptr->req->Key;
	      }
	    }
	    else
	    {
	      if ( strcmp( req_ptr->old_key_str, req_ptr->req->KeyStr))
	      {
	        if ( strcmp( req_ptr->req->KeyStr ,""))
	        {
	          trigg_change = 1;
	          strncpy( key, req_ptr->req->KeyStr, sizeof(key));
	        }
	        else
	          strncpy( req_ptr->old_key_str, req_ptr->req->KeyStr,
				sizeof( req_ptr->old_key_str));
	      }
	    }
	  }
	  else
	  {
	    if ( req_ptr->function & NMPS_REQUESTFUNC_INTKEY)
	    {
	      if ( req_ptr->req->TriggDetected)
	      {
	        if ( req_ptr->req->Key != 0)
	        {
	          trigg_detect = 1;
	          sprintf( key, "%d", req_ptr->req->Key);
	        }
	        else
	          req_ptr->req->TriggDetected = 0;
	      }
	    }
	    else
	    {
	      if ( req_ptr->req->TriggDetected)
	      {
	        if ( strcmp( req_ptr->req->KeyStr, ""))
	        {
	          trigg_detect = 1;
	          strncpy( key, req_ptr->req->KeyStr, sizeof(key));
	        }
	        else
	          req_ptr->req->TriggDetected = 0;
	      }
	    }
	  }
	  if ( trigg_change || trigg_detect)
	  {
	    /* Time to send a request */
            reset_trigg = 0;
            keep_key = 0;

	    if ( req_ptr->function & NMPS_REQUESTFUNC_CHECKSUM)
	    {
              /* Check if valid checksum */
              int int_key, valid_key;

	      if ( req_ptr->function & NMPS_REQUESTFUNC_INTKEY) {
	        int_key = req_ptr->req->Key;
                valid_key = 1;
              }
              else {
	        sts = sscanf( req_ptr->req->KeyStr, "%d", &int_key);
                valid_key = (sts == 1);
	      }
              if ( valid_key)
                sts = nmpstrans_check_checksum( int_key);
              if ( !valid_key || EVEN(sts)) {
                /* Checksum is not ok */
	        if ( req_ptr->req->AlarmText[1][0] != 0)
	        {
	          sprintf( alarm_text, "%s %s", "Checksum error",
			   key);
	          strcpy( alarm_name, "NMpsTrans");
	          sts = nmpstrans_alarm_send( alarm_text, 
				alarm_name, 'B');

	        }
                keep_key = 1;
                reset_trigg = 1;
              }
            }

	    /* Check that the Cell is not full */
	    if ( req_ptr->function & NMPS_REQUESTFUNC_CELLINSERT &&
	        !(req_ptr->function & NMPS_REQUESTFUNC_DISPLAYOBJECT) &&
	        req_ptr->cell_ptr->CellFull)
	    {
	      /* Send an alarm, alarmtext idx 1, cell full */
	      if ( req_ptr->req->AlarmText[1][0] != 0)
	      {
	        sprintf( alarm_text, "%s %s", req_ptr->req->AlarmText[1], key);
	        strcpy( alarm_name, "NMpsTrans");
	        sts = nmpstrans_alarm_send( alarm_text, 
				alarm_name, 'B');

	      }
	      req_ptr->req->ErrorDetected = 1;
              reset_trigg = 1;
              keep_key = 0;
            }
	    if ( reset_trigg)
	    {
	      /* Reset trigg */
	      if ( req_ptr->function & NMPS_REQUESTFUNC_INTKEY)
	      {
                if ( !keep_key)
	          req_ptr->req->Key = 0;
	        if ( trigg_change)
	          req_ptr->old_key = req_ptr->req->Key;
	        else if (trigg_detect)
	          req_ptr->req->TriggDetected = 0;
	      }
	      else
	      {
                if ( !keep_key)
	          strcpy( req_ptr->req->KeyStr, "");
	        if ( trigg_change)
	          strncpy( req_ptr->old_key_str, req_ptr->req->KeyStr, 
			sizeof(req_ptr->old_key_str));
	        else if (trigg_detect)
	          req_ptr->req->TriggDetected = 0;
	      }
	      if ( req_ptr->function & NMPS_DATARCVFUNC_DISPLAYRESET &&
	           req_ptr->function & NMPS_DATARCVFUNC_DISPLAYOBJECT)
	        memset( req_ptr->display_object_ptr, 0, 
			req_ptr->display_object_size);
	    }
	    else
	    {
	      sts = nmpstrans_request_data( transctx, key, i,
			&transctx->req_translist, req_ptr);
	      if ( sts == NMPS__REMTRANSBUSY)
	      {
	        /* Wait till next scan */
	      }
	      else
	      {
	        /* Reset trigg */
	        if ( req_ptr->function & NMPS_REQUESTFUNC_INTKEY)
	        {
	          if ( trigg_change)
	            req_ptr->old_key = req_ptr->req->Key;
	          else if (trigg_detect)
	            req_ptr->req->TriggDetected = 0;
	        }
	        else
	        {
	          if ( trigg_change)
	            strncpy( req_ptr->old_key_str, req_ptr->req->KeyStr, 
			sizeof(req_ptr->old_key_str));
	          else if (trigg_detect)
	            req_ptr->req->TriggDetected = 0;
	        }
	      }
	    }
	  }

	  if ( req_ptr->req->AcceptDetected)
	  {
	    sts = nmpstrans_req_accept_detected( transctx, req_ptr);
	    if ( sts == NMPS__NOWAITACCEPT)
	    {
	      /* Nothing to wait for */
	      /* Send an alarm, alarmtext idx 2, No accept wait status */
	      if ( req_ptr->req->AlarmText[2][0] != 0)
	      {
	        strcpy( alarm_text, req_ptr->req->AlarmText[2]);
	        strcpy( alarm_name, "NMpsTrans");
	        sts = nmpstrans_alarm_send( alarm_text, 
				alarm_name, 'B');

	      }
	    }
	    req_ptr->req->AcceptDetected = 0;
	  }

	  if ( req_ptr->req->ResetDetected)
	  {
	    sts = nmpstrans_req_reset_detected( transctx, req_ptr);
	    req_ptr->req->ResetDetected = 0;
	  }

	  /* Is there anything to receive ? */
	  sts = nmpstrans_req_receive_data( transctx, &transctx->req_translist,
		req_ptr);
	  if ( sts == NMPS__TRANSCELLFULL)
	  {
	    if ( !req_ptr->cell_full_msg_sent)
	    {
	      req_ptr->cell_full_msg_sent = 1;

	      /* Send an alarm, alarmtext idx 1, cell full */
	      if ( req_ptr->req->AlarmText[1][0] != 0)
	      {
	        strcpy( alarm_text, req_ptr->req->AlarmText[1]);
	        strcpy( alarm_name, "NMpsTrans");
	        sts = nmpstrans_alarm_send( alarm_text, 
				alarm_name, 'B');
	      }
	      req_ptr->req->ErrorDetected = 1;
	    }
	    else 
	      req_ptr->cell_full_msg_sent = 0;
	  }

	  if ( req_ptr->fatal_error)
	  {
	    /* Send an alarm, alarmtext idx 3, fatal error */
	    if ( req_ptr->req->AlarmText[3][0] != 0)
	    {
	      strcpy( alarm_text, req_ptr->req->AlarmText[3]);
	      strcpy( alarm_name, "NMpsTrans");
	      sts = nmpstrans_alarm_send( alarm_text, 
				alarm_name, 'B');
	    }
	    req_ptr->req->ErrorDetected = 1;
	    req_ptr->fatal_error = 0;
	  }
	  req_ptr++;
	}

	/* Loop through the rcv objects */
	rcv_ptr = transctx->rcvlist;
	for ( i = 0; i < transctx->rcv_count; i++)
	{

	  if ( rcv_ptr->rcv->AcceptDetected)
	  {
	    sts = nmpstrans_rcv_accept_detected( transctx, rcv_ptr);
	    if ( sts == NMPS__NOWAITACCEPT)
	    {
	      /* Nothing to wait for */
	      /* Send an alarm, alarmtext idx 2, No accept wait status */
	      if ( rcv_ptr->rcv->AlarmText[2][0] != 0)
	      {
	        strcpy( alarm_text, rcv_ptr->rcv->AlarmText[2]);
	        strcpy( alarm_name, "NMpsTrans");
	        sts = nmpstrans_alarm_send( alarm_text, 
				alarm_name, 'B');

	      }
	    }
	    rcv_ptr->rcv->AcceptDetected = 0;
	  }

	  if ( rcv_ptr->rcv->ResetDetected)
	  {
	    sts = nmpstrans_rcv_reset_detected( transctx, rcv_ptr);
	    rcv_ptr->rcv->ResetDetected = 0;
	  }

	  /* Is there anything to receive ? */
	  sts = nmpstrans_rcv_receive_data( transctx, &transctx->rcv_translist,
		rcv_ptr);
	  if ( sts == NMPS__TRANSCELLFULL)
	  {
	    if ( !rcv_ptr->cell_full_msg_sent)
	    {
	      rcv_ptr->cell_full_msg_sent = 1;

	      /* Send an alarm, alarmtext idx 1, cell full */
	      if ( rcv_ptr->rcv->AlarmText[1][0] != 0)
	      {
	        strcpy( alarm_text, rcv_ptr->rcv->AlarmText[1]);
	        strcpy( alarm_name, "NMpsTrans");
	        sts = nmpstrans_alarm_send( alarm_text, 
				alarm_name, 'B');
	      }
	      rcv_ptr->rcv->ErrorDetected = 1;
	    }
	    else 
	      rcv_ptr->cell_full_msg_sent = 0;
	  }

	  if ( rcv_ptr->fatal_error)
	  {
	    /* Send an alarm, alarmtext idx 3, fatal error */
	    if ( rcv_ptr->rcv->AlarmText[3][0] != 0)
	    {
	      strcpy( alarm_text, rcv_ptr->rcv->AlarmText[3]);
	      strcpy( alarm_name, "NMpsTrans");
	      sts = nmpstrans_alarm_send( alarm_text, 
				alarm_name, 'B');
	    }
	    rcv_ptr->rcv->ErrorDetected = 1;
	    rcv_ptr->fatal_error = 0;
	  }
	  rcv_ptr++;
	}

	/* Loop through the snd objects */
	snd_ptr = transctx->sndlist;
	for ( i = 0; i < transctx->snd_count; i++)
	{
	  if ( snd_ptr->snd->TriggDetected)
	  {
	    /* Time to send some data */
	    sts = nmpstrans_send_datasend( transctx, i,
		&transctx->snd_translist, snd_ptr);
	    if ( sts == NMPS__REMTRANSBUSY)
	    {
	      /* Wait till next scan */
	    }
	    else if ( sts == NMPS__SNDNODATA)
	    {
	      /* Send an alarm, alarmtext idx 1, no data to send */
	      if ( snd_ptr->snd->AlarmText[1][0] != 0)
	      {
	        strcpy( alarm_text, snd_ptr->snd->AlarmText[1]);
	        strcpy( alarm_name, "NMpsTrans");
	        sts = nmpstrans_alarm_send( alarm_text,
				alarm_name, 'B');
	      }
	      snd_ptr->snd->ErrorDetected = 1;
	      snd_ptr->snd->TriggDetected = 0;
	    }
	    else
	      snd_ptr->snd->TriggDetected = 0;
	  }

	  /* Look for acknowledge message */
	  sts = nmpstrans_datasend_ack( transctx, &transctx->snd_translist,
		snd_ptr);

	  if ( snd_ptr->fatal_error)
	  {
	    /* Send an alarm, alarmtext idx 3, fatal error */
	    if ( snd_ptr->snd->AlarmText[3][0] != 0)
	    {
	      strcpy( alarm_text, snd_ptr->snd->AlarmText[3]);
	      strcpy( alarm_name, "NMpsTrans");
	      sts = nmpstrans_alarm_send( alarm_text,
				alarm_name, 'B');
	    }
	    snd_ptr->snd->ErrorDetected = 1;
	    snd_ptr->fatal_error = 0;
	  }

	  snd_ptr++;
	}

	sts = translist_timeout_check( transctx, &transctx->req_translist,
			nmpstrans_request_timeout);

	sts = translist_timeout_check( transctx, &transctx->snd_translist,
			nmpstrans_datasend_timeout);

	return NMPS__SUCCESS;
}


int main()
{
	trans_ctx		transctx;
	pwr_tStatus		sts;
	qcom_sQid 		qini;
	qcom_sQattr 		qAttr;
	int 			tmo;
	char 			mp[2000];
	qcom_sQid 		qid = qcom_cNQid;
	qcom_sGet 		get;
	int 			swap = 0;

	/* Init pams and gdh */
	sts = gdh_Init("rs_nmps_trans");
	if (EVEN(sts)) LogAndExit(sts);

	/* Init qcom and bind event que */
	if (!qcom_Init(&sts, 0, "rs_nmps_trans")) {
	  errh_Fatal("qcom_Init, %m", sts);
	  exit(sts);
	} 

	qAttr.type = qcom_eQtype_private;
	qAttr.quota = 100;
	if (!qcom_CreateQ(&sts, &qid, &qAttr, "events")) {
	  errh_Fatal("qcom_CreateQ, %m", sts);
	  exit(sts);
	} 

	qini = qcom_cQini;
	if (!qcom_Bind(&sts, &qid, &qini)) {
	  errh_Fatal("qcom_Bind(Qini), %m", sts);
	  exit(-1);
	}

	/* Wait for the plcpgm has flagged initizated */
	plc_UtlWaitForPlc();

	transctx = calloc( 1 , sizeof( *transctx));
	if ( transctx == 0 ) LogAndExit( NMPS__NOMEMORY);

	sts = nmps_get_transconfig( transctx);
	if (EVEN(sts)) LogAndExit( sts);

	sts = nmpstrans_datareq_init( transctx);
	if (EVEN(sts)) LogAndExit( sts);

	sts = nmpstrans_datarcv_init( transctx);
	if (EVEN(sts)) LogAndExit( sts);

	sts = nmpstrans_datasend_init( transctx);
	if (EVEN(sts)) LogAndExit( sts);

	tmo = 1000 * transctx->transconfig->CycleTime - 1;
	    
	for (;;) {
	  get.maxSize = sizeof(mp);
	  get.data = mp;
	  qcom_Get( &sts, &qid, &get, tmo);
	  if (sts == QCOM__TMO || sts == QCOM__QEMPTY) {
	    if ( !swap) {
	      sts = nmpstrans_trans_handler( transctx);
	      if (EVEN(sts)) LogAndExit( sts);
	      transctx->transconfig->LoopCount++;
	    }
	  } 
	  else {
	    ini_mEvent  new_event;
	    qcom_sEvent *ep = (qcom_sEvent*) get.data;
	    
	    new_event.m  = ep->mask;
	    if (new_event.b.oldPlcStop && !swap) {
	      swap = 1;
	      nmpstrans_reqlist_close( transctx);
	      nmpstrans_rcvlist_close( transctx);
	      nmpstrans_sndlist_close( transctx);
	    } else if (new_event.b.swapDone && swap) {
	      swap = 0;

	      sts = nmpstrans_datareq_init( transctx);
	      if (EVEN(sts)) LogAndExit( sts);

	      sts = nmpstrans_datarcv_init( transctx);
	      if (EVEN(sts)) LogAndExit( sts);

	      sts = nmpstrans_datasend_init( transctx);
	      if (EVEN(sts)) LogAndExit( sts);

	      errh_Info("Warm restart completed");
	    } else if (new_event.b.terminate) {
              exit(0);
	    }
	  }
	}
}


static pwr_tInt32 ssabutil_chksum_calculate(	pwr_tInt32 value,
						pwr_tInt16 *weights,
						pwr_tInt16 num_figures)
{
   pwr_tInt16   sum;         /* Arbetsvariabel */
   pwr_tInt16   *weightP;    /* Pekar på vikter */

   /* Beräkna perkaren till entalssiffran */

   weightP = weights + num_figures - 1;

   /* Beräkna den viktade summan av siffrorona i talet */
   for (sum=0; num_figures>0; num_figures--) {
       sum += (value % 10) * (*weightP--);
       value /= 10 ;
   }

   /* Beräkna skillnaden till närmast högre tiotal */
   sum = ((sum/10) + 1)*10 - sum;
   if (sum == 10) sum = 0;

   return (sum);
}

static pwr_tInt32 ssabutil_chksum_lop(	pwr_tInt32 lopnummer)
{
  static  pwr_tInt16   lop_fig_weights[6] = {1,7,3,1,7,3};

  lopnummer=lopnummer*10 + 
    ssabutil_chksum_calculate( lopnummer, lop_fig_weights, 6);

  return(lopnummer);
}

static int	ssabutil_lopnr_check( pwr_tInt32	lopnr)
{
  pwr_tInt32	tst_lopnr;
	
  tst_lopnr = lopnr / 10;
  tst_lopnr = ssabutil_chksum_lop( tst_lopnr);

  if ( tst_lopnr != lopnr)
    return 0;
  else
    return 1;
}
