/* 
 * Proview   $Id: opc_server.cpp,v 1.5 2007-03-13 15:48:41 claes Exp $
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

#include <net/if.h>
#include <net/if_arp.h>

#include "pwr.h"
#include "pwr_version.h"
#include "pwr_baseclasses.h"
#include "pwr_opcclasses.h"
#include "co_cdh.h"
#include "rt_gdh.h"
#include "opc_utl.h"
#include "opc_soap_H.h"
#include "Service.nsmap"

typedef struct {
  int address;
  int access;
} opc_sClientAccess;

static pwr_tTime opc_start_time;
static pwr_sClass_Opc_ServerConfig *opc_config;
static opc_sClientAccess opc_client_access[20];
static int opc_client_access_cnt = 0;
static int opc_current_access;


int main()
{
  struct soap soap;
  int m,s;   // Master and slave sockets
  pwr_tStatus sts;

  sts = gdh_Init("opc_server");
  if ( EVEN(sts)) {
    exit(sts);
  }

#if 0
  pwr_tOid config_oid;

  // Get OpcServerConfig object
  sts = gdh_GetClassList( pwr_cClass_Opc_ServerConfig, &config_oid);
  if ( EVEN(sts)) {
    // Not configured
    exit(0);
  }

  sts = gdh_ObjidToPointer( config_oid, &opc_config);
  if ( EVEN(sts)) {
    exit(0);
  }
#endif  

  // Test
  pwr_sClass_Opc_ServerConfig sc;
  memset( &sc, 0, sizeof(sc));
  strcpy( sc.ClientAccess[0].Address, "192.168.62.186");
  sc.ClientAccess[0].Access = pwr_eOpc_AccessEnum_ReadWrite;
  opc_config = &sc;
  // End Test

  for ( int i = 0; 
	i < (int)(sizeof(opc_config->ClientAccess)/sizeof(opc_config->ClientAccess[0])); 
	i++) {
    if ( strcmp( opc_config->ClientAccess[i].Address, "") != 0) {
      opc_client_access[opc_client_access_cnt].address = 
	inet_network( opc_config->ClientAccess[i].Address);
      if ( opc_client_access[opc_client_access_cnt].address != -1) {
	opc_client_access[opc_client_access_cnt].access = opc_config->ClientAccess[i].Access;
	opc_client_access_cnt++;
      }
    }      
  }

  clock_gettime( CLOCK_REALTIME, &opc_start_time);

  soap_init( &soap);
  m = soap_bind( &soap, NULL, 18083, 100);
  if ( m < 0)
    soap_print_fault( &soap, stderr);
  else {
    fprintf( stderr, "Socket connection successfull: master socket = %d\n", m);

    for ( int i = 1;; i++) {
      s = soap_accept( &soap);
      if ( s < 0) {
	soap_print_fault( &soap, stderr);
	break;
      }

      fprintf( stderr, "%d: accepted connection from IP=%lu.%lu.%lu.%lu socket=%d", i,
	       (soap.ip>>24)&0xFF,(soap.ip>>16)&0xFF,(soap.ip>>8)&0xFF,soap.ip&0xFF, s);

      if ( soap_serve( &soap) != SOAP_OK)         // Process RPC request
	soap_print_fault( &soap, stderr);
      fprintf( stderr, "Request served\n");
      soap_destroy( &soap);   // Clean up class instances
      soap_end( &soap);       // Clean up everything and close socket
    }
  }

  soap_done( &soap);     // Close master socket and detach environment
  return 0;
}



SOAP_FMAC5 int SOAP_FMAC6 __ns1__GetStatus(struct soap*, 
					   _ns1__GetStatus *ns1__GetStatus, 
					   _ns1__GetStatusResponse *ns1__GetStatusResponse)
{
  pwr_tTime current_time;

  clock_gettime( CLOCK_REALTIME, &current_time);

  ns1__GetStatusResponse->GetStatusResult = new ns1__ReplyBase();
  ns1__GetStatusResponse->GetStatusResult->RcvTime = current_time.tv_sec;
  ns1__GetStatusResponse->GetStatusResult->ReplyTime = current_time.tv_sec;
  ns1__GetStatusResponse->GetStatusResult->RevisedLocaleID = new std::string( "en");
  ns1__GetStatusResponse->GetStatusResult->ServerState = ns1__serverState__running;
  ns1__GetStatusResponse->GetStatusResult->ClientRequestHandle = ns1__GetStatus->ClientRequestHandle;

  ns1__GetStatusResponse->Status = new ns1__ServerStatus();
  ns1__GetStatusResponse->Status->VendorInfo = new std::string("Proview " pwrv_cPwrVersionStr);
  ns1__GetStatusResponse->Status->SupportedInterfaceVersions.push_back( ns1__interfaceVersion__XML_USCOREDA_USCOREVersion_USCORE1_USCORE0);
  ns1__GetStatusResponse->Status->StartTime = opc_start_time.tv_sec;
  ns1__GetStatusResponse->Status->ProductVersion = new std::string( pwrv_cPwrVersionStr);

  return 0;
}

SOAP_FMAC5 int SOAP_FMAC6 __ns1__Read(struct soap*, 
				      _ns1__Read *ns1__Read, 
				      _ns1__ReadResponse *ns1__ReadResponse)
{
  return 0;
}

SOAP_FMAC5 int SOAP_FMAC6 __ns1__Write(struct soap*, 
				       _ns1__Write *ns1__Write, 
				       _ns1__WriteResponse *ns1__WriteResponse)
{
  return 0;
}

SOAP_FMAC5 int SOAP_FMAC6 __ns1__Subscribe(struct soap*, 
					   _ns1__Subscribe *ns1__Subscribe, 
					   _ns1__SubscribeResponse *ns1__SubscribeResponse)
{
  return 0;
}

SOAP_FMAC5 int SOAP_FMAC6 __ns1__SubscriptionPolledRefresh(struct soap*, 
							   _ns1__SubscriptionPolledRefresh *ns1__SubscriptionPolledRefresh, 
							   _ns1__SubscriptionPolledRefreshResponse *ns1__SubscriptionPolledRefreshResponse)
{
  return 0;
}

SOAP_FMAC5 int SOAP_FMAC6 __ns1__SubscriptionCancel(struct soap*, 
						    _ns1__SubscriptionCancel *ns1__SubscriptionCancel, 
						    _ns1__SubscriptionCancelResponse *ns1__SubscriptionCancelResponse)
{
  return 0;
}

bool opcsrv_get_properties( bool is_item, pwr_tCid pcid, pwr_tAttrRef *parp, 
			    pwr_tAttrRef *arp, unsigned int propmask, gdh_sAttrDef *bd,
			    std::vector<ns1__ItemProperty *>& properties)
{
  pwr_tStatus sts;

  if ( !is_item) {
    if ( propmask & opc_mProperty_Description) {
      pwr_tAttrRef aaref;
      pwr_tString80 desc;

      sts = gdh_ArefANameToAref( arp, "Description", &aaref);
      if ( ODD(sts)) {
	sts = gdh_GetObjectInfoAttrref( &aaref, desc, sizeof(desc));
	if ( ODD(sts)) {
	  ns1__ItemProperty *ip = new ns1__ItemProperty();
	  ip->Name = std::string("description");
	  ip->Value = (char *) malloc( sizeof(pwr_tString80));
	  strncpy( ip->Value, desc, sizeof(pwr_tString80));
	  strcpy( ip->ValueType, "xsd:string");
	  properties.push_back( ip);
	}
      }
      if ( EVEN(sts)) {
	ns1__ItemProperty *ip = new ns1__ItemProperty();
	ip->Name = std::string("description");
	ip->Value = (char *) calloc(1, 1);
	strcpy( ip->ValueType, "xsd:string");
	properties.push_back( ip);
      }
    }
  }
  else {
    // IsItem

    // Description
    if ( propmask & opc_mProperty_Description) {
      
      switch ( pcid) {
      case pwr_cClass_Di:
      case pwr_cClass_Do:
      case pwr_cClass_Dv:
      case pwr_cClass_Ai:
      case pwr_cClass_Ao:
      case pwr_cClass_Av:
      case pwr_cClass_Ii:
      case pwr_cClass_Io:
      case pwr_cClass_Iv: {
	if ( strcmp( bd->attrName, "ActualValue") != 0)
	  break;

	pwr_tAttrRef aaref;
	pwr_tString80 desc;
	
	// Description from signal object
	sts = gdh_ArefANameToAref( parp, "Description", &aaref);
	if ( EVEN(sts)) break;
	
	sts = gdh_GetObjectInfoAttrref( &aaref, desc, sizeof(desc));
	if ( EVEN(sts)) break;
	
	ns1__ItemProperty *ip = new ns1__ItemProperty();
	ip->Name = std::string("description");
	ip->Value = (char *) calloc( 1, sizeof(desc));
	strncpy( ip->Value, desc, sizeof(desc));
	strcpy( ip->ValueType, "xsd:string");
	properties.push_back( ip);
	break;
      }
      default: {
	ns1__ItemProperty *ip = new ns1__ItemProperty();
	ip->Name = std::string("description");
	ip->Value = (char *) calloc(1,1);
	strcpy( ip->ValueType, "xsd:string");
	properties.push_back( ip);
      }
      }

    }

    // DataType
    if ( propmask & opc_mProperty_DataType) {
      char *type_p;
      ns1__ItemProperty *ip = new ns1__ItemProperty();

      if ( opc_pwrtype_to_string( bd->attr->Param.Info.Type, &type_p)) {
	ip->Name = std::string("dataType");
	ip->Value = type_p;
	strcpy( ip->ValueType, "xsd:QName");
	properties.push_back( ip);
      }
      else {
	// Untranslatable type TODO
      }     
    }

    // Quality
    if ( propmask & opc_mProperty_Quality) {
      char *qual_p;
      ns1__ItemProperty *ip = new ns1__ItemProperty();

      if ( opc_quality_to_string( ns1__qualityBits__good, &qual_p)) {
	ip->Name = std::string("quality");
	ip->Value = qual_p;
	strcpy( ip->ValueType, "xsd:string");
	properties.push_back( ip);
      }
    }

    // Timestamp
    if ( propmask & opc_mProperty_Timestamp) {
      // TODO ...
    }

    // Access Rights
    if ( propmask & opc_mProperty_AccessRights) {
      ns1__ItemProperty *ip = new ns1__ItemProperty();

      ip->Name = std::string("accessRights");
      ip->Value = (char *) malloc( 20);
      strcpy( ip->ValueType, "xsd:string");

      switch ( opc_current_access) {
      case pwr_eOpc_AccessEnum_ReadOnly:
	strcpy( ip->Value, "readable");
	break;
      case pwr_eOpc_AccessEnum_ReadWrite:
	if ( bd->attr->Param.Info.Flags & PWR_MASK_RTVIRTUAL ||
	     bd->attr->Param.Info.Flags & PWR_MASK_PRIVATE)
	  strcpy( ip->Value, "readable");
	else
	    strcpy( ip->Value, "readWriteable");
	break;
      default:
	strcpy( ip->Value, "unknown");
	break;
      }
      properties.push_back( ip);
    }

    // EngineeringUnits
    if ( propmask & opc_mProperty_EngineeringUnits) {
      if ( parp) {

	switch ( pcid) {
	case pwr_cClass_Ai:
	case pwr_cClass_Ao:
	case pwr_cClass_Av: {
	  if ( strcmp( bd->attrName, "ActualValue") != 0)
	    break;

	  pwr_tAttrRef aaref;
	  pwr_tString16 unit;

	  // Get Range from channel
	  sts = gdh_ArefANameToAref( parp, "Unit", &aaref);
	  if ( EVEN(sts)) break;
	  
	  sts = gdh_GetObjectInfoAttrref( &aaref, &unit, sizeof(unit));
	  if ( EVEN(sts)) break;

	  ns1__ItemProperty *ip = new ns1__ItemProperty();
	  ip->Name = std::string("engineeringUnits");
	  ip->Value = (char *) malloc( 20);
	  strcpy( ip->Value, unit);
	  strcpy( ip->ValueType, "xsd:string");
	  properties.push_back( ip);
	  break;
	}
	default: ;
 	}
      }
    }

    // EuType
    if ( propmask & opc_mProperty_EuType) {
      switch( bd->attr->Param.Info.Type) {
      case pwr_eType_Float32: {
	ns1__ItemProperty *ip = new ns1__ItemProperty();
	ip->Name = std::string("euType");
	ip->Value = (char *) malloc( 20);
	strcpy( ip->Value, "analog");
	strcpy( ip->ValueType, "xsd:string");
	properties.push_back( ip);
	break;
      }
      default: {
	ns1__ItemProperty *ip = new ns1__ItemProperty();
	ip->Name = std::string("euType");
	ip->Value = (char *) malloc( 20);
	strcpy( ip->Value, "noEnum");
	strcpy( ip->ValueType, "xsd:string");
	properties.push_back( ip);
	break;
      }
      }
    }

    
    // HighEU
    if ( propmask & opc_mProperty_HighEU) {
      if ( parp) {
	pwr_tAttrRef aaref;
	pwr_tFloat32 fval;

	switch ( pcid) {
	case pwr_cClass_Av:
	case pwr_cClass_Ai:
	case pwr_cClass_Ao:
	  sts = gdh_ArefANameToAref( parp, "PresMaxLimit", &aaref);
	  if ( ODD(sts)) {
	    sts = gdh_GetObjectInfoAttrref( &aaref, &fval, sizeof(fval));
	    if ( ODD(sts)) {
	      ns1__ItemProperty *ip = new ns1__ItemProperty();
	      ip->Name = std::string("highEU");
	      ip->Value = (char *) malloc( 20);
	      sprintf( ip->Value, "%5.2f", fval);
	      strcpy( ip->ValueType, "xsd:double");
	      properties.push_back( ip);
	    }
	  }
	  break;
	default: ;
 	}
      }
    }

    // LowEU
    if ( propmask & opc_mProperty_LowEU) {
      if ( parp) {
	pwr_tAttrRef aaref;
	pwr_tFloat32 fval;

	switch ( pcid) {
	case pwr_cClass_Av:
	case pwr_cClass_Ai:
	case pwr_cClass_Ao:
	  sts = gdh_ArefANameToAref( parp, "PresMinLimit", &aaref);
	  if ( ODD(sts)) {
	    sts = gdh_GetObjectInfoAttrref( &aaref, &fval, sizeof(fval));
	    if ( ODD(sts)) {
	      ns1__ItemProperty *ip = new ns1__ItemProperty();
	      ip->Name = std::string("lowEU");
	      ip->Value = (char *) malloc( 20);
	      sprintf( ip->Value, "%5.2f", fval);
	      strcpy( ip->ValueType, "xsd:double");
	      properties.push_back( ip);
	    }
	  }
	  break;
	default: ;
 	}
      }
    }

    // HighIR
    if ( propmask & opc_mProperty_HighIR) {
      if ( parp) {

	switch ( pcid) {
	case pwr_cClass_Ai:
	case pwr_cClass_Ii:
	case pwr_cClass_Ao:
	case pwr_cClass_Io: {
	  if ( strcmp( bd->attrName, "ActualValue") != 0)
	    break;

	  pwr_tAttrRef aaref;
	  pwr_tAttrRef sigchancon;
	  pwr_tFloat32 fval;

	  // Get Range from channel
	  sts = gdh_ArefANameToAref( parp, "SigChanCon", &aaref);
	  if ( EVEN(sts)) break;
	  
	  sts = gdh_GetObjectInfoAttrref( &aaref, &sigchancon, sizeof(sigchancon));
	  if ( EVEN(sts)) break;

	  if ( cdh_ObjidIsNull( sigchancon.Objid))
	    break;

	  sts = gdh_ArefANameToAref( &sigchancon, "ActValRangeHigh", &aaref);
	  if ( EVEN(sts)) break;

	  sts = gdh_GetObjectInfoAttrref( &aaref, &fval, sizeof(fval));
	  if ( EVEN(sts)) break;

	  ns1__ItemProperty *ip = new ns1__ItemProperty();
	  ip->Name = std::string("highIR");
	  ip->Value = (char *) malloc( 20);
	  sprintf( ip->Value, "%5.2f", fval);
	  strcpy( ip->ValueType, "xsd:double");
	  properties.push_back( ip);
	  break;
	}
	default: ;
 	}
      }
    }

    // LowIR
    if ( propmask & opc_mProperty_LowIR) {
      if ( parp) {
	
	switch ( pcid) {
	case pwr_cClass_Ai:
	case pwr_cClass_Ii:
	case pwr_cClass_Ao:
	case pwr_cClass_Io: {
	  if ( strcmp( bd->attrName, "ActualValue") != 0)
	    break;

	  pwr_tAttrRef sigchancon;
	  pwr_tAttrRef aaref;
	  pwr_tFloat32 fval;

	  // Get Range from channel
	  sts = gdh_ArefANameToAref( parp, "SigChanCon", &aaref);
	  if ( EVEN(sts)) break;
	  
	  sts = gdh_GetObjectInfoAttrref( &aaref, &sigchancon, sizeof(sigchancon));
	  if ( EVEN(sts)) break;

	  if ( cdh_ObjidIsNull( sigchancon.Objid))
	    break;

	  sts = gdh_ArefANameToAref( &sigchancon, "ActValRangeLow", &aaref);
	  if ( EVEN(sts)) break;

	  sts = gdh_GetObjectInfoAttrref( &aaref, &fval, sizeof(fval));
	  if ( EVEN(sts)) break;

	  ns1__ItemProperty *ip = new ns1__ItemProperty();
	  ip->Name = std::string("lowIR");
	  ip->Value = (char *) malloc( 20);
	  sprintf( ip->Value, "%5.2f", fval);
	  strcpy( ip->ValueType, "xsd:double");
	  properties.push_back( ip);
	  break;
	}
	default: ;
 	}
      }
    }

  }
  return true;
}

int opcsrv_get_access( struct soap *so)
{
  int access = pwr_eOpc_AccessEnum_None;
      
  for ( int i = 0; i < opc_client_access_cnt; i++) {
    if ( opc_client_access[i].address == (int)so->ip) {
      access = opc_client_access[i].access;
      break;
    }
  }
  return access;
}
  
SOAP_FMAC5 int SOAP_FMAC6 __ns1__Browse(struct soap *so, _ns1__Browse *ns1__Browse, 
					_ns1__BrowseResponse *ns1__BrowseResponse)
{
  pwr_tStatus sts;
  pwr_tOid oid, child, ch;
  pwr_tOName name;
  pwr_tCid cid;
  unsigned int property_mask;
  pwr_tTime current_time;
  unsigned int erropt = 0;

  opc_current_access = opcsrv_get_access( so);
  clock_gettime( CLOCK_REALTIME, &current_time);

  ns1__BrowseResponse->BrowseResult = new ns1__ReplyBase();
  ns1__BrowseResponse->BrowseResult->RcvTime = current_time.tv_sec;
  ns1__BrowseResponse->BrowseResult->ReplyTime = current_time.tv_sec;
  ns1__BrowseResponse->BrowseResult->ClientRequestHandle = ns1__Browse->ClientRequestHandle;
  ns1__BrowseResponse->BrowseResult->ServerState = ns1__serverState__running;

  if ( (!ns1__Browse->ItemName || ns1__Browse->ItemName->empty()) &&
       (!ns1__Browse->ItemPath || ns1__Browse->ItemPath->empty())) {
    // Return rootlist
    for ( sts = gdh_GetRootList( &oid); ODD(sts); sts = gdh_GetNextSibling( oid, &oid)) {
      sts = gdh_ObjidToName( oid, name, sizeof(name), cdh_mName_object);
      if ( EVEN(sts)) {
	opcsrv_returnerror( ns1__BrowseResponse->Errors, 0, opc_eResultCode_E_FAIL, erropt);
	return 0;
      }

      sts = gdh_GetObjectClass( oid, &cid);
      if ( EVEN(sts)) {
	opcsrv_returnerror( ns1__BrowseResponse->Errors, 0, opc_eResultCode_E_FAIL, erropt);
	return 0;
      }
      
      ns1__BrowseElement *element = new ns1__BrowseElement();
      element->Name = new std::string( name);
      element->ItemName = element->Name;
      element->IsItem = false;
      if ( cid == pwr_eClass_PlantHier || cid == pwr_eClass_NodeHier)
	element->HasChildren = ODD( gdh_GetChild( child, &ch)) ? true : false;
      else
	element->HasChildren = true;

      if ( ns1__Browse->ReturnAllProperties)
	property_mask = ~0;
      else
	opc_propertynames_to_mask( ns1__Browse->PropertyNames, &property_mask);

      pwr_tAttrRef aref = cdh_ObjidToAref( oid);
      opcsrv_get_properties( false, cid, 0, &aref, 
			     property_mask, 0,
			     element->Properties);

      ns1__BrowseResponse->Elements.push_back( element);
    }
  }
  else {
    // Return attributes and children
    pwr_tOName pname;
    pwr_tOName itemname;
    pwr_tObjName aname;
    gdh_sAttrDef *bd;
    int 	rows;
    pwr_sAttrRef paref;
    pwr_sAttrRef aref;
    
    if ( ns1__Browse->ItemPath && !ns1__Browse->ItemPath->empty()) {
      strncpy( pname, ns1__Browse->ItemPath->c_str(), sizeof( pname));
      if ( ns1__Browse->ItemName && !ns1__Browse->ItemName->empty()) {
	strcat( pname, ns1__Browse->ItemName->c_str());
      }
    }
    else
      strncpy( pname, ns1__Browse->ItemName->c_str(), sizeof(pname));

    sts = gdh_NameToAttrref( pwr_cNOid, pname, &paref);
    if ( EVEN(sts)) {
      opcsrv_returnerror( ns1__BrowseResponse->Errors, 0, opc_eResultCode_E_UNKNOWNITEMNAME, erropt);
      return 0;
    }

    sts = gdh_GetAttrRefTid( &paref, &cid);
    if ( EVEN(sts)) {
      opcsrv_returnerror( ns1__BrowseResponse->Errors, 0, opc_eResultCode_E_FAIL, erropt);
      return 0;
    }

    if ( paref.Flags.b.Array) {
      // Return all elements
      pwr_tTypeId a_type;
      unsigned int a_size, a_offs, a_dim;
      pwr_tAttrRef oaref;
      char *s;
      char *attrname;


      sts = gdh_AttrArefToObjectAref( &paref, &oaref);
      if ( EVEN(sts)) {
	opcsrv_returnerror( ns1__BrowseResponse->Errors, 0, opc_eResultCode_E_FAIL, erropt);
	return 0;
      }

      sts = gdh_GetAttrRefTid( &oaref, &cid);
      if ( EVEN(sts)) {
	opcsrv_returnerror( ns1__BrowseResponse->Errors, 0, opc_eResultCode_E_FAIL, erropt);
	return 0;
      }

      if ( !( attrname = strrchr( pname, '.'))) {
	opcsrv_returnerror( ns1__BrowseResponse->Errors, 0, opc_eResultCode_E_FAIL, erropt);
	return 0;
      }
      attrname++;

      // Get body definition
      sts = gdh_GetObjectBodyDef( cid, &bd, &rows, pwr_cNOid);
      if ( EVEN(sts)) {
	opcsrv_returnerror( ns1__BrowseResponse->Errors, 0, opc_eResultCode_E_FAIL, erropt);
	return 0;
      }
      int bd_idx = -1;
      for ( int i = 0; i < rows; i++) {
	if ( cdh_NoCaseStrcmp( attrname, bd[i].attrName) == 0) {
	  bd_idx = i;
	  break;
	}
      }
      if ( bd_idx == -1) {
	// E_INVALIDITEMNAME
	opcsrv_returnerror( ns1__BrowseResponse->Errors, 0, opc_eResultCode_E_FAIL, erropt);
	free( (char *)bd);
	return 0;
      }

      sts = gdh_GetAttributeCharAttrref( &paref, &a_type, &a_size, &a_offs, &a_dim);
      if ( EVEN(sts)) {
	opcsrv_returnerror( ns1__BrowseResponse->Errors, 0, opc_eResultCode_E_FAIL, erropt);
	return 0;
      }

      for ( int i = 0; i < (int)a_dim; i++) {
	ns1__BrowseElement *element = new ns1__BrowseElement();

	sprintf( itemname, "%s[%d]", pname, i);
	s = strrchr( itemname, '.');
	if ( s)
	  strcpy( aname, s + 1);
	else {
	  opcsrv_returnerror( ns1__BrowseResponse->Errors, 0, opc_eResultCode_E_FAIL, erropt);
	  return 0;
	}
	element->Name = new std::string( aname);
	element->ItemName = new std::string( itemname);
	element->IsItem = true;
	element->HasChildren = false;

	if ( ns1__Browse->ReturnAllProperties)
	  property_mask = ~0;
	else
	  opc_propertynames_to_mask( ns1__Browse->PropertyNames, &property_mask);

	if ( property_mask) {
	  sts = gdh_NameToAttrref( pwr_cNOid, itemname, &aref);
	  if ( EVEN(sts)) {
	    opcsrv_returnerror( ns1__BrowseResponse->Errors, 0, opc_eResultCode_E_FAIL, erropt);
	    return 0;
	  }
	  
	  opcsrv_get_properties( true, cid, &paref, &aref,
				 property_mask, &bd[bd_idx],
				 element->Properties);

	}
	ns1__BrowseResponse->Elements.push_back( element);
      }
      free( (char *)bd);

      return 0;      
    }

    if ( !cdh_tidIsCid( cid)) {
      opcsrv_returnerror( ns1__BrowseResponse->Errors, 0, opc_eResultCode_E_FAIL, erropt);
      return 0;
    }

    sts = gdh_GetObjectBodyDef( cid, &bd, &rows, pwr_cNOid);
    if ( ODD(sts)) {

      for ( int i = 0; i < rows; i++) {
	if ( bd[i].flags & gdh_mAttrDef_Shadowed)
	  continue;
	if ( bd[i].attr->Param.Info.Flags & PWR_MASK_RTVIRTUAL || 
	     bd[i].attr->Param.Info.Flags & PWR_MASK_PRIVATE)
	  continue;
	if ( bd[i].attr->Param.Info.Type == pwr_eType_CastId ||
	     bd[i].attr->Param.Info.Type == pwr_eType_DisableAttr)
	  continue;
	if ( bd[i].attr->Param.Info.Flags & PWR_MASK_RTHIDE)
	  continue;
	
	sts = gdh_ArefANameToAref( &paref, bd[i].attrName, &aref);
	if ( EVEN(sts)) {
	  opcsrv_returnerror( ns1__BrowseResponse->Errors, 0, opc_eResultCode_E_FAIL, erropt);
	  return 0;
	}
	if ( bd[i].attr->Param.Info.Flags & PWR_MASK_DISABLEATTR) {
	  pwr_tDisableAttr disabled;

	  sts = gdh_ArefDisabled( &aref, &disabled);
	  if ( EVEN(sts)) {
	    opcsrv_returnerror( ns1__BrowseResponse->Errors, 0, opc_eResultCode_E_FAIL, erropt);
	    return 0;
	  }
	    
	  if ( disabled)
	    continue;
	}
	
	if ( bd[i].attr->Param.Info.Flags & PWR_MASK_ARRAY ||
	     bd[i].attr->Param.Info.Flags & PWR_MASK_CLASS ) {
	  ns1__BrowseElement *element = new ns1__BrowseElement();
	  
	  cdh_SuppressSuper( aname, bd[i].attrName);
	  element->Name = new std::string( aname);
	  strcpy( itemname, pname);
	  strcat( itemname, ".");
	  strcat( itemname, bd[i].attrName);
	  element->ItemName = new std::string( itemname);
	  element->IsItem = false;
	  element->HasChildren = true;

	  if ( ns1__Browse->ReturnAllProperties)
	    property_mask = ~0;
	  else
	    opc_propertynames_to_mask( ns1__Browse->PropertyNames, &property_mask);

	  if ( property_mask)
	    opcsrv_get_properties( element->IsItem, cid, &paref, &aref, 
				   property_mask, &bd[i],
				   element->Properties);

	  ns1__BrowseResponse->Elements.push_back( element);
	}
	else {
	  ns1__BrowseElement *element = new ns1__BrowseElement();
	  
	  cdh_SuppressSuper( aname, bd[i].attrName);
	  element->Name = new std::string( aname);
	  strcpy( itemname, pname);
	  strcat( itemname, ".");
	  strcat( itemname, bd[i].attrName);
	  element->ItemName = new std::string( itemname);
	  element->IsItem = true;
	  element->HasChildren = false;

	  if ( ns1__Browse->ReturnAllProperties)
	    property_mask = ~0;
	  else
	    opc_propertynames_to_mask( ns1__Browse->PropertyNames, &property_mask);

	  if ( property_mask)
	    opcsrv_get_properties( element->IsItem, cid, &paref, &aref, 
				   property_mask, &bd[i],
				   element->Properties);

	  ns1__BrowseResponse->Elements.push_back( element);
	} 
      }
      free( (char *)bd);      
    }

    if ( paref.Flags.b.Object) {
      for ( sts = gdh_GetChild( paref.Objid, &child); 
	    ODD(sts); 
	    sts = gdh_GetNextSibling( child, &child)) {
	sts = gdh_ObjidToName( child, name, sizeof(name), cdh_mName_object);
	if ( EVEN(sts)) continue;

	sts = gdh_GetObjectClass( child, &cid);
	if ( EVEN(sts)) continue;
      
	ns1__BrowseElement *element = new ns1__BrowseElement();
	
	element->Name = new std::string( name);
	strcpy( itemname, pname);
	strcat( itemname, "-");
	strcat( itemname, name);
	element->ItemName = new std::string( itemname);
	element->IsItem = false;
	if ( cid == pwr_eClass_PlantHier || cid == pwr_eClass_NodeHier)
	  element->HasChildren = ODD( gdh_GetChild( child, &ch)) ? true : false;
	else
	  element->HasChildren = true;
					       
	if ( ns1__Browse->ReturnAllProperties)
	  property_mask = ~0;
	else
	  opc_propertynames_to_mask( ns1__Browse->PropertyNames, &property_mask);

	if ( property_mask) {
	  aref = cdh_ObjidToAref( child);
	  opcsrv_get_properties( element->IsItem, cid, &paref, &aref, 
				 property_mask, 0,
				 element->Properties);
	}
	ns1__BrowseResponse->Elements.push_back( element);	
      }
    }
  }
  return 0;
}

SOAP_FMAC5 int SOAP_FMAC6 __ns1__GetProperties(struct soap*, 
					       _ns1__GetProperties *ns1__GetProperties, 
					       _ns1__GetPropertiesResponse *ns1__GetPropertiesResponse)
{
  unsigned int property_mask;
  pwr_tCid cid;
  pwr_tAName iname;
  char *aname;
  pwr_tStatus sts;
  pwr_tAttrRef aref;
  pwr_tAttrRef paref;
  gdh_sAttrDef *bd;
  int 	rows;

  if ( ns1__GetProperties->ReturnAllProperties)
    property_mask = ~0;
  else
    opc_propertynames_to_mask( ns1__GetProperties->PropertyNames, &property_mask);

  for ( int i = 0; i < (int)ns1__GetProperties->ItemIDs.size(); i++) {
    ns1__PropertyReplyList *plist = new ns1__PropertyReplyList();
    std::string *path;

    if ( ns1__GetProperties->ItemIDs[i]->ItemPath)
      path = ns1__GetProperties->ItemIDs[i]->ItemPath;
    else
      path = ns1__GetProperties->ItemPath;

    plist->ItemPath = path;
    plist->ItemName = new std::string(*ns1__GetProperties->ItemIDs[i]->ItemName);

    if ( path) {
      strcpy( iname, path->c_str());
      strcat( iname, plist->ItemName->c_str());
    }
    else
      strcpy( iname, plist->ItemName->c_str());

    sts = gdh_NameToAttrref( pwr_cNOid, iname, &aref);
    if ( EVEN(sts)) {
      // E_INVALIDITEMNAME
      return 0;
    }

    if ( aref.Flags.b.Object || aref.Flags.b.ObjectAttr) {
      // This is an object
      sts = gdh_GetAttrRefTid( &aref, &cid);
      if ( EVEN(sts)) {
	// E_INVALIDITEMNAME
	return 0;
      }
      if ( !cdh_tidIsCid( cid)) {
	// E_INVALIDITEMNAME
	return 0;
      }

      opcsrv_get_properties( false, cid, 0, &aref, 
			     property_mask, 0,
			     plist->Properties);
    }
    else {
      // Get the object attrref and class for this attribute
      if ( !( aname = strrchr( iname, '.'))) {
	// E_INVALIDITEMNAME
	return 0;
      }
      aname++;
      
      sts = gdh_AttrArefToObjectAref( &aref, &paref);
      if ( EVEN(sts)) {
	return 0;
      }

      sts = gdh_GetAttrRefTid( &paref, &cid);
      if ( EVEN(sts)) {
	// E_INVALIDITEMNAME
	return 0;
      }

      // Get body definition
      sts = gdh_GetObjectBodyDef( cid, &bd, &rows, pwr_cNOid);
      if ( EVEN(sts)) {
	// E_INVALIDITEMNAME
	return 0;
      }
      int bd_idx = -1;
      for ( int i = 0; i < rows; i++) {
	if ( cdh_NoCaseStrcmp( aname, bd[i].attrName) == 0) {
	  bd_idx = i;
	  break;
	}
      }
      if ( bd_idx == -1) {
	// E_INVALIDITEMNAME
	free( (char *)bd);
	return 0;
      }

      opcsrv_get_properties( true, cid, &paref, &aref,
			     property_mask, &bd[bd_idx],
			     plist->Properties);
      free( (char *)bd);
    }

    ns1__GetPropertiesResponse->PropertyLists.push_back( plist);
  }

  return 0;
}

