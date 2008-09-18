/* 
 * Proview   $Id: ge_graph_journal.h,v 1.3 2008-09-18 14:55:45 claes Exp $
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

#ifndef ge_journal_h
#define ge_journal_h

#include <fstream.h>
#include <vector.h>

using namespace std;

class Graph;

class JournalPos {
 public:
  JournalPos() {}
  streampos undo_pos;
  streampos redo_pos;
  streampos end_pos;
};

/* ge_journal.h -- Journal function */

typedef enum {
  journal_eAction_DeleteObject,
  journal_eAction_DeleteSelect,
  journal_eAction_MoveObject,
  journal_eAction_MoveSelect,
  journal_eAction_CreateObject,
  journal_eAction_AntePropertiesSelect,
  journal_eAction_PostPropertiesSelect,
  journal_eAction_AntePropertiesObject,
  journal_eAction_PostPropertiesObject,
  journal_eAction_AnteGroupSelect,
  journal_eAction_PostGroupSelect,
  journal_eAction_UngroupSelect,
  journal_eAction_PopSelect,
  journal_eAction_PushSelect,
  journal_eAction_AntePaste,
  journal_eAction_PostPaste,
  journal_eAction_AnteRename,
  journal_eAction_PostRename,
  journal_eAction_No
  } journal_eAction;

typedef enum {
  journal_eStatus_Empty,
  journal_eStatus_Stored,
  journal_eStatus_Undo,
  journal_eStatus_Redo,
  journal_eStatus_AnteProperties,
  journal_eStatus_AntePaste,
  journal_eStatus_AnteGroup
} journal_eStatus;

  

class GraphJournal {
  public:
    GraphJournal( Graph *g, int *sts);
    ~GraphJournal();

    int store( journal_eAction action, grow_tObject o);
    int undo();
    int redo();
    void check_object_number( grow_tObject o);
    int open( char *name);
    int clear( char *name = 0);
    int restore( char *fname);
    int store_undo_delete_select();
    int store_redo_delete_select();
    int undo_delete_select();
    int redo_delete_select();
    int store_undo_delete_object( grow_tObject o);
    int store_redo_delete_object( grow_tObject o);
    int undo_delete_object();
    int redo_delete_object();
    int store_undo_create_object( grow_tObject o);
    int store_redo_create_object( grow_tObject o);
    int undo_create_object();
    int redo_create_object();
    int store_properties_select();
    int undo_properties_select();
    int store_properties_object( grow_tObject o);
    int undo_properties_object();
    int store_undo_group_select( grow_tObject o);
    int store_redo_group_select();
    int undo_group_select();
    int redo_group_select();
    int store_undo_ungroup_select();
    int store_redo_ungroup_select();
    int undo_ungroup_select();
    int redo_ungroup_select();
    int store_undo_paste();
    int store_redo_paste();
    int undo_paste();
    int redo_paste();
    int store_undo_pop_select();
    int store_redo_pop_select();
    int undo_pop_select();
    int redo_pop_select();
    int store_undo_push_select();
    int store_redo_push_select();
    int undo_push_select();
    int redo_push_select();
    int store_undo_rename( grow_tObject o);
    int store_redo_rename( grow_tObject o);
    int undo_rename();
    int redo_rename();
    static void print( char *fname);
    static char *tag_to_str( int tag);
    static char *action_to_str( int action);

    Graph *graph;
    fstream fp;
    int empty;
    journal_eStatus status;
    vector<JournalPos> poslist;
    vector<grow_tObject> pastelist;
    int current_idx;
    char graphname[80];
    pwr_tFileName filename;
    char rename_name[80];
};


#endif
















