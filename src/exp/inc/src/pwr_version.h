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

#define pwrv_cPwrVersionStr      "V4.1.0"
#define pwrv_cPwrVersion         SET_VERSION('V', 4, 1, 0)

#define pwrv_cSmdVersion         SET_VERSION('V', 4, 1, 0)
#define pwrv_cSmdVersionStr      "V4.1.0"

#define pwrv_cBmdVersion         SET_VERSION('V', 4, 1, 0)
#define pwrv_cBmdVersionStr      "V4.1.0"

#define pwrv_cWbdbVersion        SET_VERSION('V', 4, 1, 0)
#define pwrv_cWbdbVersionStr     "V4.1.0"
#define pwrv_cWbdbVersionShortStr "V41"

#define pwrv_cLffVersion         SET_VERSION('V', 4, 1, 0)
#define pwrv_cLffVersionStr      "V4.1.0"

#endif
