/* rt_inet.c -- Internet help functions

   PROVIEW/R
   Copyright (C) 1998 by Comator Process AB.

   */

#include "rt_inet.h"
#include <net/if_arp.h>


pwr_tBoolean
inet_SetArpEntry (
  pwr_tStatus	*sts,
  int		s,
  struct arpreq	*rp  
)
{

#if 0
  return (ioctl(s, SIOCSARP, (caddr_t)rp) == 0);
#else
  return 1;
#endif
}


pwr_tBoolean
inet_DeleteArpEntry (
  pwr_tStatus	*sts,
  int		s,
  struct arpreq	*rp  
)
{

#if 0
  return (ioctl(s, SIOCDARP, (caddr_t)rp) == 0);
#else
  return 1;
#endif
}


pwr_tBoolean
inet_GetArpEntry (
  pwr_tStatus	*sts,
  int		s,
  struct arpreq	*rp  
)
{

#if 0
  return (ioctl(s, SIOCGARP, (caddr_t)rp) == 0);
#else
  return 1;
#endif
}

