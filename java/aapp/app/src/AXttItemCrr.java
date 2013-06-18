package jpwr.app;

import jpwr.rt.PwrtObjid;

public class AXttItemCrr extends AXttItemBase {
	String text;
	int type;
	
	AXttItemCrr(AXtt axtt, String text, int type, PlowNode destination, int destCode) {
		this.text = text;
		this.type = type;
		
		node = new PlowNode(axtt.cmn, 0, 0, axtt.ncObject);
		switch ( type) {
		case AXtt.CRR_READ:
			node.setAnnotPixmap(0, axtt.iconCrrRead);
			break;
		case AXtt.CRR_WRITE:
			node.setAnnotPixmap(0, axtt.iconCrrWrite);
			break;
		case AXtt.CRR_READWRITE:
			node.setAnnotPixmap(0, axtt.iconCrrRead);
			node.setAnnotPixmap(1, axtt.iconCrrWrite);
			break;
		}
		node.setAnnotation(0, text, 22);
		axtt.cmn.insertNode(node, destination, destCode);

		node.setUserData(this);		
	}

	public String getText() {
		return text;
	}
}
