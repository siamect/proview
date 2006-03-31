/* 
 * Proview   $Id: ge_graph_web.cpp,v 1.8 2006-03-31 14:34:12 claes Exp $
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
 **/

/* ge_graph_web.cpp -- generate website */

#include "glow_std.h"

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>


extern "C" {
#include "co_cdh.h"
#include "co_time.h"
#include "co_ccm_msg.h"
#include "co_dcli.h"
#include "co_cnf.h"
#include "pwr_baseclasses.h"
#include "rt_gdh.h"
}

#include <Xm/Xm.h>
#include <Xm/XmP.h>
#include <Xm/Text.h>
#include <Mrm/MrmPublic.h>
#include <X11/Intrinsic.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>

#include "glow.h"
#include "glow_growctx.h"
#include "glow_growapi.h"
#include "glow_growwidget.h"

#include "ge_graph.h"
#include "ge_util.h"
#include "ge_msg.h"

extern "C" {
#if LDH
#include "wb_ldh.h"
#include "wb_trv.h"
#endif
}

typedef enum {
      graph_eWebTarget_RightFrame      = 0,
      graph_eWebTarget_ParentWindow    = 1,
      graph_eWebTarget_SeparateWindow  = 2
    } graph_eWebTarget;

#if LDH
static int graph_get_applet_size( char *graphname, int *width, int *height);
#endif

#if LDH
static int graph_webhandler_insert( pwr_sAttrRef *aref, void *o_p,
				    void *c_p, void *dum1, void *dum2, void *dum3)
{
  int *count = (int *)c_p;
  pwr_tObjid *objid_p = (pwr_tObjid *) o_p;
  

  if ( *count == 0)
    *objid_p = aref->Objid;

  (*count)++;
  return 1;
}
#endif

int Graph::generate_web( ldh_tSesContext ldhses)
{
#if LDH
  unsigned long	node_count;
  pwr_tObjid	*nodelist;
  pwr_tObjid    node_objid;
  int           sts;
  int           size;
  pwr_tObjid    webhandler_objid;
  int           webhandler_cnt = 0;
  pwr_tClassId  classid;
  pwr_tObjid    webgraph_objid;
  pwr_tObjid    weblink_objid;
  pwr_tClassId  class_vect[2];
  char          *value_p;
  pwr_tString80 title;
  pwr_tString80 text;
  pwr_tString80 file_name;
  pwr_tBoolean  enable_login;
  pwr_tBoolean  enable_alarmlist;
  pwr_tBoolean  enable_eventlist;
  pwr_tBoolean  enable_navigator;
  char          style_sheet[80];
  char          start_URL[80];
  char          name[80];
  char          graph_name[80];
  char          graph_text[80];
  char          link_URL[80];
  char          link_text[80];
  char          *s;
  ofstream      fp_login;
  ofstream      fp_start;
  ofstream      fp_menu;
  ofstream      fp_ow;
  ofstream      fp_ows;
  char          fname[120];
  char          menu_file_name[80];
  char          codebase[200];
  pwr_tEnum     web_target;
  char          target_str[80];
  char          sname[80];

  ge_get_systemname( sname);

  // Get codebase for applets from global config file
  cnf_get_value( "appletCodebase", codebase);

  // Find the WebHandler object
  
  sts = trv_get_rtnodes( ldhses, &node_count, &nodelist);
  if ( ODD(sts))
  {
    if ( node_count > 0)
    {
      node_objid = *nodelist;
      XtFree( (char *)nodelist);
    }
    else
      sts = 0;
  }
  if ( EVEN(sts)) {
    return GE__NONODEOBJECT;
  }

  class_vect[0] = pwr_cClass_WebHandler;
  class_vect[1] = 0;
  sts = trv_get_objects_hcn( ldhses, node_objid, class_vect,
	    NULL, graph_webhandler_insert, (void *)&webhandler_objid, 
            (void *)&webhandler_cnt,0,0,0);
  if ( EVEN(sts) || webhandler_cnt == 0) {
    return GE__NOWEBHANDLER;
  }
  if ( webhandler_cnt > 1) {
    return GE__WEBHANDLERS;
  }

  // Get WebHandler data

  // Attribute FileName
  sts = ldh_GetObjectPar( ldhses, webhandler_objid, "RtBody", "FileName",
			  &value_p, &size);
  if (EVEN(sts)) return sts;
  strcpy( file_name, value_p);
  XtFree( value_p); 

  // Attribute Title
  sts = ldh_GetObjectPar( ldhses, webhandler_objid, "RtBody", "Title",
			  &value_p, &size);
  if (EVEN(sts)) return sts;
  strcpy( title, value_p);
  XtFree( value_p); 

  // Attribute Text
  sts = ldh_GetObjectPar( ldhses, webhandler_objid, "RtBody", "Text",
			  &value_p, &size);
  if (EVEN(sts)) return sts;
  strcpy( text, value_p);
  XtFree( value_p); 

  // Attribute EnableLogin
  sts = ldh_GetObjectPar( ldhses, webhandler_objid, "RtBody", "EnableLogin",
			  &value_p, &size);
  if (EVEN(sts)) return sts;
  enable_login = *(pwr_tBoolean *)value_p;
  XtFree( value_p); 

  // Attribute EnableAlarmList
  sts = ldh_GetObjectPar( ldhses, webhandler_objid, "RtBody", "EnableAlarmList",
			  &value_p, &size);
  if (EVEN(sts)) return sts;
  enable_alarmlist = *(pwr_tBoolean *)value_p;
  XtFree( value_p); 

  // Attribute EnableEventList
  sts = ldh_GetObjectPar( ldhses, webhandler_objid, "RtBody", "EnableEventList",
			  &value_p, &size);
  if (EVEN(sts)) return sts;
  enable_eventlist = *(pwr_tBoolean *)value_p;
  XtFree( value_p); 

  // Attribute EnableNavigator
  sts = ldh_GetObjectPar( ldhses, webhandler_objid, "RtBody", "EnableNavigator",
			  &value_p, &size);
  if (EVEN(sts)) return sts;
  enable_navigator = *(pwr_tBoolean *)value_p;
  XtFree( value_p); 

  // Attribute StyleSheet
  sts = ldh_GetObjectPar( ldhses, webhandler_objid, "RtBody", "StyleSheet",
			  &value_p, &size);
  if (EVEN(sts)) return sts;
  strcpy( style_sheet, value_p);
  XtFree( value_p); 

  // Attribute StartURL
  sts = ldh_GetObjectPar( ldhses, webhandler_objid, "RtBody", "StartURL",
			  &value_p, &size);
  if (EVEN(sts)) return sts;
  strcpy( start_URL, value_p);
  XtFree( value_p); 

  // Login applet as default start URL
  if ( strcmp( start_URL, "") == 0 && enable_login)
    strcpy( start_URL, "pwr_login.html");

  // pwr_css as default css
  if ( strcmp( style_sheet, "") == 0)
    strcpy( style_sheet, "pwr_css.css");

  // Parse the name of the start page
  if ( (s = strrchr( file_name, '/')) ||
       (s = strrchr( file_name, '<')) ||
       (s = strrchr( file_name, ':')))
    strcpy( name, s+1);
  else
    strcpy( name, file_name);

  if ( (s = strrchr( name, '.')))
    *s = 0;

  strcpy( menu_file_name, name);
  strcat( menu_file_name, "_menu.html");

  // Generate html-file for login applet
  strcpy( fname, "$pwrp_web/pwr_login.html");
  dcli_translate_filename( fname, fname);

  fp_login.open( fname);

  fp_login <<
"<!--     Generated by Ge         -->" << endl <<
"<!--     Do not edit this file   -->" << endl <<
"</html>" << endl <<
"  <head>" << endl <<
"    <title>Login</title>" << endl <<
"  </head>" << endl <<
"  <body>" << endl <<
"" << endl <<
"    <object classid=\"clsid:8AD9C840-044E-11D1-B3E9-00805F499D93\"" << endl <<
"      width=300 height=120  codebase=\"" << codebase << "\">" << endl <<
"      <param name = code value=jpwr.jop.JopLoginApplet.class >" << endl <<
"      <param name =\"archive\" value=\"pwr_rt_client.jar,pwr_jop.jar,pwr_jopc.jar\">" << endl <<
"      <param name=\"type\" value=\"application/x-java-applet;version=1.3\">" << endl <<
"      <param name=\"scriptable\" value=\"false\">" << endl <<
"  </body>" << endl <<
"</html>" << endl;
  fp_login.close();

  // Generate html-file for start page

  sprintf( fname, "$pwrp_web/%s.html", name);
  dcli_translate_filename( fname, fname);

  fp_start.open( fname);

  fp_start <<
"<! Generated by Ge >" << endl <<
"<html>" << endl <<
"  <head>" << endl <<
"    <title>" << title << "</title>" << endl <<
"  </head>" << endl <<
"" << endl <<
"  <frameset cols=\"20%,80%\">" << endl <<
"    <frame name=\"left\" src=\"" << menu_file_name << "\">" << endl;
  if ( strcmp( start_URL, "") != 0)
    fp_start <<
"    <frame name=\"right\" src=\"" << start_URL << "\">" << endl;
  else
    fp_start <<
"    <frame NAME=\"right\">" << endl;
  fp_start <<
"  </frameset>" << endl <<
"" << endl <<
"</html>" << endl;
  fp_start.close();

  // Generate menu file

  sprintf( fname, "$pwrp_web/%s", menu_file_name);
  dcli_translate_filename( fname, fname);

  fp_menu.open( fname);

  fp_menu <<
"<html>" << endl <<
"  <head>" << endl <<
"    <link rel=\"stylesheet\" type=\"text/css\" href=\"" << style_sheet << "\">" << endl <<
"    <title>Menu</title>" << endl <<
"  </head>" << endl <<
"  <body>" << endl <<
"    <h1>" << title << "</h1><br>" << endl <<
"    <h2>" << text << "</h2>" << endl <<
"    <hr>" << endl <<
"" << endl;

  if ( enable_login) {
    fp_menu <<
"    <a href=\"pwr_login.html\" target=\"right\">Login</a><br>" << endl;
  }

  if ( enable_navigator) {
    fp_menu <<
"    <a href=\"javascript:open_applet('pwr_navigator.html','Navigator',1,400,600)\">Navigator</a><br>" << endl;
  }

  if ( enable_alarmlist) {
    fp_menu <<
"    <a href=\"javascript:open_applet('pwr_events.html','AlarmandEventlist',1,600,600)\">Alarm and Event list</a><br>" << endl;
  }

  if ( enable_login || enable_navigator) {
    fp_menu <<
"    <hr>" << endl;
  }

  // Get all WebGraph objects
  sts = ldh_GetChild( ldhses, webhandler_objid, &webgraph_objid);
  while( ODD(sts)) {

    sts = ldh_GetObjectClass( ldhses, webgraph_objid, &classid);
    if ( EVEN(sts)) return sts;

    if ( classid == pwr_cClass_WebGraph) {

      // Attribute Name
      sts = ldh_GetObjectPar( ldhses, webgraph_objid, "RtBody", "Name",
			  &value_p, &size);
      if (EVEN(sts)) return sts;
      cdh_ToLower( graph_name, value_p);
      XtFree( value_p); 

        // Attribute WebTarget
      sts = ldh_GetObjectPar( ldhses, webgraph_objid, "RtBody", "WebTarget",
			  &value_p, &size);
      if (EVEN(sts)) return sts;
      web_target = *(pwr_tEnum *) value_p;
      XtFree( value_p); 

      if ( (s == strrchr( graph_name, '.')))
        *s = 0;

      // Attribute Text
      sts = ldh_GetObjectPar( ldhses, webgraph_objid, "RtBody", "Text",
			  &value_p, &size);
      if (EVEN(sts)) return sts;
      strcpy( graph_text, value_p);
      XtFree( value_p); 

      switch ( web_target) {
        case graph_eWebTarget_ParentWindow :
          strcpy( target_str, "_parent");
          fp_menu <<
"    <a href=\"" << graph_name << ".html\" target=\"" << target_str << "\">" << graph_text << "</a><br>" << endl;
          break;
        case graph_eWebTarget_SeparateWindow :
	{
          int width, height;
          int resize = 0;

          strcpy( target_str, "_blank");
          sts = graph_get_applet_size( graph_name, &width, &height);
          if ( EVEN(sts)) {
            width = height = 700;
            resize = 1;
          }

          fp_menu <<
"    <a href=\"javascript:open_applet('" << graph_name << ".html','" << 
graph_text << "'," << resize << "," << width+20 << "," << height+20 
<< ")\">" << graph_text << "</a><br>" << endl;
          break;
        }
        default:
          strcpy( target_str, "right");
          fp_menu <<
"    <a href=\"" << graph_name << ".html\" target=\"" << target_str << "\">" << graph_text << "</a><br>" << endl;
      }

    }
    sts = ldh_GetNextSibling( ldhses, webgraph_objid, &webgraph_objid);
  }

  fp_menu <<
"    <hr>" << endl <<
"" << endl;

  // Get all WebLink objects
  sts = ldh_GetChild( ldhses, webhandler_objid, &weblink_objid);
  while( ODD(sts)) {

    sts = ldh_GetObjectClass( ldhses, weblink_objid, &classid);
    if ( EVEN(sts)) return sts;

    if ( classid == pwr_cClass_WebLink) {

      // Attribute URL
      sts = ldh_GetObjectPar( ldhses, weblink_objid, "RtBody", "URL",
			  &value_p, &size);
      if (EVEN(sts)) return sts;
      strcpy( link_URL, value_p);
      XtFree( value_p); 

      // Attribute Text
      sts = ldh_GetObjectPar( ldhses, weblink_objid, "RtBody", "Text",
			  &value_p, &size);
      if (EVEN(sts)) return sts;
      strcpy( link_text, value_p);
      XtFree( value_p); 

      // Attribute WebTarget
      sts = ldh_GetObjectPar( ldhses, weblink_objid, "RtBody", "WebTarget",
			  &value_p, &size);
      if (EVEN(sts)) return sts;
      web_target = *(pwr_tEnum *) value_p;
      XtFree( value_p); 

      switch ( web_target) {
        case graph_eWebTarget_ParentWindow :
          strcpy( target_str, "_parent");
          break;
        case graph_eWebTarget_SeparateWindow :
          strcpy( target_str, "_blank");
          break;
        default:
          strcpy( target_str, "right");
      }

      fp_menu <<
"    <a href=\"" << link_URL << "\" target=\"" << target_str << "\">" << link_text << "</a><br>" << endl;
 
    }
    sts = ldh_GetNextSibling( ldhses, weblink_objid, &weblink_objid);
  }

  fp_menu <<
"  </body>" << endl <<
"</html>" << endl;

  // Function for opening a window without menues and toolbar
  fp_menu <<
"<script language=\"JavaScript\">" << endl <<
"function open_applet(url,name,resizable,width,height)" << endl <<
"{" << endl <<
"  var win = window.open(url, name, 'resizable='+resizable+',menubar=no,scrollbars=no,location=no,toolbar=no,width='+width+',height='+height+',directories=no');" << endl <<
"}" << endl <<
"</script>" << endl;
  fp_menu.close();

  // Generate html-file for opwindow applet

  sprintf( fname, "$pwrp_web/%s_opwin_menu.html", name);
  dcli_translate_filename( fname, fname);

  fp_ow.open( fname);

  fp_ow <<
"<!--     Generated by Ge         -->" << endl <<
"<!--     Do not edit this file   -->" << endl <<
"</html>" << endl <<
"  <head>" << endl <<
"    <title>" << title << "</title>" << endl <<
"  </head>" << endl <<
"  <body>" << endl <<
"    <object classid=\"clsid:8AD9C840-044E-11D1-B3E9-00805F499D93\"" << endl <<
"      width=100% height=100%  codebase=\"" << codebase << "\">" << endl <<
"      <param name = code value=jpwr.jop.JopOpWindowApplet.class >" << endl <<
    "      <param name =\"archive\" value=\"pwr_rt_client.jar,pwr_jop.jar,pwr_jopc.jar,pwrp_" << sname << "_web.jar\">" << endl <<
"      <param name=\"type\" value=\"application/x-java-applet;version=1.3\">" << endl <<
"      <param name=\"scriptable\" value=\"false\">" << endl <<
"    <embed type=\"application/x-java-applet;version=1.4\" " << endl <<
"      code = jpwr.jop.JopOpWindowApplet.class " << endl <<
    "      archive=\"pwr_jop.jar,pwr_jopc.jar,pwr_rt_client.jar,pwrp_" << sname << "_web.jar\" " << endl <<
"      width = 100% height = 100% scriptable=false " << endl <<
"      pluginspage=\"http://java.sun.com/products/plugin/index.html#download\">" << endl <<
"  </body>" << endl <<
"</html>" << endl;
  fp_ow.close();

  // Generate html-file for start page for opwindow applet

  sprintf( fname, "$pwrp_web/%s_opwin.html", name);
  dcli_translate_filename( fname, fname);

  fp_ows.open( fname);

  fp_ows <<
"<!--     Generated by Ge         -->" << endl <<
"<!--     Do not edit this file   -->" << endl <<
"<html>" << endl <<
"  <head>" << endl <<
"    <title>" << title << "</title>" << endl <<
"  </head>" << endl <<
"  <frameset cols=\"242,*\">" << endl <<
"    <frame name=\"left\" src=\""  << name << "_opwin_menu.html\">" << endl <<
"    <frame name=\"right\" src=\"xtt_help_index.html\">" << endl <<
"  </frameset>" << endl <<
"</html>" << endl;

  fp_ows.close();

  printf( "-- Web startpages generated $pwrp_web/%s.html and %s_opwin.html\n", name, name);
#endif
  return 1;
}

int Graph::generate_web_help()
{
  pwr_tStatus sts;
  pwr_tFileName fname;

  // Copy default css-file to pwrp_web
  system( "cp $pwr_exe/pwr_css.css $pwrp_web/");

  // If $pwrp_exe/xtt_help.dat exist convert this to html
  sts = dcli_search_file( "$pwrp_exe/xtt_help.dat", fname,
			  DCLI_DIR_SEARCH_INIT);
  if ( ODD(sts)) {
    system( "co_convert -d $pwrp_web -t $pwrp_exe/xtt_help.dat");
  }
  sts = dcli_search_file( "$pwrp_exe/xtt_help.dat", fname,
			  DCLI_DIR_SEARCH_END);
  return 1;
}


//
//  Get width and height for applet from generated html-file
//
#if LDH
static int graph_get_applet_size( char *graphname, int *width, int *height) 
{
  char fname[200];
  FILE *file;
  int w, h;
  int width_found = 0;
  int height_found = 0;
  char *s;
  char line[400];

  sprintf( fname, "$pwrp_web/%s.html", graphname);
  dcli_translate_filename( fname, fname);

  file = fopen( fname, "r");
  if ( !file)
    return GE__NOFILE;

  while( dcli_read_line( line, sizeof(line), file)) {
    if ( width_found && height_found)
      break;
    if ( (s = strstr( line, "width="))) {
      if ( sscanf( s+6, "%d", &w) == 1)
        width_found = 1;
      continue;
    }
    if ( (s = strstr( line, "height="))) {
      if ( sscanf( s+7, "%d", &h) == 1)
        height_found = 1;
      continue;
    }
  }
  fclose( file);
  if ( width_found && height_found) {
    *width = w;
    *height = h;
    return GE__SUCCESS;
  }
  return GE__NOSIZE;
}
#endif




