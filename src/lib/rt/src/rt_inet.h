#ifndef rt_inet_h
#define rt_inet_h

/* rt_inet.h -- Internet help functions

   PROVIEW/R
   Copyright (C) 1998 by Comator Process AB.

   .  */

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
# if defined(OS_LINUX)
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
