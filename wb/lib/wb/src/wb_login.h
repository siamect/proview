/* 
 * Proview   $Id: wb_login.h,v 1.3 2005-09-06 10:43:31 claes Exp $
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

#ifndef wb_nav_macros_h
#include "wb_nav_macros.h"
#endif

typedef struct login_s_Prv login_sPrv;

typedef enum {
  login_mAttr_Navigator	= 1 << 0
} login_mAttr;

struct login_s_Prv {
  char		    username[40];
  char		    password[40];
  char		    group[40];
  pwr_mPrv	    priv;
  login_mAttr  attribute;
};

struct login_widgets 
{                                             
  Widget	toplevel;
  Widget	login_window;
  Widget	label;
  Widget        adb;
  Widget	usernamevalue;
  Widget	passwordvalue;
};

typedef struct s_login_ctx
{
  t_commonpart	cp;
  struct login_widgets	widgets;
  void			(*login_bc_success)();
  void			(*login_bc_cancel)();
  char			groupname[40];
  char			password[40];
} t_login_ctx, *login_ctx;

pwr_dImport login_sPrv	login_prv;

/* Function prototypes. */

pwr_tStatus login_new (
  void		*parent_ctx,
  Widget	parent_wid,
  char		*name,
  char		*groupname,
  void		(*login_bc_success)(),
  void		(*login_bc_cancel)()
);
void login_delete (
  login_ctx	loginctx
);
pwr_tStatus login_get_values (
  login_ctx	loginctx
);
pwr_tStatus login_user_check  (
  char			*groupname,
  char			*username,
  char			*password
);
pwr_tStatus login_insert_login_info (
  char		*groupname,
  char		*password,
  char		*username,
  unsigned long	priv,
  unsigned long	attr
);
void login_message ( 
  login_ctx	loginctx,
  char 		*new_label 
);

pwr_tStatus login_show_priv ( 
  void
);
#endif
