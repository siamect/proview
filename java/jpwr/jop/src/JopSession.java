
package jpwr.jop;
import jpwr.rt.*;
import java.lang.*;

public class JopSession {
  public JopSession() {
  }

  Object sessionRep;

  // New implementation
  public JopSession( JopEngine engine, Object rootObject) {
    // Create a JopSessionRep object which class i not known by the compiler
    try {
      Class clazz = Class.forName( "jpwr.jop.JopSessionRep");
      try {
        sessionRep = clazz.newInstance();
      }
      catch ( InstantiationException e) {
        System.out.println("InstatiationException");
      }
      catch ( IllegalAccessException e) {
        System.out.println("IllegalAccessException");
      }
    }
    catch (ClassNotFoundException e) {
      System.out.println("Class not found");
    }

    ((JopSessionIfc) sessionRep).setSession(this);
    ((JopSessionIfc) sessionRep).setRoot(rootObject);
    ((JopSessionIfc) sessionRep).setEngine(engine);
  }

  public Object getRoot() {
    return ((JopSessionIfc) sessionRep).getRoot();
  }

  public JopEngine getEngine() {
    return ((JopSessionIfc) sessionRep).getEngine();
  }

  public Gdh getGdh() {
    return ((JopSessionIfc) sessionRep).getEngine().gdh;
  }

  public void addUtility( Object utility) {
    ((JopSessionIfc) sessionRep).addUtility( utility);
  }

  public void removeUtility( Object utility) {
    ((JopSessionIfc) sessionRep).removeUtility( utility);
  }

  public void setNavigator( Object navigator) {
    ((JopSessionIfc) sessionRep).setNavigator( navigator);
  }

  public void openNavigator( PwrtObjid oid) {
    ((JopSessionIfc) sessionRep).openNavigator( oid);
  }

  public void openAlarmList() {
    ((JopSessionIfc) sessionRep).openAlarmList();
  }

  public void openLogin() {
    ((JopSessionIfc) sessionRep).openLogin();
  }

  public void openGraphFrame( String name, String instance, boolean scrollbar) {
    ((JopSessionIfc) sessionRep).openGraphFrame( name, instance, scrollbar);
  }

  public void openFlowFrame( PwrtObjid oid, String center) {
    ((JopSessionIfc) sessionRep).openFlowFrame( oid, center);
  }

  public void openCrrFrame( String name) {
    ((JopSessionIfc) sessionRep).openCrrFrame( name);
  }

  public void executeCommand( String command) {
    ((JopSessionIfc) sessionRep).executeCommand( command);
  }

  public boolean isApplet() {
    return ((JopSessionIfc) sessionRep).isApplet();
  }

  public boolean isApplication() {
    return ((JopSessionIfc) sessionRep).isApplication();
  }

  public boolean isOpWindowApplet() {
    return ((JopSessionIfc) sessionRep).isOpWindowApplet();
  }
}









