/* 
 * Proview   $Id: rt_procom.h,v 1.1 2006-09-14 14:16:07 claes Exp $
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

#ifndef rt_procom_h
#define rt_procom_h

#include "co_procom.h"
#include "rt_qcom.h"

class rt_procom : public co_procom {
 public:
  errh_eAnix 	m_anix;
  char 		m_name[32];
  pwr_tSid 	m_sid;
  pwr_tVid      m_vid;
  char		m_volumename[32];
  int		m_global;
  qcom_sGet 	*m_getmsg;
  
  rt_procom( co_provider *p, errh_eAnix anix, char *name, pwr_tSid sid, pwr_tVid vid,
	     char *volumename, int global);
  void init();
  void mainLoop();
  virtual ~rt_procom() {}
  virtual void provideObjects( pwr_tStatus, vector<procom_obj>&);
  virtual void provideObject( pwr_tStatus sts, pwr_tOix oix, pwr_tOix fthoix, pwr_tOix bwsoix,
		      pwr_tOix fwsoix, pwr_tOix fchoix, pwr_tOix lchoix, pwr_tCid cid,
		      char *name, char *longname) {}
  virtual void provideBody( pwr_tStatus sts, pwr_tOix oix, int size, void *body) {}
  virtual void provideAttr( pwr_tStatus sts, pwr_tOix oix, int size, void *buff);
  virtual void provideStatus( pwr_tStatus sts);
  virtual void flushNodes();
};


#endif

