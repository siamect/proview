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
    
    switch ( typeId) {
    case Pwr.eType_Float32: {
      float value0 = dyn.en.gdh.getObjectRefInfoFloat( p);
      if ( value0 != oldValueF  || firstScan) {
        sb = cFormat.format( value0, sb);
        dyn.comp.setAnnot1(new String(sb));
        dyn.repaintNow = true;
        oldValueF = value0;
      }
      break;
    }
    case Pwr.eType_Int32:
    case Pwr.eType_UInt32:
    case Pwr.eType_Int16:
    case Pwr.eType_UInt16:
    case Pwr.eType_Int8:
    case Pwr.eType_UInt8: {
      int value0 = dyn.en.gdh.getObjectRefInfoInt( p);
      if ( value0 != oldValueI || firstScan) {
	sb = cFormat.format( value0, sb);
	dyn.comp.setAnnot1(new String(sb));
        dyn.repaintNow = true;
        oldValueI = value0;
      }
      break;
    }
    case Pwr.eType_String:
    case Pwr.eType_Objid:
    case Pwr.eType_Time:
    case Pwr.eType_DeltaTime: {
      String value0 = dyn.en.gdh.getObjectRefInfoString( p, typeId);
      if ( firstScan || value0.compareTo( oldValueS) != 0) {
        sb = cFormat.format( value0, sb);
	dyn.comp.setAnnot1(new String(sb));
        dyn.repaintNow = true;
	oldValueS = value0;
      }
      break;
    }
    case Pwr.eType_Status:
    case Pwr.eType_NetStatus: {
      int value0 = dyn.en.gdh.getObjectRefInfoInt( p);
      if ( value0 != oldValueI || firstScan) {
	switch ( cFormat.type()) {
	case GeCFormat.FRM_M: {
	  CdhrString cstr = dyn.en.gdh.getMsg( value0);
	  dyn.comp.setAnnot1( cstr.str);
          dyn.repaintNow = true;
	  break;
	}
	case GeCFormat.FRM_1M: {
	  CdhrString cstr = dyn.en.gdh.getMsgText( value0);
	  dyn.comp.setAnnot1( cstr.str);
          dyn.repaintNow = true;
	  break;
	}
	}
      }
      oldValueI = value0;
      break;
    }
    }
    if ( firstScan)
      firstScan = false;
  }
}









