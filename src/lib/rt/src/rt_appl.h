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

 private:
  errh_eAnix m_anix;
  double m_scantime;
  qcom_sQid m_qid;
  char m_name[80];
  pwr_tOid m_apploid;
};

#endif

