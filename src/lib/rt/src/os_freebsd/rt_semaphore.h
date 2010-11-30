/* 
 * Proview   $Id: rt_semaphore.h,v 1.2 2005-09-01 14:57:57 claes Exp $
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

/* rt_sect.h */

/*-< SEMAPHORE.H >--------------------------------------------------*--------*/
/* POSIX.1b                   Version 1.0        (c) 1998  GARRET   *     ?  */
/* (POSIX.1b implementation for Linux)                              *   /\|  */
/*                                                                  *  /  \  */
/*                          Created:     25-Aug-98    K.A. Knizhnik * / [] \ */
/*                          Last update: 27-Aug-98    K.A. Knizhnik * GARRET */
/*------------------------------------------------------------------*--------*/
/* Semaphore interface                                              *        */
/*------------------------------------------------------------------*--------*/

#ifndef rt_semaphore_h
#define rt_semaphore_h

#ifdef __cplusplus
extern "C" { 
#endif


    typedef struct {
	long  semkey;
	int   semid;
	int   initialized;
    } sem_t;

    
    // Not POSIX. The caller generates the key
    int posix_sem_init_shared(sem_t *sem, int key, unsigned int value);
    
    int posix_sem_init(sem_t *sem, int pshared, unsigned int value);

    sem_t *posix_sem_open(const char *name, int oflag, ...);

    int posix_sem_post(sem_t *sem);

    int posix_sem_getvalue(sem_t *sem, int *sval);

    int posix_sem_wait(sem_t *sem);

    int posix_sem_trywait(sem_t *sem);

    int posix_sem_unlink(const char *name);

    int posix_sem_close(sem_t *sem);

    int posix_sem_destroy(sem_t *sem);


#ifdef __cplusplus
} 
#endif

#endif
