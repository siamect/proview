/** 
 * Proview   $Id: flow_utils.c,v 1.2 2005-09-01 14:56:12 claes Exp $
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
 **/


#include <stdlib.h>
#include "flow_utils.h"

static void  qsort_swp(int, char *, char *);

void flow_qsort(
  void *basearg, 
  size_t nel, 
  size_t width, 
  int (* compar)(const void*, const void*)
)
{
  int     gap,ngap,i,j;
  int     t1,t2;
  char    *jd;
  char    *base = basearg;

  t1 = nel * width;
  for(ngap=nel/2; ngap > 0; ngap = ngap / 2)
  {
    gap = ngap * width;
    t2 = gap + width;
    jd = base + gap;
    for(i=t2; i <= t1; i += width)
      for(j = i - t2 ; j >= 0 ; j -= gap)
      {
        if ((*compar)(base + j, jd + j) <= 0)
          break;
        qsort_swp(width, base + j, jd + j);
      }
  }
}

static void qsort_swp(int w, char *a, char *b)
{
  char    tmp;

  while(w--)
  {
    tmp = *a;
    *a++ = *b;
    *b++ = tmp;
  }
}

