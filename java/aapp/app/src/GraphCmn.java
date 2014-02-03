package jpwr.app;
import android.graphics.Canvas;
import android.view.MotionEvent;
import jpwr.jopg.*;

public class GraphCmn implements PlowCmnIfc {

		public static final int ACTION_CLICK = 1;
		public static final int ACTION_UP = 2;
		public static final int ACTION_DOWN = 3;
		public static final int ACTION_MOVE = 4;

		public Graph graph;
		int scanCount = 0;
		double buttonDownX;
		double buttonDownY;
		
		public GraphCmn(Graph graph) {
			this.graph = graph;
		}
		
		public int type() {
			return PlowCmnIfc.TYPE_GRAPH;
		}
		public Object getUserData() {
			return null;
		}
		public void setUserData(Object userData) {
		}
		public void draw() {
			graph.ctx.draw();
		}
		public void zoom(double factor) {
			graph.ctx.zoom(factor);
		}
                public void zoom(double factor, double x, double y) {
		    graph.ctx.zoom(factor, x, y);
		}
		public void scroll( int x, int y) {
		    graph.ctx.scroll( x, y);
		}
		public void pageUp() {}
		public void pageDown() {}
		public synchronized void eventHandler(int action, double fx, double fy) {
			switch ( action) {
			case ACTION_UP: {
			    GlowEvent event = new GlowEvent();
			    event.x = (fx + graph.ctx.cmn.mw.offset_x) / graph.ctx.cmn.mw.zoom_factor_x;
			    event.y = (fy + graph.ctx.cmn.mw.offset_y) / graph.ctx.cmn.mw.zoom_factor_y;
			    event.event = Glow.eEvent_MB1Up;
				graph.cmn.setNodraw();
			    graph.ctx.eventHandler( event);
				graph.cmn.resetNodraw();

				break;
			}
			case ACTION_CLICK: {
			    GlowEvent event = new GlowEvent();
			    event.x = (fx + graph.ctx.cmn.mw.offset_x) / graph.ctx.cmn.mw.zoom_factor_x;
			    event.y = (fy + graph.ctx.cmn.mw.offset_y) / graph.ctx.cmn.mw.zoom_factor_y;
			    event.event = Glow.eEvent_MB1Up;
				graph.cmn.setNodraw();
			    graph.ctx.eventHandler( event);
				graph.cmn.resetNodraw();

				System.out.println("GraphCmn event click");
				GlowEvent event2 = new GlowEvent();
				event2.x = (fx + graph.ctx.cmn.mw.offset_x) / graph.ctx.cmn.mw.zoom_factor_x;
				event2.y = (fy + graph.ctx.cmn.mw.offset_y) / graph.ctx.cmn.mw.zoom_factor_y;
				event2.event = Glow.eEvent_MB1Click;
				graph.cmn.setNodraw();
				graph.ctx.eventHandler( event2);
				graph.cmn.resetNodraw();
				break;
			}
			case ACTION_DOWN: {
			    GlowEvent event = new GlowEvent();
			    event.x = (fx + graph.ctx.cmn.mw.offset_x) / graph.ctx.cmn.mw.zoom_factor_x;
			    event.y = (fy + graph.ctx.cmn.mw.offset_y) / graph.ctx.cmn.mw.zoom_factor_y;
			    event.event = Glow.eEvent_MB1Down;
				graph.cmn.setNodraw();
			    graph.ctx.eventHandler( event);
				graph.cmn.resetNodraw();
				buttonDownX = event.x;
				buttonDownY = event.y;
				break;
			}
			case ACTION_MOVE: {
			    GlowEvent event = new GlowEvent();
			    event.x = (fx + graph.ctx.cmn.mw.offset_x) / graph.ctx.cmn.mw.zoom_factor_x;
			    event.y = (fy + graph.ctx.cmn.mw.offset_y) / graph.ctx.cmn.mw.zoom_factor_y;
			    event.event = Glow.eEvent_ButtonMotion;
				graph.cmn.setNodraw();
			    graph.ctx.eventHandler( event);
				graph.cmn.resetNodraw();
				break;
			}
			}
		}
		public void select(Object node) {}
		public void selectClear() {}
		public Object getSelect() {return null;}
		public void configure() {}
		public void dynamicOpen() {}
		public void dynamicClose() {
			graph.ctx.traceDisconnect();
		}
		public synchronized void dynamicUpdate() {
/*
			scanCount++;
			if ( scanCount == 1)
			    graph.gdh.getObjectRefInfoAll();
*/
			graph.cmn.setNodraw();
			graph.ctx.traceScan();
			graph.cmn.resetNodraw();
		}
		public void setCanvas(Canvas canvas) {
			graph.cmn.gdraw.setCanvas(canvas);
		}
		public Object getFirst() {
			return null;
		}
		public Object getLast() {
			return null;
		}
		public Object getNext(Object elem) {
			return null;
		}
		public Object getPrevious(Object elem) {
			return null;
		}
		public int remove(Object e) {
			return 0;
		}
		public void setNodraw() {
		    graph.ctx.cmn.setNodraw();
		}
		public void resetNodraw() {
		    graph.ctx.cmn.resetNodraw();
		}
    		public int getAppMotion() {
		    return graph.cmn.getAppMotion();
    		}
    		public boolean getSliderActive() {
		    return graph.ctx.getSliderActive();
    		}
    
}
