/* 
 * Proview   $Id: wb_provider.cpp,v 1.2 2005-09-01 14:57:49 claes Exp $
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

extern "C" {
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <fcntl.h>
#include <string.h>
#include <stdio.h>
}
extern "C" {
#include "pwr.h"
#include "pwr_class.h"
#include "co_cdh.h"
#include "wb_ldh_msg.h"
}

#include "wb_vext.h"
#include "wb_provider.h"

void wb_procom::put( vext_sAMsg *msg, int size, pwr_tStatus *sts)
{
  msg->Any.message_type = 1;
  if ( msgsnd( m_msgsndid, (void *)msg, size, 0) == -1) {
    *sts = LDH__MSGSND;
    return;
  }
  *sts = LDH__SUCCESS;
}

void wb_procom::receive( vext_sQMsg *msg, int size, pwr_tStatus *sts)
{
  if ( !m_connected) {
    key_t key;
    int fd = -1;

    fd = open( m_key, O_RDWR | O_CREAT, 0777);
    if ( fd < 0) {
      *sts = LDH__NOPROV;
      return;
    }
    close( fd);

    key = ftok( m_key, 0);

    m_msgrcvid = msgget( (key_t)key, 0666 | IPC_CREAT);
    if ( m_msgrcvid == -1) {
      *sts = LDH__MSGGET;
      return;
    }
    m_msgsndid = msgget( (key_t)(key + 1), 0666 | IPC_CREAT);
    if ( m_msgsndid == -1) {
      *sts = LDH__MSGGET;
      return;
    }
    m_connected = 1;
  }

  if ( msgrcv( m_msgrcvid, (void *)msg, size, 0, 0) == -1) {
    *sts = LDH__MSGRCV;
    return;
  }
  *sts = LDH__SUCCESS;
}

void wb_procom::provideObject( pwr_tStatus sts, pwr_tOix oix, pwr_tOix fthoix, pwr_tOix bwsoix,
		      pwr_tOix fwsoix, pwr_tOix fchoix, pwr_tOix lchoix, pwr_tCid cid,
		      char *name, char *longname)
{
  vext_sAMsg amsg;
  pwr_tStatus asts;

  amsg.Object.Type = vext_eMsgType_Object;
  amsg.Object.Status = sts;
  amsg.Object.oix = oix;
  amsg.Object.fthoix = fthoix;
  amsg.Object.bwsoix = bwsoix;
  amsg.Object.fwsoix = fwsoix;
  amsg.Object.fchoix = fchoix;
  amsg.Object.lchoix = lchoix;
  amsg.Object.cid = cid;
  if ( ODD(sts)) {
    strcpy( amsg.Object.name, name);
    strcpy( amsg.Object.longname, longname);
  }

  put( &amsg, sizeof(amsg), &asts);
}

void wb_procom::provideBody( pwr_tStatus sts, pwr_tOix oix, int size, void *body)
{
  vext_sAMsg amsg;
  pwr_tStatus asts;

  amsg.ObjectBody.Status = sts;
  amsg.ObjectBody.oix = oix;
  if ( (int)sizeof(amsg.ObjectBody.body) < size)
    size = sizeof(amsg.ObjectBody.body);
  amsg.ObjectBody.size = size;
  if ( ODD(sts))
    memcpy( amsg.ObjectBody.body, body, size);
  put( &amsg, sizeof(amsg), &asts);
}

void wb_procom::mainloop()
{
  vext_sQMsg qmsg;
  pwr_tStatus sts;

  for (;;) {
    receive( &qmsg, sizeof(qmsg), &sts);

    switch( qmsg.Any.Type) {
    case vext_eMsgType_Object:
      printf( "Object\n");
      m_provider->object( this);
      break;
    case vext_eMsgType_ObjectOid:
      printf( "ObjectOid %d\n", qmsg.Oid.Oix);
      m_provider->objectOid( this, qmsg.Oid.Oix);
      break;
    case vext_eMsgType_ObjectBody:
      printf( "ObjectBody %d\n", qmsg.Oid.Oix);
      m_provider->objectBody( this, qmsg.Oid.Oix);
      break;
    case vext_eMsgType_ObjectName:
      printf( "ObjectName %s\n", qmsg.ObjectName.Name);
      m_provider->objectName( this, qmsg.ObjectName.Name);
      break;
    default:
      ;
    }
  }
}


