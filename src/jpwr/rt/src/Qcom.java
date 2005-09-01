/* 
 * Proview   $Id: Qcom.java,v 1.3 2005-09-01 14:57:52 claes Exp $
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
  public native QcomrCreateQ createIniEventQ(String name);
  public native QcomrGetIniEvent getIniEvent(int qix, int nid, int timeoutTime);
  private native PwrtStatus putString( int qix, int nid, String data);
  public native CdhrString getString( int qix, int nid);
}
