/* 
 * Proview   $Id: ssabox_sr_regulator_methods.c,v 1.2 2006-02-01 08:21:21 claes Exp $
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

/*
* Author: Jonas Haulin   email: jonas@haulin.se 
* Contains internal and engine functions for antisway objects. 
*/

#include "ssabox_sr_regulator_methods.h"


/* Prototype declarations of internal functions */
static void remove_tail(SR_RefList *tail);


void SR_addNewRef(SR_RefList **objectRefListpp, double aR, double uR, double xR)
{
  SR_RefList *newest;

  newest = (SR_RefList *) malloc(sizeof(SR_RefList));
  if (newest == NULL) {
    fprintf(stderr, "SR_addNewRef: Could not allocate memory!\n");
    exit(99);
  }
  newest->older = *objectRefListpp; //ok if this happens to be NULL
  newest->a = aR;
  newest->u = uR;
  newest->x = xR;

  *objectRefListpp = newest; //the object RefList pointer always points at the newest reference.
}

void SR_extractRef(SR_RefList **objectRefListpp, int delay, double *aDelayp, double *uDelayp, double *xDelayp)
{
  int i = 0;
  SR_RefList *sought;
 
  sought = *objectRefListpp;
  if (sought == NULL) //This should not happen..
    return;

  /* Find delayed references */
  while ((sought->older != NULL) && (i++ < delay))
    sought = sought->older;   

  /* Extract delayed references */
  *aDelayp = sought->a;
  *uDelayp = sought->u;
  *xDelayp = sought->x;

  if ((i > delay) && (sought->older != NULL)) { // Remove tail of post-delay references if it exists
    remove_tail(sought->older);
    sought->older = NULL;
  }
}

static void remove_tail(SR_RefList *tail) //recursive function to remove tail of list.
{
  if (tail->older != NULL)
    remove_tail(tail->older);
    
  free(tail);
}

