package jpwr.jop;
import jpwr.rt.*;

public class GeDynValue extends GeDynElem {
  public String attribute;

  public boolean attrFound;
  PwrtRefId subid;
  int p;
  public int typeId;
  public boolean oldValueB;
  public float oldValueF;
  public int oldValueI;
  public String oldValueS;
  boolean firstScan = true;
  GeCFormat	cFormat;
  StringBuffer sb = new StringBuffer();

  public GeDynValue( GeDyn dyn, String attribute, String format) {
    super( dyn, GeDyn.mDynType_Value, GeDyn.mActionType_No);
    this.attribute = attribute;
    this.cFormat = new GeCFormat(format);
  }
  public void connect() {
    String attrName = dyn.getAttrName( attribute);
    if ( attrName.compareTo("") != 0) {
      GdhrRefObjectInfo ret = dyn.en.gdh.refObjectInfo( attrName);
      if ( ret.evenSts())
	System.out.println( "Value: " + attrName);
      else {
	attrFound = true;
	p = ret.id;
	subid = ret.refid;
	typeId = ret.typeId;
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

    if ( typeId == Pwr.eType_Float32) {
      float value0 = dyn.en.gdh.getObjectRefInfoFloat( p);
      if ( value0 != oldValueF  || firstScan) {
        sb = cFormat.format( value0, sb);
        dyn.comp.setAnnot1(new String(sb));
        dyn.repaintNow = true;
        oldValueF = value0;
      }
    }
    else if ( typeId == Pwr.eType_Int32 ||
	      typeId == Pwr.eType_UInt32 ||
              typeId == Pwr.eType_Int16 ||
	      typeId == Pwr.eType_UInt16 ||
              typeId == Pwr.eType_Int8 ||
	      typeId == Pwr.eType_UInt8) {
      int value0 = dyn.en.gdh.getObjectRefInfoInt( p);
      if ( value0 != oldValueI || firstScan) {
	sb = cFormat.format( value0, sb);
	dyn.comp.setAnnot1(new String(sb));
        dyn.repaintNow = true;
        oldValueI = value0;
      }
    }
    else if ( typeId == Pwr.eType_String || 
              typeId == Pwr.eType_Objid) {
      String value0 = dyn.en.gdh.getObjectRefInfoString( p, typeId);
      if ( firstScan || value0.compareTo( oldValueS) != 0) {
        sb = cFormat.format( value0, sb);
	dyn.comp.setAnnot1(new String(sb));
        dyn.repaintNow = true;
	oldValueS = value0;
      }
    }
    if ( firstScan)
      firstScan = false;
  }
}









