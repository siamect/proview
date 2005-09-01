/* 
 * Proview   $Id: XttObjAttr.java,v 1.2 2005-09-01 14:57:51 claes Exp $
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

package jpwr.jop;
//package navigator;
//import AttrObj;
import javax.swing.tree.DefaultMutableTreeNode;
import javax.swing.tree.DefaultTreeModel;
import jpwr.rt.*;

/**
 *  Description of the Class
 *
 *@author     JN3920
 *@created    November 12, 2002
 */
public class XttObjAttr extends AttrObj
{
  /**
   *  Constructor for the XttObjAttr object
   *
   *@param  obj  Description of the Parameter
   */
  public XttObjAttr(CdhrObjAttr obj)
  {
    this.objid = obj.objid;
    this.name = obj.name;
    this.type = obj.type;
    this.size = obj.size;
    this.flags = obj.flags;
    this.elements = obj.elements;
  }


  /**  Constructor for the XttObjAttr object */
  public XttObjAttr() { }


  /**
   *  Description of the Method
   *
   *@return    Description of the Return Value
   */
  public String toString()
  {
    if((this.flags & Pwr.mAdef_array) > 0)
    {
      return this.name;
    }
    return super.toString();
  }
}

