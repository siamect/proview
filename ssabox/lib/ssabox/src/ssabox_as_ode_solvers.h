/*********************************************************************
**********************************************************************
*
*       S S A B  O X E L Ö S U N D
*       = = = =  = = = = = = = = =
**********************************************************************
*
* Filename      : 
*
* Description   : This header file is included by AS_ODE_solvers.c
*               : and ra_plc_user.h.
*		: It contains prototypes for the discrete-time
*               : ODE solvers.
*                 
*               Date    Sign           Action
* Revision      050213  jonas_h        First edition.
*               
*
*
**********************************************************************
**********************************************************************/

#ifndef ssabox_as_ode_solvers_h
#define ssabox_as_ode_solvers_h

#include <math.h>

#include "ssabox_as_phasor_methods.h"

/*prototype declarations*/
void AS_thetaIntegrator(double *, double *, double , double , double, double dt);
void AS_xIntegrator(double *, double *, double, double dt);
void AS_xIntegratorUA(double *, double *, double, double, double dt);
void AS_xcIntegratorU(double *, double, double dt);
void AS_xIntegrator2U(double *xm, double *xc, double um, double dt);
void AS_uIntegratorFwd(double *, double, double dt);
void AS_LIntegratorFwd(double *, double , double dt);

#endif
