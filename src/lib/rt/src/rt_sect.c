/* 
 * Proview   $Id: rt_sect.c,v 1.4 2006-06-08 11:58:21 claes Exp $
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

#if	defined(OS_ELN)
# include $vaxelnc
# include $kernelmsg
# include $mutex
# include stdlib
# include descrip
# include string
#elif	defined(OS_VMS)
# pragma builtins
# include <stdlib.h>
# include <descrip.h>
# include <string.h>
# include <secdef.h>
# include <ssdef.h>
# include <libdef.h>
# include <starlet.h>
# include <lib$routines.h>
#elif	defined(OS_LYNX)
# include <errno.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <unistd.h>
# include <sys/mman.h>    // for shared memory
# include <sys/types.h>   // for "mode_t"
# include <sys/stat.h>    // for "S_IRWXU" etc.
# include <semaphore.h>
#elif defined(OS_LINUX)
# include <errno.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <unistd.h>
# include <sys/types.h>   // for "mode_t"
# include <sys/ipc.h>     // for shared memory and semaphores
# include <sys/shm.h>     // for shared memory
# include <sys/sem.h>     // for semaphores
# include <sys/stat.h>    // for "S_IRWXU" etc.
# include <sys/file.h>
#endif

#include "pwr.h"
#include "rt_sect.h"
#include "rt_errh.h"


#if defined(OS_LYNX) || defined(OS_LINUX)
 pwr_tBoolean sect_must_create = 0;
#endif

static void
segName (
  sect_sHead		*shp,
  char			*name
);

/* Construct the name of a section.
   It does not access the database! */

static void
segName (
  sect_sHead		*shp,
  char			*name
)
{
#if defined(OS_ELN) || defined(OS_VMS)
    strncpy(shp->name, name, sizeof(shp->name) - 1);
    shp->name[sizeof(shp->name) - 1] = '\0';

#elif defined(OS_LYNX) || defined(OS_LINUX)

  static int doinit = 1;
  static char buf[4];
  if (doinit) {
    char *str = getenv(pwr_dEnvBusId);
    strncpy(buf, (str ? str : "XXX"), 3);
    buf[3] = '\0';
  }
  sprintf(shp->name, "%.*s_%.3s", (int) sizeof(shp->name) - 4 - 1, name, buf);
#endif
}


/* Map (and optionally create) a shared memory section.
   Name must not be more than 15 characters because of $ASCEFC.
   Returns the virtual address of the newly mapped (created) section.  */

sect_sHead *
sect_Alloc (
  pwr_tStatus		*sts,
  pwr_tBoolean		*created,
  sect_sHead		*shp,
  size_t		size,
  char			*name
)
{
  pwr_tStatus		lsts = 1;
  sect_sHead		*lshp = NULL;

  if (shp == NULL) shp = lshp = (sect_sHead *) calloc(1, sizeof(*lshp));
  if (shp == NULL) pwr_Return(NULL, sts, 2/*SECT__VIRMEM*/);
  
  do {
    if (shp->flags.b.mapped) {
      lsts = 2/*SECT__WASMAPPED*/;
      break;
    }

    shp->base = NULL;
    segName(shp, name);

#if defined(OS_ELN) || defined (OS_VMS)
    shp->namedsc.dsc$w_length	= strlen(name);
    shp->namedsc.dsc$b_dtype	= DSC$K_DTYPE_T;
    shp->namedsc.dsc$b_class	= DSC$K_CLASS_S;
    shp->namedsc.dsc$a_pointer	= shp->name;
#endif

#if defined(OS_ELN)
    ker$create_area(&lsts, &shp->area, &shp->base, size, &shp->namedsc, NULL);
    if (EVEN(lsts)) break;

    shp->flags.b.mapped = 1;
    *created = (lsts != KER$_AREA_EXISTS);
    if (*created) memset(shp->base, 0, size);

#elif defined(OS_VMS)
    {
      pwr_tUInt32	inadr[2];
      pwr_tUInt32	retadr[2];

      inadr[0] = 10000;
      inadr[1] = inadr[0] + size;

      lsts = sys$crmpsc(inadr, retadr, 0, 
			SEC$M_WRT | SEC$M_EXPREG | SEC$M_GBL | SEC$M_PAGFIL,
			&shp->namedsc, NULL, 0, 0,
			(((size + 511) / 512) + 1), 0, 0, 0);

      if (EVEN(lsts)) {
	errh_Error("sect_Alloc, new section: %s, size = %d\n%m", name, size, lsts);
	break;
      }

      shp->base		    = (void *)retadr[0];
      shp->sectadr[0]	    = retadr[0];
      shp->sectadr[1]	    = retadr[1];
      shp->size		    = retadr[1] - retadr[0] + 1;
      shp->flags.b.mapped   = 1;

      pwr_Assert(size <= shp->size);
      *created = (lsts == SS$_CREATED);
      if (*created)
	errh_Info("sect_Alloc, new section: %s, size = %d", name, size);
    }

#elif defined(OS_LYNX)
    {
      int    shm_fd;                                              
      int    shMemFlags = O_RDWR;              
      mode_t shMemMode  = S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP; 

      *created = 0;
      /* This is the only way I know to find out if the memory is created */
      shm_fd = shm_open(shp->name, shMemFlags, shMemMode); 

      if (sect_must_create && shm_fd != -1) { /* Do we need to check errno ? */
       // printf("sect_Alloc: %s already exists. It will be unlinked and created\n", shp->name); 
        if (shm_unlink(shp->name) == -1) {
          lsts = 2;             
	  perror("sect_Alloc: shm_unlink");                
          break;
	}

        shMemFlags |= O_CREAT | O_EXCL; 
        shm_fd = shm_open(shp->name, shMemFlags, shMemMode); 
        if(shm_fd == -1) {
          lsts = 2;             
	  perror("sect_Alloc: shm_open, O_CREATE");                
          break;
	}
	*created = 1;
      }
      else if(shm_fd == -1) {
	if (errno == ENOENT) { /* It didn't exist */
          shMemFlags |= O_CREAT | O_EXCL; 
        shm_fd = shm_open(shp->name, shMemFlags, shMemMode); 

          if(shm_fd == -1) {
            lsts = 2;             
	    perror("sect_Alloc: shm_open, O_CREATE ");                
            break;
	  } else
	    *created = 1;
	} else {
          lsts = 2;             
	  perror("sect_Alloc: shm_open ");                
          break;
	}
      } 
      
      /* Set size */
      if (ftruncate(shm_fd, size) != 0) {
	close(shm_fd);
	if (*created)
	  shm_unlink(shp->name);
        lsts = 2;
	perror("sect_Alloc: ftruncate ");                
	break;
      }

      shp->base = mmap(0, size, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
      if (shp->base == MAP_FAILED) {
	close(shm_fd);
	if (*created)
	  shm_unlink(shp->name);
        lsts = 2;
	perror("sect_Alloc: mmap ");                
        break;
      }
     
      close(shm_fd);

      shp->size		    = size;
      shp->flags.b.mapped   = 1;
      lsts = 1; 
    }
#elif defined(OS_LINUX)
    {
      int    shm_fd;                                              
      int    shMemFlags = O_RDWR;              
      mode_t shMemMode  = S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP; 
      key_t  shm_key;
      int    shm_id;

      *created = 0;
      /* Create unique key */
      shm_fd = open(shp->name, shMemFlags, shMemMode); 

      if (sect_must_create && shm_fd != -1) { /* Do we need to check errno ? */
        errh_Info("sect_Alloc: %s already exists. It will be unlinked and created\n", shp->name); 
        if (unlink(shp->name) == -1) {
          lsts = 2;             
          errh_Error("sect_Alloc: unlink failed on !%s! errno: %d", shp->name, errno);
          break;
        }
        shMemFlags |= O_CREAT | O_EXCL; 
        shm_fd = open(shp->name, shMemFlags, shMemMode); 
        if(shm_fd == -1) {
          lsts = 2;             
          errh_Error("sect_Alloc: open O_CREATE failed errno: %d", errno);
          break;
        }
        *created = 1;
      } else if(shm_fd == -1) {
        if (errno == ENOENT) { /* It didn't exist */
          shMemFlags |= O_CREAT | O_EXCL; 
          shm_fd = open(shp->name, shMemFlags, shMemMode); 
          if(shm_fd == -1) {
            lsts = 2;             
            errh_Error("sect_Alloc: open O_CREATE | O_EXCL failed errno: %d",
                      errno);
            break;
          } else {
            *created = 1;
          }
        } else {
          lsts = 2;             
          errh_Error("sect_Alloc: open failed errno: %d", errno);
          break;
        }
      } 

      /* Get shm key */
      shm_key = ftok(shp->name, 'P');
      close(shm_fd);

      /* Create shm section */
      shm_id = shmget(shm_key, size, IPC_CREAT | 0660);
      if (shm_id == -1) {
        lsts = 2;
        errh_Error("sect_Alloc: Couldn't attach/create shm section");
        break;
      }

      shp->base = shmat(shm_id, (void *)NULL, 0);
      if (shp->base == (void *)-1) {
        if (*created)
          unlink(shp->name);
        lsts = 2;
        errh_Error("sect_Alloc: shmat failed");
        break;
      }
     
      shp->size		    = size;
      shp->flags.b.mapped   = 1;
      lsts = 1; 
    }
#endif

    if (*created) errh_Info("Mapped %s, base: %x, size: %d", shp->name, shp->base, shp->size);

    pwr_Return(shp, sts, lsts);
  } while (0);

  /* Something went wrong, clean up!  */

  if (lshp != NULL) free(lshp);
  pwr_Return(NULL, sts, lsts);
}

/* Unmap a shared memory section.  */

pwr_tBoolean
sect_Free (
  pwr_tStatus		*sts,
  sect_sHead		*shp
)
{
  pwr_tStatus		lsts;

  if (!shp->flags.b.mapped)
    pwr_Return(FALSE, sts, 4/*SECT__NOTMAPPED*/);

#if defined(OS_ELN)
  ker$delete(&lsts, shp->area);
#elif defined(OS_VMS)
  lsts = sys$deltva(shp->sectadr, NULL, 0);
#endif

  if (ODD(lsts))
    shp->flags.b.mapped = 0;

  pwr_Return(ODD(lsts), sts, lsts);
}

/* Initialize a shared memory lock.  */

pwr_tBoolean
sect_InitLock (
  pwr_tStatus		*sts,
  sect_sHead		*shp,
  sect_sMutex		*mp
)
{
  pwr_tStatus		lsts = 1;

#ifdef	OS_ELN
  ELN$INITIALIZE_AREA_LOCK((AREA)shp->area, *mp, &lsts);

#elif defined(OS_VMS)
  {
    pwr_tUInt32		i;

    /* Clear mutex variable and build free list.  */

    memset(mp, 0, sizeof(*mp));
    lsts = sys$ascefc(64, &shp->namedsc, 0, 0);
    if (ODD(lsts)) {
      for (i=0; i<32; i++) {
	while (_INSQTI(&mp->list[i].entry, &mp->freeroot) == 1);
	mp->list[i].flag = 64+i;
	lsts = sys$clref(mp->list[i].flag);
	if (EVEN(lsts)) break;
      }
    }
  }

#elif defined(OS_LYNX)
  if (sem_init(mp, 1, 1) != 0) {
    errh_Error("sect_InitLock: sem_init, errno: %d", errno);
    lsts = 2;
  }
#elif defined(OS_LINUX)
  if (posix_sem_init_shared(mp, ftok(shp->name, 'P'), 1) != 0) {
    errh_Error("sect_InitLock: sem_init(%s), errno: %d", shp->name, errno);
    lsts = 2;
  }  
#endif

  errh_ReturnOrBugcheck(ODD(lsts), sts, lsts, "");
}

/* Lock a shared memory section.  */

pwr_tBoolean
sect_Lock (
  pwr_tStatus		*sts,
  sect_sHead		*shp,
  sect_sMutex		*mp
)
{
  pwr_tStatus		lsts = 1;

#if defined(OS_ELN)
  ELN$LOCK_AREA((AREA)shp->area, *mp);
#elif defined(OS_VMS)
  {
    sect_sMutexEntry	*ep;
    int			i;

    /* Get free list entry.  */

    while ( 1) {
      i = _REMQHI(&mp->freeroot, (void **)&ep);
      if ( i == 1)
        continue;
      else if (i == 3) 
        sleep(1);
      else
        break;
    }

    /* Insert in ownership list.  */

    while ((i = _INSQTI(ep, &mp->ownerroot)) == 1);
    if (i == 0) {

      /* Contention, we must wait for the resource.  */

      lsts = sys$ascefc(64, &shp->namedsc, 0, 0);
      if (ODD(lsts)) lsts = sys$waitfr(ep->flag);
      if (ODD(lsts)) lsts = sys$clref(ep->flag);
    }
  }
#elif defined(OS_LYNX)
  while (sem_wait(mp) != 0) {
    if (errno != EINTR) {
      perror("sect_Lock: sem_wait ");
      lsts = 2;
      break;
    }
  }
#elif defined(OS_LINUX)
  while (posix_sem_wait(mp) != 0) {
    if (errno != EINTR) {
      perror("sect_Lock: sem_wait ");
      lsts = 2;
      break;
    }
  }
#endif

  errh_ReturnOrBugcheck(ODD(lsts), sts, lsts, "");
}

/* Unlock a shared memory section.  */

pwr_tBoolean
sect_Unlock (
  pwr_tStatus		*sts,
  sect_sHead		*shp,
  sect_sMutex		*mp
)
{
  pwr_tStatus		lsts = 1;

#if defined(OS_ELN)
  ELN$UNLOCK_AREA((AREA)shp->area, *mp);
#elif defined(OS_VMS)
  {
    sect_sMutexEntry	*ep;
    pwr_tUInt32		i;

    /* When the entry is removed the resource becomes
       free if noone else is waiting...  */

    while ((i = _REMQHI(&mp->ownerroot, (void **)&ep)) == 1);
    if (i == 3) {
      lsts = LIB$_QUEWASEMP;
    } else {

      /* Reinsert entry in free list */

      while (_INSQTI(ep, &mp->freeroot) == 1);

      if (i == 0) {
	/* Someone was waiting.  */

	ep = (sect_sMutexEntry *)(
	  ((pwr_tUInt32)&mp->ownerroot) +
	  mp->ownerroot.low /* flink */);
	lsts = sys$ascefc(64, &shp->namedsc, 0, 0);
	if (ODD(lsts)) lsts = sys$setef(ep->flag);
      }
    }
  }
#elif defined(OS_LYNX)
  if (sem_post(mp) != 0) {
    perror("sect_Unlock: sem_signal ");
    lsts = 2;
  }
#elif defined(OS_LINUX)
  if (posix_sem_post(mp) != 0) {
    perror("sect_Unlock: sem_signal ");
    lsts = 2;
  }
#endif

  errh_ReturnOrBugcheck(ODD(lsts), sts, lsts, "");

}

