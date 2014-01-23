package jpwr.app;

import android.graphics.Canvas;

public interface PlowCmnIfc {
	public static final int TYPE_PLOW = 1;
	public static final int TYPE_FLOW = 2;
	public static final int TYPE_GRAPH = 3;
	public static final int TYPE_EV = 4;
	public static final int TYPE_OPWIN = 5;
	
	public int type();
	public Object getUserData();
	public void setUserData(Object userData);
	public void draw();
	public void setNodraw();
	public void resetNodraw();
	public void zoom(double factor);
        public void zoom(double factor, double x, double y);
	public void scroll( int x, int y);
	public void pageUp();
	public void pageDown();
	public void eventHandler(int action, double fx, double fy);
	public void select(Object node);
	public void selectClear();
	public Object getSelect();
	public void configure();
	public void dynamicOpen();
	public void dynamicClose();
	public void dynamicUpdate();
	public void setCanvas(Canvas canvas);
	public Object getFirst();
	public Object getLast();
	public Object getNext(Object elem);
	public Object getPrevious(Object elem);
	public int remove(Object e);

}
