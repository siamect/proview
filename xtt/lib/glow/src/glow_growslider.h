#ifndef glow_growslider_h
#define glow_growslider_h

#include "glow_grownode.h"

class GrowSlider : public GrowNode {
  public:
    GrowSlider( GlowCtx *glow_ctx, char *name, GlowNodeClass *node_class,
	double x1, double y1, int nodraw = 0, int rel_annot_pos = 0);
    GrowSlider() {};
    void copy_from( const GrowSlider& n);
    glow_eObjectType type() { return glow_eObjectType_GrowSlider;};
    void save( ofstream& fp, glow_eSaveMode mode);
    void open( ifstream& fp);
    void set_range( double min_val, double max_val);
    void get_info( glow_eDirection *dir, double *max_val, double *min_val,
		double *max_position, double *min_position);
    void get_origo( glow_eDirection direction, double *pos);
    void set_info( glow_eDirection dir, double max_val, double min_val,
		double max_position, double min_position);
    void export_javabean( GlowTransform *t, void *node,
	glow_eExportPass pass, int *shape_cnt, int node_cnt, int in_nc, ofstream &fp);

    glow_eDirection direction;
    double max_value;
    double min_value;
    double max_pos;
    double min_pos;
};


#endif
