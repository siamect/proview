#ifndef ge_curve_h
#define ge_curve_h

/* ge_curve.h -- Curve widget

   PROVIEW/R
   Copyright (C) 1996 by Comator Process AB.

   <Description>.  */

#if defined __cplusplus
extern "C" {
#endif

#ifndef glow_growapi_h
#include "glow_growapi.h"
#endif


#define CURVE_MAX_COLS 101

typedef enum {
  curve_eDataType_LogFile,
  curve_eDataType_DsTrend,
  curve_eDataType_ODBC
  } curve_eDataType;


typedef enum {
  curve_eAxis_x,
  curve_eAxis_y
  } curve_eAxis;

class GeCurveData {
  public:
    GeCurveData( curve_eDataType datatype);
    curve_eDataType type;
    int     rows;
    int     cols;
    char    name[CURVE_MAX_COLS][120];
    double  *data[CURVE_MAX_COLS];
    double  max_value[CURVE_MAX_COLS];
    double  min_value[CURVE_MAX_COLS];
    double  max_value_axis[CURVE_MAX_COLS];
    double  min_value_axis[CURVE_MAX_COLS];
    int     lines[CURVE_MAX_COLS];
    glow_eDrawType color[CURVE_MAX_COLS];
    glow_eDrawType fillcolor[CURVE_MAX_COLS];
    pwr_eType value_type[CURVE_MAX_COLS];
    curve_eAxis axis_type[CURVE_MAX_COLS];
    double  axis_width[CURVE_MAX_COLS];
    char    format[CURVE_MAX_COLS][20];
    int     x_reverse;
    void get_borders();
    void get_default_axis();
    void select_color();
    void scale( int axis_type, int value_type, 
                double min_value, double max_value, 
                double *min_value_axis, double *max_value_axis, 
                int *lines, char *format,
		double *axis_width, int not_zero, int allow_odd);
    ~GeCurveData();
};

class GeCurve {
  public:
    GeCurve( void *gc_parent_ctx, Widget parent_widget, char *curve_name,
         char *filename, GeCurveData *curve_data, int pos_right);
    void 	*parent_ctx;
    Widget	parent_wid;
    char 	name[80];
    Widget	grow_widget;
    Widget	curve_widget;
    Widget	axisform_widget;
    Widget	curveform_widget;
    Widget	pane_widget;
    Widget	toplevel;
    Widget      growcurve_main_widget;
    Widget      growaxis_main_widget;
    Widget      grownames_main_widget;
    Widget      nav_widget;
    Widget      minmax_widget;
    Widget      minmax_textmin_widget;
    Widget      minmax_textmax_widget;
    CurveCtx    *growcurve_ctx;
    GrowCtx     *growaxis_ctx;
    GrowCtx     *grownames_ctx;
    glow_eDrawType curve_border;
    glow_eDrawType curve_color;
    glow_eDrawType background_dark;
    glow_eDrawType background_bright;
    glow_eDrawType border_dark;
    glow_eDrawType border_bright;
    grow_tObject curve_object;
    grow_tObject curve_axisobject;
    GeCurveData *cd;
    double       axis_window_width;
    int          hide[CURVE_MAX_COLS];
    grow_tObject name_rect[CURVE_MAX_COLS];
    grow_tObject hide_rect[CURVE_MAX_COLS];
    grow_tObject axis_rect[CURVE_MAX_COLS];
    grow_tObject axis_object[CURVE_MAX_COLS];
    grow_tObject axis_lineobject;
    int          auto_refresh;
    int          axis_displayed;
    int          minmax_idx;
    void 	 (*close_cb)( void *);
    int          initial_right_position;

    int read_file( char *filename);
    int configure_curves();
    int configure_axes();
    void points_added();
    void pop();
    ~GeCurve();
};

#if defined __cplusplus
}
#endif
#endif
















