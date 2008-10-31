/* 
 * Proview   $Id: xtt_tbl.h,v 1.4 2008-10-31 12:51:30 claes Exp $
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

#ifndef xtt_tbl_h
#define xtt_tbl_h

#ifndef pwr_privilege_h
#include "pwr_privilege.h"
#endif

#include "glow.h"
#include "rt_sevcli.h"

/* xtt_tbl.h -- Sev Table Viewer */


class TblNav;
class XttSevHist;
class CoLogin;
class CoWow;

class XttTbl {
 public:
  XttTbl( void *xn_parent_ctx, sevcli_tCtx xn_sevcli);
  void 		*parent_ctx;
  char 		name[80];
  TblNav	*tblnav;
  sevcli_tCtx  	sevcli;
  CoLogin      	*cologin;
  int 		command_open;
  void 		(*close_cb)( void *);
  char		base_user[80];
  char		user[80];
  unsigned int	base_priv;
  unsigned int	priv;
  int	       	verify;
  int	       	ccm_func_registred;
  CoWow		*wow;
  int		quiet;

  virtual void message( char severity, const char *message) {}
  virtual XttSevHist *sevhist_new( pwr_tOid oid, char *aname) { return 0;}
  virtual CoLogin *login_new( const char       	*wl_name,
			      const char       	*wl_groupname,
			      void		(* wl_bc_success)( void *),
			      void		(* wl_bc_cancel)( void *),
			      pwr_tStatus  	*status) { return 0;}
  virtual void set_prompt( const char *prompt) {}

  int is_authorized( unsigned int access = pwr_mAccess_AllSev, int msg = 1);
  void open_login();
  void logout();
  void activate_print();
  void activate_opensevhist();
  void activate_delete_item();
  void activate_zoom_in();
  void activate_zoom_out();
  void activate_zoom_reset();
  void activate_help();
  void activate_help_project();
  void activate_help_proview();

  static void message( void *attr, char severity, const char *message);
  static int is_authorized( void *ctx, unsigned int access, int msg);
  static void delete_item_yes( void *ctx, void *data);
  virtual ~XttTbl();

  int command( char* input_str);
  int readcmdfile( char *incommand);
  int read_bootfile( char *systemname, char *systemgroup);

};


#endif








