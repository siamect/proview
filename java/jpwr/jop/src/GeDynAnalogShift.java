/* 
 * Proview   $Id: GeDynAnalogShift.java,v 1.4 2005-11-04 11:40:46 claes Exp $
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
import java.awt.*;
import javax.swing.*;

public class GeDynAnalogShift extends GeDynElem {
  String attribute;

  boolean attrFound;
  PwrtRefId subid;
  int p;
  public int typeId;
  float oldValueF;
  int oldValueI;
  boolean firstScan = true;

  public GeDynAnalogShift( GeDyn dyn, String attribute) {
    super( dyn, GeDyn.mDynType_AnalogShift, GeDyn.mActionType_No);
    this.attribute = attribute;
  }
  public void connect() {
    String attrName = dyn.getAttrName( attribute);
    if ( attrName.compareTo("") != 0) {
      GdhrRefObjectInfo ret = dyn.en.gdh.refObjectInfo( attrName);
      if ( ret.evenSts())
	System.out.println( "AnalogShift: " + attrName);
      else {
	attrFound = true;
	p = ret.id;
	subid = ret.refid;
	typeId = ret.typeId;
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

    switch ( typeId) {
    case Pwr.eType_Float32: {
      float value0 = dyn.en.gdh.getObjectRefInfoFloat( p);
      if ( value0 != oldValueF  || firstScan) {
        dyn.comp.setPage( (int)(value0 + 1.5));
        dyn.repaintNow = true;
        oldValueF = value0;
      }
      break;
    }
    case Pwr.eType_Int32:
    case Pwr.eType_UInt32:
    case Pwr.eType_Int16:
    case Pwr.eType_UInt16:
    case Pwr.eType_Int8:
    case Pwr.eType_UInt8: {
      int value0 = dyn.en.gdh.getObjectRefInfoInt( p);
      if ( value0 != oldValueI  || firstScan) {
        dyn.comp.setPage( value0 + 1);
        dyn.repaintNow = true;
        oldValueI = value0;
      }
      break;
    }
    }
    if ( dyn.repaintNow) {
      ((JComponent)dyn.comp).repaint();
      dyn.repaintNow = false;
    }
    if ( firstScan)
      firstScan = false;
  }
}







