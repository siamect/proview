/* 
 * Proview   $Id: rt_appl.h,v 1.5 2006-06-08 04:28:12 claes Exp $
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

#ifndef rt_appl_h
#define rt_appl_h

#include <string.h>
#include "pwr.h"
#include "rt_qcom.h"
#include "rt_errh.h"

/*! \file rt_appl.h
    \brief Contains the rt_appl class. */
/*! \addtogroup rt */
/*@{*/


//! Baseclass for applications.
/*! rt_appl is a baseclass for applications, where the subclasses should
  implement the virtual functions open(), close() and scan().
*/
class rt_appl {
 public:

  //! Constructor.
  rt_appl( 
	  const char *name, 	       	//< Process name.
	  errh_eAnix anix, 		//< Application index.
	  double scantime = 1.0,	//< Scantime for call of the scan() function.
	  qcom_sQid qid = qcom_cNQid	//< Qcom queue identity.
	  ) : m_anix(anix), m_scantime(scantime), m_maxdelay(5), m_qid(qid) 
    { strcpy( m_name, name); }
  void init();

  void register_appl( const char *name);
  void mainloop();
  double scantime() { return m_scantime;}
  void set_scantime( double time) { m_scantime = time;}
  pwr_tOid& apploid() { return m_apploid;}

  //! Initialize the application.
  /*! The open() function is called after initialization, and may contain setup of 
    direct links to objects and attributes in the realtime database. 

    The open() function is also called after a soft restart.
  */
  virtual void open() {};

  //! Closes the application.
  /*! the close() is called when a restart or stop event is received. It should 
    unlink to all direct links.
  */
  virtual void close() {};

  //! Cyclic function.
  /*! The scan() function is called cyclic with the scantime specified in the constructor.
    In this function all the control and supervision work of the application is placed.
  */
  virtual void scan() {};

  virtual ~rt_appl() {}

 private:
  errh_eAnix m_anix;
  double m_scantime;
  double m_maxdelay;
  qcom_sQid m_qid;
  char m_name[80];
  pwr_tOid m_apploid;
};

#endif

/** @} */
