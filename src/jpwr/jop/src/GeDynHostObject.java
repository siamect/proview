package jpwr.jop;
import jpwr.rt.*;

public class GeDynHostObject extends GeDynElem {

  public GeDynHostObject( GeDyn dyn, String hostObject) {
    super( dyn, GeDyn.mDynType_HostObject, GeDyn.mActionType_No);
    dyn.setHostObject( hostObject);
  }
  public void connect() {
  }
  public void disconnect() {
  }
  public void scan() {
  }
}









