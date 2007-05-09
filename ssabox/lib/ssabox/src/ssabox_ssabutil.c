/* 
 * Proview   $Id: ssabox_ssabutil.c,v 1.3 2007-05-09 14:00:08 claes Exp $
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
 */

/************************************************************************
*
* Filename:		rs_ssabutil.c
*
*			Datum	Pgm.		Anm
* Reviderad		920724	CS		Skapad.
*
* Beskrivning:
*	Filen innehåller diverse rutiner för proview/R.
*
**************************************************************************/

/*_Include files_________________________________________________________*/

#ifdef OS_ELN
#include stdio
#include string
#else
#include <stdio.h>
#include <string.h>
#endif

#include "pwr.h"
#include "rt_gdh.h"
#include "ssabox_ssabutil.h"
#include "co_time.h"

/* Nice functions */
#define ODD(a)	(((int)(a) & 1) != 0)
#define EVEN(a)	(((int)(a) & 1) == 0)
#define max(Dragon,Eagle) ((Dragon) > (Eagle) ? (Dragon) : (Eagle))
#define min(Dragon,Eagle) ((Dragon) < (Eagle) ? (Dragon) : (Eagle))
#define abs(Dragon) ((Dragon) >= 0 ? (Dragon) : (-(Dragon)))


/* Global variable */

/* Local static variables */

/*************************************************************************
*
* Name:		ssabutil_lopnr_check()
*
* Type		int
*
* Type		Parameter	IOGF	Description
*
* Description:
*	Kontrollerar att checksiffran {r ok p} ett l|pnummer.
*	Returnerar0 om ej ok, 1 om ok.
*
**************************************************************************/

int	ssabutil_lopnr_check( pwr_tInt32	lopnr)
{
	pwr_tInt32	tst_lopnr;
	
	tst_lopnr = lopnr / 10;
	tst_lopnr = ssabutil_chksum_lop( tst_lopnr);

	if ( tst_lopnr != lopnr)
	  return 0;
	else
	  return 1;
}


/************************************************************************
*
* Namn:         ssabutil_chksum_lop
*
* Typ:          pwr_tInt32
*
* TYP           PARAMETER        IOGF   BESKRIVNING
* pwr_tInt32    ssabutil_chksum_lop O      Löpnummer med checksiffra.
* pwr_tInt32    lopnummer    	 I      Löpnumret som checksiffran skall
*                                       beräknas ifrån.
*
* Beskrivning:  Rutinen beräknar checksiffra i löpnummer.
*
*************************************************************************/
pwr_tInt32 ssabutil_chksum_lop(	pwr_tInt32 lopnummer)
{
	static  pwr_tInt16   lop_fig_weights[6] = {1,7,3,1,7,3};

	lopnummer=lopnummer*10 + 
              		ssabutil_chksum_calculate( lopnummer, 
						lop_fig_weights, 
						6);

	return(lopnummer);
}
/* END_OF ssabutil_chksum_lop */


/************************************************************************
*
* Namn:         ssabutil_chksum_kupong
*
* Typ:          pwr_tInt32
*
* TYP           PARAMETER        IOGF   BESKRIVNING
* pwr_tInt32    ssabutil_chksum_kupong O  Kupongnummer med checksiffra.
* pwr_tInt32    kupong_nummer    I      Kupongnumret som checksiffran skall
*                                       beräknas ifrån.
*
* Beskrivning:  Rutinen beräknar checksiffra i kupongnummer.
*
*************************************************************************/
pwr_tInt32 ssabutil_chksum_kupong(	pwr_tInt32 kupong_nummer)
{
	static  pwr_tInt16	kupong_fig_weights[5] = {1,3,9,1,7};

	kupong_nummer=kupong_nummer*10 + 
              		ssabutil_chksum_calculate( kupong_nummer, 
						kupong_fig_weights, 
						5);

	return(kupong_nummer);
}
/* END_OF ssabutil_chksum_kupong */


/************************************************************************
*
* Namn:         ssabutil_chksum_calculate
*
* Typ:          pwr_tInt32
*
* TYP           PARAMETER        IOGF   BESKRIVNING
* pwr_tInt32         ssabutil_chksum_
*		calculate	 O      Beräknad checksiffra.
* pwr_tInt32         value            I      Talet som checksiffran skall beräknas
*                                       ifrån.
* pwr_tInt16	*weights         I      Pekare till viktvektor.
* pwr_tInt16	num_figures      I      Antalet sifror i value.
*
*
* Beskrivning:  Rutinen beräknar checksiffra i följande steg.
*               1.  Varje siffra i value multipliceras med sin respektive
*                   vikt.
*               2.  Produkterna adderas.
*               3.  Checksiffra = Skillnaden till närmast högre tiotal
*
*************************************************************************/
pwr_tInt32 ssabutil_chksum_calculate(	pwr_tInt32 value,
					pwr_tInt16 *weights,
					pwr_tInt16 num_figures)
{
   pwr_tInt16   sum;         /* Arbetsvariabel */
   pwr_tInt16   *weightP;    /* Pekar på vikter */

   /* Beräkna perkaren till entalssiffran */

   weightP = weights + num_figures - 1;

   /* Beräkna den viktade summan av siffrorona i talet */

   for (sum=0; num_figures>0; num_figures--)
   {
       sum += (value % 10) * (*weightP--);
       value /= 10 ;
   }

   /* Beräkna skillnaden till närmast högre tiotal */

   sum = ((sum/10) + 1)*10 - sum;
   if (sum == 10) sum = 0;

   return (sum);
}
/* END_OF ssabutil_chksum_calculate  */


pwr_tStatus sutl_sleep( float time)
{
#ifdef OS_VMS
        int sts;
        sts = lib$wait(&time);
#elif OS_ELN
        LARGE_INTEGER   l_time;

        l_time.high = -1;
        l_time.low = - time * 10000000;
        ker$wait_any( NULL, NULL, &l_time);
#elif defined(OS_LYNX) || defined (OS_LINUX)
        pwr_tDeltaTime  p_time;

        time_FloatToD( &p_time, time);
        nanosleep( (pwr_tTime *)&p_time, NULL);
#endif

        return 1;
}
