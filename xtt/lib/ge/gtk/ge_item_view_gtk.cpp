#include "glow_std.h"

#include <cstdio>
#include <cstring>
#include <vector>

#include "co_dcli.h"

#include "flow.h"
#include "flow_browctx.h"
#include "flow_browapi.h"
#include "flow_browwidget_gtk.h"
#include "glow.h"
#include "glow_colpalctx.h"
#include "glow_colpalapi.h"
#include "glow_colpalwidget_gtk.h"
#include "glow_msg.h"

#include "glow_growctx.h"
#include "glow_growapi.h"
#include "glow_growwidget_gtk.h"
#include "ge_graph_gtk.h"
#include "ge_gtk.h"
#include "ge_subpalette_gtk.h"
#include "ge_subgraphs_gtk.h"
#include "ge_util.h"
#include "ge_msg.h"
#include "ge_item_view_gtk.h"
#include "wb_wnav_selformat.h"
#include "cow_wow_gtk.h"
#include "cow_logw_gtk.h"
#include "wb_nav_gtk.h"
#include "wb_log.h"

#include "ge_item_view_gtk.h"

static void *graph_list_files();
static gpointer graph_list_store( char *);
static void directory_changed( GFileMonitor *, GFile *, GFile *, GFileMonitorEvent, gpointer);
static gboolean button_press_tree_widget( GtkWidget *, GdkEvent *, gpointer);
static void select_tree_item_pos( GtkWidget *, gint, gint);
static void autosave_toggled( GtkToggleButton *, gpointer);


GeItemViewGtk::GeItemViewGtk( gpointer gectx):
  ge_ctx(gectx),
  toplevel_widget(0),
  tree_widget(0)
{
  GtkCellRenderer *text_renderer;
  GtkTreeViewColumn *name_column;
  GtkWidget *scrolled_widget;
  GFile *dir;
  GFileMonitor *monitor;
  char full_path[256];

  tree_widget = (GtkWidget *)
    g_object_new( GTK_TYPE_TREE_VIEW,
		  "rules-hint", TRUE,
		  "headers-visible", FALSE,
		  "reorderable", TRUE,
		  "enable-search", TRUE,
		  "search-column", 0,
		  NULL);

  g_signal_connect( tree_widget, "row-activated",
		    G_CALLBACK(activate_tree_widget), this);

  g_signal_connect( tree_widget, "button-press-event",
		    G_CALLBACK(button_press_tree_widget), this);

  g_signal_connect( tree_widget, "focus-out-event",
		    G_CALLBACK(focus_out_tree_widget), this);

  text_renderer = gtk_cell_renderer_text_new();
  name_column =
    gtk_tree_view_column_new_with_attributes( "", text_renderer, "text", 0, NULL);
  g_object_set( name_column, "resizable", FALSE, "clickable", TRUE, NULL);
  gtk_tree_view_append_column( GTK_TREE_VIEW(tree_widget), name_column);

  autosave_button = gtk_check_button_new_with_label( "Autosave");
  autosave_toggled( GTK_TOGGLE_BUTTON(autosave_button), (gpointer) TRUE);
  g_signal_connect( autosave_button, "toggled",
		    G_CALLBACK(autosave_toggled), (gpointer) FALSE);

  scrolled_widget = gtk_scrolled_window_new( NULL, NULL);
  gtk_container_add( GTK_CONTAINER(scrolled_widget), tree_widget);

  toplevel_widget = gtk_vbox_new( FALSE, 0);
  gtk_box_pack_start( GTK_BOX(toplevel_widget), autosave_button, FALSE, FALSE, 8);
  gtk_box_pack_start( GTK_BOX(toplevel_widget), scrolled_widget, TRUE, TRUE, 0);

  update();

  dcli_translate_filename( full_path, "$pwrp_pop");
  dir = g_file_new_for_path( full_path);
  monitor = g_file_monitor_directory( dir, G_FILE_MONITOR_NONE, NULL, NULL);
  g_signal_connect( monitor, "changed",
		    G_CALLBACK( directory_changed), this);
}

GeItemViewGtk::~GeItemViewGtk()
{
  gtk_widget_destroy( toplevel_widget);
}

GtkWidget *GeItemViewGtk::widget() const
{
  return toplevel_widget;
}

void GeItemViewGtk::update()
{
  GtkListStore *store;
  char *texts;

  texts = (char *) graph_list_files();
  if ( !texts)
    return;

  store = (GtkListStore *) graph_list_store( texts);
  gtk_tree_view_set_model( GTK_TREE_VIEW(tree_widget), GTK_TREE_MODEL(store));

  free( texts);
}

void GeItemViewGtk::update( char *full_name, int event)
{
  GtkTreeModel *store;
  GtkTreeIter iter;
  char *nameutf8;
  char *name_p;
  char *value;
  gboolean rv;

  name_p = strrchr( full_name, '/');
  full_name = strstr( name_p, ".pwg");
  if ( !full_name)
    return;

  name_p[full_name - name_p] = 0;
  name_p++; // skip slash
  nameutf8 = g_convert( name_p, -1, "UTF-8", "ISO8859-1", NULL, NULL, NULL);

  store = gtk_tree_view_get_model( GTK_TREE_VIEW(tree_widget));
  rv = gtk_tree_model_get_iter_first( store, &iter);

  if ( event == G_FILE_MONITOR_EVENT_CREATED) {
    GtkTreeIter sibl;

    while ( rv) {
      gtk_tree_model_get( store, &iter, 0, &value, -1);
      if ( strcmp( name_p, value) < 0)
	break;
      rv = gtk_tree_model_iter_next( store, &iter);
    }

    sibl = iter;
    if ( rv)
      gtk_list_store_insert_before( GTK_LIST_STORE(store), &iter, &sibl);
    else
      gtk_list_store_append( GTK_LIST_STORE(store), &iter);

    gtk_list_store_set( GTK_LIST_STORE(store), &iter, 0, nameutf8, -1);
  }
  else if ( event == G_FILE_MONITOR_EVENT_DELETED) {
    while ( rv) {
      gtk_tree_model_get( store, &iter, 0, &value, -1);
      if ( strcmp( name_p, value) == 0) {
	gtk_list_store_remove( GTK_LIST_STORE(store), &iter);
	break;
      }
      rv = gtk_tree_model_iter_next( store, &iter);
    }
  }

  g_free( nameutf8);
}

char *GeItemViewGtk::selected_text( GtkWidget *tree_widget)
{
  char *sel_text;
  GtkTreeIter iter;
  GtkTreeModel *store;
  GtkTreeSelection *selection;
  gboolean selected;

  sel_text = NULL;
  store = gtk_tree_view_get_model( GTK_TREE_VIEW(tree_widget));
  selection = gtk_tree_view_get_selection( GTK_TREE_VIEW(tree_widget));
  selected = gtk_tree_selection_get_selected( selection, NULL, &iter);
  if ( selected)
    gtk_tree_model_get( GTK_TREE_MODEL(store), &iter, 0, &sel_text, -1);

  return sel_text;
}

char *GeItemViewGtk::selected_text() const
{
  return selected_text( tree_widget);
}

void GeItemViewGtk::activate_menu_open( GtkWidget *w, gpointer data)
{
  GeItemViewGtk *item_view;
  GeGtk *ge;

  item_view = (GeItemViewGtk *) data;
  ge = (GeGtk *) item_view->ge_ctx;
  if ( ge->graph->is_modified()) {
    int rv;
    char title[] = "Save changes";
    char message[64];

    sprintf( message, "Your changes will be lost. Do you want to save?");
    rv = ge->create_modal_dialog( title, message, "Yes", "No", NULL, NULL);
    if ( rv == wow_eModalDialogReturn_Button1) {
      GeGtk::activate_save( NULL, item_view->ge_ctx);
    }
  }

  activate_tree_widget( GTK_TREE_VIEW(item_view->tree_widget), NULL, NULL, ge);
}

void GeItemViewGtk::activate_menu_delete( GtkWidget *w, gpointer data)
{
  char *sel_text;
  GeItemViewGtk *item_view;

  item_view = (GeItemViewGtk *) data;
  sel_text = item_view->selected_text();
  if ( sel_text) {
    int rv;
    char title[] = "Delete graph";
    char message[64];
    GeGtk *ge;
    pwr_tCmd cmd;

    sprintf( message, "Do you want to delete %s?", sel_text);
    ge = (GeGtk *) item_view->ge_ctx;
    rv = ge->create_modal_dialog( title, message, "Yes", "Cancel", NULL, NULL);
    if ( rv != wow_eModalDialogReturn_Button1)
      return;
        
    sprintf( cmd, "rm -f $pwrp_pop/%s.pwg", sel_text);
    system( cmd);
  }
}

void *graph_list_files()
{
  int file_cnt;
  int allocated, old_allocated;
  pwr_tString80 *file_p;
  pwr_tString80 *old_file_p;
  char found_file[80];
  char fname[80];
  int sts;
  char dev[80];
  char dir[80];
  char file[80];
  char type[80];
  int version;

  // Get the pwg files and order them
  dcli_translate_filename( fname, "$pwrp_pop/*.pwg");

  file_cnt = 0;
  allocated = 0;
  sts = dcli_search_file( fname, found_file, DCLI_DIR_SEARCH_INIT);
  while ( ODD(sts)) {
    if ( strstr( found_file, "__p")) {
      // Skip subgraph pages
      sts = dcli_search_file( fname, found_file, DCLI_DIR_SEARCH_NEXT);
      continue;
    }
    file_cnt++;
    if ( file_cnt > allocated - 1) {
      if ( allocated == 0) {
        allocated = 100;
        file_p = (pwr_tString80 *) malloc( allocated * sizeof(*file_p));
      }
      else {
        old_file_p = file_p;
        old_allocated = allocated;
        allocated += 100;
        file_p = (pwr_tString80 *) malloc( allocated * sizeof(*file_p));
        memcpy( file_p, old_file_p, old_allocated * sizeof(*file_p));
        free( old_file_p);
      }
    }
    dcli_parse_filename( found_file, dev, dir, file, type, &version);
    strcpy( file_p[file_cnt - 1], file);
    if ( strcmp( file, "") == 0)
      file_cnt--;
    sts = dcli_search_file( fname, found_file, DCLI_DIR_SEARCH_NEXT);
  }
  dcli_search_file( fname, found_file, DCLI_DIR_SEARCH_END);

  if ( !file_cnt) {
    return 0;
  }

  strcpy( file_p[file_cnt], "");
  qsort( file_p, file_cnt, sizeof(*file_p), Ge::sort_files);

  return file_p;
}

gpointer graph_list_store( char *texts)
{
  GtkListStore *store;
  GtkTreeIter iter;
  char *nameutf8;
  char *name_p;
  int textsize;

  textsize = 80;
  name_p = texts;
  store = gtk_list_store_new( 1, G_TYPE_STRING);

  while ( strcmp( name_p, "") != 0) {
    nameutf8 = g_convert( name_p, -1, "UTF-8", "ISO8859-1", NULL, NULL, NULL);

    gtk_list_store_append( store, &iter);
    gtk_list_store_set( store, &iter, 0, nameutf8, -1);

    name_p += textsize;
    g_free( nameutf8);
  }

  return store;
}

void directory_changed( GFileMonitor *, GFile *file, GFile *,
                        GFileMonitorEvent event, gpointer item_view)
{
  char *name;

  switch (event) {
  case G_FILE_MONITOR_EVENT_CHANGES_DONE_HINT:
    break;

  case G_FILE_MONITOR_EVENT_DELETED:
  case G_FILE_MONITOR_EVENT_CREATED:
    name = g_file_get_parse_name( file);
    ((GeItemViewGtk *)item_view)->update( name, event);
    g_free( name);
    break;

  default:
    break;
  }
}

void GeItemViewGtk::activate_tree_widget( GtkTreeView *tree_widget, GtkTreePath *path,
                                          GtkTreeViewColumn *column, gpointer data)
{
  char *sel_text;
  gboolean autosave;
  GeItemViewGtk *item_view;
  GeGtk *ge;

  sel_text = GeItemViewGtk::selected_text( GTK_WIDGET(tree_widget));

  if ( path && column) {
    item_view = (GeItemViewGtk *) data;
    ge = (GeGtk *) item_view->ge_ctx;

    autosave = gtk_toggle_button_get_active(
					    GTK_TOGGLE_BUTTON(item_view->autosave_button));
    if ( ge->graph->is_modified()) {
      char name[80];
      ge->graph->get_name( name);
      if ( strcmp( name, "") == 0) {
	ge->wow->DisplayError("Not saved", "Save current graph first");
	return;
      }

      if ( autosave)
	GeGtk::activate_save( NULL, item_view->ge_ctx);
      else {
	int rv;
	char title[] = "Save changes";
	char message[64];
	
	sprintf( message, "Your changes will be lost.\nDo you want to save?");
	rv = ge->create_modal_dialog( title, message, "Yes", "No", NULL, NULL);
	if ( rv == wow_eModalDialogReturn_Button1) {
	  GeGtk::activate_save( NULL, item_view->ge_ctx);
	}
      }
    }
    
  }
  else {
    ge = (GeGtk *) data;
  }

  if ( sel_text) {
    ge->open_graph( sel_text);
  }
}

gboolean GeItemViewGtk::focus_out_tree_widget( GtkWidget *tree_widget, GdkEvent *, gpointer)
{
  GtkTreeIter iter;
  GtkTreeModel *store;
  GtkTreeSelection *selection;
  gboolean selected;

  store = gtk_tree_view_get_model( GTK_TREE_VIEW(tree_widget));
  selection = gtk_tree_view_get_selection( GTK_TREE_VIEW(tree_widget));
  selected = gtk_tree_selection_get_selected( selection, NULL, &iter);
  if ( selected)
    gtk_tree_selection_unselect_iter( selection, &iter);

  return FALSE;
}

gboolean button_press_tree_widget( GtkWidget *tree_widget, GdkEvent *event, gpointer data)
{
  GdkEventButton *ev;
  GtkMenu *menu;
  GtkWidget *menu_item;
  static const char *item_text[] = {
    "Open",
    " ",
    "Delete",
    ""
  };
  static GCallback item_cb[] = {
    G_CALLBACK(GeItemViewGtk::activate_menu_open),
    NULL,
    G_CALLBACK(GeItemViewGtk::activate_menu_delete),
    NULL,
  };

  ev = (GdkEventButton *) event;
  if ( ev->type != GDK_BUTTON_PRESS || ev->button != 3)
    return FALSE;

  menu = (GtkMenu *) gtk_menu_new();
  for ( int i = 0; item_text[i][0]; i++) {
    if ( item_text[i][0] == ' ') {
      menu_item = gtk_separator_menu_item_new();
    }
    else {
      menu_item = gtk_menu_item_new_with_label( item_text[i]);
      g_signal_connect( menu_item, "activate", G_CALLBACK(item_cb[i]), data);
    }
    gtk_widget_show( menu_item);
    gtk_menu_shell_append( GTK_MENU_SHELL(menu), menu_item);
  }

  select_tree_item_pos( tree_widget, ev->x, ev->y);
  gtk_menu_popup( menu, NULL, NULL, NULL, NULL, ev->button,
		  gtk_get_current_event_time());

  return TRUE;
}

void select_tree_item_pos( GtkWidget *tree_widget, gint x, gint y)
{
  GtkTreeSelection *selection;
  int sel_count;

  selection = gtk_tree_view_get_selection( GTK_TREE_VIEW(tree_widget));
  sel_count = gtk_tree_selection_count_selected_rows( selection);
  if ( sel_count <= 1) {
    GtkTreePath *path;

    /* Get tree path for row that was clicked */
    gtk_tree_view_get_path_at_pos( GTK_TREE_VIEW(tree_widget),
				   x, y, &path, NULL, NULL, NULL);
    gtk_tree_selection_unselect_all( selection);
    gtk_tree_selection_select_path( selection, path);
    gtk_tree_path_free( path);
  }
}

void autosave_toggled( GtkToggleButton *autosave_button, gpointer read)
{
#if 0
  GSettings *s;
  gboolean autosave;

  s = g_settings_new( "org.gtk.proview");
  if ( (bool) read) {
    autosave = g_settings_get_boolean( s, "autosave");
    gtk_toggle_button_set_active( autosave_button, autosave);
  }
  else {
    autosave = gtk_toggle_button_get_active( autosave_button);
    g_settings_set_boolean( s, "autosave", autosave);
  }
#endif
}
