#ifndef ssabox_sr_regulator_methods_h
#define ssabox_sr_regulator_methods_h 

#include "pwr_baseclasses.h"
#include "pwr_basecomponentclasses.h"
#include "pwr_ssaboxclasses.h"
#include "ssabox_as_phasor_methods.h" //included to get max, min macros etc.

/* Macros */

#define SR_OBJ_LISTPP ((SR_RefList **) &object->RefListp)




/* Typedefs / structs */

#ifndef ssabox_sr_servoreg_structs
#define ssabox_sr_servoreg_structs 

typedef struct SR_RefList {
  double a;
  double u;
  double x;
  struct SR_RefList *older;
} SR_RefList;

#endif

/* Prototype declarations of externally visible regulator methods.*/

void SR_addNewRef(SR_RefList **objectRefListp, double aR, double uR, double xR);
void SR_extractRef(SR_RefList **objectRefListp, int delay, double *aDelayp, double *uDelayp, double *xDelayp);


#endif
