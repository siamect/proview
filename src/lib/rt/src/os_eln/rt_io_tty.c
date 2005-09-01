/* 
 * Proview   $Id: rt_io_tty.c,v 1.2 2005-09-01 14:57:57 claes Exp $
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

/* rt_io_tty.c 

   Samlingsfil för kommunikationsrutiner mha DDA via 
   ELN$TTY_xx rutiner.  */

#include $vaxelnc
#include $elnmsg
#include descrip
#include $dda_utility

#include "pwr.h"
#include "rt_io_tty.h"

/****************************************************************************
* Namn          : io_tty_ini( ttnr, ttport, io_tty_read_messobj,
*			      io_tty_read_message,rx_mess_size,
*			      io_tty_write_messobj,
*			      io_tty_write_message,tx_mess_size )
*
* Typ           : pwr_tInt32			Returstatus.
*
* TYP             PARAMETER     IUGF    BESKRIVNING
*
* pwr_tInt8		  ttnr[]	I	Devicenamn, tty-port ex "TTA0".
* PORT		  *ttport	 U	Eln PORT objekt kopplad mot
*					den seriella DDA porten 'ttnr'.
* MESSAGE	  *io_tty_read_messobj	 L{s message objekt.
* char		  **io_tty_read_message  L{s message char pekare.
* pwr_tInt32		  rx_mess_size		 Storlek på message data
* MESSAGE	  *io_tty_write_messobj	 Skriv message objekt
* char		  **io_tty_write_message Skriv message char pekare.
* pwr_tInt32		  tx_mess_size	         Storlek på message data
*
* Beskrivning   : Skapar en PORT mot DDA portens terminallina och sammankopplar
*		  dessa.
*		  Skapar endast message om MESSAGE ptr != NULL
****************************************************************************/
pwr_tInt32 io_tty_ini ( pwr_tInt8  ttnr[],
		   PORT	 *ttport,
		   MESSAGE	*io_tty_read_messobj,
		   char		**io_tty_read_message,
		   pwr_tInt32	rx_mess_size,
		   MESSAGE	*io_tty_write_messobj,
		   char		**io_tty_write_message,
		   pwr_tInt32	tx_mess_size)
{
 static pwr_tInt8	access[8] = "$ACCESS";
 pwr_tInt8		ttfull[40];
 pwr_tInt8		*ttname;
 pwr_tInt32		sts;
 struct		dsc$descriptor_s data_port_name;  	
 PORT		data_port;


 /* Create message objects for this port */
 if (io_tty_read_messobj != NULL)
    {
    ker$create_message( &sts, 
			 io_tty_read_messobj, 
			 io_tty_read_message, 
			 rx_mess_size);
    if ( EVEN(sts)) return sts;
    }
 if (io_tty_write_messobj != NULL)
    {
    ker$create_message( &sts, 
			io_tty_write_messobj, 
			io_tty_write_message, 
			tx_mess_size);
    if ( EVEN(sts)) return sts;
    }
 strcpy(&ttfull,ttnr);
 strcpy(&ttfull[strlen(ttnr)],&access);
 ttname = ttfull;

 data_port_name.dsc$w_length  = strlen(ttname);  	
 data_port_name.dsc$a_pointer = ttname;  		
 data_port_name.dsc$b_class   = DSC$K_CLASS_S;  	
 data_port_name.dsc$b_dtype   = DSC$K_DTYPE_T;  	


 ker$translate_name( &sts, &data_port, &data_port_name, NAME$LOCAL );

 if(ODD(sts))
    ker$create_port( &sts, ttport, NULL);

 if(ODD(sts))
    ker$connect_circuit( &sts, ttport, &data_port, NULL,NULL,NULL,NULL);

 return(sts);
}

/****************************************************************************
* Namn          : io_tty_sdrc(srport, sbuf, sant, mbuf, mant, 
*			      io_tty_read_messobj, io_tty_read_message, 
*			      io_tty_write_messobj, io_tty_write_message,
*                             timeout, nrofsnd, termmask, rcvant) 
*
* Typ           : pwr_tInt32			Returstatus
*
* TYP             PARAMETER     IUGF    BESKRIVNING
*
* PORT		  *srport	I	Seriell kanal DDA, PORT.
* pwr_tUInt8		  *sbuf		IU	Sändbuffer adress.
* pwr_tInt32		  sant		I	Antal byte att sända. NULL medför att
*					ingen sändning sker.
* pwr_tUInt8		  *mbuf		IU 	Mottagningsbuffer.
* pwr_tInt32		  mant		I	Antal byte att mottaga. NULL medför
*					ingen mottagning.
* MESSAGE	  *io_tty_read_messobj	 L{s message objekt.
* char		  **io_tty_read_message  L{s message char pekare.
* MESSAGE	  *io_tty_write_messobj	 Skriv message objekt
* char		  **io_tty_write_message Skriv message char pekare.
* pwr_tUInt16	  timeout	I	Timeouttid i sekunder vid mottagning.
*					NULL medför ingen timeoutfunktion.
* pwr_tInt32		  nrofsnd	I	Antal omsändningsförsök då timeout
*					vid mottagning har inträffat.
*					Används endast då sant och mant > 0.
* DDA$_BREAK_MASK termmask	I	Bitarray för teckenterminering enl.
*					DEC Multinational Character Set.
*					NULL medför terminering på antal tecken.
* pwr_tInt32		  *rcvant	 U    	Returnerar antaler lästa byte.
*
* Beskrivning   : Modulen har tre olika funktioner beroende på anropsparametrar.
*
* 	 	  1. Endast sändning av data. Sant != NULL och mant == NULL.
*	             Detta medför sändning av sbuf med sant byte en gång.
*
*		  2. Endast mottagning av data. Mant != NULL och sant == NULL.
*		     Om termmask finns terminerar läsningen antingen på denna
*  		     mask eller då mant tecken är inlästa. Då termmask == NULL
*		     sker terminering på mant antal tecken.
*		     Om "timeout" != NULL sker alltid terminering då tiden hur
*		     löpt ut. Ingen tömmning av typeaheadbuffern sker.
*
*		  3. Sändning och mottaging av data. Sant och mant != NULL.
*		     - Tömmning av typeaheadbuffern.
*		     - Sändning av sbuf sant tecken.
*		     - Mottaging av mant ( eller teckenterminering ) tecken,
*		       rcvant innehåller antal mottagna tecken.
*		     - Vid timeout sker nrofsnd antal försök.
*
******************************************************************************/
pwr_tInt32 io_tty_sdrc ( PORT            *srport,	/* DDA port 		     */
		    pwr_tUInt8           *sbuf,	/* Sändbuffer		     */
		    pwr_tInt32	    sant,	/* Antal byte att sända	     */
		    pwr_tUInt8	    *mbuf,	/* Motagningsbuffer	     */
		    pwr_tInt32	    mant,	/* Antal byte att mottaga    */
	 	    MESSAGE	    *tty_read_messobj,
		    char	    **tty_read_message,
		    MESSAGE	    *tty_write_messobj,
		    char	    **tty_write_message,
		    pwr_tUInt16	    timeout,	/* Timeouttid i sekunder     */
		    pwr_tInt32	    nrofsnd,	/* Antal omsändingsförsök    
						   vid mottagningstimeout    */
		    DDA$_BREAK_MASK termmask,	/* Termineringsmask          */
		    pwr_tInt32	    *rcvant)	/* Antal mottagna byte	     */
				

{

 pwr_tInt32		sts;

 pwr_tInt32		smdum,option,msgsize,fsts;
 static pwr_tInt32	time_secs = -10000;		/* Timeout tid i msek tidbas */

 LARGE_INTEGER 	waittime;

 DDA$_EXTENDED_STATUS_INFO	ext_sts;


 /****************************************************************************/
 /** Test av anrops parametrar. 					    **/
 /****************************************************************************/

 if( mant <= 0 && sant <= 0 ) return( ELN$_INVALBUFSIZ ); 

 /****************************************************************************/
 /** Initiera läs funktionerna 						    **/
 /****************************************************************************/

 if( mant > 0 )
 {

   msgsize = mant;			/* Sätt messagesize lika med minsta
					   antal lästa byte, oberoende av
					   termineringstyp. Vid terminering
					   via termmask terminarar läsningen
					   även då mant(msgsize) antal tecken
					   blivit inlästa.                   */

   /** Beräkna ut timeuttiden om det finns någon                            **/

   if( timeout != NULL )
   {
       waittime.high = -1;
       waittime.low  = time_secs*timeout;
       option = 2;		  	    
   }
   else
   {
       option = 0;		  
   }

   /** Aktivera termineringsmasken om det finns någon                       **/

   if( termmask != NULL )                   
       option += 5;
   else
       option ++;
 }


 /****************************************************************************/
 /** 		ENDAST SKRIVNING TILL DDA-PORT			 	    **/
 /** Skriv till DDA port "sant" antal tecken från "sbuf" buffern.	    **/
 /****************************************************************************/

 if( sant > 0 && mant <= 0)
 {
     eln$tty_write( &sts,			/* Status	             */
		    srport,			/* DDA port ut	             */
	            sant,			/* Sänd längd	             */
		    sbuf,			/* Sändbuffer	             */
		    &smdum,			/* Antalet sända bytes       */
		    tty_write_messobj,
		    tty_write_message);
		
     return(sts);   
 }


 /****************************************************************************/
 /** 		ENDAST LÄSNING FRÅN DDA-PORT			 	    **/
 /** Läs från DDA port "mant" tecken eller till termineringstecken inlästs. **/
 /** Ingen tömming av typeaheadbuffern före läsning 			    **/
 /****************************************************************************/ 
 if( mant > 0 && sant <= 0)
 {
     eln$tty_read(&sts,				/* Status             	     */
      		  srport,			/* DDA port 		     */
		  msgsize,			/* Antal tecken att läsa     */
		  mbuf,		      		/* Meddelande buffer	     */
		  rcvant,			/* Antal lästa tecken	     */
		  option,			/* Läs option		     */
		  termmask,	     		/* Termineringsmask	     */
		  &waittime,	       		/* Timeout 		     */
		  mant,		       		/* Minsta antal läsbyte	     */
		  &ext_sts,	       		/* Extended status	     */
		  tty_read_messobj,   		/* Message objekt	     */
	          tty_read_message);		/* Message pointer	     */

     return(sts); 
 }
 
 /****************************************************************************/
 /**            SKRIVNING OCH LÄSNING FRÅN DDA-PORT. 		 	    **/
 /** Skriv "sant" tecken ur "sbuf" till TTY. Töm typeaheadbuffern.          **/
 /** Läs "mant" tecken från TTY till "mbuf" eller terminara på tecken.      **/
 /** Vid ELN$_TIMEOUT gör "nrofsnd" antal sänd/läsnings försök		    **/
 /****************************************************************************/
 
 do
 {
     nrofsnd--;

     /** Töm typeaheadbuffern på devicet			            **/
/**************************** JL 920428 ********************
     sts = io_tty_typecl(srport,  
		  tty_read_messobj,
	          tty_read_message);
*************************************************/

     /** Skriv till DDA port "sant" antal tecken från "sbuf" buffern.       **/

     eln$tty_write( &sts,			/* Status	             */
		    srport,			/* DDA port ut	             */
	            sant,			/* Sänd längd	             */
		    sbuf,			/* Sändbuffer	             */
		    &smdum,			/* Antalet sända bytes       */
		    tty_write_messobj,
		    tty_write_message);
		                                            
     if(sts == ELN$_PARITY || sts == ELN$_BREAK_DETECTED || 
        sts == ELN$_FRAME_ERROR) 
     {
        fsts = io_tty_readfault(srport);

	if(EVEN(fsts))
        {
            eln$tty_write( &sts,		/* Status	             */
		           srport,		/* DDA port ut	             */
	                   sant,		/* Sänd längd	             */
		           sbuf,		/* Sändbuffer	             */
		           &smdum,		/* Antalet sända bytes       */
		           tty_write_messobj,
		           tty_write_message);
	}
     }
     if(EVEN(sts))return(sts);

     /** Läs från DDA port.						    **/

     eln$tty_read(&sts,	   			/* Status		     */
      		  srport,			/* DDA port 		     */
		  msgsize,	       		/* Antal tecken att läsa     */
	          mbuf,		       		/* Meddelande buffer	     */
		  rcvant,			/* Antal lästa tecken	     */
		  option,			/* Läs option		     */
		  termmask,	       		/* Termineringsmask	     */
	          &waittime,	       		/* Timeout 		     */
		  mant,		       		/* Minsta antal läsbyte	     */
		  &ext_sts,	       		/* Extended status	     */
		  tty_read_messobj,	    	/* Message objekt	     */
	          tty_read_message);		/* Message pointer	     */

     if(sts == ELN$_PARITY || sts == ELN$_BREAK_DETECTED || 
        sts == ELN$_FRAME_ERROR) 
     {
        fsts = io_tty_readfault(srport);
     }

     if(EVEN(sts))return(sts);
 } while ( sts == ELN$_TIMEOUT && nrofsnd >= 0 );
 return(sts);				
}

/****************************************************************************
* Namn          : io_tty_typecl(clttport)
*
* Typ           : pwr_tInt32                 Returstatus.
*
* TYP             PARAMETER     IUGF    BESKRIVNING
*
* PORT		  *clttport	I	Seriell kanal DDA, PORT.
* MESSAGE	  *tty_read_messobj	L{s message objekt 
* char		  **tty_read_message	L{s messsage char pekare.
*
* Beskrivning   : Tömmer typeahedbuffer på den seriella kanalen med clttport
*		  PORTen.
*
*******************************************************************************/

pwr_tInt32 io_tty_typecl( PORT	*clttport,	/* DDA port 		      */
	 	     MESSAGE	    *tty_read_messobj,
		     char	    **tty_read_message)
{
 pwr_tInt32		sts;
 pwr_tInt32		nbbyte,bytebuff;
 pwr_tUInt8		typaheadbuf[200];

 DDA$_EXTENDED_STATUS_INFO	ext_sts;

 bytebuff = sizeof(typaheadbuf);

 do
 {
     
     eln$tty_read(&sts,		  		/* Status		     */
      		  clttport,			/* DDA port 		     */
		  bytebuff,			/* Antal tecken att läsa     */
		  &typaheadbuf,	      		/* Meddelande buffer	     */
		  &nbbyte,	       		/* Antal lästa tecken	     */
		  1,		  		/* Läs option		     */
		  NULL,		  		/* Termineringsmask	     */
		  NULL,		  		/* Timeout 		     */
		  0,		  		/* Minsta antal läsbyte	     */
		  &ext_sts,	      		/* Extended status	     */
		  tty_read_messobj,	 	/* Message objekt	     */
	          tty_read_message);	      	/* Message pointer	     */

     if(EVEN(sts))return(sts);

 }while( nbbyte >= bytebuff );			/* Fler tömmningar kan ske   */
 return(sts);
}

/****************************************************************************
* Namn          : io_tty_typere(rettport, mtbuf, tant, rcvant)
*
* Typ           : pwr_tInt32                 Returstatus
*
* TYP             PARAMETER     IUGF    BESKRIVNING
*
* PORT		   *rettport	I	DDA port.
* pwr_tUInt8		   *mtbuf	IU	Motagningsbuffer
* pwr_tInt32		   tant		I	Antal byte att mottaga  
* pwr_tInt32		   *rcvtant)	 U	Antal mottagna byte	
*
* Beskrivning   : Läser "tant" byte från typeaheadbuffern och lägger dessa
*		  i "mtbuf". Rcvant returnerar antalet lästa byte.
*
******************************************************************************/

pwr_tInt32 io_tty_typere( PORT	*rettport,	/* DDA port 		     */
		     pwr_tUInt8	*mtbuf,		/* Motagningsbuffer	     */
		     pwr_tInt32	tant,		/* Antal byte att mottaga    */
		     pwr_tInt32	*rcvtant)	/* Antal mottagna byte	     */

{
 pwr_tInt32		sts;

 DDA$_EXTENDED_STATUS_INFO	ext_sts;

     
 eln$tty_read(&sts,	     			/* Status		     */
      	      rettport,				/* DDA port 		     */
	      tant,	      			/* Antal tecken att läsa     */
	      mtbuf,	       			/* Meddelande buffer	     */
	      rcvtant,	      			/* Antal lästa tecken  	     */
	      1,				/* Läs option		     */
	      NULL,				/* Termineringsmask	     */
	      NULL,	      			/* Timeout 		     */
	      0,	    			/* Minsta antal läsbyte	     */
	      &ext_sts,	     			/* Extended status	     */
	      NULL,	    			/* Message objekt	     */
	      NULL);	       			/* Message pointer	     */

 return(sts);
}


/****************************************************************************
* Namn          : io_tty_exit(exttport)
*
* Typ           : pwr_tInt32                 Returstatus
*
* TYP             PARAMETER     IUGF    BESKRIVNING
*
* PORT		  *exttport	I	DDA port.
*
* Beskrivning   : Tar med kopplingen mellan en job port och en DDA port.
*
******************************************************************************/

pwr_tInt32 io_tty_exit( PORT		*exttport) 	/* DDA port 		     */


{
 pwr_tInt32		sts;

 ker$disconnect_circuit ( &sts, exttport );
 if(ODD(sts)) ker$delete( &sts, exttport );
 return(sts);
}

/****************************************************************************
* Namn          : io_tty_readfault(fttport)
*
* Typ           : pwr_tInt32                 Returstatus
*
* TYP             PARAMETER     IUGF    BESKRIVNING
*
* PORT		  *fttport	I	DDA port.
*
* Beskrivning   : Denna funktion anropas då ett av följande fel uppstått vid
*		  ELN$TTY_READ , BREAK_DETECTED , FRAME_ERROR ,PARITY.
*	          Läsning sker tills ovan fel ej uppstår.
*
******************************************************************************/

static pwr_tInt32 io_tty_readfault( PORT	*fttport) 	/* DDA port 		     */


{
 pwr_tInt32		sts;
 pwr_tInt32		nbbyte,i;
 pwr_tUInt8		faultbuf[20];

 DDA$_EXTENDED_STATUS_INFO	ext_sts;

 for(i = 0 ; i < 100 ; i++ )
 {
     eln$tty_read(&sts,		  		/* Status		     */
      		  fttport,			/* DDA port 		     */
		  20,				/* Antal tecken att läsa     */
		  &faultbuf,	      		/* Meddelande buffer	     */
		  &nbbyte,	       		/* Antal lästa tecken	     */
		  1,		  		/* Läs option		     */
		  NULL,		  		/* Termineringsmask	     */
		  NULL, 			/* Timeout 		     */
		  0,		  		/* Minsta antal läsbyte	     */
		  &ext_sts,	      		/* Extended status	     */
		  NULL,				/* Message objekt	     */
	          NULL);	   	   	/* Message pointer	     */

     if(ODD(sts))
     {
        return(sts);
     }
     else
     {
       if(!(sts == ELN$_PARITY || sts == ELN$_BREAK_DETECTED 
            || sts == ELN$_FRAME_ERROR)) return (sts);
     }
 }
}
/****************************************************************************
* Namn          : io_tty_set7bin(sstport)
*
* Typ           : pwr_tInt32                 Returstatus
*
* TYP             PARAMETER     IUGF    BESKRIVNING
*
* PORT		  *sstport	I	DDA port.
*
* Beskrivning   : Sätter terminalkarakteristiken till characher size 7
*		  nottsync och passall.
*
******************************************************************************/

extern pwr_tInt32 io_tty_set7bin( PORT	*sstport) /* DDA port 		     */


{
 pwr_tInt32		sts;

 DDA$_TERMINAL_CHAR_PTR	termcharP;

 eln$tty_get_characteristics(sstport,
			     &termcharP,
			     &sts);
 if(EVEN(sts))return (sts);

 termcharP->passall   = TRUE;
 termcharP->tty_sync  = FALSE;
 termcharP->char_size = 7;

 eln$tty_set_characteristics(sstport,
			     termcharP,
			     &sts);

 return (sts);
}
