/* rt_plc_arithm.c -- <short description>

   PROVIEW/R
   Copyright (C) 1996 by Comator Process AB.

   <Description>.  */

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

