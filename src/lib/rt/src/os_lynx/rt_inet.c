/* 
 * Proview   $Id: rt_inet.c,v 1.2 2005-09-01 14:57:57 claes Exp $
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

/* rt_inet.c -- Internet help functions */

#include "rt_inet.h"
#include <sockio.h>


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

