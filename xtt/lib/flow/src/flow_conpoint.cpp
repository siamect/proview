#include "flow_std.h"


#include <iostream.h>
#include <math.h>

#include "flow_conpoint.h"
#include "flow_draw.h"

void FlowConPoint::zoom()
{
  p.zoom();
}

void FlowConPoint::nav_zoom()
{
  p.nav_zoom();
}

void FlowConPoint::print_zoom()
{
  p.print_zoom();
}

void FlowConPoint::save( ofstream& fp, flow_eSaveMode mode)
{
  fp <<	int(flow_eSave_ConPoint) << endl;
  fp <<	int(flow_eSave_ConPoint_number) << FSPACE << number << endl;
  fp <<	int(flow_eSave_ConPoint_direction) << FSPACE << int(direction) << endl;
  fp <<	int(flow_eSave_ConPoint_p) << endl;
  p.save( fp, mode);
  fp << int(flow_eSave_ConPoint_trace_attribute) << FSPACE << trace_attribute << endl;
  fp << int(flow_eSave_ConPoint_trace_attr_type) << FSPACE << int(trace_attr_type) << endl;
  fp <<	int(flow_eSave_End) << endl;
}

void FlowConPoint::open( ifstream& fp)
{
  int		type;
  int 		end_found = 0;
  char		dummy[40];
  int		tmp;

  for (;;)
  {
    fp >> type;
    switch( type) {
      case flow_eSave_ConPoint: break;
      case flow_eSave_ConPoint_number: fp >> number; break;
      case flow_eSave_ConPoint_direction: fp >> tmp; direction = (flow_eDirection)tmp; break;
      case flow_eSave_ConPoint_p: p.open( fp); break;
      case flow_eSave_ConPoint_trace_attribute:
        fp.get();
        fp.getline( trace_attribute, sizeof(trace_attribute));
        break;
      case flow_eSave_ConPoint_trace_attr_type: fp >> tmp; trace_attr_type = (flow_eTraceType)tmp; break;
      case flow_eSave_End: end_found = 1; break;
      default:
        cout << "FlowConPoint:open syntax error" << endl;
        fp.getline( dummy, sizeof(dummy));
    }
    if ( end_found)
      break;
  }
}


void FlowConPoint::traverse( int x, int y)
{
  p.traverse( x, y);
}

int FlowConPoint::event_handler( void *pos, flow_eEvent event, int x, int y,
	void *node)
{
  return 0;
}

void FlowConPoint::conpoint_select( void *pos, int x, int y, 
	double *distance, void **cp)
{
  int px, py;
  double dist;

  px = ((FlowPoint *)pos)->z_x - ctx->offset_x + p.z_x;
  py = ((FlowPoint *)pos)->z_y - ctx->offset_y + p.z_y;

  dist = sqrt( 1.0*(x-px)*(x-px) + 1.0*(y-py)*(y-py));
  if ( dist < *distance)
  {
    *distance = dist;
    *cp = (void *) this;
  }
}

int FlowConPoint::get_conpoint( int num, double *x, double *y,
	flow_eDirection *dir)
{
  if ( number == num)
  {
    *x = p.x;
    *y = p.y;
    *dir = direction;
    return 1;
  }
  return 0;
}

void FlowConPoint::set_trace_attr( char *object, char *attribute, 
	flow_eTraceType type)
{
  strncpy( trace_attribute, attribute, sizeof( trace_attribute));
  trace_attr_type = type;
}

void FlowConPoint::get_trace_attr( char *object, char *attribute, 
	flow_eTraceType *type)
{
  strncpy( attribute, trace_attribute, sizeof( trace_attribute));
  *type = trace_attr_type;
}

ostream& operator<< ( ostream& o, const FlowConPoint cp)
{
  o << '(' << cp.p.x << ',' << cp.p.y << ')'; 
  return o;
}

