/* rt_sect.c -- <short description>

   PROVIEW/R
   Copyright (C) 1996 by Comator Process AB.

   <Description>.  */

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
