/* 
 * Proview   $Id: rt_cvolc.h,v 1.3 2008-06-24 07:06:56 claes Exp $
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

#ifndef rt_cvolc_h
#define rt_cvolc_h

/* rt_cvolc.h -- Cached volumes, client calls.  */


#ifndef rt_cvol_h
#include "rt_cvol.h"
#endif

/* Callable functions.  */

void *
cvolc_GetObjectInfo (
  pwr_tStatus		*sts,
  const gdb_sNode	*np,
  const pwr_sAttrRef	*arp,
  gdb_sCclass		*ccp,
  const pwr_sAttrRef	*rarp,
  pwr_tUInt32		ridx,
  mvol_sAttribute	*ap,
  void			*p,
  int			size
);

void
cvolc_LockObject (
  pwr_tStatus		*sts,
  gdb_sObject		*op
);

gdb_sObject *
cvolc_NameToObject (
  pwr_tStatus		*sts,
  gdb_sObject		*p_op,
  cdh_sParseName	*pn,
  int			index,
  pwr_tBitMask		trans
);

gdb_sObject *
cvolc_OidToObject (
  pwr_tStatus		*sts,
  gdb_sVolume		*vp,
  pwr_tObjid		oid,
  pwr_tBitMask		trans,
  cvol_eHint		hint
);

pwr_tBoolean
cvolc_SetObjectInfo (
  pwr_tStatus		*sts,
  const gdb_sNode	*np,
  const pwr_sAttrRef	*arp,
  const gdb_sCclass	*ccp,
  const pwr_sAttrRef	*rarp,
  pwr_tUInt32		ridx,
  mvol_sAttribute	*ap,
  const void		*p,
  int			size
);

void
cvolc_TouchObject (
  gdb_sObject		*op
);

void
cvolc_UnlockObject (
  pwr_tStatus		*sts,
  gdb_sObject		*op
);

void
cvolc_FileList (
  pwr_tStatus		*sts,
  gdb_sObject		*p_op,
  char			*dir,
  char			*pattern,
  pwr_tString40		*filelist[],
  int			*filecnt
);

#endif
