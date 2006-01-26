/*2005-02-13      File: AS_phasor_methods.c
* Author: Jonas Haulin   email: joha7211@student.uu.se 
* Contains internal and engine functions for antisway objects. 
*/

#include "ssabox_as_phasor_methods.h"


/* Prototype declarations of internal functions */

static unsigned int makeSet_man(AS_phasorSet *Setp, const AS_shaper *shp, double uCommand, double uc, double amax, double omega, int verbose);
static unsigned int makeSet_auto(AS_phasorSet *Setp, const AS_shaper *shp, unsigned int *astsp, double SxDiff, double ext_match, double uc, double amaxH, 
				 double amaxS, double umaxH, double umaxS, double umin, double omega, int verbose, int overlap);
static unsigned int mergeSet(AS_phasorSet *Setp, const AS_shaper *shp, double SuDiff, double amax, double omega, int reflected, int verbose);
static void phaseInterval(const AS_phasorSet *Setp, double phi, double SaWanted, double SamaxS, double *aRatio, double *ext, int reflected);
static void cleanSet(AS_phasorSet *Setp);
static double timeStepSet(AS_phasorSet *Setp, double omegadt);
static double addPhasor(AS_phasorSet *Setp, double phiS, double phiE, double SaAdd);
static double removePhasor(AS_phasorSet *Setp, int n);
static void reflectSet(AS_phasorSet *Setp, double *reflectionPoint);
static void assignSet(AS_phasorSet *destp, const AS_phasorSet *srcp);
static void appendSet(AS_phasorSet *destp, const AS_phasorSet *srcp);
static double delta_x(const AS_phasorSet *Setp, double uc, double romega);
static void reflectShiftAndInvertSet(AS_phasorSet *Setp, double shift);
static int supAmax(AS_phasorSet *Setp, double amax);
static int subUmin(AS_phasorSet *Setp, double uc, double romega, double umin);
static int supUmax(AS_phasorSet *Setp, double uc, double romega, double umax);
static AS_shaper *reflectShaper(const AS_shaper *shp);
static double getX(const AS_phasorSet *Setp);
static double getY(const AS_phasorSet *Setp);
static double arctan360N(double Y, double X);
static int mQSize(const AS_messageQ *mQ);
static AS_messageQ *mQLast(const AS_messageQ *mQ);



/* Function: AS_Engine_man 
 *
 * ::Engine for manual mode set processing::
 * Takes all necessary variables from AntiSway_exec to create or change the phasor set.
 * Returns an error status variable. Writes the current acceleration in *aRp.
 */

unsigned int AS_Engine_man( int newCall, int hoisting, int hoisted, int verbose, const AS_shaper *shp,
			    double uCommand, double uR, double DLc, double omega, double amaxH, double amaxS, double dt, 
			    double *aRp, AS_phasorSet *Setp ) 
{ 
  unsigned int sts=0;
  double SaComp,Sa0;

  if (shp->ID == AS_NO_AS) { /* If NO_AS shaper, remove unnecessary constraints */
    hoisting=0;
    hoisted=0;
    amaxS=amaxH;
  }

  /* Allocate or reallocate phasors if needed */

  /* Correct for hoisting */
  if (hoisting) { 

    /* Calculate compensation acceleration SaComp=-1.5*DLc*Dth/cos(thc) */ 
    SaComp=-1.5*DLc*(-getX(Setp))*omega/cos(-getY(Setp)); 

    /* If there is a chance SaComp can't be allocated - reduce and allocate residual separately */
    if (fabs(SaComp)>amaxH-amaxS) { 
      Sa0=AS_SIGN(SaComp)*(amaxH-amaxS);
      addPhasor(Setp, 0.0, -omega*dt, SaComp-Sa0); /* Add compensation residual */
      AS_collapseSet(Setp, shp, amaxS);
      SaComp=Sa0;
      sts |= AS_ERR_ACC_MARGIN;
      if (verbose >= AS_VERB_ERR)
	printf("AntiSway/Engine_man: Partial compensation through residual phasor addition: %f SaComp needed, %f available |amax margin|\n",SaComp,amaxH-amaxS);
    }
    /* Adjust set so that uCommand is still met, given the compensation phasor */
    sts |= makeSet_man(Setp, shp, uCommand-SaComp*dt, uR, amaxS, omega, verbose); 

    /* Add the compensation phasor */
    addPhasor(Setp, 0.0, -omega*dt, SaComp); 
    cleanSet(Setp);

  }

  /* If no hoisting -  adjust set if new command or new omega */
  else if (newCall || hoisted) { 
    sts |= makeSet_man(Setp, shp, uCommand, uR, amaxS, omega, verbose); 
  }    

  /* Step up time and return error status */
  *aRp=timeStepSet(Setp, omega*dt); 
  return sts;
}


/* Function: AS_Engine_auto
 *
 * ::Engine for automatic mode set processing::
 * Takes all necessary variables from AntiSway_exec to create or change the phasor set.
 * Returns an error status variable, and writes autostatus in *astsp. Writes the current acceleration in *aRp.
 */

unsigned int AS_Engine_auto( int newCall, int hoisting, int hoisted, int verbose, pwr_sClass_Ssab_AntiSway *other, const AS_shaper *shp,
			     double xCommand, double xR, double uR, double DLc, double omega, double amaxH, double amaxS, double umaxH, double umaxS, double umin, double dt,
			     double *aRp, unsigned int *astsp, AS_phasorSet *Setp )
{

  double thisExt, otherExt, SaComp;
  unsigned int sts=0;

  if (newCall || (hoisting || hoisted)) {
    
    if (hoisting) { //add indirect compensation phasor to this object.
      /* Calculate compensation acceleration SaComp=-1.5*DLc*Dth/cos(thc) */ 
      SaComp=-1.5*DLc*(-getX(Setp))*omega/cos(-getY(Setp)); 
      addPhasor(Setp, 0.0, -omega*dt, SaComp); /* Add compensation phasor */
      AS_collapseSet(Setp, shp, amaxS);
    }

    //make new set, regardless of other
    sts |= makeSet_auto(Setp, shp, astsp, (xCommand-xR), 0.0, uR, amaxH, amaxS, umaxH, umaxS, umin, omega, verbose, 1);

    //set needmatch flag if in 2D
    if (*astsp & AS_AUTO_2D)
      *astsp |= AS_AUTO_NEEDMATCH;
  }

  else if ((*astsp & AS_AUTO_2D) && (other->autostatus & AS_AUTO_NEEDMATCH)) {
    
    // match this if other is longer
    thisExt=((Setp->N > 0) ? -(Setp->ph[Setp->N - 1].phiEnd) : 0.0);
    otherExt=((AS_OTHER_SETP->N > 0) ? -(AS_OTHER_SETP->ph[AS_OTHER_SETP->N - 1].phiEnd) : 0.0);
    if ((otherExt-thisExt)/omega > AS_AUTO_MATCH_LIM)
      sts |= makeSet_auto(Setp, shp, astsp, (xCommand-xR), otherExt, uR, amaxH, amaxS, umaxH, umaxS, umin, omega, verbose, 1);
    else if ((otherExt-thisExt)/omega < -AS_AUTO_MATCH_LIM) //other is shorter, set this needmatch flag
      *astsp |= AS_AUTO_NEEDMATCH;
    other->autostatus &= (~AS_AUTO_NEEDMATCH); //in all cases reset other needmatch flag
  }

  /* Calculate current reference acceleration and step up time. */
  *aRp=timeStepSet(Setp, omega*dt);

  return sts;
}


/* Internal function: makeSet_man 
 *
 * ::Manual mode wrapper function for mergeSet::
 * Reflects the set and allocates phasors backwards (end allocation) if a non-collapse shaper is used, and end allocation is possible.
 * End allocation can be used instead of collapse when uCommand is decreased before it has been attained.
 * Returns the status from mergeSet.
 */

static unsigned int makeSet_man(AS_phasorSet *Setp, const AS_shaper *shp, double uCommand, double uc, double amax, double omega, int verbose)
{
  unsigned int sts = 0;
  AS_shaper *shr;
  double reflectionPoint;  
  /* SuDiff=uCommand-uc - Setp->extSum/omega; */ /* The velocity change that must be allocated: (Ref. Vel - current vel.) - already allocated. */

  if (AS_SHP_COLLAPSEOK || Setp->N > AS_COLLAPSE_LIM) { /* shaper ok with AS_collapseSet, or too many phasors in set */
    AS_collapseSet(Setp, shp, amax);
  }
  else if ( fabs(uCommand-uc) < fabs(Setp->extSum/omega) && Setp->N > 0 ) { /* Non-collapse shaper. End allocation. */ 
    reflectionPoint=0.0;
    reflectSet(Setp,&reflectionPoint);
    shr=reflectShaper(shp);
    sts |= mergeSet(Setp, shr, (uCommand-uc - Setp->extSum/omega), amax, omega, 1, verbose);
    reflectSet(Setp,&reflectionPoint);
    free(shr);
    if (!(sts & AS_ERR_MERGE_OFFSETLIM)) /* End allocation succeeded all the way - exit*/
      return sts;
  } 
  sts |= mergeSet(Setp, shp, (uCommand-uc - Setp->extSum/omega), amax, omega, 0, verbose); /* Collapsed or non-collapsed set front allocation */
  return sts;
}


/* Internal function: makeSet_auto 
 *
 * ::Automatic mode set maker::
 * Will adjust the set so that a distance SxDiff is travelled, and so that the final reference velocity equals zero, 
 * given the current (ideal) velocity. The phase extension (travel time) of the set will match ext_match as precisely as possible.
 * If ext_match equals zero, the extension (time) will obviously be minimized.
 */

static unsigned int makeSet_auto(AS_phasorSet *Setp, const AS_shaper *shp, unsigned int *astsp, double SxDiff, double ext_match, double uc, 
				 double amaxH, double amaxS, double umaxH, double umaxS, double umin, double omega, int verbose, int overlap)
{
  unsigned int sts=0;
  double uCmd, Sxad, sfact, ext_a, phi_d, ext_travel, ext_min, uCmd_minext;
  AS_phasorSet aSet;
  AS_phasorSet dSet;

  aSet.ph=NULL;
  AS_emptySet(&aSet);
  dSet.ph=NULL;
  AS_emptySet(&dSet);  
  AS_collapseSet(Setp, shp, amaxS);   /* Collapse current set, regardless of whether shaper works optimally with collapse or not. */
  
  /* Loop over domain of command velocities */

  for (sfact=-1.0, ext_min=DBL_MAX, ext_travel=DBL_MAX, uCmd_minext=uc; sfact<=1.0; sfact+=AS_UAUTO_INCREMENT) {
    assignSet(&aSet,Setp); /* create work copy */
    uCmd=sfact*umaxS;
    sts |= mergeSet(&aSet, shp, (uCmd-uc - aSet.extSum/omega), amaxS, omega, 0, 0);
    sts |= mergeSet(&dSet, shp, uCmd, amaxS, omega, 0, 0); /* will be reflected, shifted and inverted, which is why -uCmd is not used */
    if (!sts && uCmd!=0.0) { /* If no error from mergeSet, and non-zero uCmd - continue on. */
      /* Calculate the distance that will be travelled during acceleration and deceleration. */
      Sxad = delta_x(&aSet, uc, 1.0/omega); /* distance travelled during acc. */
      Sxad += delta_x(&dSet, 0.0, 1.0/omega); /* add distance travelled during dec. Note that 0.0 is used as uc. */

      if (aSet.N == 0) /* No phasors allocated for the acceleration (u already equals uCmd) */
	ext_a=0.0;
      else
	ext_a=-(aSet.ph[aSet.N - 1].phiEnd); /* phase duration of acc=(end of last phasor) */    
      /* Determine phase location for deceleration = -omega*time_for_dec */
      phi_d=-(omega*(SxDiff-Sxad)/uCmd + ext_a);
      /* Determine total travel phase extension */
      if ((phi_d <= 0.0) && (overlap || -phi_d >= ext_a)) { /* Will only tolerate negative (or zero) phase (i.e. positive time), and deceleration start after acc if not overlap */
	if (dSet.N > 0) 
	  ext_travel=-(phi_d + dSet.ph[dSet.N - 1].phiEnd); /* total travel phase extension = offset_dec + ext_dec */
	else /* Empty set needs special treatment. Situation should not occur, however.. */
	  ext_travel=-phi_d;
	if (fabs(ext_travel-ext_match) < ext_min) { /* Found new minimum extension */
	  ext_min=fabs(ext_travel-ext_match);
	  uCmd_minext=uCmd;
	}
      }
    }
    else /* Reset error from mergeSet */
      sts=0; 

    AS_emptySet(&dSet);
    AS_emptySet(&aSet);
  }

  /* Check that a minimum positive extension was actually found before using uCmd_minext. */
  if (ext_min != DBL_MAX) {

    /* Use uCmd that yields min time */
    uCmd=uCmd_minext;
    assignSet(&aSet,Setp);
    sts=0;
    sts |= mergeSet(&aSet, shp, (uCmd-uc - aSet.extSum/omega), amaxS, omega, 0, verbose);
    sts |= mergeSet(&dSet, shp, uCmd, amaxS, omega, 0, verbose); /* will be reflected, shifted and inverted, which is why -uCmd is not used */
    /* Calculate the distance that will be travelled during acceleration and deceleration. */
    Sxad = delta_x(&aSet, uc, 1.0/omega); /* distance travelled during acc. */
    Sxad += delta_x(&dSet, 0.0, 1.0/omega); /* add distance travelled during dec. Note that 0.0 is used as uc. */
    if (aSet.N == 0) /* No phasors allocated for the acceleration (u already equals uCmd) */
      ext_a=0.0;
    else
      ext_a=-(aSet.ph[aSet.N - 1].phiEnd); /* phase duration of acc=(end of last phasor) */    
    /* Determine phase location for deceleration = -omega*time_for_dec */
    phi_d=-(omega*(SxDiff-Sxad)/uCmd+ext_a);
  
    /* Allocate phasors to final set. Acc part and Dec part. Collect status */
    reflectShiftAndInvertSet(&dSet, phi_d); /* Create the deceleration part by reflecting, shifting and inverting the phasors */
    assignSet(Setp,&aSet); /* Replace Set with the acceleration part */
    AS_emptySet(&aSet); /* get rid of aSet */
    appendSet(Setp, &dSet); /* Append the deceleration part */
    AS_emptySet(&dSet); /* get rid of dSet */
    qsort(Setp->ph, Setp->N, sizeof(AS_phasor), AS_cmpPhasors);
    cleanSet(Setp);

    /* Check for unallowed acc */
    if (supAmax(Setp, shp->AMax*amaxH)) {
      if (AS_SHP_NEG) { /* Negative shaper - redo with non-negative (dpulse or ddpulse) */
	*astsp |= AS_AUTO_REMADE_SHP;
	if (AS_SHP_ROBUST)
	  sts |= makeSet_auto(Setp, AS_SetupIST(AS_DDPULSE), astsp, SxDiff, ext_match, uc, amaxH, amaxS, umaxH, umaxS, umin, omega, verbose, overlap);
	else 
	  sts |= makeSet_auto(Setp, AS_SetupIST(AS_DPULSE), astsp, SxDiff, ext_match, uc, amaxH, amaxS, umaxH, umaxS, umin, omega, verbose, overlap);
      }
      else if (overlap) { /* Overlap - redo without overlap */
	*astsp |= AS_AUTO_REMADE_OVERLAP;
	sts |= makeSet_auto(Setp, shp, astsp, SxDiff, ext_match, uc, amaxH, amaxS, umaxH, umaxS, umin, omega, verbose, 0);
      }
      else {/* No more tricks in the bag.. Give up. */
	*astsp |= AS_AUTO_AMAX_FAILURE;
	sts |= mergeSet(Setp, shp, (0.0-uc - Setp->extSum/omega), amaxS, omega, 0, verbose);
	if (verbose >= AS_VERB_ERR)
	  printf("\nAntiSway/makeSet_auto: Could not make set without exceeding amax. Will set zero command velocity. amaxH=%f\n",shp->AMax*amaxH);
      }
    }
    /* Check for too low velocities */
    else if (subUmin(Setp, uc, 1.0/omega, umin)) { /* umin never exceeded */
      if (ext_match != 0.0) {/* if matching a given ext, redo without matching */
	*astsp |= AS_AUTO_REMADE_MATCH;
	sts |= makeSet_auto(Setp, shp, astsp, SxDiff, 0.0, uc, amaxH, amaxS, umaxH, umaxS, umin, omega, verbose, overlap);
      }
      else {
	*astsp |= AS_AUTO_UMIN_FAILURE;
	sts |= mergeSet(Setp, shp, (0.0-uc - Setp->extSum/omega), amaxS, omega, 0, verbose);
	if (verbose >= AS_VERB_ERR)
	  printf("\nAntiSway/makeSet_auto: Could not make set with uR > umin. Will set zero command velocity. umin=%f\n",umin);
	}
    }
    /* Check for too high velocities */
    else if (supUmax(Setp, uc, 1.0/omega, umaxH)) {
      if (overlap) {/* Overlap - redo without overlap */
	*astsp |= AS_AUTO_REMADE_OVERLAP;
	sts |= makeSet_auto(Setp, shp, astsp, SxDiff, ext_match, uc, amaxH, amaxS, umaxH, umaxS, umin, omega, verbose, 0);
      }
      else /* Resign.. use the set with umax violation (will be cut down in exec) */
	*astsp |= AS_AUTO_UMAX_FAILURE;
    }
  }

  /* No minimum positive extension found.. */
  else { 
    *astsp |= AS_AUTO_POSEXT_FAILURE;
    sts |= mergeSet(Setp, shp, (0.0-uc - Setp->extSum/omega), amaxS, omega, 0, verbose);
    if (verbose >= AS_VERB_ERR)
      printf("\nAntiSway/makeSet_auto: Could not find a minimum positive extension. Will set zero command velocity.\n");
  }  

  return sts;
}


/* Internal function: mergeSet 
 *
 * ::Main function for allocating phasors to attain uCommand::
 * Merges one or several shaper sets with the (possibly collapsed) phasor set so that a new reference velocity can be attained. 
 * Minor differences in execution if the set is reflected (e.g. cannot allocate behind time zero, so a limit offset is used)
 * Returns an error status.
 */

static unsigned int mergeSet(AS_phasorSet *Setp, const AS_shaper *shp, double SuDiff, double amax, double omega, int reflected, int verbose) 
{
  double ext, aRatio, uDiff, offset, S, tmp, offsetlim;
  int n, count, addCount;

  /* Initialize variables */
  S=AS_SIGN(SuDiff); /* Extracting the sign */
  uDiff=fabs(SuDiff); /* Absolute value */
   
  offset=0.0; /* Phase offset. Positive*/
  offsetlim=(reflected ? (shp->phi[shp->N - 1] - Setp->ph[Setp->N - 1].phiEnd) : DBL_MAX);
  count = 0;
  addCount = 0;
  while (fabs(uDiff) > AS_EPSILON && (offsetlim-offset > AS_EPSILON && count < AS_MERGE_ITER_LIM)) { /* Allocate phasors until uCommand is attained */
    ext=shp->DMin; /* Maximum phase extension of added phasors, initialized to DMin of shaper. Positive */
    aRatio=(reflected ? 1.0 : (shp->AMin + shp->AMax)/shp->AMin); /* Maximum possible relative magnitude of acceleration. Positive */
    for (n=0; n < shp->N; n++) /* Repeat for all the pulses in the shaper */
      phaseInterval(Setp, shp->phi[n]-offset, S*shp->A[n]*amax, AS_SIGN(S*shp->A[n])*AS_MAX(1.0,fabs(shp->A[n]))*amax, &aRatio, &ext, reflected);
    if (aRatio>0.0) { /* Possible to allocate phasors at current offset value ? */
      tmp=omega/(aRatio*shp->ASum*amax);
      ext=AS_MIN(ext,uDiff*tmp);
      uDiff-=ext/tmp; /* count down uDiff, the velocity change remaining to place out */
      for (n=0; n < shp->N; n++,addCount++) { /* allocate phasors at shaper locations  */
	tmp=shp->phi[n]-offset;
	addPhasor(Setp, tmp, tmp-ext, S*aRatio*shp->A[n]*amax); /* add phasor at phi[n]-offset of extension ext and */
                                                              /* acceleration sign*ratio*shaperMag*amax to the set */
      }
      cleanSet(Setp);
    }
    offset+=ext; /* count up offset */
    count++;
  } /* End of while loop */

  if (verbose >= AS_VERB_ALL)
    printf("\nAntiSway/mergeSet exited while loop, count=%d. Added %d phasors totally.\n",count,addCount);

  if (offsetlim-offset <= AS_EPSILON) /* Could not allocate phasors further (should be possible only in reflected mode). */
    return AS_ERR_MERGE_OFFSETLIM;

  if (count>=AS_MERGE_ITER_LIM) {
    if (verbose >= AS_VERB_ERR)
      AS_diagnose(Setp,omega,reflected,SuDiff,uDiff);
    return AS_ERR_MERGE_ITER;
  }

  return 0;
}


/* Internal function: phaseInterval
 *
 * ::Phase interval controller used by mergeSet::
 * Function that will check if a given phase angle phi is within the interval of a phasor in the set.
 * If so, the ratio of the acceleration that can be allocated anyway is returned in aRatio.
 * The value stored in ext will correspond to the distance to phiEnd if within an interval, and 
 * to next phiStart if not within an interval. 
 * Minor differences if reflected set: aRatio set to zero if phi is before the set start so that the set is really shortened, and not just elongated even more.
 *                                     Same if phi is beyond last phasor. 
 */

static void phaseInterval(const AS_phasorSet *Setp, double phi, double SaWanted, double SamaxS, double *aRatio, double *ext, int reflected) 
{
  int cont;
  const AS_phasor *php;

  php=Setp->ph;
  cont=1;
  while (php < Setp->ph + Setp->N && cont) {
    if ( phi - php->phiEnd > AS_EPSILON ) {/* Found first phasor located after phi in time */
      if ( phi - php->phiStart > AS_EPSILON ) { /* in front of phasor (i.e. not within, or at phiStart of phasor) ? */
	*ext=AS_MIN(*ext, phi-php->phiStart);
	*aRatio=( (php==Setp->ph && reflected) ? 0.0 : AS_MIN(*aRatio, SamaxS/SaWanted) ); /* Set ratio to zero if in reflected mode, and in front of FIRST phasor */
      }
      else { /* Within phasor */
	*ext=AS_MIN(*ext, phi - php->phiEnd);
	*aRatio=AS_MIN( *aRatio , AS_MAX(0.0,(SamaxS - php->Sa)/SaWanted) );
      }
      cont=0; /* Do not continue further */
    }
    else /* Current phasor before phi in time. Step up phasor pointer to look at next. */ 
      php++; 
  }
  if (php == Setp->ph + Setp->N) /* Found no phasors after phi in time - Need to adjust aRatio. ext is fine.*/
    *aRatio=(reflected ? 0.0 : AS_MIN(*aRatio, SamaxS/SaWanted)); /* Set ratio to zero if in reflected mode */
}


/* Internal function: cleanSet
 *
 * ::Cleaning function used when phasors have been allocated::
 * Will remove junk phasors with zero phase extension,
 * split overlapping phasors, and join together adjacent phasors
 * with equal acceleration. Called by mergeSet.
 */

static void cleanSet(AS_phasorSet *Setp) 
{ 
  double s1,s2,e1,e2,Sa1,Sa2;
  int n;
  const AS_phasor *php;
   
  if (Setp->N <= 0)
    return;

  /* Delete zero extension phasors */
  for (n=0; n < Setp->N; n++) {
    php=Setp->ph + n;
    if ( AS_MIN(fabs(php->phiEnd - php->Phi) , fabs(php->phiStart - php->Phi)) < AS_EPSILON )
      removePhasor(Setp, n--); /* n is stepped down so that the subsequent phasor will not be skipped in the next loop run */
  }

  /* Find overlaps and split. Will render two or three phasors, depending on the type of overlap. */  
  php=Setp->ph;
  while (php+1 < Setp->ph + Setp->N) {
    if ((php+1)->phiStart - php->phiEnd > AS_EPSILON) {/* overlap: end1 is beyond start2 */
      s1=php->phiStart;
      s2=(php+1)->phiStart;
      e1=php->phiEnd;
      e2=(php+1)->phiEnd;
      Sa1=php->Sa;
      Sa2=(php+1)->Sa;
      n=php - Setp->ph;
      removePhasor(Setp, n); /* Remove n */
      removePhasor(Setp, n); /* Remove the phasor that previously had index n+1 */
      if (fabs(s1-s2) < AS_EPSILON) { /* Same starting point */
	s1=0.5*(s1+s2);
	s2=s1;
      }
      else {
	if (s2>s1) /*2 starts before 1 in time */
	  addPhasor(Setp, s2, s1, Sa2);
	else /*1 starts before 2 in time */
	  addPhasor(Setp, s1, s2, Sa1);
      }
      if (fabs(e1-e2) < AS_EPSILON) {/* Same end point */
	e1=0.5*(e1+e2);
	e2=e1;
      }
      else {
	if (e2>e1) /*2 ends before 1 in time */
	  addPhasor(Setp, e2, e1, Sa1);
	else /*1 ends before 2 in time */
	  addPhasor(Setp, e1, e2, Sa2);
      }
      if (Sa1+Sa2!=0.0) /* The overlap part */
	addPhasor(Setp, AS_MIN(s1,s2), AS_MAX(e1,e2), Sa1+Sa2);
      php=Setp->ph; /* reset phasor counter/pointer */
    }
    else
      php++;
  }

  /* Join adjacent phasors with equal acceleration. */
  php=Setp->ph;
  while (php+1 < Setp->ph + Setp->N) {
    if (fabs(php->phiEnd - (php+1)->phiStart)<AS_EPSILON && php->Sa == (php+1)->Sa) { /* adjacent and equal acc. */
      Sa1=php->Sa;
      s1=php->phiStart;
      e2=(php+1)->phiEnd;
      n=php - Setp->ph;
      removePhasor(Setp, n); /* Remove n */
      removePhasor(Setp, n); /* Remove the phasor that previously had index n+1 */
      addPhasor(Setp, s1, e2, Sa1);
      php=Setp->ph;
    }
    else
      php++;
  }

  /* Correct extSum */
  for (php=Setp->ph, Setp->extSum=0.0; php < Setp->ph + Setp->N; php++) 
    Setp->extSum += (php->phiStart - php->phiEnd) * (php->Sa); 

} /* End of function cleanSet */


/* Function: AS_collapseSet
 * 
 * ::Set reducing function used before allocating new phasors::
 * Replaces the phasor set with simple phasors equivalent to its total vector sum.
 * The resulting phasors are the minimum phase/time representation of the set, given a=amax, located in the first
 * half plane, [0, -AS_PI). If one phasor is not enough (i.e. the one phasor would reach into the 2nd half plane, 
 * or into the positive half plane), the undesired part is reflected into the first negative half plane.
 * Example: If a newly allocated set is collapsed before it has been time incremented, the result will be an empty set.
 */

 void AS_collapseSet(AS_phasorSet *Setp, const AS_shaper *shp, double amax) 
{ 
  double XSum,YSum,PhiNew,RNew,extp5,sArg,base;
  int n;
  
  if (Setp->N == 0) /* If no phasors in set - do nothing */
    return;
  if (shp->ID == AS_NO_AS) { /* If NO_AS shaper - remove all */
    AS_emptySet(Setp);
    return;
  }

  /* sum up phasors */
  XSum=getX(Setp);
  YSum=getY(Setp);
  /* Remove phasors */
  AS_emptySet(Setp);

  /* do loop over half planes */
  base=0.0;
  n=0;
  do { 
    /* Calculate residual phasor */
    PhiNew=arctan360N(YSum, XSum);
    RNew=sqrt(XSum*XSum + YSum*YSum);
    sArg=RNew*AS_GRAV_ACCEL/(2.0*amax);
    if (sArg>1.0) { /* Too large arcsin argument - reduce! (residual allocated in next half plane) */
      RNew=2.0*amax/AS_GRAV_ACCEL;
      extp5=0.5*AS_PI;
    }
    else
      extp5=asin(sArg); /* Half extension of residual phasor, given a=amax */
    /* Make sure PhiNew is in the half plane [base, base-AS_PI) */ 
    while (PhiNew <= base-AS_PI) { 
      PhiNew+=AS_PI;
      RNew*=-1.0;
    }
    while (PhiNew > base) { 
      PhiNew-=AS_PI;
      RNew*=-1.0;
    }

    /* Add the calculated residual phasor. */
    if (PhiNew+extp5 > base) { /* Overlap into previous phase? */
      addPhasor(Setp, base, PhiNew-extp5, AS_SIGN(-RNew)*amax);
      addPhasor(Setp, -AS_PI+PhiNew+extp5, base-AS_PI, AS_SIGN(RNew)*amax);
    }
    else if (PhiNew-extp5 < base-AS_PI) { /* Overlap into next half plane ? */
      addPhasor(Setp, base, PhiNew-extp5+AS_PI, AS_SIGN(RNew)*amax);
      addPhasor(Setp, PhiNew+extp5, base-AS_PI, AS_SIGN(-RNew)*amax);
    }
    else /* No overlap */   
      addPhasor(Setp, PhiNew+extp5, PhiNew-extp5, AS_SIGN(-RNew)*amax);

    /* Count down sums. Loop only over the newly added phasors. (Using that phasors are added at end positions)*/
    for ( ; n < Setp->N; n++) {
      XSum -= Setp->ph[n].R*cos(Setp->ph[n].Phi);
      YSum -= Setp->ph[n].R*sin(Setp->ph[n].Phi);
    }
    base-=AS_PI;
  } while (sArg>1.0);

  cleanSet(Setp);
 
} /* end of function */


/* Internal function: timeStepSet
 *
 * ::Time incrementing function used by engines::
 * Performs one time step on the phasor set. All phasors are shifted
 * with an angle +omegadt. Necessary operations are performed on 
 * phasors at the time edge, i.e about to extend into positive phase. 
 * Function returns the current acceleration command. 
 */

static double timeStepSet(AS_phasorSet *Setp, double omegadt) 
{ 
  double ac,DextSum;
  int m;
  AS_phasor *php;

  ac=0.0;
  m=Setp->N;
  if (m>0) {
    for (php=Setp->ph; php < Setp->ph + m; php++) {
      php->Phi += omegadt;
      php->phiStart += omegadt;
      php->phiEnd += omegadt;
    }

    /* First phasor in set at time edge ? */
    while (Setp->ph[0].phiStart > 0.0) { 
      if (Setp->ph[0].phiEnd > 0.0) {  /* Completely over the edge ? If so remove. */
	DextSum=removePhasor(Setp, 0);
	Setp->extSum -= DextSum;
	ac+=DextSum/omegadt;
	if (Setp->N == 0)  /* Removed last phasor */
    	  break;
      }
      else { /* If not completely over the edge, adjust its members */
	DextSum=Setp->ph[0].phiStart*Setp->ph[0].Sa;
	Setp->ph[0].phiStart=0.0;
	Setp->ph[0].Phi = Setp->ph[0].phiEnd * 0.5;
	Setp->ph[0].R = -2.0*(Setp->ph[0].Sa)*sin(fabs(Setp->ph[0].Phi))/AS_GRAV_ACCEL;
	Setp->extSum -= DextSum;
	ac+=DextSum/omegadt;
      }
    }
  }
  return ac;
}


static double addPhasor(AS_phasorSet *Setp, double pS, double pE, double SaAdd) 
{
  int m=Setp->N;
  AS_phasor *php;

  if (m==0) /* Make sure ph is null if empty set */
    Setp->ph = NULL;

  /* Allocate memory for the new phasor */

  Setp->ph = (AS_phasor *) realloc(Setp->ph, (size_t) (m+1)*sizeof(AS_phasor));
  if (Setp->ph == NULL) {
    fprintf(stderr, "AntiSway/addPhasor: Could not allocate memory!\n");
    exit(99);
  }

  /* Initialize the new phasor */
  
  php = Setp->ph+m;
  php->Sa=SaAdd;
  php->phiStart=pS;
  php->phiEnd=pE;
  php->R=-2.0*SaAdd*sin(0.5*(pS-pE))/AS_GRAV_ACCEL;
  php->Phi=0.5*(pS+pE);
  (Setp->N)++;

  /* Sort the set */

  qsort(Setp->ph, Setp->N, sizeof(AS_phasor), AS_cmpPhasors);

  return (pS-pE)*SaAdd; /* Returns DextSum */
}


static double removePhasor(AS_phasorSet *Setp, int n) 
{
  double DextSum=0.0;
  int m=Setp->N;

  if (m>0 && (n>=0 && n<m)) {
    DextSum=(Setp->ph[n].phiStart - Setp->ph[n].phiEnd)*Setp->ph[n].Sa;
    Setp->ph[n].Phi = -DBL_MAX; /* Make sure phasor n is sorted to the last position */
    (Setp->N)--;
    qsort(Setp->ph, m, sizeof(AS_phasor), AS_cmpPhasors);
    m--;
    if (m > 0) {
      Setp->ph = (AS_phasor *) realloc(Setp->ph, (size_t) m*sizeof(AS_phasor));
      if (Setp->ph == NULL) {
	fprintf(stderr, "AntiSway/removePhasor: Could not allocate memory!\n");
	exit(99);
      }
    }
    else {/* Last phasor was removed */
      free(Setp->ph);
      Setp->ph=NULL;
    }
  } 

  return DextSum;
}



static void reflectSet(AS_phasorSet *Setp, double *reflectionPoint)
{  
  double pStmp;
  AS_phasor *php;
  int m;
  m=Setp->N;

  if (m==0) /* Empty set */
    return;

  if (*reflectionPoint == 0.0) /* Set reflection point (midpoint of set) if it has not been set in a previous call. */
    *reflectionPoint = 0.5*(Setp->ph[0].phiStart + Setp->ph[m-1].phiEnd);
  for (php = Setp->ph; php < Setp->ph + m; php++) {
    pStmp=php->phiStart;
    php->phiStart = 2.0*(*reflectionPoint) - php->phiEnd;
    php->phiEnd =  2.0*(*reflectionPoint) - pStmp;
    php->Phi = 2.0*(*reflectionPoint) - php->Phi;
  } 
  qsort(Setp->ph, m, sizeof(AS_phasor), AS_cmpPhasors);

}

void assignSet(AS_phasorSet *destp, const AS_phasorSet *srcp) // changed from static function to globally accessible. //changed back when auto and man were joined to one object.
{
  int sN=srcp->N;
  AS_emptySet(destp);

  if (sN == 0) /* Nothing to do if empty source set. Would cause exit in next step.. */
    return;

  destp->ph = (AS_phasor *) malloc((size_t) sN*sizeof(AS_phasor)); 
  if (destp->ph == NULL) {
    fprintf(stderr, "AntiSway/assignSet: Could not allocate memory!\n");
    exit(99);
  }
  memcpy(destp->ph, srcp->ph, (size_t) sN*sizeof(AS_phasor)); 
  destp->N=sN;
  destp->extSum=srcp->extSum;
}

static void appendSet(AS_phasorSet *destp, const AS_phasorSet *srcp)
{
  int dN=destp->N; /* Check that these numbers are not zero? */
  int sN=srcp->N;

  if ((sN+dN) == 0) /* Nothing to do if both destination and source set are empty. Would cause exit in next step.. */
    return;

  destp->ph = (AS_phasor *) realloc(destp->ph, (size_t) (dN+sN)*sizeof(AS_phasor));
  if (destp->ph == NULL) {
    fprintf(stderr, "AntiSway/appendSet: Could not allocate memory!\n");
    exit(99);
  }
  memcpy(destp->ph+dN, srcp->ph, (size_t) sN*sizeof(AS_phasor));
  destp->N += sN;
  destp->extSum += srcp->extSum;
}

static double delta_x(const AS_phasorSet *Setp, double uc, double romega)
{
  double delx,timeExt;
  const AS_phasor *php;

  if (Setp->N == 0) /* Empty set */
    return 0.0;

  /* Add the effect from the first phasor */
  php = Setp->ph;
  delx = uc*(0.0 - php->phiStart)*romega; //const vel part before phasor
  timeExt = (php->phiStart - php->phiEnd)*romega;  /* time extension of phasor = ext/omega */
  delx += uc*timeExt + 0.5*php->Sa*timeExt*timeExt; // travel distance of the phasor = const vel part + acc par 
  uc += php->Sa*timeExt; /* Velocity change induced by phasor */
  php++;

  /* Loop over all phasors except first*/
  for ( ; php < (Setp->ph+Setp->N); php++) {
    delx += uc*((php-1)->phiEnd - php->phiStart)*romega; //Constant velocity part before next phasor
    timeExt = (php->phiStart - php->phiEnd)*romega; /* time extension = ext/omega */
    delx += uc*timeExt + 0.5*php->Sa*timeExt*timeExt; /* travel distance of the phasor = const vel part + acc part */
    uc += php->Sa*timeExt; /* Velocity change induced by phasor */
  }
  
  return delx;
}

static void reflectShiftAndInvertSet(AS_phasorSet *Setp, double shift)
{
  double reflPoint;
  AS_phasor *php;

  reflPoint=0.0;
  reflectSet(Setp, &reflPoint);
  
  for (php=Setp->ph; php < (Setp->ph + Setp->N); php++) {
    php->phiStart += shift;
    php->Phi += shift;
    php->phiEnd += shift;
    php->Sa = -php->Sa;
    php->R = -php->R;
  }
}

static int supAmax(AS_phasorSet *Setp, double amax)
{
  const AS_phasor *php;
  
  for (php=Setp->ph; php < (Setp->ph + Setp->N); php++) {
    if (fabs(php->Sa) > amax)
      return 1;
  }

  return 0;
}

static int subUmin(AS_phasorSet *Setp, double uc, double romega, double umin)
{
  const AS_phasor *php;
  double maxu = fabs(uc);

  for (php=Setp->ph; php < (Setp->ph + Setp->N); php++) {
    uc += php->Sa*(php->phiStart - php->phiEnd)*romega;
    maxu=AS_MAX(maxu, fabs(uc));
  }
  return (umin > maxu);
}

static int supUmax(AS_phasorSet *Setp, double uc, double romega, double umax)
{
  const AS_phasor *php;
  double maxu = fabs(uc);

  for (php=Setp->ph; php < (Setp->ph + Setp->N); php++) {
    uc += php->Sa*(php->phiStart - php->phiEnd)*romega;
    maxu=AS_MAX(maxu, fabs(uc));
  }
  return (maxu > umax);
}

static AS_shaper *reflectShaper(const AS_shaper *shp) 
{
  double shaperEnd;
  int n,m;
  AS_shaper *shr;
  
  m=shp->N;
  shr = (AS_shaper *) malloc((size_t) sizeof(AS_shaper));
  if (shr == NULL) {
    fprintf(stderr, "AntiSway/reflectShaper: Could not allocate memory!\n");
    exit(99);
  }
  *shr=*shp;
  for (n=0, shaperEnd=shr->phi[m-1]; n<m; n++) {
    shr->phi[n] = shaperEnd - shp->phi[m-1-n];
    shr->A[n] = shp->A[m-1-n];
  }
  
  return shr;
}

static double getX(const AS_phasorSet *Setp)
{
  double X;
  const AS_phasor *php;
  for (php=Setp->ph, X=0.0; php < (Setp->ph + Setp->N); php++)
    X += php->R*cos(php->Phi);

  return X;    
}

static double getY(const AS_phasorSet *Setp)
{
  double Y;
  const AS_phasor *php;
  for (php=Setp->ph, Y=0.0; php < (Setp->ph + Setp->N); php++)
    Y += php->R*sin(php->Phi);

  return Y;    
}

static double arctan360N(double Y, double X) 
{
  /* Will return an angle phi such that tan(phi) = Y/X
   * The return value will be in the interval [0, -2AS_PI). */

  double phi;

  phi=atan2(Y,X);
  return (phi>0.0 ? phi-2.0*AS_PI : phi);
}



void AS_emptySet(AS_phasorSet *Setp)
{
  if (Setp->ph != NULL) {
    free(Setp->ph);
    Setp->ph=NULL;
  }
  Setp->N=0;
  Setp->extSum=0.0;
}

void AS_emptyQ(double **aQpp, double **acpp, double **aEpp)
{
  if (*aQpp != NULL) {
    free(*aQpp);
    *aQpp=NULL;
  }  
  *acpp=NULL;
  *aEpp=NULL;
}

int AS_cmpPhasors(const void *p1, const void *p2) 
{  
  /* Comparison function used by qsort. Note that one phasor is 
   * considered larger than another if it is centered later in time
   * i.e. if its Phi value is smaller (more negative).  */

  const AS_phasor *ph1 = (AS_phasor *) p1;
  const AS_phasor *ph2 = (AS_phasor *) p2;
  
  if (ph1->Phi > ph2->Phi) /* ph1 less than ph2 (ph1 before ph2 in time)*/
    return -1;
  else if (ph1->Phi < ph2->Phi) /* ph1 greater than ph2 (ph1 after ph2 in time)*/
    return 1;
  else /* equal */
    return 0;
}

void AS_enQMessage(AS_messageQ **objectmQp, int nlimit, const char *newMessage) 
{
// call this function as AS_enQMessage(&(AS_OBJ_MESSAGEQP(i)), newM)
// objectmQp - a local copy of a pointer to the object attribute messageQ[i], which is a pointer to an allocated AS_messageQ object
// *objectmQp - the object attribute messageQ[i] (a pointer to the first AS_messageQ instance in the queue)
// **objectmQp - the first AS_messageQ instance in the queue

  AS_messageQ *last, *enqueued; // copy of pointer to last item in queue.

  if (mQSize(*objectmQp) > nlimit)
    return;
 
  enqueued = (AS_messageQ *) malloc(sizeof(AS_messageQ));
  if (enqueued == NULL) {
    fprintf(stderr, "AS_enQMessage: Could not allocate memory!\n");
    exit(99);
  }
  enqueued->next = NULL;
  enqueued->count = 0;
  enqueued->mess = strdup(newMessage);
  if (enqueued->mess == NULL) {
    fprintf(stderr, "AS_enQMessage: Could not allocate memory!\n");
    exit(99);
  }
      
  last = mQLast(*objectmQp);
  if (last == NULL)
    *objectmQp = enqueued;
  else
    last->next = enqueued;
}

void AS_deQMessage(AS_messageQ **objectmQp)
{
  AS_messageQ *dequeued;
  
  if (*objectmQp != NULL) { // Exists message item in queue
    dequeued = *objectmQp;  // dequeued also points at first item, = object->messageQ[i]
    *objectmQp = (*objectmQp)->next; // equal to assignment object->messageQ[i] = object->messageQ[i]->next. Will be NULL if it is the last item in queue
    free(dequeued->mess);
    free(dequeued);
  }
  // function is called as AS_deQMessage(&(AS_OBJ_MESSAGEQP(i)))
  // objectmQp - a local copy of a pointer to the object attribute messageQ[i], which is a pointer to an allocated AS_messageQ object
  // *objectmQp - the object attribute messageQ[i] (a pointer to the first AS_messageQ instance in the queue)
  // **objectmQp - the first AS_messageQ instance in the queue 
}

static int mQSize(const AS_messageQ *mQ) // call this function as mQSize(AS_OBJ_MESSAGEQP(i)), i.e. pass a pointer to the first item in the queue
{
  int n = 0;
  while (mQ != NULL) {
    mQ = mQ->next;
    n++;
  }
  return n;
}

static AS_messageQ *mQLast(const AS_messageQ *mQ) // call this function as mQLast(AS_OBJ_MESSAGEQP(i)), i.e. pass a pointer to the first item in the queue
{
  if (mQ == NULL)
    return NULL;
  while (mQ->next != NULL)
    mQ = mQ->next;
  return (AS_messageQ *) mQ;
}


void AS_diagnose(const AS_phasorSet *Setp,double omega, int reflected,double SuDiff, double uDiff)
{
  printf("\n AntiSway/mergeSet: While loop count to exceeded limit!\n");
  printf("\n\n\n\n\t reflected=%d, SuDiff=%f uDiff=%f\n\n",reflected,SuDiff,uDiff);
  AS_displaySet(Setp,omega);
}

void AS_displaySet(const AS_phasorSet *Setp, double omega) 
{ 
  /* Display current contents of the set. */
  
  double XSum, YSum, uSum;
  const AS_phasor *php;
  int n;

  printf("\n\tAntiSway/AS_displaySet: Phasor Set: N=%d ext=%f\n\t-----------------------------\n",Setp->N, Setp->extSum);
  for (php=Setp->ph,n=0,XSum=0.0,YSum=0.0,uSum=0.0; n<Setp->N; php++,n++) {
    printf("\n%d.\tS=%f\tR=%f\ta=%f\n\tE=%f\tPhi=%f\n",n+1,php->phiStart,php->R,php->Sa,php->phiEnd,php->Phi);
    XSum+=php->R*cos(php->Phi);
    YSum+=php->R*sin(php->Phi);
    uSum+=(php->phiStart - php->phiEnd)/omega * (php->Sa);
  }
  printf("\tTotal Vector Sum: X=%f Y=%f\n\tTotal Velocity Change: deltaU=%f\n\n",XSum,YSum,uSum);
}

void AS_displayQ(const double *acp, const double *aEp, double uc, double dt) 
{ 
/* Display current contents of queue. */
  int n;
  double SaSum, am;
  const double *atmp;

  printf("\n\tAntiSway/displayQ: Acceleration Queue: \n\t------------------------\n\n");
  for (atmp=acp, n=0, am=10000.0, SaSum=0.0; atmp<aEp; atmp++,n++) {
    if (*atmp!=am) {
      am=*atmp;
      printf("a(%d)\t%f\n",n, *atmp);
    }
    SaSum+=*atmp;
  }  
  printf("\n\tTotal acc: ASum=%f uSum=%f uc=%f uFinal=%f dt=%f\n",SaSum, SaSum*dt, uc, SaSum*dt+uc,dt);
}

const AS_shaper* AS_SetupIST(int mode) 
{ 
  /* Function will define the impulse shaping technique used and return a pointer to the shaper.
   * Note that the actual shaper is kept in this function, not in the main function.
   * The maximum number of pulses in the shaper is defined by AS_ISTMAX. Remember to control the 
   * value of AS_ISTMAX when a new shaper is added! Note that some asymmetric shapers can
   * spend long time in the mergeSet while loop, as they keep allocating phasors of ever smaller extension.
   * If the shaper will work fine with AS_collapseSet, it should be added to the collapse condition AS_SHP_COLLAPSEOK in 
   * AS_phasor_methods.h
   *
   * struct shaper: 
   * 
   *    Defines an Impulse Shaping Technique (a shaper). Members are:
   *
   *    int ID        ID number of the shaper as defined by macros AS_NO_AS, AS_DPULSE etc
   *    int N         Number of pulses in the shaper. Room for ISTMAX pulses 
   *    double A[]    Array containing the relative magnitudes of the pulses (signed). 
   *                  Magnitude +/-1.0 corresponds to maximum acceleration. 
   *    double ASum   The sum of the relative magnitudes.
   *    double AMin   Minimum (relative) amplitude of pulses. Absolute value.
   *    double AMax   Maximum (relative) amplitude of pulses. Absolute value.
   *    double phi[]  Array containing the phase locations of the pulses.
   *    double DMin   Minimum phase distance between pulses. Absolute value.
   *    
   *
   */

  /* AS_ISTMAX = 5 currently */

  const static AS_shaper no_antisway = { AS_NO_AS, 1, {1.0, 0.0, 0.0, 0.0, 0.0}, 1.0, 1.0, 1.0, {0.0, 0.0, 0.0, 0.0, 0.0}, AS_PI };
  const static AS_shaper d_pulse = { AS_DPULSE, 2, {1.0, 1.0, 0.0, 0.0, 0.0}, 2.0, 1.0, 1.0, {0.0, -AS_PI, 0.0, 0.0, 0.0}, AS_PI };
  const static AS_shaper dd_pulse = { AS_DDPULSE, 3, {0.5, 1.0, 0.5, 0.0, 0.0}, 2.0, 0.5, 1.0, {0.0, -AS_PI, -2.0*AS_PI, 0.0, 0.0}, AS_PI };
  const static AS_shaper um_zv = { AS_UMZV, 3, {1.0, -1.0, 1.0, 0.0, 0.0}, 1.0, 1.0, 1.0, {0.0, -AS_PI/3.0, -AS_PI/1.5, 0.0, 0.0}, AS_PI/3.0 };
  const static AS_shaper um_zvd = { AS_UMZVD, 5, {1.0, -1.0, 1.0, -1.0, 1.0}, 1.0, 1.0, 1.0, {0.0, -0.56242392468373, -2.30111498488178, -4.03980604507982, -4.60222996976355}, 0.56242392468373 };
  const static AS_shaper zv_122 = { AS_ZV122, 3, {1.0, -2.0, 2.0, 0.0, 0.0}, 1.0, 1.0, 2.0, {0.0, -1.31811607165282, -1.82347658193698, 0.0, 0.0},  0.505360510284160 }; /* angles are 0, -AS_PI/2+asin(1/4), -AS_PI/2-asin(1/4) */
  const static AS_shaper zvd_122 = { AS_ZVD122, 5, {1.0, -2.0, 2.0, -2.0, 2.0}, 1.0, 1.0, 2.0, {0.0, -0.95759740098188, -1.74292794797482, -3.96664437406570, -4.26595739903213}, 0.29931302496643 };


  switch (mode) {
  case AS_NO_AS: 
    return &no_antisway;
    break;
  case AS_DPULSE: 
    return &d_pulse;
    break;
  case AS_DDPULSE: 
    return &dd_pulse;
    break;
  case AS_UMZV: 
    return &um_zv;
    break;
  case AS_UMZVD: 
    return &um_zvd;
    break;
  case AS_ZV122: 
    return &zv_122;
    break;
  case AS_ZVD122: 
    return &zvd_122;
    break;
  default:
    return NULL;
  }
  return NULL;
}  
