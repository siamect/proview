/* 
 * Proview   $Id: MhrEvent.java,v 1.4 2005-09-01 14:57:52 claes Exp $
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

package jpwr.rt;
import java.io.Serializable; 
/**
 *  Description of the Class
 *
 *@author     claes
 *@created    November 26, 2002
 */
public class MhrEvent implements Serializable
{
  public String eventTime;
  public String eventText;
  public String eventName;
  public int eventFlags;
  public int eventStatus;
  public int eventPrio;
  public MhrsEventId eventId;
  public MhrsEventId targetId;
  public int eventType;
  public PwrtObjid object;

  public MhrEvent(String eventText, 
                  String eventName, 
		  String eventTime, 
		  int eventFlags, 
		  int eventPrio, 
		  int eventStatus, 
		  int eventId_nix, 
		  String eventId_birthTime, 
		  int eventId_idx,
		  int targetId_nix, 
		  String targetId_birthTime, 
		  int targetId_idx,
		  int eventType,
		  PwrtObjid object)
  {
    this.eventText = eventText;
    this.eventName = eventName;
    this.eventTime = eventTime;
    this.eventFlags = eventFlags;
    this.eventPrio = eventPrio;
    this.eventStatus = eventStatus;
    this.eventId = new MhrsEventId(eventId_nix, eventId_birthTime, eventId_idx); 
    this.targetId = new MhrsEventId(targetId_nix, targetId_birthTime, targetId_idx);
    this.eventType = eventType;
    this.object = object;
  }
  public MhrEvent getCopy()
  {
    return new MhrEvent(eventText, 
                        eventName, 
		        eventTime, 
		        eventFlags, 
		        eventPrio, 
		        eventStatus, 
		        eventId.nix, 
		        eventId.birthTime, 
		        eventId.idx,
		        targetId.nix, 
		        targetId.birthTime, 
		        targetId.idx,
		        eventType,
		        object);
  }
  public String toString()
  {
    return new String(eventTime + eventText + eventName + eventFlags + eventPrio + eventType);
  }

}

