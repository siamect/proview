/* 
 * Proview   $Id: GeDynDigShift.java,v 1.2 2005-09-01 14:57:50 claes Exp $
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

package jpwr.jop;
import jpwr.rt.*;

public class GeDynDigShift extends GeDynElem {
  String attribute;

  boolean attrFound;
  PwrtRefId subid;
  int p;
  boolean inverted;
  boolean oldValue;
  boolean firstScan = true;

  public GeDynDigShift( GeDyn dyn, String attribute) {
    super( dyn, GeDyn.mDynType_DigShift, GeDyn.mActionType_No);
    this.attribute = attribute;
  }
  public void connect() {
    String attrName = dyn.getAttrName( attribute);
    if ( attrName.compareTo("") != 0) {
      GdhrRefObjectInfo ret = dyn.en.gdh.refObjectInfo( attrName);
      if ( ret.evenSts())
	System.out.println( "DigShift: " + attrName);
      else {
	attrFound = true;
	p = ret.id;
	subid = ret.refid;
	inverted = GeDyndata.getAttrInverted( attribute);
      }
    }
  }
  public void disconnect() {
    if ( attrFound)
      dyn.en.gdh.unrefObjectInfo( subid);
  }
  public void scan() {
    if ( !attrFound)
      return;

    boolean value0 = dyn.en.gdh.getObjectRefInfoBoolean( p);

    if ( firstScan) {
      if ( (!inverted && value0) || (inverted && !value0)) { 
        ((GeComponent)dyn.comp).setLastPage();
        dyn.repaintNow = true;
      }
      oldValue = value0;
      firstScan = false;
    } 

    if ( oldValue != value0) {
      // Shift to first or last page
      if ( (!inverted && value0) || (inverted && !value0))
        dyn.comp.setLastPage();
      else if ( (!inverted && !value0) || (inverted && value0))
        dyn.comp.setFirstPage();
      dyn.repaintNow = true;
    }
    oldValue = value0;
  }
}












