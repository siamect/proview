#ifndef xtt_trend_h
#define xtt_trend_h

/* xtt_trend.h -- DsTrend curves

   PROVIEW/R
   Copyright (C) 1996 by Comator Process AB.

   <Description>.  */

#if defined __cplusplus
extern "C" {
#endif

#ifndef pwr_h
# include "pwr.h"
#endif

#ifndef ge_curve_h
# include "ge_curve.h"
#endif

#define XTT_TREND_MAX 20

class XttTrend {
  public:
    XttTrend(
	void *xn_parent_ctx,
	Widget	xn_parent_wid,
	char *xn_name,
	Widget *w,
        pwr_sAttrRef *objid,
        pwr_sAttrRef *plotgroup,
        int *sts);
    void       *xnav;
    Widget     parent_widget;
    int        trend_cnt;
    GeCurveData *gcd;
    GeCurve    *curve;
    pwr_tRefId subid[XTT_TREND_MAX];
    pwr_sClass_DsTrend *trend_p[XTT_TREND_MAX];
    int        element_size[XTT_TREND_MAX];
    int        interval[XTT_TREND_MAX];
    int        max_time;
    int        min_interval;
    int        min_interval_idx;
    int        max_points;
    XtIntervalId  timerid;
    int        last_buffer[XTT_TREND_MAX];
    int        last_next_index[XTT_TREND_MAX];
    void       (*close_cb)( void *, XttTrend *);
    void pop();
    ~XttTrend();
};
#if defined __cplusplus
}
#endif
#endif












