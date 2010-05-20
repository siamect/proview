/* 
 * Proview   $Id: JopMethodsMenu.java,v 1.6 2007-01-30 13:02:27 claes Exp $
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

public class JopMethodsMenu implements ActionListener, PopupMenuListener,
       JopDynamic {

  JPopupMenu popup;
  JopSession session;
  Gdh gdh;
  JopMethods methods;
    //String object;
    //PwrtAttrRef aref;
    //int classid;
    //int utility;

  JopMethodsMenu( JopSession session, String object, int utility, 
		  Component invoker, int x, int y) {
    this.session = session;
    gdh = session.getEngine().gdh;

    if ( object == null)
      return;
    CdhrAttrRef oret = gdh.nameToAttrRef( object);
    if ( oret.evenSts()) return;

    CdhrTypeId cret = gdh.getAttrRefTid( oret.aref);
    if ( cret.evenSts()) return;

    System.out.println( "Menu object: " + object + " cid " + cret.typeId);

    methods = new JopMethods( session, oret.aref, object, cret.typeId, utility);

    popup = new JPopupMenu();

    JMenuItem item;

    if ( methods.openGraphFilter()) {
      popup.add( item = new JMenuItem( "Graph"));
      item.addActionListener( this);
    }

    if ( methods.openObjectGraphFilter()) {
      popup.add( item = new JMenuItem( "ObjectGraph"));
      item.addActionListener( this);
    }

    if ( methods.helpFilter()) {
      popup.add( item = new JMenuItem( "Help"));
      item.addActionListener( this);
    }

    if ( methods.dataSheetFilter()) {
      popup.add( item = new JMenuItem( "DataSheet"));
      item.addActionListener( this);
    }

    if ( methods.rtNavigatorFilter()) {
      popup.add( item = new JMenuItem( "Navigator"));
      item.addActionListener( this);
    }

    if ( methods.openTraceFilter()) {
      popup.add( item = new JMenuItem( "Open Plc"));
      item.addActionListener( this);
    }

    if ( methods.openCrossrefFilter()) {
      popup.add( item = new JMenuItem( "Crossreferences"));
      item.addActionListener( this);
    }

    if ( methods.helpClassFilter()) {
      popup.add( item = new JMenuItem( "Class Help"));
      item.addActionListener( this);
    }
 
    if ( methods.circuitDiagramFilter()) {
      popup.add( item = new JMenuItem( "Circuit Diagram"));
      item.addActionListener( this);
    }
 
    if ( methods.histEventFilter()){
      popup.add(item= new JMenuItem( "Event Log"));
      item.addActionListener( this);
    }

    if ( methods.simulateFilter()){
      popup.add(item= new JMenuItem( "Simulate"));
      item.addActionListener( this);
    }
    popup.addPopupMenuListener( this);
    popup.show( invoker, x,  y);
    session.getEngine().add(this);
  }

  public void popupMenuWillBecomeVisible( PopupMenuEvent e) {}
  public void popupMenuWillBecomeInvisible( PopupMenuEvent e) {}
  public void popupMenuCanceled( PopupMenuEvent e) {}

  public void actionPerformed( ActionEvent event) {

    if ( event.getActionCommand().equals("Navigator")) {
      methods.rtNavigator();
    }
    else if ( event.getActionCommand().equals("Open Plc")) {
      methods.openTrace();
    }
    else if ( event.getActionCommand().equals("Graph")) {
      methods.openGraph();
    }
    else if ( event.getActionCommand().equals("Help")) {
      methods.help();
    }
    else if ( event.getActionCommand().equals("DataSheet")) {
      methods.dataSheet();
    }
    else if ( event.getActionCommand().equals("Crossreferences")) {
      methods.openCrossref();
    }
    else if ( event.getActionCommand().equals("Class Help")) {
      methods.helpClass();
    }
    else if ( event.getActionCommand().equals("Circuit Diagram")) {
      methods.circuitDiagram();
    }
    else if ( event.getActionCommand().equals("ObjectGraph")) {
      methods.openObjectGraph();
    }
    else if ( event.getActionCommand().equals("Hist Event")) {
      methods.histEvent();
    }
    else if ( event.getActionCommand().equals("Simulate")) {
      methods.simulate();
    }
  }  

  public boolean isVisible() {
    return popup.isVisible();
  }
  public void repaint() {
    popup.repaint();
  }

  public void dynamicUpdate( boolean animationOnly) {
    if ( !isVisible())
      session.getEngine().remove(this);
    else
      repaint();
  }
  public void dynamicOpen() {}
  public void dynamicClose() {}
  public Object dynamicGetRoot() {
    return this;
  }

}











