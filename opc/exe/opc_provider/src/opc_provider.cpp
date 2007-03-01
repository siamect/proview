/* 
 * Proview   $Id: opc_provider.cpp,v 1.1 2007-03-01 09:12:54 claes Exp $
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

#include <vector.h>
#include <string.h>
#include <stdio.h>
#include <iostream.h>
#include <fstream.h>
#include "pwr.h"
#include "pwr_class.h"
#include "pwr_baseclasses.h"
#include "wb_vext.h"
#include "rt_procom.h"
#include "opc_provider.h"
#include "wb_ldh.h"
#include "wb_ldh_msg.h"
#include "rt_gdh_msg.h"
#include "co_cdh.h"
#include "co_dcli.h"
#include "opc_soap_H.h"
#include "Service.nsmap"

#define START_OIX 1000

static pwr_tVid opc_vid;
static char opc_vname[32];
static char opc_endpoint[256];

// Wb only
void opc_provider::object( co_procom *pcom)
{
  if ( m_list.size() <= 1 || m_list[0].fchoix == 0) {
    pcom->provideObject( LDH__NOSUCHOBJ,0,0,0,0,0,0,0,"","");
    return;
  }
  objectOid( pcom, m_list[0].fchoix);
}

void opc_provider::objectOid( co_procom *pcom, pwr_tOix oix)
{
  if ( m_list.size() == 0) {
    // Load Rootlist
    _ns1__Browse browse;
    _ns1__BrowseResponse browse_response;

    std::string s("dataType");
    browse.PropertyNames.push_back( s);
    if ( soap_call___ns1__Browse( &soap, opc_endpoint, NULL, &browse, &browse_response) ==
	 SOAP_OK) {
      for ( int i = 0; i < (int)browse_response.Elements.size(); i++) {
	procom_obj o;
	
	strcpy( o.name, browse_response.Elements[i]->Name->c_str());
	strcpy( o.lname, o.name);
	o.oix = next_oix++;
	if ( i != 0)
	  o.bwsoix = o.oix - 1;
	if ( i != (int)browse_response.Elements.size() - 1)
	  o.fwsoix = o.oix + 1;
	if ( browse_response.Elements[i]->HasChildren)
	  o.flags |= procom_obj_mFlags_Loaded;
	else
	  o.flags |= procom_obj_mFlags_Loaded;
	m_list.push_back( o);
      }
    }
    else
      soap_print_fault( &soap, stderr);
  }

  if ( oix >= m_list.size() || oix < 0) {
    pcom->provideStatus( GDH__NOSUCHOBJ);
    return;
  }
  pcom->provideObjects( GDH__SUCCESS, m_list);
}

void opc_provider::objectName( co_procom *pcom, char *name)
{

  for ( int i = 0; i < (int) m_list.size(); i++) {
    if  ( !m_list[i].flags & procom_obj_mFlags_Deleted) {
      if ( cdh_NoCaseStrcmp( name, longname(m_list[i].oix)) == 0) {
	objectOid( pcom, i);
	return;
      }
    }
  }
  if ( m_env == pvd_eEnv_Wb)
    pcom->provideObject( 0,0,0,0,0,0,0,0,"","");
  else
    pcom->provideStatus( GDH__NOSUCHOBJ);

}

// Wb only
void opc_provider::objectBody( co_procom *pcom, pwr_tOix oix)
{
}

// Wb only
void opc_provider::createObject( co_procom *pcom, pwr_tOix destoix, int desttype,
		     pwr_tCid cid, char *name)
{
}

// Wb only
void opc_provider::moveObject( co_procom *pcom, pwr_tOix oix, pwr_tOix destoix, 
				 int desttype)
{
}

// Wb only
void opc_provider::deleteObject( co_procom *pcom, pwr_tOix oix)
{
}

// Wb only
void opc_provider::copyObject( co_procom *pcom, pwr_tOix oix, pwr_tOix destoix, int desttype,
				 char *name)
{
}

// Wb only
void opc_provider::deleteFamily( co_procom *pcom, pwr_tOix oix)
{
}

// Wb only
void opc_provider::renameObject( co_procom *pcom, pwr_tOix oix, char *name)
{
}

void opc_provider::writeAttribute( co_procom *pcom, pwr_tOix oix, unsigned int offset,
		       unsigned int size, char *buffer)
{
  if ( oix >= m_list.size() || oix <= 0) {
    pcom->provideStatus( LDH__NOSUCHOBJ);
    return;
  }

  if ( offset + size > m_list[oix].body_size) {
    pcom->provideStatus( LDH__NOSUCHATTR);
    return;
  }

  memcpy( (void *)((unsigned long)m_list[oix].body + (unsigned long)offset), buffer, size);
  pcom->provideStatus( 1);
}

// Rt only
void opc_provider::readAttribute( co_procom *pcom, pwr_tOix oix, unsigned int offset,
		       unsigned int size)
{
  if ( oix >= m_list.size() || oix <= 0) {
    pcom->provideStatus( GDH__NOSUCHOBJ);
    return;
  }

  if ( offset + size > m_list[oix].body_size) {
    pcom->provideStatus( GDH__NOSUCHOBJ);
    return;
  }

  void *p = (void *)((unsigned long)m_list[oix].body + (unsigned long)offset);
  pcom->provideAttr( GDH__SUCCESS, oix, size, p);
}

// Rt only
void opc_provider::subAssociateBuffer( co_procom *pcom, void **buff, int oix, int offset, 
				      int size, pwr_tSubid sid) 
{
  if ( oix < (int)m_list.size())
    *buff = (char *)m_list[oix].body + offset;
  else 
    *buff = 0;
}

// Wb only
void opc_provider::commit( co_procom *pcom)
{
  pwr_tStatus sts;

  save( &sts);
  pcom->provideStatus( sts);
}

// Wb only
void opc_provider::abort( co_procom *pcom)
{
  pwr_tStatus sts;

  m_list.clear();
  next_oix = 1;
  load( &sts);
  pcom->provideStatus( sts);
}

void opc_provider::delete_tree( pwr_tOix oix)
{
  m_list[oix].flags = procom_obj_mFlags_Deleted;

  for ( pwr_tOix ix = m_list[oix].fchoix;
	ix;
	ix = m_list[ix].fwsoix)
    delete_tree( ix);
}

char *opc_provider::longname( pwr_tOix oix)
{
  if ( m_list[oix].fthoix == 0)
    strcpy( m_list[oix].lname, m_list[oix].name);
  else {
    strcpy( m_list[oix].lname, longname( m_list[oix].fthoix));
    strcat( m_list[oix].lname, "-");
    strcat( m_list[oix].lname, m_list[oix].name);
  }
  return m_list[oix].lname;
}

bool opc_provider::find( pwr_tOix fthoix, char *name, pwr_tOix *oix)
{

  for ( int i = 0; i < (int) m_list.size(); i++) {
    if  ( !m_list[i].flags & procom_obj_mFlags_Deleted) {
      if ( m_list[i].fthoix == fthoix && 
	   cdh_NoCaseStrcmp( name, m_list[i].name) == 0) {
	*oix = m_list[i].oix;
	return true;
      }
    }
  }
  return false;
}


void usage()
{
  cout << "remote_pvd_pwrcli   Proview provider client" << endl << endl <<
    "Arguments: " << endl <<
    "  1   Opc server URL" << endl <<
    "  2   Extern volume id" << endl <<
    "  3   Extern volume name" << endl <<
    "  4   Server identity (optional, default 200)" << endl;
}

int main(int argc, char *argv[])
{
  pwr_tStatus sts;
  char server_url[256];
  char extern_vid[40];
  char extern_volume_name[40];
  int server_id;
  
  /* Read arguments */
  if ( argc < 4) {
    usage();
    strcpy( server_url, "http://localhost:18083");
    strcpy( extern_vid, "0.1.99.55");
    strcpy( extern_volume_name, "MyOpcVolume");
    // exit(0);
  }
  else {
  strcpy( server_url, argv[1]);
  strcpy( extern_vid, argv[2]);
  strcpy( extern_volume_name, argv[3]);
  }

  if ( argc >= 5) {
    sts = sscanf( argv[4], "%d", &server_id);
    if ( sts != 1) {
      usage();
      exit(0);
    } 
  }
  else
    server_id = 200;

  strcpy( opc_endpoint, server_url);
  cdh_StringToVolumeId( extern_vid, &opc_vid);
  strcpy( opc_vname, extern_volume_name);

  opc_provider provider( pvd_eEnv_Rt);
  rt_procom procom( &provider,
		    errh_eAnix_appl20, 	// Application index
		    "opc_provider", // Process name
		    server_id,	       	// Sid
		    opc_vid, 		// Vid
		    opc_vname,  	// Volume name
		    0);			// Global
  
  procom.init();

  // provider.nodeUp();
  soap_init( &provider.soap);

  procom.mainLoop();
}

