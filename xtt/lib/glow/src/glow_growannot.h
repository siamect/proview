#ifndef glow_growannot_h
#define glow_growannot_h

#include "glow_annot.h"
#include "glow_text.h"
#include "glow_rect.h"

class GrowAnnot : public GlowAnnot {
  public:
    GrowAnnot( GlowCtx *glow_ctx, double x = 0, double y = 0,
	int annot_num = 0, glow_eDrawType d_type = glow_eDrawType_TextHelveticaBold,
	int t_size = 2, glow_eAnnotType a_type = glow_eAnnotType_OneLine,
	int rel_pos = 0, glow_mDisplayLevel display_lev = glow_mDisplayLevel_1,
	int nodraw =0):
	GlowAnnot(glow_ctx,x,y,annot_num,d_type,t_size,a_type,
	rel_pos,display_lev), temporary_scale(0) {};
    void save( ofstream& fp, glow_eSaveMode mode);
    void open( ifstream& fp);
    glow_eObjectType type() { return glow_eObjectType_GrowAnnot;};
    GlowTransform trf;
    int		temporary_scale;
    void draw( GlowTransform *t, int highlight, int hot, void *node, void *colornode);
    void erase( GlowTransform *t, int hot, void *node);
    void erase_background( GlowTransform *t, int hot, void *node);
    void nav_draw( GlowTransform *t, int highlight, void *node, void *colornode);
    void nav_erase( GlowTransform *t, void *node);
    void get_borders( GlowTransform *t, double *x_right, double *x_left, 
	double *y_high, double *y_low);
    void export_javabean( GlowTransform *t, void *node,
	glow_eExportPass pass, int *shape_cnt, int node_cnt, int in_nc, ofstream &fp);
    void export_javabean_font( GlowTransform *t, void *node,
	glow_eExportPass pass, ofstream &fp);
};

#endif
