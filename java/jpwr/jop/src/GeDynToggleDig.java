package jpwr.jop;
import jpwr.rt.*;
import java.awt.event.*;

public class GeDynToggleDig extends GeDynElem {
  String attribute;

  public GeDynToggleDig( GeDyn dyn, String attribute) {
    super( dyn, GeDyn.mDynType_No, GeDyn.mActionType_ToggleDig);
    this.attribute = attribute;
  }
  public void action( int eventType, MouseEvent e) {
    switch ( eventType) {
    case GeDyn.eEvent_MB1Down:
      dyn.comp.setColorInverse( 1);
      dyn.repaintNow = true;
      break;
    case GeDyn.eEvent_MB1Up:
      dyn.comp.setColorInverse( 0);
      dyn.repaintNow = true;
      break;
    case GeDyn.eEvent_MB1Click:
      if ( (dyn.actionType & GeDyn.mActionType_Confirm) != 0)
	break;

      String attrName = dyn.getAttrName( attribute);        
      PwrtStatus sts = dyn.en.gdh.toggleObjectInfo( attrName);
      if ( sts.evenSts())
	System.out.println( "ToggleDig: " + attrName);
      break;
    }
  }
}









