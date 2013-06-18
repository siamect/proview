package jpwr.app;

public interface PlowNodeIfc {
	public String getAnnot(int number);
	public PlowPixmapData getPixmap(int number);
	public boolean getHighlight();
	public boolean getInvert();
	public double getAnnotPosition();
	public void setAnnotPosition(double pos);	
}
