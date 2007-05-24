/* 
 * Proview   $Id: co_syi.c,v 1.1 2007-05-24 14:48:38 claes Exp $
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

/* co_syi.c -- System information
   This module gives information about the system.  */

#include $kerneldef
#include $kernelmsg
#include $vaxelnc
#include $ni_utility

#include stdio
#include string
#include "pwr.h"
#include "co_syi.h"
#include "co_syi_msg.h"

typedef struct {
  short	len;
  char	name[6];
} nodedesc;


char *
syi_HostName (
  pwr_tStatus *status,
  char *ibuffer,
  int isize
)
{
  static char buffer[32];
  char *name;
  int size;
  int len;
  globalref nodedesc ker$gt_host_name;

  pwr_dStatus(sts, status, SYI__SUCCESS);

  if (ibuffer != NULL && isize > 0) {
    name = ibuffer;
    size = isize;
  } else {
    name = buffer;
    size = sizeof(buffer);
  }

  len = MIN(ker$gt_host_name.len, size-1);

  memcpy(name, &ker$gt_host_name.name, len);
  name[len] = '\0';

  return name;
}

char *
syi_NodeName (
  pwr_tStatus *status,
  char *ibuffer,
  int isize
)
{
  static char buffer[32];
  char *name;
  int size;
  int len;
  globalref nodedesc ker$gt_node_name;

  pwr_dStatus(sts, status, SYI__SUCCESS);

  if (ibuffer != NULL && isize > 0) {
    name = ibuffer;
    size = isize;
  } else {
    name = buffer;
    size = sizeof(buffer);
  }

  len = MIN(ker$gt_node_name.len, size-1);

  memcpy(name, &ker$gt_node_name.name, len);
  name[len] = '\0';

  return name;
}

char *
syi_Ethernet (
  pwr_tStatus *status,
  char *ibuffer,
  int size
)
{
  struct eln$ni_attributes *ap;
  struct eln$ni_configuration cfg;
  unsigned int count;
  int len;
  static char buffer[] = "xx-xx-xx-xx-xx-xx";

  pwr_dStatus(sts, status, SYI__SUCCESS);

  eln$ni_get_configuration(sts, &count, &cfg);
  if (EVEN(sts)) return NULL;

  eln$ni_get_attributes(sts, cfg.clist.list[0].control_port, &ap);
  if (EVEN(sts)) return NULL;

  len = sprintf(buffer,"%02X-%02X-%02X-%02X-%02X-%02X",
    (long)ap->hardware_address.address[0],
    (long)ap->hardware_address.address[1],
    (long)ap->hardware_address.address[2],
    (long)ap->hardware_address.address[3],
    (long)ap->hardware_address.address[4],
    (long)ap->hardware_address.address[5]);

  if (size >= len) {
    memcpy(ibuffer, buffer, len);
    return ibuffer;
  } else
    return buffer;
}

char *
syi_DECnet (
  pwr_tStatus *status,
  char *ibuffer,
  int size
)
{
  struct eln$ni_attributes *ap;
  struct eln$ni_configuration cfg;
  unsigned int count;
  static char buffer[] = "65535..";
  int len;

  pwr_dStatus(sts, status, SYI__SUCCESS);

  eln$ni_get_configuration(&sts, &count, &cfg);
  if (EVEN(sts)) return NULL;

  eln$ni_get_attributes(&sts, cfg.clist.list[0].control_port, &ap);
  if (EVEN(sts)) return NULL;

  len = sprintf(buffer,"%d",
	((short)ap->physical_address.address[5]/4) * 1024 +
	(short)ap->physical_address.address[4]);

  if (size >= len) {
    memcpy(ibuffer, buffer, len);
    return ibuffer;
  } else
    return buffer;
}

char *
syi_DECnetHost (
  pwr_tStatus *status,
  char *ibuffer,
  int size
)
{
  globalref unsigned short ker$gq_host_address [3];
  static char buffer[] = "65535..";
  int len;

  pwr_dStatus(sts, status, SYI__SUCCESS);

  len = sprintf(buffer, "%d", ker$gq_host_address[2]);

  if (size >= len) {
    memcpy(ibuffer, buffer, len);
    return ibuffer;
  } else
    return buffer;
}

/* Return true if node is booted locally or
   false if booted from remote node via network.  */

pwr_tBoolean
syi_LocalBoot (
  pwr_tStatus *status
)
{
  globalref unsigned short ker$gq_host_address[3];

  pwr_dStatus(sts, status, SYI__SUCCESS);

  return ((ker$gq_host_address[2]>>10) & 63) == 0;
}

/* .  */

char*
syi_BootDisk (
  pwr_tStatus *status,
  char *ibuffer,
  int size
)
{
  globalref struct scr *ker$ga_device_list;
  struct scr *sp;
  unsigned int tmp;	
  static char buffer[sizeof("DUA900:")] = "";
  int len;
  FILE *fp = NULL;
  char fname[80];
  int i;

  pwr_dStatus(sts, status, SYI__SUCCESS);

  if (!syi_LocalBoot(sts))
    return NULL;

  /* Loop over all devices.  */
  for (
    sp = (struct scr *)((int)ker$ga_device_list + (int)(&ker$ga_device_list)), len = 0;
    sp != NULL && len == 0;
    sp = (struct scr *)((int)sp + (int)sp->scr$l_next)
  ) {
    if (strncmp(sp->scr$t_name, "DUA", 3))
      continue;

    /* Look for SYSBOOT.EXE on DUA0 - DUA9.
       We assume that the first disk found whith
       [SYS0.SYSEXE]SYSBOOT.EXE present is the boot disk.  */

    for (i = 0; i < 9; i++) {
      sprintf(fname, "DUA%d:[SYS0.SYSEXE]SYSBOOT.EXE", i);
      fp = fopen(fname,"r");
      if (fp) {
        len = sprintf(buffer, "DUA%d:", i);
	break;
      }
    }

    if (fp) {
      fclose(fp);
      break;
    }
  }

  if (size >= len) {
    memcpy(ibuffer, buffer, len);
    return ibuffer;
  } else
    return buffer;
}

char *
syi_NodeSpec (
  pwr_tStatus *status,
  char *ibuffer,
  int isize
)
{
  globalref unsigned short ker$gq_node_address [3];
  static char buffer[] = "65535..";
  int len;

  pwr_dStatus(sts, status, SYI__NYI);

  len = sprintf(buffer,"%d", ker$gq_node_address[2]);

  if (isize >= len) {
    memcpy(ibuffer, buffer, len);
    return ibuffer;
  } else {
    return buffer;
  }
}

char *
syi_HostSpec (
  pwr_tStatus *status,
  char *ibuffer,
  int isize
)
{
  globalref unsigned short ker$gq_host_address [3];
  static char buffer[] = "65535..";
  int len;

  pwr_dStatus(sts, status, SYI__SUCCESS);

  len = sprintf(buffer,"%d", ker$gq_host_address[2]);

  if (isize >= len) {
    memcpy(ibuffer, buffer, len);
    return ibuffer;
  } else {
    return buffer;
  }
}
