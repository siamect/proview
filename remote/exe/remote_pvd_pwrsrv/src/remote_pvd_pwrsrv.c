/* 
 * Proview   $Id: remote_pvd_pwrsrv.c,v 1.4 2007-06-13 14:35:06 claes Exp $
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
 **/

#include <string.h>
#include <stdio.h>
#include "pwr.h"
#include "pwr_class.h"
#include "pwr_baseclasses.h"
#include "remote_pvd_udp.h"
#include "rt_gdh.h"
#include "rt_gdh_msg.h"
#include "rs_remote_msg.h"

#include "co_cdh.h"
#include "co_dcli.h"

typedef struct sSubItem {
  int			rix;
  pwr_tObjid		oid;
  pwr_tObjName		attribute;
  int			size;
  void			*p;
  pwr_tRefId		dlid;
  struct sSubItem 	*next;
  struct sSubItem 	*prev;
} pwrsrv_sSubItem;

static pwrsrv_sSubItem *pwrsrv_sublist = 0;

static void pwrsrv_Oid( rpvd_sMsgOid *msg)
{
  rpvd_sMsgObject rmsg;
  pwr_tOid fth;
  pwr_tOid fch;
  pwr_tOid lch;
  pwr_tOid bws;
  pwr_tOid fws;
  pwr_tCid cid;
  pwr_tObjName name;
  pwr_tOid o;
  pwr_tStatus sts;
  pwr_tOid plist[200];
  int psize = 0;
  int i, j;
  char *s;

  rmsg.Type = rpvd_eMsg_Object;
  rmsg.Id = msg->Id;
  rmsg.Status = 1;
  rmsg.Oid = msg->Oid;

  sts = gdh_GetObjectClass( msg->Oid, &cid);
  if ( EVEN(sts)) {
    rmsg.Status = sts;
    udp_Send( (char *)&rmsg, sizeof(rmsg));
    return;
  }

  plist[psize++] = msg->Oid;
  for ( sts = gdh_GetParent( plist[psize-1], &o); 
	ODD(sts);
	sts = gdh_GetParent( plist[psize-1], &o)) {
    plist[psize++] = o;
  }
  /* Add volume object */
  plist[psize].oix = 0;
  plist[psize++].vid = msg->Oid.vid;

  for ( j = 0; j < psize; j++) {
    i = psize - j - 1;

    sts = gdh_GetObjectClass( plist[i], &cid);

    if ( i == psize - 1) {
      /* Volume object */
      fth.oix = 0;

      sts = gdh_GetRootList( &fch);
      if ( EVEN(sts)) {
	fch.oix = 0;
	lch.oix = 0;
      }
      else {
	sts = 1;
	for ( lch = fch; ODD(sts); sts = gdh_GetNextSibling( lch, &o)) {
	  lch = o;
	}
      }
      fws.oix = 0;
      bws.oix = 0;

      sts = gdh_ObjidToName( plist[i], name, sizeof(name), cdh_mName_volume);
      if ( (s = strrchr( name, ':')))
	*s = 0;
    }
    else {
      sts = gdh_GetParent( plist[i], &fth);
      if ( EVEN(sts)) 
	fth.oix = 0;
      sts = gdh_GetChild( plist[i], &fch);
      if ( EVEN(sts)) {
	fch.oix = 0;
	lch.oix = 0;
      }
      else {
	lch = fch;
	for ( sts = gdh_GetNextSibling( lch, &o); 
	      ODD(sts); 
	      sts = gdh_GetNextSibling( lch, &o)) {
	  lch = o;
	}
      }
      sts = gdh_GetNextSibling( plist[i], &fws);
      if ( EVEN(sts))
	fws.oix = rmsg.o[j-1].fchoix;
      sts = gdh_GetPreviousSibling( plist[i], &bws);
      if ( EVEN(sts))
	bws.oix = rmsg.o[j-1].lchoix;
      sts = gdh_ObjidToName( plist[i], name, sizeof(name), cdh_mName_object);
    }
    rmsg.o[j].oix = plist[i].oix;
    rmsg.o[j].fthoix = fth.oix;
    rmsg.o[j].fchoix = fch.oix;
    rmsg.o[j].lchoix = lch.oix;
    rmsg.o[j].fwsoix = fws.oix;
    rmsg.o[j].bwsoix = bws.oix;
    rmsg.o[j].cid = cid;
    strcpy( rmsg.o[j].name, name);
  }
  rmsg.OSize = psize;
  udp_Send( (char *)&rmsg, sizeof(rmsg));
}

static void pwrsrv_ObjectName( rpvd_sMsgObjectName *msg)
{
  rpvd_sMsgOid m;
  pwr_tStatus sts;
  char name[256];

  /* Simulate an oid request */
  m.Type = rpvd_eMsg_Oid;
  m.Id = msg->Id;
  if ( msg->POid.oix) {
    sts = gdh_ObjidToName( msg->POid, name, sizeof(name), cdh_mName_volumeStrict);
    if ( EVEN(sts)) {
      rpvd_sMsgObject rmsg;
      rmsg.Type = rpvd_eMsg_Object;
      rmsg.Id = msg->Id;
      rmsg.Status = sts;
      udp_Send( (char *)&rmsg, sizeof(rmsg));
      return;
    }
    strcat( name, "-");
    strcat( name, msg->Name);
  }
  else
    strncpy( name, msg->Name, sizeof(name));
  sts = gdh_NameToObjid( name, &m.Oid);
  if ( EVEN(sts)) {
    rpvd_sMsgObject rmsg;
    rmsg.Type = rpvd_eMsg_Object;
    rmsg.Id = msg->Id;
    rmsg.Status = sts;
    udp_Send( (char *)&rmsg, sizeof(rmsg));
    return;
  }
  
  pwrsrv_Oid( &m);
}

static void pwrsrv_ReadAttribute( rpvd_sMsgReadAttribute *msg)
{
  rpvd_sMsgAttribute rmsg;
  pwr_tAName name;
  pwr_tUInt32 asize, aoffs, aelem;
  pwr_tTypeId atid;
  pwr_tStatus sts;
  
  rmsg.Type = rpvd_eMsg_Attribute;
  rmsg.Id = msg->Id;
  rmsg.Status = 1;
  rmsg.Oid = msg->Oid;
  strcpy( rmsg.Attribute, msg->Attribute);

  sts = gdh_ObjidToName( msg->Oid, name, sizeof(name), cdh_mName_volumeStrict);
  if ( EVEN(sts)) {
    rmsg.Status = sts;
    udp_Send( (char *)&rmsg, sizeof(rmsg));
    return;
  }

  strcat( name, ".");
  strcat( name, msg->Attribute);

  sts = gdh_GetAttributeCharacteristics( name, &atid, &asize, &aoffs, &aelem);
  if ( EVEN(sts)) {
    rmsg.Status = sts;
    udp_Send( (char *)&rmsg, sizeof(rmsg));
    return;
  }

  sts = gdh_GetObjectInfo( name, &rmsg.Value, asize);
  rmsg.Status = sts;
  rmsg.Size = asize;
  rmsg.Tid = atid;
  udp_Send( (char *)&rmsg, sizeof(rmsg));
}

static void pwrsrv_WriteAttribute( rpvd_sMsgWriteAttribute *msg)
{
  rpvd_sMsgAny rmsg;
  pwr_tAName name;
  pwr_tUInt32 asize, aoffs, aelem;
  pwr_tTypeId atid;
  pwr_tStatus sts;
  
  rmsg.Type = rpvd_eMsg_Status;
  rmsg.Id = msg->Id;

  sts = gdh_ObjidToName( msg->Oid, name, sizeof(name), cdh_mName_volumeStrict);
  if ( EVEN(sts)) {
    rmsg.Status = sts;
    udp_Send( (char *)&rmsg, sizeof(rmsg));
    return;
  }

  strcat( name, ".");
  strcat( name, msg->Attribute);

  sts = gdh_GetAttributeCharacteristics( name, &atid, &asize, &aoffs, &aelem);
  if ( EVEN(sts)) {
    rmsg.Status = sts;
    udp_Send( (char *)&rmsg, sizeof(rmsg));
    return;
  }

  sts = gdh_SetObjectInfo( name, &msg->Value, msg->Size);
  rmsg.Status = sts;
  udp_Send( (char *)&rmsg, sizeof(rmsg));
}

static void pwrsrv_SubAdd( rpvd_sMsgSubAdd *msg)
{
  rpvd_sMsgAny rmsg;
  pwr_tAName name;
  pwr_tUInt32 asize, aoffs, aelem;
  pwr_tTypeId atid;
  pwr_tStatus sts;
  pwrsrv_sSubItem *si;
  void *p;
  pwr_tRefId dlid;
  
  rmsg.Type = rpvd_eMsg_Status;
  rmsg.Id = msg->Id;

  /* Check that this rix doesn't exist */
  for ( si = pwrsrv_sublist; si; si = si->next) {
    if ( si->rix == msg->Rix) {
      return;
    }
  }

  sts = gdh_ObjidToName( msg->Oid, name, sizeof(name), cdh_mName_volumeStrict);
  if ( EVEN(sts)) {
    rmsg.Status = sts;
    udp_Send( (char *)&rmsg, sizeof(rmsg));
    return;
  }

  if ( strcmp( msg->Attribute, "") != 0) {
    strcat( name, ".");
    strcat( name, msg->Attribute);

    sts = gdh_GetAttributeCharacteristics( name, &atid, &asize, &aoffs, &aelem);
    if ( EVEN(sts)) {
      rmsg.Status = sts;
      udp_Send( (char *)&rmsg, sizeof(rmsg));
      return;
    }
  }
  else {
    sts = gdh_GetObjectSize( msg->Oid, &asize);
    if ( EVEN(sts)) {
      rmsg.Status = sts;
      udp_Send( (char *)&rmsg, sizeof(rmsg));
      return;
    }
  }

  /* Direct link to atttribute */
  sts = gdh_RefObjectInfo( name, &p, &dlid, asize);
  if ( EVEN(sts)) {
    rmsg.Status = sts;
    udp_Send( (char *)&rmsg, sizeof(rmsg));
    return;
  }
       
  /* Insert first in sublist */
  si = (pwrsrv_sSubItem *) calloc( 1, sizeof(pwrsrv_sSubItem));
  si->oid = msg->Oid;
  strcpy( si->attribute, msg->Attribute);
  si->size = asize;
  si->rix = msg->Rix;
  si->p = p;
  si->dlid = dlid;
       
  si->next = pwrsrv_sublist;
  if ( si->next)
    si->next->prev = si;
  pwrsrv_sublist = si;

  
  printf( "SubAdd: %d %s\n", msg->Rix, name);

  rmsg.Status = GDH__SUCCESS;
  udp_Send( (char *)&rmsg, sizeof(rmsg));
}

static void pwrsrv_SubRemove( rpvd_sMsgSubRemove *msg)
{
  rpvd_sMsgAny rmsg;
  pwrsrv_sSubItem *si;
  
  rmsg.Type = rpvd_eMsg_Status;
  rmsg.Id = msg->Id;

  // Remove and unref
  for ( si = pwrsrv_sublist; si; si = si->next) {
    if ( si->rix == msg->Rix) {
      gdh_UnrefObjectInfo( si->dlid);
      if ( si->prev)
	si->prev->next = si->next;
      else
	pwrsrv_sublist = si->next;
      if ( si->next)
	si->next->prev = si->prev;
      free( (char *)si);
      break;
    }
  }
  printf( "SubRemove: %d\n", msg->Rix);

  rmsg.Status = GDH__SUCCESS;
  udp_Send( (char *)&rmsg, sizeof(rmsg));
}

static void pwrsrv_SubSend( rpvd_sMsgAny *m)
{
  rpvd_sMsgSubSend msg;
  rpvd_sMsgAny rmsg;
  pwrsrv_sSubItem *si;
  char *subp;
  int msize;
  pwr_tStatus sts;

  msg.Type = rpvd_eMsg_SubSend;
  msg.Id = m->Id;
  msg.Status = GDH__SUCCESS;
  msg.More = 0;

  subp = (char *) &msg.Data;

  for ( si = pwrsrv_sublist; si; si = si->next) {
    if ( subp - (char *)&msg.Data + 8 + si->size + 4 > sizeof(msg.Data)) {
      /* Buffer is full, send */
      *(int *)subp = -1;

      msg.More = 1;
      msize = subp - (char *)&msg + 4;
      sts = udp_Send( (char *)&msg, msize);
      if ( EVEN(sts)) return;

      sts = udp_Receive( (char **)&rmsg, 1000);
      if ( EVEN(sts) || sts == REM__TIMEOUT)
	return;
      subp = (char *) &msg.Data;
      msg.More = 0;
    }
    *(int *)subp = si->rix;
    subp += 4;
    *(int *)subp = si->size;
    subp += 4;
    if ( si->p)
      memcpy( subp, si->p, si->size);
    subp += si->size;
  }
  *(int *)subp = -1;

  msize = subp - (char *)&msg + 4;
  udp_Send( (char *)&msg, msize);
}

static void pwrsrv_NodeUp( rpvd_sMsgAny *msg)
{
  rpvd_sMsgAny rmsg;
  pwrsrv_sSubItem *si, *si_next;

  rmsg.Type = rpvd_eMsg_Status;
  rmsg.Id = msg->Id;
  rmsg.Status = GDH__SUCCESS;

  /* Remove all subcli */
  for ( si = pwrsrv_sublist; si; si = si_next) {
    si_next = si->next;
    free( (char *) si);
  }
  pwrsrv_sublist = 0;
 
  udp_Send( (char *)&rmsg, sizeof(rmsg));
}

void usage()
{
  printf( "remote_pvd_pwrsrv   Proview provider server\n\n");
  printf(   "Arguments: \n");
  printf(   "  1   Remote address\n");
  printf(   "  2   Remote host name\n");
  printf(   "  3   Udp port number (optional)\n\n");
}

int main(int argc, char *argv[])
{
  pwr_tStatus sts;
  char remote_address[40];
  char remote_host_name[40];
  rpvd_sMsg *msg;
  rpvd_sMsgAny umsg;
  rpvd_sMsgAny *rmsg;
  int udp_port;

  /* Read arguments */
  if ( argc < 3) {
    usage();
    exit(0);
  }
  strcpy( remote_address, argv[1]);
  strcpy( remote_host_name, argv[2]);

  if ( argc >= 4) {
    sts = sscanf( argv[3], "%d", &udp_port);
    if ( sts != 1) {
      usage();
      exit(0);
    } 
  }
  else
    udp_port = 3051;

  sts = udp_Init( remote_address, remote_host_name, udp_port);
  if ( EVEN(sts)) {
    exit(0);
  }

  sts = gdh_Init( "remote_pvd_pwrsrv");
  if ( EVEN(sts)) return sts;

  umsg.Type = rpvd_eMsg_NodeUp;
  umsg.Id = 0;
  sts = udp_Request( (char *)&umsg, sizeof(umsg), (char **)&rmsg);
  if ( ODD(sts) && sts != REM__TIMEOUT && ODD(rmsg->Status))
    printf( "Link up\n");
  
  for (;;) {
    sts = udp_Receive( (char **)&msg, 5000);
    if ( sts == REM__TIMEOUT) {
      printf( "Alive\n");
    }
    else if ( ODD(sts)) {
      switch ( msg->Any.Type) {
      case rpvd_eMsg_Oid:
	pwrsrv_Oid( &msg->Oid);
	break;
      case rpvd_eMsg_ObjectName:
	pwrsrv_ObjectName( &msg->ObjectName);
	break;
      case rpvd_eMsg_ReadAttribute:
	pwrsrv_ReadAttribute( &msg->ReadAttribute);
	break;
      case rpvd_eMsg_WriteAttribute:
	pwrsrv_WriteAttribute( &msg->WriteAttribute);
	break;
      case rpvd_eMsg_SubAdd:
	pwrsrv_SubAdd( &msg->SubAdd);
	break;
      case rpvd_eMsg_SubRemove:
	pwrsrv_SubRemove( &msg->SubRemove);
	break;
      case rpvd_eMsg_SubRequest:
	pwrsrv_SubSend( &msg->Any);
	break;
      case rpvd_eMsg_NodeUp:
	pwrsrv_NodeUp( &msg->Any);
	break;
      }
    }
  }
}


