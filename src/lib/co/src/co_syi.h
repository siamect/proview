/* 
 * Proview   $Id: co_syi.h,v 1.2 2008-06-24 07:03:42 claes Exp $
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
 */

/* co_syi.h -- System information */
/* This module gives information about the system.  */

#ifndef co_syi_h
#define co_syi_h

#if defined __cplusplus
extern "C" {
#endif

char		*syi_BootDisk (pwr_tStatus*, char*, int);
int		syi_Busid (pwr_tStatus*);
char		*syi_Ethernet (pwr_tStatus*, char*, int);
char		*syi_HostName (pwr_tStatus*, char*, int);
char		*syi_HostSpec (pwr_tStatus*, char*, int);
char		*syi_NodeName (pwr_tStatus*, char*, int);
char		*syi_NodeSpec (pwr_tStatus*, char*, int);
pwr_tBoolean	syi_LocalBoot (pwr_tStatus*);
pwr_tStatus 	syi_UserName( char *user, int len);

#if defined __cplusplus
}
#endif
#endif
