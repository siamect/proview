/* 
 * Proview   $Id: Errh.java,v 1.4 2005-09-01 14:57:52 claes Exp $
 * Copyright (C) 2005 SSAB Oxelösund AB.
 *
 * This program is free software; you can redistribute it and/or 
 * modify it under the terms of the GNU General Public License as 
 * published by the Free Software Foundation, either version 2 of 
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful 
 * but WITHOUT ANY WARRANTY; without even the implied warranty of 
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the 
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License 
 * along with the program, if not, write to the Free Software 
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

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



