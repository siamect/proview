/* xtt_evlist.cpp -- Console message window.

   PROVIEW/R  
   Copyright (C) 1996 by Comator Process AB.

   <Description>.  */

#include "glow_std.h"

#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>

extern "C" {
#include "co_cdh.h"
#include "co_dcli.h"
#include "co_time.h"
#include "pwr_baseclasses.h"
#include "rt_gdh.h"
#include "rt_syi.h"
}

#include <Xm/Xm.h>
#include <Xm/XmP.h>
#include <Xm/Text.h>
#include <Mrm/MrmPublic.h>
#include <X11/Intrinsic.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>

// Status is defined as int i xlib...
#ifdef Status
# undef Status
#endif

extern "C" {
#include "flow_x.h"
#include "co_mrm_util.h"
}
#include "flow.h"
#include "flow_browctx.h"
#include "flow_browapi.h"
#include "flow_browwidget.h"
#include "xtt_clognav.h"
#include "xtt_menu.h"

#include "xnav_bitmap_leaf12.h"
#include "xnav_bitmap_map12.h"
#include "xnav_bitmap_openmap12.h"

static int clognav_init_brow_cb( FlowCtx *fctx, void *client_data);
static int clognav_brow_cb( FlowCtx *ctx, flow_tEvent event);

//
//  Free pixmaps
//
void CLogNavBrow::free_pixmaps()
{
  brow_FreeAnnotPixmap( ctx, pixmap_leaf);
  brow_FreeAnnotPixmap( ctx, pixmap_map);
  brow_FreeAnnotPixmap( ctx, pixmap_openmap);
}

//
//  Create pixmaps for leaf, closed map and open map
//
void CLogNavBrow::allocate_pixmaps()
{
	flow_sPixmapData pixmap_data;
	int i;

          for ( i = 0; i < 9; i++)
          {
	    pixmap_data[i].width =xnav_bitmap_leaf12_width;
	    pixmap_data[i].height =xnav_bitmap_leaf12_height;
	    pixmap_data[i].bits = (char *)xnav_bitmap_leaf12_bits;
          }

	  brow_AllocAnnotPixmap( ctx, &pixmap_data, &pixmap_leaf);

          for ( i = 0; i < 9; i++)
          {
	    pixmap_data[i].width =xnav_bitmap_map12_width;
	    pixmap_data[i].height =xnav_bitmap_map12_height;
	    pixmap_data[i].bits = (char *)xnav_bitmap_map12_bits;
          }

	  brow_AllocAnnotPixmap( ctx, &pixmap_data, &pixmap_map);

          for ( i = 0; i < 9; i++)
          {
	    pixmap_data[i].width =xnav_bitmap_openmap12_width;
	    pixmap_data[i].height =xnav_bitmap_openmap12_height;
	    pixmap_data[i].bits = (char *)xnav_bitmap_openmap12_bits;
          }

	  brow_AllocAnnotPixmap( ctx, &pixmap_data, &pixmap_openmap);

}

//
// Create nodeclasses
//
void CLogNavBrow::create_nodeclasses()
{
  allocate_pixmaps();

  // Create common-class

  brow_CreateNodeClass( ctx, "EventDefault", 
		flow_eNodeGroup_Common, &nc_event);
  brow_AddFrame( nc_event, 0, 0, 35, 0.8, flow_eDrawType_Line, -1, 1);
  brow_AddAnnot( nc_event, 0.8, 0.6, 0,
		flow_eDrawType_TextHelveticaBold, 2, flow_eAnnotType_OneLine, 
		0);
  brow_AddAnnotPixmap( nc_event, 0, 1.6, 0.1, flow_eDrawType_Line, 2, 0);
  brow_AddAnnotPixmap( nc_event, 1, 2.2, 0.1, flow_eDrawType_Line, 2, 0);
  brow_AddAnnot( nc_event, 3.3, 0.6, 1,
		flow_eDrawType_TextHelveticaBold, 2, flow_eAnnotType_OneLine, 
		0);
  brow_AddAnnot( nc_event, 10, 0.6, 2,
		flow_eDrawType_TextHelveticaBold, 2, flow_eAnnotType_OneLine, 
		0);
  brow_AddAnnot( nc_event, 26, 0.6, 3,
		flow_eDrawType_TextHelveticaBold, 2, flow_eAnnotType_OneLine, 
		1);

  // Nodeclass for Info message
  brow_CreateNodeClass( ctx, "InfoMsg", 
		flow_eNodeGroup_Common, &nc_msg_info);
  brow_AddFrame( nc_msg_info, 0, 0, 35, 0.8, flow_eDrawType_Line, -1, 1);
  brow_AddFilledRect( nc_msg_info, 0.2, 0.15, 0.4, 0.4, flow_eDrawType_Green);
  brow_AddRect( nc_msg_info, 0.2, 0.15, 0.4, 0.4, flow_eDrawType_Line, 0, 0);
  brow_AddAnnot( nc_msg_info, 0.8, 0.6, 0,
		flow_eDrawType_TextHelveticaBold, 2, flow_eAnnotType_OneLine, 
		0);
  brow_AddAnnot( nc_msg_info, 2, 0.6, 1,
		flow_eDrawType_TextHelveticaBold, 2, flow_eAnnotType_OneLine, 
		0);
  brow_AddAnnot( nc_msg_info, 8, 0.6, 2,
		flow_eDrawType_TextHelveticaBold, 2, flow_eAnnotType_OneLine, 
		0);
  brow_AddAnnot( nc_msg_info, 11, 0.6, 3,
		flow_eDrawType_TextHelveticaBold, 2, flow_eAnnotType_OneLine, 
		0);
  brow_AddAnnot( nc_msg_info, 18, 0.6, 4,
		flow_eDrawType_TextHelveticaBold, 2, flow_eAnnotType_OneLine, 
		0);

  // Nodeclass for Warning message
  brow_CreateNodeClass( ctx, "WarningMsg", 
		flow_eNodeGroup_Common, &nc_msg_warning);
  brow_AddFrame( nc_msg_warning, 0, 0, 35, 0.8, flow_eDrawType_Line, -1, 1);
  brow_AddFilledRect( nc_msg_warning, 0.2, 0.15, 0.4, 0.4, flow_eDrawType_Yellow);
  brow_AddRect( nc_msg_warning, 0.2, 0.15, 0.4, 0.4, flow_eDrawType_Line, 0, 0);
  brow_AddAnnot( nc_msg_warning, 0.8, 0.6, 0,
		flow_eDrawType_TextHelveticaBold, 2, flow_eAnnotType_OneLine, 
		0);
  brow_AddAnnot( nc_msg_warning, 2, 0.6, 1,
		flow_eDrawType_TextHelveticaBold, 2, flow_eAnnotType_OneLine, 
		0);
  brow_AddAnnot( nc_msg_warning, 8, 0.6, 2,
		flow_eDrawType_TextHelveticaBold, 2, flow_eAnnotType_OneLine, 
		0);
  brow_AddAnnot( nc_msg_warning, 11, 0.6, 3,
		flow_eDrawType_TextHelveticaBold, 2, flow_eAnnotType_OneLine, 
		0);
  brow_AddAnnot( nc_msg_warning, 18, 0.6, 4,
		flow_eDrawType_TextHelveticaBold, 2, flow_eAnnotType_OneLine, 
		0);

  // Nodeclass for Error message
  brow_CreateNodeClass( ctx, "ErrorMsg", 
		flow_eNodeGroup_Common, &nc_msg_error);
  brow_AddFrame( nc_msg_error, 0, 0, 35, 0.8, flow_eDrawType_Line, -1, 1);
  brow_AddFilledRect( nc_msg_error, 0.2, 0.15, 0.4, 0.4, flow_eDrawType_LineRed);
  brow_AddRect( nc_msg_error, 0.2, 0.15, 0.4, 0.4, flow_eDrawType_Line, 0, 0);
  brow_AddAnnot( nc_msg_error, 0.8, 0.6, 0,
		flow_eDrawType_TextHelveticaBold, 2, flow_eAnnotType_OneLine, 
		0);
  brow_AddAnnot( nc_msg_error, 2, 0.6, 1,
		flow_eDrawType_TextHelveticaBold, 2, flow_eAnnotType_OneLine, 
		0);
  brow_AddAnnot( nc_msg_error, 8, 0.6, 2,
		flow_eDrawType_TextHelveticaBold, 2, flow_eAnnotType_OneLine, 
		0);
  brow_AddAnnot( nc_msg_error, 11, 0.6, 3,
		flow_eDrawType_TextHelveticaBold, 2, flow_eAnnotType_OneLine, 
		0);
  brow_AddAnnot( nc_msg_error, 18, 0.6, 4,
		flow_eDrawType_TextHelveticaBold, 2, flow_eAnnotType_OneLine, 
		0);

  // Nodeclass for Fatal messages
  brow_CreateNodeClass( ctx, "FatalMsg", 
		flow_eNodeGroup_Common, &nc_msg_fatal);
  brow_AddFrame( nc_msg_fatal, 0, 0, 35, 0.8, flow_eDrawType_Line, -1, 1);
  brow_AddFilledRect( nc_msg_fatal, 0.2, 0.15, 0.4, 0.4, flow_eDrawType_LineRed);
  brow_AddRect( nc_msg_fatal, 0.2, 0.15, 0.4, 0.4, flow_eDrawType_Line, 4, 0);
  brow_AddAnnot( nc_msg_fatal, 0.8, 0.6, 0,
		flow_eDrawType_TextHelveticaBold, 2, flow_eAnnotType_OneLine, 
		0);
  brow_AddAnnot( nc_msg_fatal, 2, 0.6, 1,
		flow_eDrawType_TextHelveticaBold, 2, flow_eAnnotType_OneLine, 
		0);
  brow_AddAnnot( nc_msg_fatal, 8, 0.6, 2,
		flow_eDrawType_TextHelveticaBold, 2, flow_eAnnotType_OneLine, 
		0);
  brow_AddAnnot( nc_msg_fatal, 11, 0.6, 3,
		flow_eDrawType_TextHelveticaBold, 2, flow_eAnnotType_OneLine, 
		0);
  brow_AddAnnot( nc_msg_fatal, 18, 0.6, 4,
		flow_eDrawType_TextHelveticaBold, 2, flow_eAnnotType_OneLine, 
		0);

  // Nodeclass for Proview restart
  brow_CreateNodeClass( ctx, "Restart", 
		flow_eNodeGroup_Common, &nc_restart);
  brow_AddFrame( nc_restart, 0, 0, 35, 0.8, flow_eDrawType_Line, -1, 1);
  brow_AddFilledRect( nc_restart, 0, 0, 48, 0.8, flow_eDrawType_Yellow);
  brow_AddAnnot( nc_restart, 11, 0.6, 0,
		flow_eDrawType_TextHelveticaBold, 2, flow_eAnnotType_OneLine, 
		0);
  brow_AddAnnot( nc_restart, 18, 0.6, 1,
		flow_eDrawType_TextHelveticaBold, 2, flow_eAnnotType_OneLine, 
		0);

  brow_CreateNodeClass( ctx, "Text", 
		flow_eNodeGroup_Common, &nc_text);
  brow_AddFrame( nc_text, 0, 0, 35, 0.8, flow_eDrawType_Line, -1, 1);
  brow_AddRect( nc_text, 0.2, 0.15, 0.4, 0.4, flow_eDrawType_Line, 0, 0);
  brow_AddAnnot( nc_text, 18, 0.6, 0,
		flow_eDrawType_TextHelveticaBold, 2, flow_eAnnotType_OneLine, 
		0);

}

void CLogNavBrow::brow_setup()
{
  brow_sAttributes brow_attr;
  unsigned long mask;

  mask = 0;
  mask |= brow_eAttr_indentation;
  brow_attr.indentation = 0.5;
  mask |= brow_eAttr_annotation_space;
  brow_attr.annotation_space = 0.5;
  brow_SetAttributes( ctx, &brow_attr, mask); 
  brow_SetCtxUserData( ctx, clognav);

  brow_EnableEvent( ctx, flow_eEvent_MB1Click, flow_eEventType_CallBack, 
	clognav_brow_cb);
  brow_EnableEvent( ctx, flow_eEvent_MB1DoubleClick, flow_eEventType_CallBack, 
	clognav_brow_cb);
  brow_EnableEvent( ctx, flow_eEvent_MB3Press, flow_eEventType_CallBack, 
	clognav_brow_cb);
  brow_EnableEvent( ctx, flow_eEvent_MB3Down, flow_eEventType_CallBack, 
	clognav_brow_cb);
  brow_EnableEvent( ctx, flow_eEvent_SelectClear, flow_eEventType_CallBack, 
	clognav_brow_cb);
  brow_EnableEvent( ctx, flow_eEvent_ObjectDeleted, flow_eEventType_CallBack, 
	clognav_brow_cb);
  brow_EnableEvent( ctx, flow_eEvent_Key_Up, flow_eEventType_CallBack, 
	clognav_brow_cb);
  brow_EnableEvent( ctx, flow_eEvent_Key_Down, flow_eEventType_CallBack, 
	clognav_brow_cb);
  brow_EnableEvent( ctx, flow_eEvent_Key_Right, flow_eEventType_CallBack, 
	clognav_brow_cb);
  brow_EnableEvent( ctx, flow_eEvent_Key_Left, flow_eEventType_CallBack, 
	clognav_brow_cb);
  brow_EnableEvent( ctx, flow_eEvent_Key_PF3, flow_eEventType_CallBack, 
	clognav_brow_cb);
  brow_EnableEvent( ctx, flow_eEvent_Radiobutton, flow_eEventType_CallBack, 
	clognav_brow_cb);
}

//
// Backcall routine called at creation of the brow widget
// Enable event, create nodeclasses and insert the root objects.
//
static int clognav_init_brow_cb( FlowCtx *fctx, void *client_data)
{
  int start_idx = 1;

  CLogNav *clognav = (CLogNav *) client_data;
  BrowCtx *ctx = (BrowCtx *)fctx;

  clognav->brow = new CLogNavBrow( ctx, (void *)clognav);

  clognav->brow->brow_setup();
  clognav->brow->create_nodeclasses();

  clognav->get_files();
  clognav->read( &start_idx, 1);
  return 1;
}

CLogNav::CLogNav(
	void *clog_parent_ctx,
	Widget	clog_parent_wid,
	Widget *w) :
	parent_ctx(clog_parent_ctx), parent_wid(clog_parent_wid),
	clog_size(0), max_size(10000), current_pos_high(0),
	current_pos_low(0)
{
  form_widget = ScrolledBrowCreate( parent_wid, "CLogNav", NULL, 0, 
	clognav_init_brow_cb, this, (Widget *)&brow_widget);
  XtManageChild( form_widget);

  *w = form_widget;
}


//
//  Delete ev
//
CLogNav::~CLogNav()
{
  delete brow;
  XtDestroyWidget( form_widget);
}

CLogNavBrow::~CLogNavBrow()
{
  free_pixmaps();
}

void CLogNav::set_filter( bool success, bool info, bool warning, bool error, bool fatal,
		 bool text, char *str)
{
  filter.show_success = success;
  filter.show_info = info;
  filter.show_warning = warning;
  filter.show_error = error;
  filter.show_fatal = fatal;
  filter.show_text = text;
  strcpy( filter.str, str);

  draw();
}

void CLogNav::get_filter( bool *success, bool *info, bool *warning, bool *error, 
			  bool *fatal, bool *text)
{
  *success = filter.show_success;
  *info = filter.show_info;
  *warning = filter.show_warning;
  *error = filter.show_error;
  *fatal = filter.show_fatal;
  *text = filter.show_text;
}

void CLogNav::get_files()
{
  char nodename[40];
  pwr_tStatus sts;
  char file_spec[200];
  char found_file[200];
  pwr_tTime time = {0,0};
  struct stat info;

  syi_NodeName( &sts, nodename, sizeof(nodename));

  sprintf( file_spec, "$pwrp_log/pwr_%s.log", nodename);
  sts = dcli_search_file( file_spec, found_file, DCLI_DIR_SEARCH_INIT);
  if ( ODD(sts)) {
    stat( found_file, &info);
    time.tv_sec = info.st_ctime;

    CLogFile *cf = new CLogFile( found_file, time);
    file_list.push_back( *cf);
    delete cf;
  }
  else {
    CLogFile *cf = new CLogFile();
    file_list.push_back( *cf);
    delete cf;
  }
  dcli_search_file( file_spec, found_file, DCLI_DIR_SEARCH_END);

  sprintf( file_spec, "$pwrp_log/pwr_%s.log.*.gz", nodename);
  sts = dcli_search_file( file_spec, found_file, DCLI_DIR_SEARCH_INIT);
  while ( ODD(sts)) {

    stat( found_file, &info);
    time.tv_sec = info.st_ctime;

    CLogFile *cf = new CLogFile( found_file, time);
    file_list.push_back( *cf);
    delete cf;

    sts = dcli_search_file( file_spec, found_file, DCLI_DIR_SEARCH_NEXT);
  }
  dcli_search_file( file_spec, found_file, DCLI_DIR_SEARCH_END);

}

int CLogNav::next_file()
{
  if ( current_pos_low <= 1)
    return 0;

  int pos = current_pos_low - 1;
  read( &pos, 1);
  return 1;
}

int CLogNav::prev_file()
{
  if ( current_pos_high >= (int)file_list.size())
    return 0;

  int pos = current_pos_high + 1;
  read( &pos, 1);
  return 1;
}

void CLogNav::read( int *pos_list, int pos_cnt)
{
  char line[400];
  errh_eSeverity severity;
  pwr_tTime time;
  pwr_tTime ztime = {0,0};
  char time_str[40];
  char logger[40];
  int pid;
  int sts;
  FILE *fp;
  char filename[200];
  CLogMsg *msg;
  char tmpfile[200] = "/tmp/clog.tmp";
  int found;

  msg_list.clear();

  current_pos_low = file_list.size();
  current_pos_high = 0;
  for ( int i = file_list.size() - 1; i >=  0; i--) {
    found = 0;
    for ( int j = 0; j < pos_cnt; j++) {
      if ( pos_list[j] -1 == i) {
	found = 1;
	break;
      }
    }
    if ( !found)
      continue;

    if ( i + 1 > current_pos_high)
      current_pos_high = i + 1;
    if ( i + 1 < current_pos_low)
      current_pos_low = i + 1;

    if ( i == 0) {
      strcpy( filename, file_list[0].name);
      if ( strcmp( filename, "") == 0)
	continue;
    }
    else {
      // Unzip the file
      char cmd[300];
      
      sprintf( cmd, "zcat %s > %s", file_list[i].name, tmpfile);
      system( cmd);
      strcpy( filename, tmpfile);
    }

    fp = fopen( filename, "r");
    if ( !fp)
      return;

    while( 1) {
      sts = dcli_read_line( line, sizeof(line), fp);
      if ( !sts)
	break;

      if ( line[1] == ' ' && line[27] == ' ' &&
	   (line[0] == 'S' || line[0] == 'I' || line[0] == 'W' || 
	    line[0] == 'E' || line[0] == 'F')) {
	switch ( line[0]) {
	case 'S': severity = errh_eSeverity_Success; break;
	case 'I': severity = errh_eSeverity_Info; break;
	case 'W': severity = errh_eSeverity_Warning; break;
	case 'E': severity = errh_eSeverity_Error; break;
	case 'F': severity = errh_eSeverity_Fatal; break;
	default: ;
	}
	strncpy( logger, &line[2], 18);
	logger[18] = 0;

	sts = sscanf( &line[22], "%d", &pid);
	if ( sts != 1)
	  pid = 0;
	
	strncpy( &time_str[2], &line[28], 20);
	time_str[0] = '2';
	time_str[1] = '0';
	time_str[22] = 0;
	time_FormAsciiToA( time_str, HUNDRED, GB, &time);

	msg = new CLogMsg( severity, logger, pid, time, &line[49]);
	msg_list.push_back(*msg);
	delete msg;
      }
      else {
	dcli_trim( line, line);
	msg = new CLogMsg( errh_eSeverity_Null, "", 0, ztime, line);
	msg_list.push_back(*msg);
	delete msg;
      }
    }
    fclose( fp);
    if ( i != 0)
      unlink( filename);
  }
  draw();
}

void CLogNav::draw()
{
  ItemMsgBase *item;
  brow_tNode dest = 0;

  brow_SetNodraw( brow->ctx);
  brow_DeleteAll( brow->ctx);

  for ( int i = 0; i < (int)msg_list.size(); i++) {
    
    if ( strncmp( msg_list[i].text, "Setting log file to:", 20) == 0) {
      item = new ItemMsgRestart( this, "Restart", msg_list[i].time, 
				 dest, flow_eDest_After);
      dest = item->node;
    }

    bool insert = false;
    switch ( msg_list[i].severity) {
    case errh_eSeverity_Success:
      if ( filter.show_success)
	insert = true;
      break;
    case errh_eSeverity_Info:
      if ( filter.show_info)
	insert = true;
      break;
    case errh_eSeverity_Warning:
      if ( filter.show_warning)
	insert = true;
      break;
    case errh_eSeverity_Error:
      if ( filter.show_error)
	insert = true;
      break;
    case errh_eSeverity_Fatal:
      if ( filter.show_fatal)
	insert = true;
      break;
    case errh_eSeverity_Null:
      if ( filter.show_text)
	insert = true;
      break;
    }

    if ( insert && strcmp( filter.str, "") != 0) {
      if ( strstr( msg_list[i].logger, filter.str) == 0 &&
	   strstr( msg_list[i].text, filter.str) == 0)
	insert = false;
    }


    if ( insert) {
      item = new ItemMsg( this, "Msg",
			msg_list[i].severity, msg_list[i].logger, 
			msg_list[i].pid,  msg_list[i].time, msg_list[i].text,
			dest, flow_eDest_After);
      dest = item->node;
    }
  }

  brow_ResetNodraw( brow->ctx);
  brow_Redraw( brow->ctx, 0);

}

void CLogNav::set_input_focus()
{
  if ( flow_IsViewable( brow_widget))
    XtCallAcceptFocus( brow_widget, CurrentTime);
}

//
//  Zoom
//
void CLogNav::zoom( double zoom_factor)
{
  brow_Zoom( brow->ctx, zoom_factor);
}

//
//  Return to base zoom factor
//
void CLogNav::unzoom()
{
  brow_UnZoom( brow->ctx);
}

void CLogNav::set_nodraw()
{
  brow_SetNodraw( brow->ctx);
}

void CLogNav::reset_nodraw()
{
  brow_ResetNodraw( brow->ctx);
  brow_Redraw( brow->ctx, 0);
}

//
// Callbacks from brow
//
static int clognav_brow_cb( FlowCtx *ctx, flow_tEvent event)
{
  CLogNav		*clognav;
  ItemMsg 		*item;

  if ( event->event == flow_eEvent_ObjectDeleted)
  {
    brow_GetUserData( event->object.object, (void **)&item);
    delete item;
    return 1;
  }

  brow_GetCtxUserData( (BrowCtx *)ctx, (void **) &clognav);
  switch ( event->event)
  {
    case flow_eEvent_Key_Up:
    {
      brow_tNode	*node_list;
      int		node_count;
      brow_tObject	object;
      int		sts;

      brow_GetSelectedNodes( clognav->brow->ctx, &node_list, &node_count);
      if ( !node_count)
      {
        sts = brow_GetLast( clognav->brow->ctx, &object);
        if ( EVEN(sts)) return 1;
      }
      else
      {
        sts = brow_GetPrevious( clognav->brow->ctx, node_list[0], &object);
        if ( EVEN(sts))
        {
          sts = brow_GetLast( clognav->brow->ctx, &object);
          if ( EVEN(sts))
	  {
            if ( node_count)
	      free( node_list);
            return 1;
 	  }
        }
      }
      brow_SelectClear( clognav->brow->ctx);
      brow_SetInverse( object, 1);
      brow_SelectInsert( clognav->brow->ctx, object);
      if ( !brow_IsVisible( clognav->brow->ctx, object))
        brow_CenterObject( clognav->brow->ctx, object, 0.25);
      if ( node_count)
        free( node_list);
      break;
    }
    case flow_eEvent_Key_Down:
    {
      brow_tNode	*node_list;
      int		node_count;
      brow_tObject	object;
      int		sts;

      brow_GetSelectedNodes( clognav->brow->ctx, &node_list, &node_count);
      if ( !node_count)
      {
        sts = brow_GetFirst( clognav->brow->ctx, &object);
        if ( EVEN(sts)) return 1;
      }
      else
      {
        sts = brow_GetNext( clognav->brow->ctx, node_list[0], &object);
        if ( EVEN(sts))
        {
          sts = brow_GetFirst( clognav->brow->ctx, &object);
          if ( EVEN(sts))
	  {
            if ( node_count)
	      free( node_list);
            return 1;
 	  }
        }
      }
      brow_SelectClear( clognav->brow->ctx);
      brow_SetInverse( object, 1);
      brow_SelectInsert( clognav->brow->ctx, object);
      if ( !brow_IsVisible( clognav->brow->ctx, object))
        brow_CenterObject( clognav->brow->ctx, object, 0.75);
      if ( node_count)
        free( node_list);
      break;
    }
    case flow_eEvent_SelectClear:
      brow_ResetSelectInverse( clognav->brow->ctx);
      break;
    case flow_eEvent_MB1Click:
      // Select
      switch ( event->object.object_type)
      {
        case flow_eObjectType_Node:
          if ( brow_FindSelectedObject( clognav->brow->ctx, event->object.object))
          {
            brow_SelectClear( clognav->brow->ctx);
          }
          else
          {
            brow_SelectClear( clognav->brow->ctx);
            brow_SetInverse( event->object.object, 1);
            brow_SelectInsert( clognav->brow->ctx, event->object.object);
          }
          break;
        default:
          brow_SelectClear( clognav->brow->ctx);
      }
      break;
    case flow_eEvent_MB3Down:
    {
      brow_SetClickSensitivity( clognav->brow->ctx, 
				      flow_mSensitivity_MB3Press);
      break;
    }
    default:
      ;
  }
  return 1;
}

ItemMsg::ItemMsg( CLogNav *item_clognav, char *item_name, errh_eSeverity item_severity,
     	char *item_logger, int item_pid,
	pwr_tTime item_time, char *item_text,
	brow_tNode dest, flow_eDest dest_code):
	ItemMsgBase(item_clognav, item_name, dest), 
	severity(item_severity), pid(item_pid), time(item_time)
{
  char type_str[2];
  char time_str[40];
  char pid_str[40];

  strcpy( logger, item_logger);
  strncpy( text, item_text, sizeof(text));
  text[sizeof(text)-1] = 0;

  switch ( severity) {
  case errh_eSeverity_Success:
    brow_CreateNode( clognav->brow->ctx, item_name, clognav->brow->nc_msg_info,
		dest, dest_code, (void *) this, 1, &node);
    strcpy( type_str, "S");
    break;
  case errh_eSeverity_Info:
    brow_CreateNode( clognav->brow->ctx, item_name, clognav->brow->nc_msg_info,
		dest, dest_code, (void *) this, 1, &node);
    strcpy( type_str, "I");
    break;
  case errh_eSeverity_Warning:
    brow_CreateNode( clognav->brow->ctx, item_name, clognav->brow->nc_msg_warning,
		dest, dest_code, (void *) this, 1, &node);
    strcpy( type_str, "W");
    break;
  case errh_eSeverity_Error:
    brow_CreateNode( clognav->brow->ctx, item_name, clognav->brow->nc_msg_error,
		dest, dest_code, (void *) this, 1, &node);
    strcpy( type_str, "E");
    break;
  case errh_eSeverity_Fatal:
    brow_CreateNode( clognav->brow->ctx, item_name, clognav->brow->nc_msg_fatal,
		dest, dest_code, (void *) this, 1, &node);
    strcpy( type_str, "F");
    break;
  default:
    brow_CreateNode( clognav->brow->ctx, item_name, clognav->brow->nc_text,
		dest, dest_code, (void *) this, 1, &node);
    brow_SetAnnotation( node, 0, text, strlen(text));
    return;
  }
  sprintf( pid_str, "%5d", pid);

  brow_SetAnnotation( node, 0, type_str, strlen(type_str));
  time_AtoAscii( &time, time_eFormat_ComprDateAndTime, time_str, 
	sizeof(time_str));
  brow_SetAnnotation( node, 1, logger, strlen(logger));
  brow_SetAnnotation( node, 2, pid_str, strlen(pid_str));
  brow_SetAnnotation( node, 3, time_str, strlen(time_str));
  brow_SetAnnotation( node, 4, text, strlen(text));

//  brow_SetAnnotPixmap( node, 0, clognav->brow->pixmap_leaf);
}

ItemMsgRestart::ItemMsgRestart( CLogNav *item_clognav, char *item_name, 
				pwr_tTime item_time,
	brow_tNode dest, flow_eDest dest_code):
	ItemMsgBase(item_clognav, item_name, dest), 
	time(item_time)
{
  char time_str[40];

  brow_CreateNode( clognav->brow->ctx, item_name, clognav->brow->nc_restart,
		   dest, dest_code, (void *) this, 1, &node);

  time_AtoAscii( &time, time_eFormat_ComprDateAndTime, time_str, 
	sizeof(time_str));
  brow_SetAnnotation( node, 0, time_str, strlen(time_str));
  brow_SetAnnotation( node, 1, "Proview startup", strlen("Proview startup"));
}








