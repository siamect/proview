/* 
 * Proview   $Id: Hist.java,v 1.2 2005-09-01 14:57:52 claes Exp $
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

/**
 *  Title: Hist.java Description: Klass som fungerar som en port mot
 *  Historiska Händelselistan Copyright: <p>
 *
 *  Company SSAB<p>
 *
 *
 *
 *@author     JN
 *@version    1.0
 */

package jpwr.rt;
/**
 *  Description of the Class
 *
 *@author     Jonas Nylund
 *@created    July 5, 2004
 */
public class Hist
{
  static
  {
    System.loadLibrary("jpwr_rt_gdh");
    initHistIDs();  
  }

  /**
   *@author                            claes
   *@created                           November 26, 2002
   *@ingroup                           MSGH_DS
   *@brief                             Defines a bit pattern.
   *@param  mh_mEventFlags_Return      Setting this flag enables a return
   *      message associated with this message to be shown in the event list.
   *@param  mh_mEventFlags_Ack         Setting this flag enables an
   *      acknowledgement message associated with this message to be shown in
   *      the event list.
   *@param  mh_mEventFlags_Bell
   *@param  mh_mEventFlags_Force
   *@param  mh_mEventFlags_InfoWindow
   *@param  mh_mEventFlags_Returned
   *@param  mh_mEventFlags_NoObject
   */
  public static final int mh_mEventFlags_Return = 0x01;
  public static final int mh_mEventFlags_Ack = 0x02;
  public static final int mh_mEventFlags_Bell = 0x04;
  public static final int mh_mEventFlags_Force = 0x08;
  public static final int mh_mEventFlags_InfoWindow = 0x10;
  public static final int mh_mEventFlags_Returned = 0x20;
  public static final int mh_mEventFlags_NoObject = 0x40;
  
  
  public static final  int mh_mEventStatus_NotRet = (1 << 0);
  public static final  int mh_mEventStatus_NotAck = (1 << 1);
  public static final  int mh_mEventStatus_Block  = (1 << 2);
  


 /** 
 * @ingroup MSGH_DS
 * @brief Event prio
 *
 * This enumeration defines the priority of the event. 
 * This affects how the message handler treats the generated message. 
 * For A and B priorities the alarm window displays number of alarms, 
 * number of unacknowledged alarms, identities of the alarms, and associated 
 * message texts. For C and D priorities, only number of alarms and number of 
 * unacknowledged alarms are shown. 
 * @param mh_eEventPrio_A Priority A, the highest priority. 
 * Alarm messages of this priority are shown in the upper part of the alarm window. 
 * @param mh_eEventPrio_B Priority B. 
 * These messages are shown in the lower part of the alarm window. 
 * @param mh_eEventPrio_C Priority C. 
 * @param mh_eEventPrio_D Priority D. This is the lowest priority. 
 */
  public static final int mh_eEventPrio__ = 0;   
  public static final int mh_eEventPrio_A = 67;  
  public static final int mh_eEventPrio_B = 66; 
  public static final int mh_eEventPrio_C = 65;
  public static final int mh_eEventPrio_D = 64;
  public static final int mh_eEventPrio_  = 63;
  
  
  public static final int mh_eEvent__		= 0;    
  public static final int mh_eEvent_Ack		= 1;
  public static final int mh_eEvent_Block	= 2;
  public static final int mh_eEvent_Cancel	= 3;
  public static final int mh_eEvent_CancelBlock	= 4;
  public static final int mh_eEvent_Missing	= 5;
  public static final int mh_eEvent_Reblock	= 6;
  public static final int mh_eEvent_Return	= 7;
  public static final int mh_eEvent_Unblock	= 8;
  public static final int mh_eEvent_Info	= 32;
  public static final int mh_eEvent_Alarm	= 64;
  public static final int mh_eEvent_   		= 65;
  
  public static final int EventType_ClearAlarmList = 66;

  private static boolean initDone = false;

  /**
   *  Constructor for the Hist object
   *
   *@param  root  Description of the Parameter
   */
  public Hist()
  {
    
    if(!initDone)
    {
      initDone = true;
    }
  }

  private native static void initHistIDs();

  /**
   *  Description of the Method
   *
   *@param  query     The query to the HistDB
   *@return Vector    A Vector containing matching events
   */

  public native static MhrEvent[] getHistList(String startTime,
					  String stopTime,
					  boolean typeAlarm,
					  boolean typeInfo,
					  boolean typeReturn,
					  boolean typeAck,
					  boolean prioA,
					  boolean prioB,
					  boolean prioC,
					  boolean prioD,
					  String name,
					  String text);
}




