package jpwr.jop;
import jpwr.rt.*;
import java.awt.event.*;

public class GeDynStoDig extends GeDynElem {
  String attribute;

  public GeDynStoDig( GeDyn dyn, String attribute) {
    super( dyn, GeDyn.mDynType_No, GeDyn.mActionType_StoDig);
    this.attribute = attribute;
  }
  public void action( int eventType, MouseEvent e) {
    String attrName;

    switch ( eventType) {
    case GeDyn.eEvent_MB1Down:
      dyn.comp.setColorInverse( 1);
      dyn.repaintNow = true;

      attrName = dyn.getAttrName( attribute);
      PwrtStatus sts = dyn.en.gdh.setObjectInfo( attrName, true);
      if ( sts.evenSts())
	System.out.println( "StoDig: " + attrName);
      break;
    case GeDyn.eEvent_MB1Up:
      dyn.comp.setColorInverse( 0);
      dyn.repaintNow = true;

      attrName = dyn.getAttrName( attribute);        
      dyn.en.gdh.setObjectInfo( attrName, false);
      break;
    }
  }
}

