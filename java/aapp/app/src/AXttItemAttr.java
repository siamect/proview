package jpwr.app;

import jpwr.rt.*;

public class AXttItemAttr extends AXttItemBase {
	PwrtObjid objid;
	String fullName;
	String name;
	int flags;
	int type;
	int refId;
	PwrtRefId refRefId;
	int refTypeId;
	boolean firstScan = true;
	float oldValueF;
	int oldValueI;
	boolean oldValueB;
	String oldValueS;
	
	AXttItemAttr(AXtt axtt, PwrtObjid objid, String name, String fullName, int flags, int type,
			 PlowNode destination, int destCode) {
		this.objid = objid;
		this.name = name;
		this.fullName = fullName;
		this.flags = flags;
		this.type = type;
		this.node = node;
		
		node = new PlowNode(axtt.cmn, 0, 0, axtt.ncObject);
		node.setAnnotPixmap(0, axtt.iconAttr);
		node.setAnnotation(0, name, 22);
		axtt.cmn.insertNode(node, destination, destCode);

		node.setUserData(this);		

		String refName;
		switch ( type) {
		case Pwr.eType_Boolean:
			refName = fullName + "##Boolean";
			break;
		case Pwr.eType_Float32:
			refName = fullName + "##Float32";
			break;
		case Pwr.eType_Int32:
			refName = fullName + "##Int32";
			break;
		case Pwr.eType_UInt32:
			refName = fullName + "##UInt32";
			break;
		case Pwr.eType_String:
			refName = fullName + "##String80";
			break;
		default:
			refName = fullName;
		}
		GdhrRefObjectInfo ret = axtt.gdh.refObjectInfo(refName);
		if ( ret.oddSts()) {
			refId = ret.id;
			refRefId = ret.refid;
			refTypeId = ret.typeId;
		}
		else 
			refId = 0;
	}
	public void close(AXtt axtt) {
		if ( refRefId !=  null)
			axtt.gdh.unrefObjectInfo(refRefId);
	}
	public String getFullName() {
		return fullName;
	}
	public int getType() {
		return type;
	}
	
	@Override
	public void scan(AXtt axtt) {
		String valueStr = null;
		
		switch ( type) {
		case Pwr.eType_Float32: {
			float value = axtt.gdh.getObjectRefInfoFloat(refId);
			if ( firstScan || value != oldValueF) {
				valueStr = "" + value;
				oldValueF = value;
			}
			break;
		}
		case Pwr.eType_Boolean: {
			boolean value = axtt.gdh.getObjectRefInfoBoolean(refId);
			if ( firstScan || value != oldValueB) {
				valueStr = "" + (int)(value?1:0);
				oldValueB = value;
			}
			break;
		}
		case Pwr.eType_Int32:
		case Pwr.eType_Int16:
		case Pwr.eType_Int8:
		case Pwr.eType_UInt32:
		case Pwr.eType_UInt16:
		case Pwr.eType_UInt8: {
			int value = axtt.gdh.getObjectRefInfoInt(refId);
			if ( firstScan || value != oldValueI) {
				valueStr = "" + value;
				oldValueI = value;
			}
			break;
		}
		case Pwr.eType_String:
		case Pwr.eType_AttrRef:
		case Pwr.eType_Objid: {
			String value = axtt.gdh.getObjectRefInfoString(refId, refTypeId);
			if ( firstScan || value.equals(oldValueS)) {
				valueStr = "" + value;
				oldValueS = value;
				valueStr = value;
			}
			break;
		}
		}
		if ( valueStr != null)
			node.setAnnotation(1, valueStr, 10);
	}
}
