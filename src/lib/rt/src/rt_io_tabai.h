#ifndef rt_io_tabai_h
#define rt_io_tabai_h

/* rt_io_tabai.h -- <short description>

   PROVIEW/R
   Copyright (C) 1996 by Comator Process AB.

   <Description>.  */

#ifndef rt_io_filter_ai_h
#include "rt_io_filter_ai.h"
#endif

pwr_sClass_ai_value_base *io_IniAIvalP;		/* Pointer to AI-value 	*/
pwr_sClass_Ai_7436	*io_IniAIcon	[64];	/* Pointer to Card obj  */
pwr_tInt32			io_IniAIcsr 	[64];	/* Device adress	*/  
pwr_sClass_Ai		*io_IniAI_obj	[128];	/* Pointer to AI_OBJECT	*/	
pwr_sClass_ChanAi	*io_IniAIChan	[128];	/* Pointer to chan_AI obj*/
pwr_sClass_ChanAit	*io_IniAITChan	[128];	/* Pointer to chan_AIT obj*/
pwr_tInt16			io_IniAIscanint [128];	/* Scanning interval count */

pwr_tUInt16			io_IniAIRackAddr[128];	/* Rack address for RTP */
pwr_tUInt16			io_IniAICardType[128];	/* Card type for RTP */
pwr_tObjid			io_IniAIconobjid[128];

io_sFilterCard_Ai	io_IniFilterCardAi[64];	/* Filter data */
pwr_tFloat32		io_IniAIScanTimeBus;	 /* Scan time in seconds */

pwr_tInt32			io_actantai;  /*  Number of di signals in system  */
#endif
