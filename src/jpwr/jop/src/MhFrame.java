package jpwr.jop;
import jpwr.rt.*;
//import Logg;
import java.awt.*;
import java.awt.event.*;
import java.awt.font.*;
import java.awt.geom.*;
import java.awt.image.*;
import javax.swing.event.*;
import java.io.*;
import java.net.*;
import java.util.*;
import javax.swing.*;
import javax.swing.table.AbstractTableModel;
import javax.swing.JOptionPane;
import javax.swing.SwingUtilities;
import java.awt.event.*;


/**
 *  Description of the Class
 *
 *@author     JN3920
 *@created    November 12, 2002
 */
public class MhFrame extends JFrame
{
  JopSession session;
  JopEngine engine;
  XttTree xttTree;
  Object root;
  
  /**  Description of the Field */
  BorderLayout borderLayout1 = new BorderLayout();

  /**  Description of the Field */
  BorderLayout borderLayout5 = new BorderLayout();

  /**  Description of the Field */
  JPanel contentPane;
  /**  Description of the Field */
  JLabel labelMessage = new JLabel("MhClient version 1.0");

  /**  Description of the Field */
  JPanel messagePanel = new JPanel();

  /**  Description of the Field */
  boolean sim = false;

  /**  Description of the Field */
  Dimension size;

  /**  Description of the Field */
  JPanel userPanel = new JPanel();


  /**  Description of the Field */
  MhTable mhTable;


  /**  Constructor for the MhClient object */
/*
  public MhFrame() 
  {
    addWindowListener(
      new WindowAdapter()
      {
        public void windowClosing(WindowEvent e)
        {
          destroy();
          System.exit(0);
        }
      });
      
    this.init(); 
  }
*/

  public MhFrame()
  {
    engine = new JopEngine( 1000, (Object)this);
    session = new JopSession( engine, (Object)this);
    root = (Object) this;
    init();

  }
  public MhFrame( JopSession session)
  {
    this.session = session;
    engine = session.getEngine();
    root = session.getRoot();
    init();
  }  
  
  /**  Description of the Method */
  public void destroy()
  {
    if(mhTable != null)
      mhTable.close();
  /*  
    if(!sim)
    {
      super.destroy();
    }
  */
  }


  /**  Description of the Method */
  public void init()
  {
    //super.init();
    size = new Dimension(570, 570);
    contentPane = (JPanel)this.getContentPane();
    contentPane.setLayout(borderLayout1);


    Dimension d = messagePanel.getSize();
    d.height += 20;
    messagePanel.setPreferredSize(d);
    contentPane.add(messagePanel, BorderLayout.SOUTH);

    contentPane.setOpaque(true);
    userPanel.setLayout(new GridBagLayout());
    GridBagConstraints gbc = new GridBagConstraints();
    gbc.anchor = GridBagConstraints.WEST;
    gbc.insets = new Insets(2, 2, 2, 2);

    messagePanel.setLayout(borderLayout5);
    contentPane.setOpaque(true);
    contentPane.setBackground(Color.white);
    this.setSize(size);

    messagePanel.add(labelMessage, BorderLayout.NORTH);
    //Logg.logg("MhClient: Före XttTree-skapande", 6);
    this.mhTable = new MhTable(root, false, this.labelMessage);
    //Logg.logg("MhClient: mhTable-skapande klart", 6);
    this.contentPane.add(this.mhTable.splitPane, BorderLayout.CENTER);

  }
  
  public void setMess(String mess, boolean error)
  {
    /*
    if(error)
    {
      this.labelMessage.setColor(Color.red);
    }
    else
    {
      this.labelMessage.setColor(Color.blue);
    }
    */
    this.labelMessage.setText(mess);
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

    MhFrame frame = new MhFrame();
    frame.pack();
    frame.setVisible(true);
  }
}

