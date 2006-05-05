/* 
 * Proview   $Id: pwr_version.h,v 1.6 2006-05-05 11:34:50 claes Exp $
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

#ifndef pwr_version_h
#define pwr_version_h

/*
  File: pwr_inc:pwr_version.h

  Description:
    Defines version varibles to use in messages to user.
*/

#ifndef pwr_h
# include "pwr.h"
#endif

#if defined OS_VMS
# define pwrv_cOpSys              "OpenVMS"
#elif defined OS_ELN
# define pwrv_cOpSys              "VAXELN"
#elif defined OS_LYNX
# define pwrv_cOpSys              "LynxOS"
#elif defined OS_LINUX
# define pwrv_cOpSys              "Linux"
#elif defined OS_WNT
# define pwrv_cOpSys              "Windows NT"
#else
# define pwrv_cOpSys              "Unknown Operating System"
#endif

#if defined VAX || defined __VAX 
#define pwrv_cHardware           "VAX"
#elif defined __Alpha_AXP
#define pwrv_cHardware           "Alpha AXP"
#elif defined __powerpc__ 
#define pwrv_cHardware           "PowerPC"
#elif defined __x86__ 
#define pwrv_cHardware           "x86"
#else
#define pwrv_cHardware           "Unknown Hardware"
#endif

#if (pwr_dHost_byteOrder == pwr_dLittleEndian)
# define SET_VERSION(a, b, c, d)  ((pwr_tVersion)((d << 24) + (c << 16) + (b << 8) + a))		  
#else
# define SET_VERSION(a, b, c, d)  ((pwr_tVersion)((a << 24) + (a << 16) + (a << 8) + a))		  
#endif

#define pwrv_cBuildTimeStr       "14 Oct 2004"

#define pwrv_cPwrVersionStr      "V4.2.0"
#define pwrv_cPwrVersion         SET_VERSION('V', 4, 2, 0)

#define pwrv_cSmdVersion         SET_VERSION('V', 4, 2, 0)
#define pwrv_cSmdVersionStr      "V4.2.0"

#define pwrv_cBmdVersion         SET_VERSION('V', 4, 2, 0)
#define pwrv_cBmdVersionStr      "V4.2.0"

#define pwrv_cWbdbVersion        SET_VERSION('V', 4, 2, 0)
#define pwrv_cWbdbVersionStr     "V4.2.0"
#define pwrv_cWbdbVersionShortStr "V42"

#define pwrv_cLffVersion         SET_VERSION('V', 4, 2, 0)
#define pwrv_cLffVersionStr      "V4.2.0"

#endif
