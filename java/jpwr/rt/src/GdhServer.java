
package jpwr.rt;
import java.net.*;
import java.io.*;
import java.util.*;
//for test
import java.sql.Timestamp;
import java.util.Date;
import javax.swing.*;
//end for test
/**
 *  Description of the Class
 *
 *@author     claes, Jonas
 *@created    November 25, 2002
 *@version    1.2 beta: Test with connecting to msghandler
 */
public class GdhServer
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
  public final static int GET_SUBSCRIPTIONS = 41;
  public final static int CRR_SIGNAL = 42;
  public final static int CRR_OBJECT = 43;
  public final static int GET_PARENT = 44;
  public final static int GET_OBJECT_INFO_OBJID = 45;

  public final static int GET_OBJECT_REF_INFO_BOOLEAN_ARRAY = 46;
  public final static int GET_OBJECT_REF_INFO_FLOAT_ARRAY = 47;
  public final static int GET_OBJECT_REF_INFO_INT_ARRAY = 48;
  public final static int GET_OBJECT_REF_INFO_STRING_ARRAY = 49;

  public final static int PORT = 4445;

  public final static int __IO_EXCEPTION = 2000;
  public final static int __UNREFED = 0;

  static ArrayList subscriptions = new ArrayList();

  static int subscriptionCount = 0;

  static int threadCount = 0;
  static int totalThreadCount = 0;
  Gdh gdh;
  Errh errh;
  int maxConnections;
  String currentConnectionsStr;
  static boolean ignoreHandler = false;
  static boolean log = false;
  static boolean logRefInfoAll = false;
  static boolean logStatistics = false;

  static int lastIndexReffed = 0;

  /**
   *  The main program for the GdhServer class
   *
   *@param  args  The command line arguments
   */
  public static void main(String[] args)
  {
    for(int i = 0; i < args.length; i++)
    {
      if(args[i].equals("-i"))
      {
        ignoreHandler = true;
      }
      else if(args[i].equals("-l"))
      {
        log = true;
      }
      else if(args[i].equals("-logRefInfoAll"))
      {
        logRefInfoAll = true;
      }

      else if(args[i].equals("-s"))
      {
        logStatistics = true;
      }
    }
    if(log)
    {
      System.out.println("GdhServer starting ver 3.8");
    }
    if(logRefInfoAll)
    {
      System.out.println("GdhServer starting ver 3.8 -logRefInfoAll");
    }

    GdhServer gdhServer = new GdhServer();
    gdhServer.openServerSocket();
    System.out.println("GdhServer exiting");
    System.exit(0);
  }


  /**
   *  Description of the Method
   */
  public void GdhServer()
  {
  }


  /**
   *  Gets the handlerObject attribute of the GdhServer object
   *
   *@return    The handlerObject value
   */
  private int getHandlerObject()
  {
    if(ignoreHandler)
    {
      maxConnections = 50;
      return 1;
    }
    CdhrObjid cdhrObjid;
    CdhrString cdhrString;
    CdhrInt cdhrInt;

    cdhrObjid = gdh.getClassList(Pwrb.cClass_WebHandler);
    if(cdhrObjid.evenSts())
    {
      errh.info("No WebHandler is configured, GdhServer terminating");
      return cdhrObjid.getSts();
    }
    cdhrString = gdh.objidToName(cdhrObjid.objid, Cdh.mName_volumeStrict);
    if(cdhrString.evenSts())
    {
      return cdhrString.getSts();
    }
    String attr = cdhrString.str + ".MaxConnections";
    cdhrInt = gdh.getObjectInfoInt(attr);
    if(cdhrInt.evenSts())
    {
      return cdhrInt.getSts();
    }
    maxConnections = cdhrInt.value;
    currentConnectionsStr = cdhrString.str + ".CurrentConnections";
    setCurrentConnections(threadCount);
    errh.info("GdhServer started, MaxConnections: " + maxConnections);
    return 1;
  }


  /**
   *  Sets the currentConnections attribute of the GdhServer object
   *
   *@param  connections  The new currentConnections value
   */
  private void setCurrentConnections(int connections)
  {
    PwrtStatus sts;

    if(!ignoreHandler)
    {
      sts = gdh.setObjectInfo(currentConnectionsStr, connections);
    }
  }


  /**
   *  Description of the Method
   */
  public void openServerSocket()
  {
    ServerSocket serverSocket = null;
    try
    {
      serverSocket = new ServerSocket(PORT);
      serverSocket.setSoTimeout(1000);
    }
    catch(IOException e)
    {
      errh.fatal("Could not listen on port " + PORT);
      return;
    }

    gdh = new Gdh((Object)null);
    errh = new Errh("GdhServer", Errh.eAnix_webmon);
    errh.setStatus( Errh.PWR__SRVSTARTUP);
    
    int sts = getHandlerObject();
    if(sts % 2 == 0)
    {
      errh.setStatus( 0);
      return;
    }

    errh.setStatus( Errh.PWR__SRUN);
    Qcom qcom = new Qcom();
    QcomrCreateQ qque = qcom.createIniEventQ("GdhServer");
    if(qque.evenSts())
    {
      System.out.println("Gdhser");
      errh.fatal("GdhSever couldn create EventQue");
      return;
    }

    QcomrGetIniEvent qrGetIniEv;
    while(true)
    {
      Socket cliSocket = null;
      try
      {
        // Wait for accept
        cliSocket = serverSocket.accept();

      }
      catch(InterruptedIOException e)
      {
	qrGetIniEv = qcom.getIniEvent(qque.qix, qque.nid, 0);
        if(qrGetIniEv.timeout)
          continue;
	else if(qrGetIniEv.terminate)
	{
          System.out.println("GdhServer received killmess from QCom");
	  return;
	}
	else 
        {
	  //Do nothing for the moment
	  //But in the future we should reinitialize if swap
	  continue;
	}

      }

      catch(IOException e)
      {
        errh.error("Accept failed.");
        continue;
      }
      if(threadCount <= maxConnections)
      {
        // Create a new thread
        threadCount++;
        totalThreadCount++;
        setCurrentConnections(threadCount);
        GdhThread gdhThread = new GdhThread(cliSocket, threadCount, maxConnections);
      }
      else
      {
        errh.warning("Connection dismissed, max number of connections exceeded");
        try
        {
          cliSocket.close();
        }
        catch(IOException e2)
        {
          errh.error("Connection close failed");
        }
      }
    }
  }


  /**
   *  Description of the Class
   *
   *@author     claes
   *@created    November 25, 2002
   */
  private class GdhThread extends Thread
  {
    Socket clientSocket;
    public Vector thSub = new Vector();
    int maxConnections;
    int threadNumber;


    /**
     *  Constructor for the GdhThread object
     *
     *@param  clientSocket    Description of the Parameter
     *@param  threadNumber    Description of the Parameter
     *@param  maxConnections  Description of the Parameter
     */
    public GdhThread(Socket clientSocket, int threadNumber, int maxConnections)
    {
      /************ In case of debugging this might be useful
      System.out.println("threadnumber : " + threadNumber + "maxconn " + maxConnections);
      try{
      System.out.println("HostName :" + clientSocket.getInetAddress().getHostName() +
                         "Delay : " + clientSocket.getTcpNoDelay());
      }
      catch(SocketException exc){}
      *************************************/
      this.threadNumber = threadNumber;
      this.clientSocket = clientSocket;
      this.maxConnections = maxConnections;
      start();
    }


    /**
     *  Main processing method for the GdhThread object
     */
    public void run()
    {
      if(log)
      {
        errh.info("New connection (" + threadNumber + ")");
      }
      ObjectInputStream in;
      ObjectOutputStream out;
      //BufferedOutputStream buff;
      //ObjectOutputStream outbuff;
      Sub sub;
      Vector subCopy;
      int i;

      try
      {
        out = new ObjectOutputStream(new BufferedOutputStream(clientSocket.getOutputStream()));
        out.flush();
	in = new ObjectInputStream(new BufferedInputStream(clientSocket.getInputStream()));
	
      }
      catch(IOException e)
      {
        errh.error("DataStream failed");
        threadCount--;
        setCurrentConnections(threadCount);
        return;
      }

      try
      {

        int function = in.readInt();
        while(function != 9999)
        {
          switch (function)
          {
            case SET_OBJECT_INFO_BOOLEAN:
              try
              {
                String attrName = in.readUTF();
                boolean value = in.readBoolean();
                PwrtStatus sts = gdh.setObjectInfo(attrName, value);
                out.writeInt(sts.getSts());
                out.flush();
              }
              catch(IOException e)
              {
                System.out.println("setObjectInfoBoolean: IO exception");
              }
              break;
            case SET_OBJECT_INFO_INT:
              try
              {
                String attrName = in.readUTF();
                int value = in.readInt();
                PwrtStatus sts = gdh.setObjectInfo(attrName, value);
                out.writeInt(sts.getSts());
                out.flush();
              }
              catch(IOException e)
              {
                System.out.println("setObjectInfoInt: IO exception");
              }
              break;
            case SET_OBJECT_INFO_FLOAT:
              try
              {
                String attrName = in.readUTF();
                float value = in.readFloat();
                PwrtStatus sts = gdh.setObjectInfo(attrName, value);
                out.writeInt(sts.getSts());
                out.flush();
              }
              catch(IOException e)
              {
                System.out.println("setObjectInfoFloat: IO exception");
              }
              break;
            case SET_OBJECT_INFO_STRING:
              try
              {
                String attrName = in.readUTF();
                String value = in.readUTF();
                PwrtStatus sts = gdh.setObjectInfo(attrName, value);
                out.writeInt(sts.getSts());
                out.flush();
              }
              catch(IOException e)
              {
                System.out.println("setObjectInfoString: IO exception");
              }
              break;
            case GET_OBJECT_INFO_BOOLEAN:
              try
              {
                String attrName = in.readUTF();
                CdhrBoolean ret = gdh.getObjectInfoBoolean(attrName);
                out.writeInt(ret.sts);
                //out.flush();
                if(ret.oddSts())
                {
                  out.writeBoolean(ret.value);
                  //out.flush();
                }
		out.flush();
              }
              catch(IOException e)
              {
                System.out.println("setObjectInfoBoolean: IO exception");
              }
              break;
            case GET_OBJECT_INFO_INT:
              try
              {
                String attrName = in.readUTF();
                CdhrInt ret = gdh.getObjectInfoInt(attrName);
                out.writeInt(ret.sts);
                //out.flush();
                if(ret.oddSts())
                {
                  out.writeInt(ret.value);
		  //out.flush();
                }
		out.flush();
              }
              catch(IOException e)
              {
                System.out.println("setObjectInfoInt: IO exception");
              }
              break;
            case GET_OBJECT_INFO_FLOAT:
              try
              {
                String attrName = in.readUTF();
                CdhrFloat ret = gdh.getObjectInfoFloat(attrName);
                out.writeInt(ret.sts);
                //out.flush();
                if(ret.oddSts())
                {
                  out.writeFloat(ret.value);
                  //out.flush();
                }
		out.flush();
              }
              catch(IOException e)
              {
                System.out.println("setObjectInfoBoolean: IO exception");
              }
              break;
            case GET_OBJECT_INFO_STRING:
              try
              {
                String attrName = in.readUTF();
                CdhrString ret = gdh.getObjectInfoString(attrName);
                out.writeInt(ret.sts);
                //out.flush();
                if(ret.oddSts())
                {
                  out.writeUTF(ret.str);
                  //out.flush();
                }
		out.flush();
              }
              catch(IOException e)
              {
                System.out.println("setObjectInfoString: IO exception");
              }
              break;
            case GET_OBJECT_INFO_OBJID:
              try
              {
                String name = in.readUTF();
                CdhrObjid ret = gdh.getObjectInfoObjid(name);
                out.writeInt(ret.getSts());
                //out.flush();
                if(ret.oddSts())
                {
                  out.writeInt(ret.objid.oix);
                  out.writeInt(ret.objid.vid);
                  //out.flush();
                }
		out.flush();
              }
              catch(IOException e)
              {
                System.out.println("nameToObjid: IO exception");
              }
              break;
            case TOGGLE_OBJECT_INFO:
              try
              {
                String attrName = in.readUTF();
                PwrtStatus sts = gdh.toggleObjectInfo(attrName);
                out.writeInt(sts.getSts());
                out.flush();
              }
              catch(IOException e)
              {
                System.out.println("setObjectInfoBoolean: IO exception");
              }
              break;
            case REF_OBJECT_INFO:
              try
              {
                String attrName = in.readUTF();

                Sub ret = this.refObjectInfo(attrName, threadNumber);

                thSub.add(ret);
                out.writeInt(ret.sts);
                if(ret.oddSts())
                {
                  out.writeInt(ret.refid.rix);
                  out.writeInt(ret.refid.nid);
                  out.writeInt(thSub.size() - 1);
                  out.writeInt(ret.typeId);
		  out.writeInt(ret.size);
		  out.writeInt(ret.elements);
                }
                out.flush();
              }
              catch(IOException e)
              {
                System.out.println("refObjectInfo: IO exception");
              }
              break;
            case REF_OBJECT_INFO_LIST:
              try
              {
                int subCnt = in.readInt();
                /*println*/
                System.out.println("List : " + subCnt);
                for(i = 0; i < subCnt; i++)
                {
                  String attrName = in.readUTF();

                  Sub ret = this.refObjectInfo(attrName, threadNumber);
                  thSub.add(ret);
                  out.writeInt(ret.sts);
                  //out.flush();
                  if(ret.oddSts())
                  {
                    out.writeInt(ret.refid.rix);
                    out.writeInt(ret.refid.nid);
                    out.writeInt(thSub.size() - 1);
                    out.writeInt(ret.typeId);
		    out.writeInt(ret.size);
  		    out.writeInt(ret.elements);
                  }
                }
                out.flush();
                System.out.println("efter REF_OBJECT_INFO_LIST");

              }
              catch(IOException e)
              {
                System.out.println("refObjectInfoList: IO exception");
              }
              break;
            case REF_OBJECT_INFO_VECTOR:
              try
              {
                Vector vec = (Vector)in.readObject();
                if(vec == null)
                {
                  System.out.println("vector == null");
                  out.writeObject(null);
                  out.flush();
                  break;
                }
		String attrName;
		Vector gdhRet = new Vector(vec.size());
		for(int j = 0; j < vec.size(); j++)
                {
                  attrName = (String)vec.get(j);
		  Sub ret = this.refObjectInfo(attrName, threadNumber);
		  if(ret.oddSts())
		  {
		    thSub.add(ret);
                    Sub retToCli = new Sub(ret.attrName,
                                   new PwrtRefId(thSub.size() - 1, ret.refid.nid),
                                   ret.id,
                                   ret.typeId,
				   ret.subscriptionsIndex,
				   ret.elements,
				   ret.size      );
                    retToCli.sts = ret.sts;
		    gdhRet.add(retToCli);
		  }
		  else
		  {
		    gdhRet.add(ret);
		  }

		}

                out.writeObject(gdhRet);
                out.flush();
              }
              catch(NotSerializableException e)
              {
                System.out.println("refObjectInfo_Vector: NotSerialziable");
              }
              catch(IOException e)
              {
                System.out.println("refObjectInfo_Vector: IO exception");
              }
              catch(ClassNotFoundException e)
              {
                System.out.println("REF_OBJECT_INFO_VECTOR: ClassNotFoundException");
                out.writeObject(null);
                out.flush();
              }

              break;
            case GET_OBJECT_REF_INFO_ALL:
              if(logRefInfoAll)
	      {
	        System.out.println("Start GET_OBJECT_REF_INFO_ALL" + (new Timestamp(new Date().getTime())).toString());
              }
              Sub subElement;

	      
              for(i = 0; i < thSub.size(); i++)
              {
                subElement = ((Sub)thSub.elementAt(i));
                int index = subElement.getIndex();
                
		int id = subElement.id;
		int typeId = subElement.typeId;
		int elements = subElement.elements;
		int size = subElement.size;
		if(log)
		{
		    System.out.println("getObjectRefInfoAll:" + elements + " " + id);
		}
                switch (typeId)
                {
                  case Pwr.eType_Int32:
                  case Pwr.eType_UInt32:
                  case Pwr.eType_Int16:
                  case Pwr.eType_UInt16:
                  case Pwr.eType_Int8:
                  case Pwr.eType_UInt8:
		    if(elements > 1)
		    {
		      //its an array
                      out.writeObject(gdh.getObjectRefInfoIntArray(id, elements));
		    }
                    else
                      out.writeInt(gdh.getObjectRefInfoInt(id));
		    break;
                  case Pwr.eType_Float32:
		    if(elements > 1)
		    {
		      //its an array
                      out.writeObject(gdh.getObjectRefInfoFloatArray(id, elements));
		    }
                    else
                      out.writeFloat(gdh.getObjectRefInfoFloat(id));
		    break;
                  case 0:
                  case Pwr.eType_Boolean:
		    if(elements > 1)
		    {
		      //its an array
                      out.writeObject(gdh.getObjectRefInfoBooleanArray(id, elements));
		    }
                    else
                      out.writeBoolean(gdh.getObjectRefInfoBoolean(id));
		    break;
                  default:
		    if(elements > 1)
		    {
		      //its an array
                      out.writeObject(gdh.getObjectRefInfoStringArray(id, typeId, size, elements));
		    }
                    else
                      out.writeObject(gdh.getObjectRefInfoString(id, typeId));
		    break;
                }
              }
              try
              {
                out.flush();
                //must be done if we dont want memory leaks...
                out.reset();
		if(logRefInfoAll)
		{
                  System.out.println("Slut GET_OBJECT_REF_INFO_ALL" + (new Timestamp(new Date().getTime())).toString());
                }
	      }
              catch(OutOfMemoryError e)
              {
                long K = 1024;
                long freeMem = Runtime.getRuntime().freeMemory() / K;
                long totalMem = Runtime.getRuntime().totalMemory() / K;
                System.out.println("Slut på minne, storlek på thSub: " + thSub.size() +
                  "storlek på sub: " + subscriptions.size() +
                  "Tillgängligt minne: " + freeMem + " KB" +
                  "Totalt minne: " + totalMem + " KB");
              }

              catch(IOException e)
              {
                System.out.println("getObjectRefInfoAll: IO exception");
              }
              break;
            case GET_OBJECT_REF_INFO_FLOAT:
              try
              {
                int id = in.readInt();
                int index = ((Sub)thSub.elementAt(id)).getIndex();
                out.writeFloat(gdh.getObjectRefInfoFloat(index));
                out.flush();
              }
              catch(IOException e)
              {
                System.out.println("getObjectRefInfoFloat: IO exception");
              }
              break;
            case GET_OBJECT_REF_INFO_BOOLEAN:
              try
              {
                int id = in.readInt();
                int index = ((Sub)thSub.elementAt(id)).getIndex();
                out.writeBoolean(gdh.getObjectRefInfoBoolean(index));
                out.flush();
              }
              catch(IOException e)
              {
                System.out.println("getObjectRefInfoBoolean: IO exception");
              }
              break;
            case GET_OBJECT_REF_INFO_INT:
              try
              {
                int id = in.readInt();
                int index = ((Sub)thSub.elementAt(id)).getIndex();
                out.writeInt(gdh.getObjectRefInfoInt(index));
                out.flush();
              }
              catch(IOException e)
              {
                System.out.println("getObjectRefInfoInt: IO exception");
              }
              break;
            case GET_OBJECT_REF_INFO_STRING:
              try
              {
                int id = in.readInt();
                int typeid = in.readInt();
                int index = ((Sub)thSub.elementAt(id)).getIndex();
                out.writeUTF(gdh.getObjectRefInfoString(index, typeid));
                out.flush();
              }
              catch(IOException e)
              {
                System.out.println("getObjectRefInfoString: IO exception");
              }
              break;






            case GET_OBJECT_REF_INFO_FLOAT_ARRAY:
              try
              {
                int id = in.readInt();
                int index = ((Sub)thSub.elementAt(id)).getIndex();
		int elements = ((Sub)thSub.elementAt(id)).getElements();
                out.writeObject(gdh.getObjectRefInfoFloatArray(index, elements));
                out.flush();
              }
              catch(IOException e)
              {
                System.out.println("getObjectRefInfoFloatArray: IO exception");
              }
              break;
            case GET_OBJECT_REF_INFO_BOOLEAN_ARRAY:
              try
              {
                int id = in.readInt();
                int index = ((Sub)thSub.elementAt(id)).getIndex();
		int elements = ((Sub)thSub.elementAt(id)).getElements();
                out.writeObject(gdh.getObjectRefInfoBooleanArray(index, elements));
                out.flush();
              }
              catch(IOException e)
              {
                System.out.println("getObjectRefInfoBooleanArray: IO exception");
              }
              break;
            case GET_OBJECT_REF_INFO_INT_ARRAY:
              try
              {
                int id = in.readInt();
                int index = ((Sub)thSub.elementAt(id)).getIndex();
		int elements = ((Sub)thSub.elementAt(id)).getElements();
                out.writeObject(gdh.getObjectRefInfoIntArray(index, elements));
                out.flush();
              }
              catch(IOException e)
              {
                System.out.println("getObjectRefInfoIntArray: IO exception");
              }
              break;
            case GET_OBJECT_REF_INFO_STRING_ARRAY:
              try
              {
                int id = in.readInt();
                int typeid = in.readInt();
                int index = ((Sub)thSub.elementAt(id)).getIndex();
		int elements = ((Sub)thSub.elementAt(id)).getElements();
		int size = ((Sub)thSub.elementAt(id)).getSize();
                out.writeObject(gdh.getObjectRefInfoStringArray(index, typeid, size, elements));
                out.flush();
              }
              catch(IOException e)
              {
                System.out.println("getObjectRefInfoStringArray: IO exception");
              }
              break;














            case UNREF_OBJECT_INFO:
              try
              {
                int rix = in.readInt();
                int nid = in.readInt();
                int index = ((Sub)thSub.elementAt(rix)).getIndex();
                PwrtStatus sts = this.unrefObjectInfo(new PwrtRefId(index, nid),
                  threadNumber);
                out.writeInt(sts.getSts());
                out.flush();
              }
              catch(IOException e)
              {
                System.out.println("unrefObjectInfo: IO exception");
              }
              break;
            case UNREF_OBJECT_INFO_ALL:
              //subCopy = getSubscriptions();
              for(i = 0; i < thSub.size(); i++)
              {
                sub = ((Sub)thSub.elementAt(i));
                int index = ((Sub)thSub.elementAt(i)).getIndex();
		PwrtStatus sts = this.unrefObjectInfo(new PwrtRefId(index, sub.refid.nid),
                  threadNumber);
                //System.out.println("unrefall :" + index + refid.nid);
                /*if(sub.sts == __UNREFED)
                {
                  continue;
                }
                this.unrefObjectInfo(sub.refid, threadNumber);
		*/
              }
              try
              {
                out.writeInt(1);
                out.flush();
              }
              catch(IOException e)
              {
                System.out.println("unrefObjectInfoAll: IO exception");
              }
              break;
            case NAME_TO_OBJID:
              try
              {
                String name = in.readUTF();
                CdhrObjid ret = gdh.nameToObjid(name);
                out.writeInt(ret.getSts());
                out.flush();
                if(ret.oddSts())
                {
                  out.writeInt(ret.objid.oix);
                  out.writeInt(ret.objid.vid);
                  out.flush();
                }
              }
              catch(IOException e)
              {
                System.out.println("nameToObjid: IO exception");
              }
              break;
            case OBJID_TO_NAME:
              try
              {
                int oix = in.readInt();
                int vid = in.readInt();
                int nameType = in.readInt();
                PwrtObjid objid = new PwrtObjid(oix, vid);
                CdhrString ret = gdh.objidToName(objid, nameType);
                out.writeInt(ret.getSts());
                if(ret.oddSts())
                {
                  out.writeUTF(ret.str);
                }
                out.flush();
              }
              catch(IOException e)
              {
                System.out.println("objidToName: IO exception");
              }
              break;
            case GET_ROOT_LIST:
              try
              {
                CdhrObjid ret = gdh.getRootList();
                out.writeInt(ret.getSts());
                if(ret.oddSts())
                {
                  out.writeInt(ret.objid.oix);
                  out.writeInt(ret.objid.vid);
                }
		out.flush();
              }
              catch(IOException e)
              {
                System.out.println("getRootList: IO exception");
              }
              break;
            case GET_NEXT_OBJECT:
              try
              {
                int oix = in.readInt();
                int vid = in.readInt();
                PwrtObjid objid = new PwrtObjid(oix, vid);
                CdhrObjid ret = gdh.getNextObject(objid);
                out.writeInt(ret.getSts());
                out.flush();
                if(ret.oddSts())
                {
                  out.writeInt(ret.objid.oix);
                  out.writeInt(ret.objid.vid);
                  out.flush();
                }
              }
              catch(IOException e)
              {
                System.out.println("getNextObject: IO exception");
              }
              break;
            case GET_CHILD:
              try
              {
                int oix = in.readInt();
                int vid = in.readInt();
                PwrtObjid objid = new PwrtObjid(oix, vid);
                CdhrObjid ret = gdh.getChild(objid);
                out.writeInt(ret.getSts());
                //out.flush();
                if(ret.oddSts())
                {
                  out.writeInt(ret.objid.oix);
                  out.writeInt(ret.objid.vid);
                  //out.flush();
                }
		out.flush();
              }
              catch(IOException e)
              {
                System.out.println("getChild: IO exception");
              }
              break;
            case GET_PARENT:
              try
              {
                int oix = in.readInt();
                int vid = in.readInt();
                PwrtObjid objid = new PwrtObjid(oix, vid);
                CdhrObjid ret = gdh.getParent(objid);
                out.writeInt(ret.getSts());
                //out.flush();
                if(ret.oddSts())
                {
                  out.writeInt(ret.objid.oix);
                  out.writeInt(ret.objid.vid);
                  //out.flush();
                }
		out.flush();
              }
              catch(IOException e)
              {
                System.out.println("getParent: IO exception");
              }
              break;
            case GET_NEXT_SIBLING:
              try
              {
                int oix = in.readInt();
                int vid = in.readInt();
                PwrtObjid objid = new PwrtObjid(oix, vid);
                CdhrObjid ret = gdh.getNextSibling(objid);
                out.writeInt(ret.getSts());
                //out.flush();
                if(ret.oddSts())
                {
                  out.writeInt(ret.objid.oix);
                  out.writeInt(ret.objid.vid);
                  //out.flush();
                }
		out.flush();
              }
              catch(IOException e)
              {
                System.out.println("getnextSibling: IO exception");
              }
              break;
            case GET_OBJECT_CLASS:
              try
              {
                int oix = in.readInt();
                int vid = in.readInt();
                PwrtObjid objid = new PwrtObjid(oix, vid);
                CdhrClassId ret = gdh.getObjectClass(objid);
                out.writeInt(ret.getSts());
                if(ret.oddSts())
                {
                  out.writeInt(ret.classId);
                }
		out.flush();
              }
              catch(IOException e)
              {
                System.out.println("getObjectClass: IO exception");
              }
              break;
            case GET_CLASS_LIST:
              try
              {
                int classid = in.readInt();
                CdhrObjid ret = gdh.getClassList(classid);
                out.writeInt(ret.getSts());
                //out.flush();
                if(ret.oddSts())
                {
                  out.writeInt(ret.objid.oix);
                  out.writeInt(ret.objid.vid);
                  //out.flush();
                }
		out.flush();
              }
              catch(IOException e)
              {
                System.out.println("getClassList: IO exception");
              }
              break;
            case CLASS_ID_TO_OBJID:
              try
              {
                int classid = in.readInt();
                CdhrObjid ret = gdh.classIdToObjid(classid);
                out.writeInt(ret.getSts());
                if(ret.oddSts())
                {
                  out.writeInt(ret.objid.oix);
                  out.writeInt(ret.objid.vid);
                }
                out.flush();
              }
              catch(IOException e)
              {
                System.out.println("classIdToObjid: IO exception");
              }
              break;
            case POLL:
              try
              {
                out.writeInt(POLL);
                out.flush();
              }
              catch(IOException e)
              {
                System.out.println("poll: IO exception");
              }
              break;
            case STATISTICS:
              try
              {
                int sub_total = in.readInt();
                int sub_late = in.readInt();
                int lock_rejected = in.readInt();
                if(logStatistics)
                {
                  errh.info("Statistics tread " + threadNumber);
                  errh.info("Sub total:     " + sub_total);
                  errh.info("Sub late:      " + sub_late);
                  errh.info("Lock rejected: " + lock_rejected);
                }
              }
              catch(IOException e)
              {
                System.out.println("statistics: IO exception");
              }
              break;
            case CHECK_USER:
              try
              {
                String systemGroup = in.readUTF();
                String user = in.readUTF();
                String password = in.readUTF();

                // Get the systemgroup
                CdhrString retstr =
                  gdh.getObjectInfoString("pwrNode-System.SystemGroup");
                if(retstr.oddSts())
                {
                  systemGroup = retstr.str;
                }

                CdhrInt ret = RtSecurity.checkUser(systemGroup, user,
                  password);
                if(ret.oddSts())
                {
                  if(log)
                  {
                    errh.success("Successfull login: " + systemGroup +
                      " " + user + " (Priv " + ret.value + ")");
                  }
                }
                else
                {
                  if(log)
                  {
                    errh.info("Login failure: " + systemGroup +
                      " " + user);
                  }
                }
                out.writeInt(ret.sts);
                out.flush();
                if(ret.oddSts())
                {
                  out.writeInt(ret.value);
                  out.flush();
                }
              }
              catch(IOException e)
              {
                System.out.println("setObjectInfoInt: IO exception");
              }
              break;
            case GET_NODE_OBJECT:
              try
              {
                CdhrObjid ret = gdh.getNodeObject();
                out.writeInt(ret.getSts());
                out.flush();
                if(ret.oddSts())
                {
                  out.writeInt(ret.objid.oix);
                  out.writeInt(ret.objid.vid);
                  out.flush();
                }
              }
              catch(IOException e)
              {
                System.out.println("getNodeObject: IO exception");
              }
              break;
            case GET_ATTRIBUTE_CHAR:
              try
              {
                String attrName = in.readUTF();
                GdhrGetAttributeChar ret = gdh.getAttributeChar(attrName);
                out.writeInt(ret.getSts());
                out.flush();
                if(ret.oddSts())
                {
                  out.writeInt(ret.typeId);
                  out.writeInt(ret.size);
                  out.writeInt(ret.offset);
                  out.writeInt(ret.elements);
                  out.flush();
                }
              }
              catch(IOException e)
              {
                System.out.println("getAttributeChar: IO exception");
              }
              break;
            case CREATE_INSTANCE_FILE:
              try
              {
                String from = in.readUTF();
                String to = in.readUTF();
                String instance = in.readUTF();
                String translFrom = Gdh.translateFilename(from);
                int sts = RtUtilities.createInstanceFile(translFrom, to,
                  instance);
                out.writeInt(sts);
                out.flush();
              }
              catch(IOException e)
              {
                System.out.println("createInstanceFile: IO exception");
              }
              break;
            case CRR_SIGNAL:
              try
              {
                String name = in.readUTF();
                CdhrString ret = gdh.crrSignal(name);
                out.writeInt(ret.getSts());
                if(ret.oddSts())
                {
                  out.writeUTF(ret.str);
                }
		out.flush();
              }
              catch(IOException e)
              {
                System.out.println("crrSignal: IO exception");
              }
              break;
            case CRR_OBJECT:
              try
              {
                String name = in.readUTF();
                CdhrString ret = gdh.crrObject(name);
                out.writeInt(ret.getSts());
                if(ret.oddSts())
                {
                  out.writeUTF(ret.str);
                }
		out.flush();
              }
              catch(IOException e)
              {
                System.out.println("crrObject: IO exception");
              }
              break;
            case LOG_STRING:
              try
              {
                String str = in.readUTF();
                System.out.println("Log: " + str);
              }
              catch(IOException e)
              {
                System.out.println("logString: IO exception");
              }
              break;
            case GET_CLASS_ATTRIBUTE:
              try
              {
                int classid = in.readInt();
                int oix = in.readInt();
                int vid = in.readInt();
                PwrtObjid objid = new PwrtObjid(oix, vid);
                CdhrObjAttr ret_obj = (CdhrObjAttr)gdh.getClassAttribute(classid, objid);
                out.writeObject(ret_obj);
                out.flush();
              }
              catch(IOException e)
              {
                System.out.println("getClassAttribute: IO exception");
              }
              break;
            case GET_ALL_CLASS_ATTRIBUTES:
              try
              {
                int classid = in.readInt();
                int oix = in.readInt();
                int vid = in.readInt();
                PwrtObjid objid = new PwrtObjid(oix, vid);
                CdhrObjAttr attr = (CdhrObjAttr)gdh.getClassAttribute(classid, objid);
                Vector v = new Vector();
                while(attr != null)
                {
                  v.add(attr);
                  attr = gdh.getClassAttribute(classid, attr.objid);
                }
                out.writeObject(v);
                out.flush();
              }
              catch(IOException e)
              {
                System.out.println("getAllClassAttributes: IO exception");
              }
              break;
            case GET_ALL_SIBLINGS:
              try
              {
                PwrtObjid objid = (PwrtObjid)in.readObject();
                CdhrObjid sibling = (CdhrObjid)gdh.getNextSibling(objid);
                Vector v = new Vector();
                while(sibling.oddSts())
                {
                  v.add(sibling);
                  sibling = gdh.getNextSibling(sibling.objid);
                }
                out.writeObject(v);
                out.flush();
              }
              catch(IOException e)
              {
                System.out.println("getAllSiblings: IO exception");
              }
              catch(ClassNotFoundException e)
              {
                System.out.println("getAllSiblings: ClassNotFoundException");
              }
              break;
            case GET_ALL_XTT_SIBLINGS:
              try
              {
                PwrtObjid objid = (PwrtObjid)in.readObject();

                String name = null;
                String fullName = null;
                String className = null;
                String description = " ";
                CdhrObjid cdhrObjId;
                CdhrClassId cdhrClassId;
                int sts = 2;
                boolean hasChildren = false;
                Vector v = new Vector();
                CdhrObjid classObj;

                cdhrObjId = (CdhrObjid)gdh.getNextSibling(objid);
                while(cdhrObjId.oddSts())
                {
                  cdhrClassId = gdh.getObjectClass(cdhrObjId.objid);
                  if(cdhrClassId.oddSts())
                  {
                    classObj = gdh.classIdToObjid(cdhrClassId.classId);
                    if(classObj.oddSts())
                    {
                      className = gdh.objidToName(classObj.objid, Cdh.mName_object).str;
                      fullName = gdh.objidToName(cdhrObjId.objid, Cdh.mName_pathStrict).str;
                      name = gdh.objidToName(cdhrObjId.objid, Cdh.mName_object).str;
                      CdhrString ret = gdh.getObjectInfoString(fullName + ".Description");
                      if(ret.oddSts())
                      {
                        description = ret.str;
                      }
                      else
                      {
                        description = " ";
                      }
                      sts = 1;
                      if(gdh.getChild(cdhrObjId.objid).oddSts())
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
                  cdhrObjId = gdh.getNextSibling(cdhrObjId.objid);
                  hasChildren = false;
                  sts = 2;
                }
                out.writeObject(v);
                out.flush();
              }
              catch(IOException e)
              {
                System.out.println("getAllXttSiblings: IO exception");
              }
              catch(ClassNotFoundException e)
              {
                System.out.println("getAllXttSiblings: ClassNotFoundException");
              }
              break;
            case GET_ALL_XTT_CHILDREN:
              try
              {
                PwrtObjid objid = (PwrtObjid)in.readObject();

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

                cdhrObjId = (CdhrObjid)gdh.getChild(objid);
                while(cdhrObjId.oddSts())
                {
                  cdhrClassId = gdh.getObjectClass(cdhrObjId.objid);
                  if(cdhrClassId.oddSts())
                  {
                    classObj = gdh.classIdToObjid(cdhrClassId.classId);
                    if(classObj.oddSts())
                    {
                      className = gdh.objidToName(classObj.objid, Cdh.mName_object).str;
                      fullName = gdh.objidToName(cdhrObjId.objid, Cdh.mName_pathStrict).str;
                      name = gdh.objidToName(cdhrObjId.objid, Cdh.mName_object).str;
                      CdhrString ret = gdh.getObjectInfoString(fullName + ".Description");
                      if(ret.oddSts())
                      {
                        description = ret.str;
                      }
                      else
                      {
                        description = " ";
                      }
                      sts = 1;
                      if(gdh.getChild(cdhrObjId.objid).oddSts())
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
                  cdhrObjId = gdh.getNextSibling(cdhrObjId.objid);
                  hasChildren = false;
                  sts = 2;
                }
                out.writeObject(v);
                out.flush();
              }
              catch(IOException e)
              {
                System.out.println("getAllXttChildren: IO exception");
              }
              catch(ClassNotFoundException e)
              {
                System.out.println("getAllXttChildren: ClassNotFoundException");
              }
              break;
            case GET_SUBSCRIPTIONS:
              try
              {
                //System.out.println("GET_SUBSCRIPTIONS");
                int nrOfSub = 0;
                ArrayList subcp = this.getSubscriptions();
                String ObjectAttributeNames[] = new String[subcp.size()];
                String ThreadNumbersConnected[] = new String[subcp.size()];
                String NumberOfPrenumerations[] = new String[subcp.size()];
                String Status[] = new String[subcp.size()];
                String Refid[] = new String[subcp.size()];
                ListIterator iter = subcp.listIterator();
		while(iter.hasNext())
                {
		  SubElement subEl = (SubElement)iter.next();
		  int ii = iter.previousIndex();
                  ThreadNumbersConnected[ii] = " ";
                  NumberOfPrenumerations[ii] = " ";
                  ObjectAttributeNames[ii] = subEl.sub.attrName;
                  Status[ii] = Integer.toString(subEl.sub.sts);
                  Refid[ii] = Integer.toString(subEl.sub.refid.rix);
                  for(int j = 0; j < maxConnections; j++)
                  {
                    if(subEl.reffedByThreadBitSet.get(j))
                    {
                      nrOfSub++;
                      ThreadNumbersConnected[ii] += j + ", ";
                      NumberOfPrenumerations[ii] += subEl.reffedByThread[j] + ", ";
                    }

                  }
                }

                out.writeInt(subcp.size());
                out.writeInt(nrOfSub);
                out.writeObject(ObjectAttributeNames);
                out.writeObject(ThreadNumbersConnected);
                out.writeObject(NumberOfPrenumerations);
                out.writeObject(Status);
                out.writeObject(Refid);
                out.writeLong(Runtime.getRuntime().freeMemory());
		out.writeLong(Runtime.getRuntime().totalMemory());
		out.flush();

                //must be done if we dont want memory leaks...
                out.reset();

              }
              catch(OutOfMemoryError e)
              {
                long freeMem = Runtime.getRuntime().freeMemory();
                long totalMem = Runtime.getRuntime().totalMemory();
                System.out.println("Slut på minne, storlek på subcp: " + subscriptions.size() +
                  "Tillgängligt minne: " + freeMem +
                  "Totalt minne: " + totalMem);
              }
              catch(IOException e)
              {
                System.out.println("GET_SUBSCRIPTIONS: IO exception");
              }
              break;
            default:
              errh.error("Received unknown function : " + function);
          }
          function = in.readInt();
        }
      }
      catch(IOException e)
      {
        try
        {
          out.close();
        }
        catch(IOException e2)
        {
          System.err.println("Close failed");
        }
        try
        {
          in.close();
        }
        catch(IOException e2)
        {
          System.err.println("Close failed");
        }
        try
        {
          clientSocket.close();
        }
        catch(IOException e2)
        {
          System.err.println("Close failed");
        }
        //check that all subscriptions has stopped
	for(i = 0; i < thSub.size(); i++)
        {
          try
          {
            sub = ((Sub)thSub.elementAt(i));
            int index = ((Sub)thSub.elementAt(i)).getIndex();

            PwrtStatus sts = this.unrefObjectInfo(new PwrtRefId(index, sub.refid.nid),
              threadNumber);

          }
          catch(ArrayIndexOutOfBoundsException exc)
          {
          }
        }
        //minska storleken på subscriptions för att spara minne
        this.trimRefObjectList();
        threadCount--;
        setCurrentConnections(threadCount);
        if(log)
        {
          errh.info("Connection closed (" + threadNumber + ") (" +
            threadCount + ")");
        }
      }
    }
    


    /**
     *  Description of the Method
     *
     *@param  attrName      Description of the Parameter
     *@param  threadNumber  Description of the Parameter
     *@return               Description of the Return Value
     */
    public synchronized Sub refObjectInfo(String attrName, int threadNumber)
    {
      SubElement sub;
      int firstUnreffedIndex = subscriptionCount;
      boolean notFoundUnreffed = true;

      //loopa igenom och titta om vi redan har refererat objektet
      int index = subscriptions.indexOf(new SubElement(attrName));
      if(index >= 0)
      {
        sub = (SubElement)subscriptions.get(index);
	sub.reffedByThreadBitSet.set(threadNumber);
        sub.reffedByThread[threadNumber]++;
	return sub.sub;
      }
      ListIterator iter = subscriptions.listIterator();
      while(iter.hasNext())
      {
        sub = (SubElement)iter.next();
        //spara undan eventuellt avreffererat elements index
        if(notFoundUnreffed && sub.sub.sts == __UNREFED)
        {
          firstUnreffedIndex = iter.previousIndex();
          notFoundUnreffed = false;
          break;
	}
      }

      GdhrRefObjectInfo ret = gdh.refObjectInfo(attrName);

      if(ret.oddSts())
      {
        sub = new SubElement(maxConnections, threadNumber);
        if(notFoundUnreffed)
        {
          //System.out.println("this.refObjectInfo ej reffad ej lucka i subsc.. " + attrName + " " + subscriptionCount);
          sub.setSub(ret, subscriptionCount, attrName);
          subscriptions.add(sub);
          subscriptionCount++;
          sub.reffedByThreadBitSet.set(threadNumber);
          sub.reffedByThread[threadNumber]++;
	  //lastIndexReffed = subscriptions.size();
        }
        else
        {
          //System.out.println("this.refObjectInfo ej reffad  " + attrName + " " + firstUnreffedIndex);
          sub.setSub(ret, firstUnreffedIndex, attrName);
          subscriptions.remove(firstUnreffedIndex);
          subscriptions.add(firstUnreffedIndex, sub);
          sub.reffedByThreadBitSet.set(threadNumber);
          sub.reffedByThread[threadNumber]++;
        }
      }
      else
      {
        sub = new SubElement(maxConnections, threadNumber);
        sub.setSub(ret, -1, " ");
      }
      return sub.getSub();
    }



    /**
     *  Description of the Method
     *
     *@param  refid         Description of the Parameter
     *@param  threadNumber  Description of the Parameter
     *@return               Description of the Return Value
     */
    public synchronized PwrtStatus unrefObjectInfo(PwrtRefId refid, int threadNumber)
    {
      //System.out.println("unrefObjectInfo");
      SubElement sub;
      boolean doNotUnrefObject = false;
      try
      {
        //System.out.println("unrefObjectInfo: " + refid.rix);
        sub = (SubElement)subscriptions.get(refid.rix);

        if(sub.reffedByThread[threadNumber] > 0)
        {
          sub.reffedByThread[threadNumber]--;
          if(sub.reffedByThread[threadNumber] <= 0)
          {
            sub.reffedByThreadBitSet.clear(threadNumber);
          }
        }
        
        for(int i = 0; i < sub.reffedByThread.length; i++)
        {
          if(sub.reffedByThread[i] > 0)
          {
            doNotUnrefObject = true;
            break;
          }
        }
      }
      catch(IndexOutOfBoundsException e)
      {
        System.out.println("unrefObjectInfo : indexoutofbounds");
        return new PwrtStatus(2);
      }
      PwrtStatus ret = new PwrtStatus(1);
      if(!doNotUnrefObject)
      {
        /*
	if(refid.rix < lastIndexReffed)
	{
	  lastIndexReffed = refid.rix;
	}
	*/
        ret = gdh.unrefObjectInfo(sub.sub.refid);
        sub.sub.sts = __UNREFED;
        sub.sub.attrName = " ";
      }
      //System.out.println("unrefObjectInfo: ret.sts " + ret.getSts());
      return ret;
    }



    /**
     *  Description of the Method
     *
     *@param  attrName      Description of the Parameter
     *@param  threadNumber  Description of the Parameter
     *@return               Description of the Return Value
     */
    public synchronized void trimRefObjectList()
    {
      SubElement sub;
      int i = subscriptions.size();
      int oldi = i;
      while(i > 0)
      {
        i--;
        sub = (SubElement)subscriptions.get(i);
        if(sub.sub.sts == __UNREFED)
        {
          subscriptions.remove(i);
	  subscriptionCount--;
	}
	else
	{
	  break;
	}
      }
      //System.out.println("Minskar subscriptions-listan med " + oldi + " - " + (i + 1));
      subscriptions.trimToSize();
      System.gc();
      System.runFinalization();
 
      //System.out.println("subscriptionsstorlek " + subscriptions.size());
    }



    /**
     *  Gets the subscriptions attribute of the GdhThread object
     *
     *@return    The subscriptions value
     */
    public synchronized ArrayList getSubscriptions()
    {
      return (ArrayList)subscriptions.clone();
    }
  }
  //GdhThread

}

