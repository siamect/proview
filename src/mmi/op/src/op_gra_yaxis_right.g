// 
//  Proview   $Id: op_gra_yaxis_right.g,v 1.2 2005-09-06 10:39:34 claes Exp $
//  Copyright (C) 2005 SSAB Oxelösund AB.
//
//  This program is free software; you can redistribute it and/or 
//  modify it under the terms of the GNU General Public License as 
//  published by the Free Software Foundation, either version 2 of 
//  the License, or (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful 
//  but WITHOUT ANY WARRANTY; without even the implied warranty of 
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the 
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License 
//  along with the program, if not, write to the Free Software 
//  Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
//
op_gra_yaxis_right: model

	op_gra_yaxis_right backgrflag 1
	first_obj: line 0 0 0 0
	first_obj fcolor 0
	first_obj vis 0

	// =======================================================
	//    define templates for graph basic components
	// =======================================================

	temp: model
				// y axis attributes
				ecolor 7
				estyle 1		
				ewidth 1.

		y_axis_line: 	line 0 0 0 0

		y_axis_majortick: line 0 0 2 0
		y_axis_minortick: line 0 0 1 0

				// y axis label attributes - cntr aligned text
				tcolor 7
				font 1
				prec 0
				path 1
				height 1.5
				size 0 0
				align 1 3

		y_axis_label: 	text "%g" 4 0

	endm		// end of template submodel


// ==========================================================
// y axis
		// yaxis attributes
	ecolor 7
	estyle 1
	ewidth 1.
	fcolor 9
	fstyle 1
	finter 1

	y_axis: graphaxis temp.y_axis_line \
			temp.y_axis_majortick \
			temp.y_axis_minortick \
			temp.y_axis_label

	y_axis coordlimits 0.5 0. 0.5 40.

	y_axis dynprop \
	    (y_min_value (= * \
		(valuelimits y_min_value y_max_value) \
		(majorspacing y_tickmajor) \
		(minorspacing y_tickminor) \
		(theight text_height) \
	))
endm
