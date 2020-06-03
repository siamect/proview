#include <float.h>
#include <math.h>
#include "pwr.h"
#include "rt_plc.h"
#include "ra_plc_user.h"



void Pwrt01_NotEqual_exec(plc_sThread *tp, pwr_sClass_Pwrt01_NotEqual *o)
{
  o->In1 = *o->In1P;
  o->In2 = *o->In2P;
  if ((isnan(o->In1) && isnan(o->In2)) || 
      (isinf(o->In1) && isinf(o->In2)))
    o->Status = 0;
  else
    o->Status = !(fabsf(o->In1 - o->In2) < o->MaxValue * FLT_EPSILON);
}

