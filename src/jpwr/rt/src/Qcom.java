/**
 * Title:	<p>
 * Description:	<p>
 * Copyright:	<p>
 * Company	SSAB<p>
 * @author	CS
 * @version	1.0
 */

package jpwr.rt;

public class Qcom {
  static {
    System.loadLibrary("jpwr_rt_gdh");
  }
  
  public Qcom() {
  }
  public PwrtStatus put( int qix, int nid, String data) {
    return putString( qix, nid, data);
  }

  public native QcomrCreateQ createQ( int qix, int nid, String name);
  private native PwrtStatus putString( int qix, int nid, String data);
  public native CdhrString getString( int qix, int nid);
}
