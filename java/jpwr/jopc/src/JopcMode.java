/* 
 * Proview   $Id: JopcMode.java,v 1.2 2005-09-01 14:57:51 claes Exp $
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

package jpwr.jopc;
import jpwr.rt.*;
import jpwr.jop.*;
import java.awt.*;
import java.awt.geom.*;
import java.awt.image.*;
import java.awt.font.*;
import javax.swing.*;
import java.awt.event.*;

public class JopcMode extends JopcModeGen implements JopDynamic {
  int accMode = 0;
  int pManMode;
  int pAutoMode;
  int pCascadeMode;
  boolean setSlider = false;
  int pSetSlider;
  boolean outSlider = false;
  int pOutSlider;
  float setMaxShowOld = 0;
  float setMinShowOld = 0;
  int pSetMaxShow;
  int pSetMinShow;
  PwrtRefId setMaxShowSubid;
  PwrtRefId setMinShowSubid;
  float outMaxShowOld = 0;
  float outMinShowOld = 0;
  int pOutMaxShow;
  int pOutMinShow;
  PwrtRefId outMaxShowSubid;
  PwrtRefId outMinShowSubid;
  LocalDb ldb;
  public JopcMode( JopSession session, String instance, boolean scrollbar) {
    super( session, instance, scrollbar, true);

    // engine = new JopEngine( 1000, session.getRoot());
    // ldb = new LocalDb();
    // engine.setLocalDb(ldb);
    geInit();

    engine.add(this);

  }
  public Object dynamicGetRoot() {
    return this;
  }
  GeDyn dd;
  public void dynamicOpen() {
    GdhrRefObjectInfo ret;
    ret = engine.ldb.refObjectInfo( this, "$local.SetSliderDisable##Boolean");
    if ( ret.oddSts())
      pSetSlider = ret.id;
    else
      System.out.println("$local.SetSlider not found");

    ret = engine.ldb.refObjectInfo( this, "$local.OutSliderDisable##Boolean");
    if ( ret.oddSts())
      pOutSlider = ret.id;
    else
      System.out.println("$local.OutSlider not found");

    ret = engine.ldb.refObjectInfo( this, "$local.ManMode##Boolean");
    if ( ret.oddSts())
      pManMode = ret.id;
    else
      System.out.println("$local.ManMode not found");

    ret = engine.ldb.refObjectInfo( this, "$local.AutoMode##Boolean");
    if ( ret.oddSts())
      pAutoMode = ret.id;
    else
      System.out.println("$local.AutoMode not found");

    ret = engine.ldb.refObjectInfo( this, "$local.CascadeMode##Boolean");
    if ( ret.oddSts())
      pCascadeMode = ret.id;
    else
      System.out.println("$local.CascadeMode not found");

    dd = new GeDyn( null);
    dd.setSession( session);
    dd.setInstance( engine.getInstance()); 

    String attrName = dd.getAttrName( "$object.SetMaxShow##Float32");
    ret = engine.gdh.refObjectInfo( attrName);
    if ( ret.evenSts())
      System.out.println( "JopcAv: " + attrName);
    else {
      setMaxShowSubid = ret.refid;
      pSetMaxShow = ret.id;
    }

    attrName = dd.getAttrName( "$object.SetMinShow##Float32");
    ret = engine.gdh.refObjectInfo( attrName);
    if ( ret.evenSts())
      System.out.println( "JopcAv: " + attrName);
    else {
      setMinShowSubid = ret.refid;
      pSetMinShow = ret.id;
    }

    attrName = dd.getAttrName( "$object.OutMaxShow##Float32");
    ret = engine.gdh.refObjectInfo( attrName);
    if ( ret.evenSts())
      System.out.println( "JopcAv: " + attrName);
    else {
      outMaxShowSubid = ret.refid;
      pOutMaxShow = ret.id;
    }

    attrName = dd.getAttrName( "$object.OutMinShow##Float32");
    ret = engine.gdh.refObjectInfo( attrName);
    if ( ret.evenSts())
      System.out.println( "JopcAv: " + attrName);
    else {
      outMinShowSubid = ret.refid;
      pOutMinShow = ret.id;
    }

    attrName = dd.getAttrName( "$object.AccMod##Int32");
    CdhrInt reti = engine.gdh.getObjectInfoInt( attrName);
    if ( reti.evenSts())
      System.out.println( "JopcMode: AccMode");
    else
      accMode = reti.value;

    pwr_slider110.setActionDisabled(true);
    pwr_slider111.setActionDisabled(true);
  }
  public void dynamicClose() {
    engine.gdh.unrefObjectInfo( setMaxShowSubid);
    engine.gdh.unrefObjectInfo( setMinShowSubid);
    engine.gdh.unrefObjectInfo( outMaxShowSubid);
    engine.gdh.unrefObjectInfo( outMinShowSubid);
    engine.ldb.unrefObjectInfo( pSetSlider);
    engine.ldb.unrefObjectInfo( pOutSlider);
  }
  public void dynamicUpdate( boolean animationOnly) {
    if ( animationOnly)
      return;

    boolean setSliderValue = engine.ldb.getObjectRefInfoBoolean( pSetSlider);
    boolean outSliderValue = engine.ldb.getObjectRefInfoBoolean( pOutSlider);
    boolean manModeValue = engine.ldb.getObjectRefInfoBoolean( pManMode);
    boolean autoModeValue = engine.ldb.getObjectRefInfoBoolean( pAutoMode);
    boolean cascadeModeValue = engine.ldb.getObjectRefInfoBoolean( pCascadeMode);
    float setMinShow = engine.gdh.getObjectRefInfoFloat( pSetMinShow);
    float setMaxShow = engine.gdh.getObjectRefInfoFloat( pSetMaxShow);
    float outMinShow = engine.gdh.getObjectRefInfoFloat( pOutMinShow);
    float outMaxShow = engine.gdh.getObjectRefInfoFloat( pOutMaxShow);
    if ( engine.gdh.isAuthorized( jopButtontoggle15.dd.access) ) {
      if ( setSliderValue) {
	engine.ldb.setObjectInfo( this, "$local.SetSliderDisable##Boolean", false);
	setSlider = !setSlider;
	pwr_slider110.setActionDisabled(!setSlider);
	if ( setSlider)
	  jopButtontoggle15.tsetFillColor( GeColor.COLOR_115);
	else
	  jopButtontoggle15.resetFillColor();
	jopButtontoggle15.repaintForeground();
      }
    }
    if ( engine.gdh.isAuthorized( jopButtontoggle16.dd.access) ) {
      if ( outSliderValue) {
	engine.ldb.setObjectInfo( this, "$local.OutSliderDisable##Boolean", false);
	outSlider = !outSlider;
	pwr_slider111.setActionDisabled(!outSlider);
	if ( outSlider)
	  jopButtontoggle16.tsetFillColor( GeColor.COLOR_115);
	else
	  jopButtontoggle16.resetFillColor();
	jopButtontoggle16.repaintForeground();
      }
    }
    if ( manModeValue) {
      engine.ldb.setObjectInfo( this, "$local.ManMode##Boolean", false);
      if ( (accMode & 1) != 0) {
	String attrName = dd.getAttrName( "$object.OpMod##Int32");
	PwrtStatus rsts = engine.gdh.setObjectInfo( attrName, 1);
	if ( rsts.evenSts())
	  System.out.println("Error set OpMod");
      }
    }
    else if ( autoModeValue) {
      engine.ldb.setObjectInfo( this, "$local.AutoMode##Boolean", false);
      if ( (accMode & 2) != 0) {
	String attrName = dd.getAttrName( "$object.OpMod##Int32");
	PwrtStatus rsts = engine.gdh.setObjectInfo( attrName, 2);
	if ( rsts.evenSts())
	  System.out.println("Error set OpMod");
      }
    }
    else if ( cascadeModeValue) {
      engine.ldb.setObjectInfo( this, "$local.CascadeMode##Boolean", false);
      if ( (accMode & 4) != 0) {
	String attrName = dd.getAttrName( "$object.OpMod##Int32");
	PwrtStatus rsts = engine.gdh.setObjectInfo( attrName, 4);
	if ( rsts.evenSts())
	  System.out.println("Error set OpMod");
      }
    }
    if ( setMaxShow != setMaxShowOld || setMinShow != setMinShowOld) {
      jopBar12.setMinValue(setMinShow);
      jopBar12.setMaxValue(setMaxShow);
      jopBar12.update();
      jopBar13.setMinValue(setMinShow);
      jopBar13.setMaxValue(setMaxShow);
      jopBar13.update();

      GeDyn dyn = pwr_slider110.dd;
      for ( int i = 0; i < dyn.elements.length; i++) {
	if ( dyn.elements[i].getActionType() == GeDyn.mActionType_Slider) {
	  GeDynSlider elem = (GeDynSlider)dyn.elements[i];
	  elem.setMinValue( setMinShow);
	  elem.setMaxValue( setMaxShow);
	  elem.update();
	}
      }
      dyn = pwr_slider111.dd;
      for ( int i = 0; i < dyn.elements.length; i++) {
	if ( dyn.elements[i].getActionType() == GeDyn.mActionType_Slider) {
	  GeDynSlider elem = (GeDynSlider)dyn.elements[i];
	  elem.setMinValue( setMinShow);
	  elem.setMaxValue( setMaxShow);
	  elem.update();
	}
      }

      setMaxShowOld = setMaxShow;
      setMinShowOld = setMinShow;
    }
    if ( outMaxShow != outMaxShowOld || outMinShow != outMinShowOld) {
      jopBar14.setMinValue(outMinShow);
      jopBar14.setMaxValue(outMaxShow);
      jopBar14.update();

      GeDyn dyn = pwr_slider111.dd;
      for ( int i = 0; i < dyn.elements.length; i++) {
	if ( dyn.elements[i].getActionType() == GeDyn.mActionType_Slider) {
	  GeDynSlider elem = (GeDynSlider)dyn.elements[i];
	  elem.setMinValue( outMinShow);
	  elem.setMaxValue( outMaxShow);
	  elem.update();
	}
      }

      outMaxShowOld = outMaxShow;
      outMinShowOld = outMinShow;
    }
    
  }
}







