package jpwr.jop;
import jpwr.rt.*;
import java.awt.*;
import java.awt.event.*;

public class GeDynConfirm extends GeDynElem {
  String text;

  public GeDynConfirm( GeDyn dyn, String text) {
    super( dyn, GeDyn.mDynType_No, GeDyn.mActionType_Confirm);
    this.text = text;
  }
  public void action( int eventType, MouseEvent e) {
    switch ( eventType) {
    case GeDyn.eEvent_MB1Down:
      break;
    case GeDyn.eEvent_MB1Click:
    case GeDyn.eEvent_ValueChanged:
      JopConfirmDialog.open( (Component)dyn.comp, text);
      break;
    }
  }
}


