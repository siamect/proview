package jpwr.app;

public class PlowCmnEv extends PlowCmn {
	public PlowCmnEv(PlowAppl appl, double zoom_factor) {
		super(appl, zoom_factor);
	}
		
		public int type() {
		return PlowCmnIfc.TYPE_EV;
	}	
}
