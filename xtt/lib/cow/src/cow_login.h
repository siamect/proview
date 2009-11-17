/* 
 * Proview   $Id: co_login.h,v 1.2 2008-10-31 12:51:30 claes Exp $
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

#ifndef cow_login_h
#define cow_login_h

#ifndef pwr_h
#include "pwr.h"
#endif

#ifndef pwr_privilege_h
#include "pwr_privilege.h"
#endif

typedef enum {
  login_mAttr_Navigator	= 1 << 0
} login_mAttr;

class CoLogin {
 private:
  static char		m_username[40];
  static char	        m_password[40];
  static char	        m_ucpassword[40];
  static char		m_group[40];
  static pwr_mPrv	m_priv;
  static login_mAttr    m_attribute;
 public:
  void		*parent_ctx;
  char		name[80];
  void	       	(*bc_success)( void *);
  void	       	(*bc_cancel)( void *);
  char	       	groupname[40];
  char	       	password[40];

  CoLogin( void		*wl_parent_ctx,
	  const char   	*wl_name,
	  const char   	*wl_groupname,
	  void		(* wl_bc_success)( void *),
	  void		(* wl_bc_cancel)( void *),
	  pwr_tStatus  	*sts);

  virtual ~CoLogin();

  void activate_ok();
  void activate_cancel();

  virtual pwr_tStatus get_values() {return 0;}
  virtual void message( const char *new_label) {}

  static pwr_tStatus user_check( const char *groupname, const char *username, const char *password);
  static pwr_tStatus insert_login_info( const char *groupname, const char *password, const char *username, 
					unsigned long priv, unsigned long attr);
  static pwr_tStatus get_login_info( char *groupname, char *password, char *username, 
			      unsigned long *priv, unsigned long *attr);
  static unsigned long privilege() { return m_priv;}
  static char *username() { return m_username;}
  static char *ucpassword() { return m_ucpassword;}
};

#endif
