/* 
 * Proview   $Id: rt_csup.h,v 1.3 2005-09-01 14:57:55 claes Exp $
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

#ifndef rt_csup_h
#define rt_csup_h 

/* rt_csup.h -- Cycle supervision */

#ifndef rt_lst_h
# include "rt_lst.h"
#endif

typedef struct {
  lst_sEntry le;
  pwr_sClass_CycleSup *o;
  pwr_sAttrRef aref;
  pwr_tDlid dlid;
  int is_owner;
} csup_sObject;

void		csup_Exit(pwr_tStatus*, lst_sEntry*);
lst_sEntry*	csup_Init (pwr_tStatus*, pwr_tObjid, float);
int		csup_Exec(pwr_tStatus*, lst_sEntry*, pwr_tDeltaTime*, pwr_tDeltaTime*, pwr_tTime*);

#endif
