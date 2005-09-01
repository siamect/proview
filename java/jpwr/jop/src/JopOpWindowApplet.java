/* 
 * Proview   $Id: JopOpWindowApplet.java,v 1.2 2005-09-01 14:57:50 claes Exp $
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
import java.util.*;
import java.applet.Applet;
import java.awt.*;
import java.awt.event.*;
import java.awt.font.*;
import java.awt.geom.*;
import java.awt.image.*;
import javax.swing.*;
import javax.swing.event.*;
import jpwr.rt.*;

public class JopOpWindowApplet extends JopApplet
{
  JScrollPane scrollPane;
  JPanel contentPane;
  BorderLayout borderLayout1 = new BorderLayout();
  JopOpWindow localPanel;
  Dimension size;

  public void setSize(int width, int height)
  {
    Logg.logg("JopXttApplet: setSize()", 1);

    super.setSize(width, height);

    validate();
  }


  public String GetAppletInfo()
  {
    return "Applet information";
  }

  public String[][] getParameterInfo()
  {
    return null;
  }
  public void destroy()
  {
    // session.close();
      super.destroy();
  }

  public void init()
  {
    super.init();
    setSize(new Dimension(370, 570));

    localPanel = new JopOpWindow( session, (Object)this);
    contentPane = (JPanel) this.getContentPane();
    contentPane.setLayout(borderLayout1);
    scrollPane = new JScrollPane(localPanel);
    contentPane.add(scrollPane, BorderLayout.CENTER);
    contentPane.setOpaque(true);
    localPanel.setLayout(new BoxLayout( localPanel, BoxLayout.Y_AXIS));
    localPanel.setOpaque(true);
    localPanel.setBackground( Color.white);
    // this.setTitle("Operator Window");
    size = new Dimension( 300, 900);
    setSize( size);
    engine.setFrameReady();
  }

  public void start() {
  }


  public void stop() {
  }
}

