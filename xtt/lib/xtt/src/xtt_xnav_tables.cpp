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
#include "pwr_ssabclasses.h"
#include "rt_xnav_msg.h"
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

extern "C" {
#include "rt_trace.h"
}
#include "xtt_xnav.h"
#include "xtt_item.h"

extern "C" {
#include "rt_gdh.h"
#include "rt_qdb.h"
#include "rt_hash.h"
#include "rt_pool.h"
#include "rt_net.h"
#include "rt_sub.h"
#include "rt_io_base.h"
}

static int	xnav_subcli_remote_cnt;


static int	xnav_get_nodename( pwr_tNodeId nid, char *nodename);
static int xnav_show_subsrv_insert( XNav *xnav, sub_sServer *ssrvp);
extern "C" void xnav_show_subsrv_scan( XNav *xnav);
static int xnav_show_subcli_insert( XNav *xnav, gdb_sNode *np,
		sub_sClient *sclip);
extern "C" void xnav_show_subcli_scan( XNav *xnav);

int XNav::show_plcthreads()
{
  item_sTableHeader 	th;
  item_sTable 		t;
  item_sTableSubid	ts;
  pwr_tTypeId		attrtype;
  unsigned int		attrsize, attroffs, attrelem;
  pwr_tSubid		subid;
  char			object_name[120];
  char			attr_name[140];
  char			*s;
  int			sts;
  pwr_tObjid		objid;
  pwr_sAttrRef		attrref;
  pwr_sClass_PlcThread	*object_ptr;

  brow_pop();
  brow_SetNodraw( brow->ctx);

  th.table_cnt = 0;
  strcpy( th.title[th.table_cnt++], "PlcThread");
  strcpy( th.title[th.table_cnt++], "Prio");
  strcpy( th.title[th.table_cnt++], "LoopCnt");
  strcpy( th.title[th.table_cnt++], "ScanTime");
  strcpy( th.title[th.table_cnt++], "Mean");
  strcpy( th.title[th.table_cnt++], "Coverage");
  strcpy( th.title[th.table_cnt++], "Count 1/8");
  strcpy( th.title[th.table_cnt++], "Count 1/4");
  strcpy( th.title[th.table_cnt++], "Count 1/2");
  strcpy( th.title[th.table_cnt++], "Count 1/1");
  new ItemTableHeader( brow, this, "Title", &th,  NULL, flow_eDest_IntoLast);

  sts = gdh_GetClassList ( pwr_cClass_PlcThread, &objid);
  while ( ODD(sts))
  {
    memset( &attrref, 0, sizeof( attrref));
    attrref.Objid = objid;
    sts = gdh_DLRefObjectInfoAttrref ( &attrref,
		(pwr_tAddress *) &object_ptr,
		&subid);
    if ( EVEN(sts)) return sts;

    sts = gdh_ObjidToName ( objid, object_name,
			sizeof(object_name), cdh_mName_volumeStrict);
    if ( EVEN(sts)) return sts;

    t.elem_cnt = 0;

    // Object name
    s = strrchr( object_name, '-');
    if ( s == 0)
      s = object_name;
    else
      s++;

    strcpy( t.elem[t.elem_cnt].fix_str, s);
    t.elem[t.elem_cnt++].type_id = xnav_eType_FixStr;

    // Prio
    strcpy( attr_name, object_name);
    strcat( attr_name, ".Prio");
    sts = gdh_GetAttributeCharacteristics ( attr_name,
		&attrtype, &attrsize, &attroffs, &attrelem);
    if ( EVEN(sts)) return sts;

    t.elem[t.elem_cnt].value_p = &object_ptr->Prio;
    t.elem[t.elem_cnt].type_id = attrtype;
    t.elem[t.elem_cnt].size = attrsize;
    strcpy( t.elem[t.elem_cnt++].format, "%3d");

    // Count
    strcpy( attr_name, object_name);
    strcat( attr_name, ".Count");
    sts = gdh_GetAttributeCharacteristics ( attr_name,
		&attrtype, &attrsize, &attroffs, &attrelem);
    if ( EVEN(sts)) return sts;

    t.elem[t.elem_cnt].value_p = &object_ptr->Count;
    t.elem[t.elem_cnt].type_id = attrtype;
    t.elem[t.elem_cnt].size = attrsize;
    strcpy( t.elem[t.elem_cnt++].format, "%10d");

    // ScanTime
    strcpy( attr_name, object_name);
    strcat( attr_name, ".ScanTime");
    sts = gdh_GetAttributeCharacteristics ( attr_name,
		&attrtype, &attrsize, &attroffs, &attrelem);
    if ( EVEN(sts)) return sts;

    t.elem[t.elem_cnt].value_p = &object_ptr->ScanTime;
    t.elem[t.elem_cnt].type_id = attrtype;
    t.elem[t.elem_cnt].size = attrsize;
    strcpy( t.elem[t.elem_cnt++].format, "%8.3f");

    // ScanTimeMean
    strcpy( attr_name, object_name);
    strcat( attr_name, ".ScanTimeMean");
    sts = gdh_GetAttributeCharacteristics ( attr_name,
		&attrtype, &attrsize, &attroffs, &attrelem);
    if ( EVEN(sts)) return sts;

    t.elem[t.elem_cnt].value_p = &object_ptr->ScanTimeMean;
    t.elem[t.elem_cnt].type_id = attrtype;
    t.elem[t.elem_cnt].size = attrsize;
    strcpy( t.elem[t.elem_cnt++].format, "%8.3f");

    // Coverage
    strcpy( attr_name, object_name);
    strcat( attr_name, ".Coverage");
    sts = gdh_GetAttributeCharacteristics ( attr_name,
		&attrtype, &attrsize, &attroffs, &attrelem);
    if ( EVEN(sts)) return sts;

    t.elem[t.elem_cnt].value_p = &object_ptr->Coverage;
    t.elem[t.elem_cnt].type_id = attrtype;
    t.elem[t.elem_cnt].size = attrsize;
    strcpy( t.elem[t.elem_cnt++].format, "%8.3f");

    // Count_1_8
    strcpy( attr_name, object_name);
    strcat( attr_name, ".Count_1_8");
    sts = gdh_GetAttributeCharacteristics ( attr_name,
		&attrtype, &attrsize, &attroffs, &attrelem);
    if ( EVEN(sts)) return sts;

    t.elem[t.elem_cnt].value_p = &object_ptr->Count_1_8;
    t.elem[t.elem_cnt].type_id = attrtype;
    t.elem[t.elem_cnt].size = attrsize;
    strcpy( t.elem[t.elem_cnt++].format, "%10d");

    // Count_1_4
    strcpy( attr_name, object_name);
    strcat( attr_name, ".Count_1_4");
    sts = gdh_GetAttributeCharacteristics ( attr_name,
		&attrtype, &attrsize, &attroffs, &attrelem);
    if ( EVEN(sts)) return sts;

    t.elem[t.elem_cnt].value_p = &object_ptr->Count_1_4;
    t.elem[t.elem_cnt].type_id = attrtype;
    t.elem[t.elem_cnt].size = attrsize;
    strcpy( t.elem[t.elem_cnt++].format, "%10d");

    // Count_1_2
    strcpy( attr_name, object_name);
    strcat( attr_name, ".Count_1_2");
    sts = gdh_GetAttributeCharacteristics ( attr_name,
		&attrtype, &attrsize, &attroffs, &attrelem);
    if ( EVEN(sts)) return sts;

    t.elem[t.elem_cnt].value_p = &object_ptr->Count_1_2;
    t.elem[t.elem_cnt].type_id = attrtype;
    t.elem[t.elem_cnt].size = attrsize;
    strcpy( t.elem[t.elem_cnt++].format, "%10d");

    // Count_1_1
    strcpy( attr_name, object_name);
    strcat( attr_name, ".Count_1_1");
    sts = gdh_GetAttributeCharacteristics ( attr_name,
		&attrtype, &attrsize, &attroffs, &attrelem);
    if ( EVEN(sts)) return sts;

    t.elem[t.elem_cnt].value_p = &object_ptr->Count_1_1;
    t.elem[t.elem_cnt].type_id = attrtype;
    t.elem[t.elem_cnt].size = attrsize;
    strcpy( t.elem[t.elem_cnt++].format, "%10d");

    ts.subid[0] = subid;
    ts.subid_cnt = 1;
    new ItemTable( brow, this, objid, &t, &ts, -1, 0, 0, 0, NULL, flow_eDest_IntoLast);

    sts = gdh_GetNextObject ( objid, &objid);
  }

  brow_ResetNodraw( brow->ctx);
  brow_Redraw( brow->ctx, 0);
  force_trace_scan();
  return XNAV__SUCCESS;
}

int XNav::show_nethandler()
{
  item_sTableHeader 	th;
  item_sTable 		t;
  item_sTableSubid	ts;
  int			sts;
  pool_sQlink		*nl;
  gdb_sNode		*np;

  brow_pop();
  brow_SetNodraw( brow->ctx);

  th.table_cnt = 0;
  strcpy( th.title[th.table_cnt++], "Node");
  strcpy( th.title[th.table_cnt++], "Os");
  strcpy( th.title[th.table_cnt++], "Link");
  strcpy( th.title[th.table_cnt++], "UpCount");
  strcpy( th.title[th.table_cnt++], "Time up");
  new ItemTableHeader( brow, this, "Title", &th,  NULL, flow_eDest_IntoLast);

  gdb_ScopeLock {
    for (
	  nl = pool_Qsucc(&sts, gdbroot->pool, &gdbroot->db->nod_lh);
	  nl != &gdbroot->db->nod_lh;
	  nl = pool_Qsucc(&sts, gdbroot->pool, nl))
    {

      np = pool_Qitem(nl, gdb_sNode, nod_ll);

      if ( strcmp( np->name, "******") == 0)
        continue;

      t.elem_cnt = 0;

      // Node name
      strcpy( t.elem[t.elem_cnt].fix_str, np->name);
      t.elem[t.elem_cnt++].type_id = xnav_eType_FixStr;

      // Os
      switch ( np->hw) {
        case co_eHW_x86: strcpy( t.elem[t.elem_cnt].fix_str, "x86"); break;
        case co_eHW_68k: strcpy( t.elem[t.elem_cnt].fix_str, "68k"); break;
        case co_eHW_VAX: strcpy( t.elem[t.elem_cnt].fix_str, "VAX"); break;
        case co_eHW_Alpha: strcpy( t.elem[t.elem_cnt].fix_str, "AXP"); break;
        case co_eHW_PPC: strcpy( t.elem[t.elem_cnt].fix_str, "PPC"); break;
        default: strcpy( t.elem[t.elem_cnt].fix_str, "-");
      }
      switch ( np->os) {
        case co_eOS_Lynx: strcat( t.elem[t.elem_cnt].fix_str, "_Lynx"); break;
        case co_eOS_Linux: strcat( t.elem[t.elem_cnt].fix_str, "_Linux"); break;
        case co_eOS_VMS: strcat( t.elem[t.elem_cnt].fix_str, "_VMS"); break;
        case co_eOS_ELN: strcat( t.elem[t.elem_cnt].fix_str, "_ELN"); break;
        default: ;
      }
      t.elem[t.elem_cnt++].type_id = xnav_eType_FixStr;

      if (np == gdbroot->my_node)
      {
        // Local node
        strcpy( t.elem[t.elem_cnt].fix_str, "Local");
        t.elem[t.elem_cnt++].type_id = xnav_eType_FixStr;
      }
      else
      {
        t.elem[t.elem_cnt].value_p = &np->flags;
        t.elem[t.elem_cnt].type_id = xnav_eType_GdbNodeFlags;
        t.elem[t.elem_cnt].size = sizeof(np->flags.b);
        strcpy( t.elem[t.elem_cnt++].format, "%s");
      }

      // UpCnt
      t.elem[t.elem_cnt].value_p = &np->upcnt;
      t.elem[t.elem_cnt].type_id = pwr_eType_UInt32;
      t.elem[t.elem_cnt].size = sizeof(np->upcnt);
      strcpy( t.elem[t.elem_cnt++].format, "%8d");

      // Time up
      t.elem[t.elem_cnt].value_p = &np->timeup;
      t.elem[t.elem_cnt].type_id = pwr_eType_Time;
      t.elem[t.elem_cnt++].size = sizeof(np->timeup);

      ts.subid_cnt = 0;
      new ItemTable( brow, this, pwr_cNObjid, &t, &ts, -1, 0, 0, 0, NULL, 
		flow_eDest_IntoLast);
    }
  } gdb_ScopeUnlock;

  brow_ResetNodraw( brow->ctx);
  brow_Redraw( brow->ctx, 0);
  force_trace_scan();
  return XNAV__SUCCESS;
}


static int	xnav_get_nodename( pwr_tNodeId nid, char *nodename)
{
	gdb_sNode 		*np;
	int			sts;

	np = (gdb_sNode*) hash_Search( &sts, gdbroot->nid_ht, &nid);
	if (ODD(sts)) 
	  strcpy( nodename, np->name);
	else
	  strcpy( nodename, "Undef");
	return sts;
}

int XNav::show_subsrv()
{
  pool_sQlink		*sl;
  sub_sServer	 	*ssrvp;
  item_sTableHeader 	th;
  int			sts;
  ItemTableHeader	*item_header;

  brow_pop();
  brow_SetNodraw( brow->ctx);

  th.table_cnt = 0;
  strcpy( th.title[th.table_cnt++], "Subid");
  strcpy( th.title[th.table_cnt++], "Count");
  strcpy( th.title[th.table_cnt++], "Node");
  strcpy( th.title[th.table_cnt++], "Size");
  strcpy( th.title[th.table_cnt++], "Offset");
  strcpy( th.title[th.table_cnt++], "Attribute");
  item_header = new ItemTableHeader( brow, this, "Title", &th,  NULL, flow_eDest_IntoLast);
  item_header->add_bc( xnav_show_subsrv_scan, NULL);

  gdb_ScopeLock {
    for (
	  sl = pool_Qsucc(&sts, gdbroot->pool, &gdbroot->db->subs_lh);
	  sl != &gdbroot->db->subs_lh;
	  sl = pool_Qsucc(&sts, gdbroot->pool, sl))
    {
      ssrvp = pool_Qitem(sl, sub_sServer, subs_ll);

      xnav_show_subsrv_insert( this, ssrvp);
    }
  } gdb_ScopeUnlock;

  brow_ResetNodraw( brow->ctx);
  brow_Redraw( brow->ctx, 0);
  force_trace_scan();
  return XNAV__SUCCESS;
}

static int xnav_show_subsrv_insert( XNav *xnav, sub_sServer *ssrvp)
{
  item_sTable 		t;
  item_sTableSubid	ts;
  ItemTable		*item;

  t.elem_cnt = 0;

  // Subid
  t.elem[t.elem_cnt].value_p = &ssrvp->sid;
  t.elem[t.elem_cnt].type_id = pwr_eType_RefId;
  t.elem[t.elem_cnt].size = sizeof(ssrvp->sid);
  strcpy( t.elem[t.elem_cnt++].format, "%8d");

  // Count
  t.elem[t.elem_cnt].value_p = &ssrvp->count;
  t.elem[t.elem_cnt].type_id = pwr_eType_UInt32;
  t.elem[t.elem_cnt].size = sizeof(ssrvp->count);
  strcpy( t.elem[t.elem_cnt++].format, "%8d");

  // Node
  xnav_get_nodename( ssrvp->nid, t.elem[t.elem_cnt].fix_str);
  t.elem[t.elem_cnt++].type_id = xnav_eType_FixStr;

  // Size
  t.elem[t.elem_cnt].value_p = &ssrvp->aref.Size;
  t.elem[t.elem_cnt].type_id = pwr_eType_UInt32;
  t.elem[t.elem_cnt].size = sizeof(ssrvp->aref.Size);
  strcpy( t.elem[t.elem_cnt++].format, "%8d");

  // Offset
  t.elem[t.elem_cnt].value_p = &ssrvp->aref.Offset;
  t.elem[t.elem_cnt].type_id = pwr_eType_UInt32;
  t.elem[t.elem_cnt].size = sizeof(ssrvp->aref.Offset);
  strcpy( t.elem[t.elem_cnt++].format, "%8d");

  // Attribute
  t.elem[t.elem_cnt].value_p = &ssrvp->aref;
  t.elem[t.elem_cnt].type_id = pwr_eType_AttrRef;
  t.elem[t.elem_cnt++].size = sizeof(ssrvp->aref);

  ts.subid_cnt = 0;
  item = new ItemTable( xnav->brow, xnav, pwr_cNObjid, &t, &ts, -1, 0, 0, 0, NULL, 
		flow_eDest_IntoLast);
  item->found = 1;
  return XNAV__SUCCESS;
}

extern "C" void xnav_show_subsrv_scan( XNav *xnav)
{
  brow_tObject	*node_list;
  brow_tObject	*object_p;
  int		node_cnt;
  ItemTable	*item;
  int		i;
  pool_sQlink	*sl;
  sub_sServer	 *ssrvp;
  int		sts;
  int		found;
  int		change_detected = 0;

  brow_SetNodraw( xnav->brow->ctx);
  brow_GetObjectList( xnav->brow->ctx, &node_list, &node_cnt);

  // Reset found
  object_p = node_list + 1;
  for ( i = 1; i < node_cnt; i++)
  {    
    brow_GetUserData( *object_p, (void **)&item);
    item->found = 0;
    object_p++;    
  }

  gdb_ScopeLock {
    for (
	  sl = pool_Qsucc(&sts, gdbroot->pool, &gdbroot->db->subs_lh);
	  sl != &gdbroot->db->subs_lh;
	  sl = pool_Qsucc(&sts, gdbroot->pool, sl))
    {
      ssrvp = pool_Qitem(sl, sub_sServer, subs_ll);

      object_p = node_list + 1;
      found = 0;
      for ( i = 1; i < node_cnt; i++)
      {    
        brow_GetUserData( *object_p, (void **)&item);
        if ( memcmp( item->col.elem[0].value_p, &ssrvp->sid, sizeof(pwr_tSubid))
		 == 0)
        {
          found = 1;
          break;
        }		
        object_p++;
      }
      if ( !found)
      {
        xnav_show_subsrv_insert( xnav, ssrvp);
        change_detected = 1;
      }
      else
        item->found = 1;
    }
  } gdb_ScopeUnlock;

  // Remove all that were not found
  object_p = node_list + 1;
  for ( i = 1; i < node_cnt; i++)
  {    
    brow_GetUserData( *object_p, (void **)&item);
    if ( ! item->found)
    {
      brow_DeleteNode( xnav->brow->ctx, item->node);
      change_detected = 1;
    }
    else
      object_p++;
  }
  brow_ResetNodraw( xnav->brow->ctx);
  if ( change_detected)
    brow_Redraw( xnav->brow->ctx, 0);
}


int XNav::show_subcli()
{
  pool_sQlink		*sl;
  pool_sQlink		*nl;
  sub_sClient 		*sclip;
  gdb_sNode 		*np;
  item_sTableHeader 	th;
  int			sts;
  ItemTableHeader	*item_header;
  item_sTable 		t;
  item_sTableSubid	ts;

  brow_pop();
  brow_SetNodraw( brow->ctx);

  t.elem_cnt = 0;
  t.elem[t.elem_cnt++].type_id = xnav_eType_Empty;

  // Local
  strcpy( t.elem[t.elem_cnt].fix_str, "Local:");
  t.elem[t.elem_cnt++].type_id = xnav_eType_FixStr;

  t.elem[t.elem_cnt].value_p = &gdbroot->my_node->subc_lc;
  t.elem[t.elem_cnt].type_id = pwr_eType_UInt32;
  t.elem[t.elem_cnt].size = sizeof(gdbroot->my_node->subc_lc);
  strcpy( t.elem[t.elem_cnt++].format, "%8d");

  // Remote
  strcpy( t.elem[t.elem_cnt].fix_str, "Remote:");
  t.elem[t.elem_cnt++].type_id = xnav_eType_FixStr;

  t.elem[t.elem_cnt].value_p = &xnav_subcli_remote_cnt;
  t.elem[t.elem_cnt].type_id = pwr_eType_UInt32;
  t.elem[t.elem_cnt].size = sizeof(xnav_subcli_remote_cnt);
  strcpy( t.elem[t.elem_cnt++].format, "%8d");

  // Unknown
  strcpy( t.elem[t.elem_cnt].fix_str, "Unknown:");
  t.elem[t.elem_cnt++].type_id = xnav_eType_FixStr;

  t.elem[t.elem_cnt].value_p = &gdbroot->no_node->subc_lc;
  t.elem[t.elem_cnt].type_id = pwr_eType_UInt32;
  t.elem[t.elem_cnt].size = sizeof(gdbroot->no_node->subc_lc);
  strcpy( t.elem[t.elem_cnt++].format, "%8d");

  ts.subid_cnt = 0;
  new ItemTable( brow, this, pwr_cNObjid, &t, &ts, -1, 0, 0, 0, NULL, 
		flow_eDest_IntoLast);

  th.table_cnt = 0;
  strcpy( th.title[th.table_cnt++], "Subid");
  strcpy( th.title[th.table_cnt++], "Time");
  strcpy( th.title[th.table_cnt++], "Count");
  strcpy( th.title[th.table_cnt++], "Node");
  strcpy( th.title[th.table_cnt++], "Size");
  strcpy( th.title[th.table_cnt++], "Attribute");
  item_header = new ItemTableHeader( brow, this, "Title", &th,  NULL, flow_eDest_IntoLast);
  item_header->add_bc( xnav_show_subcli_scan, NULL);

  xnav_subcli_remote_cnt = 0;
  gdb_ScopeLock {
    for (
	  nl = pool_Qsucc(&sts, gdbroot->pool, &gdbroot->db->nod_lh);
	  nl != &gdbroot->db->nod_lh;
	  nl = pool_Qsucc(&sts, gdbroot->pool, nl)) 
    {
      np = pool_Qitem(nl, gdb_sNode, nod_ll);
      for (
	    sl = pool_Qsucc(&sts, gdbroot->pool, &np->subc_lh);
	    sl != &np->subc_lh;
	    sl = pool_Qsucc(&sts, gdbroot->pool, sl))
      {
        sclip = pool_Qitem(sl, sub_sClient, subc_ll);
        xnav_show_subcli_insert( this, np, sclip);
      }
      if ( !(np == gdbroot->my_node || np == gdbroot->no_node))
        xnav_subcli_remote_cnt += np->subc_lc;
    }
  } gdb_ScopeUnlock;

  brow_ResetNodraw( brow->ctx);
  brow_Redraw( brow->ctx, 0);
  force_trace_scan();
  return XNAV__SUCCESS;
}

static int xnav_show_subcli_insert( XNav *xnav, gdb_sNode *np,
		sub_sClient *sclip)
{
  item_sTable 		t;
  item_sTableSubid	ts;
  ItemTable		*item;
  static int 		zero = 0;
  char			astr[80];
  net_sSubData 		*sdatap;
  int			sts;

  t.elem_cnt = 0;

  // Subid
  t.elem[t.elem_cnt].value_p = &sclip->sid;
  t.elem[t.elem_cnt].type_id = pwr_eType_RefId;
  t.elem[t.elem_cnt].size = sizeof(sclip->sid);
  strcpy( t.elem[t.elem_cnt++].format, "%8d");

  // Time
  t.elem[t.elem_cnt].value_p = &sclip->lastupdate;
  t.elem[t.elem_cnt].type_id = xnav_eType_ShortTime;
  t.elem[t.elem_cnt++].size = sizeof(sclip->lastupdate);

  // Count
  t.elem[t.elem_cnt].value_p = &sclip->count;
  t.elem[t.elem_cnt].type_id = pwr_eType_UInt32;
  t.elem[t.elem_cnt].size = sizeof(sclip->count);
  strcpy( t.elem[t.elem_cnt++].format, "%8d");

  // Node
  if ( sclip->nid == pwr_cNNodeId)
    strcpy( t.elem[t.elem_cnt].fix_str, "Unknown");
  else
    strcpy( t.elem[t.elem_cnt].fix_str, np->name);
  t.elem[t.elem_cnt++].type_id = xnav_eType_FixStr;

  // Size
  sdatap = (net_sSubData *) pool_Address( &sts, gdbroot->pool, sclip->subdata);
  if (sdatap == NULL)
    t.elem[t.elem_cnt].value_p = &zero;
  else
    t.elem[t.elem_cnt].value_p = &sdatap->size;
  t.elem[t.elem_cnt].type_id = pwr_eType_UInt16;
  t.elem[t.elem_cnt].size = sizeof(pwr_tUInt16);
  strcpy( t.elem[t.elem_cnt++].format, "%8hd");

  // Name
  if (sclip->sub_by_name)
    strcpy( t.elem[t.elem_cnt].fix_str, sclip->name);
  else 
  {
    if ( sclip->aref.Flags.b.Indirect)
      strcpy( astr, "@");
    else
      strcpy( astr, "");
    cdh_ArefToString( astr, &sclip->aref, 1);
    strcpy( t.elem[t.elem_cnt].fix_str, astr);
  }
  t.elem[t.elem_cnt++].type_id = xnav_eType_FixStr;

  ts.subid_cnt = 0;
  item = new ItemTable( xnav->brow, xnav, pwr_cNObjid, &t, &ts, -1, 0, 0, 0, NULL, 
		flow_eDest_IntoLast);
  item->found = 1;
  return XNAV__SUCCESS;
}

extern "C" void xnav_show_subcli_scan( XNav *xnav)
{
  brow_tObject	*node_list;
  brow_tObject	*object_p;
  int		node_cnt;
  ItemTable	*item;
  int		i;
  pool_sQlink	*sl;
  pool_sQlink	*nl;
  sub_sClient 	*sclip;
  gdb_sNode 	*np;
  int		sts;
  int		found;
  int		change_detected = 0;

  brow_SetNodraw( xnav->brow->ctx);
  brow_GetObjectList( xnav->brow->ctx, &node_list, &node_cnt);

  // Reset found
  object_p = node_list + 2;
  for ( i = 2; i < node_cnt; i++)
  {    
    brow_GetUserData( *object_p, (void **)&item);
    item->found = 0;
    object_p++;    
  }

  xnav_subcli_remote_cnt = 0;
  gdb_ScopeLock {
    for (
	  nl = pool_Qsucc(&sts, gdbroot->pool, &gdbroot->db->nod_lh);
	  nl != &gdbroot->db->nod_lh;
	  nl = pool_Qsucc(&sts, gdbroot->pool, nl)) 
    {
      np = pool_Qitem(nl, gdb_sNode, nod_ll);
      for (
	    sl = pool_Qsucc(&sts, gdbroot->pool, &np->subc_lh);
	    sl != &np->subc_lh;
	    sl = pool_Qsucc(&sts, gdbroot->pool, sl))
      {
        sclip = pool_Qitem(sl, sub_sClient, subc_ll);

        object_p = node_list + 2;
        found = 0;
        for ( i = 1; i < node_cnt; i++)
        {    
          brow_GetUserData( *object_p, (void **)&item);
          if ( memcmp( item->col.elem[0].value_p, &sclip->sid, sizeof(pwr_tSubid))
		 == 0)
          {
            found = 1;
            break;
          }		
          object_p++;
        }
        if ( !found)
        {
          xnav_show_subcli_insert( xnav, np, sclip);
          change_detected = 1;
        }
        else
          item->found = 1;
      }
      if ( !(np == gdbroot->my_node || np == gdbroot->no_node))
        xnav_subcli_remote_cnt += np->subc_lc;
    }
  } gdb_ScopeUnlock;

  // Remove all that were not found
  object_p = node_list + 2;
  for ( i = 2; i < node_cnt; i++)
  {    
    brow_GetUserData( *object_p, (void **)&item);
    if ( ! item->found)
    {
      brow_DeleteNode( xnav->brow->ctx, item->node);
      change_detected = 1;
    }
    else
      object_p++;
  }
  brow_ResetNodraw( xnav->brow->ctx);
  if ( change_detected)
    brow_Redraw( xnav->brow->ctx, 0);
}


int XNav::show_device()
{
  item_sTableHeader 	th;
  item_sTable 		t;
  item_sTableSubid	ts;
  pwr_tTypeId		attrtype;
  unsigned int		attrsize, attroffs, attrelem;
  pwr_tSubid		subid;
  char			object_name[120];
  char			attr_name[140];
  int			sts;
  pwr_tObjid		rack_objid;
  pwr_tObjid		device_objid;
  pwr_sAttrRef		attrref;
  pwr_tClassId		classid;
  pwr_tUInt32		process;
  pwr_tUInt32		address;
  pwr_tObjid		thread_objid;
  char			namebuf[80];
  int			rack_class_cnt;
  pwr_tClassId		*rack_class;
  int			i;
  void			*attr_ptr;

  brow_pop();
  brow_SetNodraw( brow->ctx);

  th.table_cnt = 0;
  strcpy( th.title[th.table_cnt++], "Device");
  strcpy( th.title[th.table_cnt++], "Class");
  strcpy( th.title[th.table_cnt++], "ErrorCount");
  strcpy( th.title[th.table_cnt++], "Process");
  strcpy( th.title[th.table_cnt++], "PlcThread");
  strcpy( th.title[th.table_cnt++], "Address (oct)");
  new ItemTableHeader( brow, this, "Title", &th,  NULL, flow_eDest_IntoLast);

  // Get the rack objects
  sts = io_GetIoTypeClasses( io_eType_Rack, &rack_class, &rack_class_cnt);
  if ( EVEN(sts)) return sts;

  for ( i = 0; i < rack_class_cnt; i++)
  {
    sts = gdh_GetClassList ( rack_class[i], &rack_objid);
    while (ODD(sts))
    {
      // Get all children
      sts = gdh_GetChild( rack_objid, &device_objid);
      while( ODD(sts))
      {

        sts = gdh_ObjidToName ( device_objid, object_name,
			sizeof(object_name), cdh_mName_volumeStrict);
        if ( EVEN(sts)) return sts;


        t.elem_cnt = 0;
        ts.subid_cnt = 0;

        // Object name
	xnav_cut_segments( namebuf, object_name, 2);

        strcpy( t.elem[t.elem_cnt].fix_str, namebuf);
        t.elem[t.elem_cnt++].type_id = xnav_eType_FixStr;

        // Class name
        sts = gdh_GetObjectClass( device_objid, &classid);
        sts = gdh_ObjidToName( cdh_ClassIdToObjid( classid),
		  namebuf, sizeof(namebuf), cdh_mName_object);
        strcpy( t.elem[t.elem_cnt].fix_str, namebuf);
        t.elem[t.elem_cnt++].type_id = xnav_eType_FixStr;

        // ErrorCount
        strcpy( attr_name, object_name);
        strcat( attr_name, ".ErrorCount");
        sts = gdh_GetAttributeCharacteristics ( attr_name,
		&attrtype, &attrsize, &attroffs, &attrelem);
        if ( EVEN(sts)) {
          strcpy( t.elem[t.elem_cnt].fix_str, "       -");
	  t.elem[t.elem_cnt++].type_id = xnav_eType_FixStr;
	}
	else {
	  sts = gdh_NameToAttrref( pwr_cNObjid, attr_name, &attrref);
	  if ( EVEN(sts)) return sts;

	  sts = gdh_DLRefObjectInfoAttrref ( &attrref, &attr_ptr, &subid);
	  if ( EVEN(sts)) return sts;

	  t.elem[t.elem_cnt].value_p = attr_ptr;
	  t.elem[t.elem_cnt].type_id = attrtype;
	  t.elem[t.elem_cnt].size = attrsize;
	  strcpy( t.elem[t.elem_cnt++].format, "%8d");
	  ts.subid[ts.subid_cnt++] = subid;
	}

        // Process
        strcpy( attr_name, object_name);
        strcat( attr_name, ".Process");
        sts = gdh_GetObjectInfo( attr_name,
		(void *) &process, sizeof(process));
        if ( ODD(sts))
          sprintf( t.elem[t.elem_cnt].fix_str, "%4d", process);
        else
          strcpy( t.elem[t.elem_cnt].fix_str, "-");
        t.elem[t.elem_cnt++].type_id = xnav_eType_FixStr;

        // ThreadObject
        strcpy( attr_name, object_name);
        strcat( attr_name, ".ThreadObject");
        sts = gdh_GetObjectInfo( attr_name,
		(void *) &thread_objid, sizeof(thread_objid));
        if ( ODD(sts))
        {
          sts = gdh_ObjidToName ( thread_objid, namebuf,
			sizeof(namebuf), cdh_mName_object);
          if ( ODD(sts))
            strcpy( t.elem[t.elem_cnt].fix_str, namebuf);
          else
            strcpy( t.elem[t.elem_cnt].fix_str, "-");
        }
        else
          strcpy( t.elem[t.elem_cnt].fix_str, "-");
        t.elem[t.elem_cnt++].type_id = xnav_eType_FixStr;

        // RegAddress
        strcpy( attr_name, object_name);
        strcat( attr_name, ".RegAddress");
        sts = gdh_GetObjectInfo( attr_name,
		(void *) &address, sizeof(address));
        if ( ODD(sts))
          sprintf( t.elem[t.elem_cnt].fix_str, "%o", address);
        else
          strcpy( t.elem[t.elem_cnt].fix_str, "-");
        t.elem[t.elem_cnt++].type_id = xnav_eType_FixStr;

        new ItemDevice( brow, this, device_objid, &t, &ts, -1, 0, 0, 0, 
		NULL, flow_eDest_IntoLast);
        sts = gdh_GetNextSibling ( device_objid, &device_objid);
      }
      sts = gdh_GetNextObject ( rack_objid, &rack_objid);
    }
  }
  free( (char *)rack_class);

  brow_ResetNodraw( brow->ctx);
  brow_Redraw( brow->ctx, 0);
  force_trace_scan();
  return XNAV__SUCCESS;
}

int XNav::show_channels( pwr_tObjid card_objid)
{
  item_sTableHeader 	th;
  item_sTable 		t;
  item_sTableSubid	ts;
  pwr_tTypeId		attrtype;
  unsigned int		attrsize, attroffs, attrelem;
  pwr_tSubid		subid;
  char			object_name[120];
  char			attr_name[140];
  int			sts;
  pwr_tObjid		chan_objid;
  pwr_sAttrRef		attrref;
  pwr_tObjid		signal_objid;
  char			descr[80];
  char			namebuf[80];
  void			*attr_ptr;
  ItemChannel		*item;
  pwr_tClassId		chan_classid;
  char			signal_name[120];

  brow_pop();
  brow_SetNodraw( brow->ctx);

  th.table_cnt = 0;
  strcpy( th.title[th.table_cnt++], "Channel");
  strcpy( th.title[th.table_cnt++], "Value");
  strcpy( th.title[th.table_cnt++], "Flags");
  strcpy( th.title[th.table_cnt++], "Signal");
  strcpy( th.title[th.table_cnt++], "");
  strcpy( th.title[th.table_cnt++], "");
  strcpy( th.title[th.table_cnt++], "");
  strcpy( th.title[th.table_cnt++], "Description");
  new ItemTableHeader( brow, this, "Title", &th,  NULL, flow_eDest_IntoLast);

  // Get all children
  sts = gdh_GetChild( card_objid, &chan_objid);
  while( ODD(sts))
  {

    sts = gdh_ObjidToName ( chan_objid, object_name,
			sizeof(object_name), cdh_mName_volumeStrict);
    if ( EVEN(sts)) return sts;

    sts = gdh_GetObjectClass ( chan_objid, &chan_classid);
    if ( EVEN(sts)) return sts;

    // Get connected signal
    strcpy( attr_name, object_name);
    strcat( attr_name, ".SigChanCon");
    sts = gdh_GetObjectInfo( attr_name,
		(void *) &signal_objid, sizeof(signal_objid));
    if ( ODD(sts))
    {
      sts = gdh_ObjidToName ( signal_objid, signal_name,
			sizeof(signal_name), cdh_mNName);
      if ( EVEN(sts))
      {
        signal_objid = pwr_cNObjid;
        strcpy( signal_name, "-");
      }
    }
    else
      signal_objid = pwr_cNObjid;

    t.elem_cnt = 0;
    ts.subid_cnt = 0;

    // Object name
    xnav_cut_segments( namebuf, object_name, 2);

    strcpy( t.elem[t.elem_cnt].fix_str, namebuf);
    t.elem[t.elem_cnt++].type_id = xnav_eType_FixStr;

    // Value
    if ( cdh_ObjidIsNotNull( signal_objid))
    {
      strcpy( attr_name, signal_name);
      switch( chan_classid)
      {
        case pwr_cClass_ChanDi:
        case pwr_cClass_ChanDo:
        case pwr_cClass_ChanAi:
        case pwr_cClass_ChanAo:
          strcat( attr_name, ".ActualValue");
          break;
        case pwr_cClass_ChanCo:
          strcat( attr_name, ".RawValue");
          break;
      }
      sts = gdh_GetAttributeCharacteristics ( attr_name,
		&attrtype, &attrsize, &attroffs, &attrelem);
      if ( EVEN(sts)) return sts;

      sts = gdh_NameToAttrref( pwr_cNObjid, attr_name, &attrref);
      if ( EVEN(sts)) return sts;

      sts = gdh_DLRefObjectInfoAttrref ( &attrref, &attr_ptr, &subid);
      if ( EVEN(sts)) return sts;

      t.elem[t.elem_cnt].value_p = attr_ptr;
      t.elem[t.elem_cnt].type_id = attrtype;
      t.elem[t.elem_cnt].size = attrsize;

      switch( chan_classid)
      {
        case pwr_cClass_ChanDi:
        case pwr_cClass_ChanDo:
        case pwr_cClass_ChanCo:
          strcpy( t.elem[t.elem_cnt++].format, "%8d");
          break;
        case pwr_cClass_ChanAi:
        case pwr_cClass_ChanAo:
          strcpy( t.elem[t.elem_cnt++].format, "%f");
          break;
      }
      ts.subid[ts.subid_cnt++] = subid;
    }
    else
      t.elem[t.elem_cnt++].type_id = xnav_eType_Empty;

    // Flags
    t.elem[t.elem_cnt++].type_id = xnav_eType_Empty;

    // Signal
    if ( cdh_ObjidIsNotNull( signal_objid))
    {
      strcpy( t.elem[t.elem_cnt].fix_str, signal_name);
    }
    else
      strcpy( t.elem[t.elem_cnt].fix_str, "-");
    t.elem[t.elem_cnt++].type_id = xnav_eType_FixStr;


    // Description in signal
    t.elem[t.elem_cnt++].type_id = xnav_eType_Empty;
    t.elem[t.elem_cnt++].type_id = xnav_eType_Empty;
    t.elem[t.elem_cnt++].type_id = xnav_eType_Empty;

    if ( cdh_ObjidIsNotNull( signal_objid))
      strcpy( attr_name, signal_name);
    else
      strcpy( attr_name, object_name);
    strcat( attr_name, ".Description");
    sts = gdh_GetObjectInfo( attr_name,
		(void *) &descr, sizeof(descr));
    if ( EVEN(sts)) return sts;

    strcpy( t.elem[t.elem_cnt].fix_str, descr);
    t.elem[t.elem_cnt++].type_id = xnav_eType_FixStr;

    item = new ItemChannel( brow, this, chan_objid, &t, &ts, -1, 0, 0, 1, 
		NULL, flow_eDest_IntoLast);
    item->signal_objid = signal_objid;

    sts = gdh_GetNextSibling ( chan_objid, &chan_objid);
  }

  brow_ResetNodraw( brow->ctx);
  brow_Redraw( brow->ctx, 0);
  force_trace_scan();
  return XNAV__SUCCESS;
}




int XNav::show_object( pwr_tObjid objid, brow_tNode node)
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
    if ( attr_exist)
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

int XNav::show_remnode()
{
  item_sTableHeader 	th;
  item_sTable 		t;
  item_sTableSubid	ts;
  char			object_name[120];
  char			id[40];
  char			description[80];
  int			sts;
  pwr_tObjid		objid;
  char			namebuf[80];
  pwr_tCid		cid;
  void			*object_ptr;

  brow_pop();
  brow_SetNodraw( brow->ctx);

  th.table_cnt = 0;
  strcpy( th.title[th.table_cnt++], "RemNode");
  strcpy( th.title[th.table_cnt++], "");
  strcpy( th.title[th.table_cnt++], "Type");
  strcpy( th.title[th.table_cnt++], "Description");
  new ItemTableHeader( brow, this, "Title", &th,  NULL, flow_eDest_IntoLast);

  for ( int i = 0; i < 6; i++) {
    switch ( i) {
    case 0: cid = pwr_cClass_RemnodeUDP; break;
    case 1: cid = pwr_cClass_RemnodeTCP; break;
    case 2: cid = pwr_cClass_Remnode3964R; break;
    case 3: cid = pwr_cClass_RemnodeALCM; break;
    case 4: cid = pwr_cClass_RemnodeSerial; break;
    case 5: cid = pwr_cClass_RemnodeModbus; break;
    }

    sts = gdh_GetClassList( cid, &objid);
    while ( ODD(sts)) {
      sts = gdh_ObjidToPointer( objid, (void **) &object_ptr);
      if ( EVEN(sts)) return sts;

      sts = gdh_ObjidToName ( objid, object_name,
			      sizeof(object_name), cdh_mName_volumeStrict);
      if ( EVEN(sts)) return sts;

      switch ( i) {
      case 0: 
	strncpy( id, ((pwr_sClass_RemnodeUDP *)object_ptr)->Id, sizeof(id));
	strncpy( description, ((pwr_sClass_RemnodeUDP *)object_ptr)->Description, 
		 sizeof(description));
	break;
      case 1: 
	strncpy( id, ((pwr_sClass_RemnodeTCP *)object_ptr)->Id, sizeof(id));
	strncpy( description, ((pwr_sClass_RemnodeTCP *)object_ptr)->Description, 
		 sizeof(description));
	break;
      case 2: 
	strncpy( id, ((pwr_sClass_Remnode3964R *)object_ptr)->Id, sizeof(id));
	strncpy( description, ((pwr_sClass_Remnode3964R *)object_ptr)->Description, 
		 sizeof(description));
	break;
      case 3: 
	strncpy( id, ((pwr_sClass_RemnodeALCM *)object_ptr)->Id, sizeof(id));
	strncpy( description, ((pwr_sClass_RemnodeALCM *)object_ptr)->Description, 
		 sizeof(description));
	break;
      case 4: 
	strncpy( id, ((pwr_sClass_RemnodeSerial *)object_ptr)->Id, sizeof(id));
	strncpy( description, ((pwr_sClass_RemnodeSerial *)object_ptr)->Description, 
		 sizeof(description));
	break;
      case 5: 
	strncpy( id, ((pwr_sClass_RemnodeModbus *)object_ptr)->Id, sizeof(id));
	strncpy( description, ((pwr_sClass_RemnodeModbus *)object_ptr)->Description, 
		 sizeof(description));
	break;
      }

      t.elem_cnt = 0;

      // Object name
      xnav_cut_segments( namebuf, object_name, 2);

      strcpy( t.elem[t.elem_cnt].fix_str, namebuf);
      t.elem[t.elem_cnt++].type_id = xnav_eType_FixStr;

      t.elem[t.elem_cnt++].type_id = xnav_eType_Empty;
      
      // Type
      strcpy( t.elem[t.elem_cnt].fix_str, id);
      t.elem[t.elem_cnt++].type_id = xnav_eType_FixStr;

      // Description
      strcpy( t.elem[t.elem_cnt].fix_str, description);
      t.elem[t.elem_cnt++].type_id = xnav_eType_FixStr;

      ts.subid_cnt = 0;
      new ItemRemNode( brow, this, objid, &t, &ts, -1, 0, 0, 1, NULL, flow_eDest_IntoLast);

      sts = gdh_GetNextObject ( objid, &objid);
    }
  }

  brow_ResetNodraw( brow->ctx);
  brow_Redraw( brow->ctx, 0);
  force_trace_scan();
  return XNAV__SUCCESS;
}

int XNav::show_remtrans( pwr_tObjid remnode_objid)
{
  item_sTableHeader 	th;
  item_sTable 		t;
  item_sTableSubid	ts;
  pwr_tTypeId		attrtype;
  unsigned int		attrsize, attroffs, attrelem;
  pwr_tSubid		subid;
  char			object_name[120];
  char			attr_name[140];
  int			sts;
  pwr_tObjid		objid;
  pwr_sAttrRef		attrref;
  pwr_sClass_RemTrans	*object_ptr;
  char			namebuf[80];

  brow_pop();
  brow_SetNodraw( brow->ctx);

  th.table_cnt = 0;
  strcpy( th.title[th.table_cnt++], "RemTrans");
  strcpy( th.title[th.table_cnt++], "");
  strcpy( th.title[th.table_cnt++], "DataValid");
  strcpy( th.title[th.table_cnt++], "Direction");
  strcpy( th.title[th.table_cnt++], "TransCnt");
  strcpy( th.title[th.table_cnt++], "TransTime");
  strcpy( th.title[th.table_cnt++], "");
  strcpy( th.title[th.table_cnt++], "ErrCount");
  strcpy( th.title[th.table_cnt++], "LastSts");
  new ItemTableHeader( brow, this, "Title", &th,  NULL, flow_eDest_IntoLast);

  if ( cdh_ObjidIsNull( remnode_objid))
    sts = gdh_GetClassList ( pwr_cClass_RemTrans, &objid);
  else
    sts = gdh_GetChild( remnode_objid, &objid);
  while ( ODD(sts))
  {
    memset( &attrref, 0, sizeof( attrref));
    attrref.Objid = objid;
    sts = gdh_DLRefObjectInfoAttrref ( &attrref,
		(pwr_tAddress *) &object_ptr,
		&subid);
    if ( EVEN(sts)) return sts;

    sts = gdh_ObjidToName ( objid, object_name,
			sizeof(object_name), cdh_mName_volumeStrict);
    if ( EVEN(sts)) return sts;

    t.elem_cnt = 0;

    // Object name
    xnav_cut_segments( namebuf, object_name, 1);

    strcpy( t.elem[t.elem_cnt].fix_str, namebuf);
    t.elem[t.elem_cnt++].type_id = xnav_eType_FixStr;

    t.elem[t.elem_cnt++].type_id = xnav_eType_Empty;

    // DataValid
    strcpy( attr_name, object_name);
    strcat( attr_name, ".DataValid");
    sts = gdh_GetAttributeCharacteristics ( attr_name,
		&attrtype, &attrsize, &attroffs, &attrelem);
    if ( EVEN(sts)) return sts;

    t.elem[t.elem_cnt].value_p = &object_ptr->DataValid;
    t.elem[t.elem_cnt].type_id = attrtype;
    t.elem[t.elem_cnt].size = attrsize;
    strcpy( t.elem[t.elem_cnt++].format, "%2d");

    // Direction
    switch( object_ptr->Direction)
    {
      case 1:
        strcpy( t.elem[t.elem_cnt].fix_str, "Rcv");
        break;
      case 2:
        strcpy( t.elem[t.elem_cnt].fix_str, "Snd");
        break;
    }
    t.elem[t.elem_cnt++].type_id = xnav_eType_FixStr;

    // TransCount
    strcpy( attr_name, object_name);
    strcat( attr_name, ".TransCount");
    sts = gdh_GetAttributeCharacteristics ( attr_name,
		&attrtype, &attrsize, &attroffs, &attrelem);
    if ( EVEN(sts)) return sts;

    t.elem[t.elem_cnt].value_p = &object_ptr->TransCount;
    t.elem[t.elem_cnt].type_id = attrtype;
    t.elem[t.elem_cnt].size = attrsize;
    strcpy( t.elem[t.elem_cnt++].format, "%2d");

    // TransTime
    strcpy( attr_name, object_name);
    strcat( attr_name, ".TransTime");
    sts = gdh_GetAttributeCharacteristics ( attr_name,
		&attrtype, &attrsize, &attroffs, &attrelem);
    if ( EVEN(sts)) return sts;

    t.elem[t.elem_cnt].value_p = &object_ptr->TransTime;
    t.elem[t.elem_cnt].type_id = attrtype;
    t.elem[t.elem_cnt].size = attrsize;
    strcpy( t.elem[t.elem_cnt++].format, "");

    t.elem[t.elem_cnt++].type_id = xnav_eType_Empty;

    // ErrCount
    strcpy( attr_name, object_name);
    strcat( attr_name, ".ErrCount");
    sts = gdh_GetAttributeCharacteristics ( attr_name,
		&attrtype, &attrsize, &attroffs, &attrelem);
    if ( EVEN(sts)) return sts;

    t.elem[t.elem_cnt].value_p = &object_ptr->ErrCount;
    t.elem[t.elem_cnt].type_id = attrtype;
    t.elem[t.elem_cnt].size = attrsize;
    strcpy( t.elem[t.elem_cnt++].format, "%8d");

    // LastSts
    strcpy( attr_name, object_name);
    strcat( attr_name, ".LastSts");
    sts = gdh_GetAttributeCharacteristics ( attr_name,
		&attrtype, &attrsize, &attroffs, &attrelem);
    if ( EVEN(sts)) return sts;

    t.elem[t.elem_cnt].value_p = &object_ptr->LastSts;
    t.elem[t.elem_cnt].type_id = attrtype;
    t.elem[t.elem_cnt].size = attrsize;
    strcpy( t.elem[t.elem_cnt++].format, "%8d");

    ts.subid[0] = subid;
    ts.subid_cnt = 1;
    new ItemRemTrans( brow, this, objid, &t, &ts, -1, 0, 0, 0, NULL,
		      flow_eDest_IntoLast);

    if ( cdh_ObjidIsNull( remnode_objid))
      sts = gdh_GetNextObject( objid, &objid);
    else
      sts = gdh_GetNextSibling( objid, &objid);
  }

  brow_ResetNodraw( brow->ctx);
  brow_Redraw( brow->ctx, 0);
  force_trace_scan();
  return XNAV__SUCCESS;
}

int XNav::show_plcpgm()
{
  item_sTableHeader 	th;
  item_sTable 		t;
  item_sTableSubid	ts;
  char			object_name[120];
  char			attr_name[140];
  int			sts;
  pwr_tObjid		objid;
  pwr_sClass_PlcThread	*object_ptr;
  pwr_tObjid		thread_objid;
  char			namebuf[80];
  pwr_tObjid		parent;
  pwr_tCid		cid;

  brow_pop();
  brow_SetNodraw( brow->ctx);

  th.table_cnt = 0;
  strcpy( th.title[th.table_cnt++], "PlcPgm");
  strcpy( th.title[th.table_cnt++], "");
  strcpy( th.title[th.table_cnt++], "");
  strcpy( th.title[th.table_cnt++], "");
  strcpy( th.title[th.table_cnt++], "PlcThread");
  strcpy( th.title[th.table_cnt++], "Description");
  new ItemTableHeader( brow, this, "Title", &th,  NULL, flow_eDest_IntoLast);

  sts = gdh_GetClassList ( pwr_cClass_plc, &objid);
  while ( ODD(sts))
  {
    // Don't display template plcpgm in class volumes
    sts = gdh_GetParent( objid, &parent);
    if ( ODD(sts)) {
      sts = gdh_GetObjectClass( parent, &cid);
      if ( EVEN(sts)) return sts;

      if ( cid == pwr_eClass_ClassDef) {
	sts = gdh_GetNextObject ( objid, &objid);
	continue;
      }
    }

    sts = gdh_ObjidToPointer( objid, (void **) &object_ptr);
    if ( EVEN(sts)) return sts;

    sts = gdh_ObjidToName ( objid, object_name,
			sizeof(object_name), cdh_mNName);
    if ( EVEN(sts)) return sts;

    t.elem_cnt = 0;

    // Object name
    strcpy( t.elem[t.elem_cnt].fix_str, object_name);
    t.elem[t.elem_cnt++].type_id = xnav_eType_FixStr;

    t.elem[t.elem_cnt++].type_id = xnav_eType_Empty;
    t.elem[t.elem_cnt++].type_id = xnav_eType_Empty;
    t.elem[t.elem_cnt++].type_id = xnav_eType_Empty;

    // ThreadObject
    strcpy( attr_name, object_name);
    strcat( attr_name, ".ThreadObject");
    sts = gdh_GetObjectInfo( attr_name,
		(void *) &thread_objid, sizeof(thread_objid));
    if ( ODD(sts))
    {
      sts = gdh_ObjidToName ( thread_objid, namebuf,
			sizeof(namebuf), cdh_mName_object);
      if ( ODD(sts))
        strcpy( t.elem[t.elem_cnt].fix_str, namebuf);
      else
        strcpy( t.elem[t.elem_cnt].fix_str, "-");
    }
    else
      strcpy( t.elem[t.elem_cnt].fix_str, "-");
    t.elem[t.elem_cnt++].type_id = xnav_eType_FixStr;

    // Description
    strcpy( t.elem[t.elem_cnt].fix_str, object_ptr->Description);
    t.elem[t.elem_cnt++].type_id = xnav_eType_FixStr;

    ts.subid_cnt = 0;
    new ItemPlc( brow, this, objid, &t, &ts, -1, 0, 0, 1, NULL, flow_eDest_IntoLast);

    sts = gdh_GetNextObject ( objid, &objid);
  }

  brow_ResetNodraw( brow->ctx);
  brow_Redraw( brow->ctx, 0);
  force_trace_scan();
  return XNAV__SUCCESS;
}


