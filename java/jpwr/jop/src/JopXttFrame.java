package jpwr.jop;
import java.util.*;
import java.awt.*;
import java.awt.event.*;
import java.awt.font.*;
import java.awt.geom.*;
import java.awt.image.*;
import javax.swing.*;
import javax.swing.event.*;
import javax.swing.tree.*;
import java.net.URL;
import jpwr.rt.*;

/**
 *  Description of the Class
 *
 *@author     JN3920
 *@created    November 12, 2002
 */
public class JopXttFrame extends JFrame
{
  JopSession session;
  JopEngine engine;
  XttTree xttTree;
  Object root;
  /**  Description of the Method */
  public JopXttFrame()
  {
    engine = new JopEngine( 1000, (Object)this);
    session = new JopSession( engine, (Object)this);
    root = (Object) this;
    init((PwrtObjid)null);

  }
  public JopXttFrame( JopSession session, PwrtObjid oid )
  {
    this.session = session;
    engine = session.getEngine();
    root = session.getRoot();
    init(oid);
  }    
  public void init(PwrtObjid oid)
  {
    setSize(new Dimension(370, 570));
    URL url = null;
    Logg.loggTo = Logg.TO_SCREEN;
    Logg.loggPrio = 1;
    Logg.logg("JopXttApplet: Före XttTree-skapande", 6);
    this.xttTree = new XttTree(this.session, url, this);
    this.getContentPane().add(this.xttTree);
    this.xttTree.createUserInputs();
    Logg.logg("JopXttApplet: XttTree-skapande klart", 6);

    class WindowCatcher extends WindowAdapter
    {
        public void windowClosing( WindowEvent e )
	{
	  setVisible( false);
        }
    }
    addWindowListener( new WindowCatcher());
    setDefaultCloseOperation( DO_NOTHING_ON_CLOSE);
    if(oid != null)
    {
      this.xttTree.find(oid);
    }
  }

  public static void main(String[] args)
  {
    boolean debug = false;
    for(int i = 0; i < args.length; i++)
    {
      if(args[i].equals("-d") || args[i].equals("-D"))
      {
        debug = true;
      }
    }
    JopXttFrame frame = new JopXttFrame();
    frame.pack();
    frame.setVisible(true);
  }

}

