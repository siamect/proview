/* 
 * Proview   $Id$
 * Copyright (C) 2008 SSAB Oxelösund AB.
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

/*
 * rt_io_m_pb_hilscher.c -- Proview agent for the Hilscher CIF 50-PB
 * Profibus DP board.
 */

/* Enable this define to spam the Proview log with diagnostic messages
from the slaves (this is probably a bad idea).  */
/* #define SLAVE_DIAG_VERBOSE  */

/* Enable this define to permit the agent to write to the board's flash in
order to remove the SyCon database.  */
#define FLASH_WRITE_ENABLE

/* A note on assertions: There are some asserts in the code, most of these
are only used during initialization to verify that we do not write outside
our buffers or provide the Hilscher API with messages that would be too long.

The asserts may be disabled completely by using the NDEBUG macro, defined
in assert.h (man assert(3)). I would, however, recommend against that.  */

/*
The code refers to some documents provided by Hilscher, the documents are:

	CIFLinux_en.pdf: found in cif2620.tbz, the Linux version of the Hilscher
			driver and API.

	dpm_pie.pdf: found on the system software disc that comes with the board.
*/

#define BOARD_INIT_ERROR -1

#pragma pack(1)

#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <assert.h>

#include "cif_user.h"
#include "rcs_user.h"
#include "dpm_user.h"

#include "rt_io_pb_locals.h"

#include "pwr.h"
#include "co_cdh.h"
#include "pwr_baseclasses.h"
#include "pwr_profibusclasses.h"
#include "rt_io_base.h"
#include "rt_io_bus.h"
#include "rt_io_msg.h"
#include "rt_errh.h"
#include "rt_io_agent_init.h"
#include "rt_pb_msg.h"

typedef struct {
	/* Board number.  */
	unsigned short dev_number;
	/* Number of occupied bytes in the input area of the dual port ram.  */
	unsigned short input_size;
	/* Number of occupied bytes in the output area of the dual port ram.  */
	unsigned short output_size;
	/* Message counter for communicating with the Profibus board,
	(pre-)incremented before sending each message, the value is then
	sent with the message.  This message counter is optional.  */
	unsigned char cif_msgcnt;
	/* Watchdog counter, used by DevTriggerWatchDog() as explained on page
	58 of CIFLinux_en.pdf.  */
	unsigned short watchdog;
} io_sAgentLocal;

/* Prototypes for functions exported to Proview.  */
static pwr_tStatus IoAgentInit(io_tCtx ctx, io_sAgent *ap);
static pwr_tStatus IoAgentRead(io_tCtx ctx, io_sAgent *ap);
static pwr_tStatus IoAgentWrite(io_tCtx ctx, io_sAgent *ap);
static pwr_tStatus IoAgentClose(io_tCtx ctx, io_sAgent *ap);

#ifdef FLASH_WRITE_ENABLE
/*** Structures for board flashing.  ***/

/* Start segment of the database, shall be 8 for
CIF30-DPM, CIF104-DPM, COM-DPM, COM-PB, CIF104-PB and CIF 50-PB.
Shall be 0 for CIF 30-PB and CIF 60-PB. */
typedef struct {
	/* Used to terminate the table, se to 1 for every entry but the
	last (dummy entry), which should be 0.  */
	unsigned char entry;
	/* The DEVINFO struct for this board.  */
	DEVINFO info;
	/* Start segment of the database for this board.  */
	unsigned char db_startsegment;
	/* A human readable name for this board.  Max 99 characters!  */
	unsigned char board_name[100];
} boards;

/* Table of known boards, consists of four fields according to the board type.
The last entry must be a null termination (entry == 0).
*/
boards known_boards[] = {
	{ 1, { 8, 50, 66, { 'C', 'I', 'F'} }, 8, "Hilscher CIF 50-PB" },
	{ 0 }
};
#endif /* FLASH_WRITE_ENABLE  */



/* Initializes Profibus DP interface on the CIF board referred to by local.
DRV_NO_ERROR is returned upon success.  */
static short
dpm_init_master(io_sAgentLocal *local, pwr_sClass_Pb_Hilscher *op,
		io_sAgent *ap)
{
	short rv;
	DPM_PLC_PARAMETER prm;

	/* Clears struct.  */
	memset(&prm, 0, sizeof(prm));

	/* Defines standard parameters.  */
	prm.bMode = DPM_SET_MODE_BUFFERED_HOST_CONTROLLED;
	prm.bCycleTime = 0;
	prm.bFormat = DPM_FORMAT_SWAP;
	/* Sets the timeout (in milliseconds) for the watchdog supervising the
	Profibus DP board.  After this amount of time has passed without
	any data exchange with the host, the board will be reset which will
	eventually make the watchdogs in the slaves time out (normally resulting
	in all outputs being cleared).  A zero value will disable the watchdog
	functionality altogether.  */
	prm.usWatchDogTime = op->StallTime;
	prm.bRedundancy = 0;
	prm.bSlStateMethod = DPM_SL_STATE_NOT_SYCHRONOUS;
	prm.bEnableBits.biIdentNumberActive = 0;
	prm.bEnableBits.biHighPriorHandshake = 0;
	/* 0x7505 in LE */
	*((unsigned char *) &(prm.usIdentNumber)) = 0x75;
	*(((unsigned char *) &(prm.usIdentNumber)) + 1) = 0x05;

	/* Writes parameters to the board local->dev_number, task 2, size is
	 * hardcoded to 16 as specified in dpm_pie.pdf, page 9.  */
	if ((rv = DevPutTaskParameter(local->dev_number, 2,
			16,  &prm)) != DRV_NO_ERROR) {
		errh_Error("Profibus DP Master %s - "
				" DevPutTaskParameter() failed with return code %d",
				ap->Name, rv);
		return rv;
	}

	/* Resets board to put new master parameters into effect. */
	if ((rv = DevReset(local->dev_number, WARMSTART, 8000)) != DRV_NO_ERROR) {
		errh_Error("Profibus DP Master %s - "
				" DevReset() failed with return code %d",
				ap->Name, rv);
		return rv;
	}

	return rv;
}

/* Opens and initializes the board referred to by local. Upon success
 * DRV_NO_ERROR will be returned.  */
static short
dpm_init(io_sAgentLocal *local, io_sAgent *ap)
{
	short rv;

	if ((rv = DevOpenDriver()) != DRV_NO_ERROR) {
		errh_Error("Profibus DP Master %s - "
				" DevOpenDriver() failed with return code %d",
				ap->Name, rv);
		return rv;
	} else if ((rv = DevInitBoard(local->dev_number)) != DRV_NO_ERROR) {
		errh_Error("Profibus DP Master %s - "
				" DevInitBoard() failed with return code %d",
				ap->Name, rv);
		return rv;
	}

	return DRV_NO_ERROR;
}

/* Closes the board referred to by local. Upon success DRV_NO_ERROR will
be returned.  */
static short
dpm_exit(io_sAgentLocal *local, io_sAgent *ap)
{
	short rv;
	short rv_ret = DRV_NO_ERROR;

	if ((rv = DevExitBoard(local->dev_number)) != DRV_NO_ERROR) {
		errh_Error("Profibus DP Master %s - "
				" DevExitBoard() failed with return code %d",
				ap->Name, rv);
		rv_ret = rv;
	}
	if ((rv = DevCloseDriver()) != DRV_NO_ERROR) {
		errh_Error("Profibus DP Master %s - "
				" DevCloseDriver() failed with return code %d",
				ap->Name, rv);
		rv_ret = rv;
	}

	return rv_ret;
}

/* Returns the number of bytes required to represent the inputs of card
(Profibus module) cp.  Based on the IoRackInit() method of
rt_io_m_pb_dp_slave.c.  */
static short
module_cnt_inputs(io_sCard *cp)
{
	short input_counter = 0;
	int latent_input_count = 0;

	pwr_tCid cid;

	io_sChannel *chanp;
	int i;
	pwr_tInt32 chan_size;
	pwr_sClass_ChanDi *chan_di;
	pwr_sClass_ChanAi *chan_ai;
	pwr_sClass_ChanAit *chan_ait;
	pwr_sClass_ChanIi *chan_ii;

	/* From v4.1.3 we can have subclasses, find the super class */
	for (cid = cp->Class; ODD(gdh_GetSuperClass(cid, &cid, cp->Objid));)
		;

	if (cid == pwr_cClass_Pb_Module) {

		/* New style configuring (from v4.1.3) with Pb_Module objects or
		subclass.  Loop all channels in the module and set channel size and
		offset. */	 
		for (i = 0; i < cp->ChanListSize; ++i) {
			chanp = &cp->chanlist[i];

			if (is_diag(&chanp->ChanAref)) {
				chanp->udata |= PB_UDATA_DIAG;
				if (chanp->ChanClass != pwr_cClass_ChanIi)
					errh_Error("Diagnostic channel class, card %s", cp->Name);
				continue;
			}

			if (chanp->ChanClass != pwr_cClass_ChanDi) {
				input_counter += latent_input_count;
				latent_input_count = 0;
			}

			switch (chanp->ChanClass) {
     
			case pwr_cClass_ChanDi:
				chan_di = (pwr_sClass_ChanDi *) chanp->cop;
				if (chan_di->Number == 0) {
					input_counter += latent_input_count;
					latent_input_count = 0;
				}
				if (chan_di->Number == 0)
					latent_input_count = GetChanSize(chan_di->Representation);
				break;

			case pwr_cClass_ChanAi:
				chan_ai = (pwr_sClass_ChanAi *) chanp->cop;
				chan_size = GetChanSize(chan_ai->Representation);
				input_counter += chan_size;
				break;
  
			case pwr_cClass_ChanAit:
				chan_ait = (pwr_sClass_ChanAit *) chanp->cop;
				chan_size = GetChanSize(chan_ait->Representation);
				input_counter += chan_size;
				break;
  
			case pwr_cClass_ChanIi:
				chan_ii = (pwr_sClass_ChanIi *) chanp->cop;
				chan_size = GetChanSize(chan_ii->Representation);
				input_counter += chan_size;
				break;
			}
		}
	}

	return (input_counter + latent_input_count);
}

/* Returns the number of bytes required to represent the outputs of card
(Profibus module) cp.  Based on the IoRackInit() method of
rt_io_m_pb_dp_slave.c.  */
static short
module_cnt_outputs(io_sCard *cp)
{
	short output_counter = 0;
	int latent_output_count = 0;

	pwr_tCid cid;

	io_sChannel *chanp;
	int i;
	pwr_tInt32 chan_size;
	pwr_sClass_ChanDo *chan_do;
	pwr_sClass_ChanAo *chan_ao;
	pwr_sClass_ChanIo *chan_io;

	/* From v4.1.3 we can have subclasses, find the super class */
	for (cid = cp->Class; ODD(gdh_GetSuperClass(cid, &cid, cp->Objid));)
		;

	if (cid == pwr_cClass_Pb_Module) {

		/* New style configuring (from v4.1.3) with Pb_Module objects or
		subclass.  Loop all channels in the module and set channel size and
		offset. */	 
		for (i=0; i<cp->ChanListSize; i++) {
			chanp = &cp->chanlist[i];

			if(is_diag(&chanp->ChanAref)) {
				chanp->udata |= PB_UDATA_DIAG;
				if (chanp->ChanClass != pwr_cClass_ChanIi)
					errh_Error("Diagnostic channel class, card %s", cp->Name);
				continue;
			}

			if (chanp->ChanClass != pwr_cClass_ChanDo) {
				output_counter += latent_output_count;
				latent_output_count = 0;
			}

			switch (chanp->ChanClass) {

			case pwr_cClass_ChanDo:
				chan_do = (pwr_sClass_ChanDo *) chanp->cop;
				if (chan_do->Number == 0) {
					output_counter += latent_output_count;
					latent_output_count = 0;
				}
				chan_size = GetChanSize(chan_do->Representation);
				if (chan_do->Number == 0)
					latent_output_count = GetChanSize(chan_do->Representation);
				break;

			case pwr_cClass_ChanAo:
				chan_ao = (pwr_sClass_ChanAo *) chanp->cop;
				chan_size = GetChanSize(chan_ao->Representation);
				output_counter += chan_size;
				break;

			case pwr_cClass_ChanIo:
				chan_io = (pwr_sClass_ChanIo *) chanp->cop;
				chan_size = GetChanSize(chan_io->Representation);
				output_counter += chan_size;
				break;
			}
		}
	}

	return (output_counter + latent_output_count);
}

/* Builds a struct for IO area allocation for the modules attached
to the slave op.  The starting offset is derived from the local struct of
ap.  The address table will be written to the buffer provided in add_tab.
add_tab will also have its length field updated.  Not more than add_tab_len
bytes will be written to add_tab.

The function will also set up the required fields in op so that Proview
may perform IO operations using the IoAgentRead() and IoAgentWrite() functions
in this module.  */
static void
dpm_set_add_tab(pwr_sClass_Pb_DP_Slave *op, io_sAgent *ap, io_sRack *rp,
		DPM_SL_PRM_ADD_TAB *add_tab, int add_tab_len)
{
	io_sCard *cardp;
	short input_counter;
	short output_counter;
	io_sRack *sl;

	/* A minimal address table consists of one word of length counter + two
	bytes of input and output counters.  */
	assert(add_tab_len >= 4);

	/* Handles for the number of bytes in the board's dual port memory that the
	slaves/modules have occupied so far.  */
	unsigned short *input_size;
	unsigned short *output_size;
	input_size = (unsigned short *) &((io_sAgentLocal *) ap->Local)->input_size;
	output_size =
			(unsigned short *) &((io_sAgentLocal *) ap->Local)->output_size;

	/* This lets Proview find the IO-area for this module (in this case it is
	actually handled at the agent level, thus in this file).  The offsets
	are known at this time, while the number of IO-bytes are calculated below,
	the latter still needs to be initialized to zero.  */
	op->OffsetInputs = *input_size;
	op->OffsetOutputs = *output_size;
	op->BytesOfInput = 0;
	op->BytesOfOutput = 0;

	/* Finds our rack (slave).  */
	for (sl = rp;
			sl != NULL && ((pwr_sClass_Pb_DP_Slave *) sl->op)->SlaveAddress
			!= op->SlaveAddress;
			sl = sl->next)
		;
	if (sl == NULL
			|| ((pwr_sClass_Pb_DP_Slave *) sl->op)->SlaveAddress
			!= op->SlaveAddress) {
		errh_Error("Profibus DP Master %s - "
				" Can't find rack for slave %d",
				ap->Name, op->SlaveAddress);
		return;
	}

	/* Iterates through the cards (modules in Profibus terminology) of
	the current rack, counting the length of the input area as we go.  */
	for (cardp = sl->cardlist; cardp; cardp = cardp->next) {

		/* Gets the size of the input area for this module.  */
		input_counter = module_cnt_inputs(cardp);

		/* Saves the length of this module's input area.  */
		op->BytesOfInput += input_counter;

		/* Only modules with and input or output area are required to
		have an entry in the add tab.  */
		if (input_counter > 0) {
			assert(add_tab->bInput_Count + 4 < add_tab_len);
			/* The MSB tells the Hilscher board whether to treat the address
			as a byte (set) or word (unset) offset
			(see dpm_pie.pdf, page 45).  */
			add_tab->ausEA_Offset[add_tab->bInput_Count++]
				= *input_size | EA_OFFSET_BYTE;
			*input_size += input_counter;
		}
	}

	/* Now process the output areas.  */
	for (cardp = sl->cardlist; cardp; cardp = cardp->next) {

		/* Gets the size of the output area for this module.  */
		output_counter = module_cnt_outputs(cardp);

		/* Saves the length of this module's input area.  */
		op->BytesOfOutput += output_counter;

		/* Only modules with and input or output area are required to
		have an entry in the add tab.  */
		if (output_counter > 0) {
			assert(add_tab->bInput_Count + add_tab->bOutput_Count + 4
					< add_tab_len);
			/* The MSB tells the Hilscher board whether to treat the address
			as a byte (set) or word (unset) offset
			(see dpm_pie.pdf, page 45).  */
			/* The input offsets precedes the output offsets.  */
			add_tab->ausEA_Offset[
					add_tab->bInput_Count + add_tab->bOutput_Count++]
					= *output_size | EA_OFFSET_BYTE;
			*output_size += output_counter;
		}
	}

	assert(add_tab->bInput_Count + add_tab->bOutput_Count <= MAX_EA_OFFSET_LEN);

	/* Updates the length of the add_tab struct.  */
	add_tab->usAdd_Tab_Len = sizeof(add_tab->usAdd_Tab_Len)
			+ sizeof(add_tab->bInput_Count) + sizeof(add_tab->bOutput_Count)
			+ 2 * add_tab->bInput_Count + 2 * add_tab->bOutput_Count;
}

/* Writes instructions for reflashing the board to the error log.  */
static void
flashing_disabled_warning(io_sAgent *ap)
{
	errh_Error("Profibus DP Master %s - DDLM Download: this board",
			ap->Name);
	errh_Error("appears to be set for offline configuration.  To make");
	errh_Error("the board usable in Proview, delete the configuration");
	errh_Error("database from the board's flash.  This can be done");
	errh_Error("with the tool cif50_rmdb, alternatively the profibus");
	errh_Error("agent may be recompiled with flashing support.");
}

/* Produces human readable log entries based on the f-flag (error state) of the
message passed in msg.  */
static void
dpm_ddlm_answer_msg_print_error(RCS_MESSAGE *msg, io_sAgent *ap)
{
	char *s;

	if (msg == NULL)
		return;

	switch (msg->a) {

	case DDLM_Download:
		switch (msg->f) {
		case 0:
			return;
		case CON_NO:
			s = "master not in offline configuration mode. Delete"
					" the data base within the DEVICE, else the DEVICE is not"
					" able to execute the download of the dataset file";
			break;
		case CON_IV:
			s = "parameter fault in request. Please check the"
					" downloaded parameter in your request message. The"
					" Siemens master chip has denied the bus parameter data"
					" set.";
			break;
		case CON_TO:
			s = "timeout. If Start and End_Seq messaging is used a"
					" message timeout occurred during the sequenced download.";
			break;
		case CON_NI:
			s = "area_code unknown. Please check the area code"
					" of the message in its limits.";
			break;
		case CON_EA:
			s = "overstep of the maximum buffer length of 1000"
					" bytes per dataset file.";
			break;
		case CON_IP:
			s = "faulty parameter detected. The configured"
					" data_control time is set to zero. This is not allowed.";
			break;
		case CON_SE:
			s = "sequence error. End_seq was called without"
					" Start_seq or the area_code is different in the download"
					" message in comparison to the Start_seq area_code";
			break;
		case CON_DI:
			s = "data incomplete or faulty. The check routine for the"
					" downloaded parameter file detected inconsistencies";
			break;
		default:
			return;
		}
		errh_Info("Profibus DP Master %s - DDLM Download: %s", ap->Name, s);
		/* Provides extended instructions if the board's flash is configured for
		offline configuration.  */
		if (msg->f == CON_NO)
			flashing_disabled_warning(ap);
		break;

	case DDLM_Slave_Diag:
		switch (msg->f) {
		case 0:
			return;
		case CON_NA:
			s = "no response of the station";
			break;
		case CON_DS:
			s = "master not into the logical network in token ring";
			break;
		case TASK_F_DEVICE_ADR:
			s = "Remote Address in request service out of range";
			break;
		default:
			return;
		}
		errh_Info("Profibus DP Master %s - Station %d - DDLM Slave Diag: %s",
				ap->Name, ((RCS_MESSAGETELEGRAM_10 *) msg)->device_adr, s);
		break;

	default:
		return;

	}
}

/* Performs a DDLM download, used to configure the master or
slave bus parameters.  The agent's local struct is passed in local, the
destination address is passed in address, 0-125 to set slave bus parameters
or 127 to set master bus parameters.  The bus parameters are read from
the buffer prmdata and shall be of length prmlen.  DRV_NO_ERROR is returned
if successful.  */
static short
dpm_ddlm_download(io_sAgentLocal *local, unsigned char address,
		unsigned int prmlen, void *prmdata, io_sAgent *ap)
{
	short rv;
	int got_response = 0;
	RCS_MESSAGE msg, rcv_msg;
	DDLM_DOWNLOAD_REQUEST *download = (DDLM_DOWNLOAD_REQUEST *) &(msg.d[0]);

	/* Clears struct.  */
	memset(&msg, 0, sizeof(msg));
	memset(&rcv_msg, 0, sizeof(rcv_msg));

	/* Defines message header.  */
	msg.rx = 3;
	msg.tx = 16;
	msg.ln = prmlen + 4;
	msg.nr = ++local->cif_msgcnt;
	msg.a = 0;
	msg.f = 0;
	msg.b = DDLM_Download;
	msg.e = 0;

	/* Defines service header.  */
	download->bReq_Add = 0;
	download->bArea_Code = address;
	download->usAdd_Offset = 0;

	/* PRM data starts at d[4] in the message according to
	dpm_pie.pdf, page 43.  */
	assert(prmlen <= 240);
	memcpy(&(msg.d[4]), prmdata, prmlen);

	if ((rv = DevPutMessage(local->dev_number, (MSG_STRUC *) &msg, 5000))
			!= DRV_NO_ERROR) {
		errh_Error("Profibus DP Master %s - DDLM_Download - DevPutMessage()"
				" returned %d", ap->Name, rv);
		return rv;
	}

	/* Checks confirmation message.  */
	while (!got_response) {
		if ((rv = DevGetMessage(local->dev_number, sizeof(rcv_msg),
				(MSG_STRUC *) &rcv_msg, 5000)) != DRV_NO_ERROR) {
			errh_Error("Profibus DP Master %s - DDLM_Download - DevGetMessage()"
					" returned %d", ap->Name, rv);
			return rv;
		} else if (rcv_msg.nr != local->cif_msgcnt
				|| rcv_msg.rx != 16
				|| rcv_msg.tx != 3
				|| rcv_msg.a != DDLM_Download) {
			/* Discards all messages prior to our response.  */
			memset(&rcv_msg, 0, sizeof(rcv_msg));
			continue;
		} else if (rcv_msg.f != 0) {
			dpm_ddlm_answer_msg_print_error(&rcv_msg, ap);
			return rcv_msg.f;
		} else {
			got_response = 1;
		}
	}

	return rv;
}

/* Configures the master bus parameters of the board referred to by
local, the struct containing the parameters should be passed in op.  */
static short
dpm_download_master_prm(io_sAgentLocal *local, pwr_sClass_Pb_Hilscher *op,
		io_sAgent *ap)
{
	DPM_BUS_DP prm;

	/* Clears struct.  */
	memset(&prm, 0, sizeof(prm));

	/* Defines Profibus DP bus parameters, these are described briefly
	in dpm_pie.pdf, page 41.  */
	prm.usBus_Para_Len = 32;
	/* The master's address is hardcoded to 0.  */
	prm.bFDL_Add = 0;
	prm.bBaudrate = ((op->BaudRate == 500) ? (DP_BAUD_500)
			: (op->BaudRate == 1500) ? (DP_BAUD_1500)
			: (op->BaudRate == 3000) ? (DP_BAUD_3000)
			: (op->BaudRate == 6000) ? (DP_BAUD_6000)
			: (op->BaudRate == 12000) ? (DP_BAUD_12000)
			: (DP_BAUD_1500));
	prm.usTSL = op->Tsl;
	prm.usMin_TSDR = op->MinTsdr;
	prm.usMax_TSDR = op->MaxTsdr;
	prm.bTQUI = op->Tqui;
	prm.bTSET = op->Tset;
	prm.ulTTR = op->Ttr;
	prm.bG = op->G;
	prm.bHSA = op->Hsa;
	prm.bMax_Retry_Limit = op->MaxRetryLimit;
	prm.Bp_Flag.bError_Action_Flag = ((op->BpFlag) ? (1) : (0));
	prm.usMin_Slave_Intervall = op->MinSlaveInterval;
	prm.usPoll_Timeout = op->PollTimeout;
	prm.usData_Control_Time = op->DataControlTime;
	prm.bAlarm_Max = 0;
	prm.bMax_User_Global_Control = 1;

	/* The length of the total message is 35 according to dpm_pie.pdf,
	page 43. The service header accounts for 4 of these, thus we set
	the length to 31 here.  */
	return dpm_ddlm_download(local, 127, 31, &prm, ap);
}

/* Configures the bus parameters for one slave.  The agent's local struct
is passed in local.  op holds the slave to configure, the corresponding
agent pointer and rack pointer must be passed in ap and rp respectively.
DRV_NO_ERROR will be returned upon success.  */
static short
dpm_download_slave_prm(io_sAgentLocal *local, pwr_sClass_Pb_DP_Slave *op,
		io_sAgent *ap, io_sRack *rp)
{
	unsigned char buf[DPM_MAX_LEN_DATA_UNIT];
	unsigned int bufcnt = 0;

	/* DPM_SL_PRM_HEADER starts at offset 4 in the data buffer according
	to dpm_pie.pdf, page 45 -- however, since the first 4 bytes are
	occupied by the service header, which we'll fill in later, we'll
	define the offset to be 0. */
	DPM_SL_PRM_HEADER *hdr = (DPM_SL_PRM_HEADER *) &buf[0];
	DPM_SL_PRM_PRM_DATA *prm = (DPM_SL_PRM_PRM_DATA *) &buf[16];
	DPM_SL_PRM_CFG_DATA *cfg;
	DPM_SL_PRM_ADD_TAB *add_tab;
	DPM_SL_PRM_USR_DATA *usr;

	/* Clears buffer.  */
	memset(&buf, 0, sizeof(buf));

	/* Defines prm header.  */
	hdr->Sl_Flag.biExtra_Alarm_SAP = 0;
	hdr->Sl_Flag.biDPV1_Data_Types = 0;
	hdr->Sl_Flag.biDPV1_Supported = 0;
	hdr->Sl_Flag.biPublisher_Enable = 0;
	hdr->Sl_Flag.biFail_Safe = 0;
	hdr->Sl_Flag.biNew_Prm = 0;
	hdr->Sl_Flag.biActive = 1;
	hdr->bSlave_Typ = 0;

	bufcnt = 16;

	/* Defines prm data */
	prm->usPrmDataLen = 7 + sizeof(prm->usPrmDataLen);
	prm->Station_Status.bWD_On = 1;
	prm->Station_Status.bFreeze_Req = 0;
	prm->Station_Status.bSync_Req = 0;
	prm->Station_Status.bUnLock_Req = 0;
	prm->Station_Status.bLock_Req = 1;
	prm->bWD_Fact_1 = op->WdFact1;
	prm->bWD_Fact_2 = op->WdFact2;
	prm->bMin_Tsdr = 0;
	/* Ident number in LE */
	prm->usIdent_Number = swap16(op->PNOIdent);
	prm->bGroup_Ident = op->GroupIdent;

	bufcnt += prm->usPrmDataLen;

	/* Defines user prm data.  */
	prm->usPrmDataLen += op->PrmUserDataLen;
	assert(bufcnt + op->PrmUserDataLen + 2 <= DPM_MAX_LEN_DATA_UNIT);
	memcpy(&buf[bufcnt], op->PrmUserData, op->PrmUserDataLen);
	bufcnt += op->PrmUserDataLen;

	/* Defines cfg data */
	cfg = (DPM_SL_PRM_CFG_DATA *) &buf[bufcnt];
	/* Note that the cfg data length is part of the cfg data in Proview,
	by skipping these two bytes and writing the op->ConfigDataLen
	field to the usCfg_Data_Len of DPM_SL_PRM_CFG_DATA we avoid some
	endianness issues.  */
	cfg->usCfg_Data_Len = op->ConfigDataLen - 2 + sizeof(cfg->usCfg_Data_Len);
	assert(bufcnt + cfg->usCfg_Data_Len + 2 <= DPM_MAX_LEN_DATA_UNIT);
	memcpy(cfg->abCfg_Data, op->ConfigData + 2, op->ConfigDataLen - 2);

	bufcnt += cfg->usCfg_Data_Len;

	/* Defines address table.  */
	add_tab = (DPM_SL_PRM_ADD_TAB *) &buf[bufcnt];
	/* dpm_set_add_tab is also responsible for updating the length of the
	table, so no need to worry about it here :).  */
	dpm_set_add_tab(op, ap, rp, add_tab, DPM_MAX_LEN_DATA_UNIT - bufcnt);
	/* Take the length of the address table into account when filling our
	local buffer.  */
	bufcnt += add_tab->usAdd_Tab_Len;

	assert(bufcnt + 2 <= DPM_MAX_LEN_DATA_UNIT);

	/* We will leave the slave user data empty.  */
	usr = (DPM_SL_PRM_USR_DATA *) &buf[bufcnt];
	usr->usSlave_Usr_Data_Len = sizeof(usr->usSlave_Usr_Data_Len);

	bufcnt += usr->usSlave_Usr_Data_Len;

	/* The length of the whole PRM set is now known, so we can
	fill in that field in the header.  */
	hdr->usSlaveParaLen = bufcnt;

	assert(bufcnt <= DPM_MAX_LEN_DATA_UNIT);

	return dpm_ddlm_download(local, op->SlaveAddress, bufcnt, buf, ap);
}

/* Requests diagnostics from the slave address, attached to the
board referred to by local.  The function will return DRV_NO_ERROR if
the request was sent without error, however, it tells us nothing
about whether any diagnostics data was sent back.  */
static short
dpm_req_slave_diag(io_sAgentLocal *local, unsigned char address, io_sAgent *ap)
{
	short rv;
	RCS_MESSAGETELEGRAM_10 msg;

	/* Clears struct.  */
	memset(&msg, 0, sizeof(msg));

	/* Defines message header.  */
	msg.rx = 3;
	msg.tx = 16;
	msg.ln = 8;
	msg.nr = ++local->cif_msgcnt;
	msg.a = 0;
	msg.f = 0;
	msg.b = DDLM_Slave_Diag;
	msg.e = 0;

	/* Defines extended message header.  */
	msg.device_adr = address;
	msg.data_area = 0;
	msg.data_adr = 0;
	msg.data_idx = 0;
	msg.data_cnt = 0;
	msg.data_type = TASK_TDT_STRING;
	msg.function = TASK_TFC_READ;

	/* Requests diagnostic data from slave.  */
	if ((rv = DevPutMessage(local->dev_number, (MSG_STRUC *) &msg, 0))
			!= DRV_NO_ERROR) {
		errh_Info("Profibus DP Master %s - DDLM_Slave_Diag - DevPutMessage()"
				" returned %d", ap->Name, rv);
		return rv;
	}

	return DRV_NO_ERROR;
}

/* Checks if we have any pending messages and processes them.  This allows us
to gather diagnostics data asynchronously.  The desired agent's local
struct is passed via local and the associated rack list (Profibus slaves) is
passed in slave_list.  */
static void
dpm_update_slave_diag(io_sAgentLocal *local, io_sRack *slave_list,
		io_sAgent *ap)
{
	short rv;
	RCS_MESSAGETELEGRAM_10 rcv_msg;
	pwr_sClass_Pb_DP_Slave *sp;
	io_sRack *sl;

	/* Clears struct.  */
	memset(&rcv_msg, 0, sizeof(rcv_msg));

	/* Gets pending diagnostic messages, discards all other messages.  */
	while ((rv = DevGetMessage(local->dev_number, sizeof(rcv_msg),
				(MSG_STRUC *) &rcv_msg, 0)) == DRV_NO_ERROR) {
		if (rcv_msg.rx != 16 || rcv_msg.tx != 3
				|| rcv_msg.a != DDLM_Slave_Diag) {
			/* Discards received message.  */
			memset(&rcv_msg, 0, sizeof(rcv_msg));
			continue;
		} else if (rcv_msg.f != 0) {
			dpm_ddlm_answer_msg_print_error((RCS_MESSAGE *) &rcv_msg, ap);
			/* Discards received message.  */
			memset(&rcv_msg, 0, sizeof(rcv_msg));
			continue;
		} else {

			/*** Updates Proview's structs with the acquired diagnostics. ***/

			/* Finds our slave.  */
			for (sl = slave_list; sl != NULL; sl = sl->next) {
				sp = (pwr_sClass_Pb_DP_Slave *) sl->op;
				if (sp->SlaveAddress == rcv_msg.device_adr)
					break;
			}
			/* The slave from which we were to read diagnostic data could not
			be found -- abort.  */
			if (sp->SlaveAddress != rcv_msg.device_adr) {
				errh_Error("Profibus DP Master %s - Received diagnostic message"
						" from unknown slave %d", ap->Name, rcv_msg.device_adr);
				return;
			}

			/* Copies the diagnostic data into slave struct.  */
			sp->StationStatus1 = rcv_msg.d[0];
			sp->StationStatus2 = rcv_msg.d[1];
			sp->StationStatus3 = rcv_msg.d[2];
			sp->BytesOfDiag = rcv_msg.data_cnt - 6;
			/* Makes sure we don't overflow the slave's diagnostics buffer.  */
			if (sp->BytesOfDiag > sizeof(sp->Diag))
				sp->BytesOfDiag = sizeof(sp->Diag);
			memcpy(sp->Diag, rcv_msg.d + 6, sp->BytesOfDiag);
	
			/* Updates the slave's status */
			
			if (!(sp->StationStatus1 & ~pwr_mPbStationStatus1Mask_ExternalDiag)
					&& !(sp->StationStatus2
							& ~(pwr_mPbStationStatus2Mask_Default |
					pwr_mPbStationStatus2Mask_ResponseMonitoringOn))) {
				sp->Status = PB__NORMAL;
			} else if (sp->StationStatus1
					& pwr_mPbStationStatus1Mask_NonExistent) {
				sp->Status = PB__NOCONN;
			} else if ((sp->StationStatus1
					& (pwr_mPbStationStatus1Mask_ConfigFault
							| pwr_mPbStationStatus1Mask_ParamFault))
					|| (sp->StationStatus2
							& pwr_mPbStationStatus2Mask_NewParamsRequested)) {
				sp->Status = PB__CONFIGERR;
			} else if (sp->StationStatus1
					& pwr_mPbStationStatus1Mask_MasterLock) {
				sp->Status = PB__MASTERLOCK;
			} else { /* if (sp->StationStatus1
					& pwr_mPbStationStatus1Mask_NotReady) */
				sp->Status = PB__NOTREADY;
			}

		}
	}
}

/* Prints diagnostics info diag in human readable form if the macro
SLAVE_DIAG_VERBOSE is defined.  */
static void
dpm_print_diag(io_sAgent *ap, DPM_DIAGNOSTICS *diag)
{
	char *s;

	if (diag->tError.bErr_Event == 0) {
		return;
	} else if (diag->tError.bErr_Rem_Adr == 255) {
		switch (diag->tError.bErr_Event) {
		case TASK_F_NO_USR_TASK: s = "usr task not found"; break;
		case TASK_F_NO_GLOBAL_DATA: s = "no global entry in data base"; break;
		case TASK_F_NO_FDL: s = "fdl task not found"; break;
		case TASK_F_NO_PLC: s = "plc task not found"; break;
		case TASK_F_NO_BUS_DP: s = "no bus parameters"; break;
		case TASK_F_INVALID_BUS_DP: s = "bus parameters faulty"; break;
		case TASK_F_NO_SL_TAB: s = "no slave data sets"; break;
		case TASK_F_INVALID_SL_TAB: s = "slave data set faulty"; break;
		case TASK_F_REM_ADR_DOUBLE: s = "double station addr detected"; break;
		case TASK_F_A_OFFSET_MAX: s = "maximum output offset reached"; break;
		case TASK_F_E_OFFSET_MAX: s = "maximum input offset reached"; break;
		case TASK_F_A_OVERLAP: s = "overlap in output area"; break;
		case TASK_F_E_OVERLAP: s = "overlap in input area"; break;
		case TASK_F_DPM_UNKNOWN_MODE: s = "warmstart with unknown mode"; break;
		case TASK_F_RAM_OVERRUN: s = "extended ram exceeded"; break;
		case TASK_F_SL_PRM_FAULT: s = "slave data set faulty"; break;
		case TASK_F_WATCHDOG: s = "user watchdog failure"; break;
		case TASK_F_NO_DATA_ACK: s = "mode0 no acknowledge"; break;
		case TASK_F_AUTO_CLEAR: s = "auto clear activated"; break;
		case TASK_F_FATAL_ASIC_ERROR: s = "fatal error occurred"; break;
		case TASK_F_UNKNOWN_EVENT: s = "unknown event detected"; break;
		case TASK_F_SEGMENT_CNT: s = "no segments to communicate"; break;
		default: return;
		}

		errh_Info("Profibus DP Master %s - %s", ap->Name, s);
		return;
	} else {
		switch (diag->tError.bErr_Event) {
		case CON_NA: s = "no reaction of the remote station"; break;
		case CON_DS: s = "local FDL not in logical token ring"; break;
		case CON_RS: s = "master request not supported by the slave"; break;
		case CON_NR: s = "no response data"; break;
		default: return;
		}

#ifdef SLAVE_DIAG_VERBOSE
		errh_Info("Profibus DP Master %s - Station %d - %s",
				ap->Name, diag->tError.bErr_Rem_Adr, s);
#endif /* SLAVE_DIAG_VERBOSE  */
		return;
	}
}

#ifdef FLASH_WRITE_ENABLE
/*** Functions for board flashing.  ***/

/* Tries to find the db_startsegment for the board referred to by local.
If the board was found in the database, the function returns DRV_NO_ERROR
and the startsegment will be written to what db_startsegment points to.  */
static short
dpm_check_board_type(io_sAgentLocal *local, io_sAgent *ap,
		unsigned char *db_startsegment)
{
	short rv;
	DEVINFO info;
	int i;

	assert(DRV_NO_ERROR == 0);

	/* Gets device information from the board.  */
	if ((rv = DevGetInfo(local->dev_number, GET_DEV_INFO, sizeof(info), &info))
			!= DRV_NO_ERROR) {
		errh_Error(
				"Profibus DP Master %s - Error %d while checking board type.",
				ap->Name, rv);
		return rv;
	}

	errh_Info("Profibus DP Master %s -"
			" Selected board %d has the following device info:",
			ap->Name, local->dev_number);
	errh_Info("Device DPM size: %d", info.bDpmSize);
	errh_Info("Device type: %d", info.bDevType);
	errh_Info("Device model: %d", info.bDevModel);
	errh_Info("Device identification: %.3s", info.abDevIdentifier);

	/* Looks for an entry matching our board.  */
	for (i = 0; known_boards[i].entry != 0; ++i)
		if (memcmp(&known_boards[i].info, &info, sizeof(info)) == 0)
			break;

	if (known_boards[i].entry == 0) {
		/* Board was not found in table.  */
		errh_Error("Profibus DP Master %s - Unknown board.", ap->Name);
		errh_Error("To add support for this board, look up its database start");
		errh_Error("segment in DPM_PIE.PDF, page 33 and add an entry to the");
		errh_Error("known_boards table in rt_io_m_pb_hilscher.c.");
		errh_Error("The following entry may be inserted into the known_boards");
		errh_Error("table to enable flashing for this board.  Replace");
		errh_Error("<db start segment> and <board name> with data found");
		errh_Error("in the above mentioned documentation.");
		errh_Error("{ 1, { %d, %d, %d, { '%c', '%c', '%c'} }, "
				"<db start segment>, \"<board name>\" },",
				info.bDpmSize, info.bDevType, info.bDevModel,
				info.abDevIdentifier[0],
				info.abDevIdentifier[1],
				info.abDevIdentifier[2]);
		return BOARD_INIT_ERROR;
	}

	/* Board was found in table.  */
	errh_Info("Board known:");
	errh_Info("Name: %s", known_boards[i].board_name);
	errh_Info("Start segment of database: %d",
			known_boards[i].db_startsegment);

	*db_startsegment = known_boards[i].db_startsegment;

	return DRV_NO_ERROR;
}

/* Deletes the "PROFIBUS" protocol settings database from the card referred
to by local, this procedure is described on page 32 -- 33 in dpm_pie.pdf.  */
static short
dpm_delete_flash_prmdb(io_sAgentLocal *local, io_sAgent *ap)
{
	unsigned char db_startsegment;
	int s = 3;
	int got_response = 0;
	short rv;
	RCS_MESSAGE msg;

	/* Gets start segment of database.  */
	if ((rv = dpm_check_board_type(local, ap, &db_startsegment))
			!= DRV_NO_ERROR)
		return rv;

	/* Clears struct.  */
	memset(&msg, 0, sizeof(msg));

	/* Defines message header.  */
	msg.rx = 0; /* receiver = RCS-Task */
	msg.tx = 16; /* transmitter = user at HOST */
	msg.ln = 2;
	msg.nr = ++local->cif_msgcnt;
	msg.a = 0; /* no answer */
	msg.f = 0; /* no error */
	msg.b = 6; /* command = data base access */
	msg.e = 0; /* extension, not used */

	/* Defines delete database request.  */
	msg.d[0] = 4; /* mode = delete data base */
	msg.d[1] = db_startsegment;

	errh_Info("Profibus DP Master %s - Sending delete database request...",
			ap->Name);
	/* Sends delete database request.  */
	if ((rv = DevPutMessage(local->dev_number, (MSG_STRUC *) &msg, 5000))
			!= DRV_NO_ERROR) {
		errh_Error("Profibus DP Master %s -"
				" DevPutMessage failed with return code %d", ap->Name, rv);
		return rv;
	}

	/* Waits for controller to update flash.  */
	errh_Info("Profibus DP Master %s -"
			" Delete db request sent. Waiting %d seconds...", ap->Name, s);
	while ((s = sleep(s)))
			;

	/* Clears struct.  */
	memset(&msg, 0, sizeof(msg));

	/* Gets response message.  */
	while (!got_response) {
		if ((rv = DevGetMessage(local->dev_number, sizeof(msg),
				(MSG_STRUC *) &msg, 10000)) != DRV_NO_ERROR) {
			errh_Error("Profibus DP Master %s -"
					" DevGetMessage failed with return code %d\n",
					ap->Name, rv);
			return rv;
		} else if (msg.nr != local->cif_msgcnt || msg.rx != 16 || msg.tx != 0
						|| msg.a != 6) {
			/* Discards all messages prior to our response.  */
			memset(&msg, 0, sizeof(msg));
			continue;
		} else {
			if (msg.f != 0)
				errh_Error("Profibus DP Master %s -"
						" Delete db confirmation message f-flag set to %d",
						ap->Name, msg.f);
			else
				errh_Info("Profibus DP Master %s -"
						" Delete database confirmation message received.",
						ap->Name);
			got_response = 1;
		}
	}

	return rv;
}

#endif /* FLASH_WRITE_ENABLE  */

/* From cif_dev.h in the linux driver package.  */
#define RUN_FLAG 0x40
#define READY_FLAG 0x80

/* Wrapper for dpm_init_master(), takes care of checking for (and optionally
removing) the sycon database if present.  */
static short
dpm_init_master_check_sycon_db(io_sAgentLocal *local,
		pwr_sClass_Pb_Hilscher *op, io_sAgent *ap)
{
	short rv;
	DRIVERINFO di;

	assert(DRV_NO_ERROR == 0);

	/* Initializes DP Master.  */
	if ((rv = dpm_init_master(local, op, ap)) != DRV_NO_ERROR) {
		return rv;
	} else if ((rv = DevGetInfo(local->dev_number, GET_DRIVER_INFO,
			sizeof(di), &di)) != DRV_NO_ERROR) {
		errh_Error("Profibus DP Master %s -"
				" DevGetInfo failed with return code %d", ap->Name, rv);
		return rv;
	} else if ((di.bHostFlags & (READY_FLAG | RUN_FLAG))
			== (READY_FLAG | RUN_FLAG)) {
		/* RUN and RDY bits set */
		errh_Info("Profibus DP Master %s - Hostflags: 0x%X",
				ap->Name, di.bHostFlags);
		errh_Info("Device is configured by SyCon.");
#ifdef FLASH_WRITE_ENABLE
		errh_Info("Deleting SyCon database from board's flash.");
		if ((rv = dpm_delete_flash_prmdb(local, ap)) != DRV_NO_ERROR) {
			return rv;
		} else {
			/* Reinitializes DP Master.  */
			rv = dpm_init_master(local, op, ap);
		}
#else /* FLASH_WRITE_ENABLE  */
		errh_Info("Flash writing is not enabled, ");
		flashing_disabled_warning(ap);
		return BOARD_INIT_ERROR;
#endif /* FLASH_WRITE_ENABLE  */
	}

	return rv;
}



/*----------------------------------------------------------------------------*\
   Init method for the Pb_Hilscher agent  
\*----------------------------------------------------------------------------*/
static pwr_tStatus
IoAgentInit(io_tCtx ctx, io_sAgent *ap)
{
	pwr_sClass_Pb_Hilscher *op;
	pwr_tStatus status;
	io_sAgentLocal *local;
	char ok;

	pwr_tObjid slave_objid;
	pwr_tClassId slave_class;

	pwr_sClass_Pb_DP_Slave *sop;
	char name[196];

	struct timespec rqtp = {0, 20000000}; /* 20 ms */
  
	int retry;

	/* Allocates area for local data structure */
	ap->Local = calloc(1, sizeof(io_sAgentLocal));
	if (!ap->Local) {
		errh_Error("ERROR config Profibus DP Master %s - %s",
				ap->Name, "calloc");
		return IO__ERRINIDEVICE;
	}

	/* Handles for easy access to local variables.  */
	local = (io_sAgentLocal *) ap->Local;
	op = (pwr_sClass_Pb_Hilscher *) ap->op;

	op->Status = PB__NOTINIT;

	/* Initializes interface.  */
	if (ctx->Node->Restarts > 0) {
		nanosleep(&rqtp, NULL);
	}

	errh_Info("Initializing interface for Profibus DP Master %s", ap->Name);

	/* Initializes local struct.  */
	local->dev_number = op->BusNumber - 1;
	local->input_size = 0;
	local->output_size = 0;
	local->cif_msgcnt = 0;
	local->watchdog = 0;

	/* Initializes Profibus driver API.  */
	if (dpm_init(local, ap) != DRV_NO_ERROR) {
		/* Cannot open driver */
		op->Status = PB__INITFAIL;
		errh_Error("ERROR config Profibus DP Master %s - %s",
				ap->Name, "open device");
		ctx->Node->EmergBreakTrue = 1;
		return IO__ERRDEVICE;
	}

	/* If this is not the Profibus I/O process, return */
	if ((op->Process & io_mProcess_Profibus)
			&& (ctx->Process != io_mProcess_Profibus)) {
		op->Status = PB__NOTINIT;
		errh_Info("Init template I/O agent for Profibus DP Master %s, %d",
				ap->Name, ctx->Process );
		return IO__SUCCESS;
	}
  
	if (ctx->Node->Restarts > 0) {
		errh_Info("Warm restart - Skipping config of Profibus DP Master %s",
				ap->Name);
		op->Status = PB__NORMAL;
		return IO__SUCCESS;
	}

	errh_Info("Config of Profibus DP Master %s", ap->Name);

	if (op->DisableBus != 1) {
		ok = FALSE;
		if (ctx->Node->Restarts == 0) {
			retry = 0;
			while (!ok) {
				op->Status = PB__NOTINIT;
      
				/* Sets DP master parameters and checks for sycon database.  */
				if (dpm_init_master_check_sycon_db(local, op, ap)
						!= DRV_NO_ERROR) {
					op->Status = PB__INITFAIL;
					errh_Error("ERROR config Profibus DP Master %s - %s",
							ap->Name, "dp init master");
					return IO__ERRINIDEVICE;
				}

				/* Loops through all slaves (traverse agent's children)
				and initializes them.  */
				op->NumberSlaves = 0;
				status = gdh_GetChild(ap->Objid, &slave_objid);
				while (ODD(status)) {
					status = gdh_GetObjectClass(slave_objid, &slave_class);

					status = gdh_ObjidToPointer(slave_objid,
							(pwr_tAddress *) &sop);
					status = gdh_ObjidToName(slave_objid, (char *) &name,
							sizeof(name), cdh_mNName);

					errh_Info("Download Profibus DP Slave config - %s", name);

					/* Calculates IO offsets and configures the slave.  */
					if (dpm_download_slave_prm(local, sop, ap, ap->racklist)
							!= DRV_NO_ERROR) {
						errh_Error("ERROR Init Profibus DP slave %s", name);
					}
					errh_Info("Profibus DP slave %d: in offs %d, input size %d,"
							" out offs %d, out size %d",
							sop->SlaveAddress,
							sop->OffsetInputs, sop->BytesOfInput,
							sop->OffsetOutputs, sop->BytesOfOutput);
					op->NumberSlaves++;
					status = gdh_GetNextSibling(slave_objid, &slave_objid);
				}

				/* Downloads the DP bus parameters -- this initiates the
				cyclic data exchange.  */
				if (dpm_download_master_prm(local, op, ap) != DRV_NO_ERROR) {
					op->Status = PB__INITFAIL;
					errh_Error("ERROR config Profibus DP Master %s - %s",
							ap->Name, "dp download bus");
					return IO__ERRINIDEVICE;
				}

				ok = TRUE;
	
			}  /* End - While !ok */
		}  /* End - Initialization only if not restart   */
	} else
		op->Status = PB__DISABLED;

	return IO__SUCCESS;
}


/*----------------------------------------------------------------------------*\
   Read method for the Pb_Hilscher agent  
\*----------------------------------------------------------------------------*/
static pwr_tStatus
IoAgentRead(io_tCtx ctx, io_sAgent *ap)
{
	pwr_sClass_Pb_Hilscher *mp;
	pwr_sClass_Pb_DP_Slave *sp;
	io_sAgentLocal *local;
	io_sRack *slave_list;

	pwr_sClass_Pb_Hilscher *op;
	unsigned short rv;

	int i;
	DPM_DIAGNOSTICS diag;

	/* Handle for local data structure.  */
	local = (io_sAgentLocal *) ap->Local;

	/*** Data exchange code goes here:  ***/

	/* Iterates over the slaves on the bus and reads process data from their
	respective addresses.  This is really the responsibility of the rack level,
	however, keeping this code here enables us to confine all the Profibus
	board specific code at the agent level.  */
	for (slave_list = ap->racklist; slave_list != NULL;
			slave_list = slave_list->next) {

		sp = (pwr_sClass_Pb_DP_Slave *) slave_list->op;
		mp = (pwr_sClass_Pb_Hilscher *) ap->op;

		if (sp->Status == PB__NORMAL && mp->Status == PB__NORMAL
				&& sp->DisableSlave != 1 && mp->DisableBus != 1) {

			/* Triggers the board's watchdog.  */
			DevTriggerWatchDog(local->dev_number, WATCHDOG_START,
					&local->watchdog);
			/* Reads process image from the slave.  */
			rv = DevExchangeIO(local->dev_number,
					0, 0, NULL,
					sp->OffsetInputs, sp->BytesOfInput, sp->Inputs,
					100);
		}
	}

	/*** Diagnostics collecting code goes here:  ***/

	local = (io_sAgentLocal *) ap->Local;
	op = (pwr_sClass_Pb_Hilscher *) ap->op;

	/* If everything is fine we should be in state OPERATE.
	Make a poll to see if there are diagnostics, the answer also tells us
	if there are any hardware faults.  */

	if ((op->Process & io_mProcess_Profibus)
			&& (ctx->Process != io_mProcess_Profibus))
		return IO__SUCCESS;

	if (op->DisableBus == 1)
		return IO__SUCCESS;

	/* Reads the protocol states and checks for errors.  */
	DevGetTaskState(local->dev_number, 2, 64, &diag);
	/* Checks if master is in state OPERATE.  */
	switch (diag.bDPM_state) {

	case OPERATE:
		if (op->Status != PB__NORMAL) {
			op->Status = PB__NORMAL;
			errh_Info("Profibus DP Master %s - Mode changed to OPERATE",
					ap->Name);
		}
		break;
	case CLEAR:
		if (op->Status != PB__CLEARED) {
			op->Status = PB__CLEARED;
			errh_Info("Profibus DP Master %s - Mode changed to CLEAR",
					ap->Name);
		}
		break;
	case STOP:
		if (op->Status != PB__STOPPED) {
			op->Status = PB__STOPPED;
			errh_Info("Profibus DP Master %s - Mode changed to STOP", ap->Name);
		}
		break;
	case OFFLINE:
		if (op->Status != PB__NOTINIT) {
			errh_Info("Profibus DP Master %s - Mode changed to OFFLINE",
					ap->Name);
			op->Status = PB__NOTINIT;
		}
		break;
	default:
		errh_Info("Profibus DP Master %s - Unknown mode", ap->Name);
	}

	/* Outputs some bus related diagnostics (using errh_Info()).  */
	dpm_print_diag(ap, &diag);

	/* Checks if there are any new diagnostics data that we should read from
	any of our slaves.  */
	for (i = 0; i <= 127; ++i) {
		/* The abSl_diag data structure is described on page 22 in
		dpm_pie.pdf. */
		if (diag.abSl_diag[i >> 3] & (1 << (i & 7))) {
			/* Request diagnostics from slaves with unread diagnostic data.  */
			dpm_req_slave_diag(local, i, ap);
		}
	}
	/* The above code requests diagnostics from the slaves
	with new diagnostic data.  It does not seem to work reliably in
	some circumstances though.  E.g. if a slave is disconnected and reconnected
	before its watchdog times out the diagnostics would not be updated.  The
	following code covers that case, it is a bit ugly, but works.  */
	for (slave_list = ap->racklist; slave_list != NULL;
			slave_list = slave_list->next) {
		sp = (pwr_sClass_Pb_DP_Slave *) slave_list->op;
		/* The abSl_state has the same layout as the abSl_diag bitmap.  */
		if (sp->StationStatus1 & pwr_mPbStationStatus1Mask_NonExistent &&
				diag.abSl_state[sp->SlaveAddress >> 3]
				& (1 << (sp->SlaveAddress & 7)))
			dpm_req_slave_diag(local, sp->SlaveAddress, ap);
	}
	/* Collects requested slave diagnostics.  */
	dpm_update_slave_diag(local, ap->racklist, ap);
  
	return IO__SUCCESS;
}


/*----------------------------------------------------------------------------*\
   Write method for the Pb_Hilscher agent  
\*----------------------------------------------------------------------------*/
static pwr_tStatus
IoAgentWrite(io_tCtx ctx, io_sAgent *ap)
{
	pwr_sClass_Pb_Hilscher *mp;
	pwr_sClass_Pb_DP_Slave *sp;
	io_sAgentLocal *local;
	io_sRack *slave_list;

	/* Handle for local data structure.  */
	local = (io_sAgentLocal *) ap->Local;

	/* Iterates over the slaves on the bus and writes process data to their
	respective addresses.  This is really the rack level's responsibility,
	however, keeping this code here enables us to confine all the Profibus
	board specific code at the agent level.  */
	for (slave_list = ap->racklist; slave_list != NULL;
			slave_list = slave_list->next) {

		sp = (pwr_sClass_Pb_DP_Slave *) slave_list->op;
		mp = (pwr_sClass_Pb_Hilscher *) ap->op;

		if ((sp->Status == PB__NORMAL || sp->Status == PB__NOCONN)
				&& mp->Status == PB__NORMAL
				&& (sp->DisableSlave != 1) && (mp->DisableBus != 1)) {

			if (sp->BytesOfOutput > 0) {
			/* Trigger the board's watchdog.  */
			DevTriggerWatchDog(local->dev_number, WATCHDOG_START,
					&local->watchdog);
			/* Writes process image to the slave.  */
				if (DevExchangeIO(local->dev_number,
						sp->OffsetOutputs, sp->BytesOfOutput, sp->Outputs,
						0, 0, NULL,
						100)
						!= DRV_NO_ERROR)
					sp->ErrorCount++;
			}
		}
	}

	return IO__SUCCESS;
}


/*----------------------------------------------------------------------------*\
  
\*----------------------------------------------------------------------------*/
static pwr_tStatus
IoAgentClose(io_tCtx ctx, io_sAgent *ap)
{
	io_sAgentLocal *local;

	local = (io_sAgentLocal *) ap->Local;

	dpm_exit(local, ap);
	free(local);

	return IO__SUCCESS;
}


/*----------------------------------------------------------------------------*\
  Every method to be exported to the workbench should be registred here.
\*----------------------------------------------------------------------------*/

pwr_dExport pwr_BindIoMethods(Pb_Hilscher) = {
	pwr_BindIoMethod(IoAgentInit),
	pwr_BindIoMethod(IoAgentRead),
	pwr_BindIoMethod(IoAgentWrite),
	pwr_BindIoMethod(IoAgentClose),
	pwr_NullMethod
};

