/* 
 * Proview   $Id: wb_login.cpp,v 1.1 2007-01-04 07:29:03 claes Exp $
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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "wb_login_msg.h"
#include "wb_login.h"
#include "wb_utl_api.h"
#include "co_api.h"
#include "co_dcli.h"

pwr_dExport login_sPrv  login_prv;

void WLogin::activate_ok()
{
  int	sts;

  message( "");
  sts = get_values();
  if ( ODD(sts)) {
    if ( bc_success)
      ( bc_success) ();
    delete this;
    return;
  }
  else {
    message( "User not authorized");
    printf( "User not authorized\n");
    strcpy( (char *) &password, "");
  }
}

void WLogin::activate_cancel()
{
  if ( bc_cancel)
    (bc_cancel) ();

  delete this;
}

//
// Constructor
//
WLogin::WLogin( void		*wl_parent_ctx,
		char		*wl_name,
		char		*wl_groupname,
		void		(* wl_bc_success)(),
		void		(* wl_bc_cancel)(),
		pwr_tStatus   	*status) :
  parent_ctx(wl_parent_ctx), bc_success(wl_bc_success), bc_cancel(wl_bc_cancel)  
{
  strcpy( name, wl_name);
  strcpy( groupname, wl_groupname);
  *status = 1;
}

//
// Destructor
//
WLogin::~WLogin()
{
}

//
//	Check username and password and insert login infomation.
//
pwr_tStatus WLogin::user_check( char *groupname, char *username, char *password)
{
  pwr_tStatus		sts;
  unsigned int		priv;
  unsigned long		attr = 0;
  
  sts = user_CheckUser( groupname, username, password, &priv);
  if ( EVEN(sts)) return sts;

  insert_login_info( groupname, password, username, priv, attr);
  return LOGIN__SUCCESS;
}

//
//	Inserts login info in global priv struct.
//
pwr_tStatus WLogin::insert_login_info( char *groupname, char *password, char *username, 
				       unsigned long priv, unsigned long attr)
{
  strcpy(login_prv.username, username);
  strcpy(login_prv.password, password);
  strcpy(login_prv.group, groupname);
  login_prv.priv = (pwr_mPrv)priv;
  login_prv.attribute = (login_mAttr)attr;

  return LOGIN__SUCCESS;
}

//
//	Writes a userlist.
//
pwr_tStatus WLogin::show_priv()
{

  printf("\n");
  printf("Authorized privileges for    User:        %s\n", login_prv.username);
  printf("                             SystemGroup: %s\n", login_prv.group);
  if ( login_prv.priv & pwr_mPrv_RtRead)
    printf("	RtRead\n");
  if ( login_prv.priv & pwr_mPrv_RtWrite)
    printf("	RtWrite\n");
  if ( login_prv.priv & pwr_mPrv_DevRead)
    printf("	DevRead\n");
  if ( login_prv.priv & pwr_mPrv_DevPlc)
    printf("	DevPlc\n");
  if ( login_prv.priv & pwr_mPrv_DevConfig)
    printf("	DevConfig\n");
  if ( login_prv.priv & pwr_mPrv_DevClass)
    printf("	DevClass\n");

  return LOGIN__SUCCESS;
}

