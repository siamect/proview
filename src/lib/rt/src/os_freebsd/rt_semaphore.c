/* 
 * Proview   $Id: rt_semaphore.c,v 1.4 2006-02-01 09:12:17 claes Exp $
 * Copyright (C) 2005 SSAB Oxelösund AB.
 *
 * Distribution terms
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the Software), to deal in the Software without
 * restriction, including without limitation the rights to use,
 * copy, modify, merge, publish, distribute, sublicense, and/or
 * sell copies of the Software, and to permit persons to whom
 * the Software is furnished to do so, subject to the following
 * conditions:

 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, 
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES 
 * OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHOR OF THIS SOFTWARE BE LIABLE FOR ANY
 * CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF
 * CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

/*-< SEMAPHORE.C >--------------------------------------------------*--------*/
/* POSIX.1b                   Version 1.0        (c) 1998  GARRET   *     ?  */
/* (POSIX.1b implementation for Linux)                              *   /\|  */
/*                                                                  *  /  \  */
/*                          Created:     25-Aug-98    K.A. Knizhnik * / [] \ */
/*                          Last update: 27-Aug-98    K.A. Knizhnik * GARRET */
/*------------------------------------------------------------------*--------*/
/* Semaphore implementation                                         *        */
/*------------------------------------------------------------------*--------*/




#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <stdarg.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>

#include "rt_semaphore.h"


#if 0
union semun {
   int val;
   struct semid_ds *buf;
   u_short *array;
};
#endif



sem_t *posix_sem_open(const char *name, int oflag, ...)
{
   key_t key = IPC_PRIVATE;
   int semid, mode = 0;
   struct sembuf sops[2];
   sem_t* s;

   if (name != NULL) {
      int fd = open(name, O_WRONLY|O_CREAT, 0777);
      if (fd < 0) {
         return(sem_t*)-1;
      }
      close(fd);
      key = ftok(name, 'P');
      if (key < 0) {
         return(sem_t*)-1;
      }
   }

   if (oflag & O_CREAT) {
      int init_value;
      va_list ap;
      va_start(ap, oflag);
      mode = va_arg(ap, int);
      init_value = va_arg(ap, unsigned int); 
      if (init_value < 0) {
         errno = EINVAL;
         return(sem_t*)-1;
      }
      va_end(ap);
      sops[0].sem_num = 1;
      sops[0].sem_op  = 1; /* mark sempahore as initialuzed */
      sops[0].sem_flg = 0;
      sops[1].sem_num = 0;
      sops[1].sem_op  = init_value;
      sops[1].sem_flg = 0;
      mode |= IPC_CREAT;
   } else {
      sops[0].sem_num = 1;
      sops[0].sem_op  = -1; /* wait until semaphore is initialized */
      sops[0].sem_flg = 0;
      sops[1].sem_num = 1;
      sops[1].sem_op  = 1; /* restore initialized flag */
      sops[1].sem_flg = 0;
   } 
   if (oflag & O_EXCL) {
      mode |= IPC_EXCL;
   }
   semid = semget(key, 2, mode);
   if (semid < 0) {
      return(sem_t*)-1;
   }
   if (semop(semid, sops, 2) != 0) {
      return(sem_t*)-1;
   }
   s = (sem_t*)malloc(sizeof(sem_t));
   s->semid = semid;
   s->initialized = 1;
   s->semkey = key;
   return s;
}

int posix_sem_init_shared(sem_t *sem, int key, unsigned int value)
{
   int semid;
   if (value < 0) {
      errno = EINVAL;
      return -1;
   }
   sem->semkey = key;
   sem->initialized = 0;
   sem->semid = -1;
   
   semid = semget(sem->semkey, 1, IPC_CREAT|IPC_EXCL|0777);
   if (semid < 0) {
      return -1;
   }
   sem->initialized = 1;
   sem->semid = semid;
   
   if (value != 0) {
      struct sembuf sops[1];
      sops[0].sem_num = 0;
      sops[0].sem_op  = value;
      sops[0].sem_flg = 0;
      if (semop(semid, sops, 1) != 0) {
         return -1;
      }
   }

   return 0;
}

int posix_sem_init(sem_t *sem, int pshared, unsigned int value)
{
   int semid;
   if (value < 0) {
      errno = EINVAL;
      return -1;
   }
   sem->semkey = pshared ? (long)sem : IPC_PRIVATE;
   sem->initialized = 0;
   sem->semid = -1;
   
   semid = semget(sem->semkey, 1, IPC_CREAT|0777);
   if (semid < 0) {
      return -1;
   }
   sem->initialized = 1; // Initialize, so we don't need the hash table. ML
   sem->semid = semid;
   
   if (value != 0) {
      struct sembuf sops[1];
      sops[0].sem_num = 0;
      sops[0].sem_op  = value;
      sops[0].sem_flg = 0;
      if (semop(semid, sops, 1) != 0) {
         return -1;
      }
   }

   return 0;
}


int posix_sem_post(sem_t *sem)
{
   static struct sembuf sops[] = {{0, 1, SEM_UNDO}};
   return semop(sem->semid, sops, 1);
}

int posix_sem_getvalue(sem_t *sem, int *sval)
{
   int result;

   if (sval == NULL) {
      errno = EINVAL;
      return -1;
   }
   result = semctl(sem->semid, 0, GETVAL, (union semun)0);
   if (result == -1) {
      return -1;
   }
   *sval = result;
   return 0;
}

int posix_sem_wait(sem_t *sem)
{
   static struct sembuf sops[] = {{0, -1, SEM_UNDO}};
   return semop(sem->semid, sops, 1);
}

int posix_sem_trywait(sem_t *sem)
{
   static struct sembuf sops[] = {{0, -1, SEM_UNDO|IPC_NOWAIT}};
   return semop(sem->semid, sops, 1);
}


int posix_sem_unlink(const char *name)
{
   int key, semid;
   key = ftok(name, 'P');
   if (key < 0) {
      return -1;
   }
   semid = semget(key, 1, 0);
   if (semid < 0) {
      return -1;
   }
   unlink(name);
   return semctl(semid, 0, IPC_RMID, (union semun)0);
}


int posix_sem_close(sem_t *sem)
{
   free(sem);
   return 0;
}

int posix_sem_destroy(sem_t* sem) 
{
   return semctl(sem->semid, 0, IPC_RMID, (union semun)0);
}
