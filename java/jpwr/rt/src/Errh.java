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
  public static final int eAnix_webmon 		= 14;
  public static final int eAnix_webmonmh       	= 15;

  public static final int PWR__SRVSTARTUP       = 134512720;
  public static final int PWR__SRUN             = 134512731;
  public static final int PWR__SRVRESTART       = 134512736;
  public static final int PWR__SRVTERM          = 134512748;

  static {
    System.loadLibrary("jpwr_rt_gdh");
  }
  
  private static boolean initDone = false;

  public Errh( String programName, int anix) {
    if ( !initDone) {
      init( programName, anix);
      initDone = true;
    }
  }
  
  private native int init( String programName, int anix);
  public native void setStatus( int sts);
  public native void fatal( String msg);
  public native void error( String msg);
  public native void warning( String msg);
  public native void info( String msg);
  public native void success( String msg);
}



