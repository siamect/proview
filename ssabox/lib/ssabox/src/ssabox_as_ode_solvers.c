/* 
 * Proview   $Id: ssabox_as_ode_solvers.c,v 1.2 2006-02-01 08:21:21 claes Exp $
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

/*2005-02-13      File: AS_ODE_Solvers.c
* Author: Jonas Haulin   email: joha7211@student.uu.se 
* Contains functions that will integrate the discretized DE:s for sway angle, 
* trolley position and cable length
*/

#include "ssabox_as_ode_solvers.h"

void AS_thetaIntegrator(double *thetam, double *thetac, double Lc, double DLc, double ac, double dt) 
{
  /* Function: thetaIntegrator
   * Simulation of sway of crane load.
   * One-dimensional traversal and sway. Variable pendulum length, L
   * Lagrange's equation: D^2(theta)*L + 2*D(L)*D(theta) + D^2(x)*cos(theta) + g*sin(theta) = 0
   * 
   * Integrates Lagrange's equation and stores the current sway angle in thetac
   *
   * Variables
   * thetam - sway angle at time n-1
   * thetac - sway angle at time n
   * thetap - sway angle at time n+1 */
  
  /*Solving the discretized ODE for thetap*/
  double thetap = (2.0*(*thetac)*Lc + (*thetam)*(dt*DLc - Lc) - (dt*dt)*(ac*cos(*thetac) + AS_GRAV_ACCEL*sin(*thetac)))/(dt*DLc + Lc);
  *thetam=*thetac;
  *thetac=thetap;
}

void AS_xIntegrator(double *xm, double *xc, double ac, double dt) 
{
  /* Calculates the position at time n+1 from acceleration and x(n), x(n-1) */

  double xp=2.0*(*xc) - *xm + ac*(dt*dt);
  *xm=*xc;
  *xc=xp;
}

void AS_xIntegratorUA(double *xm, double *xc, double uc, double ac, double dt) 
{
/* Calculates the position at time n+1 from velocity and x(n) */

  double xp=*xc + uc*dt + 0.5*ac*(dt*dt);
  *xm=*xc;
  *xc=xp;
}

void AS_xcIntegratorU(double *xc, double uc, double dt) 
{
  *xc += uc*dt;
}

void AS_xIntegrator2U(double *xm, double *xc, double um, double dt) 
{
  double xp = *xm + 2.0*um*dt;
  *xm=*xc;
  *xc=xp;
}

void AS_uIntegratorFwd(double *uc, double ac, double dt) 
{
/* Calculates velocity at time n+1 from uc and ac with a forward difference approximation of the derivative */

  *uc += ac*dt;
}

void AS_LIntegratorFwd(double *Lc, double DLc, double dt) 
{
  *Lc += DLc*dt;
} 
