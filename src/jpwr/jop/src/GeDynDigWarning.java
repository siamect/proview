package jpwr.jop;
import jpwr.rt.*;

public class GeDynDigWarning extends GeDynElem {
  String attribute;

  boolean attrFound;
  PwrtRefId subid;
  int p;
  boolean inverted;
  boolean oldValue;
  boolean firstScan = true;

  public GeDynDigWarning( GeDyn dyn, String attribute) {
    super( dyn, GeDyn.mDynType_DigWarning, GeDyn.mActionType_No);
    this.attribute = attribute;
  }
  public void connect() {
    String attrName = dyn.getAttrName( attribute);
    if ( attrName.compareTo("") != 0) {
      GdhrRefObjectInfo ret = dyn.en.gdh.refObjectInfo( attrName);
      if ( ret.evenSts())
	System.out.println( "DigWarning: " + attrName);
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
    if ( !attrFound || dyn.ignoreColor)
      return;
    boolean value = dyn.en.gdh.getObjectRefInfoBoolean( p);
    if ( !firstScan) {
      if ( value == oldValue && !dyn.resetColor) {
	// No change since last time
	if ( (!inverted && value) || (inverted && !value))
	  dyn.ignoreColor = true;
	return;
      }
    }
    else
      firstScan = false;

    if ( (dyn.dynType & GeDyn.mDynType_Tone) != 0) {
      if ( (!inverted && value) || (inverted && !value)) {
 	dyn.comp.tsetColorTone( GeColor.COLOR_TONE_YELLOW);
	dyn.ignoreColor = true;
      }
      else {
	dyn.comp.resetFillColor();
	dyn.comp.resetColorTone();
	dyn.resetColor = true;
      }
      dyn.repaintNow = true;
    }
    else {
      if ( (!inverted && value) || (inverted && !value)) {
	dyn.comp.tsetFillColor( GeColor.COLOR_6);
	dyn.ignoreColor = true;
      }
      else {
	dyn.comp.resetFillColor();
	dyn.resetColor = true;
      }
      dyn.repaintNow = true;
    }
    oldValue = value;
  }
}
