/* 
 * Proview   $Id: JopSession.java,v 1.5 2007-01-30 06:52:55 claes Exp $
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

public class JopSession {
  public JopSession() {
  }

  Object sessionRep;

  // New implementation
  public JopSession( JopEngine engine, Object rootObject) {
    // Create a JopSessionRep object which class i not known by the compiler
    try {
      Class clazz = Class.forName( "jpwr.jop.JopSessionRep");
      try {
        sessionRep = clazz.newInstance();
      }
      catch ( InstantiationException e) {
        System.out.println("InstatiationException");
      }
      catch ( IllegalAccessException e) {
        System.out.println("IllegalAccessException");
      }
    }
    catch (ClassNotFoundException e) {
      System.out.println("Class not found");
    }

    ((JopSessionIfc) sessionRep).setSession(this);
    ((JopSessionIfc) sessionRep).setRoot(rootObject);
    ((JopSessionIfc) sessionRep).setEngine(engine);
  }

  public Object getRoot() {
    return ((JopSessionIfc) sessionRep).getRoot();
  }

  public JopEngine getEngine() {
    return ((JopSessionIfc) sessionRep).getEngine();
  }

  public Gdh getGdh() {
    return ((JopSessionIfc) sessionRep).getEngine().gdh;
  }

  public void addUtility( Object utility) {
    ((JopSessionIfc) sessionRep).addUtility( utility);
  }

  public void removeUtility( Object utility) {
    ((JopSessionIfc) sessionRep).removeUtility( utility);
  }

  public void setNavigator( Object navigator) {
    ((JopSessionIfc) sessionRep).setNavigator( navigator);
  }

  public void openNavigator( PwrtObjid oid) {
    ((JopSessionIfc) sessionRep).openNavigator( oid);
  }

  public void openAlarmList() {
    ((JopSessionIfc) sessionRep).openAlarmList();
  }

  public void openEventLog() {
    ((JopSessionIfc) sessionRep).openEventLog();
  }

  public void openLogin() {
    ((JopSessionIfc) sessionRep).openLogin();
  }

  public void openGraphFrame( String name, String instance, boolean scrollbar, boolean classGraph) {
    ((JopSessionIfc) sessionRep).openGraphFrame( name, instance, scrollbar, classGraph);
  }

  public void openFlowFrame( PwrtObjid oid, String center) {
    ((JopSessionIfc) sessionRep).openFlowFrame( oid, center);
  }

  public void openCrrFrame( String name) {
    ((JopSessionIfc) sessionRep).openCrrFrame( name);
  }

  public void executeCommand( String command) {
    ((JopSessionIfc) sessionRep).executeCommand( command);
  }

  public boolean isApplet() {
    return ((JopSessionIfc) sessionRep).isApplet();
  }

  public boolean isApplication() {
    return ((JopSessionIfc) sessionRep).isApplication();
  }

  public boolean isOpWindowApplet() {
    return ((JopSessionIfc) sessionRep).isOpWindowApplet();
  }

    public void openSearch(String object ){
	((JopSessionIfc) sessionRep).openSearch(object);
    }
}










