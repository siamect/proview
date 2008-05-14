/* 
 * Proview   $Id: ge_graph_journal.cpp,v 1.2 2008-05-14 06:51:51 claes Exp $
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

#include "glow_std.h"

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>
#include <float.h>
#include <iostream.h>
#include <fstream.h>
#include <vector.h>

#include "rt_load.h"
#include "flow.h"
#include "flow_browctx.h"
#include "flow_browapi.h"
#include "glow.h"
#include "glow_curvectx.h"
#include "glow_growctx.h"
#include "glow_curveapi.h"
#include "glow_msg.h"
#include "rt_gdh.h"
#include "rt_gdh_msg.h"
#include "co_cdh.h"
#include "co_time.h"
#include "co_dcli.h"
#include "co_wow.h"
#include "co_lng.h"

#include "glow_growctx.h"
#include "glow_growapi.h"
#include "ge_graph.h"
#include "ge_dyn.h"
#include "ge_msg.h"
#include "ge_graph_journal.h"

#define journal_cTag_Undo 	80000009
#define journal_cTag_Redo 	80000010
#define journal_cTag_Object	80000011
#define journal_cTag_End 	80000012

static const int debug = 0;

static char *gname( char *name)
{
  static char n[80];
  char *s;

  if ( (s = strrchr( name, '/')))
    strncpy( n, s+1, sizeof(n));
  else
    strncpy( n, name, sizeof(n));
  if ( (s = strchr( n, '.')))
    *s = 0;

  return n;
}


GraphJournal::GraphJournal( Graph *g, int *sts) : graph(g), status(journal_eStatus_Empty), current_idx(0)
{
  strcpy( graphname, "");
  strcpy( filename, "");
  strcpy( rename_name, "");

  // Open default file
  *sts = open( "nameless_$$");
}

GraphJournal::~GraphJournal()
{
  if ( fp) {
    fp.close();
  
    // Remove file
    pwr_tCmd cmd;
    sprintf( cmd, "rm %s", filename);
    system( cmd);
  }
}

int GraphJournal::open( char *name)
{
  pwr_tFileName fname;
  pwr_tFileName new_filename;
  char new_graphname[80];
  pwr_tTime time;
  int sts;
  bool restored = false;
  
  poslist.clear();
  current_idx = 0;
  status = journal_eStatus_Empty;

  if ( fp && strcmp( filename, "") != 0) {
    fp.close();
  
    // Remove file
    pwr_tCmd cmd;
    sprintf( cmd, "rm %s", filename);
    system( cmd);
  }

  strcpy( new_graphname, gname( name));

  sprintf( fname, "$pwrp_tmp/%s.gjl", new_graphname);
  dcli_translate_filename( new_filename, fname);

  if ( strcmp( graphname, new_graphname) != 0) {
    if ( ODD(dcli_file_time( new_filename, &time))) {
      if ( graph->create_modal_dialog_cb) {
	sts = (graph->create_modal_dialog_cb)( graph->parent_ctx, "Restore", 
	      "A journal file from previous session is found.\n\nDo you want to restore the previous session ?", 
					       "  Yes  ",  "   No   ", "Cancel", 0); 
	switch ( sts) {
	case wow_eModalDialogReturn_Button1:
	  restore( new_filename);
	  restored = true;
	  break;
	case wow_eModalDialogReturn_Button2:
	case wow_eModalDialogReturn_Button3:
	case wow_eModalDialogReturn_Deleted:
	  break;
	}
      }
    }
  }
 
  strcpy( graphname, new_graphname);
  strcpy( filename, new_filename);
  if ( !restored) {
    fp.open( filename, ios::in | ios::out | ios::trunc);
    if ( !fp) {
      printf( "Unable to open journal file %s\n", filename);
      return GE__FILEOPEN;
    }
  }
  return GE__SUCCESS;
}

int GraphJournal::clear( char *name)
{
  poslist.clear();
  current_idx = 0;
  status = journal_eStatus_Empty;

  fp.close();

  // Check if new name
  if ( name) {
    char g[80];
    pwr_tFileName fname;
    
    strcpy( g, gname( name));
    if ( strcmp( g, graphname) != 0) {
      // Remove old file
      pwr_tCmd cmd;
      sprintf( cmd, "rm %s", filename);
      system( cmd);

      strcpy( graphname, g);
      sprintf( fname, "$pwrp_tmp/%s.gjl", graphname);
      dcli_translate_filename( filename, fname);
    }
  }

  fp.open( filename, ios::in | ios::out | ios::trunc);
  if ( !fp) {
    printf( "Unable to open journal file %s\n", filename);
    return GE__FILEOPEN;
  }
  return GE__SUCCESS;  
}

int GraphJournal::store( journal_eAction action, grow_tObject o)
{
  static grow_tObject lock_object = 0;

  if ( status == journal_eStatus_AnteProperties &&
       !((action == journal_eAction_PostPropertiesObject ||
	  action == journal_eAction_PostPropertiesSelect)
	 && o == lock_object)) {
    printf( "Journalfile blocked\n");
    return 0;
  }

  switch ( action) {
  case journal_eAction_AntePropertiesSelect:
  case journal_eAction_PostPropertiesSelect:
  case journal_eAction_DeleteSelect:
  case journal_eAction_UngroupSelect:
  case journal_eAction_PopSelect:
  case journal_eAction_PushSelect: {
    grow_tObject 	*sel_list;
    int		sel_count;

    grow_GetSelectList( graph->grow->ctx, &sel_list, &sel_count);
    if ( sel_count == 0)
      return GE__SUCCESS;
    break;
  }
  default: ;
  }

  if ( action == journal_eAction_AntePropertiesSelect ||
       action == journal_eAction_AntePropertiesObject ||
       action == journal_eAction_AnteGroupSelect ||
       action == journal_eAction_AntePaste ||
       action == journal_eAction_AnteRename) {
    switch ( status) {
    case journal_eStatus_Stored:    
      break;
    case journal_eStatus_Redo:
      fp.seekp( poslist[current_idx-1].end_pos);
      break;
    case journal_eStatus_Undo:
      if ( poslist[current_idx].redo_pos < poslist[current_idx].undo_pos)
	fp.seekp( poslist[current_idx].redo_pos);
      else
	fp.seekp( poslist[current_idx].undo_pos);
      break;
    default: ;
    }

    while ( (int)poslist.size() > current_idx) {
      if ( debug)
	printf( "Remove %d\n", poslist.size()-1);
      poslist.pop_back();
    }

    JournalPos up;

    switch ( action) {
    case journal_eAction_AntePropertiesSelect:
      up.undo_pos = fp.tellp();
      fp << journal_cTag_Undo << " " << action << " " << status << " " << current_idx << endl;
      store_properties_select();
      status = journal_eStatus_AnteProperties;
      lock_object = o;
      break;
    case journal_eAction_AntePropertiesObject:
      up.undo_pos = fp.tellp();
      fp << journal_cTag_Undo << " " << action << " " << status << " " << current_idx << endl;
      store_properties_object( o);
      status = journal_eStatus_AnteProperties;
      lock_object = o;
      break;
    case journal_eAction_AnteGroupSelect:
      up.redo_pos = fp.tellp();
      fp << journal_cTag_Redo << " " << action << " " << status << " " << current_idx << endl;
      store_redo_group_select();
      status = journal_eStatus_AnteGroup;
      break;
    case journal_eAction_AntePaste:
      up.undo_pos = fp.tellp();
      fp << journal_cTag_Undo << " " << action << " " << status << " " << current_idx << endl;
      store_undo_paste();
      status = journal_eStatus_AntePaste;
      break;
    case journal_eAction_AnteRename: {
      grow_GetObjectName( o, rename_name);
      break;
    }
    default: ;
    }

    poslist.push_back(up);

    return GE__SUCCESS;
  }
  if ( action == journal_eAction_PostPropertiesSelect ||
       action == journal_eAction_PostPropertiesObject ||
       action == journal_eAction_PostGroupSelect ||
       action == journal_eAction_PostPaste ||
       action == journal_eAction_PostRename) {

    switch ( action) {
    case journal_eAction_PostPropertiesSelect:
      poslist[current_idx].redo_pos = fp.tellp();
      fp << journal_cTag_Redo << " " << action << " " << status << " " << current_idx << endl;
      store_properties_select();
      break;
    case journal_eAction_PostPropertiesObject:
      poslist[current_idx].redo_pos = fp.tellp();
      fp << journal_cTag_Redo << " " << action << " " << status << " " << current_idx << endl;
      store_properties_object( o);
      break;
    case journal_eAction_PostGroupSelect:
      poslist[current_idx].undo_pos = fp.tellp();
      fp << journal_cTag_Undo << " " << action << " " << status << " " << current_idx << endl;
      store_undo_group_select( o);
      break;
    case journal_eAction_PostPaste:
      poslist[current_idx].redo_pos = fp.tellp();
      fp << journal_cTag_Redo << " " << action << " " << status << " " << current_idx << endl;
      store_redo_paste();
      break;
    case journal_eAction_PostRename:
      poslist[current_idx].undo_pos = fp.tellp();
      fp << journal_cTag_Undo << " " << action << " " << status << " " << current_idx << endl;
      store_undo_rename( o);

      poslist[current_idx].redo_pos = fp.tellp();
      fp << journal_cTag_Redo << " " << action << " " << status << " " << current_idx << endl;
      store_redo_rename( o);
      break;

    default: ;
    }
    poslist[current_idx].end_pos = fp.tellp();
  
    status = journal_eStatus_Stored;

    current_idx++;

    if ( debug)
      printf( "Store idx: %3d  list: %3d undo: %10d redo: %10d\n", current_idx-1, poslist.size() - 1, 
	      (int)poslist[poslist.size()-1].undo_pos, (int)poslist[poslist.size()-1].redo_pos);

    return GE__SUCCESS;
  }

  switch ( status) {
  case journal_eStatus_Stored:
    break;
  case journal_eStatus_Redo:
    fp.seekp( poslist[current_idx-1].end_pos);
    break;
  case journal_eStatus_Undo:
    if ( poslist[current_idx].redo_pos < poslist[current_idx].undo_pos)
      fp.seekp( poslist[current_idx].redo_pos);
    else
      fp.seekp( poslist[current_idx].undo_pos);
    break;
  default: ;
  }

  while ( (int)poslist.size() > current_idx) {
    if ( debug)
      printf( "Remov idx: %3d  list: %3d undo: %10d redo: %10d\n", 0, poslist.size() - 1, 
	      (int)poslist[poslist.size()-1].undo_pos, (int)poslist[poslist.size()-1].redo_pos);
    poslist.pop_back();
  }

  JournalPos up;
  up.redo_pos = fp.tellp();
  fp << journal_cTag_Redo << " " << action << " " << status << " " << current_idx << endl;

  status = journal_eStatus_Stored;

  switch ( action) {
  case journal_eAction_DeleteSelect:
    store_redo_delete_select();
    break;
  case journal_eAction_DeleteObject:
    store_redo_delete_object( o);
    break;
  case journal_eAction_CreateObject:
    store_redo_create_object( o);
    break;
  case journal_eAction_UngroupSelect:
    store_redo_ungroup_select();
    break;
  case journal_eAction_PopSelect:
    store_redo_pop_select();
    break;
  case journal_eAction_PushSelect:
    store_redo_push_select();
    break;
  default: ;
  }

  up.undo_pos = fp.tellp();
  fp << journal_cTag_Undo << " " << action << " " << status << " " << current_idx << endl;

  switch ( action) {
  case journal_eAction_DeleteSelect:
    store_undo_delete_select();
    break;
  case journal_eAction_DeleteObject:
    store_undo_delete_object( o);
    break;
  case journal_eAction_CreateObject:
    store_undo_create_object( o);
    break;
  case journal_eAction_UngroupSelect:
    store_undo_ungroup_select();
    break;
  case journal_eAction_PopSelect:
    store_undo_pop_select();
    break;
  case journal_eAction_PushSelect:
    store_undo_push_select();
    break;
  default: ;
  }

  up.end_pos = fp.tellp();
  poslist.push_back(up);
  current_idx++;

  if ( debug)
    printf( "Store idx: %3d  list: %3d undo: %10d redo: %10d\n", current_idx-1, poslist.size() - 1, 
	    (int)poslist[poslist.size()-1].undo_pos, (int)poslist[poslist.size()-1].redo_pos);

  return GE__SUCCESS;
}

int GraphJournal::undo()
{
  int tag;
  int action;
  char line[100];
  int idx;

  if ( current_idx == 0)
    return 0;

  if ( debug)
    printf( "Undo  idx: %3d  list: %3d undo: %10d redo: %10d\n", current_idx-1, poslist.size() - 1, 
	    (int)poslist[current_idx-1].undo_pos, (int)poslist[current_idx-1].redo_pos);

  fp.seekp( poslist[current_idx-1].undo_pos);

  fp.getline( line, sizeof(line));
  sscanf( line, "%d %d %d %d", &tag, &action, (int *)&status, &idx);

  status = journal_eStatus_Undo;
  

  if ( tag != journal_cTag_Undo) {
    cerr << "Journal file disorder, unable to undo" << endl;
    return 0;
  }

  switch ( action) {
  case journal_eAction_DeleteSelect:
    undo_delete_select();
    break;
  case journal_eAction_DeleteObject:
    undo_delete_object();
    break;
  case journal_eAction_CreateObject:
    undo_create_object();
    break;
  case journal_eAction_AntePropertiesSelect:
  case journal_eAction_PostPropertiesSelect:
    undo_properties_select();
    break;
  case journal_eAction_AntePropertiesObject:
  case journal_eAction_PostPropertiesObject:
    undo_properties_object();
    break;
  case journal_eAction_AnteGroupSelect:
  case journal_eAction_PostGroupSelect:
    undo_group_select();
    break;
  case journal_eAction_UngroupSelect:
    undo_ungroup_select();
    break;
  case journal_eAction_AntePaste:
  case journal_eAction_PostPaste:
    undo_paste();
    break;
  case journal_eAction_PopSelect:
    undo_pop_select();
    break;
  case journal_eAction_PushSelect:
    undo_push_select();
    break;
  case journal_eAction_AnteRename:
  case journal_eAction_PostRename:
    undo_rename();
    break;
  default: ;
  }

  current_idx--;
  return GE__SUCCESS;
}

int GraphJournal::redo()
{
  streampos pos;
  streampos ipos;
  int tag;
  int action;
  char line[100];
  int idx;

  if ( current_idx >= (int)poslist.size())
    return 0;

  if ( debug)
    printf( "Redo  idx: %3d  list: %3d undo: %10d redo: %10d\n", current_idx, poslist.size() - 1, 
	    (int)poslist[current_idx].undo_pos, (int)poslist[current_idx].redo_pos);

  fp.seekp( poslist[current_idx].redo_pos);

  fp.getline( line, sizeof(line));
  sscanf( line, "%d %d %d %d", &tag, &action, (int *)&status, &idx);

  status = journal_eStatus_Redo;
  

  if ( tag != journal_cTag_Redo) {
    cerr << "Journal file disorder, unable to undo" << endl;
    return 0;
  }

  switch ( action) {
  case journal_eAction_DeleteSelect:
    redo_delete_select();
    break;
  case journal_eAction_DeleteObject:
    redo_delete_object();
    break;
  case journal_eAction_CreateObject:
    redo_create_object();
    break;
  case journal_eAction_AntePropertiesSelect:
  case journal_eAction_PostPropertiesSelect:
    undo_properties_select();
    break;
  case journal_eAction_AntePropertiesObject:
  case journal_eAction_PostPropertiesObject:
    undo_properties_object();
    break;
  case journal_eAction_AnteGroupSelect:
  case journal_eAction_PostGroupSelect:
    redo_group_select();
    break;
  case journal_eAction_UngroupSelect:
    redo_ungroup_select();
    break;
  case journal_eAction_AntePaste:
  case journal_eAction_PostPaste:
    redo_paste();
    break;
  case journal_eAction_PopSelect:
    redo_pop_select();
    break;
  case journal_eAction_PushSelect:
    redo_push_select();
    break;
  case journal_eAction_AnteRename:
  case journal_eAction_PostRename:
    redo_rename();
    break;
  default: ;
  }

  current_idx++;

  return GE__SUCCESS;
}

int GraphJournal::undo_delete_select()
{
  grow_tObject o;
  char line[100];
  int tag;
  char name_prev[80];
  grow_tObject prev;
  int sts;

  grow_SetNodraw( graph->grow->ctx);

  fp.getline( line, sizeof(line));
  sscanf( line, "%d", &tag);
  while ( tag == journal_cTag_Object) {
    fp.getline( name_prev, sizeof(name_prev));

    grow_ObjectRead( graph->grow->ctx, (ifstream&)fp, &o);  

    if ( strcmp( name_prev, "") == 0) {
      grow_OrderObject( graph->grow->ctx, o, 0, glow_eDest_After);
    }
    else {
      sts = grow_FindObjectByName( graph->grow->ctx, name_prev, &prev);
      if ( ODD(sts)) {
	grow_OrderObject( graph->grow->ctx, o, prev, glow_eDest_Before);
      }
    }

    fp.get();
    fp.getline( line, sizeof(line));
    sscanf( line, "%d", &tag);
  }
  grow_ResetNodraw( graph->grow->ctx);
  grow_Redraw( graph->grow->ctx);

  if ( tag != journal_cTag_End)
    printf( "Journalfile disorder\n");

  return GE__SUCCESS;
}

int GraphJournal::store_undo_delete_select()
{
  grow_tObject 	*sel_list;
  int		sel_count;
  grow_tObject  prev;
  int 		sts;
  char		name[80];

  grow_GetSelectList( graph->grow->ctx, &sel_list, &sel_count);
  for ( int i = sel_count - 1; i >= 0; i--) {
    fp << journal_cTag_Object << endl;

    sts = grow_GetPreviousObject( graph->grow->ctx, sel_list[i], &prev);
    if ( ODD(sts)) {
      grow_GetObjectName( prev, name);
      fp << name << endl;
    }
    else
      fp << endl;

    grow_ObjectSave( sel_list[i], (ofstream&)fp, glow_eSaveMode_Edit);
  }
  fp << journal_cTag_End << endl;
  return GE__SUCCESS;
}

int GraphJournal::store_redo_delete_select()
{
  char name[80];
  grow_tObject 	*sel_list;
  int		sel_count;

  grow_GetSelectList( graph->grow->ctx, &sel_list, &sel_count);
  for ( int i = 0; i < sel_count; i++) {
    grow_GetObjectName( sel_list[i], name);
    
    fp << journal_cTag_Object << endl;
    fp << name << endl;
  }
  fp << journal_cTag_End << endl;

  return GE__SUCCESS;
}

int GraphJournal::redo_delete_select()
{
  char name[80];
  grow_tObject o;
  int sts;
  char line[100];
  int tag;

  grow_SetNodraw( graph->grow->ctx);

  fp.getline( line, sizeof(line));
  sscanf( line, "%d", &tag);
  while ( tag == journal_cTag_Object) {

    fp.getline( name, sizeof(name));

    sts = grow_FindObjectByName( graph->grow->ctx, name, &o);
    if ( ODD(sts))
      grow_DeleteObject( graph->grow->ctx, o);

    fp.getline( line, sizeof(line));
    sscanf( line, "%d", &tag);
  }
  grow_ResetNodraw( graph->grow->ctx);
  grow_Redraw( graph->grow->ctx);

  if ( tag != journal_cTag_End)
    printf( "Journalfile disorder\n");

  return GE__SUCCESS;
}

int GraphJournal::undo_delete_object()
{
  grow_tObject o;

  grow_ObjectRead( graph->grow->ctx, (ifstream&)fp, &o);  
  grow_Redraw( graph->grow->ctx);
  return GE__SUCCESS;
}

int GraphJournal::store_undo_delete_object( grow_tObject o)
{
  grow_ObjectSave( o, (ofstream&)fp, glow_eSaveMode_Edit);
  return GE__SUCCESS;
}

int GraphJournal::store_redo_delete_object( grow_tObject o)
{
  char name[80];

  grow_GetObjectName( o, name);

  fp << name << endl;
  return GE__SUCCESS;
}

int GraphJournal::redo_delete_object()
{
  char name[80];
  grow_tObject o;
  int sts;

  fp.getline( name, sizeof(name));

  sts = grow_FindObjectByName( graph->grow->ctx, name, &o);
  if ( ODD(sts))
    grow_DeleteObject( graph->grow->ctx, o);

  return GE__SUCCESS;
}

int GraphJournal::undo_create_object()
{
  char name[80];
  grow_tObject o;
  int sts;

  fp.getline( name, sizeof(name));

  sts = grow_FindObjectByName( graph->grow->ctx, name, &o);
  if ( ODD(sts))
    grow_DeleteObject( graph->grow->ctx, o);

  return GE__SUCCESS;
}

int GraphJournal::store_undo_create_object( grow_tObject o)
{
  char name[80];

  grow_GetObjectName( o, name);

  fp << name << endl;
  return GE__SUCCESS;
}

int GraphJournal::redo_create_object()
{
  grow_tObject o;

  grow_ObjectRead( graph->grow->ctx, (ifstream&)fp, &o);  
  check_object_number( o);
  grow_Redraw( graph->grow->ctx);
  return GE__SUCCESS;
}

int GraphJournal::store_redo_create_object( grow_tObject o)
{
  grow_ObjectSave( o, (ofstream&)fp, glow_eSaveMode_Edit);
  return GE__SUCCESS;
}

int GraphJournal::undo_properties_select()
{
  grow_tObject o;
  char line[100];
  int tag;
  char name[80];
  int sts;

  fp.getline( line, sizeof(line));
  sscanf( line, "%d", &tag);
  while ( tag == journal_cTag_Object) {

    fp.getline( name, sizeof(name));

    sts = grow_FindObjectByName( graph->grow->ctx, name, &o);
    if ( ODD(sts))
      grow_ObjectOpen( o, (ifstream&)fp);  

    fp.get();
    fp.getline( line, sizeof(line));
    sscanf( line, "%d", &tag);
  }
  if ( tag != journal_cTag_End)
    printf( "Journalfile disorder\n");

  grow_Redraw( graph->grow->ctx);

  return GE__SUCCESS;
}

int GraphJournal::store_properties_select()
{
  grow_tObject 	*sel_list;
  int		sel_count;
  char name[80];

  grow_GetSelectList( graph->grow->ctx, &sel_list, &sel_count);
  for ( int i = 0; i < sel_count; i++) {
    grow_GetObjectName( sel_list[i], name);

    fp << journal_cTag_Object << endl;
    fp << name << endl;
    grow_ObjectSave( sel_list[i], (ofstream&)fp, glow_eSaveMode_Edit);
  }
  fp << journal_cTag_End << endl;
  return GE__SUCCESS;
}

int GraphJournal::undo_properties_object()
{
  grow_tObject o;
  char line[100];
  int tag;
  char name[80];
  int sts;

  fp.getline( line, sizeof(line));
  sscanf( line, "%d", &tag);
  if ( tag != journal_cTag_Object)
    return 0;

  fp.getline( name, sizeof(name));

  sts = grow_FindObjectByName( graph->grow->ctx, name, &o);
  if ( ODD(sts))
    grow_ObjectOpen( o, (ifstream&)fp);  

  fp.get();
  fp.getline( line, sizeof(line));
  sscanf( line, "%d", &tag);

  if ( tag != journal_cTag_End)
    printf( "Journalfile disorder\n");

  grow_Redraw( graph->grow->ctx);

  return GE__SUCCESS;
}

int GraphJournal::store_properties_object( grow_tObject o)
{
  char name[80];

  grow_GetObjectName( o, name);

  fp << journal_cTag_Object << endl;
  fp << name << endl;
  grow_ObjectSave( o, (ofstream&)fp, glow_eSaveMode_Edit);
  fp << journal_cTag_End << endl;
  return GE__SUCCESS;
}

int GraphJournal::undo_group_select()
{
  grow_tObject o;
  char line[100];
  char name[80];
  int tag;
  int sts;

  fp.getline( line, sizeof(line));
  sscanf( line, "%d", &tag);
  if ( tag != journal_cTag_Object)
    return 0;

  fp.getline( name, sizeof(name));

  sts = grow_FindObjectByName( graph->grow->ctx, name, &o);
  if ( ODD(sts))
    grow_UngroupGroup( graph->grow->ctx, o);

  fp.getline( line, sizeof(line));
  sscanf( line, "%d", &tag);

  if ( tag != journal_cTag_End)
    printf( "Journalfile disorder\n");

  return GE__SUCCESS;
}

int GraphJournal::store_undo_group_select( grow_tObject o)
{
  char name[80];

  grow_GetObjectName( o, name);

  fp << journal_cTag_Object << endl;
  fp << name << endl;
  fp << journal_cTag_End << endl;
  return GE__SUCCESS;
}

int GraphJournal::store_redo_group_select()
{
  char name[80];
  grow_tObject 	*sel_list;
  int		sel_count;

  grow_GetSelectList( graph->grow->ctx, &sel_list, &sel_count);
  for ( int i = 0; i < sel_count; i++) {
    grow_GetObjectName( sel_list[i], name);
    
    fp << journal_cTag_Object << endl;
    fp << name << endl;
  }
  fp << journal_cTag_End << endl;

  return GE__SUCCESS;
}

int GraphJournal::redo_group_select()
{
  char name[80];
  grow_tObject o;
  int sts;
  char line[100];
  int tag;
  char group_name[80];
  char last_group_name[80];
  grow_tObject group;
  GeDyn *data;

  grow_SelectClear( graph->grow->ctx);

  fp.getline( line, sizeof(line));
  sscanf( line, "%d", &tag);
  while ( tag == journal_cTag_Object) {

    fp.getline( name, sizeof(name));

    sts = grow_FindObjectByName( graph->grow->ctx, name, &o);
    if ( ODD(sts))
      grow_SelectInsert( graph->grow->ctx, o);

    fp.getline( line, sizeof(line));
    sscanf( line, "%d", &tag);
  }
  if ( tag != journal_cTag_End) {
    printf( "Journalfile disorder\n");
    return 0;
  }

  // Read group name
  fp.getline( line, sizeof(line));
  fp.getline( line, sizeof(line));
  sscanf( line, "%d", &tag);
  if ( tag != journal_cTag_Object) {
    printf( "Journalfile disorder\n");
    return 0;
  }
  fp.getline( line, sizeof(line));
  strcpy( group_name, line);

  sts = grow_GroupSelect( graph->grow->ctx, &group, last_group_name);
  grow_SelectClear( graph->grow->ctx);

  // Rename to previous name
  sts = grow_FindObjectByName( graph->grow->ctx, group_name, &o);
  if ( EVEN(sts))
    grow_SetObjectName( group, group_name);
 
  if ( strcmp( last_group_name, "") != 0) {  
    // Try to recover dynamics
    sts = graph->recall.get( &data, last_group_name);
    if ( ODD(sts)) {
      graph->set_recall_data( group, last_group_name);
      return GE__SUCCESS;
    }
  }
  GeDyn *dyn = new GeDyn( graph);
  grow_SetUserData( group, (void *)dyn);

  return GE__SUCCESS;
}

int GraphJournal::undo_ungroup_select()
{
  char name[80];
  grow_tObject o;
  int sts;
  char line[100];
  int tag;
  char group_name[80];
  char last_group_name[80];
  grow_tObject group;
  GeDyn *data;

  for (;;) {
    grow_SelectClear( graph->grow->ctx);

    fp.getline( line, sizeof(line));
    sscanf( line, "%d", &tag);

    if ( tag != journal_cTag_Object)
      break;
    fp.getline( group_name, sizeof(group_name));

    fp.getline( line, sizeof(line));
    sscanf( line, "%d", &tag);

    while ( tag == journal_cTag_Object) {
      
      fp.getline( name, sizeof(name));

      sts = grow_FindObjectByName( graph->grow->ctx, name, &o);
      if ( ODD(sts))
	grow_SelectInsert( graph->grow->ctx, o);
      
      fp.getline( line, sizeof(line));
      sscanf( line, "%d", &tag);
    }
    if ( tag != journal_cTag_End) {
      printf( "Journalfile disorder\n");
      return 0;
    }

    sts = grow_GroupSelect( graph->grow->ctx, &group, last_group_name);
    grow_SelectClear( graph->grow->ctx);

    // Rename to previous name
    sts = grow_FindObjectByName( graph->grow->ctx, group_name, &o);
    if ( EVEN(sts))
      grow_SetObjectName( group, group_name);
 
    if ( strcmp( last_group_name, "") != 0) {  
      // Try to recover dynamics
      sts = graph->recall.get( &data, last_group_name);
      if ( ODD(sts)) {
	graph->set_recall_data( group, last_group_name);
	return GE__SUCCESS;
      }
    }
    GeDyn *dyn = new GeDyn( graph);
    grow_SetUserData( group, (void *)dyn);
  }

  return GE__SUCCESS;
}

int GraphJournal::store_undo_ungroup_select()
{
  char name[80];
  grow_tObject 	*sel_list;
  int		sel_count;
  grow_tObject  *member_list;
  int		member_count;

  grow_GetSelectList( graph->grow->ctx, &sel_list, &sel_count);
  for ( int i = 0; i < sel_count; i++) {
    if ( grow_GetObjectType( sel_list[i]) == glow_eObjectType_GrowGroup) {
      grow_GetObjectName( sel_list[i], name);
    
      fp << journal_cTag_Object << endl;
      fp << name << endl;

      grow_GetGroupObjectList( sel_list[i], &member_list, &member_count);
      for ( int j = 0; j < member_count; j++) {
	grow_GetObjectName( member_list[j], name);
	
	fp << journal_cTag_Object << endl;
	fp << name << endl;
      }
      fp << journal_cTag_End << endl;
    }
  }
  fp << journal_cTag_End << endl;

  return GE__SUCCESS;
}

int GraphJournal::store_redo_ungroup_select()
{
  char name[80];
  grow_tObject 	*sel_list;
  int		sel_count;

  grow_GetSelectList( graph->grow->ctx, &sel_list, &sel_count);
  for ( int i = 0; i < sel_count; i++) {
    if ( grow_GetObjectType( sel_list[i]) == glow_eObjectType_GrowGroup) {
      grow_GetObjectName( sel_list[i], name);
    
      fp << journal_cTag_Object << endl;
      fp << name << endl;
    }
  }
  fp << journal_cTag_End << endl;

  return GE__SUCCESS;
}

int GraphJournal::redo_ungroup_select()
{
  grow_tObject o;
  char line[100];
  char name[80];
  int tag;
  int sts;

  fp.getline( line, sizeof(line));
  sscanf( line, "%d", &tag);
  while ( tag == journal_cTag_Object) {

    fp.getline( name, sizeof(name));

    sts = grow_FindObjectByName( graph->grow->ctx, name, &o);
    if ( ODD(sts))
      grow_UngroupGroup( graph->grow->ctx, o);

    fp.getline( line, sizeof(line));
    sscanf( line, "%d", &tag);
  }
  if ( tag != journal_cTag_End)
    printf( "Journalfile disorder\n");

  return GE__SUCCESS;
}

int GraphJournal::undo_paste()
{
  char name[80];
  grow_tObject o;
  int sts;
  char line[100];
  int tag;

  grow_SetNodraw( graph->grow->ctx);
  fp.getline( line, sizeof(line));
  sscanf( line, "%d", &tag);
  while ( tag == journal_cTag_Object) {

    fp.getline( name, sizeof(name));

    sts = grow_FindObjectByName( graph->grow->ctx, name, &o);
    if ( ODD(sts))
      grow_DeleteObject( graph->grow->ctx, o);

    fp.getline( line, sizeof(line));
    sscanf( line, "%d", &tag);
  }
  grow_ResetNodraw( graph->grow->ctx);
  grow_Redraw( graph->grow->ctx);

  if ( tag != journal_cTag_End)
    printf( "Journalfile disorder\n");

  return GE__SUCCESS;
}

int GraphJournal::store_undo_paste()
{
  char 		name[80];
  grow_tObject 	*move_list;
  int		move_count;

  grow_GetMoveList( graph->grow->ctx, &move_list, &move_count);
  for ( int i = 0; i < move_count; i++) {
    pastelist.push_back(move_list[i]);
    grow_GetObjectName( move_list[i], name);
    
    fp << journal_cTag_Object << endl;
    fp << name << endl;
  }
  fp << journal_cTag_End << endl;

  return GE__SUCCESS;
}

int GraphJournal::redo_paste()
{
  grow_tObject o;
  char line[100];
  int tag;

  fp.getline( line, sizeof(line));
  sscanf( line, "%d", &tag);
  while ( tag == journal_cTag_Object) {

    grow_ObjectRead( graph->grow->ctx, (ifstream&)fp, &o);  

    fp.get();
    fp.getline( line, sizeof(line));
    sscanf( line, "%d", &tag);
  }
  if ( tag != journal_cTag_End)
    printf( "Journalfile disorder\n");

  grow_Redraw( graph->grow->ctx);

  return GE__SUCCESS;
}

int GraphJournal::store_redo_paste()
{
  grow_SetNodraw( graph->grow->ctx);
  for ( int i = 0; i < (int)pastelist.size(); i++) {
    fp << journal_cTag_Object << endl;
    grow_ObjectSave( pastelist[i], (ofstream&)fp, glow_eSaveMode_Edit);
  }
  fp << journal_cTag_End << endl;
  grow_ResetNodraw( graph->grow->ctx);
  grow_Redraw( graph->grow->ctx);

  pastelist.clear();
  return GE__SUCCESS;
}

int GraphJournal::undo_pop_select()
{
  grow_tObject o;
  grow_tObject next;
  char line[100];
  char name[80];
  char name_next[80];
  int tag;
  int sts;

  grow_SetNodraw( graph->grow->ctx);

  fp.getline( line, sizeof(line));
  sscanf( line, "%d", &tag);
  while ( tag == journal_cTag_Object) {
    fp.getline( name, sizeof(name));
    fp.getline( name_next, sizeof(name_next));

    sts = grow_FindObjectByName( graph->grow->ctx, name, &o);
    if ( ODD(sts)) {
      if ( strcmp( name_next, "") == 0) {
	grow_OrderObject( graph->grow->ctx, o, 0, glow_eDest_After);
      }
      else {
	sts = grow_FindObjectByName( graph->grow->ctx, name_next, &next);
	if ( ODD(sts)) {
	  grow_OrderObject( graph->grow->ctx, o, next, glow_eDest_Before);
	}
      }
    }
    fp.getline( line, sizeof(line));
    sscanf( line, "%d", &tag);
  }
  grow_ResetNodraw( graph->grow->ctx);
  grow_Redraw( graph->grow->ctx);

  if ( tag != journal_cTag_End)
    printf( "Journalfile disorder\n");

  return GE__SUCCESS;
}

int GraphJournal::store_undo_pop_select()
{
  grow_tObject 	*sel_list;
  int		sel_count;
  char		name[80];
  grow_tObject  next;
  int 		sts;

  grow_GetSelectList( graph->grow->ctx, &sel_list, &sel_count);
  for ( int i = sel_count - 1; i >= 0; i--) {
    fp << journal_cTag_Object << endl;
    grow_GetObjectName( sel_list[i], name);

    fp << name << endl;

    sts = grow_GetNextObject( graph->grow->ctx, sel_list[i], &next);
    if ( ODD(sts)) {
      grow_GetObjectName( next, name);
      fp << name << endl;
    }
    else
      fp << endl;
  }
  fp << journal_cTag_End << endl;
  return GE__SUCCESS;
}

int GraphJournal::store_redo_pop_select()
{
  char name[80];
  grow_tObject 	*sel_list;
  int		sel_count;

  grow_GetSelectList( graph->grow->ctx, &sel_list, &sel_count);
  for ( int i = 0; i < sel_count; i++) {
    grow_GetObjectName( sel_list[i], name);
    
    fp << journal_cTag_Object << endl;
    fp << name << endl;
  }
  fp << journal_cTag_End << endl;
  return GE__SUCCESS;
}

int GraphJournal::redo_pop_select()
{
  char name[80];
  grow_tObject o;
  int sts;
  char line[100];
  int tag;

  grow_SetNodraw( graph->grow->ctx);
  grow_SelectClear( graph->grow->ctx);

  fp.getline( line, sizeof(line));
  sscanf( line, "%d", &tag);
  while ( tag == journal_cTag_Object) {

    fp.getline( name, sizeof(name));

    sts = grow_FindObjectByName( graph->grow->ctx, name, &o);
    if ( ODD(sts))
      grow_SelectInsert( graph->grow->ctx, o);
    
    fp.getline( line, sizeof(line));
    sscanf( line, "%d", &tag);
  }
  grow_PopSelectedObjects( graph->grow->ctx);
  grow_SelectClear( graph->grow->ctx);
  grow_ResetNodraw( graph->grow->ctx);
  grow_Redraw( graph->grow->ctx);

  if ( tag != journal_cTag_End)
    printf( "Journalfile disorder\n");

  return GE__SUCCESS;
}

int GraphJournal::undo_push_select()
{
  grow_tObject o;
  grow_tObject next;
  char line[100];
  char name[80];
  char name_next[80];
  int tag;
  int sts;

  grow_SetNodraw( graph->grow->ctx);

  fp.getline( line, sizeof(line));
  sscanf( line, "%d", &tag);
  while ( tag == journal_cTag_Object) {
    fp.getline( name, sizeof(name));
    fp.getline( name_next, sizeof(name_next));

    sts = grow_FindObjectByName( graph->grow->ctx, name, &o);
    if ( ODD(sts)) {
      if ( strcmp( name_next, "") == 0) {
	grow_OrderObject( graph->grow->ctx, o, 0, glow_eDest_After);
      }
      else {
	sts = grow_FindObjectByName( graph->grow->ctx, name_next, &next);
	if ( ODD(sts)) {
	  grow_OrderObject( graph->grow->ctx, o, next, glow_eDest_Before);
	}
      }
    }
    fp.getline( line, sizeof(line));
    sscanf( line, "%d", &tag);
  }
  grow_ResetNodraw( graph->grow->ctx);
  grow_Redraw( graph->grow->ctx);

  if ( tag != journal_cTag_End)
    printf( "Journalfile disorder\n");

  return GE__SUCCESS;
}

int GraphJournal::store_undo_push_select()
{
  grow_tObject 	*sel_list;
  int		sel_count;
  char		name[80];
  grow_tObject  next;
  int 		sts;

  grow_GetSelectList( graph->grow->ctx, &sel_list, &sel_count);
  for ( int i = sel_count - 1; i >= 0; i--) {
    fp << journal_cTag_Object << endl;
    grow_GetObjectName( sel_list[i], name);

    fp << name << endl;

    sts = grow_GetNextObject( graph->grow->ctx, sel_list[i], &next);
    if ( ODD(sts)) {
      grow_GetObjectName( next, name);
      fp << name << endl;
    }
    else
      fp << endl;
  }
  fp << journal_cTag_End << endl;
  return GE__SUCCESS;
}

int GraphJournal::store_redo_push_select()
{
  char name[80];
  grow_tObject 	*sel_list;
  int		sel_count;

  grow_GetSelectList( graph->grow->ctx, &sel_list, &sel_count);
  for ( int i = 0; i < sel_count; i++) {
    grow_GetObjectName( sel_list[i], name);
    
    fp << journal_cTag_Object << endl;
    fp << name << endl;
  }
  fp << journal_cTag_End << endl;
  return GE__SUCCESS;
}

int GraphJournal::redo_push_select()
{
  char name[80];
  grow_tObject o;
  int sts;
  char line[100];
  int tag;

  grow_SetNodraw( graph->grow->ctx);
  grow_SelectClear( graph->grow->ctx);

  fp.getline( line, sizeof(line));
  sscanf( line, "%d", &tag);
  while ( tag == journal_cTag_Object) {

    fp.getline( name, sizeof(name));

    sts = grow_FindObjectByName( graph->grow->ctx, name, &o);
    if ( ODD(sts))
      grow_SelectInsert( graph->grow->ctx, o);
    
    fp.getline( line, sizeof(line));
    sscanf( line, "%d", &tag);
  }
  grow_PushSelectedObjects( graph->grow->ctx);
  grow_SelectClear( graph->grow->ctx);
  grow_ResetNodraw( graph->grow->ctx);
  grow_Redraw( graph->grow->ctx);

  if ( tag != journal_cTag_End)
    printf( "Journalfile disorder\n");

  return GE__SUCCESS;
}

int GraphJournal::undo_rename()
{
  grow_tObject o;
  char line[100];
  char name_old[80];
  char name_new[80];
  int tag;
  int sts;

  fp.getline( name_new, sizeof(name_new));
  fp.getline( name_old, sizeof(name_old));

  sts = grow_FindObjectByName( graph->grow->ctx, name_new, &o);
  if ( ODD(sts))
    grow_SetObjectName( o, name_old);

  fp.getline( line, sizeof(line));
  sscanf( line, "%d", &tag);

  if ( tag != journal_cTag_End)
    printf( "Journalfile disorder\n");

  return GE__SUCCESS;
}

int GraphJournal::store_undo_rename( grow_tObject o)
{
  char		name[80];

  grow_GetObjectName( o, name);
  
  fp << name << endl;
  fp << rename_name << endl;
  fp << journal_cTag_End << endl;

  return GE__SUCCESS;
}

int GraphJournal::store_redo_rename( grow_tObject o)
{
  char		name[80];

  grow_GetObjectName( o, name);
  
  fp << name << endl;
  fp << rename_name << endl;
  fp << journal_cTag_End << endl;

  return GE__SUCCESS;
}

int GraphJournal::redo_rename()
{
  grow_tObject o;
  char line[100];
  char name_old[80];
  char name_new[80];
  int tag;
  int sts;

  fp.getline( name_new, sizeof(name_new));
  fp.getline( name_old, sizeof(name_old));

  sts = grow_FindObjectByName( graph->grow->ctx, name_old, &o);
  if ( ODD(sts))
    grow_SetObjectName( o, name_new);

  fp.getline( line, sizeof(line));
  sscanf( line, "%d", &tag);

  if ( tag != journal_cTag_End)
    printf( "Journalfile disorder\n");

  return GE__SUCCESS;
}

void GraphJournal::check_object_number( grow_tObject o)
{
  char name[80];
  char c;
  int num;
  int nr;

  grow_GetObjectName( o, name);
  nr = sscanf( name, "%c%d", &c, &num);
  if ( nr == 2 &&
       (c == 'C' || c == 'O')) {
    if ( num >= grow_GetNextObjectNameNumber( graph->grow->ctx))
      grow_SetNextObjectNameNumber( graph->grow->ctx, num+1);
  }
}

int GraphJournal::restore( char *fname)
{
  char line[200];
  int nr = 0;
  int tag;
  int action;
  int idx;
  int end_found = 0;
  streampos pos;
  JournalPos up;

  poslist.clear();
  current_idx = -1;
  status = journal_eStatus_Empty;

  fp.open( fname, ios::in | ios::out);
  if ( !fp) {
    printf( "Unable to open journal file %s\n", filename);
    return GE__FILEOPEN;
  }

  while ( 1) {
    while ( !(nr == 1 && tag == journal_cTag_Redo)) {
      pos = fp.tellp();
      if ( !fp.getline( line, sizeof(line))) {
	end_found = 1;
	break;
      }
      nr = sscanf( line, "%d", &tag);
      if ( nr == 1 && tag == journal_cTag_Redo) {
	sscanf( line, "%d %d %d %d", &tag, &action, (int *)&status, &idx);
	if ( current_idx < idx) {
	  up.redo_pos = pos;
	  up.undo_pos = 0;
	  up.end_pos = 0;
	  current_idx = idx;

	  if ( idx > 0)
	    poslist[idx-1].end_pos = pos;
	    
	  poslist.push_back( up);
	}
	else if ( idx == current_idx)
	  poslist[idx].redo_pos = pos;
      }
      if ( nr == 1 && tag == journal_cTag_Undo) {
	sscanf( line, "%d %d %d %d", &tag, &action, (int *)&status, &idx);
	if ( current_idx < idx) {
	  up.redo_pos = 0;
	  up.undo_pos = pos;
	  up.end_pos = 0;
	  current_idx = idx;

	  if ( idx > 0)
	    poslist[idx-1].end_pos = pos;
	    
	  poslist.push_back( up);
	}
	else if ( idx == current_idx)
	  poslist[idx].undo_pos = pos;
      }
    }

    if ( end_found)
      break;

    sscanf( line, "%d %d %d %d", &tag, &action, (int *)&status, &idx);

    switch ( action) {
    case journal_eAction_DeleteSelect:
      redo_delete_select();
      break;
    case journal_eAction_DeleteObject:
      redo_delete_object();
      break;
    case journal_eAction_CreateObject:
      redo_create_object();
      break;
    case journal_eAction_AntePropertiesSelect:
    case journal_eAction_PostPropertiesSelect:
      undo_properties_select();
      break;
    case journal_eAction_AntePropertiesObject:
    case journal_eAction_PostPropertiesObject:
      undo_properties_object();
      break;
    case journal_eAction_AnteGroupSelect:
    case journal_eAction_PostGroupSelect:
      redo_group_select();
      break;
    case journal_eAction_UngroupSelect:
      redo_ungroup_select();
      break;
    case journal_eAction_AntePaste:
    case journal_eAction_PostPaste:
      redo_paste();
      break;
    case journal_eAction_PopSelect:
      redo_pop_select();
      break;
    case journal_eAction_PushSelect:
      redo_push_select();
      break;
    case journal_eAction_AnteRename:
    case journal_eAction_PostRename:
      redo_rename();
      break;
    default: ;
    }
    nr = 0;
  }
  fp.clear();
  if ( current_idx >= 0)
    poslist[current_idx].end_pos = pos;
  current_idx++;
  status = journal_eStatus_Stored;

  return GE__SUCCESS;
}





