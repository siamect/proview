#ifndef xtt_fast_h
#define xtt_fast_h

/* xtt_fast.h -- Fast curves

   PROVIEW/R
   Copyright (C) 2003 by SSAB Oxelosund AB.

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
	  pwr_tObjid fast_object,
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












