package jpwr.app;

import java.util.StringTokenizer;
import java.util.Vector;

import jpwr.rt.*;
import jpwr.app.*;

public class AEvItemAlarm extends AXttItemBase {
	static double y = 10D;
	public MhrEvent e;
	
	public AEvItemAlarm(AEv aev, MhrEvent e,
				PlowNode destination, int destCode) {
		this.e = e.getCopy();
System.out.println("New itemalarm Status: " + e.eventStatus + " " + e.eventPrio + " " + e.eventText + " " + e.eventName);		
	
			
		switch ( this.e.eventPrio) {
		case Mh.mh_eEventPrio_A:
			node = new PlowNode(aev.cmnAla, 3D, y, aev.alaNcAAlarm);
			break;
		case Mh.mh_eEventPrio_B:
			node = new PlowNode(aev.cmnAla, 3D, y, aev.alaNcBAlarm);
			break;
		case Mh.mh_eEventPrio_C:
			node = new PlowNode(aev.cmnAla, 3D, y, aev.alaNcCAlarm);
			break;
		case Mh.mh_eEventPrio_D:
			node = new PlowNode(aev.cmnAla, 3D, y, aev.alaNcDAlarm);
			break;
		}	
		node.setAnnotation(0, this.e.eventTime, 1);
		node.setAnnotation(1, this.e.eventText, 1);
		node.setAnnotation(2, this.e.eventName, 1);
		if ( (e.eventStatus & Mh.mh_mEventStatus_NotAck) != 0)
			node.setAnnotPixmap(0, aev.iconAlarmNotAcked);			
		else
			node.setAnnotPixmap(0, null);			
		
		if ( (e.eventStatus & Mh.mh_mEventStatus_NotRet) != 0)
			node.setAnnotPixmap(1, aev.iconAlarmActive);			
		else
			node.setAnnotPixmap(1, null);			

		aev.cmnAla.insertNode(node, destination, destCode);
		y += 1.5D;

		node.setUserData(this);
	}

	public void update(AEv aev) {
		if ( (e.eventStatus & Mh.mh_mEventStatus_NotAck) != 0)
			node.setAnnotPixmap(0, aev.iconAlarmNotAcked);			
		else
			node.setAnnotPixmap(0, null);			

		if ( (e.eventStatus & Mh.mh_mEventStatus_NotRet) != 0)
			node.setAnnotPixmap(1, aev.iconAlarmActive);			
		else
			node.setAnnotPixmap(1, null);			
	}
}
