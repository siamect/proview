
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








