/* rt_inet.c -- Internet help functions

   PROVIEW/R
   Copyright (C) 1998 by Comator Process AB.

   .  */

#ifndef OS_ELN
# error this file is only for OS_ELN
#endif

#include $vaxelnc
#include stdarg
#include stsdef
#include $internet_utility
#include "rt_inet.h"


pwr_tBoolean
inet_GetArpEntry (
  pwr_tStatus	*sts,
  int		sock,
  struct arpreq	*rp  
)
{
  INET$INTERNET_ADDRESS *ia;
  NET$ETHERNET_ADDRESS	*ea;

  ia = &rp->arp_pa.sa_data;
  ea = &rp->arp_ha.sa_data;

  eln$inet_find_arp_entry(sts, ia, ea);

  return ODD(*sts);
}
  

pwr_tBoolean
inet_SetArpEntry (
  pwr_tStatus	*sts,
  int		sock,
  struct arpreq	*rp  
)
{
  INET$INTERNET_ADDRESS	*ia;
  NET$ETHERNET_ADDRESS	*ea;
  INET$SET_ARP_OPTIONS	opt;

  opt.mask_value = 0;
  opt.fields.permanent_field = (0 != (rp->arp_flags & ATF_PERM));

  ia = &rp->arp_pa.sa_data;
  ea = &rp->arp_ha.sa_data;

  eln$inet_set_arp_entry(sts, ia, ea, opt);

  return ODD(*sts);
}


pwr_tBoolean
inet_DeleteArpEntry (
  pwr_tStatus	*sts,
  int		sock,
  struct arpreq	*rp  
)
{
  INET$INTERNET_ADDRESS	*ia;

  ia = &rp->arp_pa.sa_data;

  eln$inet_delete_arp_entry(sts, ia);

  return ODD(*sts);
}
