package jpwr.jop;
import jpwr.rt.*;

public interface JopSessionIfc {
  public void setSession( Object session);
  public void setRoot( Object root);
  public Object getRoot();
  public void setEngine( JopEngine engine);
  public JopEngine getEngine();
  public void setNavigator( Object navigator);
  public void setAlarmList( Object alarmList);
  public void addUtility( Object utility);
  public void removeUtility( Object utility);
  public void openNavigator( PwrtObjid oid);
  public void openAlarmList();
  public void openLogin();
  public void openGraphFrame( String name, String instance, boolean scrollbar);
  public void openFlowFrame( PwrtObjid oid, String center);
  public void openCrrFrame( String name);
  public void executeCommand( String command);
  public boolean isApplet();
  public boolean isApplication();
  public boolean isOpWindowApplet();
}
