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

