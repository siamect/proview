#include "glow_std.h"

#include <iostream.h>
#include <fstream.h>
#include "glow_conclass.h"

GlowConClass::GlowConClass( GlowCtx *glow_ctx, char *name, 
	glow_eConType contype, glow_eCorner cornertype, glow_eDrawType d_type,
	int line_w, double arrow_w, double arrow_l, double round_corner_amnt,
	glow_eConGroup grp)
  : ctx(glow_ctx), zero(glow_ctx,0,0), con_type(contype), corner(cornertype),
    draw_type(d_type), line_width(line_w), arrow_width(arrow_w),
    arrow_length(arrow_l), round_corner_amount(round_corner_amnt), group(grp)
{
  strcpy( cc_name, name);
  zero.nav_zoom();
  zero.print_zoom();
}

void GlowConClass::save( ofstream& fp, glow_eSaveMode mode)
{

  if ( (mode == glow_eSaveMode_Trace && group != glow_eConGroup_Trace) ||
       (mode == glow_eSaveMode_Edit && group == glow_eConGroup_Trace) )
    return;
  fp <<	int(glow_eSave_ConClass) << endl;
  fp <<	int(glow_eSave_ConClass_cc_name) << FSPACE << cc_name << endl;
  fp <<	int(glow_eSave_ConClass_con_type) << FSPACE << int(con_type) << endl;
  fp <<	int(glow_eSave_ConClass_corner) << FSPACE << int(corner) << endl;
  fp <<	int(glow_eSave_ConClass_draw_type) << FSPACE << int(draw_type) << endl;
  fp <<	int(glow_eSave_ConClass_line_width) << FSPACE << line_width << endl;
  fp <<	int(glow_eSave_ConClass_arrow_width) << FSPACE << arrow_width << endl;
  fp <<	int(glow_eSave_ConClass_arrow_length) << FSPACE << arrow_length << endl;
  fp <<	int(glow_eSave_ConClass_round_corner_amount) << FSPACE << round_corner_amount << endl;
  fp <<	int(glow_eSave_ConClass_group) << FSPACE << int(group) << endl;
  fp <<	int(glow_eSave_End) << endl;
}

void GlowConClass::open( ifstream& fp)
{
  int		type;
  int 		end_found = 0;
  char		dummy[40];
  int		tmp;

  for (;;)
  {
    fp >> type;
    switch( type) {
      case glow_eSave_ConClass: break;
      case glow_eSave_ConClass_cc_name: 
        fp.get();
        fp.getline( cc_name, sizeof(cc_name));
        break;
      case glow_eSave_ConClass_con_type: fp >> tmp; con_type = (glow_eConType)tmp; break;
      case glow_eSave_ConClass_corner: fp >> tmp; corner = (glow_eCorner)tmp; break;
      case glow_eSave_ConClass_draw_type: fp >> tmp; draw_type = (glow_eDrawType)tmp; break;
      case glow_eSave_ConClass_line_width: fp >> line_width; break;
      case glow_eSave_ConClass_arrow_width: fp >> arrow_width; break;
      case glow_eSave_ConClass_arrow_length: fp >> arrow_length; break;
      case glow_eSave_ConClass_round_corner_amount: fp >> round_corner_amount; break;
      case glow_eSave_ConClass_group: fp >> tmp; group = (glow_eConGroup)tmp; break;
      case glow_eSave_End: end_found = 1; break;
      default:
        cout << "GlowConClass:open syntax error" << endl;
        fp.getline( dummy, sizeof(dummy));
    }
    if ( end_found)
      break;
  }
}

void GlowConClass::get_object_name( char *name)
{
  strcpy( name, cc_name);
}

void GlowConClass::convert( glow_eConvert version) 
{
  switch ( version) {
  case glow_eConvert_V34: {
    // Conversion of colors
    draw_type = GlowColor::convert( version, draw_type);

    break;
  }
  }  
}


ostream& operator<< ( ostream& o, const GlowConClass cc)
{
  o << "ConClass: " << cc.cc_name;
  return o;
}

