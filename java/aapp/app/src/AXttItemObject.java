package jpwr.app;

import java.util.StringTokenizer;
import java.util.Vector;

import jpwr.rt.*;
import jpwr.app.*;

public class AXttItemObject extends AXttItemBase {
	static double y = 10D;
	PwrtObjid objid;
	PwrtObjid windowObjid;
	int classid;
	boolean hasChildren;
	String fullName;
	
	public AXttItemObject(AXtt axtt, PwrtObjid objid, String fullName, boolean hasChildren, 
				PlowNode destination, int destCode) {
		this.objid = objid;
		this.fullName = fullName;
		this.hasChildren = hasChildren;
		this.windowObjid = null;
		
		CdhrClassId cret = axtt.gdh.getObjectClass(objid);
		if (cret.evenSts()) return;

		classid = cret.classId;
		
		CdhrObjid coidret = axtt.gdh.classIdToObjid(cret.classId);
		if ( coidret.evenSts()) return;
		
		CdhrString cnameret = axtt.gdh.objidToName(coidret.objid, Cdh.mName_object);
		if (cnameret.evenSts()) return;
		
		CdhrString ret = axtt.gdh.objidToName(objid, Cdh.mName_object);
		if (ret.evenSts()) return;
		
		System.out.println("Item: " + ret.str + " " + cnameret.str);

		if ( fullName == null)
			fullName = cnameret.str;
			
		if ( classid == Pwrb.cClass_plc) {
			coidret = axtt.gdh.getChild(objid);
			if ( coidret.oddSts()) 
				windowObjid = coidret.objid;			
		}
		
		node = new PlowNode(axtt.cmn, 3D, y, axtt.ncObject);
		if ( hasChildren)
			node.setAnnotPixmap(0, axtt.iconMap);
		else
			node.setAnnotPixmap(0, axtt.iconLeaf);			
		node.setAnnotation(0, ret.str, 22);
		node.setAnnotation(1, cnameret.str, 11);
		axtt.cmn.insertNode(node, destination, destCode);
		y += 1.5D;

		node.setUserData(this);
	}

	public int getClassid() {
		return classid;
	}
	public PwrtObjid getObjid() {
		return objid;
	}
	public PwrtObjid getWindowObjid() {
		return windowObjid;
	}
	@Override
	public boolean hasChildren() {
		return hasChildren;
	}
	
	static public void openRoot(AXtt axtt) {	
		CdhrObjid reto = axtt.gdh.getRootList();
		if (reto.oddSts()) {
			reto.objid.oix = 0;
			Vector<GdhrGetXttObj> v = (Vector)axtt.gdh.getAllXttChildren(reto.objid);
			
			for ( int i = 0; i < v.size(); i++) {
				GdhrGetXttObj gdhr = v.get(i);
				AXttItemObject item = new AXttItemObject(axtt, gdhr.cdhrObjId.objid, 
						gdhr.fullName, gdhr.hasChildren, null, Plow.DEST_INTOLAST);
				
			}			
		}
	}

	@Override
	public void openChildren(AXtt axtt) {
		if ( (node.getNodeOpen() & Plow.OPEN_CHILDREN) != 0) {
			axtt.cmn.closeNode(node);
			node.resetNodeOpen(Plow.OPEN_CHILDREN);
			node.setAnnotPixmap(0, axtt.iconMap);
		}
		else if ( (node.getNodeOpen() & Plow.OPEN_ATTRIBUTES) != 0) {
			axtt.cmn.closeNode(node);
			node.resetNodeOpen(Plow.OPEN_ATTRIBUTES);
		}
		else if ( (node.getNodeOpen() & Plow.OPEN_CROSSREFERENCES) != 0) {
			axtt.cmn.closeNode(node);
			node.resetNodeOpen(Plow.OPEN_CROSSREFERENCES);
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
		else if ( (node.getNodeOpen() & Plow.OPEN_CROSSREFERENCES) != 0) {
			axtt.cmn.closeNode(node);
			node.resetNodeOpen(Plow.OPEN_CROSSREFERENCES);
		}
		else {
			Vector<CdhrObjAttr> v = (Vector)axtt.gdh.getAllClassAttributes(classid, objid);

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

	public void openCrossreferences(AXtt axtt) {
		if ( (node.getNodeOpen() & Plow.OPEN_CROSSREFERENCES) != 0) {
			axtt.cmn.closeNode(node);
			node.resetNodeOpen(Plow.OPEN_CROSSREFERENCES);
		}
		else {
			if ( (node.getNodeOpen() & Plow.OPEN_ATTRIBUTES) != 0) {
				axtt.cmn.closeNode(node);
				node.resetNodeOpen(Plow.OPEN_ATTRIBUTES);
			}
			else if ( (node.getNodeOpen() & Plow.OPEN_CHILDREN) != 0) {
				axtt.cmn.closeNode(node);
				node.resetNodeOpen(Plow.OPEN_CHILDREN);
				node.setAnnotPixmap(0, axtt.iconMap);
			}
		
			CdhrClassId cret = axtt.gdh.getObjectClass( objid);
			if ( cret.evenSts()) return;

			CdhrString sret;
			switch (  cret.classId) {
			case Pwrb.cClass_Ai:
			case Pwrb.cClass_Ao:
			case Pwrb.cClass_Ii:
			case Pwrb.cClass_Io:
			case Pwrb.cClass_Di:
			case Pwrb.cClass_Do:
			case Pwrb.cClass_Po:
			case Pwrb.cClass_Dv:
			case Pwrb.cClass_Av:
			case Pwrb.cClass_Iv:
			case Pwrb.cClass_Sv:
				sret = axtt.gdh.crrSignal( fullName);
				break;
			default:
				sret = axtt.gdh.crrObject( fullName);
			}
			if ( sret.evenSts()) return;

			String delim = new String( new char[] { '\n'});
			StringTokenizer tokens = new StringTokenizer( sret.str, delim);

			int tokensCnt = tokens.countTokens();
			String text;
			int type;
			for ( int i = 0; i < tokensCnt; i++) {
				String token = tokens.nextToken();
				text = token.substring(1);
				if ( token.startsWith("0"))
					type = AXtt.CRR_READ;
				else if ( token.startsWith("1"))
					type = AXtt.CRR_WRITE;
				else
					type = AXtt.CRR_READWRITE;

				AXttItemCrr item = new AXttItemCrr(axtt, text, type, node, Plow.DEST_INTOLAST);
			}
			node.setNodeOpen(Plow.OPEN_CROSSREFERENCES);
		}

	}
}
