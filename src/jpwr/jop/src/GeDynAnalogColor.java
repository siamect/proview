package jpwr.jop;
import jpwr.rt.*;

public class GeDynAnalogColor extends GeDynElem {
  String attribute;
  double limit;
  int limitType;
  int color;

  boolean attrFound;
  PwrtRefId subid;
  int p;
  float oldValue;
  boolean firstScan = true;
  boolean oldState;
  boolean isMainInstance = false;

  public GeDynAnalogColor( GeDyn dyn, String attribute, 
			   double limit, int limitType, int color) {
    super( dyn, GeDyn.mDynType_AnalogColor, GeDyn.mActionType_No);
    this.attribute = attribute;
    this.limit = limit;
    this.limitType = limitType;
    this.color = color;
  }
  public void connect() {
    // Find the main instance
    GeDynAnalogColor mainInstance = null;
    for ( int i = dyn.elements.length - 1; i >= 0; i--) {
      if ( dyn.elements[i].getDynType() == GeDyn.mDynType_AnalogColor) {
	mainInstance = (GeDynAnalogColor)dyn.elements[i];
	break;
      }
    }

    if ( mainInstance != null) {
      if ( !mainInstance.attrFound) {
        String attrName = dyn.getAttrName( mainInstance.attribute);
        if ( attrName.compareTo("") != 0) {
          GdhrRefObjectInfo ret = dyn.en.gdh.refObjectInfo( attrName);
          if ( ret.evenSts())
	    System.out.println( "AnalogColor: " + attrName);
          else
	    mainInstance.attrFound = true;
	  mainInstance.p = ret.id;
	  mainInstance.subid = ret.refid;
	  mainInstance.isMainInstance = true;
        }
      }
      p = mainInstance.p;
      attrFound = mainInstance.attrFound;
    }
  }
  public void disconnect() {
    if ( isMainInstance && attrFound)
      dyn.en.gdh.unrefObjectInfo( subid);
  }
  public void scan() {
    if ( !attrFound || dyn.ignoreColor)
      return;

    float value = dyn.en.gdh.getObjectRefInfoFloat( p);
    int i;
    if ( !firstScan) {
      if ( !dyn.resetColor && value == oldValue) {
	if ( oldState)
	  dyn.ignoreColor = true;
	return;
      }
    }
    else
      firstScan = false;
  
    boolean state;
    boolean set_color = false;
    boolean reset_color = false;

    if ( limitType == GeDyn.eLimitType_Gt)
      state = value > limit;
    else
      state = value < limit;

    if ( state != oldState || dyn.resetColor || firstScan) {
      if ( state) {
	set_color = true;
	dyn.ignoreColor = true;
      }
      else {
	reset_color = true;
	dyn.resetColor = true;
      }
      oldState = state;
    }
    else if ( state)
      dyn.ignoreColor = true;

    oldValue = value;

    if ( !set_color && !reset_color)
      return;

    if ( (dyn.dynType & GeDyn.mDynType_Tone) != 0) {
      if ( set_color) {
	if ( color <= GeColor.COLOR_TONE_MAX)
	  dyn.comp.tsetColorTone( color);
	else
	  dyn.comp.tsetFillColor( color);
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
      if ( set_color) {
	dyn.comp.tsetFillColor( color);
	dyn.ignoreColor = true;
      }
      else {
	dyn.comp.resetFillColor();
	dyn.resetColor = true;
      }
      dyn.repaintNow = true;
    }
  }
}




