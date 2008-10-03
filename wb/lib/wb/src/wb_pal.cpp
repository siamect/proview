/* 
 * Proview   $Id: wb_pal.cpp,v 1.12 2008-10-03 14:18:37 claes Exp $
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

/* wb_pal.cpp -- Palette of configurator or plc-editor */

#if defined OS_VMS && defined __ALPHA
# pragma message disable (NOSIMPINT,EXTROUENCUNNOBJ)
#endif

#if defined OS_VMS && !defined __ALPHA
# pragma message disable (LONGEXTERN)
#endif

#include <stdio.h>
#include <stdlib.h>

#include "wb_ldh.h"
#include "co_cdh.h"
#include "co_dcli.h"
#include "pwr_baseclasses.h"

#include "flow.h"
#include "flow_browctx.h"
#include "flow_browapi.h"
#include "wb_pal.h"

#include "glow.h"
#include "glow_growctx.h"

#include "xnav_bitmap_leaf8.h"
#include "xnav_bitmap_leaf10.h"
#include "xnav_bitmap_leaf12.h"
#include "xnav_bitmap_leaf14.h"
#include "xnav_bitmap_leaf16.h"
#include "xnav_bitmap_leaf18.h"
#include "xnav_bitmap_leaf20.h"
#include "xnav_bitmap_leaf24.h"
#include "xnav_bitmap_map8.h"
#include "xnav_bitmap_map10.h"
#include "xnav_bitmap_map12.h"
#include "xnav_bitmap_map14.h"
#include "xnav_bitmap_map16.h"
#include "xnav_bitmap_map18.h"
#include "xnav_bitmap_map20.h"
#include "xnav_bitmap_map24.h"
#include "xnav_bitmap_openmap8.h"
#include "xnav_bitmap_openmap10.h"
#include "xnav_bitmap_openmap12.h"
#include "xnav_bitmap_openmap14.h"
#include "xnav_bitmap_openmap16.h"
#include "xnav_bitmap_openmap18.h"
#include "xnav_bitmap_openmap20.h"
#include "xnav_bitmap_openmap24.h"
#include "xnav_bitmap_ref8.h"
#include "xnav_bitmap_ref10.h"
#include "xnav_bitmap_ref12.h"
#include "xnav_bitmap_ref14.h"
#include "xnav_bitmap_ref16.h"
#include "xnav_bitmap_ref18.h"
#include "xnav_bitmap_ref20.h"
#include "xnav_bitmap_ref24.h"
#include "xnav_bitmap_crossref8.h"
#include "xnav_bitmap_crossref10.h"
#include "xnav_bitmap_crossref12.h"
#include "xnav_bitmap_crossref14.h"
#include "xnav_bitmap_crossref16.h"
#include "xnav_bitmap_crossref18.h"
#include "xnav_bitmap_crossref20.h"
#include "xnav_bitmap_crossref24.h"
#include "xnav_bitmap_aarithm12.h"
#include "xnav_bitmap_arithm12.h"
#include "xnav_bitmap_bodytext12.h"
#include "xnav_bitmap_document12.h"
#include "xnav_bitmap_frame12.h"
#include "xnav_bitmap_get12.h"
#include "xnav_bitmap_getp12.h"
#include "xnav_bitmap_head12.h"
#include "xnav_bitmap_initstep12.h"
#include "xnav_bitmap_logic1_12.h"
#include "xnav_bitmap_logic2_12.h"
#include "xnav_bitmap_wait12.h"
#include "xnav_bitmap_order12.h"
#include "xnav_bitmap_orderact12.h"
#include "xnav_bitmap_setcond12.h"
#include "xnav_bitmap_ssbegin12.h"
#include "xnav_bitmap_ssend12.h"
#include "xnav_bitmap_step12.h"
#include "xnav_bitmap_sto12.h"
#include "xnav_bitmap_stop12.h"
#include "xnav_bitmap_substep12.h"
#include "xnav_bitmap_text12.h"
#include "xnav_bitmap_title12.h"
#include "xnav_bitmap_trans12.h"
#include "xnav_bitmap_cell12.h"
#include "xnav_bitmap_condigital12.h"
#include "xnav_bitmap_conanalog12.h"
#include "xnav_bitmap_confeedbackdigital12.h"
#include "xnav_bitmap_confeedbackanalog12.h"
#include "xnav_bitmap_conexecuteorder12.h"
#include "xnav_bitmap_congrafcet12.h"
#include "xnav_bitmap_stepconv12.h"
#include "xnav_bitmap_stepdiv12.h"
#include "xnav_bitmap_transconv12.h"
#include "xnav_bitmap_transdiv12.h"

//
// Member functions for PalItem classes
//
PalItemClassVolume::PalItemClassVolume( Pal *pal, char *item_name,
	brow_tNode dest, flow_eDest dest_code, int item_is_root) :
	PalItem( pwr_cNObjid, item_is_root)
{

  type = pal_ePalItemType_ClassVolume;
  strcpy( name, item_name);
  if ( !is_root)
  {
    brow_CreateNode( pal->brow_ctx, name, pal->nc, 
		dest, dest_code, NULL, 1, &node);

    brow_SetAnnotPixmap( node, 0, pal->pixmap_map);
    brow_SetAnnotation( node, 0, name, sizeof(name));
    brow_SetUserData( node, (void *)this);
  }
}

int PalItemClassVolume::open( Pal *pal, double x, double y)
{
  double	node_x, node_y;
  pwr_tObjid	child;
  int		child_exist;
  int		sts, size;
  int		found;
  pwr_tObjid	ref_objid;
  pwr_tVolumeId	volume;
  pwr_tClassId	classid;
  char		volume_name[80];	

  if ( !is_root)
    brow_GetNodePosition( node, &node_x, &node_y);
  else
    node_x = node_y = 0;

  if ( !is_root && brow_IsOpen( node))
  {
    // Close
    brow_SetNodraw( pal->brow_ctx);
    brow_CloseNode( pal->brow_ctx, node);
    brow_ResetOpen( node, 1);
    brow_SetAnnotPixmap( node, 0, pal->pixmap_map);
    brow_ResetNodraw( pal->brow_ctx);
    brow_Redraw( pal->brow_ctx, node_y);
  }
  else
  {
    PalItem *item;

    // Find a class volume with this name
    found = 0;
    sts = ldh_GetVolumeList( pal->wbctx, &volume);
    while ( ODD(sts))
    {
      sts = ldh_GetVolumeClass( pal->wbctx, volume, &classid);
      if ( EVEN(sts)) return sts;

      if ( classid == pwr_eClass_ClassVolume)
      {
        sts = ldh_VolumeIdToName( pal->wbctx,
		volume, volume_name, sizeof(volume_name), &size);
        if ( EVEN(sts)) return sts;

        if ( !cdh_NoCaseStrcmp( volume_name, name)) 
        {
          strcat( volume_name, ":Class");
          sts = ldh_NameToObjid( pal->ldhses, &ref_objid, volume_name);
          if ( EVEN(sts)) return sts;
          found = 1;
          break;
        }
      }
      sts = ldh_GetNextVolume( pal->wbctx, volume, &volume);
    }
    if ( !found)
      ref_objid = objid;

    brow_SetNodraw( pal->brow_ctx);

    child_exist = 0;
    sts = ldh_GetChild( pal->ldhses, ref_objid, &child);
    while ( ODD(sts))
    {
      child_exist = 1;
      sts = pal->create_item( child, node, flow_eDest_IntoLast,
		(void **) &item, 0);
      sts = ldh_GetNextSibling( pal->ldhses, child, &child);
    }

    if ( child_exist && !is_root)
    {
      brow_SetOpen( node, 1);
      brow_SetAnnotPixmap( node, 0, pal->pixmap_openmap);
    }
    brow_ResetNodraw( pal->brow_ctx);
    if ( child_exist)
      brow_Redraw( pal->brow_ctx, node_y);
  }
  return 1;
}

PalItemClass::PalItemClass( Pal *pal, char *item_name, 
	brow_tNode dest, flow_eDest dest_code, int item_is_root) :
	PalItem( pwr_cNObjid, item_is_root)
{
  int sts;
  int size;
  flow_sAnnotPixmap *pixmap;
  pwr_sGraphPlcNode *graphbody;
  pwr_tClassId	bodyclass;
  pwr_tClassId	classid;

  strcpy( name, item_name);
  type = pal_ePalItemType_Class;
  if ( !is_root)
  {
    brow_CreateNode( pal->brow_ctx, name, pal->nc, 
		dest, dest_code, NULL, 1, &node);

    brow_SetAnnotPixmap( node, 0, pal->pixmap_leaf);
    pixmap = NULL;
    sts = ldh_ClassNameToId( pal->ldhses, &classid, name);
    if ( ODD(sts))
    {
      switch( classid)
      {
        case pwr_cClass_ConDigital: 	pixmap = pal->pixmap_condigital; break;
        case pwr_cClass_ConAnalog: 	pixmap = pal->pixmap_conanalog; break;
        case pwr_cClass_ConFeedbackDigital: pixmap = pal->pixmap_confeedbackdigital; break;
        case pwr_cClass_ConFeedbackAnalog: pixmap = pal->pixmap_confeedbackanalog; break;
        case pwr_cClass_ConExecuteOrder: pixmap = pal->pixmap_conexecuteorder; break;
        case pwr_cClass_ConGrafcet: 	pixmap = pal->pixmap_congrafcet; break;
        case pwr_cClass_StepConv: 	pixmap = pal->pixmap_stepconv; break;
        case pwr_cClass_StepDiv: 	pixmap = pal->pixmap_stepdiv; break;
        case pwr_cClass_TransConv: 	pixmap = pal->pixmap_transconv; break;
        case pwr_cClass_TransDiv: 	pixmap = pal->pixmap_transdiv; break;
        case pwr_cClass_initstep: 	pixmap = pal->pixmap_initstep; break;
        case pwr_cClass_order: 	pixmap = pal->pixmap_order; break;
        case pwr_cClass_OrderAct: 	pixmap = pal->pixmap_orderact; break;
        case pwr_cClass_setcond: 	pixmap = pal->pixmap_setcond; break;
        case pwr_cClass_ssbegin: 	pixmap = pal->pixmap_ssbegin; break;
        case pwr_cClass_ssend: 	pixmap = pal->pixmap_ssend; break;
        case pwr_cClass_step: 	pixmap = pal->pixmap_step; break;
        case pwr_cClass_substep: 	pixmap = pal->pixmap_substep; break;
        case pwr_cClass_trans: 	pixmap = pal->pixmap_trans; break;
        case pwr_cClass_inv:
        case pwr_cClass_edge: 	
	  pixmap = pal->pixmap_logic1_; 
	  break;
        case pwr_cClass_wait:
        case pwr_cClass_pulse:
        case pwr_cClass_timer:
	  pixmap = pal->pixmap_wait; 
	  break;
        case pwr_cClass_GetDv:
        case pwr_cClass_GetDi:
        case pwr_cClass_GetDo:
        case pwr_cClass_GetAv:
        case pwr_cClass_GetAi:
        case pwr_cClass_GetAo:
        case pwr_cClass_GetSv:
        case pwr_cClass_GetIi:
        case pwr_cClass_GetIo:
        case pwr_cClass_GetIv:
        case pwr_cClass_GetATv:
        case pwr_cClass_GetDTv:
        case pwr_cClass_GetDattr:
        case pwr_cClass_GetAattr:
        case pwr_cClass_GetIattr:
        case pwr_cClass_GetSattr:
	  pixmap = pal->pixmap_get; 
	  break;
        case pwr_cClass_GetAp:
        case pwr_cClass_GetDp:
        case pwr_cClass_GetIpToA:
        case pwr_cClass_GetIp:
        case pwr_cClass_GetSp:
        case pwr_cClass_GetATp:
        case pwr_cClass_GetDTp:
        case pwr_cClass_GetData:
	  pixmap = pal->pixmap_get; 
	  break;
        case pwr_cClass_stoap:
        case pwr_cClass_stodp:
        case pwr_cClass_setdp:
        case pwr_cClass_resdp:
        case pwr_cClass_StoAtoIp:
        case pwr_cClass_StoIp:
        case pwr_cClass_cstoao:
        case pwr_cClass_cstoav:
        case pwr_cClass_cstoai:
        case pwr_cClass_CStoAtoIp:
        case pwr_cClass_CStoIp:
        case pwr_cClass_cstoio:
        case pwr_cClass_cstoii:
        case pwr_cClass_cstoiv:
        case pwr_cClass_cstoap:
        case pwr_cClass_cstosv:
        case pwr_cClass_cstosp:
        case pwr_cClass_stosp:
        case pwr_cClass_StoATp:
        case pwr_cClass_StoDTp:
          pixmap = pal->pixmap_sto; 
	  break;
        case pwr_cClass_CStoATv:
        case pwr_cClass_CStoDTv:
        case pwr_cClass_CStoATp:
        case pwr_cClass_CStoDTp:
        case pwr_cClass_CStoAattr:
        case pwr_cClass_CStoIattr:
        case pwr_cClass_CStoSattr:
          pixmap = pal->pixmap_stop; 
	  break;
        case pwr_cClass_Text:	 	pixmap = pal->pixmap_text; break;
        case pwr_cClass_BodyText:	pixmap = pal->pixmap_bodytext; break;
        case pwr_cClass_Head:		pixmap = pal->pixmap_head; break;
        case pwr_cClass_Title:	pixmap = pal->pixmap_title; break;
        case pwr_cClass_Frame:	pixmap = pal->pixmap_frame; break;
	default:
	  sts = ldh_GetClassBody( pal->ldhses, classid, "GraphPlcNode", 
		&bodyclass, (char **)&graphbody, &size);
	  if ( ODD(sts))
          {
            switch ( graphbody->graphmethod)
            {
              case 0:
	        pixmap = pal->pixmap_arithm; 
		break;
              case 1:
              case 10:
	        pixmap = pal->pixmap_logic2_;
		break;
              case 2:
	        pixmap = pal->pixmap_aarithm;
		break;
              case 3:
	        pixmap = pal->pixmap_orderact;
		break;
              case 5:
	        pixmap = pal->pixmap_order;
		break;
              case 6:
	        pixmap = pal->pixmap_document;
		break;
              case 7:
              case 9:
	        pixmap = pal->pixmap_sto;
		break;
              case 14:
	        pixmap = pal->pixmap_aarithm;
		break;
              case 15:
	        pixmap = pal->pixmap_sto;
                break;
              case 16:
                if ( graphbody->graphindex == 1)
	          pixmap = pal->pixmap_cell;
	        else
	          pixmap = pal->pixmap_arithm;
	        break;
            }
          }
      }
      if ( pixmap)
        brow_SetAnnotPixmap( node, 1, pixmap);
    }
    brow_SetAnnotation( node, 0, name, sizeof(name));
    brow_SetUserData( node, (void *)this);
  }
}

PalItemObject::PalItemObject( Pal *pal, pwr_tObjid item_objid, 
	brow_tNode dest, flow_eDest dest_code, int item_is_root) :
	PalItem( item_objid, item_is_root)
{
  int sts;
  char name[80];
  int size;

  type = pal_ePalItemType_Object;
  if ( !is_root)
  {
    sts = ldh_ObjidToName( pal->ldhses, objid, ldh_eName_Object, 
		name, sizeof(name), &size);

    brow_CreateNode( pal->brow_ctx, name, pal->nc, 
		dest, dest_code, NULL, 1, &node);

    brow_SetAnnotPixmap( node, 0, pal->pixmap_leaf);
    brow_SetAnnotation( node, 0, name, sizeof(name));
    brow_SetUserData( node, (void *)this);
  }
}

PalItemMenu::PalItemMenu( Pal *pal, char *item_name, 
	brow_tNode dest, flow_eDest dest_code, PalFileMenu **item_child_list,
	int item_is_root) :
	PalItem( pwr_cNObjid, item_is_root),
        child_list(item_child_list)
{
  type = pal_ePalItemType_Menu;
  strcpy( name, item_name);
  if ( !is_root)
  {
    brow_CreateNode( pal->brow_ctx, name, pal->nc,
		dest, dest_code, (void *)this, 1, &node);

    // Set pixmap
    if ( *child_list)
      brow_SetAnnotPixmap( node, 0, pal->pixmap_map);
    else
      brow_SetAnnotPixmap( node, 0, pal->pixmap_leaf);
    // Set object name annotation
    brow_SetAnnotation( node, 0, name, strlen(name));
  }
}

int PalItemMenu::open( Pal *pal, double x, double y)
{
  int		action_open;

  if ( !is_root)
  {
    if ( !brow_IsOpen( node))
      action_open = 1;
    else 
      action_open = 0;
  }
  if ( action_open || is_root)
  {
    // Display childlist
    double	node_x, node_y;
    PalItem 	*item;
    PalFileMenu	*menu;

    if ( !is_root)
      brow_GetNodePosition( node, &node_x, &node_y);
    else
      node_y = 0;
    brow_SetNodraw( pal->brow_ctx);
    menu = *child_list;
    while ( menu)
    {
      switch ( menu->item_type)
      {
        case pal_eMenuType_Class:
          item = (PalItem *) new PalItemClass( pal, menu->title, node,
		flow_eDest_IntoLast, 0);
          break;
        case pal_eMenuType_ClassVolume:
          item = (PalItem *) new PalItemClassVolume( pal, menu->title,
		node, flow_eDest_IntoLast, 0);
          break;
        case pal_eMenuType_Menu:
          item = (PalItem *) new PalItemMenu( pal, menu->title,
		node, flow_eDest_IntoLast, &menu->child_list, 0);
          break;
        default:
          ;
      }
      menu = menu->next;
      if ( !is_root)
      {
        brow_SetOpen( node, pal_mOpen_Children);
        brow_SetAnnotPixmap( node, 0, pal->pixmap_openmap);
      }
    }
    brow_ResetNodraw( pal->brow_ctx);
    brow_Redraw( pal->brow_ctx, node_y);
  }
  else
    close( pal, x, y);
  return 1;
}

int PalItemMenu::close( Pal *pal, double x, double y)
{
  double	node_x, node_y;

  if ( brow_IsOpen( node))
  {
    // Close
    brow_GetNodePosition( node, &node_x, &node_y);
    brow_SetNodraw( pal->brow_ctx);
    brow_CloseNode( pal->brow_ctx, node);
    brow_SetAnnotPixmap( node, 0, pal->pixmap_map);
    brow_ResetOpen( node, pal_mOpen_All);
    brow_ResetNodraw( pal->brow_ctx);
    brow_Redraw( pal->brow_ctx, node_y);
  }
  return 1;
}

//
//  Free pixmaps
//
void Pal::free_pixmaps()
{
  brow_FreeAnnotPixmap( brow_ctx, pixmap_leaf);
  brow_FreeAnnotPixmap( brow_ctx, pixmap_map);
  brow_FreeAnnotPixmap( brow_ctx, pixmap_openmap);
  brow_FreeAnnotPixmap( brow_ctx, pixmap_ref);
  brow_FreeAnnotPixmap( brow_ctx, pixmap_crossref);
  brow_FreeAnnotPixmap( brow_ctx, pixmap_aarithm);
  brow_FreeAnnotPixmap( brow_ctx, pixmap_arithm);
  brow_FreeAnnotPixmap( brow_ctx, pixmap_bodytext);
  brow_FreeAnnotPixmap( brow_ctx, pixmap_document);
  brow_FreeAnnotPixmap( brow_ctx, pixmap_get);
  brow_FreeAnnotPixmap( brow_ctx, pixmap_getp);
  brow_FreeAnnotPixmap( brow_ctx, pixmap_head);
  brow_FreeAnnotPixmap( brow_ctx, pixmap_initstep);
  brow_FreeAnnotPixmap( brow_ctx, pixmap_logic1_);
  brow_FreeAnnotPixmap( brow_ctx, pixmap_logic2_);
  brow_FreeAnnotPixmap( brow_ctx, pixmap_order);
  brow_FreeAnnotPixmap( brow_ctx, pixmap_orderact);
  brow_FreeAnnotPixmap( brow_ctx, pixmap_setcond);
  brow_FreeAnnotPixmap( brow_ctx, pixmap_ssbegin);
  brow_FreeAnnotPixmap( brow_ctx, pixmap_ssend);
  brow_FreeAnnotPixmap( brow_ctx, pixmap_step);
  brow_FreeAnnotPixmap( brow_ctx, pixmap_sto);
  brow_FreeAnnotPixmap( brow_ctx, pixmap_stop);
  brow_FreeAnnotPixmap( brow_ctx, pixmap_substep);
  brow_FreeAnnotPixmap( brow_ctx, pixmap_text);
  brow_FreeAnnotPixmap( brow_ctx, pixmap_title);
  brow_FreeAnnotPixmap( brow_ctx, pixmap_trans);
  brow_FreeAnnotPixmap( brow_ctx, pixmap_cell);
  brow_FreeAnnotPixmap( brow_ctx, pixmap_condigital);
  brow_FreeAnnotPixmap( brow_ctx, pixmap_conanalog);
  brow_FreeAnnotPixmap( brow_ctx, pixmap_confeedbackdigital);
  brow_FreeAnnotPixmap( brow_ctx, pixmap_confeedbackanalog);
  brow_FreeAnnotPixmap( brow_ctx, pixmap_conexecuteorder);
  brow_FreeAnnotPixmap( brow_ctx, pixmap_congrafcet);
  brow_FreeAnnotPixmap( brow_ctx, pixmap_stepconv);
  brow_FreeAnnotPixmap( brow_ctx, pixmap_stepdiv);
  brow_FreeAnnotPixmap( brow_ctx, pixmap_transconv);
  brow_FreeAnnotPixmap( brow_ctx, pixmap_transdiv);
  brow_FreeAnnotPixmap( brow_ctx, pixmap_frame);
  brow_FreeAnnotPixmap( brow_ctx, pixmap_wait);
}

void Pal::allocate_pixmaps()
{
	flow_sPixmapData pixmap_data;
	int i;
	
          i = 0;
	  pixmap_data[i].width =xnav_bitmap_leaf8_width;
	  pixmap_data[i].height =xnav_bitmap_leaf8_height;
	  pixmap_data[i++].bits =xnav_bitmap_leaf8_bits;
	  pixmap_data[i].width =xnav_bitmap_leaf10_width;
	  pixmap_data[i].height =xnav_bitmap_leaf10_height;
	  pixmap_data[i++].bits =xnav_bitmap_leaf10_bits;
	  pixmap_data[i].width =xnav_bitmap_leaf12_width;
	  pixmap_data[i].height =xnav_bitmap_leaf12_height;
	  pixmap_data[i++].bits =xnav_bitmap_leaf12_bits;
	  pixmap_data[i].width =xnav_bitmap_leaf14_width;
	  pixmap_data[i].height =xnav_bitmap_leaf14_height;
	  pixmap_data[i++].bits =xnav_bitmap_leaf14_bits;
	  pixmap_data[i].width =xnav_bitmap_leaf16_width;
	  pixmap_data[i].height =xnav_bitmap_leaf16_height;
	  pixmap_data[i++].bits =xnav_bitmap_leaf16_bits;
	  pixmap_data[i].width =xnav_bitmap_leaf18_width;
	  pixmap_data[i].height =xnav_bitmap_leaf18_height;
	  pixmap_data[i++].bits =xnav_bitmap_leaf18_bits;
	  pixmap_data[i].width =xnav_bitmap_leaf20_width;
	  pixmap_data[i].height =xnav_bitmap_leaf20_height;
	  pixmap_data[i++].bits =xnav_bitmap_leaf20_bits;
	  pixmap_data[i].width =xnav_bitmap_leaf20_width;
	  pixmap_data[i].height =xnav_bitmap_leaf20_height;
	  pixmap_data[i++].bits =xnav_bitmap_leaf20_bits;
	  pixmap_data[i].width =xnav_bitmap_leaf24_width;
	  pixmap_data[i].height =xnav_bitmap_leaf24_height;
	  pixmap_data[i++].bits =xnav_bitmap_leaf24_bits;

	  brow_AllocAnnotPixmap( brow_ctx, &pixmap_data, &pixmap_leaf);

          i = 0;
	  pixmap_data[i].width =xnav_bitmap_map8_width;
	  pixmap_data[i].height =xnav_bitmap_map8_height;
	  pixmap_data[i++].bits =xnav_bitmap_map8_bits;
	  pixmap_data[i].width =xnav_bitmap_map10_width;
	  pixmap_data[i].height =xnav_bitmap_map10_height;
	  pixmap_data[i++].bits =xnav_bitmap_map10_bits;
	  pixmap_data[i].width =xnav_bitmap_map12_width;
	  pixmap_data[i].height =xnav_bitmap_map12_height;
	  pixmap_data[i++].bits =xnav_bitmap_map12_bits;
	  pixmap_data[i].width =xnav_bitmap_map14_width;
	  pixmap_data[i].height =xnav_bitmap_map14_height;
	  pixmap_data[i++].bits =xnav_bitmap_map14_bits;
	  pixmap_data[i].width =xnav_bitmap_map16_width;
	  pixmap_data[i].height =xnav_bitmap_map16_height;
	  pixmap_data[i++].bits =xnav_bitmap_map16_bits;
	  pixmap_data[i].width =xnav_bitmap_map18_width;
	  pixmap_data[i].height =xnav_bitmap_map18_height;
	  pixmap_data[i++].bits =xnav_bitmap_map18_bits;
	  pixmap_data[i].width =xnav_bitmap_map20_width;
	  pixmap_data[i].height =xnav_bitmap_map20_height;
	  pixmap_data[i++].bits =xnav_bitmap_map20_bits;
	  pixmap_data[i].width =xnav_bitmap_map20_width;
	  pixmap_data[i].height =xnav_bitmap_map20_height;
	  pixmap_data[i++].bits =xnav_bitmap_map20_bits;
	  pixmap_data[i].width =xnav_bitmap_map24_width;
	  pixmap_data[i].height =xnav_bitmap_map24_height;
	  pixmap_data[i++].bits =xnav_bitmap_map24_bits;

	  brow_AllocAnnotPixmap( brow_ctx, &pixmap_data, &pixmap_map);

          i = 0;
	  pixmap_data[i].width =xnav_bitmap_openmap8_width;
	  pixmap_data[i].height =xnav_bitmap_openmap8_height;
	  pixmap_data[i++].bits =xnav_bitmap_openmap8_bits;
	  pixmap_data[i].width =xnav_bitmap_openmap10_width;
	  pixmap_data[i].height =xnav_bitmap_openmap10_height;
	  pixmap_data[i++].bits =xnav_bitmap_openmap10_bits;
	  pixmap_data[i].width =xnav_bitmap_openmap12_width;
	  pixmap_data[i].height =xnav_bitmap_openmap12_height;
	  pixmap_data[i++].bits =xnav_bitmap_openmap12_bits;
	  pixmap_data[i].width =xnav_bitmap_openmap14_width;
	  pixmap_data[i].height =xnav_bitmap_openmap14_height;
	  pixmap_data[i++].bits =xnav_bitmap_openmap14_bits;
	  pixmap_data[i].width =xnav_bitmap_openmap16_width;
	  pixmap_data[i].height =xnav_bitmap_openmap16_height;
	  pixmap_data[i++].bits =xnav_bitmap_openmap16_bits;
	  pixmap_data[i].width =xnav_bitmap_openmap18_width;
	  pixmap_data[i].height =xnav_bitmap_openmap18_height;
	  pixmap_data[i++].bits =xnav_bitmap_openmap18_bits;
	  pixmap_data[i].width =xnav_bitmap_openmap20_width;
	  pixmap_data[i].height =xnav_bitmap_openmap20_height;
	  pixmap_data[i++].bits =xnav_bitmap_openmap20_bits;
	  pixmap_data[i].width =xnav_bitmap_openmap20_width;
	  pixmap_data[i].height =xnav_bitmap_openmap20_height;
	  pixmap_data[i++].bits =xnav_bitmap_openmap20_bits;
	  pixmap_data[i].width =xnav_bitmap_openmap24_width;
	  pixmap_data[i].height =xnav_bitmap_openmap24_height;
	  pixmap_data[i++].bits =xnav_bitmap_openmap24_bits;

	  brow_AllocAnnotPixmap( brow_ctx, &pixmap_data, &pixmap_openmap);

          i = 0;
	  pixmap_data[i].width =xnav_bitmap_ref8_width;
	  pixmap_data[i].height =xnav_bitmap_ref8_height;
	  pixmap_data[i++].bits =xnav_bitmap_ref8_bits;
	  pixmap_data[i].width =xnav_bitmap_ref10_width;
	  pixmap_data[i].height =xnav_bitmap_ref10_height;
	  pixmap_data[i++].bits =xnav_bitmap_ref10_bits;
	  pixmap_data[i].width =xnav_bitmap_ref12_width;
	  pixmap_data[i].height =xnav_bitmap_ref12_height;
	  pixmap_data[i++].bits =xnav_bitmap_ref12_bits;
	  pixmap_data[i].width =xnav_bitmap_ref14_width;
	  pixmap_data[i].height =xnav_bitmap_ref14_height;
	  pixmap_data[i++].bits =xnav_bitmap_ref14_bits;
	  pixmap_data[i].width =xnav_bitmap_ref16_width;
	  pixmap_data[i].height =xnav_bitmap_ref16_height;
	  pixmap_data[i++].bits =xnav_bitmap_ref16_bits;
	  pixmap_data[i].width =xnav_bitmap_ref18_width;
	  pixmap_data[i].height =xnav_bitmap_ref18_height;
	  pixmap_data[i++].bits =xnav_bitmap_ref18_bits;
	  pixmap_data[i].width =xnav_bitmap_ref20_width;
	  pixmap_data[i].height =xnav_bitmap_ref20_height;
	  pixmap_data[i++].bits =xnav_bitmap_ref20_bits;
	  pixmap_data[i].width =xnav_bitmap_ref20_width;
	  pixmap_data[i].height =xnav_bitmap_ref20_height;
	  pixmap_data[i++].bits =xnav_bitmap_ref20_bits;
	  pixmap_data[i].width =xnav_bitmap_ref24_width;
	  pixmap_data[i].height =xnav_bitmap_ref24_height;
	  pixmap_data[i++].bits =xnav_bitmap_ref24_bits;

	  brow_AllocAnnotPixmap( brow_ctx, &pixmap_data, &pixmap_ref);

          i = 0;
	  pixmap_data[i].width =xnav_bitmap_crossref8_width;
	  pixmap_data[i].height =xnav_bitmap_crossref8_height;
	  pixmap_data[i++].bits =xnav_bitmap_crossref8_bits;
	  pixmap_data[i].width =xnav_bitmap_crossref10_width;
	  pixmap_data[i].height =xnav_bitmap_crossref10_height;
	  pixmap_data[i++].bits =xnav_bitmap_crossref10_bits;
	  pixmap_data[i].width =xnav_bitmap_crossref12_width;
	  pixmap_data[i].height =xnav_bitmap_crossref12_height;
	  pixmap_data[i++].bits =xnav_bitmap_crossref12_bits;
	  pixmap_data[i].width =xnav_bitmap_crossref14_width;
	  pixmap_data[i].height =xnav_bitmap_crossref14_height;
	  pixmap_data[i++].bits =xnav_bitmap_crossref14_bits;
	  pixmap_data[i].width =xnav_bitmap_crossref16_width;
	  pixmap_data[i].height =xnav_bitmap_crossref16_height;
	  pixmap_data[i++].bits =xnav_bitmap_crossref16_bits;
	  pixmap_data[i].width =xnav_bitmap_crossref18_width;
	  pixmap_data[i].height =xnav_bitmap_crossref18_height;
	  pixmap_data[i++].bits =xnav_bitmap_crossref18_bits;
	  pixmap_data[i].width =xnav_bitmap_crossref20_width;
	  pixmap_data[i].height =xnav_bitmap_crossref20_height;
	  pixmap_data[i++].bits =xnav_bitmap_crossref20_bits;
	  pixmap_data[i].width =xnav_bitmap_crossref20_width;
	  pixmap_data[i].height =xnav_bitmap_crossref20_height;
	  pixmap_data[i++].bits =xnav_bitmap_crossref20_bits;
	  pixmap_data[i].width =xnav_bitmap_crossref24_width;
	  pixmap_data[i].height =xnav_bitmap_crossref24_height;
	  pixmap_data[i++].bits =xnav_bitmap_crossref24_bits;

	  brow_AllocAnnotPixmap( brow_ctx, &pixmap_data, 
		&pixmap_crossref);

          for ( i = 0; i < 9; i++)
          {
	    pixmap_data[i].width =xnav_bitmap_aarithm12_width;
	    pixmap_data[i].height =xnav_bitmap_aarithm12_height;
	    pixmap_data[i].bits =xnav_bitmap_aarithm12_bits;
	  }
	  brow_AllocAnnotPixmap( brow_ctx, &pixmap_data, 
			&pixmap_aarithm);

          for ( i = 0; i < 9; i++)
          {
	    pixmap_data[i].width =xnav_bitmap_arithm12_width;
	    pixmap_data[i].height =xnav_bitmap_arithm12_height;
	    pixmap_data[i].bits =xnav_bitmap_arithm12_bits;
	  }
	  brow_AllocAnnotPixmap( brow_ctx, &pixmap_data, 
			&pixmap_arithm);

          for ( i = 0; i < 9; i++)
          {
	    pixmap_data[i].width =xnav_bitmap_bodytext12_width;
	    pixmap_data[i].height =xnav_bitmap_bodytext12_height;
	    pixmap_data[i].bits =xnav_bitmap_bodytext12_bits;
	  }
	  brow_AllocAnnotPixmap( brow_ctx, &pixmap_data, 
			&pixmap_bodytext);

          for ( i = 0; i < 9; i++)
          {
	    pixmap_data[i].width =xnav_bitmap_frame12_width;
	    pixmap_data[i].height =xnav_bitmap_frame12_height;
	    pixmap_data[i].bits =xnav_bitmap_frame12_bits;
	  }
	  brow_AllocAnnotPixmap( brow_ctx, &pixmap_data, 
			&pixmap_frame);

          for ( i = 0; i < 9; i++)
          {
	    pixmap_data[i].width =xnav_bitmap_document12_width;
	    pixmap_data[i].height =xnav_bitmap_document12_height;
	    pixmap_data[i].bits =xnav_bitmap_document12_bits;
	  }
	  brow_AllocAnnotPixmap( brow_ctx, &pixmap_data, 
			&pixmap_document);

          for ( i = 0; i < 9; i++)
          {
	    pixmap_data[i].width =xnav_bitmap_get12_width;
	    pixmap_data[i].height =xnav_bitmap_get12_height;
	    pixmap_data[i].bits =xnav_bitmap_get12_bits;
	  }
	  brow_AllocAnnotPixmap( brow_ctx, &pixmap_data, 
			&pixmap_get);

          for ( i = 0; i < 9; i++)
          {
	    pixmap_data[i].width =xnav_bitmap_getp12_width;
	    pixmap_data[i].height =xnav_bitmap_getp12_height;
	    pixmap_data[i].bits =xnav_bitmap_getp12_bits;
	  }
	  brow_AllocAnnotPixmap( brow_ctx, &pixmap_data, 
			&pixmap_getp);

          for ( i = 0; i < 9; i++)
          {
	    pixmap_data[i].width =xnav_bitmap_head12_width;
	    pixmap_data[i].height =xnav_bitmap_head12_height;
	    pixmap_data[i].bits =xnav_bitmap_head12_bits;
	  }
	  brow_AllocAnnotPixmap( brow_ctx, &pixmap_data, 
			&pixmap_head);

          for ( i = 0; i < 9; i++)
          {
	    pixmap_data[i].width =xnav_bitmap_initstep12_width;
	    pixmap_data[i].height =xnav_bitmap_initstep12_height;
	    pixmap_data[i].bits =xnav_bitmap_initstep12_bits;
	  }
	  brow_AllocAnnotPixmap( brow_ctx, &pixmap_data, 
			&pixmap_initstep);

          for ( i = 0; i < 9; i++)
          {
	    pixmap_data[i].width =xnav_bitmap_logic1_12_width;
	    pixmap_data[i].height =xnav_bitmap_logic1_12_height;
	    pixmap_data[i].bits =xnav_bitmap_logic1_12_bits;
	  }
	  brow_AllocAnnotPixmap( brow_ctx, &pixmap_data, 
			&pixmap_logic1_);

          for ( i = 0; i < 9; i++)
          {
	    pixmap_data[i].width =xnav_bitmap_logic2_12_width;
	    pixmap_data[i].height =xnav_bitmap_logic2_12_height;
	    pixmap_data[i].bits =xnav_bitmap_logic2_12_bits;
	  }
	  brow_AllocAnnotPixmap( brow_ctx, &pixmap_data, 
			&pixmap_logic2_);

          for ( i = 0; i < 9; i++)
          {
	    pixmap_data[i].width =xnav_bitmap_wait12_width;
	    pixmap_data[i].height =xnav_bitmap_wait12_height;
	    pixmap_data[i].bits =xnav_bitmap_wait12_bits;
	  }
	  brow_AllocAnnotPixmap( brow_ctx, &pixmap_data, 
			&pixmap_wait);

          for ( i = 0; i < 9; i++)
          {
	    pixmap_data[i].width =xnav_bitmap_order12_width;
	    pixmap_data[i].height =xnav_bitmap_order12_height;
	    pixmap_data[i].bits =xnav_bitmap_order12_bits;
	  }
	  brow_AllocAnnotPixmap( brow_ctx, &pixmap_data, 
			&pixmap_order);

          for ( i = 0; i < 9; i++)
          {
	    pixmap_data[i].width =xnav_bitmap_orderact12_width;
	    pixmap_data[i].height =xnav_bitmap_orderact12_height;
	    pixmap_data[i].bits =xnav_bitmap_orderact12_bits;
	  }
	  brow_AllocAnnotPixmap( brow_ctx, &pixmap_data, 
			&pixmap_orderact);

          for ( i = 0; i < 9; i++)
          {
	    pixmap_data[i].width =xnav_bitmap_setcond12_width;
	    pixmap_data[i].height =xnav_bitmap_setcond12_height;
	    pixmap_data[i].bits =xnav_bitmap_setcond12_bits;
	  }
	  brow_AllocAnnotPixmap( brow_ctx, &pixmap_data, 
			&pixmap_setcond);

          for ( i = 0; i < 9; i++)
          {
	    pixmap_data[i].width =xnav_bitmap_ssbegin12_width;
	    pixmap_data[i].height =xnav_bitmap_ssbegin12_height;
	    pixmap_data[i].bits =xnav_bitmap_ssbegin12_bits;
	  }
	  brow_AllocAnnotPixmap( brow_ctx, &pixmap_data, 
			&pixmap_ssbegin);

          for ( i = 0; i < 9; i++)
          {
	    pixmap_data[i].width =xnav_bitmap_ssend12_width;
	    pixmap_data[i].height =xnav_bitmap_ssend12_height;
	    pixmap_data[i].bits =xnav_bitmap_ssend12_bits;
	  }
	  brow_AllocAnnotPixmap( brow_ctx, &pixmap_data, 
			&pixmap_ssend);

          for ( i = 0; i < 9; i++)
          {
	    pixmap_data[i].width =xnav_bitmap_step12_width;
	    pixmap_data[i].height =xnav_bitmap_step12_height;
	    pixmap_data[i].bits =xnav_bitmap_step12_bits;
	  }
	  brow_AllocAnnotPixmap( brow_ctx, &pixmap_data, 
			&pixmap_step);

          for ( i = 0; i < 9; i++)
          {
	    pixmap_data[i].width =xnav_bitmap_sto12_width;
	    pixmap_data[i].height =xnav_bitmap_sto12_height;
	    pixmap_data[i].bits =xnav_bitmap_sto12_bits;
	  }
	  brow_AllocAnnotPixmap( brow_ctx, &pixmap_data, 
			&pixmap_sto);

          for ( i = 0; i < 9; i++)
          {
	    pixmap_data[i].width =xnav_bitmap_stop12_width;
	    pixmap_data[i].height =xnav_bitmap_stop12_height;
	    pixmap_data[i].bits =xnav_bitmap_stop12_bits;
	  }
	  brow_AllocAnnotPixmap( brow_ctx, &pixmap_data, 
			&pixmap_stop);

          for ( i = 0; i < 9; i++)
          {
	    pixmap_data[i].width =xnav_bitmap_substep12_width;
	    pixmap_data[i].height =xnav_bitmap_substep12_height;
	    pixmap_data[i].bits =xnav_bitmap_substep12_bits;
	  }
	  brow_AllocAnnotPixmap( brow_ctx, &pixmap_data, 
			&pixmap_substep);

          for ( i = 0; i < 9; i++)
          {
	    pixmap_data[i].width =xnav_bitmap_text12_width;
	    pixmap_data[i].height =xnav_bitmap_text12_height;
	    pixmap_data[i].bits =xnav_bitmap_text12_bits;
	  }
	  brow_AllocAnnotPixmap( brow_ctx, &pixmap_data, 
			&pixmap_text);

          for ( i = 0; i < 9; i++)
          {
	    pixmap_data[i].width =xnav_bitmap_title12_width;
	    pixmap_data[i].height =xnav_bitmap_title12_height;
	    pixmap_data[i].bits =xnav_bitmap_title12_bits;
	  }
	  brow_AllocAnnotPixmap( brow_ctx, &pixmap_data, 
			&pixmap_title);

          for ( i = 0; i < 9; i++)
          {
	    pixmap_data[i].width =xnav_bitmap_trans12_width;
	    pixmap_data[i].height =xnav_bitmap_trans12_height;
	    pixmap_data[i].bits =xnav_bitmap_trans12_bits;
	  }
	  brow_AllocAnnotPixmap( brow_ctx, &pixmap_data, 
			&pixmap_trans);

          for ( i = 0; i < 9; i++)
          {
	    pixmap_data[i].width =xnav_bitmap_cell12_width;
	    pixmap_data[i].height =xnav_bitmap_cell12_height;
	    pixmap_data[i].bits =xnav_bitmap_cell12_bits;
	  }
	  brow_AllocAnnotPixmap( brow_ctx, &pixmap_data,
			&pixmap_cell);

          for ( i = 0; i < 9; i++)
          {
	    pixmap_data[i].width =xnav_bitmap_condigital12_width;
	    pixmap_data[i].height =xnav_bitmap_condigital12_height;
	    pixmap_data[i].bits =xnav_bitmap_condigital12_bits;
	  }
	  brow_AllocAnnotPixmap( brow_ctx, &pixmap_data,
			&pixmap_condigital);
          for ( i = 0; i < 9; i++)
          {
	    pixmap_data[i].width =xnav_bitmap_conanalog12_width;
	    pixmap_data[i].height =xnav_bitmap_conanalog12_height;
	    pixmap_data[i].bits =xnav_bitmap_conanalog12_bits;
	  }
	  brow_AllocAnnotPixmap( brow_ctx, &pixmap_data,
			&pixmap_conanalog);
          for ( i = 0; i < 9; i++)
          {
	    pixmap_data[i].width =xnav_bitmap_confeedbackdigital12_width;
	    pixmap_data[i].height =xnav_bitmap_confeedbackdigital12_height;
	    pixmap_data[i].bits =xnav_bitmap_confeedbackdigital12_bits;
	  }
	  brow_AllocAnnotPixmap( brow_ctx, &pixmap_data,
			&pixmap_confeedbackdigital);
          for ( i = 0; i < 9; i++)
          {
	    pixmap_data[i].width =xnav_bitmap_confeedbackanalog12_width;
	    pixmap_data[i].height =xnav_bitmap_confeedbackanalog12_height;
	    pixmap_data[i].bits =xnav_bitmap_confeedbackanalog12_bits;
	  }
	  brow_AllocAnnotPixmap( brow_ctx, &pixmap_data,
			&pixmap_confeedbackanalog);
          for ( i = 0; i < 9; i++)
          {
	    pixmap_data[i].width =xnav_bitmap_conexecuteorder12_width;
	    pixmap_data[i].height =xnav_bitmap_conexecuteorder12_height;
	    pixmap_data[i].bits =xnav_bitmap_conexecuteorder12_bits;
	  }
	  brow_AllocAnnotPixmap( brow_ctx, &pixmap_data,
			&pixmap_conexecuteorder);
          for ( i = 0; i < 9; i++)
          {
	    pixmap_data[i].width =xnav_bitmap_congrafcet12_width;
	    pixmap_data[i].height =xnav_bitmap_congrafcet12_height;
	    pixmap_data[i].bits =xnav_bitmap_congrafcet12_bits;
	  }
	  brow_AllocAnnotPixmap( brow_ctx, &pixmap_data,
			&pixmap_congrafcet);
          for ( i = 0; i < 9; i++)
          {
	    pixmap_data[i].width =xnav_bitmap_stepconv12_width;
	    pixmap_data[i].height =xnav_bitmap_stepconv12_height;
	    pixmap_data[i].bits =xnav_bitmap_stepconv12_bits;
	  }
	  brow_AllocAnnotPixmap( brow_ctx, &pixmap_data,
			&pixmap_stepconv);
          for ( i = 0; i < 9; i++)
          {
	    pixmap_data[i].width =xnav_bitmap_stepdiv12_width;
	    pixmap_data[i].height =xnav_bitmap_stepdiv12_height;
	    pixmap_data[i].bits =xnav_bitmap_stepdiv12_bits;
	  }
	  brow_AllocAnnotPixmap( brow_ctx, &pixmap_data,
			&pixmap_stepdiv);
          for ( i = 0; i < 9; i++)
          {
	    pixmap_data[i].width =xnav_bitmap_transconv12_width;
	    pixmap_data[i].height =xnav_bitmap_transconv12_height;
	    pixmap_data[i].bits =xnav_bitmap_transconv12_bits;
	  }
	  brow_AllocAnnotPixmap( brow_ctx, &pixmap_data,
			&pixmap_transconv);
          for ( i = 0; i < 9; i++)
          {
	    pixmap_data[i].width =xnav_bitmap_transdiv12_width;
	    pixmap_data[i].height =xnav_bitmap_transdiv12_height;
	    pixmap_data[i].bits =xnav_bitmap_transdiv12_bits;
	  }
	  brow_AllocAnnotPixmap( brow_ctx, &pixmap_data,
			&pixmap_transdiv);

}


//
// Callbacks from brow
//
int Pal::brow_cb( FlowCtx *ctx, flow_tEvent event)
{
  Pal		*pal;
  PalItem 	*item;

  brow_GetCtxUserData( (BrowCtx *)ctx, (void **) &pal);
  switch ( event->event)
  {
    case flow_eEvent_Key_PageDown: {
      brow_Page( pal->brow_ctx, 0.8);
      break;
    }
    case flow_eEvent_Key_PageUp: {
      brow_Page( pal->brow_ctx, -0.8);
      break;
    }
    case flow_eEvent_ScrollDown: {
      brow_Page( pal->brow_ctx, 0.1);
      break;
    }
    case flow_eEvent_ScrollUp: {
      brow_Page( pal->brow_ctx, -0.1);
      break;
    }
    case flow_eEvent_Key_Up:
    {
      brow_tNode	*node_list;
      int		node_count;
      brow_tObject	object;
      int		sts;

      brow_GetSelectedNodes( pal->brow_ctx, &node_list, &node_count);
      if ( !node_count) {
        if ( pal->last_selected && pal->object_exist( pal->last_selected))
          object = pal->last_selected;
        else {
          sts = brow_GetLastVisible( pal->brow_ctx, &object);
          if ( EVEN(sts)) return 1;
        }
      }
      else {
	if ( !brow_IsVisible( pal->brow_ctx, node_list[0], flow_eVisible_Partial)) {
	  sts = brow_GetLastVisible( pal->brow_ctx, &object);
	  if ( EVEN(sts)) return 1;
	}
	else {
	  sts = brow_GetPrevious( pal->brow_ctx, node_list[0], &object);
	  if ( EVEN(sts)) {
            if ( node_count)
	      free( node_list);
            return 1;
 	  }
        }
      }
      brow_SelectClear( pal->brow_ctx);
      brow_SetInverse( object, 1);
      brow_SelectInsert( pal->brow_ctx, object);
      if ( !brow_IsVisible( pal->brow_ctx, object, flow_eVisible_Full))
        brow_CenterObject( pal->brow_ctx, object, 0.25);
      if ( node_count)
        free( node_list);
      pal->last_selected = object;
      break;
    }
    case flow_eEvent_Key_Down:
    {
      brow_tNode	*node_list;
      int		node_count;
      brow_tObject	object;
      int		sts;

      brow_GetSelectedNodes( pal->brow_ctx, &node_list, &node_count);
      if ( !node_count) {
        if ( pal->last_selected && pal->object_exist( pal->last_selected))
          object = pal->last_selected;
        else {
          sts = brow_GetFirstVisible( pal->brow_ctx, &object);
          if ( EVEN(sts)) return 1;
        }
      }
      else {
	if ( !brow_IsVisible( pal->brow_ctx, node_list[0], flow_eVisible_Partial)) {
	  sts = brow_GetFirstVisible( pal->brow_ctx, &object);
	  if ( EVEN(sts)) return 1;
	}
	else {
	  sts = brow_GetNext( pal->brow_ctx, node_list[0], &object);
	  if ( EVEN(sts)) {
            if ( node_count)
	      free( node_list);
            return 1;
 	  }
        }
      }
      brow_SelectClear( pal->brow_ctx);
      brow_SetInverse( object, 1);
      brow_SelectInsert( pal->brow_ctx, object);
      if ( !brow_IsVisible( pal->brow_ctx, object, flow_eVisible_Full))
        brow_CenterObject( pal->brow_ctx, object, 0.75);
      if ( node_count)
        free( node_list);
      pal->last_selected = object;
      break;
    }
    case flow_eEvent_Key_Right:
    {
      brow_tNode	*node_list;
      int		node_count;

      brow_GetSelectedNodes( pal->brow_ctx, &node_list, &node_count);
      if ( !node_count)
        break;
      brow_GetUserData( node_list[0], (void **)&item);
      free( node_list);
      switch( item->type)
      {
        case pal_ePalItemType_ClassVolume: 
          ((PalItemClassVolume *)item)->open( pal, 0, 0);
           break;
        case pal_ePalItemType_Menu: 
	  ((PalItemMenu *)item)->open( pal, 0, 0);
          break;
        case pal_ePalItemType_Object: 
          ((PalItemObject *)item)->open( pal, 0, 0);
          break;
        default:
          ;
      }
      break;
    }
    case flow_eEvent_Key_Return:
    case flow_eEvent_Key_Left:
    {
      brow_tNode	*node_list;
      int		node_count;
      brow_tObject	object;
      int		sts;

      brow_GetSelectedNodes( pal->brow_ctx, &node_list, &node_count);
      if ( !node_count)
        return 1;

      if ( brow_IsOpen( node_list[0]))
        // Close this node
        object = node_list[0];
      else
      {
        // Close parent
        sts = brow_GetParent( pal->brow_ctx, node_list[0], &object);
        if ( EVEN(sts))
        {
          free( node_list);
          return 1;
        }
      }
      brow_GetUserData( object, (void **)&item);
      switch( item->type)
      {
        case pal_ePalItemType_ClassVolume: 
          ((PalItemClassVolume *)item)->open( pal, 0, 0);
           break;
        case pal_ePalItemType_Menu: 
	  ((PalItemMenu *)item)->open( pal, 0, 0);
          break;
        case pal_ePalItemType_Object: 
          ((PalItemObject *)item)->open( pal, 0, 0);
          break;
        default:
          ;
      }
      brow_SelectClear( pal->brow_ctx);
      brow_SetInverse( object, 1);
      brow_SelectInsert( pal->brow_ctx, object);
      if ( !brow_IsVisible( pal->brow_ctx, object, flow_eVisible_Full))
        brow_CenterObject( pal->brow_ctx, object, 0.25);
      free( node_list);
      pal->last_selected = object;
      break;
    }
    case flow_eEvent_Key_Tab:
    {
      if ( pal->traverse_focus_cb)
        (pal->traverse_focus_cb)( pal->parent_ctx, pal);
      break;
    }
    case flow_eEvent_SelectClear:
      brow_ResetSelectInverse( pal->brow_ctx);
      break;
    case flow_eEvent_ObjectDeleted:
      brow_GetUserData( event->object.object, (void **)&item);
      delete item;
      break;
    case flow_eEvent_MB1DoubleClick:
      switch ( event->object.object_type)
      {
        case flow_eObjectType_Node:
          brow_GetUserData( event->object.object, (void **)&item);
          switch( item->type)
          {
            case pal_ePalItemType_ClassVolume: 
	      ((PalItemClassVolume *)item)->open( pal,
			event->object.x, event->object.y);
              break;
            case pal_ePalItemType_Menu: 
	      ((PalItemMenu *)item)->open( pal,
			event->object.x, event->object.y);
              break;
            case pal_ePalItemType_Object: 
	      ((PalItemObject *)item)->open( pal,
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
    case flow_eEvent_MB1Click: {
      // Select
      double ll_x, ll_y, ur_x, ur_y;
      int		sts;
      pwr_tCid cid = pwr_cNCid;

      if ( pal->set_focus_cb)
        (pal->set_focus_cb)( pal->parent_ctx, pal);

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
            sts = Pal::brow_cb( ctx, doubleclick_event);
            free( (char *) doubleclick_event);
            return sts;
          }

          if ( !brow_FindSelectedObject( pal->brow_ctx, event->object.object)) {
            brow_SelectClear( pal->brow_ctx);
            brow_SetInverse( event->object.object, 1);
            brow_SelectInsert( pal->brow_ctx, event->object.object);
          }


	  if ( !pal->select_cb)
	    break;

	  switch ( event->object.object_type) {
	  case flow_eObjectType_Node:
	    brow_GetUserData( event->object.object, (void **)&item);
	    switch ( item->type) {
	    case pal_ePalItemType_Object:
	      char name[32];
	      int size;

	      sts = ldh_ObjidToName( pal->ldhses, item->objid, ldh_eName_Object,
				     name, sizeof(name), &size);
	      if ( EVEN(sts)) return sts;
	      
	      sts =  ldh_ClassNameToId( pal->ldhses, &cid, name);
	      if ( EVEN(sts)) return sts;
	      
	      break;
	    case pal_ePalItemType_Class:
	      sts = ldh_ClassNameToId( pal->ldhses, &cid, ((PalItemClass *)item)->name);
	      if ( EVEN(sts)) return sts;
	      
	      break;
	    default:
	      ;
	    }
	  default:
	    ;
	  }

	  if ( cid == pwr_cNCid)
	    break;
	  
	  pal->select_cb( pal->parent_ctx, cid);
      
          break;
        default:
          brow_SelectClear( pal->brow_ctx);
      }
      pal->last_selected = event->object.object;
      break;
    }
    case flow_eEvent_Map:
    {
      pal->displayed = 1;
      break;
    }
    case flow_eEvent_MB3Press:
    {
      // Popup menu
      pwr_tCid cid = pwr_cNCid;
      pwr_tStatus sts;

      if ( !pal->create_popup_menu_cb)
	break;

      switch ( event->object.object_type)
      {
        case flow_eObjectType_Node:
          brow_GetUserData( event->object.object, (void **)&item);
	  switch ( item->type) {
	  case pal_ePalItemType_Object:
	    char name[32];
	    int size;

	    sts = ldh_ObjidToName( pal->ldhses, item->objid, ldh_eName_Object,
			name, sizeof(name), &size);
	    if ( EVEN(sts)) return sts;

	    sts =  ldh_ClassNameToId( pal->ldhses, &cid, name);
	    if ( EVEN(sts)) return sts;

	    break;
	  case pal_ePalItemType_Class:
	    sts = ldh_ClassNameToId( pal->ldhses, &cid, ((PalItemClass *)item)->name);
	    if ( EVEN(sts)) return sts;

	    break;
	  default:
	    ;
	  }
      default:
	;
      }

      if ( cid == pwr_cNCid)
	break;

      pal->create_popup_menu( cid, event->any.x_pixel, event->any.y_pixel);
      break;
    }
    default:
      ;
  }
  return 1;
}

//
// Create nodeclasses
//
void Pal::create_nodeclasses()
{
  allocate_pixmaps();

  // Create common-class

  brow_CreateNodeClass( brow_ctx, "PaletteDefault", 
		flow_eNodeGroup_Common, &nc);
  brow_AddFrame( nc, 0, 0, 10, 0.8, flow_eDrawType_Line, -1, 1);
  brow_AddAnnotPixmap( nc, 0, 0.2, 0.1, flow_eDrawType_Line, 2, 0);
  brow_AddAnnot( nc, 1.2, 0.6, 0,
		flow_eDrawType_TextHelveticaBold, 2, flow_eAnnotType_OneLine, 
		0);
  brow_AddAnnotPixmap( nc, 1, 7, 0.1, flow_eDrawType_Line, 2, 1);
}

//
// Backcall routine called at creation of the brow widget
// Enable event, create nodeclasses and insert the root objects.
//
int Pal::init_brow_cb( FlowCtx *fctx, void *client_data)
{
  Pal *pal = (Pal *) client_data;
  BrowCtx *ctx = (BrowCtx *)fctx;
  int		sts;
  brow_sAttributes brow_attr;
  unsigned long	mask;


  pal->brow_ctx = ctx;

  mask = 0;

  mask |= brow_eAttr_indentation;
  brow_attr.indentation = 0.5;
  mask |= brow_eAttr_annotation_space;
  brow_attr.annotation_space = 0.5;
  brow_SetAttributes( pal->brow_ctx, &brow_attr, mask);
  brow_SetCtxUserData( pal->brow_ctx, pal);

  brow_EnableEvent( ctx, flow_eEvent_MB1DoubleClick, flow_eEventType_CallBack, 
	Pal::brow_cb);
  brow_EnableEvent( ctx, flow_eEvent_MB1Click, flow_eEventType_CallBack, 
	Pal::brow_cb);
  brow_EnableEvent( ctx, flow_eEvent_MB3Press, flow_eEventType_CallBack, 
	Pal::brow_cb);
  brow_EnableEvent( ctx, flow_eEvent_SelectClear, flow_eEventType_CallBack, 
	Pal::brow_cb);
  brow_EnableEvent( ctx, flow_eEvent_Key_Up, flow_eEventType_CallBack, 
	Pal::brow_cb);
  brow_EnableEvent( ctx, flow_eEvent_Key_Down, flow_eEventType_CallBack, 
	Pal::brow_cb);
  brow_EnableEvent( ctx, flow_eEvent_Key_Left, flow_eEventType_CallBack, 
	Pal::brow_cb);
  brow_EnableEvent( ctx, flow_eEvent_Key_Right, flow_eEventType_CallBack, 
	Pal::brow_cb);
  brow_EnableEvent( ctx, flow_eEvent_Key_Return, flow_eEventType_CallBack, 
	Pal::brow_cb);
  brow_EnableEvent( ctx, flow_eEvent_Key_Tab, flow_eEventType_CallBack, 
	Pal::brow_cb);
  brow_EnableEvent( ctx, flow_eEvent_Map, flow_eEventType_CallBack, 
	Pal::brow_cb);
  brow_EnableEvent( ctx, flow_eEvent_Key_PageUp, flow_eEventType_CallBack, 
	Pal::brow_cb);
  brow_EnableEvent( ctx, flow_eEvent_Key_PageDown, flow_eEventType_CallBack, 
	Pal::brow_cb);
  brow_EnableEvent( ctx, flow_eEvent_ScrollUp, flow_eEventType_CallBack, 
	Pal::brow_cb);
  brow_EnableEvent( ctx, flow_eEvent_ScrollDown, flow_eEventType_CallBack, 
	Pal::brow_cb);

  pal->create_nodeclasses();

  if ( pal->ldhses)
  {
    sts = pal->session_opened( pal->ldhses, NULL);
    if (EVEN(sts)) return sts;
  }

  return 1;
}

int Pal::session_opened( ldh_tSesContext pal_ldhses, char *pal_root_name)
{

  if ( ldhses)
    brow_DeleteAll( brow_ctx);

  if ( pal_root_name)
    strcpy( root_name, pal_root_name);

  ldhses = pal_ldhses;
  wbctx = ldh_SessionToWB( ldhses);

  if ( !menu) {
    menu = PalFile::config_tree_build( ldhses, pal_cPaletteFile,
	  pal_eNameType_Palette, root_name, NULL); 
    if ( !menu) {
      printf( "** Pal: palette entry \"%s\" not found in configuration file\n",
	    root_name);
      return 0;
    }
    PalFile::config_tree_build( ldhses, pal_cLocalPaletteFile,
	  pal_eNameType_Palette, root_name, menu); 
  }

  // Open the root item
  root_item = new PalItemMenu( this, menu->title, NULL, flow_eDest_After,
			       &menu->child_list, 1);
  ((PalItemMenu *)root_item)->open( this, 0, 0);

  return 1;
}

int Pal::session_closed()
{
  if ( ldhses)
    brow_DeleteAll( brow_ctx);
  ldhses = 0;
  wbctx = 0;
  return 1;
}

//
// Create a palette item. The class of item depends of the class
// of the object.
//
int Pal::create_item( pwr_tObjid objid, 
	brow_tNode dest, flow_eDest dest_code, void **item,
	int is_root)
{
  PalItem *im;

  im = (PalItem *) new PalItemObject( this, objid, dest, dest_code, is_root);
  *item = (void **) im;
  return 1;
}

//
// Create the palette widgets
//
Pal::Pal(
	void *pal_parent_ctx,
	char *pal_name,
	ldh_tSesContext pal_ldhses,
	char *pal_root_name,
	pwr_tStatus *status) : 
  parent_ctx(pal_parent_ctx),
  wbctx(0), ldhses(pal_ldhses), root_objid(pwr_cNObjid), root_item(0),
  last_selected(0), selection_owner(0), set_focus_cb(0),
  traverse_focus_cb(0), create_popup_menu_cb(0), select_cb(0), displayed(0), menu(0)
{
  strcpy( name, pal_name);
  strcpy( root_name, pal_root_name);
  *status = 1;
}

//
//  Delete a pal context
//
Pal::~Pal()
{
}

int Pal::object_exist( brow_tObject object)
{
  brow_tObject 	*object_list;
  int		object_cnt;
  int		i;

  brow_GetObjectList( brow_ctx, &object_list, &object_cnt);
  for ( i = 0; i < object_cnt; i++)
  {
    if ( object_list[i] == object)
      return 1;
  }
  return 0;
}

//
//  Zoom
//
void Pal::zoom( double zoom_factor)
{
  brow_Zoom( brow_ctx, zoom_factor);
}

//
//  Return to base zoom factor
//
void Pal::unzoom()
{
  brow_UnZoom( brow_ctx);
}

//
//  Return associated class of selected object
//
int Pal::get_select( pwr_tClassId *classid)
{
  brow_tNode	*node_list;
  int		node_count;
  PalItem		*item;
  int		size;
  char		name[80];
  int		sts;
  
  brow_GetSelectedNodes( brow_ctx, &node_list, &node_count);
  if ( !node_count)
    return 0;

  brow_GetUserData( node_list[0], (void **)&item);
  free( node_list);

  *classid = pwr_cNClassId;

  switch ( item->type) {
    case pal_ePalItemType_Object:
      sts = ldh_ObjidToName( ldhses, item->objid, ldh_eName_Object,
			name, sizeof(name), &size);
      if ( EVEN(sts)) return sts;

      sts =  ldh_ClassNameToId( ldhses, classid, name);
      if ( EVEN(sts)) return sts;

      break;
    case pal_ePalItemType_Class:
      sts = ldh_ClassNameToId( ldhses, classid, ((PalItemClass *)item)->name);
      if ( EVEN(sts)) return sts;

      break;
    default:
      return 0;
  }
  return 1;
}









