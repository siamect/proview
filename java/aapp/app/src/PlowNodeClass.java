package jpwr.app;

public class PlowNodeClass implements PlowArrayElem {
	  PlowArray a;
	  String nc_name;
	  int group;
	  PlowCmn cmn;

	  public PlowNodeClass( PlowCmn cmn) {
		  this.cmn = cmn;
		  a = new PlowArray( cmn);
	  }

	  public void insert( PlowArrayElem e) {
		  a.insert(e);
	  }
	  public void draw( PlowPoint p, PlowNodeIfc node) {
		  a.draw( p, node);
	  }

	  public void setBorders(PlowBorders borders) {
		  a.setBorders(borders);
	  }

	  public void setInvert(boolean invert) {			
	  }
	  public boolean eventHandler(PlowEvent e) {
		  return false;
	  }
	  public void draw(double x_left, double y_low, double x_right, double y_high) {		
	  }

}
