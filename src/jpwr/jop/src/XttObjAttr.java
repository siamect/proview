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

