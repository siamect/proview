package jpwr.jop;
import jpwr.rt.*;

public class GeDynStatusColor extends GeDynElem {
  String attribute;
  int nostatusColor;

  boolean attrFound;
  PwrtRefId subid;
  int p;
  boolean inverted;
  int oldValue;
  boolean firstScan = true;
  boolean on;
  int oldStatus;

  public static final int NO			= 0;
  public static final int SUCCESS	       	= 1;
  public static final int WARNING	        = 2;
  public static final int ERROR			= 3;
  public static final int FATAL			= 4;

  public GeDynStatusColor( GeDyn dyn, String attribute, int nostatusColor) {
    super( dyn, GeDyn.mDynType_DigLowColor, GeDyn.mActionType_No);
    this.attribute = attribute;
    this.nostatusColor = nostatusColor;
  }
  public void connect() {
    String attrName = dyn.getAttrName( attribute);
    if ( attrName.compareTo("") != 0) {
      GdhrRefObjectInfo ret = dyn.en.gdh.refObjectInfo( attrName);
      if ( ret.evenSts())
	System.out.println( "StatusColor: " + attrName);
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
    int value = dyn.en.gdh.getObjectRefInfoInt( p);
    int stsValue;

    if ( value == 0)
      stsValue = NO;
    else {
      switch ( value & 7) {
      case 3:
      case 1:
	stsValue = SUCCESS;
	break;
      case 0:
	stsValue = WARNING;
	break;
      case 2:
	stsValue = ERROR;
	break;
      case 4:
	stsValue = FATAL;
	break;
      default:
	stsValue = NO;
      }
    }

    if ( !firstScan && oldStatus != FATAL) {
      if ( oldValue == value && !dyn.resetColor)
	return;
    }
    else
      firstScan = false;
	
    if ( (dyn.dynType & GeDyn.mDynType_Tone) != 0) {
      switch( stsValue) {
      case NO:
	if ( nostatusColor <= GeColor.COLOR_TONE_MAX)
	  dyn.comp.tsetColorTone( nostatusColor);
	else
 	  dyn.comp.tsetFillColor( nostatusColor);
	break;
      case SUCCESS:
	dyn.comp.resetFillColor();
	dyn.comp.resetColorTone();
	break;
      case WARNING:
	dyn.comp.tsetColorTone( GeColor.COLOR_TONE_YELLOW);
	break;
      case ERROR:
	dyn.comp.tsetColorTone( GeColor.COLOR_TONE_RED);
	break;
      case FATAL:
	on = !on;
	if ( on)
	  dyn.comp.tsetColorTone( GeColor.COLOR_TONE_RED);
	else {
	  if ( nostatusColor <= GeColor.COLOR_TONE_MAX)
	    dyn.comp.tsetColorTone( nostatusColor);
	  else
	    dyn.comp.tsetFillColor( nostatusColor);
	}
	break;
      }
    }
    else {
      switch( stsValue) {
      case NO:
	dyn.comp.tsetFillColor( nostatusColor);
	break;
      case SUCCESS:
	dyn.comp.resetFillColor();
	break;
      case WARNING:
	dyn.comp.tsetFillColor( GeColor.COLOR_6);
	break;
      case ERROR:
	dyn.comp.tsetFillColor( GeColor.COLOR_RED);
	break;
      case FATAL:
	on = !on;
	if ( on)
	  dyn.comp.tsetFillColor( GeColor.COLOR_RED);
	else
	  dyn.comp.tsetFillColor( nostatusColor);
	break;
      }
    }
    dyn.repaintNow = true;
    oldValue = value;
    oldStatus = stsValue;
  }

}














