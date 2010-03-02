/* 
 * Proview   $Id: rt_pool.c,v 1.3 2005-12-13 15:14:27 claes Exp $
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

/* rt_pool.c

   This module implements the dynamic pool. The intent with such
   a pool is to minimize the risk of having a pool area overflowing.
   This is done by splitting the pool in a number of segments, currently
   up to 256. When that number of segments is reached, the pool
   overflows. The package can pretty easily be redesigned to be even
   more dynamic, i.e. adding a dynamic number of segments. This, however,
   costs CPU-time and implementation effort, and is considered as an
   `overkill' currently.  */

#ifdef	OS_ELN
# include $vaxelnc
# include $kernelmsg
# include stdio
# include stdlib
# include string
#else
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
#endif

#ifdef OS_VMS
# include <lib$routines.h>
#endif

#include "pwr.h"
#include "rt_pool_msg.h"
#include "rt_errh.h"
#include "rt_sect.h"
#include "rt_pool.h"

/* Convert bytesize to size expressed in pool_sEntry units.  */
#define pool_Align(offs) (((offs) + (pool_cDataSize-1)) & ~(pool_cDataSize-1))
#define entryUnits(size) (pool_Align(size)/pool_cDataSize)
#define entryPAdd(ep,offs) ((pool_sEntry *)((char *)ep + offs))

#define cEntryMark (4711)

static void *
allocLookaside (
  pwr_tStatus		*sts,
  pool_sHead		*php,
  pool_sList		*lp
);

static char *
segName (
  char			workstr[16],
  char			*name,
  pwr_tUInt32		generation
);

static pool_sSegment *
mapSegment (
  pwr_tStatus		*sts,
  pool_sHead		*php,
  pool_sSegment		*psp
);

static pool_sSegment *
ensureMapped (
  pwr_tStatus		*sts,
  pool_sHead		*php,
  pool_sSegment		*psp
);

static pwr_tBoolean
freeItem (
  pwr_tStatus		*sts,
  pool_sHead		*php,
  pool_sEntry		*ep
);

static pool_sSegment *
newSegment (
  pwr_tStatus		*sts,
  pool_sHead		*php,
  size_t		size		/* Requested size in pool_sEntry units */
);

static pwr_tBoolean
checkQ	(
  pool_sHead		*php,
  pool_sQlink		*item
);

static pwr_tBoolean
checkInitQ (
  pool_sHead		*php,
  pool_sQlink		*item
);


static void *
allocLookaside (
  pwr_tStatus		*sts,
  pool_sHead		*php,
  pool_sList		*lp
)
{
  pool_sEntry		*ep;  
  pool_uRefBits		pr;
  pool_sSegment		*psp = &php->seg[lp->seg];
  
  
  ep = entryPAdd(psp->base, lp->next * pool_cDataSize);
  lp->next = ep->next;
  
  pr.m = pool_cNRef;
  pr.b.seg = psp->seg;
  pr.b.offs = (pool_tOffset)((char *)ep + sizeof(pool_sEntry) - (char *)psp->base);
  pwr_Assert(entryUnits(lp->size + sizeof(pool_sEntry)) == ep->size);
  ep->next = pr.m;
  psp->gpsp->alloccnt++;

  memset( entryPAdd( ep, sizeof(pool_sEntry)), 0, ep->size*pool_cDataSize - sizeof(pool_sEntry));
  return (void *)entryPAdd( ep, sizeof(pool_sEntry));
}

static pwr_tBoolean
checkQ	(
  pool_sHead		*php,
  pool_sQlink		*item
)
{
  pool_sQlink		*link;

  if (item == NULL) {
    errh_Error("checkQ in pool: %s, item == NULL", php->gphp->name);
    return NO;
  }
  if (item->self == pool_cNRef) {
    errh_Error("checkQ in pool: %s, item->self == pool_cNRef, item: %u", php->gphp->name, item);
    return NO;
  }

  link = pool_Address(NULL, php, item->self);
  if (item != link) {
    errh_Error("checkQ in pool: %s, item != pool_Address(NULL, php, item->self),\n item: %u != %u",
      php->gphp->name, item, link);
    return NO;
  }

  return YES;
#if 0
  return item == pool_Address(NULL, php, item->self);
#endif
}

static pwr_tBoolean
checkInitQ (
  pool_sHead		*php,
  pool_sQlink		*item
)
{

  if (item->self == pool_cNRef) {
    item->self = item->flink = item->blink = pool_Reference(NULL, php, item);
    return (item->self != pool_cNRef);
  } else {
    return (item == pool_Address(NULL, php, item->self)) &&
	    (item->self == item->flink) &&
	    (item->self == item->blink);
  }
}

static pool_sSegment *
ensureMapped (
  pwr_tStatus		*sts,
  pool_sHead		*php,
  pool_sSegment		*psp
)
{
  if (psp->generation == psp->gpsp->generation) 
    return psp;

  if (psp->generation != 0)
    errh_ReturnOrBugcheck(NULL, sts, POOL__REMAPNYI, "");

  return mapSegment(sts, php, psp);
}

/* Free a piece of memory in a given pool.
   Raise an exception if anything goes wrong.  */

static pwr_tBoolean
freeItem (
  pwr_tStatus		*sts,
  pool_sHead		*php,
  pool_sEntry		*ep
)
{
  pwr_tStatus		lsts;
  pool_sGhead		*gphp;
  pool_sGsegment	*gpsp;
  pool_sSegment		*psp;
  pool_sEntry		*prevp;
  pool_sEntry		*tmpp;
  pool_tOffset		offs;
  pwr_tUInt32		seg;
  pool_uRefBits		pr;
  pool_sList		*lp;

  pr.m = ep->next;

  /* Fill returned entry with with the bit pattern '01010101'...  */
  memset( entryPAdd(ep, sizeof(pool_sEntry)), 85, ep->size*pool_cDataSize - sizeof(pool_sEntry));

  /* Setup pointers */

  gphp = php->gphp;
  seg = pr.b.seg;
  offs = entryUnits(pr.b.offs - sizeof(pool_sEntry));
  psp = &php->seg[seg];
  gpsp = &gphp->seg[seg];

  psp = ensureMapped(&lsts, php, psp);
  if (psp == NULL)
    errh_ReturnOrBugcheck(NO, sts, lsts, "");

  switch (gpsp->type) {
  case pool_eSegType_lookaside:
    lp = &gphp->la[gpsp->la_idx];
    ep->next = lp->next;
    lp->next = offs;
    gpsp->alloccnt--;
    return YES;
    break;
  case pool_eSegType_named:
    errh_ReturnOrBugcheck(NO, sts, POOL__FREENAMED, "");
    break;
  default:
    break;
  }


  /* Find where the offs belongs in free list */

  prevp = &gpsp->freeroot;
  while (prevp->next != pool_cNOffset) {
    if (offs < prevp->next) break;		/* found */
    prevp = (pool_sEntry *)((char *)psp->base + prevp->next * pool_cDataSize); /* try next */
  } /* While more free entries */

  /* Here prevp points to the entry after which insertion should be done
     NOTE. Prevp can point to the header or to the last fragment!  */

  ep->next = prevp->next;
  prevp->next = ((char *)ep - (char *)psp->base)/pool_cDataSize;
  gpsp->fragcnt++;
  gpsp->fragsize += ep->size;
  gpsp->alloccnt--;
  if (ep->size >= gpsp->fragmax) {
    if (ep->size == gpsp->fragmax) gpsp->fragmaxcnt++;
    else {
      gpsp->fragmax = ep->size;
      gpsp->fragmaxcnt = 1;
    }
  }

  /* Join with succeeding/preceeding fragment? */

  do {
    tmpp = (pool_sEntry *)((char *)psp->base + prevp->next * pool_cDataSize);

    if (entryPAdd(prevp, prevp->size * pool_cDataSize) == tmpp) {
      prevp->size += tmpp->size;
      prevp->next = tmpp->next;
      /* Fill recovered list info with the bit pattern '01010101'...  */
      memset(tmpp, 85, sizeof(*tmpp));

      gpsp->fragcnt--;
      if (prevp->size >= gpsp->fragmax) {
	if (prevp->size == gpsp->fragmax) gpsp->fragmaxcnt++;
	else {
	  gpsp->fragmax = prevp->size;
	  gpsp->fragmaxcnt = 1;
	} /* if new fragmax found */
      } /* if fragmax is affected */
    } /* if adjacent */
    else prevp = tmpp;

  } while ((prevp->next != pool_cNOffset) && (prevp <= ep));

  return YES;
}

/* Map a previously created segment.

   The routine returns status.
   The routine does not require any pool locking.  */

static pool_sSegment *
mapSegment (
  pwr_tStatus		*sts,
  pool_sHead		*php,
  pool_sSegment		*psp
)
{
  pwr_tStatus		lsts;
  sect_sHead		*shp;
  pwr_tBoolean		created;
  char			name[16];
  pool_sGhead		*gphp = php->gphp;

  /* Map the section */

  segName(name, gphp->name, psp->gpsp->generation);

  shp = sect_Alloc(&lsts, &created, &psp->sect, psp->gpsp->size << pool_cOffsGranul, name);
  if (shp == NULL) errh_ReturnOrBugcheck(NULL, sts, lsts, "");

  if (created) errh_Bugcheck(POOL__NOTCREATED, "pool was not created"); 

  psp->generation = psp->gpsp->generation;
  psp->base = psp->sect.base;

  return psp;
}

/* Create and map a nonexisting segment.

   The routine returns status.
   The pool must be locked before calling this routine.  */

static pool_sSegment *
newSegment (
  pwr_tStatus		*sts,
  pool_sHead		*php,
  size_t		size		/* Requested size in pool_sEntry units */
)
{
  pool_sGhead		*gphp;
  pool_sGsegment	*gpsp;
  pool_sSegment		*psp;
  sect_sHead		*shp;
  pwr_tBoolean		created;
  pwr_tUInt32		i;
  char			name[16];

  if (size << pool_cOffsAlign > pool_cMaxSize)
    pwr_Return(NULL, sts, POOL__TOOBIG);
    
  gphp = php->gphp;

  /* Find an empty slot */

  for (i = 0; i < pool_cSegs; i++)
    if (gphp->seg[i].generation == 0)
      break;

  if (i >= pool_cSegs)
    pwr_Return(NULL, sts, POOL__NOSLOT);

  gpsp = &gphp->seg[i];
  psp = &php->seg[i];

  /* Allocate the section */

  segName(name, gphp->name, gphp->generation);

  shp = sect_Alloc(sts, &created, &psp->sect, size << pool_cOffsAlign, name);
  if (shp == NULL) return NULL;
  if (!created) errh_Bugcheck(POOL__ALLREXIST, ""); 

  gpsp->type = pool_eSegType_dynamic;
  gpsp->size = size << (pool_cOffsAlign - pool_cOffsGranul);
  gpsp->fragmax = gpsp->fragsize = size;
  gpsp->generation = psp->generation = gphp->generation++;
  gpsp->alloccnt = 0;
  gpsp->fragcnt = gpsp->fragmaxcnt = 1;
  psp->base = psp->sect.base;
  psp->base->size = size;
  psp->base->next = pool_cNOffset;
  gpsp->freeroot.next = 0;

  return psp;
}

/* Construct the name of a section.
   It does not access the database!

   The routine returns the workstr, filled in.  */

static char *
segName (
  char			workstr[16],
  char			*name,
  pwr_tUInt32		generation
)
{
  sprintf(workstr, "%.11s%4.4x", name, generation);
  return workstr;
}

/* Return the virtual address corresponding to a pool
   offset.
   Signals errors.
   The routine resolves a pool reference the full way.  */

void *
pool_Address (
  pwr_tStatus		*sts,
  pool_sHead		*php,
  pool_tRef		r
)
{
  pwr_tStatus		lsts;
  pool_uRefBits		prf;
  pool_sGsegment	*gpsp;
  pool_sSegment		*psp;

  if (r == pool_cNRef)
    return NULL;

  prf.m = r;
//  comparison always false prf.b.seg 8 bits and pool_cSegs 1 << 8, 9 bits
//  if (prf.b.seg > pool_cSegs)
//    errh_ReturnOrBugcheck(NULL, sts, POOL__BADSEG, "");

  psp = &php->seg[prf.b.seg];
  gpsp = &php->gphp->seg[prf.b.seg];

  if (gpsp->generation > 0) {
    psp = ensureMapped(&lsts, php, psp);
    if (psp == NULL) errh_ReturnOrBugcheck(NULL, sts, lsts, "");

    if (prf.b.offs < gpsp->size) {
      return (pool_sData *)((char *)psp->base + prf.b.offs);
    }
  }

  errh_ReturnOrBugcheck(NULL, sts, POOL__BADOFFS, "");
}

/* Allocate a piece of memory in a given pool.

   The pool is extended if needed and possible.
   Return the virtual address of the newly allocated
   piece of memory.
   Return NULL if anything goes wrong.
   Optionally the pool reference is returned in 'prp'.  */

void *
pool_Alloc (
  pwr_tStatus		*sts,
  pool_sHead		*php,
  pwr_tUInt32		size
)
{
  pwr_tStatus		lsts = 1;
  pool_sGhead		*gphp;
  pool_sGsegment	*gpsp;
  pool_sSegment		*psp;
  pool_sEntry		*ep;
  pool_sEntry		*prevp;
  pool_sEntry		*tmpp;
  pwr_tUInt32		esize;	/* True size ( incl header) in pool_sData units */
  pwr_tUInt32		tmpsize;
  pwr_tBoolean		found;
  pool_uRefBits		pr;
  pwr_tUInt32		i = 0;

#if 0
  pwr_SetStatus(sts, 1);
#endif

  gphp = php->gphp;

  if (size == gphp->la[0].size) {
    if (gphp->la[0].next != pool_cNOffset)
      return allocLookaside(sts, php, &gphp->la[0]);
  } else if (size == gphp->la[1].size) {
    if (gphp->la[1].next != pool_cNOffset)
      return allocLookaside(sts, php, &gphp->la[1]);
  } else if (size == gphp->la[2].size) {
    if (gphp->la[2].next != pool_cNOffset)
      return allocLookaside(sts, php, &gphp->la[2]);
  } else if (size == gphp->la[3].size) {
    if (gphp->la[3].next != pool_cNOffset)
      return allocLookaside(sts, php, &gphp->la[3]);
  }

  esize = entryUnits(size + sizeof(pool_sEntry));	 /* Add space for header */

  /* Find a segment where there is room.  */

  found = FALSE;
  for (
    i = 0, gpsp = &gphp->seg[0], psp = &php->seg[0];
    i < pool_cSegs;
    i++, gpsp++, psp++
  ) {
    if (gpsp->generation == 0)
      break;

    if (gpsp->type != pool_eSegType_dynamic)
      continue;

    if (gpsp->fragmax >= esize) {
      found = TRUE;
      break;
    }
  }

  /* Allocate a new segment if noone fits,
     otherwise make sure the existing segment is mapped.  */

  if (!found)
    psp = newSegment(&lsts, php, MAX(gphp->extendsize, esize));
  else
    psp = ensureMapped(&lsts, php, psp);

  if (psp == NULL)
    errh_ReturnOrBugcheck(NULL, sts, lsts, "");

  /* Walk through the free list of the chosen segment.
     The segment is pointed out by psp/gpsp!  */

  ep = NULL;
  prevp = &gpsp->freeroot;
  while (prevp->next != pool_cNOffset) {
    tmpp = (pool_sEntry *)((char *)psp->base + prevp->next * pool_cDataSize);
    if (tmpp->size >= esize) {	/* Found */
      ep = tmpp;
      break;
    } /* If the entry is larger or equal to the requested */

    prevp = tmpp;			/* Try next */
  } /* While more entries in free list */

  /* Now ep points to the chosen free list entry. Remove this entry
     from the list, or split it if it is larger than requested.
     We need to update the fragmentation information, too.

     If we come here without a selection
     there is an internal consistency problem.  */

  if (ep == NULL)
    errh_Bugcheck(POOL__BUGCHECK, "");

  tmpsize = ep->size;

  if (ep->size == esize || 
      (ep->size - esize > 0 && 
       (ep->size - esize) * pool_cDataSize < sizeof(pool_sEntry))) {	/* Entry fits exactly */
    prevp->next = ep->next;
    --gpsp->fragcnt;
  }
  else {	/* Entry is to big, split it */
    prevp->next = prevp->next + esize;
    tmpp = (pool_sEntry *)((char *)psp->base + prevp->next * pool_cDataSize);
    tmpp->next = ep->next;
    tmpp->size = ep->size - esize;
    ep->size = esize;
  }
  memset( entryPAdd(ep, sizeof(pool_sEntry)), 0, esize*pool_cDataSize - sizeof(pool_sEntry));
  ep->next = cEntryMark;

  gpsp->alloccnt++;
  gpsp->fragsize -= esize;

  /* Keep track of the maximum fragment size and count */

  if (tmpsize == gpsp->fragmax) {
    if ((--gpsp->fragmaxcnt) == 0) {	/* In this special case we need to */
				    /* rebuild the fragmax&cnt info */
      gpsp->fragmax = 0;
      gpsp->fragmaxcnt = 1;
      prevp = &gpsp->freeroot;
      while (prevp->next != pool_cNOffset) {
	tmpp = (pool_sEntry *)((char *)psp->base + prevp->next * pool_cDataSize);
	if (tmpp->size >= gpsp->fragmax) {
	  if (tmpp->size == gpsp->fragmax) gpsp->fragmaxcnt++;
	  else {
	    gpsp->fragmaxcnt = 1;
	    gpsp->fragmax = tmpp->size;
	  } /* new fragmax entry found */
	} /* new or equal to fragmax found */
	prevp = tmpp;			/* next entry */
      } /* While more entries in free list */
    } /* If no more fragmax entries left */
  } /* If this was a fragmax entry */

  pr.m = pool_cNRef;
  pr.b.seg = psp->seg;
  pr.b.offs = (pool_tOffset)((char *)ep + sizeof(pool_sEntry) - (char *)psp->base);
  ep->next = pr.m;

  return (void *)(entryPAdd(ep, sizeof(pool_sEntry)));
}

pwr_tBoolean
pool_AllocLookasideSegment (
  pwr_tStatus		*sts,
  pool_sHead		*php,
  pwr_tUInt32		count,
  pwr_tUInt32		size
)
{
  int			i;
  pool_sGhead		*gphp;
  pool_sGsegment	*gpsp;
  pool_sSegment		*psp;
  pool_tRef		offs;
  pwr_tUInt32		esize;
  pool_sEntry		*ep;

  gphp = php->gphp;

  if (gphp->la_idx >= pool_cLists)
    pwr_Return(NO, sts, POOL__NOLIST);

  /* Find an empty slot */

  for (i = 0; i < pool_cSegs; i++)
    if (gphp->seg[i].generation == 0 ||
      (gphp->seg[i].type == pool_eSegType_lookaside &&
	gphp->seg[i].la_size == size))
      break;

  if (i >=  pool_cSegs)
    pwr_Return(NO, sts, POOL__NOSLOT);

  gpsp = &gphp->seg[i];
  psp = &php->seg[i];

  /* Allocate the section */

  if (gpsp->generation == 0) {
    esize = entryUnits(size + sizeof(pool_sEntry));   /* Add space for entry header.  */
    psp = newSegment(sts, php, count * esize);
    if (psp == NULL) return NO;
    gpsp = psp->gpsp;
    gpsp->type = pool_eSegType_lookaside;
    gpsp->la_size = size;
    gpsp->la_idx = gphp->la_idx;
    gpsp->fragcnt = count;
    gphp->la[gphp->la_idx].seg = i;
    gphp->la[gphp->la_idx].size = size;
    gphp->la_idx++;
    for (i = 0, offs = 0; i < count - 1; i++) {
      ep = (pool_sEntry *)((char *)psp->base + offs * pool_cDataSize);
      ep->size = esize;
      offs += esize;
      ep->next = offs;
    }
    ep->next = pool_cNOffset;
    gpsp->freeroot.next = 0;    
    gpsp->freeroot.size = esize;
    gphp->la[gphp->la_idx].next = 0;
  } else {
    psp = ensureMapped(sts, php, psp);
    if (psp == NULL) return NO;
  }

  return YES;
}

void *
pool_AllocNamedSegment (
  pwr_tStatus		*sts,
  pool_sHead		*php,
  pwr_tUInt32		size,
  char			*name
)
{
  int			i;
  pool_sGhead		*gphp;
  pool_sGsegment	*gpsp;
  pool_sSegment		*psp;

  gphp = php->gphp;

  /* Find an empty slot */

  for (i = 0; i < pool_cSegs; i++)
    if (gphp->seg[i].generation == 0 ||
      (gphp->seg[i].type == pool_eSegType_named &&
	strcmp(name, gphp->seg[i].name) == 0))
      break;

  if (i >= pool_cSegs)
    pwr_Return(NULL, sts, POOL__NOSLOT);

  gpsp = &gphp->seg[i];
  psp = &php->seg[i];

  /* Allocate the section */

  if (gpsp->generation == 0) {
    psp = newSegment(sts, php, entryUnits(size));
    if (psp == NULL) return NULL;
    memset(psp->base, 0, size);
    gpsp->type = pool_eSegType_named;
    strncpy(gpsp->name, name, sizeof(gpsp->name) - 1);
    gpsp->name[sizeof(gpsp->name) - 1] = '\0';
  } else {
    psp = ensureMapped(sts, php, psp);
    if (psp == NULL) return NULL;
  }

  return psp->base;
}

pool_sHead *
pool_Create (
  pwr_tStatus		*sts,
  pool_sHead		*php,
  char			*name,
  size_t		initsize,
  size_t		extendsize
)
{
  pwr_tStatus		lsts;
  pwr_tBoolean		created;
  pwr_tUInt32		seg;
  pool_sSegment		*psp;
  pool_sGsegment	*gpsp;
  pool_sGhead		*gphp;
  pool_sHead		*lphp = NULL;
  sect_sHead		*shp;
  size_t		alloc_size = 0;
  size_t		alloced_size = 0;

  if (php == NULL) php = lphp = (pool_sHead *) calloc(1, sizeof(*lphp));
  if (php == NULL)
    pwr_Return(NULL, sts, POOL__INSVIRMEM);

  if (php->created == cEntryMark)
    pwr_Return(NULL, sts, POOL__ALLRMAPPED);

  do {
    shp = sect_Alloc(&lsts, &created, &php->sect, sizeof(*gphp), name);
    if (shp == NULL) break;

    gphp = (pool_sGhead *) shp->base;

    if (created) {
      strncpy(gphp->name, name, sizeof(gphp->name) - 1);
      gphp->name[sizeof(gphp->name) - 1] = '\0';

      gphp->initsize = entryUnits(initsize);
      gphp->extendsize = entryUnits(extendsize);
      gphp->generation = 1;
    }
      
    php->gphp = gphp;

    for (
      seg = 0, gpsp = &gphp->seg[0], psp = &php->seg[0];
      seg < pool_cSegs;
      seg++, gpsp++, psp++
    ) {
      gpsp->seg	= seg;
      psp->seg	= seg;
      psp->gpsp	= gpsp;

      if (gpsp->generation > 0) {
	psp = mapSegment(sts, php, psp);
	if (psp == NULL) break;      
      }
    }
    
    /* Allocate initial segment */

    if (created) {
      if (gphp->initsize != 0) {
	alloced_size = 0;
	while (alloced_size < initsize) {
	  alloc_size = MAX(MIN(initsize - alloced_size, pool_cMaxSize), MIN(extendsize,  pool_cMaxSize));
	  psp = newSegment(&lsts, php, entryUnits(alloc_size));
	  if (psp == NULL) break;
	  alloced_size += alloc_size;
	}
      }
      if (psp == NULL) break;
      gphp->created = cEntryMark;
    }

    php->created = cEntryMark;

    pwr_Return(php, sts, 1);

  } while (0);

  /* Something is wrong, cleanup!  */

  if (lphp != NULL) free(lphp);

  pwr_Return(NULL, sts, lsts);
}

/* Dump information of pool. */

void
pool_Dump (
  pwr_tStatus		*sts,
  pool_sHead		*php
)
{
  pool_sGhead		*gphp;
  pool_sGsegment	*gpsp;
  pool_sSegment		*psp;
  pwr_tUInt32		i = 0;

  gphp = php->gphp;

  errh_Error("pooldump: gen = %d, isize = %d, esize = %d, name = %s", gphp->generation,
    gphp->initsize, gphp->extendsize, gphp->name);

  for (
    i = 0, gpsp = &gphp->seg[0], psp = &php->seg[0];
    i < pool_cSegs;
    i++, gpsp++, psp++
  ) {
    if (gpsp->generation == 0)
      break;

    errh_Error("--------------------------------------------------------------------------");
    errh_Error("[%d]      : gpsp = %d, psp = %d",
      i, gpsp, psp);
    errh_Error("[%d]      : size = %d, gen = %d, allo = %d, type = %d, seg = %d",
      i, gpsp->size, gpsp->generation, gpsp->alloccnt, gpsp->type, gpsp->seg); 
    errh_Error("[%d] frag : cnt = %d, max = %d, maxcnt = %d, size = %d",
      i, gpsp->fragcnt, gpsp->fragmax, gpsp->fragmaxcnt, gpsp->fragsize); 
    errh_Error("[%d] local: sect = %d, base = %d, gpsp = %d, gen = %d, seg = %d",
      i, psp->sect, psp->base, psp->gpsp, psp->generation, psp->seg);
  }

}

/* Free a piece of memory in a given pool.
   Raise an exception if anything goes wrong.  */

pwr_tBoolean
pool_Free (
  pwr_tStatus		*sts,
  pool_sHead		*php,
  void			*p
)
{
  pwr_tStatus		lsts;
  pool_sEntry		*ep;

  /* Check & translate poolref */

  if (p == NULL)
    errh_ReturnOrBugcheck(NO, sts, POOL__NULLADDR, "");

  ep = (pool_sEntry *) p;

  ep = entryPAdd( ep, -sizeof(pool_sEntry));		/* Back to the header */

  if (p != pool_Address(&lsts, php, ep->next))
    errh_ReturnOrBugcheck(NO, sts, POOL__NOMARK, "");

  return freeItem(sts, php, ep);
}

/* Free a piece of memory in a given pool.
   Raise an exception if anything goes wrong.  */

pwr_tBoolean
pool_FreeReference (
  pwr_tStatus		*sts,
  pool_sHead		*php,
  pool_tRef		r
)
{
  pwr_tStatus		lsts;
  pool_sEntry		*ep;

  /* Check & translate poolref */

  if (r == pool_cNRef)
    errh_ReturnOrBugcheck(NO, sts, POOL__NULLREF, "");

  ep = (pool_sEntry *) pool_Address(&lsts, php, r);

  ep = entryPAdd( ep, -sizeof(pool_sEntry));		/* Back to the header */

  if (r != ep->next)
    errh_ReturnOrBugcheck(NO, sts, POOL__NOMARK, "");

  return freeItem(sts, php, ep);
}

/* Check that a virtual address range is within a pool.  */

pool_tRef
pool_InPool (
  pwr_tStatus		*sts,
  pool_sHead		*php,
  void			*adrs,
  pwr_tUInt32		size
)
{
  pool_sGhead		*gphp;
  pool_sGsegment	*gpsp;
  pool_sSegment		*psp;
  pwr_tUInt32		seg;
  pool_uRefBits		pr;
  char			*cadrs = (char *) adrs;
  char			*cbase;

  gphp = php->gphp;

  /* Scan all segments.  */

  for (
    seg = 0, gpsp = &gphp->seg[seg], psp = &php->seg[seg];
    seg < pool_cSegs;
    seg++, gpsp++, psp++
  ) {
    if (gpsp->generation == 0)
      return pool_cNRef;

    psp = ensureMapped(sts, php, psp);
    if (psp == NULL) return pool_cNRef;

    cbase = (char *) psp->base;

    if ((cadrs < cbase) || (cadrs >= (cbase + gpsp->size))) continue;

    /* OK, we have found it within this segment.  */

    pr.m = 0;
    pr.b.seg = psp->seg;
    pr.b.offs = (char *)adrs - (char *)psp->base;
    if (pr.b.offs + size >= gpsp->size) break;
    return pr.m;
  }

  /* No matching segment found.  */

  return pool_cNRef;

}

/* Translate a virtual address to a pool reference.
   Use this function only if adrs was returned 
   from a pool_Alloc, i.e. only if it points to
   the start of the allocation unit.
   Return a pool_tRef, and signals errors.  */

pool_tRef
pool_ItemReference (
  pwr_tStatus		*sts,
  pool_sHead		*php,
  void			*p
)
{
  pwr_tStatus		lsts;
  pool_sEntry		*pep = entryPAdd(p, -sizeof(pool_sEntry));

  if (p == pool_Address(&lsts, php, pep->next))
    return pep->next;

  return pool_Reference(sts, php, p);
}

/* Allocate and initialize a queue head.
   Returns pool_tRef of allocated queue.  */

pool_sQlink *
pool_Qalloc (
  pwr_tStatus		*sts,
  pool_sHead		*php
)
{
  pool_sQlink		*head;

  head = pool_Alloc(sts, php, sizeof(*head));
  
  head->flink = head->blink = head->self = ((pool_sEntry *)entryPAdd(head,-sizeof(pool_sEntry)))->next;

  return head;
}

/* Check if a queue has one and only one member.  */

pwr_tBoolean
pool_QhasOne (
  pwr_tStatus		*sts,
  pool_sHead		*php,
  pool_sQlink		*item
)
{
  pool_sQlink		*pred;
  pool_sQlink		*succ;

  pwr_Assert(checkQ(php, item));

  pred = pool_Address(sts, php, item->blink);
  succ = pool_Address(sts, php, item->flink);
  
  pwr_Assert(checkQ(php, succ));
  pwr_Assert(checkQ(php, pred));
  pwr_Assert(item->blink == pred->self);
  pwr_Assert(item->flink == succ->self);
  pwr_Assert(pred->flink == item->self);
  pwr_Assert(succ->blink == item->self);

  return item->flink == item->blink;
}

/* Initialize a queue item.  */

pool_tRef
pool_Qinit (
  pwr_tStatus		*sts,
  pool_sHead		*php,
  pool_sQlink		*item	/* reference to header (a pool_sRefLink) */
)
{
  pool_tRef		r;

  r = pool_Reference(sts, php, item);
  if (r != pool_cNRef)
    item->self = item->flink = item->blink = r;
  return r;
}

/* Insert 'item' as predecessor to 'succ'. The linkage is
   poolrefs.  */

pool_sQlink *
pool_QinsertPred (
  pwr_tStatus		*sts,
  pool_sHead		*php,
  pool_sQlink		*item,		/* Item to insert */
  pool_sQlink		*succ		/* Insert before this element */
)
{
  pool_sQlink		*pred;
  volatile pwr_tBoolean	result;

  result = checkInitQ(php, item);
  pwr_Assert(result);

  pred = pool_Address(sts, php, succ->blink);
  
  pwr_Assert(checkQ(php, succ));
  pwr_Assert(checkQ(php, pred));
  pwr_Assert(pred->flink == succ->self);
  pwr_Assert(pred->self == succ->blink);

  item->flink = succ->self;
  item->blink = succ->blink;
  succ->blink = pred->flink = item->self;

  return pred;  
}

/* Insert 'item' as successor to 'pred'. The linkage is
   poolrefs.  */

pool_sQlink *
pool_QinsertSucc (
  pwr_tStatus		*sts,
  pool_sHead		*php,
  pool_sQlink		*item,		/* Item to insert */
  pool_sQlink		*pred		/* Insert after this element */
)
{
  pool_sQlink		*succ;
  volatile pwr_tBoolean	result;

  result = checkInitQ(php, item);
  pwr_Assert(result);

  succ = pool_Address(NULL, php, pred->flink);
  
  pwr_Assert(checkQ(php, succ));
  pwr_Assert(checkQ(php, pred));
  pwr_Assert(succ->blink == pred->self);
  pwr_Assert(succ->self == pred->flink);

  item->blink = pred->self;
  item->flink = pred->flink;
  pred->flink = succ->blink = item->self;

  return succ;  
}

/* Check if a queue header is empty.  */

pwr_tBoolean
pool_QisEmpty (
  pwr_tStatus		*sts,
  pool_sHead		*php,
  pool_sQlink		*item
)
{
  pool_sQlink		*pred;
  pool_sQlink		*succ;

  pwr_Assert(checkQ(php, item));

  pred = pool_Address(sts, php, item->blink);
  succ = pool_Address(sts, php, item->flink);
  
  pwr_Assert(checkQ(php, succ));
  pwr_Assert(checkQ(php, pred));
  pwr_Assert(item->blink == pred->self);
  pwr_Assert(item->flink == succ->self);
  pwr_Assert(pred->flink == item->self);
  pwr_Assert(succ->blink == item->self);

  return item->flink == item->self;
}

/* Check if a queue link is linked.  */

pwr_tBoolean
pool_QisLinked (
  pwr_tStatus		*sts,
  pool_sHead		*php,
  pool_sQlink		*item
)
{
  pool_sQlink		*pred;
  pool_sQlink		*succ;

  pwr_Assert(checkQ(php, item));

  pred = pool_Address(sts, php, item->blink);
  succ = pool_Address(sts, php, item->flink);
  
  pwr_Assert(checkQ(php, succ));
  pwr_Assert(checkQ(php, pred));
  pwr_Assert(item->blink == pred->self);
  pwr_Assert(item->flink == succ->self);
  pwr_Assert(pred->flink == item->self);
  pwr_Assert(succ->blink == item->self);

  return item->flink != item->self;
}

/* Check if a queue link is initiated.  */

pwr_tBoolean
pool_QisNull (
  pwr_tStatus		*sts,
  pool_sHead		*php,
  pool_sQlink		*item
)
{
  pwr_tBoolean		nullQ;

  nullQ = (item->self == pool_cNRef)
    || (item->blink == pool_cNRef)
    || (item->flink == pool_cNRef);
  
  pwr_Assert(item->blink == pool_cNRef);
  pwr_Assert(item->flink == pool_cNRef);
  pwr_Assert(item->self  == pool_cNRef);

  return nullQ;
}

/* Move the elements in queue 'old' to queue 'new'. The linkage is
   poolrefs.  */

pool_sQlink *
pool_Qmove (
  pwr_tStatus		*sts,
  pool_sHead		*php,
  pool_sQlink		*old,		/* Old queue header */
  pool_sQlink		*new		/* New queue header */
)
{
  pool_sQlink		*pred;
  pool_sQlink		*succ;
  volatile pwr_tBoolean	result;

  result = checkInitQ(php, new);
  pwr_Assert(result);

  pred = pool_Address(sts, php, old->blink);
  succ = pool_Address(sts, php, old->flink);
  
  pwr_Assert(checkQ(php, succ));
  pwr_Assert(checkQ(php, pred));
  pwr_Assert(old->blink == pred->self);
  pwr_Assert(old->flink == succ->self);
  pwr_Assert(pred->flink == old->self);
  pwr_Assert(succ->blink == old->self);

  if (old->flink != old->self) {
    new->flink = succ->self;
    new->blink = pred->self;
    succ->blink = pred->flink = new->self;
    old->flink = old->blink = old->self;
  }

  return new;  
}

/* Return predecessor to 'item'. The linkage is
   poolrefs.  */

pool_sQlink *
pool_Qpred (
  pwr_tStatus		*sts,
  pool_sHead		*php,
  pool_sQlink		*item
)
{
  pool_sQlink		*pred;

  pwr_Assert(checkQ(php, item));

  pred = pool_Address(NULL, php, item->blink);
  
  if (pred != NULL) {
    pwr_Assert(checkQ(php, pred));
    pwr_Assert(pred->flink == item->self);
    pwr_Assert(pred->self == item->blink);
  }

  return pred;  
}

/* Remove an item from a queue. The linkage is
   poolrefs.  */

pool_sQlink *
pool_Qremove (
  pwr_tStatus		*sts,
  pool_sHead		*php,
  pool_sQlink		*item
)
{
  pool_sQlink		*succ;
  pool_sQlink		*pred;

  pwr_Assert(checkQ(php, item));

  if (item->flink == item->self || item->blink == item->self) {
    pwr_Assert(item->flink == item->blink);
    return NULL;
  }

  succ = pool_Address(NULL, php, item->flink);
  pred = pool_Address(NULL, php, item->blink);

  pwr_Assert(succ->blink == item->self);
  pwr_Assert(pred->flink == item->self);

  succ->blink = item->blink;
  pred->flink = item->flink;
  item->flink = item->self;
  item->blink = item->self;

  return item;
}

/* Remove predecessor of an item, from a queue.
   The linkage is poolrefs.
   If item is queue header this call will
   remove the first item in the queue.

   Return address of removed predecessor. */

pool_sQlink *
pool_QremovePred (
  pwr_tStatus		*sts,
  pool_sHead		*php,
  pool_sQlink		*item
)
{
  pool_sQlink		*pred;
  pool_sQlink		*pred_pred;

  pwr_Assert(checkQ(php, item));

  if (item->flink == item->self || item->blink == item->self) {
    pwr_Assert(item->flink == item->blink);
    return NULL;
  }

  pred = pool_Address(NULL, php, item->blink);

  pwr_Assert(checkQ(php, pred));
  pwr_Assert(pred->flink == item->self);

  pred_pred = pool_Address(NULL, php, pred->blink);

  pwr_Assert(checkQ(php, pred_pred));
  pwr_Assert(pred_pred->flink == pred->self);

  pred_pred->flink = item->self;
  item->blink = pred_pred->self;
  pred->flink = pred->blink = pred->self;

  return pred;
}

/* Remove successor of an item, from a queue.
   The linkage is poolrefs.
   If item is queue header this call will
   remove the first item in the queue.

   Return address of removed successor. */

pool_sQlink *
pool_QremoveSucc (
  pwr_tStatus		*sts,
  pool_sHead		*php,
  pool_sQlink		*item
)
{
  pool_sQlink		*succ;
  pool_sQlink		*succ_succ;

  pwr_Assert(checkQ(php, item));

  if (item->flink == item->self || item->blink == item->self) {
    pwr_Assert(item->flink == item->blink);
    return NULL;
  }

  succ = pool_Address(NULL, php, item->flink);

  pwr_Assert(checkQ(php, succ));
  pwr_Assert(succ->blink == item->self);

  succ_succ = pool_Address(NULL, php, succ->flink);

  pwr_Assert(checkQ(php, succ_succ));
  pwr_Assert(succ_succ->blink == succ->self);

  succ_succ->blink = item->self;
  item->flink = succ_succ->self;
  succ->flink = succ->blink = succ->self;

  return succ;
}

/* Return successor to 'item'. The linkage is
   poolrefs.  */

pool_sQlink *
pool_Qsucc (
  pwr_tStatus		*sts,
  pool_sHead		*php,
  pool_sQlink		*item
)
{
  pool_sQlink		*succ;

  pwr_Assert(checkQ(php, item));

  succ = pool_Address(NULL, php, item->flink);
  
  if (succ != NULL) {
    pwr_Assert(checkQ(php, succ));
    pwr_Assert(succ->blink == item->self);
    pwr_Assert(item->flink == succ->self);
  }

  return succ;  
}

/* Allocate a piece of memory in a given pool.

   The pool is extended if needed and possible.
   Return the virtual address of the newly allocated
   piece of memory.
   Return pool_cNRef if anything goes wrong.
   Optionally the pool reference is returned in 'prp'.  */

pool_tRef
pool_RefAlloc (
  pwr_tStatus		*sts,
  pool_sHead		*php,
  pwr_tUInt32		size
)
{
  pool_sEntry		*ep;

  ep = pool_Alloc(sts, php, size);
  if (ep == NULL) return pool_cNRef;

  return entryPAdd(ep, -sizeof(pool_sEntry))->next;
}

/* Translate a virtual address to a pool reference.
   Return a pool_tRef, and signals errors.  */

pool_tRef
pool_Reference (
  pwr_tStatus		*sts,
  pool_sHead		*php,
  void			*adrs
)
{
  pool_sGhead		*gphp;
  pool_sGsegment	*gpsp;
  pool_sSegment		*psp;
  pwr_tUInt32		seg;
  pool_uRefBits		pr;
  char			*cadrs = (char *) adrs;
  char			*cbase;

  gphp = php->gphp;

  /* Scan all segments.  */

  for (
    seg = 0, gpsp = &gphp->seg[seg], psp = &php->seg[seg];
    seg < pool_cSegs;
    seg++, gpsp++, psp++
  ) {
    if (gpsp->generation == 0)
      return pool_cNRef;

    psp = ensureMapped(sts, php, psp);
    if (psp == NULL) return pool_cNRef;

    cbase = (char *) psp->base;

    if ((cadrs < cbase) || (cadrs >= (cbase + gpsp->size))) continue;

    /* OK, we have found it within this segment.  */

    pr.m = 0;
    pr.b.seg = psp->seg;
    pr.b.offs = (char *)adrs - (char *)psp->base;
    return pr.m;
  }

  /* No matching segment found.  */

  return pool_cNRef;

}
