package jpwr.app;

public interface PlowArrayElem {

	public void draw(PlowPoint p, PlowNodeIfc node);
	public void draw(double x_left, double y_low, double x_right, double y_high);
	public void setBorders(PlowBorders borders);
	public boolean eventHandler(PlowEvent e);
	public void setInvert(boolean invert);
}
