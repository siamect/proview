package jpwr.app;

public class PlowNode implements PlowComponent, PlowArrayElem, PlowNodeIfc {
	  static final int OFFSET = 2;
	  static final double ICON_SIZE = 3D;
	  PlowBorders borders = new PlowBorders();
	  PlowCmn cmn;
	  PlowNodeClass nc;
	  PlowPoint pos;
	  String n_name;
	  String annotv[] = new String[10];
	  PlowPixmapData pixmapv[] = new PlowPixmapData[3];
	  int annotsize[] = new int[10];
	  String trace_object;
	  String trace_attribute;
	  int trace_attr_type;
	  boolean highlight;
	  boolean select;
	  boolean invert;
	  PlowDimension bounds = new PlowDimension();
	  double annotPosition;
	  Object userData;
	  int level;
	  int nodeOpen;
	  
	  public PlowNode( PlowCmn cmn, double x, double y, PlowNodeClass nc) {
		  this.cmn = cmn;
		  this.nc = nc;
		  pos = new PlowPoint(x, y);
		  setBorders();
	  }	
	  public boolean getSelect() {
		  return select;
	  }
	  public void setSelect( boolean select) {
		  boolean redraw = (this.select != select);
		  this.select = select;
		  if ( redraw)
			  draw();
	  }	

	  public void setAnnotation(int number, String text, int size) {
		  if ( number >= 10)
			  return;
		  annotv[number] = text;
		  annotsize[number] = size;
	  }
	  public void setAnnotPixmap(int number, PlowPixmapData data) {
		  if ( number >= 3)
			  return;
		  pixmapv[number] = data;
	  }
	  public boolean eventHandler(PlowEvent e) {
		  System.out.println( "x(" + borders.x_left + "," + borders.x_right + ") (" + borders.y_low + "," + borders.y_high + ")");
		  switch ( e.type) {
		  case PlowEvent.TYPE_CLICK:
		  	if ( borders.x_left <= e.x && e.x <= borders.x_right &&
		  		 borders.y_low <= e.y && e.y <= borders.y_high) {
		  		cmn.currentNode = this;
		  		e.object = this;
		  		if ( e.x < borders.x_left + ICON_SIZE)
		  			e.inIcon = true;
		  		else
		  			e.inIcon = false;
		  		System.out.println("Hit !!");
		  		return true;
		  	}	
		  	break;
		  }
		  return false;
	  }
	  public String getName() {
		  return n_name;
	  }
	  public String getTraceObject() {
		  return trace_object;
	  }
	  public void setHighlight( boolean highlight) {
		  boolean redraw = (this.highlight != highlight);
		  this.highlight = highlight;
		  if ( redraw)
			  draw();
	  }	
	  public void setBorders() {
		  borders.x_left = 1e37;
		  borders.x_right = -1e37;
		  borders.y_low = 1e37;
		  borders.y_high = -1e37;
		  nc.setBorders( borders);
		  borders.x_left += pos.x;
		  borders.x_right += pos.x;
		  borders.y_low += pos.y;
		  borders.y_high += pos.y;
	  }
	  public void setBorders(PlowBorders borders) {
		  if ( this.borders.x_left < borders.x_left)
			  	borders.x_left = this.borders.x_left;
		  if ( this.borders.x_right > borders.x_right)
			  borders.x_right = this.borders.x_right;
		  if ( this.borders.y_low < borders.y_low)
			  borders.y_low = this.borders.y_low;
		  if ( this.borders.y_high > borders.y_high)
			  borders.y_high = this.borders.y_high;		  
	  }
	  public void setBounds() {
		  bounds.x = (int)((borders.x_left - cmn.x_left) * cmn.zoom_factor) - Plow.DRAWOFFSET;
	      bounds.y = (int)((borders.y_low - cmn.y_low) * cmn.zoom_factor) - Plow.DRAWOFFSET;
	      bounds.width = (int)((borders.x_right - borders.x_left) * cmn.zoom_factor) + 2 * Plow.DRAWOFFSET;
	      bounds.height = (int)((borders.y_high - borders.y_low) * cmn.zoom_factor) + 2 * Plow.DRAWOFFSET;
	  }

	  public void draw() {
		  cmn.draw(borders.x_left, borders.y_low, borders.x_right, borders.y_high);
	  }

	  public void draw(double x_left, double y_low, double x_right, double y_high) {
		  if ( borders.x_left < x_right && borders.x_right > x_left &&
			   borders.y_low < y_high && borders.y_high > y_low) {
			  draw( pos, this);
		  }
	  }
	  public void draw(PlowPoint pp, PlowNodeIfc node) {
	    nc.draw( pos, this);
	  }   
	  public void setInvert(boolean invert) {			
		  if ( invert == this.invert)
			  return;
		  this.invert = invert;
		  draw();
	  }

	  public void configure(PlowNode previous) {
		  if ( previous == null) {
			  pos.x = 0;
			  pos.y = 0;
		  }
		  else {
			  pos.x = Plow.TREE_INDENTATION * level;
			  pos.y = previous.borders.y_high;
		  }
		  setBorders();
	  }
	  public String getAnnot(int number) {
		  return annotv[number];
	  }
	  public PlowPixmapData getPixmap(int number) {
		  return pixmapv[number];
	  }
	  public boolean getHighlight() {
		  return highlight;
	  }
	  public boolean getInvert() {
		  return invert;
	  }
	  public void setAnnotPosition(double pos) {
		  annotPosition = pos;
	  }
	  public double getAnnotPosition() {
		  return annotPosition;
	  }
	  public void setUserData( Object userData) {
		  this.userData = userData;
	  }
	  public Object getUserData() {
		  return userData;
	  }
	  public void setLevel(int level) {
		  this.level = level;
	  }
	  public int getLevel() {
		  return level;
	  }
	  public int getNodeOpen() {
		  return nodeOpen;
	  }
	  public void setNodeOpen(int mask) {
		  this.nodeOpen |= mask;
	  }
	  public void resetNodeOpen(int mask) {
		  this.nodeOpen &= ~mask;
	  }
}
