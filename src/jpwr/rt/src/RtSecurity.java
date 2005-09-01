/* 
 * Proview   $Id: RtSecurity.java,v 1.2 2005-09-01 14:57:52 claes Exp $
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

package jpwr.rt;

public class RtSecurity {

  static {
    System.loadLibrary("jpwr_rt_gdh");
  }

  public static native CdhrInt checkUser( String group, String user, String password);

  public static CdhrInt checkUser2( String group, String user, String password) {
    CdhrInt cdhrInt;
    int	sts = 1;
    int priv;
    if ( user.toUpperCase().equals("CLAES") &&
         password.toUpperCase().equals("HEJHOPP")) {
      priv = Pwr.mAccess_AllPwr;
      return new CdhrInt( priv, sts);
    }
    else if ( user.toUpperCase().equals("SKIFTEL") &&
         password.toUpperCase().equals("SKIFTEL")) {
      priv = Pwr.mPrv_RtRead | Pwr.mPrv_RtWrite;
      return new CdhrInt( priv, sts);
    }
    else if ( user.toUpperCase().equals("OP55") &&
         password.toUpperCase().equals("OP55")) {
      priv = Pwr.mPrv_RtRead | Pwr.mPrv_Operator1;
      return new CdhrInt( priv, sts);
    }
    return new CdhrInt( 0, 0);
  }
}
