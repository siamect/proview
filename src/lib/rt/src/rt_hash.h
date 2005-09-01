/* 
 * Proview   $Id: rt_hash.h,v 1.2 2005-09-01 14:57:55 claes Exp $
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

#ifndef rt_hash_h
#define rt_hash_h

#include "pwr.h"
#include "rt_sect.h"
#include "rt_pool.h"

/* Data structure definitions

   The following defines the hash table. The table
   contains a mutex used to sychronize access to the table. Each
   access MUST lock the mutex (except statistics read, of course).  */

#define	hash_cVersion	4 /* Layout version number */

typedef enum {
  hash_eKey__ = 0,
  hash_eKey_uint16,
  hash_eKey_memcmp,
  hash_eKey_strncmp,
  hash_eKey_objName,
  hash_eKey_family,
  hash_eKey_int32,
  hash_eKey_uint32 = hash_eKey_int32,
  hash_eKey_oix = hash_eKey_int32,
  hash_eKey_cid = hash_eKey_int32,
  hash_eKey_tid = hash_eKey_int32,
  hash_eKey_vid = hash_eKey_int32,
  hash_eKey_nid = hash_eKey_int32,
  hash_eKey_pid = hash_eKey_int32,
  hash_eKey_qix = hash_eKey_int32,
  hash_eKey_oid,
  hash_eKey_rid = hash_eKey_oid,
  hash_eKey_qid = hash_eKey_oid,
  hash_eKey_user,
  hash_eKey_
} hash_eKey;

/* This structure is stored in pool space, in blocks of `extendentries' */

typedef pool_sQlink hash_sEntry;

/* The primary hash table (with `Size' entries) is also stored in pool space.
   It contains only a pool_sRefLink for each entry.  */

typedef union {
  pwr_tBitMask m;
  pwr_32Bits (
    pwr_Bits( created	, 1 ),

    pwr_Bits( fill	, 31 ),,,,,,,,,,,,,,,,,,,,,,,,,,,,,,
  ) b;

#define  hash_mGtable__		0
#define  hash_mGtable_created	pwr_Bit(0)  
#define  hash_mGtable_		(~hash_mGtable__)  
} hash_mGtable;

typedef struct {
  hash_mGtable		flags;
  size_t		size;		/* Primary table size */
  pool_tRef		table;		/* Primary hash table reference */

  size_t		key_size;
  size_t		record_size;
  ptrdiff_t		key_offset;
  ptrdiff_t		link_offset;

  hash_eKey		key_type;	/* Key type.  */

  /* Hash algorithm statistics */

  pwr_tUInt32		xforms;		/* # of key transformations */
  pwr_tUInt32		comps;		/* # of key comparisons */
  pwr_tUInt32		max_depth;	/* Top key comparison cnt ever */
  pwr_tUInt32		inserts;
  pwr_tUInt32		removes;
  pwr_tUInt32		searchs;
  pwr_tUInt32		used_buckets;
  pwr_tUInt32		max_used_buckets;
  pwr_tUInt32		entries;
  pwr_tUInt32		max_entries;
  pwr_tUInt32		finds;
  pwr_tUInt32		no_finds;
  pwr_tUInt32		inits;
} hash_sGtable;

/* Job local structure */

typedef struct {
  hash_sGtable		*ghtp;		/* GHT data */
  pool_sHead		*php;		/* Pool to use for storage.   */
  pool_sQlink		*tp;		/* pointer to primary hash table, optimization... */
  pwr_tBoolean		(*comp_f) (	/* Key comparison routine */
			  const void *,
			  void *
			);
  pwr_tUInt32		(*xform_f) (	/* Key transformation routine */
			  const void *,
			  size_t
			);
} hash_sTable;

void *
hash_Search (
  pwr_tStatus		*sts,
  hash_sTable		*htp,
  const void		*key
);

void *
hash_Insert (
  pwr_tStatus		*sts,
  hash_sTable		*htp,
  void			*ip		/* Address of item to be inserted.  */
);

void *
hash_Remove (
  pwr_tStatus		*sts,
  hash_sTable		*htp,
  void			*ip		/* Address of item to be removed.  */
);

hash_sTable *
hash_Create (
  pwr_tStatus		*sts,
  pool_sHead		*php,
  hash_sTable		*htp,
  hash_sGtable		*ghtp,
  pwr_tBoolean		(*comp_f) (const void *, void *),		/* Key comparison routine */
  pwr_tUInt32		(*xform_f) (const void *, size_t)	/* Key transformation routine */
);

void
hash_Init (
  hash_sGtable		*p,
  size_t		size,
  size_t		key_size,
  size_t		record_size,
  ptrdiff_t		key_offset,
  ptrdiff_t		link_offset,
  hash_eKey		key_type
);

void
hash_Print (
  pwr_tStatus		*sts,
  hash_sTable		*htp
);

#endif
