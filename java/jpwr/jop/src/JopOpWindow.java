package jpwr.jop;
import jpwr.rt.*;
import java.awt.*;
import javax.swing.*;
import java.awt.event.*;

public class JopOpWindow extends JPanel {

  JopSession session;
  JopEngine en;
  Object root;

  public JopOpWindow( JopSession session, Object root) {
    this.session = session;
    this.root = root;
    en = session.getEngine();

    CdhrObjid oretWebH = en.gdh.getClassList( Pwrb.cClass_WebHandler);
    if ( oretWebH.evenSts()) return;

    CdhrString sret = en.gdh.objidToName( oretWebH.objid, Cdh.mName_volumeStrict);            
    if ( sret.evenSts()) return;

    String s = sret.str + ".Title";
    CdhrString srettxt = en.gdh.getObjectInfoString( s);
    if ( srettxt.evenSts()) return;

    JLabel label = new JLabel( srettxt.str);
    Font f = new Font("Helvetica", Font.BOLD, 24);
    label.setFont( f);
    this.add( label);

    s = sret.str + ".Text";
    srettxt = en.gdh.getObjectInfoString( s);
    if ( srettxt.evenSts()) return;

    label = new JLabel( srettxt.str);
    f = new Font("Helvetica", Font.BOLD, 16);
    label.setFont( f);
    this.add( label);

    this.add( new JSeparator());

    s = sret.str + ".EnableLogin";
    CdhrInt iret = en.gdh.getObjectInfoInt( s);
    if ( iret.evenSts()) return;

    OpWindButton button;
    if ( iret.value != 0) {
      button = new OpWindButton( session, "", "Login",
				 OpWindButton.LOGIN);
      this.add( button);
    }
      
    s = sret.str + ".EnableAlarmList";
    iret = en.gdh.getObjectInfoInt( s);
    if ( iret.evenSts()) return;

    if ( iret.value != 0) {
      button = new OpWindButton( session, "", "Alarm and Eventlist",
				 OpWindButton.ALARMLIST);
      this.add( button);
    }
      
    s = sret.str + ".EnableNavigator";
    iret = en.gdh.getObjectInfoInt( s);
    if ( iret.evenSts()) return;

    if ( iret.value != 0) {
      button = new OpWindButton( session, "", "Navigator",
				 OpWindButton.NAVIGATOR);
      this.add( button);
    }
      
    button = new OpWindButton( session, "", "Help",
			       OpWindButton.HELP);
    this.add( button);
    this.add( new JSeparator());

    CdhrString sretName = null;
    CdhrString sretText = null;
    CdhrString sretURL = null;

    CdhrObjid oret = en.gdh.getChild( oretWebH.objid);
    while ( oret.oddSts()) {
      CdhrClassId retCid = en.gdh.getObjectClass( oret.objid);
      if ( retCid.evenSts()) return;

      switch( retCid.classId) {
        case Pwrb.cClass_WebGraph:
          sret = en.gdh.objidToName( oret.objid, Cdh.mName_volumeStrict);            
          if ( sret.evenSts()) return;

          s = sret.str + ".Name";
          sretName = en.gdh.getObjectInfoString( s);

          s = sret.str + ".Text";
          sretText = en.gdh.getObjectInfoString( s);

 
          button = new OpWindButton( session, sretName.str, sretText.str,
						  OpWindButton.WEBGRAPH);
          this.add( button);
          break;
      }
      oret = en.gdh.getNextSibling( oret.objid);
    }

    this.add( new JSeparator());

    oret = en.gdh.getChild( oretWebH.objid);
    while ( oret.oddSts()) {
      CdhrClassId retCid = en.gdh.getObjectClass( oret.objid);
      if ( retCid.evenSts()) return;

      switch( retCid.classId) {
        case Pwrb.cClass_WebLink:
          sret = en.gdh.objidToName( oret.objid, Cdh.mName_volumeStrict);
          if ( sret.evenSts()) return;

          s = sret.str + ".URL";
          sretURL = en.gdh.getObjectInfoString( s);

          s = sret.str + ".Text";
          sretText = en.gdh.getObjectInfoString( s);

 
          button = new OpWindButton( session, sretURL.str, sretText.str,
						  OpWindButton.WEBLINK);
          this.add( button);
          break;
      }
      oret = en.gdh.getNextSibling( oret.objid);
    }
  }

  class OpWindButton extends JButton {
    public static final int WEBGRAPH = 1;
    public static final int WEBLINK = 2;
    public static final int LOGIN = 3;
    public static final int NAVIGATOR = 4;
    public static final int ALARMLIST = 5;
    public static final int HELP = 6;
    JopSession session;
    String action;
    int type;
    boolean scrollbar;
    String instance;

    public OpWindButton( JopSession bsession, String name, String text, int btype) {
      this.session = bsession;
      this.action = name;
      this.type = btype;
      setText( text);
      this.addMouseListener(new MouseAdapter() {
        public void mouseReleased(MouseEvent e) {
	  switch ( type) {
	    case NAVIGATOR:
	      session.openNavigator( null);
	      break;
	    case LOGIN:
	      session.openLogin();
	      break;
	    case ALARMLIST:
	      session.openAlarmList();
	      break;
	    case HELP:
	      session.executeCommand("help index");
	      break;
	    case WEBGRAPH:
	      session.openGraphFrame( action, instance, scrollbar);
	      break;
	    case WEBLINK:
	      String cmd = "open url " + action;
	      session.executeCommand( cmd);
	      break;
	  }
	  System.out.println( "Action: " + action);
	}
	  });
    }

    public void setScrollbar( boolean scrollbar) {
      this.scrollbar = scrollbar;
    }
    public void setInstance( String instance) {
      this.instance = instance;
    }

  }
}













