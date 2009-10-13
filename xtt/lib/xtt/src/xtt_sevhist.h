/* 
 * Proview   $Id: xtt_sevhist.h,v 1.2 2008-10-31 12:51:36 claes Exp $
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

#ifndef xtt_sevhist_h
#define xtt_sevhist_h

/* xtt_sevhist.h -- Sevhist curves */

#ifndef pwr_h
# include "pwr.h"
#endif

#ifndef ge_curve_h
# include "ge_curve.h"
#endif

#ifndef rt_sevcli_h
# include "rt_sevcli.h"
#endif

/*! \file xtt_sevhist.h
    \brief Contains the XttSevHist class. */
/*! \addtogroup Xtt */
/*@{*/

#define XTT_SEVHIST_MAX 20

class CoWow;
class CoWowTimer;

//! Window that displays sevhist curve in a SevHistCurve object.
/*!
  A SevHistCurve references a data array for the time axis, and up to 10 data arrays
  with stored data. These arrays are displayed in the XttSevHist window. The measurement
  of these curves are triggered manually, on a digital signal, or on a level of one of
  the attributes. When the measurement is completed this is signaled with the New attribute
  in the SevHistCurve object. The XttSevHist window then has to read the new data and
  reconfigure the curves. XttSevHist uses the GeCurve class to display the curves.
*/
class XttSevHist {
 public:
  
  void       	*xnav;			//!< Pointer to parent XNav.
  GeCurveData 	*gcd;			//!< Curve data for GeCurve object.
  GeCurve    	*curve;			//!< Curve window.
  int        	rows;			//!< Max number of points in curves.
  pwr_eType	vtype;
  unsigned int  vsize;
  CoWowTimer    *timerid;		//!< Time id for scan.
  void       	(*close_cb)( void *, XttSevHist *); //!< Close callback to parent.
  void       	(*help_cb)( void *, const char *); //!< Open help window.
  bool		first_scan;		//!< Indicates that this is the first scan.
  char		title[250];			//!< Window title
  sevcli_tCtx	scctx;
  pwr_tOName	aname;
  pwr_tOid	oid;
  CoWow		*wow;
  long int	time_low_old;
  long int	time_high_old;

  //! Constructor
  XttSevHist( void *xn_parent_ctx,
	     const char *xn_name,
	     pwr_tOid* xn_oid,
	     pwr_tOName *xn_aname,
	     sevcli_tCtx xn_scctx,
	     int *sts);

  //! Destructor
  virtual ~XttSevHist();

  //! Pop sevhist window.
  void pop();
  int get_data( pwr_tStatus *sts, pwr_tTime from, pwr_tTime to);

  static void sevhist_close_cb( void *ctx);
  static void sevhist_higher_res_cb( void *ctx);
  static void sevhist_lower_res_cb( void *ctx);
  static void sevhist_help_cb( void *ctx);
  static void sevhist_scan( void *data);

};

/*@}*/
#endif












