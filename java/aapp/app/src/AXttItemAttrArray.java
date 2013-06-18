package jpwr.app;

import java.util.Vector;

import jpwr.rt.CdhrObjAttr;
import jpwr.rt.GdhrRefObjectInfo;
import jpwr.rt.Pwr;
import jpwr.rt.PwrtObjid;
import jpwr.rt.PwrtRefId;

public class AXttItemAttrArray extends AXttItemBase {
	PwrtObjid objid;
	String fullName;
	String name;
	int flags;
	int type;
	int elements;
	
	AXttItemAttrArray(AXtt axtt, PwrtObjid objid, String name, String fullName, int flags, int type,
			 int elements, PlowNode destination, int destCode) {
		this.objid = objid;
		this.name = name;
		this.fullName = fullName;
		this.flags = flags;
		this.type = type;
		this.elements = elements;
		
		node = new PlowNode(axtt.cmn, 0, 0, axtt.ncObject);
		node.setAnnotPixmap(0, axtt.iconAttrArray);
		node.setAnnotation(0, name, 22);
		axtt.cmn.insertNode(node, destination, destCode);

		node.setUserData(this);		
	}
	@Override
	public void openAttributes(AXtt axtt) {
		if ( (node.getNodeOpen() & Plow.OPEN_ATTRIBUTES) != 0) {
			axtt.cmn.closeNode(node);
			node.resetNodeOpen(Plow.OPEN_ATTRIBUTES);
		}
		else {
			for ( int i = 0; i < elements; i++) {
				String fName = fullName + "[" + i + "]";
				String aname = name + "[" + i + "]";
				
				if ( (flags & Pwr.mAdef_class) != 0) {
					AXttItemAttr item = new AXttItemAttr(axtt, objid, aname, fName, 
							flags, type, node, Plow.DEST_INTOLAST);
				}
				else {
					AXttItemAttrArrayElem item = new AXttItemAttrArrayElem(axtt, objid, aname, fName, 
							flags, type, node, Plow.DEST_INTOLAST);
				}

			}
			node.setNodeOpen(Plow.OPEN_ATTRIBUTES);
		}
		axtt.cmn.configure();
	}
	
	@Override
	public void close(AXtt axtt) {
	}
	
}
