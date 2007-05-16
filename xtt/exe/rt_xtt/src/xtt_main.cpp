/* 
 * Proview   $Id: xtt_main.cpp,v 1.4 2007-05-16 12:37:39 claes Exp $
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

/* rt_xtt.cpp -- Display plant and node hiererachy */

#include "flow_std.h"

#include <stdio.h>
#include <stdlib.h>

#include "pwr.h"
#include "pwr_baseclasses.h"
#include "rt_gdh.h"
#include "rt_gdh_msg.h"
#include "rt_ini_event.h"
#include "rt_qcom_msg.h"
#include "co_cdh.h"
#include "co_time.h"
#include "co_dcli.h"
#include "co_lng.h"
#include "flow.h"
#include "flow_browctx.h"
#include "flow_browapi.h"
#include "rt_xtt_main.h"
#include "rt_trace.h"
#include "xtt_xnav.h"
#include "xtt_item.h"
#include "xtt_url.h"
#include "xtt_methodtoolbar.h"
#include "co_lng.h"
#include "co_xhelp.h"
#include "co_wow.h"
#include "rt_xnav_msg.h"


// Static variables
Xtt *Xtt::hot_xtt = 0;

xnav_sStartMenu Xtt::alarm_menu[] = {
	{ "Alarm List", xnav_eItemType_Command,	menu_ePixmap_List, (void *) "show alarm"},
	{ "Event List", xnav_eItemType_Command,	menu_ePixmap_List,	(void *) "show event"},
	{ "Blocked Alarms", xnav_eItemType_Command,	menu_ePixmap_List,	(void *) "show block"},
	{ "Historical List", xnav_eItemType_Command, menu_ePixmap_List,	(void *) "show hist"},
	{ "", 0, 0, NULL}};

xnav_sStartMenu Xtt::nethandler_menu[] = {
	{ "Link", xnav_eItemType_Command, menu_ePixmap_Map, (void *) "show link"},
	{ "Subscription Client", xnav_eItemType_Command, menu_ePixmap_Map, (void *) "show subcli"},
	{ "Subscription Server", xnav_eItemType_Command, menu_ePixmap_Map, (void *) "show subsrv"},
	{ "", 0, 0, NULL}};
xnav_sStartMenu Xtt::communication_menu[] = {
	{ "RemNode", xnav_eItemType_Command,	menu_ePixmap_Map, (void *) "show remnode"},
	{ "RemTrans", xnav_eItemType_Command,	menu_ePixmap_Map, (void *) "show remtrans"},
	{ "", 0, 0, NULL}};
xnav_sStartMenu Xtt::logging_menu[] = {
	{ "Logging entry 1", xnav_eItemType_Command,	menu_ePixmap_Map, (void *) "show logging/entry=1"},
	{ "Logging entry 2", xnav_eItemType_Command,	menu_ePixmap_Map, (void *) "show logging/entry=2"},
	{ "Logging entry 3", xnav_eItemType_Command,	menu_ePixmap_Map, (void *) "show logging/entry=3"},
	{ "Logging entry 4", xnav_eItemType_Command,	menu_ePixmap_Map, (void *) "show logging/entry=4"},
	{ "Logging entry 5", xnav_eItemType_Command,	menu_ePixmap_Map, (void *) "show logging/entry=5"},
	{ "Logging entry 6", xnav_eItemType_Command,	menu_ePixmap_Map, (void *) "show logging/entry=6"},
	{ "Logging entry 7", xnav_eItemType_Command,	menu_ePixmap_Map, (void *) "show logging/entry=7"},
	{ "Logging entry 8", xnav_eItemType_Command,	menu_ePixmap_Map, (void *) "show logging/entry=8"},
	{ "Logging entry 9", xnav_eItemType_Command,	menu_ePixmap_Map, (void *) "show logging/entry=9"},
	{ "Logging entry 10", xnav_eItemType_Command,	menu_ePixmap_Map, (void *) "show logging/entry=10"},
	{ "", 0, 0, NULL}};
xnav_sStartMenu Xtt::system_menu[] = {
	{ "Nethandler", xnav_eItemType_Menu,	menu_ePixmap_Map, 	(void *)&Xtt::nethandler_menu},
	{ "Communication", xnav_eItemType_Menu,	menu_ePixmap_Map, 	(void *)&Xtt::communication_menu},
	{ "Device", 	xnav_eItemType_Command,	menu_ePixmap_Map, 	(void *) "show device"},
	{ "PlcThread", 	xnav_eItemType_Command,	menu_ePixmap_Map, 	(void *) "show plcthread"},
	{ "PlcPgm", 	xnav_eItemType_Command,	menu_ePixmap_Map,	(void *) "show plcpgm"},
	{ "Logging", 	xnav_eItemType_Menu,	menu_ePixmap_Map,	(void *)&Xtt::logging_menu},
	{ "System Messages", xnav_eItemType_Command, menu_ePixmap_List,	(void *) "open consolelog"},
	{ "System Status", 	xnav_eItemType_Command,	menu_ePixmap_Map,	(void *) "show nodeinfo"},
	{ "Nodes", 	xnav_eItemType_Command,	menu_ePixmap_Map,	(void *) "show nodeobjects"},
	{ "Volumes", 	xnav_eItemType_Command,	menu_ePixmap_Map,	(void *) "show volumes"},
	{ "", 0, 0, NULL}};
xnav_sStartMenu Xtt::root_menu[] = {
	{ "Database", 	xnav_eItemType_Command,	menu_ePixmap_Map, 	(void *) "show database"},
	{ "Alarm", 	xnav_eItemType_Menu,	menu_ePixmap_Map, 	(void *)&Xtt::alarm_menu},
	{ "Store",	xnav_eItemType_Command,	menu_ePixmap_Map, 	(void *) "show file"},
	{ "System",	xnav_eItemType_Menu,	menu_ePixmap_Map, 	(void *)&Xtt::system_menu},
	{ "Close", 	xnav_eItemType_Command,	menu_ePixmap_Leaf,	(void *) "exit"},
	{ "", 0, 0, NULL}};
  


void Xtt::hotkey_activate_command( char *cmdp)
{
  pwr_tCmd cmd;

  strcpy( cmd, cmdp);
  if ( xnav)
    xnav->command( cmd);
}

void Xtt::hotkey_activate_toggledig( char *namep)
{
  pwr_tBoolean  value;
  pwr_tAName   	name;
  int		sts;

  dcli_remove_blank( name, namep);
  if ( !strchr(name, '.'))
    strcat( name, ".ActualValue");

  sts = gdh_GetObjectInfo( name, &value, sizeof(value));
  if (EVEN(sts))  {
    printf("rt_xtt hotkey: ToggleDig. Can't get %s\n", name);
    return;
  }

  value = !value;
  sts = gdh_SetObjectInfo( name, &value, sizeof(value));
  if (EVEN(sts))
    printf("rt_xtt hotkey: ToggleDig. Can't get %s\n", name);
}

void Xtt::hotkey_activate_setdig( char *namep)
{
  pwr_tBoolean  value = 1;
  pwr_tAName   	name;
  int		sts;


  dcli_remove_blank( name, namep);
  if ( !strchr(name, '.'))
    strcat( name, ".ActualValue");

  sts = gdh_SetObjectInfo( name, &value, sizeof(value));
  if (EVEN(sts))
    printf("rt_xtt hotkey: SetDig. Can't get %s\n", name);
}

void Xtt::hotkey_activate_resetdig( char *namep)
{
  pwr_tBoolean  value = 0;
  pwr_tAName   	name;
  int		sts;

  dcli_remove_blank( name, namep);
  if ( !strchr(name, '.'))
    strcat( name, ".ActualValue");

  sts = gdh_SetObjectInfo( name, &value, sizeof(value));
  if (EVEN(sts))
    printf("rt_xtt hotkey: SetDig. Can't get %s\n", name);
}

void Xtt::open_URL_cb( void *ctx, char *url)
{
  xnav_open_URL( url);
}

void Xtt::qcom_events( void *data)
{
  Xtt *xtt = (Xtt *)data;
  char mp[2000];
  qcom_sQid qid = qcom_cNQid;
  qcom_sGet get;
  static int swap = 0;
  pwr_tStatus sts = 1;

  while ( ODD(sts)) {
    get.maxSize = sizeof(mp);
    get.data = mp;
    qcom_Get( &sts, &xtt->queid, &get, 0);
    if ( !(sts == QCOM__TMO || sts == QCOM__QEMPTY)) {
      ini_mEvent  new_event;
      qcom_sEvent *ep = (qcom_sEvent*) get.data;

      new_event.m  = ep->mask;
      if (new_event.b.oldPlcStop && !swap) {
	swap = 1;
	xtt->xnav->swap(0);
      } else if (new_event.b.swapDone && swap) {
	swap = 0;
	xtt->xnav->swap(1);
      } else if (new_event.b.terminate) {
	exit(0);
      }
    }
  }
  xtt->timerid->add( 1000, qcom_events, xtt);
}

void Xtt::findregex_ok( Xtt *xtt, char *search_str)
{
  int sts;

  sts = xtt->xnav->search( search_str, 1);
  if ( EVEN(sts))
    xtt->message( 'E', xtt->xnav->get_message( sts));
}

void Xtt::find_ok( Xtt *xtt, char *search_str)
{
  int sts;

  sts = xtt->xnav->search( search_str, 0);
  if ( EVEN(sts))
    xtt->message( 'E', xtt->xnav->get_message( sts));
}

void Xtt::xtt_message_cb( void *ctx, char severity, char *msg)
{
  ((Xtt *)ctx)->message( severity, msg);
}

void Xtt::close_ok( void *ctx, void *data)
{
  Xtt	*xtt = (Xtt *) ctx;

  delete xtt->xnav;
  exit(0);
}

void Xtt::change_value( void *ctx)
{
  Xtt *xtt = (Xtt *) ctx;

  xtt->open_change_value();
}

void Xtt::selection_changed( void *ctx)
{
  Xtt *xtt = (Xtt *) ctx;

  xtt->methodtoolbar->set_sensitive();
}

//
//  Callbackfunctions from menu entries
//
void Xtt::activate_print()
{
  pwr_tFileName filename;
  pwr_tCmd cmd;

  dcli_translate_filename( filename, "$pwrp_tmp/xnav.ps");
  xnav->print( filename);

  sprintf( cmd, "$pwr_exe/rt_print.sh %s", filename);
  system(cmd);
}

void Xtt::activate_find()
{
  open_input_dialog( "Search string", "Find object",
		     "", &find_ok);
}

void Xtt::activate_findregex()
{
   open_input_dialog( "Search string", "Find regular expression",
		      "", &findregex_ok);
}

void Xtt::activate_findnext()
{
  int sts;

  sts = xnav->search_next();
  if ( EVEN(sts))
    message( 'E', xnav->get_message( sts));
}

void Xtt::activate_collapse()
{
  xnav->brow_push_all();
}

void Xtt::activate_openobject()
{
  int		sts;
  int		is_attr;
  pwr_sAttrRef	attrref;

  sts = xnav->get_select( &attrref, &is_attr);
  if ( ODD(sts))
    xnav->open_object( &attrref);
}

void Xtt::activate_openplc()
{
  xnav->start_trace_selected();
}

void Xtt::activate_opengraph()
{
  int		sts;
  pwr_tClassId	classid;
  pwr_tAName   	name;
  pwr_tAName   	vname;
  pwr_tFileName	filename;
  int		is_attr;
  pwr_sAttrRef	attrref;
  pwr_tCmd     	cmd;

  sts = xnav->get_select( &attrref, &is_attr);
  if ( EVEN(sts)) {
    // If collect brow open collect graph
    if ( xnav->brow->ctx == xnav->collect_brow->ctx) {   
      sprintf( cmd, "open graph /collect");
      xnav->command( cmd);
    }
    return;
  }

  sts = gdh_AttrrefToName( &attrref, name, sizeof(name), cdh_mNName);
  if ( EVEN(sts)) return;

  sts = gdh_AttrrefToName( &attrref, vname, sizeof(vname), 
			   cdh_mName_volumeStrict);
  if ( EVEN(sts)) return;

  sts = gdh_GetAttrRefTid( &attrref, &classid);
  if ( EVEN(sts)) return;

  if ( cdh_tidIsCid( classid)) {
    if ( classid == pwr_cClass_DsTrend || classid == pwr_cClass_PlotGroup) {
      // Open trend
      sprintf( cmd, "open trend /name=%s /title=\"%s\"", vname, name);
      xnav->command( cmd);

      return;
    }
    sprintf( cmd, "open graph /class/inst=%s/name=\"%s\"", vname, name);
    xnav->command( cmd);
  }
  else {
    switch( classid) {
    case pwr_eType_Float32:
      sprintf( filename, "pwr_exe:pwr_t_float32.pwg");
      break;
    case pwr_eType_Int32:
      sprintf( filename, "pwr_exe:pwr_t_int32.pwg");
      break;
    case pwr_eType_Boolean:
      sprintf( filename, "pwr_exe:pwr_t_boolean.pwg");
      break;
    default:
      message( 'E', "No graph for this attribute type");
      return;
    }
    sprintf( cmd, "open graph %s/inst=%s/name=\"%s\"", filename, vname, name);
    xnav->command( cmd);
  }
}

void Xtt::activate_collect_insert()
{
  pwr_sAttrRef	attrref;
  int		is_attr;
  int		sts;

  sts = xnav->get_select( &attrref, &is_attr);
  if ( ODD(sts))
    xnav->collect_insert( &attrref);
}

void Xtt::activate_advanceduser()
{
  char cmd[80];

  strcpy( cmd, "set advanceduser");
  xnav->command( cmd);
}

void Xtt::activate_zoom_in()
{
  double zoom_factor;

  xnav->get_zoom( &zoom_factor);
  if ( zoom_factor > 40)
    return;

   xnav->zoom( 1.18);
}

void Xtt::activate_zoom_out()
{
  double zoom_factor;

  xnav->get_zoom( &zoom_factor);
  if ( zoom_factor < 15)
    return;

  xnav->zoom( 1.0 / 1.18);
}

void Xtt::activate_method( char *method, char *filter)
{
  int		sts;
  int		is_attr;
  pwr_sAttrRef	aref;
  xmenu_eItemType menu_type;

  sts = xnav->get_select( &aref, &is_attr);

  if ( aref.Flags.b.Object)
    menu_type = xmenu_eItemType_Object;
  else if ( aref.Flags.b.ObjectAttr)
    menu_type = xmenu_eItemType_AttrObject;
  else
    menu_type = xmenu_eItemType_Attribute;

  if ( ODD(sts))
    xnav->call_method( method, filter, aref,
		       menu_type,
		       xmenu_mUtility_XNav,
		       xnav->priv, 0);
}

void Xtt::activate_help()
{
  CoXHelp::dhelp("overview", "", navh_eHelpFile_Base, NULL, 0);
}

void Xtt::activate_help_project()
{
  CoXHelp::dhelp("index", "", navh_eHelpFile_Project, NULL, 0);
}

void Xtt::activate_help_proview()
{
  CoXHelp::dhelp( "version", "", navh_eHelpFile_Other, "$pwr_exe/xtt_version_help.dat", 0);
}

Xtt::Xtt( int argc, char *argv[], int *return_sts) :
  root_item(0), input_open(0), command_open(0), india_ok_cb(0), quiet(0), attach_audio(0), select_opplace(0), op_close_button(0)
{
  pwr_tStatus	sts;
  int		i;
  int	opplace_found = 0;
  pwr_tObjid op_objid;
  qcom_sQattr qAttr;
  qcom_sQid qini;

  strcpy( opplace_str, "");
  hot_xtt = this;

  errh_Init("rt_xtt", errh_eNAnix);

  sts = gdh_Init("rt_xtt");
  if (EVEN(sts)) {
    *return_sts = sts;
    return;
  }

  if (!qcom_Init(&sts, 0, "rt_xtt")) {
    *return_sts = sts;
    return;
  } 

  qAttr.type = qcom_eQtype_private;
  qAttr.quota = 100;
  if (!qcom_CreateQ(&sts, &queid, &qAttr, "events")) {
    *return_sts = sts;
    return;
  } 

  qini = qcom_cQini;
  if (!qcom_Bind(&sts, &queid, &qini)) {
    *return_sts = sts;
    return;
  }

  // Set language
  for ( i = 1; i < argc; i++) {
    if ( strcmp( argv[i], "-l") == 0 && i + 1 < argc)
      Lng::set( argv[i+1]);
    else if ( strcmp( argv[i], "-q") == 0) {
      CoWow::HideWarranty();
      quiet = 1;
    }
    else if ( strcmp( argv[i], "-a") == 0)
      attach_audio = 1;
    else if ( strcmp( argv[i], "-s") == 0)
      select_opplace = 1;
    else if ( strcmp( argv[i], "-c") == 0)
      op_close_button = 1;
    else if ( strcmp( argv[i], "-u") == 0 && i + 1 < argc) {
      char oname[80];

      strcpy( opplace_str, argv[i+1]);

      sts = gdh_GetClassList( pwr_cClass_OpPlace, &op_objid);
      while (ODD(sts)) {
	sts = gdh_ObjidToName( op_objid, oname, sizeof(oname), cdh_mName_object);
	if (ODD(sts) && cdh_NoCaseStrcmp( oname, opplace_str) == 0) {
	  sts = gdh_ObjidToName( op_objid, opplace_str, sizeof(opplace_str), cdh_mNName);
	  opplace_found = 1;
	  break;
	}
	sts = gdh_GetNextObject( op_objid, &op_objid);
      }
      if ( !opplace_found) {
	printf("** Unable to find opplace\n");
	exit(sts);
      }
    }
  }

  if ( !opplace_found && argc >= 2 && strncmp( argv[1], "-", 1) != 0) {
    pwr_tClassId op_class;

    // First argument is opplace object
    strcpy( opplace_str, argv[1]);

    sts = gdh_NameToObjid( opplace_str, &op_objid);
    if ( EVEN(sts)) {
      printf("** Unable to find opplace\n");
      exit(sts);
    }

    sts = gdh_GetObjectClass( op_objid, &op_class);
    if ( EVEN(sts)) exit( sts);

    if ( op_class != pwr_cClass_OpPlace) {
      printf("** Error in opplace object class\n");
      exit(sts);
    }

    opplace_found = 1;
  }
}

void Xtt::opplace_selected_cb( void *ctx, char *text)
{
  Xtt *xtt = (Xtt *)ctx;
  pwr_tCmd cmd;

  sprintf( cmd, "open operator %s", text);  
  if ( xtt->op_close_button)
    strcat( cmd, " /closebutton");
  xtt->xnav->command( cmd);
}

void Xtt::list_opplace()
{
  char texts[20][80];
  pwr_tStatus sts;
  pwr_tOid oid;

  int i = 0;
  for ( sts = gdh_GetClassList( pwr_cClass_OpPlace, &oid); 
	ODD(sts);
	sts = gdh_GetNextObject( oid, &oid)) {
    sts = gdh_ObjidToName( oid, texts[i], sizeof(texts[0]), cdh_mNName);
    if ( EVEN(sts)) continue;
    
    i++;
    if ( i == (int)(sizeof(texts)/sizeof(texts[0]) - 2))
      break;
  }
  strcpy( texts[i], "");

  if ( i == 0) {
    printf( "No opplace objects found\n");
    exit(0);
  }

  if ( i == 1) {
    // Only one opplace found, open it
    pwr_tCmd cmd;

    sprintf( cmd, "open operator %s", texts[0]);  
    xnav->command( cmd);
  }
  else 
    // Select oplace from list
    wow->CreateList( "Select Operator Place", (char *)texts, opplace_selected_cb, this);
}




