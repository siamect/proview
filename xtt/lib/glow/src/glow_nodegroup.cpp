/* 
 * Proview   $Id: glow_nodegroup.cpp,v 1.2 2005-09-01 14:57:54 claes Exp $
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
#include <fstream.h>
#include <stdlib.h>
#include <string.h>
#include "glow_nodegroup.h"
#include "glow_growctx.h"
#include "glow_grownode.h"
#include "glow_msg.h"

GlowNodeGroup::GlowNodeGroup( GlowCtx *glow_ctx, char *name, 
			      GlowArray& array)
  : GlowNodeClass(glow_ctx, name, glow_eNodeGroup_Common)
{
  a.copy_from_common_objects( array);
}

GlowNodeGroup::GlowNodeGroup( GlowCtx *glow_ctx, char *name)
  : GlowNodeClass(glow_ctx, name, glow_eNodeGroup_Common)
{
}

GlowNodeGroup::GlowNodeGroup( const GlowNodeGroup& nc)
  : GlowNodeClass( (GlowNodeClass &)nc)
{
  // Get unic name
  sprintf( nc_name, "Grp%d_", ((GrowCtx *)ctx)->objectname_cnt++); 
}

GlowNodeGroup::~GlowNodeGroup()
{
}

void GlowNodeGroup::ungroup( GlowTransform *t)
{
  a.set_transform( t);

  for ( int i = 0; i < a.size(); i++)
  {
    ctx->insert( a[i]);
    if ( a[i]->type() == glow_eObjectType_GrowNode ||
         a[i]->type() == glow_eObjectType_GrowGroup)
      ((GrowNode *)a[i])->ungroup();
  }
  a.clear();
}

void GlowNodeGroup::save( ofstream& fp, glow_eSaveMode mode)
{
  fp <<	int(glow_eSave_NodeGroup) << endl;
  fp << int(glow_eSave_NodeGroup_nodeclass_part) << endl;
  saved = 0;
  GlowNodeClass::save( fp, mode);
  fp << int(glow_eSave_End) << endl;
}

void GlowNodeGroup::open( ifstream& fp)
{
  int		type;
  int 		end_found = 0;
  char		dummy[40];

  for (;;)
  {
    fp >> type;
    switch( type) {
      case glow_eSave_NodeGroup: break;
      case glow_eSave_NodeGroup_nodeclass_part: 
        GlowNodeClass::open( fp);
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

