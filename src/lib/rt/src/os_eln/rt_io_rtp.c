/* 
 * Proview   $Id: rt_io_rtp.c,v 1.2 2005-09-01 14:57:56 claes Exp $
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

/* rt_io_rtp.c 
   Contains RTP io routines.  */

#if 0
#include <kerneldef.h>
#include <kernelmsg.h>
#endif
#include <stdio.h>
#include $vaxelnc
#include "pwr.h"
#include "rt_io_rtp.h"
#include "rt_io_base.h"

pwr_tInt16 io_RTPerrno;

/* Can't be static when call to RTP-routines from same JOB and 
   diffrent processes  */

static pwr_tUInt16	_CtlBuf[20];
static pwr_tUInt16	_OutBuf[20];
static pwr_tUInt16	_InBuf[20];
static pwr_tUInt16	_StatBuf[20];

static pwr_tUInt32	_CmdBuf[6]=
{
    0,
    0,
    _CtlBuf,
    _OutBuf,
    _InBuf,
    _StatBuf
};

/************************************************************************
*
* Name: rtp_init()		
*
* Type:	pwr_tBoolean			    -	Success	
*
* TYPE		PARAMETER	IOGF	DESCRIPTION
* pwr_tInt16		io_RTPerrno	  G	RTP felkod
* 
*
* Description:	Initiates rtp-driver and resets io-rack. This should be done 
*		once after power up.
*************************************************************************/

pwr_tBoolean rtp_init()
{

    pwr_tInt32   timer_flag;
    pwr_tInt32   Number_interrupts;
    pwr_tInt32   iobc_priority;

    timer_flag = 0;
    Number_interrupts = 3;
    iobc_priority = 4;

    /*** Initiate driver ***/
    io_RTPerrno = rtp$init(timer_flag,Number_interrupts,iobc_priority);
   
    if (io_RTPerrno != 0)
	return(FALSE);

    /*** Build a resetcommand and send this ***/
    _CmdBuf[0] = CMD_IOBC_OUT;
    _CtlBuf[0] = IOBC_RESET;
    io_RTPerrno = rtp$exec(_CmdBuf);
    if (io_RTPerrno != 0)
	return(FALSE);

    return(TRUE);
} /* END rtp_init */

/************************************************************************
*
* Name:	rtpdi_read(Device,Card,Word,Type)	
*
* Type:	pwr_tUInt16				Read bitpattern
*
* TYPE		PARAMETER	IOGF	DESCRIPTION
* pwr_tUInt8		Device		I	Rack address (30-3F)
* pwr_tUInt8		Card		I	Card address (1-F)
* pwr_tUInt8		Word		I	Which word 
*					(if more than one is possible)
* pwr_tUInt32	Type		I	RTP Card type (see io_rtp.h)
*
* pwr_tBoolean	io_readerr	  G	Sätts TRUE om fel 
* pwr_tInt16		io_RTPerrno	  G	RTP felkod
*
* Description:	Read digital inputs.
*************************************************************************/

pwr_tUInt16	rtpdi_read(pwr_tUInt8    Device,
		   pwr_tUInt8    Card,
		   pwr_tUInt8    Word,
		   pwr_tUInt32   Type)
{

    /* Build "read DI" command in standard RTP I/O-mode */
    _CmdBuf[0] = CMD_RTP_LIST;
    _CmdBuf[1] = 0;
    _CtlBuf[0] = CTL_DI;
    _CtlBuf[1] = Device;
    _CtlBuf[2] = 1;		    /* One command word */
    _CtlBuf[3] = 0x100 | Card;    /* Disable interupt, random access */
    _CtlBuf[4] = 0;

    /* EXEC */
    io_RTPerrno = rtp$exec(_CmdBuf);
    if (io_RTPerrno != 0)
	io_readerr = TRUE;
    else
	io_readerr = FALSE;

#ifdef DEBUG
    printf("Dev=%X STATUS:%X\n",Device,io_RTPerrno);
#endif

    /* Reverse word */
    _InBuf[0] = io_ReverseWord(&(_InBuf[0]));

    /* Return read pattern */
    return(_InBuf[0]);

} /* END rtpdi_read */

/************************************************************************
*
* Name:	rtpdo_write(Data,Device,Card,Word,Type)	
*
* Type:	void				Read bitpattern
*
* TYPE		PARAMETER	IOGF	DESCRIPTION
* pwr_tUInt16	Data		I	Outvalue
* pwr_tUInt8		Device		I	Rack address (30-3F)
* pwr_tUInt8		Card		I	Card address (1-F)
* pwr_tUInt8		Word		I	Which word 
*					(if more than one is possible)
* pwr_tUInt32	Type		I	RTP Card type (see io_rtp.h)
*
* pwr_tBoolean	io_readerr	  G	Sätts TRUE om fel 
* pwr_tInt16		io_RTPerrno	  G	RTP felkod
*
* Description:	Write data to digital outputs.
*************************************************************************/

void	rtpdo_write(pwr_tUInt16  Data,
		    pwr_tUInt8   Device,
		    pwr_tUInt8   Card,
		    pwr_tUInt8   Word,
		    pwr_tUInt32  Type)
{

    /* Build "write DO" command in standard RTP I/O-mode */
    _CmdBuf[0] = CMD_RTP_LIST;
    _CtlBuf[0] = CTL_DAO;
    _CtlBuf[1] = Device;
    _CtlBuf[2] = 1;		    /* One command word */
    _CtlBuf[3] = 0x100 | Card;    /* Disable interupt, random access */
    _CtlBuf[4] = 0;

    /* Reverse data word */
    _OutBuf[0] = Data;
    _OutBuf[0] = io_ReverseWord(&(_OutBuf[0]));

    /* EXEC */
    io_RTPerrno = rtp$exec(_CmdBuf);
    if (io_RTPerrno != 0)
	io_writeerr = TRUE;
    else
	io_writeerr = FALSE;

#ifdef DEBUG
    printf("STATUS:%X\n",io_RTPerrno);
#endif


} /* END rtpdo_write */

/************************************************************************
*
* Name:	rtpai_read(Device,Card,Channel,Type)	
*
* Type:	pwr_tUInt16				Read bitpattern
*
* TYPE		PARAMETER	IOGF	DESCRIPTION
* pwr_tUInt8		Device		I	Rack address (30-3F)
* pwr_tUInt8		Card		I	Card address (1-F)
* pwr_tUInt8		Channel		I	Which AI channel should be read
*					(if more than one is possible)
* pwr_tUInt32	Type		I	RTP Card type (see io_rtp.h)
*
* pwr_tBoolean	io_readerr	  G	Sätts TRUE om fel 
* pwr_tInt16		io_RTPerrno	  G	RTP felkod
*
* Description:	Read analog inputs. 
*		The A/D converter should be in adress 0 (slot 5)
*************************************************************************/

pwr_tUInt16	rtpai_read(pwr_tUInt8    Device,
		   pwr_tUInt8    Card,
		   pwr_tUInt8    Channel,
		   pwr_tUInt32   Type)
{

    union AICMD
    {
	struct
	{
	    int Gain:4;
	    int Chan:3;
	    int Adr:4;
	    int NU2:1;
	    int	Odd:1;
	    int	Diff:1;
	    int TD:1;
	    int NU1:1;
	} BM;

	short	W;

    } AICmd;

    pwr_tInt16	Value;

    AICmd.W = 0;

    /* Build "read AI" command in standard RTP I/O-mode */
    _CmdBuf[0] = CMD_RTP_LIST;

    switch (Type & 0x000000FF)
    {
	case RTP_AI_7436:
	    _CtlBuf[0] = CTL_WRAI_P;
	    break;

	default:
	    /* Cardtype does not exist */
	    printf("Cardtype does not exist!\n");
	    io_readerr = TRUE;
	    return(0);
	    break;
    }

    _CtlBuf[1] = Device;
    _CtlBuf[2] = 0x500 | RTP_AD_ADR;    /* Disable interupt, random access */
    _CtlBuf[3] = 1;		    /* One command word */
    
    if ((Type & RTP_DIFFR) != 0)    /* Differential input ? */
	{
	AICmd.BM.Diff = 0;	    /* Yes == 0 !! */
	AICmd.BM.Chan = Channel;
	}
    else
	{
	AICmd.BM.Diff = 1;
	AICmd.BM.Odd = (Channel & 1); /* Split address */ 
	AICmd.BM.Chan = (Channel >> 1);
	}

    AICmd.BM.Adr = Card;	

    if (Type & RTP_OTD)		    /* Open transducer detection */
	AICmd.BM.TD = 1;

    AICmd.BM.Gain = (Type & RTP_GAIN) >> 12;	    /* Gain */
    
    _CtlBuf[4] = AICmd.W;

    _CtlBuf[5] = 0;


    /* EXEC */
    io_RTPerrno = rtp$exec(_CmdBuf);
    if (io_RTPerrno != 0)
	io_readerr = TRUE;
    else
	io_readerr = FALSE;

#ifdef DEBUG
    {
    int i;
    printf("CMD[0]=%X\n",_CmdBuf[0]);

    for (i = 0;i < 6;i++)
	printf("CTL[%d]=%X\n",i,_CtlBuf[i]);
    }
#endif

#ifdef DEBUG
    printf("STATUS:%X\n",io_RTPerrno);
#endif

    /* Return read pattern */
    return(_InBuf[0]);

} /* END rtpai_read */

/************************************************************************
*
* Name:	rtpao_write(Data,Device,Card,Channel,Type)	
*
* Type:	void				Read bitpattern
*
* TYPE		PARAMETER	IOGF	DESCRIPTION
* pwr_tUInt16	Data		I	Outvalue
* pwr_tUInt8		Device		I	Rack address (30-3F)
* pwr_tUInt8		Card		I	Card address (1-F)
* pwr_tUInt8		Word		I	Which word 
*					(if more than one is possible)
* pwr_tUInt32	Type		I	RTP Card type (see io_rtp.h)
*
* pwr_tBoolean	io_readerr	  G	Sätts TRUE om fel 
* pwr_tInt16		io_RTPerrno	  G	RTP felkod
*
* Description:	Write data to analog outputs.
*************************************************************************/

void	rtpao_write(pwr_tUInt16	Data,
		    pwr_tUInt8	Device,
		    pwr_tUInt8	Card,
		    pwr_tUInt8	Channel,
		    pwr_tUInt32	Type)
{

    /* Build "write AO" command in standard RTP I/O-mode */

    _CmdBuf[0] = CMD_RTP_LIST;
    _CtlBuf[0] = CTL_DAO;
    _CtlBuf[1] = Device;
    _CtlBuf[2] = 1;		    /* One command word */
    _CtlBuf[3] = 0x100 | Card;    /* Disable interupt, random access */
    _CtlBuf[4] = 0;

    switch (Type)
    {
	case RTP_AO_7455_20:
	    
	    Data &= 0xfff;	/* mask off the most significant nibble */
	    break;
 
	case RTP_AO_7455_30:

	    Data |= (Channel << 14);
	    break;

	default:
	    break;
    }

    _OutBuf[0] = Data;

    /* EXEC */
    io_RTPerrno = rtp$exec(_CmdBuf);
    if (io_RTPerrno != 0)
	io_writeerr = TRUE;
    else
	io_writeerr = FALSE;


#ifdef DEBUG
    printf("STATUS:%X\n",io_RTPerrno);
#endif

} /* END rtpao_write */

/************************************************************************
*
* Name:	rtpco_read(DataP,Device,Card,Word,Type)	
*
* Type:	void
*
* TYPE		PARAMETER	IOGF	DESCRIPTION
* pwr_tUInt16	*DataP		I	Read Data
* pwr_tUInt8		Device		I	Rack address (30-3F)
* pwr_tUInt8		Card		I	Card address (1-F)
* pwr_tUInt8		Word		I	Which word 
*					(if more than one is possible)
* pwr_tUInt32	Type		I	RTP Card type (see io_rtp.h)
*
* pwr_tBoolean	io_readerr	  G	Sätts TRUE om fel 
* pwr_tInt16		io_RTPerrno	  G	RTP felkod
*
* Description:	Read counter card
*************************************************************************/

void rtpco_read(pwr_tUInt16	*DataP,
		pwr_tUInt8	Device,
		pwr_tUInt8	Card,
		pwr_tUInt8	Word,
		pwr_tUInt32	Type)
{

    /* Build "read DI" command in standard RTP I/O-mode */
    _CmdBuf[0] = CMD_RTP_LIST;
    _CtlBuf[0] = CTL_DI;
    _CtlBuf[1] = Device;
    _CtlBuf[2] = 1;		    /* One command word */
    _CtlBuf[3] = 0x500 | Card;    /* Disable interupt, random access */
    _CtlBuf[4] = 0;

    /* EXEC */
    io_RTPerrno = rtp$exec(_CmdBuf);
    if (io_RTPerrno != 0)
	io_readerr = TRUE;
    else
	io_readerr = FALSE;

#ifdef DEBUG
    printf("STATUS:%X\n",io_RTPerrno);
#endif

    /* Return read pattern */
    *DataP = _InBuf[0];

} /* END rtpco_read */

/************************************************************************
*
* Name:	rtpco_write(Data,Device,Card,Type)	
*
* Type:	void				Read bitpattern
*
* TYPE		PARAMETER	IOGF	DESCRIPTION
* pwr_tUInt16	Data		I	Outvalue
* pwr_tUInt8		Device		I	Rack address (30-3F)
* pwr_tUInt8		Card		I	Card address (1-F)
*					(if more than one is possible)
* pwr_tUInt32	Type		I	RTP Card type (see io_rtp.h)
*
* pwr_tBoolean	io_readerr	  G	Sätts TRUE om fel 
* pwr_tInt16		io_RTPerrno	  G	RTP felkod
*
* Description:	Write data to counter cards.
*************************************************************************/

void	rtpco_write(pwr_tUInt16  Data,
		    pwr_tUInt8   Device,
		    pwr_tUInt8   Card,
		    pwr_tUInt32  Type)
{

    /* Build "write CO" command in standard RTP I/O-mode */
    _CmdBuf[0] = CMD_RTP_LIST;
    _CtlBuf[1] = Device;
    _CtlBuf[2] = 1;		    /* One command word */

    
    /* What we are going to do depends on what kind of a counter card we got */
    switch (Type) {

    case RTP_CO_7435_33:  /* This old card doesn't accept data written to it */

	_CtlBuf[0] = CTL_DI;
	if (Data == 0)
	    _CtlBuf[3] = 0x520 | Card;    /* Disable interupt, random access, CLR */
	else
	{
	    io_writeerr = TRUE;
	    return;
	}
	break; 

    case RTP_CO_7437_33:    /* This card selects one of four counters depending
			       on data written to it */

	_CtlBuf[0] = CTL_DAO;
	_CtlBuf[3] = Card;  /* Select Card */
	_OutBuf[0] = Data;
	break;

    default:
	break;    
    } /* End case */

    _CtlBuf[4] = 0;

    /* EXEC */
    io_RTPerrno = rtp$exec(_CmdBuf);
    if (io_RTPerrno != 0)
	io_writeerr = TRUE;
    else
	io_writeerr = FALSE;

#ifdef DEBUG
    printf("STATUS:%X\n",io_RTPerrno);
#endif


} /* END rtpco_write */
