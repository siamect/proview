package jpwr.jop;
import jpwr.rt.*;

public class GeDynDigLowColor extends GeDynElem {
  String attribute;
  int color;

  boolean attrFound;
  PwrtRefId subid;
  int p;
  boolean inverted;
  boolean oldValue;
  boolean firstScan = true;

  public GeDynDigLowColor( GeDyn dyn, String attribute, int color) {
    super( dyn, GeDyn.mDynType_DigLowColor, GeDyn.mActionType_No);
    this.attribute = attribute;
    this.color = color;
  }
  public void connect() {
    String attrName = dyn.getAttrName( attribute);
    if ( attrName.compareTo("") != 0) {
      GdhrRefObjectInfo ret = dyn.en.gdh.refObjectInfo( attrName);
      if ( ret.evenSts())
	System.out.println( "DigLowColor: " + attrName);
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
    System.out.println("Disconnecting: " + attribute);
  }
  public void scan() {
    if ( !attrFound || dyn.ignoreColor)
      return;
    boolean value = dyn.en.gdh.getObjectRefInfoBoolean( p);
    if ( !firstScan) {
      if ( oldValue == value && !dyn.resetColor)
	return;
    }
    else
      firstScan = false;
	
    if ( (dyn.dynType & GeDyn.mDynType_Tone) != 0) {
      if ( (!inverted && !value) || (inverted && value)) {
	if ( color <= GeColor.COLOR_TONE_MAX)
	  dyn.comp.tsetColorTone( color);
	else
 	  dyn.comp.tsetFillColor( color);
      }
      else {
	dyn.comp.resetFillColor();
	dyn.comp.resetColorTone();
      }
      dyn.repaintNow = true;
    }
    else {
      if ( (!inverted && !value) || (inverted && value))
	dyn.comp.tsetFillColor( color);
      else
	dyn.comp.resetFillColor();
      dyn.repaintNow = true;
    }
    oldValue = value;
  }
}














