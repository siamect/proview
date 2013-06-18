package jpwr.app;
import android.graphics.Color;
import android.graphics.Paint;
import android.graphics.Typeface;

public class PlowText implements PlowArrayElem {

		PlowCmn cmn;
		double x;
		double y;
		int textColor;
		String text;
		int size;
		int font;
		
		public PlowText(PlowCmn cmn, double x, double y, int textColor, String text,
				int size, int font) {
			this.cmn = cmn;
			this.x = x;
			this.y = y;
			this.textColor = textColor;
			this.text = text;
			this.size = size;
			this.font = font;
		}

		public void draw(PlowPoint p, PlowNodeIfc node) {
			int textSize = (int) (cmn.zoom_factor/cmn.base_zoom_factor * size);
			int color;
			int font = 0;
			
			if ( node != null && node.getInvert())
	    		color = Plow.COLOR_WHITE;
	    	else
	    		color = textColor;

			cmn.gdraw.drawText(text, color, textSize, font, (float)((x + p.x) * cmn.zoom_factor - cmn.offset_x), 
	    			(float)((y + p.y) * cmn.zoom_factor - cmn.offset_y));
		}
		@Override
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
