
package jpwr.jop;
import jpwr.rt.*;
import java.io.*;
import java.awt.*;
import java.awt.geom.*;
import java.awt.image.*;
import java.awt.font.*;
import javax.swing.*;
import javax.swing.border.*;
import java.awt.event.*;
import java.net.*;
import java.applet.*;

public class JopOpWindowFrame extends JFrame {
  JScrollPane scrollPane;
  JPanel contentPane;
  BorderLayout borderLayout1 = new BorderLayout();
  JopOpWindow localPanel;
  Dimension size;
  JopEngine engine;
  JopSession session;
  Object root;
  PwrtObjid utilityObjid;

  public JopOpWindowFrame() {
    engine = new JopEngine( 1000, (Object)this);
    session = new JopSession( engine, (Object)this);
    root = (Object) this;
    localPanel = new JopOpWindow( session, (Object)this);
    init();
    pack();
  }

  private void init() {
    contentPane = (JPanel) this.getContentPane();
    contentPane.setLayout(borderLayout1);
    scrollPane = new JScrollPane(localPanel);
    contentPane.add(scrollPane, BorderLayout.CENTER);
    contentPane.setOpaque(true);
    localPanel.setLayout( new BoxLayout( localPanel, BoxLayout.Y_AXIS));
    localPanel.setOpaque(true);
    localPanel.setBackground( Color.white);
    this.setTitle("Operator Window");
    size = new Dimension( 300, 900);
    setSize( size);
    engine.setFrameReady();
  }

  // Exit when window is closed
  protected void processWindowEvent(WindowEvent e) {
    super.processWindowEvent(e);
    if (e.getID() == WindowEvent.WINDOW_CLOSING) {
	//  System.exit(0);
    }
  }
}


