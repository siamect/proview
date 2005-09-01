/* 
 * Proview   $Id: CdhrObjAttr.java,v 1.2 2005-09-01 14:57:52 claes Exp $
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
 * @version	1.1
 */

package jpwr.rt;
import java.io.Serializable;

public class CdhrObjAttr implements Serializable
{
  public PwrtObjid objid;
  public GdhrRefObjectInfo refObj;
  public String name;
  public String fullName = null;
  public int type;
  public int size;
  public int flags;
  public int elements;
  public int sts;
  
  public CdhrObjAttr(PwrtObjid objid,
                     String  name,
                     int     type,
                     int     size,
                     int     flags,
                     int     elements)
 
  {
    this.objid = objid;
    this.name = name;
    this.type = type;
    this.size = size;
    this.flags = flags;
    this.elements = elements;
  }
  public boolean evenSts() { return (sts % 2 == 0);}
  public boolean oddSts() { return (sts % 2 == 1);}
  public int getSts() { return sts;}
}
