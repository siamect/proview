#include "flow_std.h"

#include <iostream.h>
#include <fstream.h>
#include "flow_conclass.h"

FlowConClass::FlowConClass( FlowCtx *flow_ctx, char *name, 
	flow_eConType contype, flow_eCorner cornertype, flow_eDrawType d_type,
	int line_w, double arrow_w, double arrow_l, double round_corner_amnt,
	flow_eConGroup grp)
  : ctx(flow_ctx), zero(flow_ctx,0,0), con_type(contype), corner(cornertype),
    draw_type(d_type), line_width(line_w), arrow_width(arrow_w),
    arrow_length(arrow_l), round_corner_amount(round_corner_amnt), group(grp)
{
  strcpy( cc_name, name);
  zero.nav_zoom();
  zero.print_zoom();
}

void FlowConClass::save( ofstream& fp, flow_eSaveMode mode)
{

  if ( (mode == flow_eSaveMode_Trace && group != flow_eConGroup_Trace) ||
       (mode == flow_eSaveMode_Edit && group == flow_eConGroup_Trace) )
    return;
  fp <<	int(flow_eSave_ConClass) << endl;
  fp <<	int(flow_eSave_ConClass_cc_name) << FSPACE << cc_name << endl;
  fp <<	int(flow_eSave_ConClass_con_type) << FSPACE << int(con_type) << endl;
  fp <<	int(flow_eSave_ConClass_corner) << FSPACE << int(corner) << endl;
  fp <<	int(flow_eSave_ConClass_draw_type) << FSPACE << int(draw_type) << endl;
  fp <<	int(flow_eSave_ConClass_line_width) << FSPACE << line_width << endl;
  fp <<	int(flow_eSave_ConClass_arrow_width) << FSPACE << arrow_width << endl;
  fp <<	int(flow_eSave_ConClass_arrow_length) << FSPACE << arrow_length << endl;
  fp <<	int(flow_eSave_ConClass_round_corner_amount) << FSPACE << round_corner_amount << endl;
  fp <<	int(flow_eSave_ConClass_group) << FSPACE << int(group) << endl;
  fp <<	int(flow_eSave_End) << endl;
}

void FlowConClass::open( ifstream& fp)
{
  int		type;
  int 		end_found = 0;
  char		dummy[40];
  int		tmp;

  for (;;)
  {
    fp >> type;
    switch( type) {
      case flow_eSave_ConClass: break;
      case flow_eSave_ConClass_cc_name: 
        fp.get();
        fp.getline( cc_name, sizeof(cc_name));
        break;
      case flow_eSave_ConClass_con_type: fp >> tmp; con_type = (flow_eConType)tmp; break;
      case flow_eSave_ConClass_corner: fp >> tmp; corner = (flow_eCorner)tmp; break;
      case flow_eSave_ConClass_draw_type: fp >> tmp; draw_type = (flow_eDrawType)tmp; break;
      case flow_eSave_ConClass_line_width: fp >> line_width; break;
      case flow_eSave_ConClass_arrow_width: fp >> arrow_width; break;
      case flow_eSave_ConClass_arrow_length: fp >> arrow_length; break;
      case flow_eSave_ConClass_round_corner_amount: fp >> round_corner_amount; break;
      case flow_eSave_ConClass_group: fp >> tmp; group = (flow_eConGroup)tmp; break;
      case flow_eSave_End: end_found = 1; break;
      default:
        cout << "FlowConClass:open syntax error" << endl;
        fp.getline( dummy, sizeof(dummy));
    }
    if ( end_found)
      break;
  }
}

void FlowConClass::get_object_name( char *name)
{
  strcpy( name, cc_name);
}

ostream& operator<< ( ostream& o, const FlowConClass cc)
{
  o << "ConClass: " << cc.cc_name;
  return o;
}

