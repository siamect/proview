/* wb_goen.c -- <short description>

   PROVIEW/R
   Copyright (C) 1996 by Comator Process AB.

   This module creates the gobe graphic context's used in
   gredit, the nodetypes and the connectiontypes.
   The module contains a number of methods to create the nodetypes
   and to fetch information about them.  */

#include <stdio.h>
#include <string.h>

#include "wb_foe_macros.h"
#include "wb_ldh.h"

#include <Xm/Xm.h>
#include <Mrm/MrmPublic.h>
#ifndef _XtIntrinsic_h
#include <X11/Intrinsic.h>
#endif
#include <X11/cursorfont.h>

#include "flow.h"
#include "flow_api.h"

#include "wb_foe_msg.h"
#include "wb_vldh.h"
#include "wb_goen.h"
#include "wb_gre.h"

int	goen_create_nodetype_m0();
int	goen_create_nodetype_m1();
int	goen_create_nodetype_m2();
int	goen_create_nodetype_m3();
int	goen_create_nodetype_m4();
int	goen_create_nodetype_m5();
int	goen_create_nodetype_m6();
int	goen_create_nodetype_m7();
int	goen_create_nodetype_m8();
int	goen_create_nodetype_m9();
int	goen_create_nodetype_m10();
int	goen_create_nodetype_m11();
int	goen_create_nodetype_m12();
int	goen_create_nodetype_m13();
int	goen_create_nodetype_m14();
int	goen_create_nodetype_m15();
int	goen_create_nodetype_m16();
int	goen_create_nodetype_m17();
int	goen_get_parameter_m0();
int	goen_get_parameter_m1();
int	goen_get_parameter_m2();
int	goen_get_parameter_m3();
int	goen_get_parameter_m4();
int	goen_get_parameter_m5();
int	goen_get_parameter_m6();
int	goen_get_parameter_m7();
int	goen_get_parameter_m8();
int	goen_get_parameter_m9();
int	goen_get_parameter_m10();
int	goen_get_parameter_m11();
int	goen_get_parameter_m12();
int	goen_get_parameter_m13();
int	goen_get_parameter_m14();
int	goen_get_parameter_m15();
int	goen_get_parameter_m16();
int	goen_get_parameter_m17();
int	goen_get_point_info_m0();
int	goen_get_point_info_m1();
int	goen_get_point_info_m2();
int	goen_get_point_info_m3();
int	goen_get_point_info_m4();
int	goen_get_point_info_m5();
int	goen_get_point_info_m6();
int	goen_get_point_info_m7();
int	goen_get_point_info_m8();
int	goen_get_point_info_m9();
int	goen_get_point_info_m10();
int	goen_get_point_info_m11();
int	goen_get_point_info_m12();
int	goen_get_point_info_m13();
int	goen_get_point_info_m14();
int	goen_get_point_info_m15();
int	goen_get_point_info_m16();
int	goen_get_point_info_m17();
int	goen_get_location_point_m0();
int	goen_get_location_point_m1();
int	goen_get_location_point_m2();
int	goen_get_location_point_m3();
int	goen_get_location_point_m4();
int	goen_get_location_point_m5();
int	goen_get_location_point_m6();
int	goen_get_location_point_m7();
int	goen_get_location_point_m8();
int	goen_get_location_point_m9();
int	goen_get_location_point_m10();
int	goen_get_location_point_m11();
int	goen_get_location_point_m12();
int	goen_get_location_point_m13();
int	goen_get_location_point_m14();
int	goen_get_location_point_m15();
int	goen_get_location_point_m16();
int	goen_get_location_point_m17();

/*_Local variables_______________________________________________________*/

int	(* goen_get_parameter_m[20]) () = {
	&goen_get_parameter_m0,
	&goen_get_parameter_m1,
	&goen_get_parameter_m2,
	&goen_get_parameter_m3,
	&goen_get_parameter_m4,
	&goen_get_parameter_m5,
	&goen_get_parameter_m6,
	&goen_get_parameter_m7,
	&goen_get_parameter_m8,
	&goen_get_parameter_m9,
	&goen_get_parameter_m0,
	&goen_get_parameter_m0,
	&goen_get_parameter_m0,
	&goen_get_parameter_m0,
	&goen_get_parameter_m14,
	&goen_get_parameter_m15,
	&goen_get_parameter_m16,
	&goen_get_parameter_m0,
	};
int	(* goen_create_nodetype_m[20]) () = {
	&goen_create_nodetype_m0,
	&goen_create_nodetype_m1,
	&goen_create_nodetype_m2,
	&goen_create_nodetype_m3,
	&goen_create_nodetype_m4,
	&goen_create_nodetype_m5,
	&goen_create_nodetype_m6,
	&goen_create_nodetype_m7,
	&goen_create_nodetype_m8,
	&goen_create_nodetype_m9,
	&goen_create_nodetype_m0,
	&goen_create_nodetype_m0,
	&goen_create_nodetype_m0,
	&goen_create_nodetype_m0,
	&goen_create_nodetype_m14,
	&goen_create_nodetype_m15,
	&goen_create_nodetype_m16,
	&goen_create_nodetype_m0,
	};
int	(* goen_get_point_info_m[20]) () = {
	&goen_get_point_info_m0,
	&goen_get_point_info_m1,
	&goen_get_point_info_m2,
	&goen_get_point_info_m3,
	&goen_get_point_info_m4,
	&goen_get_point_info_m5,
	&goen_get_point_info_m6,
	&goen_get_point_info_m7,
	&goen_get_point_info_m8,
	&goen_get_point_info_m9,
	&goen_get_point_info_m0,
	&goen_get_point_info_m0,
	&goen_get_point_info_m0,
	&goen_get_point_info_m0,
	&goen_get_point_info_m14,
	&goen_get_point_info_m15,
	&goen_get_point_info_m16,
	&goen_get_point_info_m0,
	};
int	(* goen_get_location_point_m[20]) () = {
	&goen_get_location_point_m0,
	&goen_get_location_point_m1,
	&goen_get_location_point_m2,
	&goen_get_location_point_m3,
	&goen_get_location_point_m4,
	&goen_get_location_point_m5,
	&goen_get_location_point_m6,
	&goen_get_location_point_m7,
	&goen_get_location_point_m8,
	&goen_get_location_point_m9,
	&goen_get_location_point_m0,
	&goen_get_location_point_m0,
	&goen_get_location_point_m0,
	&goen_get_location_point_m0,
	&goen_get_location_point_m14,
	&goen_get_location_point_m15,
	&goen_get_location_point_m16,
	&goen_get_location_point_m0,
	};
#if 0
int	(* goen_get_parameter_m[20]) () = {
	&goen_get_parameter_m0,
	&goen_get_parameter_m1,
	&goen_get_parameter_m2,
	&goen_get_parameter_m3,
	&goen_get_parameter_m4,
	&goen_get_parameter_m5,
	&goen_get_parameter_m6,
	&goen_get_parameter_m7,
	&goen_get_parameter_m8,
	&goen_get_parameter_m9,
	&goen_get_parameter_m10,
	&goen_get_parameter_m11,
	&goen_get_parameter_m12,
	&goen_get_parameter_m13,
	&goen_get_parameter_m14,
	&goen_get_parameter_m15,
	&goen_get_parameter_m16,
	&goen_get_parameter_m17,
	};
int	(* goen_create_nodetype_m[20]) () = {
	&goen_create_nodetype_m0,
	&goen_create_nodetype_m1,
	&goen_create_nodetype_m2,
	&goen_create_nodetype_m3,
	&goen_create_nodetype_m4,
	&goen_create_nodetype_m5,
	&goen_create_nodetype_m6,
	&goen_create_nodetype_m7,
	&goen_create_nodetype_m8,
	&goen_create_nodetype_m9,
	&goen_create_nodetype_m10,
	&goen_create_nodetype_m11,
	&goen_create_nodetype_m12,
	&goen_create_nodetype_m13,
	&goen_create_nodetype_m14,
	&goen_create_nodetype_m15,
	&goen_create_nodetype_m16,
	&goen_create_nodetype_m17,
	};
int	(* goen_get_point_info_m[20]) () = {
	&goen_get_point_info_m0,
	&goen_get_point_info_m1,
	&goen_get_point_info_m2,
	&goen_get_point_info_m3,
	&goen_get_point_info_m4,
	&goen_get_point_info_m5,
	&goen_get_point_info_m6,
	&goen_get_point_info_m7,
	&goen_get_point_info_m8,
	&goen_get_point_info_m9,
	&goen_get_point_info_m10,
	&goen_get_point_info_m11,
	&goen_get_point_info_m12,
	&goen_get_point_info_m13,
	&goen_get_point_info_m14,
	&goen_get_point_info_m15,
	&goen_get_point_info_m16,
	&goen_get_point_info_m17,
	};
int	(* goen_get_location_point_m[20]) () = {
	&goen_get_location_point_m0,
	&goen_get_location_point_m1,
	&goen_get_location_point_m2,
	&goen_get_location_point_m3,
	&goen_get_location_point_m4,
	&goen_get_location_point_m5,
	&goen_get_location_point_m6,
	&goen_get_location_point_m7,
	&goen_get_location_point_m8,
	&goen_get_location_point_m9,
	&goen_get_location_point_m10,
	&goen_get_location_point_m11,
	&goen_get_location_point_m12,
	&goen_get_location_point_m13,
	&goen_get_location_point_m14,
	&goen_get_location_point_m15,
	&goen_get_location_point_m16,
	&goen_get_location_point_m17,
	};
#endif



/*_Methods defined for this module_______________________________________*/


/*************************************************************************
*
* Name:		goen_create_cursors()
*
* Type		int
*
* Type		Parameter	IOGF	Description
* Widget	widget		I	widget load cursors in.
* goen_t_cursors *cursors	O	array of loaded cursors.
*
* Description:
*	Creates a number of cursors for a widget.
*	The cursors are placed in a vektor of type goen_t_cursors
*	with an index specified in the file wb_goen.h.
*	The following cursors are defined:
*	GOEN_CURSOR_CLOCK
*	GOEN_CURSOR_HAND	
*	GOEN_CURSOR_CROSS
*
*	Example of use:
*	r_ggc_values.cursor = (*cursors)[ GOEN_CURSOR_HAND ];
*
**************************************************************************/
void goen_create_cursors( 
    Widget		widget,
    goen_t_cursors	*cursors
)
{
	/* Clock cursor */
	(*cursors)[ GOEN_CURSOR_CLOCK ] = XCreateFontCursor (
		XtDisplay ( widget),
		XC_watch);

	/* Hand cursor */
	(*cursors)[ GOEN_CURSOR_HAND ] = XCreateFontCursor (
		XtDisplay ( widget),
		XC_hand2);

	/* Cross cursor */
	(*cursors)[ GOEN_CURSOR_CROSS ] = XCreateFontCursor (
		XtDisplay ( widget),
		XC_crosshair);
}


/*************************************************************************
*
* Name:		goen_create_ggcs()
*
* Type		int
*
* Type		Parameter	IOGF	Description
* Widget	widget		I	neted widget.
* goe_t_colors	*colors		I	array with loaded colors.
* goe_t_fonts	*fonts		I	array with loaded fonts.
* goen_t_cursors *cursors	I	array with loaded cursors.
* goen_t_ggcs	*ggcs		O	array with created ggc's.
*
* Description:
*	Creates a number of ggc's for a neted widget.
*	The ggc's are placed in a vektor of type goen_t_ggcs
*	with an index specified in the file wb_goen.h.
*	The following ggc's are defined:
*	GOEN_GGC_LINE		Usual ggc for drawing
*	GOEN_GGC_HIGHLIGHT	ggc for highlight
*	GOEN_GGC_TEXT		ggc for text 
*	GOEN_GGC_SUBWINDOW	ggc for subwindow mark
*	GOEN_GGC_BACKGROUND	ggc for rootobject 	
*	GOEN_GGC_PINTEXT	ggc for pintext	
*	GOEN_GGC_OBJNAMETEXT	ggc for instance object name
*	GOEN_GGC_SELRECT	ggc for selection rectangle
*	GOEN_GGC_GRID		ggc for gridpoint
*	GOEN_GGC_LINEDASHED	ggc for dashed line
*
*	Example of use:
*	r_ggc_values.perm_highlight = (*ggcs)[ GOEN_GGC_HIGHLIGHT ];
*
**************************************************************************/
#if 0
void goen_create_ggcs(
	Widget		widget,
	goe_t_colors	*colors,
	goe_t_fonts	*fonts,
	goen_t_cursors	*cursors,
	goen_t_ggcs	*ggcs
)
{
	GobeGgcValuesStruct	r_ggc_values;			
	int			mask;
	int			sts;
	
	/* Highlight ggc */
	mask = GobeMgbTrackingStyle | GobeMgbLineWidth
    		| GobeMgbFillPattern | GobeMgbForeground;

	r_ggc_values.tracking_style = GobeKtrackSolid;
	r_ggc_values.line_width	= GOEN_F_HIGHLIGHT_LINEWIDTH;
	r_ggc_values.fill_pattern = GobeKfillGrey916;
	r_ggc_values.foreground = (*colors)[ GOE_K_RED ];

	sts = GobeCreateAttributes (widget,	
				0,			
				mask,			
				&r_ggc_values,		
				&(*ggcs)[ GOEN_GGC_HIGHLIGHT ]);
	if ( sts != GobeSCsuccess) printf( "gobe error %d 115 \n",sts);

	/* Subwindowmark ggc */
	mask = GobeMgbTrackingStyle | GobeMgbLineWidth | GobeMgbFillPattern
	   | GobeMgbInvisible | GobeMgbPermHighlight | GobeMgbCursor;

	r_ggc_values.tracking_style = GobeKtrackSolid;
	r_ggc_values.line_width	= 0.010;
	r_ggc_values.fill_pattern = GobeKfillGrey916;
	r_ggc_values.perm_highlight = (*ggcs)[ GOEN_GGC_HIGHLIGHT ];
	r_ggc_values.invisible	= 1;
	r_ggc_values.cursor = GOEN_F_HOTCURSOR;

	sts = GobeCreateAttributes (widget,
				0,
				mask,	
				&r_ggc_values,	
				&(*ggcs)[ GOEN_GGC_SUBWINDOW ]);
	if ( sts != GobeSCsuccess) printf( "gobe error %d 114 \n",sts);

	/* Text ggc */
	mask = GobeMgbFontList | GobeMgbCursor | GobeMgbInvisible; 
	r_ggc_values.font_list	= GOEN_F_TEXTFONT;
	r_ggc_values.cursor = GOEN_F_HOTCURSOR;
	r_ggc_values.invisible	= 1;

	sts = GobeCreateAttributes (widget,
				0,
				mask,	
				&r_ggc_values,	
				&(*ggcs)[ GOEN_GGC_TEXT ]);
	if ( sts != GobeSCsuccess) printf( "gobe error %d 113 \n",sts);

	/* Big text ggc */
	mask = GobeMgbFontList | GobeMgbCursor | GobeMgbInvisible; 
	r_ggc_values.font_list	= GOEN_F_BIGTEXTFONT;
	r_ggc_values.cursor = GOEN_F_HOTCURSOR;
	r_ggc_values.invisible	= 1;

	sts = GobeCreateAttributes (widget,
				0,
				mask,	
				&r_ggc_values,	
				&(*ggcs)[ GOEN_GGC_BIGTEXT ]);
	if ( sts != GobeSCsuccess) printf( "gobe error %d 112 \n",sts);

	/* Medium text ggc */
	mask = GobeMgbFontList | GobeMgbCursor | GobeMgbInvisible; 
	r_ggc_values.font_list	= GOEN_F_MEDIUMTEXTFONT;
	r_ggc_values.cursor = GOEN_F_HOTCURSOR;
	r_ggc_values.invisible	= 1;

	sts = GobeCreateAttributes (widget,
				0,
				mask,	
				&r_ggc_values,	
				&(*ggcs)[ GOEN_GGC_MEDIUMTEXT ]);
	if ( sts != GobeSCsuccess) printf( "gobe error %d 111 \n",sts);

	/* small text ggc */
	mask = GobeMgbFontList | GobeMgbCursor | GobeMgbInvisible; 
	r_ggc_values.font_list	= GOEN_F_SMALLTEXTFONT;
	r_ggc_values.cursor = GOEN_F_HOTCURSOR;
	r_ggc_values.invisible	= 1;

	sts = GobeCreateAttributes (widget,
				0,
				mask,	
				&r_ggc_values,	
				&(*ggcs)[ GOEN_GGC_SMALLTEXT ]);
	if ( sts != GobeSCsuccess) printf( "gobe error %d 110 \n",sts);

	/* Objectname text ggc */
	mask = GobeMgbFontList | GobeMgbCursor | GobeMgbInvisible; 
	r_ggc_values.font_list	= GOEN_F_OBJNAMEFONT;
	r_ggc_values.cursor = GOEN_F_HOTCURSOR;
	r_ggc_values.invisible	= 1;

	sts = GobeCreateAttributes (widget,
				0,
				mask,	
				&r_ggc_values,	
				&(*ggcs)[ GOEN_GGC_OBJNAMETEXT ]);
	if ( sts != GobeSCsuccess) printf( "gobe error %d 109 \n",sts);

	/* Pintext ggc */
	mask = GobeMgbFontList | GobeMgbCursor | GobeMgbInvisible; 
	r_ggc_values.font_list = GOEN_F_PINNAMEFONT;
	r_ggc_values.cursor = GOEN_F_HOTCURSOR;
	r_ggc_values.invisible	= 1;

	sts = GobeCreateAttributes (widget,
				0,
				mask,	
				&r_ggc_values,	
				&(*ggcs)[ GOEN_GGC_PINTEXT ]);
	if ( sts != GobeSCsuccess) printf( "gobe error %d 108 \n",sts);

	/* Drawing ggc */
	mask = GobeMgbTrackingStyle | GobeMgbLineWidth 
	   | GobeMgbInvisible | GobeMgbPermHighlight | GobeMgbCursor;

	r_ggc_values.tracking_style = GobeKtrackSolid;
	r_ggc_values.line_width	= GOEN_F_LINEWIDTH;
	r_ggc_values.perm_highlight = (*ggcs)[ GOEN_GGC_HIGHLIGHT ];
	r_ggc_values.invisible	= 1;
	r_ggc_values.cursor = GOEN_F_HOTCURSOR;

	sts = GobeCreateAttributes (widget,
				0,
				mask,	
				&r_ggc_values,	
				&(*ggcs)[ GOEN_GGC_LINE ]);
	if ( sts != GobeSCsuccess) printf( "gobe error %d 107 \n",sts);

	/* Drawing dashed ggc */
	mask = GobeMgbTrackingStyle | GobeMgbLineWidth 
	   | GobeMgbInvisible | GobeMgbPermHighlight | GobeMgbCursor
	   | GobeMgbLineStyle | GobeMgbDashes;

	r_ggc_values.tracking_style = GobeKtrackSolid;
	r_ggc_values.line_width	= GOEN_F_LINEWIDTH;
	r_ggc_values.perm_highlight = (*ggcs)[ GOEN_GGC_HIGHLIGHT ];
	r_ggc_values.invisible	= 1;
	r_ggc_values.cursor = GOEN_F_HOTCURSOR;
	r_ggc_values.line_style = GobeKlineDashed;
	r_ggc_values.dashes = GOEN_F_DASHES;

	sts = GobeCreateAttributes (widget,
				0,
				mask,	
				&r_ggc_values,	
				&(*ggcs)[ GOEN_GGC_LINEDASHED ]);
	if ( sts != GobeSCsuccess) printf( "gobe error %d 106 \n",sts);

	/* Background ggc */
	mask = GobeMgbTrackingStyle | GobeMgbLineWidth | GobeMgbFillPattern
	   | GobeMgbInvisible | GobeMgbCursor;

	r_ggc_values.tracking_style = GobeKtrackSolid;
	r_ggc_values.line_width	= 0.001;
	r_ggc_values.fill_pattern = GobeKfillBackground;
	r_ggc_values.invisible	= 1;
	r_ggc_values.cursor = GOEN_F_HOTCURSOR;

	sts = GobeCreateAttributes (widget,
				0,
				mask,	
				&r_ggc_values,	
				&(*ggcs)[ GOEN_GGC_BACKGROUND ]);
	if ( sts != GobeSCsuccess) printf( "gobe error %d 105 \n",sts);

	/* Create Gobe attributes */
	mask = GobeMgbTrackingStyle | GobeMgbLineWidth 
		   | GobeMgbInvisible | GobeMgbCursor
		   | GobeMgbFillPattern | GobeMgbForeground;

	r_ggc_values.tracking_style = GobeKtrackSolid;
	r_ggc_values.line_width	= 0.01;
	r_ggc_values.invisible	= 1;
	r_ggc_values.cursor = (*cursors)[ GOEN_CURSOR_HAND ];
	r_ggc_values.foreground = (*colors)[GOE_K_BLUE];
	r_ggc_values.fill_pattern = GobeKfillGrey916;

	sts = GobeCreateAttributes (widget,
				0,
				mask,	
				&r_ggc_values,	
				&(*ggcs)[ GOEN_GGC_SELRECT ]);
	if ( sts != GobeSCsuccess) printf( "gobe error %d 104 \n",sts);

	r_ggc_values.invis_nav = 1;
	mask = GobeMgbInvisNav; 
	sts = GobeCreateAttributes (widget,
				0,
				mask,	
				&r_ggc_values,	
				&(*ggcs)[ GOEN_GGC_GRID ]);
	if ( sts != GobeSCsuccess) printf( "gobe error %d 103 \n",sts);

	/* Dispay node ggc */
	mask = GobeMgbFillPattern | GobeMgbFillOutline | GobeMgbForeground
	   | GobeMgbInvisible | GobeMgbPermHighlight;

	r_ggc_values.fill_pattern = GobeKfillGrey416;
	r_ggc_values.fill_outline = 1;
	r_ggc_values.perm_highlight = (*ggcs)[ GOEN_GGC_HIGHLIGHT ];
	r_ggc_values.invisible	= 1;
	r_ggc_values.foreground = (*colors)[GOE_K_BLUE];

	sts = GobeCreateAttributes (widget,
				0,
				mask,	
				&r_ggc_values,	
				&(*ggcs)[ GOEN_GGC_DISPLAYNODE ]);
	if ( sts != GobeSCsuccess) printf( "gobe error %d 102 \n",sts);


}
#endif

/*************************************************************************
*
* Name:		goen_create_contype()
*
* Type		int
*
* Type		Parameter	IOGF	Description
* Widget	widget		I	neted widget.
* unsigned long	conclass	I	connection class.
* unsigned long	ldhses		I	ldh session.
* goe_t_colors	*colors		I	array of loaded colors.
* goe_t_fonts	*fonts		I	array of loaded fonts.
* goen_t_cursors *cursors	I	array of loaded cursors.
* unsigned long  *con_type_id	O	created neted connections type id.
*
* Description:
*	Create a neted connection type.
*	The attributes of the connection is described in grapbody in the 
*	class object.
*
*	Parameters:
*
*    unsigned long 	con_type	index for where the connection type
*					id is stored in the window object.
*    int		arrows		no, one or two arrows.
*    float		linewidth	widht of the connection line.
*    float		dashes		length of dashes.
*    int 		fillpattern	fillpattern for line.
*    int		color		color.
*    int		curvature	drawing method for the connection.
*    float		corners		radius of corners.
*
*	Values:
*
*    arrows:
*	GOEN_ARROW_NONE	0		No arrow
*	GOEN_ARROW_END	1		One arrow at end of connection
*	GOEN_ARROW_BOTH	2		Two arrows
*    fillpattern:
*	GOEN_UNFILL	0		No fillpattern	
*	GOEN_FILL1	1		Some fillpattern...
*    color:
*	GOE_K_BLACK	0		black
*	GOE_K_BLUE      1		blue
*	GOE_K_RED	2		red
*	GOE_K_GREEN     3		greem
*    curvature:
*	GOEN_ROUTECON	0		Routed connection
*	GOEN_JAGGED	1		Streight line connection
*	GOEN_RECTILINEAR 2		Rectilinear connectionstyle
*	GOEN_STRANSDIV 	3		Simultaneus transition divergence
*	GOEN_STRANSCONV 4		Simultaneus transition convergence
*	GOEN_STEPDIV 	5		Step divergence
*	GOEN_STEPCONV 	6		Step convergence
*
**************************************************************************/
int goen_create_contype(
    flow_tCtx	        ctx,
    pwr_tClassId	conclass,
    ldh_tSesContext	ldhses,
    flow_tConClass      *con_class
)
{
	pwr_tClassId			bodyclass;
	pwr_sGraphPlcConnection 	*graphbody;
	int				sts, size;
	char				name[80];
	flow_eDrawType			line_type;
	flow_eConType			con_type;
	int				line_width;
	double				arrow_width = 0;
	double				arrow_length = 0;
	double				round_corner_amount;
	flow_eCorner			corner_type;

	if ( conclass == 0)
	{
          /* Fix for syref connections */
	  flow_CreateConClass(ctx, "RefCon", 
			flow_eConType_Reference, flow_eCorner_Right,
			flow_eDrawType_Line, 2, 0, 0, 0, flow_eConGroup_Common,
			con_class);
          return 1;
	}
	/* Get graphbody for the class */
	sts = ldh_GetClassBody(ldhses, conclass, "GraphPlcCon", 
		&bodyclass, (char **)&graphbody, &size);
	if( EVEN(sts) ) return sts;

	sts = ldh_ClassIdToName( ldhses, conclass, name, sizeof(name), &size);
	if( EVEN(sts) ) return sts;

	if (graphbody->arrows != 0)
	{
	  con_type = flow_eConType_StraightOneArrow;
	  arrow_width = 0.03;
	  arrow_length = 0.045;
	}
 	else if ( graphbody->curvature == GOEN_ROUTECON ||
	     graphbody->curvature == GOEN_RECTILINEAR)
	  con_type = flow_eConType_Routed;
	else if ( graphbody->curvature == GOEN_STRANSDIV)
	  con_type = flow_eConType_TransDiv;
	else if ( graphbody->curvature == GOEN_STRANSCONV)
	  con_type = flow_eConType_TransConv;
	else if ( graphbody->curvature == GOEN_STEPDIV)
	  con_type = flow_eConType_StepDiv;
	else if ( graphbody->curvature == GOEN_STEPCONV)
	  con_type = flow_eConType_StepConv;
	else if ( graphbody->curvature == GOEN_JAGGED)
	  con_type = flow_eConType_Straight;
	else
	  con_type = flow_eConType_Straight;

	line_width = graphbody->linewidth * 400 + 0.5;
	if ( graphbody->dashes > 0)
          line_type = flow_eDrawType_LineDashed;
	else if ( graphbody->fillpattern > 0)
          line_type = flow_eDrawType_LineGray;
	else
          line_type = flow_eDrawType_Line;
	
	if ( graphbody->corners > 0.0)
	{
	  corner_type = flow_eCorner_Rounded;
	  round_corner_amount = 0.025;
	}
	else
	{
	  corner_type = flow_eCorner_Right;
	  round_corner_amount = 0;
	}

	flow_CreateConClass(ctx, name,
			con_type, corner_type, 
			line_type, line_width, arrow_width, arrow_length, 
			round_corner_amount, flow_eConGroup_Common,
			con_class);
#if 0	
	/* Specify linewidth and arrowwidth */
	if ((graphbody->linewidth < 0.0) || (graphbody->linewidth > 0.5))
	  return GOEN__BADCONGRAP;

	if (graphbody->arrows >= GOEN_MAX_ARROWS)
	  return GOEN__BADCONGRAP;
	  
	/* Specify dashes */
	if ((graphbody->dashes < 0.0) || (graphbody->dashes > 1.0))
	  return GOEN__BADCONGRAP;

		max( 3 * graphbody->linewidth, 0.015);

	/* Specify dashes */
	if ((graphbody->dashes < 0.0) || (graphbody->dashes > 1.0))
	  return GOEN__BADCONGRAP;

	/* Specify fillpattern */
	if (graphbody->fillpattern >= GOEN_MAX_FILLPATTERN)
	  return GOEN__BADCONGRAP;

	/* Specify color */
	if (graphbody->color >= GOEN_MAX_COLOR)
	  return GOEN__BADCONGRAP;

 	if ( (graphbody->curvature == GOEN_ROUTECON) ||
	     (graphbody->curvature == GOEN_RECTILINEAR) )
	{
 	  contype_values.con_curvature = NetedKcurvatureRectilinear;
	  conmask[0] |= NetedMnbConCurvature;

	  /* Specify round corners */
	  if ((graphbody->corners > 0.0) && (graphbody->dashes < 0.1))
	  {
	    contype_values.corner_round_style = NetedKcornerRound1Line;
	    conmask[0] |= NetedMnbCornerRoundStyle;
	    contype_values.corner_round_amount = graphbody->corners;
	    conmask[0] |= NetedMnbCornerRoundAmount;
	  }
	}
 	contype_values.con_ggc = ggc_contype;
	conmask[0] |= NetedMnbConGGC;

        sts = NetedCreateConType ( 	widget,
				0,
				conmask,
				2, 
				&contype_values,
				con_type_id);
	tst_neted( sts, "NetedCreateConType ", 150 );
#endif
	return GOEN__SUCCESS;
}


/*************************************************************************
*
* Name:		goen_create_nodetype()
*
* Type		int
*
* Type		Parameter	IOGF	Description
* Widget	widget		I	neted widget.
* unsigned long	class		I	class of objekt.
* ldh_tSesContext		ldhses		I	ldh session.
* unsigned int *mask		I	mask of objekt.
* goe_t_colors	*colors		I	color array.
* goe_t_fonts	*fonts		I	font array.
* goen_t_ggcs	*ggcs		I	ggc array.
* unsigned long	subwindowmark	I	drawn with or without subwindowmark
* unsigned long	node_width	I	width of node dependent on the
*					annotation.
* unsigned long *node_type_id	O	neted node type id.
*
* Description:
*	Create a nodetype of the specified objecttype, mask and graphical
*	method.
*	The routine calls the graphical method for creating a nodetype
*	of this class.
*
**************************************************************************/
int goen_create_nodetype( 
    flow_tCtx	         ctx,
    pwr_tClassId	 class,
    ldh_tSesContext	 ldhses,
    unsigned int 	 *mask,
    unsigned long	 subwindowmark,
    unsigned long	 node_width,
    flow_tNode           *node_class,
    vldh_t_node		 node
)
{
	int			sts, size, graphmethod;
	pwr_tClassId		bodyclass;
	pwr_sGraphPlcNode 	*graphbody;

	/* Get graphbody for the class */
	sts = ldh_GetClassBody(ldhses, class, "GraphPlcNode", 
		&bodyclass, (char **)&graphbody, &size);
	if( EVEN(sts) ) return sts;

	graphmethod = graphbody->graphmethod;
	if ( graphmethod >= GOEN_MAX_GRAPHMETHOD ) return GOEN__BADMETHOD;

	sts = (goen_create_nodetype_m[graphmethod]) 
		( graphbody, class, ldhses, ctx, mask,
		subwindowmark, node_width, node_class, node);

	return sts;
}


/*************************************************************************
*
* Name:		goen_get_parinfo()
*
* Type		int
*
* Type		Parameter	IOGF	Description
* unsigned long	class		I	class of objekt.
* ldh_tSesContext		ldhses		I	ldh session.
* unsigned int *mask		I	mask of objekt.
* unsigned long	node_width	I	width of node dependent on the
*					annotation.
* unsigned long	con_point	I	connectionpoint to retrurn info about.
* goen_conpoint_type *graph_pointer O	geometrical info.
*	unsigned long	type		direction of connectionpoint.
*					CON_CENTER, CON_RIGHT, 
*					CON_UP, CON_LEFT or CON_DOWN
*	float		x		x koordinate of connection point
*					relativ to lower left corner of the
*					node.
*	float		y		y dito.
* unsigned long	*par_type	O	if the parameter is defined as 
					input, intern or output.
					PAR_INPUT, PAR_INTERN or PAR_OUTPUT
* unsigned long	*par_inverted	O	if an input inverted or not.
* unsigned long	*par_index	O	index in bodydef for the corresponding
*					parameter.
*
* Description:
*	Returns geometrical info of a connectionpoint and info of the 
*	corresponding parameter.
*	
*
**************************************************************************/
int	goen_get_parinfo( 
    gre_ctx			grectx,
    pwr_tClassId		class,
    ldh_tSesContext		ldhses,
    unsigned int		*mask,
    unsigned long		node_width,
    unsigned long		con_point,
    goen_conpoint_type		*graph_pointer,
    unsigned long		*par_type,
    unsigned long		*par_inverted,
    unsigned long		*par_index,
    vldh_t_node			 node
)
{
	int			sts, size, graphmethod;
	pwr_tClassId		bodyclass;
	pwr_sGraphPlcNode 	*graphbody;

	/* Get graphbody for the class */
	sts = ldh_GetClassBody(ldhses, class, "GraphPlcNode", 
		&bodyclass, (char **)&graphbody, &size);
	if( EVEN(sts) ) return sts;

	graphmethod = graphbody->graphmethod;
	if ( graphmethod >= GOEN_MAX_GRAPHMETHOD ) return GOEN__BADMETHOD;

	sts = (goen_get_point_info_m[graphmethod]) 
		( grectx, graphbody, con_point, mask, node_width, 
		graph_pointer, node);
	if ( EVEN(sts) ) return ( sts);

	sts = (goen_get_parameter_m[graphmethod]) 
		( graphbody, class, ldhses, con_point, mask, 
		par_type, par_inverted,	par_index);
	return sts;	
}

/*************************************************************************
*
* Name:		goen_get_pointinfo()
*
* Type		int
*
* Type		Parameter	IOGF	Description
* unsigned long	class		I	class of objekt.
* ldh_tSesContext		ldhses		I	ldh session.
* unsigned int *mask		I	mask of objekt.
* unsigned long	node_width	I	width of node dependent on the
*					annotation.
* unsigned long	con_point	I	connectionpoint to retrurn info about.
* goen_conpoint_type *graph_pointer O	geometrical info.
*	unsigned long	type		direction of connectionpoint.
*					CON_CENTER, CON_RIGHT, 
*					CON_UP, CON_LEFT or CON_DOWN
*	float		x		x koordinate of connection point
*					relativ to lower left corner of the
*					node.
*					koordinate of the node.
*	float		y		y dito.
*
* Description:
*	Gets returns geometrical info of a connectionpoint.
*
**************************************************************************/
int	goen_get_pointinfo( 
    gre_ctx		 	grectx,
    pwr_tClassId		class,
    ldh_tSesContext		ldhses,
    unsigned int		*mask,
    unsigned long		node_width,
    unsigned long		con_point,
    goen_conpoint_type	*graph_pointer,
    vldh_t_node		 node
)
{
	int			sts, size, graphmethod;
	pwr_tClassId		bodyclass;
	pwr_sGraphPlcNode 	*graphbody;

	/* Get graphbody for the class */
	sts = ldh_GetClassBody(ldhses, class, "GraphPlcNode", 
		&bodyclass, (char **)&graphbody, &size);
	if ( EVEN(sts)) return sts;
	graphmethod = graphbody->graphmethod;
	if ( graphmethod >= GOEN_MAX_GRAPHMETHOD ) return GOEN__BADMETHOD;

	sts = (goen_get_point_info_m[graphmethod]) 
		( grectx, graphbody, con_point, mask, node_width, 
		graph_pointer, node);
	return sts;
}


/*************************************************************************
*
* Name:		goen_get_parameter()
*
* Type		int
*
* Type		Parameter	IOGF	Description
* unsigned long	class		I	class of objekt.
* ldh_tSesContext		ldhses		I	ldh session.
* unsigned int *mask		I	mask of objekt.
* unsigned long	node_width	I	width of node dependent on the
*					annotation.
* unsigned long	con_point	I	connectionpoint to retrurn info about.
* unsigned long	*par_type	O	if the parameter is defined as 
					input, intern or output.
					PAR_INPUT, PAR_INTERN or PAR_OUTPUT
* unsigned long	*par_inverted	O	if an input inverted or not.
* unsigned long	*par_index	O	index in bodydef for the corresponding
*					parameter.
*
* Description:
*	Gets info of a corresponding parameter to a connectionpoint.
*
**************************************************************************/
int	goen_get_parameter( 
    pwr_tClassId		class,
    ldh_tSesContext		ldhses,
    unsigned int		*mask,
    unsigned long		con_point,
    unsigned long		*par_type,
    unsigned long		*par_inverted,
    unsigned long		*par_index
)
{
	int			sts, size, graphmethod;
	pwr_tClassId		bodyclass;
	pwr_sGraphPlcNode 	*graphbody;

	/* Get graphbody for the class */
	sts = ldh_GetClassBody(ldhses, class, "GraphPlcNode", 
		&bodyclass, (char **)&graphbody, &size);
	if ( EVEN(sts)) return sts;

	graphmethod = graphbody->graphmethod;
	if ( graphmethod >= GOEN_MAX_GRAPHMETHOD ) return GOEN__BADMETHOD;

	sts = ( goen_get_parameter_m[graphmethod])
		( graphbody, class, ldhses, con_point, mask, 
		par_type, par_inverted, par_index);
	return sts;
}


/*************************************************************************
*
* Name:		goen_get_location_point()
*
* Type		int
*
* Type		Parameter	IOGF	Description
* unsigned long	class		I	class of objekt.
* ldh_tSesContext		ldhses		I	ldh session.
* unsigned int *mask		I	mask of objekt.
* unsigned long	node_width	I	width of node dependent on the
*					annotation.
* unsigned long	con_point	I	connectionpoint to retrurn info about.
* goen_point_type *location_point O	location point.
*	float		x		x koordinate.
*	float		y		y koordinate.
*
* Description:
*	Returns location point of a node.
*	The location point is the point in the node that should be positioned
*	on a gridpoint in the neted window.
*	The returned koordinates are relative the lower left corner of the node.
*
**************************************************************************/
int	goen_get_location_point( 
    gre_ctx		 	grectx,
    pwr_tClassId		class,
    ldh_tSesContext		ldhses,
    unsigned int		*mask,
    unsigned long		node_width,
    goen_point_type		*location_point,
    vldh_t_node			node
)
{
	int			sts, size, graphmethod;
	pwr_tClassId		bodyclass;
	pwr_sGraphPlcNode 	*graphbody;

	/* Get graphbody for the class */
	sts = ldh_GetClassBody(ldhses, class, "GraphPlcNode", 
		&bodyclass, (char **)&graphbody, &size);
	if ( EVEN(sts)) return sts;
	graphmethod = graphbody->graphmethod;
	if ( graphmethod >= GOEN_MAX_GRAPHMETHOD ) return GOEN__BADMETHOD;

	sts = (goen_get_location_point_m[graphbody->graphmethod]) 
		( grectx, graphbody, mask, node_width, location_point, node);
	return sts;
}




/*************************************************************************
*
* Name:		goen_get_text_width()
*
* Type		int
*
* Type		Parameter	IOGF	Description
*
* Description:
*	Calculates the width of a string and returns the width
*	in world coordinates.
*
**************************************************************************/
int	goen_get_text_width( 
	char		*text,
	XmFontList	fontlist,
	float		*width,
	float		*height,
	int		*text_rows
)
{
    	XFontStruct		*font_id;
	XmFontContext		fontcontext;
	XmStringCharSet		charset;
	Boolean			sts;
	int			length;
	int			direction;
	int			ascent, descent;
	XCharStruct		charstruct;
	static float		scale = 0.00212;
	char			text_line[1024];
	char			*s, *t;
	int			line_width;
	int			max_width;
	int			stop;
	int			rows;
	int			text_height;

	/* Get the font */
	sts = XmFontListInitFontContext( &fontcontext, fontlist);
	if ( !sts) { printf( "No such fontlist\n"); return 0;}

	sts = XmFontListGetNextFont( fontcontext, &charset, &font_id);
	if ( !sts) { printf( "No such font\n"); return 0;}

	/* Get the text in every line */
	stop = 0;
	max_width = 0;
	t = text;
	rows = 0;
	while ( !stop)
	{	
	  s = strchr( t, 10);
	  if (s == 0)
	  {
	    s = text + strlen( text);
	    stop = 1;
	  }
	  strncpy(text_line, t, s - t);
	  text_line[ s - t ] = 0;

	  /* Get the lengt of the string */
	  length = strlen( text_line);
	  XTextExtents( font_id, text_line, length, &direction, &ascent, &descent,
		&charstruct);
	  line_width = charstruct.width;
	  max_width = max( max_width, line_width);

	  t = s + 1;
	  rows++;
	}
        text_height = font_id->ascent + font_id->descent;

	*height = scale * rows * text_height;
	*width = scale * max_width;
	*text_rows = rows;
	
	XmFontListFreeFontContext( fontcontext);
	return GOEN__SUCCESS;
}

