/* 
 * Proview   $Id: wb_c_do_hvdo32.c,v 1.1 2005-12-30 15:52:05 claes Exp $
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

/* wb_c_do_hvdo32.c -- work bench methods of the Do_HVDO32 class. */

#include <stdio.h>
#include <string.h>
#include "wb_pwrs.h"
#include "wb_ldh_msg.h"
#include "wb_pwrb_msg.h"
#include "pwr_baseclasses.h"
#include "pwr_basecomponentclasses.h"
#include "pwr_ssaboxclasses.h"
#include "wb_ldh.h"
#include "wb_wsx.h"
#include "wb_nav_macros.h"


/*----------------------------------------------------------------------------*\
  Check if it's ok to adopt a new object.
\*----------------------------------------------------------------------------*/

static pwr_tStatus AnteAdopt (
  ldh_tSesContext Session,
  pwr_tObjid Card,	      /* current card object */
  pwr_tClassId Class	      /* class of child to adopt */
) {
  pwr_tStatus sts;
  pwr_sClass_Do_HVDO32 RCard;
  pwr_sdClass_Do_HVDO32 DCard;
  pwr_tUInt32 MaxChan;
  pwr_tUInt32 Chan;
  int i;
  
  if (Class != pwr_cClass_ChanDo)
    return PWRB__CHANCLASS;

  sts = ldh_ReadObjectBody(Session, Card, "RtBody", &RCard, sizeof(RCard));
  if (EVEN(sts)) return sts;

  sts = ldh_ReadObjectBody(Session, Card, "DevBody", &DCard, sizeof(DCard));
  if (EVEN(sts)) return sts;

  MaxChan = min(32, RCard.MaxNoOfChannels);
  for (i = 0, Chan = 1; i < MaxChan; i++, Chan <<= 1) {
    if ((DCard.ChannelAllocation & Chan) == 0)
      break;
  }

  if (i >= MaxChan)
    return PWRB__ALOCHAN;
  else
    return LDH__ADOPTRENAME;
}

/*----------------------------------------------------------------------------*\
  Adopt a new channel.
\*----------------------------------------------------------------------------*/

static pwr_tStatus PostAdopt (
  ldh_tSesContext Session,
  pwr_tObjid Card,	      /* current card object */
  pwr_tObjid Channel,
  pwr_tClassId Class	      /* class of child to adopt */
) {
  pwr_tStatus sts;
  pwr_sClass_Do_HVDO32 RCard;
  pwr_sdClass_Do_HVDO32 DCard;
  pwr_sClass_ChanDo ChanDo;
  pwr_tUInt32 MaxChan;
  pwr_tUInt32 Chan;
  pwr_tString80 NewName;
  pwr_tString80 Description;
  pwr_tString80 Identity;
  pwr_tString80	DefName;
  pwr_sObject DefBody;
  pwr_tObjid DefObject;
  int i;
  
  if (Class != pwr_cClass_ChanDo)
    return PWRB__CHANCLASS;

  sts = ldh_ReadObjectBody(Session, Card, "RtBody", &RCard, sizeof(RCard));
  if (EVEN(sts)) return sts;

  sts = ldh_ReadObjectBody(Session, Card, "DevBody", &DCard, sizeof(DCard));
  if (EVEN(sts)) return sts;

  MaxChan = min(32, RCard.MaxNoOfChannels);
  for (i = 0, Chan = 1; i < MaxChan; i++, Chan <<= 1) {
    if ((DCard.ChannelAllocation & Chan) == 0)
      break;
  }

  if (i >= MaxChan)
    return PWRB__ALOCHAN;

  /* allocate new channel */
  DCard.ChannelAllocation |= Chan;
  sts = ldh_SetObjectBody(Session, Card, "DevBody", (char *)&DCard, sizeof(DCard));
  if (EVEN(sts)) return sts;

  /*
    change attributes of channel

    NOTE !
    this should be done by a method of the channel object,
    but is not implemented in this version of PROVIEW/R.
  */

  switch (Class) {
  case pwr_cClass_ChanDo:
    sts = ldh_ReadObjectBody(Session, Channel, "RtBody", &ChanDo,
      sizeof(ChanDo));
    if (EVEN(sts)) return sts;
    if (ChanDo.Description[0] != '\0') {
      sprintf(Description, ChanDo.Description, i);
      if (strlen(Description) <= sizeof(ChanDo.Description) - 1) {
	strcpy(ChanDo.Description, Description);
      }
    }
    if (ChanDo.Identity[0] != '\0') {
      sprintf(Identity, ChanDo.Identity, i);
      if (strlen(Identity) <= sizeof(ChanDo.Identity) - 1) {
	strcpy(ChanDo.Identity, Identity);
      }
    }

    ChanDo.Number = i;
    sts = ldh_SetObjectBody(Session, Channel, "RtBody", (char *)&ChanDo, sizeof(ChanDo));
    strcpy(DefName, "pwrb:Class-ChanDo-Defaults");
    break;
  }

  /* change name of channel */
  
  sts = ldh_NameToObjid(Session, &DefObject, DefName);
  if (EVEN(sts)) return PWRB__SUCCESS;
  sts = ldh_ReadObjectBody(Session, DefObject, "SysBody", &DefBody,
    sizeof(DefBody));
  if (DefBody.Name[0] != '\0') {
    sprintf(NewName, DefBody.Name, i+1);
    NewName[31] = '\0';
    sts = ldh_SetObjectName(Session, Channel, NewName);
  }

  return PWRB__SUCCESS;
}

/*----------------------------------------------------------------------------*\
  Unadopt a channel.
\*----------------------------------------------------------------------------*/

static pwr_tStatus PostUnadopt (
  ldh_tSesContext Session,
  pwr_tObjid Card,	      /* current card object */
  pwr_tObjid Channel,
  pwr_tClassId Class	      /* class of child to adopt */
) {
  pwr_tStatus sts;
  pwr_sClass_Do_HVDO32 RCard;
  pwr_sClass_ChanDo ChanDo;
  pwr_sdClass_Do_HVDO32 DCard;
  pwr_tUInt32 MaxChan;
  pwr_tUInt32 Chan;
  pwr_tString80 NewName;
  
  if (Class != pwr_cClass_ChanDo)
    return PWRB__SUCCESS;

  sts = ldh_ReadObjectBody(Session, Card, "RtBody", &RCard, sizeof(RCard));
  if (EVEN(sts)) return PWRB__SUCCESS;

  sts = ldh_ReadObjectBody(Session, Card, "DevBody", &DCard, sizeof(DCard));
  if (EVEN(sts)) return PWRB__SUCCESS;

  MaxChan = min(32, RCard.MaxNoOfChannels);

  /*
    get attributes of channel

    NOTE !
    this should be done by a method of the channel object,
    but is not implemented in this version of PROVIEW/R.
  */

  switch (Class) {
  case pwr_cClass_ChanDo:
    sts = ldh_ReadObjectBody(Session, Channel, "RtBody", &ChanDo,
      sizeof(ChanDo));
    if (EVEN(sts)) return PWRB__SUCCESS;

    Chan = ChanDo.Number;
    break;
  }

  /* deallocate channel */
  if (Chan > MaxChan)
    return PWRB__SUCCESS;

  DCard.ChannelAllocation &= ~(1 << Chan);
  sts = ldh_SetObjectBody(Session, Card, "DevBody", (char *)&DCard, sizeof(DCard));
  if (EVEN(sts)) return PWRB__SUCCESS;

  sts = ldh_GetUniqueObjectName(Session, Channel, NewName);
  if (EVEN(sts)) return PWRB__SUCCESS;

  sts = ldh_SetObjectName(Session, Channel, NewName);
  if (EVEN(sts)) return PWRB__SUCCESS;

  return PWRB__SUCCESS;
}
/*----------------------------------------------------------------------------*\
  Syntax check.
\*----------------------------------------------------------------------------*/

static pwr_tStatus SyntaxCheck (
  ldh_tSesContext Session,
  pwr_tObjid Object,	      /* current object */
  int *ErrorCount,	      /* accumulated error count */
  int *WarningCount	      /* accumulated waring count */
) {
  pwr_tStatus sts;

  sts = wsx_CheckCard( Session, Object, ErrorCount, WarningCount,
		wsx_mCardOption_DevName);
  if (EVEN(sts)) return sts;

  return PWRB__SUCCESS;
}

/*----------------------------------------------------------------------------*\
  Every method to be exported to the workbench should be registred here.
\*----------------------------------------------------------------------------*/

pwr_dExport pwr_BindMethods(Do_HVDO32) = {
  pwr_BindMethod(AnteAdopt),
  pwr_BindMethod(PostUnadopt),
  pwr_BindMethod(PostAdopt),
  pwr_BindMethod(SyntaxCheck),
  pwr_NullMethod
};
