/* wb_c_object.c -- work bench methods of the Object class.

   PROVIEW/R
   Copyright (C) 1994 by Comator Process AB.  */

#include <stdio.h>
#include <string.h>
#include "pwr.h"
#include "wb_pwrs.h"
#include "wb_ldh_msg.h"
#include "wb_ldh.h"
#include "wb_dutl.h"
#include "co_cdh.h"
#include <X11/Intrinsic.h>
#include "wb_api.h"

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
    ip->Pointed.Objid, Dest, Self); 
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
		ip->Pointed.Objid, WritePriv, 1,  WritePriv, NULL);
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

  watt_new( ip->WindowContext, ip->EditorContext, ip->PointedSession,
		Template, WritePriv, 1,  WritePriv, NULL);

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
		ip->Pointed.Objid, WritePriv, 1,  WritePriv, NULL);

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

  wtt_command( ip->EditorContext, cmd);
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
  char cmd[200];
  pwr_tCid cid;

  sts = ldh_GetObjectClass( ip->PointedSession, ip->Pointed.Objid, &cid);
  if ( EVEN(sts)) return sts;

  sts = ldh_ClassIdToName( ip->PointedSession, cid, cname, sizeof(cname), &size);
  if ( EVEN(sts)) return sts;

  if ( cname[0] == '$')
    sprintf( cmd, "help %s /strict", &cname[1]);
  else
    sprintf( cmd, "help %s /strict", cname);

  wtt_command( ip->EditorContext, cmd);
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

  wtt_command( ip->EditorContext, cmd);
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
  pwr_NullMethod
};




