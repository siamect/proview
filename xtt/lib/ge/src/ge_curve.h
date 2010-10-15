/* 
 * Proview   $Id: ge_curve.h,v 1.13 2008-10-03 14:26:16 claes Exp $
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

#ifndef ge_curve_h
#define ge_curve_h

/* ge_curve.h -- Curve widget */

#ifndef glow_growapi_h
#include "glow_growapi.h"
#endif

/*! \file ge_curve.h
    \brief Contains the GeCurve class. */
/*! \addtogroup Ge */
/*@{*/

#define CURVE_MAX_COLS 101

typedef enum {
  curve_eDataType_LogFile,
  curve_eDataType_DsTrend,
  curve_eDataType_MultiTrend,
  curve_eDataType_ODBC
  } curve_eDataType;


typedef enum {
  curve_eAxis_x,
  curve_eAxis_y
  } curve_eAxis;

typedef enum {
  curve_eTimeFormat_Float,
  curve_eTimeFormat_HourMinute,
  curve_eTimeFormat_DayHour
} curve_eTimeFormat;

//! Contains data for the curves.
class GeCurveData {
  public:
    GeCurveData( curve_eDataType datatype);
    curve_eDataType type;
    int     rows[CURVE_MAX_COLS];
    int     cols;
    pwr_tAName y_name[CURVE_MAX_COLS];
    pwr_tAName x_name;
    char    y_unit[CURVE_MAX_COLS][16];
    char    x_unit[CURVE_MAX_COLS][16];
    double  *y_data[CURVE_MAX_COLS];
    double  *x_data[CURVE_MAX_COLS];
    double  y_max_value[CURVE_MAX_COLS];
    double  y_min_value[CURVE_MAX_COLS];
    double  x_max_value[CURVE_MAX_COLS];
    double  x_min_value[CURVE_MAX_COLS];
    double  y_max_value_axis[CURVE_MAX_COLS];
    double  y_min_value_axis[CURVE_MAX_COLS];
    int     y_trend_lines[CURVE_MAX_COLS];
    int     y_axis_lines[CURVE_MAX_COLS];
    int     y_axis_linelongq[CURVE_MAX_COLS];
    int     y_axis_valueq[CURVE_MAX_COLS];
    double  x_max_value_axis[CURVE_MAX_COLS];
    double  x_min_value_axis[CURVE_MAX_COLS];
    int     x_trend_lines[CURVE_MAX_COLS];
    int     x_axis_lines[CURVE_MAX_COLS];
    int     x_axis_linelongq[CURVE_MAX_COLS];
    int     x_axis_valueq[CURVE_MAX_COLS];
    glow_eDrawType color[CURVE_MAX_COLS];
    glow_eDrawType fillcolor[CURVE_MAX_COLS];
    glow_eDrawType axiscolor[CURVE_MAX_COLS];
    pwr_eType y_value_type[CURVE_MAX_COLS];
    pwr_eType x_value_type[CURVE_MAX_COLS];
    curve_eAxis y_axis_type[CURVE_MAX_COLS];
    curve_eAxis x_axis_type[CURVE_MAX_COLS];
    double  y_axis_width[CURVE_MAX_COLS];
    char    y_format[CURVE_MAX_COLS][20];
    char    x_format[CURVE_MAX_COLS][20];
    int     x_reverse;
    curve_eTimeFormat time_format;

    void get_borders();
    void get_default_axis();
    void select_color( bool dark_bg);
    void scale( int axis_type, int value_type, 
                double min_value, double max_value, 
                double *min_value_axis, double *max_value_axis, 
                int *trend_lines, int *axis_lines, int *axis_linelongq, 
		int *axis_valueq,char *format,
		double *axis_width, int not_zero, int allow_odd);
    ~GeCurveData();
};

class CoWow;

//! A curve window used for trends and logging curves.
class GeCurve {
  public:
    void 	*parent_ctx;
    char 	name[80];
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
    grow_tObject curve_markobject;
    GeCurveData *cd;
    double       axis_window_width;
    int          hide[CURVE_MAX_COLS];
    grow_tObject name_rect[CURVE_MAX_COLS+1];
    grow_tObject hide_rect[CURVE_MAX_COLS+1];
    grow_tObject hide_l1[CURVE_MAX_COLS+1];
    grow_tObject hide_l2[CURVE_MAX_COLS+1];
    grow_tObject scale_rect[CURVE_MAX_COLS+1];
    grow_tObject mark_annot[CURVE_MAX_COLS+1];
    grow_tObject cursor_annot[CURVE_MAX_COLS+1];
    grow_tObject axis_rect[CURVE_MAX_COLS+1];
    grow_tObject axis_object[CURVE_MAX_COLS+1];
    grow_tObject axis_lineobject;
    int          auto_refresh;
    int          axis_displayed;
    int          minmax_idx;
    void 	 (*close_cb)( void *);
    void 	 (*help_cb)( void *);
    void 	 (*higher_res_cb)( void *);
    void 	 (*lower_res_cb)( void *);
    int          initial_right_position;
    char	 title[300];
    double  	 last_cursor_x;
    double  	 last_mark_x;
    int		 deferred_configure_axes;
    CoWow	 *wow;

    GeCurve( void *gc_parent_ctx, char *curve_name,
         char *filename, GeCurveData *curve_data, int pos_right);
    virtual ~GeCurve();
    virtual void write_title( char *str) {}
    virtual void pop() {}
    virtual void resize() {}
    virtual void open_minmax( int idx) {}
    virtual void axis_set_width( int width) {}  
    virtual void enable_resolution_buttons() {}
    void set_inputfocus() {}

    int read_file( char *filename);
    int configure_curves();
    int configure_axes();
    void points_added();
    void set_title( char *str);
    void set_time( pwr_tTime time);
    void print( char *filename);
    void scroll( double value);
    void measure_window( double *ll_x, double *ll_y, double *ur_x, double *ur_y);
    void activate_exit();
    void activate_configure();
    void activate_print();
    void activate_background();
    void activate_filledcurves( int set);
    void activate_help();
    void activate_minmax_ok( double min_value, double max_value);
    void set_curvedata( GeCurveData *curve_data);
    void redraw();
    void x_to_points( double x, double *time, double *values);
    
    static int growcurve_cb( GlowCtx *ctx, glow_tEvent event);
    static int init_growcurve_cb( GlowCtx *fctx, void *client_data);
    static int growaxis_cb( GlowCtx *ctx, glow_tEvent event);
    static int grownames_cb( GlowCtx *ctx, glow_tEvent event);
    static int init_growaxis_cb( GlowCtx *fctx, void *client_data);
    static int init_grownames_cb( GlowCtx *fctx, void *client_data);
};

/*@}*/
#endif
















