package jpwr.app;

public interface FlowDrawIfc {
	public void rect(boolean border, int color, float x, float y, float width, float height);
	public void pixmap(PlowPixmapData pixmapData, int idx, boolean invert, float x, float y);
	public void drawText( String text, int textColor, int textSize, int font, float x, float y);
	public float measureText( String text, int textSize, int font);
}
