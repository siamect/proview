// op_gra_htrend3.g -- <short description>
//
// PROVIEW/R
// Copyright (C) 1996 by Comator Process AB.
//
// <Description>.
//
// htrend3.g - graph template for three horizontal trending line traces
//
// Copyright (C) 1989 Sherrill-Lubinski.  All Rights Reserved.
// 14 July 1989
//
op_gra_htrend3: model

	op_gra_htrend3 backgrflag 1

	// =======================================================
	//    define templates for graph basic components
	// =======================================================

	temp: model
				// x and y axes

				ecolor 7
				estyle 1		
				ewidth 1.

		x_axis_line: 	line 0 0 0 0
		y_axis_line: 	line 0 0 0 0

				// tick marks for x axis

				ecolor 7
				estyle 1		
				ewidth 1.

		x_axis_majortick: line 0 -2 0 0
		x_axis_minortick: line 0 -1 0 0

				// tick marks for left y axis

				ecolor 7
				estyle 1		
				ewidth 1.

		y1_axis_majortick: line -1 0 0 0
		y1_axis_minortick: line -0.5 0 0 0

				// tick marks for right y axis

				ecolor 7
				estyle 1		
				ewidth 1.

		y2_axis_majortick: line 1 0 0 0
		y2_axis_minortick: line 0.5 0 0 0

				// grid lines

				ecolor 7
				estyle 2
				ewidth 1.

		x_grid_tick:	line 0 0 0 24
		y_grid_tick:	line 0 0 32 0

				// x axis label - center aligned text

				tcolor 0
				font 1
				prec 0
				path 1
				height 1.5
				size 0 0

				align 2 3

		x_axis_label: 	text "%g" 0 -4

				// left y axis label - right-center aligned text

				tcolor 7
				font 1
				prec 0
				path 1
				height 1.5
				size 0 0

				align 3 3

		y1_axis_label: 	text "%g" -2 0

				// right y axis label - left-center aligned text

				tcolor 7
				font 1
				prec 0
				path 1
				height 1.5
				size 0 0

				align 1 3

		y2_axis_label: 	text "%g" 2 0

				// trace line

				ecolor 7
				estyle 1
				ewidth 1.

		trace_line: 	line 0 0 0 0

				// trace marker

				mcolor 7
				mstyle 3
				msize 2.

		trace_mark: 	mark 0 0

	endm		// end of template submodel

        // ==========================================================
        //                     define graph
        // ==========================================================

// ==========================================================
// Background rectangle

	   ecolor 7
	   estyle 1
	   ewidth 1.
	   fcolor 0
	   fstyle 1
	   finter 1

   backgr: frect 0. 0. 32. 24.

   backgr dynprop \
	(x_min_value (= * \
		(redraw) \
	)) \
	(x_max_value (= * \
		(redraw) \
	)) \
	(trace1_y_value (= * \
		(redraw) \
	)) \
	(trace1_y_array (= * \
		(redraw) \
	)) \
	(trace2_y_value (= * \
		(redraw) \
	)) \
	(trace2_y_array (= * \
		(redraw) \
	)) \
	(trace3_y_value (= * \
		(redraw) \
	)) \
	(trace3_y_array (= * \
		(redraw) \
	)) \
	(backgr_color (= * \
		(fcolor backgr_color) \
	))

// ==========================================================
// X grid: built using specially defined axis (no axis line)

   x_grid: graphaxis nil \
			temp.x_grid_tick \
			nil \
			nil

   x_grid coordlimits 0. 0. 32. 0.

   x_grid dynprop \
	(x_min_value (= * \
		(valuelimits x_min_value x_max_value) \
		(majorspacing x_tickmajor) \
	)) \
	(trace1_y_value (= * \
		(redraw) \
	)) \
	(trace1_y_array (= * \
		(redraw) \
	)) \
	(trace2_y_value (= * \
		(redraw) \
	)) \
	(trace2_y_array (= * \
		(redraw) \
	)) \
	(trace3_y_value (= * \
		(redraw) \
	)) \
	(trace3_y_array (= * \
		(redraw) \
	))

// ==========================================================
// X axis

   x_axis: graphaxis temp.x_axis_line \
			temp.x_axis_majortick \
			temp.x_axis_minortick \
			nil
//			temp.x_axis_label

   x_axis coordlimits 0. -0.5 32. -0.5

   x_axis dynprop \
	(x_min_value (= * \
		(valuelimits x_min_value x_max_value) \
		(majorspacing x_tickmajor) \
		(minorspacing x_tickminor) \
	))

// ==========================================================
// Y grid: built using specially defined axis (no axis line)

   y_grid: graphaxis nil \
			temp.y_grid_tick \
			nil \
			nil

   y_grid coordlimits 0. 0. 0. 24.

   y_grid dynprop \
	(yg_min_value (= * \
		(valuelimits yg_min_value yg_max_value) \
		(majorspacing yg_tickmajor) \
	)) \
	(trace1_y_value (= * \
		(redraw) \
	)) \
	(trace1_y_array (= * \
		(redraw) \
	))

// ==========================================================
// Left Y axis - do not display if graph is trending from the left;
//		 trending direction is determined by checking the
//		 x_min_value, x_max_value, and x_shift_val

   y1_axis: graphaxis temp.y_axis_line \
			temp.y1_axis_majortick \
			temp.y1_axis_minortick \
			temp.y1_axis_label

   y1_axis coordlimits -0.5 0. -0.5 24.

   y1_axis dynprop \
	(y1_min_value (= * \
		(valuelimits y1_min_value y1_max_value) \
		(majorspacing y1_tickmajor) \
		(minorspacing y1_tickminor) \
	))
 
// ==========================================================
// Right Y axis - do not display if graph is trending from the left;
//		  trending direction is determined by checking the
//		  x_min_value, x_max_value, and x_shift_val

   y2_axis: graphaxis temp.y_axis_line \
			temp.y2_axis_majortick \
			temp.y2_axis_minortick \
			temp.y2_axis_label

   y2_axis coordlimits 32.5 0. 32.5 24.

   y2_axis dynprop \
	(y2_min_value (= * \
		(valuelimits y2_min_value y2_max_value) \
		(majorspacing y2_tickmajor) \
		(minorspacing y2_tickminor) \
	))
 
// ==========================================================
// The traces

   trace1: graphtrace 100 \
			temp.trace_line \
			nil

   trace1 coordlimits  0. 0. 32. 24.

   trace1 dynprop \
	(x_min_value (= * \
		(tracelength num_points) \
		(xvaluelimits x_min_value x_max_value) \
		(yvaluelimits y1_min_value y1_max_value) \
		(plotreset) \
	)) \
	(trace1_y_value (= * \
		(plotshiftx x_shift_val) \
		(plotdata x_start trace1_y_value) \
	)) \
	(trace1_y_array (= * \
		(plotreset) \
		(plotdata trace1_x_array trace1_y_array) \
	)) \
	(trace1_color (= * \
		(ecolor trace1_color) \
	))

   trace2: graphtrace 100 \
			temp.trace_line \
			nil

   trace2 coordlimits  0. 0. 32. 24.

   trace2 dynprop \
	(x_min_value (= * \
		(tracelength num_points) \
		(xvaluelimits x_min_value x_max_value) \
		(yvaluelimits y2_min_value y2_max_value) \
		(plotreset) \
	)) \
	(trace2_y_value (= * \
		(plotshiftx x_shift_val) \
		(plotdata x_start trace2_y_value) \
	)) \
	(trace2_y_array (= * \
		(plotreset) \
		(plotdata trace2_x_array trace2_y_array) \
	)) \
	(trace2_color (= * \
		(ecolor trace2_color) \
	))


   trace3: graphtrace 100 \
			temp.trace_line \
			nil

   trace3 coordlimits  0. 0. 32. 24.

   trace3 dynprop \
	(x_min_value (= * \
		(tracelength num_points) \
		(xvaluelimits x_min_value x_max_value) \
		(yvaluelimits y3_min_value y3_max_value) \
		(plotreset) \
	)) \
	(trace3_y_value (= * \
		(plotshiftx x_shift_val) \
		(plotdata x_start trace3_y_value) \
	)) \
	(trace3_y_array (= * \
		(plotreset) \
		(plotdata trace3_x_array trace3_y_array) \
	)) \
	(trace3_color (= * \
		(ecolor trace3_color) \
	))

endm
