/* 
 * Proview   $Id: PwrsParInfo.java,v 1.1 2005-12-06 11:17:01 claes Exp $
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
 * @author	JN
 * @version	1.0
 */

package jpwr.rt;
import java.io.Serializable;
public class PwrsParInfo implements Serializable
{
  public String PgmName;
  public int   Type;
  public int    Offset;
  public int    Size;
  public int    Flags;
  public int    Elements;
  public int    ParamIndex;
  public PwrsParInfo( String name, 
                      int type,
		      int offset,
		      int size,
		      int flags,
		      int elements,
		      int paramindex) {
    this.PgmName = name;
    this.Type = type;
    this.Offset = offset;
    this.Size = size;
    this.Flags = flags;
    this.Elements = elements;
    this.ParamIndex = paramindex;
  }
}
