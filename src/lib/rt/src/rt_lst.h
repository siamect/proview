#ifndef rt_lst_h
#define rt_lst_h

/* rt_lst.h -- <short description>

   PROVIEW/R
   Copyright (C) 1999 by Mandator AB.

   .  */

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
