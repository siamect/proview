package jpwr.jop;
import jpwr.rt.*;

public class GeDynDigShift extends GeDynElem {
  String attribute;

  boolean attrFound;
  PwrtRefId subid;
  int p;
  boolean inverted;
  boolean oldValue;
  boolean firstScan = true;

  public GeDynDigShift( GeDyn dyn, String attribute) {
    super( dyn, GeDyn.mDynType_DigShift, GeDyn.mActionType_No);
    this.attribute = attribute;
  }
  public void connect() {
    String attrName = dyn.getAttrName( attribute);
    if ( attrName.compareTo("") != 0) {
      GdhrRefObjectInfo ret = dyn.en.gdh.refObjectInfo( attrName);
      if ( ret.evenSts())
	System.out.println( "DigShift: " + attrName);
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

    boolean value0 = dyn.en.gdh.getObjectRefInfoBoolean( p);

    if ( firstScan) {
      if ( (!inverted && value0) || (inverted && !value0)) { 
        ((GeComponent)dyn.comp).setLastPage();
        dyn.repaintNow = true;
      }
      oldValue = value0;
      firstScan = false;
    } 

    if ( oldValue != value0) {
      // Shift to first or last page
      if ( (!inverted && value0) || (inverted && !value0))
        dyn.comp.setLastPage();
      else if ( (!inverted && !value0) || (inverted && value0))
        dyn.comp.setFirstPage();
      dyn.repaintNow = true;
    }
    oldValue = value0;
  }
}












