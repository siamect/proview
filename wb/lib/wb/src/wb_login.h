/* 
 * Proview   $Id: wb_login.h,v 1.4 2007-01-04 07:29:03 claes Exp $
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

#ifndef wb_login_h
#define wb_login_h

#ifndef pwr_h
#include "pwr.h"
#endif

#ifndef pwr_privilege_h
#include "pwr_privilege.h"
#endif

typedef struct login_s_Prv login_sPrv;

typedef enum {
  login_mAttr_Navigator	= 1 << 0
} login_mAttr;

pwr_dImport login_sPrv	login_prv;

struct login_s_Prv {
  char		    username[40];
  char		    password[40];
  char		    group[40];
  pwr_mPrv	    priv;
  login_mAttr  attribute;
};

class WLogin {
 public:
  void		*parent_ctx;
  char		name[80];
  void	       	(*bc_success)();
  void	       	(*bc_cancel)();
  char	       	groupname[40];
  char	       	password[40];

  WLogin( void		*wl_parent_ctx,
	  char		*wl_name,
	  char		*wl_groupname,
	  void		(* wl_bc_success)(),
	  void		(* wl_bc_cancel)(),
	  pwr_tStatus  	*sts);
  virtual ~WLogin();

  void activate_ok();
  void activate_cancel();

  virtual pwr_tStatus get_values() {return 0;}
  virtual void message( char *new_label) {}

  static pwr_tStatus user_check( char *groupname, char *username, char *password);
  static pwr_tStatus insert_login_info( char *groupname, char *password, char *username, 
					unsigned long priv, unsigned long attr);
  static pwr_tStatus show_priv();

};

#endif
