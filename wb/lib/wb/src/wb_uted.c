/* wb_uted.c -- <short description>

   PROVIEW/R
   Copyright (C) 1996 by Comator Process AB.

   This module creates function object editor and handles
   the graphical editor.
   It contains all the methods defined for the function block editor.  */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <string.h>

#include "wb_foe_macros.h"
#include "wb_login.h"

#include <Xm/Xm.h>
#include <Mrm/MrmPublic.h>
#include <Xm/Protocols.h>
#include <X11/cursorfont.h>
#include <Xm/ToggleB.h>
#include <Xm/Text.h>
#include <X11/Intrinsic.h>
#include <Xm/MessageB.h>

#include "co_time.h"
#include "co_msg.h"
#include "co_dcli.h"
#include "co_dcli.h"
#include "wb_ldh.h"
#include "wb_foe.h"
#include "wb_foe_msg.h"
#include "wb_vldh_msg.h"
#include "wb_cmd_msg.h"
#include "wb_vldh.h"
#include "wb_api.h"
#include "wb_uted.h"

#define	BEEP	    putchar( '\7' );

#define UTED_INDEX_NOCOMMAND -1
#define UTED_TEXT_NOCOMMAND "No command selected"

#define _WaitCursor(w) \
  if(WaitCursor == 0)  \
     WaitCursor = XCreateFontCursor( XtDisplay(w), XC_watch); \
  XDefineCursor(XtDisplay(w), XtWindow(w), WaitCursor); \
  XFlush(XtDisplay(w));

#define _UndefineCursor(w)   XUndefineCursor(XtDisplay(w), XtWindow(w)); 

static Cursor WaitCursor = 0;

typedef struct
{
	char	qual[30];
	int	insert;
	int	insert_hier;
	int	value;
	int	present;
	int	type;
} uted_t_qual;

typedef struct
{
	char		command[80];
	int		process;
	int		view_sensitivity;
	int		batch_sensitivity;
	uted_t_qual	qualifier[10];
} uted_t_command;

static uted_t_command	commands[ UTED_MAX_COMMANDS ] = {
{ "List Descriptor", UTED_PROC_PWRPLC, 1, 3,	{
/*** QUALIFIER	***** INSERT *** INSERT HIER ** VALUE * TOGGLE *** TYPE ****/  
  {"Descriptor",	1, 	UTED_INS_NODE,	1, 	0, UTED_QUAL_QUAL},
  {"Hierarchy",		1, 	UTED_INS_PLNO,	1, 	0, UTED_QUAL_QUAL},
  {"Object",		1, 	UTED_INS_PLNO,	1, 	0, UTED_QUAL_QUAL},
  {"Volumes", 		0, 	0,		1, 	0, UTED_QUAL_QUAL},
  {"AllVolumes", 	0, 	0,		0, 	1, UTED_QUAL_QUAL},
  {"Output",		0, 	0,		1, 	0, UTED_QUAL_QUAL},
  {"",}}},
{ "List Signals", UTED_PROC_PWRPLC,  1, 3,	{
  {"Hierarchy",		1, 	UTED_INS_PLANT,	1, 	0, UTED_QUAL_QUAL},
  {"Volumes", 		0, 	0,		1, 	0, UTED_QUAL_QUAL},
  {"AllVolumes", 	0, 	0,		0, 	1, UTED_QUAL_QUAL},
  {"Output",		0, 	0,		1, 	0, UTED_QUAL_QUAL},
  {"Shortname", 	0, 	0,		0, 	1, UTED_QUAL_QUAL},
  {"",}}},
{ "List Channels", UTED_PROC_PWRPLC,  1, 3,	{
  {"Node",		1, 	UTED_INS_NODE,	1, 	0, UTED_QUAL_QUAL},
  {"Output",		0, 	0,		1, 	0, UTED_QUAL_QUAL},
  {"",}}},
{ "List Hierarchy", UTED_PROC_PWRPLC,  1, 3,	{
  {"Hierarchy",		1, 	UTED_INS_PLANT,	1, 	0, UTED_QUAL_QUAL},
  {"Volumes", 		0, 	0,		1, 	0, UTED_QUAL_QUAL},
  {"AllVolumes", 	0, 	0,		0, 	1, UTED_QUAL_QUAL},
  {"Output",		0, 	0,		1, 	0, UTED_QUAL_QUAL},
  {"",}}},
{ "List Plcpgm", UTED_PROC_PWRPLC,  1, 3,	{
  {"Plcpgm",		1, 	UTED_INS_PLANT,	1, 	0, UTED_QUAL_QUAL},
  {"Hierarchy",		1, 	UTED_INS_PLANT,	1, 	0, UTED_QUAL_QUAL},
  {"Volumes", 		0, 	0,		1, 	0, UTED_QUAL_QUAL},
  {"AllVolumes", 	0, 	0,		0, 	1, UTED_QUAL_QUAL},
  {"Output",		0, 	0,		1, 	0, UTED_QUAL_QUAL},
  {"",}}},
{ "Show Class", UTED_PROC_PWRPLC, 1, 3,{
  {"Classhier", 	0, 	0, 		1, 	0, UTED_QUAL_QUAL},
  {"Name", 		0, 	0, 		1, 	0, UTED_QUAL_QUAL},
  {"Full", 		0, 	0, 		0, 	1, UTED_QUAL_QUAL},
  {"All", 		0, 	0,		0, 	1, UTED_QUAL_QUAL},
  {"Contents", 		0, 	0,		0,	1, UTED_QUAL_QUAL},
  {"Output", 		0, 	0,		1, 	0, UTED_QUAL_QUAL},
  {"",}}},
{ "Show Objects", UTED_PROC_PWRPLC, 1, 3,{
  {"Hierarchy", 	1, 	UTED_INS_PLNO,	1, 	0, UTED_QUAL_QUAL},
  {"Class", 		0, 	0, 		1,	0, UTED_QUAL_QUAL},
  {"Name", 		1, 	UTED_INS_PLNO,	1, 	0, UTED_QUAL_QUAL},
  {"Parameter", 	0, 	0,		1, 	0, UTED_QUAL_QUAL},
  {"Volumes", 		0, 	0,		1, 	0, UTED_QUAL_QUAL},
  {"AllVolumes", 	0, 	0,		0, 	1, UTED_QUAL_QUAL},
  {"Objid", 		0, 	0,		1, 	0, UTED_QUAL_QUAL},
  {"Full", 		0, 	0,		0, 	1, UTED_QUAL_QUAL},
  {"Output", 		0, 	0,		1, 	0, UTED_QUAL_QUAL},
  {"",}}},
{ "Show Modules", UTED_PROC_PWRPLC, 1, 1,{
  {"Name", 		1, 	UTED_INS_PLNO,	1, 	0, UTED_QUAL_QUAL},
  {"Hierarchy",		1, 	UTED_INS_PLNO,	1, 	0, UTED_QUAL_QUAL},
  {"Objid", 		0, 	0,		1, 	0, UTED_QUAL_QUAL},
  {"Output", 		0, 	0,		1, 	0, UTED_QUAL_QUAL},
  {"",}}},
{ "Show Security", UTED_PROC_PWRPLC, 1, 1,{
  {"",}}},
{ "Compile", UTED_PROC_PWRPLC, 0, 3,{
  {"Window", 		1, 	UTED_INS_PLANT,	1, 	0, UTED_QUAL_QUAL},
  {"Plcpgm", 		1, 	UTED_INS_PLANT,	1, 	0, UTED_QUAL_QUAL},
  {"Hierarchy", 	1, 	UTED_INS_PLANT,	1, 	0, UTED_QUAL_QUAL},
  {"Volumes", 		0, 	0,		1, 	0, UTED_QUAL_QUAL},
  {"AllVolumes", 	0, 	0,		0, 	1, UTED_QUAL_QUAL},
  {"Modified", 		0, 	0,		0, 	1, UTED_QUAL_QUAL},
  {"Debug", 		0, 	0,		0, 	1, UTED_QUAL_QUAL},
  {"From_plcpgm", 	1, 	UTED_INS_PLANT,	1, 	0, UTED_QUAL_QUAL},
  {"",}}},
{ "Crossreference", UTED_PROC_PWRPLC, 1, 3,{
  {"Hierarchy", 	1, 	UTED_INS_PLNO,	1, 	0, UTED_QUAL_QUAL},
  {"Class", 		0, 	0,		1, 	0, UTED_QUAL_QUAL},
  {"Name", 		1, 	UTED_INS_PLNO,	1, 	0, UTED_QUAL_QUAL},
  {"",}}},
{ "Sort", UTED_PROC_PWRPLC,  0, 1,	{
  {"Parent",		1, 	UTED_INS_PLNO,	1, 	0, UTED_QUAL_QUAL},
  {"Class", 		0, 	0,		0, 	1, UTED_QUAL_QUAL},
  {"Signals", 		0, 	0,		0, 	1, UTED_QUAL_QUAL},
  {"",}}},
{ "Print", UTED_PROC_PWRPLC, 1, 3,{
  {"Hierarchy", 	1, 	UTED_INS_PLANT,	1,	0, UTED_QUAL_QUAL},
  {"Plcpgm", 		1, 	UTED_INS_PLANT,	1, 	0, UTED_QUAL_QUAL},
  {"All", 		0, 	0,		0, 	1, UTED_QUAL_QUAL},
  {"Nodocument", 	0, 	0,		0, 	1, UTED_QUAL_QUAL},
  {"Nooverview", 	0, 	0,		0, 	1, UTED_QUAL_QUAL},
  {"From_plcpgm", 	1, 	UTED_INS_PLANT,	1, 	0, UTED_QUAL_QUAL},
  {"",}}},
{ "Copy Objects", UTED_PROC_PWRPLC, 0, 1,{
  {"Source",	 	1, 	UTED_INS_PLNO,	1, 	0, UTED_QUAL_QUAL},
  {"Destination", 	1, 	UTED_INS_PLNO,	1, 	0, UTED_QUAL_QUAL},
  {"Name", 		0, 	0,		1, 	0, UTED_QUAL_QUAL},
  {"Hierarchy", 	0, 	0,		0, 	1, UTED_QUAL_QUAL},
  {"First", 		0, 	0,		0, 	1, UTED_QUAL_QUAL},
  {"Last", 		0, 	0,		0, 	1, UTED_QUAL_QUAL},
  {"Before", 		0, 	0,		0, 	1, UTED_QUAL_QUAL},
  {"After", 		0, 	0,		0, 	1, UTED_QUAL_QUAL},
  {"",}}},
{ "Set Attribute", UTED_PROC_PWRPLC, 0, 1,{
  {"Hierarchy", 	1, 	UTED_INS_PLNO,	1, 	0, UTED_QUAL_QUAL},
  {"Class", 		0, 	0, 		1,	0, UTED_QUAL_QUAL},
  {"Name", 		1, 	UTED_INS_PLNO,	1, 	0, UTED_QUAL_QUAL},
  {"Attribute", 	0, 	0,		1, 	0, UTED_QUAL_QUAL},
  {"Value", 		0, 	0,		1, 	0, UTED_QUAL_QUAL},
  {"Noconfirm", 	0, 	0,		0, 	1, UTED_QUAL_QUAL},
  {"Log", 		0, 	0,		0, 	1, UTED_QUAL_QUAL},
  {"Output", 		0, 	0,		1, 	0, UTED_QUAL_QUAL},
  {"",}}},
{ "Set Template", UTED_PROC_PWRPLC, 0, 1,{
  {"SignalObjectSeg",	0, 	0,		1, 	0, UTED_QUAL_QUAL},
  {"ShoSigChanCon", 	0, 	0,		1, 	0, UTED_QUAL_QUAL},
  {"SigChanConSeg",     0, 	0,		1, 	0, UTED_QUAL_QUAL},
  {"ShoDetectText", 	0, 	0,		1, 	0, UTED_QUAL_QUAL},
  {"",}}},
{ "Configure Card", UTED_PROC_PWRPLC, 0, 1,{
  {"Rack", 		1, 	UTED_INS_NODE,	1, 	0, UTED_QUAL_QUAL},
  {"Cardname", 		0, 	0, 		1,	0, UTED_QUAL_QUAL},
  {"Cardclass", 	0, 	0, 		1,	0, UTED_QUAL_QUAL},
  {"Channelname",	0, 	0, 		1,	0, UTED_QUAL_QUAL},
  {"Chanidentity",	0, 	0, 		1,	0, UTED_QUAL_QUAL},
  {"Chandescription",	0, 	0, 		1,	0, UTED_QUAL_QUAL},
  {"Table",		0, 	0, 		1,	0, UTED_QUAL_QUAL},
  {"",}}},
{ "Export PlcPgm", UTED_PROC_PWRPLC, 1, 1,{
  {"PlcPgm", 		1, 	UTED_INS_PLNO,	1, 	0, UTED_QUAL_QUAL},
  {"Window", 		1, 	UTED_INS_PLNO,	1, 	0, UTED_QUAL_QUAL},
  {"Output", 		0, 	0,		1, 	0, UTED_QUAL_QUAL},
  {"",}}},
{ "Redraw", UTED_PROC_PWRPLC, 0, 3,{
  {"Hierarchy", 	1, 	UTED_INS_PLANT,	1,	0, UTED_QUAL_QUAL},
  {"Plcpgm", 		1, 	UTED_INS_PLANT,	1, 	0, UTED_QUAL_QUAL},
  {"All", 		0, 	0,		0, 	1, UTED_QUAL_QUAL},
  {"From_plcpgm", 	1, 	UTED_INS_PLANT,	1, 	0, UTED_QUAL_QUAL},
  {"",}}},
{ "Move Object", UTED_PROC_PWRPLC, 0, 1,{
  {"Source",	 	1, 	UTED_INS_PLNO,	1, 	0, UTED_QUAL_QUAL},
  {"Destination", 	1, 	UTED_INS_PLNO,	1, 	0, UTED_QUAL_QUAL},
  {"Rename", 		1, 	0,		1, 	0, UTED_QUAL_QUAL},
  {"First", 		0, 	0,		0, 	1, UTED_QUAL_QUAL},
  {"Last", 		0, 	0,		0, 	1, UTED_QUAL_QUAL},
  {"Before", 		0, 	0,		0, 	1, UTED_QUAL_QUAL},
  {"After", 		0, 	0,		0, 	1, UTED_QUAL_QUAL},
  {"",}}},
{ "Create Object", UTED_PROC_PWRPLC, 0, 1,{
  {"Destination", 	1, 	UTED_INS_PLNO,	1, 	0, UTED_QUAL_QUAL},
  {"Name", 		0, 	0,		1, 	0, UTED_QUAL_QUAL},
  {"Class", 		0, 	0, 		1,	0, UTED_QUAL_QUAL},
  {"First", 		0, 	0,		0, 	1, UTED_QUAL_QUAL},
  {"Last", 		0, 	0,		0, 	1, UTED_QUAL_QUAL},
  {"Before", 		0, 	0,		0, 	1, UTED_QUAL_QUAL},
  {"After", 		0, 	0,		0, 	1, UTED_QUAL_QUAL},
  {"",}}},
{ "Create StructFiles", UTED_PROC_PWRPLC, 0, 1,{
  {"Files", 		0, 	0,		1, 	0, UTED_QUAL_QUAL},
  {"",}}},
{ "Create LoadFiles", UTED_PROC_PWRPLC, 0, 1,{
  {"Volume", 		0, 	0,		1, 	0, UTED_QUAL_QUAL},
  {"All",	 	0, 	0,		0, 	1, UTED_QUAL_QUAL},
  {"Class"	, 	0, 	0,		0, 	1, UTED_QUAL_QUAL},
  {"",}}},
{ "Create BootFiles", UTED_PROC_PWRPLC, 0, 1,{
  {"NodeConfig", 	0, 	0,		1, 	0, UTED_QUAL_QUAL},
  {"All", 		0, 	0,		0, 	1, UTED_QUAL_QUAL},
  {"Debug", 		0, 	0,		0, 	1, UTED_QUAL_QUAL},
  {"",}}},
{ "Create RttFiles", UTED_PROC_PWRPLC,  0, 1,	{
  {"",}}},
{ "Delete Object", UTED_PROC_PWRPLC, 0, 1,{
  {"Hierarchy", 	1, 	UTED_INS_PLNO,	1, 	0, UTED_QUAL_QUAL},
  {"Class", 		0, 	0, 		1,	0, UTED_QUAL_QUAL},
  {"Name", 		1, 	UTED_INS_PLNO,	1, 	0, UTED_QUAL_QUAL},
  {"Noconfirm", 	0, 	0,		0, 	1, UTED_QUAL_QUAL},
  {"Log", 		0, 	0,		0, 	1, UTED_QUAL_QUAL},
  {"",}}},
{ "Delete Tree", UTED_PROC_PWRPLC, 0, 1,{
  {"Name", 		1, 	UTED_INS_PLNO,	1, 	0, UTED_QUAL_QUAL},
  {"Noconfirm", 	0, 	0,		0, 	1, UTED_QUAL_QUAL},
  {"Log", 		0, 	0,		0, 	1, UTED_QUAL_QUAL},
  {"",}}},
{ "Wb Dump", UTED_PROC_PWRPLC, 0, 1,{
  {"Output", 		0, 	0, 		1,	0, UTED_QUAL_QUAL},
  {"Hierarchy", 	1, 	UTED_INS_PLNO,	1, 	0, UTED_QUAL_QUAL},
  {"Volume", 		0, 	0,		1, 	0, UTED_QUAL_QUAL},
  {"Header", 		0, 	0,		0, 	1, UTED_QUAL_QUAL},
  {"Indent", 		0, 	0, 		1,	0, UTED_QUAL_QUAL},
  {"",}}},
{ "Wb Load", UTED_PROC_PWRPLC, 0, 1,{
  {"Loadfile", 		0, 	0, 		1,	0, UTED_QUAL_QUAL},
  {"Ignore",		0, 	0,		0, 	1, UTED_QUAL_QUAL},
  {"NoIndex",		0, 	0,		0, 	1, UTED_QUAL_QUAL},
  {"Output", 		0, 	0, 		1,	0, UTED_QUAL_QUAL},
  {"Full",		0, 	0,		0, 	1, UTED_QUAL_QUAL},
  {"Announce",		0, 	0,		0, 	1, UTED_QUAL_QUAL},
  {"",}}},
{ "Wb Export", UTED_PROC_PWRPLC, 0, 1,{
  {"Output", 		0, 	0, 		1,	0, UTED_QUAL_QUAL},
  {"Hierarchy", 	1, 	UTED_INS_PLNO,	1, 	0, UTED_QUAL_QUAL},
  {"",}}},
{ "Wb Import", UTED_PROC_PWRPLC, 0, 1,{
  {"Loadfile", 		0, 	0, 		1,	0, UTED_QUAL_QUAL},
  {"Root", 		0, 	0,		1, 	0, UTED_QUAL_QUAL},
  {"Ignore",		0, 	0,		0, 	1, UTED_QUAL_QUAL},
  {"Output", 		0, 	0, 		1,	0, UTED_QUAL_QUAL},
  {"Full",		0, 	0,		0, 	1, UTED_QUAL_QUAL},
  {"Announce",		0, 	0,		0, 	1, UTED_QUAL_QUAL},
  {"",}}},
{ "",}};

extern pwr_tStatus cli$dcl_parse ();
extern pwr_tStatus cli$dispatch ();
extern int wb_show_cld ();	/* module name in wb_show.cld file */

pwr_tStatus uted_get_command_index (
  uted_ctx	utedctx,
  char		*label,
  int		*index
);
void uted_enable_entries (
  uted_ctx	utedctx,
  int		enable
);
void uted_reset_qual (
  uted_ctx	utedctx
);
void uted_submit (
  uted_ctx	utedctx,
  char		*command
);
void uted_message ( 
  uted_ctx	utedctx,
  char		*new_label
);
void uted_GetCSText (
  XmString	ar_value,
  char		*t_buffer
);
void uted_display_help (
  uted_ctx utedctx,
  char	  *topic_title
);
void uted_configure_quals (
  uted_ctx	utedctx,
  char		*label
);
void uted_questionbox (
  uted_ctx ute,
  char	  *question_title,
  char	  *question_text,
  void	  (* yes_procedure) (),
  void	  (* no_procedure) (),
  void	  (* cancel_procedure) (), 
  pwr_tBoolean cancel
);
int uted_execute	(
  uted_ctx	utedctx,
  int		show
);
void uted_message_error (
  uted_ctx	utedctx,
  pwr_tStatus	sts
);

static void remove_command_window (
  uted_ctx ute
)
{
  int height, command_height;

  if (XtIsManaged(ute->widgets.command_window))
  {
    XtVaGetValues(ute->widgets.uted_window, XmNheight, &height, NULL);      
    XtVaGetValues(ute->widgets.command_window, XmNheight, &command_height,
									 NULL);
    height -= command_height;
    XtVaSetValues(ute->widgets.uted_window, XmNheight, height, NULL);

    XtUnmanageChild( ute->widgets.command_window);
    XmToggleButtonSetState( ute->widgets.commandwind_button, 0, 0);    
  }
}



static void uted_activate_command (
  Widget		w,
  uted_ctx		utedctx,
  XmAnyCallbackStruct	*data
)
{
	Arg 		arg[1];
	char		label[40];
	XmString 	latinstr;
	char		helpstr[80];

	uted_message( utedctx, "");

	/* Get the label on this pushbutton and identify the index in
	  the command table */
	XtSetArg(arg[0], XmNlabelString, &latinstr);
	XtGetValues( w, arg, 1);
	uted_GetCSText( latinstr, label);

	if (data->event->type == ButtonRelease
	  && data->event->xbutton.state & Mod5Mask
	) {
	  strcpy(helpstr, "Help pwr_plc ");
	  strcat(helpstr, label);
	  uted_display_help( utedctx, helpstr);
	  return;
	}


	uted_configure_quals( utedctx, label);
}

static void uted_create_command (
  Widget	w,
  uted_ctx	utedctx,
  XmAnyCallbackStruct	*data
)
{
	Arg 		arg[1];
	char		label[40];
	XmString 	latinstr;
	int		index;

	/* Get the label on this pushbutton and identify the index in
	  the command table */
	XtSetArg(arg[0], XmNlabelString, &latinstr);
	XtGetValues( w, arg, 1);
	uted_GetCSText( latinstr, label);

	uted_get_command_index( utedctx, label, &index);
	utedctx->widgets.optmenubuttons[index] = w;
}

static void uted_activate_helputils (
  Widget		w,
  uted_ctx		utedctx,
  XmAnyCallbackStruct	*data
)
{
  uted_display_help ( utedctx, "Help Utilities" ) ;
}

static void uted_activate_helppwr_plc (
  Widget		w,
  uted_ctx		utedctx,
  XmAnyCallbackStruct	*data
)
{
  uted_display_help ( utedctx, "Help pwr_plc" ) ;
}

static void uted_activate_batch (
  Widget	w,
  uted_ctx	utedctx,
  XmAnyCallbackStruct	*data
)
{
	Arg		args[2];

	if (data->event->type == ButtonRelease
	  && data->event->xbutton.state & Mod5Mask
	) {
	  uted_display_help( utedctx, "Help Utilities Batch");
	  return;
	}

	uted_message( utedctx, "");
	utedctx->batch_sts = UTED_BATCH_BATCH;
	XmTextSetString( utedctx->widgets.timevalue, "");
	XtSetArg(args[0],XmNsensitive, 1);
	XtSetValues( utedctx->widgets.timelabel,args,1);
	XtManageChild( utedctx->widgets.timevalue);
}

static void uted_activate_currsess (
  Widget		w,
  uted_ctx		utedctx,
  XmAnyCallbackStruct	*data
)
{
	Arg		args[2];

	if (data->event->type == ButtonRelease
	  && data->event->xbutton.state & Mod5Mask
	) {
	  uted_display_help( utedctx, "Help Utilities CurrentSession");
	  return;
	}

	uted_message( utedctx, "");
	utedctx->batch_sts = UTED_BATCH_CURRSESS;
	XtUnmanageChild( utedctx->widgets.timevalue);
	XtSetArg(args[0],XmNsensitive, 0);
	XtSetValues( utedctx->widgets.timelabel,args,1);
}


/*************************************************************************
* Name:		void	uted_activate_quit()
*
* Description:
*	Callback from the menu.
**************************************************************************/

static void uted_activate_quit (
  Widget		w,
  uted_ctx		utedctx,
  XmAnyCallbackStruct	*data
)
{

	if (data->event->type == ButtonRelease
	  && data->event->xbutton.state & Mod5Mask
	) {
	  uted_display_help( utedctx, "Help Utilities Close");
	  return;
	}

	uted_delete(utedctx);
}

void uted_set_editmode (
  uted_ctx 	utedctx,
  int		edit,
  ldh_tSesContext ldhses
)
{
  utedctx->ldhses = ldhses;
  uted_enable_entries( utedctx, edit);
}

static void uted_activate_ok (
  Widget		w,
  uted_ctx		utedctx,
  XmAnyCallbackStruct	*data
)
{

	if ( data->reason != 10) return;

	if (data->event->type == ButtonRelease
	  && data->event->xbutton.state & Mod5Mask
	) {
	  uted_display_help( utedctx, "Help Utilities Apply");
	  return;
	}

	uted_message( utedctx, "");
	_WaitCursor(utedctx->widgets.uted_window);
	uted_execute( utedctx, 0);
	_UndefineCursor(utedctx->widgets.uted_window);
}

static void uted_activate_cancel (
  Widget		w,
  uted_ctx		utedctx,
  XmAnyCallbackStruct	*data
)
{

	if (data->event->type == ButtonRelease
	  && data->event->xbutton.state & Mod5Mask
	) {
	  uted_display_help( utedctx, "Help Utilities Cancel");
	  return;
	}

	uted_delete( utedctx);
}

static void uted_activate_show_cmd (
  Widget		w,
  uted_ctx		utedctx,
  XmAnyCallbackStruct	*data
)
{

	if (data->event->type == ButtonRelease
	  && data->event->xbutton.state & Mod5Mask
	) {
	  uted_display_help( utedctx, "Help Utilities ShowCommand");
	  return;
	}

	_WaitCursor(utedctx->widgets.uted_window);
	uted_execute( utedctx, 1);
        _UndefineCursor(utedctx->widgets.uted_window);
}

static void uted_activate_cmd_wind (
  Widget			w,
  uted_ctx			utedctx,
  XmToggleButtonCallbackStruct	*data
)
{
	int		height;
	int		command_height;
	Arg		arg[20];

	if ( data->set > 1) return;

	if (data->event->type == ButtonRelease
	  && data->event->xbutton.state & Mod5Mask
	) {
	  uted_display_help( utedctx, "Help Utilities CommandWindow");
	  XmToggleButtonSetState( w, !data->set, 0);
	  return;
	}

        if ( data->set == 1 ) 
	{
	  /* Increase the height of the ute window */
	  XtSetArg(arg[0], XmNheight, &height);
	  XtGetValues( utedctx->widgets.uted_window, arg, 1);

	  XtSetArg(arg[0], XmNheight, &command_height);
	  XtGetValues( utedctx->widgets.command_window, arg, 1);
	  height += command_height;

	  XtSetArg(arg[0], XmNheight, height);
	  XtSetValues( utedctx->widgets.uted_window, arg, 1);

	  XtManageChild(utedctx->widgets.command_window);
	}
	else
	{
	  remove_command_window( utedctx );
	}
}

static void uted_commandchanged (
  Widget			w,
  uted_ctx			utedctx,
  XmCommandCallbackStruct	*data
)
{
	static char 	str[256];
	pwr_tStatus	sts;
	pwr_tStatus	display_sts;
	int		commandfile;

	uted_message( utedctx, "");
	uted_GetCSText( data->value, str);

	display_sts = 1;
	commandfile = 0;

	dcli_remove_blank( str, str);

	sts = wtt_command( utedctx->cp.parent_ctx, str);
	if ( EVEN(sts))
	{
	  uted_message_error( utedctx, sts);
	  BEEP;
	}
	else
	{
	  uted_message( utedctx, "Done");
	  BEEP;
	}
}

static void uted_activate_present1 (
  Widget			w,
  uted_ctx			utedctx,
  XmToggleButtonCallbackStruct  *data
)
{
	uted_message( utedctx, "");
	utedctx->present_sts[0] = data->set;
}
static void uted_activate_present2 (
  Widget			w,
  uted_ctx			utedctx,
  XmToggleButtonCallbackStruct  *data
)
{
	uted_message( utedctx, "");
	utedctx->present_sts[1] = data->set;
}
static void uted_activate_present3 (
  Widget			w,
  uted_ctx			utedctx,
  XmToggleButtonCallbackStruct  *data
)
{
	uted_message( utedctx, "");
	utedctx->present_sts[2] = data->set;
}
static void uted_activate_present4 (
  Widget			w,
  uted_ctx			utedctx,
  XmToggleButtonCallbackStruct  *data
)
{
	uted_message( utedctx, "");
	utedctx->present_sts[3] = data->set;
}
static void uted_activate_present5 (
  Widget			w,
  uted_ctx			utedctx,
  XmToggleButtonCallbackStruct  *data
)
{
	uted_message( utedctx, "");
	utedctx->present_sts[4] = data->set;
}
static void uted_activate_present6 (
  Widget			w,
  uted_ctx			utedctx,
  XmToggleButtonCallbackStruct  *data
)
{
	uted_message( utedctx, "");
	utedctx->present_sts[5] = data->set;
}
static void uted_activate_present7 (
  Widget			w,
  uted_ctx			utedctx,
  XmToggleButtonCallbackStruct  *data
)
{
	uted_message( utedctx, "");
	utedctx->present_sts[6] = data->set;
}
static void uted_activate_present8 (
  Widget			w,
  uted_ctx			utedctx,
  XmToggleButtonCallbackStruct  *data
)
{
	uted_message( utedctx, "");
	utedctx->present_sts[7] = data->set;
}
static void uted_activate_present9 (
  Widget			w,
  uted_ctx			utedctx,
  XmToggleButtonCallbackStruct  *data
)
{
	uted_message( utedctx, "");
	utedctx->present_sts[8] = data->set;
}
static void uted_activate_present10 (
  Widget			w,
  uted_ctx			utedctx,
  XmToggleButtonCallbackStruct  *data
)
{
	uted_message( utedctx, "");
	utedctx->present_sts[9] = data->set;
}

static void uted_create_label (
  Widget w,
  uted_ctx utedctx,
  XmAnyCallbackStruct *data
) 
{

  utedctx->widgets.label = w;
}

static void uted_create_adb (
  Widget		w,
  uted_ctx		utedctx,
  XmAnyCallbackStruct *data
) 
{

utedctx->widgets.adb = w;
}

static void uted_create_file_entry (
  Widget		w,
  uted_ctx		utedctx,
  XmAnyCallbackStruct	*data
) 
{

  utedctx->widgets.file_entry = w;
}

static void uted_create_quit (
  Widget		w,
  uted_ctx		utedctx,
  XmAnyCallbackStruct	*data
) 
{

  utedctx->widgets.quit = w;
}

static void uted_create_commandwind_button (
  Widget		w,
  uted_ctx		utedctx,
  XmAnyCallbackStruct	*data
)
{

  utedctx->widgets.commandwind_button = w;
}

static void uted_create_batchoptmenu (
  Widget		w,
  uted_ctx		utedctx,
  XmAnyCallbackStruct *data
)
{

  utedctx->widgets.batchoptmenu = w;
}

static void uted_create_commandlabel (
  Widget		w,
  uted_ctx		utedctx,
  XmAnyCallbackStruct	*data
)
{

  utedctx->widgets.commandlabel = w;
}

static void uted_create_batch (
  Widget		w,
  uted_ctx		utedctx,
  XmAnyCallbackStruct	*data
)
{

  utedctx->widgets.batch = w;
}

static void uted_create_currsess (
  Widget		w,
  uted_ctx		utedctx,
  XmAnyCallbackStruct	*data
)
{

  utedctx->widgets.currsess = w;
}

static void uted_create_cmd_wind (
  Widget		w,
  uted_ctx		utedctx,
  XmAnyCallbackStruct	*data
)
{

  utedctx->widgets.command_window = w;
}

static void uted_create_timelabel (
  Widget		w,
  uted_ctx		utedctx, 
  XmAnyCallbackStruct	*data
)
{

  utedctx->widgets.timelabel = w;
}

static void uted_create_timevalue (
  Widget		w,
  uted_ctx		utedctx,
  XmAnyCallbackStruct	*data
)
{

  utedctx->widgets.timevalue = w;
}

static void uted_create_qualifier1 (
  Widget		w,
  uted_ctx		utedctx,
  XmAnyCallbackStruct	*data
)
{

  utedctx->widgets.qualifier[0] = w;
}

static void uted_create_value1 (
  Widget		w,
  uted_ctx		utedctx,
  XmAnyCallbackStruct	*data
)
{

  utedctx->widgets.value[0] = w;
}

static void uted_create_present1 (
  Widget		w,
  uted_ctx		utedctx,
  XmAnyCallbackStruct	*data
)
{

  utedctx->widgets.present[0] = w;
}

static void uted_create_qualifier2 (
  Widget		w,
  uted_ctx		utedctx,
  XmAnyCallbackStruct	*data
)
{

  utedctx->widgets.qualifier[1] = w;
}

static void uted_create_value2 (
  Widget		w,
  uted_ctx		utedctx,
  XmAnyCallbackStruct	*data
)
{

  utedctx->widgets.value[1] = w;
}

static void uted_create_present2 (
  Widget		w,
  uted_ctx		utedctx,
  XmAnyCallbackStruct	*data
)
{

  utedctx->widgets.present[1] = w;
}

static void uted_create_qualifier3 (
  Widget		w,
  uted_ctx		utedctx,
  XmAnyCallbackStruct	*data
)
{

  utedctx->widgets.qualifier[2] = w;
}

static void uted_create_value3 (
  Widget		w,
  uted_ctx		utedctx,
  XmAnyCallbackStruct	*data
)
{

  utedctx->widgets.value[2] = w;
}

static void uted_create_present3 (
  Widget		w,
  uted_ctx		utedctx,
  XmAnyCallbackStruct	*data
)
{

  utedctx->widgets.present[2] = w;
}

static void uted_create_qualifier4 (
  Widget		w,
  uted_ctx		utedctx,
  XmAnyCallbackStruct	*data
)
{
  
  utedctx->widgets.qualifier[3] = w;
}

static void uted_create_value4 (
  Widget		w,
  uted_ctx		utedctx,
  XmAnyCallbackStruct	*data
)
{

  utedctx->widgets.value[3] = w;
}

static void uted_create_present4 (
  Widget		w,
  uted_ctx		utedctx,
  XmAnyCallbackStruct	*data
)
{

  utedctx->widgets.present[3] = w;
}

static void uted_create_qualifier5 (
  Widget		w,
  uted_ctx		utedctx,
  XmAnyCallbackStruct	*data
)
{

  utedctx->widgets.qualifier[4] = w;
}

static void uted_create_value5 (
  Widget		w,
  uted_ctx		utedctx,
  XmAnyCallbackStruct	*data
)
{

  utedctx->widgets.value[4] = w;
}

static void uted_create_present5 (
  Widget		w,
  uted_ctx		utedctx,
  XmAnyCallbackStruct	*data
)
{

  utedctx->widgets.present[4] = w;
}

static void uted_create_qualifier6 (
  Widget		w,
  uted_ctx		utedctx,
  XmAnyCallbackStruct	*data
)
{

  utedctx->widgets.qualifier[5] = w;
}

static void uted_create_value6 (
  Widget		w,
  uted_ctx		utedctx,
  XmAnyCallbackStruct	*data
)
{

  utedctx->widgets.value[5] = w;
}

static void uted_create_present6 (
  Widget		w,
  uted_ctx		utedctx,
  XmAnyCallbackStruct	*data
)
{

  utedctx->widgets.present[5] = w;
}

static void uted_create_qualifier7 (
  Widget		w,
  uted_ctx		utedctx,
  XmAnyCallbackStruct	*data
)
{

  utedctx->widgets.qualifier[6] = w;
}

static void uted_create_value7 (
  Widget		w,
  uted_ctx		utedctx,
  XmAnyCallbackStruct	*data
)
{

  utedctx->widgets.value[6] = w;
}

static void uted_create_present7 (
  Widget		w,
  uted_ctx		utedctx,
  XmAnyCallbackStruct	*data
)
{

  utedctx->widgets.present[6] = w;
}

static void uted_create_qualifier8 (
  Widget		w,
  uted_ctx		utedctx,
  XmAnyCallbackStruct	*data
)
{

  utedctx->widgets.qualifier[7] = w;
}

static void uted_create_value8 (
  Widget		w,
  uted_ctx		utedctx,
  XmAnyCallbackStruct	*data
)
{

  utedctx->widgets.value[7] = w;
}

static void uted_create_present8 (
  Widget		w,
  uted_ctx		utedctx,
  XmAnyCallbackStruct	*data
)
{

  utedctx->widgets.present[7] = w;
}

static void uted_create_qualifier9 (
  Widget		w,
  uted_ctx		utedctx,
  XmAnyCallbackStruct	*data
)
{

  utedctx->widgets.qualifier[8] = w;
}

static void uted_create_value9 (
  Widget		w,
  uted_ctx		utedctx,
  XmAnyCallbackStruct	*data
)
{

  utedctx->widgets.value[8] = w;
}

static void uted_create_present9 (
  Widget		w,
  uted_ctx		utedctx,
  XmAnyCallbackStruct	*data
)
{

  utedctx->widgets.present[8] = w;
}

static void uted_create_qualifier10 (
  Widget		w,
  uted_ctx		utedctx,
  XmAnyCallbackStruct	*data
)
{

  utedctx->widgets.qualifier[9] = w;
}

static void uted_create_value10 (
  Widget		w,
  uted_ctx		utedctx,
  XmAnyCallbackStruct	*data
)
{

  utedctx->widgets.value[9] = w;
}

static void uted_create_present10 (
  Widget		w,
  uted_ctx		utedctx,
  XmAnyCallbackStruct	*data
)
{

  utedctx->widgets.present[9] = w;
}

void uted_qbox_cr (
  Widget  w,
  uted_ctx ute )
{
  ute->widgets.questionbox = w;
}

/************************************************************************
*
* Name: uted_qbox_yes_cb
*
* Type:	void
*
* Type		Parameter	IOGF	Description
* Widget	w		I	Widget to the pane
* uted_ctx	ute		I	Context variable
*
* Description:
*   Execute the yes procedure for the questionbox.
*
************************************************************************/

void uted_qbox_yes_cb (
  Widget  w,
  uted_ctx ute
)
{
  XtUnmanageChild( ute->widgets.questionbox);

  if ( ute->questionbox_yes != NULL)
    (ute->questionbox_yes) ( ute);
}

/************************************************************************
*
* Name: uted_qbox_no_cb
*
* Type:	void
*
* Type		Parameter	IOGF	Description
* Widget	w		I	Widget to the pane
* uted_ctx	ute		I	Context variable
*
* Description:
*   Execute the no procedure for the questionbox.
*
************************************************************************/

void uted_qbox_no_cb (
  Widget  w,
  uted_ctx ute
)
{
  XtUnmanageChild( ute->widgets.questionbox);

  if ( ute->questionbox_no != NULL)
    (ute->questionbox_no) ( ute);
}

/************************************************************************
*
* Name: uted_qbox_cancel_cb
*
* Type:	void
*
* Type		Parameter	IOGF	Description
* Widget	w		I	Widget to the pane
* uted_ctx	ute		I	Context variable
*
* Description:
*   Unmanage the questionbox.
*
************************************************************************/

void uted_qbox_cancel_cb	(
  Widget  w,
  uted_ctx ute
)
{
  if ( ute->questionbox_cancel != NULL)
    (ute->questionbox_cancel) ( ute);
  XtUnmanageChild( ute->widgets.questionbox);
}

/*************************************************************************
*
* Name:		uted_ctx	uted_new()
*
* Type		int
*
* Type		Parameter	IOGF	Description
* void 		*parent_ctx	I	Parent context adress
* Widget	parent_wid	I	Parent Widget adress
* char *	name		I	Name of the created object
* unsigned long ldhsession	I	ldh session.
*
* Description:
*	Create a new ute window
**************************************************************************/

pwr_tStatus uted_new (
  void			*parent_ctx,
  Widget		parent_wid,
  char			*name,
  char			*iconname,
  ldh_tWBContext	ldhwb,
  ldh_tSesContext	ldhsession,
  int			editmode,
  uted_ctx		*return_utedctx,
  void 			(*quit_cb)(void *)
)
{
  Arg		args[20];
  pwr_tStatus	sts;
  int i;
  uted_ctx	utedctx;
  MrmHierarchy	s_MrmH;
  MrmType	dclass;

  /* DRM database hierarchy related variables */
  static Pixmap icon = 0;
  Dimension	icon_w;
  Dimension	icon_h;

  static Atom	MwmDeleteWind = 0;
  char		uid_filename[200] = {"pwr_exe:wb_uted.uid"};
   char		*uid_filename_p = uid_filename;

  static MrmRegisterArg	reglist[] = {
    /* First the context variable */
    { "uted_ctx", 0 },

    /* Callbacks for the controlled foe widget */
    {"uted_create_adb",(caddr_t)uted_create_adb},
    {"uted_create_quit",(caddr_t)uted_create_quit},
    {"uted_activate_quit",(caddr_t)uted_activate_quit},
    {"uted_create_commandlabel",(caddr_t)uted_create_commandlabel},
    {"uted_create_batchoptmenu",(caddr_t)uted_create_batchoptmenu},
    {"uted_create_batch",(caddr_t)uted_create_batch},
    {"uted_create_currsess",(caddr_t)uted_create_currsess},
    {"uted_create_file_entry",(caddr_t)uted_create_file_entry},
    {"uted_create_label",(caddr_t)uted_create_label},
    {"uted_create_adb",(caddr_t)uted_create_adb},
    {"uted_activate_command",(caddr_t)uted_activate_command},
    {"uted_create_command",(caddr_t)uted_create_command},
    {"uted_activate_batch",(caddr_t)uted_activate_batch},
    {"uted_activate_currsess",(caddr_t)uted_activate_currsess},
    {"uted_activate_ok",(caddr_t)uted_activate_ok},
    {"uted_activate_cancel",(caddr_t)uted_activate_cancel},
    {"uted_activate_show_cmd",(caddr_t)uted_activate_show_cmd},
    {"uted_create_commandwind_button",(caddr_t)uted_create_commandwind_button},
    {"uted_activate_cmd_wind",(caddr_t)uted_activate_cmd_wind},
    {"uted_create_cmd_wind",(caddr_t)uted_create_cmd_wind},
    {"uted_create_timelabel",(caddr_t)uted_create_timelabel},
    {"uted_create_timevalue",(caddr_t)uted_create_timevalue},
    {"uted_create_qualifier1",(caddr_t)uted_create_qualifier1},
    {"uted_create_value1",(caddr_t)uted_create_value1},
    {"uted_activate_present1",(caddr_t)uted_activate_present1},
    {"uted_create_present1",(caddr_t)uted_create_present1},
    {"uted_create_qualifier2",(caddr_t)uted_create_qualifier2},
    {"uted_create_value2",(caddr_t)uted_create_value2},
    {"uted_activate_present2",(caddr_t)uted_activate_present2},
    {"uted_create_present2",(caddr_t)uted_create_present2},
    {"uted_create_qualifier3",(caddr_t)uted_create_qualifier3},
    {"uted_create_value3",(caddr_t)uted_create_value3},
    {"uted_activate_present3",(caddr_t)uted_activate_present3},
    {"uted_create_present3",(caddr_t)uted_create_present3},
    {"uted_create_qualifier4",(caddr_t)uted_create_qualifier4},
    {"uted_create_value4",(caddr_t)uted_create_value4},
    {"uted_activate_present4",(caddr_t)uted_activate_present4},
    {"uted_create_present4",(caddr_t)uted_create_present4},
    {"uted_create_qualifier5",(caddr_t)uted_create_qualifier5},
    {"uted_create_value5",(caddr_t)uted_create_value5},
    {"uted_activate_present5",(caddr_t)uted_activate_present5},
    {"uted_create_present5",(caddr_t)uted_create_present5},
    {"uted_create_qualifier6",(caddr_t)uted_create_qualifier6},
    {"uted_create_value6",(caddr_t)uted_create_value6},
    {"uted_activate_present6",(caddr_t)uted_activate_present6},
    {"uted_create_present6",(caddr_t)uted_create_present6},
    {"uted_create_qualifier7",(caddr_t)uted_create_qualifier7},
    {"uted_create_value7",(caddr_t)uted_create_value7},
    {"uted_activate_present7",(caddr_t)uted_activate_present7},
    {"uted_create_present7",(caddr_t)uted_create_present7},
    {"uted_create_qualifier8",(caddr_t)uted_create_qualifier8},
    {"uted_create_value8",(caddr_t)uted_create_value8},
    {"uted_activate_present8",(caddr_t)uted_activate_present8},
    {"uted_create_present8",(caddr_t)uted_create_present8},
    {"uted_create_qualifier9",(caddr_t)uted_create_qualifier9},
    {"uted_create_value9",(caddr_t)uted_create_value9},
    {"uted_activate_present9",(caddr_t)uted_activate_present9},
    {"uted_create_present9",(caddr_t)uted_create_present9},
    {"uted_create_qualifier10",(caddr_t)uted_create_qualifier10},
    {"uted_create_value10",(caddr_t)uted_create_value10},
    {"uted_activate_present10",(caddr_t)uted_activate_present10},
    {"uted_create_present10",(caddr_t)uted_create_present10},
    {"uted_activate_helputils",(caddr_t)uted_activate_helputils},
    {"uted_activate_helppwr_plc",(caddr_t)uted_activate_helppwr_plc},
    {"uted_commandchanged",(caddr_t)uted_commandchanged},
    {"uted_qbox_cr",(caddr_t)uted_qbox_cr},
    {"uted_qbox_yes_cb",(caddr_t)uted_qbox_yes_cb},
    {"uted_qbox_no_cb",(caddr_t)uted_qbox_no_cb},
    {"uted_qbox_cancel_cb",(caddr_t)uted_qbox_cancel_cb}
  };

  static int	reglist_num = (sizeof reglist / sizeof reglist[0]);

/*
 * Now start the module creation
 */

  sts = dcli_translate_filename( uid_filename, uid_filename);
  if ( EVEN(sts))
  {
    printf( "** pwr_exe is not defined\n");
    exit(0);
  }

/* Create object context */
 utedctx =   (uted_ctx) XtCalloc( 1, sizeof( *utedctx ) );

 utedctx->cp.parent_ctx = parent_ctx;
 utedctx->cp.name = XtNewString( name );

 utedctx->ldhwb = ldhwb;
 utedctx->ldhses = ldhsession;
 utedctx->quit_cb = quit_cb;

/* set initialization values in context */

/* Save the context structure in the widget */
 XtSetArg (args[0], XmNuserData, (unsigned int) utedctx);

/*
 * Create a new widget
 * Open the UID files (the output of the UIL compiler) in the hierarchy
 * Register the items DRM needs to bind for us.
 * Create a new widget
 * Close the hierarchy
 * Compile the additional button translations and augment and add actions
 */ 
  sts = MrmOpenHierarchy( 1, &uid_filename_p, NULL, &s_MrmH);
  if (sts != MrmSUCCESS) printf("can't open hierarchy\n");

  reglist[0].value = (caddr_t) utedctx;

  MrmRegisterNames(reglist, reglist_num);

  if (icon == 0)
    sts = MrmFetchBitmapLiteral(s_MrmH,"icon", 
	      XtScreen(parent_wid), XtDisplay(parent_wid),
	      &icon, &icon_w, &icon_h);

  i=0;
  XtSetArg(args[i],XmNiconName, iconname);  i++;
  XtSetArg(args[i],XmNiconPixmap, icon);  i++;
  XtSetArg(args[i],XmNtitle, name);  i++;
  
  /* SG 11.02.91 Save the id of the top in the context */ 
  utedctx->cp.parent_wid = XtCreatePopupShell("utilities", 
			       topLevelShellWidgetClass, parent_wid, args, i);

  /* the positioning of a top level can only be define after the creation
      of the widget . So i do it know: 
     SG 24.02.91 use the parameters received x and y 
*/
  i=0;
  XtSetArg(args[i],XmNx,100);i++;
  XtSetArg(args[i],XmNy,100);i++;
  XtSetArg(args[i],XtNallowShellResize,TRUE), i++;
  XtSetValues( utedctx->cp.parent_wid  ,args,i);

  /* now that we have a top level we can get the main window */
  sts = MrmFetchWidgetOverride(s_MrmH, "uted_window", utedctx->cp.parent_wid ,
		name, args, 1,
		&utedctx->widgets.uted_window, &dclass);
  if (sts != MrmSUCCESS) printf("can't fetch utedit widget\n");

  XtManageChild(utedctx->widgets.uted_window);

  /* SG 09.02.91 a top level should always be realized ! */
  XtPopup( utedctx->cp.parent_wid, XtGrabNone );

  /* Ask MRM to fetch the question box */
  i=0;
  if (MrmFetchWidgetOverride(s_MrmH,
			"uted_qbox", 
	     	        utedctx->widgets.uted_window,
			0,
			args , i,
			&utedctx->widgets.questionbox, &dclass) != MrmSUCCESS)
  {
    printf("can't fetch uted_qbox widget\n");
  }

  MrmCloseHierarchy(s_MrmH);

  uted_reset_qual( utedctx);

  utedctx->batch_sts = UTED_BATCH_CURRSESS;
  XtSetArg(args[0],XmNsensitive, 0);
  XtSetValues( utedctx->widgets.timelabel,args,1);
  XtUnmanageChild( utedctx->widgets.timevalue);
  XtUnmanageChild( utedctx->widgets.command_window);
  uted_enable_entries( utedctx, editmode);


  if (MwmDeleteWind == 0)
     MwmDeleteWind = XInternAtom(XtDisplay(utedctx->cp.parent_wid), "WM_DELETE_WINDOW", FALSE);

  if (MwmDeleteWind != 0)
  {
    XtVaSetValues(utedctx->cp.parent_wid, XmNdeleteResponse, XmDO_NOTHING, NULL);
    XmAddWMProtocolCallback(utedctx->cp.parent_wid, MwmDeleteWind, 
       (XtCallbackProc) uted_activate_quit, utedctx);
  }
  *return_utedctx = utedctx;
  return FOE__SUCCESS;
}


/*************************************************************************
*
* Name:		uted_delete()
*
* Type		void
*
* Type		Parameter	IOGF	Description
* foe_ctx	*utedctx		I	ute context.
*
* Description:
*	Destroys a ute instance.
*	Destroys the widget and frees allocated memory for the 
*	context.
*
**************************************************************************/

void uted_delete(
  uted_ctx	utedctx
)
{

	(utedctx->quit_cb)( utedctx->cp.parent_ctx); 

	/* Destroy the widget */
	XtDestroyWidget(utedctx->cp.parent_wid);

	/* Delete the context */
	XtFree((char *) utedctx);
}



/*************************************************************************
*
* Name:		uted_reset_qual()
*
* Type		void
*
* Type		Parameter	IOGF	Description
* uoe_ctx	*utedctx		I	ute context.
*
* Description:
*	Take away all qualifiers.
*
**************************************************************************/

void uted_reset_qual (
  uted_ctx	utedctx
)
{
	int		i;
	XmString 	cstr;

	for( i = 0; i < UTED_QUALS; i++)
	{
	  XtUnmanageChild( utedctx->widgets.qualifier[i]);
	  XtUnmanageChild( utedctx->widgets.value[i]);
	  XtUnmanageChild( utedctx->widgets.present[i]);
	  utedctx->present_sts[i] = 0;
	  XmToggleButtonSetState( utedctx->widgets.present[i], 0, 0);
	  XmTextSetString( utedctx->widgets.value[i], "");
	}

	cstr = XmStringCreateSimple( UTED_TEXT_NOCOMMAND);
	XtVaSetValues( utedctx->widgets.commandlabel, 
	  XmNlabelString, cstr,
	  NULL);
	XmStringFree( cstr);

	utedctx->current_index = UTED_INDEX_NOCOMMAND;
}

int uted_execute	(
  uted_ctx	utedctx,
  int		show
)
{
	uted_t_command	*command_ptr;
	uted_t_qual	*qual_ptr;
	char		cmd[256];
	char		command[280];
	char		*value;
	pwr_tStatus	sts;
	int		i;
	int		index;
	static char 	str[256] ;
	XmString	cstr;
	Arg		arg[10];

	if ( utedctx->current_index == UTED_INDEX_NOCOMMAND)
	{
	  uted_message( utedctx, "Select a command in the menu");
	  BEEP;
	  return FOE__SUCCESS;
	}

	index = utedctx->current_index;
	command_ptr = &commands[index];
	strcpy(cmd, command_ptr->command);

	i = 0;
	qual_ptr = &commands[index].qualifier[0];
	while ( qual_ptr->qual[0] != 0)
	{
	  if ( qual_ptr->present)
	  {
	    if ( utedctx->present_sts[i])
	    {
	      if ( qual_ptr->type == UTED_QUAL_QUAL )
	      {
	        strcat(cmd, "/");
	        strcat(cmd, qual_ptr->qual);
	      }
	      else if ( qual_ptr->type == UTED_QUAL_PAR )
	      {
	        strcat(cmd, " ");
	        strcat(cmd, qual_ptr->qual);
	      }
	    }
	  }
	  else
	  {
	    {
	      /* Get the value */
	      value = XmTextGetString( utedctx->widgets.value[i]);
	      if ( *value != 0)
	      {
	        if ( qual_ptr->type == UTED_QUAL_QUAL )
	        {
	          strcat(cmd, "/");
	          strcat(cmd, qual_ptr->qual);
	          strcat(cmd, "=");
	          strcat(cmd, value);
	        }
	        else if ( qual_ptr->type == UTED_QUAL_PAR )
	        {
	          strcat(cmd, " ");
	          strcat(cmd, value);
	        }
	      }
	      XtFree( value);
	    }
	  }
	  qual_ptr++;
	  i++;
	}
	if ( show)
	{
	}

	/* Execute something... */
	if ( command_ptr->process == UTED_PROC_PWRPLC)
	{
	  if ( show)
	  {
	    strcpy(command, "Command: pwr_plc ");
	    strcat(command, cmd);
	    uted_message( utedctx, command);

	    /* Put in command window */
	    cstr=XmStringCreateLtoR( cmd, "ISO8859-1");
	    XtSetArg(arg[0], XmNcommand, cstr);
	    XtSetValues( utedctx->widgets.command_window, arg, 1);
	    XmStringFree( cstr);
	    return FOE__SUCCESS;
	  }
	  if (utedctx->batch_sts == UTED_BATCH_CURRSESS)
	  {
	    /* Call pwr_plc in the current session */
	    strcpy( str, cmd);
 	    sts = wtt_command( utedctx->cp.parent_ctx, str);
	    if ( EVEN(sts))
	      uted_message_error( utedctx, sts);
	    else
	    {
	      uted_message( utedctx, "Done");
	      BEEP;
	    }
	  }
	  else if (utedctx->batch_sts == UTED_BATCH_BATCH)
	  {
	    uted_message( utedctx, "Batch not implemented in this environment");
	  }
	}
	else if ( command_ptr->process == UTED_PROC_DCL)
	{
	  if (utedctx->batch_sts == UTED_BATCH_CURRSESS)
	  {
	    /* Call dcl in the current session */
	  }
	  else if (utedctx->batch_sts == UTED_BATCH_BATCH)
	  {
	    /* Submit dcl */
	  }
	}
	return FOE__SUCCESS;
}

void uted_GetCSText (
  XmString	ar_value,
  char		*t_buffer
)
{
    char			*first_seg;
    XmStringContext		context;
    XmStringCharSet 		charset;
    XmStringDirection 		dir_r_to_l;
    Boolean 			sep;

    if (ar_value == 0)
    {
	strcpy(t_buffer,"");
	return;
    }
    
    
    XmStringInitContext( &context, ar_value);
    
    if ( XmStringGetNextSegment( context, &first_seg, &charset, &dir_r_to_l,
			&sep) != TRUE )
    {
      *first_seg = 0;
    }
    else
    {
      strcpy(t_buffer,first_seg);
      XtFree(first_seg);
#if 0
    /* I don't think that this should be freed, ML 950120 */
      XtFree (charset);
#endif
    }
    XmStringFreeContext (context);

}


/*************************************************************************
*
* Name:		uted_message()
*
* Type		int
*
* Type		Parameter	IOGF	Description
* uted_ctx	utedctx		I	ute context.
* char 		*new_label	I	message to show. 
*
* Description:
*	Displays a message in the ute window.
*	Changes the label of a label widget which id is stored in the context.
*
**************************************************************************/

void uted_message ( 
  uted_ctx	utedctx,
  char		*new_label
) 
{
	Arg		args[2];
	XmString	cstr;

	XtSetArg(args[0], XmNlabelString,
		cstr=XmStringCreateLtoR(new_label , "ISO8859-1"));
	XtSetValues( utedctx->widgets.label, args,1);
	XmStringFree( cstr);
}


/*************************************************************************
*
* Name:		uted_configure_quals()
*
* Type		int
*
* Type		Parameter	IOGF	Description
* uted_ctx	utedctx		I	ute context.
* char 		*label		I	command.
* int		*index		O 	command index in command table.
*
* Description:
*
**************************************************************************/

pwr_tStatus uted_get_command_index (
  uted_ctx	utedctx,
  char		*label,
  int		*index
)
{
	uted_t_command	*command_ptr;
	int		i;
	int		found;

	i = 0;
	found = 0;
	command_ptr = commands;
	while ( command_ptr->command[0] != 0)
	{
	  if ( strcmp( command_ptr->command, label) == 0)
	  {
	    *index = i;
	    found = 1;
	    break;
	  }
	  i++;
	  command_ptr++;
	}
	if ( !found)
	  return 0;

	return FOE__SUCCESS;
}

/*************************************************************************
*
* Name:		uted_configure_quals()
*
* Type		int
*
* Type		Parameter	IOGF	Description
* uted_ctx	utedctx		I	ute context.
* char 		*label		I	command.
*
* Description:
*
**************************************************************************/

void uted_configure_quals (
  uted_ctx	utedctx,
  char		*label
)
{
	Arg 		arg[1];
	Arg 		args[1];
	int		i;
	int		index;
	uted_t_qual	*qual_ptr;
	XmString 	latinstr;

	/* Reset the picture */
	uted_reset_qual( utedctx);


	uted_get_command_index( utedctx, label, &index);
	utedctx->current_index = index;

	/* Make the batch buttons sensitive or not */
	if ( commands[index].batch_sensitivity & 1)
	{
	  XtSetArg(args[0],XmNsensitive, 1);
	  XtSetValues( utedctx->widgets.currsess,args,1);
	}
	else
	{
	  XtSetArg(args[0],XmNsensitive, 0);
	  XtSetValues( utedctx->widgets.currsess,args,1);
	  XtSetArg(args[0],XmNmenuHistory, utedctx->widgets.batch);
	  XtSetValues( utedctx->widgets.batchoptmenu,args,1);
	  utedctx->batch_sts = UTED_BATCH_BATCH;
	}
	if ( commands[index].batch_sensitivity & 2)
	{
	  XtSetArg(args[0],XmNsensitive, 1);
	  XtSetValues( utedctx->widgets.batch,args,1);
	}
	else
	{
	  XtSetArg(args[0],XmNsensitive, 0);
	  XtSetValues( utedctx->widgets.batch,args,1);
	  XtSetArg(args[0],XmNmenuHistory, utedctx->widgets.currsess);
	  XtSetValues( utedctx->widgets.batchoptmenu,args,1);
	  utedctx->batch_sts = UTED_BATCH_CURRSESS;
	}
	
	/* Check the qualifiers and view the corresponding widgets */
	i = 0;
	qual_ptr = &commands[index].qualifier[0];
	while ( qual_ptr->qual[0] != 0)
	{
	  if ( qual_ptr->type == UTED_QUAL_DEFQUAL)
	  {
	    latinstr = XmStringCreateLtoR( 
		"Select an object in the Navigator", "ISO8859-1");
	    XtSetArg( arg[0], XmNlabelString, latinstr);
	    XtSetValues( utedctx->widgets.qualifier[i], arg, 1);
	    XmStringFree( latinstr);
	    XtManageChild( utedctx->widgets.qualifier[i]);
	  }
	  else
	  {
	    latinstr = XmStringCreateLtoR( qual_ptr->qual, "ISO8859-1");
	    XtSetArg( arg[0], XmNlabelString, latinstr);
	    XtSetValues( utedctx->widgets.qualifier[i], arg, 1);
	    XmStringFree( latinstr);
	    XtManageChild( utedctx->widgets.qualifier[i]);
	    if ( qual_ptr->present)
	       XtManageChild( utedctx->widgets.present[i]);
	    if ( qual_ptr->value)
	      XtManageChild( utedctx->widgets.value[i]);
	  }
	  qual_ptr++;
	  i++;
	}
	latinstr = XmStringCreateSimple( label);
	XtVaSetValues( utedctx->widgets.commandlabel, 
	  XmNlabelString, latinstr,
	  NULL);
	XmStringFree( latinstr);
}

void uted_get_filename (
  char	*filename
)
{
	int 	val;
	pwr_tTime	time;

	clock_gettime(CLOCK_REALTIME, &time);
	srand( time.tv_sec);
	val = rand();
	sprintf( filename, "pwrp_tmp:uted_batch_%d.com", val);
}

#if 0
void uted_submit (
  uted_ctx	utedctx,
  char		*command
)
{
	char	filename[80];
	char	cmd[80];
	FILE	*fout;
	char		*value;

	/* Create submit command string */
	strcpy(cmd, "pwr_uted_submit");

	/* Get the time if there is one */
	value = XmTextGetString( utedctx->widgets.timevalue);
	if ( *value != 0)
	{
	  strcat(cmd, "/after=");
	  strcat(cmd, value);
	}
	XtFree( value);

	/* create a .com file */
	uted_get_filename(filename);

	fout = fopen(filename,"w");
	fprintf( fout, "$ %s\n", command);
	fclose( fout);

	strcat(cmd, " ");
	strcat(cmd, filename);
	system(cmd);
}
#endif


/*************************************************************************
*
* Name:		uted_message_error()
* Type		void
*
* Type		Parameter	IOGF	Description
* unsigned long	sts		I	error message to print.
*
* Description:
*	Show a message.
*
**************************************************************************/
void uted_message_error (
  uted_ctx	utedctx,
  pwr_tStatus	sts
)
{
	char msg[256];

	msg_GetMsg( sts, msg,sizeof(msg));
	uted_message(utedctx, msg);
	BEEP;
}


/*************************************************************************
*
* Name:		uted_get_message_error()
* Type		void
*
* Type		Parameter	IOGF	Description
* unsigned long	sts		I	error message to print.
*
* Description:
*	Show a message.
*
**************************************************************************/
void uted_get_message_error (
  uted_ctx	utedctx,
  pwr_tStatus	sts,
  char		*str
)
{
	char msg[256];

	msg_GetMsg( sts, msg, sizeof(msg));
	strncpy(str, msg, 80);
}


/*************************************************************************
*
* Name:		uted_enable_entries()
* Type		void
*
* Type		Parameter	IOGF	Description
* unsigned long	sts		I	error message to print.
*
* Description:
*
**************************************************************************/
void uted_enable_entries (
  uted_ctx	utedctx,
  int		enable
)
{
	Arg	args[20];
	int 	j;
	uted_t_command	*command_ptr;

	if ( enable)
	  utedctx->mode = UTED_MODE_EDIT;
	else
	  utedctx->mode = UTED_MODE_VIEW;

	XtSetArg(args[0],XmNsensitive, enable );
	XtSetValues( utedctx->widgets.commandwind_button,args,1);

	command_ptr = commands;
	j = 0;
	while ( command_ptr->command[0] != 0)
	{
	  if ( !command_ptr->view_sensitivity )
	  {
	    XtSetValues( utedctx->widgets.optmenubuttons[j],args,1);
	  }
	  j++;
	  command_ptr++;
	}

	/* If view and current command is not sensitiv, change to first 
	   command */
	if ( !enable)
	{
	  if ( utedctx->current_index != UTED_INDEX_NOCOMMAND &&
	       !commands[ utedctx->current_index].view_sensitivity )
	  {
	    uted_reset_qual( utedctx);
	  }
	}

}


/*************************************************************************
*
* Name:		uted_display_help()
*
* Type		void
*
* Type		Parameter	IOGF	Description
* char 		*topic_title	I	string whith help title.
* uted_ctx 	utedctx		I	ute context.
*
* Description:
*	Display a help box widget.
*       Receive the char string which describle the key acces to the topic
*	that will be first topic.
* SG : 14.04.91  
**************************************************************************/

void uted_display_help (
  uted_ctx utedctx,
  char	  *topic_title
)
{
}

/*************************************************************************
*
* Name:		void uted_questionbox()
*
* Type		void
*
* Type		Parameter	IOGF	Description
* uted_ctx	ute		I	ute context.
* char 		*question_title I	title of the question box.
* char 		*question_text	I	question to be displayed.
* voidp         yes_procedure   I       procedure called when yes is activated
* voidp         no_procedure   	I       procedure called when no is activated
*
* Description:
* 	Displays a question box.
*	When the yes-button in the box is pressed the yes_procedure is
*	called. When the no-button is pressed the no_procedure is called.
*	yes_procedure and no_procedure can be NULL.
*	Declaration of the yes and no procedures:
*		void yes_or_no_answer( uted_ctx ute)
*
**************************************************************************/

void uted_questionbox (
  uted_ctx ute,
  char	  *question_title,
  char	  *question_text,
  void	  (* yes_procedure) (),
  void	  (* no_procedure) (),
  void	  (* cancel_procedure) (), 
  pwr_tBoolean cancel
) 
{
	Arg		args[5];
	int 		i;
	XmString	cstr;
	XmString	cstr2;
	XmString	help_label;
	Widget		help_button;
	Widget		yes_button;

	cstr = XmStringCreateLtoR( question_text, "ISO8859-1");
	cstr2 = XmStringCreateLtoR( question_title, "ISO8859-1");
	help_button = XmMessageBoxGetChild(ute->widgets.questionbox, 
	  XmDIALOG_HELP_BUTTON);
	yes_button = XmMessageBoxGetChild( ute->widgets.questionbox, 
	  XmDIALOG_OK_BUTTON);

	if (cancel)
	  help_label = XmStringCreateLtoR("Cancel", "ISO8859-1");
	else
	  help_label = XmStringCreateLtoR("Help", "ISO8859-1");

        i=0;
	XtSetArg(args[i], XmNhelpLabelString, help_label); i++;
	XtSetArg(args[i], XmNmessageString, cstr); i++;
        XtSetArg (args[i], XmNdialogTitle, cstr2); i++;
	XtSetValues( ute->widgets.questionbox, args,i);                    

	if (cancel)
	  XtVaSetValues(help_button, XmNsensitive, 1, NULL);
	else
	  XtVaSetValues(help_button, XmNsensitive, 0, NULL);

        XtManageChild( ute->widgets.questionbox);

	XmStringFree( cstr);
	XmStringFree( cstr2);
	XmStringFree(help_label);

	/* Store the yes and no functions in the context */
        ute->questionbox_yes = yes_procedure;        
        ute->questionbox_no = no_procedure;
        ute->questionbox_cancel = cancel_procedure;

	XmProcessTraversal(yes_button, XmTRAVERSE_CURRENT);

}


/************************************************************************
*
* Name: uted_raise_window ( uted_ctx utedctx)
*
* Type: void
*
* Type		Parameter	IOGF	Description
* uted_ctx	utedctx		I	Context variable
*
* Description:
*	Raises window
*
*************************************************************************/

void uted_raise_window (uted_ctx utedctx)
{
   Widget shell;

   shell = XtParent( utedctx->widgets.uted_window);
   while (!XtIsShell(shell))
      shell = XtParent(shell);

   XMapRaised(XtDisplay(shell), XtWindow(shell));
}
