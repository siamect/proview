#include "glow_std.h"


#include <iostream.h>
#include <float.h>
#include <math.h>
#include <stdlib.h>
#include "glow_growgroup.h"
#include "glow_draw.h"
#include "glow_growctx.h"
#include "glow_nodegroup.h"

GrowGroup::GrowGroup( GlowCtx *glow_ctx, char *name, GlowArray& array,
	int nodraw) :
	GrowNode(glow_ctx,name,0,0,0,nodraw,0)
{
  object_type = glow_eObjectType_GrowGroup;
  nc = new GlowNodeGroup( glow_ctx, name, array);
  get_node_borders();
}

GrowGroup::GrowGroup( GlowCtx *glow_ctx, char *name) :
	GrowNode(glow_ctx,name,0,0,0)
{
  object_type = glow_eObjectType_GrowGroup;
}

GrowGroup::~GrowGroup()
{
  erase();
  nav_erase();

  ctx->set_defered_redraw();
  ctx->delete_node_cons( this);
  ((GrowCtx *)ctx)->draw( x_left * ctx->zoom_factor_x - ctx->offset_x - DRAW_MP,
	     y_low * ctx->zoom_factor_y - ctx->offset_y - DRAW_MP,
  	     x_right * ctx->zoom_factor_x - ctx->offset_x + DRAW_MP,
	     y_high * ctx->zoom_factor_y - ctx->offset_y + DRAW_MP);
  ((GrowCtx *)ctx)->nav_draw(  x_left * ctx->nav_zoom_factor_x - ctx->nav_offset_x - 1,
	     y_low * ctx->nav_zoom_factor_y - ctx->nav_offset_y - 1,
  	     x_right * ctx->nav_zoom_factor_x - ctx->nav_offset_x + 1,
	     y_high * ctx->nav_zoom_factor_y - ctx->nav_offset_y + 1);
  ctx->redraw_defered();
  if ( hot)
    draw_set_cursor( ctx, glow_eDrawCursor_Normal);

  delete nc;
  nc = 0;
}

void GrowGroup::copy_from( const GrowGroup& n) 
{
  memcpy( this, &n, sizeof(n));
  if ( n.dynamicsize)
  {
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





