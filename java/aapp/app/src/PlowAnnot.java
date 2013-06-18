package jpwr.app;

import android.graphics.Color;
import android.graphics.Paint;
import android.graphics.Typeface;

public class PlowAnnot implements PlowArrayElem {
	public static final int RELATIVE_POSITION = 1;
	public static final int NEXT_RELATIVE_POSITION = 2;
	public static final double RELATIVE_OFFSET = 1D;
	
	double x;
	double y;
	int textSize;
	int textColor;
	int annotType;
	int font;
	int number;
	PlowCmn cmn;
	
	public PlowAnnot(PlowCmn cmn, double x, double y, int textSize, int textColor, int font,
				int annotType, int number) {
		this.cmn = cmn;
		this.x = x;
		this.y = y;
		this.textSize = textSize;
		this.textColor = textColor;
		this.font = font;
		this.annotType = annotType;
		this.number = number;
	}

	public void draw(PlowPoint p, PlowNodeIfc node) {
		if ( node == null || node.getAnnot(number) == null)
			return;
		
		
		int size = (int) (cmn.zoom_factor/cmn.base_zoom_factor * textSize);
		double xPos;
		int color;
		
		if ( node.getInvert())
    		color = Plow.COLOR_WHITE;
    	else
    		color = textColor;

		if ( (annotType & RELATIVE_POSITION) != 0) {
    		xPos = p.x + node.getAnnotPosition() + RELATIVE_OFFSET;
    		if ( xPos < p.x + x)
    			xPos = p.x + x;
    	}
    	else
    		xPos = p.x + x;

		cmn.gdraw.drawText(node.getAnnot(number), color, size, font, (float)((xPos) * cmn.zoom_factor - cmn.offset_x), 
    			(float)((y + p.y) * cmn.zoom_factor - cmn.offset_y));

    	if ( (annotType & NEXT_RELATIVE_POSITION) != 0)
    		node.setAnnotPosition(xPos - p.x + cmn.gdraw.measureText(node.getAnnot(number), size, font)/cmn.zoom_factor);

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
