package jpwr.pwrxtt;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.net.URL;
import java.util.StringTokenizer;
import java.util.Timer;
import java.util.TimerTask;
import java.util.Vector;

import android.os.AsyncTask;
import android.os.Bundle;
import android.os.Handler;
import android.support.v4.app.FragmentManager;
import android.text.Editable;
import android.text.InputType;
import android.util.Log;
import android.app.Activity;
import android.app.AlertDialog;
import android.app.Dialog;
import android.app.DialogFragment;
import android.content.Context;
import android.content.DialogInterface;
import android.content.DialogInterface.OnClickListener;
import android.content.res.AssetManager;
import android.content.res.Configuration;
import android.content.res.Resources;
import android.view.*;
import android.widget.EditText;
import android.graphics.*;
import jpwr.rt.*;
import jpwr.app.*;
import jpwr.jopg.*;

public class MainActivity extends Activity implements PlowAppl, GraphApplIfc, GdhApplIfc, OnClickListener, AEvAppl {

	Timer timer = new Timer();
	MainView view;
	Gdh gdh = null;
	Handler handler = new Handler();
	MainActivity appl;
	float lastTouchX;
	float lastTouchY;
	float downTouchX;
	float downTouchY;
	private EditText editInput;
	
	PlowDraw gdraw;
	PlowCmnIfc currentCmn;
	PlowRect r1;
	PlowAnnot a1;
	PlowAnnot a11;
	PlowAnnotPixmap p1;
	PlowNode zoomIn;
	PlowNode zoomOut;
	String inputFullName;
	Graph graph = null;
	Vector<String> graphObject = new Vector<String>();
	int inputType;
	AlertDialog inputDialog = null;
	static private boolean initDone = false;
	String pwrHost = null;
	AEv aev = null;
	
	Vector<PlowCmnIfc> cmnList = new Vector<PlowCmnIfc>();
	
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		//setContentView(R.layout.activity_main);
		appl = this;
		view = new MainView(this);
		setContentView(view);
		Log.i("PwrXtt", "Starting");
		view.invalidate();
		timer.schedule( new MyTask(), 0, 2000);

		
		PlowCmn cmn = new PlowCmn(this, 15D);		
		gdraw = new PlowDraw(this);
		cmn.setGDraw(gdraw);
		currentCmn = cmn;
		cmnList.add(cmn);
		
		editInput = new EditText(this);
		editInput.setText("10.0.2.2");
		new AlertDialog.Builder(this)
			.setTitle(R.string.app_name)
			.setMessage("Enter target node")
			.setView(editInput)
			.setPositiveButton("Ok", this)
			.setNegativeButton("Cancel", null)
			.show();
	}
	
	public void onClick(DialogInterface dialog, int position) {
		if ( dialog == inputDialog) {
			Editable value = editInput.getText();
			System.out.println( "Change Value: " + value.toString());
			new GdhTask().execute(new GdhTaskArg(GdhTask.CHANGE_VALUE, value.toString()));			
		}
		else {
			Editable value = editInput.getText();
			pwrHost = value.toString();
			System.out.println( "Input: " + value.toString());
			//MainActivity callingActivity = (MainActivity) this;
			//callingActivity.onUserSelectValue(value);
			dialog.dismiss();
			
			new GdhTask().execute(new GdhTaskArg(GdhTask.ROOTLIST,(AXttItemBase)null));

		}
	}
	
	public void onConfigurationChanged(Configuration newConfig) {
		super.onConfigurationChanged(newConfig);
		
		view.invalidate();
	}

	@Override
	public boolean onPrepareOptionsMenu(Menu menu) {

		MenuItem item_pageup = menu.findItem(R.id.pageup_option);
		MenuItem item_pagedown = menu.findItem(R.id.pagedown_option);
		MenuItem item_openobject = menu.findItem(R.id.openobject_option);
		MenuItem item_opengraph = menu.findItem(R.id.opengraph_option);
		MenuItem item_openclassgraph = menu.findItem(R.id.openclassgraph_option);
		MenuItem item_openplc = menu.findItem(R.id.openplc_option);
		MenuItem item_opencrr = menu.findItem(R.id.opencrr_option);
		MenuItem item_alarmack = menu.findItem(R.id.alarmack_option);
		MenuItem item_changevalue = menu.findItem(R.id.changevalue_option);
		if ( aev != null && currentCmn == aev.getCmnAla()) {
			// Alarm list
			item_pageup.setVisible(true);
			item_pagedown.setVisible(true);
			item_openobject.setVisible(true);
			item_opengraph.setVisible(true);
			item_openclassgraph.setVisible(true);
			item_openplc.setVisible(true);
			item_opencrr.setVisible(true);
			item_alarmack.setVisible(true);
			item_changevalue.setVisible(false);
		}
		else if ( aev != null && currentCmn == aev.getCmnEve()) {
			// Event list
			item_pageup.setVisible(true);
			item_pagedown.setVisible(true);
			item_openobject.setVisible(true);
			item_opengraph.setVisible(true);
			item_openclassgraph.setVisible(true);
			item_openplc.setVisible(true);
			item_opencrr.setVisible(true);
			item_alarmack.setVisible(false);
			item_changevalue.setVisible(false);
		}
		else if ( currentCmn == cmnList.get(0)) {
			// Navigator
			item_pageup.setVisible(true);
			item_pagedown.setVisible(true);
			item_openobject.setVisible(true);
			item_opengraph.setVisible(true);
			item_openclassgraph.setVisible(true);
			item_openplc.setVisible(true);
			item_opencrr.setVisible(true);
			item_alarmack.setVisible(false);
			item_changevalue.setVisible(true);
		}
		else if ( currentCmn.type() == PlowCmnIfc.TYPE_FLOW) {
			// Plc
			item_pageup.setVisible(true);
			item_pagedown.setVisible(true);
			item_openobject.setVisible(true);
			item_opengraph.setVisible(true);
			item_openclassgraph.setVisible(true);
			item_openplc.setVisible(true);
			item_opencrr.setVisible(true);
			item_alarmack.setVisible(false);
			item_changevalue.setVisible(false);
		}
		else if ( currentCmn.type() == PlowCmnIfc.TYPE_GRAPH) {
			// Graph
			item_pageup.setVisible(false);
			item_pagedown.setVisible(false);
			item_openobject.setVisible(false);
			item_opengraph.setVisible(false);
			item_openclassgraph.setVisible(false);
			item_openplc.setVisible(false);
			item_opencrr.setVisible(false);
			item_alarmack.setVisible(false);
			item_changevalue.setVisible(false);
		}
		else {
			// Default
			item_pageup.setVisible(true);
			item_pagedown.setVisible(true);
			item_openobject.setVisible(true);
			item_opengraph.setVisible(true);
			item_openclassgraph.setVisible(true);
			item_openplc.setVisible(true);
			item_opencrr.setVisible(true);
			item_alarmack.setVisible(false);
			item_changevalue.setVisible(false);
		}
	
		// this.invalidateOptionsMenu();

		return true;
	}

	@Override
	public boolean onOptionsItemSelected(MenuItem item) {
		switch( item.getItemId()) {
		case R.id.zoomin_option:
			currentCmn.zoom(1.25D);
			view.invalidate();
			System.out.println("Zoom in");
			break;			
		case R.id.zoomout_option:
			currentCmn.zoom(0.8D);
			view.invalidate();
			System.out.println("Zoom in");
			break;			
		case R.id.pageup_option:
			currentCmn.pageUp();
			view.invalidate();
			System.out.println("Page up");
			break;			
		case R.id.pagedown_option:
			currentCmn.pageDown();
			view.invalidate();
			System.out.println("Page down");
			break;			
		case R.id.close_option:
			switch(currentCmn.type()) {
			case PlowCmnIfc.TYPE_PLOW: {
				if ( cmnList.size() == 1) {
					// TODO Close application
					break;
				}

				PlowCmn cmn = (PlowCmn)currentCmn;
				AXtt axtt = (AXtt)currentCmn.getUserData();
				PlowNode o = (PlowNode)cmn.get(0);
				AXttItemObject itemo = (AXttItemObject)o.getUserData();
				itemo.close(axtt);
				currentCmn = cmnList.get(cmnList.size()-2);
				cmnList.removeElementAt(cmnList.size()-1);
				System.out.println( "cmnList.size() " + cmnList.size());
				view.invalidate();				
				break;
			}
			case PlowCmnIfc.TYPE_FLOW:
				new GdhTask().execute(new GdhTaskArg(GdhTask.DYNAMIC_CLOSE, currentCmn));
				System.out.println( "cmnList.size() " + cmnList.size());
				currentCmn = cmnList.get(cmnList.size()-2);
				cmnList.removeElementAt(cmnList.size()-1);
				System.out.println( "cmnList.size() " + cmnList.size());
				view.invalidate();
				System.out.println("Close flow");
				break;
			case PlowCmnIfc.TYPE_EV:
				System.out.println( "cmnList.size() " + cmnList.size());				
				currentCmn = cmnList.get(cmnList.size()-1);
				aev = null;
				view.invalidate();
				System.out.println("Close ev");
				break;
			case PlowCmnIfc.TYPE_GRAPH:
				new GdhTask().execute(new GdhTaskArg(GdhTask.DYNAMIC_CLOSE, currentCmn));
				System.out.println( "cmnList.size() " + cmnList.size());
				if (graphObject.size() > 0)
					graphObject.removeElementAt(graphObject.size()-1);
				currentCmn = cmnList.get(cmnList.size()-2);
				cmnList.removeElementAt(cmnList.size()-1);
				System.out.println( "cmnList.size() " + cmnList.size());
				view.invalidate();
				System.out.println("Close graph");
				break;
			}
			break;
		case R.id.openobject_option: {
			System.out.println("Open object");
			switch (currentCmn.type()) {
			case PlowCmnIfc.TYPE_PLOW: {
				PlowNode selectedNode = (PlowNode)currentCmn.getSelect();
				if ( selectedNode != null) {
					AXttItemBase[] items = new AXttItemBase[1];
					AXttItemBase baseItem = (AXttItemBase)selectedNode.getUserData();
					new GdhTask().execute(new GdhTaskArg(GdhTask.OPEN_ATTRIBUTES, baseItem));
				}
				break;
			}
			case PlowCmnIfc.TYPE_FLOW: {
				FlowNode selectedNode = (FlowNode)currentCmn.getSelect();
				new GdhTask().execute(new GdhTaskArg(GdhTask.OPEN_FLOWNODE, selectedNode));
				break;
			}
			}
			break;			
		}
		case R.id.opencrr_option: {
			System.out.println("Open object");
			switch (currentCmn.type()) {
			case PlowCmnIfc.TYPE_PLOW: {
				PlowNode selectedNode = (PlowNode)currentCmn.getSelect();
				if ( selectedNode != null) {
					AXttItemBase[] items = new AXttItemBase[1];
					AXttItemBase baseItem = (AXttItemBase)selectedNode.getUserData();
					new GdhTask().execute(new GdhTaskArg(GdhTask.CROSSREFERENCES, baseItem));
				}
				break;
			}
			case PlowCmnIfc.TYPE_FLOW: {
				FlowNode selectedNode = (FlowNode)currentCmn.getSelect();
				if ( selectedNode != null) {
					new GdhTask().execute(new GdhTaskArg(GdhTask.FLOW_CROSSREFERENCE, selectedNode));
				}
				break;
			}
			}
			break;			
		}
		case R.id.openplc_option: {
			System.out.println("Open plc");					
			switch (currentCmn.type()) {
			case PlowCmnIfc.TYPE_PLOW: {
				PlowNode selectedNode = (PlowNode)currentCmn.getSelect();
				if ( selectedNode != null) {
					AXttItemBase baseItem = (AXttItemBase)selectedNode.getUserData();
					if ( baseItem instanceof AXttItemObject) {
						if (((AXttItemObject)baseItem).getClassid() == Pwrb.cClass_plc) {
							PwrtObjid woid = ((AXttItemObject)baseItem).getWindowObjid();
							new ReaderTask().execute(new ReaderTaskArg(ReaderTask.FLOW_READ, woid, null));
						}
						else {
							PwrtObjid oid = ((AXttItemObject)baseItem).getObjid();
							new GdhTask().execute(new GdhTaskArg(GdhTask.OPEN_FLOW, oid));
						}
					}
					else if ( baseItem instanceof AXttItemCrr) {
						new GdhTask().execute(new GdhTaskArg(GdhTask.OPEN_FLOWCROSS, baseItem));
					}
				}
				break;
			}
			case PlowCmnIfc.TYPE_FLOW: {
				FlowNode selectedNode = (FlowNode)currentCmn.getSelect();
				if ( selectedNode != null) {
					new GdhTask().execute(new GdhTaskArg(GdhTask.OPEN_FLOWSUBWINDOW, selectedNode));
					System.out.println("Open object");					
				}
				break;
			}
			}
			break;
		}
		case R.id.opengraph_option: {
			System.out.println("Open graph");					
			switch (currentCmn.type()) {
			case PlowCmnIfc.TYPE_PLOW: {
				PlowNode selectedNode = (PlowNode)currentCmn.getSelect();
				if ( selectedNode != null) {
					AXttItemBase baseItem = (AXttItemBase)selectedNode.getUserData();
					if ( baseItem instanceof AXttItemObject) {
						PwrtObjid oid = ((AXttItemObject)baseItem).getObjid();
						new GdhTask().execute(new GdhTaskArg(GdhTask.OPEN_DEFGRAPH, oid));
					}
				}
				break;
			}
			case PlowCmnIfc.TYPE_FLOW: {
				break;
			}
			}
			break;
		}
		case R.id.openclassgraph_option: {
			System.out.println("Open Object graph " + currentCmn.type());								
			switch (currentCmn.type()) {
			case PlowCmnIfc.TYPE_PLOW: {
				PlowNode selectedNode = (PlowNode)currentCmn.getSelect();
				if ( selectedNode != null) {
					AXttItemBase baseItem = (AXttItemBase)selectedNode.getUserData();
					if ( baseItem instanceof AXttItemObject) {
						PwrtAttrRef aref = new PwrtAttrRef(((AXttItemObject)baseItem).getObjid());
						new GdhTask().execute(new GdhTaskArg(GdhTask.OPEN_CLASSGRAPH, aref));
					}
				}
				break;
			}
			case PlowCmnIfc.TYPE_FLOW: {
				break;
			}
			}
			break;
		}
		case R.id.changevalue_option: {
			System.out.println("Change value");					
			switch (currentCmn.type()) {
			case PlowCmnIfc.TYPE_PLOW: {
				PlowNode selectedNode = (PlowNode)currentCmn.getSelect();
				if ( selectedNode != null) {
					AXttItemBase baseItem = (AXttItemBase)selectedNode.getUserData();
					if ( baseItem instanceof AXttItemAttr) {
						inputFullName = ((AXttItemAttr)baseItem).getFullName();
						inputType = ((AXttItemAttr)baseItem).getType();
						editInput = new EditText(this);
						inputDialog = new AlertDialog.Builder(this)
						.setTitle(R.string.app_name)
						.setMessage("Change Value")
						.setView(editInput)
						.setPositiveButton("Ok", this)
						.setNegativeButton("Cancel", null)
						.show();
					}
				}
				break;
			}
			}
			break;
		}
		case R.id.alarmlist_option: {
			System.out.println("Open Alarmlist");					

			if ( aev == null) {
				PlowCmnEv cmnAla = new PlowCmnEv(appl, 15D);		
				cmnAla.setGDraw(gdraw);
				PlowCmnEv cmnEve = new PlowCmnEv(appl, 15D);		
				cmnEve.setGDraw(gdraw);
				currentCmn = cmnAla;
				aev = new AEv(cmnAla, cmnEve, appl);
				cmnAla.setUserData(aev);
				cmnEve.setUserData(aev);
				view.invalidate();
			}
			else {
				currentCmn = aev.getCmnAla();
				view.invalidate();
			}
			break;
		}
		case R.id.eventlist_option: {
			System.out.println("Open Eventlist");					

			if ( aev == null) {
				PlowCmnEv cmnAla = new PlowCmnEv(appl, 15D);		
				cmnAla.setGDraw(gdraw);
				PlowCmnEv cmnEve = new PlowCmnEv(appl, 15D);		
				cmnEve.setGDraw(gdraw);
				currentCmn = cmnEve;
				aev = new AEv(cmnAla, cmnEve, appl);
				cmnAla.setUserData(aev);
				cmnEve.setUserData(aev);
				view.invalidate();
			}
			else {
				currentCmn = aev.getCmnEve();
				view.invalidate();
			}
			break;
		}
		case R.id.alarmack_option: {

			if ( aev != null && currentCmn == aev.getCmnAla()) {
				aev.acknowledge();
				view.invalidate();
			}
			else {
				currentCmn = aev.getCmnEve();
				view.invalidate();
			}
			break;
		}
		}
		return true;
	}

	public void eventHandler(PlowEvent e) {
		switch (e.type) {
		case PlowEvent.TYPE_CLICK:
			if ( e.object == null)
				return;
			
			switch(currentCmn.type()) {
			case PlowCmnIfc.TYPE_PLOW:
				if ( e.inIcon) {
					// Open children
					System.out.println("Click event");
					AXttItemBase[] items = new AXttItemBase[1];
					AXttItemBase item = (AXttItemBase)((PlowNode)e.object).getUserData();
					new GdhTask().execute(new GdhTaskArg(GdhTask.OPEN_CHILDREN, item));
				}
				else {
					// Select
					if ( currentCmn.getSelect() == e.object) {
						currentCmn.select(null);
						((PlowNode)e.object).setInvert(false);
						//view.invalidate();
					}
					else {
						if ( currentCmn.getSelect() != null)
							((PlowNode)currentCmn.getSelect()).setInvert(false);
						currentCmn.select((PlowNode)e.object);
						((PlowNode)e.object).setInvert(true);					
						//view.invalidate();
					}
				}	
				break;
			case PlowCmnIfc.TYPE_EV:
				// Select
				if ( currentCmn.getSelect() == e.object) {
					currentCmn.select(null);
					((PlowNode)e.object).setInvert(false);
					//view.invalidate();
				}
				else {
					if ( currentCmn.getSelect() != null)
						((PlowNode)currentCmn.getSelect()).setInvert(false);
					currentCmn.select((PlowNode)e.object);
					((PlowNode)e.object).setInvert(true);					
					//view.invalidate();
				}
				break;
			case PlowCmnIfc.TYPE_FLOW:
				if (e.object instanceof FlowNode) {
					Object s = currentCmn.getSelect();
					if ( s == e.object) {
						((FlowNode)e.object).setSelect(false);
						currentCmn.selectClear();
						view.invalidate();
					}
					else if ( s != null) {
						((FlowNode)s).setSelect(false);						
						((FlowNode)e.object).setSelect(true);
						currentCmn.select(e.object);
						view.invalidate();
					}
					else {
						((FlowNode)e.object).setSelect(true);						
						currentCmn.select(e.object);
						view.invalidate();
					}
				}
				break;
			}
			break;
		case PlowEvent.TYPE_OBJECT_DELETED: {
			AXtt axtt = (AXtt)currentCmn.getUserData();
			AXttItemBase item = (AXttItemBase)((PlowNode)e.object).getUserData();
			if ( item instanceof AXttItemAttr) {
				((AXttItemAttr)item).close(axtt);
			}
			else if ( item instanceof AXttItemAttrArrayElem) {
				((AXttItemAttrArrayElem)item).close(axtt);
			}
			break;
		}
		}
	}

	private class GdhTaskArg {
		int action;
		Object item;

		public GdhTaskArg(int action, Object item) {
			this.action = action;
			this.item = item;
		}
	}
	private class GdhEventArg {
			int action;
			float x;
			float y;
			public GdhEventArg(int action, float x, float y) {
				this.action = action;
				this.x = x;
				this.y = y;
			}
	}
	
	private class GdhTask extends AsyncTask<GdhTaskArg, Void, Void> {
		public static final int ROOTLIST = 1;
		public static final int SCAN = 2;
		public static final int OPEN_CHILDREN = 3;
		public static final int OPEN_ATTRIBUTES = 4;
		public static final int DYNAMIC_OPEN = 5;
		public static final int DYNAMIC_CLOSE = 6;
		public static final int DYNAMIC_UPDATE = 7;
		public static final int OPEN_FLOWNODE = 8;
		public static final int CROSSREFERENCES = 9;
		public static final int OPEN_FLOWSUBWINDOW = 10;
		public static final int OPEN_FLOW = 11;
		public static final int OPEN_FLOWCROSS = 12;
		public static final int FLOW_CROSSREFERENCE = 13;
		public static final int CHANGE_VALUE = 14;
		public static final int OPEN_DEFGRAPH = 15;
		public static final int EVENTHANDLER = 16;
		public static final int OPEN_CLASSGRAPH = 17;
		public static final int OPEN_CLASSGRAPH_NAME = 18;
		
		@Override
		protected Void doInBackground(GdhTaskArg... arg) {
			
			if ( gdh == null) {
				gdh = new Gdh(appl);		
				AXtt axtt = new AXtt((PlowCmn)currentCmn, gdh);
				currentCmn.setUserData(axtt);
				gdh.refObjectInfoList();
			}

			switch ( arg[0].action) {
			case ROOTLIST: {
				AXtt axtt = (AXtt)currentCmn.getUserData();

				System.out.println("Open rootlist");
				AXttItemObject.openRoot(axtt);
				currentCmn.configure();
				initDone = true;
				break;
			}
			case SCAN: {
				if ( currentCmn.type() != PlowCmnIfc.TYPE_PLOW)
					break;

				PlowCmn cmn = (PlowCmn)currentCmn;
				AXtt axtt = (AXtt)currentCmn.getUserData();
				
				gdh.getObjectRefInfoAll();
				for ( int i = 0; i < cmn.size(); i++) {
					AXttItemBase bitem = (AXttItemBase)((PlowNode)cmn.get(i)).getUserData();
					if ( bitem instanceof AXttItemAttr) {
						((AXttItemAttr)bitem).scan(axtt);
					}
					else if ( bitem instanceof AXttItemAttrArrayElem) {
						((AXttItemAttrArrayElem)bitem).scan(axtt);
					}
				}
				break;
			}
			case OPEN_CHILDREN: {
				AXtt axtt = (AXtt)currentCmn.getUserData();

				if (((AXttItemBase)arg[0].item).hasChildren())
					((AXttItemBase)arg[0].item).openChildren(axtt);
				else
					((AXttItemBase)arg[0].item).openAttributes(axtt);
				currentCmn.configure();
				break;
			}
			case OPEN_ATTRIBUTES: {
				AXtt axtt = (AXtt)currentCmn.getUserData();

				((AXttItemBase)arg[0].item).openAttributes(axtt);
				currentCmn.configure();
				break;
			}
			case CROSSREFERENCES: {
				AXtt axtt = (AXtt)currentCmn.getUserData();

				((AXttItemBase)arg[0].item).openCrossreferences(axtt);
				currentCmn.configure();
				break;
			}
			case DYNAMIC_OPEN:
				((FlowCmn)currentCmn).dynamicOpen();
				break;
			case DYNAMIC_CLOSE:
				((PlowCmnIfc)arg[0].item).dynamicClose();
				break;
			case DYNAMIC_UPDATE:
				gdh.getObjectRefInfoAll();
				currentCmn.dynamicUpdate();
				break;
			case OPEN_FLOWNODE: {
				FlowNode node = (FlowNode)arg[0].item;
				String oName = node.getTraceObject();
				CdhrObjid oret = gdh.nameToObjid(oName);

				PlowCmn cmn = new PlowCmn(appl, 15D);		
				cmn.setGDraw(gdraw);
				currentCmn = cmn;
				cmnList.add(cmn);
				AXtt axtt = new AXtt((PlowCmn)currentCmn, gdh);
				cmn.setUserData(axtt);
				
				AXttItemObject o = new AXttItemObject(axtt, oret.objid, oName, false, null, Plow.DEST_INTOLAST);
				o.openAttributes(axtt);
				break;
			}
			case OPEN_FLOWSUBWINDOW: {
				// Open flow file for first child
				FlowNode node = (FlowNode)arg[0].item;
				PwrtObjid nodeObjid = node.getCmn().getObjid();
				CdhrString rName = gdh.objidToName(nodeObjid, Cdh.mName_pathStrict);
				if (rName.evenSts())
					break;
				String name = rName.str + "-" + node.getName();
				System.out.println( "Open flow: " + name);
				CdhrObjid oret = gdh.nameToObjid(name);
				if ( oret.evenSts()) break;
				oret = gdh.getChild(oret.objid);
				if ( oret.evenSts()) break;
				new ReaderTask().execute(new ReaderTaskArg(ReaderTask.FLOW_READ, oret.objid, null));
				break;
			}
			case OPEN_FLOW: {
				// If plc window open flow for window, else for first child
				PwrtObjid oid = (PwrtObjid)arg[0].item;
				CdhrClassId retCid = gdh.getObjectClass(oid);
				if ( retCid.evenSts())
					break;
				if ( !(retCid.classId == Pwrb.cClass_windowplc ||
						retCid.classId == Pwrb.cClass_windoworderact ||
						retCid.classId == Pwrb.cClass_windowcond ||
						retCid.classId == Pwrb.cClass_windowsubstep)) {
					CdhrObjid oret = gdh.getChild(oid);
					if (oret.evenSts()) break;

					oid = oret.objid;
				}
				new ReaderTask().execute(new ReaderTaskArg(ReaderTask.FLOW_READ, oid, null));
				break;
			}
			case OPEN_FLOWCROSS: {
				// If plc window open flow for window, else for first child
				AXttItemCrr crr = (AXttItemCrr)arg[0].item;
				String text = crr.getText();

				int idx = text.indexOf(' ');
				if ( idx < 0) break;
				text = text.substring(0, idx);
				idx = text.lastIndexOf('-');
				if ( idx < 0) break;

				String wname = text.substring(0, idx);
				String oname = text.substring(idx+1);
				System.out.println( "wname: " + wname + " oname: " + oname);
				
				CdhrObjid oret = gdh.nameToObjid(wname);
				if ( oret.evenSts()) break;

				new ReaderTask().execute(new ReaderTaskArg(ReaderTask.FLOW_READ, oret.objid, oname));
				break;
			}
			case FLOW_CROSSREFERENCE: {
				FlowNode node = (FlowNode)arg[0].item;
				String oName = node.getTraceObject();
				CdhrObjid oret = gdh.nameToObjid(oName);

				PlowCmn cmn = new PlowCmn(appl, 15D);		
				cmn.setGDraw(gdraw);
				currentCmn = cmn;
				cmnList.add(cmn);
				AXtt axtt = new AXtt((PlowCmn)currentCmn, gdh);
				cmn.setUserData(axtt);
				
				AXttItemObject o = new AXttItemObject(axtt, oret.objid, oName, false, null, Plow.DEST_INTOLAST);
				o.openCrossreferences(axtt);
				currentCmn.configure();
				break;
			}
			case CHANGE_VALUE: {
				switch ( inputType) {
				case Pwr.eType_Boolean: {
					boolean bval;
					if ( ((String)arg[0].item).equals("1"))
						bval = true;
					else if ( ((String)arg[0].item).equals("0"))
						bval = false;
					else
						break;
					PwrtStatus sts = gdh.setObjectInfo(inputFullName, bval);
					if ( sts.evenSts())
						System.out.println( "setObjectInfo " + sts.getSts() + " " + inputFullName);
					break;
				}
				case Pwr.eType_Float64:
				case Pwr.eType_Float32: {
					float fval;
					try {
						fval = Float.parseFloat((String)arg[0].item);
					}
					catch ( NumberFormatException e) {
						break;
					}
					PwrtStatus sts = gdh.setObjectInfo(inputFullName, fval);
					if ( sts.evenSts())
						System.out.println( "setObjectInfo " + sts.getSts() + " " + inputFullName);
					break;
				}
				case Pwr.eType_Int64:
				case Pwr.eType_Int32:
				case Pwr.eType_Int16:
				case Pwr.eType_Int8:
				case Pwr.eType_UInt64:
				case Pwr.eType_UInt32:
				case Pwr.eType_UInt16:
				case Pwr.eType_UInt8: {
					int ival;
					try {
						ival = Integer.parseInt((String)arg[0].item);
					}
					catch ( NumberFormatException e) {
						break;
					}
					PwrtStatus sts = gdh.setObjectInfo(inputFullName, ival);
					if ( sts.evenSts())
						System.out.println( "setObjectInfo " + sts.getSts() + " " + inputFullName);
					break;
				}				
				default: {
					PwrtStatus sts = gdh.setObjectInfo(inputFullName, (String)arg[0].item);
					if ( sts.evenSts())
						System.out.println( "setObjectInfo " + sts.getSts() + " " + inputFullName);
				}
				}	
				break;
			}
			case OPEN_DEFGRAPH: {
				PwrtObjid oid = (PwrtObjid)arg[0].item;
				CdhrString rName = gdh.objidToName(oid, Cdh.mName_pathStrict);
				if (rName.evenSts())
					break;
				String name = rName.str + ".DefGraph";
				rName = gdh.getObjectInfoString(name);
				if (rName.evenSts())
					break;
				String oname = rName.str;
				
				name = oname + ".Action";
				rName = gdh.getObjectInfoString(name);
				if (rName.evenSts())
					break;				
				String action = rName.str;
				
				name = oname + ".Object";
				rName = gdh.getObjectInfoString(name);
				if (rName.evenSts())
					break;
				String instance = rName.str;
				if ( instance.equals(""))
					instance = null;
				
				System.out.println("Open " + rName.str);
				new ReaderTask().execute(new ReaderTaskArg(ReaderTask.GROW_READ, action, instance));
				break;
			}
			case OPEN_CLASSGRAPH_NAME: {
				CdhrAttrRef aret = gdh.nameToAttrRef((String)arg[0].item);
				System.out.println("Classname " + (String)arg[0].item + " sts " + aret.getSts());
				if ( aret.evenSts())
					break;
				arg[0].item = new PwrtAttrRef(aret.aref.objid, aret.aref.body, aret.aref.offset, aret.aref.size, aret.aref.flags);

				// No break, continue with OPEN_CLASSGRAPH
			}
			case OPEN_CLASSGRAPH: {
				PwrtAttrRef aref = (PwrtAttrRef)arg[0].item;

				CdhrTypeId cret = gdh.getAttrRefTid(aref);
				System.out.println("Classgraph  type " + cret.getSts());
				if (cret.evenSts()) break; 

				CdhrObjid coidret = gdh.classIdToObjid(cret.typeId);
				if ( coidret.evenSts()) break;
				
				CdhrString cnameret = gdh.objidToName(coidret.objid, Cdh.mName_object);
				if (cnameret.evenSts()) break;
				
				String classname = cnameret.str.toLowerCase();
				if ( classname.startsWith("$")) 
					classname = classname.substring(1);
				
				// Get GraphConfiguration if any
				CdhrString nret = gdh.attrRefToName(aref, Cdh.mName_volumeStrict);
				if ( nret.evenSts()) break;
				
				String suffix = "";
				CdhrInt gcret = gdh.getObjectInfoInt(nret.str + ".GraphConfiguration");
				if (gcret.oddSts()) {
					if ( gcret.value > 0)
						suffix = Integer.toString(gcret.value);
				}
								
				String fname;
				if ( coidret.objid.vid < Cdh.cUserClassVolMin || 
						(coidret.objid.vid >= Cdh.cManufactClassVolMin && 
						coidret.objid.vid <= Cdh.cManufactClassVolMax)) {
					// Add pwr_c_ to filename
					fname = "$pwr_exe/pwr_c_" + classname + suffix + ".pwg";
				}
				else
					fname = classname + suffix + ".pwg";

				CdhrString rName = gdh.attrRefToName(aref, Cdh.mName_pathStrict);
				if (rName.evenSts())
					break;
									
				System.out.println("Open " + fname);
				new ReaderTask().execute(new ReaderTaskArg(ReaderTask.GROW_READ, fname, rName.str));
				break;
			}
			case EVENTHANDLER: {
				GdhEventArg event = (GdhEventArg)arg[0].item;
				currentCmn.eventHandler(event.action, event.x, event.y);
				break;
			}
			}
			return null;
		}

		@Override
		protected void onPostExecute(Void params) {
			super.onPostExecute(params);
			view.invalidate();
		}
	}
	private class ReaderTaskArg {
		int action;
		Object data1;
		Object data2;
		
		public ReaderTaskArg(int action, Object data1, Object data2) {
			this.action = action;
			this.data1 = data1;
			this.data2 = data2;
		}
	}
	
	private class ReaderTask extends AsyncTask<ReaderTaskArg, Void, Void> {
		public static final int FLOW_READ = 1;
		public static final int CRR_READ = 2;
		public static final int GROW_READ = 3;
				
		@Override
		protected Void doInBackground(ReaderTaskArg... arg) {

			switch ( arg[0].action) {
			case FLOW_READ:
				readFlow((PwrtObjid)arg[0].data1, (String)arg[0].data2);
				break;
			case GROW_READ:
				readGrow((String)arg[0].data1, (String)arg[0].data2);
				break;
			}
			return null;
		}

		@Override
		protected void onPostExecute(Void params) {
			super.onPostExecute(params);
			//cmn.draw();
			view.invalidate();
		}
	}
	public Resources getApplResources() {
		return getResources();
	}
	
	@Override
	public boolean onTouchEvent(MotionEvent me) {
		int viewOffset = 80;
		int action = me.getAction();
		float x = me.getX();
		float y = me.getY();
		
		switch (action) {
		case MotionEvent.ACTION_MOVE:
			System.out.println("Event Move " + action + " (" + x + "," + y + ")");
			if ( currentCmn.type() == PlowCmnIfc.TYPE_GRAPH){
				new GdhTask().execute(new GdhTaskArg(GdhTask.EVENTHANDLER, 
						new GdhEventArg(GraphCmn.ACTION_MOVE, x, y-viewOffset)));					
			}
			else {
				if ( (int)(lastTouchY - y) != 0) {
					currentCmn.scroll((int)(lastTouchX -x), (int)(lastTouchY - y));
					view.invalidate();
				}
				lastTouchX = x;
				lastTouchY = y;
			}				
			break;
		case MotionEvent.ACTION_UP:
				if ( currentCmn != null && currentCmn.type() == PlowCmnIfc.TYPE_GRAPH){
					System.out.println("Event Up   " + action + " (" + x + "," + y + ") cmn " + currentCmn.type());
					if ( Math.abs(x - downTouchX) < 10 && Math.abs(y - downTouchY) < 10 && 
							me.getEventTime() - me.getDownTime() < 700) {
						System.out.println("Event Click   " + action + " (" + x + "," + y + ") cmn " + currentCmn.type());
						new GdhTask().execute(new GdhTaskArg(GdhTask.EVENTHANDLER, 
								new GdhEventArg(GraphCmn.ACTION_CLICK, x, y-viewOffset)));					
					}
					else {
						new GdhTask().execute(new GdhTaskArg(GdhTask.EVENTHANDLER, 
								new GdhEventArg(GraphCmn.ACTION_UP, x, y-viewOffset)));
					}
				}
				else {
					if ( Math.abs(x - downTouchX) < 10 && Math.abs(y - downTouchY) < 10 && currentCmn != null) {
						currentCmn.eventHandler(action, x, y-viewOffset);
				}
			}	
			break;
		case MotionEvent.ACTION_DOWN:
			System.out.println("Event Down " + action + " (" + x + "," + y + ")");
			if ( currentCmn.type() == PlowCmnIfc.TYPE_GRAPH){
				new GdhTask().execute(new GdhTaskArg(GdhTask.EVENTHANDLER, 
						new GdhEventArg(GraphCmn.ACTION_DOWN, x, y-viewOffset)));					
			}
			lastTouchX = x;
			lastTouchY = y;
			downTouchX = x;
			downTouchY = y;
			break;
		}
		return true;
	}
	@Override
	public boolean onCreateOptionsMenu(Menu menu) {
		// Inflate the menu; this adds items to the action bar if it is present.
		getMenuInflater().inflate(R.menu.activity_main, menu);
		return true;
	}

	public void forceError() {
		if (true) {
			throw new Error("Whooops");			
		}
	}
	private class MainView extends SurfaceView {
		Bitmap bitmap = null;
		Canvas drawCanvas;
		
		public MainView(Context context) {
			super(context);
			setWillNotDraw(false);
		}

	    @Override
		protected void onDraw( Canvas canvas) {
	    	if ( currentCmn != null) {

	    		if ( currentCmn.type() == PlowCmnIfc.TYPE_GRAPH) {
	    			if ( bitmap == null) {
	    				bitmap = Bitmap.createBitmap(canvas.getWidth(), canvas.getHeight(), Bitmap.Config.ARGB_8888);
	    				drawCanvas = new Canvas(bitmap);
    	    			drawCanvas.drawColor(graph.cmn.gdraw.getColor(graph.cmn.background_color));
	    			}
	    		
	    			currentCmn.setCanvas(drawCanvas);
	    			drawCanvas.drawColor(graph.cmn.gdraw.getColor(graph.cmn.background_color));
	    			currentCmn.draw();
	    			canvas.drawBitmap(bitmap,  0F, 0F, null);
	    		}
	    		else {

	    			Canvas c = getHolder().lockCanvas();
	    			if ( c != null) {
	    				
	    				if ( currentCmn.type() == PlowCmnIfc.TYPE_GRAPH)
	    	    			c.drawColor(graph.cmn.gdraw.getColor(graph.cmn.background_color));
	    	    		else
	    	    			c.drawColor(Color.WHITE);

	    				currentCmn.setCanvas(c);
	    				currentCmn.draw();
	    				getHolder().unlockCanvasAndPost(c);
	    			}
	    		}
	    	}	
	    }
	}
	
	
	class MyTask extends TimerTask {
		@Override
		public void run() {
		runOnUiThread( new Runnable() {
		public void run() {
			if ( !initDone)
				return;
System.out.println("MainActivity TimerTask " + currentCmn.type());
			switch(currentCmn.type()) {
			case PlowCmnIfc.TYPE_PLOW:
				new GdhTask().execute(new GdhTaskArg(GdhTask.SCAN, 
						(AXttItemBase)null));
				view.invalidate();
				break;
			case PlowCmnIfc.TYPE_FLOW:
			case PlowCmnIfc.TYPE_GRAPH:
				new GdhTask().execute(new GdhTaskArg(GdhTask.DYNAMIC_UPDATE,(AXttItemBase)null));
				view.invalidate();
				break;
			case PlowCmnIfc.TYPE_EV:
				view.invalidate();
				break;
			}
		}});
		}
	}

	public void readFlow( PwrtObjid windowObjid, String  object) {
/*
		AssetManager am = getAssets();
		InputStream inputStream;
		try {
		  inputStream = am.open(filename);
		}
		catch ( IOException e ) {
			System.out.println( "Unable to open file " + filename);
			return;
		}
		BufferedReader reader = new BufferedReader(new InputStreamReader(inputStream));
*/
		String filename = Flow.getFileName(windowObjid);
		BufferedReader reader;
		URL url = null;
		try {
			// url = new URL("http://10.0.2.2/data0/pwrp/opg7/bld/common/load/" + filename);
			url = new URL("http://" + pwrHost + "/pwrp_load/" + filename);
			reader = new BufferedReader(new InputStreamReader(url.openStream()));
		} catch ( IOException e) {
			System.out.println( "Unable to open file " + filename  + " " + url);
			return;			
		}
		FlowCmn fcmn = new FlowCmn(this, gdh, windowObjid);
		currentCmn = fcmn;
		cmnList.add(fcmn);
		fcmn.setGDraw(gdraw);
		FlowCtx ctx = new FlowCtx(fcmn);
		ctx.open(reader);

		if ( object != null) {
			FlowNode node = (FlowNode)ctx.getObject(object);
			fcmn.select(node);
			node.setSelect(true);
			ctx.centerObject(node);			
		}
		
		
		new GdhTask().execute(new GdhTaskArg(GdhTask.DYNAMIC_OPEN, null));
	}

	public void readGrow( String file, String  instance) {
/*
		AssetManager am = getAssets();
		InputStream inputStream;
		try {
		  inputStream = am.open(filename);
		}
		catch ( IOException e ) {
			System.out.println( "Unable to open file " + filename);
			return;
		}
		BufferedReader reader = new BufferedReader(new InputStreamReader(inputStream));
*/
		String filename = file;
		BufferedReader reader;
		URL url = null;
		try {
			if ( filename.startsWith("$pwr_exe/")) {
				// url = new URL("http://10.0.2.2/data0/x4-8-6/rls/os_linux/hw_x86/exp/exe/" + filename.substring(9));
				url = new URL("http://" + pwrHost + "/pwr_exe/" + filename.substring(9));
			}
			else {
				// url = new URL("http://10.0.2.2/data0/pwrp/opg7/bld/x86_linux/exe/" + filename);
				url = new URL("http://" + pwrHost + "/pwrp_exe/" + filename);
			}
			reader = new BufferedReader(new InputStreamReader(url.openStream()));
		} catch ( IOException e) {
			System.out.println( "Unable to open file " + filename  + " " + url);
			return;			
		}
		graph = new Graph(this, gdh);
		graphObject.add(instance);
		graph.open(reader);
	    graph.gdraw.setActivity(this);
	    GraphCmn cmn = new GraphCmn(graph);
	    currentCmn = cmn;
		cmnList.add(cmn);

		// new GdhTask().execute(new GdhTaskArg(GdhTask.DYNAMIC_OPEN, null));
	}
	// GraphApplIfc interface
    public void openConfirmDialog( Object dyn, String text, Object object) {
      // TODO
    }
    public int getWidth() {
    	return 500;
    }
    public int getHeight() {
    	return 500;
    }
    public String getObject() {
    	if ( graphObject.size() > 0)
    		return graphObject.get(graphObject.size()-1);
    	else
    		return null;
    }
    
    static CliTable[] cliTable = new CliTable[] { 
        new CliTable( "OPEN", new String[] {"cli_arg1", "cli_arg2", "/NAME", 
    	"/FILE", "/SCROLLBAR", "/WIDTH", "/HEIGHT", "/MENU", "/NAVIGATOR", 
    	"/CENTER", "/OBJECT", "/INSTANCE", "/NEW", "/CLASSGRAPH", "/ACCESS", "/PARENT"}),
        new CliTable( "EXIT", null),
        new CliTable( "HELP", new String[] {"cli_arg1", "cli_arg2", "cli_arg3",
    	"cli_arg4", "/HELPFILE", "/POPNAVIGATOR", "/BOOKMARK", "/INDEX",
            "/BASE", "/RETURNCOMMAND", "/WIDTH", "/HEIGHT", "/VERSION"}),
        new CliTable( "SET", new String[] {"cli_arg1",
        	"/NAME", "/VALUE", "/BYPASS"}),
        new CliTable( "EXAMPLE", new String[] {"/NAME", "/HIERARCHY"}),
        new CliTable( "CHECK", new String[] {"cli_arg1", "/METHOD", "/OBJECT"}),
        new CliTable( "CALL", new String[] {"cli_arg1", "/METHOD", "/OBJECT"}),
        new CliTable( "SET", new String[] {"cli_arg1", "dcli_arg2"}) 
      };

    public int command(String cmd) {
    	System.out.println("Command: " + cmd);
        boolean local_cmd = false;

        Cli cli = new Cli( cliTable);
        String command = cli.parse( cmd);
        if (cli.oddSts()) {
        	System.out.println("JopSpider1 : " + command);
        	if ( command.equals("OPEN")) {
        		if ( cli.qualifierFound("cli_arg1")) {

        			String jgraph = "JGRAPH";
        			String graph = "GRAPH";
        			String url = "URL";
        			String trend = "TREND";
        			String fast = "FAST";
        			String cli_arg1 = cli.getQualValue("cli_arg1").toUpperCase();
        			if ( jgraph.length() >= cli_arg1.length() &&
        					jgraph.substring(0,cli_arg1.length()).equals(cli_arg1)) {
        				// Command is "OPEN JGRAPH"
        				System.out.println("Command is not implemented, \"OPEN JGRAPH\"");
        			}
        			else if ( graph.length() >= cli_arg1.length() &&
        					graph.substring(0,cli_arg1.length()).equals(cli_arg1)) {
        				// Command is "OPEN GRAPH"
        				String graphName = null;
        				String instanceValue = null;
        				boolean classGraph = false;

        				if ( cli.qualifierFound("/INSTANCE")) {
        					instanceValue = cli.getQualValue("/INSTANCE");
        					classGraph = cli.qualifierFound("/CLASSGRAPH");		      
        					boolean parent = cli.qualifierFound("/PARENT");
        					if ( parent) {
        						int idx = instanceValue.lastIndexOf( '.');
        						if ( idx != -1 && idx != 0)
        							instanceValue = instanceValue.substring( 0, idx);
        						System.out.println( "open graph /parent: " + instanceValue);
        					}
        				}
        				if ( classGraph) {
    						new GdhTask().execute(new GdhTaskArg(GdhTask.OPEN_CLASSGRAPH_NAME, instanceValue));
        				}
        				else {
        					if ( ! cli.qualifierFound("cli_arg2")) {
        						System.out.println("Syntax error");
        						return 0;
        					}
        					// Back to basic cmn
        					currentCmn = cmnList.get(0);
        					for ( int i = cmnList.size()-1; i > 0; i--)
        					cmnList.removeElementAt(i);
        					for ( int i = graphObject.size()-1; i >= 0; i--)
        						graphObject.removeElementAt(i);
        					
        					graphName = cli.getQualValue("cli_arg2").toLowerCase();
        					if ( !graphName.contains(".pwg"))
        						graphName = graphName + ".pwg";

        					new ReaderTask().execute(new ReaderTaskArg(ReaderTask.GROW_READ, graphName, instanceValue));
        				}
        			}
        			else if ( url.length() >= cli_arg1.length() &&
        					url.substring(0,cli_arg1.length()).equals(cli_arg1)) {
        				// Command is "OPEN URL"
        				Boolean newFrame = true;
        				String frameName = null;
        				String urlValue = cli.getQualValue("cli_arg2");
        				System.out.println("open url " + urlValue);
        			}
        			else if ( trend.length() >= cli_arg1.length() &&
        					trend.substring(0,cli_arg1.length()).equals(cli_arg1)) {
        				// Command is "OPEN TREND"
        			}
        			else if ( fast.length() >= cli_arg1.length() &&
        					fast.substring(0,cli_arg1.length()).equals(cli_arg1)) {
        				// Command is "OPEN FAST"
        			}
        			else if ( command.equals("HELP")) {
        			}
        		}
        	}
        	else if ( command.equals("SET")) {
        		if ( cli.qualifierFound("cli_arg1")) {
        		}
        	}
        	else if ( command.equals("CHECK")) {
        		if ( cli.qualifierFound("cli_arg1")) {

        			String methodstr = "METHOD";
        			String isattributestr = "ISATTRIBUTE";
        			String cli_arg1 = cli.getQualValue("cli_arg1").toUpperCase();
        			if ( methodstr.length() >= cli_arg1.length() &&
        					methodstr.substring(0,cli_arg1.length()).equals(cli_arg1)) {
        				// Command is "CHECK METHOD"
        				String method;
        				String object;

        				if ( cli.qualifierFound("/METHOD"))
        					method = cli.getQualValue("/METHOD");
        				else {
        					System.out.println( "Cmd: Method is missing\n");
        					return 0;
        				}

        				if ( cli.qualifierFound("/OBJECT"))
        					object = cli.getQualValue("/OBJECT");
        				else {
        					System.out.println( "Cmd: Object is missing\n");
        					return 0;
        				}
        			}	
        			else if ( isattributestr.length() >= cli_arg1.length() &&
        					isattributestr.substring(0,cli_arg1.length()).equals(cli_arg1)) {
        				// Command is "CHECK ISATTRIBUTE"
        				String method;
        				String object;

        				if ( cli.qualifierFound("/OBJECT"))
        					object = cli.getQualValue("/OBJECT");
        				else {
        					System.out.println( "Cmd: Object is missing\n");
        					return 0;
        				}
        			}
        		}
        		else if ( command.equals("CALL")) {
        			if ( cli.qualifierFound("cli_arg1")) {

        				String parameter = "METHOD";
        				String cli_arg1 = cli.getQualValue("cli_arg1").toUpperCase();
        				if ( parameter.length() >= cli_arg1.length() &&
        						parameter.substring(0,cli_arg1.length()).equals(cli_arg1)) {
        					// Command is "CHECK METHOD"
        					String method;
        					String object;

        					if ( cli.qualifierFound("/METHOD"))
        						method = cli.getQualValue("/METHOD");
        					else {
        						System.out.println( "Cmd: Method is missing\n");
        						return 0;
        					}

        					if ( cli.qualifierFound("/OBJECT"))
        						object = cli.getQualValue("/OBJECT");
        					else {
        						System.out.println( "Cmd: Object is missing\n");
        						return 0;
        					}
        				}

        			}
        		}
        		else if ( command.equals("SET")) {
        			return 1;
        		}
        	}
        	else {
        		System.out.println( "JopSpider: Parse error " + cli.getStsString());
        		return 0;
        	}
        }
        return 1;
    }

    public void invalidateView() {
    	view.invalidate();
    }
    
    // GdhApplIfc functions
    @Override
    public String getPwrHost() {
    	return pwrHost;		    	
    }
    
    public PlowCmnIfc getCurrentCmn() {
    	return currentCmn;
    }
}
