
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

public class FlowFrame extends JFrame implements JopUtilityIfc {
  JScrollPane scrollPane;
  JPanel contentPane;
  JMenuBar menuBar;
  BorderLayout borderLayout1 = new BorderLayout();
  LocalPanel localPanel = new LocalPanel();
  Dimension size;
  FlowCtx ctx;
  JopEngine engine;
  JopSession session;
  Object root;
  PwrtObjid utilityObjid;

  public FlowFrame( PwrtObjid oid, String center) {
    engine = new JopEngine( 1000, (Object)this);
    session = new JopSession( engine, (Object)this);
    session.addUtility( this);
    root = (Object) this;
    init( oid, center);
  }

  public FlowFrame( PwrtObjid oid, JopEngine engine, Object root, 
		    String center) {
    this.engine = engine;
    this.root = root;
    init( oid, center);
  }

  public FlowFrame( JopSession session, PwrtObjid oid, String center) {
    this.session = session;
    engine = session.getEngine();
    root = session.getRoot();

    init( oid, center);
  }

  private void init( PwrtObjid oid, String center) {
    utilityObjid = oid;
    contentPane = (JPanel) this.getContentPane();
    contentPane.setLayout(borderLayout1);
    scrollPane = new JScrollPane(localPanel);
    menuBar = createMenu();
    contentPane.add( menuBar, BorderLayout.NORTH);
    contentPane.add(scrollPane, BorderLayout.CENTER);
    contentPane.setOpaque(true);
    localPanel.setLayout(null);
    localPanel.setOpaque(true);
    localPanel.setBackground( Color.white);
    this.setTitle("FlowFrame");
    size = new Dimension( 1100, 900);
    setSize( size);

    String filename;
    BufferedReader reader = null;

    // Open file
    StringBuffer b1 = new StringBuffer("000");
    String s = Integer.toString( (oid.vid >> 24) & 0xff);
    b1.setLength( b1.length() - s.length());
    b1.append(s);

    StringBuffer b2 = new StringBuffer("000");
    s = Integer.toString( (oid.vid >> 16) & 0xff);
    b2.setLength( b2.length() - s.length());
    b2.append(s);

    StringBuffer b3 = new StringBuffer("000");
    s = Integer.toString( (oid.vid >> 8) & 0xff);
    b3.setLength( b3.length() - s.length());
    b3.append(s);

    StringBuffer b4 = new StringBuffer("000");
    s = Integer.toString( oid.vid & 0xff);
    b4.setLength( b4.length() - s.length());
    b4.append(s);

    StringBuffer b5 = new StringBuffer("00000000");
    s = Integer.toHexString( oid.oix);
    b5.setLength( b5.length() - s.length());
    b5.append(s);

    String fname = "pwr_" + b1 + "_" + b2 + "_" + b3 + "_" +
	b4 + "_" + b5 + ".flw";

    if ( root != null && root instanceof JopApplet) {
      try {
        URL current = ((JApplet) root).getCodeBase();
        String current_str = current.toString();
        int idx1 = current_str.lastIndexOf('/');
        int idx2 = current_str.lastIndexOf(':');
        int idx = idx1;
        if ( idx2 > idx)
          idx = idx2;
        String path = current_str.substring(0,idx + 1);
	filename = path + fname;
	System.out.println( "Opening file " + filename);
        URL fileURL = new URL( filename);
	InputStream in = fileURL.openStream();
        // in = new BufferedInputStream(in);
        InputStreamReader r2 = new InputStreamReader(in);
        reader = new BufferedReader( r2);
      }
      catch ( Exception e) {
	System.out.println( "Unable to open file");
      }
    }
    else {
      filename = "$pwrp_load/" + fname;
      filename = engine.gdh.translateFilename( filename);

      // filename = "/home/claes/flow/t.t";
      System.out.println( "Fname: " + filename);
      try {
        reader = new BufferedReader(new FileReader(filename));
      }
      catch ( Exception e) {
	System.out.println( "Unable to open file " + filename);
        return;
      }
    }
    ctx = new FlowCtx( engine.gdh, session);
    ctx.open( reader);
    System.out.println( "a.size: " + ctx.cmn.a.size());
    for ( int i = 0; i < ctx.cmn.a.size(); i++) {
      Object o = ctx.cmn.a.get(i);
      localPanel.add( (JComponent) o);
      engine.add( o);
      ((FlowComponent) o).setBounds();
    }
    engine.setFrameReady();
    setSize();
    if ( center != null)
      centerObject( center);
    enableEvents(AWTEvent.WINDOW_EVENT_MASK);
    setDefaultCloseOperation( DISPOSE_ON_CLOSE);
  }

  void setSize() {
    size = new Dimension( (int)((ctx.cmn.x_right - ctx.cmn.x_left) * ctx.cmn.zoom_factor) + Flow.DRAWOFFSET * 2,
			  (int)((ctx.cmn.y_high - ctx.cmn.y_low) * ctx.cmn.zoom_factor) + Flow.DRAWOFFSET * 2);
    localPanel.setPreferredSize( size);
    localPanel.revalidate();
  }

  class LocalPanel extends JPanel {
    public LocalPanel() {}
    public Dimension getPreferredSize() { return size;}
    public Dimension getMinimumSize() { return size;}
  }

  JMenuBar createMenu() {
    JMenuBar menuBar = new JMenuBar();
    JMenu fileMenu = new JMenu("File");
    fileMenu.setMnemonic('F');
    MenuAction closeFileAction = new MenuAction("Close");
    MenuAction subWindowFileAction = new MenuAction("Open Subwindow");

    menuBar = new JMenuBar();
    JMenu functionsMenu = new JMenu("Functions");
    fileMenu.setMnemonic('u');
    MenuAction crossrefFunctionsAction = new MenuAction("Show Crossreferences");

    JMenu viewMenu = new JMenu("View");
    fileMenu.setMnemonic('V');
    MenuAction zoomInViewAction = new MenuAction("Zoom In");
    MenuAction zoomOutViewAction = new MenuAction("Zoom Out");
    MenuAction zoomResetViewAction = new MenuAction("Zoom Reset");

    JMenuItem item;
    item = fileMenu.add( closeFileAction);
    item.setMnemonic('C');
    item = fileMenu.add( subWindowFileAction);
    item.setMnemonic('C');
    item = functionsMenu.add( crossrefFunctionsAction);
    item.setMnemonic('r');
    item.setAccelerator( KeyStroke.getKeyStroke('R', java.awt.Event.CTRL_MASK, false));
    item = viewMenu.add( zoomInViewAction);
    item.setMnemonic('I');
    item.setAccelerator( KeyStroke.getKeyStroke('P', java.awt.Event.CTRL_MASK, false));
    item = viewMenu.add( zoomOutViewAction);
    item.setMnemonic('O');
    item.setAccelerator( KeyStroke.getKeyStroke('O', java.awt.Event.CTRL_MASK, false));
    item = viewMenu.add( zoomResetViewAction);
    item.setMnemonic('R');

    menuBar.add( fileMenu);
    menuBar.add( functionsMenu);
    menuBar.add( viewMenu);
    menuBar.setBorder( new BevelBorder( BevelBorder.RAISED));

    return menuBar;
  }

  class MenuAction extends AbstractAction {
    public MenuAction( String text) {
      super(text);
    }

    public void actionPerformed( ActionEvent e) {
      System.out.println( "Menu: " + e.getActionCommand());
      if ( e.getActionCommand().equals("Close")) {

	closeFlow();
	dispose();
      }
      if ( e.getActionCommand().equals("Show Crossreferences")) {
        Object sel = ctx.getSelected();
        if ( sel != null) {
	  String name = ((FlowComponent) sel).getTraceObject();
          if ( name == null) return;

          session.openCrrFrame( name);
	}
	return;
      }
      else if ( e.getActionCommand().equals("Open Subwindow")) {
        Object sel = ctx.getSelected();
        if ( sel != null) {
	  String name = ((FlowComponent) sel).getTraceObject();
          if ( name == null) return;
          CdhrObjid ret = ctx.cmn.gdh.nameToObjid( name);
          if ( ret.evenSts()) return;
	  ret = ctx.cmn.gdh.getChild( ret.objid);
          if ( ret.evenSts()) return;

          FlowFrame f = new FlowFrame( ret.objid, engine, root, null);
          f.pack();
	  f.setVisible(true);
        }
      }
      else if ( e.getActionCommand().equals("Zoom In")) {
	zoom( 1.2);
      }
      else if ( e.getActionCommand().equals("Zoom Out")) {
	zoom( 1.0 / 1.2);
      }
      else if ( e.getActionCommand().equals("Zoom Reset")) {
	zoomReset();
      }
    }
  }

  public void zoom( double factor) {
      ctx.cmn.zoom_factor *= factor;
      for ( int i = 0; i < ctx.cmn.a.size(); i++)
	((FlowComponent) ctx.cmn.a.get(i)).setBounds();
      setSize();
      // localPanel.repaint();    
  }
  public void zoomReset() {
    zoom( ctx.cmn.base_zoom_factor / ctx.cmn.zoom_factor);
  }
  public void centerObject( String name) {
    Object o = ctx.getObject( name);
    if ( o == null) return;

    Rectangle r = ((JComponent) o).getBounds();
    ctx.unselect();
    ((FlowComponent) o).setSelect(true);

    scroll( r.x + r.width/2, r.y + r.height/2);
  }
  public void scroll( int x, int y) {
        
    JScrollBar hScroll = scrollPane.getHorizontalScrollBar();
    JScrollBar vScroll = scrollPane.getVerticalScrollBar();
    hScroll.setValues( x - getWidth()/2, size.width, 0, size.width);
    vScroll.setValues( y - getHeight()/2, size.height, 0, size.height);
  }

  // JopUtility interface
  public int getUtilityType() {
    return JopUtility.TRACE;
  }

  public PwrtObjid getUtilityObjid() {
    return utilityObjid;
  }

  public String getUtilityName() {
    return null;
  }  

  public void closeFlow() {
    for ( int i = 0; i < ctx.cmn.a.size(); i++) {
      Object o = ctx.cmn.a.get(i);
      engine.remove( o);
    }
    session.removeUtility( this);
  }

  // Exit when window is closed
  protected void processWindowEvent(WindowEvent e) {
    super.processWindowEvent(e);
    if (e.getID() == WindowEvent.WINDOW_CLOSING) {
      closeFlow();
    }
  }
}





