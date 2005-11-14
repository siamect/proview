/* 
 * Proview   $Id: rt_mh_appl.h,v 1.5 2005-11-14 16:29:25 claes Exp $
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

#ifndef rt_mh_appl_h
#define rt_mh_appl_h

/** @addtogroup MSGH */
/*@{*/

#define RS_MH_APPL_VERSION "X2.3.1"

#ifndef pwr_h
#include "pwr.h"
#endif

#ifndef pwr_class_h
#include "pwr_class.h"
#endif

#ifndef rt_mh_h
#include "rt_mh.h"
#endif

#ifndef rt_mh_net_h
#include "rt_mh_net.h"
#endif

#if defined __cplusplus
extern "C" {
#endif


/** @defgroup MSGH_DS Data Structures
 *  @ingroup MSGH
 *  @{
 */

/* Structures */

/**
 * mhs_ApplMessage
 */
typedef struct mhs_ApplMessage mh_sApplMessage;

/**
 * @brief Flags governing how mh interacts with the application.
 * @note Version V1.0 ignores these flags.
 */
typedef enum {
  mh_mApplFlags_NoAbortCleanUp	= (1 << 0), /**< If an application terminates abnormally, 
                                            mh will remove all data connected to it, i. e. 
                                            all active messages will be cancelled. If this flag 
                                            is set this clean-up will be suppressed. This can be 
                                            useful if the application will be restarted, in which 
                                            case it can find out if it has any active messages. */
  mh_mApplFlags_NoAbortMsg	= (1 << 1) /**< Mh usually generates an alarm message if a connected 
                                                application terminates abnormally, setting this flag 
                                                will suppress this message. */
} mh_mApplFlags;

/**
 * @brief The structure describing an application message. 
 */
struct mhs_ApplMessage {
  pwr_tUInt32	    Id;           /**< The identity given to the message, which is used as a handle 
                                       when generating an acknowledgement or return message to the 
                                       message and should be kept by the application for this purpose. 
                                       This attribute is only used when calling mh_ApplGetMsgInfo, 
                                       otherwise the id is supplied/returned as a parameter of its own.*/ 
  pwr_tObjid	    Object;       /**< An object that is associated with the event. When the operator 
                                       marks the message and clicks on "Select", this object will be shown
                                       in the operator window. If you do not want this facility, supply a 
                                       value of 0 and set the flag mh_EventFlags_NoObject in EventFlags.*/ 
  pwr_tObjid	    SupObject;    /**< A supervision object associated with the event. When the operator 
                                       selects the functions "ActionText", he will see the text attached 
                                       to this object. It is, for instance, possible to use the 
                                       application object for this attribute, or zero, in which case no 
                                       text will be associated.*/ 
  pwr_tObjid	    Outunit;      /**< A message can be directed to a specific out-unit by supplying the 
                                       object identity of the unit, if you supply zero the message will be
                                       sent to all out-units. */ 
  mh_mEventFlags    EventFlags;   /**< Flags that modify the properties of a message, See mh_mEventFlags.*/ 
  pwr_tTime	    EventTime;    /**< The time when the event that initiated the creation of the message 
                                       occurred. This must be supplied by the application, mh will not put
                                       a time stamp on the message. */
  pwr_tString80	    EventName;    /**< A name that identifies the event, can for instance be the name of
                                       an object. It will be shown as the first row of a message in the 
                                       alarm window.*/ 
  mh_eEvent	    EventType;    /**< Specifies the kind of message that will be generated, 
				       alarm or informational. See mh_eEvent.*/
  pwr_tString80	    EventText;    /**< An arbitrary string supplying information about the event. 
				     It will be shown as the second row of a message in the alarm window.*/ 
  mh_eEventPrio	    EventPrio;    /**< Sets the priority of the message, See mh_eEventPrio.*/ 
  mh_mEventStatus   EventStatus;  /**< Returns the status of a message, See mh_mEventStatus. 
				     This attribute is only used when calling mh_ApplGetMsgInfo.*/ 
  mh_uSupInfo	    SupInfo;      /**< Supplies the type of supervision data that should be associated 
				     with the message: analog, digital or none. See mh_eSupType.*/ 
  pwr_sAttrRef	    EventSound;    /**< Reference to sound object. Determines the alarm sound. */
};

/**@}*/

/** @defgroup MSGH_FC Function Calls
 *  @ingroup MSGH
 *  @{
 */
/** @defgroup MSG_AR Application routines
 *  @ingroup MSGH_FC
 *  @{
 */

pwr_tStatus mh_ApplCancel (
  pwr_tUInt32	  id,
  pwr_tString80	  *cancelText
);

/*------------------------------------------------------------------------------
* Connects this application to the local Handler.
*/
pwr_tStatus mh_ApplConnect (
  pwr_tObjid	  ApplObject,
  mh_mApplFlags   Flags,
  pwr_tString80   AbortEventName,
  mh_eEvent	  AbortEventType,
  mh_eEventPrio   AbortEventPrio,
  mh_mEventFlags  AbortEventFlags,
  pwr_tString80   AbortEventText,
  pwr_tUInt32	  *NoOfActMessages
);

/*------------------------------------------------------------------------------
* Informs the local Handler to remove this Application from its list of known
* applications.
*/
pwr_tStatus mh_ApplDisconnect (
);

/*------------------------------------------------------------------------------
* Send a message to the local Handler.
*/
pwr_tStatus mh_ApplMessage (
  pwr_tUInt32	  *Id,
  mh_sApplMessage *Message
);

/*------------------------------------------------------------------------------
* This function returnes a message previously created by this application.
*/
pwr_tStatus mh_ApplReturn (
  pwr_tUInt32	  Id,
  pwr_tString80   *ReturnText
);

/*------------------------------------------------------------------------------
*/
pwr_tStatus mh_ApplGetMsgInfo (
  pwr_tUInt32	    Id,
  mh_sApplMessage   *Message
);

/**@}*/
/**@}*/
/**@}*/

#if defined __cplusplus
}
#endif
#endif
