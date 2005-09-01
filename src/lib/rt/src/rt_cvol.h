/* 
 * Proview   $Id: rt_cvol.h,v 1.2 2005-09-01 14:57:55 claes Exp $
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

#ifndef rt_cvol_h
#define rt_cvol_h

/* rt_cvol.h -- Global Data Handler, Cached volumes
   Cache mounted volumes.  */


#ifndef pwr_h
#include "pwr.h"
#endif

#ifndef rt_gdb_h
#include "rt_gdb.h"
#endif

#ifndef rt_net_h
#include "rt_net.h"
#endif

typedef enum {
  cvol_eHint__ = 0,
  cvol_eHint_child,
  cvol_eHint_name,
  cvol_eHint_next,
  cvol_eHint_none,
  cvol_eHint_prev,
  cvol_eHint_
} cvol_eHint;

typedef struct {
  net_eMsg		subtype;
  unsigned int		size;
  union {
    net_sNotify		hdr;
    net_sCreateObject	c;
    net_sDeleteObject	d;
    net_sMoveObject	m;
    net_sRenameObject	r;
  } msg;
} cvol_sNotify;

/* Callable functions.  */

gdb_sObject *
cvol_AllocObject (
  pwr_tStatus		*sts,
  gdb_sNode		*np,
  gdb_sVolume		*vp
);

void
cvol_FlushObject (
  gdb_sObject		*op
);

void
cvol_FreeObject (
  pwr_tStatus		*sts,
  gdb_sVolume		*vp,
  gdb_sObject		*op,
  pwr_tBitMask		link
);

gdb_sObject *
cvol_LinkObject	(
  pwr_tStatus		*sts,
  gdb_sVolume		*vp,
  gdb_sObject		*op,
  pwr_tBitMask		link
);

gdb_sObject *
cvol_LoadObject (
  pwr_tStatus		*sts,
  gdb_sNode		*np,
  gdb_sVolume		*vp,
  net_sGobject		*gop
);

void
cvol_QforcedTrim (
  gdb_sTouchQ		*fqp
);

gdb_sTouchQ *
cvol_Qget (
  gdb_sObject		*op
);

void
cvol_QmovePred (
  gdb_sObject		*op,
  gdb_sTouchQ		*fqp,
  gdb_sTouchQ		*tqp
);

void
cvol_QmoveSucc (
  gdb_sObject		*op,
  gdb_sTouchQ		*fqp,
  gdb_sTouchQ		*tqp
);

void
cvol_Qremove (
  gdb_sObject		*op
);

void
cvol_Qtrim (
  gdb_sTouchQ		*fqp
);

void
cvol_UnlinkObject (
  pwr_tStatus		*sts,
  gdb_sVolume		*vp,
  gdb_sObject		*op,
  pwr_tBitMask		link
);

#endif
