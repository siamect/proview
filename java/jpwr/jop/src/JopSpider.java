/* 
 * Proview   $Id: JopSpider.java,v 1.7 2005-09-01 14:57:50 claes Exp $
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
import java.awt.*;
import java.lang.*;
import java.lang.reflect.*;
import javax.swing.*;
import java.net.*;
import java.applet.*;
import java.util.*;
import jpwr.rt.*;

public class JopSpider {
  int qcom_qix;
  int qcom_nid;
  JopSession session;
  JopEngine engine;

  static int op_qcom_qix;
  static int op_qcom_nid;
  static Qcom qcom;
  static String systemName;

  public JopSpider( int op_qcom_qix) {
    this.op_qcom_qix = op_qcom_qix;

    engine = new JopEngine( 1000, (Object)null);
    session = new JopSession( engine, (Object)this);

    // Attatch to qcom
    qcom = new Qcom();
    QcomrCreateQ ret = qcom.createQ( 0, 0, "jop_spider");
    if ( ret.evenSts())
      System.out.println( "Unable to open que: " + ret.getSts());
    else {
      qcom_qix = ret.qix;
      qcom_nid = ret.nid;
      op_qcom_nid = ret.nid;
    }

    // Send qcom qix to jop
    String set_cmd = "set jop_queid " + qcom_qix;
System.out.println( "qcom put");
    PwrtStatus sts = qcom.put( op_qcom_qix, op_qcom_nid, set_cmd);
System.out.println( "qcom put finished");
    if ( sts.evenSts())
      System.out.println( "Qcom put error: " + sts.getSts());

    new JopSpiderCmd( session);

//    loadFrame("Frame");
//    loadFrame("Frame");
  }  
  
  static public void setSystemName( String systemName) {
    JopSpider.systemName = systemName;
  }
  static public String getSystemName() {
    return systemName;
  }

  static CliTable[] cliTable = new CliTable[] { 
    new CliTable( "OPEN", new String[] {"cli_arg1", "cli_arg2", "/NAME", 
	"/FILE", "/SCROLLBAR", "/WIDTH", "/HEIGHT", "/MENU", "/NAVIGATOR", 
	"/CENTER", "/OBJECT", "/INSTANCE", "/NEW", "/CLASSGRAPH", "/ACCESS"}),
    new CliTable( "EXIT", null),
    new CliTable( "HELP", new String[] {"cli_arg1", "cli_arg2", "cli_arg3",
	"cli_arg4", "/HELPFILE", "/POPNAVIGATOR", "/BOOKMARK", "/INDEX",
        "/BASE", "/RETURNCOMMAND", "/WIDTH", "/HEIGHT", "/VERSION"}),
    new CliTable( "SET", new String[] {"cli_arg1",
    	"/NAME", "/VALUE", "/BYPASS"}),
    new CliTable( "EXAMPLE", new String[] {"/NAME", "/HIERARCHY"}) 
  };

  static void command( JopSession session, String cmd) {
    boolean local_cmd = false;
    Object root = session.getRoot();
    Gdh gdh = session.getEngine().gdh;

    System.out.println("JopSpider command : " + cmd);
    if ( root instanceof JopApplet) {
      if ( ((JopApplet)root).engine.isInstance())
	cmd = RtUtilities.strReplace( cmd, "$object", 
			  ((JopApplet)root).engine.getInstance());
    }

    Cli cli = new Cli( cliTable);
    String command = cli.parse( cmd);
    if (cli.oddSts()) {
	System.out.println("JopSpider1 : " + command);
      if ( command.equals("OPEN")) {
        if ( cli.qualifierFound("cli_arg1")) {

          String jgraph = "JGRAPH";
          String graph = "GRAPH";
          String url = "URL";
          String cli_arg1 = cli.getQualValue("cli_arg1").toUpperCase();
          if ( jgraph.length() >= cli_arg1.length() &&
               jgraph.substring(0,cli_arg1.length()).equals(cli_arg1)) {
            // Command is "OPEN JGRAPH"

            boolean newFrame = cli.qualifierFound("/NEW");
            boolean scrollbar = cli.qualifierFound("/SCROLLBAR");

            if ( ! cli.qualifierFound("cli_arg2")) {
	      System.out.println("Syntax error");
              return;
            }
	    String frameName = cli.getQualValue("cli_arg2");
	    String instance = cli.getQualValue("/INSTANCE");

	    if ( session.isOpWindowApplet()) {
	      frameName = frameName.substring(0,1).toUpperCase() +
		    frameName.substring(1);
	      System.out.println( "Open frame " + frameName);
	      session.openGraphFrame( frameName, instance, scrollbar, false);
	    }
            else if ( session.isApplet()) {
              System.out.println( "Loading applet \"" + frameName + "\"");
	      openURL( session, frameName, newFrame, null);
              local_cmd = true;
            }
            else {
              System.out.println( "Loading frame \"" + frameName + "\"");
	      loadFrame( session, frameName, instance, scrollbar);
              local_cmd = true;
            }
          }
          else if ( graph.length() >= cli_arg1.length() &&
               graph.substring(0,cli_arg1.length()).equals(cli_arg1)) {
            // Command is "OPEN GRAPH"
            if ( root instanceof JopApplet) {
              boolean newFrame = cli.qualifierFound("/NEW");
              boolean scrollbar = cli.qualifierFound("/SCROLLBAR");

              if ( cli.qualifierFound("/OBJECT")) {
	        String objectValue = cli.getQualValue("/OBJECT");
                String objectName;
                String appletName;
		String instance = null;

                // Replace * by node object
                if ( objectValue.charAt(0) == '*') {
                  CdhrObjid cdhr_node = gdh.getNodeObject(0);
                  if ( cdhr_node.evenSts())
                    return;
                  CdhrString cdhr_nodestr = gdh.objidToName( cdhr_node.objid, Cdh.mName_volumeStrict);
                  objectName = cdhr_nodestr.str + objectValue.substring(1);
                }
                else
                  objectName = objectValue;

                String attrName = objectName + ".Action";
                CdhrString cdhr = gdh.getObjectInfoString( attrName);
                if ( cdhr.evenSts()) {
	          System.out.println("Object name error");
                  return;
                }
                int idx = cdhr.str.lastIndexOf( ".pwg");
                if ( idx != -1)
                  appletName = cdhr.str.substring(0, idx);
                else {
                  idx = cdhr.str.lastIndexOf( ".class");
		  if ( idx != -1)
                    appletName = cdhr.str.substring(0, idx);
		  else {
		    // This is a command
		    command( session, cdhr.str);
		    return;
		  }
		}

		attrName = objectName + ".Object";
		cdhr = gdh.getObjectInfoString( attrName);
		if ( cdhr.oddSts() && !cdhr.str.equals(""))
		  instance = cdhr.str;

                if ( session.isOpWindowApplet()) {
	          appletName = appletName.substring(0,1).toUpperCase() +
		      appletName.substring(1);
	          System.out.println( "Open frame " + appletName);
	          session.openGraphFrame( appletName, instance, false, false);
		}
		else {
                  System.out.println( "Loading applet \"" + appletName + "\"");
		  openURL( session, appletName, newFrame, null);
		}
                local_cmd = true;
              }
              else {
		if ( session.isOpWindowApplet()) {
		  String frameName = null;
		  String instanceValue = null;
		  boolean classGraph = false;
                  if ( cli.qualifierFound("/INSTANCE")) {
	            instanceValue = cli.getQualValue("/INSTANCE");
		    classGraph = cli.qualifierFound("/CLASSGRAPH");		      
		  }
		  if ( !classGraph) {
		    if ( ! cli.qualifierFound("cli_arg2")) {
		      System.out.println("Syntax error");
		      return;
		    }
		    frameName = cli.getQualValue("cli_arg2").toLowerCase();

		    frameName = frameName.substring(0,1).toUpperCase() +
			frameName.substring(1);
		    System.out.println( "Open frame " + frameName);
		  }
		  session.openGraphFrame( frameName, instanceValue, scrollbar, classGraph);
		}
		else {
		  String frameName = null;
		  if ( ! cli.qualifierFound("cli_arg2")) {
		    System.out.println("Syntax error");
		    return;
		  }
		  frameName = cli.getQualValue("cli_arg2").toLowerCase();

                  if ( cli.qualifierFound("/INSTANCE")) {
	            String instanceValue = 
			cli.getQualValue("/INSTANCE").toLowerCase();
                  
                    String tempFile = frameName + "_" + 
			instanceValue.replace('å','a').replace('ä','a').replace('ö','o');
		    PwrtStatus psts = 
			gdh.createInstanceFile( 
			   "$pwrp_websrv/"+frameName+".html", 
			   tempFile+".html", instanceValue);
		    if ( psts.evenSts()) {
		      System.out.println("createInstanceFile error");
                      return;
		    }
                    frameName = tempFile;
		  }

                  System.out.println( "Loading applet \"" + frameName + "\"");

	          openURL( session, frameName, newFrame, null);
                  local_cmd = true;
		}
              }
            }
          }
          else if ( url.length() >= cli_arg1.length() &&
               url.substring(0,cli_arg1.length()).equals(cli_arg1)) {
            // Command is "OPEN URL"
            if ( root instanceof JopApplet) {
              if ( cli.qualifierFound("cli_arg2")) {
		String urlValue = cli.getQualValue("cli_arg2");
    System.out.println("open url " + urlValue);
                if ( urlValue.startsWith("pwrb_") ||
		     urlValue.startsWith("pwrs_") ||
		     urlValue.startsWith("nmps_") ||
		     urlValue.startsWith("ssab_"))
	          // Object reference manual
		  urlValue = "$pwr_doc/orm/" + urlValue;

	        openURL( session, urlValue, true, null);
	      }
	    } 
	  }
	  else {
	    System.out.println( "Unknown command");
	  }
	}
      }
      else if ( command.equals("HELP")) {
        if ( root instanceof JopApplet) {
          String fileName = "xtt_help_";
          String bookmarkValue = null;

          if ( cli.qualifierFound("/VERSION")) {
	    openURL( session, "$pwr_doc/xtt_version_help_version.html", true, null);
	  }
	  else {
            if ( cli.qualifierFound("/BASE"))
	      // Not language dependent !! TODO
	      fileName = "$pwr_doc/help/xtt_help_";
	    
            if ( cli.qualifierFound("cli_arg1"))
	      fileName += cli.getQualValue("cli_arg1").toLowerCase();
            if ( cli.qualifierFound("cli_arg2"))
	      fileName += "_" + cli.getQualValue("cli_arg2").toLowerCase();
            if ( cli.qualifierFound("cli_arg3"))
	      fileName += "_" + cli.getQualValue("cli_arg3").toLowerCase();
            if ( cli.qualifierFound("cli_arg4"))
	      fileName += "_" + cli.getQualValue("cli_arg4").toLowerCase();

	    if ( fileName.startsWith("pwrb_") ||
		 fileName.startsWith("pwrs_") ||
		 fileName.startsWith("nmps_") ||
		 fileName.startsWith("ssab_"))
	      // Object reference manual
	      fileName = "$pwr_doc/orm/" + fileName;

            if ( cli.qualifierFound("/BOOKMARK"))
	      bookmarkValue = cli.getQualValue("/BOOKMARK");

            System.out.println( "Loading helpfile \"" + fileName + "\"");
	    openURL( session, fileName, true, bookmarkValue);
	  }
          local_cmd = true;
        }
      }
      else if ( command.equals("SET")) {
        if ( cli.qualifierFound("cli_arg1")) {

          String parameter = "PARAMETER";
          String cli_arg1 = cli.getQualValue("cli_arg1").toUpperCase();
          if ( parameter.length() >= cli_arg1.length() &&
               parameter.substring(0,cli_arg1.length()).equals(cli_arg1)) {
            // Command is "SET PARAMETER"
            if ( root instanceof JopApplet) {
              String name;
              String value;
              PwrtStatus sts;

              local_cmd = true;
              if ( cli.qualifierFound("/NAME"))
	        name = cli.getQualValue("/NAME");
              else {
                System.out.println( "Cmd: name is missing\n");
                return;
              }
              if ( cli.qualifierFound("/VALUE"))
	        value = cli.getQualValue("/VALUE");
              else {
                System.out.println( "Cmd: value is missing\n");
                return;
              }
              boolean bypass = cli.qualifierFound("/BYPASS");
              if ( !bypass ) {
                // Need RtWrite or System to set attribute
                if ( ! gdh.isAuthorized( 
			     Pwr.mPrv_RtWrite | Pwr.mPrv_System)) {
                  System.out.println( "No authorized");
                  return;
                }
              }

              // Get type of attribute
              GdhrGetAttributeChar ret = gdh.getAttributeChar( name);
              if ( ret.evenSts()) return;

              if ( ret.typeId == Pwr.eType_Float32) {
                float setValue = Float.parseFloat( value);
                sts = gdh.setObjectInfo( name, setValue);
              }
              else if ( ret.typeId == Pwr.eType_Boolean) {
                boolean setValue = (Integer.parseInt( value, 10) != 0);
                sts = gdh.setObjectInfo( name, setValue);
              }
              else if ( ret.typeId == Pwr.eType_Int32 ||
		        ret.typeId == Pwr.eType_UInt32 ||
                        ret.typeId == Pwr.eType_Int16 ||
		        ret.typeId == Pwr.eType_UInt16 ||
                        ret.typeId == Pwr.eType_Int8 ||
		        ret.typeId == Pwr.eType_UInt8 ||
		        ret.typeId == Pwr.eType_Mask ||
		        ret.typeId == Pwr.eType_Enum) {
                int setValue = Integer.parseInt( value, 10);
                sts = gdh.setObjectInfo( name, setValue);
              }
              else if ( ret.typeId == Pwr.eType_String) {
                sts = gdh.setObjectInfo( name, value);
              }
              else
                return;

              if ( sts.evenSts())
                 System.out.println( "setObjectInfoError " + sts);
            }
          }
        }
      }
    }
    else
      System.out.println( "JopSpider: Pasre error " + cli.getStsString());

    if ( !local_cmd) {
      // Send to xtt
      if ( qcom != null) {
        PwrtStatus sts = qcom.put( op_qcom_qix, op_qcom_nid, cmd);
        System.out.println( "Send " + cmd + "  sts: " + sts.getSts());
        if ( sts.evenSts())
          System.out.println( "Qcom put error: " + sts.getSts());
      }
    }
    
  }
  private class JopSpiderCmd extends Thread {
    JopSession session;

    JopSpiderCmd( JopSession session) {
      this.session = session;
      start();
    }

    public void run() {
      CdhrString cret;

System.out.println( "JopSpiderCmd start");

      while ( true) {
        cret = qcom.getString( qcom_qix, qcom_nid);
//      System.out.println("awake : " + cret.getSts());
        if ( cret.oddSts()) {
          int idx;
          System.out.println( "Command received :" + cret.str);

          Cli cli = new Cli( cliTable);
          String command = cli.parse( cret.str);
          if (cli.oddSts()) {

            if ( command.equals("OPEN")) {

              if ( cli.qualifierFound("cli_arg1")) {

                String jgraph = "JGRAPH";
                String cli_arg1 = cli.getQualValue("cli_arg1").toUpperCase();
                if ( jgraph.length() >= cli_arg1.length() &&
                     jgraph.substring(0,cli_arg1.length()).equals(cli_arg1)) {
                  // Command is "OPEN JGRAPH"

                  boolean newFrame = cli.qualifierFound("/NEW");
                  boolean scrollbar = cli.qualifierFound("/SCROLLBAR");

                  if ( ! cli.qualifierFound("cli_arg2")) {
	            System.out.println("Syntax error");
                    return;
                  }
	          String frameName = cli.getQualValue("cli_arg2");

                  System.out.println( "Loading frame \"" + frameName + "\"");
	          loadFrame( session, frameName, null, scrollbar);
                }
              }
            }
            else if ( command.equals("EXIT")) {
	      System.out.println("Jop exiting");
              System.exit(0);
            }
          }   
        }
        try {
          sleep( 1000);
	}
	catch ( InterruptedException e) {
	}
      }
    }
  }

  public static Object loadFrame( JopSession session, String className, 
				  String instance, boolean scrollbar) {
    Object frame;
    
    try {
      Class clazz = Class.forName( className);
      try {
	Class argTypeList[] = new Class[] { session.getClass(), className.getClass(),
	                                    boolean.class}; 
        Object argList[] = new Object[] { session, instance, new Boolean(scrollbar)};
        Constructor constructor = clazz.getConstructor( argTypeList);

	try {
          frame = constructor.newInstance( argList);
	}
	catch ( Exception e) {
	  System.out.println("Class instatiation error: " + className);
	  return null;
	}
        // frame = clazz.newInstance();
        openFrame( frame);
        return frame;
      }
      catch ( NoSuchMethodException e) {
        System.out.println("NoSuchMethodException: Unable to get frame constructor " + className);
      }
      catch ( Exception e) {
        System.out.println("Exception: Unable to get frame class " + className + " " + e.getMessage());
      }
    }
    catch (ClassNotFoundException e) {
      System.out.println("Class not found: " + className);
    }
    return null;
  }

  public static void openFrame( Object frame) {
    boolean packFrame = false;
    if (packFrame) {
      ((JFrame)frame).pack();
    }
    else {
      ((JFrame)frame).validate();
    }
    //Center the window
    Dimension screenSize = Toolkit.getDefaultToolkit().getScreenSize();
    Dimension frameSize = ((JFrame)frame).getSize();
    if (frameSize.height > screenSize.height) {
      frameSize.height = screenSize.height;
    }
    if (frameSize.width > screenSize.width) {
      frameSize.width = screenSize.width;
    }
    ((JFrame)frame).setLocation((screenSize.width - frameSize.width) / 2, (screenSize.height - frameSize.height) / 2);
    ((JFrame)frame).setVisible(true);
  }

  static void openURL( JopSession session, String name, boolean newFrame, 
			  String bookmark) {
    System.out.println("openURL " + name);
    Object root = session.getRoot();

    // Replace any URL symbol
    name = replaceUrlSymbol( session, name);
    try {
      String url_str = null;
      if ( name.substring(0,5).equals("http:")) {
	  url_str = name;
        if ( url_str.lastIndexOf(".html") == -1 &&
	     url_str.lastIndexOf(".pdf") == -1)
          url_str = url_str + ".html";
      }
      else if ( name.startsWith("$pwr_doc/")) {
        URL current = ((JApplet) root).getDocumentBase();
        String current_str = current.toString();
        int idx1 = current_str.indexOf('/');
	if ( idx1 != -1 && current_str.length() > idx1 + 1) {
          idx1 = current_str.indexOf('/', idx1+1);
	  if ( idx1 != -1 && current_str.length() > idx1 + 1) {
	    idx1 = current_str.indexOf('/', idx1+1);
	    if ( idx1 != -1 && current_str.length() > idx1 + 1) {
	      url_str = current_str.substring(0,idx1 + 1) + "pwr_doc/" + name.substring(9);	      
	      if ( url_str.lastIndexOf(".html") == -1 &&
		   url_str.lastIndexOf(".pdf") == -1)
		url_str = url_str + ".html";
	    }
	  }
	}
      }
      else {
        URL current = ((JApplet) root).getCodeBase();
        String current_str = current.toString();
        int idx1 = current_str.lastIndexOf('/');
        int idx2 = current_str.lastIndexOf(':');
        int idx = idx1;
        if ( idx2 > idx)
          idx = idx2;
        String path = current_str.substring(0,idx + 1);
        if ( name.lastIndexOf(".html") == -1 &&
	     name.lastIndexOf(".pdf") == -1)
          url_str = new String( path + name + ".html");
        else
          url_str = new String( path + name);
        if ( bookmark != null)
	  url_str += "#" + bookmark;
      }
      System.out.println( "Opening URL: " + url_str);

      URL url = new URL( url_str);
      AppletContext appCtx = ((JApplet) root).getAppletContext();
      if ( newFrame)
        appCtx.showDocument( url, "_blank");
      else
        appCtx.showDocument( url, "_self");
    }
    catch ( MalformedURLException e) {
      System.out.println( "MalformedURL : " + name);
    }
  }

  static String replaceUrlSymbol( JopSession session, String url) {
    Gdh gdh = session.getGdh();

    CdhrObjid webConfig = gdh.getClassList( Pwrb.cClass_WebBrowserConfig);
    if ( webConfig.evenSts()) return url;

    CdhrString webName = gdh.objidToName( webConfig.objid, Cdh.mName_volumeStrict);
    if ( webConfig.evenSts()) return url;

    for ( int i = 0; i < 10; i++) {
      String attr = webName.str + ".URL_Symbols[" + i + "]";
      CdhrString attrValue = gdh.getObjectInfoString( attr);
      if ( attrValue.evenSts()) return url;

      if ( attrValue.str.equals(""))
	continue;

      StringTokenizer token = new StringTokenizer( attrValue.str);
      String symbol = "$" + token.nextToken();
      if ( !token.hasMoreTokens()) 
	continue;
  
      String value = token.nextToken();

      int idx = url.lastIndexOf( symbol);
      while ( idx != -1) {
	url = url.substring( 0, idx) + value + url.substring( idx + symbol.length());
        idx = url.lastIndexOf( symbol);
      }
    }
    return url;
  }

  //Main method
  public static void main(String[] args) {
    int op_qcom_qix = 0;
  
    try {
      UIManager.setLookAndFeel(UIManager.getSystemLookAndFeelClassName());
    }
    catch(Exception e) {
      e.printStackTrace();
    }
    if ( args.length > 0) {
      System.out.println("Queue : " + args[0]);
      try {
        op_qcom_qix = Integer.parseInt(args[0]);
      }
      catch ( NumberFormatException e) {
        System.out.println("Syntax error in que number");
	op_qcom_qix = 0;
      }
    }
    // new Gdh( (Object) null);
    new JopSpider(op_qcom_qix);
  }

  public static Image getImage( JopSession session, String image) {
    String fullName;
    if ( session.getRoot() instanceof JopApplet) {
      String name;
      try {
        URL current = ((JApplet) session.getRoot()).getCodeBase();
        String current_str = current.toString();
        int idx1 = current_str.lastIndexOf('/');
        int idx2 = current_str.lastIndexOf(':');
        int idx = idx1;
        if ( idx2 > idx)
          idx = idx2;
        String path = current_str.substring(0,idx + 1);
        String url_str;

	//        String url_str = new String( path + name);
        if ( image.substring(0,5).compareTo("jpwr/") == 0) {
          idx = image.lastIndexOf('/');
          name = image.substring(5, idx);

          url_str =  new String( "jar:" + path + "pwr_" + name + ".jar!/" + image);
	}
        else {
          idx = image.lastIndexOf('/');
          if ( idx == -1)
            name = new String(image);
          else
            name = image.substring(idx + 1);

          url_str = new String( "jar:" + path + "pwrp_" + systemName + "_web.jar!/" + name);
	}        
        System.out.println( "Opening URL: " + url_str);
        URL url = new URL( url_str);
        return Toolkit.getDefaultToolkit().getImage( url);
      }
      catch ( MalformedURLException e) {
      }
      return null;
    }
    else {
      // Add default directory /pwrp/img
      System.out.println( "Image: " + image);

      //      int idx = image.lastIndexOf('/');
      //      if ( idx == -1)
      //  fullName = new String("/pwrp/img/" + image);
      // else
        fullName = new String(image);
	//      return Toolkit.getDefaultToolkit().getImage( fullName);

      try {
        String name;
        String url_str;
        int idx;
        String path = new String("file://");
        if ( image.substring(0,5).compareTo("jpwr/") == 0) {
          idx = image.lastIndexOf('/');
          name = image.substring(5, idx);

          url_str = new String( "$pwr_lib/pwr_" + name + ".jar");
          url_str = Gdh.translateFilename( url_str);
          url_str =  new String( "jar:" + path + url_str + "!/" + image);
	}
        else {
          idx = image.lastIndexOf('/');
          if ( idx == -1)
            name = new String(image);
          else
            name = image.substring(idx + 1);

          url_str = new String( "$pwrp_lib/pwrp_" + systemName + ".jar");
	  System.out.println( "java: " + url_str);
          url_str = Gdh.translateFilename( url_str);
          url_str =  new String( "jar:" + path + url_str + "!/" + name);
	}        
        System.out.println( "Opening URL: " + url_str);
        URL url = new URL( url_str);
        return Toolkit.getDefaultToolkit().getImage( url);
      }
      catch ( MalformedURLException e) {
      }
    }
    return null;
  }
}




