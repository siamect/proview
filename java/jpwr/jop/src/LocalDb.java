/* 
 * Proview   $Id: LocalDb.java,v 1.4 2006-06-16 05:11:42 claes Exp $
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
import jpwr.rt.*;
import java.util.*;
import java.io.*;

/**
 *  Description of the Class
 *
 *@author     cs
 *@created    July 12, 2004
 *@version    1.0
 */
public class LocalDb {

  private class LocalSub {
    public float valueFloat = 0;
    public int valueInt = 0;
    public boolean valueBoolean = false;
    public String valueString = null;
    String name;
    boolean occupied = false;
    int typeId;
    int refId;
    Object owner;
    int refCount = 0;

    public LocalSub( Object owner, String name, int typeId, int refId) {
      this.owner = owner;
      this.name = name;
      this.typeId = typeId;
      this.refId = refId;
      this.occupied = true;
    }    
    public void ref() {
      refCount++;
    }
    public void unref() {
      refCount--;
    }
    public int getRefCount() {
      return refCount;
    }
  }

  //Vector<LocalSub> subscriptions = new Vector<LocalSub>();
  Vector subscriptions = new Vector();
  int subscriptionCount = 1;

  public LocalDb() {
    // Insert dummy first
    subscriptions.insertElementAt(null, 0);
  }

  public GdhrRefObjectInfo refObjectInfo( Object owner, String attributeName) {
    int typeId = getTypeId(attributeName);
    String name = getName(attributeName);
    if ( typeId == 0) {
      return new GdhrRefObjectInfo(null, 0, 0, 0);
    }

    int p = nameToId( owner, name);
    if ( p == -1) {
      PwrtRefId refid = new PwrtRefId( subscriptionCount, 0);
      int id = subscriptionCount;
      LocalSub sub = new LocalSub( owner, name, typeId, subscriptionCount);
      sub.ref();
      subscriptions.insertElementAt(sub, subscriptionCount);
      subscriptionCount++;
      return new GdhrRefObjectInfo(refid, id, 1, typeId);
    }
    else {
      PwrtRefId refid = new PwrtRefId( p, 0);
      LocalSub sub = (LocalSub) subscriptions.elementAt(p);
      sub.ref();
      return new GdhrRefObjectInfo( refid, p, 1, typeId); 
    }
  }

  public void unrefObjectInfo( int id) {
    LocalSub sub = (LocalSub) subscriptions.elementAt(id);
    if ( sub == null)
      return;
    sub.unref();
    if ( sub.getRefCount() <= 0)
      subscriptions.setElementAt( null, id);
  }

  public float getObjectRefInfoFloat(int id) {
    try {
      LocalSub sub = (LocalSub) subscriptions.elementAt(id);
      if ( sub == null)
	return 0F;
      return sub.valueFloat;
    }
    catch(ArrayIndexOutOfBoundsException e) {
      return 0F;
    }
  }

  public int getObjectRefInfoInt(int id) {
    try {
      LocalSub sub = (LocalSub) subscriptions.elementAt(id);
      if ( sub == null)
	return 0;
      return sub.valueInt;
    }
    catch(ArrayIndexOutOfBoundsException e) {
      return 0;
    }
  }

  public boolean getObjectRefInfoBoolean(int id) {
    try {
      LocalSub sub = (LocalSub) subscriptions.elementAt(id);
      if ( sub == null)
	return false;
      return sub.valueBoolean;
    }
    catch(ArrayIndexOutOfBoundsException e) {
      return false;
    }
  }

  public String getObjectRefInfoString(int id, int typeid) {
    try {
      LocalSub sub = (LocalSub) subscriptions.elementAt(id);
      if ( sub != null && sub.valueString != null)
	return sub.valueString;
      else
        return new String();
    }
    catch(ArrayIndexOutOfBoundsException e) {
      return new String();
    }
  }

  public CdhrBoolean getObjectInfoBoolean(Object owner, String attributeName) {
    String name = getName( attributeName);
    int id = nameToId( owner, name);
    if ( id == -1)
      return new CdhrBoolean( false, 0);
    return new CdhrBoolean( ((LocalSub)subscriptions.elementAt(id)).valueBoolean, 1);
  }

  public CdhrInt getObjectInfoInt(Object owner, String attributeName) {
    String name = getName( attributeName);
    int id = nameToId( owner, name);
    if ( id == -1)
      return new CdhrInt( 0, 2);
    return new CdhrInt( ((LocalSub)subscriptions.elementAt(id)).valueInt, 1);
  }

  public CdhrFloat getObjectInfoFloat(Object owner, String attributeName) {
    String name = getName( attributeName);
    int id = nameToId( owner, name);
    if ( id == -1)
      return new CdhrFloat( 0F, 2);
    return new CdhrFloat( ((LocalSub)subscriptions.elementAt(id)).valueFloat, 1);
  }

  public CdhrString getObjectInfoString(Object owner, String attributeName) {
    String name = getName( attributeName);
    int id = nameToId( owner, name);
    if ( id == -1)
      return new CdhrString( "", 2);
    return new CdhrString( ((LocalSub)subscriptions.elementAt(id)).valueString, 1);
  }

  public PwrtStatus setObjectInfo( Object owner, String attributeName, boolean value) {
    String name = getName( attributeName);
    int id = nameToId( owner, name);
    if ( id == -1)
      return new PwrtStatus(2);
    ((LocalSub)subscriptions.elementAt(id)).valueBoolean = value;
    return new PwrtStatus(1);
  }

  public PwrtStatus toggleObjectInfo( Object owner, String attributeName) {
    String name = getName( attributeName);
    int id = nameToId( owner, name);
    if ( id == -1)
      return new PwrtStatus(2);
    ((LocalSub)subscriptions.elementAt(id)).valueBoolean = 
	!((LocalSub)subscriptions.elementAt(id)).valueBoolean;
    return new PwrtStatus(1);
  }

  public PwrtStatus setObjectInfo( Object owner, String attributeName, int value) {
    String name = getName( attributeName);
    int id = nameToId( owner, name);
    if ( id == -1)
      return new PwrtStatus(2);
    ((LocalSub)subscriptions.elementAt(id)).valueInt = value;
    return new PwrtStatus(1);
  }

  public PwrtStatus setObjectInfo( Object owner, String attributeName, float value) {
    String name = getName( attributeName);
    int id = nameToId( owner, name);
    if ( id == -1)
      return new PwrtStatus(2);
    ((LocalSub)subscriptions.elementAt(id)).valueFloat = value;
    return new PwrtStatus(1);
  }

  public PwrtStatus setObjectInfo( Object owner, String attributeName, String value) {
    String name = getName( attributeName);
    int id = nameToId( owner, name);
    if ( id == -1)
      return new PwrtStatus(2);
    ((LocalSub)subscriptions.elementAt(id)).valueString = value;
    return new PwrtStatus(1);
  }

  private int nameToId( Object owner, String name) {
    for ( int i = 0; i < subscriptions.size(); i++) {
      LocalSub sub = (LocalSub)subscriptions.elementAt(i);
      if ( sub != null && owner == sub.owner && name.equalsIgnoreCase( sub.name))
	return i;
    }
    return -1;
  }

  private String getName(String attrName) {
    if ( attrName.startsWith("$local."))
      attrName = attrName.substring(7);
    int idx1 = attrName.indexOf("##");
    if ( idx1 != -1)
      return attrName.substring(0, idx1);
    return attrName;
  }

  private int getTypeId(String attrName) {
    String suffix;
    int idx1 = attrName.indexOf("##");
    if(idx1 < 0)
    {
      return Pwr.eType_Boolean;
    }

    idx1 += 2;
    int idx2 = attrName.indexOf('#', idx1);
    if(idx2 < 0)
    {
      suffix = attrName.substring(idx1).toUpperCase();
    }
    else
    {
      suffix = attrName.substring(idx1, idx2).toUpperCase();
    }
    if(suffix.compareTo("BOOLEAN") == 0)
    {
      return Pwr.eType_Boolean;
    }
    if(suffix.compareTo("FLOAT32") == 0)
    {
      return Pwr.eType_Float32;
    }
    if(suffix.compareTo("INT32") == 0)
    {
      return Pwr.eType_Int32;
    }
    if(suffix.compareTo("UINT32") == 0)
    {
      return Pwr.eType_UInt32;
    }
    if(suffix.compareTo("INT16") == 0)
    {
      return Pwr.eType_Int16;
    }
    if(suffix.compareTo("UINT16") == 0)
    {
      return Pwr.eType_UInt16;
    }
    if(suffix.compareTo("INT8") == 0)
    {
      return Pwr.eType_Int8;
    }
    if(suffix.compareTo("UINT8") == 0)
    {
      return Pwr.eType_UInt8;
    }
    if(suffix.compareTo("CHAR") == 0)
    {
      return Pwr.eType_Char;
    }
    if(suffix.compareTo("FLOAT64") == 0)
    {
      return Pwr.eType_Float64;
    }
    if(suffix.compareTo("OBJID") == 0)
    {
      return Pwr.eType_Objid;
    }
    if(suffix.compareTo("STRING") == 0)
    {
      return Pwr.eType_String;
    }
    if(suffix.compareTo("TIME") == 0)
    {
      return Pwr.eType_Time;
    }
    if(suffix.compareTo("DELTATIME") == 0)
    {
      return Pwr.eType_DeltaTime;
    }
    if(suffix.compareTo("ATTRREF") == 0)
    {
      return Pwr.eType_AttrRef;
    }
    if(suffix.substring(0, 6).compareTo("STRING") == 0)
    {
      return Pwr.eType_String;
    }
    return 0;
  }

};
