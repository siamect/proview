package jpwr.jop;
import jpwr.rt.*;
import java.awt.event.*;
import java.awt.*;
import javax.swing.*;

public class GeDynSlider extends GeDynElem {
  String attribute;
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
  boolean moveActive = false;
  Point offset = new Point();

  public GeDynSlider( GeDyn dyn, String attribute, double minValue, double maxValue,
		      int direction, double minPos, double maxPos) {
    super( dyn, GeDyn.mDynType_No, GeDyn.mActionType_Slider);
    this.attribute = attribute;
    this.minValue = minValue;
    this.maxValue = maxValue;
    this.direction = direction;
    this.minPos = minPos;
    this.maxPos = maxPos;
  }
  public void connect() {
    String attrName = dyn.getAttrName( attribute);
    if ( attrName.compareTo("") != 0) {
      GdhrRefObjectInfo ret = dyn.en.gdh.refObjectInfo( attrName);
      if ( ret.evenSts())
	System.out.println( "Slider: " + attrName);
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
    if ( !attrFound || moveActive)
      return;

    float value = dyn.en.gdh.getObjectRefInfoFloat( p);
    if ( oldValue != value || firstScan) {
      Point loc = ((JComponent)dyn.comp).getLocation();
      int pos;
	
      switch ( direction) {
      case Ge.DIRECTION_RIGHT:
	pos = (int)((maxValue - value)/(maxValue - minValue) *
		    (maxPos - minPos) + minPos);
	loc.x = pos;
	break;
      case Ge.DIRECTION_LEFT:
	pos = (int)(value /(maxValue - minValue) *
		    (maxPos - minPos) + minPos);
	loc.x = pos;
	break;
      case Ge.DIRECTION_UP:
	pos = (int)((value - minValue)/(maxValue - minValue) *
		    (maxPos - minPos) + minPos);
	loc.y = pos;
	break;
      default:
	pos = (int)((maxValue - value)/(maxValue - minValue) *
		    (maxPos - minPos) + minPos);
	loc.y = pos;
      }
      ((JComponent)dyn.comp).setLocation( loc);
      ((JComponent)dyn.comp).repaint();
    }
    oldValue = value;

    if ( firstScan)
      firstScan = false;
  }

  public void action( int eventType, MouseEvent e) {
    switch ( eventType) {
    case GeDyn.eEvent_MB1Down:
      moveActive = true;
      offset = e.getPoint();
      // ((GeComponent)dyn.comp).colorInverse = 1;
      // dyn.repaintNow = true;
      break;
    case GeDyn.eEvent_MB1Up:
      moveActive = false;
      // ((GeComponent)dyn.comp).colorInverse = 0;
      // dyn.repaintNow = true;
      break;
    case GeDyn.eEvent_SliderMoved:
      float value;
      PwrtStatus sts;
      Point new_loc = new Point();
      Point ePoint = e.getPoint();
//	    System.out.println("Mouse dragged: " + thisPoint.x + ", " +
//	     thisPoint.y);
      Point loc = ((GeComponent)dyn.comp).getLocation();
      switch ( direction) {
      case Ge.DIRECTION_RIGHT:
	new_loc.x = loc.x + ePoint.x - offset.x;
	new_loc.y = loc.y;
	if ( new_loc.x > maxPos)
	  new_loc.x = (int) maxPos;
	if ( new_loc.x < minPos)
	  new_loc.x = (int) minPos;
	value = (float)((maxPos - new_loc.x) / (maxPos - minPos) *
	        	(maxValue - minValue) + minValue); 
	break;
      case Ge.DIRECTION_LEFT:
	new_loc.x = loc.x + ePoint.x - offset.x;
	new_loc.y = loc.y;
	if ( new_loc.x > maxPos)
	  new_loc.x = (int) maxPos;
	if ( new_loc.x < minPos)
	  new_loc.x = (int) minPos;
	value = (float)((new_loc.x - minPos) / (maxPos - minPos) * 
	        	(maxValue - minValue) + minValue);
	break;
      case Ge.DIRECTION_UP:
	new_loc.y = loc.y + ePoint.y - offset.y;
	new_loc.x = loc.x;
	if ( new_loc.y > maxPos)
	  new_loc.y = (int) maxPos;
	if ( new_loc.y < minPos)
          new_loc.y = (int) minPos;
	value = (float)((new_loc.y - minPos) / (maxPos - minPos) *
			(maxValue - minValue) + minValue);
	// System.out.println("old_y: " + ePoint.y + " new_y: " + new_loc.y + "v: " + value);
	break;
      default:
	  new_loc.y = loc.y + ePoint.y - offset.y;
	  // System.out.println( "loc.y " + loc.y + " eP.y " + ePoint.y + " offset.y " + offset.y + " new_loc.y " + new_loc.y + " maxPos " + maxPos + " minPos " + minPos);
	  new_loc.x = loc.x;
	  if ( new_loc.y > maxPos)
	    new_loc.y = (int) maxPos;
	  if ( new_loc.y < minPos)
	    new_loc.y = (int) minPos;
	  value = (float)((maxPos - new_loc.y) / (maxPos - minPos) *
			  (maxValue - minValue) + minValue);
	  // System.out.println("old_y: " + ePoint.y + " new_y: " + new_loc.y + "v: " + value);
      }
      ((JComponent)dyn.comp).setLocation(new_loc);
	
      sts = dyn.en.gdh.setObjectInfo( attribute, value);
      if ( sts.evenSts())
	System.out.println( "GeSlider: " + sts);        
      break;
    }
  }
}













