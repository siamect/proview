/* 
 * Proview   $Id: opc_utl.h,v 1.3 2007-03-08 07:26:29 claes Exp $
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

#include <vector.h>
#include <string.h>
#include "pwr.h"
#include "opc_soap_Stub.h"

const int opc_cPropertySize = 17;

typedef enum {
  opc_mProperty_DataType 	= 1 << 0,
  opc_mProperty_Value 		= 1 << 1,
  opc_mProperty_Quality 	= 1 << 2,
  opc_mProperty_Timestamp 	= 1 << 3,
  opc_mProperty_AccessRights 	= 1 << 4,
  opc_mProperty_ScanRate 	= 1 << 5,
  opc_mProperty_EuType 		= 1 << 6,
  opc_mProperty_EuInfo 		= 1 << 7,
  opc_mProperty_EngineeringUnits = 1 << 8,
  opc_mProperty_Description 	= 1 << 9,
  opc_mProperty_HighEU 		= 1 << 10,
  opc_mProperty_LowEU 		= 1 << 11,
  opc_mProperty_HighIR 		= 1 << 12,
  opc_mProperty_LowIR 		= 1 << 13,
  opc_mProperty_CloseLabel 	= 1 << 14,
  opc_mProperty_OpenLabel 	= 1 << 15,
  opc_mProperty_TimeZone 	= 1 << 16
} opc_mProperty;

bool opc_pwrtype_to_string( int type, char **str);

bool opc_get_property( std::vector<ns1__ItemProperty *> properties, unsigned int mask,
		       char **valp);

void opc_mask_to_propertynames( std::vector<std::string>& pnames, unsigned int mask);

bool opc_propertynames_to_mask( std::vector<std::string>& pnames, unsigned int *mask);

bool opc_quality_to_string( int quality, char **str);
