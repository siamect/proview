/* 
 * Proview   $Id: rt_plc_data.c,v 1.4 2005-09-01 14:57:56 claes Exp $
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

#ifdef OS_ELN
#  include stdio
#  include string
#else
#  include <stdio.h>
#  include <string.h>
#endif

#include "pwr.h"
#include "rt_plc.h"
#include "pwr_baseclasses.h"

/* Nice functions */
#define ODD(a)  (((int)(a) & 1) != 0)
#define EVEN(a) (((int)(a) & 1) == 0)
#define max(Dragon,Eagle) ((Dragon) > (Eagle) ? (Dragon) : (Eagle))
#define min(Dragon,Eagle) ((Dragon) < (Eagle) ? (Dragon) : (Eagle))

typedef struct {
        pwr_tFloat32    *DataP;
        pwr_tObjid      Data_ObjId;
        } plc_t_DataInfoBrief;

/* 		PLC RUTINER			*/

/*_*
  @aref dpcollect DpCollect
*/
void DpCollect_exec(
  plc_sThread		*tp,
  pwr_sClass_DpCollect  *object)
{
	int		i;
	pwr_tBoolean	**InP;

	for (  i = 0; i < object->MaxIndex; i++)
	{
	  InP = (pwr_tBoolean **) ((char *) &object->DpIn1P +
		i * (sizeof(object->DpIn1) + sizeof(object->DpIn1P)));
	  object->Dp[i] = **InP;
	}
}

/*_*
  @aref apcollect ApCollect
*/
void ApCollect_exec(
  plc_sThread		*tp,
  pwr_sClass_ApCollect  *object)
{
	int		i;
	pwr_tFloat32	**InP;

	for (  i = 0; i < object->MaxIndex; i++)
	{
	  InP = (pwr_tFloat32 **) ((char *) &object->ApIn1P +
		i * (sizeof(object->ApIn1) + sizeof(object->ApIn1P)));
	  object->Ap[i] = **InP;
	}
}

/*_*
  @aref dpdistribute DpDistribute
*/
void DpDistribute_exec(
  plc_sThread		*tp,
  pwr_sClass_DpDistribute  *object)
{
	if ( *object->DataInP)
	  memcpy(  &object->DpOut1, *object->DataInP,
		min( object->MaxIndex, 24) * sizeof(pwr_tBoolean));
}

/*_*
  @aref apdistribute ApDistribute
*/
void ApDistribute_exec(
  plc_sThread		*tp,
  pwr_sClass_ApDistribute  *object)
{
	if ( *object->DataInP)
	  memcpy(  &object->ApOut1, *object->DataInP, 
		min( object->MaxIndex, 24) * sizeof(pwr_tFloat32));
}

/*_*
  @aref datacollect DataCollect
*/
void DataCollect_exec(
  plc_sThread		*tp,
  pwr_sClass_DataCollect  *object)
{
	int	i;
	plc_t_DataInfoBrief	**InP;

	for (  i = 0; i < object->MaxIndex; i++)
	{
	  InP = (plc_t_DataInfoBrief **) ((char *) &object->DataIn1P +
		i * (sizeof(object->DataIn1) + sizeof(object->DataIn1P)));
	  object->DataP[i] = (*InP)->DataP;
	  object->DataObjId[i] = (*InP)->Data_ObjId;
	}
}

