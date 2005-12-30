/* 
 * Proview   $Id: wb_c_ai_ai32up.c,v 1.1 2005-12-30 15:52:05 claes Exp $
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

/* wb_c_ai_ai32up.c -- work bench methods of the Ai_AI32uP class */

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
  pwr_sClass_Ai_AI32uP RCard;
  pwr_sdClass_Ai_AI32uP DCard;
  pwr_tUInt32 MaxChan;
  pwr_tUInt32 Chan;
  int i;
  
  if (Class != pwr_cClass_ChanAi && Class != pwr_cClass_ChanAit)
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
  pwr_sClass_Ai_AI32uP RCard;
  pwr_sdClass_Ai_AI32uP DCard;
  pwr_sClass_ChanAi ChanAi;
  pwr_sClass_ChanAit ChanAit;
  pwr_tUInt32 MaxChan;
  pwr_tUInt32 Chan;
  pwr_tString80 NewName;
  pwr_tString80 Description;
  pwr_tString80 Identity;
  pwr_tString80	DefName;
  pwr_sObject DefBody;
  pwr_tObjid DefObject;
  int i;
  
  if (Class != pwr_cClass_ChanAi && Class != pwr_cClass_ChanAit)
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
  case pwr_cClass_ChanAi:
    sts = ldh_ReadObjectBody(Session, Channel, "RtBody", &ChanAi,
      sizeof(ChanAi));
    if (EVEN(sts)) return sts;
    if (ChanAi.Description[0] != '\0') {
      sprintf(Description, ChanAi.Description, i);
      if (strlen(Description) <= sizeof(ChanAi.Description) - 1) {
	strcpy(ChanAi.Description, Description);
      }
    }
    if (ChanAi.Identity[0] != '\0') {
      sprintf(Identity, ChanAi.Identity, i);
      if (strlen(Identity) <= sizeof(ChanAi.Identity) - 1) {
	strcpy(ChanAi.Identity, Identity);
      }
    }

    ChanAi.Number = i;
    sts = ldh_SetObjectBody(Session, Channel, "RtBody", (char *)&ChanAi, sizeof(ChanAi));
    strcpy(DefName, "pwrb:Class-ChanAi-Defaults");
    break;
  case pwr_cClass_ChanAit:
    sts = ldh_ReadObjectBody(Session, Channel, "RtBody", &ChanAit,
      sizeof(ChanAit));
    if (EVEN(sts)) return sts;
    if (ChanAit.Description[0] != '\0') {
      sprintf(Description, ChanAit.Description, i);
      if (strlen(Description) <= sizeof(ChanAit.Description) - 1) {
	strcpy(ChanAit.Description, Description);
      }
    }
    if (ChanAit.Identity[0] != '\0') {
      sprintf(Identity, ChanAit.Identity, i);
      if (strlen(Identity) <= sizeof(ChanAit.Identity) - 1) {
	strcpy(ChanAit.Identity, Identity);
      }
    }

    ChanAit.Number = i;
    sts = ldh_SetObjectBody(Session, Channel, "RtBody", (char *)&ChanAit, sizeof(ChanAit));
    strcpy(DefName, "pwrb:Class-ChanAit-Defaults");
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
  pwr_sClass_Ai_AI32uP RCard;
  pwr_sClass_ChanAi ChanAi;
  pwr_sClass_ChanAit ChanAit;
  pwr_sdClass_Ai_AI32uP DCard;
  pwr_tUInt32 MaxChan;
  pwr_tUInt32 Chan;
  pwr_tString80 NewName;
  
  if (Class != pwr_cClass_ChanAi && Class != pwr_cClass_ChanAit)
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
  case pwr_cClass_ChanAi:
    sts = ldh_ReadObjectBody(Session, Channel, "RtBody", &ChanAi,
      sizeof(ChanAi));
    if (EVEN(sts)) return PWRB__SUCCESS;

    Chan = ChanAi.Number;
    break;
  case pwr_cClass_ChanAit:
    sts = ldh_ReadObjectBody(Session, Channel, "RtBody", &ChanAit,
      sizeof(ChanAit));
    if (EVEN(sts)) return PWRB__SUCCESS;

    Chan = ChanAit.Number;
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

pwr_dExport pwr_BindMethods(Ai_AI32uP) = {
  pwr_BindMethod(AnteAdopt),
  pwr_BindMethod(PostUnadopt),
  pwr_BindMethod(PostAdopt),
  pwr_BindMethod(SyntaxCheck),
  pwr_NullMethod
};
