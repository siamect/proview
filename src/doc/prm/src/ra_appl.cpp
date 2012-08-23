/*! \file ra_appl.cpp
    \brief Example application subclasses of rt_appl
   This file is an example file, containing to example applications.
*/

/** @addtogroup rt */
/*@{*/


/** @defgroup rt_ex Application examples
 *  @ingroup applex
 *  @{
 */
#include "pwr.h"
#include "co_error.h"
#include "co_time.h"
#include "rt_gdh.h"
#include "rt_appl.h"
#include "rt_mh_appl.h"
#include "rt_pwr_msg.h"


//! A simple c++ application example
/*! ra_appl is a subclass to rt_appl and implements the virtual
 * functions open(), close(), and scan().
 *
 * The open function fetches pointer to the digita value object Dv1 and Dv2, and to
 * the alalog value object Av1. It also connects to the message handler.
 * 
 * The scan function is called cyclically and sets one value to Av1 if Dv1 is high and
 * another value if Dv1 is low. 
 * When Dv2 goes high an alarm is sent.
 *
 * The close function unreferences the fetched pointers.
 *
 * Link the program with
 * g++ -o $pwrp_exe/ra_appl ra_appl.cpp $pwr_obj/pwr_msg_rt.o $pwr_obj/pwr_msg_co.o  
 * -DOS_POSIX -I$pwr_inc -L$pwr_lib -lpwr_rt -lpwr_co -lpwr_msg_dummy -lrt
 *
 * @see ra_appl.cpp
 */
class ra_appl : public rt_appl {
private:
  pwr_tSubid subid[20];		//! Array to store subscription identities
  int subid_cnt;		//! Number of stored subid's
  pwr_tBoolean *dv1_p;		//! Pointer to database value Dv1
  pwr_tBoolean *dv2_p;		//! Pointer to database value Dv2
  pwr_tBoolean dv2_old;		//! Old value of Dv2 for edge detection
  pwr_tFloat32 *av1_p;		//! Pointer to database value Av1
public:
  ra_appl() : rt_appl( "ra_appl", errh_eAnix_appl1), subid_cnt(0) {}
  void open();
  void close();
  void scan();
  int mh_send( const char *text, mh_eEventPrio prio);
};


void ra_appl::open()
{
  // Link to database objects
  pwr_tStatus sts;
  pwr_tUInt32 active_messages;

  sts = gdh_RefObjectInfo( (char *)"H1-Dv1.ActualValue", (void **)&dv1_p, &subid[subid_cnt++],
			   sizeof(*dv1_p));
  if ( EVEN(sts)) throw co_error(sts);
 
  sts = gdh_RefObjectInfo( (char *)"H1-Dv2.ActualValue", (void **)&dv2_p, &subid[subid_cnt++],
			   sizeof(*dv2_p));
  if ( EVEN(sts)) throw co_error(sts);
 
  sts = gdh_RefObjectInfo( (char *)"H1-Av1.ActualValue", (void **)&av1_p, &subid[subid_cnt++],
			   sizeof(*av1_p));
  if ( EVEN(sts)) throw co_error(sts);

  // Connect to message handler
  sts = mh_ApplConnect( pwr_cNObjid, (mh_mApplFlags)0, "ra_appl aborted", mh_eEvent_Info,
			mh_eEventPrio_A, 
			(mh_mEventFlags)(mh_mEventFlags_Bell | 
					 mh_mEventFlags_Ack | 
					 mh_mEventFlags_Return), 
			"ra_appl aborted", &active_messages);
  if ( EVEN(sts)) throw co_error(sts);
}

void ra_appl::close()
{
  // Unlink to database objects
  for ( int i = 0; i < subid_cnt; i++)
    gdh_UnrefObjectInfo( subid[i]);
}

void ra_appl::scan()
{
  // Do something
  if ( *dv1_p)
    *av1_p = 400;
  else
    *av1_p = 50;

  if ( *dv2_p && !dv2_old)
    mh_send( "Pump is stopped", mh_eEventPrio_A);
  dv2_old = *dv2_p;
}

//! Send alarm.
/*!
  \param text Alarm text
  \param prio Alarm prio
*/
int ra_appl::mh_send( const char *text, mh_eEventPrio prio) 	
{
  mh_sApplMessage 	msg;
  pwr_tUInt32	  	id; 
  pwr_tStatus  		sts;
  
  memset( &msg, 0, sizeof(msg));
  msg.EventPrio = prio;
  strncpy ( msg.EventText, text, sizeof(msg.EventText));
  strcpy ( msg.EventName, "from ra_appl");
  time_GetTime( &msg.EventTime);
  msg.EventFlags = (mh_mEventFlags)(mh_mEventFlags_Returned |
				    mh_mEventFlags_NoObject | 
				    mh_mEventFlags_Bell);
  msg.EventType = mh_eEvent_Alarm;
  msg.SupInfo.SupType = mh_eSupType_None; 

  sts = mh_ApplMessage( &id, &msg);
  if( EVEN(sts)) return sts;
  
  return PWR__SUCCESS;
} 

int main()
{
  ra_appl appl;

  try {
    appl.init();
    appl.register_appl( "Nodes-opg7-ra_appl");

    appl.mainloop();
  }
  catch ( co_error e) {
    errh_Fatal( "Application exception, %s", e.what().c_str());
    errh_SetStatus( PWR__APPLTERM);
    exit(0);
  }
}

   
/** @} */
/** @} */
