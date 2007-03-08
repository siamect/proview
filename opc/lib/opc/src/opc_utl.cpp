/* 
 * Proview   $Id: opc_utl.cpp,v 1.3 2007-03-08 07:26:29 claes Exp $
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


#include "pwr_class.h"
#include "opc_utl.h"
#include "opc_soap_Stub.h"

static char opc_PropertyNames[17][20] = {"dataType",
					 "value",
					 "quality",
					 "timestamp",
					 "accessRights",
					 "scanRate",
					 "euType",
					 "euInfo",
					 "engineeringUnits",
					 "description",
					 "highEU",
					 "lowEU",
					 "highIR",
					 "lowIR",
					 "closeLabel",
					 "openLabel",
					 "timeZone"};

//
// Return the corresponding opc type string for a pwr_eType
//
bool opc_pwrtype_to_string( int type, char **str)
{
  *str = (char *) malloc(20);
  switch ( type) {
  case pwr_eType_String:
  case pwr_eType_Objid:
  case pwr_eType_AttrRef:
    strcpy( *str, "string");
    break;
  case pwr_eType_Boolean:
    strcpy( *str, "boolean");
    break;
  case pwr_eType_Float32:
    strcpy( *str, "float");
    break;
  case pwr_eType_Float64:
    strcpy( *str, "double");
    break;
  case pwr_eType_Enum:
  case pwr_eType_Mask:
  case pwr_eType_Status:
  case pwr_eType_NetStatus:
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


void opc_mask_to_propertynames( std::vector<std::string>& pnames, unsigned int mask)
{
  unsigned int m;

  for ( int i = 0; i < opc_cPropertySize; i++) {
    m = 1 << i;
    if ( mask & m)
      pnames.push_back( std::string( opc_PropertyNames[i]));
  }
}

bool opc_get_property( std::vector<ns1__ItemProperty *> properties, unsigned int mask,
		       char **valp)
{
  char name[80];
  char *s;
  int idx = -1;

  for ( int i = 0; i < opc_cPropertySize; i++) {
    if ( (unsigned int)(1 << i) == mask) {
      idx = i;
      break;
    }
  }
  if ( idx == -1)
    return false;

  for ( int i = 0; i < (int)properties.size(); i++) {

    strcpy( name, properties[i]->Name.c_str());
    if ( (s = strrchr( name, ':')))
      s++;
    else
      s = name;

    if ( strcmp( s, opc_PropertyNames[idx]) == 0) {
      if ( !properties[0]->Value)
	return false;
      *valp = properties[i]->Value;
      return true;
    }
  }
  return false;
}

bool opc_propertynames_to_mask( std::vector<std::string>& pnames, unsigned int *mask)
{
  char name[40];
  char *np;

  *mask = 0;

  for ( int i = 0; i < (int) pnames.size(); i++) {
    strcpy( name, pnames[i].c_str());
    if ( (np = strrchr( name, ':')))
      np++;
    else
      np = name;

    switch ( *np) {
    case 'd':
      switch ( *(np+1)) {
      case 'a':
	*mask |= opc_mProperty_DataType;
	break;
      case 'e':
	*mask |= opc_mProperty_Description;
	break;
      default:
	return false;
      }
      break;
    case 'v':
      *mask |= opc_mProperty_Value;
      break;
    case 'q':
      *mask |= opc_mProperty_Quality;
      break;
    case 't':
      switch( *(np+4)) {
      case 's':
	*mask |= opc_mProperty_Timestamp;
	break;
      case 'Z':
	*mask |= opc_mProperty_TimeZone;
	break;
      default:
	return false;
      }
      break;
    case 's':
      *mask |= opc_mProperty_ScanRate;
      break;
    case 'e':
      switch( *(np+2)) {
      case 'T':
	*mask |= opc_mProperty_EuType;
	break;
      case 'I':
	*mask |= opc_mProperty_EuInfo;
	break;
      case 'g':
	*mask |= opc_mProperty_EngineeringUnits;
	break;
      default:
	return false;
      }
      break;
    case 'h':
      switch( *(np+4)) {
      case 'E':
	*mask |= opc_mProperty_HighEU;
	break;
      case 'I':
	*mask |= opc_mProperty_HighIR;
	break;
      default:
	return false;
      }
      break;
    case 'l':
      switch( *(np+3)) {
      case 'E':
	*mask |= opc_mProperty_LowEU;
	break;
      case 'I':
	*mask |= opc_mProperty_LowIR;
	break;
      default:
	return false;
      }
      break;
    case 'c':
      *mask |= opc_mProperty_CloseLabel;
      break;
    case 'o':
      *mask |= opc_mProperty_OpenLabel;
      break;
    default:
      return false;
    }
  }
  return true;
}

bool opc_quality_to_string( int quality, char **str)
{
  *str = (char *) malloc(30);
  switch ( quality) {
  case ns1__qualityBits__bad:
    strcpy( *str, "bad");
    break;
  case ns1__qualityBits__badConfigurationError:
    strcpy( *str, "badConfigurationError");
    break;
  case ns1__qualityBits__badNotConnected:
    strcpy( *str, "badNotConnected");
    break;
  case ns1__qualityBits__badDeviceFailure:
    strcpy( *str, "badDeviceFailure");
    break;
  case ns1__qualityBits__badSensorFailure:
    strcpy( *str, "badSensorFailure");
    break;
  case ns1__qualityBits__badLastKnownValue:
    strcpy( *str, "badLastKnownValue");
    break;
  case ns1__qualityBits__badCommFailure:
    strcpy( *str, "badCommFailure");
    break;
  case ns1__qualityBits__badOutOfService:
    strcpy( *str, "badOutOfService");
    break;
  case ns1__qualityBits__badWaitingForInitialData:
    strcpy( *str, "badWaitingForInitialData");
    break;
  case ns1__qualityBits__uncertain:
    strcpy( *str, "uncertain");
    break;
  case ns1__qualityBits__uncertainLastUsableValue:
    strcpy( *str, "uncertainLastUsableValue");
    break;
  case ns1__qualityBits__uncertainSensorNotAccurate:
    strcpy( *str, "uncertainSensorNotAccurate");
    break;
  case ns1__qualityBits__uncertainEUExceeded:
    strcpy( *str, "uncertainEUExceede");
    break;
  case ns1__qualityBits__uncertainSubNormal:
    strcpy( *str, "uncertainSubNormal");
    break;
  case ns1__qualityBits__good:
    strcpy( *str, "good");
    break;
  case ns1__qualityBits__goodLocalOverride:
    strcpy( *str, "goodLocalOverride");
    break;
  default:
    return false;
  }
  return true;
}

