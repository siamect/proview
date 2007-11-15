/* 
 * Proview   $Id: rs_remote_tcpip.c,v 1.4 2007-11-15 14:57:44 claes Exp $
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
*		===============
*                P r o v i e w
*               ===============
**************************************************************************
*
* Filename:             rs_remote_tcpip.c
*
*                       Date    Pgm.    Read.   Remark
* Modified              040108  CJu
*
* Description:		Remote transport process TCP/IP
*			Implements transport protocol TCP/IP, connection oriented
*			protocol on the IP-stack.
*
**************************************************************************
**************************************************************************/

/*_Include files_________________________________________________________*/

#include <stdio.h>
#include <errno.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/socket.h>

#include "pwr_class.h"
#include "pwr_systemclasses.h"
#include "rt_gdh.h"
#include "co_cdh.h"
#include "rt_errh.h"
#include "pwr_baseclasses.h"
#include "pwr_remoteclasses.h"
#include "rt_pwr_msg.h"
#include "rt_aproc.h"
#include "remote.h"
#include "remote_remtrans_utils.h"

//#define debug 0

#define STX 2
#define ETB 15

#define TCP_MAX_SIZE 32768 
#define TIME_INCR 0.02

int debug=0;

fd_set fdr;				/* For select call */
fd_set fde;				/* For select call */

typedef struct
{
  unsigned char protocol_id[2];
  short int msg_size;
  short int msg_id[2];
} remote_tcp_header;

remnode_item rn;
pwr_sClass_RemnodeTCP *rn_tcp;

float time_since_scan;
float time_since_rcv;
float time_since_keepalive;

char receive_buffer[65536];
unsigned char remote_tcp_id[2] = {STX, ETB};

int l_socket;				/* Local socket */
int c_socket;				/* The connected socket */
struct sockaddr_in l_addr;		/* Local named socket description */
struct sockaddr_in r_addr;		/* Remote socket description */

enum cs_modes {TCP_CLIENT, TCP_SERVER} cs_mode;

/*************************************************************************
**************************************************************************
*
* Namn : RemoteSleep
*
* Typ  : unsigned int
*
* Typ		Parameter	       IOGF	Beskrivning
*
* Beskrivning : 
*
**************************************************************************
**************************************************************************/

void RemoteSleep(float time)
{
#ifdef OS_VMS
        int sts;
        sts = lib$wait(&time);
#elif OS_ELN
        LARGE_INTEGER   l_time;

        l_time.high = -1;
        l_time.low = - time * 10000000;
        ker$wait_any(NULL, NULL, &l_time);

#elif defined(OS_LYNX) || defined(OS_LINUX)
        struct timespec rqtp, rmtp;

        rqtp.tv_sec = 0;
        rqtp.tv_nsec = time * 1000000000;
	if (time >= 1.0) {
          rqtp.tv_sec = time/1;
          rqtp.tv_nsec = 0;
	}
	else {
          rqtp.tv_nsec = time * 1000000000;
	}
        nanosleep(&rqtp, &rmtp);
#endif
        return;
}


/*************************************************************************
**************************************************************************
*
* Namn : CreateSocket
*
* Typ  : unsigned int
*
* Typ		Parameter	       IOGF	Beskrivning
*
* Beskrivning : Creates socket and sets remote side addresses
*
*
**************************************************************************
**************************************************************************/

int CreateSocket()
{
  int sts;

  if (cs_mode == TCP_CLIENT) {
    /* Create a socket for TCP */
    c_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (debug) printf("socket: %d\n", c_socket);
    if (debug && (c_socket < 0)) perror("socket");
    if (c_socket < 0) return(-1);

    /* Bind the created socket */

    if (rn_tcp->LocalPort != 0) {
      l_addr.sin_family = AF_INET;
      l_addr.sin_port = htons(rn_tcp->LocalPort);
      sts = bind(c_socket, (struct sockaddr *) &l_addr, sizeof(l_addr));
      if (debug) printf("bind: %d\n", sts);
      if (debug && (sts < 0)) perror("bind");
      if (sts != 0) return(-1);
    }
  }

  else {
    /* Create a socket for TCP */
    l_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (debug) printf("socket: %d\n", l_socket);
    if (debug && (l_socket < 0)) perror("socket");
    if (l_socket < 0) return(-1);

    /* Bind the created socket */

    if (rn_tcp->LocalPort != 0) {
      l_addr.sin_family = AF_INET;
      l_addr.sin_port = htons(rn_tcp->LocalPort);
      sts = bind(l_socket, (struct sockaddr *) &l_addr, sizeof(l_addr));
      if (debug) printf("bind: %d\n", sts);
      if (debug && (sts < 0)) perror("bind");
      if (sts != 0) return(-1);
    }
    /* Create listening queue */
    sts = listen(l_socket, 1);
    if (debug) printf("listen: %d\n", sts);      
    
  }

  rn_tcp->LinkUp = 0;
  rn_tcp->KeepaliveDiff = 0;

  time_since_scan = 0;
  time_since_rcv = 0;
  time_since_keepalive = 0;
  
  return(0);
}

/*************************************************************************
**************************************************************************
*
* Namn : Connect
*
* Typ  : unsigned int
*
* Typ		Parameter	       IOGF	Beskrivning
*
* Beskrivning : Connects local socket to server
*
*
**************************************************************************
**************************************************************************/

int Connect()
{
  int sts, sts2;
  struct sockaddr_in l_addr;
  int l_addr_len;

  sts = -1;
  if (cs_mode == TCP_CLIENT) {

    /* Initialize remote address structure */

    r_addr.sin_family = AF_INET;
    r_addr.sin_port = htons(rn_tcp->RemotePort);
    r_addr.sin_addr.s_addr = inet_addr(rn_tcp->RemoteAddress);
  
    /* Try to connect */
      
    sts = connect(c_socket, (struct sockaddr *) &r_addr, sizeof(r_addr));
    if (debug) printf("connect: %d\n", sts);      
    if (debug && (sts < 0)) perror("connect");
    
    /* Get local socket description */
    if (sts == 0) {
      l_addr_len = sizeof(struct sockaddr);
      sts2 = getsockname(c_socket, (struct sockaddr *) &l_addr, (unsigned int *)&l_addr_len);
      if (sts2 == 0) rn_tcp->LocalPort = ntohs(l_addr.sin_port);
    }
  }
  time_since_rcv = 0;
  if (sts == 0)   rn_tcp->LinkUp = 1;
  return(sts);
}

/*************************************************************************
**************************************************************************
*
* Namn : Accept
*
* Typ  : unsigned int
*
* Typ		Parameter	       IOGF	Beskrivning
*
* Beskrivning : Accepts a client
*
*
**************************************************************************
**************************************************************************/

int Accept()
{
  int l_addr_len;

  if (cs_mode == TCP_SERVER) {

    /* Wait for client */
    c_socket = accept(l_socket, (struct sockaddr *) &r_addr, (unsigned int *)&l_addr_len);

    if (debug) printf("accept: %d\n", c_socket);      
    if (debug && (c_socket < 0)) perror("accept");
    
  }
  time_since_rcv = 0;
  rn_tcp->LinkUp = 1;
  return(0);
}

/*************************************************************************
**************************************************************************
*
* Namn : Shutdown
*
* Typ  : unsigned int
*
* Typ		Parameter	       IOGF	Beskrivning
*
* Beskrivning : Shuts down connection
*
*
**************************************************************************
**************************************************************************/

void Shutdown()
{
  int sts;

  if (cs_mode == TCP_SERVER) {
    sts = close(c_socket);
  } else {
    sts = shutdown(c_socket, 2);
    if (debug) printf("shutdown: %d\n", sts);
    if (debug && (sts < 0)) perror("shutdown");
    sts = close(c_socket);
    if (debug) printf("close: %d\n", sts);
    if (debug && (sts < 0)) perror("close");
  }
  return;
}

/*************************************************************************
**************************************************************************
*
* Namn : TreatRemtrans1
*
* Typ  : unsigned int
*
* Typ		Parameter	       IOGF	Beskrivning
*
* Beskrivning : 
*
**************************************************************************
**************************************************************************/

void TreatRemtrans1(char *buf)
{
  remtrans_item *remtrans;
  unsigned char search_remtrans;
  remote_tcp_header header;  
  unsigned int sts;

  /* Extract header and convert to host byte order */

  memcpy(&header, buf, sizeof(remote_tcp_header));

  header.msg_size = ntohs(header.msg_size);
  header.msg_id[0] = ntohs(header.msg_id[0]);
  header.msg_id[1] = ntohs(header.msg_id[1]);

  /* Start searching remtrans */

  remtrans = rn.remtrans;
  search_remtrans = true;
  while(remtrans && search_remtrans)
  {
    /* Match? */
    if (remtrans->objp->Address[0] == header.msg_id[0] && 
        remtrans->objp->Address[1] == header.msg_id[1] && 
        remtrans->objp->Direction == REMTRANS_IN)
    {
      search_remtrans = false;
      sts = RemTrans_Receive(remtrans, buf + sizeof(remote_tcp_header), 
			     header.msg_size-sizeof(remote_tcp_header));
    }
    remtrans = (remtrans_item *) remtrans->next;
  }
  if (search_remtrans) rn_tcp->ErrCount++;
  return;
}
/*************************************************************************
**************************************************************************
*
* Namn : TreatRemtrans2
*
* Typ  : unsigned int
*
* Typ		Parameter	       IOGF	Beskrivning
*
* Beskrivning : 
*
**************************************************************************
**************************************************************************/

void TreatRemtrans2(char *buf, int size)
{
  remtrans_item *remtrans;
  unsigned char search_remtrans;
  unsigned int sts;

  /* Start searching remtrans */

  remtrans = rn.remtrans;
  search_remtrans = true;
  while(remtrans && search_remtrans)
  {
    /* Match? */
    if (remtrans->objp->Direction == REMTRANS_IN)
    {
      search_remtrans = false;
      sts = RemTrans_Receive(remtrans, buf, size);
    }
    remtrans = (remtrans_item *) remtrans->next;
  }
  if (search_remtrans) rn_tcp->ErrCount++;
  return;
}

/*************************************************************************
**************************************************************************
*
* Namn : Receive
*
* Typ  : unsigned int
*
* Typ		Parameter	       IOGF	Beskrivning
*
* Beskrivning : Receives data
*
*
**************************************************************************
**************************************************************************/

unsigned int Receive()

{
  unsigned char more_messages;
  int data_size;
  int buf_ix;			/* Current position in receive buffer */
  remote_tcp_header header;  

  static char saved_buffer[65536];
  static int saved_fl = 0;
  static int saved_size;
  static int expected_rest;


  data_size = recv(c_socket, receive_buffer, sizeof(receive_buffer), 0);

  if (data_size < 0) 
  {
    /* Error */
    return(-1);
  }

  if (data_size == 0)
  {
    /* Disconnected */
    return(0);
  }

  if (rn_tcp->Disable) {
    saved_fl = false;
    return(1);
  }

  buf_ix = 0;
  more_messages = true;

  time_since_rcv = 0;
  
  if (saved_fl)
  {
    if (data_size >= expected_rest)
    {
      memcpy(&saved_buffer[saved_size], &receive_buffer, expected_rest);

      TreatRemtrans1(saved_buffer);

      /* Set position in data buffer */

      buf_ix = expected_rest;
      data_size = data_size - expected_rest;
      if (data_size < sizeof(header)) more_messages = false; 
    }
    saved_fl = 0;
  }

  while (more_messages)
  {
    if (data_size > 0 && rn_tcp->DisableHeader) {
      /* Header disabled, take the first receive remtrans object */ 
      
      TreatRemtrans2(receive_buffer, data_size);

    }
    else if (data_size >= sizeof(remote_tcp_header))
    {
      memcpy(&header, &receive_buffer[buf_ix], sizeof(remote_tcp_header));

      /* Convert to host byte order */

      header.msg_size = ntohs(header.msg_size);
      header.msg_id[0] = ntohs(header.msg_id[0]);
      header.msg_id[1] = ntohs(header.msg_id[1]);

      if (header.protocol_id[0] == remote_tcp_id[0] &&
          header.protocol_id[1] == remote_tcp_id[1])
      {
	if (data_size >= header.msg_size)
        {
	  if (header.msg_size > sizeof(header))	/* Not keepalive buffer */
	  {
	    TreatRemtrans1(&receive_buffer[buf_ix]);
	  }
          else if (header.msg_size == sizeof(header))	/* Keepalive buffer */
	    rn_tcp->KeepaliveDiff--;
	  else						/* Too short */
	    rn_tcp->ErrCount++;
        }
        else
        {
	  /* Remote ip-message but not complete, save buffer */
          memcpy(&saved_buffer, &receive_buffer[buf_ix], data_size);
  	  saved_fl = 1;
	  saved_size = data_size;
	  expected_rest = header.msg_size - data_size;
        }
      }
      else
      rn_tcp->ErrCount++;
    }
    else						/* Too short */
      rn_tcp->ErrCount++;

    if (!rn_tcp->DisableHeader && header.msg_size > 0 && !saved_fl)
    {
      data_size -= header.msg_size;
      buf_ix += header.msg_size;
      if (data_size < sizeof(header)) more_messages = false; 
    }
    else more_messages = false;
  }
    
  
  return(1);    
}

/*************************************************************************
**************************************************************************
*
* Namn : SendKeepalive
*
* Typ  : unsigned int
*
* Typ		Parameter	       IOGF	Beskrivning
*
* Beskrivning : 
*
**************************************************************************
**************************************************************************/

unsigned int SendKeepalive(void)

{
  int sts;
  remote_tcp_header header;  

  /* Fill in application header and convert to network byte order */

  header.protocol_id[0] = STX;
  header.protocol_id[1] = ETB;
  header.msg_size = htons(sizeof(header));
  header.msg_id[0] = 0;
  header.msg_id[1] = 0;

  sts = send(c_socket, &header, sizeof(header), 0);

  if (sts >= 0) rn_tcp->KeepaliveDiff++;
  
  return(sts);
}

/*************************************************************************
**************************************************************************
*
* Namn : RemnodeSend
*
* Typ  : unsigned int
*
* Typ		Parameter	       IOGF	Beskrivning
*
* Beskrivning : Sends data
*
*
**************************************************************************
**************************************************************************/

unsigned int RemnodeSend(remnode_item *remnode,
			  pwr_sClass_RemTrans *remtrans,
			  char *buf,
			  int buf_size)

{
  int sts;

  static struct message_s {
    remote_tcp_header header;  
    char data[TCP_MAX_SIZE];
  }message;


  memcpy(&message.data, buf, buf_size);

  if (rn_tcp->DisableHeader) {
    sts = send(c_socket, &message.data, buf_size, 0);
  }
  else {
    message.header.protocol_id[0] = STX;
    message.header.protocol_id[1] = ETB;

    message.header.msg_size = htons(buf_size+sizeof(remote_tcp_header));
    message.header.msg_id[0] = htons(remtrans->Address[0]);
    message.header.msg_id[1] = htons(remtrans->Address[1]);
    
    sts = send(c_socket, &message, buf_size + sizeof(remote_tcp_header), 0);
  }
  if (debug) printf("send: %d\n", sts); 
  if (debug && (sts == -1)) perror("send");

  return 1;
}

/*************************************************************************
**************************************************************************
*
* Namn : main
*
* Typ  : unsigned int
*
* Typ		Parameter	       IOGF	Beskrivning
*
* Beskrivning : Main
*
*
**************************************************************************
**************************************************************************/

int main(int argc, char *argv[])
{
  remtrans_item *remtrans;
  unsigned char id[32];
  unsigned char pname[32];

  pwr_tStatus sts;
  struct timeval tv;
  int i;
  
  if (argc >= 4) debug=1;

  /* Read arg number 2, should be id for this instance */

  if (argc >= 2)
    strcpy((char *)id, argv[1]);
  else
    strcpy((char *)id, "0");

  /* Build process name with id */

  sprintf((char *) pname, "rs_remtcp_%s", id);

  /* Init of errh */

  errh_Init((char *) pname, errh_eAnix_remote);
  errh_SetStatus(PWR__SRVSTARTUP);
  
  /* Init of gdh */

  sts = gdh_Init((char *) pname);
  if (debug) printf("Gdh init: %d\n", sts);
  if ( EVEN(sts)) {
    errh_Error("gdh_Init, %m", sts);
    errh_SetStatus(PWR__SRVTERM);
    exit(sts);
  }

  /* Arg number 3 should be my remnodes objid in string representation,
     read it, convert to real objid and store in remnode_item */

  sts = 0;
  if (argc >= 3) sts = cdh_StringToObjid(argv[2], &rn.objid);
  if (debug) printf("StringToObjid: %d\n", sts);
  if ( EVEN(sts)) {
    errh_Error("cdh_StringToObjid, %m", sts);
    errh_SetStatus(PWR__SRVTERM);
    exit(sts);
  }

  
  /* Get pointer to RemnodeTCP object and store locally */
  
  sts = gdh_ObjidToPointer(rn.objid, (pwr_tAddress *) &rn_tcp);
  if (debug) printf("ObjidToPointer: %d\n", sts);
  if ( EVEN(sts)) {
    errh_Error("cdh_ObjidToPointer, %m", sts);
    errh_SetStatus(PWR__SRVTERM);
    exit(sts);
  }

  /* Initialize some internal data and make standard remtrans init */

  rn.next = NULL;
  rn.local = NULL;		// We dont use local structure since we only have one remnode
  rn.retransmit_time = rn_tcp->RetransmitTime;
  rn_tcp->ErrCount = 0;

  sts = RemTrans_Init(&rn);

  if ( EVEN(sts)) {
    errh_Error("RemTrans_Init, %m", sts);
    errh_SetStatus(PWR__SRVTERM);
    exit(sts);
  }
  
  /* Initialize no timeout value (poll) for select call */

  tv.tv_sec = 0;
  tv.tv_usec = 0;

  /* Set mode client or server */
  
  if (rn_tcp->ConnectionMode == 0) 
    cs_mode = TCP_CLIENT;
  else
    cs_mode = TCP_SERVER;

  /* Create TCP socket and init adress structures */
  rn_tcp->LinkUp = 0;

  for (i=0; i<10; i++) {
    sts = CreateSocket();
    if (sts >= 0)
      break;
    else
      RemoteSleep(3);
  }
  if (sts != 0) {
    errh_Error("CreateSocket, %m", sts);
    errh_SetStatus(PWR__SRVTERM);
    exit(sts);
  }

  /* Set (re)start time in remnode object */
  
  clock_gettime(CLOCK_REALTIME, &rn_tcp->RestartTime);
  
  /* Store remtrans objects objid in remnode_tcp object */
  remtrans = rn.remtrans;
  i = 0;
  while(remtrans) {
    rn_tcp->RemTransObjects[i++] = remtrans->objid;
    if ( i >= (int)(sizeof(rn_tcp->RemTransObjects)/sizeof(rn_tcp->RemTransObjects[0])))
      break;
    remtrans = (remtrans_item *) remtrans->next;
  }

  if (cs_mode == TCP_CLIENT) {
    /* Connect, make 10 attempts with 3 seconds interval */
    for (i=0; i<10; i++) {
      sts = Connect();
      if (sts >= 0)
        break;
      else
        RemoteSleep(3);
    }
  }
  else {
    Accept();
  }
  
  /* Connected, Loop forever */

  /* Set running status */
  
  errh_SetStatus(PWR__SRUN);  

  while (!doomsday)
  {
  
    if (rn_tcp->Disable == 1) {
      errh_Fatal("Disabled, exiting");
      errh_SetStatus(PWR__SRVTERM);
      exit(0);
    }   
    
    /* Timestamp */
    
    aproc_TimeStamp();

    RemoteSleep(TIME_INCR);

    /* Increase time counters in local remnode and prevent big counters */
    
    time_since_scan += TIME_INCR;
    time_since_keepalive += TIME_INCR;
    time_since_rcv += TIME_INCR;
    time_since_scan = min(time_since_scan, rn_tcp->ScanTime + 1.0);
    time_since_keepalive = min(time_since_keepalive, rn_tcp->KeepaliveTime + 1.0);
    time_since_rcv = min(time_since_rcv, rn_tcp->LinkTimeout + 1.0);

    /* Update retransmit time, could have been changed */
    
    rn.retransmit_time = rn_tcp->RetransmitTime;

    remtrans = rn.remtrans;
    while(remtrans) {
      remtrans->time_since_send += TIME_INCR;
      /* Prevent big counter */
      remtrans->time_since_send = min(remtrans->time_since_send, rn.retransmit_time + 1.0);
      remtrans = (remtrans_item *) remtrans->next;
    }

    if (rn_tcp->LinkUp == 0) {
      Shutdown();
      if (cs_mode == TCP_CLIENT)
        exit(0);
      else 
        Accept();
    }

    tv.tv_sec = 0;
    tv.tv_usec = 0;

    FD_ZERO(&fdr);
    FD_ZERO(&fde);
    FD_SET(c_socket, &fdr);
    FD_SET(c_socket, &fde);
    
    sts = select(32, &fdr, NULL, &fde, &tv);

    if (sts < 0 && debug) perror("select");

    if (sts < 0) exit(sts);
    
    if (sts > 0) {
      sts = Receive();
      if (sts <= 0 && debug) perror("receive:");
      if (sts < 0) exit(sts);
      if (sts == 0) rn_tcp->LinkUp = 0;
    }
    
    if (rn_tcp->LinkUp == 1) {

      if (time_since_scan >= rn_tcp->ScanTime) {
        if (!rn_tcp->Disable) RemTrans_Cyclic(&rn, &RemnodeSend);
        time_since_scan = 0;
      }

      if (time_since_keepalive >= rn_tcp->KeepaliveTime) {
        if (!rn_tcp->Disable && rn_tcp->UseKeepalive) SendKeepalive();
        time_since_keepalive = 0;
      }

      if (time_since_rcv >= rn_tcp->LinkTimeout) {
        if (rn_tcp->LinkUp) {
          errh_Info("TCP link down %s", rn_tcp->RemoteHostname);
          rn_tcp->LinkUp = 0;
        }
      }
      
    }

  }
}
