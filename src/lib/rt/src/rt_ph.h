/* 
 * Proview   $Id: rt_ph.h,v 1.2 2005-09-01 14:57:56 claes Exp $
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

#ifndef rt_ph_h
#define rt_ph_h

#if 0 /* NYI*/
/* rt_ph.h -- Runtime environment - Packet Handler */

#define RS_PH_VERSION "V1.1-1"

#ifndef pwr_h
#include "pwr.h"
#endif

#ifndef pwr_class_h
#include "pwr_class.h"
#endif

#include "rt_ph_msg.h"

#ifndef qcom_h
#include "qcom.h"
#endif

#define ph_cMsgClass            203

/* Enumerations 

typedef enum phe_Pack ph_ePack;
*/

/* Structures */

typedef struct phs_FromOp ph_sFromOp;
typedef struct phs_Standard ph_sStandard;
typedef struct phs_ToOp ph_sToOp;

/* Types */

typedef void *ph_tCtx;


/* Unions */

typedef union phu_Pack ph_uPack;



/*
* Name:
*   ph_ePack
* Description:
*   The different types of packets
*/
enum phe_Pack
    {
    ph_ePack__		= 0,    
    ph_ePack_FromOp	= 1,
    ph_ePack_ToOp	= 2,
    ph_ePack_Standard	= 3,
    ph_ePack_
    };
typedef enum phe_Pack ph_ePack;

/*
* Name:
*   ph_sFromOp   
* Description:
*   Packet sent from an operator
*/
struct phs_FromOp
    {
    ph_ePack	    Type;
    pwr_tClassId    PackClass;
    pwr_tInt32	    PackId;
    pwr_tObjid	    OpPlace;
    pwr_tObjid	    User;
    pwr_tObjid	    GraphId;
    pwr_tInt32	    StateId;
    };

/*
* Name:
*   ph_sStandard   
* Description:
*   Standard packet. Used between applications
*/
struct phs_Standard
    {
    ph_ePack	    Type;
    pwr_tClassId    PackClass;
    pwr_tInt32	    PackId;
    pwr_tObjid	    SendQue;
    };

/*
* Name:
*   ph_sToOp   
* Description:
*   Packet sent to an operator
*/
struct phs_ToOp
    {
    ph_ePack	    Type;
    pwr_tClassId    PackClass;
    pwr_tObjid	    SendQueue;
    pwr_tObjid	    GraphId;
    pwr_tInt32	    StateId;
    };


/*
* Name:
*   ph_uPack  
* Description:
*   Defines a packet
*/
union phu_Pack
    {
    ph_ePack	  Type;
    ph_sFromOp	  FromOp;
    ph_sToOp	  ToOp;
    ph_sStandard  Standard;
    };





/*
* Name:    
*   ph_Connect
*
*
* Function:
*   Connects calling application to a queue-object.
*   The returned context is used as an identifier when calling Get and 
*   Disconnect
* Description:
*   
*/
pwr_tStatus ph_Connect
    (
    char	    *QueName,
    mq_uAddress     Address,
    ph_tCtx	    *Ctx
    );

/*
* Name:    
*   ph_Disconnect
*
*
* Function:
*   Disconnects calling application from a queue-object.
*   Frees the context 
* Description:
*   
*/
pwr_tStatus ph_Disconnect
    (
    ph_tCtx	    Ctx
    );

/*
* Name:    
*   ph_FreeClassData
*
*
* Function:
*   Frees class data which where fetched with ph_GetPacketXXX
* Description:
*   
*/
pwr_tStatus ph_FreeClassData
    (
    void	    *ClassData
    );

/*
* Name:    
*   ph_GetPacket
*
*
* Function:
*   Gets the next packet in the queue. Returns immeditely if the queue is 
*   empty
* Description:
*   
*/
pwr_tStatus ph_GetPacket
    (
    ph_tCtx	    Ctx,
    ph_uPack	    *Pack,
    pwr_tClassId    *PackClass,
    void	    **ClassData
    );

/*
* Name:    
*   ph_GetWaitPacket
*
*
* Function:
*   Gets the next packet in the queue. Waits until a packet arrives or it is
*   timeouted
* Description:
*   
*/
pwr_tStatus ph_GetWaitPacket
    (
    ph_tCtx	    Ctx,
    ph_uPack	    *Pack,
    pwr_tClassId    *PackClass,
    void	    **ClassData,
    pwr_tUInt32	    Timeout
    );

/*
* Name:    
*   ph_SendPacket
*
*
* Function:
*   Sends a packet to a  queue or an operator.
* Description:
*   
*/
pwr_tStatus ph_SendPacket
    (
    char	    *QueName,
    ph_uPack	    *Pack,
    pwr_tClassId    PackClass,
    pwr_tUInt32	    ClassSize,
    void	    *ClassData
    );


#endif

#endif
