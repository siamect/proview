#ifndef wb_goeconst_h
#define wb_goeconst_h

/* wb_goeconst.h -- <short description>

   PROVIEW/R
   Copyright (C) 1996 by Comator Process AB.

   This module contains constants and constant global variables for
   the goe widget.  */

/* If TEST is set the test main program is compiled.  */

#define TEST 0

/* These include files are needed to spawn the eve editor process.  */

#include <clidef.h>
#include <lib$routines.h>
#include <descrip.h>


/* Include file specifying the DW font names.  */

/* Basic include files.  */

#include <stdio.h>
#include <math.h>

/* Flags indicating if ggc's are created for gobe or neted.  */

#define	GOE_K_GOE	0
#define	GOE_K_NETED	1

/* Macros for switching of the preallocated cursors for the canvas widget.  */

#define	SET_CLOCK_CURSOR	/* XDefineCursor(XtDisplay( ctx->canvas),
                                   ctx->cursors[ GOE_K_CURSOR_CLOCK ]);  */

#define	SET_HAND_CURSOR		/* XDefineCursor(XtDisplay(ctx->canvas),
                                   ctx->cursors[GOE_K_CURSOR_HAND]);  */

#define	SET_NORMAL_CURSOR   	/* XUndefineCursor(XtDisplay(ctx->canvas)  */

/* Global containing the access table used to store the graphics info of
   the system objects at run time. The table is initiated in the procedure
   init_graph_table. This global is accessed by all instances of goe.
   This variable introduces the only coupling between goe instances.
   This coupling is deliberate and desired.  */

#define                 GOE_K_GRAPH_TAB_MAX             200
static goe_tab_item     goe_graph_table[GOE_K_GRAPH_TAB_MAX];
static int              GOE_GRAPH_TABLE_INITIATED = FALSE;
                                                                                
/* These constants design the lines interactively drawn by the user.  */

#define line_constant 0.2
#define dash_constant 0.3

/* These constants design the palette.  */

#define        top                  115.0	/* maximum y */
#define        left_offset          3.0		/* offset from x = 0 */		
#define        rect_width           10.0	/* button rect */
#define        rect_height          7.5		
#define        obj_offset_x         1.0		/* offs. to button rect border */
#define        obj_offset_y         1.0
#define        text_offset_over     3.5		/* space over text label */
#define        text_offset_under    1.5		/* space under text label */
#define        offset               (text_offset_over + text_offset_under)
#define        obj_width            (rect_width - 2 * obj_offset_x)
#define        obj_height           (rect_height - 2 *obj_offset_y)
#define        x_increment          (rect_width + 3.0)
#define        y_increment          (rect_height + 2.0)
#define        button_separation    1.0		/* in the line choice palettes */
#define        conn_radii           1.2		/* pin and dot arc radius */
#define        sep_line_length      40.0	/* larger than the palette width */
#define        text_sym_x           2.0		/* used to position the text */
#define        text_sym_y           1.0		/* objects "A" and "L1" */
/* Coordinates for curr_attr_rect                                       */
#define  attr_rect_x  (left_offset + x_increment)
#define  attr_rect_y  (top-8.5*rect_height-y_increment-2*text_offset_over-6*offset)
                                     	
/*************************************************************************
 *   Scale of the text object in the palette and canvas.
 *************************************************************************/

#define GOE_K_TEXTSCALE_PAL	30.0
#define GOE_K_TEXTSCALE_CAN	110.0

/*************************************************************************
 *   These constants design the connection appearance
 *   on the canvas widget along with the conn_ggc global.
 *************************************************************************/

#define        goe_k_conn_radii           0.5
#define        goe_k_conn_length          3.0           
#define        goe_k_conn_no_offset       1.5           


/*************************************************************************
 * Rectangles used as identifiers in the UIL code to set the 
 * world coordinate systems for the widgets.
 * Because of geometry manager interference this only works approximately.
 * The real values are somewhat higher.
 *************************************************************************/

static GobeRectangleStruct goe_can_disp_rect 	= {{0,0},{100,100}},
                           goe_can_scroll_rect 	= {{-200,-200},{300,300}},
                           goe_pal_disp_rect	= {{0,0},{25,100}};

/*************************************************************************
 * Select masks
 *************************************************************************/

static unsigned int delete_mask[4]    = {1,0,0,0}; 
static unsigned int translate_mask[4] = {1,1,0,0}; 
static unsigned int select_mask[4]    = {1,1,1,0}; 
static unsigned int scale_mask[4]     = {1,1,1,0}; 
static unsigned int  object_mask[4]    = {1,1,1,0}; 

static unsigned int rubber_mask[4]    = {1,0,1,1}; 
static unsigned int click_mask[4]     = {1,0,0,0}; 

/*************************************************************************
 * Names of the DWT predefined colors used by GOE. 
 *************************************************************************/

static char *ColorNames[GOE_K_COLOR_MAX] =   { "black", "blue",
                                                "red",   "green"};

/*************************************************************************
 *  Names of the fonts used by GOE.
 *************************************************************************/

static  char            *font_names[GOE_K_FONT_MAX][GOE_K_SIZE_MAX] =
      {{"-*-Helvetica-Medium-R-Normal--8-*-*-ISO8859-1" ,
        "-*-Helvetica-Medium-R-Normal--10-*-*-ISO8859-1" ,
        "-*-Helvetica-Medium-R-Normal--12-*-*-ISO8859-1" ,
        "-*-Helvetica-Medium-R-Normal--14-*-*-ISO8859-1" ,
        "-*-Helvetica-Medium-R-Normal--18-*-*-ISO8859-1" ,
        "-*-Helvetica-Medium-R-Normal--24-*-*-ISO8859-1"},

       {"-*-Helvetica-Bold-R-Normal--8-*-*-ISO8859-1" ,
        "-*-Helvetica-Bold-R-Normal--10-*-*-ISO8859-1" ,
        "-*-Helvetica-Bold-R-Normal--12-*-*-ISO8859-1" ,
        "-*-Helvetica-Bold-R-Normal--14-*-*-ISO8859-1" ,
        "-*-Helvetica-Bold-R-Normal--18-*-*-ISO8859-1" ,
        "-*-Helvetica-Bold-R-Normal--24-*-*-ISO8859-1"},

      {"-*-Times-Medium-I-Normal--8-*-*-ISO8859-1" ,
        "-*-Times-Medium-I-Normal--10-*-*-ISO8859-1" ,
        "-*-Times-Medium-I-Normal--12-*-*-ISO8859-1" ,
        "-*-Times-Medium-I-Normal--14-*-*-ISO8859-1" ,
        "-*-Times-Medium-I-Normal--18-*-*-ISO8859-1" ,
        "-*-Times-Medium-I-Normal--24-*-*-ISO8859-1"}};

/*************************************************************************
 *  Arrays of strings used to display the current text writing attributes
 *  in the dialog box widget.
 *************************************************************************/

static char *style_outputs[GOE_K_FONT_MAX]= {"Normal", "Bold", "Italic"};
static char *size_outputs[GOE_K_SIZE_MAX]= {"8 Pixel",  "10 Pixel", "12 Pixel", 
                                            "14 Pixel", "18 Pixel", "24 Pixel"};

/*************************************************************************
 *  Constants that identify ggc.
 *************************************************************************/

#define        ggc_default             0
#define        ggc_min                 1
#define        ggc_fill_1              1
#define        ggc_fill_2              2
#define        ggc_fill_3              3
#define        ggc_fill_4              4
#define        ggc_fill_5              5
#define        ggc_fill_6              6
#define        ggc_line_1              7
#define        ggc_line_2              8
#define        ggc_line_3              9
#define        ggc_line_4              10
#define        ggc_line_5              11
#define        ggc_line_6              12
#define        ggc_dash_1              13
#define        ggc_dash_2              14
#define        ggc_dash_3              15
#define        ggc_dash_4              16
#define        ggc_dash_5              17
#define        ggc_dash_6              18
#define        ggc_color_1             19
#define        ggc_color_2             20
#define        ggc_color_3             21
#define        ggc_color_4             22
#define        ggc_palette_rect        23  
#define        ggc_palette_obj         24
#define        ggc_canvas_default      25 
#define        ggc_click_obj_high      26
#define        ggc_perm_rect_high      27
#define        ggc_perm_obj_high       28
#define        ggc_click_rect_high     29
#define        ggc_palette_frame       30
#define        ggc_perm_high           31
#define        ggc_perm_frame_high     32
#define        ggc_conn                33  
#define        ggc_palette_arc         34
#define        ggc_palette_arc_high    35
#define        ggc_palette_text        36
#define        ggc_palette_text_high   37
#define        ggc_canvas_high         38
#define        ggc_canvas_grid         39

/*************************************************************************
 * Constants subscripting the menu_buttons array.
 *************************************************************************/

#define        men_k_gr_min           0               
#define        men_k_gr1              0
#define        men_k_gr2              1
#define        men_k_gr3              2
#define        men_k_gr4              3
#define        men_k_gr5              4
#define        men_k_gr_max           4
#define        men_k_pi_min           5
#define        men_k_pi8              5
#define        men_k_pi10             6
#define        men_k_pi12             7
#define        men_k_pi14             8
#define        men_k_pi18             9
#define        men_k_pi24             10
#define        men_k_pi_max           10
#define        men_k_sty_min          11
#define        men_k_nor              11
#define        men_k_bol              12
#define        men_k_ita              13
#define        men_k_sty_max          13

/************************************************************************
 *  Constants subscripting the palette_objects array.
 ************************************************************************/

#define        pal_k_draw_min          0         
#define        pal_k_draw_line         0         
#define        pal_k_draw_rect         1
#define        pal_k_draw_text         2
#define        pal_k_draw_max          2         
#define        pal_k_conn_min          3
#define        pal_k_conn_pin          3
#define        pal_k_conn_dot          4
#define        pal_k_anno_text         5
#define        pal_k_conn_max          5
#define        pal_k_min               6
#define        pal_k_fill_min          6
#define        pal_k_fill_1            6
#define        pal_k_fill_2            7
#define        pal_k_fill_3            8
#define        pal_k_fill_4            9
#define        pal_k_fill_5            10
#define        pal_k_fill_6            11
#define        pal_k_fill_max          11
#define        pal_k_line_th_min       12
#define        pal_k_line_th_1         12
#define        pal_k_line_th_2         13
#define        pal_k_line_th_3         14
#define        pal_k_line_th_4         15
#define        pal_k_line_th_5         16
#define        pal_k_line_th_6         17
#define        pal_k_line_th_max       17
#define        pal_k_line_dash_min     18
#define        pal_k_line_dash_1       18
#define        pal_k_line_dash_2       19
#define        pal_k_line_dash_3       20
#define        pal_k_line_dash_4       21
#define        pal_k_line_dash_5       22
#define        pal_k_line_dash_6       23
#define        pal_k_line_dash_max     23
#define        pal_k_col_1             24
#define        pal_k_col_2             25
#define        pal_k_col_3             26
#define        pal_k_col_4             27

/*************************************************************************
 * Constants defining type of drawn object when saved or loaded. 
 *************************************************************************/

#define        type_k_line		0
#define        type_k_rect		1
#define        type_k_text		2
#define        type_k_obj_max		2
#define        type_k_dot		3
#define        type_k_pin		4
#define        type_k_anno		5

/**************************************************************************
 * String constant defining the name and location of the file used to store
 * the graphic information generated by GOE.
 **************************************************************************/

static char *goe_graphics_file = "pwr_db:wb_goe.graph";

#endif
