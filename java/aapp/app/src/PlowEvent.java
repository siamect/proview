package jpwr.app;

public class PlowEvent {
	public static final int TYPE_CLICK = 1;
	public static final int TYPE_OBJECT_DELETED = 2;
	public static final int TYPE_LONG_CLICK = 3;
	
	public int type;
	public double x;
	public double y;
	public Object object;
	public boolean inIcon;
	
	PlowEvent(int type, double x, double y, Object object) {
		this.type = type;
		this.x = x;
		this.y = y;
		this.object = object;
		this.inIcon = false;
	}
}
