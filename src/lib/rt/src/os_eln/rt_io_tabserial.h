#ifndef rt_io_tabserial_h
#define rt_io_tabserial_h

/* rt_io_tabserial.h -- <short description>

   PROVIEW/R
   Copyright (C) 1996 by Comator Process AB.

   <Description>.  */

pwr_sClass_IOUnit_ABBSR		*io_IniconSerP[20];		/* Pointer to I/O-unit obj  */
pwr_sClass_SRikt		*io_IniSRiktP[20];		/* Pointer to SRikt object */ 
pwr_tString16			io_IniTermName[20];		/* Name serial line */ 
PORT				io_IniPortIdSer[20];		/* Port id serial line */ 
MUTEX				io_IniMutexSer[20];		/* Mutex to lock/unlock comm line */ 
pwr_tUInt16			io_IniUnitAddress[20];		/* Unit address  */ 
  
pwr_sClass_ai_value_base	*io_IniAIvalSerP;		/* Pointer to AI-value 	*/
pwr_sClass_Ai			*io_IniAI_objSerP[320];		/* Pointer to AI_OBJECT	*/	
pwr_sClass_ChanAi		*io_IniAIChanSerP[320];		/* Pointer to chan_AI obj*/
pwr_sClass_ChanAit		*io_IniAITChanSerP[320];	/* Pointer to chan_AIT obj*/
pwr_tInt16			io_IniAIscanintSer[320];	/* Scanning interval count */
pwr_tObjid			io_IniconobjidSer[20];


pwr_sClass_ao_value_base	*io_IniAOvalSerP;		/* Pointer to AO-value 	*/
pwr_tInt16			io_IniAOchanSer[320];		/* Channel on device   	*/
pwr_tFloat32			io_IniAOoldSer[320];		/* Old/saved value 	*/	
pwr_sClass_Ao			*io_IniAO_objSerP[320];		/* Pointer to AO_OBJECT	*/	
pwr_sClass_ChanAo		*io_IniAOChObjSerP[320];	/* Pointer to chan_AO obj*/


pwr_sClass_di_value_base	*io_IniDIvalSerP;		/* Pointer DI-value	*/	
pwr_tInt16	 		io_IniDIKortnrSer[20];		/* Number of word on device */


pwr_sClass_do_value_base	*io_IniDOvalSerP;		/* Pointer to DO-value 	*/	
pwr_tUInt16			io_IniDOoldSer[20];		/* Old/saved value bitdata */	
pwr_tInt16			io_IniDOKortnrSer[20];		/* Word number on device    */

EVENT				io_IniABBSRInitDone;		/* Event initialization done */
pwr_tUInt32			io_IniErrorCount[20];		/* Saved errorcount */	
pwr_tUInt32			io_ErrorCheckCounter;		/* Scancounter to check error*/
pwr_tInt16			io_IniProcNumber[20];		/* Process number */
MESSAGE				io_tty_read_messobj[5];		/* Message used in tty read */
char				*io_tty_read_message[5];
MESSAGE				io_tty_write_messobj[5];	/* Message used in tty write */
char				*io_tty_write_message[5];

#define ABBSR_MAX_TERMINAL	8

#endif
