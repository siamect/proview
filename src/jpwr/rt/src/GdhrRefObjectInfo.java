/* 
 * Proview   $Id: GdhrRefObjectInfo.java,v 1.3 2005-09-01 14:57:52 claes Exp $
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
public class GdhrRefObjectInfo implements Serializable
{
  public PwrtRefId refid;
  public int id;
  public int sts;
  public int typeId;
  public int subId;
  public int elements;
  public int size;
    
  public GdhrRefObjectInfo( PwrtRefId refid, int id, int sts, int typeId) {
    this.refid = refid;
    this.id = id;
    this.sts = sts;
    this.typeId = typeId;
    //default values in case we have forgotten to use the new constructor
    this.elements = 1;
    this.size = 4;
  }
  public GdhrRefObjectInfo( PwrtRefId refid, int id, int sts, int typeId, int elements, int size) {
    this.refid = refid;
    this.id = id;
    this.sts = sts;
    this.typeId = typeId;
    this.elements = elements;
    this.size = size;
  }

  public boolean evenSts() { return (sts % 2 == 0);}
  public boolean oddSts() { return (sts % 2 == 1);}
  public int getSts() { return sts;}
  public int getElements(){ return elements;}
  public int getSize(){ return size;}
}

