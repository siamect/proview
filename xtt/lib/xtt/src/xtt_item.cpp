/* 
 * Proview   $Id: xtt_item.cpp,v 1.22 2008-10-31 12:51:36 claes Exp $
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

#include "flow_std.h"

#include <stdio.h>
#include <stdlib.h>

#include "rt_gdh.h"
#include "rt_gdh_msg.h"
#include "co_cdh.h"
#include "co_time.h"
#include "pwr_baseclasses.h"
#include "co_dcli.h"
#include "flow.h"
#include "flow_browctx.h"
#include "flow_browapi.h"
#include "xtt_xnav.h"
#include "xtt_xnav_crr.h"
#include "xtt_item.h"
#include "rt_xnav_msg.h"
#include "co_nav_msg.h"
#include "xtt_localdb.h"
#include "xtt_url.h"
#include "co_error.h"


//
// Definition of item classes
// The following classes are defined:
//    Item		superclass.
//    ItemObject	Common object.

//
// Member functions for Item classes
//




//
// Member functions for ItemObject
//
ItemObject::ItemObject( XNavBrow *brow, pwr_tObjid item_objid, 
	brow_tNode dest, flow_eDest dest_code, int item_is_root) :
  ItemBaseObject( item_objid, item_is_root), 
  alarm_level(0), max_alarm_level(0), block_level(0), max_block_level(0)
{
  int sts;
  pwr_tAName segname;
  pwr_tObjid child;
  char	descr[80];

  type = xnav_eItemType_Object;

  sts = gdh_ObjidToName( objid, name, sizeof(name), cdh_mNName);
  if ( EVEN(sts)) throw co_error(sts);
  if ( !is_root)
  {
    if ( objid.oix != 0) {
      sts = gdh_ObjidToName( objid, segname, sizeof(segname), cdh_mName_object);
      brow_CreateNode( brow->ctx, segname, brow->nc_object, 
		       dest, dest_code, (void *) this, 1, &node);
    }
    else {
      sts = gdh_VolumeIdToName( objid.vid, segname, sizeof(segname));
      brow_CreateNode( brow->ctx, segname, brow->nc_table, 
		       dest, dest_code, (void *) this, 1, &node);
    }
    if ( EVEN(sts)) throw co_error(sts);

    sts = gdh_GetObjectClass( objid, &cid);
    if ( EVEN(sts)) throw co_error(sts);

    // Set pixmap
    sts = gdh_GetChild( objid, &child);
    if( ODD(sts)) {
      if ( cid == pwr_cClass_plc)
	brow_SetAnnotPixmap( node, 0, brow->pixmap_plcpgm);
      else
	brow_SetAnnotPixmap( node, 0, brow->pixmap_map);
    }
    else
      brow_SetAnnotPixmap( node, 0, brow->pixmap_leaf);
    // Set object name annotation
    brow_SetAnnotation( node, 0, segname, strlen(segname));

    // Set class annotation
    sts = gdh_ObjidToName( cdh_ClassIdToObjid( cid),
		  segname, sizeof(segname), cdh_mName_object);
    if ( EVEN(sts)) throw co_error(sts);
    brow_SetAnnotation( node, 1, segname, strlen(segname));

    // Set description annotation
    sts = gdh_ObjidToName( objid, segname, sizeof(segname), cdh_mName_volumeStrict);
    if ( EVEN(sts)) throw co_error(sts);

    if ( objid.oix != 0) {
      strcat( segname, ".Description");
      sts = gdh_GetObjectInfo( segname, descr, sizeof(descr));
      if ( ODD(sts))
	brow_SetAnnotation( node, 2, descr, strlen(descr));
    }
    else {
      gdh_sVolumeInfo info;

      sts = gdh_GetVolumeInfo( objid.vid, &info);
      if ( ODD(sts)) {
	time_AtoAscii( &info.time, time_eFormat_DateAndTime, descr, sizeof(descr));
	strcat( descr, "  ");
	if ( info.isLoaded)
	  strcat( descr, "L");
	if ( info.isCached)
	  strcat( descr, "C");
	if ( info.isMounted)
	  strcat( descr, "M");
	brow_SetAnnotation( node, 3, descr, strlen(descr));
      }
    }
  }
}

int ItemBaseObject::open_children( XNavBrow *brow, double x, double y)
{
  double	node_x, node_y;
  pwr_tObjid	child;
  int		child_exist;
  int		sts;

  if ( cdh_ObjidIsNull( objid))
    return 1;

  if ( !is_root)
    brow_GetNodePosition( node, &node_x, &node_y);
  else 
    node_y = 0;

  if ( !is_root && brow_IsOpen( node))
  {
    // Close
    brow_SetNodraw( brow->ctx);
    brow_CloseNode( brow->ctx, node);
    if ( brow_IsOpen( node) & xnav_mOpen_Attributes)
      brow_RemoveAnnotPixmap( node, 1);
    if ( brow_IsOpen( node) & xnav_mOpen_Children) {
      if ( cid == pwr_cClass_plc)
	brow_SetAnnotPixmap( node, 0, brow->pixmap_plcpgm);
      else
	brow_SetAnnotPixmap( node, 0, brow->pixmap_map);
    }
    brow_ResetOpen( node, xnav_mOpen_All);
    brow_ResetNodraw( brow->ctx);
    brow_Redraw( brow->ctx, node_y);
  }
  else
  {
    Item *item;

    // Create some children
    brow_SetNodraw( brow->ctx);

    child_exist = 0;
    sts = gdh_GetChild( objid, &child);
    while ( ODD(sts))
    {
      child_exist = 1;

      item = new ItemObject( brow, child, node, flow_eDest_IntoLast, 0);
      sts = gdh_GetNextSibling( child, &child);
    }

    if ( child_exist && !is_root)
    {
      brow_SetOpen( node, xnav_mOpen_Children);
      brow_SetAnnotPixmap( node, 0, brow->pixmap_openmap);
    }
    brow_ResetNodraw( brow->ctx);
    if ( child_exist)
      brow_Redraw( brow->ctx, node_y);
    else
      return XNAV__NOCHILD;
  }
  return 1;
}

int ItemBaseObject::open_trace( XNavBrow *brow, double x, double y)
{
  pwr_tClassId 	classid;
  pwr_tObjid	parent;
  pwr_tOName   	name;
  int		sts;
  if ( brow->usertype != brow_eUserType_XNav)
    return 0;

  XNav *xnav = (XNav *) brow->userdata;

  if ( cdh_ObjidIsNull( objid))
    return 1;
  
  sts = gdh_GetObjectClass( objid, &classid);
  if ( EVEN(sts)) return sts;

  if ( classid == pwr_cClass_plc ||
       classid == pwr_cClass_windowplc ||
       classid == pwr_cClass_windowcond ||
       classid == pwr_cClass_windoworderact ||
       classid == pwr_cClass_windowsubstep)
  {
    xnav->start_trace( objid, NULL);
  }
  else
  {
    sts = gdh_GetParent( objid, &parent);
    if ( EVEN(sts)) return sts;

    sts = gdh_GetObjectClass( parent, &classid);
    if ( EVEN(sts)) return sts;

    if ( classid == pwr_cClass_plc ||
         classid == pwr_cClass_windowplc ||
         classid == pwr_cClass_windowcond ||
         classid == pwr_cClass_windoworderact ||
         classid == pwr_cClass_windowsubstep)
    {
      sts = gdh_ObjidToName( objid,
	    	name, sizeof(name), cdh_mName_object);
      if ( EVEN(sts)) return sts;

      xnav->start_trace( parent, name);
    }
  }

  return 1;
}

int ItemBaseObject::open_attributes( XNavBrow *brow, double x, double y)
{
  double	node_x, node_y;

  if ( cdh_ObjidIsNull( objid))
    return 1;

  brow_GetNodePosition( node, &node_x, &node_y);

  if ( brow_IsOpen( node) & xnav_mOpen_Attributes)
  {
    // Attributes is open, close
    brow_SetNodraw( brow->ctx);
    brow_CloseNode( brow->ctx, node);
    brow_ResetOpen( node, xnav_mOpen_Attributes);
    brow_RemoveAnnotPixmap( node, 1);
    brow_ResetNodraw( brow->ctx);
    brow_Redraw( brow->ctx, node_y);
  }
  else 
  {
    int			sts;
    pwr_tClassId	classid;
    unsigned long	elements;
    Item 	*item;
    int		attr_exist;
    int		i;
    gdh_sAttrDef *bd;
    int 	rows;

    if ( brow_IsOpen( node) & xnav_mOpen_Children ||
	 brow_IsOpen( node) & xnav_mOpen_Crossref)
    {
      // Close children first
      brow_SetNodraw( brow->ctx);
      brow_CloseNode( brow->ctx, node);
      brow_ResetOpen( node, xnav_mOpen_Children);
      brow_ResetOpen( node, xnav_mOpen_Crossref);
      if ( cid == pwr_cClass_plc)
	brow_SetAnnotPixmap( node, 0, brow->pixmap_plcpgm);
      else
	brow_SetAnnotPixmap( node, 0, brow->pixmap_map);
      brow_ResetNodraw( brow->ctx);
      brow_Redraw( brow->ctx, node_y);
    }

    // Create some attributes
    brow_SetNodraw( brow->ctx);


    // Get objid for rtbody or sysbody

    sts = gdh_GetObjectClass ( objid, &classid);
    if ( EVEN(sts)) return sts;

    if ( brow->usertype == brow_eUserType_XNav && ((XNav *)brow->userdata)->gbl.show_truedb)
      sts = gdh_GetTrueObjectBodyDef( classid, &bd, &rows);
    else
      sts = gdh_GetObjectBodyDef( classid, &bd, &rows, objid);
    if ( EVEN(sts)) return sts;

    for ( i = 0; i < rows; i++) {
      if ( bd[i].flags & gdh_mAttrDef_Shadowed)
	continue;
      if ( bd[i].attr->Param.Info.Flags & PWR_MASK_RTVIRTUAL || 
	   bd[i].attr->Param.Info.Flags & PWR_MASK_PRIVATE)
	continue;
      if ( (bd[i].attr->Param.Info.Type == pwr_eType_CastId ||
	    bd[i].attr->Param.Info.Type == pwr_eType_DisableAttr) && 
	   ((brow->usertype == brow_eUserType_XNav && 
	     !((XNav *)brow->userdata)->gbl.show_truedb) ||
	    brow->usertype == brow_eUserType_XAttNav))
	continue;
      if ( bd[i].attr->Param.Info.Flags & PWR_MASK_RTHIDE &&
	   ((brow->usertype == brow_eUserType_XNav && 
	     !((XNav *)brow->userdata)->gbl.show_allattr) ||
	    brow->usertype == brow_eUserType_XAttNav))
	continue;

      if ( bd[i].attr->Param.Info.Flags & PWR_MASK_DISABLEATTR &&
	   ((brow->usertype == brow_eUserType_XNav && 
	     !((XNav *)brow->userdata)->gbl.show_truedb) ||
	    brow->usertype == brow_eUserType_XAttNav)) {
	pwr_sAttrRef aref = cdh_ObjidToAref( objid);
	pwr_sAttrRef aaref;
	pwr_tDisableAttr disabled;

	sts = gdh_ArefANameToAref( &aref, bd[i].attrName, &aaref);
	if ( EVEN(sts)) return sts;

	sts = gdh_ArefDisabled( &aaref, &disabled);
	if ( EVEN(sts)) return sts;

	if ( disabled)
	  continue;
      }

      if ( classid == pwr_eClass_System)
	bd[i].attr->Param.Info.Flags |= PWR_MASK_CONST;

      elements = 1;
      if ( bd[i].attr->Param.Info.Flags & PWR_MASK_ARRAY ) {
	attr_exist = 1;
	int aelem, asize; 

	if ( bd[i].attr->Param.Info.Flags & PWR_MASK_DYNAMIC) {
	  sts = gdh_GetDynamicAttrSize( objid, bd[i].attrName, (pwr_tUInt32 *)&asize);
	  if ( EVEN(sts)) return sts;

	  aelem = asize / bd[i].attr->Param.Info.Size;
	}
	else {
	  aelem = bd[i].attr->Param.Info.Elements;
	  asize = bd[i].attr->Param.Info.Size;
	}
	
	item = (Item *) new ItemAttrArray( brow, objid, node, 
					   flow_eDest_IntoLast,
					   bd[i].attrName,
					   aelem, 
					   bd[i].attr->Param.Info.Type, 
					   bd[i].attr->Param.TypeRef, 
					   asize,
					   bd[i].attr->Param.Info.Flags, 0);
      }
      else if ( bd[i].attr->Param.Info.Flags & PWR_MASK_CLASS ) {
	attr_exist = 1;
	item = (Item *) new ItemAttrObject( brow, objid, node, 
					   flow_eDest_IntoLast,
					   bd[i].attrName,
					   bd[i].attr->Param.TypeRef,
					   bd[i].attr->Param.Info.Size,
					   bd[i].attr->Param.Info.Flags, 0, 0);
      }
      else {
	attr_exist = 1;
	item = (Item *) new ItemAttr( brow, objid, node, 
				      flow_eDest_IntoLast, 
				      bd[i].attrName,
				      bd[i].attr->Param.Info.Type, 
				      bd[i].attr->Param.TypeRef, 
				      bd[i].attr->Param.Info.Size,
				      bd[i].attr->Param.Info.Flags, 0, 
				      item_eDisplayType_Attr);
      } 
    }
    free( (char *)bd);
    
    if ( attr_exist && !is_root) {
      brow_SetOpen( node, xnav_mOpen_Attributes);
      brow_SetAnnotPixmap( node, 1, brow->pixmap_openattr);
    }
    brow_ResetNodraw( brow->ctx);
    if ( attr_exist)
      brow_Redraw( brow->ctx, node_y);
  }
  return 1;
}

int ItemBaseObject::open_attribute( XNavBrow *brow, double x, double y, 
		char *attr_name, int element)
{
  int		sts;
  pwr_tClassId	classid;
  unsigned long	elements;
  Item 		*item;
  int		attr_exist;
  int		i;
  gdh_sAttrDef 	*bd;
  int 		rows;

  if ( cdh_ObjidIsNull( objid))
    return 1;

  // Get objid for rtbody or sysbody

  sts = gdh_GetObjectClass ( objid, &classid);
  if ( EVEN(sts)) return sts;

  sts = gdh_GetObjectBodyDef( classid, &bd, &rows, objid);
  if ( EVEN(sts)) return sts;

  attr_exist = 0;
  for ( i = 0; i < rows; i++) {
    if ( bd[i].attr->Param.Info.Flags & PWR_MASK_RTVIRTUAL || 
	 bd[i].attr->Param.Info.Flags & PWR_MASK_PRIVATE)
      continue;

    if ( cdh_NoCaseStrcmp( attr_name, bd[i].attrName) == 0) {
      if ( bd[i].attr->Param.Info.Flags & PWR_MASK_RTVIRTUAL || 
	   bd[i].attr->Param.Info.Flags & PWR_MASK_PRIVATE) {
	// This parameter does not contain any useful information 
	return XNAV__PRIVATTR;
      }

      elements = 1;
      if ( bd[i].attr->Param.Info.Flags & PWR_MASK_ARRAY ) {
	if ( element == -1)
	  return XNAV__NOELEMENT;
	attr_exist = 1;
	item = (Item *) new ItemAttrArrayElem( brow, objid, node, 
	        flow_eDest_IntoLast, 
		bd[i].attrName, element, 
		bd[i].attr->Param.Info.Type,
		bd[i].attr->Param.TypeRef,
		bd[i].attr->Param.Info.Size / bd[i].attr->Param.Info.Elements, 
		bd[i].attr->Param.Info.Flags, 
	        0, item_eDisplayType_Path);
      }
      else {
	attr_exist = 1;
	item = (Item *) new ItemAttr( brow, objid, node,
		flow_eDest_IntoLast, bd[i].attrName,
		bd[i].attr->Param.Info.Type, 
		bd[i].attr->Param.TypeRef, 
	        bd[i].attr->Param.Info.Size, 
	        bd[i].attr->Param.Info.Flags, 
		0, item_eDisplayType_Path);
      }
      break;
    }
  }
  return 1;
}

int ItemBaseObject::open_crossref( XNavBrow *brow, double x, double y)
{
  double	node_x, node_y;
  int		crossref_exist;
  int		sts;
  pwr_tClassId	classid;
  XNav 		*xnav;
  char		file[20] = "*";

  if ( brow->usertype == brow_eUserType_XNav)
    xnav = (XNav *) brow->userdata;
  else
    xnav = 0;

  if ( cdh_ObjidIsNull( objid))
    return 1;

  if ( !is_root)
    brow_GetNodePosition( node, &node_x, &node_y);
  else 
    node_y = 0;

  if ( !is_root && brow_IsOpen( node))
  {
    // Close
    brow_SetNodraw( brow->ctx);
    brow_CloseNode( brow->ctx, node);
    if ( brow_IsOpen( node) & xnav_mOpen_Attributes)
      brow_RemoveAnnotPixmap( node, 1);
    if ( brow_IsOpen( node) & xnav_mOpen_Children) {
      if ( cid == pwr_cClass_plc)
	brow_SetAnnotPixmap( node, 0, brow->pixmap_plcpgm);
      else
	brow_SetAnnotPixmap( node, 0, brow->pixmap_map);
    }
    brow_ResetOpen( node, xnav_mOpen_All);
    brow_ResetNodraw( brow->ctx);
    brow_Redraw( brow->ctx, node_y);
  }
  else
  {
    // Fetch the cross reference list
    crossref_exist = 0;
    brow_SetNodraw( brow->ctx);

    sts = gdh_GetObjectClass ( objid, &classid);
    if ( EVEN(sts)) return sts;

    switch ( classid)
    {
      case pwr_cClass_Di:
      case pwr_cClass_Dv:
      case pwr_cClass_Do:
      case pwr_cClass_Po:
      case pwr_cClass_Av:
      case pwr_cClass_Ai:
      case pwr_cClass_Ao:
      case pwr_cClass_Iv:
      case pwr_cClass_Ii:
      case pwr_cClass_Io:
      case pwr_cClass_Co:
        sts = xnav_crr_signal( brow, file, name, node);
        break;
      default:
        sts = xnav_crr_object( brow, file, name, node);
    }
    if ( xnav && sts == NAV__OBJECTNOTFOUND)
      xnav->message('E', "Object not found in crossreferens file");
    else if ( xnav && sts == NAV__NOCROSSREF)
      xnav->message('I', "There is no crossreferences for this object");
    else if ( ODD(sts))
    {
      brow_SetOpen( node, xnav_mOpen_Crossref);
      crossref_exist = 1;
    }
    brow_ResetNodraw( brow->ctx);
    if ( crossref_exist)
      brow_Redraw( brow->ctx, node_y);
  }
  return 1;
}

void ItemBaseObject::close( XNavBrow *brow, double x, double y)
{
  double	node_x, node_y;

  if ( cdh_ObjidIsNull( objid))
    return;

  if ( brow_IsOpen( node))
  {
    // Close
    brow_GetNodePosition( node, &node_x, &node_y);
    brow_SetNodraw( brow->ctx);
    brow_CloseNode( brow->ctx, node);
    if ( brow_IsOpen( node) & xnav_mOpen_Attributes)
      brow_RemoveAnnotPixmap( node, 1);
    if ( brow_IsOpen( node) & xnav_mOpen_Children) {
      if ( cid == pwr_cClass_plc)
	brow_SetAnnotPixmap( node, 0, brow->pixmap_plcpgm);
      else
	brow_SetAnnotPixmap( node, 0, brow->pixmap_map);
    }
    brow_ResetOpen( node, xnav_mOpen_All);
    brow_ResetNodraw( brow->ctx);
    brow_Redraw( brow->ctx, node_y);
  }
}

//
// ItemAttrArray
//
int ItemAttrArray::open_attributes( XNavBrow *brow, double x, double y)
{
  double	node_x, node_y;
  int		i;

  brow_GetNodePosition( node, &node_x, &node_y);

  if ( brow_IsOpen( node) & xnav_mOpen_Attributes)
  {
    // Attributes is open, close
    brow_SetNodraw( brow->ctx);
    brow_CloseNode( brow->ctx, node);
    brow_ResetOpen( node, xnav_mOpen_All);
    brow_ResetNodraw( brow->ctx);
    brow_Redraw( brow->ctx, node_y);
  }
  else 
  {
    Item *item;

    // Create some elements
    brow_SetNodraw( brow->ctx);

    for ( i = 0; i < elements; i++) {
      if ( flags & PWR_MASK_CLASS)
	item = (Item *) new ItemAttrObject( brow, objid, node, 
					   flow_eDest_IntoLast,
					   name, type_id,
					    size / elements, flags, i, 0);
      else
	item = (Item *) new ItemAttrArrayElem( brow, objid, node, 
		flow_eDest_IntoLast, name, i, type_id, tid, 
		size / elements, flags, is_root, item_eDisplayType_Attr);
    }

    if ( !is_root)
    {
      brow_SetOpen( node, xnav_mOpen_Attributes);
    }
    brow_ResetNodraw( brow->ctx);
    brow_Redraw( brow->ctx, node_y);
  }
  return 1;
}

void ItemAttrArray::close( XNavBrow *brow, double x, double y)
{
  double	node_x, node_y;

  if ( brow_IsOpen( node) & xnav_mOpen_Attributes)
  {
    // Attributes is open, close
    brow_GetNodePosition( node, &node_x, &node_y);
    brow_SetNodraw( brow->ctx);
    brow_CloseNode( brow->ctx, node);
    brow_ResetOpen( node, xnav_mOpen_All);
    brow_ResetNodraw( brow->ctx);
    brow_Redraw( brow->ctx, node_y);
  }
}

int ItemBaseAttr::open_children( XNavBrow *brow, double x, double y)
{
  double	node_x, node_y;

  if ( cdh_ObjidIsNull( objid))
    return 1;

  brow_GetNodePosition( node, &node_x, &node_y);

  if ( brow_IsOpen( node) & xnav_mOpen_Children)
  {
    // Attributes is open, close
    brow_SetNodraw( brow->ctx);
    brow_CloseNode( brow->ctx, node);
    brow_ResetOpen( node, xnav_mOpen_Children);
    brow_RemoveAnnotPixmap( node, 1);
    brow_ResetNodraw( brow->ctx);
    brow_Redraw( brow->ctx, node_y);
  }
  else {
    int sts;

    if ( type_id == pwr_eType_Enum) {
      gdh_sValueDef *vd;
      int rows;

      sts = gdh_GetEnumValueDef( tid, &vd, &rows);
      if ( EVEN(sts)) return XNAV__NOCHILDREN;

      // Create some children
      brow_SetNodraw( brow->ctx);

      for ( int i = 0; i < rows; i++) {
        new ItemEnum( brow, objid, node, flow_eDest_IntoLast,
		      vd[i].Value->Text, attr, 
		      type_id, tid,
		      size, flags, vd[i].Value->Value, 0, 1);
      }
      free( (char *)vd);
    }
    else if ( type_id == pwr_eType_Mask) {
      gdh_sBitDef *bd;
      int rows;

      sts = gdh_GetMaskBitDef( tid, &bd, &rows);
      if ( EVEN(sts)) return XNAV__NOCHILDREN;

      // Create some children
      brow_SetNodraw( brow->ctx);

      for ( int i = 0; i < rows; i++) {
        new ItemMask( brow, objid, node, flow_eDest_IntoLast,
		      bd[i].Bit->Text, attr, type_id,
		      tid, size, flags, (unsigned int) bd[i].Bit->Value, 0, 
		      1);
      }
      free( (char *)bd);
    }
    else
      return 0;

    brow_SetOpen( node, xnav_mOpen_Children);
    brow_SetAnnotPixmap( node, 1, brow->pixmap_openmap);
    brow_ResetNodraw( brow->ctx);
    brow_Redraw( brow->ctx, node_y);
  }
  return 1;
}

void ItemBaseAttr::close( XNavBrow *brow, double x, double y)
{
  double	node_x, node_y;

  if ( brow_IsOpen( node) & xnav_mOpen_Children) {
    // Children is open, close
    brow_GetNodePosition( node, &node_x, &node_y);
    brow_SetNodraw( brow->ctx);
    brow_CloseNode( brow->ctx, node);
    brow_ResetOpen( node, xnav_mOpen_All);
    brow_ResetNodraw( brow->ctx);
    brow_Redraw( brow->ctx, node_y);
  }
}

ItemAttr::ItemAttr( XNavBrow *brow, pwr_tObjid item_objid,
	brow_tNode dest, flow_eDest dest_code,
        char *attr_name, int attr_type_id, pwr_tTid attr_tid, 
	int attr_size, int attr_flags,
	int item_is_root, item_eDisplayType item_display_type) :
	ItemBaseAttr( item_objid, attr_name,
	attr_type_id, attr_tid, attr_size, attr_flags, item_is_root, item_display_type)
{
  pwr_tOName obj_name;
  pwr_tAName	annot;
  int	sts;

  type = xnav_eItemType_Attr;

  strcpy( name, attr_name);

  switch ( display_type) {
  case item_eDisplayType_Path:
    sts = gdh_ObjidToName( objid, obj_name, sizeof(obj_name), cdh_mNName); 
    if ( EVEN(sts)) throw co_error(sts);
    
    sprintf( annot, "%s.%s", obj_name, attr_name);
    break;
  default: {
      char *s = strrchr( name, '.');
      if ( s)
	s++;
      else
	s = name;
      strcpy( annot, s);
    }
  }
  if ( !is_root)
  {
    brow_CreateNode( brow->ctx, attr_name, brow->nc_attr, 
		dest, dest_code, (void *) this, 1, &node);

    switch( type_id)
    {
      case pwr_eType_Objid:
        brow_SetAnnotPixmap( node, 0, brow->pixmap_ref);
        break;
      case pwr_eType_AttrRef:
        brow_SetAnnotPixmap( node, 0, brow->pixmap_attrref);
        break;
      case pwr_eType_Enum:
        brow_SetAnnotPixmap( node, 0, brow->pixmap_attrenum);
        break;
      case pwr_eType_Mask:
        brow_SetAnnotPixmap( node, 0, brow->pixmap_attrmask);
        break;
      default:
        brow_SetAnnotPixmap( node, 0, brow->pixmap_attr);
    }
    brow_SetAnnotation( node, 0, annot, strlen(annot));
    sts = gdh_ObjidToName( objid, obj_name, sizeof(obj_name), cdh_mName_volumeStrict); 
    if ( EVEN(sts)) throw co_error(sts);

    if ( flags & PWR_MASK_CONST) {
      pwr_tAName aname;
      char buff[512];
      char str[512];
      int len;      

      strcpy( aname, obj_name);
      strcat( aname, ".");
      strcat( aname, attr_name);

      sts = gdh_GetObjectInfo( aname, buff, size);
      ((XNav *)brow->userdata)->attrvalue_to_string( type_id, tid, buff, str, sizeof(str), &len, NULL);
      brow_SetAnnotation( node, 1, str, len);
    }
    else
      brow_SetTraceAttr( node, obj_name, attr_name, flow_eTraceType_User);
  }
}

ItemAttrArray::ItemAttrArray( XNavBrow *brow, pwr_tObjid item_objid,
	brow_tNode dest, flow_eDest dest_code,
	char *attr_name, int attr_elements, int attr_type_id, pwr_tTid attr_tid,
	int attr_size, int attr_flags, int item_is_root) :
	Item( item_objid, item_is_root),
	elements(attr_elements), type_id(attr_type_id), tid(attr_tid),
	size(attr_size), flags(attr_flags)
{
  char *annot;
  type = xnav_eItemType_AttrArray;

  strcpy( name, attr_name);
  if ( !is_root) {
    brow_CreateNode( brow->ctx, attr_name, brow->nc_object, 
		dest, dest_code, (void *) this, 1, &node);

    brow_SetAnnotPixmap( node, 0, brow->pixmap_attrarray);
    if ( (annot = strrchr( name, '.')))
      annot++;
    else
      annot = name;
    brow_SetAnnotation( node, 0, annot, strlen(annot));
  }
}

ItemAttrArrayElem::ItemAttrArrayElem( XNavBrow *brow, pwr_tObjid item_objid,
	brow_tNode dest, flow_eDest dest_code,
	char *attr_name, int attr_element, int attr_type_id, pwr_tTid attr_tid,
	int attr_size, int attr_flags, int item_is_root, item_eDisplayType item_display_type) :
	ItemBaseAttr( item_objid, attr_name,
	attr_type_id, attr_tid, attr_size, attr_flags, item_is_root, item_display_type),
	element(attr_element)
{
  pwr_tOName	obj_name;
  pwr_tAName	annot;
  int	sts;

  type = xnav_eItemType_AttrArrayElem;

  sprintf( &attr[strlen(attr)], "[%d]", element);
  sprintf( name, "%s[%d]", attr_name, element);

  switch( display_type) {
  case item_eDisplayType_Path:
    sts = gdh_ObjidToName( objid, obj_name, sizeof(obj_name), cdh_mNName); 
    if ( EVEN(sts)) throw co_error(sts);
    strcpy( annot, obj_name);
    strcat( annot, ".");
    strcat( annot, name);
    break;
  default: {
      char *s = strrchr( name, '.');
      if ( s)
	s++;
      else
	s = name;
      strcpy( annot, s);
    }
  }
  
  if ( !is_root)
  {
    brow_CreateNode( brow->ctx, name, brow->nc_attr, 
		dest, dest_code, (void *) this, 1, &node);

    brow_SetAnnotPixmap( node, 0, brow->pixmap_attrarrayelem);
    brow_SetAnnotation( node, 0, annot, strlen(annot));
    sts = gdh_ObjidToName( objid, obj_name, sizeof(obj_name), cdh_mName_volumeStrict); 
    if ( EVEN(sts)) throw co_error(sts);
    brow_SetTraceAttr( node, obj_name, name, flow_eTraceType_User);
  }
}

ItemAttrObject::ItemAttrObject( XNavBrow *brow, pwr_tObjid item_objid,
	brow_tNode dest, flow_eDest dest_code,
	char *attr_name, int attr_cid,
	int attr_size, int attr_flags, int attr_element, int item_is_root) :
	Item( item_objid, item_is_root),
	cid(attr_cid), size(attr_size), flags(attr_flags), element(attr_element)
{
  char 		*annot;
  pwr_tAName   	segname;
  pwr_tObjName 	classname;
  pwr_sAttrRef 	aref;
  char 		descr[80];
  pwr_tCid 	classid;
  pwr_tStatus 	sts;
  
  type = xnav_eItemType_AttrObject;
  strcpy( name, attr_name);
  if ( flags & PWR_MASK_ARRAY)
    sprintf( &name[strlen(name)], "[%d]", element);
  if ( !is_root) {
    brow_CreateNode( brow->ctx, attr_name, brow->nc_object, 
		dest, dest_code, (void *) this, 1, &node);

    if ( flags & PWR_MASK_CASTATTR)
      brow_SetAnnotPixmap( node, 0, brow->pixmap_castattr);
    else
      brow_SetAnnotPixmap( node, 0, brow->pixmap_object);
    if ((annot = strrchr( name, '.')))
      annot++;
    else
      annot = name;	 
    brow_SetAnnotation( node, 0, annot, strlen(annot));
  }

  sts = gdh_ObjidToName( objid, segname, sizeof(segname), cdh_mName_volumeStrict);
  if ( EVEN(sts)) throw co_error(sts);

  strcat( segname, ".");
  strcat( segname, name);

  sts = gdh_NameToAttrref( pwr_cNObjid, segname, &aref);
  if ( EVEN(sts)) throw co_error(sts);

  // Set class annotation
  sts = gdh_GetAttrRefTid( &aref, &classid);
  if ( EVEN(sts)) throw co_error(sts);
  sts = gdh_ObjidToName( cdh_ClassIdToObjid( classid),
			 classname, sizeof(classname), cdh_mName_object);
  if ( EVEN(sts)) throw co_error(sts);
  brow_SetAnnotation( node, 1, classname, strlen(classname));

  // Set description annotation
  strcat( segname, ".Description");
  sts = gdh_GetObjectInfo( segname, descr, sizeof(descr));
  if ( ODD(sts))
    brow_SetAnnotation( node, 2, descr, strlen(descr));
}

void ItemAttrObject::close( XNavBrow *brow, double x, double y)
{
  double	node_x, node_y;

  if ( cdh_ObjidIsNull( objid))
    return;

  if ( brow_IsOpen( node))
  {
    // Close
    brow_GetNodePosition( node, &node_x, &node_y);
    brow_SetNodraw( brow->ctx);
    brow_CloseNode( brow->ctx, node);
    if ( brow_IsOpen( node) & xnav_mOpen_Attributes)
      brow_RemoveAnnotPixmap( node, 1);
    if ( brow_IsOpen( node) & xnav_mOpen_Children)
      brow_SetAnnotPixmap( node, 0, brow->pixmap_map);
    brow_ResetOpen( node, xnav_mOpen_All);
    brow_ResetNodraw( brow->ctx);
    brow_Redraw( brow->ctx, node_y);
  }
}

int ItemAttrObject::open_attributes( XNavBrow *brow, double x, double y)
{
  double	node_x, node_y;

  if ( cdh_ObjidIsNull( objid))
    return 1;

  brow_GetNodePosition( node, &node_x, &node_y);

  if ( brow_IsOpen( node) & xnav_mOpen_Attributes)
  {
    // Attributes is open, close
    brow_SetNodraw( brow->ctx);
    brow_CloseNode( brow->ctx, node);
    brow_ResetOpen( node, xnav_mOpen_Attributes);
    brow_RemoveAnnotPixmap( node, 1);
    brow_ResetNodraw( brow->ctx);
    brow_Redraw( brow->ctx, node_y);
  }
  else 
  {
    int			sts;
    unsigned long	elements;
    Item 	*item;
    int		attr_exist;
    int		i;
    gdh_sAttrDef *bd;
    int 	rows;
    pwr_tAName	attr_name;

    if ( brow_IsOpen( node) & xnav_mOpen_Children ||
	 brow_IsOpen( node) & xnav_mOpen_Crossref)
    {
      // Close children first
      brow_SetNodraw( brow->ctx);
      brow_CloseNode( brow->ctx, node);
      brow_ResetOpen( node, xnav_mOpen_Children);
      brow_ResetOpen( node, xnav_mOpen_Crossref);
      brow_SetAnnotPixmap( node, 0, brow->pixmap_map);
      brow_ResetNodraw( brow->ctx);
      brow_Redraw( brow->ctx, node_y);
    }

    // Create some attributes
    brow_SetNodraw( brow->ctx);

    if ( brow->usertype == brow_eUserType_XNav && ((XNav *)brow->userdata)->gbl.show_truedb)
      sts = gdh_GetTrueObjectBodyDef( cid, &bd, &rows);
    else
      sts = gdh_GetObjectBodyDef( cid, &bd, &rows, objid);
    if ( EVEN(sts)) return sts;

    for ( i = 0; i < rows; i++) {
      if ( bd[i].flags & gdh_mAttrDef_Shadowed)
	continue;
      if ( bd[i].attr->Param.Info.Flags & PWR_MASK_RTVIRTUAL || 
	   bd[i].attr->Param.Info.Flags & PWR_MASK_PRIVATE)
	continue;
      if ( (bd[i].attr->Param.Info.Type == pwr_eType_CastId ||
	    bd[i].attr->Param.Info.Type == pwr_eType_DisableAttr) && 
	   ((brow->usertype == brow_eUserType_XNav && 
	     !((XNav *)brow->userdata)->gbl.show_truedb) ||
	    brow->usertype == brow_eUserType_XAttNav))
	continue;
      if ( bd[i].attr->Param.Info.Flags & PWR_MASK_RTHIDE &&
	   ((brow->usertype == brow_eUserType_XNav && 
	     !((XNav *)brow->userdata)->gbl.show_allattr) ||
	    brow->usertype == brow_eUserType_XAttNav))
	continue;

      strcpy( attr_name, name);
      strcat( attr_name, ".");
      strcat( attr_name, bd[i].attrName);

      if ( bd[i].attr->Param.Info.Flags & PWR_MASK_DISABLEATTR &&
	   ((brow->usertype == brow_eUserType_XNav && 
	     !((XNav *)brow->userdata)->gbl.show_truedb) ||
	    brow->usertype == brow_eUserType_XAttNav)) {
	pwr_sAttrRef aref = cdh_ObjidToAref( objid);
	pwr_sAttrRef aaref;
	pwr_tDisableAttr disabled;

	sts = gdh_ArefANameToAref( &aref, attr_name, &aaref);
	if ( EVEN(sts)) return sts;

	sts = gdh_ArefDisabled( &aaref, &disabled);
	if ( EVEN(sts)) return sts;

	if ( disabled)
	  continue;
      }

      elements = 1;
      if ( bd[i].attr->Param.Info.Flags & PWR_MASK_ARRAY ) {
	attr_exist = 1;
	item = (Item *) new ItemAttrArray( brow, objid, node, 
					   flow_eDest_IntoLast,
					   attr_name,
					   bd[i].attr->Param.Info.Elements, 
					   bd[i].attr->Param.Info.Type, 
					   bd[i].attr->Param.TypeRef, 
					   bd[i].attr->Param.Info.Size,
					   bd[i].attr->Param.Info.Flags, 0);
      }
      else if ( bd[i].attr->Param.Info.Flags & PWR_MASK_CLASS ) {
	attr_exist = 1;
	item = (Item *) new ItemAttrObject( brow, objid, node, 
					   flow_eDest_IntoLast,
					   attr_name,
					   bd[i].attr->Param.TypeRef,
					   bd[i].attr->Param.Info.Size,
					   bd[i].attr->Param.Info.Flags, 0, 0);
      }
      else {
	attr_exist = 1;
	item = (Item *) new ItemAttr( brow, objid, node, 
				      flow_eDest_IntoLast, 
				      attr_name,
				      bd[i].attr->Param.Info.Type,
				      bd[i].attr->Param.TypeRef,
				      bd[i].attr->Param.Info.Size, 
				      bd[i].attr->Param.Info.Flags, 0, 
				      item_eDisplayType_Attr);
      } 
    }
    free( (char *)bd);
    
    if ( attr_exist && !is_root) {
      brow_SetOpen( node, xnav_mOpen_Attributes);
      brow_SetAnnotPixmap( node, 1, brow->pixmap_openattr);
    }
    brow_ResetNodraw( brow->ctx);
    if ( attr_exist)
      brow_Redraw( brow->ctx, node_y);
  }
  return 1;
}

int ItemAttrObject::open_crossref( XNavBrow *brow, double x, double y)
{
  double	node_x, node_y;
  int		crossref_exist;
  int		sts;
  XNav 		*xnav;
  char		aname[240];
  char		file[20] = "*";

  if ( brow->usertype == brow_eUserType_XNav)
    xnav = (XNav *) brow->userdata;
  else
    xnav = 0;

  if ( cdh_ObjidIsNull( objid))
    return 1;

  if ( !is_root)
    brow_GetNodePosition( node, &node_x, &node_y);
  else 
    node_y = 0;

  if ( !is_root && brow_IsOpen( node))
  {
    // Close
    brow_SetNodraw( brow->ctx);
    brow_CloseNode( brow->ctx, node);
    if ( brow_IsOpen( node) & xnav_mOpen_Attributes)
      brow_RemoveAnnotPixmap( node, 1);
    if ( brow_IsOpen( node) & xnav_mOpen_Children)
      brow_SetAnnotPixmap( node, 0, brow->pixmap_map);
    brow_ResetOpen( node, xnav_mOpen_All);
    brow_ResetNodraw( brow->ctx);
    brow_Redraw( brow->ctx, node_y);
  }
  else
  {
    // Fetch the cross reference list
    crossref_exist = 0;
    brow_SetNodraw( brow->ctx);

    sts = gdh_ObjidToName( objid, aname, sizeof(aname), 
			   cdh_mName_path | cdh_mName_object);
    if ( EVEN(sts)) return sts;

    strcat( aname, ".");
    strcat( aname, name);

    switch ( cid)
    {
      case pwr_cClass_Di:
      case pwr_cClass_Dv:
      case pwr_cClass_Do:
      case pwr_cClass_Po:
      case pwr_cClass_Av:
      case pwr_cClass_Ai:
      case pwr_cClass_Ao:
      case pwr_cClass_Iv:
      case pwr_cClass_Ii:
      case pwr_cClass_Io:
      case pwr_cClass_Co:
        sts = xnav_crr_signal( brow, file, aname, node);
        break;
      default:
        sts = xnav_crr_object( brow, file, aname, node);
    }
    if ( xnav && sts == NAV__OBJECTNOTFOUND)
      xnav->message('E', "Object not found in crossreferens file");
    else if ( xnav && sts == NAV__NOCROSSREF)
      xnav->message('I', "There is no crossreferences for this object");
    else if ( ODD(sts))
    {
      brow_SetOpen( node, xnav_mOpen_Crossref);
      crossref_exist = 1;
    }
    brow_ResetNodraw( brow->ctx);
    if ( crossref_exist)
      brow_Redraw( brow->ctx, node_y);
  }
  return 1;
}

ItemHeader::ItemHeader( XNavBrow *brow, const char *item_name, const char *title,
	brow_tNode dest, flow_eDest dest_code) :
	Item( pwr_cNObjid, 0)
{
  type = xnav_eItemType_Header;
  strcpy( name, item_name);
  brow_CreateNode( brow->ctx, "header", brow->nc_header,
		dest, dest_code, (void *)this, 1, &node);
  brow_SetAnnotation( node, 0, title, strlen(title));
}

ItemHeaderLarge::ItemHeaderLarge( XNavBrow *brow, const char *item_name, const char *title,
	brow_tNode dest, flow_eDest dest_code) :
	Item( pwr_cNObjid, 0)
{
  type = xnav_eItemType_HeaderLarge;
  strcpy( name, item_name);
  brow_CreateNode( brow->ctx, "header", brow->nc_headerlarge,
		dest, dest_code, (void *)this, 1, &node);
  brow_SetAnnotation( node, 0, title, strlen(title));
}


ItemTableHeader::ItemTableHeader( XNavBrow *brow ,XNav *tab_xnav, 
	const char *item_name, 
	item_sTableHeader *title,
	brow_tNode dest, flow_eDest dest_code) :
	Item( pwr_cNObjid, 0), xnav(tab_xnav), scan(NULL), disconnect(NULL)
{
  type = xnav_eItemType_TableHeader;
  strcpy( name, item_name);
  brow_CreateNode( brow->ctx, "header", brow->nc_table_header,
		dest, dest_code, (void *)this, 0, &node);
  for ( int i = 0; i < title->table_cnt; i++)
  {
    if ( title->title[i][0])
      brow_SetAnnotation( node, i, title->title[i], strlen(title->title[i]));
  }
  brow_SetTraceAttr( node, "Yes", "", flow_eTraceType_User);
}

ItemText::ItemText( XNavBrow *brow, const char *item_name, const char *text,
	brow_tNode dest, flow_eDest dest_code) :
	Item( pwr_cNObjid, 0)
{
  type = xnav_eItemType_Text;
  strcpy( name, item_name);
  brow_CreateNode( brow->ctx, "text", brow->nc_object,
		dest, dest_code, (void *)this, 1, &node);
  brow_SetAnnotation( node, 0, text, strlen(text));
}

ItemHelpHeader::ItemHelpHeader( XNavBrow *brow, const char *item_name, const char *title,
	brow_tNode dest, flow_eDest dest_code) :
	Item( pwr_cNObjid, 0)
{
  type = xnav_eItemType_HelpHeader;
  strcpy( name, item_name);
  brow_CreateNode( brow->ctx, "header", brow->nc_header,
		dest, dest_code, (void *)this, 1, &node);
  brow_SetAnnotation( node, 0, title, strlen(title));
  brow_SetAnnotPixmap( node, 0, brow->pixmap_closehelp);
}

void ItemHelpHeader::close( XNavBrow *brow, double x, double y)
{
  XNav *xnav;

  if ( brow->usertype == brow_eUserType_XNav)
    xnav = (XNav *) brow->userdata;
  else
    xnav = 0;

  if ( xnav)
    xnav->brow_push();
}

ItemHelp::ItemHelp( XNavBrow *brow, const char *item_name, const char *text, const char *text2, 
	const char *text3, const char *item_link, const char *item_bookmark, 
	const char *item_file_name, navh_eHelpFile help_file_type, int help_index, brow_tNode dest, flow_eDest dest_code) :
	Item( pwr_cNObjid, 0), file_type(help_file_type), index(help_index)
{
  type = xnav_eItemType_Help;
  strcpy( name, item_name);
  strcpy( link, item_link);
  strcpy( bookmark, item_bookmark);
  if ( item_file_name)
    strcpy( file_name, item_file_name);
  else
    strcpy( file_name, "");
  brow_CreateNode( brow->ctx, "help", brow->nc_text,
		dest, dest_code, (void *)this, 1, &node);
  brow_SetAnnotation( node, 0, text, strlen(text));
  if ( text2[0] != 0)
    brow_SetAnnotation( node, 1, text2, strlen(text2));
  if ( text3[0] != 0)
    brow_SetAnnotation( node, 2, text3, strlen(text3));
  if ( link[0] != 0 || index)
    brow_SetAnnotPixmap( node, 0, brow->pixmap_morehelp);
}

int ItemHelp::open_children( XNavBrow *brow, double x, double y)
{
  if ( brow->usertype != brow_eUserType_XNav)
    return 0;

  XNav *xnav = (XNav *) brow->userdata;
  int sts;

  if ( index)
  {
    sts = xnav->help_index( file_type, file_name, 1);
  }
  else if ( strcmp( link, "") != 0)
  {
    if ( strncmp( link, "$web:", 5) == 0) {
      // Open the url
      xnav_open_URL( &link[5]);
    } 
    else if ( (strstr( link, ".htm") != 0) || 
	      (strstr( link, ".pdf") != 0)) {
      // Open the url
      xnav_open_URL( link);
    }
    else {
      if ( file_name[0] == 0)
      {
        sts = xnav->help( link, bookmark, navh_eHelpFile_Base, NULL, 1);
        if (EVEN(sts))
          sts = xnav->help( link, bookmark, navh_eHelpFile_Project, NULL, 1);
      }
      else
        sts = xnav->help( link, bookmark, navh_eHelpFile_Other, file_name, 1);
    }
  }
  return 1;
}

ItemHelpBold::ItemHelpBold( XNavBrow *brow, const char *item_name, const char *text, const char *text2, 
	const char *text3, const char *item_link, const char *item_bookmark, 
	const char *item_file_name, navh_eHelpFile help_file_type, int help_index, brow_tNode dest, flow_eDest dest_code) :
	Item( pwr_cNObjid, 0), file_type(help_file_type), index(help_index)
{
  type = xnav_eItemType_HelpBold;
  strcpy( name, item_name);
  strcpy( link, item_link);
  strcpy( bookmark, item_bookmark);
  if ( item_file_name)
    strcpy( file_name, item_file_name);
  else
    strcpy( file_name, "");
  brow_CreateNode( brow->ctx, "help", brow->nc_object,
		dest, dest_code, (void *)this, 1, &node);
  brow_SetAnnotation( node, 0, text, strlen(text));
  if ( text2[0] != 0)
    brow_SetAnnotation( node, 1, text2, strlen(text2));
  if ( text3[0] != 0)
    brow_SetAnnotation( node, 2, text3, strlen(text3));
  if ( link[0] != 0 || index)
    brow_SetAnnotPixmap( node, 0, brow->pixmap_morehelp);
}

int ItemHelpBold::open_children( XNavBrow *brow, double x, double y)
{
  if ( brow->usertype != brow_eUserType_XNav)
    return 0;

  XNav *xnav = (XNav *) brow->userdata;
  int sts;

  if ( index)
  {
    sts = xnav->help_index( file_type, file_name, 1);
  }
  else if ( strcmp( link, "") != 0)
  {
    if ( strncmp( link, "$web:", 5) == 0) {
      // Open the url
      xnav_open_URL( &link[5]);
    } 
    else if ( (strstr( link, ".htm") != 0) || 
	      (strstr( link, ".pdf") != 0)) {
      // Open the url
      xnav_open_URL( link);
    }
    else {
      if ( file_name[0] == 0)
      {
        sts = xnav->help( link, bookmark, navh_eHelpFile_Base, NULL, 1);
        if (EVEN(sts))
          sts = xnav->help( link, bookmark, navh_eHelpFile_Project, NULL, 1);
      }
      else
        sts = xnav->help( link, bookmark, navh_eHelpFile_Other, file_name, 1);
    }
  }
  return 1;
}

ItemFile::ItemFile( XNavBrow *brow, const char *item_name, const char *text, 
	const char *item_file_name, item_eFileType item_filetype,
	brow_tNode dest, flow_eDest dest_code) :
	Item( pwr_cNObjid, 0), file_type(item_filetype)
{

  type = xnav_eItemType_File;
  strcpy( name, item_name);
  strcpy( file_name, item_file_name);

  brow_CreateNode( brow->ctx, name, brow->nc_object,
		dest, dest_code, (void *)this, 1, &node);
  if ( file_type == item_eFileType_Script)
    brow_SetAnnotPixmap( node, 0, brow->pixmap_script);
  else if ( file_type == item_eFileType_Graph) {
    strncpy( name, text, sizeof(name));
    brow_SetAnnotPixmap( node, 0, brow->pixmap_graph);
  }
  else if ( file_type == item_eFileType_RttLog)
    brow_SetAnnotPixmap( node, 0, brow->pixmap_curve);
  else
    brow_SetAnnotPixmap( node, 0, brow->pixmap_file);
  brow_SetAnnotation( node, 0, text, strlen(text));
}

int ItemFile::open_children( XNavBrow *brow, double x, double y)
{
  if ( brow->usertype != brow_eUserType_XNav)
    return 0;

  XNav *xnav = (XNav *) brow->userdata;
  int sts;

  switch ( file_type)
  {
    case item_eFileType_Script:
    {
      pwr_tCmd cmd;
      strcpy( cmd, "@");
      strcat( cmd, file_name);
      sts = xnav->command( cmd);
      break;
    }
    case item_eFileType_Graph:
      xnav->open_graph( name, file_name, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
      break;
    case item_eFileType_RttLog:
      xnav->open_rttlog( name, file_name);
      break;
    default:
      xnav->message('I', "Unknown filetype");
  }
  return 1;
}


ItemCollect::ItemCollect( XNavBrow *brow, pwr_tObjid item_objid, char *attr_name,
	brow_tNode dest, flow_eDest dest_code, int attr_type_id, pwr_tTid attr_tid,
	int attr_size, int item_is_root) :
	ItemBaseAttr( item_objid, attr_name,
	attr_type_id, attr_tid, attr_size, 0, item_is_root, item_eDisplayType_Path)
{
  int sts;
  pwr_tOName obj_name;
  pwr_tObjid tst_objid;

  type = xnav_eItemType_Collect;
  if ( !is_root)
  {
    sts = gdh_ObjidToName( objid, obj_name, sizeof(obj_name), cdh_mNName);
    if ( EVEN(sts)) throw co_error(sts);
    sts = gdh_NameToObjid( obj_name, &tst_objid);
    if ( EVEN(sts)) {
      // Try volume name
      sts = gdh_ObjidToName( objid, obj_name, sizeof(obj_name), 
			     cdh_mName_volumeStrict);
      if ( EVEN(sts)) return;
    }

    strcpy( name, obj_name);
    strcat( name, ".");
    strcat( name, attr);

    brow_CreateNode( brow->ctx, "collect", brow->nc_attr, 
		dest, dest_code, (void *)this, 1, &node);

    // Set pixmap
    brow_SetAnnotPixmap( node, 0, brow->pixmap_leaf);

    // Set object name annotation
    brow_SetAnnotation( node, 0, name, strlen(name));

    brow_SetTraceAttr( node, obj_name, attr, flow_eTraceType_User);
  }
}

static int add_window( XNav *xnav, pwr_tObjid objid, brow_tNode node, int *child_exist)
{
  pwr_tObjid child;
  int sts;
  Item *item;
  pwr_tClassId classid;

  sts = gdh_GetChild( objid, &child);
  while ( ODD(sts))
  {
    
    sts = gdh_GetObjectClass( child, &classid);
    if ( EVEN(sts)) return sts;

    if ( classid == pwr_cClass_windowplc ||
         classid == pwr_cClass_windowcond ||
         classid == pwr_cClass_windoworderact ||
         classid == pwr_cClass_windowsubstep)
    {
      *child_exist = 1;
      sts = xnav->create_object_item( child, node, flow_eDest_IntoLast,
		(void **)&item, 0);
      if ( EVEN(sts)) return sts;
    }
    sts = add_window( xnav, child, node, child_exist);
    if ( EVEN(sts)) return sts;

    sts = gdh_GetNextSibling( child, &child);
  }
  return 1;
}

int ItemPlc::open_children( XNavBrow *brow, double x, double y)
{
  if ( brow->usertype != brow_eUserType_XNav)
    return 0;

  XNav *xnav = (XNav *) brow->userdata;
  double	node_x, node_y;
  int		child_exist;

  if ( !is_root)
  {
    brow_GetNodePosition( node, &node_x, &node_y);

    if ( brow_IsOpen( node))
    {
      // Close
      close( brow, 0, 0);
    }
    else
    {
      // Create some children
      brow_SetNodraw( brow->ctx);

      child_exist = 0;
      add_window( xnav, objid, node, &child_exist);

      if ( child_exist && !is_root)
      {
        brow_SetOpen( node, xnav_mOpen_Children);
        brow_SetAnnotPixmap( node, 0, brow->pixmap_openmap);
      }
      brow_ResetNodraw( brow->ctx);
      if ( child_exist)
        brow_Redraw( brow->ctx, node_y);
    }
  }
  return 1;
}


ItemMenu::ItemMenu( XNavBrow *brow, const char *item_name, 
	brow_tNode dest, flow_eDest dest_code, xnav_sMenu **item_child_list,
	int item_is_root) :
	Item( pwr_cNObjid, item_is_root), child_list(item_child_list)
{
  type = xnav_eItemType_Menu;
  strcpy( name, item_name);
  if ( !is_root)
  {
    brow_CreateNode( brow->ctx, name, brow->nc_object,
		dest, dest_code, (void *)this, 1, &node);

    // Set pixmap
    if ( *child_list)
      brow_SetAnnotPixmap( node, 0, brow->pixmap_map);
    else
      brow_SetAnnotPixmap( node, 0, brow->pixmap_leaf);
    // Set object name annotation
    brow_SetAnnotation( node, 0, name, strlen(name));
  }
}

int ItemMenu::open_children( XNavBrow *brow, double x, double y)
{
  int		action_open = 0;

  if ( !is_root)
  {
    if ( !brow_IsOpen( node))
      action_open = 1;
  }
  if ( action_open || is_root)
  {
    // Display childlist
    Item 		*item;
    xnav_sMenu		*menu;
    flow_sAnnotPixmap   *pixmap;

    brow_SetNodraw( brow->ctx);
    menu = *child_list;
    while ( menu)
    {
      switch ( menu->pixmap) {
      case menu_ePixmap_Map:
	pixmap = brow->pixmap_map;
	break;
      case menu_ePixmap_Graph:
	pixmap = brow->pixmap_graph;
	break;
      case menu_ePixmap_Script:
	pixmap = brow->pixmap_script;
	break;
      case menu_ePixmap_List:
	pixmap = brow->pixmap_list;
	break;
      case menu_ePixmap_Leaf:
	pixmap = brow->pixmap_leaf;
	break;
      default:
	pixmap = brow->pixmap_map;
      }

      switch ( menu->item_type) {
      case xnav_eItemType_Menu:
	item = (Item *) new ItemMenu( brow, menu->title, node, 
				      flow_eDest_IntoLast, &menu->child_list,
				      0);
	break;
      case xnav_eItemType_Command:
	item = (Item *) new ItemCommand( brow, menu->title, 0, node, 
					 flow_eDest_IntoLast, menu->command, 0,
					 pixmap);
	break;
      default:
	;
      }
      menu = menu->next;
      if ( !is_root) {
        brow_SetOpen( node, xnav_mOpen_Children);
        brow_SetAnnotPixmap( node, 0, brow->pixmap_openmap);
      }
    }
    brow_ResetNodraw( brow->ctx);
    brow_Redraw( brow->ctx, 0);
  }
  else
    close( brow, x, y);
  return 1;
}

void ItemMenu::close( XNavBrow *brow, double x, double y)
{
  double	node_x, node_y;

  if ( brow_IsOpen( node))
  {
    // Close
    brow_GetNodePosition( node, &node_x, &node_y);
    brow_SetNodraw( brow->ctx);
    brow_CloseNode( brow->ctx, node);
    if ( brow_IsOpen( node) & xnav_mOpen_Attributes)
      brow_RemoveAnnotPixmap( node, 1);
    if ( brow_IsOpen( node) & xnav_mOpen_Children)
      brow_SetAnnotPixmap( node, 0, brow->pixmap_map);
    brow_ResetOpen( node, xnav_mOpen_All);
    brow_ResetNodraw( brow->ctx);
    brow_Redraw( brow->ctx, node_y);
  }
}

ItemCommand::ItemCommand( XNavBrow *brow, char *item_name, char *item_text,
	brow_tNode dest, flow_eDest dest_code, char *item_command,
	int item_is_root, flow_sAnnotPixmap *pixmap) :
	Item( pwr_cNObjid, item_is_root)
{
  type = xnav_eItemType_Command;

  strcpy( command, item_command);
  strcpy( name, item_name);
  if ( !is_root)
  {
    brow_CreateNode( brow->ctx, "command_item", brow->nc_object,
		dest, dest_code, (void *)this, 1, &node);

    // Set pixmap
    brow_SetAnnotPixmap( node, 0, pixmap);
    // Set object name annotation
    brow_SetAnnotation( node, 0, name, strlen(name));
    if ( item_text != 0)
      brow_SetAnnotation( node, 1, item_text, strlen(item_text));
  }
}

int ItemCommand::open_children( XNavBrow *brow, double x, double y)
{
  if ( brow->usertype != brow_eUserType_XNav)
    return 0;

  XNav *xnav = (XNav *) brow->userdata;

  xnav->command( command);
  return 1;
}


ItemCrossref::ItemCrossref( XNavBrow *brow, char *item_ref_name, 
	char *item_ref_class, int item_write, brow_tNode dest, 
	flow_eDest dest_code) :
	Item( pwr_cNObjid, 0), write(item_write)
{
  int sts;
  pwr_tOName window_name;
  char *s;
  pwr_tCid cid;

  type = xnav_eItemType_Crossref;

  strcpy( name, item_ref_name);
  strcpy( ref_class, item_ref_class);

  // Get window objid
  strcpy( window_name, name);
  s = strrchr( window_name, '-');
  if ( !s)
    return;

  strcpy( ref_name, s + 1);
  *s = 0;
  sts = gdh_NameToObjid( window_name, &objid);
  if ( EVEN(sts)) return;

  sts = gdh_GetObjectClass( objid, &cid);
  if ( EVEN(sts)) return;

  switch ( cid) {
  case pwr_cClass_windowplc:
  case pwr_cClass_windowcond:
  case pwr_cClass_windoworderact:
  case pwr_cClass_windowsubstep:
    break;
  default:
    // No plc object, use original name
    sts = gdh_NameToObjid( name, &objid);
    if ( EVEN(sts)) return;
  }

  brow_CreateNode( brow->ctx, "crr", brow->nc_object, 
		dest, dest_code, (void *) this, 1, &node);

//  brow_SetAnnotPixmap( node, 0, brow->pixmap_ref);
  if ( write == 1)
    brow_SetAnnotPixmap( node, 0, brow->pixmap_crrwrite);
  else if ( write == 2)
  {
    brow_SetAnnotPixmap( node, 0, brow->pixmap_crrwrite);
    brow_SetAnnotPixmap( node, 1, brow->pixmap_crrread);
  }
  else
    brow_SetAnnotPixmap( node, 0, brow->pixmap_crrread);

  brow_SetAnnotation( node, 0, name, strlen(name));

  // Set ref_class annotation
  brow_SetAnnotation( node, 1, ref_class, strlen(ref_class));
}

int ItemCrossref::open_trace( XNavBrow *brow, double x, double y)
{
  if ( brow->usertype != brow_eUserType_XNav)
    return 0;

  XNav *xnav = (XNav *) brow->userdata;

  xnav->start_trace( objid, ref_name);
  return 1;
}


ItemLocal::ItemLocal( XNavBrow *brow, const char *item_name, const char *attr, 
	int attr_type, int attr_size, double attr_min_limit, 
	double attr_max_limit, int attr_nochange,
	void *attr_value_p, brow_tNode dest, flow_eDest dest_code) :
	Item( pwr_cNObjid, 0), value_p(attr_value_p), first_scan(1), 
	type_id(attr_type), size(attr_size), 
	min_limit(attr_min_limit), max_limit(attr_max_limit),
        nochange(attr_nochange)
{

  type = xnav_eItemType_Local;

  strcpy( name, item_name);
  memset( old_value, 0, sizeof(old_value));

//  sts = xnav->ldb.add( attr, attr_type, value_p);
//  if (EVEN(sts)) return;

  brow_CreateNode( brow->ctx, item_name, brow->nc_attr, 
		dest, dest_code, (void *) this, 1, &node);

  brow_SetAnnotPixmap( node, 0, brow->pixmap_attr);

  brow_SetAnnotation( node, 0, item_name, strlen(item_name));
  brow_SetTraceAttr( node, attr, "", flow_eTraceType_User);
}

ItemObjectStruct::ItemObjectStruct( XNavBrow *brow, char *item_name, 
	char *attr, 
	int attr_type, int attr_size, int attr_nochange,
	void *attr_value_p, pwr_tObjid attr_objid, pwr_tRefId attr_subid,
	brow_tNode dest, flow_eDest dest_code) :
	Item( attr_objid, 0), value_p(attr_value_p), first_scan(1), 
	type_id(attr_type), size(attr_size), 
        nochange(attr_nochange), subid(attr_subid)
{
  type = xnav_eItemType_ObjectStruct;

  strcpy( name, item_name);
  memset( old_value, 0, sizeof(old_value));

  brow_CreateNode( brow->ctx, item_name, brow->nc_attr, 
		dest, dest_code, (void *) this, 1, &node);

  brow_SetAnnotPixmap( node, 0, brow->pixmap_attr);

  brow_SetAnnotation( node, 0, item_name, strlen(item_name));
  brow_SetTraceAttr( node, attr, "", flow_eTraceType_User);
}

ItemTable::ItemTable( XNavBrow *brow, XNav *tab_xnav, pwr_tObjid objid, 
        item_sTable *table_columns, 
	item_sTableSubid *table_subid,
	int tab_change_value_idx,
	double tab_min_limit, double tab_max_limit, int relative_pos,
	brow_tNode dest, flow_eDest dest_code) :
	ItemBaseObject( objid, 0), xnav(tab_xnav), change_value_idx(tab_change_value_idx),
	min_limit(tab_min_limit), max_limit(tab_max_limit), 
	scan(NULL), disconnect(NULL), first_scan(1)
{
  int		sts;
  pwr_tObjid	child;

  type = xnav_eItemType_Table;

  memset( old_value, 0, sizeof(old_value));
  memcpy( &col, table_columns, sizeof(col));
  memcpy( &subid, table_subid, sizeof(subid));

  brow_CreateNode( brow->ctx, "Table", brow->nc_table, 
		dest, dest_code, (void *) this, relative_pos, &node);

  if ( cdh_ObjidIsNotNull( objid))
  {
    // Set pixmap
    sts = gdh_GetChild( objid, &child);
    if( ODD(sts))
      brow_SetAnnotPixmap( node, 0, brow->pixmap_map);
    else
      brow_SetAnnotPixmap( node, 0, brow->pixmap_leaf);
  }
  brow_SetTraceAttr( node, "Yes", "", flow_eTraceType_User);

}

int ItemDevice::open_children( XNavBrow *brow, double x, double y)
{
  if ( brow->usertype != brow_eUserType_XNav)
    return 0;

  XNav *xnav = (XNav *) brow->userdata;

  if ( cdh_ObjidIsNull( objid))
    return 1;

  return xnav->show_channels( objid);
}

int ItemChannel::open_children( XNavBrow *brow, double x, double y)
{
  if ( brow->usertype != brow_eUserType_XNav)
    return 0;

  XNav *xnav = (XNav *) brow->userdata;

  if ( cdh_ObjidIsNull( objid) || cdh_ObjidIsNull( signal_aref.Objid))
    return 1;

  return xnav->show_object( &signal_aref, node);
}

int ItemChannel::open_crossref( XNavBrow *brow, double x, double y)
{
  double	node_x, node_y;
  int		crossref_exist;
  int		sts;
  pwr_tOName   	signal_name;
  XNav 		*xnav;
  char		file[20] = "*";

  if ( brow->usertype == brow_eUserType_XNav)
    xnav = (XNav *) brow->userdata;
  else
    xnav = 0;

  if ( cdh_ObjidIsNull( signal_aref.Objid))
    return 1;

  if ( !is_root)
    brow_GetNodePosition( node, &node_x, &node_y);
  else 
    node_y = 0;

  if ( !is_root && brow_IsOpen( node))
  {
    // Close
    brow_SetNodraw( brow->ctx);
    brow_CloseNode( brow->ctx, node);
    if ( brow_IsOpen( node) & xnav_mOpen_Attributes)
      brow_RemoveAnnotPixmap( node, 1);
    if ( brow_IsOpen( node) & xnav_mOpen_Children)
      brow_SetAnnotPixmap( node, 0, brow->pixmap_map);
    brow_ResetOpen( node, xnav_mOpen_All);
    brow_ResetNodraw( brow->ctx);
    brow_Redraw( brow->ctx, node_y);
  }
  else
  {
    // Fetch the cross reference list
    crossref_exist = 0;
    brow_SetNodraw( brow->ctx);

    sts = gdh_AttrrefToName( &signal_aref, signal_name, sizeof(signal_name), 
		cdh_mNName);

    sts = xnav_crr_signal( brow, file, signal_name, node);

    if ( sts == NAV__OBJECTNOTFOUND && xnav)
      xnav->message('E', "Object not found in crossreferens file");
    else if ( sts == NAV__NOCROSSREF && xnav)
      xnav->message('I', "There is no crossreferences for this object");
    else if ( ODD(sts))
    {
      brow_SetOpen( node, xnav_mOpen_Crossref);
      crossref_exist = 1;
    }
    brow_ResetNodraw( brow->ctx);
    if ( crossref_exist)
      brow_Redraw( brow->ctx, node_y);
  }
  return 1;
}


int ItemRemNode::open_children( XNavBrow *brow, double x, double y)
{
  if ( brow->usertype != brow_eUserType_XNav)
    return 0;

  XNav *xnav = (XNav *) brow->userdata;

  return xnav->show_remtrans( objid);
}

int ItemRemTrans::open_children( XNavBrow *brow, double x, double y)
{
  if ( brow->usertype != brow_eUserType_XNav)
    return 0;

  XNav *xnav = (XNav *) brow->userdata;
  pwr_tOName    remtrans_name;
  pwr_tAName   	namebuf;
  char		structname[40];
  char		structfile[80];
  int           sts;
  pwr_tObjid    child;
  char          *msg;

  // Open buffer object as a struct, get file and struct
  sts = gdh_ObjidToName( objid, remtrans_name, sizeof(remtrans_name), 
			cdh_mNName);
  if (EVEN(sts)) return sts;

  strcpy( namebuf, remtrans_name);
  strcat( namebuf, ".StructName");
  sts = gdh_GetObjectInfo ( namebuf,
		(pwr_tAddress *) structname,
		sizeof(structname));
  if ( ODD(sts) && (strcmp(structname, "") != 0)) {
    strcpy( namebuf, remtrans_name);
    strcat( namebuf, ".StructFile");
    sts = gdh_GetObjectInfo ( namebuf,
		(pwr_tAddress *) structfile,
		sizeof(structfile));
    if ( ODD(sts) && (strcmp(structfile, "") != 0)) {
      sts = gdh_GetChild( objid, &child);
      if ( ODD(sts)) {
        sts = xnav->show_object_as_struct( child, structname, 
		structfile);
        if ( EVEN(sts) && dcli_readstruct_get_message( &msg))
          xnav->message( 'E', msg);
        if ( ODD(sts))
	  return sts;
      }
    }
  }

  // Not successfull, show childobjects
  return ItemTable::open_children( brow, x, y);
}

ItemEnum::ItemEnum( 
	XNavBrow *brow,
	pwr_tObjid item_objid, 
	brow_tNode dest, flow_eDest dest_code,
	char *attr_enum_name, char *attr_name, 
	int attr_type_id, pwr_tTid attr_tid, 
	int attr_size, int attr_flags,
	unsigned int item_num, int item_is_element, int item_element) :
	ItemBaseAttr( item_objid, attr_name,
	attr_type_id, attr_tid, attr_size, attr_flags, 0, item_eDisplayType_Attr),
	num(item_num), is_element(item_is_element), element(item_element)
{
  pwr_tOName obj_name;
  pwr_tStatus sts;

  type = xnav_eItemType_Enum;
  sprintf( name, "%s%u", attr_name, num);
  strcpy( enum_name, attr_enum_name);

  brow_CreateNode( brow->ctx, enum_name, brow->nc_enum, 
		dest, dest_code, (void *) this, 1, &node);

  brow_SetAnnotPixmap( node, 0, brow->pixmap_attr);
  brow_SetAnnotation( node, 0, enum_name, strlen(enum_name));

  sts = gdh_ObjidToName( objid, obj_name, sizeof(obj_name), cdh_mName_volumeStrict); 
  if ( EVEN(sts)) throw co_error(sts);
 
  brow_SetTraceAttr( node, obj_name, attr_name, flow_eTraceType_User);
}

int ItemEnum::set_value()
{
  pwr_tStatus sts;
  pwr_tAName name;

  sts = gdh_ObjidToName( objid, name, sizeof(name), cdh_mName_volumeStrict); 
  if ( EVEN(sts)) return sts;

  strcat( name, ".");
  strcat( name, attr);

  sts = gdh_SetObjectInfo( name, &num, sizeof(pwr_tEnum));
  return sts;
}

ItemMask::ItemMask( 
	XNavBrow *brow,
	pwr_tObjid item_objid, 
	brow_tNode dest, flow_eDest dest_code,
	char *attr_mask_name, char *attr_name, 
	int attr_type_id, pwr_tTid attr_tid, 
	int attr_size, int attr_flags,
	unsigned int item_num, int item_is_element, int item_element) :
	ItemBaseAttr( item_objid, attr_name,
	attr_type_id, attr_tid, attr_size, attr_flags, 0, item_eDisplayType_Attr),
	num(item_num), is_element(item_is_element), element(item_element)
{
  pwr_tOName obj_name;
  pwr_tStatus sts;

  type = xnav_eItemType_Mask;
  sprintf( name, "%s%u", attr_name, num);
  strcpy( mask_name, attr_mask_name);

  brow_CreateNode( brow->ctx, mask_name, brow->nc_enum, 
		dest, dest_code, (void *) this, 1, &node);

  brow_SetAnnotPixmap( node, 0, brow->pixmap_attr);
  brow_SetAnnotation( node, 0, mask_name, strlen(mask_name));

  sts = gdh_ObjidToName( objid, obj_name, sizeof(obj_name), cdh_mName_volumeStrict); 
  if ( EVEN(sts)) throw co_error(sts);
 
  brow_SetTraceAttr( node, obj_name, attr_name, flow_eTraceType_User);
}

int ItemMask::set_value( int bittrue)
{
  pwr_tStatus sts;
  pwr_tAName name;
  pwr_tMask value;

  sts = gdh_ObjidToName( objid, name, sizeof(name), cdh_mName_volumeStrict); 
  if ( EVEN(sts)) return sts;

  strcat( name, ".");
  strcat( name, attr);

  sts = gdh_GetObjectInfo( name, &value, sizeof(pwr_tMask));
  if ( EVEN(sts)) return sts;

  if ( bittrue)
    value = value | num;
  else
    value = value & ~num;
  sts = gdh_SetObjectInfo( name, &value, sizeof(pwr_tMask));
  return sts;
}

int ItemMask::toggle_value()
{
  pwr_tStatus sts;
  pwr_tAName name;
  pwr_tMask value;

  sts = gdh_ObjidToName( objid, name, sizeof(name), cdh_mName_volumeStrict); 
  if ( EVEN(sts)) return sts;

  strcat( name, ".");
  strcat( name, attr);

  sts = gdh_GetObjectInfo( name, &value, sizeof(pwr_tMask));
  if ( EVEN(sts)) return sts;

  if ( value & num)
    value = value & ~num;
  else
    value = value | num;
  sts = gdh_SetObjectInfo( name, &value, sizeof(pwr_tMask));
  return sts;
}









