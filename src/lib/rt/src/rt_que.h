#ifndef rt_que_h
#define rt_que_h

/* rt_que.h -- <short description>

   PROVIEW/R
   Copyright (C) 1999 by Mandator AB.

   .  */

#include "pwr.h"
#include "rt_sync.h"
#include "rt_lst.h"

typedef struct {
  thread_sMutex	mutex;
  thread_sCond	cond;
  lst_sEntry	lh;
} que_sQue;

que_sQue *		que_Create(pwr_tStatus*, que_sQue*);
void *			que_Get(pwr_tStatus*, que_sQue*, pwr_tDeltaTime*, void*);
void			que_Put(pwr_tStatus*, que_sQue*, lst_sEntry*, void*);

#endif
