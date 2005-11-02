/* 
 * Proview   $Id: JopSessionRep.java,v 1.6 2005-11-02 14:02:18 claes Exp $
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
import java.lang.*;
import java.util.*;
import java.awt.*;
import javax.swing.*;

public class JopSessionRep implements JopSessionIfc {
  JopEngine engine;
  Object root;
  JopSession session;
  XttTree navigator;
  MhFrame alarmList;
  JopLoginFrame login;
  Vector frames = new Vector();

  public JopSessionRep() {
  }

  public void setSession( Object session) {
    this.session = (JopSession) session;
  }
  public void setRoot( Object root) {
    this.root = root;
  }
  public Object getRoot() {
    return root;
  }
  public void setEngine( JopEngine engine) {
    this.engine = engine;
  }
  public JopEngine getEngine() {
    return engine;
  }

  public void openNavigator( PwrtObjid oid) {
    if ( navigator == null) {
      JopXttFrame xtt = new JopXttFrame( session, oid);
      xtt.setVisible( true);
    }
    else {
      if ( oid != null)
	navigator.find( oid);
	JopXttFrame xtt = (JopXttFrame) navigator.getRoot();
        xtt.setVisible( true);
        xtt.setState(Frame.NORMAL);
        xtt.toFront();
        // xtt.requestFocus();      // Has no effect... 
    }
  }

  public void openAlarmList() {
    if ( alarmList == null) {
      alarmList = new MhFrame(session);
      alarmList.setVisible( true);
    }
    else {
      alarmList.setVisible( true);
      alarmList.setState(Frame.NORMAL);
      alarmList.toFront();
      // alarmList.requestFocus();      // Has no effect... 
    }
  }

  public void openLogin() {
    if ( login == null) {
      login = new JopLoginFrame( session);
      login.setVisible( true);
    }
    else {
      login.setVisible( true);
      login.setState(Frame.NORMAL);
      login.toFront();
      // login.requestFocus();      // Has no effect... 
    }
  }

  public void openFlowFrame( PwrtObjid oid, String center) {
    FlowFrame flow = (FlowFrame) getUtility( JopUtility.TRACE, oid, null);
    if ( flow != null) {
      flow.centerObject( center);
      flow.setState(Frame.NORMAL);
      flow.toFront();
      // flow.requestFocus();      // Has no effect... 
    }
    else {
      flow = new FlowFrame( session, oid, center);
      flow.setVisible( true);
      addUtility( flow);
      System.out.println( "Add utility flow: " + flow + " " + oid.oix + " " + oid.vid);
    }
  }

  public void openGraphFrame( String name, String instance, boolean scrollbar, boolean classGraph) {
    CdhrAttrRef aref = null;
    if ( instance != null) {
      if ( instance.startsWith("&")) {
	// Objid pointer attribute
        CdhrString cstr = engine.gdh.getObjectInfoString( instance.substring(1));
        if ( cstr.evenSts()) return;	  

	instance = cstr.str;
      }

      JopLog.log("openGraphFrame");
      aref = engine.gdh.nameToAttrRef( instance);
      if ( aref.evenSts()) return;

      if ( classGraph) {
	CdhrTypeId tid = engine.gdh.getAttrRefTid( aref.aref);
	if ( tid.evenSts()) return;

        CdhrObjid coid = engine.gdh.classIdToObjid( tid.typeId);
	if ( coid.evenSts()) return;

	CdhrString sret = engine.gdh.objidToName( coid.objid, Cdh.mName_object);
	if ( sret.evenSts()) return;

	// Find any GraphConfiguration suffix
	String suffix = "";
	CdhrInt gcret = engine.gdh.getObjectInfoInt( instance + ".GraphConfiguration");
	if ( gcret.oddSts()) {
	  if ( gcret.value > 0)
	    suffix = Integer.toString(gcret.value);
	}
	if ( coid.objid.vid < Cdh.cUserClassVolMin ||
	    (coid.objid.vid >= Cdh.cManufactClassVolMin && 
	     coid.objid.vid <= Cdh.cManufactClassVolMax)) {
	  // Class is a base class, java classname starts with JopC
	  if ( coid.objid.vid == 1)
	    name = "jpwr.jopc.Jopc" + sret.str.substring(1,2).toUpperCase() + 
	      sret.str.substring(2).toLowerCase() + suffix;
	  else
	    name = "jpwr.jopc.Jopc" + sret.str.substring(0,1).toUpperCase() + 
	      sret.str.substring(1).toLowerCase() + suffix;
	}
	else
	  // Java name equals class name
	  name = sret.str.substring(0,1).toUpperCase() + sret.str.substring(1).toLowerCase() +
	      suffix;
	JopLog.log("openGraphFrame classgraph " + name);
      }
    }
    Object graph;
    if ( aref == null)
      graph = getUtility( JopUtility.GRAPH, (PwrtObjid)null, name);
    else
      graph = getUtility( JopUtility.GRAPH, aref.aref, name);
    if ( graph != null) {
      ((JFrame)graph).setState(Frame.NORMAL);
      ((JFrame)graph).toFront();
      // ((JFrame)graph).requestFocus();  // Has no effect...
    }
    else {
      graph = JopSpider.loadFrame( session, name, instance, scrollbar);
      if ( graph != null) {
        addUtility( graph);
        System.out.println( "Add utility graph " + name);
      }
    }
  }

  public void openCrrFrame( String name) {
    CrrFrame crr = new CrrFrame( session, name);;
    crr.setVisible( true);
  }
  
  public void setNavigator( Object navigator) {
    this.navigator = (XttTree) navigator;
  }

  public void setAlarmList( Object alarmList) {
    this.alarmList = (MhFrame) alarmList;
  }

  public void addUtility( Object utility) {
    frames.add( utility);
  }

  public void removeUtility( Object utility) {
    frames.remove( utility);
  }

  public Object getUtility( int type, PwrtObjid objid, String name) {
    switch ( type) {
    case JopUtility.TRACE:
      for ( int i = 0; i < frames.size(); i++) {
	Object o = frames.get(i);
	PwrtObjid utilityObjid = ((JopUtilityIfc) o).getUtilityObjid();
	if ( ((JopUtilityIfc) o).getUtilityType() == type &&
	     utilityObjid.oix == objid.oix && utilityObjid.vid == objid.vid) {
	  return o;
	}
      }
      return null;
    case JopUtility.GRAPH:
      for ( int i = 0; i < frames.size(); i++) {
	Object o = frames.get(i);
	String utilityName = ((JopUtilityIfc) o).getUtilityName();
        if ( objid != null) {
	  PwrtObjid utilityObjid = ((JopUtilityIfc) o).getUtilityObjid();
	  if ( ((JopUtilityIfc) o).getUtilityType() == type &&
	       utilityName.equals( name) &&
	       utilityObjid != null &&
	       utilityObjid.oix == objid.oix && utilityObjid.vid == objid.vid)
	    return o;    
        }
        else {
	  if ( ((JopUtilityIfc) o).getUtilityType() == type &&
	       utilityName.equals( name))
	    return o;    
	}
      }
      return null;
    default:
      return null;
    }

  }

  public Object getUtility( int type, PwrtAttrRef aref, String name) {
    switch ( type) {
    case JopUtility.TRACE:
      return null;
    case JopUtility.GRAPH:
      for ( int i = 0; i < frames.size(); i++) {
	Object o = frames.get(i);
	String utilityName = ((JopUtilityIfc) o).getUtilityName();
        if ( aref != null) {
	  PwrtAttrRef utilityAref = ((JopUtilityIfc) o).getUtilityAttrRef();
	  if ( ((JopUtilityIfc) o).getUtilityType() == type &&
	       utilityName.equals( name) &&
	       utilityAref != null &&
	       utilityAref.equals( aref))
	    return o;    
        }
        else {
	  if ( ((JopUtilityIfc) o).getUtilityType() == type &&
	       utilityName.equals( name))
	    return o;    
	}
      }
      return null;
    default:
      return null;
    }

  }

  public void executeCommand( String command) {
    JopSpider.command( session, command);
  }

  public boolean isApplet() {
    return ( root instanceof JApplet);
  }
  public boolean isApplication() {
    return !( root instanceof JApplet);
  }
  public boolean isOpWindowApplet() {
    return ( root instanceof JopOpWindowApplet);
  }

    public void openSearch(String object){
	HistSearch HSWindow = new HistSearch(object,session );
        HSWindow.pack();        
        HSWindow.show();
    }
}




