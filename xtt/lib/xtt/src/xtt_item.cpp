/* rt_xtt.cpp -- Display plant and node hiererachy

   PROVIEW/R  
   Copyright (C) 1996 by Comator Process AB.

   <Description>.  */

#include "flow_std.h"

#include <stdio.h>
#include <stdlib.h>

extern "C" {
#include "rt_gdh.h"
#include "rt_gdh_msg.h"
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
#include "rt_xtt.h"
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
	ItemBaseObject( item_objid, item_is_root)
{
  int sts;
  char	segname[120];
  pwr_tObjid child;
  pwr_tClassId classid;
  char	descr[80];

  type = xnav_eItemType_Object;

  sts = gdh_ObjidToName( objid, name, sizeof(name), cdh_mNName);
  if ( EVEN(sts)) throw co_error(sts);
  if ( !is_root)
  {
    sts = gdh_ObjidToName( objid, segname, sizeof(segname), cdh_mName_object);
    if ( EVEN(sts)) throw co_error(sts);
    brow_CreateNode( brow->ctx, segname, brow->nc_object, 
		dest, dest_code, (void *) this, 1, &node);

    // Set pixmap
    sts = gdh_GetChild( objid, &child);
    if( ODD(sts))
      brow_SetAnnotPixmap( node, 0, brow->pixmap_map);
    else
      brow_SetAnnotPixmap( node, 0, brow->pixmap_leaf);
    // Set object name annotation
    brow_SetAnnotation( node, 0, segname, strlen(segname));

    // Set class annotation
    sts = gdh_GetObjectClass( objid, &classid);
    if ( EVEN(sts)) throw co_error(sts);
    sts = gdh_ObjidToName( cdh_ClassIdToObjid( classid),
		  segname, sizeof(segname), cdh_mName_object);
    if ( EVEN(sts)) throw co_error(sts);
    brow_SetAnnotation( node, 1, segname, strlen(segname));

    // Set description annotation
    sts = gdh_ObjidToName( objid, segname, sizeof(segname), cdh_mNName);
    if ( EVEN(sts)) throw co_error(sts);

    strcat( segname, ".Description");
    sts = gdh_GetObjectInfo( segname, descr, sizeof(descr));
    if ( ODD(sts))
      brow_SetAnnotation( node, 2, descr, strlen(descr));

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
    if ( brow_IsOpen( node) & xnav_mOpen_Children)
      brow_SetAnnotPixmap( node, 0, brow->pixmap_map);
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

int ItemBaseObject::open_trace( XNavBrow *brow, XNav *xnav, double x, double y)
{
  pwr_tClassId 	classid;
  pwr_tObjid	parent;
  char		name[80];
  int		sts;

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
    pwr_tObjid		parameter;
    char		classname[80];
    char		hiername[80];
    char		parname[80];
    char		fullname[80];
    char		*s;
    pwr_tClassId	classid;
    unsigned long	elements;
    pwr_sParInfo	parinfo;
    pwr_tObjid		body;
    pwr_tClassId	parameter_class;

    Item 	*item;
    int		attr_exist;
    int		i, j;

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


    // Get objid for rtbody or sysbody

    sts = gdh_GetObjectClass ( objid, &classid);
    if ( EVEN(sts)) return sts;

    sts = gdh_ObjidToName ( cdh_ClassIdToObjid(classid), classname,
		sizeof(classname), cdh_mName_volumeStrict);
    if ( EVEN(sts)) return sts;

    attr_exist = 0;
    for ( j = 0; j < 2; j++)
    {
      strcpy( hiername, classname);
      if ( j == 0)
        strcat( hiername, "-RtBody");
      else
        strcat( hiername, "-SysBody");

      sts = gdh_NameToObjid( hiername, &body);
      if ( EVEN(sts)) 
        continue;

      // Get first attribute of the body
      i = 0;
      sts = gdh_GetChild( body, &parameter);
      while ( ODD(sts))
      {
        sts = gdh_ObjidToName ( parameter, hiername, sizeof(hiername),
			cdh_mName_volumeStrict);
        if ( EVEN(sts)) return sts;

        /* Skip hierarchy */
        s = strrchr( hiername, '-');
        if ( s == 0)
          strcpy( parname, hiername);
        else
          strcpy( parname, s + 1);

        /* Get parameter info for this parameter */
        strcpy( fullname, hiername);
        sts = gdh_GetObjectInfo( fullname, &parinfo, sizeof(parinfo));
        if (EVEN(sts)) return sts;
        sts = gdh_GetObjectClass( parameter, &parameter_class);
        if ( EVEN(sts)) return sts;

        if ( parinfo.Flags & PWR_MASK_RTVIRTUAL || 
             parinfo.Flags & PWR_MASK_PRIVATE)
        {
          /* This parameter does not contain any useful information, take the
		next one */
          sts = gdh_GetNextSibling ( parameter, &parameter);
	  i++;
          continue;
        }

        elements = 1;
        if ( parinfo.Flags & PWR_MASK_ARRAY )
        {
          attr_exist = 1;
          item = (Item *) new ItemAttrArray( brow, objid, node, 
		flow_eDest_IntoLast,
		parname,
		parinfo.Elements, 
		parinfo.Type, parinfo.Size, 0);
        }
        else
        {
          attr_exist = 1;
          item = (Item *) new ItemAttr( brow, objid, node, 
		flow_eDest_IntoLast, parname,
		parinfo.Type, parinfo.Size, 0, item_eDisplayType_Attr);
        } 
        sts = gdh_GetNextSibling ( parameter, &parameter);
	i++;
      }
    }
    if ( attr_exist && !is_root)
    {
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
  pwr_tObjid	parameter;
  char		classname[80];
  char		hiername[80];
  char		parname[80];
  char		up_parname[80];
  char		fullname[80];
  char		*s;
  pwr_tClassId	classid;
  unsigned long	elements;
  pwr_sParInfo	parinfo;
  pwr_tObjid	body;
  pwr_tClassId	parameter_class;

  Item 	*item;
  int		attr_exist;
  int		i, j;

  if ( cdh_ObjidIsNull( objid))
    return 1;

  // Get objid for rtbody or sysbody

  sts = gdh_GetObjectClass ( objid, &classid);
  if ( EVEN(sts)) return sts;

  sts = gdh_ObjidToName ( cdh_ClassIdToObjid(classid), classname,
		sizeof(classname), cdh_mName_volumeStrict);
  if ( EVEN(sts)) return sts;

  attr_exist = 0;
  for ( j = 0; j < 2; j++)
  {
    strcpy( hiername, classname);
    if ( j == 0)
      strcat( hiername, "-RtBody");
    else
      strcat( hiername, "-SysBody");

    sts = gdh_NameToObjid( hiername, &body);
    if ( EVEN(sts)) 
      continue;

    // Get first attribute of the body
    i = 0;
    sts = gdh_GetChild( body, &parameter);
    while ( ODD(sts))
    {
      sts = gdh_ObjidToName ( parameter, hiername, sizeof(hiername),
			cdh_mName_volumeStrict);
      if ( EVEN(sts)) return sts;

      /* Skip hierarchy */
      s = strrchr( hiername, '-');
      if ( s == 0)
        strcpy( parname, hiername);
      else
        strcpy( parname, s + 1);

      /* Get parameter info for this parameter */
      strcpy( fullname, hiername);
      sts = gdh_GetObjectInfo( fullname, &parinfo, sizeof(parinfo));
      if (EVEN(sts)) return sts;
      sts = gdh_GetObjectClass( parameter, &parameter_class);
      if ( EVEN(sts)) return sts;

      cdh_ToUpper( up_parname, parname);
      if ( strcmp( attr_name, up_parname) == 0)
      {
        if ( parinfo.Flags & PWR_MASK_RTVIRTUAL || 
             parinfo.Flags & PWR_MASK_PRIVATE)
        {
          // This parameter does not contain any useful information 
          return XNAV__PRIVATTR;
        }

        elements = 1;
        if ( parinfo.Flags & PWR_MASK_ARRAY )
        {
	  if ( element == -1)
            return XNAV__NOELEMENT;
          attr_exist = 1;
          item = (Item *) new ItemAttrArrayElem( brow, objid, node, 
		flow_eDest_IntoLast, 
		parname, element, parinfo.Type,
		parinfo.Size / parinfo.Elements, 0, item_eDisplayType_Path);
        }
        else
        {
          attr_exist = 1;
          item = (Item *) new ItemAttr( brow, objid, node,
		flow_eDest_IntoLast, parname,
		parinfo.Type, parinfo.Size, 0, item_eDisplayType_Path);
        }
        break;
      }
      sts = gdh_GetNextSibling ( parameter, &parameter);
      i++;
    }
  }
  return 1;
}

int ItemBaseObject::open_crossref( XNavBrow *brow, XNav *xnav, 
				   double x, double y)
{
  double	node_x, node_y;
  int		crossref_exist;
  int		sts;
  pwr_tClassId	classid;

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
        sts = xnav_crr_signal( brow, NULL, name, node);
        break;
      default:
        sts = xnav_crr_object( brow, NULL, name, node);
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

int ItemBaseObject::close( XNavBrow *brow, double x, double y)
{
  double	node_x, node_y;

  if ( cdh_ObjidIsNull( objid))
    return 1;

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
  return 1;
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

    for ( i = 0; i < elements; i++)
    {
      item = (Item *) new ItemAttrArrayElem( brow, objid, node, 
		flow_eDest_IntoLast, name, i, type_id, 
		size / elements, is_root, item_eDisplayType_Attr);
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

int ItemAttrArray::close( XNavBrow *brow, double x, double y)
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
  return 1;
}

ItemAttr::ItemAttr( XNavBrow *brow, pwr_tObjid item_objid,
	brow_tNode dest, flow_eDest dest_code,
	char *attr_name, int attr_type_id, int attr_size,
	int item_is_root, item_eDisplayType item_display_type) :
	ItemBaseAttr( item_objid, attr_name,
	attr_type_id, attr_size, item_is_root, item_display_type)
{
  char	obj_name[120];
  char	annot[120];
  int	sts;

  type = xnav_eItemType_Attr;

  strcpy( name, attr_name);

  switch ( display_type)
  {
    case item_eDisplayType_Path:
      sts = gdh_ObjidToName( objid, obj_name, sizeof(obj_name), cdh_mNName); 
      if ( EVEN(sts)) throw co_error(sts);

      sprintf( annot, "%s.%s", obj_name, attr_name);
      break;
    default:
      strcpy( annot, attr_name);
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
      default:
        brow_SetAnnotPixmap( node, 0, brow->pixmap_attr);
    }
    brow_SetAnnotation( node, 0, annot, strlen(annot));
    sts = gdh_ObjidToName( objid, obj_name, sizeof(obj_name), cdh_mName_volumeStrict); 
    if ( EVEN(sts)) throw co_error(sts);

    brow_SetTraceAttr( node, obj_name, attr_name, flow_eTraceType_User);
  }
}

ItemAttrArray::ItemAttrArray( XNavBrow *brow, pwr_tObjid item_objid,
	brow_tNode dest, flow_eDest dest_code,
	char *attr_name, int attr_elements, int attr_type_id,
	int attr_size, int item_is_root) :
	Item( item_objid, item_is_root),
	elements(attr_elements), type_id(attr_type_id),
	size(attr_size)
{
  type = xnav_eItemType_AttrArray;

  strcpy( name, attr_name);
  if ( !is_root)
  {
    brow_CreateNode( brow->ctx, attr_name, brow->nc_object, 
		dest, dest_code, (void *) this, 1, &node);

    brow_SetAnnotPixmap( node, 0, brow->pixmap_attrarray);
    brow_SetAnnotation( node, 0, attr_name, strlen(attr_name));
  }
}

ItemAttrArrayElem::ItemAttrArrayElem( XNavBrow *brow, pwr_tObjid item_objid,
	brow_tNode dest, flow_eDest dest_code,
	char *attr_name, int attr_element, int attr_type_id,
	int attr_size, int item_is_root, item_eDisplayType item_display_type) :
	ItemBaseAttr( item_objid, attr_name,
	attr_type_id, attr_size, item_is_root, item_display_type),
	element(attr_element)
{
  char	obj_name[120];
  char	annot[120];
  int	sts;

  type = xnav_eItemType_AttrArrayElem;

  sprintf( &attr[strlen(attr)], "[%d]", element);
  sprintf( name, "%s[%d]", attr_name, element);

  switch( display_type)
  {
    case item_eDisplayType_Path:
      sts = gdh_ObjidToName( objid, obj_name, sizeof(obj_name), cdh_mNName); 
      if ( EVEN(sts)) throw co_error(sts);
      strcpy( annot, obj_name);
      strcat( annot, ".");
      strcat( annot, name);
      break;
    default:
      strcpy( annot, name);
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

ItemHeader::ItemHeader( XNavBrow *brow, char *item_name, char *title,
	brow_tNode dest, flow_eDest dest_code) :
	Item( pwr_cNObjid, 0)
{
  type = xnav_eItemType_Header;
  strcpy( name, item_name);
  brow_CreateNode( brow->ctx, "header", brow->nc_header,
		dest, dest_code, (void *)this, 1, &node);
  brow_SetAnnotation( node, 0, title, strlen(title));
}

ItemHeaderLarge::ItemHeaderLarge( XNavBrow *brow, char *item_name, char *title,
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
	char *item_name, 
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

ItemText::ItemText( XNavBrow *brow, char *item_name, char *text,
	brow_tNode dest, flow_eDest dest_code) :
	Item( pwr_cNObjid, 0)
{
  type = xnav_eItemType_Text;
  strcpy( name, item_name);
  brow_CreateNode( brow->ctx, "text", brow->nc_object,
		dest, dest_code, (void *)this, 1, &node);
  brow_SetAnnotation( node, 0, text, strlen(text));
}

ItemHelpHeader::ItemHelpHeader( XNavBrow *brow, char *item_name, char *title,
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

int ItemHelpHeader::close( XNavBrow *brow, XNav *xnav, double x, double y)
{
  if ( xnav)
    xnav->brow_push();
  return 1;
}

ItemHelp::ItemHelp( XNavBrow *brow, char *item_name, char *text, char *text2, 
	char *text3, char *item_link, char *item_bookmark, 
	char *item_file_name, navh_eHelpFile help_file_type, int help_index, brow_tNode dest, flow_eDest dest_code) :
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

int ItemHelp::open_children( XNavBrow *brow, XNav *xnav, double x, double y)
{
  int sts;

  if ( index)
  {
    sts = xnav->help_index( file_type, file_name, 1);
  }
  else if ( strcmp( link, "") != 0)
  {
    if ( (strstr( link, ".htm") != 0) || (strstr( link, ".pdf") != 0)) {
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

ItemHelpBold::ItemHelpBold( XNavBrow *brow, char *item_name, char *text, char *text2, 
	char *text3, char *item_link, char *item_bookmark, 
	char *item_file_name, navh_eHelpFile help_file_type, int help_index, brow_tNode dest, flow_eDest dest_code) :
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

int ItemHelpBold::open_children( XNavBrow *brow, XNav *xnav, double x, double y)
{
  int sts;

  if ( index)
  {
    sts = xnav->help_index( file_type, file_name, 1);
  }
  else if ( strcmp( link, "") != 0)
  {
    if ( (strstr( link, ".htm") != 0) || (strstr( link, ".pdf") != 0)) {
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

ItemFile::ItemFile( XNavBrow *brow, char *item_name, char *text, 
	char *item_file_name, item_eFileType item_filetype,
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
  else if ( file_type == item_eFileType_Graph)
    brow_SetAnnotPixmap( node, 0, brow->pixmap_graph);
  else if ( file_type == item_eFileType_RttLog)
    brow_SetAnnotPixmap( node, 0, brow->pixmap_curve);
  else
    brow_SetAnnotPixmap( node, 0, brow->pixmap_file);
  brow_SetAnnotation( node, 0, text, strlen(text));
}

int ItemFile::open_children( XNavBrow *brow, XNav *xnav, double x, double y)
{
  int sts;

  switch ( file_type)
  {
    case item_eFileType_Script:
    {
      char cmd[120];
      strcpy( cmd, "@");
      strcat( cmd, file_name);
      sts = xnav->command( cmd);
      break;
    }
    case item_eFileType_Graph:
      xnav->open_graph( name, file_name, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
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
	brow_tNode dest, flow_eDest dest_code, int attr_type_id,
	int attr_size, int item_is_root) :
	ItemBaseAttr( item_objid, attr_name,
	attr_type_id, attr_size, item_is_root, item_eDisplayType_Path)
{
  int sts;
  char obj_name[120];
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

int ItemPlc::open_children( XNavBrow *brow, XNav *xnav, double x, double y)
{
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

int ItemPlc::close( XNavBrow *brow, double x, double y)
{
  return ((ItemObject *)this)->close( brow, x, y);
}

int ItemPlc::open_attributes( XNavBrow *brow, double x, double y)
{
  return ((ItemObject *)this)->open_attributes( brow, x, y);
}

int ItemPlc::open_trace( XNavBrow *brow, XNav *xnav, double x, double y)
{
  return ((ItemObject *)this)->open_trace( brow, xnav, x, y);
}


ItemMenu::ItemMenu( XNavBrow *brow, char *item_name, 
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
    Item 		*item;
    xnav_sMenu		*menu;

    brow_SetNodraw( brow->ctx);
    menu = *child_list;
    while ( menu)
    {
      switch ( menu->item_type)
      {
        case xnav_eItemType_Menu:
          item = (Item *) new ItemMenu( brow, menu->title, node, 
		flow_eDest_IntoLast, &menu->child_list,
		0);
          break;
        case xnav_eItemType_Command:
          item = (Item *) new ItemCommand( brow, menu->title, node, 
		flow_eDest_IntoLast, menu->command, 0,
		brow->pixmap_map);
          break;
        default:
          ;
      }
      menu = menu->next;
      if ( !is_root)
      {
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

int ItemMenu::close( XNavBrow *brow, double x, double y)
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
  return 1;
}

ItemCommand::ItemCommand( XNavBrow *brow, char *item_name, 
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
  }
}

int ItemCommand::open_children( XNavBrow *brow, XNav *xnav, double x, double y)
{

  xnav->command( command);
  return 1;
}


ItemCrossref::ItemCrossref( XNavBrow *brow, char *item_ref_name, 
	char *item_ref_class, int item_write, brow_tNode dest, 
	flow_eDest dest_code) :
	Item( pwr_cNObjid, 0), write(item_write)
{
  int sts;
  char window_name[120];
  char *s;

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

int ItemCrossref::open_trace( XNavBrow *brow, XNav *xnav, double x, double y)
{
  xnav->start_trace( objid, ref_name);
  return 1;
}


ItemLocal::ItemLocal( XNavBrow *brow, char *item_name, char *attr, 
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

int ItemDevice::open_children( XNavBrow *brow, XNav *xnav, double x, double y)
{
  if ( cdh_ObjidIsNull( objid))
    return 1;

  return xnav->show_channels( objid);
}

int ItemChannel::open_children( XNavBrow *brow, XNav *xnav, double x, double y)
{
  if ( cdh_ObjidIsNull( objid))
    return 1;

  return xnav->show_object( signal_objid, node);
}

int ItemChannel::open_crossref( XNavBrow *brow, XNav *xnav, double x, double y)
{
  double	node_x, node_y;
  int		crossref_exist;
  int		sts;
  char		signal_name[120];

  if ( cdh_ObjidIsNull( signal_objid))
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

    sts = gdh_ObjidToName( signal_objid, signal_name, sizeof(signal_name), 
		cdh_mNName);

    sts = xnav_crr_signal( xnav->brow, NULL, signal_name, node);

    if ( sts == NAV__OBJECTNOTFOUND)
      xnav->message('E', "Object not found in crossreferens file");
    else if ( sts == NAV__NOCROSSREF)
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


int ItemRemNode::open_children( XNavBrow *brow, XNav *xnav, double x, double y)
{
  return xnav->show_remtrans( objid);
}

int ItemRemTrans::open_children( XNavBrow *brow, XNav *xnav, double x, double y)
{
  char          remtrans_name[120];
  char		namebuf[140];
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





