/* 
 * Proview   $Id: rt_inet.c,v 1.2 2005-09-01 14:57:56 claes Exp $
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
