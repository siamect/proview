/* 
 * Proview   $Id: JopOpWindowFrame.java,v 1.4 2008-06-24 13:35:11 claes Exp $
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

  public void setLabelText( String text) {
    localPanel.setLabelText( text);
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


