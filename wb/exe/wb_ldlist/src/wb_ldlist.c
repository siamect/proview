/* 
 * Proview   $Id: wb_ldlist.c,v 1.2 2005-09-01 14:57:49 claes Exp $
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

/* wb_ldlist.c -- 
   Lists load data files.  */

#include <stdio.h>
#include <stdlib.h>
#include <descrip.h>
#include <string.h>
#include <lib$routines.h>

#include "pwr.h"
#include "pwr_class.h"
#include "wb_ldh_msg.h"
#include "rt_load.h"
#include "wb_ldhi.h"
#include "wb_ldh.h"
#include "co_cdh.h"
#include "co_time.h"

FILE *lf;
FILE *lsf;

/*============================================================================*\
  Function prototypes to local functions.
\*============================================================================*/

static pwr_tBoolean ReadHead (load_sHead *Head);
static pwr_tBoolean ListSectBoot (load_sHead *Head);
static pwr_tBoolean ListSectFile (load_sHead *Head);
static pwr_tBoolean ListSectObjBody (load_sHead *Head);
static pwr_tBoolean ListSectObjHead (load_sHead *Head);
static pwr_tBoolean ListSectVolume (load_sHead *Head);

static void ErrorExit(char *string);
static int psts(unsigned long int sts, FILE *logfile, char *string);

static void usage(char *mes) {
  printf("PWR_LDLIST-E-%s\n", mes);
  printf("Usage: PWR_LDLIST [-ob] loadfile [listfile]\n");
  exit(0);
}

static void help() {
  printf("PWR_LDLIST lists a load file, to screen or to a list file.\n");
  printf("Usage: PWR_LDLIST [-ob] loadfile [listfile]\n");
  printf("Flags are:\n\t-o\tincludes object headers\n");
  printf("\t-b\tincludes object bodies\n");
  exit(0);
}


main (
  int			argc,
  char			*argv[]
)
{
  load_sHead		Head;
  int			bflag = 0;
  int			oflag = 0;
  char			c;
  
  while (--argc > 0 && (*++argv)[0] == '-') {
    while (c = *++argv[0]) {
      switch (c) {
      case 'o':
	++oflag;
	break;
      case 'b':
	++bflag;
	break;
      default:
	usage("Unknown flag");
      }
    }
  }

  if ((*argv)[0] == '?') help();
  if (argc < 1) usage("No load file given!\n");

  lf = fopen(*argv, "rb");
  if (lf == NULL) {
    perror(*argv);
    exit(1);
  }
  if (argc >= 2) {
    lsf = fopen(*++argv, "w");
    if (lsf == NULL) {
      perror(*argv);
      exit(1);
    }
  } else {
    lsf = stdout;
  }

  for(;;) {
    if (!ReadHead(&Head)) exit(1);
    if (Head.SectType == load_eSect_End)
      break;
    switch ((int) Head.SectType) {
    case load_eSect_Boot:
      if (!ListSectBoot(&Head)) exit(1);
      break;
    case load_eSect_CreObj:
    case load_eSect_ChgObj:
    case load_eSect_DelObj:
    case load_eSect_ObjHead:
      if (oflag) {
	if (!ListSectObjHead(&Head)) exit(1);
      } else {
      if (fseek(lf, Head.SectSize, 1) != 0)
	exit(LDH__FILEPOS);
      }
      break;
    case load_eSect_Volume:
      if(!ListSectVolume(&Head)) exit(1);
      break;
    case load_eSect_File:
      if (!ListSectFile(&Head)) exit(1);
      break;
    case load_eSect_ObjBody:
      if (bflag) {
	if(!ListSectObjBody(&Head)) exit(1);
      } else {
      if (fseek(lf, Head.SectSize, 1) != 0)
	exit(LDH__FILEPOS);
      }
      break;
    default:
      break;
    }
  }

  fclose(lf);
  fclose(lsf);
}

static pwr_tBoolean
ReadHead (
  load_sHead		*Head
)
{
  static char		*SectType[] = {
					"?", "Boot", "CreObj",
					"ChgObj", "DelObj",
					"End", "File",
					"ObjBody", "ObjHead",
					"Volume", "VolRef", "?"
				      };

  memset(Head, 0, sizeof(Head));

  fread(Head, sizeof(*Head), 1, lf);
  if (feof(lf)) {
    Head->SectType = load_eSect_End;
    return 1;
  }
  
  if (Head->HeadVersion != load_cVersionHead)
    return psts(LDH__BADVER, NULL, "HeadVersion");

  fprintf(lsf, "HeadVersion.: %d\n", Head->HeadVersion);
  fprintf(lsf, "SectType....: load_eSect_%s\n",
    Head->SectType > load_eSect_ ? SectType[load_eSect_] : SectType[Head->SectType]);
  fprintf(lsf, "SectVersion.: %d\n", Head->SectVersion);
  fprintf(lsf, "SectSize....: %d\n", Head->SectSize);
  fprintf(lsf, "\n");

  return 1;
}

static pwr_tBoolean
ListSectBoot (
  load_sHead		*Head
) {
#if 0
  load_sBoot		Boot;
  int			i;
  int			j;
  char			timbuf[24];

  if (Head->SectVersion != load_cVersionBoot)
    return psts(LDH__BADVER, NULL, "Section: Boot");

  for (i = 0; i < Head->SectSize; i+= sizeof(Boot)) {
    memset(&Boot, 0, sizeof(Boot));
    fread(&Boot, sizeof(Boot), 1, lf);

    fprintf(lsf, "BootVersion.: %d\n", Boot.BootVersion);
    time_AtoAscii(&Boot.BootCreTime, 0, timbuf, sizeof(timbuf));
    fprintf(lsf, "BootCreTime.: %s\n", timbuf);
    fprintf(lsf, "\n%11s %11s %8s\n", "NodeIndex", "DnoVersion", "NodeName");
    for (j = 1; j < 256; j++) {
      if (Boot.NodeDb[j].DnoVersion == 0) continue;
      fprintf(lsf, "%11d %11d %-8s\n", j, Boot.NodeDb[j].DnoVersion, Boot.NodeDb[j].NodeName);
    }
    fprintf(lsf, "\n");
  }
#endif
  printf("Section Boot not implemented!\n");
  return 1;
}

static pwr_tBoolean
ListSectVolume (
  load_sHead		*Head
)
{
  load_sVolume		Volume;
  char			timbuf[24];
  int			i;

  if (Head->SectVersion != load_cVersionVolume)
    return psts(LDH__BADVER, NULL, "Section: Volume");

  for (i = 0; i < Head->SectSize; i+= sizeof(Volume)) {
    memset(&Volume, 0, sizeof(Volume));
    fread(&Volume, sizeof(Volume), 1, lf);

    time_AtoAscii(&Volume.CreTime, 0, timbuf, sizeof(timbuf));
    fprintf(lsf, "Id...............: %s\n", cdh_VolumeIdToString(NULL, Volume.Id, 1, 1));
    fprintf(lsf, "Name.............: %s\n", Volume.Name);
    fprintf(lsf, "Class............: %s\n", cdh_ClassIdToString(NULL, Volume.Class, 1));
    fprintf(lsf, "ClassName........: %s\n", Volume.ClassName);
    fprintf(lsf, "Version..........: %d\n", Volume.Version);
    fprintf(lsf, "CreTime..........: %s\n", timbuf);
    fprintf(lsf, "Cardinality......: %d\n", Volume.Cardinality);
    fprintf(lsf, "BodySize.........: %d\n", Volume.BodySize);
    fprintf(lsf, "BodySize.........: %d\n", Volume.BodySize);
    fprintf(lsf, "Volume object body:\n");
#if 0
    fprintf(lsf, "  Description....: %d\n", Volume.Volume.Root.Description);
    fprintf(lsf, "  NextOix........: %d\n", Volume.Volume.Root.NextOix);
    fprintf(lsf, "  RtVersion......: %d\n", Volume.Volume.Root.RtVersion);
    time_AtoAscii(&Volume.Volume.Root.RtCreTime, 0, timbuf, sizeof(timbuf));
    fprintf(lsf, "  RtCreTime......: %s\n", timbuf);
#if 0
    fprintf(lsf, "  RtCreator......: %d\n", Volume.Volume.Root.RtCreator);
#endif
    fprintf(lsf, "  RtCardinality..: %d\n", Volume.Volume.Root.RtCardinality);
    fprintf(lsf, "  RtBodySize.....: %d\n", Volume.Volume.Root.RtBodySize);
    switch (Volume.Class) {
    case pwr_eClass_RootVolume:
    case pwr_eClass_SubVolume:
      fprintf(lsf, "  OperatingSystem: %d\n", Volume.Volume.Root.Description);
      break;
    case pwr_eClass_ClassVolume:
      break;
    }
#endif
    fprintf(lsf, "\n");
  }
  return 1;
}

static pwr_tBoolean
ListSectFile (
  load_sHead		*Head
)
{
  load_sFile		File;
  int			i;
  char			timbuf[24];
  static char		*FileType[] = {"?", "Boot", "Diff", "Volume", "?"};

  if (Head->SectVersion != load_cVersionFile)
    return psts(LDH__BADVER, NULL, "Section: File");

  for (i = 0; i < Head->SectSize; i+= sizeof(File)) {
    memset(&File, 0, sizeof(File));
    fread(&File, sizeof(File), 1, lf);

    fprintf(lsf, "PwrMDVersion.: %d\n", File.PwrVersion);
    fprintf(lsf, "FormatVersion: %d\n", File.FormatVersion);
    fprintf(lsf, "FileType.....: load_eFile_%s\n",
      File.FileType > load_eFile_ ? FileType[load_eFile_] : FileType[File.FileType]);
    time_AtoAscii(&File.CreationTime, 0, timbuf, sizeof(timbuf));
    fprintf(lsf, "CreationTime.: %s\n", timbuf);
    fprintf(lsf, "\n");
  }
  return 1;
}

static pwr_tBoolean
ListSectObjHead (
  load_sHead		*Head
)
{
  load_sObjHead		ObjHead;
  int			i;

  if (Head->SectVersion != load_cVersionObjHead)
    return psts(LDH__BADVER, NULL, "Section: ObjHead");

  fprintf(lsf, "%-32s", "ObjectName");
  fprintf(lsf, " %9s", "Objid");
  fprintf(lsf, " %9s", "Class");
  fprintf(lsf, " %9s", "Father");
  fprintf(lsf, " %6s", "Size");
  fprintf(lsf, " %17s", "Server");
  fprintf(lsf, " %9s", "Flags");
  fprintf(lsf, " %11s", "HeadGen");
  fprintf(lsf, " %11s", "BodyGen");
  fprintf(lsf, " %8s", "Change");
  fprintf(lsf, "\n");

  for (i = 1; i < Head->SectSize; i+= sizeof(ObjHead)) {
    memset(&ObjHead, 0, sizeof(ObjHead));
    fread(&ObjHead, sizeof(ObjHead), 1, lf);

    fprintf(lsf, "%-32s", ObjHead.Name);
    fprintf(lsf, " %9X", ObjHead.Objid.oix);
    fprintf(lsf, " %9X", ObjHead.Class);
    fprintf(lsf, " %9X", ObjHead.Father.oix);
    fprintf(lsf, " %6d", ObjHead.Size);
    fprintf(lsf, " %8X%9X", ObjHead.Server.vid, ObjHead.Server.oix);
    fprintf(lsf, " %9X", ObjHead.Flags);
    fprintf(lsf, " %11d", ObjHead.HeadGeneration);
    fprintf(lsf, " %11d", ObjHead.BodyGeneration);
    fprintf(lsf, " %08.8X", ObjHead.Change);
    fprintf(lsf, "\n");
  }
  fprintf(lsf, "\n");
  return 1;
}

static pwr_tBoolean
ListSectObjBody (
  load_sHead		*Head
)
{
  load_sObjBody		ObjBody;
  int			i;
  int			j;
  char			c;
  static char		Body[ldhi_cSizBody];
  static char		tab[] = "\
!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\
 ! #$!&'()*+,-./0123456789:;<=>!\
@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_\
`abcdefghijklmnopqrstuvwxyz{|}~!\
!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\
!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\
!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\
!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!";

  if (Head->SectVersion != load_cVersionObjBody)
    return psts(LDH__BADVER, NULL, "Section: ObjBody");

  fprintf(lsf, "%9s %9s %9s  Data...\n", "Objid", "Offset", "Size");

  for (i = 0; i < Head->SectSize;) {
    memset(&ObjBody, 0, sizeof(ObjBody));
    fread(&ObjBody, sizeof(ObjBody), 1, lf);
    fread(&Body, ObjBody.Size, 1, lf);
    i += sizeof(ObjBody) + ObjBody.Size;

#if 0
    for (j = 0; j < MIN(ObjBody.Size, 80); j++) {
      c = Body[j];
      Body[j] = tab[c] == '!' ? '.' : c;
    }
    Body[j] = '\0';
    fprintf(lsf, "%11X %11d %11d  %s\n", ObjBody.Objid, ObjBody.Offset, ObjBody.Size, Body);
#endif
    fprintf(lsf, "%9X %9d %9d\n", ObjBody.Objid.oix, ObjBody.Offset, ObjBody.Size);
    }
  fprintf(lsf, "\n");
  return 1;
}

static void
ErrorExit (
  char *string
)
{
  perror(string);
  exit(1);
}

static int
psts (
  unsigned long int	sts,
  FILE			*logfile,
  char *string
)
{
  static int		msgsts;
  static int		msglen;
  static char		msg[256];
  static		$DESCRIPTOR(msgdesc, msg);

  if (!(sts & 1)) {
    msgsts = sts;
    lib$sys_getmsg(&msgsts, &msglen, &msgdesc, 0, 0);
    msg[msglen]='\0';
    if (logfile != NULL)
      fprintf(logfile, "%s\n%s\n", string, msg);
    else
      printf("%s\n%s\n", string, msg);
   }

  return sts & 1;
}
