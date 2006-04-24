/* 
 * Proview   $Id: HistServer.java,v 1.3 2006-04-24 13:21:46 claes Exp $
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
 *@version    0.1 beta: Första testversionen
 */
public class HistServer
{
  public final static int HISTPORT = 4447;
  
  public final static int __IO_EXCEPTION = 2000;

  static boolean ignoreHandler = false;
  static boolean log = false;
  static boolean logStatistics = false;
  static boolean test = false;
  boolean keepRunning = true;

  /**
   *  The main program for the HistServer class
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
      else if(args[i].equals("-s"))
      {
        logStatistics = true;
      }
      else if(args[i].equals("-t"))
      {
	test = true;
      }
    }
    if(log)
    {
      System.out.println("HistServer starting");
    }
    
    Hist hist = new Hist();
    HistServer hs = new HistServer();
    hs.run(hist);

    
    System.out.println("HistServer exiting");
    System.exit(0);
    
  }
  public void run(Hist hist)
  {
    // boolean keepRunning = true;
    
    ServerSocket serverSocket = null;
    if(test)
    {
      boolean[] type = new boolean[4];
      type[0] = type[1] = type[2] = type[3] = false;
      type[0] = true;
      boolean[] prio = new boolean[4];
      prio[0] = prio[1] = prio[2] = prio[3] = false;
      HistQuery query = new HistQuery("2003-11-05 09:26:49", "2004-11-05 09:26:49", type, prio, "*", "*");
      MhrEvent[] ev = Hist.getHistList(query.startTime,
				       query.stopTime,
				       query.type[0],
				       query.type[1],
				       query.type[2],
				       query.type[3],
				       query.priority[0],
				       query.priority[1],
				       query.priority[2],
				       query.priority[3],
				       query.name,
				       query.text);
      int i = 0;
      System.out.println("No events: "+ ev.length);
      while(i < ev.length)
      {
        System.out.println(ev[i].toString());
        i++;
      }
      return;
    }
    try
    {
      serverSocket = new ServerSocket(HISTPORT);
      serverSocket.setSoTimeout(1000);
    }
    catch(IOException e)
    {
      System.out.println("IOException in openServerSocket");
      //errh.fatal("Could not listen on port " + HISTPORT);
      System.exit(1);
    }

    //    gdh = new Gdh((Object)null);
    //    errh = new Errh("MhServer", Errh.eAnix_webmonmh);
    //    errh.setStatus( Errh.PWR__SRVSTARTUP);

    if(log)
    {
      System.out.println("JHist: Before waiting for client");
    }

    //      mh = new Mh((Object)null, maxAlarms, maxEvents);

    //    errh.setStatus( Errh.PWR__SRUN);


    //    Qcom qcom = new Qcom();
    //    QcomrCreateQ qque = qcom.createIniEventQ("MhServer");
    //    if(qque.evenSts())
    //    {
    //      System.out.println("MH:Error during qreateque");
    //      errh.fatal("MH:Error during qcom.createIniEventQ");
    //      return;
    //    }

    //    QcomrGetIniEvent qrGetIniEv;

    while(keepRunning)
    {
      Socket cliSocket = null;
      try
      {
        if(log)
        {
	    //System.out.println(" Wait for accept ");
        }
        cliSocket = serverSocket.accept();
      }
      catch(InterruptedIOException e)
      {
        continue;
/*
      qrGetIniEv = qcom.getIniEvent(qque.qix, qque.nid, 0);
      if(qrGetIniEv.timeout)
      {
        //do nothing
        continue;
      }
      else if(qrGetIniEv.terminate)
      {
        //Time to die
        System.out.println("MhServer received killmess from QCom");
        return;
      }
      else 
      {
        //All other messages is qurrently ignored
        //But perhaps we should reinitialize when we get
        //swapdone
        continue;
      }
*/
      }
      catch(IOException e)
      {
	//errh.error("Accept failed.");
        this.keepRunning = false;
        continue;
      }
      if(log)
      {
        System.out.println("New client for HistServer");
      }
      new HistThread(hist, cliSocket);
    }
  }


  private class HistThread extends Thread
  {
    Hist hist;
    Socket socket;

    public HistThread(Hist hist, Socket socket)
    {
      this.hist = hist;
      this.socket = socket;
      start();
    }

    public void run()
    {
      this.handleClient(this.socket);
    }

    public void handleClient(Socket socket)
    {
      ObjectInputStream in = null;
      ObjectOutputStream out = null;
      try{
        out = new ObjectOutputStream(socket.getOutputStream());
	in = new ObjectInputStream(socket.getInputStream());
        //wait for the question	    
        HistQuery query = (HistQuery)in.readObject();
	if(log)
	{
	  System.out.println("Recieved a query");
	  System.out.println("query: Prio(ABCD): " + query.priority[0] + query.priority[1] + query.priority[2] + query.priority[3]);
	  System.out.println("       type(Akt Mess Ret Kvitt): " + query.type[0] + query.type[1] + query.type[2] + query.type[3]);
	  System.out.println("       startTime: " + query.startTime);
	  System.out.println("       stopTime: " + query.stopTime);
	  System.out.println("       name: " + query.name);
	  System.out.println("       text: " + query.text);



	}
        //send the answer
        out.writeObject(Hist.getHistList(query.startTime,
				       query.stopTime,
				       query.type[0],
				       query.type[1],
				       query.type[2],
				       query.type[3],
				       query.priority[0],
				       query.priority[1],
				       query.priority[2],
				       query.priority[3],
				       query.name,
				       query.text));
      }
      catch(IOException e)
      {
	System.out.println("Exception in hist.handleQuery:" + e.toString());
	//errh.error("hist.handleCLient: DataStream failed");
      }
      catch(Exception e)
      {
	System.out.println("Exception in hist.handleQuery:" + e.toString());
	//errh.error("hist.handleCLient: Exception");
      }

      finally
      {
	  //System.out.println("finally");
        try
	{
	  out.close();
	}
	catch(Exception e)
	{
	}
	try
	{
	  in.close();
	}
	catch(Exception e)
	{
	  System.out.println("Closing client socket");
	}
	try
	{
	  socket.close();
	}
	catch(Exception e)
	{
	}
      }//finally
    }//handleClient
  }//HistThread
}//HistServer

