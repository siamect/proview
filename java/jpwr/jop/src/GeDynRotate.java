package jpwr.jop;
import javax.swing.*;
import jpwr.rt.*;

public class GeDynRotate extends GeDynElem {
  String attribute;
  double x0;
  double y0;
  double factor;

  boolean attrFound;
  PwrtRefId subid;
  int p;
  int typeId;
  float oldValue;
  boolean firstScan = true;

  public GeDynRotate( GeDyn dyn, String attribute, double x0, double y0, double factor) {
    super( dyn, GeDyn.mDynType_Rotate, GeDyn.mActionType_No);
    this.attribute = attribute;
    this.x0 = x0;
    this.y0 = y0;
    this.factor = factor;
  }
  public void connect() {
    String attrName = dyn.getAttrName( attribute);
    if ( attrName.compareTo("") != 0) {
      GdhrRefObjectInfo ret = dyn.en.gdh.refObjectInfo( attrName);
      if ( ret.evenSts())
	System.out.println( "Rotate: " + attrName);
      else {
	attrFound = true;
	p = ret.id;
	subid = ret.refid;
	typeId = ret.typeId;
	dyn.x0 = x0;
	dyn.y0 = y0;
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
      dyn.rotate = value0 * factor;
      oldValue = value0;
      // dyn.repaintNow = true;
      ((JComponent)dyn.comp).repaint();
    }
    if ( firstScan)
      firstScan = false;
  }
}













