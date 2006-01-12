/* 
 * Proview   $Id: remote_remio_utils.h,v 1.1 2006-01-12 06:39:33 claes Exp $
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

/*************************************************************************
*
* Filename:             remio_utils.h
*
*                       Date    Pgm.    Read.   Remark
* Modified              
*
* Description:		Remote I/O utilities
*
*************************************************************************/

#if defined __cplusplus
extern "C" {
#endif

/*_function prototypes__________________________________________________*/

pwr_tFloat32 ConvAItoAct(pwr_tInt16 raw, pwr_sClass_RemChan_Ai *body);
pwr_tFloat32 ConvAOtoAct(pwr_tInt16 raw, pwr_sClass_RemChan_Ao *body);
pwr_tInt16   ConvAOtoRaw(pwr_tFloat32 act, pwr_sClass_RemChan_Ao *body);

pwr_tStatus  RemIO_Init_ALCM		(remnode_item *remnode);
pwr_tStatus  RemIO_Stall_ALCM		(remnode_item *remnode);
pwr_tStatus  RemIO_Receive_ALCM		(remnode_item *remnode,
		 	        	 bsp_buffer *buffer,
			        	 int size);
pwr_tStatus  RemIO_Cyclic_ALCM		(remnode_item *remnode,
			       		 void (* send_pollbuff) 
				    	 (remnode_item *remnode,
				     	 pssupd_buffer *buf));

pwr_tStatus  RemIO_Init_3964R		(remnode_item *remnode);

pwr_tStatus  RemIO_Stall_3964R		(remnode_item *remnode,
					 int stall_action);

pwr_tStatus  RemIO_Receive_3964R(remnode_item   *remnode,
		 	         unsigned char  *buffer,
			         int            size);

pwr_tStatus  RemIO_Cyclic_3964R(remnode_item *remnode,
			        void (* send_pollbuff) 
				     (remnode_item *remnode,
				      pssupd_buffer_vnet *buf));
				      
#if defined __cplusplus
}
#endif
