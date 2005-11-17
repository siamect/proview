/* 
 * Proview   $Id: xtt_fast.h,v 1.4 2005-11-17 09:03:20 claes Exp $
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

#ifndef xtt_fast_h
#define xtt_fast_h

/* xtt_fast.h -- Fast curves */

#if defined __cplusplus
extern "C" {
#endif

#ifndef pwr_h
# include "pwr.h"
#endif

#ifndef ge_curve_h
# include "ge_curve.h"
#endif

#ifndef rt_fast_h
# include "rt_fast.h"
#endif

/*! \file xtt_fast.h
    \brief Contains the XttFast class. */
/*! \addtogroup Xtt */
/*@{*/

#define XTT_FAST_MAX 20

//! Window that displays fast curve in a DsFastCurve object.
/*!
  A DsFastCurve references a data array for the time axis, and up to 10 data arrays
  with stored data. These arrays are displayed in the XttFast window. The measurement
  of these curves are triggered manually, on a digital signal, or on a level of one of
  the attributes. When the measurement is completed this is signaled with the New attribute
  in the DsFastCurve object. The XttFast window then has to read the new data and
  reconfigure the curves. XttFast uses the GeCurve class to display the curves.
*/
class XttFast {
 public:
  
  //! Constructor
  XttFast(
	  void *xn_parent_ctx,
	  Widget	xn_parent_wid,
	  char *xn_name,
	  Widget *w,
	  pwr_sAttrRef *fast_arp,
	  int *sts);

  void       	*xnav;			//!< Pointer to parent XNav.
  Widget     	parent_widget;		//!< Parent widget.
  int        	fast_cnt;			//!< Number of fast curves.
  GeCurveData 	*gcd;			//!< Curve data for GeCurve object.
  GeCurve    	*curve;			//!< Curve window.
  pwr_tRefId 	new_subid;			//!< Subid of New attribute in DsFastCurve.
  pwr_tBoolean 	*new_p;			//!< Pointer to New attribute, which indicates new curve data. 
  pwr_sAttrRef 	time_buff;		//!< Attrref to data array for time axis.
  pwr_sAttrRef 	buff[FAST_CURVES]; 	//!< Attrefs to data arrays for curves.
  pwr_eType  	type[FAST_CURVES]; 	//!< Types of curves.
  int	       	fast_idx[FAST_CURVES]; 	//!< Conversion from DsFastCurve index to GeCurveData index.
  int	       	curve_idx[XTT_FAST_MAX]; //!< Conversion from GeCurveData index to DsFastCurve index.
  int        	element_size[XTT_FAST_MAX]; //!< Size of data element in curves.
  int        	max_points;		//!< Max number of points in curves.
  XtIntervalId  timerid;		//!< Time id for scan.
  void       	(*close_cb)( void *, XttFast *); //!< Close callback to parent.
  void       	(*help_cb)( void *, char *); //!< Open help window.
  pwr_tBoolean 	old_new;		//!< Old value of new. Used to detect edge of New.
  pwr_tMask	fast_function;		//!< Function configured in DsFastCurve object.
  bool		first_scan;		//!< Indicates that this is the first scan.
  bool		axis_configured;	//!< Axis range is configured in DsFastCurve object.
  pwr_sAttrRef 	trigg_index_attr;	//!< Attrref to TriggIndex attribute in DsFastCurve object.
  pwr_sAttrRef 	first_index_attr;	//!< Attrref to FirstIndex attribute in DsFastCurve object.
  pwr_sAttrRef 	last_index_attr;	//!< Attrref to LastIndex attribute in DsFastCurve object.
  pwr_sAttrRef 	trigg_time_attr;	//!< Attrref to TriggTime attribute in DsFastCurve object.

  //! Pop fast window.
  void pop();

  //! Destructor
  ~XttFast();
};

/*@}*/
#if defined __cplusplus
}
#endif
#endif












