/* 
 * Proview   $Id: opc_server.cpp,v 1.2 2007-03-02 08:52:20 claes Exp $
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

#include "pwr.h"
#include "pwr_version.h"
#include "co_cdh.h"
#include "rt_gdh.h"
#include "opc_utl.h"
#include "opc_soap_H.h"
#include "Service.nsmap"

static bool opc_type_to_string( int type, char **str)
{
  *str = (char *) malloc(20);
  switch ( type) {
  case pwr_eType_String:
    strcpy( *str, "string");
    break;
  case pwr_eType_Float32:
    strcpy( *str, "float");
    break;
  case pwr_eType_Float64:
    strcpy( *str, "double");
    break;
  case pwr_eType_Int32:
    strcpy( *str, "int");
    break;
  case pwr_eType_Int16:
    strcpy( *str, "short");
    break;
  case pwr_eType_Int8:
    strcpy( *str, "char");
    break;
  case pwr_eType_UInt32:
    strcpy( *str, "unsignedInt");
    break;
  case pwr_eType_UInt16:
    strcpy( *str, "unsignedShort");
    break;
  case pwr_eType_UInt8:
    strcpy( *str, "unsignedChar");
    break;
  case pwr_eType_Time:
    strcpy( *str, "dateTime");
    break;
  case pwr_eType_DeltaTime:
    strcpy( *str, "duration");
    break;
  default:
    free( *str);
    *str = 0;
    return false;
  }
  return true;
}


int main()
{
  struct soap soap;
  int m,s;   // Master and slave sockets
  pwr_tStatus sts;

  sts = gdh_Init("opc_server");
  if ( EVEN(sts)) {
    exit(sts);
  }  

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

SOAP_FMAC5 int SOAP_FMAC6 __ns1__Browse(struct soap*, _ns1__Browse *ns1__Browse, 
					_ns1__BrowseResponse *ns1__BrowseResponse)
{
  pwr_tStatus sts;
  pwr_tOid oid, child, ch;
  pwr_tOName name;
  pwr_tCid cid;

  if ( (!ns1__Browse->ItemName || ns1__Browse->ItemName->empty()) &&
       (!ns1__Browse->ItemPath || ns1__Browse->ItemPath->empty())) {
    // Return rootlist
    for ( sts = gdh_GetRootList( &oid); ODD(sts); sts = gdh_GetNextSibling( oid, &oid)) {
      sts = gdh_ObjidToName( oid, name, sizeof(name), cdh_mName_object);
      if ( EVEN(sts)) continue;

      sts = gdh_GetObjectClass( oid, &cid);
      if ( EVEN(sts)) continue;
      
      ns1__BrowseElement *element = new ns1__BrowseElement();
      element->Name = new std::string( name);
      element->ItemName = element->Name;
      element->IsItem = ( cid == pwr_eClass_PlantHier || cid == pwr_eClass_NodeHier) ? true : false;
      element->HasChildren = ODD( gdh_GetChild( oid, &ch)) ? true : false;
      ns1__BrowseResponse->Elements.push_back( element);	

      for ( int i = 0; i < (int)ns1__Browse->PropertyNames.size(); i++) {
	ns1__ItemProperty *property = new ns1__ItemProperty();
	property->Name = ns1__Browse->PropertyNames[i];

	if ( property->Name == "\"\":dataType") {
	  property->Value = (char *) malloc( 6);
	  strcpy( property->Value, "float");
	}
	element->Properties.push_back( property);
      }
    }
  }
  else {
    // Return attributes and children
    pwr_tOName itemname;
    gdh_sAttrDef *bd;
    int 	rows;
    
    if ( ns1__Browse->ItemPath && !ns1__Browse->ItemPath->empty()) {
      strncpy( itemname, ns1__Browse->ItemPath->c_str(), sizeof( itemname));
      if ( ns1__Browse->ItemName && !ns1__Browse->ItemName->empty()) {
	strcat( itemname, "-");
	strcat( itemname, ns1__Browse->ItemName->c_str());
      }
    }
    else
      strncpy( itemname, ns1__Browse->ItemName->c_str(), sizeof(itemname));

    sts = gdh_NameToObjid( itemname, &oid);
    if ( EVEN(sts)) {
      return 0;
    }

    sts = gdh_GetObjectBodyDef( cid, &bd, &rows, oid);
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
	
	if ( bd[i].attr->Param.Info.Flags & PWR_MASK_DISABLEATTR) {
	  pwr_sAttrRef aref = cdh_ObjidToAref( oid);
	  pwr_sAttrRef aaref;
	  pwr_tDisableAttr disabled;

	  sts = gdh_ArefANameToAref( &aref, bd[i].attrName, &aaref);
	  if ( EVEN(sts)) return sts;

	  sts = gdh_ArefDisabled( &aaref, &disabled);
	  if ( EVEN(sts)) return sts;

	  if ( disabled)
	    continue;
	}
	
	if ( bd[i].attr->Param.Info.Flags & PWR_MASK_ARRAY ) {
	}
	else if ( bd[i].attr->Param.Info.Flags & PWR_MASK_CLASS ) {
	}
	else {
	  ns1__BrowseElement *element = new ns1__BrowseElement();
	  
	  element->Name = new std::string( bd[i].attrName);
	  element->ItemName = element->Name;
	  element->ItemPath = new std::string( itemname);
	  element->IsItem = true;
	  element->HasChildren = false;

	  for ( int i = 0; i < (int)ns1__Browse->PropertyNames.size(); i++) {
	    ns1__ItemProperty *property = new ns1__ItemProperty();
	    property->Name = ns1__Browse->PropertyNames[i];

	    if ( property->Name == "\"\":dataType") {
	      char *type_p;

	      if ( ! opc_type_to_string( bd[i].attr->Param.Info.Type, &type_p)) {
		// Untranslatable type
		element->IsItem = false;
		element->HasChildren = false;		
		continue;
	      }
	      property->Value = type_p;
	    }
	    element->Properties.push_back( property);
	  }

	  ns1__BrowseResponse->Elements.push_back( element);	

	} 
      }
      free( (char *)bd);      
    }

    for ( sts = gdh_GetChild( oid, &child); ODD(sts); sts = gdh_GetNextSibling( child, &child)) {
      sts = gdh_ObjidToName( child, name, sizeof(name), cdh_mName_object);
      if ( EVEN(sts)) continue;

      sts = gdh_GetObjectClass( child, &cid);
      if ( EVEN(sts)) continue;
      
      ns1__BrowseElement *element = new ns1__BrowseElement();
	  
      element->Name = new std::string( name);
      element->ItemName = element->Name;
      element->ItemPath = new std::string( itemname);
      element->IsItem = ( cid == pwr_eClass_PlantHier || cid == pwr_eClass_NodeHier) ? true : false;
      element->HasChildren = ODD( gdh_GetChild( child, &ch)) ? true : false;
      ns1__BrowseResponse->Elements.push_back( element);	
    }
  }
  return 0;
}

SOAP_FMAC5 int SOAP_FMAC6 __ns1__GetProperties(struct soap*, 
					       _ns1__GetProperties *ns1__GetProperties, 
					       _ns1__GetPropertiesResponse *ns1__GetPropertiesResponse)
{
  return 0;
}

