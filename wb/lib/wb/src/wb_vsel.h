/* 
 * Proview   $Id: wb_vsel.h,v 1.6 2008-10-31 12:51:32 claes Exp $
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

#ifndef wb_vsel_h
#define wb_vsel_h

/* wb_vsel.h -- select volume */

#ifndef pwr_h
#include "pwr.h"
#endif

#ifndef wb_h
#include "wb.h"
#endif

#ifndef cow_wow_h
#include "cow_wow.h"
#endif

#define VSEL_MAX_VOLUMES 200

class WVsel {
 public:
  WVsel (
	 pwr_tStatus   *status,
	 void		*wv_parent_ctx,
	 const char    	*wv_name,
	 ldh_tWBContext wv_wbctx,
	 char		*volumename,
	 int		(*bc_success)( void *, pwr_tVolumeId *, int),
	 void		(*bc_cancel)(),
	 int		(*bc_time_to_exit)( void *),
	 int		show_volumes,
	 wb_eType       wv_wb_type);
  virtual ~WVsel();

  void                  *parent_ctx;
  char                  name[40];
  ldh_tWBContext 	wbctx;
  int			(*vsel_bc_success)(void *, pwr_tVolumeId *, int);
  void			(*vsel_bc_cancel)();
  int			(*vsel_bc_time_to_exit)(void *);
  pwr_tVolumeId		volumes[VSEL_MAX_VOLUMES];
  int			volume_count;
  int			all;
  int			write_priv;
  wb_eType              wb_type;
  CoWow			*wow;

  virtual void list_add_item( char *str) {}
  virtual void list_clear() {}

  pwr_tStatus load_volumelist();
  pwr_tStatus check_volumelist( int quiet, int *display_window);

};

#endif




