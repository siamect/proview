/*********************************************************************
**********************************************************************
*
*       S S A B  O X E L Ö S U N D
*       = = = =  = = = = = = = = =
**********************************************************************
*
* Filename      : 
*
* Description   : This header file is included by the plc modules
*		: and allows the user to include function prototypes, 
*		: defines, typedefs etc.
*                 
*               Date    Sign           Action
* Revision      040923  SSABCJ         First edition.
*               050213  jonas_h        Antisway
*
*
**********************************************************************
**********************************************************************/

#include <stddef.h>
#include <time.h>
#include <sys/time.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <float.h>
#include "libiberty.h"

#include "pwr_baseclasses.h"
#include "pwr_basecomponentclasses.h"
#include "pwr_ssaboxclasses.h"
#include "rt_plc.h"
#include "co_time.h"
#include "pwr.h"

#include "ssabox_as_ode_solvers.h"
#include "ssabox_as_phasor_methods.h"


void Ssab_AntiSway_init(pwr_sClass_Ssab_AntiSway *object);

void Ssab_AntiSway_exec(plc_sThread *tp, pwr_sClass_Ssab_AntiSway *object);
