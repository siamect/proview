package jpwr.jop;
import jpwr.rt.*;
import java.awt.event.*;

public class GeDynIncrAnalog extends GeDynElem {
  String attribute;
  double increment;
  double min_value;
  double max_value;

  public GeDynIncrAnalog( GeDyn dyn, String attribute, double increment, double min_value,
			  double max_value) {
    super( dyn, GeDyn.mDynType_No, GeDyn.mActionType_IncrAnalog);
    this.attribute = attribute;
    this.increment = increment;
    this.min_value = min_value;
    this.max_value = max_value;
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
      CdhrFloat ret = dyn.en.gdh.getObjectInfoFloat( attrName);
      if ( ret.evenSts()) {
	System.out.println( "IncrAnalog " + attrName);
	break;
      }
      ret.value += increment;
      if ( !( min_value == 0 && max_value == 0)) {
	if ( ret.value < min_value)
	  ret.value = (float) min_value;
	if ( ret.value > max_value)
	  ret.value = (float) max_value;
      }
      PwrtStatus sts = dyn.en.gdh.setObjectInfo( attrName, ret.value);
      if ( sts.evenSts())
	System.out.println( "IncrAnalog " + attrName);
      break;
    }
  }
}

