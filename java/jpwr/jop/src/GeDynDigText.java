package jpwr.jop;
import jpwr.rt.*;

public class GeDynDigText extends GeDynElem {
  String attribute;
  String lowText;

  boolean attrFound;
  PwrtRefId subid;
  int p;
  boolean inverted;
  boolean oldValue;
  boolean firstScan = true;
  String highText;

  public GeDynDigText( GeDyn dyn, String attribute, String lowText) {
    super( dyn, GeDyn.mDynType_Invisible, GeDyn.mActionType_No);
    this.attribute = attribute;
    this.lowText = lowText;
  }
  public void connect() {
    String attrName = dyn.getAttrName( attribute);
    if ( attrName.compareTo("") != 0) {
      GdhrRefObjectInfo ret = dyn.en.gdh.refObjectInfo( attrName);
      if ( ret.evenSts())
	System.out.println( "DigText: " + attrName);
      else {
	attrFound = true;
	p = ret.id;
	subid = ret.refid;
	inverted = GeDyndata.getAttrInverted( attribute);
	highText = dyn.comp.getAnnot1();
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

    if ( (!inverted && value) || (inverted && !value))
      dyn.comp.setAnnot1( highText);
    else
      dyn.comp.setAnnot1( lowText);
    dyn.repaintNow = true;
    oldValue = value;
  }
}

