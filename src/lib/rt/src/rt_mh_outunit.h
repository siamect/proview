#ifndef rt_mh_outunit_h
#define rt_mh_outunit_h

/* rt_mh_outunit.h -- <short description>

   PROVIEW/R
   Copyright (C) 1996 by Comator Process AB.

   <Description>.  */




#ifndef pwr_h
#include "pwr.h"
#endif

#ifndef pwr_class_h
#include "pwr_class.h"
#endif

#ifndef rt_mh_net_h
#include "rt_mh_net.h"
#endif

#ifndef rt_qcom_h
#include "rt_qcom.h"
#endif

#if defined __cplusplus
extern "C" {
#endif

#define mh_cSelLSize 20

typedef pwr_tUInt32 mh_mOutunitFlags;

#define mh_mOutunitFlags_ReadWait 1

/** 
 * @defgroup MSGH_O Outunits  
 * @ingroup  MSGH_FC
 * @{
 */

/* Callback prototypes */

/**
 * @brief Callback function to handle aknowledge messages. 
 *
 * It should be declared to the mh_Outunit API at the mh_OutunitConnect call. 
 * The function can have any name, but the address to the routine should be 
 * passed in the mh_OutunitConnect call. 
 * The mh_OutunitReceive calls this function if an acknowledge message arrives. 
 * @param *mh_cbOutunitAck Addres of an acknowledge message.
 * @return pwr_tStatus - Status return in standard VMS-format. 
 */
typedef pwr_tStatus (*mh_cbOutunitAck)(mh_sAck *); 

/** 
 * @brief  Callback function to handle alarm messages. 
 *
 * It should be declared to the mh_Outunit API at the mh_OutunitConnect call. 
 * The function can have any name, but the address to the routine should be passed in 
 * the mh_OutunitConnect call. 
 * The mh_Outunit API calls this function if an alarm message arrives. 
 * @param mh_cbOutunitAlarm Address of an alarm message. 
 * @return pwr_tStatus - Status return in standard VMS-format. 
 */
typedef pwr_tStatus (*mh_cbOutunitAlarm)(mh_sMessage *); 

/** 
 * @brief  Callback function to handle block messages. 
 * 
 * It should be declared to the mh_Outunit API at the mh_OutunitConnect call. 
 * The function can have any name, but the address to the routine should be passed 
 * in the mh_OutunitConnect call. 
 * The mh_Outunit API calls this function if a block message arrives. 
 * @param mh_cbOutunitBlock 
 * @return pwr_tStatus - Status return in standard VMS-format. 
 */
typedef pwr_tStatus (*mh_cbOutunitBlock)(mh_sBlock *);

/** 
 * @brief Callback function to handle cancel messages. 
 * 
 * It should be declared to the mh_Outunit API at the mh_OutunitConnect call. 
 * The function can have any name, but the address to the routine should be passed 
 * in the mh_OutunitConnect call. 
 * The mh_Outunit API calls this function if a cancel message arrives. 
 * @param mh_cbOutunitCancel Address of an cancel message.   
 * @return pwr_tStatus - Status return in standard VMS-format. 
 */
typedef pwr_tStatus (*mh_cbOutunitCancel)(mh_sReturn*); 

/** 
 * @brief Callback function to clear alarmlist. 
 * 
 * This is a callback function. It should be declared to the mh_Outunit API 
 * at the mh_OutunitConnect call. The function can have any name, but the 
 * address to the routine should be passed in the mh_OutunitConnect call. 
 * @param mh_cbOutunitClearAlarmList PAMS address of the node of which alarms 
 * should be cleared. 
 * @return pwr_tStatus - Status return in standard VMS-format. 
 */
typedef pwr_tStatus (*mh_cbOutunitClearAlarmList)(pwr_tNodeIndex); 

/** 
 * @brief  Callback function to clear blocklist.
 *  
 *  
 * @param mh_cbOutunitClearBlockList  
 * @return pwr_tStatus - Status return in standard VMS-format. 
 */
typedef pwr_tStatus (*mh_cbOutunitClearBlockList)(pwr_tNodeIndex); 

/** 
 * @brief Callback function to handle info messages.  
 * 
 * It should be declared to the mh_Outunit API at the mh_OutunitConnect call. 
 * The function can have any name, but the address to the routine should be passed 
 * in the mh_OutunitConnect call. 
 * The mh_Outunit API calls this function if an info message arrives. 
 * @param mh_cbOutunitInfo Address of an info message. 
 * @return pwr_tStatus - Status return in standard VMS-format. 
 */
typedef pwr_tStatus (*mh_cbOutunitInfo)(mh_sMessage *); 

/** 
 * @brief Callback function to handle return messages. 
 *
 * It should be declared to the mh_Outunit API at the mh_OutunitConnect call. 
 * The function can have any name, but the address to the routine should be passed 
 * in the mh_OutunitConnect call. 
 * The mh_Outunit API calls this function if a return message arrives. 
 * @param mh_cbOutunitReturn Address of a return message. 
 * @return pwr_tStatus - Status return in standard VMS-format. 
 */
typedef pwr_tStatus (*mh_cbOutunitReturn)(mh_sReturn *); 


/* Exported functions */


pwr_tStatus mh_OutunitAck(
  mh_sEventId      *Id
);
pwr_tStatus mh_OutunitBlock(
  pwr_tObjid       Object,
  mh_eEventPrio       Prio
);


pwr_tStatus mh_OutunitConnect(
  pwr_tObjid         Outunit,
  mh_eOutunitType     Type,
  mh_mOutunitFlags    Flags,
  mh_cbOutunitAck     Callback_Ack,
  mh_cbOutunitAlarm   Callback_Alarm,  
  mh_cbOutunitBlock   Callback_Block,
  mh_cbOutunitCancel  Callback_Cancel,
  mh_cbOutunitClearAlarmList  Callback_ClearAlarmList,
  mh_cbOutunitClearBlockList  Callback_ClearBlockList,
  mh_cbOutunitInfo    Callback_Info,
  mh_cbOutunitReturn  Callback_Return

);
pwr_tStatus mh_OutunitDisconnect();
pwr_tStatus mh_OutunitReceive();

/* Sets the timeout time in ms for mh_OutunitReceive.
 * This routine must be called before mh_OutunitReceive.
 * An negative number means wait forever
 */
pwr_tStatus mh_OutunitSetTimeout(int Timeout);

pwr_tStatus mh_OutunitUpdate();

/** @} */
#if defined __cplusplus
}
#endif
#endif
