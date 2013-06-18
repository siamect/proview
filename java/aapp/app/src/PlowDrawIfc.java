package jpwr.app;

import android.graphics.Canvas;

public interface PlowDrawIfc {
	public void rect(boolean border, int color, float x1, float y1, float x2, float y2);
	public void arc(boolean border, int color, float x1, float y1, float x2, float y2, float angel1, float angel2);
	public void line(boolean border, int color, float x1, float y1, float x2, float y2);
	public void pixmap(PlowPixmapData pixmapData, int idx, boolean invert, float x, float y);
	public void drawText( String text, int textColor, int textSize, int font, float x, float y);
	public float measureText( String text, int textSize, int font);
	public void arrow(int color, float x1, float y1, float x2, float y2, float x3, float y3);
	public int getCanvasWidth();
	public int getCanvasHeight();
	public void setCanvas(Canvas canvas);

}
