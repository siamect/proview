/**
 * Title:	<p>
 * Description:	<p>
 * Copyright:	<p>
 * Company	SSAB<p>
 * @author	CS
 * @version	1.0
 */

package jpwr.rt;

public class Errh {
  static {
    System.loadLibrary("jpwr_rt_gdh");
  }
  
  private static boolean initDone = false;

  public Errh( String programName) {

    if ( !initDone) {
      init( programName);
      initDone = true;
    }
  }
  
  private native int init( String programName);
  public native void fatal( String msg);
  public native void error( String msg);
  public native void warning( String msg);
  public native void info( String msg);
  public native void success( String msg);
}



