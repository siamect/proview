package jpwr.jop;
import jpwr.rt.*;
import javax.swing.*;

public class GeDynFillLevel extends GeDynElem {
  String attribute;
  int color;
  int direction;
  double minValue;
  double maxValue;
  double minPos;
  double maxPos;

  boolean attrFound;
  PwrtRefId subid;
  int p;
  float oldValue;
  boolean firstScan = true;

  public GeDynFillLevel( GeDyn dyn, String attribute, int color, int direction,
			 double minValue, double maxValue, double minPos, double maxPos) {
    super( dyn, GeDyn.mDynType_FillLevel, GeDyn.mActionType_No);
    this.attribute = attribute;
    this.color = color;
    this.direction = direction;
    this.minValue = minValue;
    this.maxValue = maxValue;
    this.minPos = minPos;
    this.maxPos = maxPos;
  }
  public void connect() {
    if ( minValue == maxValue)
      return;

    String attrName = dyn.getAttrName( attribute);
    if ( attrName.compareTo("") != 0) {
      GdhrRefObjectInfo ret = dyn.en.gdh.refObjectInfo( attrName);
      if ( ret.evenSts())
	System.out.println( "FillLevel: " + attrName);
      else {
	attrFound = true;
	p = ret.id;
	subid = ret.refid;
	if ( maxPos == 0 && minPos == 0)
	  dyn.comp.setLevelDirection( direction);
	else
	  dyn.comp.setLevelDirection( Ge.DIRECTION_DOWN);
	if ( (dyn.dynType & GeDyn.mDynType_Tone) != 0) {
	  if ( color <= GeColor.COLOR_TONE_MAX)
	    dyn.comp.setLevelColorTone( color);
	  else
	    dyn.comp.setLevelFillColor( color);
	}
	else
	  dyn.comp.setLevelFillColor( color);
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
    float value = dyn.en.gdh.getObjectRefInfoFloat( p);
    if ( !firstScan) {
      if ( value == oldValue)
	// No change since last time
	return;
    }
    else
      firstScan = false;

    float level = 1;
    if ( maxPos == 0 && minPos == 0) {
      level = (float)((value - minValue) / (maxValue - minValue));
    }
    else {
      switch( direction) {
      case Ge.DIRECTION_LEFT:
      case Ge.DIRECTION_RIGHT:
	float width = ((JComponent)dyn.comp).getWidth();
	level = (float)(((value - minValue) / (maxValue - minValue) * ( maxPos - minPos)
			+ width - maxPos) / width);
	break;
      case Ge.DIRECTION_DOWN:
      case Ge.DIRECTION_UP:
	float height = ((JComponent)dyn.comp).getHeight();
	level = (float)(((value - minValue) / (maxValue - minValue) * ( maxPos - minPos)
			+ height - maxPos) / height);
	break;
      }
    }
    dyn.comp.setFillLevel( level);
    dyn.repaintNow = true;
    oldValue = value;
  }
}









