/************************************************************************
*************************************************************************
*
*		 P S S - 9 0 0 0
*		=================
*************************************************************************
*
* Filename:		RT_RTT_RTTSYS.C
*			Date	Pgm.	Read.	Remark
* Modified					Initial creation
*
* Description:
*	Application functions in rtt.
*
**************************************************************************
**************************************************************************/

/*_Include files_________________________________________________________*/
#include "pwr_inc:rt_rtt_menu.h"
#include "pwr_inc:rt_rtt_msg.h"

/*_Local rtt database____________________________________________________*/
RTT_DB_START
#include "pwrp_rtt:dtt_appl_RTTSYS_m.rdb1"
RTT_DB_CONTINUE
#include "pwrp_rtt:dtt_appl_RTTSYS_m.rdb2"
RTT_DB_END

/*************************************************************************
*
* Name:		RTTSYS_SHOW_NODES
*
* Type		int
*
* Type		Parameter	IOGF	Description
* menu_ctx	ctx		I	context of the picture.
* int		event		I 	type of event.
* char		*parameter_ptr	I	pointer to the parameter which value
*					has been changed.
*
* Description:
*	Application function of a picture.
*
**************************************************************************/

int RTTSYS_SHOW_NODES ( ctx, event, parameter_ptr)
menu_ctx	ctx;
int		event;
char		*parameter_ptr;
{ 

  switch ( event)
  {
    /**********************************************************
    *	Initialization of the picture
    ***********************************************************/
    case RTT_APPL_INIT:
      break;
    /**********************************************************
    *	Udate of the picture
    ***********************************************************/
    case RTT_APPL_UPDATE:
      break;
    /**********************************************************
    *	Exit of the picture
    ***********************************************************/
    case RTT_APPL_EXIT:
      break;
    /**********************************************************
    *	The value of a parameter is changed.
    ***********************************************************/
    case RTT_APPL_VALUECHANGED:
      break;
    /**********************************************************
    *	Next page key activated.
    ***********************************************************/
    case RTT_APPL_NEXTPAGE:
      break;
    /**********************************************************
    *	Pevious page key activated.
    ***********************************************************/
    case RTT_APPL_PREVPAGE:
      break;
  }
  return RTT__SUCCESS;
}
