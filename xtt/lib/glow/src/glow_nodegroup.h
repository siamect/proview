#ifndef glow_nodegroup_h
#define glow_nodegroup_h

#include "glow_nodeclass.h"

class GlowNodeGroup : public GlowNodeClass {
  public:
    GlowNodeGroup( GlowCtx *glow_ctx, char *name, GlowArray& array);
    GlowNodeGroup( GlowCtx *glow_ctx, char *name);
    GlowNodeGroup( const GlowNodeGroup& nc);
    ~GlowNodeGroup();
    glow_eObjectType type() { return glow_eObjectType_NodeGroup;};
    void save( ofstream& fp, glow_eSaveMode mode);
    void open( ifstream& fp);
    void ungroup( GlowTransform *t);
};


#endif
