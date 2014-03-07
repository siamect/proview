package jpwr.app;

import android.graphics.Bitmap;

public class AGraphInfo {
    public static final int TYPE_GRAPH = 0;
    public static final int TYPE_LINK = 1;
    public int type;
    public Bitmap bpm;
    public Bitmap bpmInverted;
    public String graph;
    public String text;
    public int iconX;
    public int iconY;
    public int iconWidth;
    public int iconHeight;
    public int textX;
    public int textY;
    public boolean inverted = false;
}
