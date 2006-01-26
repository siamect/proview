/*2005-09-14      File: SR_regulator_methods.c
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

