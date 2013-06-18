package jpwr.app;

import java.util.StringTokenizer;
import java.util.Vector;

import jpwr.rt.*;
import jpwr.app.*;

public class AEvItemEvent extends AXttItemBase {
	static double y = 10D;
	MhrEvent e;
	public PlowNodeClass nc;
	
	public AEvItemEvent(AEv aev, MhrEvent e,
				PlowNode destination, int destCode) {
		this.e = e.getCopy();
System.out.println("New itemevent: " + e.eventPrio + " " + e.eventText + " " + e.eventName);		
		switch(this.e.eventType) {
		case Mh.mh_eEvent_Info:
			nc = aev.eveNcInfo;
			break;
		case Mh.mh_eEvent_Alarm:
			switch ( this.e.eventPrio) {
			case Mh.mh_eEventPrio_A:
				nc = aev.eveNcAAlarm;
				break;
			case Mh.mh_eEventPrio_B:
				nc = aev.eveNcBAlarm;
				break;
			case Mh.mh_eEventPrio_C:
				nc = aev.eveNcCAlarm;
				break;
			case Mh.mh_eEventPrio_D:
				nc = aev.eveNcDAlarm;
				break;
			default:
				nc = aev.eveNcAlarm;
				break;
			}
			break;
		default:
			nc = aev.eveNcAlarm;
		}
		node = new PlowNode(aev.cmnAla, 3D, y, nc);

		switch(this.e.eventType) {
		case Mh.mh_eEvent_Alarm:
			node.setAnnotPixmap(0, aev.iconEventAlarm);			
			break;
		case Mh.mh_eEvent_Ack:
			node.setAnnotPixmap(0, aev.iconEventAcked);			
			break;
		case Mh.mh_eEvent_Return:
			node.setAnnotPixmap(0, aev.iconEventReturn);			
			break;
		case Mh.mh_eEvent_Block:
			node.setAnnotPixmap(0, aev.iconEventBlock);			
			break;
		case Mh.mh_eEvent_Unblock:
			node.setAnnotPixmap(0, aev.iconEventUnblock);			
			break;
		}
		node.setAnnotation(0, this.e.eventTime, 1);
		node.setAnnotation(1, this.e.eventText, 1);
		node.setAnnotation(2, this.e.eventName, 1);
		aev.cmnEve.insertNode(node, destination, destCode);
		y += 1.5D;

		node.setUserData(this);
	}

}
