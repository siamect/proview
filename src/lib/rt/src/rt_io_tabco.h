#ifndef rt_io_tabco_h
#define rt_io_tabco_h

/* rt_io_tabco.h -- <short description>

   PROVIEW/R
   Copyright (C) 1996 by Comator Process AB.

   <Description>.  */

pwr_sClass_co_value_base  *io_IniCOvalueP;    /* Pointer CO-value	*/	
pwr_sClass_ca_value_base  *io_IniCAvalueP;    /* Pointer CA-value	*/	
pwr_tInt32		  io_IniCOcsr[64];    /* Device adress	*/  
pwr_sClass_Co_PI24BO	  *io_IniCOcon[64];   /* Pointer cardobj  */	
pwr_sClass_ChanCo	  *io_IniCOChan[64];  /* Pointer to chan_CO obj*/
pwr_tInt32		  io_IniCAold[256];   /* Saved CA values	*/  

pwr_tUInt16		  io_IniCORackAddr[64]; /* Rack address for RTP */
pwr_tUInt16		  io_IniCOCardType[64]; /* Card type for RTP */
pwr_tObjid		  io_IniCoCardId[64];	/* GPd 920212 , rtp dioc */


pwr_tInt32		  io_actantco;	    /*  Number of CO signals */
pwr_tBoolean		  io_co_firstlap;

#endif
