/* 
 * Proview   $Id: JopApplet.java,v 1.2 2005-09-01 14:57:50 claes Exp $
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


/**
 * Title:        <p>
 * Description:  Utöka befintliga JButton till PwrButton.<p>
 * Copyright:    Copyright (c) <p>
 * Company:      <p>
 * @author
 * @version 1.0
 */

package jpwr.jop;
import javax.swing.*;
import java.awt.event.*;
import java.awt.*;
import java.applet.*;
import jpwr.rt.*;

public class JopApplet extends JApplet
{
  public JopSession session;
  public JopEngine engine;

  public JopApplet()
  {
  }

  public void init() {
    String instance = this.getParameter("instance");

    engine = new JopEngine(1000, (Object)this);
    session = new JopSession( engine, (Object)this);
    if ( instance != null && instance.length() != 0) {
      // Substitutes for åäö because of mozilla...
      instance = instance.replace( '\\', 'å');
      instance = instance.replace( '/', 'ä');
      instance = instance.replace( '@', 'ö');

      System.out.println( "Parameter instance: " + instance);
      engine.gdh.logString("Parameter instance: " + instance);
      engine.setInstance( instance);
    }
  }

  public void stop() {
//      System.exit(0);
  }
  public void destroy() {
    engine.close();
  }
  public String getAppletInfo(){
    return "Applet Information";
  }
  public String[][] getParameterInfo() {
    return null;
  }
}
