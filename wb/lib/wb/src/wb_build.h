/* 
 * Proview   $Id: wb_build.h,v 1.3 2007-01-30 07:13:02 claes Exp $
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
 **/

#ifndef wb_build_h
#define wb_build_h

#include "pwr.h"
#include "wb_ldh.h"
#include "wb_session.h"
#include "wb_wnav.h"
#include "wb_build_opt.h"

class Wtt;

class wb_build : public wb_status
{
 public:
  wb_build( wb_session ses, WNav *wnav = 0):
    m_session(ses), m_wnav(wnav), m_hierarchy(pwr_cNOid) {};

  void classlist( pwr_tCid cid);
  void node( char *nodename, void *volumelist, int volumecnt);
  void volume();
  void rootvolume( pwr_tVid vid);
  void classvolume( pwr_tVid vid);
  void planthier( pwr_tOid oid);
  void nodehier( pwr_tOid oid);
  void plcpgm( pwr_tOid oid);
  void xttgraph( pwr_tOid oid);
  void webhandler( pwr_tOid oid);
  void application( pwr_tOid oid);

  wb_build_opt opt;
  wb_session m_session;
  WNav *m_wnav;
  pwr_tOid m_hierarchy;
};

#endif

