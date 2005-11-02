/* 
 * Proview   $Id: PwrtAttrRef.java,v 1.1 2005-11-02 14:02:20 claes Exp $
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

public class PwrtAttrRef implements Serializable
{
  public static final int INDIRECT    	= 1 << 0;
  public static final int OBJECT   	= 1 << 1;
  public static final int OBJECTATTR   	= 1 << 2;
  public static final int ARRAY    	= 1 << 3;
  public static final int SHADOWED    	= 1 << 4;
  public static final int CASTATTR    	= 1 << 5;
  public static final int DISABLEATTR   = 1 << 6;

  public PwrtObjid objid;
  public int body;
  public int offset;
  public int size;
  public int flags;
  
  public PwrtAttrRef( PwrtObjid objid, int body, int offset,
		    int size, int flags) {
    this.objid = objid;
    this.offset = offset;
    this.body = body;
    this.size = size;
    this.flags = flags;
  }
  public PwrtAttrRef( PwrtObjid objid) {
    this.objid = objid;
    this.body = 0;
    this.offset = 0;
    this.size = 0;
    this.flags = OBJECT;
  }
  public PwrtObjid getObjid() { return objid;}
  public int getOix() { return objid.getOix();}
  public int getVid() { return objid.getVid();}
  public int getBody() { return body;}
  public int getOffset() { return offset;}
  public int getSize() { return size;}
  public int getFlags() { return flags;}

  public boolean isNull() {
    return objid.isNull();
  }
  public boolean isEqual( PwrtAttrRef aref) {
    return ( objid.isEqual( aref.objid) &&
	     offset == aref.offset && size == aref.size);
  }
}
