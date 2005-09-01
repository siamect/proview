/* 
 * Proview   $Id: ini_loader.h,v 1.2 2005-09-01 14:57:48 claes Exp $
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

#ifndef ini_loader_h
#define ini_loader_h

#ifdef	OS_ELN
pwr_tStatus
ini_PlcLoad (
  pwr_tUInt32		plcversion,
  char			*plcfile,
  int			debugflag
);

pwr_tStatus
ini_PlcStart (
  pwr_tUInt32		plcversion
);

#endif

#ifdef  OS_VMS
pwr_tStatus
ini_PlcStart (
  pwr_tUInt32		plcversion,
  char			*plcfile,
  pwr_tUInt32		busid
);
#endif

void ini_StartApplications ();

#endif
