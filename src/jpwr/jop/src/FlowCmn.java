package jpwr.jop;
import jpwr.rt.*;
import java.io.*;
import java.util.*;


public class FlowCmn {
  static final int display_level = Flow.mDisplayLevel_1;
  boolean debug;
  boolean antiAliasing;
  Object ctx;
  Gdh gdh;
  JopSession session;
  double zoom_factor;
  double base_zoom_factor;
  int offset_x;
  int offset_y;
  double x_right;
  double x_left;
  double y_high;
  double y_low;

  Vector a = new Vector();
  Vector a_nc = new Vector();
  Vector a_cc = new Vector();

  public FlowCmn( Object ctx, Gdh gdh, JopSession session) {
    this.ctx = ctx;
    this.gdh = gdh;
    this.session = session;
    this.debug = false;
    this.antiAliasing = true;
  }

  public void unselect() {
    ((FlowCtxInterface)ctx).unselect();
  }
}
