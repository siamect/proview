/* 
 * Proview   $Id: rt_sect.c,v 1.2 2005-09-01 14:57:57 claes Exp $
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

/* rt_sect.c -- <short description>
*/

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <semaphore.h>

void *
mapit (
  char	  *poolName,
  int	  memorySize,
  char	  fill
)
{
  int	      created  = 0;
  void	      *myData = NULL;
  int	      sts = 0;

  size_t storedSize = 0;
  int    shm_fd;
  int    shMemFlags = O_CREAT | O_EXCL | O_RDWR;
  mode_t shMemMode  = S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP;

  sts = shm_unlink(poolName);
  perror(" Error : "); 
  if (memorySize == 0) return NULL;

  shm_fd = shm_open(poolName, shMemFlags, shMemMode);

  if(shm_fd == -1)
  {
    perror(" Error : "); 
    return NULL;
  }

  sts = ftruncate(shm_fd, memorySize);
  if(sts < 0)
  {
    perror(" Error :");
    return NULL;
  }

  myData = mmap(0, memorySize, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
  printf("myData = %x\n", myData);
  if(myData == MAP_FAILED)
  {
     perror(" Error : "); 
     return NULL;
  }

  sts = close(shm_fd);
  memset(myData, fill, memorySize);

  if (created) {
    sts = munmap(myData,memorySize);
    if(sts == -1)
    {
       perror(" Error : "); 
       return NULL;
    }

    shm_fd = shm_open(poolName, O_RDWR, shMemMode);
    if(shm_fd == -1)
    {
      perror(" Error : "); 
      return NULL;
    }
    myData = mmap(0, memorySize, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
    printf("myData = %x\n", myData);
    if(myData == MAP_FAILED)
    {
       perror(" Error : "); 
       return NULL;
    }
  }
#if 0
  sts = munmap(myData,myData->totalSize);
#endif

  sts = close(shm_fd);

  return myData;
}

main (int argc, char **argv)
{
  int		i;
  void		*addr;
  char		a = 'a';
  char		s[80];

  for (i = 0; i < 10; i++) {
    sprintf(s, "/Lasse%x", i);
    addr = mapit(s, i * 10000, a++);
    printf("Addr: %x\n", addr);
  }
}
