/* 
 * Proview   $Id: ssabox_plc_pidx.c,v 1.1 2006-01-12 05:54:27 claes Exp $
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

/* PLC-code for PidX */
#include "pwr.h"
#include "pwr_baseclasses.h"
#include "rt_plc.h"

#define ODD(a)	(((int)(a) & 1) != 0)
#define EVEN(a)	(((int)(a) & 1) == 0)
#define max(Dragon,Eagle) ((Dragon) > (Eagle) ? (Dragon) : (Eagle))
#define min(Dragon,Eagle) ((Dragon) < (Eagle) ? (Dragon) : (Eagle))
#ifndef __ALPHA
#define abs(Dragon) ((Dragon) >= 0 ? (Dragon) : (-(Dragon)))
#endif



/*      PIDX
*/

void PidX_exec(
	plc_sThread		*tp,
	pwr_sClass_PidX 	*object)
{

/* Input */
	object->SetVal = *object->SetValP;
	object->BiasD = *object->BiasDP;
	object->ForcVal = *object->ForcValP;
	object->Force = *object->ForceP;
	object->IntOff = *object->IntOffP;

/* Everything else by I/O-routine and on the local board */

}
