package jpwr.app;

import android.graphics.Color;
import android.graphics.Paint;

public class PlowRect implements PlowArrayElem {

	PlowCmn cmn;
	double x;
	double y;
	double width;
	double height;
	int borderColor;
	int fillColor;
	boolean fill;
	boolean border;
	boolean fixColor;
	
	public PlowRect(PlowCmn cmn, double x, double y, double width, double  height,
				int fillColor, int borderColor, boolean fill, boolean border, boolean fixColor) {
		this.cmn = cmn;
		this.width = width;
		this.height = height;
		this.x = x;
		this.y = y;
		this.borderColor = borderColor;
		this.fillColor = fillColor;
		this.fill = fill;
		this.border = border;
		this.fixColor = fixColor;
	}

	@Override
	public void draw(PlowPoint p, PlowNodeIfc node) {
		if ( !fill && !border)
			return;
    	Paint paint = new Paint();
    	if ( node != null && node.getInvert() && !fixColor) {
        	cmn.gdraw.rect(false, Plow.COLOR_BLACK, (float)((x + p.x) * cmn.zoom_factor - cmn.offset_x), 
    				(float)((y + p.y) * cmn.zoom_factor - cmn.offset_y), 
    				(float)((x + p.x + width) * cmn.zoom_factor - cmn.offset_x), 
    				(float)((y + p.y + height) * cmn.zoom_factor) - cmn.offset_y);
    	}
    	else {
    		if ( fill) {
    			cmn.gdraw.rect(false, fillColor, (float)((x + p.x) * cmn.zoom_factor - cmn.offset_x), 
    					(float)((y + p.y) * cmn.zoom_factor - cmn.offset_y), 
    					(float)((x + p.x + width) * cmn.zoom_factor - cmn.offset_x), 
    					(float)((y + p.y + height) * cmn.zoom_factor - cmn.offset_y));
    		}
    		if ( border) {
    			cmn.gdraw.rect(true, borderColor, (float)((x + p.x) * cmn.zoom_factor - cmn.offset_x), 
    					(float)((y + p.y) * cmn.zoom_factor - cmn.offset_y), 
    					(float)((x + p.x + width) * cmn.zoom_factor - cmn.offset_x), 
    					(float)((y + p.y + height) * cmn.zoom_factor - cmn.offset_y));
    		}
    	}
	}	
	
	@Override
	public void setBorders(PlowBorders borders) {
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
