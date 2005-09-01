/* 
 * Proview   $Id: JopFrame.java,v 1.2 2005-09-01 14:57:50 claes Exp $
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
import jpwr.rt.*;

public class JopFrame extends JFrame
{
  public JopSession session;
  public JopEngine engine;
  public PwrtObjid utilityObjid = null;

  public JopFrame()
  {
    try {
      jbInit();
    }
    catch(Exception e) {
      e.printStackTrace();
    }
  }

  public JopFrame( JopSession session, String instance) {
    this.session = session;
    engine = session.getEngine();
    if ( instance != null) {
      engine.setInstance( instance);

      CdhrObjid oret = engine.gdh.nameToObjid( instance);
      if ( oret.oddSts())
	utilityObjid = oret.objid;
    }
    enableEvents(AWTEvent.WINDOW_EVENT_MASK);
  }

  private void jbInit() throws Exception {
    engine = new JopEngine(1000, (Object)this);
    session = new JopSession( engine, (Object) this);
    enableEvents(AWTEvent.WINDOW_EVENT_MASK);
    setDefaultCloseOperation( DISPOSE_ON_CLOSE);
  }

  // Exit when window is closed
  protected void processWindowEvent(WindowEvent e) {
    super.processWindowEvent(e);
    if (e.getID() == WindowEvent.WINDOW_CLOSING) {
      session.removeUtility( this);
      System.out.println( "JopFrame closing down...");
      engine.removeWindow( this);
      // System.exit(0);
    }
  }

}








