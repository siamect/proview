package jpwr.jop;
import jpwr.rt.*;

public class GeDynDigBorder extends GeDynElem {
  String attribute;
  int color;

  boolean attrFound;
  PwrtRefId subid;
  int p;
  boolean inverted;
  boolean oldValue;
  boolean firstScan = true;

  public GeDynDigBorder( GeDyn dyn, String attribute, int color) {
    super( dyn, GeDyn.mDynType_DigBorder, GeDyn.mActionType_No);
    this.attribute = attribute;
    this.color = color;
  }
  public void connect() {
    String attrName = dyn.getAttrName( attribute);
    if ( attrName.compareTo("") != 0) {
      GdhrRefObjectInfo ret = dyn.en.gdh.refObjectInfo( attrName);
      if ( ret.evenSts())
	System.out.println( "DigColor: " + attrName);
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

    if ( (!inverted && !value) || (inverted && value)) {
      dyn.comp.tsetBorderColor( color);
    }
    else {
      dyn.comp.resetBorderColor();
    }
    dyn.repaintNow = true;
    oldValue = value;
  }
}
