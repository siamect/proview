/* 
 * Proview   $Id: GeDynXYCurve.java,v 1.2 2007-09-19 15:08:35 claes Exp $
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
import java.awt.*;
import java.awt.geom.*;
import java.awt.image.*;
import java.awt.font.*;
import javax.swing.*;
import jpwr.rt.*;

public class GeDynXYCurve extends GeDynElem {
  String x_attr;
  String y_attr;
  String y_minvalue_attr;
  String y_maxvalue_attr;
  String x_minvalue_attr;
  String x_maxvalue_attr;
  String noofpoints_attr;
  String update_attr;
  float y_min_value;
  float y_max_value;
  float x_min_value;
  float x_max_value;
  int datatype;
  public int curve_color;
  public int fill_color;
  public int fill_curve;
  int noofpoints;
  int noOfPoints;

  int xAttrType;
  int yAttrType;
  public float[] curveX;
  public float[] curveY;
  boolean updateAttrFound;
  PwrtRefId updateSubid;
  int updateP;
  boolean updateOldValue;
  boolean noofpointsAttrFound;
  PwrtRefId noofpointsSubid;
  int noofpointsP;
  int noofpointsOldValue;
  boolean yMinvalueAttrFound;
  PwrtRefId yMinvalueSubid;
  int yMinvalueP;
  boolean yMaxvalueAttrFound;
  PwrtRefId yMaxvalueSubid;
  int yMaxvalueP;
  boolean xMinvalueAttrFound;
  PwrtRefId xMinvalueSubid;
  int xMinvalueP;
  boolean xMaxvalueAttrFound;
  PwrtRefId xMaxvalueSubid;
  int xMaxvalueP;

  boolean firstScan = true;

  public GeDynXYCurve( GeDyn dyn, String x_attr, String y_attr, String y_minvalue_attr,
		       String y_maxvalue_attr, String x_minvalue_attr, String x_maxvalue_attr,
		       String noofpoints_attr, String update_attr, float y_min_value, 
		       float y_max_value, float x_min_value, float x_max_value, 
		       int noofpoints, int datatype, int curve_color, int fill_color, 
		       int fill_curve) {
    super( dyn, GeDyn.mDynType_XY_Curve, GeDyn.mActionType_No);
    this.x_attr = x_attr;
    this.y_attr = y_attr;
    this.y_minvalue_attr = y_minvalue_attr;
    this.y_maxvalue_attr = y_maxvalue_attr;
    this.x_minvalue_attr = x_minvalue_attr;
    this.x_maxvalue_attr = x_maxvalue_attr;
    this.noofpoints_attr = noofpoints_attr;
    this.update_attr = update_attr;
    this.y_min_value = y_min_value;
    this.y_max_value = y_max_value;
    this.x_min_value = x_min_value;
    this.x_max_value = x_max_value;
    this.datatype = datatype;
    this.curve_color = curve_color;
    this.fill_color = fill_color;
    this.noofpoints = noofpoints;
    this.fill_curve = fill_curve;
  }
  public void connect() {
    xAttrType = GeDyn.getTypeId( x_attr);
    switch ( datatype) {
    case GeDyn.eCurveDataType_XYArrays:
      yAttrType = GeDyn.getTypeId( y_attr);
      break;
    }

    String attrName = dyn.getAttrName( update_attr);
    if ( attrName.compareTo("") != 0) {
      GdhrRefObjectInfo ret = dyn.en.gdh.refObjectInfo( attrName);
      if ( ret.evenSts())
	System.out.println( "XYCurve: " + attrName);
      else {
	updateAttrFound = true;
	updateP = ret.id;
	updateSubid = ret.refid;
      }
    }

    attrName = dyn.getAttrName( noofpoints_attr);
    if ( attrName.compareTo("") != 0) {
      GdhrRefObjectInfo ret = dyn.en.gdh.refObjectInfo( attrName);
      if ( ret.evenSts())
	System.out.println( "XYCurve: " + attrName);
      else {
	noofpointsAttrFound = true;
	noofpointsP = ret.id;
	noofpointsSubid = ret.refid;
      }
    }

    attrName = dyn.getAttrName( y_minvalue_attr);
    if ( attrName.compareTo("") != 0) {
      GdhrRefObjectInfo ret = dyn.en.gdh.refObjectInfo( attrName);
      if ( ret.evenSts())
	System.out.println( "XYCurve: " + attrName);
      else {
	yMinvalueAttrFound = true;
	yMinvalueP = ret.id;
	yMinvalueSubid = ret.refid;
      }
    }

    attrName = dyn.getAttrName( y_maxvalue_attr);
    if ( attrName.compareTo("") != 0) {
      GdhrRefObjectInfo ret = dyn.en.gdh.refObjectInfo( attrName);
      if ( ret.evenSts())
	System.out.println( "XYCurve: " + attrName);
      else {
	yMaxvalueAttrFound = true;
	yMaxvalueP = ret.id;
	yMaxvalueSubid = ret.refid;
      }
    }

    attrName = dyn.getAttrName( x_minvalue_attr);
    if ( attrName.compareTo("") != 0) {
      GdhrRefObjectInfo ret = dyn.en.gdh.refObjectInfo( attrName);
      if ( ret.evenSts())
	System.out.println( "XYCurve: " + attrName);
      else {
	xMinvalueAttrFound = true;
	xMinvalueP = ret.id;
	xMinvalueSubid = ret.refid;
      }
    }

    attrName = dyn.getAttrName( x_maxvalue_attr);
    if ( attrName.compareTo("") != 0) {
      GdhrRefObjectInfo ret = dyn.en.gdh.refObjectInfo( attrName);
      if ( ret.evenSts())
	System.out.println( "XYCurve: " + attrName);
      else {
	xMaxvalueAttrFound = true;
	xMaxvalueP = ret.id;
	xMaxvalueSubid = ret.refid;
      }
    }
  }
  public void disconnect() {
    if ( updateAttrFound)
      dyn.en.gdh.unrefObjectInfo( updateSubid);
    if ( noofpointsAttrFound)
      dyn.en.gdh.unrefObjectInfo( noofpointsSubid);
    if ( yMinvalueAttrFound)
      dyn.en.gdh.unrefObjectInfo( yMinvalueSubid);
    if ( yMaxvalueAttrFound)
      dyn.en.gdh.unrefObjectInfo( yMaxvalueSubid);
    if ( xMinvalueAttrFound)
      dyn.en.gdh.unrefObjectInfo( xMinvalueSubid);
    if ( xMaxvalueAttrFound)
      dyn.en.gdh.unrefObjectInfo( xMaxvalueSubid);
  }
  public void scan() {
    String attrName;
    int attrSize;
    boolean update = false;
    int size = 1;
    noOfPoints = noofpoints;

    if ( firstScan)
      update = true;

    if ( updateAttrFound) {
      boolean value = dyn.en.gdh.getObjectRefInfoBoolean( updateP);
      if ( value && !updateOldValue)
	update = true;
      updateOldValue = value;
    }

    if ( noofpointsAttrFound) {
      int value = dyn.en.gdh.getObjectRefInfoInt( updateP);
      if ( value != noofpointsOldValue) {
	update = true;
	noOfPoints = value;
	noofpointsOldValue = value;
      }
    }

    if ( yMinvalueAttrFound) {
      float value = dyn.en.gdh.getObjectRefInfoFloat( yMinvalueP);
      if ( value != y_min_value) {
	y_min_value = value;
	update = true;
      }
    }
    if ( yMaxvalueAttrFound) {
      float value = dyn.en.gdh.getObjectRefInfoFloat( yMaxvalueP);
      if ( value != y_max_value) {
	y_max_value = value;
	update = true;
      }
    }

    if ( xMinvalueAttrFound) {
      float value = dyn.en.gdh.getObjectRefInfoFloat( xMinvalueP);
      if ( value != x_min_value) {
	x_min_value = value;
	update = true;
      }
    }
    if ( xMaxvalueAttrFound) {
      float value = dyn.en.gdh.getObjectRefInfoFloat( xMaxvalueP);
      if ( value != x_max_value) {
	x_max_value = value;
	update = true;
      }
    }

    if ( update) {
      attrName = dyn.getAttrName( x_attr);
      attrSize = GeDyn.getAttrSize( x_attr);
      if ( attrSize == -1) return;

      switch ( datatype) {
      case GeDyn.eCurveDataType_XYArrays:
	if ( attrSize < noOfPoints)
	  noOfPoints = attrSize;
	size = noOfPoints;
	break;
      case GeDyn.eCurveDataType_PointArray:
	if ( attrSize/2 < noOfPoints)
	  noOfPoints = attrSize/2;
	size = noOfPoints * 2;
	break;
      case GeDyn.eCurveDataType_TableObject:
	if ( (attrSize-1)/2 < noOfPoints)
	  noOfPoints = (attrSize-1)/2;
	size = noOfPoints * 2 + 1;
	break;
      }
      
      // Read x-array
      CdhrFloatArray rxf;
      CdhrIntArray rxi;
      switch ( xAttrType) {
      case Pwr.eType_Float32:
	rxf = dyn.en.gdh.getObjectInfoFloatArray( attrName, size);
	if ( rxf.evenSts())
	  return;

	switch ( datatype) {
	case GeDyn.eCurveDataType_XYArrays:
	  curveX = new float[noOfPoints];
	  for ( int i = 0; i < noOfPoints; i++)
	    curveX[i] = (rxf.value[i] - x_min_value) / ( x_max_value - x_min_value);
	  break;
	case GeDyn.eCurveDataType_PointArray:
	  curveX = new float[noOfPoints];
	  curveY = new float[noOfPoints];
	  for ( int i = 0; i < noOfPoints; i++) {
	    curveX[i] = (rxf.value[2*i] - x_min_value) / ( x_max_value - x_min_value);
	    curveY[i] = (rxf.value[2*i+1] - y_min_value) / ( y_max_value - y_min_value);
	  }
	  dyn.repaintNow = true;
	  break;
	case GeDyn.eCurveDataType_TableObject:
	  noOfPoints = (int)rxf.value[0];
	  if ( noOfPoints > noofpoints)
	    noOfPoints = noofpoints;
	  if ( attrSize < noOfPoints)
	    noOfPoints = attrSize;
	  curveY = new float[noOfPoints];
	  curveX = new float[noOfPoints];
	  for ( int i = 0; i < noOfPoints; i++) {
	    curveX[i] = (rxf.value[2*i+1] - x_min_value) / ( x_max_value - x_min_value);
	    curveY[i] = (rxf.value[2*i+2] - y_min_value) / ( y_max_value - y_min_value);
	  }
	  dyn.repaintNow = true;
	  break;
	}
	break;
      case Pwr.eType_Int32:
      case Pwr.eType_Int16:
      case Pwr.eType_Int8:
      case Pwr.eType_UInt32:
      case Pwr.eType_UInt16:
      case Pwr.eType_UInt8:
	rxi = dyn.en.gdh.getObjectInfoIntArray( attrName, size);
	if ( rxi.evenSts())
	  return;

	switch ( datatype) {
	case GeDyn.eCurveDataType_XYArrays:
	  curveX = new float[noOfPoints];
	  for ( int i = 0; i < noOfPoints; i++)
	    curveX[i] = ((float)rxi.value[i] - x_min_value) / ( x_max_value - x_min_value);
	  break;
	case GeDyn.eCurveDataType_PointArray:
	  curveX = new float[noOfPoints];
	  curveY = new float[noOfPoints];
	  for ( int i = 0; i < noOfPoints; i++) {
	    curveX[i] = ((float)rxi.value[2*i] - x_min_value) / ( x_max_value - x_min_value);
	    curveY[i] = ((float)rxi.value[2*i+1] - y_min_value) / ( y_max_value - y_min_value);
	  }
	  dyn.repaintNow = true;
	  break;
	case GeDyn.eCurveDataType_TableObject:
	  noOfPoints = (int)rxi.value[0];
	  if ( noOfPoints > noofpoints)
	    noOfPoints = noofpoints;
	  if ( attrSize < noOfPoints)
	    noOfPoints = attrSize;
	  curveY = new float[noOfPoints];
	  curveX = new float[noOfPoints];
	  for ( int i = 0; i < noOfPoints; i++) {
	    curveX[i] = ((float)rxi.value[2*i+1] - x_min_value) / ( x_max_value - x_min_value);
	    curveY[i] = ((float)rxi.value[2*i+2] - y_min_value) / ( y_max_value - y_min_value);
	  }
	  dyn.repaintNow = true;
	  break;
	}
	break;
      default:
	return;
      }

      // Read y-array
      switch ( datatype) {
      case GeDyn.eCurveDataType_XYArrays:
	attrName = dyn.getAttrName( y_attr);
	attrSize = GeDyn.getAttrSize( y_attr);
	if ( attrSize < noOfPoints)
	  noOfPoints = attrSize;

	curveY = new float[noOfPoints];

	CdhrFloatArray ryf;
	CdhrIntArray ryi;
	switch ( yAttrType) {
	case Pwr.eType_Float32:
	  ryf = dyn.en.gdh.getObjectInfoFloatArray( attrName, noOfPoints);
	  if ( ryf.evenSts())
	    return;

	  for ( int i = 0; i < noOfPoints; i++)
	    curveY[i] = (ryf.value[i] - y_min_value) / ( y_max_value - y_min_value);
	  dyn.repaintNow = true;
	  break;
	case Pwr.eType_Int32:
	case Pwr.eType_Int16:
	case Pwr.eType_Int8:
	case Pwr.eType_UInt32:
	case Pwr.eType_UInt16:
	case Pwr.eType_UInt8:
	  ryi = dyn.en.gdh.getObjectInfoIntArray( attrName, noOfPoints);
	  if ( ryi.evenSts())
	    return;

	  for ( int i = 0; i < noOfPoints; i++)
	    curveY[i] = ((float)ryi.value[i] - y_min_value) / ( y_max_value - y_min_value);
	  
	  dyn.repaintNow = true;

	  break;
	default:
	  return;
	}
	break;
      }

    }
    firstScan = false;
  }
}









