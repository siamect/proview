/* 
 * Proview   $Id: GeDynRotate.java,v 1.2 2005-09-01 14:57:50 claes Exp $
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
import javax.swing.*;
import jpwr.rt.*;

public class GeDynRotate extends GeDynElem {
  String attribute;
  double x0;
  double y0;
  double factor;

  boolean attrFound;
  PwrtRefId subid;
  int p;
  int typeId;
  float oldValue;
  boolean firstScan = true;

  public GeDynRotate( GeDyn dyn, String attribute, double x0, double y0, double factor) {
    super( dyn, GeDyn.mDynType_Rotate, GeDyn.mActionType_No);
    this.attribute = attribute;
    this.x0 = x0;
    this.y0 = y0;
    this.factor = factor;
  }
  public void connect() {
    String attrName = dyn.getAttrName( attribute);
    if ( attrName.compareTo("") != 0) {
      GdhrRefObjectInfo ret = dyn.en.gdh.refObjectInfo( attrName);
      if ( ret.evenSts())
	System.out.println( "Rotate: " + attrName);
      else {
	attrFound = true;
	p = ret.id;
	subid = ret.refid;
	typeId = ret.typeId;
	dyn.x0 = x0;
	dyn.y0 = y0;
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

    float value0 = dyn.en.gdh.getObjectRefInfoFloat( p);
    if ( value0 != oldValue  || firstScan) {
      dyn.rotate = value0 * factor;
      oldValue = value0;
      // dyn.repaintNow = true;
      ((JComponent)dyn.comp).repaint();
    }
    if ( firstScan)
      firstScan = false;
  }
}













