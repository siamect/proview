/** 
 * Proview   $Id: co_api_user.cpp,v 1.1 2008-06-24 06:51:43 claes Exp $
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

#if defined OS_VMS && defined __ALPHA
# pragma message disable (NOSIMPINT,EXTROUENCUNNOBJ)
#endif

#if defined OS_VMS && !defined __ALPHA
# pragma message disable (LONGEXTERN)
#endif

#include <stdio.h>
#include <stdlib.h>

#include "pwr.h"
#include "co_api_user.h"
#include "co_dcli.h"
#include "co_user.h"

//
// c-api to co_user
//

int user_CheckUser( char *systemgroup, char *user, char *password, 
	unsigned int *priv)
{
  GeUser *gu;
  int sts;
  char filename[120];

  gu = new GeUser();
  sts = dcli_get_defaultfilename( user_cFilename, filename, "");
  sts = gu->load( filename);
  if ( ODD(sts))
    sts = gu->get_user( systemgroup, user, password, priv);
  delete gu;
  return sts;
}

int user_CheckSystemGroup( char *systemgroup)
{
  GeUser *gu;
  int sts;
  pwr_tMask attributes;
  char filename[120];
  pwr_tOix id;
  pwr_tString80 desc;

  gu = new GeUser();
  sts = dcli_get_defaultfilename( user_cFilename, filename, "");
  sts = gu->load( filename);
  if ( ODD(sts))
    sts = gu->get_system_data( systemgroup, &attributes, &id, desc);
  delete gu;
  return sts;
}

int user_GetUserPriv( char *systemgroup, char *user, unsigned int *priv)
{
  GeUser *gu;
  int sts;
  char filename[120];

  gu = new GeUser();
  sts = dcli_get_defaultfilename( user_cFilename, filename, "");
  sts = gu->load( filename);
  if ( ODD(sts))
    sts = gu->get_user_priv( systemgroup, user, priv);
  delete gu;
  return sts;
}

void user_PrivToString( unsigned int priv, char *str, int size)
{
  GeUser::priv_to_string( priv, str, size);
}

void user_RtPrivToString( unsigned int priv, char *str, int size)
{
  GeUser::rt_priv_to_string( priv, str, size);
}

void user_DevPrivToString( unsigned int priv, char *str, int size)
{
  GeUser::dev_priv_to_string( priv, str, size);
}

char *user_PwCrypt( char *password)
{
  return UserList::pwcrypt( password);
}
