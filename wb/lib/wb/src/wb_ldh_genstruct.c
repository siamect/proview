/** 
 * Proview   $Id: wb_ldh_genstruct.c,v 1.3 2005-09-01 14:57:58 claes Exp $
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

#include <stdio.h>
#include <ctype.h>
#include <errno.h>
#include <string.h>
#ifdef OS_VMS
#include <descrip.h>
#include <lib$routines.h>
#include <X11/Intrinsic.h>
#endif
#include "pwr.h"
#include "wb_ldh.h"
#include "wb_ldhi.h"
#include "wb_genstruct.h"
#include "wb_ldh_msg.h"
#include "co_cdh.h"
#include "co_msg.h"
#include "wb_utl.h"

/* Error checking */
#define	If_Error_Then_Return  if ( (status & 1) != 1) return
#define	If_Error_Then_Return_Status  if ( (status & 1) != 1) return( status )
#define	If_Error_Then  if ( (status & 1) != 1)
#define	If_Not_Error_Then  if ( (status & 1) )

static char *typstr[] =
{
 "0",
  "pwr_tBoolean",
  "pwr_tFloat32",
  "pwr_tFloat64",
  "pwr_tChar",
  "pwr_tInt8",
  "pwr_tInt16",
  "pwr_tInt32",
  "pwr_tUInt8",
  "pwr_tUInt16",
  "pwr_tUInt32",
  "pwr_tObjid",
  "12",
  "pwr_tString",
  "pwr_tEnum", 
  "15", 
  "pwr_tMask", 
  "17",
  "pwr_tTime",
  "pwr_tText",
  "pwr_sAttrRef",
  "pwr_tUInt64",
  "pwr_tInt64",
  "pwr_tClassId",
  "pwr_tTypeId",
  "pwr_tVolumeId",
  "pwr_tObjectIx",
  "pwr_tRefId",
  "pwr_tDeltaTime",
  "29",
};
     
FILE *fp;

static int genstr_process_classhier (
  void *session,
  pwr_tObjid odin,
  char *volname
);
int genstr_psts (
  unsigned long int sts,
  FILE *logfile
);
static void genstr_process_body (
  void *session,
  ldh_sParDef *bop,
  int rows,
  FILE *fsp
);
static int genstr_process_classdef (
  void *session,
  pwr_tObjid od
);


static void printobjdid (
  void *session,
  pwr_tObjid od,
  char *txt
) {
  char name[512];
  int retlen;
  int status;

  status = ldh_ObjidToName(session, od,
	ldh_eName_Hierarchy, name, 512, &retlen);
  If_Error_Then_Return;
 
  printf("%s: %s\n", txt, name );

}


/*
 * Process all hierarchies
 *   Get rootlist
 *   while found
 *     if object is of type $classhier
 *       process Class
 *     get sibling
 *   end while
 * end
 */

int genstr_process_top (
  ldh_tSession session,
  char *volname
) {
  pwr_tObjid root_od;
  int status;
  char vname[32];
  pwr_tVolumeId volid;
  ldh_sVolumeInfo 	info;
  ldh_tVolContext	volctx;
  int			volume_attached;
  ldh_tSession 	l_session;
  int			size;

  /* Attach to volume */
  status = ldh_GetVolumeInfo( ldh_SessionToVol( session), &info);
  if (EVEN(status)) return status;

  status = ldh_VolumeIdToName( ldh_SessionToWB( session), info.Volume, 
		vname, sizeof(vname), &size);
  if (EVEN(status)) return status;

  cdh_ToUpper(vname, vname);
  cdh_ToUpper(volname, volname);

  if ( strcmp( volname, vname))
  {
    status = ldh_VolumeNameToId( ldh_SessionToWB( session), volname, &volid);
    if ( EVEN(status)) return status;

    status = ldh_AttachVolume( ldh_SessionToWB( session), volid, &volctx);
    if ( EVEN(status)) return status;

    status = ldh_OpenSession( &l_session, volctx, 
		ldh_eAccess_ReadOnly, ldh_eUtility_Pwr);
    if (EVEN(status))
    {
      ldh_DetachVolume( ldh_SessionToWB( session), volctx);
      return status;
    }

    volume_attached = 1;
  }
  else
  {
    volume_attached = 0;
    l_session = session;
  }

  status = ldh_NameToObjid( l_session, &root_od, "Class");
  if (EVEN(status))
  {
    if ( volume_attached)
    {
      ldh_DetachVolume( ldh_SessionToWB( l_session), volctx);
      ldh_CloseSession( l_session);
      return status;
    }
  }
  status = genstr_process_classhier( l_session, root_od, volname);
  if ( volume_attached)
  {
    ldh_DetachVolume( ldh_SessionToWB( l_session), volctx);
    ldh_CloseSession( l_session);
  }
  return status;
}

/*
 * Process all classes
 *   Get child
 *   create headerfile
 *   while found
 *     if object is of type $classdef
 *       process Classdef
 *     get sibling
 *   end while
 * end
 */

static int genstr_process_classhier (
  void *session,
  pwr_tObjid odin,
  char *volname
) {
  pwr_tObjName name;
  pwr_tObjName cname;
  int retlen;
  pwr_tClassId class;
  pwr_tObjid od;
  int status;
  char fname[512];
  char hname[80];

  printobjdid( session, odin, "\n   Reading" );

  status = ldh_ObjidToName(session, odin, ldh_eName_Object, name, sizeof(name), &retlen);
  If_Error_Then_Return_Status;
  
  cdh_ToUpper(cname, volname);
  if (strcmp(cname, "PWRS") == 0) {
    sprintf(fname, "pwr_inc:pwr_systemclasses.h");
    sprintf(hname, "pwr_systemclasses_h");
  } else if (strcmp(cname, "PWRB") == 0) {
    sprintf(fname, "pwr_inc:pwr_baseclasses.h");
    sprintf(hname, "pwr_baseclasses_h");
  } else if (strcmp(cname, "MPS") == 0) {
    cdh_ToLower(cname, volname);
    sprintf(fname, "pwr_inc:pwr_%sclasses.h", cname );
    sprintf(hname, "pwr_%sclasses_h", cname);
  } else {
    cdh_ToLower(cname, volname);
    sprintf(fname, "pwrp_inc:pwr_%sclasses.h", cname );
    sprintf(hname, "pwr_%sclasses_h", cname);
  }

  fp = fopen( fname, "w");
  if (fp == NULL)
    {
    perror(fname);
#ifdef OS_VMS
    genstr_psts(vaxc$errno, NULL);
#endif
    return(12);
    }

  fprintf(fp, "#ifndef %s\n", hname);
  fprintf(fp, "#define %s\n\n", hname);
  fprintf(fp, "#ifndef pwr_class_h\n");
  fprintf(fp, "#include \"pwr_class.h\"\n");
  fprintf(fp, "#endif\n\n");

  status = ldh_GetChild( session, odin, &od);

  while ( status & 1 )
  {
    printobjdid( session, od, "   Class" );

    status = ldh_GetObjectClass(session, od, &class);
    If_Error_Then_Return_Status;

    if (class == pwr_eClass_ClassDef) {
      genstr_process_classdef( session, od);
    }

    status = ldh_GetNextSibling(session, od, &od);
  }
  fprintf(fp, "\n#endif\n");
  fclose( fp );

  return LDH__SUCCESS;
}


/*
 * Process a classdef
 *   Get sysbody of the rtbody object
 *   Get the RtBody definition
 *
 *   print typdef
 *   for each parameter
 * 	print definition
 *   end
 *   printf structname
 *   printf class definition
 *
 * end
 */

static int genstr_process_classdef (
  void *session,
  pwr_tObjid od
) {
  pwr_tStatus sts;
  char name[512];
  char rtname[512];
  char devname[512];
  char bodyname[32];
  char fname[512];
  pwr_tObjName ClassName;
  pwr_tObjName UClassName;
  pwr_tObjName LClassName;
  char *lc;
  char *uc;
  char *c;
  char range;
  char urange;
  int retlen;
  pwr_tObjid rtbody;
  pwr_tObjid devbody;
  pwr_sObjBodyDef *objbodydef = NULL;
  int oblen;
  ldh_sParDef *body = NULL;
  int rows;
  int IsWrittenClass = 0;
  FILE *fsp;
  pwr_tClassId class = cdh_ClassObjidToId(od);

  sts = ldh_ObjidToName(session, od, ldh_eName_Hierarchy, name, sizeof(name), &retlen);
  if (EVEN(sts)) return sts;
  sts = ldh_ObjidToName(session, od, ldh_eName_Object, ClassName, sizeof(ClassName), &retlen);
  if (EVEN(sts)) return sts;

  cdh_ToUpper(UClassName, ClassName);
  cdh_ToLower(LClassName, ClassName);

  strcpy(devname, name);
  strcat(devname, "-DevBody");

  if (od.vid == 1) {
    strcpy(rtname, name);
    strcat(rtname, "-SysBody");
    strcpy(bodyname, "SysBody");
  } else {
    strcpy(rtname, name);
    strcat(rtname, "-RtBody");
    strcpy(bodyname, "RtBody");
  }

  if (od.vid > 2) {
    range = 'p';
    urange = 'P';
    sprintf(fname, "pwrp_inc:pwrp_c_%s.h", LClassName);
    uc = UClassName;
    lc = LClassName;
    c = ClassName;
  } else if (od.vid == 1) {
    range = 's';
    urange = 'S';
    uc = &UClassName[1];
    lc = &LClassName[1];
    c = &ClassName[1];
    sprintf(fname, "pwr_inc:pwrs_c_%s.h", &LClassName[1]);
  } else {
    range = 'b';
    urange = 'B';
    lc = LClassName;
    uc = UClassName;
    c = ClassName;
    sprintf(fname, "pwr_inc:pwrb_c_%s.h", LClassName);
  }

  fsp = fopen( fname, "w");
  fprintf(fsp, "#ifndef pwr%c_c_%s_h\n", range, lc);
  fprintf(fsp, "#define pwr%c_c_%s_h\n\n", range, lc);
  fprintf(fsp, "\n/* %s\n\n   PROVIEW/R\n   Copyright (C) 2005 by SSAB Oxelösund AB.  */\n\n", fname);
  fprintf(fsp, "#ifndef pwr_class_h\n");
  fprintf(fsp, "#include \"pwr_class.h\"\n");
  fprintf(fsp, "#endif\n\n");

  /* Get SysBody or RtBody if exists */
  while (1) {
    char *cp;

    sts = ldh_NameToObjid( session, &rtbody, rtname );
    if (EVEN(sts)) break;

    sts = ldh_GetObjectBody( session, rtbody, "SysBody", (void *) &objbodydef, &oblen );
    if (EVEN(sts)) break;

    /* Get the RtBody definition */
    sts = ldh_GetObjectBodyDef(session, class, bodyname, 1, &body, &rows );
    if (EVEN(sts)) break;

    if (0 && *objbodydef->StructName == '\0')
      cp = ClassName;
    else
      cp = objbodydef->StructName;

    if (*cp == '\0' || rows == 0) {
      fprintf(fp, "\n\n/* Class: %s (no StructName or %s contents)  */\n\n", ClassName, bodyname);
      fprintf(fsp, "\n\n/* Body: (no StructName or %s contents)  */\n\n", bodyname);
    } else {
      fprintf(fp, "\n#ifndef pwr_cClass_%s\n", cp);
      fprintf(fp, "#define pwr_cClass_%s %u\n", cp, class);
      fprintf(fsp, "\n#ifndef pwr_cClass_%s\n", cp);
      fprintf(fsp, "#define pwr_cClass_%s %u\n", cp, class);
      IsWrittenClass = 1;
      fprintf(fp, "\n\n/* Class: %s\n   Body:  %s  */\n\n", ClassName, bodyname);
      fprintf(fsp, "\n/* Body:  %s  */\n\n", bodyname);

      fprintf(fp, "\ntypedef struct {\n");
      fprintf(fsp, "\ntypedef struct {\n");

      genstr_process_body(session, body, rows, fsp);
      fprintf(fp, "} pwr_sClass_%s;\n", cp);
      fprintf(fsp, "} pwr_sClass_%s;\n", cp);
    }
    break;
  }
  if (objbodydef != NULL) {
    free((char *) objbodydef);
    objbodydef = NULL;
  }
  if (body != NULL) {
    free((char *) body);
    body = NULL;
  }

  /* Get DevBody if exists */

  while (1) {
    sts = ldh_NameToObjid(session, &devbody, devname);
    if (EVEN(sts)) break;

    sts = ldh_GetObjectBody(session, devbody, "SysBody", (void *) &objbodydef, &oblen);
    if (EVEN(sts)) break;

    /* Get the DevBody definition */

    sts = ldh_GetObjectBodyDef(session, class, "DevBody", 1, &body, &rows );
    if (EVEN(sts)) break;

    if (rows == 0) {
      break;
    } else {
      if (!IsWrittenClass) {
	fprintf(fp, "\n#ifndef pwr_cClass_%s\n", ClassName);
	fprintf(fp, "#define pwr_cClass_%s %u\n", ClassName, class);
	fprintf(fsp, "\n#ifndef pwr_cClass_%s\n", ClassName);
	fprintf(fsp, "#define pwr_cClass_%s %u\n", ClassName, class);
	IsWrittenClass = 1;
      }      
      fprintf(fp, "\n/* Class: %s\n   Body:  DevBody  */\n", ClassName);
      fprintf(fsp, "\n/* Body:  DevBody  */\n");
      fprintf(fp, "\ntypedef struct {\n");
      fprintf(fsp, "\ntypedef struct {\n");

      genstr_process_body(session, body, rows, fsp);
      fprintf(fp, "} pwr_sdClass_%s;\n", ClassName);
      fprintf(fsp, "} pwr_sdClass_%s;\n", ClassName);
    }
    break;
  }

  if (objbodydef != NULL)
    free((char *) objbodydef);
  if (body != NULL)
    free((char *) body);

  if (IsWrittenClass ) {
    fprintf(fp, "\n#endif\n");
    fprintf(fsp, "\n#endif\n#endif\n");
  } else {
    fprintf(fsp, "\n#endif\n");
  }
  fclose(fsp);
  return 1;
}


static void genstr_process_body (
  void *session,
  ldh_sParDef *bop,
  int rows,
  FILE *fsp
) {
  pwr_tStatus sts;
  char size[20];
  pwr_sParInfo *bp;
  int retlen;
  pwr_tObjName classname;
  pwr_tClassId class;
  pwr_sBuffer *Buffer;
  int i;
  int FillSize;
  int FillerIndex = 0;

  for (i = 0; i < rows; ++i) {
    bp = &bop->Par->Param.Info;
    switch (bop->ParClass) {
    case pwr_eClass_Input:
    case pwr_eClass_Param:
    case pwr_eClass_Intern:
    case pwr_eClass_Output:
    case pwr_eClass_ObjXRef:
    case pwr_eClass_AttrXRef:
      if ( bp->Type == pwr_eType_String) {
	if (bop->Par->Param.TypeRef == pwr_eType_String) {
	  if (bop->ParClass == pwr_eClass_Input) {
	    fprintf(fp, "  %-36s*%sP;\n", "pwr_tChar", bp->PgmName );
	    fprintf(fsp, "  %-36s*%sP;\n", "pwr_tChar", bp->PgmName );
	  }
	  fprintf(fp, "  %-36s%s[%d]", "pwr_tChar", bp->PgmName, bp->Size );
	  fprintf(fsp, "  %-36s%s[%d]", "pwr_tChar", bp->PgmName, bp->Size );
	} else {
	  sprintf(size, "%s%d", typstr[ cdh_TypeIdToIndex(bp->Type) ], bp->Size / bp->Elements );
	  if (bop->ParClass == pwr_eClass_Input) {
	    fprintf(fp, "  %-36s*%s%sP;\n", size,
	      (bp->Flags & PWR_MASK_POINTER) ? "*" : "", bp->PgmName);
	    fprintf(fsp, "  %-36s*%s%sP;\n", size,
	      (bp->Flags & PWR_MASK_POINTER) ? "*" : "", bp->PgmName);
	  }
	  fprintf(fp, "  %-36s%s%s", size, (bp->Flags & PWR_MASK_POINTER) ? "*" : "",
	    bp->PgmName);
	  fprintf(fsp, "  %-36s%s%s", size, (bp->Flags & PWR_MASK_POINTER) ? "*" : "",
	    bp->PgmName);
	  if ( bp->Flags & PWR_MASK_ARRAY) {
	    fprintf(fp, "[%d]", bp->Elements);
	    fprintf(fsp, "[%d]", bp->Elements);
	  }
	}
      } else if ( bp->Type == pwr_eType_Text) {
	sprintf(size, "%s%d", typstr[ cdh_TypeIdToIndex(bp->Type) ], bp->Size / bp->Elements );
	if (bop->ParClass == pwr_eClass_Input) {
	  fprintf(fp, "  %-36s*%s%sP;\n", size,
	    (bp->Flags & PWR_MASK_POINTER) ? "*" : "", bp->PgmName);
	  fprintf(fsp, "  %-36s*%s%sP;\n", size,
	    (bp->Flags & PWR_MASK_POINTER) ? "*" : "", bp->PgmName);
	}
	fprintf(fp, "  %-36s%s%s", size, (bp->Flags & PWR_MASK_POINTER) ? "*" : "",
	  bp->PgmName);
	fprintf(fsp, "  %-36s%s%s", size, (bp->Flags & PWR_MASK_POINTER) ? "*" : "",
	  bp->PgmName);
	if ( bp->Flags & PWR_MASK_ARRAY) {
	  fprintf(fp, "[%d]", bp->Elements);
	  fprintf(fsp, "[%d]", bp->Elements);
	}
      } else {  /* Not pwr_eType_String */
	if (bop->ParClass == pwr_eClass_Input) {
	  fprintf(fp, "  %-36s*%s%sP;\n", typstr[ cdh_TypeIdToIndex(bp->Type) ],
	    (bp->Flags & PWR_MASK_POINTER) ? "*" : "", bp->PgmName);
	  fprintf(fsp, "  %-36s*%s%sP;\n", typstr[ cdh_TypeIdToIndex(bp->Type) ],
	    (bp->Flags & PWR_MASK_POINTER) ? "*" : "", bp->PgmName);
	}
	fprintf(fp, "  %-36s%s%s", typstr[ cdh_TypeIdToIndex(bp->Type) ], 
	  (bp->Flags & PWR_MASK_POINTER) ? "*" : "", bp->PgmName);
	fprintf(fsp, "  %-36s%s%s", typstr[ cdh_TypeIdToIndex(bp->Type) ], 
	  (bp->Flags & PWR_MASK_POINTER) ? "*" : "", bp->PgmName);
	if (bp->Flags & PWR_MASK_ARRAY) {
	  fprintf(fp, "[%d]", bp->Elements);
	  fprintf(fsp, "[%d]", bp->Elements);
	}
      }

      fputs(";\n", fp);
      fputs(";\n", fsp);
      break;
    case pwr_eClass_Buffer:
      Buffer = (pwr_sBuffer *) bp;
      sts = ldh_GetObjectClass(session, cdh_ClassIdToObjid(Buffer->Class), &class);
      if (EVEN(sts)) {
	fprintf(fp, "  /* *** Unknown buffer class ***  %s; */\n", bp->PgmName);
	fprintf(fsp, "  /* *** Unknown buffer class ***  %s; */\n", bp->PgmName);
      } else if (class != pwr_eClass_ClassDef ) {
	fprintf(fp, "  /* *** Unknown buffer class ***  %s; */\n", bp->PgmName);
	fprintf(fsp, "  /* *** Unknown buffer class ***  %s; */\n", bp->PgmName);
      } else {
	sts = ldh_ObjidToName(session, cdh_ClassIdToObjid(Buffer->Class), ldh_eName_Object,
	  classname, sizeof(classname), &retlen);
	fprintf(fp, "  pwr_s%-31s%s;\n", &classname[1], bp->PgmName);
	fprintf(fsp, "  pwr_s%-31s%s;\n", &classname[1], bp->PgmName);
      }
      break;
    default:
      break;
    }
    FillSize = ldhi_dAlign(bp->Size) - bp->Size;
    if (FillSize > 0) {
      fprintf(fp, "  %-36sfiller_%d[%d];\n", "char", FillerIndex, FillSize);
      fprintf(fsp, "  %-36sfiller_%d[%d];\n", "char", FillerIndex++, FillSize);
    }
    ++bop;
  }
}


int genstr_psts (
  unsigned long int sts,
  FILE *logfile
) {
  static char msg[256];

  if (!(sts & 1)) {
    msg_GetMsg( sts, msg, sizeof(msg));

    if (logfile != NULL)
      fprintf(logfile, "%s\n", msg);
    else
      printf("%s\n", msg);
  }

  return sts & 1;
}


void uppercase( char *out, char *in )
{
  while (*in)
  {
    *out++ = _toupper( *in );
    ++in;
  }
}
