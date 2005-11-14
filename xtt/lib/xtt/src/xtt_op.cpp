/* 
 * Proview   $Id: xtt_op.cpp,v 1.7 2005-11-14 16:13:19 claes Exp $
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

/* xtt_op.cpp -- Alarm and event window in xtt */

#include "glow_std.h"

#include <stdio.h>
#include <stdlib.h>

extern "C" {
#include "co_cdh.h"
#include "co_time.h"
#include "pwr_baseclasses.h"
#include "rt_gdh.h"
#include "rt_mh.h"
#include "rt_mh_outunit.h"
}

#include <Xm/Xm.h>
#include <Xm/MwmUtil.h>
#include <Xm/XmP.h>
#include <Xm/Text.h>
#include <Xm/PushB.h>
#include <Mrm/MrmPublic.h>
#include <X11/Intrinsic.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>

extern "C" {
#include "flow_x.h"
#include "co_wow.h"
}
#include "co_lng.h"
#include "xtt_op.h"
#include "rt_xnav_msg.h"

#define OP_HEIGHT_MIN 80
#define OP_HEIGHT_INC 20
#define OP_HEIGHT_MAX (OP_HEIGHT_MIN + 3 * OP_HEIGHT_INC)


static void op_activate_exit( Widget w, Op *op, XmAnyCallbackStruct *data);
static void op_activate_aalarm_ack( Widget w, Op *op, XmAnyCallbackStruct *data);
static void op_activate_balarm_ack( Widget w, Op *op, XmAnyCallbackStruct *data);
static void op_activate_aalarm_incr( Widget w, Op *op, XmAnyCallbackStruct *data);
static void op_activate_aalarm_decr( Widget w, Op *op, XmAnyCallbackStruct *data);
static void op_activate_alarmlist( Widget w, Op *op, XmAnyCallbackStruct *data);
static void op_activate_eventlist( Widget w, Op *op, XmAnyCallbackStruct *data);
static void op_activate_eventlog( Widget w, Op *op, XmAnyCallbackStruct *data);
static void op_activate_navigator( Widget w, Op *op, XmAnyCallbackStruct *data);
static void op_activate_help( Widget w, Op *op, XmAnyCallbackStruct *data);
static void op_activate_appl1( Widget w, Op *op, XmAnyCallbackStruct *data);
static void op_activate_appl2( Widget w, Op *op, XmAnyCallbackStruct *data);
static void op_activate_appl3( Widget w, Op *op, XmAnyCallbackStruct *data);
static void op_activate_appl4( Widget w, Op *op, XmAnyCallbackStruct *data);
static void op_activate_appl5( Widget w, Op *op, XmAnyCallbackStruct *data);
static void op_activate_appl6( Widget w, Op *op, XmAnyCallbackStruct *data);
static void op_activate_appl7( Widget w, Op *op, XmAnyCallbackStruct *data);
static void op_activate_appl8( Widget w, Op *op, XmAnyCallbackStruct *data);
static void op_activate_appl9( Widget w, Op *op, XmAnyCallbackStruct *data);
static void op_activate_appl10( Widget w, Op *op, XmAnyCallbackStruct *data);
static void op_activate_appl11( Widget w, Op *op, XmAnyCallbackStruct *data);
static void op_activate_appl12( Widget w, Op *op, XmAnyCallbackStruct *data);
static void op_activate_appl13( Widget w, Op *op, XmAnyCallbackStruct *data);
static void op_activate_appl14( Widget w, Op *op, XmAnyCallbackStruct *data);
static void op_activate_appl15( Widget w, Op *op, XmAnyCallbackStruct *data);
static void op_create_alarmcnt_label( Widget w, Op *op, XmAnyCallbackStruct *data);
static void op_create_aalarm_label1( Widget w, Op *op, XmAnyCallbackStruct *data);
static void op_create_aalarm_label2( Widget w, Op *op, XmAnyCallbackStruct *data);
static void op_create_aalarm_label3( Widget w, Op *op, XmAnyCallbackStruct *data);
static void op_create_aalarm_label4( Widget w, Op *op, XmAnyCallbackStruct *data);
static void op_create_aalarm_label5( Widget w, Op *op, XmAnyCallbackStruct *data);
static void op_create_aalarm_active1( Widget w, Op *op, XmAnyCallbackStruct *data);
static void op_create_aalarm_active2( Widget w, Op *op, XmAnyCallbackStruct *data);
static void op_create_aalarm_active3( Widget w, Op *op, XmAnyCallbackStruct *data);
static void op_create_aalarm_active4( Widget w, Op *op, XmAnyCallbackStruct *data);
static void op_create_aalarm_active5( Widget w, Op *op, XmAnyCallbackStruct *data);
static void op_create_balarm_label( Widget w, Op *op, XmAnyCallbackStruct *data);
static void op_create_balarm_mark( Widget w, Op *op, XmAnyCallbackStruct *data);
static void op_create_balarm_active( Widget w, Op *op, XmAnyCallbackStruct *data);
static void op_create_appl_form( Widget w, Op *op, XmAnyCallbackStruct *data);
static void op_jop_command_cb( void *op, char *command);


Op::Op(
	void *op_parent_ctx,
	Widget	op_parent_wid,
	char *opplace,
	pwr_tStatus *status) :
	parent_ctx(op_parent_ctx), parent_wid(op_parent_wid), start_jop(0), 
	jop(NULL), command_cb(NULL), map_cb(NULL), help_cb(NULL), 
	close_cb(NULL), get_alarm_info_cb(NULL), ack_last_cb(NULL)
{
  char		uid_filename[120] = {"xtt_op.uid"};
  char		*uid_filename_p = uid_filename;
  Arg 		args[20];
  pwr_tStatus	sts;
  int		i;
  MrmHierarchy s_DRMh;
  MrmType dclass;
  char		name[20] = "Operator Window";
  int		width;
  int 		decor, func;

  static MrmRegisterArg	reglist[] = {
        { "op_ctx", 0 },
	{"op_activate_exit",(caddr_t)op_activate_exit },
	{"op_activate_aalarm_ack",(caddr_t)op_activate_aalarm_ack },
	{"op_activate_aalarm_incr",(caddr_t)op_activate_aalarm_incr },
	{"op_activate_aalarm_decr",(caddr_t)op_activate_aalarm_decr },
	{"op_create_alarmcnt_label",(caddr_t)op_create_alarmcnt_label },
	{"op_create_aalarm_label1",(caddr_t)op_create_aalarm_label1 },
	{"op_create_aalarm_label2",(caddr_t)op_create_aalarm_label2 },
	{"op_create_aalarm_label3",(caddr_t)op_create_aalarm_label3 },
	{"op_create_aalarm_label4",(caddr_t)op_create_aalarm_label4 },
	{"op_create_aalarm_label5",(caddr_t)op_create_aalarm_label5 },
	{"op_create_aalarm_active1",(caddr_t)op_create_aalarm_active1 },
	{"op_create_aalarm_active2",(caddr_t)op_create_aalarm_active2 },
	{"op_create_aalarm_active3",(caddr_t)op_create_aalarm_active3 },
	{"op_create_aalarm_active4",(caddr_t)op_create_aalarm_active4 },
	{"op_create_aalarm_active5",(caddr_t)op_create_aalarm_active5 },
	{"op_activate_balarm_ack",(caddr_t)op_activate_balarm_ack },
	{"op_activate_alarmlist",(caddr_t)op_activate_alarmlist },
	{"op_activate_eventlist",(caddr_t)op_activate_eventlist },
	{"op_activate_eventlog",(caddr_t)op_activate_eventlog },
	{"op_activate_navigator",(caddr_t)op_activate_navigator },
	{"op_activate_help",(caddr_t)op_activate_help },
	{"op_create_balarm_label",(caddr_t)op_create_balarm_label },
	{"op_create_balarm_mark",(caddr_t)op_create_balarm_mark },
	{"op_create_balarm_active",(caddr_t)op_create_balarm_active },
	{"op_create_appl_form",(caddr_t)op_create_appl_form }
	};
  static int	reglist_num = (sizeof reglist / sizeof reglist[0]);

  *status = 1;

  Lng::get_uid( uid_filename, uid_filename);

  reglist[0].value = (caddr_t) this;

  // Motif
  MrmInitialize();

  // Save the context structure in the widget

  width = XWidthOfScreen(flow_Screen(op_parent_wid));

  // decor =  MWM_DECOR_BORDER | MWM_DECOR_RESIZEH;
  // func =  MWM_FUNC_RESIZE | MWM_FUNC_MOVE;
  decor =  MWM_DECOR_ALL | MWM_DECOR_MINIMIZE | 
	MWM_DECOR_MAXIMIZE | MWM_DECOR_TITLE | MWM_DECOR_MENU;
  func =  MWM_FUNC_ALL | MWM_FUNC_MINIMIZE | MWM_FUNC_MAXIMIZE;

  i = 0;
  XtSetArg(args[i], XmNuserData, (unsigned int) this);i++;
  XtSetArg(args[i], XmNdeleteResponse, XmDO_NOTHING);i++;
  XtSetArg(args[i],XmNmwmDecorations, decor);i++;
  XtSetArg(args[i],XmNmwmFunctions, func);i++;

  XtSetArg(args[i],XmNwidth, width - 20);i++;
  XtSetArg(args[i],XmNheight, OP_HEIGHT_MIN);i++;
  XtSetArg(args[i],XmNminHeight, OP_HEIGHT_MIN);i++;
  XtSetArg(args[i],XmNmaxHeight, OP_HEIGHT_MAX);i++;
  XtSetArg(args[i],XmNheightInc, OP_HEIGHT_INC);i++;
  XtSetArg(args[i],XmNx, 1);i++;
  XtSetArg(args[i],XmNy, 1);i++;

  sts = MrmOpenHierarchy( 1, &uid_filename_p, NULL, &s_DRMh);
  if (sts != MrmSUCCESS) printf("can't open %s\n", uid_filename);

  MrmRegisterNames(reglist, reglist_num);

  parent_wid_op = XtCreatePopupShell( name, 
		topLevelShellWidgetClass, parent_wid, args, i);

  i = 0;
  XtSetArg(args[i], XmNuserData, (unsigned int) this);i++;

  sts = MrmFetchWidgetOverride( s_DRMh, "op_window", parent_wid_op,
			name, args, i, &toplevel, &dclass);
  if (sts != MrmSUCCESS)  printf("can't fetch %s\n", name);

  MrmCloseHierarchy(s_DRMh);

 
  i = 0;
/*
  XtSetValues( toplevel ,args,i);
*/    
  XtManageChild( toplevel);

  configure( opplace);
  XtPopup( parent_wid_op, XtGrabNone);

  // Connect the window manager close-button to exit
  flow_AddCloseVMProtocolCb( parent_wid_op, 
	(XtCallbackProc)op_activate_exit, this);

  if ( start_jop)
  {
    jop = new Jop( (void *)this);
    jop->command_cb = &op_jop_command_cb;
  }
  flow_UnmapWidget( aalarm_active[0]);
  flow_UnmapWidget( aalarm_active[1]);
  flow_UnmapWidget( aalarm_active[2]);
  flow_UnmapWidget( aalarm_active[3]);
  flow_UnmapWidget( aalarm_active[4]);
  flow_UnmapWidget( balarm_active);

  wow_DisplayWarranty( toplevel);


  *status = sts;
}


//
//  Delete op
//
Op::~Op()
{
  if ( jop)
    delete jop;
  XtDestroyWidget( parent_wid_op);
}

int Op::jop_command( char *command) 
{ 
  if ( jop) 
    return jop->command( command);
  return 0;
}

void Op::map()
{
  flow_UnmapWidget( parent_wid_op);
  flow_MapWidget( parent_wid_op);
}


void Op::scan()
{
  if ( jop)
    jop->scan();
}

int Op::appl_action( int idx)
{
  char cmd[80];
  char name[80];
  int sts;

  if ( command_cb)
  {
    sts = gdh_ObjidToName( button_objid[idx], name, sizeof(name), cdh_mNName);
    strcpy( cmd, "ope gra/obj=");
    strcat( cmd, name);

    command_cb( parent_ctx, cmd);
  }
  return XNAV__SUCCESS;
}

void  Op::update_alarm_info()
{
  evlist_sAlarmInfo info;
  int sts;
  Arg 		args[4];
  XmString	cstr;
  int		i, j;
  int		height, active_height;
  int		background;
  char          str[40];
  char		text[120];

  if ( get_alarm_info_cb)
  {
    sts = (get_alarm_info_cb)( parent_ctx, &info);
    if ( EVEN(sts)) return;

/*
    printf("Info A: %s  %d %d\n", info.a_alarm_text[0], info.a_alarm_active[0], 
	info.a_alarm_exist[0]);
    printf("Info A: %s  %d %d\n", info.a_alarm_text[1], info.a_alarm_active[1], 
	info.a_alarm_exist[1]);
    printf("Info A: %s  %d %d\n", info.a_alarm_text[2], info.a_alarm_active[2], 
	info.a_alarm_exist[2]);
    printf("Info A: %s  %d %d\n", info.a_alarm_text[3], info.a_alarm_active[3], 
	info.a_alarm_exist[3]);
    printf("Info A: %s  %d %d\n", info.a_alarm_text[4], info.a_alarm_active[4], 
	info.a_alarm_exist[4]);
    printf("Info B: %s  %d\n", info.b_alarm_text[0], info.b_alarm_active[0]);
    printf("Info B: %s  %d\n", info.b_alarm_text[1], info.b_alarm_active[1]);
*/
    XtSetArg(args[j=0],XmNbackground, &background);j++;
    XtGetValues( aalarm_active[0], args, j);

    height = 22;
    active_height = 22;

    sprintf( str, "%d", info.alarms_total);
    cstr=XmStringCreateLtoR( str, "ISO8859-1");
    XtSetArg(args[j=0],XmNlabelString, cstr);j++;
    XtSetValues( alarmcnt_label, args, j);
    XmStringFree( cstr);

    for ( i = 0; i < 5; i++)
    {
      if ( info.a_alarm_exist[i])
      {
	sprintf( text, "%s  %s", info.a_alarm_alias[i], info.a_alarm_text[i]);
        cstr=XmStringCreateLtoR( text, "ISO8859-1");
        XtSetArg(args[j=0],XmNlabelString, cstr);j++;
        XtSetValues( aalarm_label[i], args, j);
        XmStringFree( cstr);

        XtVaSetValues( aalarm_label[i], XtVaTypedArg, XmNbackground,
		XmRString, "red", 4, NULL);

        if ( info.a_alarm_active[i])
        {
          flow_MapWidget( aalarm_active[i]);
        }
        else
        {
          flow_UnmapWidget( aalarm_active[i]);
        }
      }
      else
      {
        cstr=XmStringCreateLtoR( "", "ISO8859-1");
        XtSetArg(args[j=0],XmNlabelString, cstr);j++;
        XtSetArg(args[j],XmNbackground, background);j++;
        XtSetValues( aalarm_label[i], args, j);
        XmStringFree( cstr);

        flow_UnmapWidget( aalarm_active[i]);
      }
    }

    if ( info.b_alarm_exist[0])
    {
      balarm_type = evlist_eEventType_Alarm;
      balarm_prio = mh_eEventPrio_B;
      sprintf( text, "%s  %s", info.b_alarm_alias[0], info.b_alarm_text[0]);
      cstr=XmStringCreateLtoR( text, "ISO8859-1");
      XtSetArg(args[j=0],XmNlabelString, cstr);j++;
      XtSetValues( balarm_label, args, j);
      XmStringFree( cstr);

      XtVaSetValues( balarm_label, XtVaTypedArg, XmNbackground,
		XmRString, "yellow", 7, NULL);

      cstr=XmStringCreateLtoR( "B", "ISO8859-1");
      XtSetArg(args[j=0],XmNlabelString, cstr);j++;
      XtSetValues( balarm_mark, args, j);
      XmStringFree( cstr);

      if ( info.b_alarm_active[0])
      {
        flow_MapWidget( balarm_active);
      }
      else
      {
        flow_UnmapWidget( balarm_active);
      }
    }
    else if ( info.c_alarm_exist[0])
    {
      balarm_type = evlist_eEventType_Alarm;
      balarm_prio = mh_eEventPrio_C;
      sprintf( text, "%s  %s", info.c_alarm_alias[0], info.c_alarm_text[0]);
      cstr=XmStringCreateLtoR( text, "ISO8859-1");
      XtSetArg(args[j=0],XmNlabelString, cstr);j++;
      XtSetValues( balarm_label, args, j);
      XmStringFree( cstr);

      XtVaSetValues( balarm_label, XtVaTypedArg, XmNbackground,
		XmRString, "lightblue", 7, NULL);

      cstr=XmStringCreateLtoR( "C", "ISO8859-1");
      XtSetArg(args[j=0],XmNlabelString, cstr);j++;
      XtSetValues( balarm_mark, args, j);
      XmStringFree( cstr);

      if ( info.b_alarm_active[0])
      {
        flow_MapWidget( balarm_active);
      }
      else
      {
        flow_UnmapWidget( balarm_active);
      }
    }
    else if ( info.d_alarm_exist[0])
    {
      balarm_type = evlist_eEventType_Alarm;
      balarm_prio = mh_eEventPrio_D;
      sprintf( text, "%s  %s", info.d_alarm_alias[0], info.d_alarm_text[0]);
      cstr=XmStringCreateLtoR( text, "ISO8859-1");
      XtSetArg(args[j=0],XmNlabelString, cstr);j++;
      XtSetValues( balarm_label, args, j);
      XmStringFree( cstr);

      XtVaSetValues( balarm_label, XtVaTypedArg, XmNbackground,
		XmRString, "violet", 7, NULL);

      cstr=XmStringCreateLtoR( "D", "ISO8859-1");
      XtSetArg(args[j=0],XmNlabelString, cstr);j++;
      XtSetValues( balarm_mark, args, j);
      XmStringFree( cstr);

      if ( info.b_alarm_active[0])
      {
        flow_MapWidget( balarm_active);
      }
      else
      {
        flow_UnmapWidget( balarm_active);
      }
    }
    else if ( info.i_alarm_exist[0])
    {
      balarm_type = evlist_eEventType_Info;
      sprintf( text, "%s  %s", info.i_alarm_alias[0], info.i_alarm_text[0]);
      cstr=XmStringCreateLtoR( text, "ISO8859-1");
      XtSetArg(args[j=0],XmNlabelString, cstr);j++;
      XtSetValues( balarm_label, args, j);
      XmStringFree( cstr);

      XtVaSetValues( balarm_label, XtVaTypedArg, XmNbackground,
		XmRString, "green", 7, NULL);

      cstr=XmStringCreateLtoR( "I", "ISO8859-1");
      XtSetArg(args[j=0],XmNlabelString, cstr);j++;
      XtSetValues( balarm_mark, args, j);
      XmStringFree( cstr);

      if ( info.b_alarm_active[0])
      {
        flow_MapWidget( balarm_active);
      }
      else
      {
        flow_UnmapWidget( balarm_active);
      }
    }
    else
    {
      cstr=XmStringCreateLtoR( "", "ISO8859-1");
      XtSetArg(args[j=0],XmNlabelString, cstr);j++;
      XtSetArg(args[j],XmNbackground, background);j++;
      XtSetValues( balarm_label, args, j);
      XmStringFree( cstr);

      cstr=XmStringCreateLtoR( "", "ISO8859-1");
      XtSetArg(args[j=0],XmNlabelString, cstr);j++;
      XtSetValues( balarm_mark, args, j);
      XmStringFree( cstr);

      flow_UnmapWidget( balarm_active);
    }


    for ( i = 0; i < 5; i++)
    {
      XtSetArg(args[j=0],XmNheight, height);j++;
      XtSetValues( aalarm_label[i], args, j);

      XtSetArg(args[j=0],XmNheight, active_height);j++;
      XtSetValues( aalarm_active[i], args, j);
    }
    XtSetArg(args[j=0],XmNheight, height);j++;
    XtSetValues( balarm_label, args, j);

  }
}

int Op::configure( char *opplace_str)
{
  int 		sts;
  int		i;
  pwr_tObjid 	opplace;
  pwr_tObjid 	user;
  pwr_sClass_OpPlace *opplace_p;
  pwr_sClass_User *user_p;
  pwr_sAttrRef	attrref;
  XmFontList    fontlist;
  XmFontListEntry entry;

  sts = gdh_NameToObjid( opplace_str, &opplace);
  if ( EVEN(sts)) return sts;

  sts = gdh_ObjidToPointer( opplace, (void **) &opplace_p);
  if ( EVEN(sts)) return sts;

  // Fix 
  if ( strncmp( opplace_p->OpWinProgram, "Jop", 3) == 0)
    start_jop = 1;

  // Find matching user object
  sts = gdh_GetClassList( pwr_cClass_User, &user);
  while ( ODD (sts))
  {

    sts = gdh_ObjidToPointer( user, (void **) &user_p);
    if ( EVEN(sts)) return sts;

    if ( user_p->OpNumber == opplace_p->OpNumber)
      break;
    sts = gdh_GetNextObject( user, &user);
  }
  if ( EVEN(sts)) return sts;

  // Load font
  entry = XmFontListEntryCreate( "tag1", XmFONT_IS_FONT, 
	XLoadQueryFont( flow_Display(appl_form), 
	"-*-Helvetica-Bold-R-Normal--12-*-*-*-P-*-ISO8859-1"));
  fontlist = XmFontListAppendEntry( NULL, entry);
  XtFree( (char *)entry);

  // Examine Graph objects
  button_cnt = user_p->NoFastAvail;
  if ( button_cnt > 15)
    button_cnt = 15;
  for ( i = 0; i < button_cnt; i++)
  {
    if ( i >= 15)
      break;
    memset( &attrref, 0, sizeof(attrref));
    sts = gdh_ClassAttrToAttrref( pwr_cClass_XttGraph, ".ButtonText", &attrref);
    if ( EVEN(sts)) return sts;

    attrref = cdh_ArefAdd( &user_p->FastAvail[i], &attrref);
    sts = gdh_GetObjectInfoAttrref( &attrref, (void *)button_title[i], 
		sizeof(button_title[0]));
    if ( EVEN(sts)) 
      strcpy( button_title[i], "");

    button_objid[i] = attrref.Objid;
  }

  // Create the application buttons
  for ( i = 0; i < button_cnt; i++)
  {
    Widget b[15];
    switch ( i)
    {
      case 0:
        b[i] = XtVaCreateManagedWidget( button_title[i],
		xmPushButtonWidgetClass, appl_form,
   		XmNleftAttachment,	XmATTACH_FORM,
    		XmNtopAttachment,  	XmATTACH_FORM,
    		XmNheight,		25,
		XmNfontList, 		fontlist,
		NULL);
        XtAddCallback( b[i], XmNactivateCallback, 
		(XtCallbackProc)op_activate_appl1, (void *)this);
        break;
      case 1:
      case 2:
      case 3:
        b[i] = XtVaCreateManagedWidget( button_title[i],
		xmPushButtonWidgetClass, appl_form,
   		XmNleftAttachment,	XmATTACH_FORM,
    		XmNtopAttachment,  	XmATTACH_FORM,
		XmNleftAttachment,    	XmATTACH_WIDGET,
		XmNleftWidget,		b[i-1],
    		XmNheight,		25,
		XmNfontList, 		fontlist,
		NULL);
        if ( i == 1)
          XtAddCallback( b[i], XmNactivateCallback, 
		(XtCallbackProc)op_activate_appl2, (void *)this);
        else if ( i == 2)
          XtAddCallback( b[i], XmNactivateCallback, 
		(XtCallbackProc)op_activate_appl3, (void *)this);
        else
          XtAddCallback( b[i], XmNactivateCallback, 
		(XtCallbackProc)op_activate_appl4, (void *)this);
        break;
      case 4:
        b[i] = XtVaCreateManagedWidget( button_title[i],
		xmPushButtonWidgetClass, appl_form,
   		XmNleftAttachment,	XmATTACH_FORM,
    		XmNtopAttachment,  	XmATTACH_FORM,
		XmNleftAttachment,    	XmATTACH_WIDGET,
		XmNleftWidget,		b[i-1],
    		XmNrightAttachment,  	XmATTACH_FORM,
    		XmNheight,		25,
		XmNfontList, 		fontlist,
		NULL);
        XtAddCallback( b[i], XmNactivateCallback, 
		(XtCallbackProc)op_activate_appl5, (void *)this);
        break;
      case 5:
        b[i] = XtVaCreateManagedWidget( button_title[i],
		xmPushButtonWidgetClass, appl_form,
   		XmNleftAttachment,	XmATTACH_FORM,
    		XmNtopAttachment,  	XmATTACH_WIDGET,
    		XmNtopWidget,	  	b[0],
    		XmNheight,		25,
		XmNfontList, 		fontlist,
		NULL);
        XtAddCallback( b[i], XmNactivateCallback, 
		(XtCallbackProc)op_activate_appl6, (void *)this);
        break;
      case 6:
      case 7:
      case 8:
        b[i] = XtVaCreateManagedWidget( button_title[i],
		xmPushButtonWidgetClass, appl_form,
		XmNleftAttachment,    	XmATTACH_WIDGET,
		XmNleftWidget,		b[i-1],
    		XmNtopAttachment,  	XmATTACH_WIDGET,
    		XmNtopWidget,	  	b[0],
    		XmNheight,		25,
		XmNfontList, 		fontlist,
		NULL);
        if ( i == 6)
          XtAddCallback( b[i], XmNactivateCallback, 
		(XtCallbackProc)op_activate_appl7, (void *)this);
        else if ( i == 7)
          XtAddCallback( b[i], XmNactivateCallback, 
		(XtCallbackProc)op_activate_appl8, (void *)this);
        else
          XtAddCallback( b[i], XmNactivateCallback, 
		(XtCallbackProc)op_activate_appl9, (void *)this);
        break;
      case 9:
        b[i] = XtVaCreateManagedWidget( button_title[i],
		xmPushButtonWidgetClass, appl_form,
		XmNleftAttachment,    	XmATTACH_WIDGET,
		XmNleftWidget,		b[i-1],
    		XmNrightAttachment,  	XmATTACH_FORM,
    		XmNtopAttachment,  	XmATTACH_WIDGET,
    		XmNtopWidget,	  	b[0],
    		XmNheight,		25,
		XmNfontList, 		fontlist,
		NULL);
        XtAddCallback( b[i], XmNactivateCallback, 
		(XtCallbackProc)op_activate_appl10, (void *)this);
        break;
      case 10:
        b[i] = XtVaCreateManagedWidget( button_title[i],
		xmPushButtonWidgetClass, appl_form,
   		XmNleftAttachment,	XmATTACH_FORM,
    		XmNtopAttachment,  	XmATTACH_WIDGET,
    		XmNtopWidget,	  	b[5],
   		XmNbottomAttachment,	XmATTACH_FORM,
    		XmNheight,		25,
		XmNfontList, 		fontlist,
		NULL);
        XtAddCallback( b[i], XmNactivateCallback, 
		(XtCallbackProc)op_activate_appl11, (void *)this);
        break;
      case 11:
      case 12:
      case 13:
        b[i] = XtVaCreateManagedWidget( button_title[i],
		xmPushButtonWidgetClass, appl_form,
		XmNleftAttachment,    	XmATTACH_WIDGET,
		XmNleftWidget,		b[i-1],
    		XmNtopAttachment,  	XmATTACH_WIDGET,
    		XmNtopWidget,	  	b[5],
   		XmNbottomAttachment,	XmATTACH_FORM,
    		XmNheight,		25,
		XmNfontList, 		fontlist,
		NULL);
        if ( i == 11)
          XtAddCallback( b[i], XmNactivateCallback, 
		(XtCallbackProc)op_activate_appl12, (void *)this);
        else if ( i == 12)
          XtAddCallback( b[i], XmNactivateCallback, 
		(XtCallbackProc)op_activate_appl13, (void *)this);
        else
          XtAddCallback( b[i], XmNactivateCallback, 
		(XtCallbackProc)op_activate_appl14, (void *)this);
        break;
      case 14:
        b[i] = XtVaCreateManagedWidget( button_title[i],
		xmPushButtonWidgetClass, appl_form,
		XmNleftAttachment,    	XmATTACH_WIDGET,
		XmNleftWidget,		b[i-1],
    		XmNrightAttachment,  	XmATTACH_FORM,
    		XmNtopAttachment,  	XmATTACH_WIDGET,
    		XmNtopWidget,	  	b[5],
   		XmNbottomAttachment,	XmATTACH_FORM,
    		XmNheight,		25,
		XmNfontList, 		fontlist,
		NULL);
        XtAddCallback( b[i], XmNactivateCallback, 
		(XtCallbackProc)op_activate_appl15, (void *)this);
        break;
    }
  }
  XmFontListFree( fontlist);

  return XNAV__SUCCESS;
}

static void op_activate_exit( Widget w, Op *op, XmAnyCallbackStruct *data)
{
  if ( op->close_cb)
    (op->close_cb)(op->parent_ctx);
  else
    delete op;
}

static void op_activate_aalarm_ack( Widget w, Op *op, XmAnyCallbackStruct *data)
{

  if ( op->ack_last_cb)
    (op->ack_last_cb)( op->parent_ctx, evlist_eEventType_Alarm, mh_eEventPrio_A);
}

static void op_activate_balarm_ack( Widget w, Op *op, XmAnyCallbackStruct *data)
{
  if ( op->ack_last_cb)
    (op->ack_last_cb)( op->parent_ctx, op->balarm_type, op->balarm_prio);
}

static void op_activate_aalarm_incr( Widget w, Op *op, XmAnyCallbackStruct *data)
{
  Arg 		args[20];
  int		i;
  short		height;

  i = 0;
  XtSetArg(args[i],XmNheight, &height);i++;
  XtGetValues( op->parent_wid_op, args, i);

  if ( height >= OP_HEIGHT_MAX)
    return;

  height += OP_HEIGHT_INC;
  i = 0;
  XtSetArg(args[i],XmNheight, height);i++;
  XtSetValues( op->parent_wid_op, args, i);
}

static void op_activate_aalarm_decr( Widget w, Op *op, XmAnyCallbackStruct *data)
{
  Arg 		args[20];
  int		i;
  short		height;

  i = 0;
  XtSetArg(args[i],XmNheight, &height);i++;
  XtGetValues( op->parent_wid_op, args, i);

  if ( height <= OP_HEIGHT_MIN)
    return;

  height -= OP_HEIGHT_INC;
  i = 0;
  XtSetArg(args[i],XmNheight, height);i++;
  XtSetValues( op->parent_wid_op, args, i);
}

static void op_activate_alarmlist( Widget w, Op *op, XmAnyCallbackStruct *data)
{
  char cmd[20] = "show alarm";
  if ( op->command_cb)
    op->command_cb( op->parent_ctx, cmd);
}

static void op_activate_eventlist( Widget w, Op *op, XmAnyCallbackStruct *data)
{
  char cmd[20] = "show event";
  if ( op->command_cb)
    op->command_cb( op->parent_ctx, cmd);
}

static void op_activate_eventlog( Widget w, Op *op, XmAnyCallbackStruct *data)
{
  char cmd[20] = "show histlist";
  if ( op->command_cb)
    op->command_cb( op->parent_ctx, cmd);
}

static void op_activate_navigator( Widget w, Op *op, XmAnyCallbackStruct *data)
{
  if ( op->map_cb)
    op->map_cb( op->parent_ctx);
}

static void op_activate_help( Widget w, Op *op, XmAnyCallbackStruct *data)
{
  if ( op->help_cb)
    op->help_cb( op->parent_ctx, "index");
}

static void op_activate_appl1( Widget w, Op *op, XmAnyCallbackStruct *data)
{
  op->appl_action(0);
}

static void op_activate_appl2( Widget w, Op *op, XmAnyCallbackStruct *data)
{
  op->appl_action(1);
}

static void op_activate_appl3( Widget w, Op *op, XmAnyCallbackStruct *data)
{
  op->appl_action(2);
}

static void op_activate_appl4( Widget w, Op *op, XmAnyCallbackStruct *data)
{
  op->appl_action(3);
}

static void op_activate_appl5( Widget w, Op *op, XmAnyCallbackStruct *data)
{
  op->appl_action(4);
}

static void op_activate_appl6( Widget w, Op *op, XmAnyCallbackStruct *data)
{
  op->appl_action(5);
}

static void op_activate_appl7( Widget w, Op *op, XmAnyCallbackStruct *data)
{
  op->appl_action(6);
}

static void op_activate_appl8( Widget w, Op *op, XmAnyCallbackStruct *data)
{
  op->appl_action(7);
}

static void op_activate_appl9( Widget w, Op *op, XmAnyCallbackStruct *data)
{
  op->appl_action(8);
}

static void op_activate_appl10( Widget w, Op *op, XmAnyCallbackStruct *data)
{
  op->appl_action(9);
}

static void op_activate_appl11( Widget w, Op *op, XmAnyCallbackStruct *data)
{
  op->appl_action(10);
}

static void op_activate_appl12( Widget w, Op *op, XmAnyCallbackStruct *data)
{
  op->appl_action(11);
}

static void op_activate_appl13( Widget w, Op *op, XmAnyCallbackStruct *data)
{
  op->appl_action(12);
}

static void op_activate_appl14( Widget w, Op *op, XmAnyCallbackStruct *data)
{
  op->appl_action(13);
}

static void op_activate_appl15( Widget w, Op *op, XmAnyCallbackStruct *data)
{
  op->appl_action(14);
}

static void op_create_alarmcnt_label( Widget w, Op *op, XmAnyCallbackStruct *data)
{
  op->alarmcnt_label = w;
}

static void op_create_aalarm_label1( Widget w, Op *op, XmAnyCallbackStruct *data)
{
  op->aalarm_label[0] = w;
}

static void op_create_aalarm_label2( Widget w, Op *op, XmAnyCallbackStruct *data)
{
  op->aalarm_label[1] = w;
}

static void op_create_aalarm_label3( Widget w, Op *op, XmAnyCallbackStruct *data)
{
  op->aalarm_label[2] = w;
}

static void op_create_aalarm_label4( Widget w, Op *op, XmAnyCallbackStruct *data)
{
  op->aalarm_label[3] = w;
}

static void op_create_aalarm_label5( Widget w, Op *op, XmAnyCallbackStruct *data)
{
  op->aalarm_label[4] = w;
}

static void op_create_aalarm_active1( Widget w, Op *op, XmAnyCallbackStruct *data)
{
  op->aalarm_active[0] = w;
}

static void op_create_aalarm_active2( Widget w, Op *op, XmAnyCallbackStruct *data)
{
  op->aalarm_active[1] = w;
}

static void op_create_aalarm_active3( Widget w, Op *op, XmAnyCallbackStruct *data)
{
  op->aalarm_active[2] = w;
}

static void op_create_aalarm_active4( Widget w, Op *op, XmAnyCallbackStruct *data)
{
  op->aalarm_active[3] = w;
}

static void op_create_aalarm_active5( Widget w, Op *op, XmAnyCallbackStruct *data)
{
  op->aalarm_active[4] = w;
}

static void op_create_balarm_label( Widget w, Op *op, XmAnyCallbackStruct *data)
{
  op->balarm_label = w;
}

static void op_create_balarm_mark( Widget w, Op *op, XmAnyCallbackStruct *data)
{
  op->balarm_mark = w;
}

static void op_create_balarm_active( Widget w, Op *op, XmAnyCallbackStruct *data)
{
  op->balarm_active = w;
}

static void op_create_appl_form( Widget w, Op *op, XmAnyCallbackStruct *data)
{
  op->appl_form = w;
}

static void op_jop_command_cb( void *op, char *command)
{
  if ( ((Op *)op)->command_cb)
    ((Op *)op)->command_cb( ((Op *)op)->parent_ctx, command);
}
