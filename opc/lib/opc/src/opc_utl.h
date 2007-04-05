/* 
 * Proview   $Id: opc_utl.h,v 1.15 2007-04-05 13:32:03 claes Exp $
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

typedef enum {
  opc_mRequestOption_ReturnErrorText      = 1 << 0,
  opc_mRequestOption_ReturnDiagnosticInfo = 1 << 1,
  opc_mRequestOption_ReturnItemTime       = 1 << 2,
  opc_mRequestOption_ReturnItemPath       = 1 << 3,
  opc_mRequestOption_ReturnItemName       = 1 << 4
} opc_mRequestOptions;

const int opc_cDataTypeSize = 20;

typedef enum {
  opc_eDataType_string ,
  opc_eDataType_boolean ,
  opc_eDataType_float ,
  opc_eDataType_double ,
  opc_eDataType_decimal ,
  opc_eDataType_long ,
  opc_eDataType_int ,
  opc_eDataType_short ,
  opc_eDataType_byte ,
  opc_eDataType_unsignedLong ,
  opc_eDataType_unsignedInt ,
  opc_eDataType_unsignedShort ,
  opc_eDataType_unsignedByte ,
  opc_eDataType_base64Binary ,
  opc_eDataType_dateTime ,
  opc_eDataType_time ,
  opc_eDataType_date ,
  opc_eDataType_duration ,
  opc_eDataType_QName ,
  opc_eDataType_anyType , 
  opc_eDataType_ 
} opc_eDataType;

typedef enum {
  opc_eResultCode_S_,
  opc_eResultCode_S_CLAMP,
  opc_eResultCode_S_DATAQUEUEOVERFLOW,
  opc_eResultCode_S_UNSUPPORTEDRATE,
  opc_eResultCode_E_ACCESS_DENIED,
  opc_eResultCode_E_BUSY,
  opc_eResultCode_E_FAIL,
  opc_eResultCode_E_INVALIDCONTINUATIONPOINT,
  opc_eResultCode_E_INVALIDFILTER,
  opc_eResultCode_E_INVALIDHOLDTIME,
  opc_eResultCode_E_INVALIDITEMNAME,
  opc_eResultCode_E_INVALIDITEMPATH,
  opc_eResultCode_E_INVALIDPID,
  opc_eResultCode_E_NOSUBSCRIPTION,
  opc_eResultCode_E_NOTSUPPORTED,
  opc_eResultCode_E_OUTOFMEMORY,
  opc_eResultCode_E_RANGE,
  opc_eResultCode_E_READONLY,
  opc_eResultCode_E_SERVERSTATE,
  opc_eResultCode_E_TIMEDOUT,
  opc_eResultCode_E_UNKNOWNITEMNAME,
  opc_eResultCode_E_UNKNOWNITEMPATH,
  opc_eResultCode_E_WRITEONLY,
  opc_eResultCode_E_BADTYPE,
  opc_eResultCode__
} opc_eResultCode;

void opcsrv_returnerror(std::vector<s0__OPCError *>& errors, std::string **rc, int err_code, unsigned int options);
bool opc_requestoptions_to_mask( s0__RequestOptions *options, unsigned int *mask);
std::string& opc_datetime( pwr_tTime *tp);
pwr_tStatus time_AtoOPCAscii (pwr_tTime *tp, char *buf, int bufsize);
pwr_tStatus opc_time_OPCAsciiToA( char *tstr, pwr_tTime *ts);
const char *opc_resultcode_to_string( int code);
const char *opc_resultcode_to_text( int code);
bool opc_string_to_resultcode(char *str, int *code);
xsd__anyType* opc_opctype_to_value(void *bufp, int size, int opc_type);
bool opc_convert_pwrtype_to_opctype(void *bufin, void *bufout, int size, int opc_type, int pwr_type);
bool opc_convert_opctype_to_pwrtype(void *bufp, int size, xsd__anyType *value, pwr_eType pwr_type);
bool opc_string_to_opctype(const char *str, int *type);
char *opc_opctype_to_string(int type);
bool opc_pwrtype_to_opctype(int pwrtype, int *opctype);
bool opc_opctype_to_pwrtype(int type, int *pwrtype); 
bool opc_pwrtype_to_string( int type, char **str);

bool opc_get_property( std::vector<s0__ItemProperty *> properties, unsigned int mask,
		       xsd__anyType **valp);

void opc_mask_to_propertynames( std::vector<std::string>& pnames, unsigned int mask);

bool opc_propertynames_to_mask( std::vector<std::string>& pnames, unsigned int *mask);

bool opc_quality_to_string( int quality, char **str);

bool opc_cmp_pwr( void *p1, void *p2, int size, int type, float deadband);
