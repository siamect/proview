package jpwr.jop;
import jpwr.rt.*;

public class GeDynInvisible extends GeDynElem {
  String attribute;

  boolean attrFound;
  PwrtRefId subid;
  int p;
  boolean inverted;
  boolean oldValue;
  boolean firstScan = true;

  public GeDynInvisible( GeDyn dyn, String attribute) {
    super( dyn, GeDyn.mDynType_Invisible, GeDyn.mActionType_No);
    this.attribute = attribute;
  }
  public void connect() {
    String attrName = dyn.getAttrName( attribute);
    if ( attrName.compareTo("") != 0) {
      GdhrRefObjectInfo ret = dyn.en.gdh.refObjectInfo( attrName);
      if ( ret.evenSts())
	System.out.println( "DigInvisible: " + attrName);
      else {
	attrFound = true;
	p = ret.id;
	subid = ret.refid;
	inverted = GeDyndata.getAttrInverted( attribute);
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
    boolean value = dyn.en.gdh.getObjectRefInfoBoolean( p);
    if ( !firstScan) {
      if ( value == oldValue) {
	// No change since last time
	return;
      }
    }
    else
      firstScan = false;

    if ( (!inverted && value) || (inverted && !value)) {
      dyn.invisible = true;
    }
    else {
      dyn.invisible = false;
    }
    dyn.repaintNow = true;
    oldValue = value;
  }
}








