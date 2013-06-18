package jpwr.app;

import java.util.Vector;

public class PlowArray {
	  //Vector<Object> a = new Vector<Object>();
	  Vector<PlowArrayElem> a = new Vector<PlowArrayElem>();
	  PlowCmn cmn;

	  public PlowArray( PlowCmn cmn) {
	    this.cmn = cmn;
	  }

	  public void draw( PlowPoint p, PlowNodeIfc node) {
	    for ( int i = 0; i < a.size(); i++) {
	      ((PlowArrayElem)a.get(i)).draw(p, node);
	    }
	  }

	  public void draw() {
		  PlowPoint p = new PlowPoint();
		  
		  for ( int i = 0; i < a.size(); i++) {
			  ((PlowArrayElem)a.get(i)).draw( p, null);
		  }
	  }

	  public void draw(double x_left, double y_low, double x_right, double y_high) {		
		  for ( int i = 0; i < a.size(); i++) {
			  ((PlowArrayElem)a.get(i)).draw(x_left, y_low, x_right, y_high);
		  }
	  }

	  public void setBorders(PlowBorders borders) { 
		  for ( int i = 0; i < a.size(); i++) {
			  ((PlowArrayElem)a.get(i)).setBorders(borders);
		  }
	  }

	  public void insert( PlowArrayElem e) {
		  a.add(e);
	  }

	  public boolean insertNode(PlowArrayElem e, PlowArrayElem destination, int code) {
		  if ( find(e))
			  return false;

		  int idx;
		  int destination_level = 0;
		  if ( destination == null) {
			  switch ( code) {
			  case Plow.DEST_INTOLAST:
				  idx = a.size();
				  break;
			  default:
				  idx = 0;
			  }
			  destination_level = 0;
		  }
		  else {
			  boolean found = false;
			  for ( idx = 0; idx < a.size(); idx++) {
				  if ( a.get(idx) == destination) {
					  found = true;
					  destination_level = ((PlowNode)a.get(idx)).getLevel(); 
					  idx++;
					  break;
				  }
			  }
			  if ( !found)
				  return false;
		  }
		  switch ( code) {
		  case Plow.DEST_INTOFIRST:
			  a.insertElementAt(e, idx);
			  if ( destination != null)
				  ((PlowNode)e).setLevel(destination_level + 1);
			  else
				  ((PlowNode)e).setLevel(destination_level);
			  break;
		  case Plow.DEST_INTOLAST: {
			  int i;
			  for ( i = idx; i < a.size(); i++) {
				  if ( ((PlowNode)a.get(i)).getLevel() <= destination_level)
					  break;
			  }
			  idx = i;
			  a.insertElementAt(e, idx);
			  if ( destination != null)
				  ((PlowNode)e).setLevel(destination_level + 1);
			  else
				  ((PlowNode)e).setLevel(destination_level);
			  break;
		  }
		  case Plow.DEST_AFTER: {
			  int i;
			  for ( i = idx; i < a.size(); i++) {
				  if ( ((PlowNode)a.get(i)).getLevel() < destination_level)
					  break;
			  }
			  idx = i;
			  a.insertElementAt(e, idx);
			  ((PlowNode)e).setLevel(destination_level);
			  break;
		  }
		  case Plow.DEST_BEFORE:
			  if ( idx > 0)
				  idx--;
			  a.insertElementAt(e, idx);
			  ((PlowNode)e).setLevel(destination_level);
			  break;
		  }
		  return true;
	  }
	  public void closeNode(PlowArrayElem element) {
		  boolean found = false;
		  int idx = 0;
		  int next_idx;
		  int i;
		  
		  for ( i = 0; i < a.size(); i++) {
			  if ( a.get(i) == element) {
				  idx = i;
				  found = true;
				  break;
			  }
		  }
		  if ( !found)
			  return;
		  
		  int level = ((PlowNode)a.get(idx)).getLevel();
		  for ( i = idx + 1; i < a.size(); i++) {
			  if ( ((PlowNode)a.get(i)).getLevel() <= level)
				  break;
		  }
		  next_idx = i;
		  if ( next_idx == idx + 1)
			  return;
		  for ( i = idx + 1; i < next_idx; i++) {
			  PlowEvent e = new PlowEvent(PlowEvent.TYPE_OBJECT_DELETED, 0D, 0D, (PlowNode)a.get(i));
			  cmn.appl.eventHandler(e);
			  if ( cmn.getSelect() == a.get(i))
				  cmn.selectClear();
			  a.remove(i);
			  i--;
			  next_idx--;
		  }
	  }
	  public boolean eventHandler(PlowEvent e) {
		  for ( int i = 0; i < a.size(); i++) {
			  if ( ((PlowArrayElem)a.get(i)).eventHandler(e))
				  return true;
		  }
		  return false;
	  }
	  public void setInvert(boolean invert) {
		  for ( int i = 0; i < a.size(); i++) {
			  ((PlowArrayElem)a.get(i)).setInvert(invert);
		  }
	  }
	  public void configure() {
		  for ( int i = 0; i < a.size(); i++) {
			  if ( i == 0)
				  ((PlowNode)a.get(i)).configure(null);
			  else
				  ((PlowNode)a.get(i)).configure((PlowNode)a.get(i-1));
		  }
	  }
	  public boolean find(PlowArrayElem e) {
		  for ( int i = 0; i < a.size(); i++) {
			  if ( a.get(i) == e)
				  return true;
		  }
		  return false;
	  }
	  
	  public PlowArrayElem get_next( PlowArrayElem element) {
		  int i;

		  for ( i = 0; i < a.size(); i++) {
		    if ( a.get(i) == element) {
		      if ( i == a.size() - 1)
		        return null;
		      return a.get(i+1);
		    }
		  }
		  return null;
		}

		public PlowArrayElem get_previous( PlowArrayElem element) {
		  int i;

		  for ( i = 0; i < a.size(); i++) {
		    if ( a.get(i) == element) {
		      if ( i == 0)
		        return null;
		      return a.get(i-1);
		    }
		  }
		  return null;
		}

		public PlowArrayElem get_first() {
		  if ( a.size() == 0)
		    return null;
		  return a.get(0);
		}

		public PlowArrayElem get_last() {
			if ( a.size() == 0)
				return null;
			return a.get(a.size() - 1);
		}

		public int remove(PlowArrayElem e) {
			if ( a.remove(e))
				return 1;
			return 0;
		}

}
