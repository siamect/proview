#include "flow_std.h"

#include <iostream.h>
#include <fstream.h>
#include "flow_nodeclass.h"
#include "flow_annot.h"
#include "flow_annotpixmap.h"
#include "flow_conpoint.h"
#include "flow_msg.h"

FlowNodeClass::FlowNodeClass( FlowCtx *flow_ctx, char *name, 
	flow_eNodeGroup grp)
  : ctx(flow_ctx), a(10,10), group(grp)
{
  strcpy( nc_name, name);
}

void FlowNodeClass::print( FlowPoint *pos, void *node, int highlight)
{
  int		i;

  for ( i = 0; i < a.a_size; i++)
  {
    a.a[i]->print( pos, node, highlight);
  }
}

void FlowNodeClass::save( ofstream& fp, flow_eSaveMode mode)
{

  if ( (mode == flow_eSaveMode_Trace && group != flow_eNodeGroup_Trace) ||
       (mode == flow_eSaveMode_Edit && group == flow_eNodeGroup_Trace) )
    return;
  fp <<	int(flow_eSave_NodeClass) << endl;
  fp <<	int(flow_eSave_NodeClass_nc_name) << FSPACE << nc_name << endl;
  fp <<	int(flow_eSave_NodeClass_a) << endl;
  a.save( fp, mode);
  fp <<	int(flow_eSave_NodeClass_group) << FSPACE << int(group) << endl;
  fp <<	int(flow_eSave_End) << endl;
}

void FlowNodeClass::open( ifstream& fp)
{
  int		type;
  int 		end_found = 0;
  char		dummy[40];
  int		tmp;

  for (;;)
  {
    fp >> type;
    switch( type) {
      case flow_eSave_NodeClass: break;
      case flow_eSave_NodeClass_nc_name:
        fp.get();
        fp.getline( nc_name, sizeof(nc_name));
        break;
      case flow_eSave_NodeClass_a: a.open( ctx, fp); break;
      case flow_eSave_NodeClass_group: fp >> tmp; group = (flow_eNodeGroup)tmp; break;
      case flow_eSave_End: end_found = 1; break;
      default:
        cout << "FlowNodeClass:open syntax error" << endl;
        fp.getline( dummy, sizeof(dummy));
    }
    if ( end_found)
      break;
  }
}

void FlowNodeClass::draw( FlowPoint *pos, int highlight, int hot, void *node)
{
  int		i;

  for ( i = 0; i < a.a_size; i++)
  {
    a.a[i]->draw( pos, highlight, hot, node);
  }
}

void FlowNodeClass::nav_draw( FlowPoint *pos, int highlight, void *node)
{
  int		i;

  for ( i = 0; i < a.a_size; i++)
  {
    a.a[i]->nav_draw( pos, highlight, node);
  }
}

void FlowNodeClass::draw_inverse( FlowPoint *pos, int hot, void *node)
{
  int		i;

  for ( i = 0; i < a.a_size; i++)
  {
    switch ( a.a[i]->type())
    { 
      case flow_eObjectType_Radiobutton:
      case flow_eObjectType_Image:
        a.a[i]->draw( pos, 0, hot, node);
        break;
      default:
        a.a[i]->draw_inverse( pos, hot, node);
    }
  }
}

void FlowNodeClass::erase( FlowPoint *pos, int hot, void *node)
{
  int		i;

  for ( i = 0; i < a.a_size; i++)
  {
    a.a[i]->erase( pos, hot, node);
  }
}

void FlowNodeClass::nav_erase( FlowPoint *pos, void *node)
{
  int		i;

  for ( i = 0; i < a.a_size; i++)
  {
    a.a[i]->nav_erase( pos, node);
  }
}

int FlowNodeClass::get_conpoint( int num, double *x, double *y, 
	flow_eDirection *dir)
{
  int		i, sts;

  for ( i = 0; i < a.a_size; i++)
  {
    sts = a.a[i]->get_conpoint( num, x, y, dir);
    if ( sts)
      return sts;
  }
  return FLOW__NOCONPOINT;
}

int FlowNodeClass::get_conpoint_trace_attr( int num, char *trace_attr, 
	flow_eTraceType *type)
{
  int		i;

  for ( i = 0; i < a.a_size; i++)
  {
    if ( a.a[i]->type() == flow_eObjectType_ConPoint &&
         ((FlowConPoint *)a.a[i])->number == num)
    {
      a.a[i]->get_trace_attr( NULL, trace_attr, type);
      return FLOW__SUCCESS;
    }
  }
  return FLOW__NOCONPOINT;
}

int FlowNodeClass::event_handler( void *pos, flow_eEvent event, int x, int y,
	void *node)
{
  return a.event_handler( pos, event, x, y, node);
}

void FlowNodeClass::erase_annotation( void *pos, int highlight, int hot, 
	void *node, int num)
{
  int		i;

  for ( i = 0; i < a.a_size; i++)
  {
    if ( a.a[i]->type() == flow_eObjectType_Annot &&
         ((FlowAnnot *)a.a[i])->number == num)
    {
      a.a[i]->erase( pos, hot, node);
      a.a[i]->nav_erase( pos, node);
      break;
    }
  }
}

void FlowNodeClass::draw_annotation( void *pos, int highlight, int hot, 
	void *node, int num)
{
  int		i;

  for ( i = 0; i < a.a_size; i++)
  {
    if ( a.a[i]->type() == flow_eObjectType_Annot &&
         ((FlowAnnot *)a.a[i])->number == num)
    {
      a.a[i]->draw( pos, highlight, hot, node);
      a.a[i]->nav_draw( pos, highlight, node);
      break;
    }
  }
}

void FlowNodeClass::open_annotation_input( void *pos, void *node, int num)
{
  int		i;

  for ( i = 0; i < a.a_size; i++)
  {
    if ( a.a[i]->type() == flow_eObjectType_Annot &&
         ((FlowAnnot *)a.a[i])->number == num)
    {
      ((FlowAnnot *)a.a[i])->open_annotation_input( pos, node);
      break;
    }
  }
}

void FlowNodeClass::close_annotation_input( void *node, int num)
{
  int		i;

  for ( i = 0; i < a.a_size; i++)
  {
    if ( a.a[i]->type() == flow_eObjectType_Annot &&
         ((FlowAnnot *)a.a[i])->number == num)
    {
      ((FlowAnnot *)a.a[i])->close_annotation_input( node);
      break;
    }
  }
}

int FlowNodeClass::get_annotation_input( void *node, int num, char **text)
{
  int		i, sts;

  for ( i = 0; i < a.a_size; i++)
  {
    if ( a.a[i]->type() == flow_eObjectType_Annot &&
         ((FlowAnnot *)a.a[i])->number == num)
    {
      sts = ((FlowAnnot *)a.a[i])->get_annotation_input( node, text);
      break;
    }
  }
  return sts;
}

void FlowNodeClass::move_widgets( void *node, int x, int y)
{
  for ( int i = 0; i < a.a_size; i++)
  {
    if ( a.a[i]->type() == flow_eObjectType_Annot )
      ((FlowAnnot *)a.a[i])->move_widgets( node, x, y);
  }
}

void FlowNodeClass::configure_annotations( void *pos, void *node)
{
  int		i;

  for ( i = 0; i < a.a_size; i++)
  {
    if ( a.a[i]->type() == flow_eObjectType_Annot)
    {
      ((FlowAnnot *)a.a[i])->configure_annotations( pos, node);
    }
    else if ( a.a[i]->type() == flow_eObjectType_AnnotPixmap)
    {
      ((FlowAnnotPixmap *)a.a[i])->configure_annotations( pos, node);
    }
  }
}

void FlowNodeClass::measure_annotation( int num, char *text, double *width,
	double *height)
{
  int		i;

  for ( i = 0; i < a.a_size; i++)
  {
    if ( a.a[i]->type() == flow_eObjectType_Annot &&
         ((FlowAnnot *)a.a[i])->number == num)
    {
      ((FlowAnnot *)a.a[i])->measure_annot( text, width, height);
      break;
    }
  }
}

void FlowNodeClass::get_obstacle_borders( double pos_x, double pos_y, double *x_right, 
		double *x_left, double *y_high, double *y_low, void *node)
{ 
  int i;

  switch ( group)
  {
    case flow_eNodeGroup_Document:
      for ( i = 0; i < a.a_size; i++)
      {
        if ( a.a[i]->type() == flow_eObjectType_Rect)
          a.a[i]->get_borders(pos_x, pos_y, x_right, x_left, y_high, y_low, node);
      }
      break;
    default:
      a.get_borders(pos_x, pos_y, x_right, x_left, y_high, y_low, node);
  }
}

void FlowNodeClass::get_object_name( char *name)
{
  strcpy( name, nc_name);
}


