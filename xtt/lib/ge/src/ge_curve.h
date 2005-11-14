/* 
 * Proview   $Id: ge_curve.h,v 1.6 2005-11-14 16:18:32 claes Exp $
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

#if defined __cplusplus
extern "C" {
#endif

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
  curve_eDataType_ODBC
  } curve_eDataType;


typedef enum {
  curve_eAxis_x,
  curve_eAxis_y
  } curve_eAxis;

//! Contains data for the curves.
class GeCurveData {
  public:
    GeCurveData( curve_eDataType datatype);
    curve_eDataType type;
    int     rows;
    int     cols;
    pwr_tAName name[CURVE_MAX_COLS];
    double  *data[CURVE_MAX_COLS];
    double  max_value[CURVE_MAX_COLS];
    double  min_value[CURVE_MAX_COLS];
    double  max_value_axis[CURVE_MAX_COLS];
    double  min_value_axis[CURVE_MAX_COLS];
    int     trend_lines[CURVE_MAX_COLS];
    int     axis_lines[CURVE_MAX_COLS];
    int     axis_linelongq[CURVE_MAX_COLS];
    int     axis_valueq[CURVE_MAX_COLS];
    glow_eDrawType color[CURVE_MAX_COLS];
    glow_eDrawType fillcolor[CURVE_MAX_COLS];
    glow_eDrawType axiscolor[CURVE_MAX_COLS];
    pwr_eType value_type[CURVE_MAX_COLS];
    curve_eAxis axis_type[CURVE_MAX_COLS];
    double  axis_width[CURVE_MAX_COLS];
    char    format[CURVE_MAX_COLS][20];
    int     x_reverse;
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

//! A curve window used for trends and logging curves.
class GeCurve {
  private:
    void	write_title( char *str);

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
    char	 title[300];

    int read_file( char *filename);
    int configure_curves();
    int configure_axes();
    void points_added();
    void pop();
    void set_title( char *str);
    void set_time( pwr_tTime time);
    void print( char *filename);
    ~GeCurve();
};

/*@}*/
#if defined __cplusplus
}
#endif
#endif
















