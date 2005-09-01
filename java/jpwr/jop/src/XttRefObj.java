/* 
 * Proview   $Id: XttRefObj.java,v 1.2 2005-09-01 14:57:51 claes Exp $
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
import java.util.Vector;
import javax.swing.tree.DefaultMutableTreeNode;
import jpwr.rt.*;
/**
 *  Description of the Class
 *
 *@author     JN3920
 *@created    November 12, 2002
 */
public class XttRefObj extends DynamicObj implements JopDynamic
{
  /**  Description of the Field */
  public XttObjAttr objAttr = null;
  //static JopEngine en;
  /**  Description of the Field */
  int classid;
//  String showableClassName = null;
  /**  Description of the Field */
  String fullName;
  /**  Description of the Field */
  int index;
  
  static boolean initDone = false;
  /**  Description of the Field */
  static String[] AttributeNameArray = {"ActualValue", 
                                        "ActualValue", 
					/*"ActualValue",*/
                                        "ActualValue", 
					"ActualValue", 
					"ActualValue", 
					"ActualValue", 
					
					"Status", 
					"Status", 
					"Status", 
					"Status", 
					"Status", 
					"Status", 
					"Status", 
					"Status", 
					"Status", 
					"Status", 
					"Status", 
					"Status", 
					
					"Action", 
					"Action", 
					
					"in", 
					
					"ActVal", 
					"ActVal", 
					"ActVal", 
					"ActVal", 
					"ActVal", 
					"ActVal", 
					"ActVal", 
					"ActVal", 
					"ActVal", 
					"ActVal", 
					
					"MaxVal",
					 
					"High", 
					
					"Low", 
					
					"OutVal", 
					"OutVal"};
  /**  Description of the Field */
  static int[] TypeClassId =           {Pwrb.cClass_Dv, 
                                        Pwrb.cClass_Av,  
					/*Pwrb.cClass_Sv,*/
					Pwrb.cClass_Di, 
					Pwrb.cClass_Do, 
					Pwrb.cClass_Ai,
                                        Pwrb.cClass_Ao, 
					
					Pwrb.cClass_and,
					Pwrb.cClass_or, 
					Pwrb.cClass_xor, 
					Pwrb.cClass_edge,
					Pwrb.cClass_sr_s,
					Pwrb.cClass_sr_r,
					Pwrb.cClass_pulse,
					Pwrb.cClass_wait,
					Pwrb.cClass_timer,
					Pwrb.cClass_inv,
					Pwrb.cClass_waith,
					Pwrb.cClass_darithm,
					
					Pwrb.cClass_DSup,
					Pwrb.cClass_ASup,
					
					Pwrb.cClass_csub, 
					
					Pwrb.cClass_sum, 
					Pwrb.cClass_limit, 
					Pwrb.cClass_select, 
					Pwrb.cClass_ramp, 
					Pwrb.cClass_filter, 
					Pwrb.cClass_speed, 
					Pwrb.cClass_curve, 
					Pwrb.cClass_adelay, 
					Pwrb.cClass_aarithm, 
					Pwrb.cClass_timint, 
					
					Pwrb.cClass_maxmin, 
					
					Pwrb.cClass_comph, 
					
					Pwrb.cClass_compl, 
					
					Pwrb.cClass_pid, 
					Pwrb.cClass_mode};
  /**  Description of the Field */
  static String[] TypeNameArray =      {"Dv", 
                                        "Av",  
					/*"Sv",*/
					"Di", 
					"Do", 
					"Ai", 
					"Ao", 
					
					"And",
					"Or",
					"Xor",
					"Edge", 
					"SR_S",
					"SR_R",
					"Pulse",
					"Wait",
					"Timer",
					"Inv",
					"Waith",
					"DArithm", 
					
					"DSup",
					"ASup",
					
					"CSub", 
					
					"Sum", 
					"Limit", 
					"Select", 
					"Ramp", 
					"Filter", 
					"Speed", 
					"Curve", 
					"ADelay", 
					"AArithm",
					"Timint", 
					
					"MaxMin", 
					
					"Comph", 
					
					"Compl", 
					
					"PID", 
					"Mode"};
  /**  Description of the Field */
  static CdhrObjAttr[] AttrObj = new CdhrObjAttr[TypeNameArray.length];
  /**  Description of the Field */
  static boolean firstTime = true;


  /**
   *  Constructor for the XttRefObj object
   *
   *@param  fullName  Description of the Parameter
   *@param  en        Description of the Parameter
   *@param  classid   Description of the Parameter
   *@param  treeNode  Description of the Parameter
   *@param  index     Description of the Parameter
   */
  public XttRefObj(String fullName, JopEngine en, int classid,  /*String showableClassName,*/DefaultMutableTreeNode treeNode, int index)
  {
    this.index = index;
    this.fullName = fullName;
    this.en = en;
    this.classid = classid;
//    this.showableClassName = showableClassName;
    if(index < 0)
    {
      Logg.logg("Fel vid försök att debugga", 0);
      return;
    }
    CdhrObjAttr oa = AttrObj[index];
    Logg.logg("XttRefObj konstruktor", 6);
    if(oa != null)
    {
      objAttr = new XttObjAttr(oa);
      objAttr.treeNode = treeNode;
      objAttr.showName = false;
      en.add(this);
    }
    else
    {
      Logg.logg("Kan ej debugga " + fullName + " classid: " + classid + " på index " + index, 0);
    }
  }


  /**
   *  Gets the pwrtRefId attribute of the XttRefObj object
   *
   *@return    The pwrtRefId value
   */
  public PwrtRefId getPwrtRefId()
  {
    if(objAttr != null && objAttr.refObj != null)
    {
      return objAttr.refObj.refid;
    }
    return null;
  }


  /**
   *  Gets the xttObjAttr attribute of the XttRefObj object
   *
   *@return    The xttObjAttr value
   */
  public XttObjAttr getXttObjAttr()
  {

    Logg.logg("XttRefObj: getXttObjAttr", 6);
    String s = null;
    String suffix;
    suffix = this.setTypeIdString(objAttr.type, objAttr.size);
    s = this.fullName + "." + objAttr.name + suffix;
    objAttr.fullName = s;
    Logg.logg("XttRefObj:  Name " + s, 6);
    return objAttr;
  }


  public Object dynamicGetRoot() 
  {
    return null;
  }

  /**  Description of the Method */
  public void dynamicClose()
  {
  }



  /**  Description of the Method */
  public void dynamicOpen()
  {
  }


  /**
   *  Description of the Method
   *
   *@param  animationOnly  Description of the Parameter
   */
  public void dynamicUpdate(boolean animationOnly)
  {
    if(animationOnly)
    {
      Logg.logg("XttRefObj: animationOnly", 6);
      return;
    }
    this.setObjectAttributeValue(objAttr);
  }


  /**  Description of the Method */
  public void localDynamicClose()
  {
  }


  /**
   *  Description of the Method
   *
   *@return    Description of the Return Value
   */
  public String toString()
  {
    if(objAttr != null)
    {
      return objAttr.toString();
    }
    else
    {
      return " ";
    }
  }


  /**
   *  Gets the wantedClassAttribute attribute of the XttRefObj class
   *
   *@param  index  Description of the Parameter
   *@return        The wantedClassAttribute value
   */
  public static CdhrObjAttr getWantedClassAttribute(int index)
  {
    PwrtObjid objid_obj = new PwrtObjid(0, 0);
    Vector attr_vec = en.gdh.getAllClassAttributes(TypeClassId[index], objid_obj);
    if(attr_vec != null)
    {
      for(int i = 0; i < attr_vec.size(); i++)
      {
        CdhrObjAttr attr = (CdhrObjAttr)attr_vec.get(i);
        if(attr.name.compareTo(AttributeNameArray[index]) == 0)
        {
          return attr;
        }
      }
    }
    return null;
  }


  /**
   *  Description of the Method
   *
   *@param  s  Description of the Parameter
   *@return    Description of the Return Value
   */
  public static int checkTypeName(String s)
  {
    for(int i = 0; i < XttRefObj.TypeNameArray.length; i++)
    {
      if(s.compareTo(XttRefObj.TypeNameArray[i]) == 0)
      {
        return i;
      }
    }
    return -1;
  }


  /**
   *  Description of the Method
   *
   *@param  en  Description of the Parameter
   */
  public static void init(JopEngine en)
  {
    if(XttRefObj.initDone)
      return;
    XttRefObj.en = en;
    for(int i = 0; i < TypeClassId.length; i++)
    {
      AttrObj[i] = XttRefObj.getWantedClassAttribute(i);
    }
    XttRefObj.initDone = true;
  }

}


