#ifndef rt_io_tabdi_h
#define rt_io_tabdi_h

/* rt_io_tabdi.h -- <short description>

   PROVIEW/R
   Copyright (C) 1996 by Comator Process AB.

   <Description>.  */

#ifndef rt_io_filter_di_h
#include "rt_io_filter_di.h"
#endif

pwr_sClass_di_value_base  *io_IniDIvalueP;	/* Pointer DI-value	*/	
pwr_tInt32		  io_IniDIcsr[128];	/* Device adress	*/  
pwr_sClass_Di_7437_37	  *io_IniDIcon[128];	/* Pointer card   DI*/	
pwr_tInt16		  io_IniDIKortnr[128];	/* Number of word on device */
pwr_tUInt16		  io_IniDIRackAddr[128];	/* Rack address for RTP */
pwr_tUInt16		  io_IniDICardType[128];	/* Card type for RTP */
pwr_tObjid		  io_IniDICardId[128];	/* GPd 920212 , rtp dioc */

io_sFilterCard_Di	  io_IniFilterCardDi[128]; /* Filter data */
pwr_tFloat32		  io_IniDIScanTimePLC;	 /* Scan time in seconds */

pwr_tInt32		  io_actantdi;		/*  Number of DI signals */

#endif
