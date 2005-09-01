/* 
 * Proview   $Id: GdhrGetXttObj.java,v 1.2 2005-09-01 14:57:52 claes Exp $
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

public class GdhrGetXttObj implements Serializable
{
  public String name;
  public String fullName;
  public String className;
  public String description;
  public CdhrObjid cdhrObjId;
  public CdhrClassId cdhrClassId;
  public int sts;
  public boolean hasChildren;
  
  public GdhrGetXttObj(String      name, 
                       String      fullName,
		       String      description, 
		       String      className, 
		       CdhrObjid   cdhrObjId, 
		       CdhrClassId cdhrClassId, 
		       int         sts,
                       boolean     hasChildren) 
  {
    this.name = name; 
    this.fullName = fullName;
    this.description = description;
    this.className = className;
    this.cdhrObjId = cdhrObjId;
    this.cdhrClassId = cdhrClassId;
    this.sts = sts;
    this.hasChildren = hasChildren;
  }
  public boolean evenSts() { return (sts % 2 == 0);}
  public boolean oddSts() { return (sts % 2 == 1);}
  public int getSts() { return sts;}
}
