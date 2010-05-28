/* 
 * Proview   $Id: JopSessionIfc.java,v 1.6 2008-06-24 13:35:11 claes Exp $
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

public interface JopSessionIfc {
  public void setSession( Object session);
  public void setRoot( Object root);
  public Object getRoot();
  public void setEngine( JopEngine engine);
  public String getLang();
  public void setLang( String lang);
  public JopEngine getEngine();
  public void setNavigator( Object navigator);
  public void setAlarmList( Object alarmList);
  public void addUtility( Object utility);
  public void removeUtility( Object utility);
  public void openNavigator( PwrtObjid oid);
  public void openAlarmList();
  public void openEventLog();
  public void openLogin();
  public void openLanguage();
  public void openGraphFrame( String name, String instance, boolean scrollbar, boolean classGraph);
  public void openFlowFrame( PwrtObjid oid, String center);
  public void openCrrFrame( String name);
  public int executeCommand( String command);
  public boolean isApplet();
  public boolean isApplication();
  public boolean isOpWindowApplet();
  public void openSearch(String object);
  public void setOpWindowLabelText( String text );
  public void setOpWindowLanguage( int language );
}
