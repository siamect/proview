/* rt_bckdef.h -- <short description>

   PROVIEW/R
   Copyright (C) 1996 by Comator Process AB.

   <Description>.  */

#if defined(OS_VMS)
#  include <descrip.h>
#elif defined(OS_ELN)
#  include descrip.h
#endif

#include "pwr.h"

/* Trace flag: Define BCK_TRACE for trace printouts */

/* #+define BCK_TRACE 1 */

/* Default cycle times */

#define	BCK_DEFAULT_SLOW 600
#define BCK_DEFAULT_FAST 100

/* Backup process names */

#if defined(OS_VMS) || defined(OS_ELN)
#  define BCK_FILE_PROC_NAME "BCK file proc"
#  define BCK_FAST_PROC_NAME "BCK fast proc"
#  define BCK_SLOW_PROC_NAME "BCK slow proc"
#endif
/**************************/
/* File format definition */
/**************************/

#ifdef OS_ELN
#  define	seekbug	1		/* Enable workaround code */
#endif				/* but only for VAXELN... */

/* The backup file consists of 2 areas,
   o a header which contains file info and pointers. This area is
     fixed length and defined as BCK_FILEHEAD_STRUCT. It is always
     the first thing in the file.
   o the data section. This section contains the backup data. The
     section is varying in size. Information here is pointed out by the
     header area. The routines bck_falloc and bck_ffree are used
     to allocate and free areas in this section.  */

#define	BCK_FILE_VERSION	4

typedef struct {

	pwr_tInt32	version;	/* File layout version */
	pwr_tTime	creationtime;	/* Time when file was created */
	pwr_tTime	updatetime [2];	/* Time when header was last updated */
	pwr_tInt32	curdata [2];	/* backup data 0 for fast cycle, and */
				/* 1 for slow. Contains file address */
	pwr_tInt32	cursize [2];	/* size in bytes of curdata areas */

	/* Node identification */

	pwr_tNodeId	nodidx;		/* node index that produced the file */

} BCK_FILEHEAD_STRUCT;

/* Each data section starts with a cycle header.  */

typedef struct {
	pwr_tTime	objtime;	/* Time up to which new objects are included */
	pwr_tUInt32	length;		/* Length of section including this header */
	pwr_tUInt16	cycle;		/* 0=fast, 1=slow */
	pwr_tUInt16	segments;	/* # of segments in section */
} BCK_CYCLEHEAD_STRUCT;

/* Each data section area is divided in a number of segments,
   each containing information pointed out by a single backup
   object. The segment starts with a header which contains the
   identification of the object and parameter the data is backed
   up from. It also contains the size of the following data part.
   This header is fixed size and defined as BCK_DATAHEAD_STRUCT  */

typedef struct {
	pwr_sAttrRef	attrref;	/* Objid for the object */
	pwr_tClassId	class;		/* Class of object */
	pwr_tBoolean	valid;		/* Validity flag */
	pwr_tBoolean	dynamic;	/* Dynamic object */
	pwr_tString80	dataname;	/* Name of object[.attribute] */
} BCK_DATAHEAD_STRUCT;

#ifdef OS_ELN

/* Areas for synchronisation between methods and the main
   processing component in bck.  */

#  define BCK_WRITE_DONE_NAME		"bck_write_done"
#  define	BCK_FORCED_ACTIVATION_NAMES	"bck_activation"

  static AREA		bck_forced_activation;
  static AREA		bck_write_done;

#  define	BCK_MAP_AREAS							\
{									\
  void *adrs;								\
  $DESCRIPTOR (actdsc, BCK_FORCED_ACTIVATION_NAMES);			\
  $DESCRIPTOR (namedsc, BCK_WRITE_DONE_NAME);				\
  ker$create_area_event (						\
		NULL,				/* status */		\
		&bck_write_done,		/* area_id */		\
		&adrs,				/* data_pointer */	\
		0,				/* area_size */		\
		&namedsc,			/* area_name */		\
		EVENT$CLEARED,			/* initial_state */	\
		NULL);				/* virtual_address */	\
  ker$create_area_event (						\
		NULL,				/* status */		\
		&bck_forced_activation,		/* area_id */		\
		&adrs,				/* data_pointer */	\
		0,				/* area_size */		\
		&actdsc,			/* area_name */		\
		EVENT$CLEARED,			/* initial_state */	\
		NULL);				/* virtual_address */	\
}
#elif defined (OS_VMS)

#  define BCK_EFC	(32*3)		/* First flag of EF cluster 3 */
#  define BCK_EFC_NAME	"BCK_EFC"	/* Name of EF cluster */
#  define BCK_ACTIVATE	BCK_EFC		/* Forced activation flag */
#  define BCK_WRITE_DONE (BCK_EFC+1)	/* write done flag */

#elif defined (OS_LYNX) || defined(OS_LINUX)

#  define SIG_BCK_FORCE (SIGRTMIN + 1)        /* forced activation sig  */
#  define SIG_BCK_WRITE_DONE (SIGRTMIN + 2)   /* backup done            */

#endif

