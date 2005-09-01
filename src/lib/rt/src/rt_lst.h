/* 
 * Proview   $Id: rt_lst.h,v 1.2 2005-09-01 14:57:55 claes Exp $
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

#ifndef rt_lst_h
#define rt_lst_h

#ifndef pwr_h
# include "pwr.h"
#endif

#ifndef rt_sync_h
# include "rt_sync.h"
#endif

typedef struct lst_sEntry lst_sEntry;
struct lst_sEntry {
  lst_sEntry *flink;
  lst_sEntry *blink;
  void *item;
};
  
pwr_tBoolean	lst_HasOne	(thread_sMutex*, lst_sEntry*);
lst_sEntry *	lst_Init	(thread_sMutex*, lst_sEntry*, void*);
lst_sEntry *	lst_InsertPred	(thread_sMutex*, lst_sEntry*, lst_sEntry*, void*);
lst_sEntry *	lst_InsertSucc	(thread_sMutex*, lst_sEntry*, lst_sEntry*, void*);
pwr_tBoolean	lst_IsEmpty	(thread_sMutex*, lst_sEntry*);
pwr_tBoolean	lst_IsLinked	(thread_sMutex*, lst_sEntry*);
pwr_tBoolean	lst_IsNull	(thread_sMutex*, lst_sEntry*);
pwr_tBoolean	lst_IsSucc	(thread_sMutex*, lst_sEntry*, lst_sEntry*);
lst_sEntry *	lst_Move	(thread_sMutex*, lst_sEntry*, lst_sEntry*);
void *		lst_Pred	(thread_sMutex*, lst_sEntry*, lst_sEntry**);
void *		lst_Remove	(thread_sMutex*, lst_sEntry*);
void *		lst_RemovePred	(thread_sMutex*, lst_sEntry*, lst_sEntry**);
void *		lst_RemoveSucc	(thread_sMutex*, lst_sEntry*, lst_sEntry**);
void *		lst_Succ	(thread_sMutex*, lst_sEntry*, lst_sEntry**);

#endif
