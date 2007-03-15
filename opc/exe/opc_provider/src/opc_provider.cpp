/* 
 * Proview   $Id: opc_provider.cpp,v 1.7 2007-03-15 15:25:36 claes Exp $
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
#define procom_obj_mFlags_Analog (1 << 31)

typedef map<pwr_tUInt32, opcprv_sub>::iterator sublist_iterator;

static pwr_tVid opc_vid;
static char opc_vname[32];
static char opc_endpoint[256];

// Wb only
void opc_provider::object( co_procom *pcom)
{
  if ( m_list.size() <= 1 || m_list[0].po.fchoix == 0) {
    pcom->provideObject( LDH__NOSUCHOBJ,0,0,0,0,0,0,0,"","");
    return;
  }
  objectOid( pcom, m_list[0].po.fchoix);
}

void opc_provider::insert_object( pwr_tOix fth, pwr_tOix bws, s0__BrowseElement *element,
				  int first, int last, int load_children, std::string *path)
{
  opcprv_obj o;
  xsd__anyType *valp;
  int opctype;
	
  strcpy( o.po.name, name_to_objectname( (char *) element->Name->c_str()));
  if ( element->ItemPath)
    path = element->ItemPath;
  if ( path) {
    strcpy( o.item_name, path->c_str());
    strcat( o.item_name, element->ItemName->c_str());
  }
  else
    strcpy( o.item_name, element->ItemName->c_str());

  o.po.oix = next_oix++;
  o.po.fthoix = fth;
  if ( !element->IsItem) {
    o.po.cid = pwr_cClass_Opc_Hier;
    o.po.body_size = sizeof(pwr_sClass_Opc_Hier);
    o.po.body = calloc( 1, o.po.body_size);
    if ( opc_get_property( element->Properties, opc_mProperty_Description, &valp))
      strncpy( ((pwr_sClass_Opc_Hier *)o.po.body)->Description, 
	       ((xsd__string *)valp)->__item.c_str(), 
	       sizeof(((pwr_sClass_Opc_Hier *)o.po.body)->Description));
  }
  else {
    if ( opc_get_property( element->Properties, opc_mProperty_DataType, &valp)) {
      if ( !opc_string_to_opctype( ((xsd__string *)valp)->__item.c_str(), &opctype))
	opctype = opc_eDataType_;
      
      switch ( opctype) {
      case opc_eDataType_string:
	o.po.cid = pwr_cClass_Opc_String;
	o.po.body_size = sizeof(pwr_sClass_Opc_String);
	o.po.body = calloc( 1, o.po.body_size);
	break;
      case opc_eDataType_boolean:
	o.po.cid = pwr_cClass_Opc_Boolean;
	o.po.body_size = sizeof(pwr_sClass_Opc_Boolean);
	o.po.body = calloc( 1, o.po.body_size);
	break;
      case opc_eDataType_float:
	o.po.cid = pwr_cClass_Opc_Float;
	o.po.body_size = sizeof(pwr_sClass_Opc_Float);
	o.po.body = calloc( 1, o.po.body_size);
	break;
      case opc_eDataType_double:
	o.po.cid = pwr_cClass_Opc_Double;
	o.po.body_size = sizeof(pwr_sClass_Opc_Double);
	o.po.body = calloc( 1, o.po.body_size);
	break;
      case opc_eDataType_decimal:
	o.po.cid = pwr_cClass_Opc_Decimal;
	o.po.body_size = sizeof(pwr_sClass_Opc_Decimal);
	o.po.body = calloc( 1, o.po.body_size);
	break;
      case opc_eDataType_int:
	o.po.cid = pwr_cClass_Opc_Int;
	o.po.body_size = sizeof(pwr_sClass_Opc_Int);
	o.po.body = calloc( 1, o.po.body_size);
	break;
      case opc_eDataType_short:
	o.po.cid = pwr_cClass_Opc_Short;
	o.po.body_size = sizeof(pwr_sClass_Opc_Short);
	o.po.body = calloc( 1, o.po.body_size);
	break;
      case opc_eDataType_byte:
	o.po.cid = pwr_cClass_Opc_Byte;
	o.po.body_size = sizeof(pwr_sClass_Opc_Byte);
	o.po.body = calloc( 1, o.po.body_size);
	break;
      case opc_eDataType_unsignedLong:
	o.po.cid = pwr_cClass_Opc_UnsignedLong;
	o.po.body_size = sizeof(pwr_sClass_Opc_UnsignedLong);
	o.po.body = calloc( 1, o.po.body_size);
	break;
      case opc_eDataType_unsignedInt:
	o.po.cid = pwr_cClass_Opc_UnsignedInt;
	o.po.body_size = sizeof(pwr_sClass_Opc_UnsignedInt);
	o.po.body = calloc( 1, o.po.body_size);
	break;
      case opc_eDataType_unsignedShort:
	o.po.cid = pwr_cClass_Opc_UnsignedShort;
	o.po.body_size = sizeof(pwr_sClass_Opc_UnsignedShort);
	o.po.body = calloc( 1, o.po.body_size);
	break;
      case opc_eDataType_unsignedByte:
	o.po.cid = pwr_cClass_Opc_UnsignedByte;
	o.po.body_size = sizeof(pwr_sClass_Opc_UnsignedByte);
	o.po.body = calloc( 1, o.po.body_size);
	break;
      case opc_eDataType_base64Binary:
	o.po.cid = pwr_cClass_Opc_Base64Binary;
	o.po.body_size = sizeof(pwr_sClass_Opc_Base64Binary);
	o.po.body = calloc( 1, o.po.body_size);
	break;
      case opc_eDataType_dateTime:
	o.po.cid = pwr_cClass_Opc_DateTime;
	o.po.body_size = sizeof(pwr_sClass_Opc_DateTime);
	o.po.body = calloc( 1, o.po.body_size);
	break;
      case opc_eDataType_time:
	o.po.cid = pwr_cClass_Opc_Time;
	o.po.body_size = sizeof(pwr_sClass_Opc_Time);
	o.po.body = calloc( 1, o.po.body_size);
	break;
      case opc_eDataType_date:
	o.po.cid = pwr_cClass_Opc_Date;
	o.po.body_size = sizeof(pwr_sClass_Opc_Date);
	o.po.body = calloc( 1, o.po.body_size);
	break;
      case opc_eDataType_duration:
	o.po.cid = pwr_cClass_Opc_Duration;
	o.po.body_size = sizeof(pwr_sClass_Opc_Duration);
	o.po.body = calloc( 1, o.po.body_size);
	break;
      case opc_eDataType_QName:
	o.po.cid = pwr_cClass_Opc_QName;
	o.po.body_size = sizeof(pwr_sClass_Opc_QName);
	o.po.body = calloc( 1, o.po.body_size);
      default:
	o.po.cid = pwr_cClass_Opc_Hier;
	o.po.body_size = sizeof(pwr_sClass_Opc_Hier);
	o.po.body = calloc( 1, o.po.body_size);
      }

      if ( opc_get_property( element->Properties, opc_mProperty_Description, &valp)) {
	pwr_tString80 desc;
	strncpy( desc, ((xsd__string *)valp)->__item.c_str(), sizeof(desc));

	switch ( o.po.cid) {
	case pwr_cClass_Opc_Float:
	  strncpy( ((pwr_sClass_Opc_Float *)o.po.body)->Description, desc, 
		   sizeof(((pwr_sClass_Opc_Float *)o.po.body)->Description));
	  break;
	case pwr_cClass_Opc_Double:
	  strncpy( ((pwr_sClass_Opc_Float *)o.po.body)->Description, desc, 
		   sizeof(((pwr_sClass_Opc_Double *)o.po.body)->Description));
	  break;
	case pwr_cClass_Opc_Int:
	  strncpy( ((pwr_sClass_Opc_Int *)o.po.body)->Description, desc, 
		   sizeof(((pwr_sClass_Opc_Int *)o.po.body)->Description));
	  break;
	case pwr_cClass_Opc_Short:
	  strncpy( ((pwr_sClass_Opc_Short *)o.po.body)->Description, desc, 
		   sizeof(((pwr_sClass_Opc_Short *)o.po.body)->Description));
	  break;
	case pwr_cClass_Opc_Byte:
	  strncpy( ((pwr_sClass_Opc_Byte *)o.po.body)->Description, desc, 
		   sizeof(((pwr_sClass_Opc_Byte *)o.po.body)->Description));
	  break;
	case pwr_cClass_Opc_UnsignedInt:
	  strncpy( ((pwr_sClass_Opc_UnsignedInt *)o.po.body)->Description, desc, 
		   sizeof(((pwr_sClass_Opc_UnsignedInt *)o.po.body)->Description));
	  break;
	case pwr_cClass_Opc_UnsignedShort:
	  strncpy( ((pwr_sClass_Opc_UnsignedShort *)o.po.body)->Description, desc, 
		   sizeof(((pwr_sClass_Opc_UnsignedShort *)o.po.body)->Description));
	  break;
	case pwr_cClass_Opc_UnsignedByte:
	  strncpy( ((pwr_sClass_Opc_UnsignedByte *)o.po.body)->Description, desc, 
		   sizeof(((pwr_sClass_Opc_UnsignedByte *)o.po.body)->Description));
	  break;
	}
      }
    }
    else {
      o.po.cid = pwr_cClass_Opc_Hier;
      o.po.body_size = sizeof(pwr_sClass_Opc_Hier);
      o.po.body = calloc( 1, o.po.body_size);
      if ( opc_get_property( element->Properties, opc_mProperty_Description, &valp)) {
	strncpy( ((pwr_sClass_Opc_Hier *)o.po.body)->Description, 
		 ((xsd__string *)valp)->__item.c_str(),
		 sizeof(((pwr_sClass_Opc_Hier *)o.po.body)->Description));
      }
    }
  }
  if ( first)
    m_list[fth].po.fchoix = o.po.oix;
  else {
    o.po.bwsoix = bws;
    m_list[bws].po.fwsoix = o.po.oix;
  }
  if ( last) {
    m_list[fth].po.lchoix = o.po.oix;
    if ( !first) {
      o.po.fwsoix = m_list[fth].po.fchoix;
      m_list[o.po.fwsoix].po.bwsoix = o.po.oix;
    }
    else {
      // Single child
      o.po.fwsoix = o.po.oix;
      o.po.bwsoix = o.po.oix;
    }
  }
  if ( element->HasChildren && load_children)
    o.po.flags |= procom_obj_mFlags_Loaded;
  else if ( !element->HasChildren)
    o.po.flags |= procom_obj_mFlags_Loaded;

  m_list.push_back( o);

  if ( opc_get_property( element->Properties, opc_mProperty_EuType, &valp)) {
    if ( ((xsd__string *)valp)->__item == "analog")
      m_list[o.po.oix].po.flags |= procom_obj_mFlags_Analog;
  }
  if ( m_list[o.po.oix].po.flags & procom_obj_mFlags_Analog) {
    // Get analog properties
    _s0__GetProperties get_properties;
    _s0__GetPropertiesResponse properties_response;
    s0__ItemIdentifier id;      
    pwr_tFloat32 high_eu = 0;
    pwr_tFloat32 low_eu = 0;
    pwr_tString16 engineering_units = "";

    id.ItemName = new std::string( o.item_name);
    
    get_properties.ItemIDs.push_back( &id);

    opc_mask_to_propertynames( get_properties.PropertyNames, 
			       opc_mProperty_HighEU | opc_mProperty_LowEU |
			       opc_mProperty_EngineeringUnits);
    
    if ( soap_call___s0__GetProperties( &soap, opc_endpoint, NULL, &get_properties, 
					 &properties_response) == SOAP_OK) {
      if ( properties_response.PropertyLists.size() > 0 &&
	   properties_response.PropertyLists[0]->Properties.size() > 0) {
	  	
	if ( opc_get_property( properties_response.PropertyLists[0]->Properties, 
			       opc_mProperty_HighEU, &valp)) {
	  high_eu = ((xsd__double *)valp)->__item;
	}
	if ( opc_get_property( properties_response.PropertyLists[0]->Properties, 
			       opc_mProperty_LowEU, &valp)) {
	  low_eu = ((xsd__double *)valp)->__item;
	}
	if ( opc_get_property( properties_response.PropertyLists[0]->Properties, 
			       opc_mProperty_EngineeringUnits, &valp)) {
	  strncpy( engineering_units, ((xsd__string *)valp)->__item.c_str(), 
				       sizeof(engineering_units));
	}
	
	void *body = m_list[o.po.oix].po.body;
	switch ( o.po.cid) {
	case pwr_cClass_Opc_Float:
	  ((pwr_sClass_Opc_Float *)body)->HighEU = high_eu;
	  ((pwr_sClass_Opc_Float *)body)->LowEU = low_eu;
	  strcpy( ((pwr_sClass_Opc_Float *)body)->EngineeringUnits, engineering_units);
	  break;
	case pwr_cClass_Opc_Double:
	  ((pwr_sClass_Opc_Float *)body)->HighEU = high_eu;
	  ((pwr_sClass_Opc_Float *)body)->LowEU = low_eu;
	  strcpy( ((pwr_sClass_Opc_Float *)body)->EngineeringUnits, engineering_units);
	  break;
	default: ;
	}
      }
    }
  }


  if ( load_children) {
    _s0__Browse browse;
    _s0__BrowseResponse browse_response;

    browse.ItemName = new std::string( o.item_name);
    opc_mask_to_propertynames( browse.PropertyNames, 
			       opc_mProperty_DataType | opc_mProperty_Description |
			       opc_mProperty_EuType);

    if ( soap_call___s0__Browse( &soap, opc_endpoint, NULL, &browse, &browse_response) ==
	 SOAP_OK) {
      pwr_tOix next_bws;
      pwr_tOix bws = 0;

      for ( int i = 0; i < (int)browse_response.Elements.size(); i++) {
	next_bws = next_oix;
	insert_object( o.po.oix, bws, browse_response.Elements[i],
		       i == 0, i == (int)browse_response.Elements.size() - 1, 0, 0);
	bws = next_bws;
      }
    }
  }
}

void opc_provider::objectOid( co_procom *pcom, pwr_tOix oix)
{
  if ( m_list.size() == 0) {
    // Insert volume object
    opcprv_obj vo;

    vo.po.cid = pwr_eClass_ExternVolume;
    strcpy( vo.po.name, opc_vname);
    vo.po.body_size = sizeof(pwr_sExternVolume);
    vo.po.body = calloc( 1, vo.po.body_size);
    vo.po.oix = 0;
    vo.po.flags |= procom_obj_mFlags_Loaded;
    m_list.push_back( vo);

    // Insert ServerState object
    opcprv_obj so;
    so.po.cid = pwr_cClass_Opc_ServerState;
    strcpy( so.po.name, "OpcServerState");
    so.po.body_size = sizeof(pwr_sClass_Opc_ServerState);
    so.po.body = calloc( 1, so.po.body_size);
    memcpy( so.po.body, &server_state, so.po.body_size);
    so.po.oix = next_oix++;
    so.po.fthoix = 0;
    so.po.bwsoix = so.po.oix;
    so.po.fwsoix = so.po.oix;
    m_list[0].po.fchoix = so.po.oix;
    m_list[0].po.lchoix = so.po.oix;
    so.po.flags |= procom_obj_mFlags_Loaded;
    m_list.push_back( so);

    // Load Rootlist
    _s0__Browse browse;
    _s0__BrowseResponse browse_response;

    opc_mask_to_propertynames( browse.PropertyNames, 
			       opc_mProperty_DataType | opc_mProperty_Description |
			       opc_mProperty_EuType);
    if ( soap_call___s0__Browse( &soap, opc_endpoint, NULL, &browse, &browse_response) ==
	 SOAP_OK) {
      pwr_tOix bws = so.po.oix;
      pwr_tOix next_bws;
      for ( int i = 0; i < (int)browse_response.Elements.size(); i++) {
	next_bws = next_oix;
	insert_object( oix, bws, browse_response.Elements[i],
		       0, i == (int)browse_response.Elements.size() - 1, 1, 0);
	bws = next_bws;
      }
    }
    else
      soap_print_fault( &soap, stderr);
  }
  else if ( oix < m_list.size()) {
    if ( !(m_list[oix].po.flags & procom_obj_mFlags_Loaded)) {
      _s0__Browse browse;
      _s0__BrowseResponse browse_response;

      browse.ItemName = new std::string( m_list[oix].item_name);
      opc_mask_to_propertynames( browse.PropertyNames, 
				 opc_mProperty_DataType | opc_mProperty_Description |
				 opc_mProperty_EuType);
      
      if ( soap_call___s0__Browse( &soap, opc_endpoint, NULL, &browse, &browse_response) ==
	   SOAP_OK) {
	pwr_tOix next_bws;
	pwr_tOix bws = 0;

	if ( browse_response.Errors.size() > 0) {
	  errlog( browse.ItemName, browse_response.Errors);
	}
	for ( int i = 0; i < (int)browse_response.Elements.size(); i++) {
	  next_bws = next_oix;
	  insert_object( oix, bws, browse_response.Elements[i],
			 i == 0, i == (int)browse_response.Elements.size() - 1, 0, 0);
	  bws = next_bws;
	}
      }
      m_list[oix].po.flags |= procom_obj_mFlags_Loaded;
    }
  }

  if ( oix >= m_list.size() || oix < 0) {
    pcom->provideStatus( GDH__NOSUCHOBJ);
    return;
  }

#if 0
  for ( int i = 0; i < (int)m_list.size(); i++) {
    printf( "oix %2d bws %2d fws %2d fth %2d fch %2d lch %2d flags %lu %s\n", 
	    m_list[i].po.oix, m_list[i].po.bwsoix, m_list[i].po.fwsoix, m_list[i].po.fthoix, 
	    m_list[i].po.fchoix, m_list[i].po.lchoix, m_list[i].po.flags, m_list[i].po.name);
  }
#endif
  vector<procom_obj>olist;
  for ( int i = 0; i < (int) m_list.size(); i++) {
    if  ( m_list[i].po.flags & procom_obj_mFlags_Loaded)
      olist.push_back( m_list[i].po);
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
    if  ( !m_list[i].po.flags & procom_obj_mFlags_Deleted) {
      if ( cdh_NoCaseStrcmp( name, longname(m_list[i].po.oix)) == 0) {
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

  if ( offset + size > m_list[oix].po.body_size) {
    pcom->provideStatus( LDH__NOSUCHATTR);
    return;
  }

  memcpy( (void *)((unsigned long)m_list[oix].po.body + (unsigned long)offset), buffer, size);
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

  if ( offset + size > m_list[oix].po.body_size) {
    pcom->provideStatus( GDH__NOSUCHOBJ);
    return;
  }

  switch ( m_list[oix].po.cid) {
  case pwr_cClass_Opc_Float:
    if ( offset == (unsigned int) ((char *) &((pwr_sClass_Opc_Float *)m_list[oix].po.body)->Value -
		    (char *)m_list[oix].po.body)) {      
      _s0__Read read;
      _s0__ReadResponse read_response;

      s0__ReadRequestItem *item = new s0__ReadRequestItem();      
      item->ItemName = new std::string( m_list[oix].item_name);
      read.ItemList = new s0__ReadRequestItemList;
      read.ItemList->Items.push_back( item);
				   
      if ( soap_call___s0__Read( &soap, opc_endpoint, NULL, &read, &read_response) ==
	   SOAP_OK) {
	if ( read_response.RItemList && read_response.RItemList->Items.size() > 0) {
	  // printf( "Read Value: \"%s\"\n", read_response.RItemList->Items[0]->Value);
	}
      }
    }
    break;
  case pwr_cClass_Opc_String:
    if ( offset == (unsigned int) ((char *) ((pwr_sClass_Opc_String *)m_list[oix].po.body)->Value -
		    (char *)m_list[oix].po.body)) {      
      _s0__Read read;
      _s0__ReadResponse read_response;

      s0__ReadRequestItem *item = new s0__ReadRequestItem();      
      item->ItemName = new std::string( m_list[oix].item_name);
      read.ItemList = new s0__ReadRequestItemList;
      read.ItemList->Items.push_back( item);
				   
      if ( soap_call___s0__Read( &soap, opc_endpoint, NULL, &read, &read_response) ==
	   SOAP_OK) {
	if ( read_response.RItemList && read_response.RItemList->Items.size() > 0) {
#if 0
	  strncpy( ((pwr_sClass_Opc_String *)m_list[oix].po.body)->Value, 
		   read_response.RItemList->Items[0]->Value,
		   sizeof(((pwr_sClass_Opc_String *)m_list[oix].po.body)->Value));
	  printf( "Read Value: \"%s\"\n", read_response.RItemList->Items[0]->Value);
#endif
	}
      }
    }
    break;
  }

  void *p = (void *)((unsigned long)m_list[oix].po.body + (unsigned long)offset);
  pcom->provideAttr( GDH__SUCCESS, oix, size, p);
}

// Rt only
void opc_provider::subAssociateBuffer( co_procom *pcom, void **buff, int oix, int offset, 
				      int size, pwr_tSubid sid) 
{
  if ( oix >= (int)m_list.size()) {
    *buff = 0;
    return;
  }

  *buff = (char *)m_list[oix].po.body + offset;

  switch ( m_list[oix].po.cid) {
  case pwr_cClass_Opc_String:
  case pwr_cClass_Opc_Boolean:
  case pwr_cClass_Opc_Float:
  case pwr_cClass_Opc_Double:
  case pwr_cClass_Opc_Decimal:
  case pwr_cClass_Opc_Long:
  case pwr_cClass_Opc_Int:
  case pwr_cClass_Opc_Short:
  case pwr_cClass_Opc_Byte:
  case pwr_cClass_Opc_UnsignedLong:
  case pwr_cClass_Opc_UnsignedInt:
  case pwr_cClass_Opc_UnsignedShort:
  case pwr_cClass_Opc_UnsignedByte:
  case pwr_cClass_Opc_Base64Binary:
  case pwr_cClass_Opc_Time:
  case pwr_cClass_Opc_Date:
  case pwr_cClass_Opc_Duration:
  case pwr_cClass_Opc_QName:
    if ( *buff == ((pwr_sClass_Opc_String *)m_list[oix].po.body)->Value) {
      // Add opc subscription
      _s0__Subscribe subscribe;
      _s0__SubscribeResponse subscribe_response;
      char handle[20];

      subscribe.Options = new s0__RequestOptions();
      subscribe.Options->ReturnItemTime = (bool *) malloc( sizeof(bool));
      *subscribe.Options->ReturnItemTime = true;

      subscribe.ItemList = new s0__SubscribeRequestItemList();
      s0__SubscribeRequestItem *ritem = new s0__SubscribeRequestItem();
      ritem->ItemName = new std::string( m_list[oix].item_name);
      sprintf( handle, "%d", oix);
      ritem->ClientItemHandle = new std::string( handle);
      ritem->RequestedSamplingRate = (int *) malloc( sizeof(int));
      *ritem->RequestedSamplingRate = 1000;

      subscribe.ItemList->Items.push_back( ritem);

      if ( soap_call___s0__Subscribe( &soap, opc_endpoint, NULL, &subscribe, &subscribe_response) ==
	 SOAP_OK) {
	opcprv_sub sub;

	sub.handle = *subscribe_response.ServerSubHandle;
	sub.oix = oix;
	m_sublist[sid.rix] = sub;

	if ( subscribe_response.RItemList && subscribe_response.RItemList->Items.size()) {
	  for ( int i = 0; i < (int)subscribe_response.RItemList->Items.size(); i++) {
	    // subscribe_response.RItemList->Items[i]->ItemValue...
	  }
	}
      }
    }
    break;
  default: ;
  }
}

// Rt only
void opc_provider::subDisassociateBuffer( co_procom *pcom, pwr_tSubid sid) 
{
  sublist_iterator it = m_sublist.find( sid.rix);
  if ( it != m_sublist.end()) {
    // Cancel subscription
    _s0__SubscriptionCancel subcancel;
    _s0__SubscriptionCancelResponse subcancel_response;

    subcancel.ServerSubHandle = new std::string(it->second.handle);

    if ( soap_call___s0__SubscriptionCancel( &soap, opc_endpoint, NULL, &subcancel, &subcancel_response) ==
	 SOAP_OK) {
      // Where are the fault codes ???
    }

    m_sublist.erase( it);
  }
}

// Rt only
void opc_provider::cyclic( co_procom *pcom)
{
  int size = 0;
  _s0__SubscriptionPolledRefresh subpoll;
  _s0__SubscriptionPolledRefreshResponse subpoll_response;
  
  
  for ( sublist_iterator it = m_sublist.begin(); it != m_sublist.end(); it++) {
    subpoll.ServerSubHandles.push_back( it->second.handle);
    size++;
  }

  if ( size) {
    if ( soap_call___s0__SubscriptionPolledRefresh( &soap, opc_endpoint, NULL, &subpoll, &subpoll_response) ==
	 SOAP_OK) {
      if ( (int) subpoll_response.RItemList.size() != size) {
	return;
      }

      int idx = 0;
      for ( sublist_iterator it = m_sublist.begin(); it != m_sublist.end(); it++) {
	if ( subpoll_response.RItemList[idx]->Items.size()) { 
	  switch ( m_list[it->second.oix].po.cid) {
	  case pwr_cClass_Opc_String:
	    strcpy( ((pwr_sClass_Opc_String *)m_list[it->second.oix].po.body)->Value,
		    ((xsd__string *)subpoll_response.RItemList[idx]->Items[0]->Value)->__item.c_str());
	    break;
	  default: ;
	  }
	}
	idx++;
      }
      
    }
  }
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
  m_list[oix].po.flags = procom_obj_mFlags_Deleted;

  for ( pwr_tOix ix = m_list[oix].po.fchoix;
	ix;
	ix = m_list[ix].po.fwsoix)
    delete_tree( ix);
}

char *opc_provider::longname( pwr_tOix oix)
{
  if ( m_list[oix].po.fthoix == 0)
    strcpy( m_list[oix].po.lname, m_list[oix].po.name);
  else {
    strcpy( m_list[oix].po.lname, longname( m_list[oix].po.fthoix));
    strcat( m_list[oix].po.lname, "-");
    strcat( m_list[oix].po.lname, m_list[oix].po.name);
  }
  return m_list[oix].po.lname;
}

bool opc_provider::find( pwr_tOix fthoix, char *name, pwr_tOix *oix)
{

  for ( int i = 0; i < (int) m_list.size(); i++) {
    if  ( !m_list[i].po.flags & procom_obj_mFlags_Deleted) {
      if ( m_list[i].po.fthoix == fthoix && 
	   cdh_NoCaseStrcmp( name, m_list[i].po.name) == 0) {
	*oix = m_list[i].po.oix;
	return true;
      }
    }
  }
  return false;
}

void opc_provider::get_server_state()
{
  _s0__GetStatus get_status;
  _s0__GetStatusResponse get_status_response;
  get_status.ClientRequestHandle = new std::string("Opc Client");

  if ( soap_call___s0__GetStatus( &soap, opc_endpoint, NULL, &get_status, &get_status_response) ==
       SOAP_OK) {
    if ( get_status_response.Status->VendorInfo)
      strcpy( server_state.VendorInfo, get_status_response.Status->VendorInfo->c_str());
    if ( get_status_response.Status->ProductVersion)
      strcpy( server_state.ProductVersion, get_status_response.Status->ProductVersion->c_str());
    // TODO server_state.StartTime.tv_sec = get_status_response.Status->StartTime;
    server_state.ServerState = get_status_response.GetStatusResult->ServerState;
  }
  else {
    server_state.ServerState = s0__serverState__commFault;    
    soap_print_fault( &soap, stderr);
  }
}

//
// Create a valid object name from an item name
//
char *opc_provider::name_to_objectname( char *name)
{
  static char n[32];
  char *s, *t;

  for ( s = name, t = n; *s; s++) {
    if ( t - n >= (int)sizeof(n) - 1)
      break;

    if ( *s == '[' || *s == ']' || *s == '-' || *s == '/' || *s == '.')
      *t = '$';
    else
      *t = *s;
    t++;
  }
  *t = 0;
  return n;
}

void opc_provider::errlog( std::string* item, std::vector<s0__OPCError *>& errvect)
{
  for ( int i = 0; i < (int) errvect.size(); i++)
    printf( "OPC Error: %s  %s\n", item->c_str(), errvect[i]->ID.c_str());
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

