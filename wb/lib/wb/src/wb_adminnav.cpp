/* ge_adminnav.cpp -- Display object info

   PROVIEW/R  
   Copyright (C) 1996 by Comator Process AB.

   <Description>.  */

#include "glow_std.h"

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

extern "C" {
#include "co_cdh.h"
#include "co_time.h"
#include "pwr_baseclasses.h"
#include "co_dcli.h"
}

#include <Xm/Xm.h>
#include <Xm/XmP.h>
#include <Xm/Text.h>
#include <Mrm/MrmPublic.h>
#include <X11/Intrinsic.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>

#include "flow.h"
#include "flow_browctx.h"
#include "flow_browapi.h"
#include "flow_browwidget.h"

#include "glow.h"
#include "glow_ctx.h"

#include "wb_adminnav.h"

static char null_str[] = "";

static int adminnav_init_brow_cb( FlowCtx *fctx, void *client_data);
static int admin_read_line( char *line, unsigned int maxsize, FILE *file);

//
// Convert attribute string to value
//
void AdminNav::message( char sev, char *text)
{
  (message_cb)( parent_ctx, sev, text);
}

//
// Create the navigator widget
//
AdminNav::AdminNav(
	void *xn_parent_ctx,
	Widget	xn_parent_wid,
	char *xn_name,
	Widget *w,
	pwr_tStatus *status) :
	parent_ctx(xn_parent_ctx), parent_wid(xn_parent_wid),
	trace_started(0),
	message_cb(NULL), set_focus_cb(NULL),
	menu_tree(NULL), baselist(NULL), displayed(0)
{
  strcpy( name, xn_name);

  form_widget = ScrolledBrowCreate( parent_wid, name, NULL, 0, 
	adminnav_init_brow_cb, this, (Widget *)&brow_widget);
  XtManageChild( form_widget);

  // Create the root item
  *w = form_widget;

  *status = 1;
}

//
//  Delete a nav context
//
AdminNav::~AdminNav()
{
  if ( trace_started)
    XtRemoveTimeOut( trace_timerid);

  delete brow;
  XtDestroyWidget( form_widget);
}

//
//  Print
//
int AdminNav::print( char *filename)
{
  return brow_Print( brow->ctx, filename);
}

//
//  Get zoom
//
void AdminNav::get_zoom( double *zoom_factor)
{
  brow_GetZoom( brow->ctx, zoom_factor);
}

//
//  Zoom
//
void AdminNav::zoom( double zoom_factor)
{
  brow_Zoom( brow->ctx, zoom_factor);
}

//
//  Return to base zoom factor
//
void AdminNav::unzoom()
{
  brow_UnZoom( brow->ctx);
}

void AdminNav::set_inputfocus( int focus)
{
//  brow_SetInputFocus( brow->ctx);
  Arg 		args[2];
  Pixel 	bg, fg;

  if ( !displayed)
    return;

  XtVaGetValues( form_widget, XmNbackground, &bg, XmNforeground, &fg, NULL);
  if ( !focus)
  {
    XtSetArg(args[0], XmNborderColor, bg);
    XtSetValues( form_widget, args, 1);
  }
  else
  {
    XtCallAcceptFocus( brow_widget, CurrentTime);
    XtSetArg(args[0], XmNborderColor, fg);
    XtSetValues( form_widget, args, 1);
  }
}

//
// Callbacks from brow
//
static int adminnav_brow_cb( FlowCtx *ctx, flow_tEvent event)
{
  AdminNav		*adminnav;
  ItemProjHier 	*item;

  if ( event->event == flow_eEvent_ObjectDeleted)
  {
    brow_GetUserData( event->object.object, (void **)&item);
    delete item;
    return 1;
  }

  brow_GetCtxUserData( (BrowCtx *)ctx, (void **) &adminnav);
  adminnav->message( ' ', null_str);
  switch ( event->event)
  {
    case flow_eEvent_Key_Up:
    {
      brow_tNode	*node_list;
      int		node_count;
      brow_tObject	object;
      int		sts;

      brow_GetSelectedNodes( adminnav->brow->ctx, &node_list, &node_count);
      if ( !node_count)
      {
        sts = brow_GetLast( adminnav->brow->ctx, &object);
        if ( EVEN(sts)) return 1;
      }
      else
      {
        sts = brow_GetPrevious( adminnav->brow->ctx, node_list[0], &object);
        if ( EVEN(sts))
        {
          sts = brow_GetLast( adminnav->brow->ctx, &object);
          if ( EVEN(sts))
	  {
            if ( node_count)
	      free( node_list);
            return 1;
 	  }
        }
      }
      brow_SelectClear( adminnav->brow->ctx);
      brow_SetInverse( object, 1);
      brow_SelectInsert( adminnav->brow->ctx, object);
      if ( !brow_IsVisible( adminnav->brow->ctx, object))
        brow_CenterObject( adminnav->brow->ctx, object, 0.25);
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

      brow_GetSelectedNodes( adminnav->brow->ctx, &node_list, &node_count);
      if ( !node_count)
      {
        sts = brow_GetFirst( adminnav->brow->ctx, &object);
        if ( EVEN(sts)) return 1;
      }
      else
      {
        sts = brow_GetNext( adminnav->brow->ctx, node_list[0], &object);
        if ( EVEN(sts))
        {
          sts = brow_GetFirst( adminnav->brow->ctx, &object);
          if ( EVEN(sts))
	  {
            if ( node_count)
	      free( node_list);
            return 1;
 	  }
        }
      }
      brow_SelectClear( adminnav->brow->ctx);
      brow_SetInverse( object, 1);
      brow_SelectInsert( adminnav->brow->ctx, object);
      if ( !brow_IsVisible( adminnav->brow->ctx, object))
        brow_CenterObject( adminnav->brow->ctx, object, 0.75);
      if ( node_count)
        free( node_list);
      break;
    }
    case flow_eEvent_SelectClear:
      brow_ResetSelectInverse( adminnav->brow->ctx);
      break;
    case flow_eEvent_MB1Click:
      // Select
      double ll_x, ll_y, ur_x, ur_y;
      int		sts;

      if ( adminnav->set_focus_cb)
        (adminnav->set_focus_cb)( adminnav->parent_ctx, adminnav);

      switch ( event->object.object_type)
      {
        case flow_eObjectType_Node:
          brow_MeasureNode( event->object.object, &ll_x, &ll_y,
			&ur_x, &ur_y);
	  if ( event->object.x < ll_x + 1.0)
          {
            // Simulate doubleclick
            flow_tEvent doubleclick_event;

            doubleclick_event = (flow_tEvent) calloc( 1, sizeof(*doubleclick_event));
            memcpy( doubleclick_event, event, sizeof(*doubleclick_event));
            doubleclick_event->event = flow_eEvent_MB1DoubleClick;
            sts = adminnav_brow_cb( ctx, doubleclick_event);
            free( (char *) doubleclick_event);
            return sts;
          }

          if ( brow_FindSelectedObject( adminnav->brow->ctx, event->object.object))
          {
            brow_SelectClear( adminnav->brow->ctx);
          }
          else
          {
            brow_SelectClear( adminnav->brow->ctx);
            brow_SetInverse( event->object.object, 1);
            brow_SelectInsert( adminnav->brow->ctx, event->object.object);
          }
          break;
        default:
          brow_SelectClear( adminnav->brow->ctx);
      }
      break;
    case flow_eEvent_Key_Return:
    case flow_eEvent_Key_Right:
    {
      brow_tNode	*node_list;
      int		node_count;

      brow_GetSelectedNodes( adminnav->brow->ctx, &node_list, &node_count);
      if ( !node_count)
        break;
      brow_GetUserData( node_list[0], (void **)&item);
      free( node_list);
      switch( item->type)
      {
        case adminnav_eItemType_ProjHier: 
	  ((ItemProjHier *)item)->open_children( adminnav, 0, 0);
          break;
        case adminnav_eItemType_Project: 
	  ((ItemProject *)item)->open_children( adminnav, 0, 0);
          break;
        case adminnav_eItemType_Db: 
	  ((ItemDb *)item)->open_children( adminnav, 0, 0);
          break;
        default:
          ;
      }
      break;
    }
    case flow_eEvent_Key_PF4:
    case flow_eEvent_Key_Left:
    {
      brow_tNode	*node_list;
      int		node_count;
      brow_tObject	object;
      int		sts;

      brow_GetSelectedNodes( adminnav->brow->ctx, &node_list, &node_count);
      if ( !node_count)
        break;
      if ( brow_IsOpen( node_list[0]))
        // Close this node
        object = node_list[0];
      else
      {
        // Close parent
        sts = brow_GetParent( adminnav->brow->ctx, node_list[0], &object);
        if ( EVEN(sts))
          break;
      }
      brow_GetUserData( object, (void **)&item);
      switch( item->type)
      {
        case adminnav_eItemType_ProjHier: 
	  ((ItemProjHier *)item)->close( adminnav, 0, 0);
          break;
        case adminnav_eItemType_Project: 
	  ((ItemProject *)item)->close( adminnav, 0, 0);
          break;
        case adminnav_eItemType_Db: 
	  ((ItemDb *)item)->close( adminnav, 0, 0);
          break;
        default:
          ;
      }
      brow_SelectClear( adminnav->brow->ctx);
      brow_SetInverse( object, 1);
      brow_SelectInsert( adminnav->brow->ctx, object);
      if ( !brow_IsVisible( adminnav->brow->ctx, object))
        brow_CenterObject( adminnav->brow->ctx, object, 0.25);
      free( node_list);
      break;
    }
    case flow_eEvent_MB1DoubleClick:
      switch ( event->object.object_type)
      {
        case flow_eObjectType_Node:
          brow_GetUserData( event->object.object, (void **)&item);
          switch( item->type)
          {
            case adminnav_eItemType_ProjHier:
	      ((ItemProjHier *)item)->open_children( adminnav,
			event->object.x, event->object.y);
              break;
            case adminnav_eItemType_Project: 
	      ((ItemProject *)item)->open_children( adminnav,
			event->object.x, event->object.y);
              break;
            case adminnav_eItemType_Db: 
	      ((ItemDb *)item)->open_children( adminnav,
			event->object.x, event->object.y);
              break;
            default:
              ;
          }
          break;
        default:
          ;
      }
      break;
    case flow_eEvent_Key_Tab:
    {
      break;
    }
    case flow_eEvent_Map:
    {
      adminnav->displayed = 1;
      break;
    }
    default:
      ;
  }
  return 1;
}


int AdminNav::get_select( void **item)
{
  brow_tNode	*node_list;
  int		node_count;
  
  brow_GetSelectedNodes( brow->ctx, &node_list, &node_count);
  if ( node_count != 1)
    return 0;

  brow_GetUserData( node_list[0], (void **)item);
  free( node_list);

  return 1;
}

void	AdminNav::tree_build()
{
  adminnav_sMenu *menu_p;

  brow_SetNodraw( brow->ctx);

  tree_load();

  // Create the root items
  for ( menu_p = menu_tree; menu_p; menu_p = menu_p->next)
  {
    switch( menu_p->item_type)
    {
      case adminnav_eItemType_ProjHier:
        new ItemProjHier( this, menu_p->name, menu_p->hierarchy, menu_p->child_list, 
		NULL, flow_eDest_After);
        break;
      case adminnav_eItemType_Project:
        new ItemProject( this, menu_p->name, menu_p->project, 
		menu_p->base, menu_p->root, menu_p->hierarchy, 
		menu_p->description, 
		NULL, flow_eDest_After);
        break;
      default:
        ;
    }
  }

  brow_ResetNodraw( brow->ctx);
  brow_Redraw( brow->ctx, 0);
}

void AdminNav::refresh()
{
  char	 	open_objects[100][80];
  int		open_cnt;
  brow_tObject 	*object_list;
  int		object_cnt;
  Item	 	*object_item;
  int		i, j;
  int		open_type[100];
  int		open;
  int		found;
  brow_tNode	*node_list;
  int		sel_node_count;
  Item		*item_sel;
  char		sel_object[80];
  int		sel_type;

  // Store all open objects

  open_cnt = 0;
  brow_GetObjectList( brow->ctx, &object_list, &object_cnt);
  for ( i = 0; i < object_cnt; i++)
  {
    if ( (open = brow_IsOpen( object_list[i])))
    {
      brow_GetUserData( object_list[i], (void **)&object_item);
      switch( object_item->type)
      {
        case adminnav_eItemType_ProjHier:
          strcpy( open_objects[open_cnt], ((ItemProjHier *)object_item)->hierarchy);
          break;
        case adminnav_eItemType_Project:
          strcpy( open_objects[open_cnt], ((ItemProject *)object_item)->hierarchy);
          break;
        case adminnav_eItemType_Db:
          strcpy( open_objects[open_cnt], ((ItemProject *)object_item)->hierarchy);
          break;
        default:
          ;
      }
      open_type[open_cnt] = open;
      open_cnt++;
      if ( open_cnt == 100)
        break;
    }
  }

  // Store selected object

  brow_GetSelectedNodes( brow->ctx, &node_list, &sel_node_count);
  if ( sel_node_count > 0)
  {
    brow_GetUserData( node_list[0], (void **)&item_sel);
    sel_type = item_sel->type;
    switch( item_sel->type)
    {
      case adminnav_eItemType_ProjHier:
        strcpy( sel_object, ((ItemProjHier *)item_sel)->hierarchy);
        break;
      case adminnav_eItemType_Project:
        strcpy( sel_object, ((ItemProject *)item_sel)->hierarchy);
        break;
      case adminnav_eItemType_Db:
        strcpy( sel_object, ((ItemDb *)item_sel)->hierarchy);
        break;
      case adminnav_eItemType_Volume:
        strcpy( sel_object, ((ItemVolume *)item_sel)->hierarchy);
        break;
      default:
          ;
    }
    free( node_list);
  }

  brow_SetNodraw( brow->ctx);
  brow_DeleteAll( brow->ctx);
  
  // Restore the rootlist
  tree_free();
  tree_build();

  // Open all previously open objects
  for ( i = 0; i < open_cnt; i++)
  {
    brow_GetObjectList( brow->ctx, &object_list, &object_cnt);
    found = 0;
    for ( j = object_cnt - 1; j >= 0; j--)
    {
      brow_GetUserData( object_list[j], (void **)&object_item);
      switch( object_item->type)
      {
        case adminnav_eItemType_ProjHier:
          if ( strcmp( ((ItemProjHier *)object_item)->hierarchy, 
			open_objects[i]) == 0)
          {
            if ( open_type[i] & adminnav_mOpen_Children)
              ((ItemProjHier *)object_item)->open_children( this, 0, 0);
            found = 1;
          }
          break;
        case adminnav_eItemType_Project:
          if ( strcmp( ((ItemProject *)object_item)->hierarchy, 
			open_objects[i]) == 0)
          {
            if ( open_type[i] & adminnav_mOpen_Children)
              ((ItemProject *)object_item)->open_children( this, 0, 0);
            found = 1;
          }
          break;
        case adminnav_eItemType_Db:
          if ( strcmp( ((ItemDb *)object_item)->hierarchy, 
			open_objects[i]) == 0)
          {
            if ( open_type[i] & adminnav_mOpen_Children)
              ((ItemDb *)object_item)->open_children( this, 0, 0);
            found = 1;
          }
          break;
        default:
          ;
      }
      if ( found)
        break;
    }
  }

  // Select previously selected
  if ( sel_node_count > 0)
  {
    brow_GetObjectList( brow->ctx, &object_list, &object_cnt);
    for ( j = object_cnt - 1; j >= 0; j--)
    {
      brow_GetUserData( object_list[j], (void **)&object_item);
      found = 0;

      switch( object_item->type)
      {
        case adminnav_eItemType_ProjHier:
          if ( strcmp( ((ItemProjHier *)object_item)->hierarchy, 
			sel_object) == 0)
          {
            brow_SetInverse( ((ItemProjHier *)object_item)->node, 1);
            brow_SelectInsert( brow->ctx, ((ItemProjHier *)object_item)->node);
            found = 1;
          }
          break;
        case adminnav_eItemType_Project:
          if ( strcmp( ((ItemProject *)object_item)->hierarchy, 
			sel_object) == 0)
          {
            brow_SetInverse( ((ItemProject *)object_item)->node, 1);
            brow_SelectInsert( brow->ctx, ((ItemProject *)object_item)->node);
            found = 1;
          }
          break;
        case adminnav_eItemType_Db:
          if ( strcmp( ((ItemDb *)object_item)->hierarchy, 
			sel_object) == 0)
          {
            brow_SetInverse( ((ItemDb *)object_item)->node, 1);
            brow_SelectInsert( brow->ctx, ((ItemDb *)object_item)->node);
            found = 1;
          }
          break;
        case adminnav_eItemType_Volume:
          if ( strcmp( ((ItemVolume *)object_item)->hierarchy, 
			sel_object) == 0)
          {
            brow_SetInverse( ((ItemVolume *)object_item)->node, 1);
            brow_SelectInsert( brow->ctx, ((ItemVolume *)object_item)->node);
            found = 1;
          }
          break;
        default:
          ;
      }

      if ( found)
        break;
    }
  }

  brow_ResetNodraw( brow->ctx);
  brow_Redraw( brow->ctx, 0);

}

int AdminNav::basename_to_baseroot( char *name, char *root)
{
  adminnav_sBase *base_p;

  for ( base_p = baselist; base_p; base_p = base_p->next)
  {
    if ( strcmp( base_p->name, name) == 0)
    {
      strcpy( root, base_p->root);
      return 1;
    }
  }
  return 0;
}

void AdminNav::tree_load()
{
  FILE		*fp;
  char		filename[80]="$pwra_db/pwr_projectlist.dat";
  char		item_str[5][80];
  int		nr;
  char		*s1, *s2, *s;
  int		len;
  char		line[200];
  adminnav_sMenu *menu_p;
  adminnav_sBase *base_p;

  dcli_translate_filename( filename, filename);
  if ( !check_file( filename))
    return;

  fp = fopen( filename, "r");
  if ( !fp)
    return;

  while ( admin_read_line( line, sizeof(line), fp))
  {
    nr = dcli_parse( line, " 	", "", (char *)item_str, 
		sizeof( item_str) / sizeof( item_str[0]), sizeof( item_str[0]), 0);
    if ( nr == 0)
      continue;
    if ( item_str[0][0] == '!' || item_str[0][0] == '#')
      continue;

    if ( strcmp( item_str[0], "%base") == 0)
    {
      if ( nr < 3)
        continue;
      base_p = (adminnav_sBase *) calloc( 1, sizeof(*menu_p));
      strcpy( base_p->name, item_str[1]);
      strcpy( base_p->root, item_str[2]);

      // Insert first in baselist
      base_p->next = baselist;
      baselist = base_p;

      continue;
    }

    menu_p = (adminnav_sMenu *) calloc( 1, sizeof(*menu_p));
    strcpy( menu_p->project, item_str[0]);
    menu_p->item_type = adminnav_eItemType_Project;
    if ( nr > 1)
      strcpy( menu_p->base, item_str[1]);
    if ( nr > 2)
      strcpy( menu_p->root, item_str[2]);
    if ( nr > 3 && strcmp( item_str[3], "-") != 0)
      strcpy( menu_p->hierarchy, item_str[3]);
    if ( (s1 = strchr( line, '"')) && (s2=strrchr( line, '"')) && s2 > s1)
    {
      len = s2 - s1 - 1;
      if ( len > (int) sizeof(menu_p->description))
        len = sizeof(menu_p->description);
      strncpy( menu_p->description, s1+1, len);
      menu_p->description[len] = 0;
    }
    if ( strcmp( menu_p->hierarchy, "") != 0)
    {
      // Name is the last segment of the hierarchy
      if ( (s = strrchr( menu_p->hierarchy, '-')))
        strcpy( menu_p->name, s+1);
      else
        strcpy( menu_p->name, menu_p->hierarchy);
    }
    else
      strcpy( menu_p->name, menu_p->project);

    insert_tree( menu_p, NULL, menu_p->hierarchy);
  }  

  fclose(fp);
}

void AdminNav::insert_tree( adminnav_sMenu *menu, adminnav_sMenu *parent, 
	char *hierarchy)
{
  adminnav_sMenu *menu_p, *menu_np;
  char lhierarchy[80];
  char hier[80];
  char *s;

  strcpy( lhierarchy, hierarchy);
  if ( !parent && 
        (strcmp( hierarchy, "") == 0 || !strchr( hierarchy, '-')))
  {
    // Insert as toplevel
    if ( ! menu_tree)
      menu_tree = menu;
    else
    {
      for ( menu_p = menu_tree; menu_p->next; menu_p = menu_p->next)
        ;
      menu_p->next = menu;
    }
    return;
  }

  if ( !(s = strchr( lhierarchy, '-')))
  {
    // Last segment, insert last
    if ( ! parent->child_list)
      parent->child_list = menu;
    else
    {
      for ( menu_p = parent->child_list; menu_p->next; menu_p = menu_p->next)
        ;
      menu_p->next = menu;
    }
    menu->parent = parent;
  }
  else
  {
    *s = 0;
    // Try to find this hierarchy
    if ( !parent)
      menu_p = menu_tree;
    else
      menu_p = parent->child_list;
    for ( ; menu_p; menu_p = menu_p->next)
    {
      if ( strcmp( menu_p->name, lhierarchy) == 0)
      {
        insert_tree( menu, menu_p, s+1);
        return;
      }
    }

    // Not found, create a hierarchy item, and insert last
    menu_np = (adminnav_sMenu *) calloc( 1, sizeof(*menu_np));
    strcpy( menu_np->name, lhierarchy);
    if ( !parent)
      strcpy( menu_np->hierarchy, lhierarchy);
    else
    {
      strcpy( hier, parent->hierarchy);
      strcat( hier, "-");
      strcat( hier, lhierarchy);
      strcpy( menu_np->hierarchy, hier);
    }
    menu_np->item_type = adminnav_eItemType_ProjHier;

    if ( !parent)
    {
      if ( !menu_tree)
        menu_tree = menu_np;
      else
      {
        for ( menu_p = menu_tree; menu_p->next; menu_p = menu_p->next)
          ;
        menu_p->next = menu_np;
      }
    }
    else
    {
      if ( ! parent->child_list)
        parent->child_list = menu_np;
      else
      {
        for ( menu_p = parent->child_list; menu_p->next; menu_p = menu_p->next)
          ;
        menu_p->next = menu_np;
      }
    }
    insert_tree( menu, menu_np, s+1);
  }
}

void AdminNav::tree_free()
{
  menu_tree = 0;
  baselist = 0;
}

void AdminNav::enable_events()
{
  brow_EnableEvent( brow->ctx, flow_eEvent_MB1Click, flow_eEventType_CallBack, 
	adminnav_brow_cb);
  brow_EnableEvent( brow->ctx, flow_eEvent_MB1DoubleClick, flow_eEventType_CallBack, 
	adminnav_brow_cb);
  brow_EnableEvent( brow->ctx, flow_eEvent_Key_PF4, flow_eEventType_CallBack, 
	adminnav_brow_cb);
  brow_EnableEvent( brow->ctx, flow_eEvent_Key_Return, flow_eEventType_CallBack, 
	adminnav_brow_cb);
  brow_EnableEvent( brow->ctx, flow_eEvent_Key_Right, flow_eEventType_CallBack, 
	adminnav_brow_cb);
  brow_EnableEvent( brow->ctx, flow_eEvent_Key_Left, flow_eEventType_CallBack, 
	adminnav_brow_cb);
  brow_EnableEvent( brow->ctx, flow_eEvent_SelectClear, flow_eEventType_CallBack, 
	adminnav_brow_cb);
  brow_EnableEvent( brow->ctx, flow_eEvent_ObjectDeleted, flow_eEventType_CallBack, 
	adminnav_brow_cb);
  brow_EnableEvent( brow->ctx, flow_eEvent_Key_Up, flow_eEventType_CallBack, 
	adminnav_brow_cb);
  brow_EnableEvent( brow->ctx, flow_eEvent_Key_Down, flow_eEventType_CallBack, 
	adminnav_brow_cb);
  brow_EnableEvent( brow->ctx, flow_eEvent_Key_PF3, flow_eEventType_CallBack, 
	adminnav_brow_cb);
  brow_EnableEvent( brow->ctx, flow_eEvent_Key_Tab, flow_eEventType_CallBack, 
	adminnav_brow_cb);
  brow_EnableEvent( brow->ctx, flow_eEvent_Map, flow_eEventType_CallBack, 
	adminnav_brow_cb);
}

//
// Backcall routine called at creation of the brow widget
// Enable event, create nodeclasses and insert the root objects.
//
static int adminnav_init_brow_cb( FlowCtx *fctx, void *client_data)
{
  AdminNav *adminnav = (AdminNav *) client_data;
  BrowCtx *ctx = (BrowCtx *)fctx;

  adminnav->brow = new WNavBrow( ctx, (void *)adminnav);

  adminnav->brow->brow_setup();
  adminnav->brow->create_nodeclasses();
  adminnav->enable_events();


  // Create the root item
  adminnav->tree_build();

  return 1;
}

static int admin_read_line( char *line, unsigned int maxsize, FILE *file)
{ 
	char	*s;

	if (fgets( line, maxsize, file) == NULL)
	  return 0;
	line[maxsize-1] = 0;
	s = strchr( line, 10);
	if ( s != 0)
	  *s = 0;

	return 1;
}

ItemProjHier::ItemProjHier( AdminNav *adminnav, 
	char *item_name, char *item_hier, adminnav_sMenu *item_child_list,
	brow_tNode dest, flow_eDest dest_code) :
	Item(adminnav_eItemType_ProjHier), child_list(item_child_list)
{

  strcpy( name, item_name);
  strcpy( hierarchy, item_hier);
  

  brow_CreateNode( adminnav->brow->ctx, item_name, 
		adminnav->brow->nc_object, 
		dest, dest_code, (void *) this, 1, &node);

  brow_SetAnnotPixmap( node, 0, adminnav->brow->pixmap_map);

  brow_SetAnnotation( node, 0, item_name, strlen(item_name));
}

int ItemProjHier::open_children( AdminNav *adminnav, double x, double y)
{
  int		action_open;

  if ( !brow_IsOpen( node))
    action_open = 1;
  else 
    action_open = 0;

  if ( action_open)
  {
    // Display childlist
    double	node_x, node_y;
    Item 		*item;
    adminnav_sMenu	*menu;

    brow_GetNodePosition( node, &node_x, &node_y);
    brow_SetNodraw( adminnav->brow->ctx);

    menu = child_list;
    while ( menu)
    {
      switch ( menu->item_type)
      {
        case adminnav_eItemType_Project:
          item = (Item *) new ItemProject( adminnav, menu->name, 
		menu->project, menu->base, menu->root, menu->hierarchy, 
		menu->description,
		node, flow_eDest_IntoLast);
          break;
        case adminnav_eItemType_ProjHier:
          item = (Item *) new ItemProjHier( adminnav, 
		menu->name, menu->hierarchy, menu->child_list, node, flow_eDest_IntoLast);
          break;
    
        default:
          ;
      }
      menu = menu->next;
      brow_SetOpen( node, adminnav_mOpen_Children);
      brow_SetAnnotPixmap( node, 0, adminnav->brow->pixmap_openmap);
    }
    brow_ResetNodraw( adminnav->brow->ctx);
    brow_Redraw( adminnav->brow->ctx, node_y);
  }
  else
    close( adminnav, x, y);
  return 1;
}

int ItemProjHier::close( AdminNav *adminnav, double x, double y)
{
  double	node_x, node_y;

  if ( brow_IsOpen( node))
  {
    // Close
    brow_GetNodePosition( node, &node_x, &node_y);
    brow_SetNodraw( adminnav->brow->ctx);
    brow_CloseNode( adminnav->brow->ctx, node);
    brow_SetAnnotPixmap( node, 0, adminnav->brow->pixmap_map);
    brow_ResetOpen( node, adminnav_mOpen_All);
    brow_ResetNodraw( adminnav->brow->ctx);
    brow_Redraw( adminnav->brow->ctx, node_y);
  }
  return 1;
}

ItemProject::ItemProject( AdminNav *adminnav, char *item_name, char *item_project,
	char *item_base, char *item_root, char *item_hier, char *item_descr,
	brow_tNode dest, flow_eDest dest_code) :
	Item( adminnav_eItemType_Project), node(NULL)
{
  type = adminnav_eItemType_Project;
  strcpy( name, item_name);
  strcpy( project, item_project);
  strcpy( base, item_base);
  strcpy( root, item_root);
  strcpy( hierarchy, item_hier);
  strcpy( description, item_descr);

  brow_CreateNode( adminnav->brow->ctx, name, adminnav->brow->nc_table,
		dest, dest_code, (void *)this, 1, &node);

  // Set pixmap
  brow_SetAnnotPixmap( node, 0, adminnav->brow->pixmap_project);

  // Set object name annotation
  brow_SetAnnotation( node, 0, name, strlen(name));
  brow_SetAnnotation( node, 1, project, strlen(project));
  brow_SetAnnotation( node, 2, description, strlen(description));
}

int ItemProject::open_children( AdminNav *adminnav, double x, double y)
{
  int		action_open;
  char		filename[120];
  char		dblist[100][40];
  int		db_cnt = 0;
  char		item_str[5][80];
  int		nr;
  FILE		*fp;
  char		line[200];
  int		i, found;

  if ( !brow_IsOpen( node))
    action_open = 1;
  else 
    action_open = 0;

  if ( action_open)
  {
    // Display childlist
    double	node_x, node_y;

    // Get databases... 
    strcpy( filename, root);
    strcat( filename, "/common/db/pwrp_cnf_volumelist.dat");
    fp = fopen( filename, "r");
    if ( !fp)
      return 0;

    brow_GetNodePosition( node, &node_x, &node_y);
    brow_SetNodraw( adminnav->brow->ctx);

    while ( admin_read_line( line, sizeof(line), fp))
    {
      nr = dcli_parse( line, " 	", "", (char *)item_str, 
		sizeof( item_str) / sizeof( item_str[0]), sizeof( item_str[0]), 0);
      if ( nr < 3)
        continue;

      // Check that it's not already inserted
      found = 0;
      for ( i = 0; i < db_cnt; i++)
      {
        if ( strcmp( dblist[i], item_str[2]) == 0)
        {
          found = 1;
          break;
        }
      }
      if ( !found)
      {
        strcpy( dblist[db_cnt], item_str[2]);
        db_cnt++;
      }
    }
    fclose( fp);

    for ( i = 0; i < db_cnt; i++)
    {
      new ItemDb( adminnav, dblist[i],
		project, dblist[i], root, node, flow_eDest_IntoLast);
    }
    if ( db_cnt)
      brow_SetOpen( node, adminnav_mOpen_Children);
    brow_ResetNodraw( adminnav->brow->ctx);
    brow_Redraw( adminnav->brow->ctx, node_y);
  }
  else
    close( adminnav, x, y);
  return 1;
}

int ItemProject::close( AdminNav *adminnav, double x, double y)
{
  double	node_x, node_y;

  if ( brow_IsOpen( node))
  {
    // Close
    brow_GetNodePosition( node, &node_x, &node_y);
    brow_SetNodraw( adminnav->brow->ctx);
    brow_CloseNode( adminnav->brow->ctx, node);
    brow_ResetOpen( node, adminnav_mOpen_All);
    brow_ResetNodraw( adminnav->brow->ctx);
    brow_Redraw( adminnav->brow->ctx, node_y);
  }
  return 1;
}

ItemDb::ItemDb( AdminNav *adminnav, char *item_name, char *item_project,
	char *item_db, char *item_root,
	brow_tNode dest, flow_eDest dest_code) :
	Item( adminnav_eItemType_Db), node(NULL)
{
  type = adminnav_eItemType_Db;
  strcpy( name, item_name);
  strcpy( project, item_project);
  strcpy( db, item_db);
  strcpy( root, item_root);

  brow_CreateNode( adminnav->brow->ctx, name, adminnav->brow->nc_object,
		dest, dest_code, (void *)this, 1, &node);

  // Set pixmap
  brow_SetAnnotPixmap( node, 0, adminnav->brow->pixmap_database);

  // Set object name annotation
  brow_SetAnnotation( node, 0, name, strlen(name));
}

int ItemDb::open_children( AdminNav *adminnav, double x, double y)
{
  int		action_open;
  char		filename[120];
  char		item_str[5][80];
  int		nr;
  char		line[200];
  FILE		*fp;
  int		child_cnt = 0;

  if ( !brow_IsOpen( node))
    action_open = 1;
  else 
    action_open = 0;

  if ( action_open)
  {
    // Display childlist
    double	node_x, node_y;

    // Get databases... 
    strcpy( filename, root);
    strcat( filename, "/common/db/pwrp_cnf_volumelist.dat");
    fp = fopen( filename, "r");
    if ( !fp)
      return 0;

    brow_GetNodePosition( node, &node_x, &node_y);
    brow_SetNodraw( adminnav->brow->ctx);

    while ( admin_read_line( line, sizeof(line), fp))
    {
      nr = dcli_parse( line, " 	", "", (char *)item_str, 
		sizeof( item_str) / sizeof( item_str[0]), sizeof( item_str[0]), 0);
      if ( nr < 3)
        continue;

      if ( strcmp( db, item_str[2]) == 0)
      {
        child_cnt++;
        new ItemVolume( adminnav, item_str[0], 
		project, db, item_str[0], root, node, flow_eDest_IntoLast);
      }
    }
    fclose( fp);

    if ( child_cnt)
      brow_SetOpen( node, adminnav_mOpen_Children);
    brow_ResetNodraw( adminnav->brow->ctx);
    brow_Redraw( adminnav->brow->ctx, node_y);
  }
  else
    close( adminnav, x, y);
  return 1;
}

int ItemDb::close( AdminNav *adminnav, double x, double y)
{
  double	node_x, node_y;

  if ( brow_IsOpen( node))
  {
    // Close
    brow_GetNodePosition( node, &node_x, &node_y);
    brow_SetNodraw( adminnav->brow->ctx);
    brow_CloseNode( adminnav->brow->ctx, node);
    brow_ResetOpen( node, adminnav_mOpen_All);
    brow_ResetNodraw( adminnav->brow->ctx);
    brow_Redraw( adminnav->brow->ctx, node_y);
  }
  return 1;
}

ItemVolume::ItemVolume( AdminNav *adminnav, char *item_name, char *item_project,
	char *item_db, char *item_volume, char *item_root,
	brow_tNode dest, flow_eDest dest_code) :
	Item( adminnav_eItemType_Volume), node(NULL)
{
  type = adminnav_eItemType_Volume;
  strcpy( name, item_name);
  strcpy( project, item_project);
  strcpy( db, item_db);
  strcpy( volume, item_volume);
  strcpy( root, item_root);

  brow_CreateNode( adminnav->brow->ctx, name, adminnav->brow->nc_object,
		dest, dest_code, (void *)this, 1, &node);

  // Set pixmap
  brow_SetAnnotPixmap( node, 0, adminnav->brow->pixmap_volume);

  // Set object name annotation
  brow_SetAnnotation( node, 0, name, strlen(name));
}
