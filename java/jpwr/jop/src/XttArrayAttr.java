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

