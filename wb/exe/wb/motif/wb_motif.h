/* 
 * Proview   $Id: wb_motif.h,v 1.2 2008-10-31 12:51:31 claes Exp $
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

#ifndef wb_motif_h
#define wb_motif_h

#ifndef wb_main_h
#include "wb_main.h"
#endif

class WbMotif : public Wb {
 public:
  Widget toplevel;
  Widget mainwindow;

  WbMotif( int argc, char *argv[]);
  ~WbMotif() {};
  Wtt *wtt_new( const char *name, const char *iconname, ldh_tWBContext wbctx,
		pwr_tVolumeId volid, ldh_tVolume volctx,
		wnav_sStartMenu *root_menu, pwr_tStatus *status);
  WVsel *vsel_new( pwr_tStatus *status, const char *name, ldh_tWBContext wbctx,
		   char *volumename,
		   int (*bc_success)( void *, pwr_tVolumeId *, int),
		   void (*bc_cancel)(),
		   int (*bc_time_to_exit)( void *),
		   int show_volumes, wb_eType wb_type);

};

#endif
