package jpwr.app;

import android.graphics.Color;
import android.graphics.ColorMatrix;
import android.graphics.ColorMatrixColorFilter;
import android.graphics.Paint;

public class PlowAnnotPixmap implements PlowArrayElem {
	double x;
	double y;
	int number;
	PlowCmn cmn;
	static float[] inv = {	-1f, 0f, 0f, 1f, 1f,
			0f,-1f, 0f, 1f, 1f,
			0f, 0f,-1f, 1f, 1f,
			0f, 0f, 0f, 1f, 0f};
	
	public PlowAnnotPixmap(PlowCmn cmn, double x, double y, int number) {
		this.cmn = cmn;
		this.x = x;
		this.y = y;
		this.number = number;
	}

	public void draw(PlowPoint p, PlowNodeIfc node) {
		if ( node == null || node.getPixmap(number) == null)
			return;
		
		boolean invert = ( node != null && node.getInvert());

    	int idx = (int)(cmn.zoom_factor/cmn.base_zoom_factor * 6 - 3);
    	if ( idx < 0)
    		idx = 0;
    	if ( idx > 7)
    		idx = 7;
    	cmn.gdraw.pixmap(node.getPixmap(number), idx, invert, (float)((x + p.x) * cmn.zoom_factor - cmn.offset_x), 
					(float)((y + p.y) * cmn.zoom_factor - cmn.offset_y));		    	
	}

	public void setBorders(PlowBorders borders) {
		double width = 0;
		double height = 0;
		if ( x < borders.x_left)
			borders.x_left = x;
		if ( x + width > borders.x_right)
			borders.x_right = x + width;
		if ( y < borders.y_low)
			borders.y_low = y;
		if ( y + height > borders.y_high)
			borders.y_high = y + height;
	}
	public boolean eventHandler(PlowEvent e) {
		return false;
	}
	public void setInvert(boolean invert) {			
	}
	public void draw(double x_left, double y_low, double x_right, double y_high) {		
	}

}
