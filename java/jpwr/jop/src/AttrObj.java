package jpwr.jop;
import javax.swing.tree.DefaultMutableTreeNode;
import javax.swing.tree.DefaultTreeModel;
//package navigator;
import jpwr.rt.*;

/**
 *  Description of the Class
 *
 *@author     JN3920
 *@created    November 12, 2002
 */
public class AttrObj extends TreeObj
{
  /**  Description of the Field */
  int elements = 0;
  /**  Description of the Field */
  int flags;
  /**  Description of the Field */
//  String fullName = null;
  /**  Description of the Field */
  int lengthToSecondCol = 20;
  /**  Description of the Field */
//  String name = null;
  /**  Description of the Field */
  PwrtObjid objid;
  /**  Description of the Field */
  GdhrRefObjectInfo refObj;
  /**  Description of the Field */
  boolean showName = true;
  /**  Description of the Field */
  int size;
  /**  Description of the Field */
  DefaultMutableTreeNode treeNode;
  /**  Description of the Field */
  int type;
  /**  Description of the Field */
  boolean valueBoolean;
  /**  Description of the Field */
  float valueFloat;

  /**  Description of the Field */
  int valueInt;
  /**  Description of the Field */
  String valueString = " ";
  /**  Description of the Field */
  static DefaultTreeModel treeModel;


  /**
   *  Sets the value attribute of the AttrObj object
   *
   *@param  value  The new value value
   */
  public void setValue(int value)
  {
    if(this.valueInt == value)
    {
      return;
    }
    this.valueInt = value;
    treeModel.nodeChanged(this.treeNode);
  }


  /**
   *  Sets the value attribute of the AttrObj object
   *
   *@param  value  The new value value
   */
  public void setValue(float value)
  {
    if(this.valueFloat == value)
    {
      return;
    }
    this.valueFloat = value;
    treeModel.nodeChanged(this.treeNode);
  }


  /**
   *  Sets the value attribute of the AttrObj object
   *
   *@param  value  The new value value
   */
  public void setValue(boolean value)
  {
    if(this.valueBoolean == value)
    {
      return;
    }
    this.valueBoolean = value;
    treeModel.nodeChanged(this.treeNode);
  }


  /**
   *  Sets the value attribute of the AttrObj object
   *
   *@param  value  The new value value
   */
  public void setValue(String value)
  {
    if(this.valueString.compareTo(value) == 0)
    {
      return;
    }
    this.valueString = value;
    treeModel.nodeChanged(this.treeNode);
  }


  /**
   *  Description of the Method
   *
   *@return    Description of the Return Value
   */
  public String toString()
  {
    int spaceLength = this.lengthToSecondCol - this.name.length();
    String space = " ";
    String n = " ";
    String s;
    if(showName)
    {
      n = this.name;
      while(spaceLength > 1)
      {
        spaceLength--;
        space += " ";
      }
    }

    switch (this.type)
    {
      case Pwr.eType_Float32:
      case Pwr.eType_Float64:
        s = n + space + this.valueFloat;
        break;
      case Pwr.eType_UInt32:
      case Pwr.eType_UInt64:
      case Pwr.eType_Int32:
      case Pwr.eType_Int64:

        //s = n + space + this.valueInt;
	s = n + space + (new Integer( (this.valueInt & 65535) )).intValue();
        break;

      case Pwr.eType_UInt16:
        s = n + space + (new Integer( (this.valueInt & 65535) )).intValue();
        break;
      case Pwr.eType_Int8:
      case Pwr.eType_UInt8:
        s = n + space + (new Integer(this.valueInt)).byteValue();
        break;
      case Pwr.eType_Int16:
        s = n + space + (new Integer(this.valueInt)).shortValue();
        break;
      case Pwr.eType_Boolean:
        if(this.valueBoolean)
        {
          s = n + space + "1";
        }
        else
        {
          s = n + space + "0";
        }
        break;
      default:
        s = n + space + this.valueString;
        break;
    }
    return s;
  }
}

