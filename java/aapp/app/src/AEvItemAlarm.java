package jpwr.app;

import java.util.StringTokenizer;
import java.util.Vector;

import jpwr.rt.*;
import jpwr.app.*;

public class AEvItemAlarm extends AXttItemBase {
	static double y = 10D;
	public MhrEvent e;
	public PlowNodeClass nc;
	
	public AEvItemAlarm(AEv aev, MhrEvent e,
				PlowNode destination, int destCode) {
		this.e = e.getCopy();
System.out.println("New itemalarm Status: " + e.eventStatus + " " + e.eventPrio + " " + e.eventText + " " + e.eventName);		
	
			
                switch ( this.e.eventType) {
		case Mh.mh_eEvent_Alarm:
		case Mh.mh_eEvent_MaintenanceAlarm:
		case Mh.mh_eEvent_SystemAlarm:
		case Mh.mh_eEvent_UserAlarm1:
		case Mh.mh_eEvent_UserAlarm2:
		case Mh.mh_eEvent_UserAlarm3:
		case Mh.mh_eEvent_UserAlarm4:
		    switch ( this.e.eventPrio) {
		    case Mh.mh_eEventPrio_A:
			nc = aev.alaNcAAlarm;
			break;
		    case Mh.mh_eEventPrio_B:
			nc = aev.alaNcBAlarm;
			break;
		    case Mh.mh_eEventPrio_C:
			nc = aev.alaNcCAlarm;
			break;
		    case Mh.mh_eEventPrio_D:
			nc = aev.alaNcDAlarm;
			break;
		    }	
		    break;
		case Mh.mh_eEvent_Info:
		    nc = aev.alaNcInfo;
		    break;
		case Mh.mh_eEvent_InfoSuccess:
		    nc = aev.alaNcInfoSuccess;
		    break;
		default: 
		    return;
		}
		node = new PlowNode(aev.cmnAla, 3D, y, nc);
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
