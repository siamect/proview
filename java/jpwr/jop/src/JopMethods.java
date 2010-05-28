/* 
 * Proview   $Id$
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
import javax.swing.event.*;
import java.awt.event.*;

public class JopMethods {

  JopSession session;
  Gdh gdh;
  String object;
  PwrtAttrRef aref;
  int classid;
  int utility;

  public JopMethods( JopSession session, PwrtAttrRef aref, String object, int classid, int utility) {
    this.session = session;
    gdh = session.getEngine().gdh;
    this.object = object;
    this.utility = utility;
    this.aref = aref;
    this.classid = classid;

  }


  public boolean callFilterMethod( String method) {
    if ( method.equals("RtNavigator")) {
      return rtNavigatorFilter();
    }
    else if ( method.equals("Open Plc")) {
      return openTraceFilter();
    }
    else if ( method.equals("Graph")) {
      return openGraphFilter();
    }
    else if ( method.equals("Help")) {
      return helpFilter();
    }
    else if ( method.equals("DataSheet")) {
      return dataSheetFilter();
    }
    else if ( method.equals("Crossreferences")) {
      return openCrossrefFilter();
    }
    else if ( method.equals("Help Class")) {
      return helpClassFilter();
    }
    else if ( method.equals("CircuitDiagram")) {
      return circuitDiagramFilter();
    }
    else if ( method.equals("ObjectGraph")) {
      return openObjectGraphFilter();
    }
    else if ( method.equals("Hist Event...")) {
      return histEventFilter();
    }
    else if ( method.equals("Simulate")) {
      return simulateFilter();
    }
    else if ( method.equals("Open Object")) {
      return false; // NYI
    }
    else if ( method.equals("Trend")) {
      return false; // NYI
    }
    else if ( method.equals("Fast")) {
      return false; // NYI
    }
    else if ( method.equals("Photo")) {
      return false; // NYI
    }
    else if ( method.equals("Note")) {
      return false; // NYI
    }
    else if ( method.equals("Block Events...")) {
      return false; // NYI
    }
    return false;
  }

  public void callMethod( String method) {
    if ( method.equals("RtNavigator")) {
      rtNavigator();
    }
    else if ( method.equals("Open Plc")) {
      openTrace();
    }
    else if ( method.equals("Graph")) {
      openGraph();
    }
    else if ( method.equals("Help")) {
      help();
    }
    else if ( method.equals("DataSheet")) {
      dataSheet();
    }
    else if ( method.equals("Crossreferences")) {
      openCrossref();
    }
    else if ( method.equals("Class Help")) {
      helpClass();
    }
    else if ( method.equals("CircuitDiagram")) {
      circuitDiagram();
    }
    else if ( method.equals("ObjectGraph")) {
      openObjectGraph();
    }
    else if ( method.equals("Hist Event...")) {
      histEvent();
    }
    else if ( method.equals("Simulate")) {
      simulate();
    }
    else if ( method.equals("Open Object")) {
      // NYI
    }
    else if ( method.equals("Trend")) {
      // NYI
    }
    else if ( method.equals("Fast")) {
      // NYI
    }
    else if ( method.equals("Photo")) {
      // NYI
    }
    else if ( method.equals("Note")) {
      // NYI
    }
    else if ( method.equals("Block Events...")) {
      // NYI
    }
  }

  //
  // Methods and filter metods for object
  //
  public boolean rtNavigatorFilter() {
    if ( utility == JopUtility.NAVIGATOR)
      return false;

    return true;
  }
  public void rtNavigator() {
    session.openNavigator( aref.getObjid());
  }

  public boolean openCrossrefFilter() {
    return true;
  }
  public void openCrossref() {
    session.openCrrFrame( object);
  }

  public boolean openTraceFilter() {
    if ( utility == JopUtility.TRACE)
      return false;

    if ( classid == Pwrb.cClass_plc ||
	 classid == Pwrb.cClass_windowplc ||
	 classid == Pwrb.cClass_windowsubstep ||
	 classid == Pwrb.cClass_windoworderact ||
	 classid == Pwrb.cClass_windowplc)
      return true;

    CdhrObjid pret = gdh.getParent( aref.getObjid());
    if ( pret.evenSts())
      return false;

    CdhrClassId cret = gdh.getObjectClass( pret.objid);

     if ( cret.evenSts()) return false;

    if ( cret.classId == Pwrb.cClass_windowplc ||
	 cret.classId == Pwrb.cClass_windowsubstep ||
	 cret.classId == Pwrb.cClass_windoworderact ||
	 cret.classId == Pwrb.cClass_windowplc)
      return true;

    return false;
  }

  public void openTrace() {
    if ( classid == Pwrb.cClass_windowplc ||
	 classid == Pwrb.cClass_windowsubstep ||
	 classid == Pwrb.cClass_windoworderact ||
	 classid == Pwrb.cClass_windowplc) {
      session.openFlowFrame( aref.getObjid(), null);
    }
    else if ( classid == Pwrb.cClass_plc) {
      // Open child 
      CdhrObjid child = gdh.getChild( aref.getObjid());
      if ( child.evenSts()) return;

      CdhrClassId cret = gdh.getObjectClass( child.objid);
      if ( cret.evenSts()) return;

      if ( cret.classId == Pwrb.cClass_windowplc ||
	   cret.classId == Pwrb.cClass_windowsubstep ||
	   cret.classId == Pwrb.cClass_windoworderact ||
	   cret.classId == Pwrb.cClass_windowplc) {
        session.openFlowFrame( child.objid, null);
      }
    }
    else {
      // Open parent window and center object
      CdhrObjid parent = gdh.getParent( aref.getObjid());
      if ( parent.evenSts()) return;

      int idx = object.lastIndexOf('-');
      if ( idx == -1) return;
      String name = object.substring( idx + 1);

      session.openFlowFrame( parent.objid, name);
    }
  }

  public boolean openGraphFilter() {
    if ( utility == JopUtility.GRAPH) {
      // Check caller name...
    }

    String attr = object + ".DefGraph";
    CdhrString sret = gdh.getObjectInfoString( attr);
    if ( sret.evenSts())
      return false;
    if ( sret.str.equals(""))
      return false;

    return true;
  }
  public void openGraph() {
    String attr = object + ".DefGraph";
    CdhrString xttgraph = gdh.getObjectInfoString( attr);
    if ( xttgraph.evenSts()) return;

    attr = xttgraph.str + ".Action";
    CdhrString actionRet = gdh.getObjectInfoString( attr);
    if ( actionRet.evenSts()) return;

    int idx = actionRet.str.lastIndexOf( '.');
    String action;
    if ( idx != -1)
      action = actionRet.str.substring(0,1).toUpperCase() +
	actionRet.str.substring(1, idx);
    else
      action = actionRet.str.substring(0,1).toUpperCase() +
	actionRet.str.substring(1);

    String instance = null;
    attr = xttgraph.str + ".Object";
    CdhrString instanceRet = gdh.getObjectInfoString( attr);
    if ( instanceRet.oddSts() && ! instanceRet.str.equals(""))
      instance = instanceRet.str;      

    session.openGraphFrame( action, instance, false, false);
  }

  public boolean helpFilter() {
    String attr = object + ".HelpTopic";
    CdhrString sret = gdh.getObjectInfoString( attr);
    if ( sret.evenSts() || sret.str.equals(""))
      return false;

    return true;
  }
  public void help() {
    String attr = object + ".HelpTopic";
    CdhrString sret = gdh.getObjectInfoString( attr);
    if ( sret.evenSts()) return;

    String cmd = "help " + sret.str;
    session.executeCommand( cmd);
  }

  public boolean dataSheetFilter() {
    String attr = object + ".DataSheet";
    CdhrString sret = gdh.getObjectInfoString( attr);
    if ( sret.evenSts() || sret.str.equals(""))
      return false;

    return true;
  }
  public void dataSheet() {
    String attr = object + ".DataSheet";
    CdhrString sret = gdh.getObjectInfoString( attr);
    if ( sret.evenSts()) return;

    String cmd = "open url \"" + sret.str + "\"";
    System.out.println( "dataSheet: " + cmd);
    session.executeCommand( cmd);
  }

  public boolean circuitDiagramFilter() {
    String attr = object + ".CircuitDiagram";
    CdhrString sret = gdh.getObjectInfoString( attr);
    if ( sret.evenSts() || sret.str.equals(""))
      return false;

    return true;
  }
  public void circuitDiagram() {
    String attr = object + ".CircuitDiagram";
    CdhrString sret = gdh.getObjectInfoString( attr);
    if ( sret.evenSts()) return;

    String cmd = "open url \"" + sret.str + "\"";
    System.out.println( "circuitDiagram: " + cmd);
    session.executeCommand( cmd);
  }

  public boolean helpClassFilter() {
    return true;
  }
  public void helpClass() {
    CdhrObjid oret = gdh.classIdToObjid( classid);
    if ( oret.evenSts()) return;

    CdhrString sret = gdh.objidToName( oret.objid, Cdh.mName_volumeStrict); 
    if ( sret.evenSts()) return;

    int idx = sret.str.lastIndexOf(":");
    if ( idx == -1) return;

    String volume = sret.str.substring( 0, idx);
    idx = sret.str.lastIndexOf("-");
    if ( idx == -1) return;

    String className = sret.str.substring(idx + 1);

    String url = volume.toLowerCase() + "_" + className.toLowerCase() + ".html";
    String cmd = "open url \"" + url + "\"";
    session.executeCommand( cmd);
  }

  public boolean openObjectGraphFilter() {
    if ( classid == Pwrs.cClass_Node)
      return true;

    CdhrObjid coid = gdh.classIdToObjid( classid);
    if ( coid.evenSts()) return false;

    CdhrString sret = gdh.objidToName( coid.objid, Cdh.mName_object);
    if ( sret.evenSts()) return false;

    String name;
    if ( coid.objid.vid < Cdh.cUserClassVolMin || 
	 (coid.objid.vid >= Cdh.cManufactClassVolMin && 
	  coid.objid.vid <= Cdh.cManufactClassVolMax)) {
      // Class is a base class, java classname starts with JopC_
      if ( coid.objid.vid == 1)
	name = "jpwr.jopc.Jopc" + sret.str.substring(1,2).toUpperCase() + 
	  sret.str.substring(2).toLowerCase();
      else if ( coid.objid.vid == 10) {
	if ( sret.str.startsWith( "BaseFcPPO"))
	  name = "jpwr.bcompfc.Jopc" + sret.str.substring(0,1).toUpperCase() + 
	      sret.str.substring(1).toLowerCase();
	else
	  name = "jpwr.bcomp.Jopc" + sret.str.substring(0,1).toUpperCase() + 
	      sret.str.substring(1).toLowerCase();
      }
      else
	name = "jpwr.jopc.Jopc" + sret.str.substring(0,1).toUpperCase() + 
	  sret.str.substring(1).toLowerCase();
    }
    else
      // Java name equals class name
      name = sret.str.substring(0,1).toUpperCase() + sret.str.substring(1).toLowerCase();
    System.out.println( "classGraphFilter: " + name + ", class: " + sret.str);

    try {
      Class clazz = Class.forName( name);
    }
    catch ( Exception e) {
      return false;
    }
    return true;
  }
  public void openObjectGraph() {
    String cmd = "open graph /class /instance=" + object;
    System.out.println( "classGraph: " + cmd);
    session.executeCommand( cmd);
  }

  public boolean histEventFilter(){
    return true;
  }

  public void histEvent(){
    session.openSearch(object);
  }

  public boolean simulateFilter() {

    // Check IOSimulFlag in IOHandler object
    CdhrObjid hoid = gdh.getClassList( Pwrb.cClass_IOHandler);
    if ( hoid.evenSts()) return false;

    CdhrString sret = gdh.objidToName( hoid.objid, Cdh.mName_pathStrict);
    if ( sret.evenSts()) return false;

    CdhrBoolean fret = gdh.getObjectInfoBoolean( sret.str + ".IOSimulFlag");
    if ( fret.evenSts()) return false;

    if ( !fret.value)
      return false;

    // Check SimConnect attribute
    sret = gdh.getObjectInfoString( object + ".SimConnect");
    if ( sret.evenSts()) return false;

    return true;
  }

  public void simulate() {
    // Get SimConnect attribute
    CdhrString sret = gdh.getObjectInfoString( object + ".SimConnect");
    if ( sret.evenSts()) return;

    String cmd = "open graph /class /instance=" + sret.str;
    System.out.println( "simulateClassGraph: " + cmd);
    session.executeCommand( cmd);
  }
}

