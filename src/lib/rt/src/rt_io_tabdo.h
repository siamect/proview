#ifndef rt_io_tabdo_h
#define rt_io_tabdo_h

/* rt_io_tabdo.h -- <short description>

   PROVIEW/R
   Copyright (C) 1996 by Comator Process AB.

   <Description>.  */

#ifndef rt_io_filter_po_h
#include "rt_io_filter_po.h"
#endif

pwr_sClass_do_value_base  *io_IniDOvalueP;	/* Pointer to DO-value */	
pwr_tUInt16		  io_IniDOold   [128];	/* Old/saved value bitdata */	
pwr_sClass_Do_7437_83	  *io_IniDOcon  [128];	/* Pointer to config object */	
pwr_tInt32		  io_IniDOcsr   [128];	/* Device adress 		*/  
pwr_tInt16		  io_IniDoKortnr[128];	/* Word number on device */
pwr_tUInt16		  io_IniDORackAddr[128];	/* Rack address for RTP */
pwr_tUInt16		  io_IniDOCardType[128];	/* Card type for RTP */
pwr_tObjid		  io_IniDoCardId[128];	/* Objid for rtp dioc 929214 GPd */

io_sFilterCard_Po	  io_IniFilterCardPo[128]; /* Filter data */
pwr_tFloat32		  io_IniDOScanTimePLC;	 /* Scan time in seconds */

pwr_tInt32		  io_actantdo;  	/*  Number of do signals in system */
#endif
