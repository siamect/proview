/* 
 * Proview   $Id: xtt_trend.h,v 1.4 2005-11-17 09:03:20 claes Exp $
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
 */

#ifndef xtt_trend_h
#define xtt_trend_h

/* xtt_trend.h -- DsTrend curves */

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
    void       (*help_cb)( void *, char *);
    void pop();
    ~XttTrend();
};
#if defined __cplusplus
}
#endif
#endif












