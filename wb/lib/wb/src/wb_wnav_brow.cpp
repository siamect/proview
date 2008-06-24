/* 
 * Proview   $Id: wb_wnav_brow.cpp,v 1.10 2008-06-24 07:52:21 claes Exp $
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

#include "flow_std.h"

#include <stdio.h>
#include <stdlib.h>
#if defined(OS_VMS)
#include <descrip.h>
#endif

#include "pwr_privilege.h"
#include "co_cdh.h"
#include "co_dcli.h"
#include "co_msg.h"
#include "co_time.h"
#include "pwr_baseclasses.h"
#include "wb_wnav_msg.h"
#include "wb_ldh.h"
#include "flow.h"
#include "flow_browctx.h"
#include "flow_browapi.h"

#include "wb_wnav_brow.h"
#include "wb_wnav.h"
#include "wb_wnav_item.h"


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
#include "xnav_bitmap_openattr8.h"
#include "xnav_bitmap_openattr10.h"
#include "xnav_bitmap_openattr12.h"
#include "xnav_bitmap_openattr14.h"
#include "xnav_bitmap_openattr16.h"
#include "xnav_bitmap_openattr18.h"
#include "xnav_bitmap_openattr20.h"
#include "xnav_bitmap_openattr24.h"
#include "xnav_bitmap_attr8.h"
#include "xnav_bitmap_attr10.h"
#include "xnav_bitmap_attr12.h"
#include "xnav_bitmap_attr14.h"
#include "xnav_bitmap_attr16.h"
#include "xnav_bitmap_attr18.h"
#include "xnav_bitmap_attr20.h"
#include "xnav_bitmap_attr24.h"
#include "xnav_bitmap_attrenum12.h"
#include "xnav_bitmap_attrmask12.h"
#include "xnav_bitmap_attrarra8.h"
#include "xnav_bitmap_attrarra10.h"
#include "xnav_bitmap_attrarra12.h"
#include "xnav_bitmap_attrarra14.h"
#include "xnav_bitmap_attrarra16.h"
#include "xnav_bitmap_attrarra18.h"
#include "xnav_bitmap_attrarra20.h"
#include "xnav_bitmap_attrarra24.h"
#include "xnav_bitmap_attrarel8.h"
#include "xnav_bitmap_attrarel10.h"
#include "xnav_bitmap_attrarel12.h"
#include "xnav_bitmap_attrarel14.h"
#include "xnav_bitmap_attrarel16.h"
#include "xnav_bitmap_attrarel18.h"
#include "xnav_bitmap_attrarel20.h"
#include "xnav_bitmap_attrarel24.h"
#include "xnav_bitmap_crrread12.h"
#include "xnav_bitmap_crrwrite12.h"
#include "xnav_bitmap_script12.h"
#include "xnav_bitmap_graph12.h"
#include "xnav_bitmap_file12.h"
#include "xnav_bitmap_symbol12.h"
#include "xnav_bitmap_morehelp8.h"
#include "xnav_bitmap_morehelp10.h"
#include "xnav_bitmap_morehelp12.h"
#include "xnav_bitmap_morehelp14.h"
#include "xnav_bitmap_morehelp16.h"
#include "xnav_bitmap_morehelp18.h"
#include "xnav_bitmap_morehelp20.h"
#include "xnav_bitmap_morehelp24.h"
#include "xnav_bitmap_closehelp12.h"
#include "xnav_bitmap_objname8.h"
#include "xnav_bitmap_objname10.h"
#include "xnav_bitmap_objname12.h"
#include "xnav_bitmap_objname14.h"
#include "xnav_bitmap_objname16.h"
#include "xnav_bitmap_objname18.h"
#include "xnav_bitmap_objname20.h"
#include "xnav_bitmap_objname24.h"
#include "xnav_bitmap_object8.h"
#include "xnav_bitmap_object10.h"
#include "xnav_bitmap_object12.h"
#include "xnav_bitmap_object14.h"
#include "xnav_bitmap_object16.h"
#include "xnav_bitmap_object18.h"
#include "xnav_bitmap_object20.h"
#include "xnav_bitmap_object24.h"
#include "xnav_bitmap_attrref8.h"
#include "xnav_bitmap_attrref10.h"
#include "xnav_bitmap_attrref12.h"
#include "xnav_bitmap_attrref14.h"
#include "xnav_bitmap_attrref16.h"
#include "xnav_bitmap_attrref18.h"
#include "xnav_bitmap_attrref20.h"
#include "xnav_bitmap_attrref24.h"
#include "xnav_bitmap_attrxref12.h"
#include "xnav_bitmap_ref_err12.h"
#include "xnav_bitmap_crossref_err12.h"
#include "xnav_bitmap_attrref_err12.h"
#include "xnav_bitmap_attrxref_err12.h"
#include "xnav_bitmap_docblock12.h"
#include "xnav_bitmap_project8.h"
#include "xnav_bitmap_project10.h"
#include "xnav_bitmap_project12.h"
#include "xnav_bitmap_project14.h"
#include "xnav_bitmap_project16.h"
#include "xnav_bitmap_project18.h"
#include "xnav_bitmap_project20.h"
#include "xnav_bitmap_project24.h"
#include "xnav_bitmap_database8.h"
#include "xnav_bitmap_database10.h"
#include "xnav_bitmap_database12.h"
#include "xnav_bitmap_database14.h"
#include "xnav_bitmap_database16.h"
#include "xnav_bitmap_database18.h"
#include "xnav_bitmap_database20.h"
#include "xnav_bitmap_database24.h"
#include "xnav_bitmap_volume8.h"
#include "xnav_bitmap_volume10.h"
#include "xnav_bitmap_volume12.h"
#include "xnav_bitmap_volume14.h"
#include "xnav_bitmap_volume16.h"
#include "xnav_bitmap_volume18.h"
#include "xnav_bitmap_volume20.h"
#include "xnav_bitmap_volume24.h"
#include "xnav_bitmap_castattr8.h"
#include "xnav_bitmap_castattr10.h"
#include "xnav_bitmap_castattr12.h"
#include "xnav_bitmap_castattr14.h"
#include "xnav_bitmap_castattr16.h"
#include "xnav_bitmap_castattr18.h"
#include "xnav_bitmap_castattr20.h"
#include "xnav_bitmap_castattr24.h"
#include "xnav_bitmap_uncastattr8.h"
#include "xnav_bitmap_uncastattr10.h"
#include "xnav_bitmap_uncastattr12.h"
#include "xnav_bitmap_uncastattr14.h"
#include "xnav_bitmap_uncastattr16.h"
#include "xnav_bitmap_uncastattr18.h"
#include "xnav_bitmap_uncastattr20.h"
#include "xnav_bitmap_uncastattr24.h"



#if 0
WNavBrow::~WNavBrow()
{
  if ( this != ((WNav *)wnav)->brow_stack[0] && this != ((WNav *)wnav)->brow)
  {
    brow_DeleteSecondaryCtx( ctx);
    free_pixmaps();
  }
}
#endif

void WNavBrow::brow_setup()
{
  brow_sAttributes brow_attr;
  unsigned long mask;

  mask = 0;
  mask |= brow_eAttr_indentation;
  brow_attr.indentation = 0.5;
  mask |= brow_eAttr_annotation_space;
  brow_attr.annotation_space = 0.5;
  brow_SetAttributes( ctx, &brow_attr, mask); 
  brow_SetCtxUserData( ctx, userdata);
}

//
//  Free pixmaps
//
void WNavBrow::free_pixmaps()
{
  brow_FreeAnnotPixmap( ctx, pixmap_leaf);
  brow_FreeAnnotPixmap( ctx, pixmap_map);
  brow_FreeAnnotPixmap( ctx, pixmap_openmap);
  brow_FreeAnnotPixmap( ctx, pixmap_ref);
  brow_FreeAnnotPixmap( ctx, pixmap_crossref);
  brow_FreeAnnotPixmap( ctx, pixmap_openattr);
  brow_FreeAnnotPixmap( ctx, pixmap_attr);
  brow_FreeAnnotPixmap( ctx, pixmap_attrarray);
  brow_FreeAnnotPixmap( ctx, pixmap_attrarrayelem);
  brow_FreeAnnotPixmap( ctx, pixmap_crrread);
  brow_FreeAnnotPixmap( ctx, pixmap_crrwrite);
  brow_FreeAnnotPixmap( ctx, pixmap_script);
  brow_FreeAnnotPixmap( ctx, pixmap_file);
  brow_FreeAnnotPixmap( ctx, pixmap_graph);
  brow_FreeAnnotPixmap( ctx, pixmap_symbol);
  brow_FreeAnnotPixmap( ctx, pixmap_morehelp);
  brow_FreeAnnotPixmap( ctx, pixmap_closehelp);
  brow_FreeAnnotPixmap( ctx, pixmap_objname);
  brow_FreeAnnotPixmap( ctx, pixmap_object);
  brow_FreeAnnotPixmap( ctx, pixmap_attrref);
  brow_FreeAnnotPixmap( ctx, pixmap_attrxref);
  brow_FreeAnnotPixmap( ctx, pixmap_ref_err);
  brow_FreeAnnotPixmap( ctx, pixmap_crossref_err);
  brow_FreeAnnotPixmap( ctx, pixmap_attrref_err);
  brow_FreeAnnotPixmap( ctx, pixmap_attrxref_err);
  brow_FreeAnnotPixmap( ctx, pixmap_project);
  brow_FreeAnnotPixmap( ctx, pixmap_database);
  brow_FreeAnnotPixmap( ctx, pixmap_volume);
  brow_FreeAnnotPixmap( ctx, pixmap_docblock);
}

//
//  Create pixmaps for leaf, closed map and open map
//
void WNavBrow::allocate_pixmaps()
{
	flow_sPixmapData pixmap_data;
	int i;
	
          i = 0;
	  pixmap_data[i].width =xnav_bitmap_leaf8_width;
	  pixmap_data[i].height =xnav_bitmap_leaf8_height;
	  pixmap_data[i++].bits = (char *)xnav_bitmap_leaf8_bits;
	  pixmap_data[i].width =xnav_bitmap_leaf10_width;
	  pixmap_data[i].height =xnav_bitmap_leaf10_height;
	  pixmap_data[i++].bits = (char *)xnav_bitmap_leaf10_bits;
	  pixmap_data[i].width =xnav_bitmap_leaf12_width;
	  pixmap_data[i].height =xnav_bitmap_leaf12_height;
	  pixmap_data[i++].bits = (char *)xnav_bitmap_leaf12_bits;
	  pixmap_data[i].width =xnav_bitmap_leaf14_width;
	  pixmap_data[i].height =xnav_bitmap_leaf14_height;
	  pixmap_data[i++].bits = (char *)xnav_bitmap_leaf14_bits;
	  pixmap_data[i].width =xnav_bitmap_leaf16_width;
	  pixmap_data[i].height =xnav_bitmap_leaf16_height;
	  pixmap_data[i++].bits = (char *)xnav_bitmap_leaf16_bits;
	  pixmap_data[i].width =xnav_bitmap_leaf18_width;
	  pixmap_data[i].height =xnav_bitmap_leaf18_height;
	  pixmap_data[i++].bits = (char *)xnav_bitmap_leaf18_bits;
	  pixmap_data[i].width =xnav_bitmap_leaf20_width;
	  pixmap_data[i].height =xnav_bitmap_leaf20_height;
	  pixmap_data[i++].bits = (char *)xnav_bitmap_leaf20_bits;
	  pixmap_data[i].width =xnav_bitmap_leaf20_width;
	  pixmap_data[i].height =xnav_bitmap_leaf20_height;
	  pixmap_data[i++].bits = (char *)xnav_bitmap_leaf20_bits;
	  pixmap_data[i].width =xnav_bitmap_leaf24_width;
	  pixmap_data[i].height =xnav_bitmap_leaf24_height;
	  pixmap_data[i++].bits = (char *)xnav_bitmap_leaf24_bits;

	  brow_AllocAnnotPixmap( ctx, &pixmap_data, &pixmap_leaf);

          i = 0;
	  pixmap_data[i].width =xnav_bitmap_map8_width;
	  pixmap_data[i].height =xnav_bitmap_map8_height;
	  pixmap_data[i++].bits = (char *)xnav_bitmap_map8_bits;
	  pixmap_data[i].width =xnav_bitmap_map10_width;
	  pixmap_data[i].height =xnav_bitmap_map10_height;
	  pixmap_data[i++].bits = (char *)xnav_bitmap_map10_bits;
	  pixmap_data[i].width =xnav_bitmap_map12_width;
	  pixmap_data[i].height =xnav_bitmap_map12_height;
	  pixmap_data[i++].bits = (char *)xnav_bitmap_map12_bits;
	  pixmap_data[i].width =xnav_bitmap_map14_width;
	  pixmap_data[i].height =xnav_bitmap_map14_height;
	  pixmap_data[i++].bits = (char *)xnav_bitmap_map14_bits;
	  pixmap_data[i].width =xnav_bitmap_map16_width;
	  pixmap_data[i].height =xnav_bitmap_map16_height;
	  pixmap_data[i++].bits = (char *)xnav_bitmap_map16_bits;
	  pixmap_data[i].width =xnav_bitmap_map18_width;
	  pixmap_data[i].height =xnav_bitmap_map18_height;
	  pixmap_data[i++].bits = (char *)xnav_bitmap_map18_bits;
	  pixmap_data[i].width =xnav_bitmap_map20_width;
	  pixmap_data[i].height =xnav_bitmap_map20_height;
	  pixmap_data[i++].bits = (char *)xnav_bitmap_map20_bits;
	  pixmap_data[i].width =xnav_bitmap_map20_width;
	  pixmap_data[i].height =xnav_bitmap_map20_height;
	  pixmap_data[i++].bits = (char *)xnav_bitmap_map20_bits;
	  pixmap_data[i].width =xnav_bitmap_map24_width;
	  pixmap_data[i].height =xnav_bitmap_map24_height;
	  pixmap_data[i++].bits = (char *)xnav_bitmap_map24_bits;

	  brow_AllocAnnotPixmap( ctx, &pixmap_data, &pixmap_map);

          i = 0;
	  pixmap_data[i].width =xnav_bitmap_openmap8_width;
	  pixmap_data[i].height =xnav_bitmap_openmap8_height;
	  pixmap_data[i++].bits = (char *)xnav_bitmap_openmap8_bits;
	  pixmap_data[i].width =xnav_bitmap_openmap10_width;
	  pixmap_data[i].height =xnav_bitmap_openmap10_height;
	  pixmap_data[i++].bits = (char *)xnav_bitmap_openmap10_bits;
	  pixmap_data[i].width =xnav_bitmap_openmap12_width;
	  pixmap_data[i].height =xnav_bitmap_openmap12_height;
	  pixmap_data[i++].bits = (char *)xnav_bitmap_openmap12_bits;
	  pixmap_data[i].width =xnav_bitmap_openmap14_width;
	  pixmap_data[i].height =xnav_bitmap_openmap14_height;
	  pixmap_data[i++].bits = (char *)xnav_bitmap_openmap14_bits;
	  pixmap_data[i].width =xnav_bitmap_openmap16_width;
	  pixmap_data[i].height =xnav_bitmap_openmap16_height;
	  pixmap_data[i++].bits = (char *)xnav_bitmap_openmap16_bits;
	  pixmap_data[i].width =xnav_bitmap_openmap18_width;
	  pixmap_data[i].height =xnav_bitmap_openmap18_height;
	  pixmap_data[i++].bits = (char *)xnav_bitmap_openmap18_bits;
	  pixmap_data[i].width =xnav_bitmap_openmap20_width;
	  pixmap_data[i].height =xnav_bitmap_openmap20_height;
	  pixmap_data[i++].bits = (char *)xnav_bitmap_openmap20_bits;
	  pixmap_data[i].width =xnav_bitmap_openmap20_width;
	  pixmap_data[i].height =xnav_bitmap_openmap20_height;
	  pixmap_data[i++].bits = (char *)xnav_bitmap_openmap20_bits;
	  pixmap_data[i].width =xnav_bitmap_openmap24_width;
	  pixmap_data[i].height =xnav_bitmap_openmap24_height;
	  pixmap_data[i++].bits = (char *)xnav_bitmap_openmap24_bits;

	  brow_AllocAnnotPixmap( ctx, &pixmap_data, &pixmap_openmap);

          i = 0;
	  pixmap_data[i].width =xnav_bitmap_ref8_width;
	  pixmap_data[i].height =xnav_bitmap_ref8_height;
	  pixmap_data[i++].bits = (char *)xnav_bitmap_ref8_bits;
	  pixmap_data[i].width =xnav_bitmap_ref10_width;
	  pixmap_data[i].height =xnav_bitmap_ref10_height;
	  pixmap_data[i++].bits = (char *)xnav_bitmap_ref10_bits;
	  pixmap_data[i].width =xnav_bitmap_ref12_width;
	  pixmap_data[i].height =xnav_bitmap_ref12_height;
	  pixmap_data[i++].bits = (char *)xnav_bitmap_ref12_bits;
	  pixmap_data[i].width =xnav_bitmap_ref14_width;
	  pixmap_data[i].height =xnav_bitmap_ref14_height;
	  pixmap_data[i++].bits = (char *)xnav_bitmap_ref14_bits;
	  pixmap_data[i].width =xnav_bitmap_ref16_width;
	  pixmap_data[i].height =xnav_bitmap_ref16_height;
	  pixmap_data[i++].bits = (char *)xnav_bitmap_ref16_bits;
	  pixmap_data[i].width =xnav_bitmap_ref18_width;
	  pixmap_data[i].height =xnav_bitmap_ref18_height;
	  pixmap_data[i++].bits = (char *)xnav_bitmap_ref18_bits;
	  pixmap_data[i].width =xnav_bitmap_ref20_width;
	  pixmap_data[i].height =xnav_bitmap_ref20_height;
	  pixmap_data[i++].bits = (char *)xnav_bitmap_ref20_bits;
	  pixmap_data[i].width =xnav_bitmap_ref20_width;
	  pixmap_data[i].height =xnav_bitmap_ref20_height;
	  pixmap_data[i++].bits = (char *)xnav_bitmap_ref20_bits;
	  pixmap_data[i].width =xnav_bitmap_ref24_width;
	  pixmap_data[i].height =xnav_bitmap_ref24_height;
	  pixmap_data[i++].bits = (char *)xnav_bitmap_ref24_bits;

	  brow_AllocAnnotPixmap( ctx, &pixmap_data, &pixmap_ref);

          i = 0;
	  pixmap_data[i].width =xnav_bitmap_crossref8_width;
	  pixmap_data[i].height =xnav_bitmap_crossref8_height;
	  pixmap_data[i++].bits = (char *)xnav_bitmap_crossref8_bits;
	  pixmap_data[i].width =xnav_bitmap_crossref10_width;
	  pixmap_data[i].height =xnav_bitmap_crossref10_height;
	  pixmap_data[i++].bits = (char *)xnav_bitmap_crossref10_bits;
	  pixmap_data[i].width =xnav_bitmap_crossref12_width;
	  pixmap_data[i].height =xnav_bitmap_crossref12_height;
	  pixmap_data[i++].bits = (char *)xnav_bitmap_crossref12_bits;
	  pixmap_data[i].width =xnav_bitmap_crossref14_width;
	  pixmap_data[i].height =xnav_bitmap_crossref14_height;
	  pixmap_data[i++].bits = (char *)xnav_bitmap_crossref14_bits;
	  pixmap_data[i].width =xnav_bitmap_crossref16_width;
	  pixmap_data[i].height =xnav_bitmap_crossref16_height;
	  pixmap_data[i++].bits = (char *)xnav_bitmap_crossref16_bits;
	  pixmap_data[i].width =xnav_bitmap_crossref18_width;
	  pixmap_data[i].height =xnav_bitmap_crossref18_height;
	  pixmap_data[i++].bits = (char *)xnav_bitmap_crossref18_bits;
	  pixmap_data[i].width =xnav_bitmap_crossref20_width;
	  pixmap_data[i].height =xnav_bitmap_crossref20_height;
	  pixmap_data[i++].bits = (char *)xnav_bitmap_crossref20_bits;
	  pixmap_data[i].width =xnav_bitmap_crossref20_width;
	  pixmap_data[i].height =xnav_bitmap_crossref20_height;
	  pixmap_data[i++].bits = (char *)xnav_bitmap_crossref20_bits;
	  pixmap_data[i].width =xnav_bitmap_crossref24_width;
	  pixmap_data[i].height =xnav_bitmap_crossref24_height;
	  pixmap_data[i++].bits = (char *)xnav_bitmap_crossref24_bits;

	  brow_AllocAnnotPixmap( ctx, &pixmap_data, &pixmap_crossref);

          i = 0;
	  pixmap_data[i].width =xnav_bitmap_openattr8_width;
	  pixmap_data[i].height =xnav_bitmap_openattr8_height;
	  pixmap_data[i++].bits = (char *)xnav_bitmap_openattr8_bits;
	  pixmap_data[i].width =xnav_bitmap_openattr10_width;
	  pixmap_data[i].height =xnav_bitmap_openattr10_height;
	  pixmap_data[i++].bits = (char *)xnav_bitmap_openattr10_bits;
	  pixmap_data[i].width =xnav_bitmap_openattr12_width;
	  pixmap_data[i].height =xnav_bitmap_openattr12_height;
	  pixmap_data[i++].bits = (char *)xnav_bitmap_openattr12_bits;
	  pixmap_data[i].width =xnav_bitmap_openattr14_width;
	  pixmap_data[i].height =xnav_bitmap_openattr14_height;
	  pixmap_data[i++].bits = (char *)xnav_bitmap_openattr14_bits;
	  pixmap_data[i].width =xnav_bitmap_openattr16_width;
	  pixmap_data[i].height =xnav_bitmap_openattr16_height;
	  pixmap_data[i++].bits = (char *)xnav_bitmap_openattr16_bits;
	  pixmap_data[i].width =xnav_bitmap_openattr18_width;
	  pixmap_data[i].height =xnav_bitmap_openattr18_height;
	  pixmap_data[i++].bits = (char *)xnav_bitmap_openattr18_bits;
	  pixmap_data[i].width =xnav_bitmap_openattr20_width;
	  pixmap_data[i].height =xnav_bitmap_openattr20_height;
	  pixmap_data[i++].bits = (char *)xnav_bitmap_openattr20_bits;
	  pixmap_data[i].width =xnav_bitmap_openattr20_width;
	  pixmap_data[i].height =xnav_bitmap_openattr20_height;
	  pixmap_data[i++].bits = (char *)xnav_bitmap_openattr20_bits;
	  pixmap_data[i].width =xnav_bitmap_openattr24_width;
	  pixmap_data[i].height =xnav_bitmap_openattr24_height;
	  pixmap_data[i++].bits = (char *)xnav_bitmap_openattr24_bits;

	  brow_AllocAnnotPixmap( ctx, &pixmap_data, 
		&pixmap_openattr);

          i = 0;
	  pixmap_data[i].width =xnav_bitmap_attr8_width;
	  pixmap_data[i].height =xnav_bitmap_attr8_height;
	  pixmap_data[i++].bits = (char *)xnav_bitmap_attr8_bits;
	  pixmap_data[i].width =xnav_bitmap_attr10_width;
	  pixmap_data[i].height =xnav_bitmap_attr10_height;
	  pixmap_data[i++].bits = (char *)xnav_bitmap_attr10_bits;
	  pixmap_data[i].width =xnav_bitmap_attr12_width;
	  pixmap_data[i].height =xnav_bitmap_attr12_height;
	  pixmap_data[i++].bits = (char *)xnav_bitmap_attr12_bits;
	  pixmap_data[i].width =xnav_bitmap_attr14_width;
	  pixmap_data[i].height =xnav_bitmap_attr14_height;
	  pixmap_data[i++].bits = (char *)xnav_bitmap_attr14_bits;
	  pixmap_data[i].width =xnav_bitmap_attr16_width;
	  pixmap_data[i].height =xnav_bitmap_attr16_height;
	  pixmap_data[i++].bits = (char *)xnav_bitmap_attr16_bits;
	  pixmap_data[i].width =xnav_bitmap_attr18_width;
	  pixmap_data[i].height =xnav_bitmap_attr18_height;
	  pixmap_data[i++].bits = (char *)xnav_bitmap_attr18_bits;
	  pixmap_data[i].width =xnav_bitmap_attr20_width;
	  pixmap_data[i].height =xnav_bitmap_attr20_height;
	  pixmap_data[i++].bits = (char *)xnav_bitmap_attr20_bits;
	  pixmap_data[i].width =xnav_bitmap_attr20_width;
	  pixmap_data[i].height =xnav_bitmap_attr20_height;
	  pixmap_data[i++].bits = (char *)xnav_bitmap_attr20_bits;
	  pixmap_data[i].width =xnav_bitmap_attr24_width;
	  pixmap_data[i].height =xnav_bitmap_attr24_height;
	  pixmap_data[i++].bits = (char *)xnav_bitmap_attr24_bits;

	  brow_AllocAnnotPixmap( ctx, &pixmap_data, &pixmap_attr);

          i = 0;
	  pixmap_data[i].width =xnav_bitmap_attrarra8_width;
	  pixmap_data[i].height =xnav_bitmap_attrarra8_height;
	  pixmap_data[i++].bits = (char *)xnav_bitmap_attrarra8_bits;
	  pixmap_data[i].width =xnav_bitmap_attrarra10_width;
	  pixmap_data[i].height =xnav_bitmap_attrarra10_height;
	  pixmap_data[i++].bits = (char *)xnav_bitmap_attrarra10_bits;
	  pixmap_data[i].width =xnav_bitmap_attrarra12_width;
	  pixmap_data[i].height =xnav_bitmap_attrarra12_height;
	  pixmap_data[i++].bits = (char *)xnav_bitmap_attrarra12_bits;
	  pixmap_data[i].width =xnav_bitmap_attrarra14_width;
	  pixmap_data[i].height =xnav_bitmap_attrarra14_height;
	  pixmap_data[i++].bits = (char *)xnav_bitmap_attrarra14_bits;
	  pixmap_data[i].width =xnav_bitmap_attrarra16_width;
	  pixmap_data[i].height =xnav_bitmap_attrarra16_height;
	  pixmap_data[i++].bits = (char *)xnav_bitmap_attrarra16_bits;
	  pixmap_data[i].width =xnav_bitmap_attrarra18_width;
	  pixmap_data[i].height =xnav_bitmap_attrarra18_height;
	  pixmap_data[i++].bits = (char *)xnav_bitmap_attrarra18_bits;
	  pixmap_data[i].width =xnav_bitmap_attrarra20_width;
	  pixmap_data[i].height =xnav_bitmap_attrarra20_height;
	  pixmap_data[i++].bits = (char *)xnav_bitmap_attrarra20_bits;
	  pixmap_data[i].width =xnav_bitmap_attrarra20_width;
	  pixmap_data[i].height =xnav_bitmap_attrarra20_height;
	  pixmap_data[i++].bits = (char *)xnav_bitmap_attrarra20_bits;
	  pixmap_data[i].width =xnav_bitmap_attrarra24_width;
	  pixmap_data[i].height =xnav_bitmap_attrarra24_height;
	  pixmap_data[i++].bits = (char *)xnav_bitmap_attrarra24_bits;

	  brow_AllocAnnotPixmap( ctx, &pixmap_data, &pixmap_attrarray);

          i = 0;
	  pixmap_data[i].width =xnav_bitmap_attrarel8_width;
	  pixmap_data[i].height =xnav_bitmap_attrarel8_height;
	  pixmap_data[i++].bits = (char *)xnav_bitmap_attrarel8_bits;
	  pixmap_data[i].width =xnav_bitmap_attrarel10_width;
	  pixmap_data[i].height =xnav_bitmap_attrarel10_height;
	  pixmap_data[i++].bits = (char *)xnav_bitmap_attrarel10_bits;
	  pixmap_data[i].width =xnav_bitmap_attrarel12_width;
	  pixmap_data[i].height =xnav_bitmap_attrarel12_height;
	  pixmap_data[i++].bits = (char *)xnav_bitmap_attrarel12_bits;
	  pixmap_data[i].width =xnav_bitmap_attrarel14_width;
	  pixmap_data[i].height =xnav_bitmap_attrarel14_height;
	  pixmap_data[i++].bits = (char *)xnav_bitmap_attrarel14_bits;
	  pixmap_data[i].width =xnav_bitmap_attrarel16_width;
	  pixmap_data[i].height =xnav_bitmap_attrarel16_height;
	  pixmap_data[i++].bits = (char *)xnav_bitmap_attrarel16_bits;
	  pixmap_data[i].width =xnav_bitmap_attrarel18_width;
	  pixmap_data[i].height =xnav_bitmap_attrarel18_height;
	  pixmap_data[i++].bits = (char *)xnav_bitmap_attrarel18_bits;
	  pixmap_data[i].width =xnav_bitmap_attrarel20_width;
	  pixmap_data[i].height =xnav_bitmap_attrarel20_height;
	  pixmap_data[i++].bits = (char *)xnav_bitmap_attrarel20_bits;
	  pixmap_data[i].width =xnav_bitmap_attrarel20_width;
	  pixmap_data[i].height =xnav_bitmap_attrarel20_height;
	  pixmap_data[i++].bits = (char *)xnav_bitmap_attrarel20_bits;
	  pixmap_data[i].width =xnav_bitmap_attrarel24_width;
	  pixmap_data[i].height =xnav_bitmap_attrarel24_height;
	  pixmap_data[i++].bits = (char *)xnav_bitmap_attrarel24_bits;

	  brow_AllocAnnotPixmap( ctx, &pixmap_data, &pixmap_attrarrayelem);

          for ( i = 0; i < 9; i++)
          {
	    pixmap_data[i].width =xnav_bitmap_attrenum12_width;
	    pixmap_data[i].height =xnav_bitmap_attrenum12_height;
	    pixmap_data[i].bits = (char *)xnav_bitmap_attrenum12_bits;
          }

	  brow_AllocAnnotPixmap( ctx, &pixmap_data, &pixmap_attrenum);

          for ( i = 0; i < 9; i++)
          {
	    pixmap_data[i].width =xnav_bitmap_attrmask12_width;
	    pixmap_data[i].height =xnav_bitmap_attrmask12_height;
	    pixmap_data[i].bits = (char *)xnav_bitmap_attrmask12_bits;
          }

	  brow_AllocAnnotPixmap( ctx, &pixmap_data, &pixmap_attrmask);
          for ( i = 0; i < 9; i++)
          {
	    pixmap_data[i].width =xnav_bitmap_crrread12_width;
	    pixmap_data[i].height =xnav_bitmap_crrread12_height;
	    pixmap_data[i].bits = (char *)xnav_bitmap_crrread12_bits;
          }

	  brow_AllocAnnotPixmap( ctx, &pixmap_data, &pixmap_crrread);

          for ( i = 0; i < 9; i++)
          {
	    pixmap_data[i].width =xnav_bitmap_crrwrite12_width;
	    pixmap_data[i].height =xnav_bitmap_crrwrite12_height;
	    pixmap_data[i].bits = (char *)xnav_bitmap_crrwrite12_bits;
          }

	  brow_AllocAnnotPixmap( ctx, &pixmap_data, &pixmap_crrwrite);

          for ( i = 0; i < 9; i++)
          {
	    pixmap_data[i].width =xnav_bitmap_script12_width;
	    pixmap_data[i].height =xnav_bitmap_script12_height;
	    pixmap_data[i].bits = (char *)xnav_bitmap_script12_bits;
          }

	  brow_AllocAnnotPixmap( ctx, &pixmap_data, &pixmap_script);

          for ( i = 0; i < 9; i++)
          {
	    pixmap_data[i].width =xnav_bitmap_graph12_width;
	    pixmap_data[i].height =xnav_bitmap_graph12_height;
	    pixmap_data[i].bits = (char *)xnav_bitmap_graph12_bits;
          }

	  brow_AllocAnnotPixmap( ctx, &pixmap_data, &pixmap_graph);

          for ( i = 0; i < 9; i++)
          {
	    pixmap_data[i].width =xnav_bitmap_file12_width;
	    pixmap_data[i].height =xnav_bitmap_file12_height;
	    pixmap_data[i].bits = (char *)xnav_bitmap_file12_bits;
          }

	  brow_AllocAnnotPixmap( ctx, &pixmap_data, &pixmap_file);

          for ( i = 0; i < 9; i++)
          {
	    pixmap_data[i].width =xnav_bitmap_symbol12_width;
	    pixmap_data[i].height =xnav_bitmap_symbol12_height;
	    pixmap_data[i].bits = (char *)xnav_bitmap_symbol12_bits;
          }

	  brow_AllocAnnotPixmap( ctx, &pixmap_data, &pixmap_symbol);

          i = 0;
	  pixmap_data[i].width =xnav_bitmap_morehelp8_width;
	  pixmap_data[i].height =xnav_bitmap_morehelp8_height;
	  pixmap_data[i++].bits = (char *)xnav_bitmap_morehelp8_bits;
	  pixmap_data[i].width =xnav_bitmap_morehelp10_width;
	  pixmap_data[i].height =xnav_bitmap_morehelp10_height;
	  pixmap_data[i++].bits = (char *)xnav_bitmap_morehelp10_bits;
	  pixmap_data[i].width =xnav_bitmap_morehelp12_width;
	  pixmap_data[i].height =xnav_bitmap_morehelp12_height;
	  pixmap_data[i++].bits = (char *)xnav_bitmap_morehelp12_bits;
	  pixmap_data[i].width =xnav_bitmap_morehelp14_width;
	  pixmap_data[i].height =xnav_bitmap_morehelp14_height;
	  pixmap_data[i++].bits = (char *)xnav_bitmap_morehelp14_bits;
	  pixmap_data[i].width =xnav_bitmap_morehelp16_width;
	  pixmap_data[i].height =xnav_bitmap_morehelp16_height;
	  pixmap_data[i++].bits = (char *)xnav_bitmap_morehelp16_bits;
	  pixmap_data[i].width =xnav_bitmap_morehelp18_width;
	  pixmap_data[i].height =xnav_bitmap_morehelp18_height;
	  pixmap_data[i++].bits = (char *)xnav_bitmap_morehelp18_bits;
	  pixmap_data[i].width =xnav_bitmap_morehelp20_width;
	  pixmap_data[i].height =xnav_bitmap_morehelp20_height;
	  pixmap_data[i++].bits = (char *)xnav_bitmap_morehelp20_bits;
	  pixmap_data[i].width =xnav_bitmap_morehelp20_width;
	  pixmap_data[i].height =xnav_bitmap_morehelp20_height;
	  pixmap_data[i++].bits = (char *)xnav_bitmap_morehelp20_bits;
	  pixmap_data[i].width =xnav_bitmap_morehelp24_width;
	  pixmap_data[i].height =xnav_bitmap_morehelp24_height;
	  pixmap_data[i++].bits = (char *)xnav_bitmap_morehelp24_bits;

	  brow_AllocAnnotPixmap( ctx, &pixmap_data, &pixmap_morehelp);

          for ( i = 0; i < 9; i++)
          {
	    pixmap_data[i].width =xnav_bitmap_closehelp12_width;
	    pixmap_data[i].height =xnav_bitmap_closehelp12_height;
	    pixmap_data[i].bits = (char *)xnav_bitmap_closehelp12_bits;
          }

	  brow_AllocAnnotPixmap( ctx, &pixmap_data, &pixmap_closehelp);

          i = 0;
	  pixmap_data[i].width =xnav_bitmap_objname8_width;
	  pixmap_data[i].height =xnav_bitmap_objname8_height;
	  pixmap_data[i++].bits = (char *)xnav_bitmap_objname8_bits;
	  pixmap_data[i].width =xnav_bitmap_objname10_width;
	  pixmap_data[i].height =xnav_bitmap_objname10_height;
	  pixmap_data[i++].bits = (char *)xnav_bitmap_objname10_bits;
	  pixmap_data[i].width =xnav_bitmap_objname12_width;
	  pixmap_data[i].height =xnav_bitmap_objname12_height;
	  pixmap_data[i++].bits = (char *)xnav_bitmap_objname12_bits;
	  pixmap_data[i].width =xnav_bitmap_objname14_width;
	  pixmap_data[i].height =xnav_bitmap_objname14_height;
	  pixmap_data[i++].bits = (char *)xnav_bitmap_objname14_bits;
	  pixmap_data[i].width =xnav_bitmap_objname16_width;
	  pixmap_data[i].height =xnav_bitmap_objname16_height;
	  pixmap_data[i++].bits = (char *)xnav_bitmap_objname16_bits;
	  pixmap_data[i].width =xnav_bitmap_objname18_width;
	  pixmap_data[i].height =xnav_bitmap_objname18_height;
	  pixmap_data[i++].bits = (char *)xnav_bitmap_objname18_bits;
	  pixmap_data[i].width =xnav_bitmap_objname20_width;
	  pixmap_data[i].height =xnav_bitmap_objname20_height;
	  pixmap_data[i++].bits = (char *)xnav_bitmap_objname20_bits;
	  pixmap_data[i].width =xnav_bitmap_objname20_width;
	  pixmap_data[i].height =xnav_bitmap_objname20_height;
	  pixmap_data[i++].bits = (char *)xnav_bitmap_objname20_bits;
	  pixmap_data[i].width =xnav_bitmap_objname24_width;
	  pixmap_data[i].height =xnav_bitmap_objname24_height;
	  pixmap_data[i++].bits = (char *)xnav_bitmap_objname24_bits;

	  brow_AllocAnnotPixmap( ctx, &pixmap_data, &pixmap_objname);

          i = 0;
	  pixmap_data[i].width =xnav_bitmap_object8_width;
	  pixmap_data[i].height =xnav_bitmap_object8_height;
	  pixmap_data[i++].bits = (char *)xnav_bitmap_object8_bits;
	  pixmap_data[i].width =xnav_bitmap_object10_width;
	  pixmap_data[i].height =xnav_bitmap_object10_height;
	  pixmap_data[i++].bits = (char *)xnav_bitmap_object10_bits;
	  pixmap_data[i].width =xnav_bitmap_object12_width;
	  pixmap_data[i].height =xnav_bitmap_object12_height;
	  pixmap_data[i++].bits = (char *)xnav_bitmap_object12_bits;
	  pixmap_data[i].width =xnav_bitmap_object14_width;
	  pixmap_data[i].height =xnav_bitmap_object14_height;
	  pixmap_data[i++].bits = (char *)xnav_bitmap_object14_bits;
	  pixmap_data[i].width =xnav_bitmap_object16_width;
	  pixmap_data[i].height =xnav_bitmap_object16_height;
	  pixmap_data[i++].bits = (char *)xnav_bitmap_object16_bits;
	  pixmap_data[i].width =xnav_bitmap_object18_width;
	  pixmap_data[i].height =xnav_bitmap_object18_height;
	  pixmap_data[i++].bits = (char *)xnav_bitmap_object18_bits;
	  pixmap_data[i].width =xnav_bitmap_object20_width;
	  pixmap_data[i].height =xnav_bitmap_object20_height;
	  pixmap_data[i++].bits = (char *)xnav_bitmap_object20_bits;
	  pixmap_data[i].width =xnav_bitmap_object20_width;
	  pixmap_data[i].height =xnav_bitmap_object20_height;
	  pixmap_data[i++].bits = (char *)xnav_bitmap_object20_bits;
	  pixmap_data[i].width =xnav_bitmap_object24_width;
	  pixmap_data[i].height =xnav_bitmap_object24_height;
	  pixmap_data[i++].bits = (char *)xnav_bitmap_object24_bits;

	  brow_AllocAnnotPixmap( ctx, &pixmap_data, &pixmap_object);

          i = 0;
	  pixmap_data[i].width =xnav_bitmap_attrref8_width;
	  pixmap_data[i].height =xnav_bitmap_attrref8_height;
	  pixmap_data[i++].bits = (char *)xnav_bitmap_attrref8_bits;
	  pixmap_data[i].width =xnav_bitmap_attrref10_width;
	  pixmap_data[i].height =xnav_bitmap_attrref10_height;
	  pixmap_data[i++].bits = (char *)xnav_bitmap_attrref10_bits;
	  pixmap_data[i].width =xnav_bitmap_attrref12_width;
	  pixmap_data[i].height =xnav_bitmap_attrref12_height;
	  pixmap_data[i++].bits = (char *)xnav_bitmap_attrref12_bits;
	  pixmap_data[i].width =xnav_bitmap_attrref14_width;
	  pixmap_data[i].height =xnav_bitmap_attrref14_height;
	  pixmap_data[i++].bits = (char *)xnav_bitmap_attrref14_bits;
	  pixmap_data[i].width =xnav_bitmap_attrref16_width;
	  pixmap_data[i].height =xnav_bitmap_attrref16_height;
	  pixmap_data[i++].bits = (char *)xnav_bitmap_attrref16_bits;
	  pixmap_data[i].width =xnav_bitmap_attrref18_width;
	  pixmap_data[i].height =xnav_bitmap_attrref18_height;
	  pixmap_data[i++].bits = (char *)xnav_bitmap_attrref18_bits;
	  pixmap_data[i].width =xnav_bitmap_attrref20_width;
	  pixmap_data[i].height =xnav_bitmap_attrref20_height;
	  pixmap_data[i++].bits = (char *)xnav_bitmap_attrref20_bits;
	  pixmap_data[i].width =xnav_bitmap_attrref20_width;
	  pixmap_data[i].height =xnav_bitmap_attrref20_height;
	  pixmap_data[i++].bits = (char *)xnav_bitmap_attrref20_bits;
	  pixmap_data[i].width =xnav_bitmap_attrref24_width;
	  pixmap_data[i].height =xnav_bitmap_attrref24_height;
	  pixmap_data[i++].bits = (char *)xnav_bitmap_attrref24_bits;

	  brow_AllocAnnotPixmap( ctx, &pixmap_data, &pixmap_attrref);

          for ( i = 0; i < 9; i++)
          {
	    pixmap_data[i].width =xnav_bitmap_attrxref12_width;
	    pixmap_data[i].height =xnav_bitmap_attrxref12_height;
	    pixmap_data[i].bits = (char *)xnav_bitmap_attrxref12_bits;
          }

	  brow_AllocAnnotPixmap( ctx, &pixmap_data, &pixmap_attrxref);

          for ( i = 0; i < 9; i++)
          {
	    pixmap_data[i].width =xnav_bitmap_ref_err12_width;
	    pixmap_data[i].height =xnav_bitmap_ref_err12_height;
	    pixmap_data[i].bits = (char *)xnav_bitmap_ref_err12_bits;
          }

	  brow_AllocAnnotPixmap( ctx, &pixmap_data, &pixmap_ref_err);

          for ( i = 0; i < 9; i++)
          {
	    pixmap_data[i].width =xnav_bitmap_crossref_err12_width;
	    pixmap_data[i].height =xnav_bitmap_crossref_err12_height;
	    pixmap_data[i].bits = (char *)xnav_bitmap_crossref_err12_bits;
          }

	  brow_AllocAnnotPixmap( ctx, &pixmap_data, &pixmap_crossref_err);

          for ( i = 0; i < 9; i++)
          {
	    pixmap_data[i].width =xnav_bitmap_attrref_err12_width;
	    pixmap_data[i].height =xnav_bitmap_attrref_err12_height;
	    pixmap_data[i].bits = (char *)xnav_bitmap_attrref_err12_bits;
          }

	  brow_AllocAnnotPixmap( ctx, &pixmap_data, &pixmap_attrref_err);

          for ( i = 0; i < 9; i++)
          {
	    pixmap_data[i].width =xnav_bitmap_attrxref_err12_width;
	    pixmap_data[i].height =xnav_bitmap_attrxref_err12_height;
	    pixmap_data[i].bits = (char *)xnav_bitmap_attrxref_err12_bits;
          }

	  brow_AllocAnnotPixmap( ctx, &pixmap_data, &pixmap_attrxref_err);

          for ( i = 0; i < 9; i++)
          {
	    pixmap_data[i].width =xnav_bitmap_docblock12_width;
	    pixmap_data[i].height =xnav_bitmap_docblock12_height;
	    pixmap_data[i].bits = (char *)xnav_bitmap_docblock12_bits;
          }

	  brow_AllocAnnotPixmap( ctx, &pixmap_data, &pixmap_docblock);

          i = 0;
	  pixmap_data[i].width =xnav_bitmap_project8_width;
	  pixmap_data[i].height =xnav_bitmap_project8_height;
	  pixmap_data[i++].bits = (char *)xnav_bitmap_project8_bits;
	  pixmap_data[i].width =xnav_bitmap_project10_width;
	  pixmap_data[i].height =xnav_bitmap_project10_height;
	  pixmap_data[i++].bits = (char *)xnav_bitmap_project10_bits;
	  pixmap_data[i].width =xnav_bitmap_project12_width;
	  pixmap_data[i].height =xnav_bitmap_project12_height;
	  pixmap_data[i++].bits = (char *)xnav_bitmap_project12_bits;
	  pixmap_data[i].width =xnav_bitmap_project14_width;
	  pixmap_data[i].height =xnav_bitmap_project14_height;
	  pixmap_data[i++].bits = (char *)xnav_bitmap_project14_bits;
	  pixmap_data[i].width =xnav_bitmap_project16_width;
	  pixmap_data[i].height =xnav_bitmap_project16_height;
	  pixmap_data[i++].bits = (char *)xnav_bitmap_project16_bits;
	  pixmap_data[i].width =xnav_bitmap_project18_width;
	  pixmap_data[i].height =xnav_bitmap_project18_height;
	  pixmap_data[i++].bits = (char *)xnav_bitmap_project18_bits;
	  pixmap_data[i].width =xnav_bitmap_project20_width;
	  pixmap_data[i].height =xnav_bitmap_project20_height;
	  pixmap_data[i++].bits = (char *)xnav_bitmap_project20_bits;
	  pixmap_data[i].width =xnav_bitmap_project20_width;
	  pixmap_data[i].height =xnav_bitmap_project20_height;
	  pixmap_data[i++].bits = (char *)xnav_bitmap_project20_bits;
	  pixmap_data[i].width =xnav_bitmap_project24_width;
	  pixmap_data[i].height =xnav_bitmap_project24_height;
	  pixmap_data[i++].bits = (char *)xnav_bitmap_project24_bits;

	  brow_AllocAnnotPixmap( ctx, &pixmap_data, &pixmap_project);

          i = 0;
	  pixmap_data[i].height =xnav_bitmap_database8_height;
	  pixmap_data[i++].bits = (char *)xnav_bitmap_database8_bits;
	  pixmap_data[i].width =xnav_bitmap_database10_width;
	  pixmap_data[i].height =xnav_bitmap_database10_height;
	  pixmap_data[i++].bits = (char *)xnav_bitmap_database10_bits;
	  pixmap_data[i].width =xnav_bitmap_database12_width;
	  pixmap_data[i].height =xnav_bitmap_database12_height;
	  pixmap_data[i++].bits = (char *)xnav_bitmap_database12_bits;
	  pixmap_data[i].width =xnav_bitmap_database14_width;
	  pixmap_data[i].height =xnav_bitmap_database14_height;
	  pixmap_data[i++].bits = (char *)xnav_bitmap_database14_bits;
	  pixmap_data[i].width =xnav_bitmap_database16_width;
	  pixmap_data[i].height =xnav_bitmap_database16_height;
	  pixmap_data[i++].bits = (char *)xnav_bitmap_database16_bits;
	  pixmap_data[i].width =xnav_bitmap_database18_width;
	  pixmap_data[i].height =xnav_bitmap_database18_height;
	  pixmap_data[i++].bits = (char *)xnav_bitmap_database18_bits;
	  pixmap_data[i].width =xnav_bitmap_database20_width;
	  pixmap_data[i].height =xnav_bitmap_database20_height;
	  pixmap_data[i++].bits = (char *)xnav_bitmap_database20_bits;
	  pixmap_data[i].width =xnav_bitmap_database20_width;
	  pixmap_data[i].height =xnav_bitmap_database20_height;
	  pixmap_data[i++].bits = (char *)xnav_bitmap_database20_bits;
	  pixmap_data[i].width =xnav_bitmap_database24_width;
	  pixmap_data[i].height =xnav_bitmap_database24_height;
	  pixmap_data[i++].bits = (char *)xnav_bitmap_database24_bits;

	  brow_AllocAnnotPixmap( ctx, &pixmap_data, &pixmap_database);

          i = 0;
	  pixmap_data[i].width =xnav_bitmap_volume8_width;
	  pixmap_data[i].height =xnav_bitmap_volume8_height;
	  pixmap_data[i++].bits = (char *)xnav_bitmap_volume8_bits;
	  pixmap_data[i].width =xnav_bitmap_volume10_width;
	  pixmap_data[i].height =xnav_bitmap_volume10_height;
	  pixmap_data[i++].bits = (char *)xnav_bitmap_volume10_bits;
	  pixmap_data[i].width =xnav_bitmap_volume12_width;
	  pixmap_data[i].height =xnav_bitmap_volume12_height;
	  pixmap_data[i++].bits = (char *)xnav_bitmap_volume12_bits;
	  pixmap_data[i].width =xnav_bitmap_volume14_width;
	  pixmap_data[i].height =xnav_bitmap_volume14_height;
	  pixmap_data[i++].bits = (char *)xnav_bitmap_volume14_bits;
	  pixmap_data[i].width =xnav_bitmap_volume16_width;
	  pixmap_data[i].height =xnav_bitmap_volume16_height;
	  pixmap_data[i++].bits = (char *)xnav_bitmap_volume16_bits;
	  pixmap_data[i].width =xnav_bitmap_volume18_width;
	  pixmap_data[i].height =xnav_bitmap_volume18_height;
	  pixmap_data[i++].bits = (char *)xnav_bitmap_volume18_bits;
	  pixmap_data[i].width =xnav_bitmap_volume20_width;
	  pixmap_data[i].height =xnav_bitmap_volume20_height;
	  pixmap_data[i++].bits = (char *)xnav_bitmap_volume20_bits;
	  pixmap_data[i].width =xnav_bitmap_volume20_width;
	  pixmap_data[i].height =xnav_bitmap_volume20_height;
	  pixmap_data[i++].bits = (char *)xnav_bitmap_volume20_bits;
	  pixmap_data[i].width =xnav_bitmap_volume24_width;
	  pixmap_data[i].height =xnav_bitmap_volume24_height;
	  pixmap_data[i++].bits = (char *)xnav_bitmap_volume24_bits;

	  brow_AllocAnnotPixmap( ctx, &pixmap_data, &pixmap_volume);

          i = 0;
	  pixmap_data[i].width =xnav_bitmap_castattr8_width;
	  pixmap_data[i].height =xnav_bitmap_castattr8_height;
	  pixmap_data[i++].bits = (char *)xnav_bitmap_castattr8_bits;
	  pixmap_data[i].width =xnav_bitmap_castattr10_width;
	  pixmap_data[i].height =xnav_bitmap_castattr10_height;
	  pixmap_data[i++].bits = (char *)xnav_bitmap_castattr10_bits;
	  pixmap_data[i].width =xnav_bitmap_castattr12_width;
	  pixmap_data[i].height =xnav_bitmap_castattr12_height;
	  pixmap_data[i++].bits = (char *)xnav_bitmap_castattr12_bits;
	  pixmap_data[i].width =xnav_bitmap_castattr14_width;
	  pixmap_data[i].height =xnav_bitmap_castattr14_height;
	  pixmap_data[i++].bits = (char *)xnav_bitmap_castattr14_bits;
	  pixmap_data[i].width =xnav_bitmap_castattr16_width;
	  pixmap_data[i].height =xnav_bitmap_castattr16_height;
	  pixmap_data[i++].bits = (char *)xnav_bitmap_castattr16_bits;
	  pixmap_data[i].width =xnav_bitmap_castattr18_width;
	  pixmap_data[i].height =xnav_bitmap_castattr18_height;
	  pixmap_data[i++].bits = (char *)xnav_bitmap_castattr18_bits;
	  pixmap_data[i].width =xnav_bitmap_castattr20_width;
	  pixmap_data[i].height =xnav_bitmap_castattr20_height;
	  pixmap_data[i++].bits = (char *)xnav_bitmap_castattr20_bits;
	  pixmap_data[i].width =xnav_bitmap_castattr20_width;
	  pixmap_data[i].height =xnav_bitmap_castattr20_height;
	  pixmap_data[i++].bits = (char *)xnav_bitmap_castattr20_bits;
	  pixmap_data[i].width =xnav_bitmap_castattr24_width;
	  pixmap_data[i].height =xnav_bitmap_castattr24_height;
	  pixmap_data[i++].bits = (char *)xnav_bitmap_castattr24_bits;

	  brow_AllocAnnotPixmap( ctx, &pixmap_data, &pixmap_castattr);

          i = 0;
	  pixmap_data[i].width =xnav_bitmap_uncastattr8_width;
	  pixmap_data[i].height =xnav_bitmap_uncastattr8_height;
	  pixmap_data[i++].bits = (char *)xnav_bitmap_uncastattr8_bits;
	  pixmap_data[i].width =xnav_bitmap_uncastattr10_width;
	  pixmap_data[i].height =xnav_bitmap_uncastattr10_height;
	  pixmap_data[i++].bits = (char *)xnav_bitmap_uncastattr10_bits;
	  pixmap_data[i].width =xnav_bitmap_uncastattr12_width;
	  pixmap_data[i].height =xnav_bitmap_uncastattr12_height;
	  pixmap_data[i++].bits = (char *)xnav_bitmap_uncastattr12_bits;
	  pixmap_data[i].width =xnav_bitmap_uncastattr14_width;
	  pixmap_data[i].height =xnav_bitmap_uncastattr14_height;
	  pixmap_data[i++].bits = (char *)xnav_bitmap_uncastattr14_bits;
	  pixmap_data[i].width =xnav_bitmap_uncastattr16_width;
	  pixmap_data[i].height =xnav_bitmap_uncastattr16_height;
	  pixmap_data[i++].bits = (char *)xnav_bitmap_uncastattr16_bits;
	  pixmap_data[i].width =xnav_bitmap_uncastattr18_width;
	  pixmap_data[i].height =xnav_bitmap_uncastattr18_height;
	  pixmap_data[i++].bits = (char *)xnav_bitmap_uncastattr18_bits;
	  pixmap_data[i].width =xnav_bitmap_uncastattr20_width;
	  pixmap_data[i].height =xnav_bitmap_uncastattr20_height;
	  pixmap_data[i++].bits = (char *)xnav_bitmap_uncastattr20_bits;
	  pixmap_data[i].width =xnav_bitmap_uncastattr20_width;
	  pixmap_data[i].height =xnav_bitmap_uncastattr20_height;
	  pixmap_data[i++].bits = (char *)xnav_bitmap_uncastattr20_bits;
	  pixmap_data[i].width =xnav_bitmap_uncastattr24_width;
	  pixmap_data[i].height =xnav_bitmap_uncastattr24_height;
	  pixmap_data[i++].bits = (char *)xnav_bitmap_uncastattr24_bits;

	  brow_AllocAnnotPixmap( ctx, &pixmap_data, &pixmap_uncastattr);

}

//
// Create nodeclasses
//
void WNavBrow::create_nodeclasses()
{
  allocate_pixmaps();

  // Create common-class

  brow_CreateNodeClass( ctx, "NavigatorDefault", 
		flow_eNodeGroup_Common, &nc_object);
  brow_AddFrame( nc_object, 0, 0, 20, 0.8, flow_eDrawType_Line, -1, 1);
  brow_AddAnnotPixmap( nc_object, 0, 0.2, 0.1, flow_eDrawType_Line, 2, 0);
  brow_AddAnnotPixmap( nc_object, 1, 1.1, 0.1, flow_eDrawType_Line, 2, 0);
  brow_AddAnnot( nc_object, 2, 0.6, 0,
		flow_eDrawType_TextHelveticaBold, 2, flow_eAnnotType_OneLine, 
		0);
  brow_AddAnnot( nc_object, 7, 0.6, 1,
		flow_eDrawType_TextHelveticaBold, 2, flow_eAnnotType_OneLine, 
		1);
  brow_AddAnnot( nc_object, 11, 0.6, 2,
		flow_eDrawType_TextHelveticaBold, 2, flow_eAnnotType_OneLine, 
		1);

  // Create multi object class

  brow_CreateNodeClass( ctx, "NavigatorObject", 
		flow_eNodeGroup_Common, &nc_multiobject);
  brow_AddFrame( nc_multiobject, 0, 0, 20, 0.8, flow_eDrawType_Line, -1, 1);
  brow_AddAnnotPixmap( nc_multiobject, 0, 0.2, 0.1, flow_eDrawType_Line, 2, 0);
  brow_AddAnnotPixmap( nc_multiobject, 1, 1.1, 0.1, flow_eDrawType_Line, 2, 0);
  brow_AddAnnot( nc_multiobject, 2, 0.6, 0,
		flow_eDrawType_TextHelveticaBold, 2, flow_eAnnotType_OneLine, 
		0);
  brow_AddAnnot( nc_multiobject, 7, 0.6, 1,
		flow_eDrawType_TextHelveticaBold, 2, flow_eAnnotType_OneLine, 
		1);
  brow_AddAnnot( nc_multiobject, 11, 0.6, 2,
		flow_eDrawType_TextHelveticaBold, 2, flow_eAnnotType_OneLine, 
		1);
  brow_AddAnnot( nc_multiobject, 15, 0.6, 3,
		flow_eDrawType_TextHelveticaBold, 2, flow_eAnnotType_OneLine, 
		1);
  brow_AddAnnotPixmap( nc_multiobject, 2, 12.2, 0.1, flow_eDrawType_Line, 2, 1);
  brow_AddAnnot( nc_multiobject, 12.7, 0.6, 4,
		flow_eDrawType_TextHelveticaBold, 2, flow_eAnnotType_OneLine, 
		1);
  brow_AddAnnotPixmap( nc_multiobject, 3, 14.2, 0.1, flow_eDrawType_Line, 2, 1);
  brow_AddAnnot( nc_multiobject, 14.7, 0.6, 5,
		flow_eDrawType_TextHelveticaBold, 2, flow_eAnnotType_OneLine, 
		1);
  brow_AddAnnotPixmap( nc_multiobject, 4, 16.2, 0.1, flow_eDrawType_Line, 2, 1);
  brow_AddAnnot( nc_multiobject, 16.7, 0.6, 6,
		flow_eDrawType_TextHelveticaBold, 2, flow_eAnnotType_OneLine, 
		1);
  brow_AddAnnotPixmap( nc_multiobject, 5, 18.2, 0.1, flow_eDrawType_Line, 2, 1);
  brow_AddAnnot( nc_multiobject, 18.7, 0.6, 7,
		flow_eDrawType_TextHelveticaBold, 2, flow_eAnnotType_OneLine, 
		1);

  // Create attribute nodeclass

  brow_CreateNodeClass( ctx, "NavigatorAttr", 
		flow_eNodeGroup_Common, &nc_attr);
  brow_AddFrame( nc_attr, 0, 0, 20, 0.8, flow_eDrawType_Line, -1, 1);
  brow_AddAnnotPixmap( nc_attr, 0, 0.2, 0.1, flow_eDrawType_Line, 2, 0);
  brow_AddAnnotPixmap( nc_attr, 1, 1.1, 0.1, flow_eDrawType_Line, 2, 0);
  brow_AddAnnot( nc_attr, 2, 0.6, 0,
		flow_eDrawType_TextHelveticaBold, 2, flow_eAnnotType_OneLine, 
		0);
  brow_AddAnnot( nc_attr, 8, 0.6, 1,
		flow_eDrawType_TextHelveticaBold, 2, flow_eAnnotType_OneLine, 
		1);

  // Create attribute nodeclass for fullname display

  brow_CreateNodeClass( ctx, "NavigatorAttrFull", 
		flow_eNodeGroup_Common, &nc_attr_full);
  brow_AddFrame( nc_attr_full, 0, 0, 60, 0.8, flow_eDrawType_Line, -1, 1);
  brow_AddAnnotPixmap( nc_attr_full, 0, 0.2, 0.1, flow_eDrawType_Line, 2, 0);
  brow_AddAnnotPixmap( nc_attr_full, 1, 1.1, 0.1, flow_eDrawType_Line, 2, 0);
  brow_AddAnnot( nc_attr_full, 2, 0.6, 0,
		flow_eDrawType_TextHelveticaBold, 2, flow_eAnnotType_OneLine, 
		0);
  brow_AddAnnot( nc_attr_full, 25, 0.6, 1,
		flow_eDrawType_TextHelveticaBold, 2, flow_eAnnotType_OneLine, 
		1);

  // Input with Used and Inverted radiobutton
  brow_CreateNodeClass( ctx, "NavigatorAttrInput", 
		flow_eNodeGroup_Common, &nc_attr_input);
  brow_AddRadiobutton( nc_attr_input, 14, 0.1, 0.7, 0.7, 0, flow_eDrawType_Line, 1);
  brow_AddRadiobutton( nc_attr_input, 18, 0.1, 0.7, 0.7, 1, flow_eDrawType_Line, 1);
  brow_AddFrame( nc_attr_input, 0, 0, 20, 0.8, flow_eDrawType_Line, -1, 1);
  brow_AddAnnotPixmap( nc_attr_input, 0, 0.2, 0.1, flow_eDrawType_Line, 2, 0);
  brow_AddAnnotPixmap( nc_attr_input, 1, 1.1, 0.1, flow_eDrawType_Line, 2, 0);
  brow_AddAnnot( nc_attr_input, 2, 0.6, 0,
		flow_eDrawType_TextHelveticaBold, 2, flow_eAnnotType_OneLine, 
		0);
  brow_AddAnnot( nc_attr_input, 8, 0.6, 1,
		flow_eDrawType_TextHelveticaBold, 2, flow_eAnnotType_OneLine, 
		1);
  brow_AddText( nc_attr_input, "Used", 12, 0.6, 
	flow_eDrawType_TextHelveticaBold, 2);
  brow_AddText( nc_attr_input, "Inverted", 15, 0.6,
	flow_eDrawType_TextHelveticaBold, 2);

  // Input with Inverted radiobutton
  brow_CreateNodeClass( ctx, "NavigatorAttrInputNoinv",
		flow_eNodeGroup_Common, &nc_attr_inputinv);
  brow_AddRadiobutton( nc_attr_inputinv, 15, 0.1, 0.7, 0.7, 0, flow_eDrawType_Line, 1);
  brow_AddFrame( nc_attr_inputinv, 0, 0, 20, 0.8, flow_eDrawType_Line, -1, 1);
  brow_AddAnnotPixmap( nc_attr_inputinv, 0, 0.2, 0.1, flow_eDrawType_Line, 2, 0);
  brow_AddAnnotPixmap( nc_attr_inputinv, 1, 1.1, 0.1, flow_eDrawType_Line, 2, 0);
  brow_AddAnnot( nc_attr_inputinv, 2, 0.6, 0,
		flow_eDrawType_TextHelveticaBold, 2, flow_eAnnotType_OneLine, 
		0);
  brow_AddAnnot( nc_attr_inputinv, 8, 0.6, 1,
		flow_eDrawType_TextHelveticaBold, 2, flow_eAnnotType_OneLine, 
		1);
  brow_AddText( nc_attr_inputinv, "Inverted", 12, 0.6, 
	flow_eDrawType_TextHelveticaBold, 2);

  // Input and output with Used radiobutton
  brow_CreateNodeClass( ctx, "NavigatorAttrOutput", 
		flow_eNodeGroup_Common, &nc_attr_output);
  brow_AddRadiobutton( nc_attr_output, 14, 0.1, 0.7, 0.7, 0, flow_eDrawType_Line, 1);
  brow_AddFrame( nc_attr_output, 0, 0, 20, 0.8, flow_eDrawType_Line, -1, 1);
  brow_AddAnnotPixmap( nc_attr_output, 0, 0.2, 0.1, flow_eDrawType_Line, 2, 0);
  brow_AddAnnotPixmap( nc_attr_output, 1, 1.1, 0.1, flow_eDrawType_Line, 2, 0);
  brow_AddAnnot( nc_attr_output, 2, 0.6, 0,
		flow_eDrawType_TextHelveticaBold, 2, flow_eAnnotType_OneLine, 
		0);
  brow_AddAnnot( nc_attr_output, 8, 0.6, 1,
		flow_eDrawType_TextHelveticaBold, 2, flow_eAnnotType_OneLine, 
		1);
  brow_AddText( nc_attr_output, "Used", 12, 0.6, 
	flow_eDrawType_TextHelveticaBold, 2);

  // Create multiline attribute nodeclass

  brow_CreateNodeClass( ctx, "NavigatorAttrMultiLine", 
		flow_eNodeGroup_Common, &nc_attr_multiline);
  brow_AddFrame( nc_attr_multiline, 0, 0, 20, 0.8, flow_eDrawType_Line, -1, 1);
  brow_AddAnnotPixmap( nc_attr_multiline, 0, 0.2, 0.1, flow_eDrawType_Line, 2, 0);
  brow_AddAnnotPixmap( nc_attr_multiline, 1, 1.1, 0.1, flow_eDrawType_Line, 2, 0);
  brow_AddAnnot( nc_attr_multiline, 2, 0.6, 0,
		flow_eDrawType_TextHelveticaBold, 2, flow_eAnnotType_OneLine, 
		0);
  brow_AddAnnot( nc_attr_multiline, 8, 0.6, 1,
		flow_eDrawType_TextHelveticaBold, 2, flow_eAnnotType_MultiLine, 
		1);

  // Create multiline attribute nodeclass for fullname

  brow_CreateNodeClass( ctx, "NavigatorAttrMultiLineFull", 
		       flow_eNodeGroup_Common, &nc_attr_multiline_full);
  brow_AddFrame( nc_attr_multiline_full, 0, 0, 60, 0.8, 
		 flow_eDrawType_Line, -1, 1);
  brow_AddAnnotPixmap( nc_attr_multiline_full, 0, 0.2, 0.1, 
		       flow_eDrawType_Line, 2, 0);
  brow_AddAnnotPixmap( nc_attr_multiline_full, 1, 1.1, 0.1, 
		       flow_eDrawType_Line, 2, 0);
  brow_AddAnnot( nc_attr_multiline_full, 2, 0.6, 0,
		flow_eDrawType_TextHelveticaBold, 2, flow_eAnnotType_OneLine, 
		0);
  brow_AddAnnot( nc_attr_multiline_full, 25, 0.6, 1,
		flow_eDrawType_TextHelveticaBold, 2,
		flow_eAnnotType_MultiLine, 1);

  // Create attribute nodeclass

  brow_CreateNodeClass( ctx, "NavigatorEnum", 
		flow_eNodeGroup_Common, &nc_enum);
  brow_AddRadiobutton( nc_enum, 12, 0.1, 0.7, 0.7, 0, flow_eDrawType_Line, 1);
  brow_AddFrame( nc_enum, 0, 0, 20, 0.8, flow_eDrawType_Line, -1, 1);
  brow_AddAnnotPixmap( nc_enum, 0, 0.2, 0.1, flow_eDrawType_Line, 2, 0);
  brow_AddAnnotPixmap( nc_enum, 1, 1.1, 0.1, flow_eDrawType_Line, 2, 0);
  brow_AddAnnot( nc_enum, 2, 0.6, 0,
		flow_eDrawType_TextHelveticaBold, 2, flow_eAnnotType_OneLine, 
		0);
 
  // Create table nodeclass

  brow_CreateNodeClass( ctx, "NavigatorTable", 
		flow_eNodeGroup_Common, &nc_table);
  brow_AddFrame( nc_table, 0, 0, 20, 0.8, flow_eDrawType_Line, -1, 1);
  brow_AddAnnotPixmap( nc_table, 0, 0.2, 0.1, flow_eDrawType_Line, 2, 0);
  brow_AddAnnotPixmap( nc_table, 1, 1.1, 0.1, flow_eDrawType_Line, 2, 0);
  brow_AddAnnot( nc_table, 2, 0.6, 0,
		flow_eDrawType_TextHelveticaBold, 2, flow_eAnnotType_OneLine, 
		0);
  brow_AddAnnot( nc_table, 8, 0.6, 1,
		flow_eDrawType_TextHelveticaBold, 2, flow_eAnnotType_OneLine, 
		1);
  brow_AddAnnot( nc_table, 12, 0.6, 2,
		flow_eDrawType_TextHelveticaBold, 2, flow_eAnnotType_OneLine, 
		1);
  brow_AddAnnot( nc_table, 16, 0.6, 3,
		flow_eDrawType_TextHelveticaBold, 2, flow_eAnnotType_OneLine, 
		1);
  brow_AddAnnot( nc_table, 20, 0.6, 4,
		flow_eDrawType_TextHelveticaBold, 2, flow_eAnnotType_OneLine, 
		1);
  brow_AddAnnot( nc_table, 24, 0.6, 5,
		flow_eDrawType_TextHelveticaBold, 2, flow_eAnnotType_OneLine, 
		1);
  brow_AddAnnot( nc_table, 28, 0.6, 6,
		flow_eDrawType_TextHelveticaBold, 2, flow_eAnnotType_OneLine, 
		1);
  brow_AddAnnot( nc_table, 32, 0.6, 7,
		flow_eDrawType_TextHelveticaBold, 2, flow_eAnnotType_OneLine, 
		1);
  brow_AddAnnot( nc_table, 35, 0.6, 8,
		flow_eDrawType_TextHelveticaBold, 2, flow_eAnnotType_OneLine, 
		1);
  brow_AddAnnot( nc_table, 38, 0.6, 9,
		flow_eDrawType_TextHelveticaBold, 2, flow_eAnnotType_OneLine, 
		1);

  // Create text

  brow_CreateNodeClass( ctx, "NavigatorText", 
		flow_eNodeGroup_Common, &nc_text);
  brow_AddFrame( nc_text, 0, 0, 20, 0.8, flow_eDrawType_Line, -1, 1);
  brow_AddAnnotPixmap( nc_text, 0, 0.2, 0.1, flow_eDrawType_Line, 2, 0);
  brow_AddAnnotPixmap( nc_text, 1, 1.1, 0.1, flow_eDrawType_Line, 2, 0);
  brow_AddAnnot( nc_text, 2, 0.6, 0,
		flow_eDrawType_TextHelvetica, 2, flow_eAnnotType_OneLine, 
		0);
  brow_AddAnnot( nc_text, 7, 0.6, 1,
		flow_eDrawType_TextHelvetica, 2, flow_eAnnotType_OneLine, 
		1);
  brow_AddAnnot( nc_text, 11, 0.6, 2,
		flow_eDrawType_TextHelvetica, 2, flow_eAnnotType_OneLine, 
		1);

  // Create Header

  brow_CreateNodeClass( ctx, "NavigatorHead", 
		flow_eNodeGroup_Common, &nc_header);
  brow_AddFrame( nc_header, 0, 0, 20, 0.8, flow_eDrawType_LineGray, 2, 1);
  brow_AddAnnotPixmap( nc_header, 0, 0.2, 0.1, flow_eDrawType_Line, 2, 0);
  brow_AddAnnot( nc_header, 2, 0.6, 0,
		flow_eDrawType_TextHelveticaBold, 2, flow_eAnnotType_OneLine, 
		0);
  brow_AddAnnot( nc_header, 8, 0.6, 1,
		flow_eDrawType_TextHelveticaBold, 2, flow_eAnnotType_OneLine, 
		1);

  // Create Header2

  brow_CreateNodeClass( ctx, "NavigatorHeadLarge", 
		flow_eNodeGroup_Common, &nc_headerlarge);
  brow_AddFrame( nc_headerlarge, 0, 0, 20, 1.4, flow_eDrawType_Line, -1, 1);
  brow_AddAnnotPixmap( nc_headerlarge, 0, 0.5, 0.1, flow_eDrawType_Line, 2, 0);
  brow_AddAnnot( nc_headerlarge, 2, 1.0, 0,
		flow_eDrawType_TextHelveticaBold, 4, flow_eAnnotType_OneLine, 
		0);
  brow_AddAnnot( nc_headerlarge, 8, 1.0, 1,
		flow_eDrawType_TextHelveticaBold, 4, flow_eAnnotType_OneLine, 
		1);

  // Create TableHeader

  brow_CreateNodeClass( ctx, "NavigatorTableHead", 
		flow_eNodeGroup_Common, &nc_table_header);
  brow_AddFrame( nc_table_header, 0, 0, 20, 0.8, flow_eDrawType_LineGray, 2, 1);
  brow_AddAnnotPixmap( nc_table_header, 0, 0.2, 0.1, flow_eDrawType_Line, 2, 0);
  brow_AddAnnot( nc_table_header, 2, 0.6, 0,
		flow_eDrawType_TextHelveticaBold, 2, flow_eAnnotType_OneLine, 
		0);
  brow_AddAnnot( nc_table_header, 8, 0.6, 1,
		flow_eDrawType_TextHelveticaBold, 2, flow_eAnnotType_OneLine, 
		0);
  brow_AddAnnot( nc_table_header, 12, 0.6, 2,
		flow_eDrawType_TextHelveticaBold, 2, flow_eAnnotType_OneLine, 
		0);
  brow_AddAnnot( nc_table_header, 16, 0.6, 3,
		flow_eDrawType_TextHelveticaBold, 2, flow_eAnnotType_OneLine, 
		0);
  brow_AddAnnot( nc_table_header, 20, 0.6, 4,
		flow_eDrawType_TextHelveticaBold, 2, flow_eAnnotType_OneLine, 
		0);
  brow_AddAnnot( nc_table_header, 24, 0.6, 5,
		flow_eDrawType_TextHelveticaBold, 2, flow_eAnnotType_OneLine, 
		0);
  brow_AddAnnot( nc_table_header, 28, 0.6, 6,
		flow_eDrawType_TextHelveticaBold, 2, flow_eAnnotType_OneLine, 
		0);
  brow_AddAnnot( nc_table_header, 32, 0.6, 7,
		flow_eDrawType_TextHelveticaBold, 2, flow_eAnnotType_OneLine, 
		0);
  brow_AddAnnot( nc_table_header, 35, 0.6, 8,
		flow_eDrawType_TextHelveticaBold, 2, flow_eAnnotType_OneLine, 
		0);
  brow_AddAnnot( nc_table_header, 38, 0.6, 9,
		flow_eDrawType_TextHelveticaBold, 2, flow_eAnnotType_OneLine, 
		0);
}

