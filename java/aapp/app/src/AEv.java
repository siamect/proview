package jpwr.app;
import java.io.IOException;
import java.io.ObjectInputStream;
import java.io.ObjectOutputStream;
import java.net.Socket;
import java.net.SocketTimeoutException;
import java.util.Vector;

import android.graphics.Color;
import android.os.AsyncTask;

import jpwr.pwrxtt.R;

import jpwr.rt.*;

public class AEv {
	final static int PORT = 4446;
	
	public PlowCmn cmnAla;
	public PlowCmn cmnEve;
	public PlowNodeClass alaNcAlarm;
	public PlowNodeClass alaNcAAlarm;
	public PlowNodeClass alaNcBAlarm;
	public PlowNodeClass alaNcCAlarm;
	public PlowNodeClass alaNcDAlarm;
	public PlowNodeClass eveNcAlarm;
	public PlowNodeClass eveNcAAlarm;
	public PlowNodeClass eveNcBAlarm;
	public PlowNodeClass eveNcCAlarm;
	public PlowNodeClass eveNcDAlarm;
	public PlowNodeClass eveNcInfo;
	public PlowPixmapData iconEventAlarm;
	public PlowPixmapData iconEventAcked;
	public PlowPixmapData iconEventReturn;
	public PlowPixmapData iconEventBlock;
	public PlowPixmapData iconEventUnblock;
	public PlowPixmapData iconAlarmActive;
	public PlowPixmapData iconAlarmNotAcked;
	Socket mhSocket;
	String pwrHost;
	AEvAppl appl;
	AEv aev;
	int maxNrOfAlarms = 100;
	int maxNrOfEvents = 100;
	boolean sendAck;
	MhrsEventId ackEventId;
	
	public AEv(PlowCmn cmnAla, PlowCmn cmnEve, AEvAppl appl) {
		this.cmnAla = cmnAla;
		this.cmnEve = cmnEve;
		this.pwrHost = appl.getPwrHost();
		this.appl = appl;
		aev = this;
		
		createPixmaps();
		createNodeClasses();

		new MhReceiveTask().execute();
	
	}

	public PlowCmn getCmnAla() {
		return cmnAla;
	}

	public PlowCmn getCmnEve() {
		return cmnEve;
	}
	
	public void newMess(MhrEvent ev) {
		switch (ev.eventType) {
		case Mh.mh_eEvent_Alarm:
			// Add to alarm and event list
			new AEvItemAlarm(aev, ev, null, Plow.DEST_AFTER);
			cmnAla.configure();

			new AEvItemEvent(aev, ev, null, Plow.DEST_AFTER);
			cmnEve.configure();
	        break;
	      case Mh.mh_eEvent_Return:
	       
	    	  for( PlowNode n = (PlowNode)cmnAla.getFirst(); n != null; n = (PlowNode)cmnAla.getNext(n)) {
					AEvItemAlarm item = (AEvItemAlarm)n.getUserData();

					if ((ev.targetId.nix == item.e.eventId.nix) && (ev.targetId.idx == item.e.eventId.idx)) {
						// The alarm is acknowledged and can be removed
						if((item.e.eventStatus & Mh.mh_mEventStatus_NotAck) == 0) {
							cmnAla.remove(n);
							cmnAla.configure();
						}
						else {
							// Set the return bit and display it
							item.e.eventStatus &= ~Mh.mh_mEventStatus_NotRet;
							item.update(this);
						}
						break;
					}
	    	  }
	    	  if((ev.eventFlags & Mh.mh_mEventFlags_Return) != 0) {
	    		  // Add copy to event list
	    		  new AEvItemEvent(aev, ev, null, Plow.DEST_AFTER);
	    		  cmnEve.configure();
	    	  }
	    	  break;
	      case Mh.mh_eEvent_Ack:
	    	  for( PlowNode n = (PlowNode)cmnAla.getFirst(); n != null; n = (PlowNode)cmnAla.getNext(n)) {
					AEvItemAlarm item = (AEvItemAlarm)n.getUserData();

					if((ev.targetId.nix == item.e.eventId.nix) && (ev.targetId.idx == item.e.eventId.idx)) {
						if((item.e.eventStatus & Mh.mh_mEventStatus_NotRet) == 0) {
							cmnAla.remove(n);
							cmnAla.configure();
						}
						else {
							item.e.eventStatus &= ~Mh.mh_mEventStatus_NotAck;
							item.update(this);
						}
						break;
					}
	    	  }
	    	  if((ev.eventFlags & Mh.mh_mEventFlags_Ack) != 0) {
	    		  // Add copy to event list
	    		  new AEvItemEvent(aev, ev, null, Plow.DEST_AFTER);
	    		  cmnEve.configure();
	    	  }
	    	  break;
	      case Mh.mh_eEvent_Info:
	    	  // Add to alarm list
	    	  if((ev.eventFlags & Mh.mh_mEventFlags_InfoWindow) != 0) {
	  				new AEvItemAlarm(aev, ev, null, Plow.DEST_AFTER);
	  				cmnAla.configure();
	    	  }
	    	  // Add copy to event list
    		  new AEvItemEvent(aev, ev, null, Plow.DEST_AFTER);
    		  cmnEve.configure();
    		  break;
	      case Mh.EventType_ClearAlarmList:
	    	  cmnAla.clear();
	    	  cmnAla.configure();
	    	  System.out.println("Clearing alarmlist");
	    	  break;
	      
	      default:
	    	  break;
		}
	    if(cmnAla.size() > maxNrOfAlarms) {
	    	PlowNode n = (PlowNode)cmnAla.getLast();
	    	cmnAla.remove(n);
	    }
	    if(cmnEve.size() > maxNrOfEvents) {
	    	PlowNode n = (PlowNode)cmnAla.getLast();
	    	cmnEve.remove(n);
	    }
	}
	
	public void createPixmaps() {	
		// Eve pixmaps
		iconEventAlarm = new PlowPixmapData(cmnEve, new int[] {R.drawable.xnav_bitmap_eventalarm8, R.drawable.xnav_bitmap_eventalarm10, 
				R.drawable.xnav_bitmap_eventalarm12, R.drawable.xnav_bitmap_eventalarm14, R.drawable.xnav_bitmap_eventalarm16, 
				R.drawable.xnav_bitmap_eventalarm18, R.drawable.xnav_bitmap_eventalarm20, R.drawable.xnav_bitmap_eventalarm24});
		iconEventAcked = new PlowPixmapData(cmnEve, new int[] {R.drawable.xnav_bitmap_eventacked8, R.drawable.xnav_bitmap_eventacked10, 
				R.drawable.xnav_bitmap_eventacked12, R.drawable.xnav_bitmap_eventacked14, R.drawable.xnav_bitmap_eventacked16, 
				R.drawable.xnav_bitmap_eventacked18, R.drawable.xnav_bitmap_eventacked20, R.drawable.xnav_bitmap_eventacked24});
		iconEventReturn = new PlowPixmapData(cmnEve, new int[] {R.drawable.xnav_bitmap_eventreturn8, R.drawable.xnav_bitmap_eventreturn10, 
				R.drawable.xnav_bitmap_eventreturn12, R.drawable.xnav_bitmap_eventreturn14, R.drawable.xnav_bitmap_eventreturn16, 
				R.drawable.xnav_bitmap_eventreturn18, R.drawable.xnav_bitmap_eventreturn20, R.drawable.xnav_bitmap_eventreturn24});
		iconEventBlock = new PlowPixmapData(cmnEve, new int[] {R.drawable.xnav_bitmap_blockr_8, R.drawable.xnav_bitmap_blockr_10, 
				R.drawable.xnav_bitmap_blockr_12, R.drawable.xnav_bitmap_blockr_14, R.drawable.xnav_bitmap_blockr_16, 
				R.drawable.xnav_bitmap_blockr_18, R.drawable.xnav_bitmap_blockr_20, R.drawable.xnav_bitmap_blockr_24});
		iconEventUnblock = new PlowPixmapData(cmnEve, new int[] {R.drawable.xnav_bitmap_blockl_8, R.drawable.xnav_bitmap_blockl_10, 
				R.drawable.xnav_bitmap_blockl_12, R.drawable.xnav_bitmap_blockl_14, R.drawable.xnav_bitmap_blockl_16, 
				R.drawable.xnav_bitmap_blockl_18, R.drawable.xnav_bitmap_blockl_20, R.drawable.xnav_bitmap_blockl_24});

		// Ala pixmaps
		iconAlarmActive = new PlowPixmapData(cmnAla, new int[] {R.drawable.xnav_bitmap_alarm8, R.drawable.xnav_bitmap_alarm10, 
				R.drawable.xnav_bitmap_alarm12, R.drawable.xnav_bitmap_alarm14, R.drawable.xnav_bitmap_alarm16, 
				R.drawable.xnav_bitmap_alarm18, R.drawable.xnav_bitmap_alarm20, R.drawable.xnav_bitmap_alarm24});
		iconAlarmNotAcked = new PlowPixmapData(cmnAla, new int[] {R.drawable.xnav_bitmap_ack8, R.drawable.xnav_bitmap_ack10, 
				R.drawable.xnav_bitmap_ack12, R.drawable.xnav_bitmap_ack14, R.drawable.xnav_bitmap_ack16, 
				R.drawable.xnav_bitmap_ack18, R.drawable.xnav_bitmap_ack20, R.drawable.xnav_bitmap_ack24});
	}
	
	public void createNodeClasses() {
		// Ala nodeclasses
		PlowRect r1 = new PlowRect(cmnAla, 0D, 0D, 50D, 1.5D, Plow.COLOR_WHITE, Plow.COLOR_BLACK,true,false,false);
		PlowRect r2 = new PlowRect(cmnAla, 0.4D, 0.3D, 0.9D, 0.9D, Plow.COLOR_RED, Plow.COLOR_BLACK,true,true,true);
		PlowAnnot a1 = new PlowAnnot(cmnAla, 4D, 1.1D, 12, Plow.COLOR_BLACK, 0, PlowAnnot.NEXT_RELATIVE_POSITION, 0);
		PlowAnnot a2 = new PlowAnnot(cmnAla, 10D, 1.1D, 12, Plow.COLOR_BLACK, 0, PlowAnnot.RELATIVE_POSITION, 1);
		PlowAnnot a3 = new PlowAnnot(cmnAla, 26D, 1.1D, 12, Plow.COLOR_BLACK, 0, PlowAnnot.RELATIVE_POSITION, 2);
		PlowAnnotPixmap p1 = new PlowAnnotPixmap(cmnAla, 2D, 0.28D, 0);
		PlowAnnotPixmap p2 = new PlowAnnotPixmap(cmnAla, 3D, 0.28D, 1);
		alaNcAAlarm = new PlowNodeClass(cmnAla);
		alaNcAAlarm.insert(r1);
		alaNcAAlarm.insert(r2);
		alaNcAAlarm.insert(a1);
		alaNcAAlarm.insert(a2);
		alaNcAAlarm.insert(a3);
		alaNcAAlarm.insert(p1);
		alaNcAAlarm.insert(p2);
		cmnAla.insert_nc(alaNcAAlarm);		

		r1 = new PlowRect(cmnAla, 0D, 0D, 50D, 1.5D, Plow.COLOR_WHITE, Plow.COLOR_BLACK,true,false,false);
		r2 = new PlowRect(cmnAla, 0.4D, 0.3D, 0.9D, 0.9D, Plow.COLOR_YELLOW, Plow.COLOR_BLACK,true,true,true);
		a1 = new PlowAnnot(cmnAla, 4D, 1.1D, 12, Plow.COLOR_BLACK, 0, PlowAnnot.NEXT_RELATIVE_POSITION, 0);
		a2 = new PlowAnnot(cmnAla, 10D, 1.1D, 12, Plow.COLOR_BLACK, 0, PlowAnnot.RELATIVE_POSITION, 1);
		a3 = new PlowAnnot(cmnAla, 26D, 1.1D, 12, Plow.COLOR_BLACK, 0, PlowAnnot.RELATIVE_POSITION, 2);
		p1 = new PlowAnnotPixmap(cmnAla, 2D, 0.28D, 0);
		p2 = new PlowAnnotPixmap(cmnAla, 3D, 0.28D, 1);
		alaNcBAlarm = new PlowNodeClass(cmnAla);
		alaNcBAlarm.insert(r1);
		alaNcBAlarm.insert(r2);
		alaNcBAlarm.insert(a1);
		alaNcBAlarm.insert(a2);
		alaNcBAlarm.insert(a3);
		alaNcBAlarm.insert(p1);
		alaNcBAlarm.insert(p2);
		cmnAla.insert_nc(alaNcBAlarm);		

		r1 = new PlowRect(cmnAla, 0D, 0D, 50D, 1.5D, Plow.COLOR_WHITE, Plow.COLOR_BLACK,true,false,false);
		r2 = new PlowRect(cmnAla, 0.4D, 0.3D, 0.9D, 0.9D, Plow.COLOR_BLUE, Plow.COLOR_BLACK,true,true,true);
		a1 = new PlowAnnot(cmnAla, 4D, 1.1D, 12, Plow.COLOR_BLACK, 0, PlowAnnot.NEXT_RELATIVE_POSITION, 0);
		a2 = new PlowAnnot(cmnAla, 10D, 1.1D, 12, Plow.COLOR_BLACK, 0, PlowAnnot.RELATIVE_POSITION, 1);
		a3 = new PlowAnnot(cmnAla, 26D, 1.1D, 12, Plow.COLOR_BLACK, 0, PlowAnnot.RELATIVE_POSITION, 2);
		p1 = new PlowAnnotPixmap(cmnAla, 2D, 0.28D, 0);
		p2 = new PlowAnnotPixmap(cmnAla, 3D, 0.28D, 1);
		alaNcCAlarm = new PlowNodeClass(cmnAla);
		alaNcCAlarm.insert(r1);
		alaNcCAlarm.insert(r2);
		alaNcCAlarm.insert(a1);
		alaNcCAlarm.insert(a2);
		alaNcCAlarm.insert(a3);
		alaNcCAlarm.insert(p1);
		alaNcCAlarm.insert(p2);
		cmnAla.insert_nc(alaNcCAlarm);		

		r1 = new PlowRect(cmnAla, 0D, 0D, 50D, 1.5D, Plow.COLOR_WHITE, Plow.COLOR_BLACK,true,false,false);
		r2 = new PlowRect(cmnAla, 0.4D, 0.3D, 0.9D, 0.9D, Plow.COLOR_VIOLET, Plow.COLOR_BLACK,true,true,true);
		a1 = new PlowAnnot(cmnAla, 4D, 1.1D, 12, Plow.COLOR_BLACK, 0, PlowAnnot.NEXT_RELATIVE_POSITION, 0);
		a2 = new PlowAnnot(cmnAla, 10D, 1.1D, 12, Plow.COLOR_BLACK, 0, PlowAnnot.RELATIVE_POSITION, 1);
		a3 = new PlowAnnot(cmnAla, 26D, 1.1D, 12, Plow.COLOR_BLACK, 0, PlowAnnot.RELATIVE_POSITION, 2);
		p1 = new PlowAnnotPixmap(cmnAla, 2D, 0.28D, 0);
		p2 = new PlowAnnotPixmap(cmnAla, 3D, 0.28D, 1);
		alaNcDAlarm = new PlowNodeClass(cmnAla);
		alaNcDAlarm.insert(r1);
		alaNcDAlarm.insert(r2);
		alaNcDAlarm.insert(a1);
		alaNcDAlarm.insert(a2);
		alaNcDAlarm.insert(a3);
		alaNcDAlarm.insert(p1);
		alaNcDAlarm.insert(p2);
		cmnAla.insert_nc(alaNcDAlarm);		

		r1 = new PlowRect(cmnAla, 0D, 0D, 50D, 1.5D, Plow.COLOR_WHITE, Plow.COLOR_BLACK,true,false,false);
		a1 = new PlowAnnot(cmnAla, 5D, 1.1D, 12, Plow.COLOR_BLACK, 0, PlowAnnot.NEXT_RELATIVE_POSITION, 0);
		a2 = new PlowAnnot(cmnAla, 11D, 1.1D, 12, Plow.COLOR_BLACK, 0, PlowAnnot.RELATIVE_POSITION, 1);
		a3 = new PlowAnnot(cmnAla, 26D, 1.1D, 12, Plow.COLOR_BLACK, 0, PlowAnnot.RELATIVE_POSITION, 2);
		p1 = new PlowAnnotPixmap(cmnAla, 2D, 0.28D, 0);
		p2 = new PlowAnnotPixmap(cmnAla, 3D, 0.28D, 1);
		alaNcAlarm = new PlowNodeClass(cmnAla);
		alaNcAlarm.insert(r1);
		alaNcAlarm.insert(a1);
		alaNcAlarm.insert(a2);
		alaNcAlarm.insert(a3);
		alaNcAlarm.insert(p1);
		alaNcAlarm.insert(p2);
		cmnAla.insert_nc(alaNcAlarm);		

		// Eve nodeclass
		r1 = new PlowRect(cmnEve, 0D, 0D, 50D, 1.5D, Plow.COLOR_WHITE, Plow.COLOR_BLACK,true,false,false);
		r2 = new PlowRect(cmnEve, 0.4D, 0.3D, 0.9D, 0.9D, Plow.COLOR_RED, Plow.COLOR_BLACK,true,true,true);
		a1 = new PlowAnnot(cmnEve, 4D, 1.1D, 12, Plow.COLOR_BLACK, 0, PlowAnnot.NEXT_RELATIVE_POSITION, 0);
		a2 = new PlowAnnot(cmnEve, 10D, 1.1D, 12, Plow.COLOR_BLACK, 0, PlowAnnot.RELATIVE_POSITION, 1);
		a3 = new PlowAnnot(cmnEve, 26D, 1.1D, 12, Plow.COLOR_BLACK, 0, PlowAnnot.RELATIVE_POSITION, 2);
		p1 = new PlowAnnotPixmap(cmnEve, 2D, 0.28D, 0);
		eveNcAAlarm = new PlowNodeClass(cmnEve);
		eveNcAAlarm.insert(r1);
		eveNcAAlarm.insert(r2);
		eveNcAAlarm.insert(a1);
		eveNcAAlarm.insert(a2);
		eveNcAAlarm.insert(a3);
		eveNcAAlarm.insert(p1);
		cmnEve.insert_nc(eveNcAAlarm);		

		r1 = new PlowRect(cmnEve, 0D, 0D, 50D, 1.5D, Plow.COLOR_WHITE, Plow.COLOR_BLACK,true,false,false);
		r2 = new PlowRect(cmnEve, 0.4D, 0.3D, 0.9D, 0.9D, Plow.COLOR_YELLOW, Plow.COLOR_BLACK,true,true,true);
		a1 = new PlowAnnot(cmnEve, 4D, 1.1D, 12, Plow.COLOR_BLACK, 0, PlowAnnot.NEXT_RELATIVE_POSITION, 0);
		a2 = new PlowAnnot(cmnEve, 10D, 1.1D, 12, Plow.COLOR_BLACK, 0, PlowAnnot.RELATIVE_POSITION, 1);
		a3 = new PlowAnnot(cmnEve, 26D, 1.1D, 12, Plow.COLOR_BLACK, 0, PlowAnnot.RELATIVE_POSITION, 2);
		p1 = new PlowAnnotPixmap(cmnEve, 2D, 0.28D, 0);
		eveNcBAlarm = new PlowNodeClass(cmnEve);
		eveNcBAlarm.insert(r1);
		eveNcBAlarm.insert(r2);
		eveNcBAlarm.insert(a1);
		eveNcBAlarm.insert(a2);
		eveNcBAlarm.insert(a3);
		eveNcBAlarm.insert(p1);
		cmnEve.insert_nc(eveNcBAlarm);		

		r1 = new PlowRect(cmnEve, 0D, 0D, 50D, 1.5D, Plow.COLOR_WHITE, Plow.COLOR_BLACK,true,false,false);
		r2 = new PlowRect(cmnEve, 0.4D, 0.3D, 0.9D, 0.9D, Plow.COLOR_BLUE, Plow.COLOR_BLACK,true,true,true);
		a1 = new PlowAnnot(cmnEve, 4D, 1.1D, 12, Plow.COLOR_BLACK, 0, PlowAnnot.NEXT_RELATIVE_POSITION, 0);
		a2 = new PlowAnnot(cmnEve, 10D, 1.1D, 12, Plow.COLOR_BLACK, 0, PlowAnnot.RELATIVE_POSITION, 1);
		a3 = new PlowAnnot(cmnEve, 26D, 1.1D, 12, Plow.COLOR_BLACK, 0, PlowAnnot.RELATIVE_POSITION, 2);
		p1 = new PlowAnnotPixmap(cmnEve, 2D, 0.28D, 0);
		eveNcCAlarm = new PlowNodeClass(cmnEve);
		eveNcCAlarm.insert(r1);
		eveNcCAlarm.insert(r2);
		eveNcCAlarm.insert(a1);
		eveNcCAlarm.insert(a2);
		eveNcCAlarm.insert(a3);
		eveNcCAlarm.insert(p1);
		cmnEve.insert_nc(eveNcCAlarm);		

		r1 = new PlowRect(cmnEve, 0D, 0D, 50D, 1.5D, Plow.COLOR_WHITE, Plow.COLOR_BLACK,true,false,false);
		r2 = new PlowRect(cmnEve, 0.4D, 0.3D, 0.9D, 0.9D, Plow.COLOR_VIOLET, Plow.COLOR_BLACK,true,true,true);
		a1 = new PlowAnnot(cmnEve, 4D, 1.1D, 12, Plow.COLOR_BLACK, 0, PlowAnnot.NEXT_RELATIVE_POSITION, 0);
		a2 = new PlowAnnot(cmnEve, 10D, 1.1D, 12, Plow.COLOR_BLACK, 0, PlowAnnot.RELATIVE_POSITION, 1);
		a3 = new PlowAnnot(cmnEve, 26D, 1.1D, 12, Plow.COLOR_BLACK, 0, PlowAnnot.RELATIVE_POSITION, 2);
		p1 = new PlowAnnotPixmap(cmnEve, 2D, 0.28D, 0);
		eveNcDAlarm = new PlowNodeClass(cmnEve);
		eveNcDAlarm.insert(r1);
		eveNcDAlarm.insert(r2);
		eveNcDAlarm.insert(a1);
		eveNcDAlarm.insert(a2);
		eveNcDAlarm.insert(a3);
		eveNcDAlarm.insert(p1);
		cmnEve.insert_nc(eveNcDAlarm);		

		r1 = new PlowRect(cmnEve, 0D, 0D, 50D, 1.5D, Plow.COLOR_WHITE, Plow.COLOR_BLACK,true,false,false);
		r2 = new PlowRect(cmnEve, 0.4D, 0.3D, 0.9D, 0.9D, Plow.COLOR_GREEN, Plow.COLOR_BLACK,true,true,true);
		a1 = new PlowAnnot(cmnEve, 4D, 1.1D, 12, Plow.COLOR_BLACK, 0, PlowAnnot.NEXT_RELATIVE_POSITION, 0);
		a2 = new PlowAnnot(cmnEve, 10D, 1.1D, 12, Plow.COLOR_BLACK, 0, PlowAnnot.RELATIVE_POSITION, 1);
		a3 = new PlowAnnot(cmnEve, 26D, 1.1D, 12, Plow.COLOR_BLACK, 0, PlowAnnot.RELATIVE_POSITION, 2);
		p1 = new PlowAnnotPixmap(cmnEve, 2D, 0.28D, 0);
		eveNcInfo = new PlowNodeClass(cmnEve);
		eveNcInfo.insert(r1);
		eveNcInfo.insert(r2);
		eveNcInfo.insert(a1);
		eveNcInfo.insert(a2);
		eveNcInfo.insert(a3);
		eveNcInfo.insert(p1);
		cmnEve.insert_nc(eveNcInfo);		

		r1 = new PlowRect(cmnEve, 0D, 0D, 50D, 1.5D, Plow.COLOR_WHITE, Plow.COLOR_BLACK,true,false,false);
		a1 = new PlowAnnot(cmnEve, 4D, 1.1D, 12, Plow.COLOR_BLACK, 0, PlowAnnot.NEXT_RELATIVE_POSITION, 0);
		a2 = new PlowAnnot(cmnEve, 10D, 1.1D, 12, Plow.COLOR_BLACK, 0, PlowAnnot.RELATIVE_POSITION, 1);
		a3 = new PlowAnnot(cmnEve, 26D, 1.1D, 12, Plow.COLOR_BLACK, 0, PlowAnnot.RELATIVE_POSITION, 2);
		p1 = new PlowAnnotPixmap(cmnEve, 2D, 0.28D, 0);
		eveNcAlarm = new PlowNodeClass(cmnEve);
		eveNcAlarm.insert(r1);
		eveNcAlarm.insert(a1);
		eveNcAlarm.insert(a2);
		eveNcAlarm.insert(a3);
		eveNcAlarm.insert(p1);
		cmnEve.insert_nc(eveNcAlarm);		
	}

	public void acknowledge() {
		for( PlowNode n = (PlowNode)cmnAla.getFirst(); n != null; n = (PlowNode)cmnAla.getNext(n)) {
			AEvItemAlarm item = (AEvItemAlarm)n.getUserData();

			if((item.e.eventStatus & Mh.mh_mEventStatus_NotAck) != 0) {
				ackEventId = item.e.eventId;
				sendAck = true;
				break;
			}
  	  	}
	}
	
	private class MhReceiveTask extends AsyncTask<Void, Void, Void> {
	    ObjectInputStream in;
	    ObjectOutputStream out;
	    boolean keepRunning = true;
	    
		@Override
		protected Void doInBackground(Void... params) {
			try {
				mhSocket = new Socket(pwrHost, PORT);
				mhSocket.setKeepAlive(true);
					
		        out = new ObjectOutputStream( mhSocket.getOutputStream());
		        out.flush();
		        
		        in = new ObjectInputStream( mhSocket.getInputStream());
		        mhSocket.setSoTimeout(1000);
			}
			catch(IOException e) {
				System.out.println("IOException ObjectOutputStream/ObjectInputStream");
				return null;
			}

			try {
		        maxNrOfAlarms = in.readInt();
		        if(maxNrOfAlarms == -1) {
		        	System.out.println("Max number of alarm exceeded, restart");
		        	this.keepRunning = false;
		        }
		        else {
		        	maxNrOfEvents = in.readInt();
		        	int nrOfAlarms = in.readInt();
		        	if (nrOfAlarms > 0) {
		        		Vector alarmVec = new Vector(nrOfAlarms);
		        		alarmVec = (Vector)in.readObject();
	        			System.out.println("Alarm size " + alarmVec.size());
		        		for ( int i = 0; i < alarmVec.size(); i++) {
		        			System.out.println("Alarm item: " + alarmVec.get(i));
		        			newMess((MhrEvent)alarmVec.get(i));		        			
			        		System.out.println("Alarm: (" + nrOfAlarms + ")  " + ((MhrEvent)alarmVec.get(i)).eventPrio + " " + ((MhrEvent)alarmVec.get(i)).eventText + " " + ((MhrEvent)alarmVec.get(i)).eventName );
		        		}
		        		cmnAla.configure();
		        		//appl.invalidateView();
		        	}
		        	int nrOfEvents = in.readInt();
		        	if(nrOfEvents > 0) {
		        		Vector eventVec = new Vector(nrOfEvents);
		        		eventVec = (Vector)in.readObject();
		        		for ( int i = 0; i < eventVec.size(); i++) {
		        			new AEvItemEvent(aev, (MhrEvent)eventVec.get(i), null, Plow.DEST_AFTER);
		        			System.out.println("Event: " + ((MhrEvent)eventVec.get(i)).eventText);
		        		}
		        		cmnEve.configure();
		        	}
		        }
		    }
			catch(SocketTimeoutException e) {				
			}
			catch(Exception e) {
				System.out.println(e.toString());
				this.keepRunning = false;
			}

			while(this.keepRunning) {
				try {
					// Wait for events messages from server.
					MhrEvent ev = (MhrEvent)in.readObject();
					newMess(ev);

					if (!(appl.getCurrentCmn() == cmnAla || appl.getCurrentCmn() == cmnEve))
						this.keepRunning = false;
				}
				catch(SocketTimeoutException e) {				
					if (!(appl.getCurrentCmn() == cmnAla || appl.getCurrentCmn() == cmnEve))
						this.keepRunning = false;

					System.out.println("Timeout " + sendAck + " " + ackEventId);
					if (  sendAck && ackEventId != null) {
						sendAck = false;
						System.out.println("Send ack" + ackEventId);
						try {
							out.writeObject(ackEventId);
							out.flush();
						}
						catch (Exception ex) {
							System.out.println("Mh ack send exception");
						}
					}
				}
		        catch(ClassNotFoundException e) {
		        	System.out.println("Exception in wait for messages: " + e.toString());
		        	System.out.println("Error, can't interpret message from server");
		        }
		        catch(Exception e) {
		          System.out.println("Exception in wait for messages: " + e.toString());
		          System.out.println("Terminating");
		          this.keepRunning = false;	
		        }
			}
			try {
		        in.close();
		        out.close();
			}
			catch(Exception e) {
			}
			
			return null;
		}

		@Override
		protected void onPostExecute(Void params) {
			super.onPostExecute(params);
		}
	}

}
