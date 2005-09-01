/* 
 * Proview   $Id: rt_que.h,v 1.2 2005-09-01 14:57:56 claes Exp $
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

#ifndef rt_que_h
#define rt_que_h

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
