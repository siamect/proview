/* 
 * Proview   $Id: ini_loader.c,v 1.2 2005-09-01 14:57:48 claes Exp $
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

#if defined(OS_ELN)
# include $vaxelnc
# include $loader_utility
# include $kernelmsg
# include stdio
# include descrip
#elif defined(OS_VMS)
# include <stdio.h>
# include <descrip.h>
# include <jpidef.h>
# include <string.h>
# include <starlet.h>
# include <lib$routines.h>
#elif defined(OS_LYNX) || defined(OS_LINUX) || defined(OS_MACOS) || defined OS_FREEBSD
# include <stdio.h>
# include <string.h>
#endif

#include "pwr.h"
#include "pwr_class.h"
#include "rt_gdh.h"
#include "rt_errh.h"
#include "rt_ini_msg.h"
#include "rt_load.h"
#include "ini_loader.h"

#if defined(OS_VMS)
  struct item_descr {
      short	buffer_length;	        /* Item buffer length */
      short	item_code;		/* Item code */
      void	*buffer_addr;		/* Item buffer address */
      short	*return_length_addr;	/* Item resultant length */
      };
#endif


/* PLC job behaviour  */

#define	PLC_PRIORITY	5
#define	PLC_KERNELSTACK	30
#define	PLC_DEBUG	FALSE
#define PLC_NAME	"PLC_%d_%d"

#ifdef OS_ELN
  static pwr_tUInt32 loadcount=0;	/* # of times ini_PlcLoad was called */
#endif

#ifdef	OS_ELN
pwr_tStatus
ini_PlcLoad (
  pwr_tUInt32		plcversion,
  char			*plcfile,
  int			debugflag
)
{
  pwr_tStatus		sts;
  char			progname[255];
  char			filename[255];
  VARYING_STRING(255)	filenamevar;
  VARYING_STRING(41)	prognamevar;
  pwr_tBoolean		PlcDebug;

  /* Increase loadcount.  */

  PlcDebug = debugflag;
  loadcount++;

  /* Construct the filename and the progname.  */

  strcpy (filename, plcfile);
  CSTRING_TO_VARYING(filename, filenamevar);

  sprintf (progname, PLC_NAME, plcversion, loadcount);
  CSTRING_TO_VARYING(progname, prognamevar);

  /* Start loading.  */

  errh_Info("Loading plc '%s'", progname);

  eln$load_program (
		&filenamevar,		/* Name of .EXE-file		*/
		&prognamevar,		/* Internal ELN prog. name	*/
		TRUE,			/* Kernel mode? 		*/
		PlcDebug,		/* Start with debug?		*/
		FALSE,			/* Power recovery?		*/
		PLC_KERNELSTACK,	/* Kernel Stack size		*/
		1,			/* Initial User stack size	*/
		100,			/* Message limit		*/
		10,			/* Job Priority			*/
		8,			/* Process Priority		*/
		&sts);

  return sts;
}
#endif


#ifdef	OS_ELN
pwr_tStatus
ini_PlcStart (
  pwr_tUInt32		plcversion
)
{
  pwr_tStatus		sts;
  PORT			jobport;
  char			progname[80];
  struct dsc$descriptor	prognamedsc;

  /* Build program name.  */

  sprintf(progname, PLC_NAME, plcversion, loadcount);
  prognamedsc.dsc$b_dtype = DSC$K_DTYPE_T;
  prognamedsc.dsc$b_class = DSC$K_CLASS_S;
  prognamedsc.dsc$a_pointer = progname;
  prognamedsc.dsc$w_length = strlen(progname);

  /* Start it.  */

  ker$create_job(&sts, &jobport, &prognamedsc, NULL);
  return sts;
}
#endif


#ifdef	OS_VMS
pwr_tStatus
ini_PlcStart (
  pwr_tUInt32	    plcversion,
  char		    *plcfile,
  pwr_tUInt32	    busid
)
{
  pwr_tUInt32	    sts;
  unsigned int	    flags;
  char		    command[128];
  char		    procname[20];
  $DESCRIPTOR(commanddsc, "");
  $DESCRIPTOR(procnamedsc, "");

  commanddsc.dsc$a_pointer  = command;
  commanddsc.dsc$w_length   = sprintf(command, "run %s", plcfile);
  procnamedsc.dsc$a_pointer = procname;
  procnamedsc.dsc$w_length  = sprintf(procname, "PWR_PLC_%d", busid);

  flags = 1; /* Nowait and Notify */
  sts = lib$spawn(&commanddsc, NULL, NULL, &flags, &procnamedsc);
  if (EVEN(sts)) errh_Error("lib$spawn %s\n%m", command, sts);

  return sts;
}
#endif


void ini_StartApplications ()
{
  pwr_tStatus sts;
  pwr_sAppl *applp;
  pwr_tObjid objid;
#ifdef	OS_ELN
  $DESCRIPTOR(prognamedsc, "");
  $DESCRIPTOR(nulldsc, "");
  $DESCRIPTOR(argdsc, "");
  VARYING_STRING(255) filename;
  VARYING_STRING(41) progname;
  char tmpfilename[255],hostspec[20],*cp;
  PORT jobport;
#endif
#ifdef	OS_VMS
  $DESCRIPTOR(prognamedsc, "");
  $DESCRIPTOR(nulldsc, "");
  $DESCRIPTOR(argdsc, "");
  char spawnbuf [512] = "";
  $DESCRIPTOR (SpawnCommand, "");
  $DESCRIPTOR (SpawnPrcnam, "Appl Start");
  pwr_tUInt32 Flags;
#endif

  /* Find the $Appl objects on this particular node */

  sts = gdh_GetClassList(pwr_eClass_Appl, &objid);
  while (ODD(sts)) {
    if (ODD(gdh_ObjidToPointer(objid, (pwr_tAddress *)&applp))) {

      /* Found an object on this node, load the program */

#ifdef	OS_ELN

      if (strlen(applp->FileName) > 0) {

        /* Check whether we booted from a local disk or not */
        if (ini_LocalBoot()) {

	  /* Local boot */

	  /* Find the filename. Beyond the last ':' */
          cp = strrchr(applp->FileName,':');
          if ( cp ) 
	    *cp++;
	  else
	     cp = applp->FileName;

	  /* Build the application filename. */
	  sprintf(tmpfilename,"%s%s",ini_BootFilesLocation(),cp);

	} else {

	  /* Remote boot */

	  /* Check if application name contains '::'. If not - add hostspec */
          cp = strstr(applp->FileName,"::");
	  if (!cp ) {
	    ini_GetNodeInfo(NULL, NULL, NULL, hostspec, NULL, NULL);
	    sprintf(tmpfilename,"%s::%s",hostspec,applp->FileName);
	  } else {
	    strcpy(tmpfilename,applp->FileName);
	  }
	}
        errh_Info("Load program '%s'", tmpfilename);

        CSTRING_TO_VARYING(tmpfilename, filename);
	CSTRING_TO_VARYING(applp->ProgramName, progname);

	eln$load_program(&filename, &progname, applp->KernelMode,
	  applp->StartWithDebug, FALSE, applp->KernelStackSize, 1,
	  10, applp->JobPriority, applp->ProcessPriority, &sts);
        if (EVEN(sts)) errh_Error("eln$load_program '%s'\n%m", applp->ProgramName, sts);
      }

      if (ODD(sts)) {

        /* Start it */

        errh_Info("Start program '%s'", applp->ProgramName);

	prognamedsc.dsc$a_pointer = applp->ProgramName;
	prognamedsc.dsc$w_length = strlen(applp->ProgramName);

	argdsc.dsc$a_pointer = applp->Arg;
	argdsc.dsc$w_length = strlen (applp->Arg);

	ker$create_job(&sts, &jobport, &prognamedsc, NULL, &nulldsc, &nulldsc, &nulldsc, &argdsc);

	if (EVEN(sts)) errh_Error("ker$create_job '%s'\n%m", applp->ProgramName, sts);
      }


#elif defined(OS_VMS)

      sprintf(spawnbuf, "$ @pwr_exe:rt_ini_appl_start %s \"%s\" %d %d \"%s\"",
	applp->FileName, applp->ProgramName,
	applp->StartWithDebug, applp->JobPriority,
	applp->Arg);
      SpawnCommand.dsc$w_length = strlen(spawnbuf);
      SpawnCommand.dsc$a_pointer = spawnbuf;
      Flags = 1; /* Nowait and Notify */
      errh_Info("Starting %s\nin process %s", applp->FileName, applp->ProgramName);
      sts = lib$spawn(&SpawnCommand, NULL, NULL, &Flags);
      if (EVEN(sts)) errh_Error("lib$spawn '%s'\n%m", spawnbuf, sts);

#elif defined(OS_LYNX) || defined(OS_LINUX) || defined(OS_MACOS) || defined OS_FREEBSD

      errh_Error("NYI. start %s \"%s\" %d %d \"%s\"",
	applp->FileName, applp->ProgramName,
	applp->StartWithDebug, applp->JobPriority,
	applp->Arg);
#endif

    } /* Local appl object */

    sts = gdh_GetNextObject (objid, &objid);
  }
}
