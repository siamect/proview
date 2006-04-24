/* 
 * Proview   $Id: CrrFrame.java,v 1.3 2006-04-24 13:21:46 claes Exp $
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
import javax.swing.event.*;
import javax.swing.border.*;
import java.awt.event.*;
import java.util.*;

public class CrrFrame extends JFrame {
  JScrollPane scrollPane;
  JPanel contentPane;
  JMenuBar menuBar;
  JList crrList;
  BorderLayout borderLayout1 = new BorderLayout();
  JopSession session;
  JopEngine en;
  Object root;
  private String titles[];
  private ImageIcon images[];

  public CrrFrame( JopSession sess, String objectName) {
    this.session = sess;
    en = session.getEngine();
    root = session.getRoot();
    contentPane = (JPanel) this.getContentPane();
    contentPane.setLayout(borderLayout1);
    crrList = createList(objectName);
    menuBar = createMenu();
    contentPane.add( menuBar, BorderLayout.NORTH);
    scrollPane = new JScrollPane( crrList);
    contentPane.add(scrollPane, BorderLayout.CENTER);
    contentPane.setOpaque(true);
    this.setTitle(objectName);
    setSize( 600, 300);
  }

  JMenuBar createMenu() {
    menuBar = new JMenuBar();
    JMenu fileMenu = new JMenu("File");
    fileMenu.setMnemonic('F');
    MenuAction closeFileAction = new MenuAction("Close");

    JMenu functionsMenu = new JMenu("Functions");
    fileMenu.setMnemonic('u');
    MenuAction openPlcFunctionsAction = new MenuAction("Open Plc");

    JMenuItem item;
    item = fileMenu.add( closeFileAction);
    item.setMnemonic('C');
    item = functionsMenu.add( openPlcFunctionsAction);
    item.setMnemonic('I');
    item.setAccelerator( KeyStroke.getKeyStroke('L', java.awt.Event.CTRL_MASK, false));

    menuBar.add( fileMenu);
    menuBar.add( functionsMenu);
    menuBar.setBorder( new BevelBorder( BevelBorder.RAISED));

    return menuBar;
  }

  class MenuAction extends AbstractAction {
    public MenuAction( String text) {
      super(text);
    }

    public void actionPerformed( ActionEvent e) {
      if ( e.getActionCommand().equals("Close")) {
	dispose();
      }
      else if ( e.getActionCommand().equals("Open Plc")) {
	int selected[] = crrList.getSelectedIndices();
	if ( selected.length == 0)
	  return;
	StringTokenizer t = new StringTokenizer( titles[selected[0]]);
	String name = t.nextToken();
	int idx = name.lastIndexOf('-');
	String windowName = name.substring( 0, idx);
	String segmentName = name.substring( idx + 1);
	CdhrObjid ret = en.gdh.nameToObjid( windowName);
	if ( ret.evenSts())
	  return;
        session.openFlowFrame( ret.objid, segmentName);
      }
    }
  }

  // Exit when window is closed
  protected void processWindowEvent(WindowEvent e) {
    super.processWindowEvent(e);
    if (e.getID() == WindowEvent.WINDOW_CLOSING) {
    }
  }

  JList createList( String objectName) {
      CdhrObjid oret = en.gdh.nameToObjid( objectName);
      if ( oret.evenSts()) return null;

      CdhrClassId cret = en.gdh.getObjectClass( oret.objid);
      if ( cret.evenSts()) return null;

      CdhrString sret;
      switch (  cret.classId) {
      case Pwrb.cClass_Ai:
      case Pwrb.cClass_Ao:
      case Pwrb.cClass_Di:
      case Pwrb.cClass_Do:
      case Pwrb.cClass_Po:
      case Pwrb.cClass_Dv:
      case Pwrb.cClass_Av:
        sret = en.gdh.crrSignal( objectName);
        break;
      default:
        sret = en.gdh.crrObject( objectName);
      }
      if ( sret.evenSts()) return null;

      Image crrRead = JopSpider.getImage( session, "jpwr/jop/crrread.gif");
      Image crrWrite = JopSpider.getImage( session, "jpwr/jop/crrwrite.gif");
      Image crrReadWrite = JopSpider.getImage( session, "jpwr/jop/crrreadwrite.gif");
      ImageIcon crrReadIcon = new ImageIcon( crrRead);
      ImageIcon crrWriteIcon = new ImageIcon( crrWrite);
      ImageIcon crrReadWriteIcon = new ImageIcon( crrReadWrite);

      String delim = new String( new char[] { '\n'});
      StringTokenizer tokens = new StringTokenizer( sret.str, delim);

      int tokensCnt = tokens.countTokens();
      titles = new String[tokensCnt];
      images = new ImageIcon[tokensCnt];
      for ( int i = 0; i < tokensCnt; i++) {
	String token = tokens.nextToken();
	titles[i] = token.substring(1);
        if ( token.startsWith("0"))
	  images[i] = crrReadIcon;
	else if ( token.startsWith("1"))
	  images[i] = crrWriteIcon;
	else
	  images[i] = crrReadWriteIcon;
      }
      final JList list = new JList( titles);
      MouseListener mouseListener = new MouseAdapter() {
	public void mouseClicked( MouseEvent e) {
	  if ( e.getClickCount() == 2) {
	    int index = list.locationToIndex( e.getPoint());
	    StringTokenizer t = new StringTokenizer( titles[index]);
	    String name = t.nextToken();
	    int idx = name.lastIndexOf('-');
	    String windowName = name.substring( 0, idx);
	    String segmentName = name.substring( idx + 1);
	    CdhrObjid ret = en.gdh.nameToObjid( windowName);
	    if ( ret.evenSts())
	      return;
            session.openFlowFrame( ret.objid, segmentName);
	    dispose();
	  }
	}
      };
      list.addMouseListener( mouseListener);
      list.setCellRenderer( new CrrCellRenderer());
      return list;
  }

  class CrrCellRenderer extends JLabel implements ListCellRenderer {
      Color highlightColor = new Color( 230, 230, 255);

      CrrCellRenderer() {
	  setOpaque( true);
      }
      public Component getListCellRendererComponent( JList list,
						     Object object,
						     int index,
						     boolean isSelected,
						     boolean cellHasFocus) {
	  if ( index == -1) {
	      int selected = list.getSelectedIndex();
	      if ( selected == -1)
		  return this;
	      else
		  index = selected;
	  }
	  setText( " " + titles[index]);
	  setIcon( images[index]);
	  if ( isSelected) {
	    setBackground( highlightColor);
	    setForeground( Color.black);
	  }
	  else {
	    setBackground( Color.white);
	    setForeground( Color.black);
	  }
	  return this;
      }
  }
}





