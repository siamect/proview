/* rt_io_ssab.c -- <short description>

   PROVIEW/R
   Copyright (C) 1996 by Comator Process AB.

   Driver functions for SSAB cardtype.
   Devicetypes DI,DO,AI,AO and COUNTers.
   This module includes exeptionhandler for all faults.

   io_writeerr and io_readerr signals machinecheck faults.

   io_fatal_error signal that an unexpected exeption has accured.  */

#include $vaxelnc
#include chfdef
#include descrip
#include "rt_io_ssab.h"

/****************************************************************************
* Name		: ssab_close
* 
* Type		: int		return status.
*
* TYPE		  PARAMETER	IOGF	DESCRIPTION
*
* int		  dev	I	device
*
* Description:	  Delete a DEVICE object.
****************************************************************************/
int ssab_close(	
		int	*dev)
{

 int	sts;

 ker$delete( &sts, *dev);
 return sts;

}/*  end ssab_close  */

/****************************************************************************
* Name		: ssabd_ini
* 
* Type		: int		return status.
*
* TYPE		  PARAMETER	IOGF	DESCRIPTION
*
* int		  inidev	I	device descriptor adress
* int		  adressini	 O	register pointeradress 
* int		  relvec	I	relative vector number 
* int 		  hv_device	I	device variable
*
* Description:	  Create a DEVICE object for digital I/O.
****************************************************************************/
int ssabd_ini(	char	*inidev,	
		int	adressini,
		int 	hv_device,
		int	relvec)
{

 int	inistat;
 static char		devname[20];
 $DESCRIPTOR( devname_desc, devname);

 strcpy( devname, inidev);
 devname_desc.dsc$w_length = strlen(devname);  	

 ker$create_device(
		&inistat,			/* Returstatus 		*/
	        &devname_desc,			/* Device name 		*/
		relvec,				/* Relative vector	*/
		NULL,				/* ISR-adr. no interupt	*/
		NULL,				/* reg.pointer lengdh	*/
		NULL,				/* Region pointer	*/
		adressini,			/* Register pointer	*/
		NULL,				/* Adapter pointer	*/
		NULL,				/* Vector pointer	*/
		NULL,				/* Interupt priority	*/
		hv_device,			/* Device variable	*/
		1,				/* Device count		*/
		NULL);				/* pwr isr		*/

 return(inistat);

}/*  end ssabd_ini  */

/****************************************************************************
* Name		: ssabai_ini
* 
* Type		: int		return status.
*
* TYPE		  PARAMETER	IOGF	DESCRIPTION
*
* int		  inidevai	I	device descriptor adress
* int		  adressiniai	 O	register pointeradress 
* int 		  hv_deviceai	I	device variable
*
* Description   : Create a DEVICE object for analog input.
****************************************************************************/
int ssabai_ini(	char *inidevai,
		int adressiniai,
		int hv_deviceai,
		int	relvec)
{

 int	inistat;
 static char		devname[20];
 $DESCRIPTOR( devname_desc, devname);

 strcpy( devname, inidevai);
 devname_desc.dsc$w_length = strlen(devname);  	

 ker$create_device(
		&inistat,			/* Returstatus 		*/
	        &devname_desc,			/* Device name 		*/
		relvec,				/* Relative vector	*/
		SSABAIINT,			/* ISR-adr.interupt	*/
		NULL,				/* reg.pointer lengdh	*/
		NULL,				/* Region pointer	*/
		adressiniai,			/* Register pointer	*/
		NULL,				/* Adapter pointer	*/
		NULL,				/* Vector pointer	*/
		NULL,				/* Interupt priority	*/
		hv_deviceai,			/* Device variable	*/
		1,				/* Device count		*/
		NULL);				/* pwr isr		*/

 return(inistat);

}/*  end ssabai_ini  */

/****************************************************************************
* Name		: ssabao_ini
* 
* Type		: int		return status.
*
* TYPE		  PARAMETER	IOGF	DESCRIPTION
*
* int		  inidevao	I	device descriptor adress
* int		  adressiniao	 U	register pointeradress 
* int 		  hv_deviceao	I	device variable
*
*
* Description	: Create a DEVICE object for analog output.
****************************************************************************/
int ssabao_ini(	char *inidevao,
		int adressiniao,
		int hv_deviceao,
		int	relvec)
{

 int	inistat;
 static char		devname[20];
 $DESCRIPTOR( devname_desc, devname);

 strcpy( devname, inidevao);
 devname_desc.dsc$w_length = strlen(devname);  	

 ker$create_device(
		&inistat,			/* Returstatus 		*/
	        &devname_desc,			/* Device name 		*/
		relvec,				/* Relative vector	*/
		SSABAOINT,			/* ISR-adr.interupt	*/
		NULL,				/* reg.pointer lengdh	*/
		NULL,				/* Region pointer	*/
		adressiniao,			/* Register pointer	*/
		NULL,				/* Adapter pointer	*/
		NULL,				/* Vector pointer	*/
		NULL,				/* Interupt priority	*/
		hv_deviceao,			/* Device variable	*/
		1,				/* Device count		*/
		NULL);				/* pwr isr		*/

 return(inistat);

}/*  end ssabao_ini  */


/****************************************************************************
* Name		: ssabdi_read
* 
* Type		: unsigned short	Input value from device.
*
* TYPE		  PARAMETER	IOGF	DESCRIPTION
*
* struct 
* register_def    *reg_diP	I	Pointer to input CSR.
* int		  grupp		I	Word on device.
*
* Description   : Read bitdata from digitalinput device.
****************************************************************************/
#if 0
unsigned short	ssabdi_read(	register_def	*reg_diP,
				int		grupp)
{
  static unsigned short di_data;

  io_readerr = FALSE;  

  switch(grupp)
  {
      case 1:
          di_data = read_register(&reg_diP->diodata1);  
          break;

      case 2:
          di_data = read_register(&reg_diP->diodata2);  
          break;
  }
  return(di_data);
}/*  end ssabdi_read  */
#endif
unsigned short	ssabdi_read(	register_def	*reg_diP,
				int		word)
{
  io_readerr = FALSE;  

  return read_register(&reg_diP->csr[word]);  
}/*  end ssabdi_read  */

/****************************************************************************
* Name		: ssabdo_write
* 
* Type		: viod
*
* TYPE		  PARAMETER	IOGF	DESCRIPTION
*
* unsigned short  bit16_dodata	I	Output value.
* struct 
* register_def    *reg_doP	I	Pointer to output CSR.
* int		  grupp		I	Word on device.
*
* Description:    Write bitdata on digitaloutput device.
****************************************************************************/
void	ssabdo_write(	unsigned short	bit16_dodata,
			register_def	*reg_doP,
			int		word)
{
  static short int	dumread1;
  io_writeerr = FALSE;

  /*	  
  **  Dummy read för att ej få FATLMEMERR
  */	  
  dumread1 = reg_doP->csr[word];

  write_register(bit16_dodata,&reg_doP->csr[word]);

}/*  end ssabdo_write  */
                           
/****************************************************************************
* Name		: ssabai_read
* 
* Type		: unsigned short	Input value from device.
*
* TYPE		  PARAMETER	IOGF	DESCRIPTION
*
* struct 
* ai_register_def *reg_aiP	I	Pointer to input CSR.
* int		  ai_channel	I	Channel nummber ( 0 - 31 ).
*
* Description:    Read bitdata from analoginput device.
****************************************************************************/
unsigned short	ssabai_read(ai_register_def *reg_aiP,
			    int		    ai_channel)
{
  static unsigned short ai_data;

  static short int	dumread1;
  static int	i,iowr;

  static LARGE_INTEGER	 waittime = {-100000,-1};	/* wait 10 ms */

  io_writeerr	= FALSE ;
  io_readerr	= FALSE ;


  /*	  
  **  Dummy read för att ej få FATLMEMERR
  */	  

  dumread1 = reg_aiP->aicsr0;

  /*	  
  **  Start conversion  
  */	  

  write_register(ai_channel,&reg_aiP->aicsr0 );	

  /*	  
  **  Read register until coversion is ready  
  */	  

  for( i = 0 ;  i < RMAX ; i++ )
  {
      ai_data = read_register( &reg_aiP->aicsr0 );
      if( ai_data & READYAI )   
        return(ai_data &= MASKAI); 
  }

  /*	  
  **  Error conversion on channel ( timeout ) 
  */	  

  return( AI_FAULT );

}/*  end ssabai_read  */

                           
/****************************************************************************
* Name		: ssabaiup_read
* 
* Type		: unsigned short	Input value from device.
*
* TYPE		  PARAMETER	IOGF	DESCRIPTION
*
* struct 
* ai_register_def *reg_aiP	I	Pointer to input CSR.
* int		  ai_channel	I	Channel nummber ( 0 - 31 ).
*
* Description:    Read bitdata from analoginput device.
****************************************************************************/
short		ssabaiup_read( register_def 	*reg_aiP,
			      int		ai_channel)
{
  static unsigned short ai_data;

  io_readerr	= FALSE ;

  return read_register( &reg_aiP->csr[ai_channel]);
}/*  end ssabaiup_read  */

/****************************************************************************
* Name		: ssabao_write
* 
* Type		: void
*
* TYPE		  PARAMETER	IOGF	DESCRIPTION
*
* unsigned short  bit16_aodata	I	Output value.
* struct 
* ao_register_def *reg_aoP	I	Pointer to output CSR.
* int		  ao_channel	I	Channel nummber ( 0 - 3 ).
*
* Description:    Write bitdata on analogoutput device.
****************************************************************************/
void	ssabao_write(	unsigned short	bit16_aodata,
			register_def	*reg_aoP,
			int		ao_channel)
{
  static short int	dumread1;

  io_writeerr = FALSE ;

  /* Dummy read för att ej få FATLMEMERR */	  
  dumread1 = reg_aoP->csr[ao_channel];

  write_register(bit16_aodata,&reg_aoP->csr[ao_channel]);
}/*  end ssabao_write  */

/****************************************************************************
* Name		: ssabco_read
* 
* Type		: void
*
* TYPE		  PARAMETER	IOGF	DESCRIPTION
*
* struct 
* co_register_def *reg_coP	I	Pointer to input CSR.
* int		  numofco	I	Number of word to read on device.
* unsshort        *co_outvalueP  O      Pointer to outputvalue array.
*
* Description   : Read bitdata from counterdevice.
*  Modif 94.11.18   store the index of the  word that is currently read
*   ( co_id_p in order to read the seond word if the first fail with an error
* other than machinecheck ) 
****************************************************************************/
void		ssabco_read(co_register_def *reg_coP,
			    int		    numofco,
			    int		    *co_id_p , 
			    unsigned short  *co_outvalueP,
			    int		     channel)
{
  static int		 i;

  io_readerr = FALSE;  
  
  for(i = 0 ; i < numofco ; i++ , co_outvalueP++) 
  {
      switch(i)
      {
          case 0:
              *co_id_p = 0 ; 
              *co_outvalueP = read_register(&reg_coP->cocsr1[channel]);
          break;
    
          case 1:
              *co_id_p = 1 ; 
              *co_outvalueP = read_register(&reg_coP->cocsr2[channel]);  
              *co_id_p = 2 ; 
          break;

      } /* switch i */
  } /* for */
  return;
}/*  end ssabco_read  */

/****************************************************************************
* Name		: ssabco_write
* 
* Type		: viod
*
* TYPE		  PARAMETER	IOGF	DESCRIPTION
*
* unsigned short  *codata	I	Pointer to outputdata.
* struct 
* co_register_def *reg_coP	I	Pointer to output CSR.
*
* Description:    Write configure/initial data to counter-device.
****************************************************************************/
void	ssabco_write(unsigned short	*codata,
		     co_register_def	*reg_coP,
		     int		channel)
{
  static short int	dumreadc1;

  io_writeerr = FALSE;

  /*	  
  **  Dummy read för att ej få FATLMEMERR
  */	  
  dumreadc1 = reg_coP->cocsr1[channel];

  write_register(*codata,&reg_coP->cocsr1[channel]);

  dumreadc1 = reg_coP->cocsr2[channel];

  codata++;
  write_register(*codata,&reg_coP->cocsr2[channel]);

  return;
}/*  end ssabco_write  */

/****************************************************************************
* Name		: mashfailread
* 
* Type		: pwr_tBoolean
*
* TYPE		  PARAMETER	IOGF	DESCRIPTION
*
* Upphov	: Jan Lindström
*
* Description:    Exeption handler for machinecheck faults READ.
****************************************************************************/
pwr_tBoolean	machfailread(signalP,mechanismP)

struct	chf$signal_array *signalP;
struct	chf$mech_array 	 *mechanismP;

{
  static int	rsts;
  globalvalue	ker$_machinechk;
  globalvalue	ss$_unwind;

  if(signalP->chf$l_sig_name == ss$_unwind) return TRUE;

  if(signalP->chf$l_sig_name == ker$_machinechk)
  {
      io_readerr = TRUE ;			/* Machine faults */
      ker$unwind(&rsts,mechanismP->chf$l_mch_depth,NULL);			
      if(! (rsts & 1))return FALSE;	  
  }
  else
  {
      io_fatal_error = TRUE ;                   /* Unexpected fault */
      ker$unwind(&rsts,mechanismP->chf$l_mch_depth,NULL);			
      if(! (rsts & 1))return FALSE;	  
  }
  return TRUE;
}

/****************************************************************************
* Name		: mashfailwrite
* 
* Type		: pwr_tBoolean
*
* TYPE		  PARAMETER	IOGF	DESCRIPTION
*
* Description:    Exeption handler for machinecheck faults WRITE.
****************************************************************************/
pwr_tBoolean	machfailwrite(signalP,mechanismP)

struct	chf$signal_array *signalP;
struct	chf$mech_array 	 *mechanismP;

{
  static int	wsts;
  globalvalue	ker$_machinechk;
  globalvalue	ss$_unwind;

  if(signalP->chf$l_sig_name == ss$_unwind) return TRUE;

  if(signalP->chf$l_sig_name == ker$_machinechk)
  {
      io_writeerr = TRUE ;
      ker$unwind(&wsts,mechanismP->chf$l_mch_depth,NULL);			
      if(! (wsts & 1))return FALSE;	  
  }
  else
  {
      io_fatal_error = TRUE ;		/* Unexpected fault */
      ker$unwind(&wsts,mechanismP->chf$l_mch_depth,NULL);			
      if(! (wsts & 1))return FALSE;	  
  }
  return TRUE;
}

/****************************************************************************
* Name		: SSABAIINT
* 
* Type		: void
*
* TYPE		  PARAMETER	IOGF	DESCRIPTION
*
* Description:    Interupt routine for unexpected interupt for AI-signals.
****************************************************************************/
void	SSABAIINT()
{
    return;
}
/****************************************************************************
* Name		: SSABAOINT
* 
* Type		: void
*
* TYPE		  PARAMETER	IOGF	DESCRIPTION
*
* Description:    Interupt routine for unexpected interupt for AO-signals.
****************************************************************************/
void	SSABAOINT()
{
    return;
}

/****************************************************************************
* Name		: ssabco_read2
* 
* Type		: void
*
* TYPE		  PARAMETER	IOGF	DESCRIPTION
*
* struct 
* co_register_def *reg_coP	I	Pointer to input CSR.
* int		  numofco	I	Number of word to read on device.
* unsshort        *co_outvalueP  O      Pointer to outputvalue array.
*
* Description   : Read only the second word of the register        SG 941115
****************************************************************************/
void		ssabco_read2(co_register_def *reg_coP,
			    int		    numofco,
			    int		    *co_id_p , 
			    unsigned short  *co_outvalueP,
			    int		    channel)
{
  static int		 i;

  io_readerr = FALSE;  
  
  co_outvalueP++ ; 
  *co_id_p = 1 ; 
  *co_outvalueP = read_register(&reg_coP->cocsr2[channel]);  
  *co_id_p = 2 ; 
  return;

}/*  end ssabco_read2  */


/****************************************************************************
* Name		: machfailread_co
* 
* Type		: pwr_tBoolean
*
* TYPE		  PARAMETER	IOGF	DESCRIPTION
*
*
* Description:    Exeption handler for machinecheck faults READ in CO kort.
*
*  MODIF  SG 941115  
*  Do not return io_fatal when te error is other than machinecheck.
****************************************************************************/
pwr_tBoolean	machfailread_co(signalP,mechanismP)

struct	chf$signal_array *signalP;
struct	chf$mech_array 	 *mechanismP;

{
  static int	rsts;
  globalvalue	ker$_machinechk;
  globalvalue	ss$_unwind;

  if(signalP->chf$l_sig_name == ss$_unwind) return TRUE;

  if(signalP->chf$l_sig_name == ker$_machinechk)
  {
      io_readerr = TRUE ;			/* Machine faults */
      ker$unwind(&rsts,mechanismP->chf$l_mch_depth,NULL);			
      if(! (rsts & 1))return FALSE;	  
  }
  else
  {
      io_readerr = TRUE ;			/* Machine faults */
      io_readerr_co_nb++ ; 
      io_readerr_co_sts = signalP->chf$l_sig_name ; 
      ker$unwind(&rsts,mechanismP->chf$l_mch_depth,NULL);			
      if(! (rsts & 1))return FALSE;	  
  }
  return TRUE;
}
