#ifndef rt_dsfast_h
#define rt_dsfast_h

#include <string.h>
#include <vector>
extern "C" {
#include "pwr.h"
#include "pwr_baseclasses.h"
}

/*! \file rt_dsfast.h
    \brief Contains the rt_dsfast class. */
/*! \addtogroup rt */
/*@{*/

#define FAST_CURVES 10
#define fast_cNoTrigg 0xffffffff

typedef enum {
  fast_mFunction_ManTrigg      	= 1 << 0,	//!< Allow manual trigg.
  fast_mFunction_LevelTrigg     = 1 << 1,	//!< Trigg on level.
  fast_mFunction_BeforeTrigg	= 1 << 2,	//!< Display points before trigg.
  fast_mFunction_AlwaysPrepared	= 1 << 3,	//!< Overwrite old curve as soon as the old is viewed.
  fast_mFunction_User		= 1 << 4	//!< Curve is handled by the user.
} fast_mFunction;

class fastobject {
 public:
  fastobject( pwr_tObjid fast_oid) : oid( fast_oid), p(0), trigg(0), time_buffer(0),
    old_level(0), stop_index(0), scan_div(0), scan_cnt(0)
    { memset( attributes, 0, sizeof(attributes));
    memset( buffers, 0, sizeof(buffers));}
  void open( double base_scantime);
  void close();
  void scan();

 private:
  pwr_tObjid	oid;
  pwr_sClass_DsFastCurve *p;
  pwr_tBoolean 	*trigg;
  void		*attributes[FAST_CURVES];
  void		*buffers[FAST_CURVES];
  void		*time_buffer;
  pwr_tDlid	p_dlid;
  pwr_tDlid	trigg_dlid;
  pwr_tDlid	attributes_dlid[FAST_CURVES];
  pwr_tDlid	buffers_dlid[FAST_CURVES];
  pwr_tDlid     time_buffer_dlid;
  pwr_tUInt32 	attributes_size[FAST_CURVES];
  pwr_tTime	prepare_time;
  pwr_tBoolean	old_prepare;
  pwr_tBoolean	old_trigg;
  int		current_index;
  int		new_cnt;
  pwr_tFloat32	old_level;
  int		stop_index;
  int		scan_div;
  int		scan_cnt;
  double	scan_base;
};

//! Handling of TrendCurve objects.
/*! ... 
*/

class rt_fast {
 public:
  rt_fast() : fast_cnt(0), scan_time(0.1)
    {}

  void init( qcom_sQid *qid);
  void open();
  void close();
  void scan();
  double scantime() { return scan_time;}

 private:
  vector<fastobject*>  objects;
  int		fast_cnt;
  double	scan_time;
};

/*@}*/
#endif




