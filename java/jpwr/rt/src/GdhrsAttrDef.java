/* 
 * Proview   $Id: GdhrsAttrDef.java,v 1.1 2005-12-06 11:17:01 claes Exp $
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
import jpwr.rt.PwrsParInfo;

public class GdhrsAttrDef implements Serializable
{
  public String attrName;
  public int attrLevel;
  public int attrClass;
  public int flags;
  public int typeRef;
  public PwrsParInfo info;
  public int sts;
  
  public GdhrsAttrDef( String attrName,
		       int attrLevel,
		       int attrClass,
		       int flags,
		       int typeRef,
		       PwrsParInfo info,
		       int sts) {
    this.attrName = attrName;
    this.attrLevel = attrLevel;
    this.attrClass = attrClass;
    this.flags = flags;
    this.info = info;
    this.typeRef = typeRef;
    this.sts = sts;
  }
  public boolean evenSts() { return (sts % 2 == 0);}
  public boolean oddSts() { return (sts % 2 == 1);}
  public int getSts() { return sts;}
}
