package jpwr.app;

import java.util.Vector;

import jpwr.pwrxtt.MainActivity;

import android.graphics.Canvas;
import android.graphics.Paint;

public class OpwinCmn implements PlowCmnIfc {
	  Object userData;
	  Canvas canvas;
	  Vector<AGraphInfo> graphList;
	  MainActivity appl;
	  boolean configured = false;
	  int canvasHeight;
	  int canvasWidth;
	  int offsetY = 0;
	  int borderY = 0;
	  
	  public OpwinCmn( MainActivity appl, Vector<AGraphInfo> graphList) {
		  this.graphList = graphList;
		  this.appl = appl;
	  }
	  public int type() {
			return TYPE_OPWIN;
	  }
	  public void configureIcons() {
		  canvasWidth = canvas.getWidth();
		  canvasHeight = canvas.getHeight();

		  // Get largest bitmap size
		  int iconWidthMax = 0;
		  int iconHeightMax = 0;
		  for ( int i = 0; i < graphList.size(); i++) {
			  AGraphInfo graph = graphList.get(i);
			  graph.iconWidth = graph.bpm.getWidth();
			  graph.iconHeight = graph.bpm.getHeight();
			  if (graph.iconWidth > iconWidthMax)
				  iconWidthMax = graph.iconWidth;
			  if (graph.iconHeight > iconHeightMax)
				  iconHeightMax = graph.iconHeight;
		  }
		  int offsetX = 100;
		  int offsetY = 100;
		  int distX = iconWidthMax + 50;
		  int distY = iconHeightMax + 70;
		  int x = offsetX;
		  int y = offsetY;
		  for ( int i = 0; i < graphList.size(); i++) {
			  AGraphInfo graph = graphList.get(i);
			  graph.iconX = x;
			  graph.iconY = y;
			  graph.textX = graph.iconX + graph.iconWidth/2;
			  graph.textY = graph.iconY + graph.iconHeight + 15;

			  borderY = graph.textY + 100;

			  x += distX;
			  if ( x + distX > canvasWidth) {
				  x = offsetX;
				  y += distY;
			  }
		  }		  
		  configured = true;
	  }
	  public Object getUserData() {
		return userData;	
	  }
	  public void setUserData(Object userData) {
		  this.userData = userData;
	  }
	  public void draw() {
		  Paint p = new Paint();

		  if ( !configured)
			  configureIcons();
		  
		  for ( int i = 0; i < graphList.size(); i++) {
			  AGraphInfo graph = graphList.get(i);
			  canvas.drawBitmap(graph.bpm, graph.iconX, graph.iconY - offsetY, p);
			  int textWidth = (int) p.measureText(graph.text);
			  canvas.drawText(graph.text, graph.textX - textWidth/2, graph.textY - offsetY, p);
		  }
	  }
	  public void setNodraw() {}
	  public void resetNodraw() {}
	  public void zoom(double factor) {}
	  public void scroll( int x, int y) {
		  if ( borderY < canvasHeight)
			  return;
		  offsetY += y;
		  if ( offsetY < 0)
			  offsetY = 0;
		  if ( offsetY > borderY - canvasHeight)
			  offsetY = borderY - canvasHeight;
	  }
	  public void pageUp() {}
	  public void pageDown() {}
	  public void eventHandler(int action, double fx, double fy) {
		  switch (action) {
		  case PlowEvent.TYPE_CLICK:
			  System.out.println("Opwin click " + fx + " " + fy);
			  for ( int i = 0; i < graphList.size(); i++) {
				  if ( fx > graphList.get(i).iconX && fx <= graphList.get(i).iconX + graphList.get(i).iconWidth &&
					   fy > graphList.get(i).iconY - offsetY && fy <= graphList.get(i).iconY + graphList.get(i).iconHeight - offsetY) {
					System.out.println("Hit in icon");	  
					String cmd = "open graph " + graphList.get(i).graph;
					appl.command(cmd);
				  }
			  }
			  break;
		  }
	  }
	  public void select(Object node) {}
	  public void selectClear() {}
	  public Object getSelect() { return null;}
	  public void configure() {}
	  public void dynamicOpen() {}
	  public void dynamicClose() {}
	  public void dynamicUpdate() {}
	  public void setCanvas(Canvas canvas) {
		  this.canvas = canvas;
	  }
	  public Object getFirst() { return null;}
	  public Object getLast() { return null;}
	  public Object getNext(Object elem) {return null;}
	  public Object getPrevious(Object elem) {return null;}
	  public int remove(Object e) {return 0;}
}
