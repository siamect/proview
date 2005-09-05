/** 
 * Proview   $Id: wb_vrepext.cpp,v 1.3 2005-09-05 08:34:34 claes Exp $
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



#include <iostream>
#include <fstream>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <fcntl.h>

#include "wb_vrepext.h"
#include "wb_orepext.h"
#include "wb_vext.h"
#include "wb_ldh_msg.h"
#include "wb_dbs.h"
#include "wb_bdrep.h"
#include "wb_cdrep.h"
#include "wb_attribute.h"
#include "wb_erep.h"
#include "wb_merep.h"
#include "wb_tdrep.h"
#include "wb_ldh_msg.h"
#include "wb_vrepwbl.h"
#include "wb_vrepref.h"
#include "wb_print_wbl.h"
#include "wb_volume.h"
#include "pwr_baseclasses.h"

extern "C" {
#include "co_dcli.h"
}

wb_vrepext::wb_vrepext( wb_erep *erep, pwr_tVid vid) :
  wb_vrep(vid), m_erep(erep), m_merep(erep->merep()), m_connected(0)
{
  strcpy( m_provider, "");

  m_vid = vid;
  m_cid = pwr_eClass_ExternVolume;
  createVolumeObject("");
}

wb_vrepext::wb_vrepext( wb_erep *erep, pwr_tVid vid, char *name, char *provider) :
  wb_vrep(vid), m_erep(erep), m_merep(erep->merep()), m_connected(0)
{
  strcpy( m_provider, provider);

  m_vid = vid;
  m_cid = pwr_eClass_ExternVolume;
  createVolumeObject( name);
  strcpy( m_name, name);
}

wb_vrepext::~wb_vrepext()
{
}

wb_orep *wb_vrepext::object(pwr_tStatus *sts, pwr_tOid oid)
{
  if ( oid.vid != m_vid) {
    *sts = LDH__BADOBJID;
    return 0;
  }
  
  if ( oid.oix == 0) {
    // Volume object
    *sts = LDH__SUCCESS;
    return new wb_orepext( this, volume_object);
  }

  // Look in cache
  if ( m_cashe.m_oid.oix == oid.oix) {
    wb_orepext *orep = new wb_orepext( this, m_cashe);
    return orep;
  }
    
  vext_sQMsg qmsg;
  vext_sAMsg amsg;

  qmsg.Oid.Type = vext_eMsgType_ObjectOid;
  qmsg.Oid.Oix = oid.oix; 
  put( &qmsg, sizeof(qmsg), sts);
  if ( EVEN(*sts)) return 0;
  receive( &amsg, sizeof(amsg), sts);
  if ( EVEN(*sts)) return 0;

  if ( ODD( amsg.Object.Status)) {
    *sts = LDH__SUCCESS;
    ext_object exto( &amsg.Object, m_vid);
    m_cashe = exto;
    wb_orepext *orep = new wb_orepext( this, exto);
    return orep;
  }
  else {
    *sts = amsg.Object.Status;
    return 0;
  }
}

wb_orep *wb_vrepext::object(pwr_tStatus *sts, pwr_tCid cid)
{
  *sts = LDH__NOSUCHOBJ;
  return 0;
}

wb_orep *wb_vrepext::object(pwr_tStatus *sts, wb_name &name)
{
  if ( name.hasVolume() && !name.hasObject()) {
    // Volume object
    *sts = LDH__SUCCESS;
    return new wb_orepext( this, volume_object);
  }

  vext_sQMsg qmsg;
  vext_sAMsg amsg;

  qmsg.ObjectName.Type = vext_eMsgType_ObjectName;
  strcpy( qmsg.ObjectName.Name, name.name(cdh_mName_path | cdh_mName_object));
  put( &qmsg, sizeof(qmsg), sts);
  if ( EVEN(*sts)) return 0;
  receive( &amsg, sizeof(amsg), sts);
  if ( EVEN(*sts)) return 0;

  if ( ODD( amsg.Object.Status)) {
    *sts = LDH__SUCCESS;
    ext_object exto( &amsg.Object, m_vid);
    wb_orepext *orep = new wb_orepext( this, exto);
    return orep;
  }
  else {
    *sts = amsg.Object.Status;
    return 0;
  }
}

wb_vrep *wb_vrepext::next()
{
  pwr_tStatus sts;

  return m_erep->nextVolume( &sts, vid());
}

void wb_vrepext::info()
{

  cout << "Volume : " << volume_name << " " << volume_class << " " << m_vid << endl;

}

void wb_vrepext::unref()
{
  if (--m_nRef == 0)
    delete this;
}

wb_vrep *wb_vrepext::ref()
{
  m_nRef++;
  return this;
}

wb_orep *wb_vrepext::object(pwr_tStatus *sts)
{
  vext_sQMsg qmsg;
  vext_sAMsg amsg;

  qmsg.Any.Type = vext_eMsgType_Object;
  put( &qmsg, sizeof(qmsg), sts);
  if ( EVEN(*sts)) return 0;
  receive( &amsg, sizeof(amsg), sts);
  if ( EVEN(*sts)) return 0;

  if ( ODD( amsg.Object.Status)) {
    *sts = LDH__SUCCESS;
    ext_object exto( &amsg.Object, m_vid);
    wb_orepext *orep = new wb_orepext( this, exto);
    return orep;
  }
  else {
    *sts = amsg.Object.Status;
    return 0;
  }
}

wb_orep *wb_vrepext::ancestor(pwr_tStatus *sts, const wb_orep *o)
{
  wb_orepext *orep = 0;
  *sts = LDH__NOSUCHOBJ;
  return orep;
}

wb_orep *wb_vrepext::parent(pwr_tStatus *sts, const wb_orep *o)
{
  if ( o->poid().oix == 0) {
    *sts = LDH__NOSUCHOBJ;
    return 0;
  }
  return object( sts, o->poid());
}

wb_orep *wb_vrepext::after(pwr_tStatus *sts, const wb_orep *o)
{
  if ( o->aoid().oix == 0) {
    *sts = LDH__NO_SIBLING;
    return 0;
  }
  return object( sts, o->aoid());
}

wb_orep *wb_vrepext::before(pwr_tStatus *sts, const wb_orep *o)
{
  if ( o->boid().oix == 0) {
    *sts = LDH__NO_SIBLING;
    return 0;
  }
  return object( sts, o->boid());
}

wb_orep *wb_vrepext::first(pwr_tStatus *sts, const wb_orep *o)
{
  if ( o->foid().oix == 0) {
    *sts = LDH__NO_CHILD;
    return 0;
  }
  return object( sts, o->foid());
}

wb_orep *wb_vrepext::child(pwr_tStatus *sts, const wb_orep *o, wb_name &name)
{
  wb_orepext *orep = 0;
  *sts = LDH__NO_CHILD;  
  return orep;
}

wb_orep *wb_vrepext::last(pwr_tStatus *sts, const wb_orep *o)
{
  if ( o->loid().oix == 0) {
    *sts = LDH__NO_CHILD;
    return 0;
  }
  return object( sts, o->loid());
}

wb_orep *wb_vrepext::next(pwr_tStatus *sts, const wb_orep *o)
{
  *sts = LDH__NOSUCHOBJ;
  return 0;
}

wb_orep *wb_vrepext::previous(pwr_tStatus *sts, const wb_orep *o)
{
  return 0;
}

void wb_vrepext::objectName(const wb_orep *o, char *str)
{
  *str = 0;
}

bool wb_vrepext::writeAttribute(pwr_tStatus *sts, wb_orep *o, pwr_eBix bix, size_t offset, size_t size, void *p)
{
  *sts = LDH__SUCCESS;
  return false;
}

void *wb_vrepext::readAttribute(pwr_tStatus *sts, const wb_orep *o, pwr_eBix bix, size_t offset, size_t size, void *p)
{
  void *body = readBody( sts, o, bix, 0);
  if ( EVEN(*sts))
    return 0;

  if ( p) {
    memcpy( p, (char *)body + offset, size);
    return p;
  }
  return (void *)((char *)body + offset);
}

bool wb_vrepext::writeBody(pwr_tStatus *sts, wb_orep *o, pwr_eBix bix, void *p)
{
  *sts = LDH__NOSUCHBODY;
  return false;
}

void *wb_vrepext::readBody(pwr_tStatus *sts, const wb_orep *o, pwr_eBix bix, void *p)
{
  vext_sQMsg qmsg;
  static vext_sAMsg amsg;

  if ( bix != pwr_eBix_rt) {
    *sts = LDH__NOSUCHBODY;
    return 0;
  }

  qmsg.Oid.Type = vext_eMsgType_ObjectBody;
  qmsg.Oid.Oix = o->oid().oix;
  put( &qmsg, sizeof(qmsg), sts);
  if ( EVEN(*sts)) return 0;
  receive( &amsg, sizeof(amsg), sts);
  if ( EVEN(*sts)) return 0;

  if ( ODD( amsg.ObjectBody.Status)) {
    *sts = LDH__SUCCESS;
    if ( p) {
      memcpy( p, &amsg.ObjectBody.body, amsg.ObjectBody.size);
      return p;
    }
    return &amsg.ObjectBody.body;
  }
  *sts = LDH__NOSUCHBODY;
  return 0;
}


wb_orep *wb_vrepext::createObject(pwr_tStatus *sts, wb_cdef cdef, wb_destination &d, wb_name &name)
{
  return 0;
}

bool wb_vrepext::createVolumeObject( char *name)
{
  pwr_tStatus sts;

  wb_cdrep *cdrep = m_merep->cdrep( &sts, m_cid);
  wb_cdef cdef = wb_cdef( cdrep);

  ext_object exto;
  strcpy( exto.m_name, name);
  exto.m_oid.vid = m_vid;
  exto.m_oid.oix = 0;
  exto.m_cid = m_cid;
  exto.m_flags = cdef.flags();
  exto.rbody_size = cdef.size( pwr_eBix_rt);
  if ( exto.rbody_size) {
    exto.rbody = malloc( exto.rbody_size);
    cdef.templateBody( &sts, pwr_eBix_rt, exto.rbody, exto.m_oid);
    if ( EVEN(sts)) return false;
  }
  exto.dbody_size = cdef.size( pwr_eBix_dev);
  if ( exto.dbody_size) {
    exto.dbody = malloc( exto.dbody_size);
    cdef.templateBody( &sts, pwr_eBix_dev, exto.dbody, exto.m_oid);
    if ( EVEN(sts)) return false;
  }

  volume_object = exto;
  strcpy( volume_name, name);
  return true;
}


wb_orep *wb_vrepext::copyObject(pwr_tStatus *sts, const wb_orep *orep, wb_destination &d, wb_name &name)
{
  return 0;
}

bool wb_vrepext::moveObject(pwr_tStatus *sts, wb_orep *orep, wb_destination &d)
{
  return true;
}

bool wb_vrepext::deleteObject(pwr_tStatus *sts, wb_orep *orep)
{
  return true;
}

bool wb_vrepext::deleteFamily(pwr_tStatus *sts, wb_orep *orep)
{
  return true;
}

bool wb_vrepext::renameObject(pwr_tStatus *sts, wb_orep *orep, wb_name &name) 
{
  *sts = LDH__SUCCESS;
  return true;
}

bool wb_vrepext::commit(pwr_tStatus *sts) 
{
  return true;
}

bool wb_vrepext::abort(pwr_tStatus *sts) 
{
  return true;
}

void wb_vrepext::put( vext_sQMsg *msg, int size, pwr_tStatus *sts)
{
  if ( !m_connected) {
    vext_sAMsg amsg;
    int fd = -1;
    key_t key;
    
    fd = open( m_provider, O_RDWR | O_CREAT, 0777);
    if ( fd < 0) {
      *sts = LDH__NOPROV;
      return;
    }
    close( fd);

    key = ftok( m_provider, 0);
    m_msgsndid = msgget( key, 0666 /* | IPC_CREAT */);
    if ( m_msgsndid == -1) {
      *sts = LDH__MSGGET;
      return;
    }

    m_msgrcvid = msgget( (key_t)(key + 1), 0666 /* | IPC_CREAT */);
    if ( m_msgrcvid == -1) {
      *sts = LDH__MSGGET;
      return;
    }
    // Clear the receive que
    while( msgrcv( m_msgrcvid, (void *) &amsg, sizeof(amsg), 0, IPC_NOWAIT) != -1) ;

    m_connected = 1;
  }

  msg->Any.message_type = 1;
  if ( msgsnd( m_msgsndid, (void *)msg, size, 0) == -1) {
    *sts = LDH__MSGSND;
    return;
  }
  *sts = LDH__SUCCESS;
}

void wb_vrepext::receive( vext_sAMsg *msg, int size, pwr_tStatus *sts)
{
  if ( msgrcv( m_msgrcvid, (void *)msg, size, 0, 0) == -1) {
    *sts = LDH__MSGRCV;
    return;
  }
  *sts = LDH__SUCCESS;
}



