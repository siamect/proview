/* 
 * Proview   $Id: wb_main.h,v 1.3 2008-10-31 12:51:31 claes Exp $
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

#include <map>

#ifndef wb_main_h
#define wb_main_h

#ifndef wb_ldh_h
#include "wb_ldh.h"
#endif

#ifndef wb_wtt_h
#include "wb_wtt.h"
#endif

#ifndef wb_vsel_h
#include "wb_vsel.h"
#endif


class Wb {
 public:
  typedef map<pwr_tVid, Wtt*>::iterator wttlist_iterator;

  static Wb *main_wb;

  map<pwr_tVid, Wtt*> wttlist;
  ldh_tWBContext wbctx;
  int announce;
  int appl_count;

  Wb() : announce(0), appl_count(0) {}
  virtual ~Wb() {}
  virtual Wtt *wtt_new( const char *name, const char *iconname, ldh_tWBContext wbctx,
			pwr_tVolumeId volid, ldh_tVolume volctx,
			wnav_sStartMenu *root_menu, pwr_tStatus *status)
    { return 0;}

  virtual WVsel *vsel_new( pwr_tStatus *status, const char *name, ldh_tWBContext wbctx,
			   char *volumename,
			   int (*bc_success)( void *, pwr_tVolumeId *, int),
			   void (*bc_cancel)(),
			   int (*bc_time_to_exit)( void *),
			   int show_volumes, wb_eType wb_type)
    { return 0;}

  void wttlist_add( pwr_tStatus *sts, Wtt *wtt, pwr_tVid vid);
  void wttlist_remove( pwr_tStatus *sts, Wtt* wtt);
  void wttlist_find( pwr_tStatus *sts, pwr_tVid vid, Wtt **wtt);

  static void find_wnav_cb( void *ctx, pwr_tOid oid);
  static void find_plc_cb( void *ctx, pwr_tOid oid);
  static void login_success( void *ctx);
  static void login_cancel( void *ctx);
  static void wtt_close( void *wttctx);
  static int time_to_exit( void *wttctx);
  static void wtt_open_volume( void *wttctx, wb_eType type, const char *filename, 
			       wow_eFileSelType file_type);
  static int vsel_success( void *vselctx, pwr_tVolumeId *volumelist, int volume_count);
  static void vsel_cancel();
  static void usage();
  static int psts(unsigned long int sts, FILE *logfile);
};

#endif
