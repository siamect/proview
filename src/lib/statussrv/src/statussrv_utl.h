/** 
 * Proview   $Id: statussrv_utl.h,v 1.4 2007-06-01 11:27:58 claes Exp $
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

#ifndef statussrv_utl_h
#define statussrv_utl_h

#ifndef pwr_h
# include "pwr.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif



typedef struct {
  char Version[20];
  pwr_tStatus SystemStatus;
  char SystemStatusStr[120];
  pwr_tString80 Description;
  pwr_tTime SystemTime;
  pwr_tTime BootTime;
  pwr_tTime RestartTime;
  int Restarts;
} statussrv_sGetStatus;

typedef struct {
  pwr_tStatus 	ServerSts[20];
  char 		ServerStsStr[20][120];
  char 		ServerStsName[20][80];
  pwr_tStatus 	ApplSts[20];
  char 		ApplStsStr[20][120];
  char 		ApplStsName[20][80];
} statussrv_sGetExtStatus;


pwr_tStatus statussrv_GetStatus( char *nodename, statussrv_sGetStatus *result);
pwr_tStatus statussrv_GetExtStatus( char *nodename, statussrv_sGetExtStatus *result);
pwr_tStatus statussrv_Restart( char *nodename);
pwr_tStatus statussrv_XttStart( char *nodename, char *opplace, char *lang, char *display,
				char *gui);
pwr_tStatus statussrv_RtMonStart( char *nodename, char *lang, char *display, char *gui);

#ifdef __cplusplus
}
#endif

#endif
