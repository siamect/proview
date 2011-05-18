/* 
 * Proview   $Id$
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

/* rt_io_m_udp_io.c -- I/O methods for UDP communication. */

#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

#include "pwr.h"
#include "pwr_basecomponentclasses.h"
#include "pwr_otherioclasses.h"
#include "co_time.h"
#include "co_cdh.h"
#include "rt_io_base.h"
#include "rt_io_bus.h"
#include "rt_io_card_init.h"
#include "rt_io_card_close.h"
#include "rt_io_card_read.h"
#include "rt_io_card_read.h"
#include "rt_io_msg.h"

typedef struct {
  float time_since_rcv;
  float time_since_keepalive;
  int socket;				
  struct sockaddr_in local_addr;
  struct sockaddr_in remote_addr;
  int byte_ordering;
  int float_representation;
  int input_area_size;
  int output_area_size;
  int input_buffer_size;
  int output_buffer_size;
  char *input_buffer;
  char *output_buffer;
  char *input_area;
  char *output_area;
  int softlimit_logged;
} io_sLocalUDP_IO;

typedef struct {
  unsigned char protocol_id[2];
  unsigned short int msg_size;
  unsigned short int msg_id[2];
} io_sUDP_Header;

#define STX 2
#define ETB 15
#define ENQ 5
#define ACK 6
#define UDP_MAX_SIZE 32768

static int SendKeepalive( io_sLocalUDP_IO *local, pwr_sClass_UDP_IO *op)
{
  int sts;
  io_sUDP_Header header;  

  /* Fill in application header and convert to network byte order */

  header.protocol_id[0] = STX;
  header.protocol_id[1] = ETB;
  header.msg_size = htons(sizeof(header));
  header.msg_id[0] = 0;
  header.msg_id[1] = 0;

  sts = sendto( local->socket, &header, sizeof(header), 0, 
		(struct sockaddr *) &local->remote_addr, sizeof(struct sockaddr));

  if (sts >= 0) 
    op->KeepAliveDiff++;

  return sts;
}

static pwr_tStatus IoCardInit( io_tCtx ctx,
			       io_sAgent *ap,
			       io_sRack *rp,
			       io_sCard *cp)
{
  io_sLocalUDP_IO *local;
  pwr_sClass_UDP_IO *op = (pwr_sClass_UDP_IO *)cp->op;
  int sts;
  unsigned char badr[4];
  unsigned int iadr[4] = {-1, -1, -1, -1};
  struct hostent *he;
  struct sockaddr_in address;
  socklen_t address_len = sizeof(struct sockaddr_in);
  unsigned int input_area_offset = 0;
  unsigned int input_area_chansize = 0;
  unsigned int output_area_offset = 0;
  unsigned int output_area_chansize = 0;

  local = (io_sLocalUDP_IO *) calloc( 1, sizeof(io_sLocalUDP_IO));
  cp->Local = local;


  /* Create a socket for UDP */
  local->socket = socket(AF_INET, SOCK_DGRAM, 0);
  if ( local->socket < 0) { 
    errh_Error( "UDP_IO, error creating socket, %d, '%s'", local->socket, cp->Name);
    op->Status = IO__INITFAIL;
    return IO__INITFAIL;
  }

  if ( op->LocalPort != 0) {

    /* Set local port */
    local->local_addr.sin_family = AF_INET;
    local->local_addr.sin_port = htons(op->LocalPort);

    /* Bind the created socket */
    sts = bind(local->socket, (struct sockaddr *) &local->local_addr, 
	       sizeof(local->local_addr));
    if (sts != 0) {
      errh_Error( "UDP_IO, error bind socket, %d, '%s'", sts, cp->Name);
      op->Status = IO__INITFAIL;
      return IO__INITFAIL;
    }
  }
  else {
    getsockname( local->socket, (struct sockaddr *) &address, &address_len);
    op->LocalPort = ntohs(address.sin_port);
  }   

  /* Initialize remote address structure */

  local->remote_addr.sin_family = AF_INET;
  local->remote_addr.sin_port = htons(op->RemotePort);
  sscanf((char *) &(op->RemoteAddress), "%d.%d.%d.%d", 
	&iadr[0], &iadr[1], &iadr[2], &iadr[3]);
	
  /* If none or invalid ip-address is given, use hostname to get hostent struct,
     otherwise use the given ip address directly */
  
  if ((iadr[0] < 0 || iadr[0] > 255) ||
      (iadr[0] < 0 || iadr[0] > 255) ||
      (iadr[0] < 0 || iadr[0] > 255) ||
      (iadr[0] < 0 || iadr[0] > 255)) {
    he = gethostbyname(op->RemoteHostName);
    if (he) {
      memcpy( &local->remote_addr.sin_addr, he->h_addr, 4);
      sprintf( op->RemoteAddress, "%s", inet_ntoa(local->remote_addr.sin_addr));
    }
    else {
      errh_Error( "UDP_IO, unknown remote host %s, '%s'", op->RemoteHostName, cp->Name);
      op->Status = IO__INITFAIL;
      return IO__INITFAIL;
    }
  }
  else {
    badr[0] = (unsigned char) iadr[0];
    badr[1] = (unsigned char) iadr[1];
    badr[2] = (unsigned char) iadr[2];
    badr[3] = (unsigned char) iadr[3];
    memcpy(&local->remote_addr.sin_addr, &badr, 4);
  }

  op->Link = pwr_eUpDownEnum_Down;
  op->KeepAliveDiff = 0;

  local->byte_ordering = op->ByteOrdering;
  
  io_bus_card_init( ctx, cp, &input_area_offset, &input_area_chansize,
		    &output_area_offset, &output_area_chansize, local->byte_ordering);

  local->input_area_size = input_area_offset + input_area_chansize;
  local->output_area_size = output_area_offset + output_area_chansize;

  op->InputAreaSize = local->input_area_size;
  op->OutputAreaSize = local->output_area_size;

  if ( op->EnableHeader) {
    local->input_buffer_size = local->input_area_size + sizeof(io_sUDP_Header);
    local->output_buffer_size = local->output_area_size + sizeof(io_sUDP_Header);
  }
  else {
    local->input_buffer_size = local->input_area_size;
    local->output_buffer_size = local->output_area_size;
  }

  if ( local->input_area_size > 0)
    local->input_buffer = calloc( 1, local->input_buffer_size);
  if ( local->output_area_size > 0)
    local->output_buffer = calloc( 1, local->output_buffer_size);

  if ( op->EnableHeader) {
    local->input_area = local->input_buffer + sizeof(io_sUDP_Header);
    local->output_area = local->output_buffer + sizeof(io_sUDP_Header);
  }
  else {
    local->input_area = local->input_buffer;
    local->output_area = local->output_buffer;
  }

  errh_Info( "Init of UDP_IO '%s'", cp->Name);
  op->Status = IO__SUCCESS;

  return IO__SUCCESS;
}

static pwr_tStatus IoCardClose( io_tCtx ctx,
			        io_sAgent *ap,
			        io_sRack *rp,
			        io_sCard *cp)
{
  io_sLocalUDP_IO *local = (io_sLocalUDP_IO *)cp->Local;

  close( local->socket);

  if ( local->input_buffer)
    free( local->input_buffer);
  if ( local->output_buffer)
    free( local->output_buffer);
  if ( cp->Local)
    free( cp->Local);

  return IO__SUCCESS;
}

static pwr_tStatus IoCardRead( io_tCtx ctx,
			       io_sAgent *ap,
			       io_sRack	*rp,
			       io_sCard	*cp)
{
  io_sLocalUDP_IO *local = (io_sLocalUDP_IO *)cp->Local;
  pwr_sClass_UDP_IO *op = (pwr_sClass_UDP_IO *)cp->op;
  fd_set fds;				/* For select call */
  static char buf[UDP_MAX_SIZE];
  char unknown[24];
  unsigned char badr[24];
  struct sockaddr_in from;
  unsigned int fromlen;
  int size;
  unsigned int sts;
  io_sUDP_Header header;  
  struct timeval tv;
  int data_received = 0;
 
  local->time_since_keepalive += ctx->ScanTime;
  local->time_since_rcv += ctx->ScanTime;
  local->time_since_keepalive = MIN(local->time_since_keepalive, op->KeepAliveTime + 1.0);
  local->time_since_rcv = MIN(local->time_since_rcv, op->LinkTimeout + 1.0);

  tv.tv_sec = 0;
  tv.tv_usec = 0;

  FD_ZERO( &fds);
  FD_SET( local->socket, &fds);
  sts = select(32, &fds, NULL, NULL, &tv);  
  if ( sts < 0) {
    op->ErrorCount++;
    goto read_error;
  }

  fromlen = sizeof(struct sockaddr);

  size = recvfrom( local->socket, buf, sizeof(buf), MSG_DONTWAIT,
		   (struct sockaddr *) &from, &fromlen);
  if (size <= 0) {
    if ( errno != EAGAIN) {
      errh_Info("UDP IO Receive failure %d %s", size, op->RemoteHostName);
      op->ErrorCount++;
    }
    goto read_error;
  }

  if ( memcmp(&from.sin_addr, &local->remote_addr.sin_addr, sizeof(struct in_addr)) != 0) {
    /* Wrong address */
    memcpy(&badr, &from.sin_addr, 4);
    sprintf(unknown, "%d.%d.%d.%d", badr[0], badr[1], badr[2], badr[3]);
    errh_Info("UDP_IO Receive from unknown source %s", unknown);
    op->ErrorCount++;
    goto read_error;
  }

  /* Set link up */
  local->time_since_rcv = 0;
  if ( op->Link == pwr_eUpDownEnum_Down) {
    errh_Info("UDP IO link up to %s", op->RemoteHostName);
    op->Link = pwr_eUpDownEnum_Up;
  }

  if ( size < local->input_buffer_size) {
    errh_Info("UDP IO message size too small %s", op->RemoteHostName);
    op->ErrorCount++;
    goto read_error;
  }

  memcpy( local->input_buffer, buf, size);

  if ( !op->EnableHeader)
    data_received = 1;
  else {
    /* Header enabled */
    memcpy(&header, buf, sizeof(io_sUDP_Header));

    /* Convert the header to host byte order */
    header.msg_size = ntohs(header.msg_size);
    header.msg_id[0] = ntohs(header.msg_id[0]);
    header.msg_id[1] = ntohs(header.msg_id[1]);

    if (header.protocol_id[0] == STX && size == header.msg_size) {
      /* This is a valid message */
      if (header.protocol_id[1] == ETB || header.protocol_id[1] == ENQ) {
	/* Keepalive */
	if (header.msg_id[0] == 0 && header.msg_id[1] == 0) {
	  /* Keepalive */
	  op->KeepAliveDiff--;
	}
	else {
	  /* Data */
	  data_received = 1;
	}
      }
      else if (header.protocol_id[1] == ACK) {
	/* Acknowledge message */ 
      }
      else {
	/* Weird header */
	op->ErrorCount++;
	errh_Info("UDP IO receive weird header %s, %02x %02x %04x %04x %04x", 
		    op->RemoteHostName, header.protocol_id[0], header.protocol_id[1],
		    header.msg_size, header.msg_id[0], header.msg_id[1]);
	goto read_error;
      }
    }
  }

  if ( data_received) {
    io_bus_card_read( ctx, rp, cp, local->input_area, 0, 
		      local->byte_ordering, pwr_eFloatRepEnum_FloatIEEE);
  }

 read_error:
  if ( local->time_since_rcv >=  op->LinkTimeout && op->LinkTimeout > 0) {
    if ( op->Link == pwr_eUpDownEnum_Up) {
      errh_Info("UDP IO link down %s", op->RemoteHostName);
      op->Link = pwr_eUpDownEnum_Down;
    }
  }

  if ( local->time_since_keepalive >= op->KeepAliveTime) {
    if ( op->UseKeepAlive) 
      SendKeepalive( local, op);
    local->time_since_keepalive = 0;
  }

  if ( op->ErrorCount == op->ErrorSoftLimit && !local->softlimit_logged) {
    errh_Warning( "IO Card ErrorSoftLimit reached, '%s'", cp->Name);
    local->softlimit_logged = 1;
  }
  if ( op->ErrorCount >= op->ErrorHardLimit) {
    errh_Error( "IO Card ErrorHardLimit reached '%s', IO stopped", cp->Name);
    ctx->Node->EmergBreakTrue = 1;
    return IO__ERRDEVICE;
  }    

  return IO__SUCCESS;
}

static pwr_tStatus IoCardWrite( io_tCtx ctx,
				io_sAgent *ap,
				io_sRack	*rp,
				io_sCard	*cp)
{
  io_sLocalUDP_IO *local = (io_sLocalUDP_IO *)cp->Local;
  pwr_sClass_UDP_IO *op = (pwr_sClass_UDP_IO *)cp->op;
  int status;

  if ( op->EnableHeader) {
    io_sUDP_Header *hp = (io_sUDP_Header *)local->output_buffer;
    hp->protocol_id[0] = STX;
    hp->protocol_id[1] = ETB;

    hp->msg_size = htons(local->output_area_size);
    hp->msg_id[0] = htons(op->MessageId[0]);
    hp->msg_id[1] = htons(op->MessageId[1]);
  }

  io_bus_card_write( ctx, cp, &local->output_area,
		     local->byte_ordering, pwr_eFloatRepEnum_FloatIEEE);
    
  status = sendto( local->socket, local->output_buffer, local->output_buffer_size, 0,
		   (struct sockaddr *) &local->remote_addr, sizeof(struct sockaddr));

  if ( op->ErrorCount == op->ErrorSoftLimit && !local->softlimit_logged) {
    errh_Warning( "IO Card ErrorSoftLimit reached, '%s'", cp->Name);
    local->softlimit_logged = 1;
  }
  if ( op->ErrorCount >= op->ErrorHardLimit) {
    errh_Error( "IO Card ErrorHardLimit reached '%s', IO stopped", cp->Name);
    ctx->Node->EmergBreakTrue = 1;
    return IO__ERRDEVICE;
  }    

  return IO__SUCCESS;
}


/*  Every method should be registred here. */

pwr_dExport pwr_BindIoMethods(UDP_IO) = {
  pwr_BindIoMethod(IoCardInit),
  pwr_BindIoMethod(IoCardClose),
  pwr_BindIoMethod(IoCardRead),
  pwr_BindIoMethod(IoCardWrite),
  pwr_NullMethod
};
