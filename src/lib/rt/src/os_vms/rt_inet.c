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

#ifndef OS_VMS
# error this file is only for OS_VMS
#endif

#include "pwr.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <in.h>
#include <inet.h>
#include <socket.h>
#include <ucx$inetdef.h>
#include <descrip.h>        /* VMS descriptor stuff */
#include <errno.h>          /* Unix style error codes for IO routines. */
#include <iodef.h>          /* I/O FUNCTION CODE DEFS */
#include <lib$routines.h>   /* LIB$ RTL-routine signatures.*/
#include <ssdef.h>          /* SS$_<xyz> sys ser return stati <8-) */
#include <starlet.h>        /* Sys ser calls */
#include "rt_inet.h"

struct iosb_struct {
  short		sts;
  short		count;
  long		dep;
} iosb;

struct itmlist_2 {
  short		length;
  short		code;
  void		*address;
};

struct itmlist_3 {
  short		length;
  short		code;
  void		*address;
  int		*retlen;
};

struct  ioctl_comm {
  int		command;
  void		*address;
};

pwr_tBoolean
inet_SetArpEntry (
  pwr_tStatus	*sts,
  int		sock,
  struct arpreq	*rp  
)
{
  return NO;
}


pwr_tBoolean
inet_DeleteArpEntry (
  pwr_tStatus	*sts,
  int		sock,
  struct arpreq	*rp  
)
{
  return NO;
}

pwr_tBoolean
inet_GetArpEntry (
  pwr_tStatus	*sts,
  int		isock,
  struct arpreq	*rp  
)
{
  struct ipaddr_in	*ipaddr;
  char			*hwaddr;
  pwr_tStatus		lsts;
  static int			retlen;
  static struct  ARPREQDEF	req;             /* ARP request ioctl block */
  static struct  ioctl_comm	command = {SIOCGARP, &req};
  struct  itmlist_3	desc = {sizeof(command), UCX$C_IOCTL, &command, &retlen};
  int			sdc;  /* socket device channel */
  int		sock = 0;

  if (isock == 0) {
    sock = socket(AF_INET, SOCK_DGRAM, 0);
  } else {
    sock = isock;
  }

  memset(&req, 0, sizeof(req));
  memcpy(&req.ARP$T_PA[4], &rp->arp_pa.sa_data, sizeof(rp->arp_pa.sa_data));

  req.ARP$T_PA[0] = AF_INET;
  retlen = sizeof(command);

  sdc = vaxc$get_sdc(sock);
  *sts = sys$qiow(0, sdc, IO$_SENSEMODE, &iosb, 0, 0, 0, 0, 0, 0, 0, &desc);
  if (ODD(*sts)) {
    if (EVEN(iosb.sts)) {
      *sts = iosb.sts;
    } else {
      memcpy(&rp->arp_ha.sa_data, &req.ARP$T_HA[2], sizeof(rp->arp_ha.sa_data));
    }
  }

  if (isock == 0) {
    close(sock);
  }

  return ODD(*sts);
}

