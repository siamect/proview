#ifndef rt_io_tabao_h
#define rt_io_tabao_h

/* rt_io_tabao.h -- <short description>

   PROVIEW/R
   Copyright (C) 1996 by Comator Process AB.

   <Description>.  */

pwr_sClass_ao_value_base *io_IniAOvalP;		/* Pointer to AO-value 	*/
pwr_sClass_Ao_7455_30	*io_IniAOcon	[512];	/* Pointer to card obj  */
pwr_tInt32			io_IniAOcsr   	[512];	/* Device adress	*/  
pwr_tInt16			io_IniAOchan  	[512];	/* Channel on device   	*/
pwr_tFloat32			io_IniAOold   	[512];	/* Old/saved value 	*/	
pwr_sClass_Ao		*io_IniAO_obj	[512];	/* Pointer to AO_OBJECT	*/	
pwr_sClass_ChanAo	*io_IniAOChObj	[512];	/* Pointer to chan_AO obj*/

pwr_tUInt16			io_IniAORackAddr[512]; /* Rack address for RTP */
pwr_tUInt16			io_IniAOCardType[512]; /* Card type for RTP */
pwr_tObjid      io_IniAOconobjid[512];

pwr_tInt32		io_actantao;	        /*  Number of ao signals in system  */

#endif
