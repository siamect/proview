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
    String systemGroup = "SSAB";
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
  {
    CdhrObjAttr attr = (CdhrObjAttr)this.getClassAttribute(classid, objid_obj);
    Vector v = new Vector();
    while(attr != null)
    {
      v.add(attr);
      attr = this.getClassAttribute(classid, attr.objid);
    }
    return v;
  }
  public Vector getAllSiblings( PwrtObjid objid )
  {
    CdhrObjid sibling = (CdhrObjid)this.getNextSibling(objid);
    Vector v = new Vector();
    while(sibling.oddSts())
    {
      v.add(sibling);
      sibling = this.getNextSibling(sibling.objid);
    }
    return v;
  }
  
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
    Vector v = new Vector();
    
    CdhrObjid classObj;
    
    cdhrObjId = (CdhrObjid)this.getChild(objid);
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
  
  public Vector refObjectInfo_Vector( Vector vec )
  {
    Vector retVec = new Vector();
    for(int i = 0;i < vec.size();i++)
    {
      GdhrRefObjectInfo ret = this.refObjectInfo( (String)vec.get(i) );
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
  public native CdhrString objidToName( PwrtObjid objid, int nameType);
  public native CdhrObjid getRootList();
  public native CdhrObjid getNextObject( PwrtObjid objid);
  public native CdhrObjid getChild( PwrtObjid objid);
  public native CdhrObjid getParent( PwrtObjid objid);
  public native CdhrObjid getNextSibling( PwrtObjid objid);
  public native CdhrClassId getObjectClass( PwrtObjid objid);
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
}








