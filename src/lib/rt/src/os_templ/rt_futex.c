/* 
 * Proview   $Id: rt_futex.c,v 1.2 2005-09-01 14:57:57 claes Exp $
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

/* rt_futex.c -- Futex operations

   PROVIEW/R
   Contains functions that are heavily os-dependant.  
   
   Author: Robert Karlsson 21 Apr 2004 
   
   Description:
   This module provides an interface to futexes - "fast user level
   locking in Linux". This is achieved through the multiplexing
   system call sys_futex(). As implemented below this interface provides
   a synchronization mechanism that can be used both between threads
   in one process as well as between threads in different processes */

#if !defined(OS_LINUX)
# error "This file is valid only for OS_LINUX"
#endif

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/syscall.h>
#include <unistd.h>
#define FUTEX_WAIT (0)
#define FUTEX_WAKE (1)
#define FUTEX_FD (2)
#define FUTEX_REQUEUE (3)


int 
futex_wait (
      int *futex, 
      int val
)
{
  int  ok;
  ok = syscall(SYS_futex, futex, FUTEX_WAIT, val, NULL);
  if (ok == -1) {
    return errno;
  }
  else {
    return ok;
  }
}

int 
futex_timed_wait (
      int *futex, 
      int val, 
      const struct timespec * timespec
)
{
  int  ok;
  ok = syscall(SYS_futex, futex, FUTEX_WAIT, val, timespec);
  if (ok == -1) {
    return errno;
  }
  else {
    return ok;
  }
}

int 
futex_wake (
      int *futex,
      int nr
)
{
  int  ok;
  ok = syscall(SYS_futex, futex, FUTEX_WAKE, nr, NULL);
  if (ok == -1) {
    return errno;
  }
  else {
    return ok;
  }
}
