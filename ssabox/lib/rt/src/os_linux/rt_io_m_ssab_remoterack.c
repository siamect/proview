/* 
 * Proview   $Id: rt_io_m_ssab_remoterack.c,v 1.4 2006-06-02 07:14:29 claes Exp $
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

/* rt_io_m_ssab_remoterack.c -- io methods for ssab remote rack objects. */

#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#include "pwr.h"
#include "pwr_baseclasses.h"
#include "pwr_basecomponentclasses.h"
#include "pwr_ssaboxclasses.h"
#include "rt_io_base.h"
#include "rt_errh.h"
#include "rt_io_rack_init.h"
#include "rt_io_m_ssab_locals.h"
#include "rt_io_msg.h"


/*----------------------------------------------------------------------------*\
  
\*----------------------------------------------------------------------------*/

static pwr_tStatus IoRackInit (
  io_tCtx	ctx,
  io_sAgent	*ap,
  io_sRack	*rp
) 
{
  io_sRackLocal *local;
  pwr_sClass_Ssab_RemoteRack *op;
  int sts;

  op = (pwr_sClass_Ssab_RemoteRack *) rp->op;
  local = calloc( 1, sizeof(*local));
  rp->Local = local;
  
  /* Create socket, store in local struct */
  
  local->s = socket(AF_INET, SOCK_DGRAM, 0);
  if (local->s < 0) { 
    errh_Error( "Error creating socket for IO remote rack %s, %d", rp->Name, local->s);
    op->Status = IO__INITFAIL;
    return IO__ERRINIDEVICE;
  }
  
  /* Bind local port if explicitly numbered ( != 0 ) */
  
  if (op->LocalPort != 0) {
    local->my_addr.sin_family = AF_INET;
    local->my_addr.sin_port = htons(op->LocalPort);
    sts = bind(local->s, (struct sockaddr *) &local->my_addr, sizeof(local->my_addr));
    if (sts != 0) { 
      errh_Error( "Error binding local socket for IO remote rack %s, %d", rp->Name, sts);
      op->Status = IO__INITFAIL;
      return IO__ERRINIDEVICE;
    }
  }
  
  /* Initialize remote address structure */

  local->rem_addr.sin_family = AF_INET;
  local->rem_addr.sin_port = htons(op->RemotePort);
  local->rem_addr.sin_addr.s_addr = inet_addr((char *) &(op->Address));
  
  /* Connect to remote address */
  
  sts = connect(local->s, (struct sockaddr *) &local->rem_addr, sizeof(local->rem_addr)); 
  if (sts != 0) { 
    errh_Error( "Error binding remote socket for IO remote rack %s, %d", rp->Name, sts);
    op->Status = IO__INITFAIL;
    return IO__ERRINIDEVICE;
  }
  
  local->next_read_req_item = 0;
  local->next_write_req_item = 0;
  
  op->RX_packets = 0;
  op->TX_packets = 0;
    
  /* Log initialization */
  
  errh_Info( "Init of IO remote rack %s/%s:%d",
  	     rp->Name, inet_ntoa(local->rem_addr.sin_addr), ntohs(local->rem_addr.sin_port));
  op->Status = IO__NORMAL;
  return IO__SUCCESS;
}

static pwr_tStatus IoRackClose (
  io_tCtx	ctx,
  io_sAgent	*ap,
  io_sRack	*rp
) 
{
  if (rp->Local) free (rp->Local);
  return IO__SUCCESS;
}

static pwr_tStatus IoRackRead (
  io_tCtx	ctx,
  io_sAgent	*ap,
  io_sRack	*rp
) 
{
  int sts;
  fd_set fds;
  struct timeval tv;
  io_sRackLocal *local = (io_sRackLocal *) rp->Local;
  pwr_sClass_Ssab_RemoteRack *op = (pwr_sClass_Ssab_RemoteRack *) rp->op;
  struct bfb_buf rbuf;
  int size;

  bzero(&local->read_area, sizeof(local->read_area));
  bzero(&local->write_area, sizeof(local->write_area));

  sts = 1;
  while (sts > 0) {
    FD_ZERO(&fds);
    FD_SET(local->s, &fds);
    tv.tv_sec = 0;
    tv.tv_usec = 0;
    sts = select(32, &fds, NULL, NULL, &tv);
    if (sts > 0) {
      size = recv(local->s, &rbuf, sizeof(rbuf), 0);
      if (rbuf.service == BFB_SERVICE_READ) {
        bzero(&local->read_area, sizeof(local->read_area));
        memcpy(&local->read_area, &rbuf, size);
      }
      else if (rbuf.service == BFB_SERVICE_WRITE) {
        bzero(&local->write_area, sizeof(local->write_area));
        memcpy(&local->write_area, &rbuf, size);
      }
      op->RX_packets++;
    }
  } 
  
  return IO__SUCCESS;
}

static pwr_tStatus IoRackWrite (
  io_tCtx	ctx,
  io_sAgent	*ap,
  io_sRack	*rp
) 
{
  int sts;
  io_sRackLocal *local = (io_sRackLocal *) rp->Local;
  pwr_sClass_Ssab_RemoteRack *op = (pwr_sClass_Ssab_RemoteRack *) rp->op;

  // Calc length and send write request
  local->write_req.service = BFB_SERVICE_WRITE;  
  local->write_req.length = local->next_write_req_item*4 + 4;
  sts = send(local->s, &local->write_req, local->write_req.length, 0);
  op->TX_packets++;
  
  // Calc length and send read request
  local->read_req.service = BFB_SERVICE_READ;  
  local->read_req.length = local->next_read_req_item*4 + 4;
  sts = send(local->s, &local->read_req, local->read_req.length, 0);
  op->TX_packets++;
  
  local->next_read_req_item = 0;
  local->next_write_req_item = 0;

  return IO__SUCCESS;
}


/*----------------------------------------------------------------------------*\
  
\*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*\
  Every method to be exported to the workbench should be registred here.
\*----------------------------------------------------------------------------*/

pwr_dExport pwr_BindIoMethods(Ssab_RemoteRack) = {
  pwr_BindIoMethod(IoRackInit),
  pwr_BindIoMethod(IoRackClose),
  pwr_BindIoMethod(IoRackRead),
  pwr_BindIoMethod(IoRackWrite),
  pwr_NullMethod
};


