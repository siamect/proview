/* 
 * Proview   $Id: PwrtObjid.java,v 1.2 2005-09-01 14:57:52 claes Exp $
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
import java.io.Serializable;

public class PwrtObjid implements Serializable
{
  public int oix;
  public int vid;
  
  public PwrtObjid( int oix, int vid) {
    this.oix = oix;
    this.vid = vid;
  }
  public int getOix() { return oix;}
  public int getVid() { return vid;}

  public boolean isNull() {
    return ( oix == 0 && vid == 0);
  }
  public boolean isEqual( PwrtObjid objid) {
    return ( objid.oix == oix && objid.vid == vid);
  }
}
