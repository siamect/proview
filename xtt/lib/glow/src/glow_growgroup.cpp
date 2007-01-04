/* 
 * Proview   $Id: glow_growgroup.cpp,v 1.5 2007-01-04 07:57:38 claes Exp $
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


#include <iostream.h>
#include <float.h>
#include <math.h>
#include <stdlib.h>
#include "glow_growgroup.h"
#include "glow_draw.h"
#include "glow_growctx.h"
#include "glow_nodegroup.h"

GrowGroup::GrowGroup( GrowCtx *glow_ctx, char *name, GlowArray& array,
	int nodraw) :
	GrowNode(glow_ctx,name,0,0,0,nodraw,0)
{
  object_type = glow_eObjectType_GrowGroup;
  nc = new GlowNodeGroup( glow_ctx, name, array);
  get_node_borders();
}

GrowGroup::GrowGroup( GrowCtx *glow_ctx, char *name) :
	GrowNode(glow_ctx,name,0,0,0)
{
  object_type = glow_eObjectType_GrowGroup;
}

GrowGroup::~GrowGroup()
{
  erase( &ctx->mw);
  erase( &ctx->navw);

  ctx->set_defered_redraw();
  ctx->delete_node_cons( this);
  ctx->draw( &ctx->mw, x_left * ctx->mw.zoom_factor_x - ctx->mw.offset_x - DRAW_MP,
	     y_low * ctx->mw.zoom_factor_y - ctx->mw.offset_y - DRAW_MP,
  	     x_right * ctx->mw.zoom_factor_x - ctx->mw.offset_x + DRAW_MP,
	     y_high * ctx->mw.zoom_factor_y - ctx->mw.offset_y + DRAW_MP);
  ctx->draw( &ctx->navw, x_left * ctx->navw.zoom_factor_x - ctx->navw.offset_x - 1,
	     y_low * ctx->navw.zoom_factor_y - ctx->navw.offset_y - 1,
  	     x_right * ctx->navw.zoom_factor_x - ctx->navw.offset_x + 1,
	     y_high * ctx->navw.zoom_factor_y - ctx->navw.offset_y + 1);
  ctx->redraw_defered();
  if ( hot)
    ctx->gdraw->set_cursor( &ctx->mw, glow_eDrawCursor_Normal);

  delete nc;
  nc = 0;
}

void GrowGroup::copy_from( const GrowGroup& n) 
{
  memcpy( this, &n, sizeof(n));
  if ( n.dynamicsize) {
    dynamic = (char *) calloc( 1, n.dynamicsize);
    memcpy( dynamic, n.nc->dynamic, n.dynamicsize);
  }
  // Get unic name
  sprintf( n_name, "Grp%d_", ((GrowCtx *)ctx)->objectname_cnt++); 

  nc = new GlowNodeGroup( (GlowNodeGroup &)*n.nc);

  if ( ctx->userdata_copy_callback)
    (ctx->userdata_copy_callback)( this, user_data, &user_data, glow_eUserdataCbType_Node);
}

void GrowGroup::ungroup()
{
  nc->a.set_last_group( n_name);
  ((GlowNodeGroup *)nc)->ungroup( &trf);
}

void GrowGroup::save( ofstream& fp, glow_eSaveMode mode) 
{ 

  fp << int(glow_eSave_GrowGroup) << endl;
  fp << int(glow_eSave_GrowGroup_grownode_part) << endl;
  GrowNode::save( fp, mode);
  fp << int(glow_eSave_GrowGroup_nc) << endl;
  ((GlowNodeGroup *)nc)->save( fp, mode);
  fp << int(glow_eSave_End) << endl;
}

void GrowGroup::open( ifstream& fp)
{
  int		type;
  int 		end_found = 0;
  char		dummy[40];

  for (;;)
  {
    fp >> type;
    switch( type) {
      case glow_eSave_GrowGroup: break;
      case glow_eSave_GrowGroup_grownode_part: 
        GrowNode::open( fp);
        break;
      case glow_eSave_GrowGroup_nc: 
        nc = new GlowNodeGroup( ctx, n_name);
        nc->open( fp);
        break;
      case glow_eSave_End: end_found = 1; break;
      default:
        cout << "GrowGroup:open syntax error" << endl;
        fp.getline( dummy, sizeof(dummy));
    }
    if ( end_found)
      break;
  }
}
 
void GrowGroup::trace_scan()
{
  if ( trace.p && ctx->trace_scan_func)
    ctx->trace_scan_func( (void *) this, trace.p);

  nc->a.trace_scan();
}

int GrowGroup::trace_init()
{
  int sts;

  //  if ( strcmp( trace.data[0], "") != 0)
  sts = ctx->trace_connect_func( (void *) this, &trace);

  nc->a.trace_init();
  return sts;
}

void GrowGroup::trace_close()
{
  if ( trace.p)
    ctx->trace_disconnect_func( (void *) this);

  nc->a.trace_close();
}

void GrowGroup::get_nodegroups( void *a)
{
  ((GlowArray *)a)->insert( nc);

  // To do, this isn't implemented yet
  nc->a.get_nodegroups( a);
}

int GrowGroup::get_object_group( GlowArrayElem *object, GlowArrayElem **group)
{
  int sts;

  for ( int i = 0; i < nc->a.size(); i++) {
    if ( nc->a[i]->type() == glow_eObjectType_GrowGroup) {
      sts = ((GrowGroup *)nc->a[i])->get_object_group( object, group);
      if (ODD(sts))
        return sts;
    }
    else if ( nc->a[i] == object) {
      *group = this;
      return 1;
    }
  }
  return 0;
}

int GrowGroup::get_background_object_limits(GlowTransform *t,
		    glow_eTraceType type,
		    double x, double y, GlowArrayElem **background,
		    double *min, double *max, glow_eDirection *direction)
{
  int sts;

  if ( t) {
    GlowTransform trf_tot = *t * trf;
    sts = nc->a.get_background_object_limits( &trf_tot, type, x, y,
					      background, min, max, direction);
  }
  else
    sts = nc->a.get_background_object_limits( &trf, type, x, y,
					      background, min, max, direction);
  return sts;
}

GlowArrayElem *GrowGroup::get_node_from_name( char *name)
{
  int i;
        
  for ( i = 0; i < nc->a.a_size; i++) {
    if ( (nc->a.a[i]->type() == glow_eObjectType_Node ||
          nc->a.a[i]->type() == glow_eObjectType_GrowNode ||
          nc->a.a[i]->type() == glow_eObjectType_GrowConGlue) &&
	 strcmp( ((GlowNode *) nc->a.a[i])->n_name, name) == 0)
      return nc->a.a[i];
    else if ( nc->a.a[i]->type() == glow_eObjectType_GrowGroup) {
      GlowArrayElem *n = ((GrowGroup *)nc->a.a[i])->get_node_from_name( name);
      if ( n)
	return n;
    }
  }
  return 0;
}

void GrowGroup::call_redraw_node_cons()
{
  ctx->redraw_node_cons( this);
        
  for ( int i = 0; i < nc->a.a_size; i++) {
    nc->a.a[i]->call_redraw_node_cons();
  }
}

void GrowGroup::link_insert( void **start)
{
  for ( int i = 0; i < nc->a.a_size; i++) {
    if ( nc->a[i]->type() == glow_eObjectType_Node ||
	 nc->a[i]->type() == glow_eObjectType_GrowNode ||
	 nc->a[i]->type() == glow_eObjectType_GrowGroup) 
    nc->a.a[i]->link_insert( start);
  }
}

void GrowGroup::convert( glow_eConvert version) 
{
  switch ( version) {
  case glow_eConvert_V34: {
    // Conversion of colors
    GrowNode::convert( version);
    for ( int i = 0; i < nc->a.a_size; i++) {
      nc->a.a[i]->convert( version);
    }

    break;
  }
  }  
}

void GrowGroup::set_rootnode( void *node)
{
  nc->a.set_rootnode( node);
}





