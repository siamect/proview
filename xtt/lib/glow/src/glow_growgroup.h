#ifndef glow_growgroup_h
#define glow_growgroup_h

#include "glow_grownode.h"

class GrowGroup : public GrowNode {
  public:
    GrowGroup( GlowCtx *glow_ctx, char *name, GlowArray& array, 
	       int nodraw = 0);
    GrowGroup() {};
    GrowGroup( GlowCtx *glow_ctx, char *name);
    ~GrowGroup();
    void copy_from( const GrowGroup& n); 
    void save( ofstream& fp, glow_eSaveMode mode);
    void open( ifstream& fp);
    void ungroup();
    void trace_scan();
    int trace_init();
    void trace_close();
    void get_nodegroups( void *a);
    int get_object_group( GlowArrayElem *object, GlowArrayElem **group);
    void get_objectlist( GlowArrayElem ***list, int *size)
		{ *list = nc->a.a; *size = nc->a.size();};
    int get_background_object_limits(GlowTransform *t,
		    glow_eTraceType type,
		    double x, double y, GlowArrayElem **background,
		    double *min, double *max, glow_eDirection *direction);
};


#endif
