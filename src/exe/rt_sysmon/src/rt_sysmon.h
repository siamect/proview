#ifndef rt_sysmon_h
#define rt_sysmon_h

using namespace std;

#include <string.h>
#include <vector>
extern "C" {
#include "pwr.h"
#include "pwr_baseclasses.h"
#include "rt_qcom.h"
}

/*! \file rt_sysmon.h
    \brief Contains the rt_sysmon class. */
/*! \addtogroup rt */
/*@{*/

class sysmon_object {
 public:
  sysmon_object( pwr_sAttrRef *arp) : aref(*arp), p(0), scan_div(0), scan_cnt(0)
    {}
  void open( double base_scantime);
  void close();
  void scan();
  pwr_tStatus status();

 protected:
  pwr_sAttrRef	aref;
  pwr_tAddress  p;
  pwr_tRefId    p_dlid;
  pwr_tCid 	cid;
  int		scan_div;
  int		scan_cnt;
  virtual void	exec() {};
};

class disksup_object: public sysmon_object {
 public:
  disksup_object( pwr_sAttrRef *arp) : sysmon_object(arp)
    {}

  void exec();
};

//! Monitoring of system supervisory objects.
/*! ... 
*/

class rt_sysmon {
 public:
  rt_sysmon() : sysmon_cnt(0), scan_time(10)
    {}

  void init( qcom_sQid *qid);
  void open();
  void close();
  void scan();
  double scantime() { return scan_time;}
  static int connect_alarm();
  static int alarm_send( pwr_tOid oid, char *alarm_text,
			 char *alarm_name, int alarm_prio);

 private:
  vector<sysmon_object*>  objects;
  pwr_sClass_SysMonConfig *conf;
  pwr_tRefId	conf_dlid;
  int		sysmon_cnt;
  double	scan_time;
};

/*@}*/
#endif




