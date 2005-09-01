/* 
 * Proview   $Id: JopTextField.java,v 1.2 2005-09-01 14:57:49 claes Exp $
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


package jpwr.beans;
import jpwr.rt.*;
import jpwr.jop.*;
import java.awt.event.*;
import java.awt.*;
import javax.swing.*;
import javax.swing.Timer;

public class JopTextField extends GeTextField
{
  public JopTextField()
  {
  }

  Color  lowColor = new Color( 175, 175, 175 );

  public void setLowColor( Color lowColor) {
    this.lowColor = lowColor;
  }
  public Color getLowColor() {
    return lowColor;
  }
  public void setAccess( int access) {
    dd.access = access;
  }
  public int getAccess() {
    return dd.access;
  }
  public void setPwrAttribute( String pwrAttribute) { 
    dd.data[0] = pwrAttribute;
  }
  public String getPwrAttribute() {
   return dd.data[0];
  }
  public void setPwrAttrColor( String pwrAttrColor) { 
    dd.data[2] = pwrAttrColor;
  }
  public String getPwrAttrColor() { 
    return dd.data[2];
  }
  public void setFormat( String format) {
    dd.data[1] = format;
  }
  public String getFormat() {
    return dd.data[1];
  }
  public void setMinimumValue( float minValue) {
    this.minValue = minValue;
  }
  public float getMinimumValue() {
    return this.minValue;
  }
  public void setMaximumValue( float maxValue) {
    this.maxValue = maxValue;
  }
  public float getMaximumValue() {
    return this.maxValue;
  }

  public void dynamicOpen() {
    String attrName = dd.getAttrName( dd.data[0]);
    if ( attrName.compareTo("") != 0) {
      ret = en.gdh.refObjectInfo( attrName);
      if ( ret.evenSts())
        System.out.println( "ObjectInfoError " + attrName);
      else {
        dd.attrFound[0] = true;
        dd.p[0] = ret.id;
        dd.subid[0] = ret.refid;
        dd.typeId[0] = ret.typeId;
        dd.setFormat( dd.data[1]);
      }
    }
    if ( dd.data[2] != null) {
      attrName = dd.getAttrName( dd.data[2]);
      if ( attrName.compareTo("") != 0) {
        ret = en.gdh.refObjectInfo( attrName);
        if ( ret.evenSts())
          System.out.println( "ObjectInfoError " + attrName);
        else {
          dd.attrFound[1] = true;
          dd.p[1] = ret.id;
          dd.subid[1] = ret.refid;
          dd.typeId[1] = ret.typeId;
          dd.inverted[1] = GeDyndata.getAttrInverted( dd.data[2]);
        }
      }
    }
    normalColor = getBackground();
    setBackground( normalColor);
  }
  public void dynamicClose() {
    if ( dd.attrFound[0])
      en.gdh.unrefObjectInfo( dd.subid[0]);
    if ( dd.attrFound[1])
      en.gdh.unrefObjectInfo( dd.subid[1]);
  }
  public void dynamicUpdate( boolean animationOnly) {
    if ( animationOnly)
      return;
    if ( !dd.attrFound[0] || focus)
      return;

    boolean repaintNow = false;
    if ( dd.typeId[0] == Pwr.eType_Float32) {
      float value0 = en.gdh.getObjectRefInfoFloat( dd.p[0]);
      if ( value0 != dd.oldValueF[0]) {
        sb = dd.format( value0, sb);
        setText(new String(sb));
        repaintNow = true;
        dd.oldValueF[0] = value0;
      }
    }
    else if ( dd.typeId[0] == Pwr.eType_Int32) {
      int value0 = en.gdh.getObjectRefInfoInt( dd.p[0]);
      if ( value0 != dd.oldValueI) {
        sb = dd.format( value0, sb);
        setText(new String(sb));
        repaintNow = true;
        dd.oldValueI = value0;
      }
    }
    else if ( dd.typeId[0] == Pwr.eType_String) {
      String value0 = en.gdh.getObjectRefInfoString( dd.p[0], dd.typeId[0]);
      if ( value0.compareTo( dd.oldValueS) != 0) {
        sb = dd.format( value0, sb);
        setText(new String(sb));
        repaintNow = true;
        dd.oldValueS = value0;
      }
    }
    if ( dd.attrFound[1]) {
      boolean value1 = en.gdh.getObjectRefInfoBoolean( dd.p[1]);
      if ( value1 != dd.oldValueB[1] || dd.firstScan) {
        if ( (!dd.inverted[1] && value1) || (dd.inverted[1] && !value1))
          setBackground( normalColor);
        else
          setBackground( lowColor);
        repaintNow = true;
        dd.oldValueB[1] = value1;
      }
    }
    if ( repaintNow)
//      repaintForeground();
    if ( dd.firstScan)
      dd.firstScan = false;

//    if ( valueAttr < alarmLimitLow || valueAttr > alarmLimitHigh)
//      setBackground( alarmColor);
//    else
//      setBackground( normalColor);
  }

}
