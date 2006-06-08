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
  rt_appl( char *name, errh_eAnix anix, 
	   double scantime = 1.0, qcom_sQid qid = qcom_cNQid) : 
    m_anix(anix), m_scantime(scantime), m_qid(qid) 
    { strcpy( m_name, name); }
  void init();
  virtual void open() {};
  virtual void close() {};
  virtual void scan() {};
  void register_appl( char *name);
  void mainloop();
  double scantime() { return m_scantime;}
  void set_scantime( double time) { m_scantime = time;}
  pwr_tOid& apploid() { return m_apploid;}

  virtual ~rt_appl() {}

 private:
  errh_eAnix m_anix;
  double m_scantime;
  qcom_sQid m_qid;
  char m_name[80];
  pwr_tOid m_apploid;
};

#endif

/** @} */
