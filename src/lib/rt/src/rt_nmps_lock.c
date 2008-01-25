/* 
 * Proview   $Id: rt_nmps_lock.c,v 1.1 2008-01-25 14:32:44 claes Exp $
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

#include <string.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#include "pwr.h"
#include "rt_sect.h"
#include "rt_nmps_lock.h"

sect_sHead *nmps_locksect = 0;

void nmps_create_lock( pwr_tStatus *sts)
{
  pwr_tBoolean created;

  if ( !nmps_locksect) {
    nmps_locksect = sect_Alloc( sts, &created, 0, sizeof(sect_sMutex), 
				nmps_cName_Lock);
    if ( ODD(*sts) && created)
      sect_InitLock( sts, nmps_locksect, (sect_sMutex *)nmps_locksect->base);
  }
}

void nmps_unlink_lock( pwr_tStatus *sts)
{
  if ( nmps_locksect) {
    if ( shmdt( nmps_locksect) == -1)
      *sts = 0;
    else
      *sts = 1;
  }
}

void nmps_delete_lock( pwr_tStatus *sts)
{
  if ( nmps_locksect) {
    char   segname[128];
    char	 busid[8];
    char   *str = getenv(pwr_dEnvBusId);
    key_t  key;
    int    shm_id;
    struct shmid_ds   ds;

    strncpy( busid, (str ? str : "XXX"), 3);
    busid[3] = '\0';

    sprintf(segname, "%s_%.3s", nmps_cName_Lock, busid);

    key    = ftok(segname, 'P');
    shm_id = shmget(key, 0, 0660);
    if ( shmdt( nmps_locksect->base) == -1)
      printf( "Detach of nmps lock failed\n");
    if ( shmctl(shm_id, IPC_RMID, &ds) == -1)
      printf( "Remove of nmps lock failed\n");
    unlink(segname);
    sect_Free( sts, nmps_locksect);
  }
}
