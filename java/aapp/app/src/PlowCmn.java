package jpwr.app;

import android.content.res.Resources;
import android.graphics.Canvas;
import android.graphics.Rect;
import android.view.MotionEvent;

public class PlowCmn implements PlowCmnIfc {
	// public Canvas canvas;
	public PlowDraw gdraw;
	PlowAppl appl;
	double zoom_factor;
	double base_zoom_factor;
	int offset_x;
	int offset_y;
	double x_right;
	double x_left;
	double y_high;
	double y_low;
	int canvasWidth;
	int canvasHeight;
	PlowArray a;
	PlowArray a_nc;
	PlowNode currentNode;
	PlowNode selectedNode;
	PlowBorders borders;
	Object userData;
	
	public PlowCmn(PlowAppl appl, double zoom_factor) {
		this.appl = appl;
		this.zoom_factor = zoom_factor;
		base_zoom_factor = zoom_factor;
		offset_x = 0;
		offset_y = 0;
		currentNode = null;
		borders = new PlowBorders();
		
		a = new PlowArray(this);
		a_nc = new PlowArray(this);
	}

	@Override
	public int type() {
		return PlowCmnIfc.TYPE_PLOW;
	}	
	public void setGDraw(PlowDraw gdraw) {
		this.gdraw = gdraw;
		canvasWidth = gdraw.getCanvasWidth();
		canvasHeight = gdraw.getCanvasHeight();
	}
	public Resources getResources() {
		return appl.getApplResources();
	}
	public void insert(PlowArrayElem e) {
		a.insert(e);
	}
	public void insertNode(PlowArrayElem e, PlowArrayElem destination, int destCode) {
		a.insertNode(e, destination, destCode);
	}
	public void closeNode(PlowArrayElem e) {
		a.closeNode(e);
	}
	
	public void insert_nc(PlowArrayElem e) {
		a_nc.insert(e);
	}

	public void draw(double x_left, double y_low, double x_right, double y_high) {
		gdraw.canvas.save(Canvas.CLIP_SAVE_FLAG);
		gdraw.canvas.clipRect((float)x_left, (float)y_high, (float)x_right, (float)y_low);
		a.draw(x_left, y_low, x_right, y_high);
		gdraw.canvas.restore();
	}

	@Override
	public void eventHandler(int action, double fx, double fy) {
		double x = (fx + offset_x) / zoom_factor;
		double y = (fy + offset_y) / zoom_factor;
		int type;
		boolean long_click = false;

		switch( action) {
		case PlowCmnIfc.ACTION_CLICK:
			type = PlowEvent.TYPE_CLICK;
			break;
		case PlowCmnIfc.ACTION_LONG_CLICK:
			type = PlowEvent.TYPE_CLICK;
			long_click = true;
			break;
		default:
			return;
		}
		
		System.out.println("Event: " + type + "(" + x + "," + y + ")");
		
		PlowEvent e = new PlowEvent(type, x, y, null);
		currentNode = null;
		a.eventHandler(e);

		if ( long_click)
		    e.type = PlowEvent.TYPE_LONG_CLICK;
		appl.eventHandler(e);

		switch (e.type) {
		case PlowEvent.TYPE_OBJECT_DELETED:
			if ( currentNode == selectedNode)
				selectedNode = null;
			break;
		}
		
	}
	public void draw() {
		a.draw();
	}

	public void zoom(double factor) {
		zoom_factor *= factor;
		draw();
	}
        public void zoom(double factor, double x, double y) {
		zoom_factor *= factor;
		scroll( 0, (int)((y + offset_y) * factor - (y + offset_y)));
		draw();
	}
	public void scroll( int x, int y) {
		offset_y += y;
		// offset_x += x;
		if ( offset_y < borders.y_low * zoom_factor)
			offset_y = (int)(borders.y_low * zoom_factor);
		if ( offset_y > borders.y_high * zoom_factor - canvasHeight + 80) {
			offset_y = (int)(borders.y_high * zoom_factor - canvasHeight + 80);
			if ( offset_y < borders.y_low * zoom_factor)
			    offset_y = (int)(borders.y_low * zoom_factor);
		}
	}
	public void pageUp() {
		offset_y -= canvasHeight * 0.8;
		if ( offset_y < borders.y_low * zoom_factor)
			offset_y = (int)(borders.y_low * zoom_factor);
	}
	public void pageDown() {
		offset_y += canvasHeight * 0.8;
		if ( offset_y > borders.y_high * zoom_factor - canvasHeight + 80)
			offset_y = (int)(borders.y_high * zoom_factor - canvasHeight + 80);
	}
	
	public void configure() {
		a.configure();
		a.setBorders(borders);
	}
	public int size() {
		return a.a.size();
	}
	public PlowArrayElem get(int i) {
		return a.a.get(i);
	}
    @Override
	public void select(Object node) {
		selectedNode = (PlowNode)node;
	}
    @Override
	public void selectClear() {
		selectedNode = null;
	}
    @Override
	public PlowNode getSelect() {
		return selectedNode;
	}
	public void resetInvert() {
		a.setInvert(false);
	}
	public void dynamicOpen() {}
	public void dynamicClose() {}
	public void dynamicUpdate() {}

	public Object getUserData() {
		return userData;
	}
	public void setUserData(Object userData) {
		this.userData = userData;
	}
	public void setCanvas(Canvas canvas) {
		gdraw.setCanvas(canvas);
		if ( canvasWidth == 0) {
		    canvasWidth = gdraw.getCanvasWidth();
		    canvasHeight = gdraw.getCanvasHeight();
		}
	}
	public Object getFirst() {
		return a.get_first();
	}
	public Object getLast() {
		return a.get_last();
	}
	public Object getNext(Object elem) {
		return a.get_next((PlowArrayElem)elem);
	}
	public Object getPrevious(Object elem) {
		return a.get_previous((PlowArrayElem)elem);
	}
	public int remove(Object e) {
		return a.remove((PlowArrayElem)e);
	}
	public void clear() {
		a.a.clear();
		selectClear();
	}
        public void setNodraw() {
	}
        public void resetNodraw() {
	}
        public int getNodraw() {
	    return 0;
	}
}
