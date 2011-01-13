/* 
 * Proview   $Id: remote_pvd_pwrcli.cpp,v 1.4 2008-10-31 12:51:30 claes Exp $
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

#include <vector>
#include <map>
#include <string.h>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include "pwr.h"
#include "pwr_class.h"
#include "pwr_baseclasses.h"
#include "wb_vext.h"
#include "remote_pvd_pwrcli.h"
#include "remote_pvd_udp.h"
#include "rt_procom.h"
#include "rt_gdh.h"
#include "rt_gdh_msg.h"
#include "rs_remote_msg.h"

extern "C" {
#include "remote_ndc.h"
#include "rt_vol.h"
#include "rt_gdb.h"
#include "co_cdh.h"
#include "co_dcli.h"
#include "co_time.h"
}

#define max(Dragon,Eagle) ((Dragon) > (Eagle) ? (Dragon) : (Eagle))

class subitem {
 public:
  int m_size;
  void *m_p;
  pwr_tCid m_cid;
  int m_offset;
  pwr_tOName m_attr;
  pwr_tOid m_oid;
  subitem() : m_size(0), m_p(0), m_cid(0), m_offset(0) {
    m_oid = pwr_cNObjid;
    strcpy( m_attr,"");
  }
  subitem( int size, pwr_tCid cid, int offset, char *attr, pwr_tOid oid) : 
    m_size(size), m_cid(cid), m_offset(offset), m_oid(oid) {
    strcpy( m_attr, attr);
    m_p = calloc( 1, size);
  }
  ~subitem() {
    if ( m_p)
      free( m_p);
  }
  subitem( const subitem& x) : m_size(x.m_size), m_p(0), m_cid(x.m_cid), 
			       m_offset(x.m_offset), m_oid(x.m_oid) {
    strcpy( m_attr, x.m_attr);
    if ( x.m_p) {
      m_p = malloc( m_size);
      memcpy( m_p, x.m_p, m_size);
    }
  }
  subitem& operator=(const subitem& x) {
    m_size = x.m_size;
    m_cid = x.m_cid;
    m_offset = x.m_offset;
    strcpy( m_attr, x.m_attr);
    m_oid = x.m_oid;
    if ( x.m_p) {
      m_p = malloc( m_size);
      memcpy( m_p, x.m_p, m_size);
    }
    else
      m_p = 0;
    return *this;
  }
};

typedef map<int, subitem>::iterator sublist_iterator;
static pwr_tVid rpvd_vid;
static char rpvd_vname[32];
static int rpvd_opsys;
static map<int, subitem> rpvd_sublist;
static int rpvd_id = 1;
const bool pvd_cLog = false;

static void logg( const char *logger, pwr_tOix oix, const char *text)
{
  char nowstr[40];

  time_AtoAscii( 0, time_eFormat_Time, nowstr, sizeof(nowstr));
  printf( "%15s %30s %10d %s\n", nowstr, logger, oix, text);
}

void remote_pvd_pwrcli::objectOid( co_procom *pcom, pwr_tOix oix)
{
  rpvd_sMsgOid msg;
  rpvd_sMsgObject *rmsg;
  pwr_tStatus sts;

  msg.Type = rpvd_eMsg_Oid;
  msg.Id = rpvd_id++;
  msg.Oid.oix = oix;
  msg.Oid.vid = rpvd_vid;

  if ( pvd_cLog) logg( "ObjOid", oix, "");

  sts = udp_Request( (char *)&msg, sizeof(msg), (char **)&rmsg);
  if ( pvd_cLog) logg( "Reply", sts, "");
  if ( EVEN(sts)) {
    pcom->provideStatus( sts);
    return;
  }
  if ( sts == REM__TIMEOUT) {
    pcom->provideStatus( REM__UDPNOCON);
    return;
  }
  while ( rmsg->Id != msg.Id) {
    dispatch( pcom, (rpvd_sMsg *)rmsg);

    sts = udp_Receive( (char **)&rmsg, 1000);
    if ( sts == REM__TIMEOUT) {
      pcom->provideStatus( REM__DISORDER);
      return;
    }
  }
  if ( rmsg->Type != rpvd_eMsg_Object) {
    pcom->provideStatus( REM__DISORDER);
    return;
  }
  if ( EVEN( rmsg->Status)) {
    pcom->provideStatus( rmsg->Status);
    return;
  }

  vector<procom_obj> m_list;
  for ( int i = 0; i < rmsg->OSize; i++) {
    procom_obj item;
      
    item.oix = rmsg->o[i].oix;
    item.fthoix = rmsg->o[i].fthoix;
    item.fchoix = rmsg->o[i].fchoix;
    item.lchoix = rmsg->o[i].lchoix;
    item.fwsoix = rmsg->o[i].fwsoix;
    item.bwsoix = rmsg->o[i].bwsoix;
    item.cid = rmsg->o[i].cid;
    strcpy( item.name, rmsg->o[i].name);
    m_list.push_back(item);
  }
  pcom->provideObjects( GDH__SUCCESS, m_list);
}

void remote_pvd_pwrcli::objectName( co_procom *pcom, char *name, pwr_tOix poix)
{
  rpvd_sMsgObjectName msg;
  rpvd_sMsgObject *rmsg;
  pwr_tStatus sts;

  msg.Type = rpvd_eMsg_ObjectName;
  msg.Id = rpvd_id++;
  strcpy( msg.Name, name);
  msg.POid.vid = rpvd_vid;
  msg.POid.oix = poix;
  
  if ( pvd_cLog) logg( "ObjName", poix, name);

  sts = udp_Request( (char *)&msg, sizeof(msg), (char **)&rmsg);
  if ( EVEN(sts)) {
    pcom->provideStatus( sts);
    return;
  }
  if ( sts == REM__TIMEOUT) {
    pcom->provideStatus( REM__UDPNOCON);
    return;
  }
  while ( rmsg->Id != msg.Id) {
    dispatch( pcom, (rpvd_sMsg *)rmsg);

    sts = udp_Receive( (char **)&rmsg, 1000);
    if ( sts == REM__TIMEOUT) {
      pcom->provideStatus( REM__DISORDER);
      return;
    }
  }
  if ( rmsg->Type != rpvd_eMsg_Object) {
    pcom->provideStatus( REM__DISORDER);
    return;
  }
  if ( EVEN( rmsg->Status)) {
    pcom->provideStatus( rmsg->Status);
    return;
  }

  vector<procom_obj> m_list;
  for ( int i = 0; i < rmsg->OSize; i++) {
    procom_obj item;
      
    item.oix = rmsg->o[i].oix;
    item.fthoix = rmsg->o[i].fthoix;
    item.fchoix = rmsg->o[i].fchoix;
    item.lchoix = rmsg->o[i].lchoix;
    item.fwsoix = rmsg->o[i].fwsoix;
    item.bwsoix = rmsg->o[i].bwsoix;
    item.cid = rmsg->o[i].cid;
    strcpy( item.name, rmsg->o[i].name);
    m_list.push_back(item);
  }
  pcom->provideObjects( GDH__SUCCESS, m_list);
}

void remote_pvd_pwrcli::writeAttribute( co_procom *pcom, pwr_tOix oix, 
					unsigned int offset,
					unsigned int size, char *buffer)
{
  rpvd_sMsgWriteAttribute msg;
  rpvd_sMsgAny *rmsg;
  pwr_tStatus sts;
  pwr_tAttrRef aref;
  pwr_tOName aname;
  pwr_tCid cid;

  msg.Type = rpvd_eMsg_WriteAttribute;
  msg.Id = rpvd_id++;
  msg.Oid.oix = oix;
  msg.Oid.vid = rpvd_vid;
  msg.Size = size;
  
  memset( &aref, 0, sizeof(aref));
  aref.Objid = msg.Oid;
  aref.Offset = offset;
  aref.Size = size;

  sts = gdh_GetObjectClass( msg.Oid, &cid);
  if ( EVEN(sts)) {
    pcom->provideStatus( sts);
    return;
  }

  sts = gdh_ClassAttrrefToAttr( cid, &aref, aname, sizeof(aname));
  if ( EVEN(sts)) {
    pcom->provideStatus( sts);
    return;
  }

  strcpy( msg.Attribute, aname);
  // TODO Float conversion
  if ( rpvd_opsys == 0 || rpvd_opsys == gdbroot->my_node->os)
    memcpy( &msg.Value, buffer, size);
  else {
    gdb_sNode 		n;
    gdb_sClass		*cp;
    int			rsize;

    memset( &n, 0, sizeof(n));
    n.os = (co_eOS) rpvd_opsys;

    rsize = aref.Size;
    cp = (gdb_sClass *) hash_Search(&sts, gdbroot->cid_ht, &cid);
    if (cp != NULL) {
      rndc_ConvertData(&sts, &n, cp, &msg.Value, buffer, (pwr_tUInt32 *)&rsize,
		      ndc_eOp_decode, aref.Offset, 0);
    }
  }

  if ( pvd_cLog) logg( "Write", aref.Objid.oix, aname);

  sts = udp_Request( (char *)&msg, sizeof(msg), (char **)&rmsg);
  if ( EVEN(sts)) {
    pcom->provideStatus( sts);
    return;
  }
  if ( sts == REM__TIMEOUT) {
    pcom->provideStatus( REM__UDPNOCON);
    return;
  }
  
  while ( rmsg->Id != msg.Id) {
    dispatch( pcom, (rpvd_sMsg *)rmsg);

    sts = udp_Receive( (char **)&rmsg, 1000);
    if ( sts == REM__TIMEOUT) {
      pcom->provideStatus( REM__DISORDER);
      return;
    }
  }
  if ( rmsg->Type != rpvd_eMsg_Status) {
    pcom->provideStatus( REM__DISORDER);
    return;
  }
  pcom->provideStatus( rmsg->Status);
}

void remote_pvd_pwrcli::readAttribute( co_procom *pcom, pwr_tOix oix, 
				       unsigned int offset, unsigned int size)
{
  rpvd_sMsgReadAttribute msg;
  rpvd_sMsgAttribute *rmsg;
  pwr_tStatus sts;
  pwr_tAttrRef aref;
  pwr_tOName aname;
  pwr_tCid cid;

  msg.Type = rpvd_eMsg_ReadAttribute;
  msg.Id = rpvd_id++;
  msg.Oid.oix = oix;
  msg.Oid.vid = rpvd_vid;
  
  memset( &aref, 0, sizeof(aref));
  aref.Objid = msg.Oid;
  aref.Offset = offset;
  aref.Size = size;

  sts = gdh_GetObjectClass( msg.Oid, &cid);
  if ( EVEN(sts)) {
    pcom->provideStatus( sts);
    return;
  }

  sts = gdh_ClassAttrrefToAttr( cid, &aref, aname, sizeof(aname));
  if ( EVEN(sts)) {
    pcom->provideStatus( sts);
    return;
  }

  strcpy( msg.Attribute, aname);

  if ( pvd_cLog) logg( "Read", aref.Objid.oix, aname);

  sts = udp_Request( (char *)&msg, sizeof(msg), (char **)&rmsg);
  if ( pvd_cLog) logg( "Reply", sts, "");
  if ( EVEN(sts)) {
    pcom->provideStatus( sts);
    return;
  }
  if ( sts == REM__TIMEOUT) {
    pcom->provideStatus( REM__UDPNOCON);
    return;
  }
  while ( rmsg->Id != msg.Id) {
    dispatch( pcom, (rpvd_sMsg *)rmsg);

    sts = udp_Receive( (char **)&rmsg, 1000);
    if ( sts == REM__TIMEOUT) {
      pcom->provideStatus( REM__DISORDER);
      return;
    }
  }
  if ( rmsg->Type != rpvd_eMsg_Attribute) {
    pcom->provideStatus( REM__DISORDER);
    return;
  }
  if ( EVEN( rmsg->Status)) {
    pcom->provideStatus( rmsg->Status);
    return;
  }

  // TODO Float conversion !!!
  if ( rpvd_opsys == 0 || rpvd_opsys == gdbroot->my_node->os) {
    void *p = &rmsg->Value;
    pcom->provideAttr( GDH__SUCCESS, oix, rmsg->Size, p);
  }
  else {
    gdb_sNode 		n;
    gdb_sClass		*cp;
    void		*p = NULL;

    memset( &n, 0, sizeof(n));
    n.os = (co_eOS) rpvd_opsys;

    p = malloc( max(rmsg->Size, (int)aref.Size));
    size = aref.Size;
    cp = (gdb_sClass *) hash_Search(&sts, gdbroot->cid_ht, &cid);
    if (cp != NULL) {
      rndc_ConvertData(&sts, &n, cp, p, &rmsg->Value, (pwr_tUInt32 *)&size,
		      ndc_eOp_encode, aref.Offset, 0);
      pcom->provideAttr( GDH__SUCCESS, oix, rmsg->Size, p);
    }
    else {
      pcom->provideStatus( GDH__NOSUCHCLASS);
    }
  }

}

void remote_pvd_pwrcli::dispatch( co_procom *pcom, rpvd_sMsg *msg)
{
  pwr_tStatus sts;

  switch ( msg->Any.Type) {
  case rpvd_eMsg_NodeUp: {
    rpvd_sMsgAny rmsg;

    printf( "NodeUp\n");

    rmsg.Type = REM__SUCCESS;
    rmsg.Id = msg->Any.Id;
    sts = udp_Send( (char *)&rmsg, sizeof(rmsg));

    pcom->flushNodes();
    subRestore();
    break;
  }
  default: ;
  }
}

void remote_pvd_pwrcli::receive( co_procom *pcom)
{
  rpvd_sMsgAny *msg;
  pwr_tStatus sts;

  // Get any message
  sts = udp_Receive( (char **)&msg, 20);
  if ( ODD(sts) && sts != REM__TIMEOUT) {
    dispatch( pcom, (rpvd_sMsg *)msg);
  }  
}

void remote_pvd_pwrcli::cyclic( co_procom *pcom)
{
  // Get any message
  receive( pcom);

  // Get subscribed values
  subRequest( pcom);

}

void remote_pvd_pwrcli::subRestore()
{
  rpvd_sMsgSubAdd msg;
  rpvd_sMsgAny *rmsg;
  pwr_tStatus sts;

  for ( sublist_iterator it = rpvd_sublist.begin(); it != rpvd_sublist.end(); it++) {
    
    msg.Type = rpvd_eMsg_SubAdd;
    msg.Id = rpvd_id++;

    msg.Oid = it->second.m_oid;
    strcpy( msg.Attribute, it->second.m_attr);
    msg.Rix = it->first;
    msg.Size = it->second.m_size;

    sts = udp_Request( (char *)&msg, sizeof(msg), (char **)&rmsg);
  }

}

void remote_pvd_pwrcli::subAssociateBuffer( co_procom *pcom, void **buff, int oix,
					    int offset, int size, pwr_tSubid subid)
{
  rpvd_sMsgSubAdd msg;
  rpvd_sMsgAny *rmsg;
  pwr_tStatus sts;
  pwr_tAttrRef aref;
  pwr_tOName aname;
  pwr_tCid cid;
  pwr_tUInt32 osize;

  msg.Type = rpvd_eMsg_SubAdd;
  msg.Id = rpvd_id++;
  msg.Oid.oix = oix;
  msg.Oid.vid = rpvd_vid;
  
  sts = gdh_GetObjectSize( msg.Oid, &osize);
  if ( EVEN(sts)) {
    pcom->provideStatus( sts);
    return;
  }

  memset( &aref, 0, sizeof(aref));
  aref.Objid = msg.Oid;
  aref.Offset = offset;
  aref.Size = size;
  if ( (int) osize == size)
    aref.Flags.b.Object = 1;

  sts = gdh_GetObjectClass( msg.Oid, &cid);
  if ( EVEN(sts)) {
    pcom->provideStatus( sts);
    return;
  }

  if ( aref.Flags.b.Object)
    strcpy( aname, "");
  else {
    sts = gdh_ClassAttrrefToAttr( cid, &aref, aname, sizeof(aname));
    if ( EVEN(sts)) {
      pcom->provideStatus( sts);
      return;
    }
  }
  strcpy( msg.Attribute, aname);
  msg.Rix = subid.rix;
  msg.Size = size;


  if ( pvd_cLog) logg( "AssoBuff", msg.Oid.oix, aname);

  sts = udp_Request( (char *)&msg, sizeof(msg), (char **)&rmsg);
  if ( EVEN(sts)) {
    pcom->provideStatus( sts);
    return;
  }
  if ( sts == REM__TIMEOUT) {
    pcom->provideStatus( REM__UDPNOCON);
    return;
  }
  while ( rmsg->Id != msg.Id) {
    dispatch( pcom, (rpvd_sMsg *)rmsg);

    sts = udp_Receive( (char **)&rmsg, 1000);
    if ( sts == REM__TIMEOUT) {
      pcom->provideStatus( REM__DISORDER);
      return;
    }
  }
  if ( rmsg->Type != rpvd_eMsg_Status) {
    pcom->provideStatus( REM__DISORDER);
    return;
  }
  if ( EVEN( rmsg->Status)) {
    pcom->provideStatus( rmsg->Status);
    return;
  }

  pcom->provideStatus( rmsg->Status);
  if ( ODD(rmsg->Status)) {
    // Add to local list
    subitem s(size, cid, offset, msg.Attribute, msg.Oid);
    rpvd_sublist[subid.rix] = s;
    *buff = rpvd_sublist[subid.rix].m_p;
  }
}

void remote_pvd_pwrcli::subDisassociateBuffer( co_procom *pcom, pwr_tSubid subid)
{
  rpvd_sMsgSubRemove msg;
  rpvd_sMsgAny *rmsg;
  pwr_tStatus sts;

  // Remove from local list
  sublist_iterator it = rpvd_sublist.find( subid.rix);
  if ( it != rpvd_sublist.end())
    rpvd_sublist.erase( it);

  // Send message
  msg.Type = rpvd_eMsg_SubRemove;
  msg.Id = rpvd_id++;
  msg.Rix = subid.rix;

  if ( pvd_cLog) logg( "DisoBuff", subid.rix, "(rix)");

  sts = udp_Request( (char *)&msg, sizeof(msg), (char **)&rmsg);
  if ( EVEN(sts)) {
    pcom->provideStatus( sts);
    return;
  }
  if ( sts == REM__TIMEOUT) {
    pcom->provideStatus( REM__UDPNOCON);
    return;
  }
  while ( rmsg->Id != msg.Id) {
    dispatch( pcom, (rpvd_sMsg *)rmsg);

    sts = udp_Receive( (char **)&rmsg, 1000);
    if ( sts == REM__TIMEOUT) {
      pcom->provideStatus( REM__DISORDER);
      return;
    }
  }
  if ( rmsg->Type != rpvd_eMsg_Status) {
    pcom->provideStatus( REM__DISORDER);
    return;
  }
  pcom->provideStatus( rmsg->Status);
}


void remote_pvd_pwrcli::subRequest( co_procom *pcom)
{
  rpvd_sMsgAny msg;
  rpvd_sMsgSubSend *rmsg;
  pwr_tStatus sts;
  char *subp;
  int rix;
  int size;

  msg.Type = rpvd_eMsg_SubRequest;
  msg.Id = rpvd_id++;
  for (;;) {
    sts = udp_CheckLink();
    if ( EVEN(sts)) return;

    sts = udp_Send( (char *)&msg, sizeof(msg));
    if ( EVEN(sts)) return;

    sts = udp_Receive( (char **)&rmsg, 1000);
    if ( EVEN(sts))
      return;
    if ( sts == REM__TIMEOUT) {
      udp_LinkFailure();
      return;
    }
    while ( rmsg->Id != msg.Id) {
      dispatch( pcom, (rpvd_sMsg *)rmsg);

      sts = udp_Receive( (char **)&rmsg, 1000);
      if ( sts == REM__TIMEOUT) {
	pcom->provideStatus( REM__DISORDER);
	return;
      }
    }

    // Unpack the message
    subp = (char *) &rmsg->Data;
    while ( *(int *)subp != -1) {
      rix = *(int *)subp;
      subp += 4;
      size = *(int *)subp;
      subp += 4;
      
      sublist_iterator it = rpvd_sublist.find( rix);
      if ( it != rpvd_sublist.end()) {
	// TODO Data conversion !!!

	if ( rpvd_opsys == 0 || rpvd_opsys == gdbroot->my_node->os) {
	  memcpy( it->second.m_p, subp, it->second.m_size);
	}
	else {
	  gdb_sNode 	n;
	  gdb_sClass	*cp;
	  int size;
	  
	  memset( &n, 0, sizeof(n));
	  n.os = (co_eOS) rpvd_opsys;
	  n.fm.b.bo = co_eBO_little;
	  n.netver = gdbroot->my_node->netver;
	  size = it->second.m_size;

	  cp = (gdb_sClass *) hash_Search(&sts, gdbroot->cid_ht, &it->second.m_cid);
	  if (cp != NULL) {
	    rndc_ConvertData(&sts, &n, cp, it->second.m_p, subp, 
			    (pwr_tUInt32 *)&size, ndc_eOp_encode, it->second.m_offset, 0);
	  }
	}
      }
      subp += size;
    }
    if ( !rmsg->More)
      break;
  }
}

void remote_pvd_pwrcli::nodeUp()
{
  rpvd_sMsgAny msg;
  rpvd_sMsgAny *rmsg;
  pwr_tStatus sts;

  msg.Type = rpvd_eMsg_NodeUp;
  msg.Id = rpvd_id++;
  
  sts = udp_Request( (char *)&msg, sizeof(msg), (char **)&rmsg);
  if ( EVEN(sts) || sts == REM__TIMEOUT) {
  }
}

void usage()
{
  cout << "remote_pvd_pwrcli   Proview provider client" << endl << endl <<
    "Arguments: " << endl <<
    "  1   Remote address" << endl <<
    "  2   Remote host name" << endl <<
    "  3   Remote volume id" << endl <<
    "  4   Remote volume name" << endl <<
    "  5   Server identity (optional, default 200)" << endl <<
    "  6   Udp port number (optional, default 3051)" << endl <<
    "  7   Remote operating system (optional)" << endl << endl;
}

int main(int argc, char *argv[])
{
  pwr_tStatus sts;
  char remote_address[40];
  char remote_host_name[40];
  char remote_vid[40];
  char remote_volume_name[40];
  int udp_port;
  int server_id;
  
  /* Read arguments */
  if ( argc < 5) {
    usage();
    exit(0);
  }
  strcpy( remote_address, argv[1]);
  strcpy( remote_host_name, argv[2]);
  strcpy( remote_vid, argv[3]);
  strcpy( remote_volume_name, argv[4]);

  if ( argc >= 6) {
    sts = sscanf( argv[5], "%d", &server_id);
    if ( sts != 1) {
      usage();
      exit(0);
    } 
  }
  else
    server_id = 200;

  if ( argc >= 7) {
    sts = sscanf( argv[6], "%d", &udp_port);
    if ( sts != 1) {
      usage();
      exit(0);
    } 
  }
  else
    udp_port = 3051;

  if ( argc >= 8) {
    sts = sscanf( argv[7], "%d", &rpvd_opsys);
    if ( sts != 1) {
      usage();
      exit(0);
    } 
  }
  else
    rpvd_opsys = 0;
  

  cdh_StringToVolumeId( remote_vid, &rpvd_vid);
  strcpy( rpvd_vname, remote_volume_name);

  sts = udp_Init( remote_address, remote_host_name, udp_port);
  if ( EVEN(sts)) {
    exit(0);
  }

  remote_pvd_pwrcli provider( pvd_eEnv_Rt);
  rt_procom procom( &provider,
		    errh_eAnix_appl20, 	// Application index
		    "remote_pvd_pwrcli", // Process name
		    server_id,	       	// Sid
		    rpvd_vid, 		// Vid
		    rpvd_vname,  	// Volume name
		    0);			// Global
  

  procom.init();

  provider.nodeUp();

  procom.mainLoop();
}


