#ifndef glow_growcurve_h
#define glow_growcurve_h

#include "glow_growtrend.h"

class GrowCurve : public GrowTrend {
  public:
    GrowCurve( GlowCtx *glow_ctx, char *name, glow_sCurveData *data,
                double x = 0, double y = 0, 
		double w = 0, double h = 0, 
		glow_eDrawType border_d_type = glow_eDrawType_Line, 
		int line_w = 1, 
		glow_mDisplayLevel display_lev = glow_mDisplayLevel_1,
		int fill_rect = 0, int display_border = 1, 
	        glow_eDrawType fill_d_type = glow_eDrawType_Line, 
	        int nodraw = 0);
    void configure_curves( glow_sCurveData *data);
    void add_points( glow_sCurveData *data);
};


#endif

