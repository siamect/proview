/* 
 * Proview   $Id: rt_inet.h,v 1.2 2005-09-01 14:57:55 claes Exp $
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

#ifndef rt_inet_h
#define rt_inet_h

/* rt_inet.h -- Internet help functions */

#include "pwr.h"

#if defined(OS_VMS) || defined(OS_ELN)
# if defined(OS_ELN) 
#  include socket
# else
#  include <socket.h>
# endif

# define ATF_COM  2   /* completed entry (arp_ha valid) */
# define ATF_PERM 4   /* permanent entry */
# define ATF_PUBL 8   /* publish (respond for other host) */

struct arpreq {
  struct  sockaddr arp_pa;                /* protocol address */
  struct  sockaddr arp_ha;                /* hardware address */
  int     arp_flags;                      /* flags */
};

#else
# include <sys/ioctl.h>
# if defined(OS_LINUX) || defined OS_MACOS
#  include <sys/socket.h>
# else
#  include <socket.h>
# endif
# include <net/if.h>
# include <net/if_arp.h>
#endif


pwr_tBoolean
inet_SetArpEntry (
  pwr_tStatus	*sts,
  int		sock,
  struct arpreq	*rp  
);

pwr_tBoolean
inet_DeleteArpEntry (
  pwr_tStatus	*sts,
  int		sock,
  struct arpreq	*rp  
);

pwr_tBoolean
inet_GetArpEntry (
  pwr_tStatus	*sts,
  int		sock,
  struct arpreq	*rp  
);
#endif
