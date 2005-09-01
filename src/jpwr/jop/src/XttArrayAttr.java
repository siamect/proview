/* 
 * Proview   $Id: XttArrayAttr.java,v 1.2 2005-09-01 14:57:51 claes Exp $
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
public class XttArrayAttr extends AttrObj
{
  /**
   *  Constructor for the XttArrayAttr object
   *
   *@param  name      Description of the Parameter
   *@param  fullName  Description of the Parameter
   *@param  type      Description of the Parameter
   *@param  elements  Description of the Parameter
   */
  public XttArrayAttr(String name, String fullName, int type, int elements)
  {
    this.name = name;
    this.fullName = fullName;
    this.type = type;
    this.elements = elements;
  }


  /**
   *  Constructor for the XttArrayAttr object
   *
   *@param  dummyname  Description of the Parameter
   */
  public XttArrayAttr(String dummyname)
  {
    this.name = dummyname;
  }
}

