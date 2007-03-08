/* 
 * Proview   $Id: opc_provider.cpp,v 1.3 2007-03-08 07:26:29 claes Exp $
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
#include "pwr_opcclasses.h"
#include "opc_utl.h"
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

void opc_provider::insert_object( pwr_tOix fth, pwr_tOix bws, ns1__BrowseElement *element,
				  int first, int last, int load_children)
{
  procom_obj o;
  char *valp;
	
  strcpy( o.name, element->Name->c_str());
  o.oix = next_oix++;
  o.fthoix = fth;
  if ( !element->IsItem) {
    o.cid = pwr_cClass_Opc_Hier;
    o.body_size = sizeof(pwr_sClass_Opc_Hier);
    o.body = calloc( 1, o.body_size);
    if ( opc_get_property( element->Properties, opc_mProperty_Description, &valp))
      strncpy( ((pwr_sClass_Opc_Hier *)o.body)->Description, valp, 
	       sizeof(((pwr_sClass_Opc_Hier *)o.body)->Description));
  }
  else {
    if ( opc_get_property( element->Properties, opc_mProperty_DataType, &valp)) {
      if ( strcmp( valp, "string") == 0) {
	o.cid = pwr_cClass_Opc_String;
	o.body_size = sizeof(pwr_sClass_Opc_String);
	o.body = calloc( 1, o.body_size);
      }
      else if ( strcmp( valp, "boolean") == 0) {
	o.cid = pwr_cClass_Opc_Boolean;
	o.body_size = sizeof(pwr_sClass_Opc_Boolean);
	o.body = calloc( 1, o.body_size);
      }
      else if ( strcmp( valp, "float") == 0) {
	o.cid = pwr_cClass_Opc_Float;
	o.body_size = sizeof(pwr_sClass_Opc_Float);
	o.body = calloc( 1, o.body_size);
      }
      else if ( strcmp( valp, "double") == 0) {
	o.cid = pwr_cClass_Opc_Double;
	o.body_size = sizeof(pwr_sClass_Opc_Double);
	o.body = calloc( 1, o.body_size);
      }
      else if ( strcmp( valp, "double") == 0) {
	o.cid = pwr_cClass_Opc_Double;
	o.body_size = sizeof(pwr_sClass_Opc_Double);
	o.body = calloc( 1, o.body_size);
      }
      else if ( strcmp( valp, "decimal") == 0) {
	o.cid = pwr_cClass_Opc_Decimal;
	o.body_size = sizeof(pwr_sClass_Opc_Decimal);
	o.body = calloc( 1, o.body_size);
      }
      else if ( strcmp( valp, "int") == 0) {
	o.cid = pwr_cClass_Opc_Int;
	o.body_size = sizeof(pwr_sClass_Opc_Int);
	o.body = calloc( 1, o.body_size);
      }
      else if ( strcmp( valp, "short") == 0) {
	o.cid = pwr_cClass_Opc_Short;
	o.body_size = sizeof(pwr_sClass_Opc_Short);
	o.body = calloc( 1, o.body_size);
      }
      else if ( strcmp( valp, "byte") == 0) {
	o.cid = pwr_cClass_Opc_Byte;
	o.body_size = sizeof(pwr_sClass_Opc_Byte);
	o.body = calloc( 1, o.body_size);
      }
      else if ( strcmp( valp, "unsignedLong") == 0) {
	o.cid = pwr_cClass_Opc_UnsignedLong;
	o.body_size = sizeof(pwr_sClass_Opc_UnsignedLong);
	o.body = calloc( 1, o.body_size);
      }
      else if ( strcmp( valp, "unsignedInt") == 0) {
	o.cid = pwr_cClass_Opc_UnsignedInt;
	o.body_size = sizeof(pwr_sClass_Opc_UnsignedInt);
	o.body = calloc( 1, o.body_size);
      }
      else if ( strcmp( valp, "unsignedShort") == 0) {
	o.cid = pwr_cClass_Opc_UnsignedShort;
	o.body_size = sizeof(pwr_sClass_Opc_UnsignedShort);
	o.body = calloc( 1, o.body_size);
      }
      else if ( strcmp( valp, "unsignedByte") == 0) {
	o.cid = pwr_cClass_Opc_UnsignedByte;
	o.body_size = sizeof(pwr_sClass_Opc_UnsignedByte);
	o.body = calloc( 1, o.body_size);
      }
      else if ( strcmp( valp, "base64Binary") == 0) {
	o.cid = pwr_cClass_Opc_Base64Binary;
	o.body_size = sizeof(pwr_sClass_Opc_Base64Binary);
	o.body = calloc( 1, o.body_size);
      }
      else if ( strcmp( valp, "dateTime") == 0) {
	o.cid = pwr_cClass_Opc_DateTime;
	o.body_size = sizeof(pwr_sClass_Opc_DateTime);
	o.body = calloc( 1, o.body_size);
      }
      else if ( strcmp( valp, "time") == 0) {
	o.cid = pwr_cClass_Opc_Time;
	o.body_size = sizeof(pwr_sClass_Opc_Time);
	o.body = calloc( 1, o.body_size);
      }
      else if ( strcmp( valp, "date") == 0) {
	o.cid = pwr_cClass_Opc_Date;
	o.body_size = sizeof(pwr_sClass_Opc_Date);
	o.body = calloc( 1, o.body_size);
      }
      else if ( strcmp( valp, "duration") == 0) {
	o.cid = pwr_cClass_Opc_Duration;
	o.body_size = sizeof(pwr_sClass_Opc_Duration);
	o.body = calloc( 1, o.body_size);
      }
      else if ( strcmp( valp, "QName") == 0) {
	o.cid = pwr_cClass_Opc_QName;
	o.body_size = sizeof(pwr_sClass_Opc_QName);
	o.body = calloc( 1, o.body_size);
      }
    }
    else {
      o.cid = pwr_cClass_Opc_Hier;
      o.body_size = sizeof(pwr_sClass_Opc_Hier);
      o.body = calloc( 1, o.body_size);
      if ( opc_get_property( element->Properties, opc_mProperty_Description, &valp))
	strncpy( ((pwr_sClass_Opc_Hier *)o.body)->Description, valp, 
		 sizeof(((pwr_sClass_Opc_Hier *)o.body)->Description));
    }
  }
  if ( first)
    m_list[fth].fchoix = o.oix;
  else {
    o.bwsoix = bws;
    m_list[bws].fwsoix = o.oix;
  }
  if ( last) {
    m_list[fth].lchoix = o.oix;
    if ( !first) {
      o.fwsoix = m_list[fth].fchoix;
      m_list[o.fwsoix].bwsoix = o.oix;
    }
    else {
      // Single child
      o.fwsoix = o.oix;
      o.bwsoix = o.oix;
    }
  }
  if ( element->HasChildren && load_children)
    o.flags |= procom_obj_mFlags_Loaded;
  else if ( !element->HasChildren)
    o.flags |= procom_obj_mFlags_Loaded;
  m_list.push_back( o);

  if ( load_children) {
    _ns1__Browse browse;
    _ns1__BrowseResponse browse_response;

    browse.ItemName = new std::string( longname(o.oix));
    opc_mask_to_propertynames( browse.PropertyNames, 
			       opc_mProperty_DataType | opc_mProperty_Description);

    if ( soap_call___ns1__Browse( &soap, opc_endpoint, NULL, &browse, &browse_response) ==
	 SOAP_OK) {
      pwr_tOix next_bws;
      pwr_tOix bws = 0;
      for ( int i = 0; i < (int)browse_response.Elements.size(); i++) {
	next_bws = next_oix;
	insert_object( o.oix, bws, browse_response.Elements[i],
		       i == 0, i == (int)browse_response.Elements.size() - 1, 0);
	bws = next_bws;
      }
    }
  }
}

void opc_provider::objectOid( co_procom *pcom, pwr_tOix oix)
{
  if ( m_list.size() == 0) {
    // Insert volume object
    procom_obj vo;

    vo.cid = pwr_eClass_ExternVolume;
    strcpy( vo.name, opc_vname);
    vo.body_size = sizeof(pwr_sExternVolume);
    vo.body = calloc( 1, vo.body_size);
    vo.oix = 0;
    vo.flags |= procom_obj_mFlags_Loaded;
    m_list.push_back( vo);

    // Insert ServerState object
    procom_obj so;
    so.cid = pwr_cClass_Opc_ServerState;
    strcpy( so.name, "OpcServerState");
    so.body_size = sizeof(pwr_sClass_Opc_ServerState);
    so.body = calloc( 1, so.body_size);
    memcpy( so.body, &server_state, so.body_size);
    so.oix = next_oix++;
    so.fthoix = 0;
    so.bwsoix = so.oix;
    so.fwsoix = so.oix;
    m_list[0].fchoix = so.oix;
    m_list[0].lchoix = so.oix;
    so.flags |= procom_obj_mFlags_Loaded;
    m_list.push_back( so);

    // Load Rootlist
    _ns1__Browse browse;
    _ns1__BrowseResponse browse_response;

    opc_mask_to_propertynames( browse.PropertyNames, 
			       opc_mProperty_DataType | opc_mProperty_Description);
    if ( soap_call___ns1__Browse( &soap, opc_endpoint, NULL, &browse, &browse_response) ==
	 SOAP_OK) {
      pwr_tOix bws = so.oix;
      pwr_tOix next_bws;
      for ( int i = 0; i < (int)browse_response.Elements.size(); i++) {
	next_bws = next_oix;
	insert_object( oix, bws, browse_response.Elements[i],
		       0, i == (int)browse_response.Elements.size() - 1, 1);
	bws = next_bws;
      }
    }
    else
      soap_print_fault( &soap, stderr);
  }
  else if ( oix < m_list.size()) {
    if ( !(m_list[oix].flags & procom_obj_mFlags_Loaded)) {
      _ns1__Browse browse;
      _ns1__BrowseResponse browse_response;

      browse.ItemName = new std::string( longname(oix));
      opc_mask_to_propertynames( browse.PropertyNames, 
				 opc_mProperty_DataType | opc_mProperty_Description);
      
      if ( soap_call___ns1__Browse( &soap, opc_endpoint, NULL, &browse, &browse_response) ==
	   SOAP_OK) {
	pwr_tOix next_bws;
	pwr_tOix bws = 0;
	for ( int i = 0; i < (int)browse_response.Elements.size(); i++) {
	  next_bws = next_oix;
	  insert_object( oix, bws, browse_response.Elements[i],
			 i == 0, i == (int)browse_response.Elements.size() - 1, 0);
	  bws = next_bws;
	}
      }
      m_list[oix].flags |= procom_obj_mFlags_Loaded;
    }
  }

  if ( oix >= m_list.size() || oix < 0) {
    pcom->provideStatus( GDH__NOSUCHOBJ);
    return;
  }

#if 0
  for ( int i = 0; i < (int)m_list.size(); i++) {
    printf( "oix %2d bws %2d fws %2d fth %2d fch %2d lch %2d flags %lu %s\n", 
	    m_list[i].oix, m_list[i].bwsoix, m_list[i].fwsoix, m_list[i].fthoix, 
	    m_list[i].fchoix, m_list[i].lchoix, m_list[i].flags, m_list[i].name);
  }
#endif
  vector<procom_obj>olist;
  for ( int i = 0; i < (int) m_list.size(); i++) {
    if  ( m_list[i].flags & procom_obj_mFlags_Loaded)
      olist.push_back( m_list[i]);
  }
  printf( "*********************************************\n");
  for ( int i = 0; i < (int)olist.size(); i++) {
    printf( "oix %2d bws %2d fws %2d fth %2d fch %2d lch %2d flags %lu %s\n", 
	    olist[i].oix, olist[i].bwsoix, olist[i].fwsoix, olist[i].fthoix, 
	    olist[i].fchoix, olist[i].lchoix, olist[i].flags, olist[i].name);
  }
  pcom->provideObjects( GDH__SUCCESS, olist);
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

void opc_provider::get_server_state()
{
  _ns1__GetStatus get_status;
  _ns1__GetStatusResponse get_status_response;
  get_status.ClientRequestHandle = new std::string("Opc Client");

  if ( soap_call___ns1__GetStatus( &soap, opc_endpoint, NULL, &get_status, &get_status_response) ==
       SOAP_OK) {
    if ( get_status_response.Status->VendorInfo)
      strcpy( server_state.VendorInfo, get_status_response.Status->VendorInfo->c_str());
    if ( get_status_response.Status->ProductVersion)
      strcpy( server_state.ProductVersion, get_status_response.Status->ProductVersion->c_str());
    server_state.StartTime.tv_sec = get_status_response.Status->StartTime;
    server_state.ServerState = get_status_response.GetStatusResult->ServerState;
  }
  else {
    server_state.ServerState = ns1__serverState__commFault;    
    soap_print_fault( &soap, stderr);
  }
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
    strcpy( server_url, "http://linuxlab1:18083");
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
  strcpy( provider.server_state.Server, server_url);

  // provider.nodeUp();
  soap_init( &provider.soap);

  provider.get_server_state();

  procom.mainLoop();
}

