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

