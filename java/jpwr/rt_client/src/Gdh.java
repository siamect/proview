package jpwr.rt;
import java.io.*;
import java.net.*;
import java.util.*;
import java.awt.*;
import javax.swing.*;

public class Gdh
{
  public final static int SET_OBJECT_INFO_BOOLEAN = 1;
  public final static int SET_OBJECT_INFO_FLOAT = 2;
  public final static int SET_OBJECT_INFO_INT = 3;
  public final static int SET_OBJECT_INFO_STRING = 4;
  public final static int GET_OBJECT_INFO_BOOLEAN = 5;
  public final static int GET_OBJECT_INFO_FLOAT = 6;
  public final static int GET_OBJECT_INFO_INT = 7;
  public final static int GET_OBJECT_INFO_STRING = 8;
  public final static int TOGGLE_OBJECT_INFO = 9;
  public final static int REF_OBJECT_INFO = 10;
  public final static int GET_OBJECT_REF_INFO_BOOLEAN = 11;
  public final static int GET_OBJECT_REF_INFO_FLOAT = 12;
  public final static int GET_OBJECT_REF_INFO_INT = 13;
  public final static int GET_OBJECT_REF_INFO_STRING = 14;
  public final static int UNREF_OBJECT_INFO = 15;
  public final static int NAME_TO_OBJID = 16;
  public final static int OBJID_TO_NAME = 17;
  public final static int GET_ROOT_LIST = 18;
  public final static int GET_NEXT_OBJECT = 19;
  public final static int GET_CHILD = 20;
  public final static int GET_NEXT_SIBLING = 21;
  public final static int GET_OBJECT_CLASS = 22;
  public final static int GET_CLASS_LIST = 23;
  public final static int CLASS_ID_TO_OBJID = 24;
  public final static int GET_OBJECT_REF_INFO_ALL = 25;
  public final static int REF_OBJECT_INFO_LIST = 26;
  public final static int POLL = 27;
  public final static int STATISTICS = 28;
  public final static int CHECK_USER = 29;
  public final static int GET_NODE_OBJECT = 30;
  public final static int LOG_STRING = 31;
  public final static int UNREF_OBJECT_INFO_ALL = 32;
  public final static int CREATE_INSTANCE_FILE = 33;
  public final static int GET_ATTRIBUTE_CHAR = 34;
  public final static int GET_CLASS_ATTRIBUTE = 35;
  public final static int GET_ALL_CLASS_ATTRIBUTES = 36;
  public final static int GET_ALL_SIBLINGS = 37;
  public final static int GET_ALL_XTT_SIBLINGS = 38;
  public final static int GET_ALL_XTT_CHILDREN = 39;
  public final static int REF_OBJECT_INFO_VECTOR = 40;
  public final static int CRR_SIGNAL = 42;
  public final static int CRR_OBJECT = 43;
  public final static int GET_PARENT = 44;
  public final static int GET_OBJECT_INFO_OBJID = 45;

  public final static int GET_OBJECT_REF_INFO_BOOLEAN_ARRAY = 46;
  public final static int GET_OBJECT_REF_INFO_FLOAT_ARRAY = 47;
  public final static int GET_OBJECT_REF_INFO_INT_ARRAY = 48;
  public final static int GET_OBJECT_REF_INFO_STRING_ARRAY = 49;



  public final static int __IO_EXCEPTION = 2000;
  public final static int __BUSY = 2002;
  public final static int __UNREFED = 0;

  private static String currentSystemGroup = null;
  private static String currentUser = null;
  private static String currentPassword = null;
  private static int currentPrivilege = Pwr.mPrv_RtRead;

  Socket gdhSocket;
  ObjectOutputStream out;
  ObjectInputStream in;
  Vector subscriptions = new Vector();
  int subscriptionCount = 0;
  boolean listSent = false;
  int subLate = 0;

  private static boolean trace = false;
  private boolean locked = false;


  public Gdh(Object root)
  {
    init(root);
  }


  private void init(Object root)
  {
    try
    {
      URL url = ((JApplet)root).getCodeBase();
      if(trace)
      {
        System.out.println("Opening socket to " + url.getHost());
      }
      gdhSocket = new Socket(url.getHost(), 4445);
      out = new ObjectOutputStream(new BufferedOutputStream(gdhSocket.getOutputStream()));
      in = new ObjectInputStream(new BufferedInputStream(gdhSocket.getInputStream()));
      
      
      poll();
    }
    catch(UnknownHostException e)
    {
      System.err.println("Don't know about host: taranis.");
      System.exit(1);
    }
    catch(IOException e)
    {
      System.err.println("Couldn't get I/O for the connection");
      System.exit(1);
    }
  }


  public void close()
  {
    try
    {
      if(trace)
      {
        System.out.println("Closing socket");
        System.out.println("Locked = " + locked);
      }
      out.close();
      in.close();
      gdhSocket.close();
    }
    catch(IOException e)
    {
      System.err.println("Couldn't close I/O connection");
    }
  }


  private void poll()
  {
    try
    {
      if(trace)
      {
        System.out.println("Polling");
      }
      out.writeInt(POLL);
      out.flush();
      int sts = in.readInt();
      if(sts != POLL)
      {
        System.out.println("Poll error");
      }
    }
    catch(IOException e)
    {
      System.err.println("IOException in poll");
    }
  }


  public PwrtStatus setObjectInfo(String attributeName, boolean value)
  {
    try
    {
      out.writeInt(SET_OBJECT_INFO_BOOLEAN);
      out.writeUTF(attributeName);
      out.writeBoolean(value);
      out.flush();
      int sts = in.readInt();
      return new PwrtStatus(sts);
    }
    catch(IOException e)
    {
      return new PwrtStatus(__IO_EXCEPTION);
    }
  }


  public PwrtStatus setObjectInfo(String attributeName, int value)
  {
    try
    {
      out.writeInt(SET_OBJECT_INFO_INT);
      out.writeUTF(attributeName);
      out.writeInt(value);
      out.flush();
      int sts = in.readInt();
      return new PwrtStatus(sts);
    }
    catch(IOException e)
    {
      return new PwrtStatus(__IO_EXCEPTION);
    }
  }


  public PwrtStatus setObjectInfo(String attributeName, float value)
  {
    try
    {
      out.writeInt(SET_OBJECT_INFO_FLOAT);
      out.writeUTF(attributeName);
      out.writeFloat(value);
      out.flush();
      int sts = in.readInt();
      return new PwrtStatus(sts);
    }
    catch(IOException e)
    {
      return new PwrtStatus(__IO_EXCEPTION);
    }
  }


  public PwrtStatus setObjectInfo(String attributeName, String value)
  {
    try
    {
      out.writeInt(SET_OBJECT_INFO_STRING);
      out.writeUTF(attributeName);
      out.writeUTF(value);
      out.flush();
      int sts = in.readInt();
      return new PwrtStatus(sts);
    }
    catch(IOException e)
    {
      return new PwrtStatus(__IO_EXCEPTION);
    }
  }


  public CdhrBoolean getObjectInfoBoolean(String attributeName)
  {
    try
    {
      out.writeInt(GET_OBJECT_INFO_BOOLEAN);
      out.writeUTF(attributeName);
      out.flush();
      int sts = in.readInt();
      if(sts % 2 == 0)
      {
        return new CdhrBoolean(false, sts);
      }

      boolean value = in.readBoolean();
      return new CdhrBoolean(value, sts);
    }
    catch(IOException e)
    {
      return new CdhrBoolean(false, __IO_EXCEPTION);
    }
  }


  public CdhrInt getObjectInfoInt(String attributeName)
  {
    try
    {
      out.writeInt(GET_OBJECT_INFO_INT);
      out.writeUTF(attributeName);
      out.flush();
      int sts = in.readInt();
      if(sts % 2 == 0)
      {
        return new CdhrInt(0, sts);
      }

      int value = in.readInt();
      return new CdhrInt(value, sts);
    }
    catch(IOException e)
    {
      return new CdhrInt(0, __IO_EXCEPTION);
    }
  }


  public CdhrFloat getObjectInfoFloat(String attributeName)
  {
    try
    {
      out.writeInt(GET_OBJECT_INFO_FLOAT);
      out.writeUTF(attributeName);
      out.flush();
      int sts = in.readInt();
      if(sts % 2 == 0)
      {
        return new CdhrFloat(0.0F, sts);
      }

      float value = in.readFloat();
      return new CdhrFloat(value, sts);
    }
    catch(IOException e)
    {
      return new CdhrFloat(0.0F, __IO_EXCEPTION);
    }
  }


  public CdhrString getObjectInfoString(String attributeName)
  {
    try
    {
      out.writeInt(GET_OBJECT_INFO_STRING);
      out.writeUTF(attributeName);
      out.flush();
      int sts = in.readInt();
      if(sts % 2 == 0)
      {
        return new CdhrString("", sts);
      }

      String value = in.readUTF();
      return new CdhrString(value, sts);
    }
    catch(IOException e)
    {
      return new CdhrString("", __IO_EXCEPTION);
    }
  }

  public CdhrObjid getObjectInfoObjid(String objectName)
  {
    try
    {
      out.writeInt(GET_OBJECT_INFO_OBJID);
      out.writeUTF(objectName);
      out.flush();
      int sts = in.readInt();
      if(sts % 2 == 0) {
        return new CdhrObjid(null, sts);
      }

      int oix = in.readInt();
      int vid = in.readInt();
      PwrtObjid objid = new PwrtObjid(oix, vid);
      return new CdhrObjid(objid, sts);
    }
    catch(IOException e)
    {
      return new CdhrObjid(null, __IO_EXCEPTION);
    }
  }


  public PwrtStatus toggleObjectInfo(String attributeName)
  {
    try
    {
      out.writeInt(TOGGLE_OBJECT_INFO);
      out.writeUTF(attributeName);
      out.flush();
      int sts = in.readInt();
      return new PwrtStatus(sts);
    }
    catch(IOException e)
    {
      return new PwrtStatus(__IO_EXCEPTION);
    }
  }


  public GdhrRefObjectInfo refObjectInfo(String attributeName)
  {
    if(!listSent)
    {
      PwrtRefId refid = new PwrtRefId(subscriptionCount, 0);
      int id = subscriptionCount;
      int typeId = getTypeId(attributeName);
      if(typeId == 0)
      {
        return new GdhrRefObjectInfo(null, 0, 0, 0);
      }
      //qqq
      Sub sub = new Sub(attributeName, null, 0, typeId, subscriptionCount, 0, 0);
      subscriptions.insertElementAt(sub, subscriptionCount);
      subscriptionCount++;
      return new GdhrRefObjectInfo(refid, id, 1, typeId);
    }
    else
    {
      try
      {
        if(trace)
        {
          System.out.println("RefObjectInfo " + attributeName);
        }

        out.writeInt(REF_OBJECT_INFO);
        out.writeUTF(attributeName);
        out.flush();
        int sts = in.readInt();
        if(sts % 2 == 0)
        {
          return new GdhrRefObjectInfo(null, 0, sts, 0);
        }

        int rix = in.readInt();
        int nid = in.readInt();
        PwrtRefId refid = new PwrtRefId(rix, nid);
        int id = in.readInt();
        int typeId = in.readInt();
	int size = in.readInt();
	int elements = in.readInt();
        Sub sub = new Sub(attributeName, refid, id, typeId,subscriptionCount, elements, size);
        sub.sts = 1;
        subscriptions.insertElementAt(sub, id);
        subscriptionCount++;
        refid = new PwrtRefId(id, 0);
        subLate++;
        return new GdhrRefObjectInfo(refid, id, sts, typeId, size, elements);
      }
      catch(IOException e)
      {
        return new GdhrRefObjectInfo(null, 0, __IO_EXCEPTION, 0);
      }
    }
  }


  public synchronized void refObjectInfoList()
  {
    Sub sub;
    int i;
    int sts;
    int rix;
    int nid;
    int id;
    int typeId;

    PwrtRefId refid;

    if(listSent)
    {
      return;
    }

    try
    {
      if(trace)
      {
        System.out.println("RefObjectInfoList");
      }
      try
      {
        while(locked)
        {
          System.out.println("refObjectInfoList Väntar på locked = false");
          wait();
        }
      }
      catch(InterruptedException e)
      {
        System.out.println("Wait avbruten");
      }
      locked = true;
      out.writeInt(REF_OBJECT_INFO_LIST);
      int size = subscriptions.size();
      out.writeInt(size);
      //out.flush();
      for(i = 0; i < size; i++)
      {
        sub = (Sub)subscriptions.elementAt(i);
        out.writeUTF(sub.attrName);
//      out.flush();
      }
      out.flush();
      for(i = 0; i < size; i++)
      {
        sub = (Sub)subscriptions.elementAt(i);
        sts = in.readInt();
        sub.sts = sts;
        if(sts % 2 == 0)
        {
          continue;
        }

        rix = in.readInt();
        nid = in.readInt();
        sub.refid = new PwrtRefId(rix, nid);
        id = in.readInt();
        typeId = in.readInt();
	sub.size = in.readInt();
	sub.elements = in.readInt();
      }
      locked = false;
      notify();
      if(trace)
      {
        System.out.println("RefObjectInfoList end");
      }
      listSent = true;
    }
    catch(IOException e)
    {
      System.out.println("GdhRefObjectInfoList IOException");
      locked = false;
      notify();
    }
  }


  public synchronized Vector refObjectInfo_Vector(Vector vec)
  {
    try
    {
      while(locked)
      {
        System.out.println("refObjectInfo_Vector Väntar på locked = false");
        wait();
      }
    }
    catch(InterruptedException e)
    {
      System.out.println("Wait avbruten");
    }

    locked = true;

    try
    {

      if(trace)
      {
        for(int i = 0; i < vec.size(); i++)
        {
          System.out.println("RefObjectInfo " + (String)vec.get(i));
        }
      }
      //just to scip the list-code( for applets that do not use the xxx_Vector method)
      listSent = true;
      out.writeInt(REF_OBJECT_INFO_VECTOR);
      out.writeObject(vec);
      out.flush();

      Vector retVec = (Vector)in.readObject();
      if(retVec == null)
      {
        locked = false;
        notify();
        return null;
      }
      for(int i = 0;i < retVec.size();i++)
      {
        Sub ret = ((Sub)retVec.get(i));
	if(ret == null || ret.evenSts())
	{
	  System.out.println("Fel vid prenumerationsinitiering");
	  continue;
	}
        ret.id = ret.refid.rix;
	subscriptions.add(ret);

      }
	
      subscriptionCount += retVec.size();
      locked = false;
      notify();
      return retVec;
    }
    catch(IOException e)
    {
      System.out.println("REF_OBJECT_INFO_VECTOR: IO_Exception");
      locked = false;
      notify();
      Vector retVec = new Vector();
      retVec.add(new GdhrRefObjectInfo(null, 0, __IO_EXCEPTION, 0));
      return retVec;
    }
    catch(ClassNotFoundException e)
    {
      locked = false;
      notify();
      System.out.println("REF_OBJECT_INFO_VECTOR: ClassNotFoundException");
      return null;
    }
    catch(Exception e)
    {
      System.out.println("Exception vid REF_OBJECT_INFO_VECTOR");
      locked = false;
      notify();
      return null;
    }

  }


  public float getObjectRefInfoFloat(int id)
  {
  try{
    Sub sub = (Sub)subscriptions.elementAt(id);
    if(sub.oddSts())
    {
      return sub.valueFloat;
    }
    return 0F;
    }
        catch(ArrayIndexOutOfBoundsException e)
    {
      //System.out.println("ArrayIndexOutOfBounds getObjectRefInfoFloat");
      return 0F;
      }
  }

  public float[] getObjectRefInfoFloatArray(int id, int elements)
  {
    try{
      Sub sub = (Sub)subscriptions.elementAt(id);
      if(sub.oddSts())
      {
        return sub.valueFloatArray;
      }
      return null;
    }
    catch(ArrayIndexOutOfBoundsException e)
    {
      //System.out.println("ArrayIndexOutOfBounds getObjectRefInfoFloat");
      return null;
    }
  }

  public boolean getObjectRefInfoBoolean(int id)
  {
  try
  {
    Sub sub = (Sub)subscriptions.elementAt(id);
    if(sub.oddSts())
    {
      return sub.valueBoolean;
    }
    return false;
   }
        catch(ArrayIndexOutOfBoundsException e)
    {
      //System.out.println("ArrayIndexOutOfBounds getObjectRefInfoBoolean");
      return false;
      }
  }

  public boolean[] getObjectRefInfoBooleanArray(int id, int elements)
  {
    try
    {
      Sub sub = (Sub)subscriptions.elementAt(id);
      if(sub.oddSts())
      {
        return sub.valueBooleanArray;
      }
      return null;
    }
    catch(ArrayIndexOutOfBoundsException e)
    {
      //System.out.println("ArrayIndexOutOfBounds getObjectRefInfoBoolean");
      return null;
    }
  }

  public int getObjectRefInfoInt(int id)
  {
    try
    {
      Sub sub = (Sub)subscriptions.elementAt(id);
      if(sub.oddSts())
      {
        return sub.valueInt;
      }
      return 0;
    }
    catch(ArrayIndexOutOfBoundsException e)
    {
      //System.out.println("ArrayIndexOutOfBounds getObjectRefInfoInt");
      return 0;
    }
  }

  public int[] getObjectRefInfoIntArray(int id, int elements)
  {
    try
    {
      Sub sub = (Sub)subscriptions.elementAt(id);
      if(sub.oddSts())
      {
        return sub.valueIntArray;
      }
      return null;
    }
    catch(ArrayIndexOutOfBoundsException e)
    {
      //System.out.println("ArrayIndexOutOfBounds getObjectRefInfoInt");
      return null;
    }
  }

  public String getObjectRefInfoString(int id, int typeid)
  {
    try
    {
    Sub sub = (Sub)subscriptions.elementAt(id);
    if(sub.evenSts() || sub.valueString == null)
    {
      if(sub.evenSts())
        System.out.println("getObjectRefInfoString substs " + sub.getSts() +  " id " + id);
      return "";
    }
    return sub.valueString;
    }
    catch(ArrayIndexOutOfBoundsException e)
    {
      //System.out.println("ArrayIndexOutOfBounds getObjectRefInfoString index " + id);
      return " ";
      }
  }

  public String[] getObjectRefInfoStringArray(int id, int typeid, int size, int elements)
  {
    try
    {
    Sub sub = (Sub)subscriptions.elementAt(id);
    if(sub.evenSts() || sub.valueStringArray == null)
    {
      if(sub.evenSts())
        System.out.println("getObjectRefInfoString substs " + sub.getSts() +  " id " + id);
      return null;
    }
    return sub.valueStringArray;
    }
    catch(ArrayIndexOutOfBoundsException e)
    {
      //System.out.println("ArrayIndexOutOfBounds getObjectRefInfoString index " + id);
      return null;
    }
  }




  public synchronized PwrtStatus unrefObjectInfo(PwrtRefId refid)
  {
    try
    {
      while(locked)
      {
        System.out.println("unrefObjectInfo Väntar på locked = false");
        wait();
        System.out.println("unrefObjectInfo Väntat klart");
      }

    }
    catch(InterruptedException e)
    {
      System.out.println("Wait avbruten");
    }
    //    if ( locked)
//      return new PwrtStatus(__BUSY);
    locked = true;
    if(refid.rix >= subscriptions.size())
    {
      //System.out.println("Försöker göra unrefObjectInfo med refid.rix= " +
      //                    refid.rix + " men sub.size= " + subscriptions.size() );
      locked = false;
      notify();
      return new PwrtStatus(__UNREFED);
    }
    Sub sub = (Sub)subscriptions.elementAt(refid.rix);
    if(sub.sts == __UNREFED)
    {
      locked = false;
      notify();
      return new PwrtStatus(__UNREFED);
    }
    sub.sts = __UNREFED;
    try
    {
      if(trace)
      {
        System.out.println("UnRefObjectInfo " + refid);
      }

      out.writeInt(UNREF_OBJECT_INFO);
      out.writeInt(refid.rix);
      out.writeInt(refid.nid);
      out.flush();
      int sts = in.readInt();
      locked = false;
      notify();
      return new PwrtStatus(sts);
    }
    catch(IOException e)
    {
      System.out.println("IOException vid unrefObjectInfo");
      locked = false;
      notify();
      return new PwrtStatus(__IO_EXCEPTION);
    }
  }


  public synchronized Vector unrefObjectInfo_Vector(Vector unref_vec)
  {
    try
    {
      while(locked)
      {
        System.out.println("unrefobjectinfo_vector Väntar på locked = false");
        wait();
      }
    }
    catch(InterruptedException e)
    {
      System.out.println("Wait avbruten");
    }
    locked = true;
    try
    {
      int i;
      //System.out.println("unrefObjectInfo_Vector");
      for(i = 0; i < unref_vec.size(); i++)
      {
        PwrtRefId refid = (PwrtRefId)unref_vec.get(i);
        Sub sub = (Sub)subscriptions.elementAt(refid.rix);
        //if(sub.sts == __UNREFED)
        //{
        //  continue;
        //}

        sub.sts = __UNREFED;
        if(trace)
        {
          System.out.println("UnRefObjectInfo_vector " + refid);
        }

        out.writeInt(UNREF_OBJECT_INFO);
        out.writeInt(refid.rix);
        out.writeInt(refid.nid);
      }
      out.flush();
      Vector ret_vec = new Vector();
      //System.out.println("unrefObjectInfo_VEctor i: " + i);
      while(i > 0)
      {
        int sts = in.readInt();
        ret_vec.add(new PwrtStatus(sts));
        i--;
      }
      locked = false;
      notify();
      return ret_vec;
    }
    catch(IOException e)
    {
      System.out.println("IOException vid unrefObjectInfo_Vector");
      locked = false;
      notify();
      return null;
    }
  }



  public synchronized PwrtStatus unrefObjectInfoAll()
  {
    Sub sub;
    //if ( locked)
    //  return new PwrtStatus(__BUSY);
    try
    {
      while(locked)
      {
        System.out.println("unrefObjectInfoAll Väntar på locked = false");
        wait();
      }
    }
    catch(InterruptedException e)
    {
      System.out.println("Wait avbruten");
    }
    locked = true;
    subscriptions.removeAllElements();
    subscriptionCount = 0;
    //for(int i = 0; i < subscriptionCount; i++)
    //{

    //sub = (Sub)subscriptions.elementAt(i);
    //sub.sts = __UNREFED;
    //}
    try
    {
      if(trace)
      {
        System.out.println("UnRefObjectInfoAll");
      }

      out.writeInt(UNREF_OBJECT_INFO_ALL);
      out.flush();
      int sts = in.readInt();
      locked = false;
      notify();
      return new PwrtStatus(sts);
    }
    catch(IOException e)
    {
      locked = false;
      notify();
      return new PwrtStatus(__IO_EXCEPTION);
    }
  }


  public CdhrObjid nameToObjid(String objectName)
  {
    try
    {
      out.writeInt(NAME_TO_OBJID);
      out.writeUTF(objectName);
      out.flush();
      int sts = in.readInt();
      if(sts % 2 == 0)
      {
        return new CdhrObjid(null, sts);
      }

      int oix = in.readInt();
      int vid = in.readInt();
      PwrtObjid objid = new PwrtObjid(oix, vid);
      return new CdhrObjid(objid, sts);
    }
    catch(IOException e)
    {
      return new CdhrObjid(null, __IO_EXCEPTION);
    }
  }


  public CdhrString objidToName(PwrtObjid objid, int nameType)
  {
    try
    {
      out.writeInt(OBJID_TO_NAME);
      out.writeInt(objid.oix);
      out.writeInt(objid.vid);
      out.writeInt(nameType);
      out.flush();
      int sts = in.readInt();
      if(sts % 2 == 0)
      {
        return new CdhrString(null, sts);
      }

      String name = in.readUTF();
      return new CdhrString(name, sts);
    }
    catch(IOException e)
    {
      return new CdhrString("", __IO_EXCEPTION);
    }
  }


  public CdhrObjid getRootList()
  {
    try
    {
      out.writeInt(GET_ROOT_LIST);
      out.flush();
      int sts = in.readInt();
      if(sts % 2 == 0)
      {
        return new CdhrObjid(null, sts);
      }

      int oix = in.readInt();
      int vid = in.readInt();
      PwrtObjid objid = new PwrtObjid(oix, vid);
      return new CdhrObjid(objid, sts);
    }
    catch(IOException e)
    {
      return new CdhrObjid(null, __IO_EXCEPTION);
    }
  }


  public CdhrObjid getNextObject(PwrtObjid objid)
  {
    try
    {
      out.writeInt(GET_NEXT_OBJECT);
      out.writeInt(objid.oix);
      out.writeInt(objid.vid);
      out.flush();
      int sts = in.readInt();
      if(sts % 2 == 0)
      {
        return new CdhrObjid(null, sts);
      }

      int oix = in.readInt();
      int vid = in.readInt();
      PwrtObjid next = new PwrtObjid(oix, vid);
      return new CdhrObjid(next, sts);
    }
    catch(IOException e)
    {
      return new CdhrObjid(null, __IO_EXCEPTION);
    }
  }


  public CdhrObjid getChild(PwrtObjid objid)
  {
    try
    {
      out.writeInt(GET_CHILD);
      out.writeInt(objid.oix);
      out.writeInt(objid.vid);
      out.flush();
      int sts = in.readInt();
      if(sts % 2 == 0)
      {
        return new CdhrObjid(null, sts);
      }

      int oix = in.readInt();
      int vid = in.readInt();
      PwrtObjid child = new PwrtObjid(oix, vid);
      return new CdhrObjid(child, sts);
    }
    catch(IOException e)
    {
      return new CdhrObjid(null, __IO_EXCEPTION);
    }
  }

  public CdhrObjid getParent(PwrtObjid objid)
  {
    try
    {
      out.writeInt(GET_PARENT);
      out.writeInt(objid.oix);
      out.writeInt(objid.vid);
      out.flush();
      int sts = in.readInt();
      if(sts % 2 == 0)
      {
        return new CdhrObjid(null, sts);
      }

      int oix = in.readInt();
      int vid = in.readInt();
      PwrtObjid child = new PwrtObjid(oix, vid);
      return new CdhrObjid(child, sts);
    }
    catch(IOException e)
    {
      return new CdhrObjid(null, __IO_EXCEPTION);
    }
  }


  public CdhrObjid getNextSibling(PwrtObjid objid)
  {
    try
    {
      out.writeInt(GET_NEXT_SIBLING);
      out.writeInt(objid.oix);
      out.writeInt(objid.vid);
      out.flush();
      int sts = in.readInt();
      if(sts % 2 == 0)
      {
        return new CdhrObjid(null, sts);
      }

      int oix = in.readInt();
      int vid = in.readInt();
      PwrtObjid next = new PwrtObjid(oix, vid);
      return new CdhrObjid(next, sts);
    }
    catch(IOException e)
    {
      return new CdhrObjid(null, __IO_EXCEPTION);
    }
  }


  public CdhrClassId getObjectClass(PwrtObjid objid)
  {
    try
    {
      out.writeInt(GET_OBJECT_CLASS);
      out.writeInt(objid.oix);
      out.writeInt(objid.vid);
      out.flush();
      int sts = in.readInt();
      if(sts % 2 == 0)
      {
        return new CdhrClassId(0, sts);
      }

      int classId = in.readInt();
      return new CdhrClassId(classId, sts);
    }
    catch(IOException e)
    {
      return new CdhrClassId(0, __IO_EXCEPTION);
    }
  }


  public CdhrObjid getClassList(int classid)
  {
    try
    {
      out.writeInt(GET_CLASS_LIST);
      out.writeInt(classid);
      out.flush();
      int sts = in.readInt();
      if(sts % 2 == 0)
      {
        return new CdhrObjid(null, sts);
      }

      int oix = in.readInt();
      int vid = in.readInt();
      PwrtObjid objid = new PwrtObjid(oix, vid);
      return new CdhrObjid(objid, sts);
    }
    catch(IOException e)
    {
      return new CdhrObjid(null, __IO_EXCEPTION);
    }
  }


  public CdhrObjid classIdToObjid(int classid)
  {
    try
    {
      out.writeInt(CLASS_ID_TO_OBJID);
      out.writeInt(classid);
      out.flush();
      int sts = in.readInt();
      if(sts % 2 == 0)
      {
        return new CdhrObjid(null, sts);
      }

      int oix = in.readInt();
      int vid = in.readInt();
      PwrtObjid objid = new PwrtObjid(oix, vid);
      return new CdhrObjid(objid, sts);
    }
    catch(IOException e)
    {
      return new CdhrObjid(null, __IO_EXCEPTION);
    }
  }


  public PwrtStatus createInstanceFile(String from, String to,
      String instance)
  {
    Sub sub;
    if(locked)
    {
      return new PwrtStatus(__BUSY);
    }
    try
    {
      out.writeInt(CREATE_INSTANCE_FILE);
      out.writeUTF(from);
      out.writeUTF(to);
      out.writeUTF(instance);
      out.flush();
      int sts = in.readInt();
      return new PwrtStatus(sts);
    }
    catch(IOException e)
    {
      return new PwrtStatus(__IO_EXCEPTION);
    }
  }


  public boolean isBusy()
  {
    return locked;
  }


  public int login(String user, String password)
  {
    // Fetch system group
    String systemGroup = "SSAB";
    try
    {
      out.writeInt(CHECK_USER);
      out.writeUTF(systemGroup);
      out.writeUTF(user);
      out.writeUTF(password);
      out.flush();
      int sts = in.readInt();

      if(sts % 2 == 0)
      {
        logout();
        return sts;
      }
      int value = in.readInt();
      currentSystemGroup = systemGroup;
      currentUser = user;
      currentPassword = password;
      currentPrivilege = value;
      return sts;
    }
    catch(IOException e)
    {
      return __IO_EXCEPTION;
    }
  }


  public void logout()
  {
    currentSystemGroup = null;
    currentUser = null;
    currentPassword = null;
    currentPrivilege = Pwr.mPrv_RtRead;
  }


  public int checkUser()
  {
    logString("ThreadGroup: " + Runtime.getRuntime() + " " + currentUser);
    if(currentUser == null)
    {
      return 0;
    }
    return login(currentUser, currentPassword);
  }


  public String getUser()
  {
    return currentUser;
  }


  public boolean isAuthorized(int access)
  {
    return (access & currentPrivilege) != 0;
  }

  public synchronized void getObjectRefInfoAll()
  {
    if(subscriptionCount == 0)
    {
      return;
    }
    try
    {
      while(locked)
      {
        System.out.println("getObjectRefInfoAll Väntar på locked = false");
        wait();
      }
    }
    catch(InterruptedException e)
    {
      System.out.println("Wait avbruten");
    }
    try
    {
      locked = true;
      out.writeInt(GET_OBJECT_REF_INFO_ALL);
      out.flush();
      
      for(int i = 0; i < subscriptions.size();i++)
      {
	  //System.out.println("getObjectRefInfoAll:" + ((jpwr.rt.Sub)(subscriptions.get(i))).elements);
	try
	{
	  if(((jpwr.rt.Sub)(subscriptions.get(i))).elements > 1)
	  {
            ((jpwr.rt.Sub)(subscriptions.get(i))).setValue(in.readObject());
	  }
          else
	  {
            switch (((jpwr.rt.Sub)(subscriptions.get(i))).typeId)
            {
              case Pwr.eType_Int32:
              case Pwr.eType_UInt32:
              case Pwr.eType_Int16:
              case Pwr.eType_UInt16:
              case Pwr.eType_Int8:
              case Pwr.eType_UInt8:
                ((jpwr.rt.Sub)(subscriptions.get(i))).setValue(in.readInt());
	      break;
              case Pwr.eType_Float32:
                ((jpwr.rt.Sub)(subscriptions.get(i))).setValue(in.readFloat());
              break;
              case 0:
              case Pwr.eType_Boolean:
                ((jpwr.rt.Sub)(subscriptions.get(i))).setValue(in.readBoolean());
              break;
              default:
                ((jpwr.rt.Sub)(subscriptions.get(i))).setValue(in.readObject());
              break;
            }
	  }
	}
	catch(Exception e)
	{
	  System.out.println("Fel vid mottagande i getObjectRefInfoAll " + e.toString());
	}
	
      }

      locked = false;
      notify();
    }
/*
    catch(ClassNotFoundException e)
    {
      locked = false;
      notify();
      System.out.println("Server returnerar fel vid getObjectRefInfoAll");
      return;
    }
*/
    catch(IOException e)
    {
      locked = false;
      notify();
      System.out.println("IOException vid getObjectRefInfoAll");
      return;
    }
  }


  public CdhrObjid getNodeObject()
  {
    try
    {
      out.writeInt(GET_NODE_OBJECT);
      out.flush();
      int sts = in.readInt();
      if(sts % 2 == 0)
      {
        return new CdhrObjid(null, sts);
      }

      int oix = in.readInt();
      int vid = in.readInt();
      PwrtObjid objid = new PwrtObjid(oix, vid);
      return new CdhrObjid(objid, sts);
    }
    catch(IOException e)
    {
      return new CdhrObjid(null, __IO_EXCEPTION);
    }
  }


  public GdhrGetAttributeChar getAttributeChar(String attributeName)
  {
    try
    {
      out.writeInt(GET_ATTRIBUTE_CHAR);
      out.writeUTF(attributeName);
      out.flush();
      int sts = in.readInt();
      if(sts % 2 == 0)
      {
        return new GdhrGetAttributeChar(0, 0, 0, 0, sts);
      }

      int typeId = in.readInt();
      int size = in.readInt();
      int offset = in.readInt();
      int elements = in.readInt();
      return new GdhrGetAttributeChar(typeId, size, offset, elements, sts);
    }
    catch(IOException e)
    {
      return new GdhrGetAttributeChar(0, 0, 0, 0, __IO_EXCEPTION);
    }
  }


  public CdhrObjAttr getClassAttribute(int classid, PwrtObjid objid_obj)
  {
    try
    {
      out.writeInt(GET_CLASS_ATTRIBUTE);
      out.writeInt(classid);
      out.writeInt(objid_obj.oix);
      out.writeInt(objid_obj.vid);
      out.flush();
      CdhrObjAttr ret_obj = (CdhrObjAttr)in.readObject();
      if(ret_obj == null)
      {
        return (CdhrObjAttr)null;
      }
      return ret_obj;
    }
    catch(IOException e)
    {
      return (CdhrObjAttr)null;
    }
    catch(ClassNotFoundException e)
    {
      return (CdhrObjAttr)null;
    }
  }


  public Vector getAllClassAttributes(int classid, PwrtObjid objid_obj)
  {
    try
    {
      out.writeInt(GET_ALL_CLASS_ATTRIBUTES);
      out.writeInt(classid);
      out.writeInt(objid_obj.oix);
      out.writeInt(objid_obj.vid);
      out.flush();
      Vector ret_obj = (Vector)in.readObject();
      return ret_obj;
    }
    catch(IOException e)
    {
      return (Vector)null;
    }
    catch(ClassNotFoundException e)
    {
      return (Vector)null;
    }
  }


  public Vector getAllSiblings(PwrtObjid objid)
  {
    try
    {
      out.writeInt(GET_ALL_SIBLINGS);
      out.writeObject(objid);
      out.flush();
      Vector ret_obj = (Vector)in.readObject();
      return ret_obj;
    }
    catch(IOException e)
    {
      return (Vector)null;
    }
    catch(ClassNotFoundException e)
    {
      return (Vector)null;
    }
  }


  public Vector getAllXttSiblings(PwrtObjid objid)
  {
    try
    {
      out.writeInt(GET_ALL_XTT_SIBLINGS);
      out.writeObject(objid);
      out.flush();
      Vector ret_obj = (Vector)in.readObject();
      return ret_obj;
    }
    catch(IOException e)
    {
      System.out.println("IOException vid getAllXttSiblings");
      return (Vector)null;
    }
    catch(ClassNotFoundException e)
    {
      System.out.println("ClassNotFoundException vid getAllXttSiblings");
      return (Vector)null;
    }
  }


  public Vector getAllXttChildren(PwrtObjid objid)
  {
    try
    {
      out.writeInt(GET_ALL_XTT_CHILDREN);
      out.writeObject(objid);
      out.flush();
      Vector ret_obj = (Vector)in.readObject();
      return ret_obj;
    }
    catch(IOException e)
    {
      System.out.println("IOException vid getAllXttChildren");
      return (Vector)null;
    }
    catch(ClassNotFoundException e)
    {
      System.out.println("ClassNotFoundException vid getAllXttChildren");
      return (Vector)null;
    }
  }

  public CdhrString crrSignal(String name)
  {
    try
    {
      out.writeInt(CRR_SIGNAL);
      out.writeUTF(name);
      out.flush();
      int sts = in.readInt();
      if(sts % 2 == 0)
      {
        return new CdhrString(null, sts);
      }

      String str = in.readUTF();
      return new CdhrString(str, sts);
    }
    catch(IOException e)
    {
      return new CdhrString("", __IO_EXCEPTION);
    }
  }

  public CdhrString crrObject(String name)
  {
    try
    {
      out.writeInt(CRR_OBJECT);
      out.writeUTF(name);
      out.flush();
      int sts = in.readInt();
      if(sts % 2 == 0)
      {
        return new CdhrString(null, sts);
      }

      String str = in.readUTF();
      return new CdhrString(str, sts);
    }
    catch(IOException e)
    {
      return new CdhrString("", __IO_EXCEPTION);
    }
  }


  public void logString(String str)
  {
    try
    {
      out.writeInt(LOG_STRING);
      out.writeUTF(str);
      out.flush();
    }
    catch(IOException e)
    {
    }
  }


  public void printStatistics(int lockRejected)
  {
    Sub sub;
    try
    {
      out.writeInt(STATISTICS);
      out.writeInt(subscriptionCount);
      out.writeInt(subLate);
      out.writeInt(lockRejected);
      out.flush();
    }
    catch(IOException e)
    {
      return;
    }
  }


  private int getTypeId(String attrName)
  {
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

}







