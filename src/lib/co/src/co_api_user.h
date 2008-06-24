/** 
 * Proview   $Id: co_api_user.h,v 1.1 2008-06-24 06:51:43 claes Exp $
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

#ifndef co_api_user_h
#define co_api_user_h

#if defined __cplusplus
extern "C" {
#endif

int user_CheckUser( char *systemgroup, char *user, char *password, 
	unsigned int *priv);
int user_CheckSystemGroup( char *systemgroup);
int user_GetUserPriv( char *systemgroup, char *user, unsigned int *priv);
void user_PrivToString( unsigned int priv, char *str, int size);
void user_RtPrivToString( unsigned int priv, char *str, int size);
void user_DevPrivToString( unsigned int priv, char *str, int size);
char *user_PwCrypt( char *password);

#if defined __cplusplus
}
#endif
#endif
