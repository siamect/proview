/* 
 * Proview   $Id: wb_c_object.c,v 1.9 2006-03-31 14:24:34 claes Exp $
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

/* wb_c_object.c -- work bench methods of the Object class. */

#include <stdio.h>
#include <string.h>
#include "pwr.h"
#include "wb_pwrs.h"
#include "wb_ldh_msg.h"
#include "wb_ldh.h"
#include "wb_dutl.h"
#include "co_cdh.h"
#include "co_dcli.h"
#include "co_api.h"
#include <X11/Intrinsic.h>
#include "wb_api.h"
#include "wb_login.h"

static pwr_tStatus CopyObject (
  ldh_sMenuCall *ip
) {
  pwr_tStatus	    retsts  = LDH__SUCCESS;
  pwr_tStatus	    sts	    = LDH__SUCCESS;
  pwr_sMenuButton   mb;
  int		    i;
  int		    j;
  ldh_eDest	    Dest;
  pwr_tObjid	    DestObject;
  pwr_tObjid	    NewObject;
  pwr_tBoolean	    Self = 0;
  
  sts = ldh_ReadObjectBody(ip->PointedSession,
    ip->ItemList[ip->ChosenItem].MenuObject,
    "SysBody", &mb, sizeof(pwr_sMenuButton));

  if (mb.MethodArguments[0][0] != '\0') {
    if (strcmp(mb.MethodArguments[0], "Before") == 0)
      Dest = ldh_eDest_Before;
    else if (strcmp(mb.MethodArguments[0], "After") == 0)
      Dest = ldh_eDest_After;
    else if (strcmp(mb.MethodArguments[0], "First") == 0)
      Dest = ldh_eDest_IntoFirst;
    else if (strcmp(mb.MethodArguments[0], "Last") == 0)
      Dest = ldh_eDest_IntoLast;
    else
      Dest = ldh_eDest__;
  }

  for (i = 0; i < ip->SelectCount; i++) {
    if (cdh_ObjidIsEqual(ip->Selected[i].Objid, ip->Pointed.Objid))
      Self = 1;
    if (cdh_ObjidIsEqual(ip->Selected[i].Objid, pwr_cNObjid))
      break;
  }

  if (!Self)
    DestObject = ip->Pointed.Objid;

  if (Dest == ldh_eDest_After || Dest == ldh_eDest_IntoFirst) {
    for (j = i - 1; j >= 0; j--) {
      if (Self)
	DestObject = ip->Selected[j].Objid;
      sts = ldh_CopyObject(ip->PointedSession, &NewObject, NULL, ip->Selected[j].Objid, DestObject, Dest); 
      if (EVEN(sts)) retsts = sts;
    }
  } else {
    for (j = 0; j < i; j++) {
      if (Self)
	DestObject = ip->Selected[j].Objid;
      sts = ldh_CopyObject(ip->PointedSession, &NewObject, NULL, ip->Selected[j].Objid, DestObject, Dest); 
      if (EVEN(sts)) retsts = sts;
    }
  }
  return retsts;
}

static int IsOkCopyObject (
  ldh_sMenuCall *ip,
  pwr_sMenuButton *mbp
) {
  pwr_tStatus	    sts	    = LDH__SUCCESS;
  pwr_tStatus	    retsts  = LDH__SUCCESS;
  ldh_eDest	    Dest;
  int		    i;
  pwr_tBoolean	    Self = 0;
  pwr_tObjid	    DestObject;
  
  if (mbp->MethodArguments[0][0] != '\0') {
    if (strcmp(mbp->MethodArguments[0], "Before") == 0)
      Dest = ldh_eDest_Before;
    else if (strcmp(mbp->MethodArguments[0], "After") == 0)
      Dest = ldh_eDest_After;
    else if (strcmp(mbp->MethodArguments[0], "First") == 0)
      Dest = ldh_eDest_IntoFirst;
    else if (strcmp(mbp->MethodArguments[0], "Last") == 0)
      Dest = ldh_eDest_IntoLast;
    else
      Dest = ldh_eDest__;
  }

  for (i = 0; i < ip->SelectCount; i++) {
    if (cdh_ObjidIsEqual(ip->Selected[i].Objid, ip->Pointed.Objid))
      Self = 1;
    if (cdh_ObjidIsNull(ip->Selected[i].Objid))
      break;
  }

  if (!Self)
    DestObject = ip->Pointed.Objid;

  for (i = 0; i < ip->SelectCount; i++) {
    if (cdh_ObjidIsNull(ip->Selected[i].Objid))
      break;
    if (Self)
      DestObject = ip->Selected[i].Objid;
    sts = ldh_IsOkCopyObject(ip->PointedSession, ip->Selected[i].Objid,
      DestObject, Dest); 
    if (EVEN(sts))
      retsts = sts;
  }

  return ODD(retsts);
}

static pwr_tStatus CopyObjectTree (
  ldh_sMenuCall	    *ip
) {
  pwr_tStatus	    sts	  = LDH__SUCCESS;
  pwr_sMenuButton   mb;
  ldh_eDest	    Dest;
  pwr_tBoolean	    Self  = 0;
  int		    i;
  
  sts = ldh_ReadObjectBody(ip->PointedSession, ip->ItemList[ip->ChosenItem].MenuObject,
    "SysBody", &mb, sizeof(pwr_sMenuButton));

  if (mb.MethodArguments[0][0] != '\0') {
    if (strcmp(mb.MethodArguments[0], "Before") == 0)
      Dest = ldh_eDest_Before;
    else if (strcmp(mb.MethodArguments[0], "After") == 0)
      Dest = ldh_eDest_After;
    else if (strcmp(mb.MethodArguments[0], "First") == 0)
      Dest = ldh_eDest_IntoFirst;
    else if (strcmp(mb.MethodArguments[0], "Last") == 0)
      Dest = ldh_eDest_IntoLast;
    else
      Dest = ldh_eDest__;
  }

  for (i = 0; i < ip->SelectCount; i++) {
    if (cdh_ObjidIsEqual(ip->Selected[i].Objid, ip->Pointed.Objid))
      Self = 1;
    if (cdh_ObjidIsNull(ip->Selected[i].Objid))
      break;
  }

  sts = ldh_CopyObjectTrees(ip->PointedSession, ip->Selected,
    ip->Pointed.Objid, Dest, Self, 0); 
  return sts;
}

static int IsOkCopyObjectTree (
  ldh_sMenuCall *ip,
  pwr_sMenuButton *mbp
) {
  pwr_tStatus	    sts	    = LDH__SUCCESS;
  pwr_tStatus	    retsts  = LDH__SUCCESS;
  ldh_eDest	    Dest;
  int		    i;
  pwr_tBoolean	    Self    = 0;
  pwr_tObjid	    DestObject;
  
  if (mbp->MethodArguments[0][0] != '\0') {
    if (strcmp(mbp->MethodArguments[0], "Before") == 0)
      Dest = ldh_eDest_Before;
    else if (strcmp(mbp->MethodArguments[0], "After") == 0)
      Dest = ldh_eDest_After;
    else if (strcmp(mbp->MethodArguments[0], "First") == 0)
      Dest = ldh_eDest_IntoFirst;
    else if (strcmp(mbp->MethodArguments[0], "Last") == 0)
      Dest = ldh_eDest_IntoLast;
    else
      Dest = ldh_eDest__;
  }

  for (i = 0; i < ip->SelectCount; i++) {
    if (cdh_ObjidIsEqual(ip->Selected[i].Objid, ip->Pointed.Objid))
      Self = 1;
    if (cdh_ObjidIsNull(ip->Selected[i].Objid))
      break;
  }

  if (!Self)
    DestObject = ip->Pointed.Objid;

  for (i = 0; i < ip->SelectCount; i++) {
    if (cdh_ObjidIsNull(ip->Selected[i].Objid))
      break;
    if (Self)
      DestObject = ip->Selected[i].Objid;
    sts = ldh_IsOkCopyObject(ip->PointedSession, ip->Selected[i].Objid,
      DestObject, Dest); 
    if (EVEN(sts))
      retsts = sts;
  }

  return ODD(retsts);
}

static int IsOkCreateObject (
  ldh_sMenuCall *ip,
  pwr_sMenuButton *mbp
) {
  pwr_tStatus	    sts;
  pwr_tClassId	    Class = cdh_ClassObjidToId(ip->Selected[0].Objid);
  ldh_eDest	    Dest;

  if (mbp->MethodArguments[0][0] != '\0') {
    if (strcmp(mbp->MethodArguments[0], "Before") == 0)
      Dest = ldh_eDest_Before;
    else if (strcmp(mbp->MethodArguments[0], "After") == 0)
      Dest = ldh_eDest_After;
    else if (strcmp(mbp->MethodArguments[0], "First") == 0)
      Dest = ldh_eDest_IntoFirst;
    else if (strcmp(mbp->MethodArguments[0], "Last") == 0)
      Dest = ldh_eDest_IntoLast;
    else
      Dest = ldh_eDest__;
  }

  sts = ldh_IsOkCreateObject(ip->PointedSession, Class,
    ip->Pointed.Objid, Dest);

  return ODD(sts);
}

static pwr_tStatus CreateObject (
  ldh_sMenuCall *ip
) {
  pwr_tStatus	    sts;
  pwr_tClassId	    Class = cdh_ClassObjidToId(ip->Selected[0].Objid);
  pwr_tObjid	    Object;
  pwr_sMenuButton   mb;
  ldh_eDest	    Dest;

  sts = ldh_ReadObjectBody(ip->PointedSession, ip->ItemList[ip->ChosenItem].MenuObject,
    "SysBody", &mb, sizeof(pwr_sMenuButton));

  if (mb.MethodArguments[0][0] != '\0') {
    if (strcmp(mb.MethodArguments[0], "Before") == 0)
      Dest = ldh_eDest_Before;
    else if (strcmp(mb.MethodArguments[0], "After") == 0)
      Dest = ldh_eDest_After;
    else if (strcmp(mb.MethodArguments[0], "First") == 0)
      Dest = ldh_eDest_IntoFirst;
    else if (strcmp(mb.MethodArguments[0], "Last") == 0)
      Dest = ldh_eDest_IntoLast;
    else
      Dest = ldh_eDest__;
  }

  sts = ldh_CreateObject(ip->PointedSession, &Object, NULL, Class,
    ip->Pointed.Objid, Dest);
  return sts;
}

static void DeleteObjectCb (
  void *CallbackData,
  dutl_eDialogButton Button
) {
  ldh_sMenuCall *mc = (ldh_sMenuCall *)CallbackData;

  if (Button == dutl_eDialogButton_1) 
    ldh_DeleteObjectTree(mc->PointedSession, mc->Pointed.Objid);

  XtFree ((char *) mc);
}

static pwr_tStatus DeleteObject (
  ldh_sMenuCall *ip
) {

  pwr_tStatus sts;
  char msg[128];
  pwr_tString80 name;
  int size;
  ldh_sMenuCall *mc;

  sts = ldh_DeleteObject(ip->PointedSession, ip->Pointed.Objid);
  if (sts == LDH__HAS_CHILD) {
    sts = ldh_ObjidToName(ip->PointedSession, ip->Pointed.Objid, 
      ldh_eName_Object, name, sizeof(name), &size);
    sprintf(msg,
      "Object '%s' has children!\nDo you want to delete the whole object tree ?",
      name);
    mc = (ldh_sMenuCall *) XtMalloc (sizeof(*mc));
    *mc = *ip;
    
    dutl_MessageDialog (ip->WindowContext, dutl_eDialogType_Question,
      dutl_eDialogModality_Modeless, "Delete Object", msg, "  Yes  ", "  No  ", NULL,
      dutl_eDialogButton_2, DeleteObjectCb, (void *)mc);
  }

  return sts;
}

static void DeleteObjectsCb (
  void *CallbackData,
  dutl_eDialogButton Button
) {
  ldh_sMenuCall	  *mc	  = (ldh_sMenuCall *)CallbackData;
  int		  i;

  if (Button == dutl_eDialogButton_1) {
    for (i = 0; mc->SelectCount; i++) {	/* count number of selected objects */
      if (cdh_ObjidIsNull(mc->Selected[i].Objid))
	break;
      ldh_DeleteObjectTree(mc->PointedSession, mc->Selected[i].Objid);
    }
  }

  XtFree((char *) mc->Selected);
  XtFree((char *) mc);

}

static pwr_tStatus DeleteObjects (
  ldh_sMenuCall *ip
) {
  pwr_tStatus	  sts = LDH__SUCCESS;
  ldh_sMenuCall	  *mc;
  char		  msg[] = "Do you want to delete selected objects ?";

  mc = (ldh_sMenuCall *) XtMalloc (sizeof(*mc));
  *mc = *ip;
    
  mc->Selected = (void *)XtMalloc((ip->SelectCount + 1) * sizeof(pwr_sAttrRef));
  memcpy(mc->Selected, ip->Selected, (ip->SelectCount + 1) * sizeof(pwr_sAttrRef));

  dutl_MessageDialog (ip->WindowContext, dutl_eDialogType_Question,
    dutl_eDialogModality_Modeless, "Delete Selected Objects", msg, "  Yes  ", "  No  ", NULL,
    dutl_eDialogButton_2, DeleteObjectsCb, (void *)mc);

  return sts;
}

#if 0
static void HelpClassErrorCb ()
{
  printf ("HelpClass error\n");
}
static pwr_tStatus HelpClass (
  ldh_sMenuCall *ip
) {
  pwr_tStatus sts;
  ldh_sSessInfo Info;
  int size;
  pwr_tFullName Name;

  sts = ldh_ObjidToName(ip->PointedSession, ip->Pointed.Objid, 
    ldh_eName_Object, Name, sizeof(Name), &size); 
  if (EVEN(sts))
    return sts;
  DXmHelpSystemDisplay (help_ctx, HELP_FILE, "topic", Name, 
    HelpClassErrorCb, NULL); 

  return sts;
}
#endif

static pwr_tStatus MoveObject (
  ldh_sMenuCall *ip
) {
  pwr_tStatus	    retsts  = LDH__SUCCESS;
  pwr_tStatus	    sts	    = LDH__SUCCESS;
  pwr_sMenuButton   mb;
  int		    i;
  int		    j;
  ldh_eDest	    Dest;
  
  sts = ldh_ReadObjectBody(ip->PointedSession, ip->ItemList[ip->ChosenItem].MenuObject,
    "SysBody", &mb, sizeof(pwr_sMenuButton));

  if (mb.MethodArguments[0][0] != '\0') {
    if (strcmp(mb.MethodArguments[0], "Before") == 0)
      Dest = ldh_eDest_Before;
    else if (strcmp(mb.MethodArguments[0], "After") == 0)
      Dest = ldh_eDest_After;
    else if (strcmp(mb.MethodArguments[0], "First") == 0)
      Dest = ldh_eDest_IntoFirst;
    else if (strcmp(mb.MethodArguments[0], "Last") == 0)
      Dest = ldh_eDest_IntoLast;
    else
      Dest = ldh_eDest__;
  }

  for (i = 0;; i++) {	/* count number of selected objects */
    if (cdh_ObjidIsNull(ip->Selected[i].Objid))
      break;
  }

  if (Dest == ldh_eDest_After || Dest == ldh_eDest_IntoFirst) {
    for (j = i - 1; j >= 0; j--) {
      sts = ldh_MoveObject(ip->PointedSession, ip->Selected[j].Objid,
	ip->Pointed.Objid, Dest); 
      if (EVEN(sts)) retsts = sts;
    }
  } else {
    for (j = 0; j < i; j++) {
      sts = ldh_MoveObject(ip->PointedSession, ip->Selected[j].Objid,
	ip->Pointed.Objid, Dest); 
      if (EVEN(sts)) retsts = sts;
    }
  }
  return retsts;
}

static int IsOkMoveObject (
  ldh_sMenuCall *ip,
  pwr_sMenuButton *mbp
) {
  pwr_tStatus	    sts	    = LDH__SUCCESS;
  pwr_tStatus	    retsts  = LDH__SUCCESS;
  ldh_eDest	    Dest;
  int		    i;
  
  if (mbp->MethodArguments[0][0] != '\0') {
    if (strcmp(mbp->MethodArguments[0], "Before") == 0)
      Dest = ldh_eDest_Before;
    else if (strcmp(mbp->MethodArguments[0], "After") == 0)
      Dest = ldh_eDest_After;
    else if (strcmp(mbp->MethodArguments[0], "First") == 0)
      Dest = ldh_eDest_IntoFirst;
    else if (strcmp(mbp->MethodArguments[0], "Last") == 0)
      Dest = ldh_eDest_IntoLast;
    else
      Dest = ldh_eDest__;
  }

  for (i = 0;; i++) {
    if (cdh_ObjidIsNull(ip->Selected[i].Objid))
      break;
    sts = ldh_IsOkMoveObject(ip->PointedSession, ip->Selected[i].Objid,
      ip->Pointed.Objid, Dest); 
    if (EVEN(sts))
      retsts = sts;
  }

  return ODD(retsts);
}

static pwr_tStatus OpenObject (
  ldh_sMenuCall *ip
) {
  pwr_tBoolean WritePriv = FALSE;
  pwr_tStatus sts;
  ldh_sSessInfo Info;

  sts = ldh_GetSessionInfo (ip->PointedSession, &Info);
  WritePriv = (ODD(sts) && Info.Access == ldh_eAccess_ReadWrite) &&
		ldh_LocalObject( ip->PointedSession, ip->Pointed.Objid);


  watt_new( ip->WindowContext, ip->EditorContext, ip->PointedSession,
		ip->Pointed, WritePriv, 1,  WritePriv, NULL);
  return 1;
}

static pwr_tStatus OpenTemplate (
  ldh_sMenuCall *ip
) {
  pwr_tBoolean WritePriv = FALSE;
  pwr_tStatus sts;
  ldh_sSessInfo Info;
  pwr_tFullName Name;
  pwr_tObjid Template;  
  pwr_sAttrRef Aref;
  int size;

  sts = ldh_ObjidToName(ip->PointedSession, ip->Pointed.Objid, 
    ldh_eName_Default, Name, sizeof(Name), &size); 
  if (EVEN(sts))
    return sts;

  strcat(Name, "-Template");
  sts = ldh_NameToObjid(ip->PointedSession, &Template, Name); 
  if (EVEN(sts))
    return sts;
  
  sts = ldh_GetSessionInfo (ip->PointedSession, &Info);
  WritePriv = (ODD(sts) && Info.Access == ldh_eAccess_ReadWrite) &&
		ldh_LocalObject( ip->PointedSession, Template);

  Aref = cdh_ObjidToAref( Template);
  watt_new( ip->WindowContext, ip->EditorContext, ip->PointedSession,
		Aref, WritePriv, 1,  WritePriv, NULL);

  return sts;
}

static pwr_tStatus SetDefaults (
  ldh_sMenuCall *ip
) {
  pwr_tBoolean WritePriv = FALSE;
  pwr_tStatus sts;
  ldh_sSessInfo Info;
  pwr_tFullName Name;
  pwr_tObjid Object;  
  int size;

  sts = ldh_ObjidToName(ip->PointedSession, ip->Pointed.Objid, 
    ldh_eName_Hierarchy, Name, sizeof(Name), &size); 
  if (EVEN(sts))
    return sts;

  strcat(Name, "-Defaults");
  sts = ldh_NameToObjid(ip->PointedSession, &Object, Name); 
  if (EVEN(sts))
    return sts;
  
  sts = ldh_GetSessionInfo (ip->PointedSession, &Info);
  WritePriv = (ODD(sts) && Info.Access == ldh_eAccess_ReadWrite) &&
		ldh_LocalObject( ip->PointedSession, Object);
  watt_new( ip->WindowContext, ip->EditorContext, ip->PointedSession,
		ip->Pointed, WritePriv, 1,  WritePriv, NULL);

  return sts;
}

//
//  Help for class i palette
//
static pwr_tStatus ClassHelp( ldh_sMenuCall *ip) 
{
  pwr_tStatus sts;
  int size;
  char cname[32];
  char cmd[200];
  pwr_tCid cid;

  cid = cdh_ClassObjidToId( ip->Pointed.Objid);

  sts = ldh_ClassIdToName( ip->PointedSession, cid, cname, sizeof(cname), &size);
  if ( EVEN(sts)) return sts;

  if ( cname[0] == '$')
    sprintf( cmd, "help %s /strict", &cname[1]);
  else
    sprintf( cmd, "help %s /strict", cname);

  wnav_command( ip->EditorContext, cmd);
  return 1;
}

//
// Help for class of an object in wnav
//
static pwr_tStatus HelpClass( ldh_sMenuCall *ip) 
{
  pwr_tStatus sts;
  int size;
  char cname[32];
  char vname[32];
  char cmd[200];
  pwr_tCid cid;
  pwr_tVid vid;
  
  sts = ldh_GetAttrRefTid( ip->PointedSession, &ip->Pointed, &cid);
  if ( EVEN(sts)) return sts;

  sts = ldh_ClassIdToName( ip->PointedSession, cid, cname, sizeof(cname), &size);
  if ( EVEN(sts)) return sts;

  vid = cdh_CidToVid(cid);
  if ( cdh_cManufactClassVolMin <= vid && vid <= cdh_cManufactClassVolMax) {
    /* Get help file for this volume */
    sts = ldh_VolumeIdToName( ldh_SessionToWB(ip->PointedSession), vid, vname, sizeof(vname), &size);
    if ( EVEN(sts)) return sts;

    cdh_ToLower( vname, vname);
    sprintf( cmd, "help %s /helpfile=\"$pwr_exe/%s/%s_xtthelp.dat\"/strict", cname, 
	     lng_get_language_str(), vname);

    wnav_command( ip->EditorContext, cmd);
    return 1;
  }

  if ( cname[0] == '$')
    sprintf( cmd, "help %s /strict", &cname[1]);
  else
    sprintf( cmd, "help %s /strict", cname);

  wnav_command( ip->EditorContext, cmd);
  return 1;
}

static pwr_tStatus HelpClassFilter( ldh_sMenuCall *ip) 
{
  return 1;
}

static pwr_tStatus Help( ldh_sMenuCall *ip) 
{
  pwr_tStatus sts;
  int size;
  char cmd[200];
  char *topic;

  sts = ldh_GetObjectPar( ip->PointedSession, ip->Pointed.Objid, "RtBody",
			  "HelpTopic", &topic, &size);
  if ( EVEN(sts))
    sts = ldh_GetObjectPar( ip->PointedSession, ip->Pointed.Objid, "SysBody",
			    "HelpTopic", &topic, &size);
  if ( EVEN(sts)) return LDH__SUCCESS;

  if ( strcmp( topic, "") == 0) {
    free( topic);
    return LDH__SUCCESS;
  }

  sprintf( cmd, "help %s /strict", topic);
  free( topic);

  wnav_command( ip->EditorContext, cmd);
  return 1;
}

static pwr_tStatus HelpFilter( ldh_sMenuCall *ip) 
{
  pwr_tStatus sts;
  int size;
  char *topic;

  sts = ldh_GetObjectPar( ip->PointedSession, ip->Pointed.Objid, "RtBody",
			  "HelpTopic", &topic, &size);
  if ( EVEN(sts))
    sts = ldh_GetObjectPar( ip->PointedSession, ip->Pointed.Objid, "SysBody",
			    "HelpTopic", &topic, &size);
  if ( EVEN(sts)) return 0;

  if ( strcmp( topic, "") == 0) {
    free( topic);
    return 0;
  }

  return 1;
}

static pwr_tStatus Cast( ldh_sMenuCall *ip) 
{
  pwr_tStatus sts;
  pwr_tCid cid;
  ldh_sAttrRefInfo info;
  
  sts = ldh_GetAttrRefTid( ip->PointedSession, &ip->Pointed, &cid);
  if ( EVEN(sts)) return sts;

  sts = ldh_GetAttrRefInfo( ip->PointedSession, &ip->Pointed, &info);
  if ( EVEN(sts)) return sts;

  if ( info.flags & PWR_MASK_CASTATTR) {
    wcast_new( ip->EditorContext, ip->WindowContext, "Casting Window", ip->PointedSession,
		ip->Pointed, &sts);
  }
  return 1;
}

static pwr_tStatus CastFilter( ldh_sMenuCall *ip) 
{
  pwr_tStatus sts;
  ldh_sAttrRefInfo info;
  
  sts = ldh_GetAttrRefInfo( ip->PointedSession, &ip->Pointed, &info);
  if ( EVEN(sts)) return sts;

  if ( info.flags & PWR_MASK_CASTATTR)
    return 1;
  return 0;
}

static pwr_tStatus Disable( ldh_sMenuCall *ip) 
{
  pwr_tStatus sts;
  ldh_sAttrRefInfo info;
  
  sts = ldh_GetAttrRefInfo( ip->PointedSession, &ip->Pointed, &info);
  if ( EVEN(sts)) return sts;

  if ( info.flags & PWR_MASK_DISABLEATTR) {
    sts = ldh_DisableAttribute( ip->PointedSession, &ip->Pointed, 1);
    if ( EVEN(sts)) return sts;
  }
  return 1;
}

static pwr_tStatus DisableFilter( ldh_sMenuCall *ip) 
{
  pwr_tStatus sts;
  ldh_sAttrRefInfo info;
  
  sts = ldh_GetAttrRefInfo( ip->PointedSession, &ip->Pointed, &info);
  if ( EVEN(sts)) return sts;

  if ( info.flags & PWR_MASK_DISABLEATTR)
    return 1;
  return 0;
}

static pwr_tStatus configure_object( ldh_sMenuCall *ip, pwr_sAttrRef *aref, 
				     unsigned int disable_mask)
{
  pwr_tStatus 	sts;
  pwr_tCid 	cid;
  ldh_sParDef 	*bodydef;
  int 		rows;
  int		i;
  pwr_sAttrRef  aaref, daref;
  pwr_tDisableAttr disable;
  int disableattr_cnt = 0;
  
  sts = ldh_GetAttrRefTid( ip->PointedSession, aref, &cid);
  if ( EVEN(sts)) return sts;

  sts = ldh_GetObjectBodyDef( ip->PointedSession, cid, "RtBody", 1,
				  &bodydef, &rows);
  if (EVEN(sts)) return sts;

  for ( i = 0; i < rows; i++) {
    if ( bodydef[i].Par->Param.Info.Flags & PWR_MASK_DISABLEATTR) {
      // Disable or enable dependent on mask
      sts = ldh_ArefANameToAref( ip->PointedSession, aref, bodydef[i].ParName, &aaref);
      if ( EVEN(sts)) return sts;

      daref = cdh_ArefToDisableAref( &aaref);

      if ( (1 << disableattr_cnt++) & disable_mask)
	disable = 1;
      else
	disable = 0;

      sts = ldh_WriteAttribute( ip->PointedSession, &daref, (void *)&disable, 
				sizeof(disable));
      if ( EVEN(sts)) return sts;
    }
#if 0
    if ( bodydef[i].Par->Param.Info.Flags & PWR_MASK_CLASS) {
      // Examine object attribute
      sts = ldh_ArefANameToAref( ip->PointedSession, aref, bodydef[i].ParName, &aaref);
      if ( EVEN(sts)) return sts;

      sts = configure_object( ip, &aaref, disable_mask, disableattr_cnt);
      if ( EVEN(sts)) return sts;
    }
#endif
  }
  free( (char *)bodydef);

  return LDH__SUCCESS;
}

static pwr_tStatus configure_object_reset( ldh_sMenuCall *ip, pwr_sAttrRef *aref,
					   pwr_sMenuButton *mb)
{
  pwr_tStatus 	sts;
  pwr_tCid 	cid;
  ldh_sParDef 	*bodydef;
  int 		rows;
  int		i, j;
  pwr_sAttrRef  aaref, daref;
  pwr_tDisableAttr disable = 0;
  char 		vect[10][80];
  int 		vect_cnt;
  int 		skip;
  char		*s;
  
  // Attribute objects in argument 2 are not controlled by the configure method
  if ( mb) {
    vect_cnt = dcli_parse( mb->MethodArguments[2], ",", "", (char *)vect, 
			   sizeof( vect) / sizeof( vect[0]), 
			   sizeof( vect[0]), 0);
    for ( j = 0; j < vect_cnt; j++)
      dcli_trim( vect[j], vect[j]);
  }
  else
    vect_cnt = 0;

  sts = ldh_GetAttrRefTid( ip->PointedSession, aref, &cid);
  if ( EVEN(sts)) return sts;

  sts = ldh_GetObjectBodyDef( ip->PointedSession, cid, "RtBody", 1,
				  &bodydef, &rows);
  if (EVEN(sts)) return sts;

  for ( i = 0; i < rows; i++) {

    // Check if attribute is controlled by this method
    skip = 0;
    if ( mb) {
      // Remove leading Super.
      s = bodydef[i].ParName;
      while ( strncmp( s, "Super.", 6) == 0)
	s += 6;
      for ( j = 0; j < vect_cnt; j++) {
	if ( cdh_NoCaseStrcmp( vect[j], s) == 0) {
	  // This object should not be reset
	  skip = 1;
	  break;
	}
      }
      if ( skip)
	continue;
    }

    if ( bodydef[i].Par->Param.Info.Flags & PWR_MASK_DISABLEATTR) {
      // Enable all attributes
      sts = ldh_ArefANameToAref( ip->PointedSession, aref, bodydef[i].ParName, &aaref);
      if ( EVEN(sts)) return sts;

      daref = cdh_ArefToDisableAref( &aaref);

      sts = ldh_WriteAttribute( ip->PointedSession, &daref, (void *)&disable, 
				sizeof(disable));
      if ( EVEN(sts)) return sts;
    }

    if ( bodydef[i].Par->Param.Info.Flags & PWR_MASK_CLASS) {
      // Reset object attribute
      sts = ldh_ArefANameToAref( ip->PointedSession, aref, bodydef[i].ParName, &aaref);
      if ( EVEN(sts)) return sts;

      sts = configure_object_reset( ip, &aaref, 0);
      if ( EVEN(sts)) return sts;
    }
  }
  free( (char *)bodydef);

  return LDH__SUCCESS;
}

static pwr_tStatus ConfigureComponent( ldh_sMenuCall *ip) 
{
  pwr_tStatus sts;
  unsigned int disable_mask;
  pwr_sMenuButton   mb;
  pwr_tEnum graph_configuration;
  pwr_sAttrRef aaref;
  char vect[10][80];
  char item[3][80];
  int i;
  int nr;
  int vect_cnt;
  
  sts = ldh_ReadObjectBody(ip->PointedSession, ip->ItemList[ip->ChosenItem].MenuObject,
    "SysBody", &mb, sizeof(pwr_sMenuButton));

  // Reset previoius disable configuration
  configure_object_reset( ip, &ip->Pointed, &mb);

  // Set disable attributes from argument 0
  vect_cnt = dcli_parse( mb.MethodArguments[0], ",", "", (char *)vect, 
		   sizeof( vect) / sizeof( vect[0]), 
		   sizeof( vect[0]), 0);
  
  for ( i = 0; i < vect_cnt; i++) {
    nr = dcli_parse( vect[i], " 	", "", (char *)item,
		   sizeof( item) / sizeof( item[0]), 
		   sizeof( item[0]), 0);
    if ( nr == 1) {

      if ( sscanf( item[0], "%d", &disable_mask) != 1)
	graph_configuration = 0;

      sts = configure_object( ip, &ip->Pointed, disable_mask);
      if ( EVEN(sts)) return sts;
    }
    else if ( nr == 2) {
      pwr_tAName aname;

      if ( sscanf( item[1], "%d", &disable_mask) != 1)
	disable_mask = 0;

      strcpy( aname, item[0]);

      sts = ldh_ArefANameToAref( ip->PointedSession, &ip->Pointed, aname, 
				 &aaref);
      if ( ODD(sts)) {
	sts = configure_object( ip, &aaref, disable_mask);
	if ( EVEN(sts)) return sts;
      }
    }
  }

  // Set GraphConfiguration from argument 1
  vect_cnt = dcli_parse( mb.MethodArguments[1], ",", "", (char *)vect, 
		   sizeof( vect) / sizeof( vect[0]), 
		   sizeof( vect[0]), 0);
  
  for ( i = 0; i < vect_cnt; i++) {
    nr = dcli_parse( vect[i], " 	", "", (char *)item,
		   sizeof( item) / sizeof( item[0]), 
		   sizeof( item[0]), 0);
    if ( nr == 1) {

      if ( sscanf( item[0], "%d", &graph_configuration) != 1)
	graph_configuration = 0;

      sts = ldh_ArefANameToAref( ip->PointedSession, &ip->Pointed, "GraphConfiguration", 
				 &aaref);
      if ( ODD(sts)) {
	sts = ldh_WriteAttribute( ip->PointedSession, &aaref, (void *)&graph_configuration, 
				  sizeof(graph_configuration));
	if ( EVEN(sts)) return sts;
      }
    }
    else if ( nr == 2) {
      pwr_tAName aname;

      if ( sscanf( item[1], "%d", &graph_configuration) != 1)
	graph_configuration = 0;

      strcpy( aname, item[0]);
      strcat( aname, ".GraphConfiguration");

      sts = ldh_ArefANameToAref( ip->PointedSession, &ip->Pointed, aname, 
				 &aaref);
      if ( ODD(sts)) {
	sts = ldh_WriteAttribute( ip->PointedSession, &aaref, (void *)&graph_configuration, 
				  sizeof(graph_configuration));
	if ( EVEN(sts)) return sts;
      }
    }
  }
  return 1;
}

static pwr_tStatus ConfigureComponentFilter( ldh_sMenuCall *ip) 
{
  return 1;
}


// Common Build filter
static pwr_tStatus BuildFilter( ldh_sMenuCall *ip) 
{
  if ( login_prv.priv & pwr_mPrv_DevConfig)
    return 1;
  else
    return 0;
}


pwr_dExport pwr_BindMethods($Object) = {
  pwr_BindMethod(CreateObject),
  pwr_BindMethod(CopyObject),
  pwr_BindMethod(CopyObjectTree),
  pwr_BindMethod(DeleteObject),
  pwr_BindMethod(DeleteObjects),
  pwr_BindMethod(IsOkCopyObject),
  pwr_BindMethod(IsOkCopyObjectTree),
  pwr_BindMethod(IsOkCreateObject),
  pwr_BindMethod(IsOkMoveObject),
  pwr_BindMethod(MoveObject),
  pwr_BindMethod(OpenObject),
  pwr_BindMethod(OpenTemplate),
  pwr_BindMethod(SetDefaults),
  pwr_BindMethod(ClassHelp),
  pwr_BindMethod(HelpClass),
  pwr_BindMethod(HelpClassFilter),
  pwr_BindMethod(Help),
  pwr_BindMethod(HelpFilter),
  pwr_BindMethod(Cast),
  pwr_BindMethod(CastFilter),
  pwr_BindMethod(Disable),
  pwr_BindMethod(DisableFilter),
  pwr_BindMethod(ConfigureComponent),
  pwr_BindMethod(ConfigureComponentFilter),
  pwr_BindMethod(BuildFilter),
  pwr_NullMethod
};




