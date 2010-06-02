/* 
 * Proview   $Id: Gdh.java,v 1.11 2008-06-24 13:27:53 claes Exp $
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
import java.util.Vector;
public class Gdh {
  static {
    System.loadLibrary("jpwr_rt_gdh");
  }
  
  private static boolean initDone = false;

  private static String currentSystemGroup = null;
  private static String	currentUser = null;
  private static String currentPassword = null;
  private static int currentPrivilege = Pwr.mAccess_AllPwr;


  public Gdh( Object root) {

    if ( !initDone) {
      init();
      initDone = true;
    }
  }
  public void close() {
  }
  
  public void getObjectRefInfoAll() {
  }

  public void refObjectInfoList() {
  }
  
  public void printStatistics( int lockRejected) {
  }

  public PwrtStatus setObjectInfo( String attributeName, float value) {
    return setObjectInfoFloat( attributeName, value);
  } 

  public PwrtStatus setObjectInfo( String attributeName, boolean value) {
    return setObjectInfoBoolean( attributeName, value);
  } 

  public PwrtStatus setObjectInfo( String attributeName, int value) {
    return setObjectInfoInt( attributeName, value);
  } 

  public PwrtStatus setObjectInfo( String attributeName, String value) {
    return setObjectInfoString( attributeName, value);
  } 

  public int login( String user, String password) {
    // Get system group
    String systemGroup = "NotUsed";
    CdhrInt ret = RtSecurity.checkUser( systemGroup, user, 
			password);
    if ( ret.evenSts()) {
      logout();
      return ret.getSts();
    }
    currentSystemGroup = systemGroup;
    currentUser = user;
    currentPassword = password;
    currentPrivilege = ret.value;
    return 1;
  }

  public void logout() {
    currentSystemGroup = null;
    currentUser = null;
    currentPassword = null;
    currentPrivilege = Pwr.mAccess_AllPwr;
  }

  public int checkUser() {
    return 1;
  }

  public String getUser() {
    return currentUser;
  }

  public boolean isAuthorized( int access) {
    return (access & currentPrivilege) != 0;
  }

  public PwrtStatus createInstanceFile( String from, String to, 
					String instance) {
    // Dummy
    return new PwrtStatus(0);
  }
  public void logString( String str) {
    // Dummy
  }

  public PwrtStatus unrefObjectInfoAll() {
    // Used by applets only, not by applications
    return new PwrtStatus(0);
  }

  public boolean isBusy() {
    // Used by applets only
    return false;
  }

  public Vector getAllClassAttributes( int classid, PwrtObjid objid_obj )
  //public Vector<CdhrObjAttr> getAllClassAttributes( int classid, PwrtObjid objid_obj )
  {
    //System.out.println("getAllClassAttributes" + classid + " " + objid_obj.oix + " " + objid_obj.vid);

    String name = this.objidToName(objid_obj, Cdh.mName_pathStrict).str;

    CdhrClassId cdhrClassId = this.getObjectClass(objid_obj);

    GdhrsAttrDef[] gdhrsAttrDefArr = this.getObjectBodyDef(classid, new PwrtAttrRef(objid_obj));

    //Vector<CdhrObjAttr> v = new Vector<CdhrObjAttr>();
    Vector v = new Vector();
    for(int i = 0;i<gdhrsAttrDefArr.length;i++)
    {
      if(gdhrsAttrDefArr[i] == null)
	break;
      //System.out.println("getAllClassAttributesFor:" + i + gdhrsAttrDefArr[i].attrName);

      while(gdhrsAttrDefArr[i].attrName.startsWith("Super."))
	gdhrsAttrDefArr[i].attrName = gdhrsAttrDefArr[i].attrName.substring(6);
      CdhrObjAttr oa;
      if((gdhrsAttrDefArr[i].info.Flags & Pwr.mAdef_class) > 0)
      {
        oa = new CdhrObjAttr(this.nameToObjid(name + "." + gdhrsAttrDefArr[i].attrName).objid,
                                       gdhrsAttrDefArr[i].attrName,
				       gdhrsAttrDefArr[i].typeRef,
				       gdhrsAttrDefArr[i].info.Size,
				       gdhrsAttrDefArr[i].info.Flags,
				       gdhrsAttrDefArr[i].info.Elements);
      }
      else
      {
	  //System.out.println("getAllClassAttr:" + name + "." + gdhrsAttrDefArr[i].attrName);
        oa = new CdhrObjAttr(this.nameToObjid(name + "." + gdhrsAttrDefArr[i].attrName).objid,
                                       gdhrsAttrDefArr[i].attrName,
				       gdhrsAttrDefArr[i].info.Type,
				       gdhrsAttrDefArr[i].info.Size,
				       gdhrsAttrDefArr[i].info.Flags,
				       gdhrsAttrDefArr[i].info.Elements);
      }
      v.add(oa);
    }
    return v;
  }

  //public Vector<CdhrObjAttr> getAllClassAttributes( String name )
  public Vector getAllClassAttributes( String name )
  {

    CdhrAttrRef attrRef = this.nameToAttrRef(name);
    CdhrTypeId cdhrTypeId = this.getAttrRefTid( attrRef.aref );

    GdhrsAttrDef[] gdhrsAttrDefArr = this.getObjectBodyDef(cdhrTypeId.getTypeId(), attrRef.aref);

    if(gdhrsAttrDefArr == null)
    {
	System.out.println("getAllClassAttributes(String name) gdhrsAttrDefArr == null");
	return this.getAllClassAttributes(this.getObjectClass(this.nameToObjid(name).objid).classId, this.nameToObjid(name).objid );

    }
    //Vector<CdhrObjAttr> v = new Vector<CdhrObjAttr>();
    Vector v = new Vector();
    for(int i = 0;i<gdhrsAttrDefArr.length;i++)
    {
      if(gdhrsAttrDefArr[i] == null)
	break;
      //System.out.println("getAllClassAttributesFor:" + i + gdhrsAttrDefArr[i].attrName);

      while(gdhrsAttrDefArr[i].attrName.startsWith("Super."))
	gdhrsAttrDefArr[i].attrName = gdhrsAttrDefArr[i].attrName.substring(6);
      CdhrObjAttr oa;
      if((gdhrsAttrDefArr[i].info.Flags & Pwr.mAdef_class) > 0)
      {
        oa = new CdhrObjAttr(this.nameToObjid(name + "." + gdhrsAttrDefArr[i].attrName).objid,
                                       gdhrsAttrDefArr[i].attrName,
				       gdhrsAttrDefArr[i].typeRef,
				       gdhrsAttrDefArr[i].info.Size,
				       gdhrsAttrDefArr[i].info.Flags,
				       gdhrsAttrDefArr[i].info.Elements);
      }
      else
      {
	  //System.out.println("getAllClassAttr2:" + name + "." + gdhrsAttrDefArr[i].attrName);
        oa = new CdhrObjAttr(this.nameToObjid(name + "." + gdhrsAttrDefArr[i].attrName).objid,
                                       gdhrsAttrDefArr[i].attrName,
				       gdhrsAttrDefArr[i].info.Type,
				       gdhrsAttrDefArr[i].info.Size,
				       gdhrsAttrDefArr[i].info.Flags,
				       gdhrsAttrDefArr[i].info.Elements);
      }
      v.add(oa);
    }
    return v;
  }





  //public Vector<CdhrObjid> getAllSiblings( PwrtObjid objid )
  public Vector getAllSiblings( PwrtObjid objid )
  {
    CdhrObjid sibling = (CdhrObjid)this.getNextSibling(objid);
    //Vector<CdhrObjid> v = new Vector<CdhrObjid>();
    Vector v = new Vector();
    while(sibling.oddSts())
    {
      v.add(sibling);
      sibling = this.getNextSibling(sibling.objid);
    }
    return v;
  }
  
  //public Vector<GdhrGetXttObj> getAllXttSiblings( PwrtObjid objid )
  public Vector getAllXttSiblings( PwrtObjid objid )
  {
    String name = null;
    String fullName = null;
    String description = " ";
    String className = null;
    CdhrObjid cdhrObjId;
    CdhrClassId cdhrClassId;
    int sts = 2;
    boolean hasChildren = false;
    //Vector<GdhrGetXttObj> v = new Vector<GdhrGetXttObj>();
    Vector v = new Vector();
    
    CdhrObjid classObj;
    
    cdhrObjId = (CdhrObjid)this.getNextSibling(objid);
    while(cdhrObjId.oddSts())
    {
      cdhrClassId = this.getObjectClass(cdhrObjId.objid);
      if(cdhrClassId.oddSts())
      {
        classObj = this.classIdToObjid( cdhrClassId.classId );
        if(classObj.oddSts())
	{
	  className = this.objidToName(classObj.objid, Cdh.mName_object).str;
          fullName = this.objidToName( cdhrObjId.objid, Cdh.mName_pathStrict).str;
	  name = this.objidToName( cdhrObjId.objid, Cdh.mName_object).str;
	  CdhrString ret = this.getObjectInfoString(fullName + ".Description");
          if(ret.oddSts())
            description = ret.str;
          else
            description = " ";
	  sts = 1;
	  if(this.getChild(cdhrObjId.objid).oddSts())
	  {
	    hasChildren = true;
	  }
	}
      }
	
      v.add(new GdhrGetXttObj(name, 
                              fullName, 
			      description,
			      className, 
			      cdhrObjId, 
			      cdhrClassId, 
			      sts,
                              hasChildren));
      cdhrObjId = this.getNextSibling(cdhrObjId.objid);
      hasChildren = false;
      sts = 2;
    }
    return v;
  }
  //public Vector<GdhrGetXttObj> getAllXttChildren( PwrtObjid objid )
  public Vector getAllXttChildren( PwrtObjid objid )
  {
    String name = null;
    String fullName = null;
    String description = " ";
    String className = null;
    CdhrObjid cdhrObjId;
    CdhrClassId cdhrClassId;
    int sts = 2;
    boolean hasChildren = false;
    //Vector<GdhrGetXttObj> v = new Vector<GdhrGetXttObj>();
    Vector v = new Vector();
    
    CdhrObjid classObj;
    
    cdhrObjId = (CdhrObjid)this.getChild(objid);
    while(cdhrObjId.oddSts())
    {
	//System.out.println("whilegetAllXttChildren");
      cdhrClassId = this.getObjectClass(cdhrObjId.objid);
      if(cdhrClassId.oddSts())
      {
        classObj = this.classIdToObjid( cdhrClassId.classId );
        if(classObj.oddSts())
	{
	  className = this.objidToName(classObj.objid, Cdh.mName_object).str;
          fullName = this.objidToName( cdhrObjId.objid, Cdh.mName_pathStrict).str;
	  name = this.objidToName( cdhrObjId.objid, Cdh.mName_object).str;
	  CdhrString ret = this.getObjectInfoString(fullName + ".Description");
          if(ret.oddSts())
            description = ret.str;
	  else
            description = " ";
	  sts = 1;
	  if(this.getChild(cdhrObjId.objid).oddSts())
	  {
	    hasChildren = true;
	  }
	}
      }
	
      v.add(new GdhrGetXttObj(name, 
                              fullName, 
			      description,
			      className, 
			      cdhrObjId, 
			      cdhrClassId, 
			      sts,
                              hasChildren));
      cdhrObjId = this.getNextSibling(cdhrObjId.objid);
      hasChildren = false;
      sts = 2;
    }
    return v;
  }

  //public Vector<GdhrRefObjectInfo> refObjectInfo_Vector( Vector vec )
  public Vector refObjectInfo_Vector( Vector vec )
  {
    //Vector<GdhrRefObjectInfo> retVec = new Vector<GdhrRefObjectInfo>();
    Vector retVec = new Vector();
    for(int i = 0;i < vec.size();i++)
    {
      GdhrRefObjectInfo ret = this.refObjectInfo( (String)vec.get(i) );

      // System.out.println("refObjectInfo_vector: " + (String)vec.get(i) + " sts: " + ret.sts);
      retVec.add(ret);
    }
    return retVec;
  }
  public Vector unrefObjectInfo_Vector( Vector vec )
  { 
    Vector retVec = new Vector();
//    for(int i = 0;i < vec.size();i++)
//    {
//      GdhrRefObjectInfo ret = this.refObjectInfo( (String)vec.get(i) );
//      retVec.add(ret);
//    }
    return retVec;
  }
  
  private native int init();
  private native PwrtStatus setObjectInfoFloat( String attributeName, float value);
  private native PwrtStatus setObjectInfoInt( String attributeName, int value);
  private native PwrtStatus setObjectInfoBoolean( String attributeName, boolean value);
  private native PwrtStatus setObjectInfoString( String attributeName, String value);
  public native CdhrFloat getObjectInfoFloat( String attributeName);
  public native CdhrInt getObjectInfoInt( String attributeName);
  public native CdhrBoolean getObjectInfoBoolean( String attributeName);
  public native CdhrString getObjectInfoString( String attributeName);
  public native CdhrObjid getObjectInfoObjid( String attributeName);
  public native CdhrFloatArray getObjectInfoFloatArray( String attributeName, int size);
  public native CdhrIntArray getObjectInfoIntArray( String attributeName, int size);
  public native PwrtStatus toggleObjectInfo( String attributeName);
  public native GdhrRefObjectInfo refObjectInfo( String attributeName);

  public native float getObjectRefInfoFloat( int id);
  public native boolean getObjectRefInfoBoolean( int id);
  public native int getObjectRefInfoInt( int id);
  public native String getObjectRefInfoString( int id, int typeid);

  public native float[] getObjectRefInfoFloatArray( int id, int elements);
  public native boolean[] getObjectRefInfoBooleanArray( int id, int elements);
  public native int[] getObjectRefInfoIntArray( int id, int elements);
  public native String[] getObjectRefInfoStringArray( int id, int typeid, int size, int elements);

  public native PwrtStatus unrefObjectInfo( PwrtRefId refid);

  public native CdhrObjid nameToObjid( String objectName);
  public native CdhrAttrRef nameToAttrRef( String objectName);
  public native CdhrString objidToName( PwrtObjid objid, int nameType);
  public native CdhrString attrRefToName( PwrtAttrRef aref, int nameType);

  public native CdhrObjid getRootList();
  public native CdhrObjid getNextObject( PwrtObjid objid);
  public native CdhrObjid getChild( PwrtObjid objid);
  public native CdhrObjid getParent( PwrtObjid objid);
  public native CdhrObjid getNextSibling( PwrtObjid objid);

  public native CdhrClassId getObjectClass( PwrtObjid objid);
  public native CdhrTypeId getAttrRefTid( PwrtAttrRef aref);
  public native CdhrObjid getClassList( int classid);
  public native CdhrObjid classIdToObjid( int classid);
  public native CdhrObjid getNodeObject( int nodeIdx);
  public native GdhrGetAttributeChar getAttributeChar( String attributeName);
  public native CdhrObjAttr getClassAttribute( int classid, PwrtObjid objid_obj );
  public static native String translateFilename( String filename);
  public native CdhrString crrSignal( String name);
  public native CdhrString crrObject( String name);
  public native CdhrString getMsg( int sts);
  public native CdhrString getMsgText( int sts);
  public native CdhrClassId getSuperClass( int classid, PwrtObjid objid);
  public native GdhrsAttrDef[] getObjectBodyDef(int classid, PwrtAttrRef aref);
  //  public native GdhrGetXttObj[] getAllXttChildrenNative(PwrtObjid objid);
}












