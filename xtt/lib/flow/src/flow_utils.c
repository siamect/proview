
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

