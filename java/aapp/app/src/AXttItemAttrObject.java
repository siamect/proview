package jpwr.app;

import java.util.Vector;

import jpwr.rt.*;
import jpwr.app.*;

public class AXttItemAttrObject extends AXttItemBase {
	PwrtObjid objid;
	String fullName;
	String name;
	int flags;
	int type;
        PwrtAttrRef aref;
	int classid;
        
    
	
	AXttItemAttrObject(AXtt axtt, PwrtObjid objid, String name, String fullName, int flags, int type,
			 PlowNode destination, int destCode) {
		this.objid = objid;
		this.name = name;
		this.fullName = fullName;
		this.flags = flags;
		this.type = type;
		
		CdhrAttrRef ar = axtt.gdh.nameToAttrRef( fullName);
		if ( ar.evenSts()) return;

		aref = ar.aref;

		CdhrTypeId cr = axtt.gdh.getAttrRefTid( ar.aref);
		if ( cr.evenSts()) return;

		classid = cr.typeId;

		CdhrObjid coidret = axtt.gdh.classIdToObjid(cr.typeId);
		if ( coidret.evenSts()) return;
		
		CdhrString cnameret = axtt.gdh.objidToName(coidret.objid, Cdh.mName_object);
		if ( cnameret.evenSts()) return;

		node = new PlowNode(axtt.cmn, 0, 0, axtt.ncObject);
		node.setAnnotPixmap(0, axtt.iconObject);
		node.setAnnotation(0, name, 22);
		node.setAnnotation(1, cnameret.str, 22);
		axtt.cmn.insertNode(node, destination, destCode);

		node.setUserData(this);		
	}

	@Override
	public void openChildren(AXtt axtt) {
		if ( (node.getNodeOpen() & Plow.OPEN_CHILDREN) != 0) {
			axtt.cmn.closeNode(node);
			node.resetNodeOpen(Plow.OPEN_CHILDREN);
			node.setAnnotPixmap(0, axtt.iconMap);
		}
		else {
			Vector<GdhrGetXttObj> v = (Vector)axtt.gdh.getAllXttChildren(objid);
			
			for ( int i = 0; i < v.size(); i++) {
				GdhrGetXttObj gdhr = v.get(i);
				AXttItemObject item = new AXttItemObject(axtt, gdhr.cdhrObjId.objid, 
						gdhr.fullName, gdhr.hasChildren, node, Plow.DEST_INTOLAST);
			}			
			node.setNodeOpen(Plow.OPEN_CHILDREN);
			node.setAnnotPixmap(0, axtt.iconOpenmap);
		}
		axtt.cmn.configure();
	}

	@Override
	public void openAttributes(AXtt axtt) {
		if ( (node.getNodeOpen() & Plow.OPEN_ATTRIBUTES) != 0) {
			axtt.cmn.closeNode(node);
			node.resetNodeOpen(Plow.OPEN_ATTRIBUTES);
		}
		else if ( (node.getNodeOpen() & Plow.OPEN_CHILDREN) != 0) {
			axtt.cmn.closeNode(node);
			node.resetNodeOpen(Plow.OPEN_CHILDREN);
			node.setAnnotPixmap(0, axtt.iconMap);
		}
		else {
			Vector<CdhrObjAttr> v = (Vector)axtt.gdh.getAllClassAttributes(type, objid);

			for ( int i = 0; i < v.size(); i++) {
				CdhrObjAttr gdhr = v.get(i);
				String fName = fullName  + "." + gdhr.name;

				if ( (gdhr.flags & Pwr.mAdef_pointer) != 0 &&
					 (gdhr.flags & Pwr.mAdef_private) != 0)
					continue;
				
				if ( (gdhr.flags & Pwr.mAdef_array) != 0) {
					AXttItemAttrArray item = new AXttItemAttrArray(axtt, gdhr.objid, gdhr.name, fName, 
						gdhr.flags, gdhr.type, gdhr.elements, node, Plow.DEST_INTOLAST);
				}
				else if ( (gdhr.flags & Pwr.mAdef_class) != 0) {
					AXttItemAttrObject item = new AXttItemAttrObject(axtt, gdhr.objid, gdhr.name, fName, 
							gdhr.flags, gdhr.type, node, Plow.DEST_INTOLAST);
				}
				else {
					AXttItemAttr item = new AXttItemAttr(axtt, gdhr.objid, gdhr.name, fName, 
							gdhr.flags, gdhr.type, node, Plow.DEST_INTOLAST);
				}

			}
			node.setNodeOpen(Plow.OPEN_ATTRIBUTES);
		}
		axtt.cmn.configure();
	}

        public PwrtAttrRef getAttrRef() {
	    return aref;	    
        }
        public int getClassId() {
	    return classid;
        }
}
