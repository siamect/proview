package jpwr.jop;
import jpwr.rt.*;

public class GeDynAnalogShift extends GeDynElem {
  String attribute;

  boolean attrFound;
  PwrtRefId subid;
  int p;
  float oldValue;
  boolean firstScan = true;

  public GeDynAnalogShift( GeDyn dyn, String attribute) {
    super( dyn, GeDyn.mDynType_AnalogShift, GeDyn.mActionType_No);
    this.attribute = attribute;
  }
  public void connect() {
    String attrName = dyn.getAttrName( attribute);
    if ( attrName.compareTo("") != 0) {
      GdhrRefObjectInfo ret = dyn.en.gdh.refObjectInfo( attrName);
      if ( ret.evenSts())
	System.out.println( "AnalogShift: " + attrName);
      else {
	attrFound = true;
	p = ret.id;
	subid = ret.refid;
      }
    }
  }
  public void disconnect() {
    if ( attrFound)
      dyn.en.gdh.unrefObjectInfo( subid);
  }
  public void scan() {
    if ( !attrFound)
      return;

    float value0 = dyn.en.gdh.getObjectRefInfoFloat( p);
    if ( value0 != oldValue  || firstScan) {
      dyn.comp.setPage( (int)(value0 + 1.5));
      dyn.repaintNow = true;
      oldValue = value0;
    }
    if ( firstScan)
      firstScan = false;
  }
}







