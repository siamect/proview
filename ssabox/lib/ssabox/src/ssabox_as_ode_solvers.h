/* 
 * Proview   $Id: ssabox_as_ode_solvers.h,v 1.2 2006-02-01 08:21:21 claes Exp $
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
