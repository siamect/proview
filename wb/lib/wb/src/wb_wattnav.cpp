/* 
 * Proview   $Id: wb_wattnav.cpp,v 1.15 2007-01-15 13:23:17 claes Exp $
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

/* wb_wattnav.cpp -- Display object info */

#if defined OS_VMS && defined __ALPHA
# pragma message disable (NOSIMPINT,EXTROUENCUNNOBJ)
#endif

#if defined OS_VMS && !defined __ALPHA
# pragma message disable (LONGEXTERN)
#endif

#include <stdio.h>
#include <stdlib.h>

#include "co_cdh.h"
#include "co_time.h"
#include "pwr_baseclasses.h"
#include "wb_watt_msg.h"
#include "rt_mh_net.h"
#include "flow.h"
#include "flow_browctx.h"
#include "flow_browapi.h"

#include "wb_watt.h"
#include "wb_wattnav.h"
#include "wb_wnav.h"
#include "wb_wnav_brow.h"
#include "wb_wnav_item.h"

void WAttNav::message( char sev, char *text)
{
  (message_cb)( parent_ctx, sev, text);
}


//
// Create the navigator widget
//
WAttNav::WAttNav(
	void 		*wa_parent_ctx,
	char 		*wa_name,
	ldh_tSesContext wa_ldhses,
	pwr_sAttrRef 	wa_aref,
	int 		wa_editmode,
	int 		wa_advanced_user,
	int		wa_display_objectname,
	wb_eUtility	wa_utility,
	pwr_tStatus 	*status) :
  parent_ctx(wa_parent_ctx),
  ldhses(wa_ldhses), aref(wa_aref), editmode(wa_editmode), 
  advanced_user(wa_advanced_user), 
  display_objectname(wa_display_objectname), bypass(0),
  trace_started(0), message_cb(NULL), utility(wa_utility), 
  displayed(0)
{
  strcpy( name, wa_name);
  *status = 1;
}

//
//  Delete a nav context
//
WAttNav::~WAttNav()
{
}

//
// Check that the current selected item is valid for change
//
int WAttNav::check_attr( int *multiline, brow_tObject *node,
	char *name, char **init_value, int *size)
{
  brow_tNode	*node_list;
  int		node_count;
  WItem		*base_item;
  int		sts;
  char          *p;
  int           len;
  
  brow_GetSelectedNodes( brow->ctx, &node_list, &node_count);
  if ( !node_count)
    return WATT__NOATTRSEL;

  brow_GetUserData( node_list[0], (void **)&base_item);
  free( node_list);

  switch( base_item->type)
  {
    case wnav_eItemType_Attr:
    case wnav_eItemType_AttrInput:
    case wnav_eItemType_AttrInputF:
    case wnav_eItemType_AttrInputInv:
    case wnav_eItemType_AttrOutput:
    case wnav_eItemType_AttrArrayElem:
    {
      WItemBaseAttr *item = (WItemBaseAttr *)base_item;

      if ( !editmode)
        return WATT__NOEDIT;
      if ( item->flags & PWR_MASK_NOEDIT && !bypass)
        return WATT__FLAGNOEDIT;
      if ( item->flags & PWR_MASK_STATE && !bypass)
        return WATT__FLAGSTATE;
      if ( cdh_tidIsCid( item->type_id))
        return WATT__FLAGNOEDIT;

      sts = item->get_value( (char **)&p);
      wnav_attrvalue_to_string( ldhses, item->type_id, p, init_value, 
				  &len);
      free( p);
      switch ( item->type_id) {
      case pwr_eType_Objid:
      case pwr_eType_AttrRef:
      case pwr_eType_ClassId:
	*size = sizeof(pwr_tOName) - 1;
	break;
      case pwr_eType_VolumeId:
      case pwr_eType_TypeId:
	*size = sizeof(pwr_tObjName) - 1;
	break;
      case pwr_eType_Time:
      case pwr_eType_DeltaTime:
	*size = 40;
	break;
      default:
	*size = item->size;
      }
      if ( item->type_id == pwr_eType_Text)
        *multiline = 1;
      else
        *multiline = 0;

      *node = item->node;
      strcpy( name, item->attr);
      break;
    }
    case wnav_eItemType_ObjectName:
    {
      static char name[32];
      char *p;

      if ( !editmode)
        return WATT__NOEDIT;

      sts = ((WItemObjectName *)base_item)->get_value( &p);
      if ( ODD(sts)) {
        strcpy( name, p);
        free( p);
      }
      else
        strcpy( name, "");
      *init_value = name;
      *multiline = 0;
      *size = sizeof( name) - 1;

      *node = ((WItemObjectName *)base_item)->node;
      break;
    }
    case wnav_eItemType_DocBlock:
    {
      static char block[10000];
      char *p;

      if ( !editmode)
        return WATT__NOEDIT;

      sts = ((WItemDocBlock *)base_item)->get_value( &p);
      if ( ODD(sts)) {
        strcpy( block, p);
        free( p);
      }
      else
        strcpy( name, "");
      *init_value = block;
      *multiline = 1;
      *size = sizeof( block);

      *node = ((WItemDocBlock *)base_item)->node;
      break;
    }
    default:
      *multiline = 0;
      return WATT__ATTRNOEDIT;
  }
  return WATT__SUCCESS;
}


//
// Callbacks from brow
//
int WAttNav::brow_cb( FlowCtx *ctx, flow_tEvent event)
{
  WAttNav		*wattnav;
  WItem	 		*item;

  if ( event->event == flow_eEvent_ObjectDeleted)
  {
    brow_GetUserData( event->object.object, (void **)&item);
    delete item;
    return 1;
  }

  brow_GetCtxUserData( (BrowCtx *)ctx, (void **) &wattnav);
  wattnav->message( ' ', "");
  switch ( event->event)
  {
    case flow_eEvent_Key_Up:
    {
      brow_tNode	*node_list;
      int		node_count;
      brow_tObject	object;
      int		sts;

      brow_GetSelectedNodes( wattnav->brow->ctx, &node_list, &node_count);
      if ( !node_count) {
        sts = brow_GetLastVisible( wattnav->brow->ctx, &object);
        if ( EVEN(sts)) return 1;
      }
      else {
	if ( !brow_IsVisible( wattnav->brow->ctx, node_list[0], flow_eVisible_Partial)) {
	  sts = brow_GetLastVisible( wattnav->brow->ctx, &object);
	  if ( EVEN(sts)) return 1;
	}
	else {
	  sts = brow_GetPrevious( wattnav->brow->ctx, node_list[0], &object);
	  if ( EVEN(sts)) {
            if ( node_count)
	      free( node_list);
            return 1;
 	  }
        }
      }
      brow_SelectClear( wattnav->brow->ctx);
      brow_SetInverse( object, 1);
      brow_SelectInsert( wattnav->brow->ctx, object);
      if ( !brow_IsVisible( wattnav->brow->ctx, object, flow_eVisible_Full))
        brow_CenterObject( wattnav->brow->ctx, object, 0.25);
      if ( node_count)
        free( node_list);
      break;
    }
    case flow_eEvent_Key_PageDown: {
      brow_Page( wattnav->brow->ctx, 0.8);
      break;
    }
    case flow_eEvent_Key_PageUp: {
      brow_Page( wattnav->brow->ctx, -0.8);
      break;
    }
    case flow_eEvent_ScrollDown: {
      brow_Page( wattnav->brow->ctx, 0.1);
      break;
    }
    case flow_eEvent_ScrollUp: {
      brow_Page( wattnav->brow->ctx, -0.1);
      break;
    }
    case flow_eEvent_Key_Down:
    {
      brow_tNode	*node_list;
      int		node_count;
      brow_tObject	object;
      int		sts;

      brow_GetSelectedNodes( wattnav->brow->ctx, &node_list, &node_count);
      if ( !node_count) {
        sts = brow_GetFirstVisible( wattnav->brow->ctx, &object);
        if ( EVEN(sts)) return 1;
      }
      else {
	if ( !brow_IsVisible( wattnav->brow->ctx, node_list[0], flow_eVisible_Partial)) {
	  sts = brow_GetFirstVisible( wattnav->brow->ctx, &object);
	  if ( EVEN(sts)) return 1;
	}
	else {
	  sts = brow_GetNext( wattnav->brow->ctx, node_list[0], &object);
	  if ( EVEN(sts)) {
            if ( node_count)
	      free( node_list);
            return 1;
 	  }
        }
      }
      brow_SelectClear( wattnav->brow->ctx);
      brow_SetInverse( object, 1);
      brow_SelectInsert( wattnav->brow->ctx, object);
      if ( !brow_IsVisible( wattnav->brow->ctx, object, flow_eVisible_Full))
        brow_CenterObject( wattnav->brow->ctx, object, 0.75);
      if ( node_count)
        free( node_list);
      break;
    }
    case flow_eEvent_SelectClear:
      brow_ResetSelectInverse( wattnav->brow->ctx);
      break;
    case flow_eEvent_MB1Click:
    {
      // Select
      double ll_x, ll_y, ur_x, ur_y;
      int		sts;

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
            sts = WAttNav::brow_cb( ctx, doubleclick_event);
            free( (char *) doubleclick_event);
            return sts;
          }

          if ( brow_FindSelectedObject( wattnav->brow->ctx, event->object.object))
          {
            brow_SelectClear( wattnav->brow->ctx);
          }
          else
          {
            brow_SelectClear( wattnav->brow->ctx);
            brow_SetInverse( event->object.object, 1);
            brow_SelectInsert( wattnav->brow->ctx, event->object.object);
          }
          break;
        default:
          brow_SelectClear( wattnav->brow->ctx);
      }
      break;
    }
    case flow_eEvent_Key_Left:
    {
      brow_tNode	*node_list;
      int		node_count;
      brow_tObject	object;
      int		sts;

      brow_GetSelectedNodes( wattnav->brow->ctx, &node_list, &node_count);
      if ( !node_count)
        return 1;

      if ( brow_IsOpen( node_list[0]))
        // Close this node
        object = node_list[0];
      else
      {
        // Close parent
        sts = brow_GetParent( wattnav->brow->ctx, node_list[0], &object);
        if ( EVEN(sts))
        {
          free( node_list);
          return 1;
        }
      }
      brow_GetUserData( object, (void **)&item);
      switch( item->type)
      {
        case wnav_eItemType_Attr: 
	  ((WItemAttr *)item)->close( 0, 0);
          break;
        case wnav_eItemType_AttrArrayElem: 
	  ((WItemAttrArrayElem *)item)->close( 0, 0);
          break;
        case wnav_eItemType_AttrArray: 
	  ((WItemAttrArray *)item)->close( 0, 0);
          break;
        case wnav_eItemType_AttrArrayOutput: 
	  ((WItemAttrArrayOutput *)item)->close( 0, 0);
          break;
        case wnav_eItemType_AttrObject: 
	  ((WItemAttrObject *)item)->close( 0, 0);
          break;
        default:
          ;
      }
      brow_SelectClear( wattnav->brow->ctx);
      brow_SetInverse( object, 1);
      brow_SelectInsert( wattnav->brow->ctx, object);
      if ( !brow_IsVisible( wattnav->brow->ctx, object, flow_eVisible_Full))
        brow_CenterObject( wattnav->brow->ctx, object, 0.25);
      free( node_list);
      break;
    }
    case flow_eEvent_Key_Right:
    {
      brow_tNode	*node_list;
      int		node_count;
      int		sts;

      brow_GetSelectedNodes( wattnav->brow->ctx, &node_list, &node_count);
      if ( !node_count)
        return 1;

      brow_GetUserData( node_list[0], (void **)&item);
      switch( item->type)
      {
        case wnav_eItemType_Attr:
	  sts = ((WItemAttr *)item)->open_children( 0, 0);
	  if ( ODD(sts))
            break;

          if ( wattnav->advanced_user && wattnav->change_value_cb)
            (wattnav->change_value_cb)( wattnav->parent_ctx);
          break;
        case wnav_eItemType_AttrInput:
        case wnav_eItemType_AttrInputF:
        case wnav_eItemType_AttrInputInv:
        case wnav_eItemType_AttrOutput:
          if ( wattnav->advanced_user && wattnav->change_value_cb)
            (wattnav->change_value_cb)( wattnav->parent_ctx);
          break;
        case wnav_eItemType_AttrArrayElem:
	  sts = ((WItemAttrArrayElem *)item)->open_children( 0, 0);
	  if ( ODD(sts))
            break;

          if ( wattnav->advanced_user && wattnav->change_value_cb)
            (wattnav->change_value_cb)( wattnav->parent_ctx);
          break;
        case wnav_eItemType_ObjectName:
        case wnav_eItemType_DocBlock:
          if ( wattnav->advanced_user && wattnav->change_value_cb)
            (wattnav->change_value_cb)( wattnav->parent_ctx);
          break;
        case wnav_eItemType_AttrArray: 
	  ((WItemAttrArray *)item)->open_attributes( 0, 0);
          break;
        case wnav_eItemType_AttrArrayOutput: 
	  ((WItemAttrArrayOutput *)item)->open_attributes( 0, 0);
          break;
        case wnav_eItemType_AttrObject: 
	  ((WItemAttrObject *)item)->open_attributes( 0, 0);
          break;
        case wnav_eItemType_Enum: 
        {
          int value;

          if ( !wattnav->advanced_user)
            break;
          brow_GetRadiobutton( node_list[0], 0, &value);
          if ( !value)
	    ((WItemEnum *)item)->set();
          break;
        }
        case wnav_eItemType_Mask: 
        {
          int value;

          if ( !wattnav->advanced_user)
            break;
          brow_GetRadiobutton( node_list[0], 0, &value);
	  ((WItemMask *)item)->set( !value);
          break;
        }
        default:
          ;
      }
      free( node_list);
      break;
    }
    case flow_eEvent_Key_ShiftRight:
    {
      brow_tNode	*node_list;
      int		node_count;
      int value;

      if ( wattnav->utility != wb_eUtility_PlcEditor)
        return 1;

      brow_GetSelectedNodes( wattnav->brow->ctx, &node_list, &node_count);
      if ( !node_count)
        return 1;

      brow_GetUserData( node_list[0], (void **)&item);
      switch( item->type)
      {
        case wnav_eItemType_AttrArray: 
	  ((WItemAttrArray *)item)->open_attributes( 0, 0);
          break;
        case wnav_eItemType_AttrInput:
        {
          brow_GetRadiobutton( node_list[0], 0, &value);
	  ((WItemAttrInput *)item)->set_mask( 0, !value);
          break;
        }
        case wnav_eItemType_AttrInputF:
        {
          brow_GetRadiobutton( node_list[0], 0, &value);
	  ((WItemAttrInputF *)item)->set_mask( 0, !value);
          break;
        }
        case wnav_eItemType_AttrOutput:
        {
          brow_GetRadiobutton( node_list[0], 0, &value);
	  ((WItemAttrOutput *)item)->set_mask( 0, !value);
          break;
        }
        case wnav_eItemType_AttrArrayOutput:
        {
          brow_GetRadiobutton( node_list[0], 0, &value);
	  ((WItemAttrArrayOutput *)item)->set_mask( 0, !value);
          break;
        }
        default:
          ;
      }
      break;
    }
    case flow_eEvent_Key_ShiftLeft:
    {
      brow_tNode	*node_list;
      int		node_count;

      if ( wattnav->utility != wb_eUtility_PlcEditor)
        return 1;

      brow_GetSelectedNodes( wattnav->brow->ctx, &node_list, &node_count);
      if ( !node_count)
        return 1;

      brow_GetUserData( node_list[0], (void **)&item);
      switch( item->type)
      {
        case wnav_eItemType_AttrInput:
        {
          int value;

          brow_GetRadiobutton( node_list[0], 1, &value);
	  ((WItemAttrInput *)item)->set_mask( 1, !value);
          break;
        }
        case wnav_eItemType_AttrInputInv:
        {
          int value;

          brow_GetRadiobutton( node_list[0], 0, &value);
	  ((WItemAttrInputInv *)item)->set_mask( 0, !value);
          break;
        }
        default:
          ;
      }
      break;
    }
    case flow_eEvent_MB1DoubleClick:
      switch ( event->object.object_type)
      {
        case flow_eObjectType_Node:
          brow_GetUserData( event->object.object, (void **)&item);
          switch( item->type)
          {
            case wnav_eItemType_Attr:
	      ((WItemAttr *)item)->open_children(
			event->object.x, event->object.y);
              break;
            case wnav_eItemType_AttrArray:
	      ((WItemAttrArray *)item)->open_attributes(
			event->object.x, event->object.y);
              break;
            case wnav_eItemType_AttrArrayOutput:
	      ((WItemAttrArrayOutput *)item)->open_attributes(
			event->object.x, event->object.y);
              break;
            case wnav_eItemType_AttrObject:
	      ((WItemAttrObject *)item)->open_attributes(
			event->object.x, event->object.y);
              break;
            case wnav_eItemType_AttrArrayElem:
	      ((WItemAttrArrayElem *)item)->open_children(
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
    case flow_eEvent_MB1DoubleClickCtrl:
      switch ( event->object.object_type)
      {
        case flow_eObjectType_Node:
          brow_GetUserData( event->object.object, (void **)&item);
          switch( item->type) {
	  case wnav_eItemType_Attr: 
	  case wnav_eItemType_AttrArrayElem: {
	    WItemBaseAttr 	*item_attr = (WItemBaseAttr *)item;
	    char 		str[200];
	    int 		sts;

	    if ( item_attr->type_id == pwr_eType_Objid) {
	      sts = wattnav->get_selection( str, sizeof(str));
	    }
	    if ( ODD(sts))
	      wattnav->set_attr_value( item_attr->node, item_attr->attr, str);
	    break;
	  }
	  default:
	    ;
          }
          break;
      default:
	;
      }
      break;
    case flow_eEvent_Radiobutton:
    {
      switch ( event->object.object_type)
      {
        case flow_eObjectType_Node:
          brow_GetUserData( event->object.object, (void **)&item);
          switch( item->type)
          {
            case wnav_eItemType_AttrInput: 
              if ( wattnav->utility != wb_eUtility_PlcEditor)
                break;
	      ((WItemAttrInput *)item)->set_mask( event->radiobutton.number,
			!event->radiobutton.value);
              break;
            case wnav_eItemType_AttrInputInv: 
              if ( wattnav->utility != wb_eUtility_PlcEditor)
                break;
	      ((WItemAttrInputInv *)item)->set_mask( event->radiobutton.number,
			!event->radiobutton.value);
              break;
            case wnav_eItemType_AttrInputF: 
              if ( wattnav->utility != wb_eUtility_PlcEditor)
                break;
	      ((WItemAttrInputF *)item)->set_mask( event->radiobutton.number,
			!event->radiobutton.value);
              break;
            case wnav_eItemType_AttrOutput: 
              if ( wattnav->utility != wb_eUtility_PlcEditor)
                break;
	      ((WItemAttrOutput *)item)->set_mask( event->radiobutton.number,
			!event->radiobutton.value);
              break;
            case wnav_eItemType_AttrArrayOutput: 
              if ( wattnav->utility != wb_eUtility_PlcEditor)
                break;
	      ((WItemAttrArrayOutput *)item)->set_mask( event->radiobutton.number,
			!event->radiobutton.value);
              break;
            case wnav_eItemType_Enum: 
              if ( !event->radiobutton.value)
	        ((WItemEnum *)item)->set();
              break;
            case wnav_eItemType_Mask: 
	      ((WItemMask *)item)->set( !event->radiobutton.value);
              break;
            default:
              ;
          }
          break;
        default:
          ;
      }


      break;
    }
    case flow_eEvent_Map:
    {
      wattnav->displayed = 1;
      break;
    }

    default:
      ;
  }
  return 1;
}

int	WAttNav::object_attr()
{
  int	i, j;
  char	body[20];
  pwr_tClassId classid;
  ldh_sParDef 	*bodydef;
  int sts;
  int rows;
  char parname[40];
  int attr_exist = 0;
  int input_cnt = 0;
  int output_cnt = 0;
  int elements;
  char *block;
  int size;
  pwr_tObjid objid = aref.Objid;
  char name[80];
  char *name_p;
  char *s;

  brow_SetNodraw( brow->ctx);

  sts = ldh_GetAttrRefTid(ldhses, &aref, &classid);
  if ( EVEN(sts)) return sts;

  if ( aref.Flags.b.ObjectAttr) {
    sts = ldh_AttrRefToName( ldhses, &aref, ldh_eName_Aref, &name_p, &size);
    if ( EVEN(sts)) return sts;

    if ( (s = strchr( name_p, '.')))
      strcpy( name, s+1);
    else
      strcpy( name, "");
  }

  // Display object name
  if ( editmode && display_objectname && aref.Flags.b.Object) {
    new WItemObjectName( brow, ldhses, aref.Objid, NULL, flow_eDest_IntoLast);
    attr_exist = 1;
  }

  for ( i = 0; i < 3; i++) {
    if ( i == 0)
      strcpy( body, "RtBody");
    else if ( i == 1)
      strcpy( body, "DevBody");
    else
      strcpy( body, "SysBody");

    sts = ldh_GetObjectBodyDef( ldhses, classid, body, 1,
		&bodydef, &rows);
    if ( EVEN(sts))
      continue;
    for ( j = 0; j < rows; j++) {
      if ( aref.Flags.b.Object)
	strcpy( parname, bodydef[j].ParName);
      else {
	strcpy( parname, name);
	strcat( parname, ".");
	strcat( parname, bodydef[j].ParName);
      }

      if ( bodydef[j].Flags & ldh_mParDef_Shadowed)
	continue;
      if ( bodydef[j].Par->Output.Info.Type == pwr_eType_Buffer)
        continue;

      if ( bodydef[j].Par->Output.Info.Flags & PWR_MASK_ARRAY ) {
       if ( bodydef[j].Par->Output.Info.Flags & PWR_MASK_INVISIBLE )
          continue;
       if ( bodydef[j].Par->Output.Info.Flags & PWR_MASK_DISABLEATTR && 
	    j > 0) {
	 pwr_tDisableAttr disabled;
	 pwr_sAttrRef aar;
	 pwr_sAttrRef ar = cdh_ObjidToAref( objid);
	 
	 sts = ldh_ArefANameToAref( ldhses, &ar, parname, &aar);
	 if ( EVEN(sts)) return sts;

	 sts = ldh_AttributeDisabled( ldhses, &ar, &disabled);
	 if ( EVEN(sts)) return sts;

	 if ( disabled)
	   continue;
        }
       
        elements = bodydef[j].Par->Output.Info.Elements;

        if ( bodydef[j].ParClass == pwr_eClass_Output &&
	     !(bodydef[j].Par->Input.Info.Flags & PWR_MASK_NOREMOVE)) {
          new WItemAttrArrayOutput( brow, ldhses, objid, NULL, 
		flow_eDest_IntoLast,
		parname,
		bodydef[j].Par->Output.Info.Elements, 
		bodydef[j].Par->Output.Info.Type, 
		bodydef[j].Par->Output.TypeRef, 
		bodydef[j].Par->Output.Info.Size, 
		bodydef[j].Par->Output.Info.Flags,
		body, output_cnt);
          output_cnt++;
        }
        else
          new WItemAttrArray( brow, ldhses, objid, NULL, 
		flow_eDest_IntoLast,
		parname,
		bodydef[j].Par->Output.Info.Elements, 
		bodydef[j].Par->Output.Info.Type, 
		bodydef[j].Par->Output.TypeRef, 
		bodydef[j].Par->Output.Info.Size, 
		bodydef[j].Par->Output.Info.Flags,
		body, 0);
        attr_exist = 1;
      }
      else if ( bodydef[j].ParClass == pwr_eClass_Input) {
        if ( bodydef[j].Par->Output.Info.Flags & PWR_MASK_INVISIBLE ) {
          input_cnt++;
          continue;
        }
        if ( bodydef[j].Par->Input.Info.Type == pwr_eType_Boolean) {
	  if ( bodydef[j].Par->Input.Info.Flags & PWR_MASK_NOREMOVE &&
	       bodydef[j].Par->Input.Info.Flags & PWR_MASK_NOINVERT)
            new WItemAttr( brow, ldhses, objid, NULL, 
		flow_eDest_IntoLast, parname,
		bodydef[j].Par->Input.Info.Type, 
		bodydef[j].Par->Input.TypeRef, 
		bodydef[j].Par->Input.Info.Size,
		bodydef[j].Par->Input.Info.Flags,
		body, 0);
	  else if ( bodydef[j].Par->Input.Info.Flags & PWR_MASK_NOREMOVE)
            new WItemAttrInputInv( brow, ldhses, objid, NULL, 
		flow_eDest_IntoLast, parname,
		bodydef[j].Par->Input.Info.Type, 
		bodydef[j].Par->Input.TypeRef, 
		bodydef[j].Par->Input.Info.Size,
		bodydef[j].Par->Input.Info.Flags,
		body, input_cnt);
	  else if ( bodydef[j].Par->Input.Info.Flags & PWR_MASK_NOINVERT)
            new WItemAttrInputF( brow, ldhses, objid, NULL, 
		flow_eDest_IntoLast, parname,
		bodydef[j].Par->Input.Info.Type, 
		bodydef[j].Par->Input.TypeRef, 
		bodydef[j].Par->Input.Info.Size,
		bodydef[j].Par->Input.Info.Flags,
		body, input_cnt);
	  else
            new WItemAttrInput( brow, ldhses, objid, NULL,
		flow_eDest_IntoLast, parname,
		bodydef[j].Par->Input.Info.Type, 
		bodydef[j].Par->Input.TypeRef, 
		bodydef[j].Par->Input.Info.Size,
		bodydef[j].Par->Input.Info.Flags,
		body, input_cnt);
        }
        else {
	  if ( bodydef[j].Par->Input.Info.Flags & PWR_MASK_NOREMOVE)
            new WItemAttr( brow, ldhses, objid, NULL,
		flow_eDest_IntoLast, parname,
		bodydef[j].Par->Input.Info.Type, 
		bodydef[j].Par->Input.TypeRef, 
		bodydef[j].Par->Input.Info.Size,
		bodydef[j].Par->Input.Info.Flags,
		body, 0);
	  else
            new WItemAttrInputF( brow, ldhses, objid, NULL,
		flow_eDest_IntoLast, parname,
		bodydef[j].Par->Input.Info.Type, 
		bodydef[j].Par->Input.TypeRef, 
		bodydef[j].Par->Input.Info.Size,
		bodydef[j].Par->Input.Info.Flags,
		body, input_cnt);
        }
        attr_exist = 1;
        input_cnt++;
      }
      else if ( bodydef[j].ParClass == pwr_eClass_Output) {
        if ( bodydef[j].Par->Output.Info.Flags & PWR_MASK_INVISIBLE ) {
          output_cnt++;
          continue;
        }

	if ( bodydef[j].Par->Output.Info.Flags & PWR_MASK_NOREMOVE)
          new WItemAttr( brow, ldhses, objid, NULL, 
		flow_eDest_IntoLast, parname,
		bodydef[j].Par->Output.Info.Type, 
		bodydef[j].Par->Output.TypeRef, 
		bodydef[j].Par->Output.Info.Size,
		bodydef[j].Par->Output.Info.Flags,
		body, 0);
        else
          new WItemAttrOutput( brow, ldhses, objid, NULL, 
		flow_eDest_IntoLast, parname,
		bodydef[j].Par->Output.Info.Type, 
		bodydef[j].Par->Output.TypeRef, 
		bodydef[j].Par->Output.Info.Size,
		bodydef[j].Par->Output.Info.Flags,
		body, output_cnt);
        attr_exist = 1;
        output_cnt++;
      }
      else {
	if ( bodydef[j].Par->Output.Info.Flags & PWR_MASK_INVISIBLE )
          continue;

        new WItemAttr( brow, ldhses, objid, NULL, 
		flow_eDest_IntoLast, parname,
		bodydef[j].Par->Output.Info.Type, 
		bodydef[j].Par->Output.TypeRef, 
		bodydef[j].Par->Output.Info.Size,
		bodydef[j].Par->Output.Info.Flags,
		body, 0);
        attr_exist = 1;
      }
    }
    free((char *) bodydef);	

    if ( aref.Flags.b.Object) {
      sts = ldh_GetDocBlock( ldhses, objid, &block, &size);
      if ( ODD(sts)) {
	new WItemDocBlock( brow, ldhses, objid, block, size,
			   NULL, flow_eDest_IntoLast);
	attr_exist = 1;
      }
    }
  }
  brow_ResetNodraw( brow->ctx);
  brow_Redraw( brow->ctx, 0);
  return WATT__SUCCESS;
}

void WAttNav::enable_events()
{
  brow_EnableEvent( brow->ctx, flow_eEvent_MB1Click, flow_eEventType_CallBack, 
	brow_cb);
  brow_EnableEvent( brow->ctx, flow_eEvent_MB1DoubleClick, flow_eEventType_CallBack, 
	brow_cb);
  brow_EnableEvent( brow->ctx, flow_eEvent_MB1DoubleClickCtrl, flow_eEventType_CallBack, 
	brow_cb);
  brow_EnableEvent( brow->ctx, flow_eEvent_SelectClear, flow_eEventType_CallBack, 
	brow_cb);
  brow_EnableEvent( brow->ctx, flow_eEvent_ObjectDeleted, flow_eEventType_CallBack, 
	brow_cb);
  brow_EnableEvent( brow->ctx, flow_eEvent_Key_Up, flow_eEventType_CallBack, 
	brow_cb);
  brow_EnableEvent( brow->ctx, flow_eEvent_Key_Down, flow_eEventType_CallBack, 
	brow_cb);
  brow_EnableEvent( brow->ctx, flow_eEvent_Key_Right, flow_eEventType_CallBack, 
	brow_cb);
  brow_EnableEvent( brow->ctx, flow_eEvent_Key_ShiftRight, flow_eEventType_CallBack, 
	brow_cb);
  brow_EnableEvent( brow->ctx, flow_eEvent_Key_Left, flow_eEventType_CallBack, 
	brow_cb);
  brow_EnableEvent( brow->ctx, flow_eEvent_Key_ShiftLeft, flow_eEventType_CallBack, 
	brow_cb);
  brow_EnableEvent( brow->ctx, flow_eEvent_Key_PF3, flow_eEventType_CallBack, 
	brow_cb);
  brow_EnableEvent( brow->ctx, flow_eEvent_Radiobutton, flow_eEventType_CallBack, 
	brow_cb);
  brow_EnableEvent( brow->ctx, flow_eEvent_Map, flow_eEventType_CallBack, 
	brow_cb);
  brow_EnableEvent( brow->ctx, flow_eEvent_Key_PageUp, flow_eEventType_CallBack, 
	brow_cb);
  brow_EnableEvent( brow->ctx, flow_eEvent_Key_PageDown, flow_eEventType_CallBack, 
	brow_cb);
  brow_EnableEvent( brow->ctx, flow_eEvent_ScrollUp, flow_eEventType_CallBack, 
	brow_cb);
  brow_EnableEvent( brow->ctx, flow_eEvent_ScrollDown, flow_eEventType_CallBack, 
	brow_cb);
}

//
// Backcall routine called at creation of the brow widget
// Enable event, create nodeclasses and insert the root objects.
//
int WAttNav::init_brow_cb( FlowCtx *fctx, void *client_data)
{
  WAttNav *wattnav = (WAttNav *) client_data;
  BrowCtx *ctx = (BrowCtx *)fctx;

  wattnav->brow = new WNavBrow( ctx, (void *)wattnav);

  wattnav->brow->ldh_cb_used = 0;
  wattnav->brow->brow_setup();
  wattnav->brow->create_nodeclasses();
  wattnav->enable_events();

  // Create the root item
  wattnav->object_attr();

  return 1;
}

int WAttNav::object_exist( brow_tObject object)
{
  brow_tObject 	*object_list;
  int		object_cnt;
  int		i;

  brow_GetObjectList( brow->ctx, &object_list, &object_cnt);
  for ( i = 0; i < object_cnt; i++)
  {
    if ( object_list[i] == object)
      return 1;
  }
  return 0;
}

//
// Set attribute value
//
int WAttNav::set_attr_value( brow_tObject node, char *name, char *value_str)
{
  WItem		*base_item;
  int		sts;
  char 		buff[1024];
  int		size;

  // Check that object still exist
  if ( !object_exist( node))
    return WATT__DISAPPEARD;

  brow_GetUserData( node, (void **)&base_item);

  switch( base_item->type)
  {
    case wnav_eItemType_Attr:
    case wnav_eItemType_AttrInput:
    case wnav_eItemType_AttrInputF:
    case wnav_eItemType_AttrInputInv:
    case wnav_eItemType_AttrOutput:
    {
      WItemBaseAttr *item = (WItemBaseAttr *)base_item;

      // Check that objid is still the same
      if ( strcmp(item->attr, name) != 0)
        return WATT__DISAPPEARD;

      sts =  wnav_attr_string_to_value( ldhses, item->type_id, 
	value_str, buff, sizeof(buff), item->size);
      if ( EVEN(sts))
        message( 'E', "Input syntax error");
      else
      {
        sts = ldh_SetObjectPar( ldhses, item->objid, item->body,
		item->attr, buff, item->size);
        switch( base_item->type) {
          case wnav_eItemType_Attr:
            ((WItemAttr *)item)->update();
            break;
          case wnav_eItemType_AttrInput:
            ((WItemAttrInput *)item)->update();
            break;
          case wnav_eItemType_AttrInputF:
            ((WItemAttrInputF *)item)->update();
            break;
          case wnav_eItemType_AttrInputInv:
            ((WItemAttrInputInv *)item)->update();
            break;
          case wnav_eItemType_AttrOutput:
            ((WItemAttrOutput *)item)->update();
            break;
          default:
            ;
        }
      }
      return sts;
    }
    case wnav_eItemType_AttrArrayElem:
    {
      char *value;

      WItemAttrArrayElem *item = (WItemAttrArrayElem *)base_item;

      // Check that objid is still the same
      if ( strcmp( item->attr, name) != 0)
        return WATT__DISAPPEARD;

      sts =  wnav_attr_string_to_value( ldhses, item->type_id, 
	value_str, buff, sizeof(buff), item->size);
      if ( EVEN(sts))
        message( 'E', "Input syntax error");
      else
      {
        sts = ldh_GetObjectPar( ldhses, item->objid, item->body,
		item->attr, (char **)&value, &size); 
        if (EVEN(sts)) return sts;

        memcpy( value + item->element * item->size, buff, item->size);
        sts = ldh_SetObjectPar( ldhses, item->objid, item->body,
		item->attr, value, size);
        free( (char *)value);

        item->update();
      }
      return sts;
    }
    case wnav_eItemType_ObjectName:
    {
      WItemObjectName *item = (WItemObjectName *)base_item;

      // Check that objid is still the same
      if ( ! cdh_ObjidIsEqual( aref.Objid, base_item->objid))
        return WATT__DISAPPEARD;

      sts = ldh_ChangeObjectName( ldhses, item->objid, value_str);
      if ( EVEN(sts)) return sts;
      item->update();
      return sts;
    }
    case wnav_eItemType_DocBlock:
    {
      WItemDocBlock *item = (WItemDocBlock *)base_item;

      // Check that objid is still the same
      if ( ! cdh_ObjidIsEqual( aref.Objid, base_item->objid))
        return WATT__DISAPPEARD;

      sts = ldh_SetDocBlock( ldhses, item->objid, value_str);
      if ( EVEN(sts)) return sts;
      item->update();
      return sts;
    }
    default:
      ;
  }
  return 1;
}

void WAttNav::redraw()
{
    brow_Redraw( brow->ctx, 0);
}

int WAttNav::select_by_name( char *name)
{
  WItem		*base_item;
  brow_tObject 	*object_list;
  int		object_cnt;
  int		i;
  int           found;
  brow_tObject  object;

  brow_GetObjectList( brow->ctx, &object_list, &object_cnt);
  found = 0;
  for ( i = 0; i < object_cnt; i++)
  {
    brow_GetUserData( object_list[i], (void **)&base_item);

    switch( base_item->type) {
      case wnav_eItemType_Attr:
      case wnav_eItemType_AttrInput:
      case wnav_eItemType_AttrInputF:
      case wnav_eItemType_AttrInputInv:
      case wnav_eItemType_AttrOutput:
      case wnav_eItemType_AttrArrayElem:
      {
        WItemBaseAttr *item = (WItemBaseAttr *)base_item;

	if ( strcmp( name, item->attr) == 0) {
          object = object_list[i];
          found = 1;
        }
        break;
      }
      default:
        ;
    }
    if ( found)
      break;
  }
  if ( !found)
    return WATT__ATTRNOTFOUND;

  brow_SelectClear( brow->ctx);
  brow_SetInverse( object, 1);
  brow_SelectInsert( brow->ctx, object);
  return WATT__SUCCESS;
}








