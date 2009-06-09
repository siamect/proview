/* 
 * Proview   $Id: opc_utl.cpp,v 1.22 2008-10-31 12:51:29 claes Exp $
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


#include <float.h>
#include <iconv.h>
#include <typeinfo>
#include "pwr_class.h"
#include "co_time.h"
#include "co_time_msg.h"
#include "opc_utl.h"
#include "opc_soap_Stub.h"
#include "opc_soap_H.h"

const char nullstr[] = "";

static pwr_tString32 str_dt[20] = {"xsd:string" ,
			    "xsd:boolean" ,
			    "xsd:float" ,
			    "xsd:double" ,
			    "xsd:decimal" ,
			    "xsd:long" ,
			    "xsd:int" ,
			    "xsd:short" ,
			    "xsd:byte" ,
			    "xsd:unsignedLong" ,
			    "xsd:unsignedInt" ,
			    "xsd:unsignedShort" ,
			    "xsd:unsignedByte" ,
			    "xsd:base64Binary" ,
			    "xsd:dateTime" ,
			    "xsd:time" ,
			    "xsd:date" ,
			    "xsd:duration" ,
			    "xsd:QName" ,
			    "xsd:anyType" };

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

static char opc_ResultCodes[24][32] = {"",
   				       "s0:S_CLAMP",
				       "s0:S_DATAQUEUEOVERFLOW",
				       "s0:S_UNSUPPORTEDRATE",
				       "s0:E_ACCESS_DENIED",
				       "s0:E_BUSY",
				       "s0:E_FAIL",
				       "s0:E_INVALIDCONTINUATIONPOINT",
				       "s0:E_INVALIDFILTER",
				       "s0:E_INVALIDHOLDTIME",
				       "s0:E_INVALIDITEMNAME",
				       "s0:E_INVALIDITEMPATH",
				       "s0:E_INVALIDPID",
				       "s0:E_NOSUBSCRIPTION",
				       "s0:E_NOTSUPPORTED",
				       "s0:E_OUTOFMEMORY",
				       "s0:E_RANGE",
				       "s0:E_READONLY",
				       "s0:E_SERVERSTATE",
				       "s0:E_TIMEDOUT",
				       "s0:E_UNKNOWNITEMNAME",
				       "s0:E_UNKNOWNITEMPATH",
				       "s0:E_WRITEONLY",
				       "s0:E_BADTYPE"};

static char opc_ResultTexts[23][140] = {
  "The value written was accepted but the output was clamped.",
  "Not every detected change has been returned since the server's buffer reached its limit and had to purge out the oldest data.",
  "The server does not support the requested rate but will use the closest available rate.",
  "The server deines access (read and/or write) to the specified item.",
  "The server is currently processing another polled refresh for one or more of the subscriptions.",
  "Unspecified error.",
  "The continuation point is not valid.",
  "The filter string is not valid.",
  "The hold time is too long (determined by the server).",
  "The item name does not conform the server's syntax.",
  "The item path does not conform the server's syntax.",
  "The property id is not valid for the item.",
  "An invalid set of subscription handles was passed to the request.",
  "The server does not support writing to the quality and/or timestamp.",
  "Ran out of memory.",
  "The value was out of range.",
  "The value is read only and may not be written to.",
  "The operation could not complete due to an abnormal server state.",
  "The operation took too long to complete (determined by the server).",
  "The item name is no longer available in the server address space.",
  "The item path is no longer available in the server address space.",
  "The value is write-only and may not be read from or returned as part of a write response.",
  "The type is not valid."};

void opcsrv_returnerror( struct soap *soap, std::vector<s0__OPCError *>& errors, 
			 std::string **rc, int err_code, unsigned int options)
{
  int ii;
  bool exists = false;

	
  if ( rc) {
    *rc = soap_new_std__string( soap, -1);
    (*rc)->assign( opc_resultcode_to_string( err_code));
  }
  
  for (ii = 0; ii < (int) errors.size(); ii++) {
    if (strcmp( errors[ii]->ID.c_str(), opc_resultcode_to_string( err_code)) == 0) {
      exists = true;
    }
  }
  
  if (!exists) {
    s0__OPCError *oe = soap_new_s0__OPCError( soap, -1);
    oe->ID.assign( opc_resultcode_to_string( err_code));
    
    if (options & opc_mRequestOption_ReturnErrorText) {
      oe->Text = soap_new_std__string( soap, -1);
      oe->Text->assign( opc_resultcode_to_text( err_code));
    }
	
    errors.push_back(oe);
  }

}

bool opc_requestoptions_to_mask( s0__RequestOptions *options, unsigned int *mask)
{

  if (!options) {
    *mask = 0;
    return false;
  }
  
  if (options->ReturnErrorText)
    *mask |= *options->ReturnErrorText ? opc_mRequestOption_ReturnErrorText : 0;

  if (options->ReturnDiagnosticInfo)
    *mask |= *options->ReturnDiagnosticInfo ? opc_mRequestOption_ReturnDiagnosticInfo : 0;

  if (options->ReturnItemTime)
    *mask |= *options->ReturnItemTime ? opc_mRequestOption_ReturnItemTime : 0;

  if (options->ReturnItemPath)
    *mask |= *options->ReturnItemPath ? opc_mRequestOption_ReturnItemPath : 0;

  if (options->ReturnItemName)
    *mask |= *options->ReturnItemName ? opc_mRequestOption_ReturnItemName : 0;
  
  return true;
}  

const char *opc_resultcode_to_string( int code)
{
  if ( code >= opc_eResultCode__)
    return nullstr;

  return opc_ResultCodes[code];
}

const char *opc_resultcode_to_text( int code)
{
  if ( code >= opc_eResultCode__)
    return nullstr;

  return opc_ResultTexts[code];
}

bool opc_string_to_resultcode(char *str, int *code)
{
  for ( int ii = 0; ii < opc_eResultCode__; ii++) {
    if (strcmp(opc_ResultCodes[ii], str) == 0) {
      *code  = ii;
      return true;
    }
  }
  
  return false;
}


//
// Return the corresponding opc type string for a pwr_eType
//

char *opc_datetime( pwr_tTime *tp)
{
  static char str[40];

  time_AtoOPCAscii( tp, str, sizeof(str));
  return str;
}

pwr_tStatus opc_time_OPCAsciiToA( char *tstr, pwr_tTime *ts)
{
  struct tm   	tt;
  int          	tmphs = 0;
  int	    	tmps1, tmps2;
  char        	buf[64];
  int		num;

  strncpy(buf, tstr, sizeof(buf) - 1);
  buf[sizeof(buf) - 1] = '\0';

  num = sscanf(tstr, "%4d-%02d-%02dT%02d:%02d:%02d.%d-%02d:%02d", &tt.tm_year,
         &tt.tm_mon, &tt.tm_mday, &tt.tm_hour, &tt.tm_min, &tt.tm_sec, &tmphs, &tmps1, &tmps2);
  if ( num < 7)
    return TIME__IVDTIME;

  tt.tm_year -= 1900;
  tt.tm_mon -= 1;
  tt.tm_wday = -1;
  tt.tm_yday = -1;
  tt.tm_isdst = -1;

  ts->tv_sec = mktime(&tt);
  ts->tv_nsec = tmphs * 100;
  return TIME__SUCCESS;
}

pwr_tStatus time_AtoOPCAscii (pwr_tTime *tp, char *buf, int bufsize)
{

   struct tm *tmpTm;
   int        buflen;
   char       tmpStr[16];
   pwr_tTime  t;
   int        tzone;

   if ( !tp) {
     time_GetTime( &t);
     tp = &t;
   }
  
   time_t sec = tp->tv_sec;
   tmpTm = localtime(&sec);
   tzone = tmpTm->tm_gmtoff / 3600;
   strftime(buf, bufsize, "%Y-%m-%dT%H:%M:%S", tmpTm);

   sprintf(tmpStr, ".%07d%s%02d:00", (int)(tp->tv_nsec / 100), tzone >= 0 ? "+" : "", tzone);
   buflen = strlen(buf);
   if (strlen(tmpStr) + buflen < (unsigned int) bufsize)
     strcpy(&buf[buflen], tmpStr);
 
   return TIME__SUCCESS;
}

//
// Return the corresponding opc type for a opc type string
//
xsd__anyType* opc_opctype_to_value( struct soap *soap, void *bufp, int size, 
				    int opc_type)
{
  switch (opc_type) {
  case opc_eDataType_string: {
    xsd__string *val = soap_new_xsd__string( soap, -1);
    val->__item.assign( (char *)bufp);
    return val;
    break;
  }
  case opc_eDataType_boolean: {
    xsd__boolean *val = soap_new_xsd__boolean( soap, -1);
    val->__item = (bool)(*(char *) bufp);
    return val;
    break;
  }
  case opc_eDataType_float: {
    xsd__float *val = soap_new_xsd__float( soap, -1);
    val->__item =  *(pwr_tFloat32 *) bufp;
    return val;
    break;
  }
  case opc_eDataType_decimal: {
    xsd__decimal_ *val = soap_new_xsd__decimal_( soap, -1);
    sprintf((char *) bufp, "%f", *(pwr_tFloat64 *) bufp);
    val->__item = std::string((char *) bufp);
    return val;
    break;
  }
  case opc_eDataType_double: {
    xsd__double *val = soap_new_xsd__double( soap, -1);
    val->__item = *(pwr_tFloat64 *) bufp;
    return val;
    break;
  }
  case opc_eDataType_long: {
    xsd__long *val = soap_new_xsd__long( soap, -1);
    val->__item = *(pwr_tInt64 *) bufp;
    return val;
    break;
  }
  case opc_eDataType_int: {
    xsd__int *val = soap_new_xsd__int( soap, -1);
    val->__item = *(pwr_tInt32 *) bufp;
    return val;
    break;
  }
  case opc_eDataType_short: {
    xsd__short *val = soap_new_xsd__short( soap, -1);
    val->__item = *(pwr_tInt16 *) bufp;
    return val;
    break;
  }
  case opc_eDataType_byte: {
    xsd__byte *val = soap_new_xsd__byte( soap, -1);
    val->__item = *(pwr_tChar *) bufp;
    return val;
    break;
  }
  case opc_eDataType_unsignedLong: {
    xsd__unsignedLong *val = soap_new_xsd__unsignedLong( soap, -1);
    val->__item = *(pwr_tUInt64 *) bufp;
    return val;
    break;
  }
  case opc_eDataType_unsignedInt: {
    xsd__unsignedInt *val = soap_new_xsd__unsignedInt( soap, -1);
    val->__item = *(pwr_tInt32 *) bufp;
    return val;
    break;
  }
  case opc_eDataType_unsignedShort: {
    xsd__unsignedShort *val = soap_new_xsd__unsignedShort( soap, -1);
    val->__item = *(pwr_tUInt16 *) bufp;
    return val;
    break;
  }
  case opc_eDataType_unsignedByte: {
    xsd__unsignedByte *val = soap_new_xsd__unsignedByte( soap, -1);
    val->__item = *(pwr_tUInt8 *) bufp;
    return val;
    break;
  }
  case opc_eDataType_dateTime: {
    xsd__dateTime *val = soap_new_xsd__dateTime( soap, -1);
    char timstr[40];

    time_AtoOPCAscii( (pwr_tTime *)bufp, timstr, sizeof(timstr));
    val->__item.assign( timstr);
    return val;
    break;
  }
  case opc_eDataType_duration: {
    xsd__duration *val = soap_new_xsd__duration( soap, -1);
    // TODO
    // char timstr[40];

    // time_DtoOPCAscii( (pwr_tTime *)bufp, timstr, sizeof(timstr));
    // val->__item = std::string( timstr);
    return val;
    break;
  } 
  default:
    break;
  }
  
  return NULL;
}

//
// Return the corresponding pwrtype for a opctype
//
bool opc_convert_opctype_to_pwrtype(void *bufp, int size, xsd__anyType *value, pwr_eType pwr_type)
{
  opc_eDataType opc_type;
  
  if ( !value)
    return false;

  if (typeid(*value) == typeid(xsd__string))
    opc_type = opc_eDataType_string;
  
  else if (typeid(*value) == typeid(xsd__boolean))
    opc_type = opc_eDataType_boolean;

  else if (typeid(*value) == typeid(xsd__float))
    opc_type = opc_eDataType_float;

  else if (typeid(*value) == typeid(xsd__decimal))
    opc_type = opc_eDataType_decimal;

  else if (typeid(*value) == typeid(xsd__double))
    opc_type = opc_eDataType_double;

  else if (typeid(*value) == typeid(xsd__long))
    opc_type = opc_eDataType_long;

  else if (typeid(*value) == typeid(xsd__int))
    opc_type = opc_eDataType_int;

  else if (typeid(*value) == typeid(xsd__short))
    opc_type = opc_eDataType_short;

  else if (typeid(*value) == typeid(xsd__byte))
    opc_type = opc_eDataType_byte;

  else if (typeid(*value) == typeid(xsd__unsignedLong))
    opc_type = opc_eDataType_unsignedLong;

  else if (typeid(*value) == typeid(xsd__unsignedInt))
    opc_type = opc_eDataType_unsignedInt;

  else if (typeid(*value) == typeid(xsd__unsignedShort))
    opc_type = opc_eDataType_unsignedShort;

  else if (typeid(*value) == typeid(xsd__unsignedByte))
    opc_type = opc_eDataType_unsignedByte;

  else if (typeid(*value) == typeid(xsd__dateTime))
    opc_type = opc_eDataType_dateTime;
  else if (typeid(*value) == typeid(xsd__duration))
    opc_type = opc_eDataType_duration;
  else
    opc_type = opc_eDataType_;

  switch (pwr_type) {
  case pwr_eType_Float64:
    switch (opc_type) {
    case opc_eDataType_boolean:
      (*(pwr_tFloat64 *) bufp) = ((xsd__boolean *) value)->__item;
      break;
    case opc_eDataType_float:
      (*(pwr_tFloat64 *) bufp) = ((xsd__float *) value)->__item;
      break;
    case opc_eDataType_double:
      (*(pwr_tFloat64 *) bufp) = ((xsd__double *) value)->__item;
      break;
    case opc_eDataType_long:
      (*(pwr_tFloat64 *) bufp) = ((xsd__long *) value)->__item;
      break;
    case opc_eDataType_int:
      (*(pwr_tFloat64 *) bufp) = ((xsd__int *) value)->__item;
      break;
    case opc_eDataType_short:
      (*(pwr_tFloat64 *) bufp) = ((xsd__short *) value)->__item;
      break;
    case opc_eDataType_byte:
      (*(pwr_tFloat64 *) bufp) = ((xsd__byte *) value)->__item;
      break;
    case opc_eDataType_unsignedLong:
      (*(pwr_tFloat64 *) bufp) = ((xsd__unsignedLong *) value)->__item;
      break;
    case opc_eDataType_unsignedInt:
      (*(pwr_tFloat64 *) bufp) = ((xsd__unsignedInt *) value)->__item;
      break;
    case opc_eDataType_unsignedShort:
      (*(pwr_tFloat64 *) bufp) = ((xsd__unsignedShort *) value)->__item;
      break;
    case opc_eDataType_unsignedByte:
      (*(pwr_tFloat64 *) bufp) = ((xsd__unsignedByte *) value)->__item;
      break;
    case opc_eDataType_dateTime:
    case opc_eDataType_string:
    case opc_eDataType_decimal:
    default:
      return false;
    }
    break;
  case pwr_eType_Float32:
    switch (opc_type) {
    case opc_eDataType_boolean:
      (*(pwr_tFloat32 *) bufp) = ((xsd__boolean *) value)->__item;
      break;
    case opc_eDataType_float:
      (*(pwr_tFloat32 *) bufp) = ((xsd__float *) value)->__item;
      break;
    case opc_eDataType_double:
      (*(pwr_tFloat32 *) bufp) = ((xsd__double *) value)->__item;
      break;
    case opc_eDataType_long:
      (*(pwr_tFloat32 *) bufp) = ((xsd__long *) value)->__item;
      break;
    case opc_eDataType_int:
      (*(pwr_tFloat32 *) bufp) = ((xsd__int *) value)->__item;
      break;
    case opc_eDataType_short:
      (*(pwr_tFloat32 *) bufp) = ((xsd__short *) value)->__item;
      break;
    case opc_eDataType_byte:
      (*(pwr_tFloat32 *) bufp) = ((xsd__byte *) value)->__item;
      break;
    case opc_eDataType_unsignedLong:
      (*(pwr_tFloat32 *) bufp) = ((xsd__unsignedLong *) value)->__item;
      break;
    case opc_eDataType_unsignedInt:
      (*(pwr_tFloat32 *) bufp) = ((xsd__unsignedInt *) value)->__item;
      break;
    case opc_eDataType_unsignedShort:
      (*(pwr_tFloat32 *) bufp) = ((xsd__unsignedShort *) value)->__item;
      break;
    case opc_eDataType_unsignedByte:
      (*(pwr_tFloat32 *) bufp) = ((xsd__unsignedByte *) value)->__item;
      break;
    case opc_eDataType_dateTime:
    case opc_eDataType_string:
    case opc_eDataType_decimal:
    default:
      return false;
    }
    break;
  case pwr_eType_Char:
    switch (opc_type) {
    case opc_eDataType_boolean:
      (*(pwr_tChar *) bufp) = ((xsd__boolean *) value)->__item;
      break;
    case opc_eDataType_float:
      (*(pwr_tChar *) bufp) = (char)((xsd__float *) value)->__item;
      break;
    case opc_eDataType_double:
      (*(pwr_tChar *) bufp) = (char)((xsd__double *) value)->__item;
      break;
    case opc_eDataType_long:
      (*(pwr_tChar *) bufp) = ((xsd__long *) value)->__item;
      break;
    case opc_eDataType_int:
      (*(pwr_tChar *) bufp) = ((xsd__int *) value)->__item;
      break;
    case opc_eDataType_short:
      (*(pwr_tChar *) bufp) = ((xsd__short *) value)->__item;
      break;
    case opc_eDataType_byte:
      (*(pwr_tChar *) bufp) = ((xsd__byte *) value)->__item;
      break;
    case opc_eDataType_unsignedLong:
      (*(pwr_tChar *) bufp) = ((xsd__unsignedLong *) value)->__item;
      break;
    case opc_eDataType_unsignedInt:
      (*(pwr_tChar *) bufp) = ((xsd__unsignedInt *) value)->__item;
      break;
    case opc_eDataType_unsignedShort:
      (*(pwr_tChar *) bufp) = ((xsd__unsignedShort *) value)->__item;
      break;
    case opc_eDataType_unsignedByte:
      (*(pwr_tChar *) bufp) = ((xsd__unsignedByte *) value)->__item;
      break;
    case opc_eDataType_dateTime:
    case opc_eDataType_string:
    case opc_eDataType_decimal:
    default:
      return false;
    }
    break;
  case pwr_eType_Int8:
    switch (opc_type) {
    case opc_eDataType_boolean:
      (*(pwr_tInt8 *) bufp) = ((xsd__boolean *) value)->__item;
      break;
    case opc_eDataType_float:
      (*(pwr_tInt8 *) bufp) = (pwr_tInt8)((xsd__float *) value)->__item;
      break;
    case opc_eDataType_double:
      (*(pwr_tInt8 *) bufp) = (pwr_tInt8)((xsd__double *) value)->__item;
      break;
    case opc_eDataType_long:
      (*(pwr_tInt8 *) bufp) = ((xsd__long *) value)->__item;
      break;
    case opc_eDataType_int:
      (*(pwr_tInt8 *) bufp) = ((xsd__int *) value)->__item;
      break;
    case opc_eDataType_short:
      (*(pwr_tInt8 *) bufp) = ((xsd__short *) value)->__item;
      break;
    case opc_eDataType_byte:
      (*(pwr_tInt8 *) bufp) = ((xsd__byte *) value)->__item;
      break;
    case opc_eDataType_unsignedLong:
      (*(pwr_tInt8 *) bufp) = ((xsd__unsignedLong *) value)->__item;
      break;
    case opc_eDataType_unsignedInt:
      (*(pwr_tInt8 *) bufp) = ((xsd__unsignedInt *) value)->__item;
      break;
    case opc_eDataType_unsignedShort:
      (*(pwr_tInt8 *) bufp) = ((xsd__unsignedShort *) value)->__item;
      break;
    case opc_eDataType_unsignedByte:
      (*(pwr_tInt8 *) bufp) = ((xsd__unsignedByte *) value)->__item;
      break;
    case opc_eDataType_dateTime:
    case opc_eDataType_string:
    case opc_eDataType_decimal:
    default:
      return false;
    }
    break;
  case pwr_eType_Int16:
    switch (opc_type) {
    case opc_eDataType_boolean:
      (*(pwr_tInt16 *) bufp) = ((xsd__boolean *) value)->__item;
      break;
    case opc_eDataType_float:
      (*(pwr_tInt16 *) bufp) = (pwr_tInt16)((xsd__float *) value)->__item;
      break;
    case opc_eDataType_double:
      (*(pwr_tInt16 *) bufp) = (pwr_tInt16)((xsd__double *) value)->__item;
      break;
    case opc_eDataType_long:
      (*(pwr_tInt16 *) bufp) = ((xsd__long *) value)->__item;
      break;
    case opc_eDataType_int:
      (*(pwr_tInt16 *) bufp) = ((xsd__int *) value)->__item;
      break;
    case opc_eDataType_short:
      (*(pwr_tInt16 *) bufp) = ((xsd__short *) value)->__item;
      break;
    case opc_eDataType_byte:
      (*(pwr_tInt16 *) bufp) = ((xsd__byte *) value)->__item;
      break;
    case opc_eDataType_unsignedLong:
      (*(pwr_tInt16 *) bufp) = ((xsd__unsignedLong *) value)->__item;
      break;
    case opc_eDataType_unsignedInt:
      (*(pwr_tInt16 *) bufp) = ((xsd__unsignedInt *) value)->__item;
      break;
    case opc_eDataType_unsignedShort:
      (*(pwr_tInt16 *) bufp) = ((xsd__unsignedShort *) value)->__item;
      break;
    case opc_eDataType_unsignedByte:
      (*(pwr_tInt16 *) bufp) = ((xsd__unsignedByte *) value)->__item;
      break;
    case opc_eDataType_dateTime:
    case opc_eDataType_string:
    case opc_eDataType_decimal:
    default:
      return false;
    }
    break;
  case pwr_eType_Int32:
    switch (opc_type) {
    case opc_eDataType_boolean:
      (*(pwr_tInt32 *) bufp) = ((xsd__boolean *) value)->__item;
      break;
    case opc_eDataType_float:
      (*(pwr_tInt32 *) bufp) = (pwr_tInt32)((xsd__float *) value)->__item;
      break;
    case opc_eDataType_double:
      (*(pwr_tInt32 *) bufp) = (pwr_tInt32)((xsd__double *) value)->__item;
      break;
    case opc_eDataType_long:
      (*(pwr_tInt32 *) bufp) = ((xsd__long *) value)->__item;
      break;
    case opc_eDataType_int:
      (*(pwr_tInt32 *) bufp) = ((xsd__int *) value)->__item;
      break;
    case opc_eDataType_short:
      (*(pwr_tInt32 *) bufp) = ((xsd__short *) value)->__item;
      break;
    case opc_eDataType_byte:
      (*(pwr_tInt32 *) bufp) = ((xsd__byte *) value)->__item;
      break;
    case opc_eDataType_unsignedLong:
      (*(pwr_tInt32 *) bufp) = ((xsd__unsignedLong *) value)->__item;
      break;
    case opc_eDataType_unsignedInt:
      (*(pwr_tInt32 *) bufp) = ((xsd__unsignedInt *) value)->__item;
      break;
    case opc_eDataType_unsignedShort:
      (*(pwr_tInt32 *) bufp) = ((xsd__unsignedShort *) value)->__item;
      break;
    case opc_eDataType_unsignedByte:
      (*(pwr_tInt32 *) bufp) = ((xsd__unsignedByte *) value)->__item;
      break;
    case opc_eDataType_dateTime:
    case opc_eDataType_string:
    case opc_eDataType_decimal:
    default:
      return false;
    }
    break;
  case pwr_eType_Int64:
    switch (opc_type) {
    case opc_eDataType_boolean:
      (*(pwr_tInt64 *) bufp) = ((xsd__boolean *) value)->__item;
      break;
    case opc_eDataType_float:
      (*(pwr_tInt64 *) bufp) = (pwr_tInt32)((xsd__float *) value)->__item;
      break;
    case opc_eDataType_double:
      (*(pwr_tInt64 *) bufp) = (pwr_tInt32)((xsd__double *) value)->__item;
      break;
    case opc_eDataType_long:
      (*(pwr_tInt64 *) bufp) = ((xsd__long *) value)->__item;
      break;
    case opc_eDataType_int:
      (*(pwr_tInt64 *) bufp) = ((xsd__int *) value)->__item;
      break;
    case opc_eDataType_short:
      (*(pwr_tInt64 *) bufp) = ((xsd__short *) value)->__item;
      break;
    case opc_eDataType_byte:
      (*(pwr_tInt64 *) bufp) = ((xsd__byte *) value)->__item;
      break;
    case opc_eDataType_unsignedLong:
      (*(pwr_tInt64 *) bufp) = ((xsd__unsignedLong *) value)->__item;
      break;
    case opc_eDataType_unsignedInt:
      (*(pwr_tInt64 *) bufp) = ((xsd__unsignedInt *) value)->__item;
      break;
    case opc_eDataType_unsignedShort:
      (*(pwr_tInt64 *) bufp) = ((xsd__unsignedShort *) value)->__item;
      break;
    case opc_eDataType_unsignedByte:
      (*(pwr_tInt64 *) bufp) = ((xsd__unsignedByte *) value)->__item;
      break;
    case opc_eDataType_dateTime:
    case opc_eDataType_string:
    case opc_eDataType_decimal:
    default:
      return false;
    }
    break;
  case pwr_eType_Boolean:
    switch (opc_type) {
    case opc_eDataType_boolean:
      (*(pwr_tBoolean *) bufp) = ((xsd__boolean *) value)->__item;
      break;
    case opc_eDataType_float:
      (*(pwr_tBoolean *) bufp) = (pwr_tBoolean)((xsd__float *) value)->__item;
      break;
    case opc_eDataType_double:
      (*(pwr_tBoolean *) bufp) = (pwr_tBoolean)((xsd__double *) value)->__item;
      break;
    case opc_eDataType_long:
      (*(pwr_tBoolean *) bufp) = ((xsd__long *) value)->__item;
      break;
    case opc_eDataType_int:
      (*(pwr_tBoolean *) bufp) = ((xsd__int *) value)->__item;
      break;
    case opc_eDataType_short:
      (*(pwr_tBoolean *) bufp) = ((xsd__short *) value)->__item;
      break;
    case opc_eDataType_byte:
      (*(pwr_tBoolean *) bufp) = ((xsd__byte *) value)->__item;
      break;
    case opc_eDataType_unsignedLong:
      (*(pwr_tBoolean *) bufp) = ((xsd__unsignedLong *) value)->__item;
      break;
    case opc_eDataType_unsignedInt:
      (*(pwr_tBoolean *) bufp) = ((xsd__unsignedInt *) value)->__item;
      break;
    case opc_eDataType_unsignedShort:
      (*(pwr_tBoolean *) bufp) = ((xsd__unsignedShort *) value)->__item;
      break;
    case opc_eDataType_unsignedByte:
      (*(pwr_tBoolean *) bufp) = ((xsd__unsignedByte *) value)->__item;
      break;
    case opc_eDataType_dateTime:
    case opc_eDataType_string:
    case opc_eDataType_decimal:
    default:
      return false;
    }
    break;
  case pwr_eType_UInt8:
    switch (opc_type) {
    case opc_eDataType_boolean:
      (*(pwr_tUInt8 *) bufp) = ((xsd__boolean *) value)->__item;
      break;
    case opc_eDataType_float:
      (*(pwr_tUInt8 *) bufp) = (pwr_tUInt8)((xsd__float *) value)->__item;
      break;
    case opc_eDataType_double:
      (*(pwr_tUInt8 *) bufp) = (pwr_tUInt8)((xsd__double *) value)->__item;
      break;
    case opc_eDataType_long:
      (*(pwr_tUInt8 *) bufp) = ((xsd__long *) value)->__item;
      break;
    case opc_eDataType_int:
      (*(pwr_tUInt8 *) bufp) = ((xsd__int *) value)->__item;
      break;
    case opc_eDataType_short:
      (*(pwr_tUInt8 *) bufp) = ((xsd__short *) value)->__item;
      break;
    case opc_eDataType_byte:
      (*(pwr_tUInt8 *) bufp) = ((xsd__byte *) value)->__item;
      break;
    case opc_eDataType_unsignedLong:
      (*(pwr_tUInt8 *) bufp) = ((xsd__unsignedLong *) value)->__item;
      break;
    case opc_eDataType_unsignedInt:
      (*(pwr_tUInt8 *) bufp) = ((xsd__unsignedInt *) value)->__item;
      break;
    case opc_eDataType_unsignedShort:
      (*(pwr_tUInt8 *) bufp) = ((xsd__unsignedShort *) value)->__item;
      break;
    case opc_eDataType_unsignedByte:
      (*(pwr_tUInt8 *) bufp) = ((xsd__unsignedByte *) value)->__item;
      break;
    case opc_eDataType_dateTime:
    case opc_eDataType_string:
    case opc_eDataType_decimal:
    default:
      return false;
    }
    break;
  case pwr_eType_UInt16:
    switch (opc_type) {
    case opc_eDataType_boolean:
      (*(pwr_tUInt16 *) bufp) = ((xsd__boolean *) value)->__item;
      break;
    case opc_eDataType_float:
      (*(pwr_tUInt16 *) bufp) = (pwr_tUInt16)((xsd__float *) value)->__item;
      break;
    case opc_eDataType_double:
      (*(pwr_tUInt16 *) bufp) = (pwr_tUInt16)((xsd__double *) value)->__item;
      break;
    case opc_eDataType_long:
      (*(pwr_tUInt16 *) bufp) = ((xsd__long *) value)->__item;
      break;
    case opc_eDataType_int:
      (*(pwr_tUInt16 *) bufp) = ((xsd__int *) value)->__item;
      break;
    case opc_eDataType_short:
      (*(pwr_tUInt16 *) bufp) = ((xsd__short *) value)->__item;
      break;
    case opc_eDataType_byte:
      (*(pwr_tUInt16 *) bufp) = ((xsd__byte *) value)->__item;
      break;
    case opc_eDataType_unsignedLong:
      (*(pwr_tUInt16 *) bufp) = ((xsd__unsignedLong *) value)->__item;
      break;
    case opc_eDataType_unsignedInt:
      (*(pwr_tUInt16 *) bufp) = ((xsd__unsignedInt *) value)->__item;
      break;
    case opc_eDataType_unsignedShort:
      (*(pwr_tUInt16 *) bufp) = ((xsd__unsignedShort *) value)->__item;
      break;
    case opc_eDataType_unsignedByte:
      (*(pwr_tUInt16 *) bufp) = ((xsd__unsignedByte *) value)->__item;
      break;
    case opc_eDataType_dateTime:
    case opc_eDataType_string:
    case opc_eDataType_decimal:
    default:
      return false;
    }
    break;
  case pwr_eType_UInt32:
  case pwr_eType_Mask:
  case pwr_eType_Enum:
  case pwr_eType_Status:
  case pwr_eType_NetStatus:
    switch (opc_type) {
    case opc_eDataType_boolean:
      (*(pwr_tUInt32 *) bufp) = ((xsd__boolean *) value)->__item;
      break;
    case opc_eDataType_float:
      (*(pwr_tUInt32 *) bufp) = (pwr_tUInt32)((xsd__float *) value)->__item;
      break;
    case opc_eDataType_double:
      (*(pwr_tUInt32 *) bufp) = (pwr_tUInt32)((xsd__double *) value)->__item;
      break;
    case opc_eDataType_long:
      (*(pwr_tUInt32 *) bufp) = ((xsd__long *) value)->__item;
      break;
    case opc_eDataType_int:
      (*(pwr_tUInt32 *) bufp) = ((xsd__int *) value)->__item;
      break;
    case opc_eDataType_short:
      (*(pwr_tUInt32 *) bufp) = ((xsd__short *) value)->__item;
      break;
    case opc_eDataType_byte:
      (*(pwr_tUInt32 *) bufp) = ((xsd__byte *) value)->__item;
      break;
    case opc_eDataType_unsignedLong:
      (*(pwr_tUInt32 *) bufp) = ((xsd__unsignedLong *) value)->__item;
      break;
    case opc_eDataType_unsignedInt:
      (*(pwr_tUInt32 *) bufp) = ((xsd__unsignedInt *) value)->__item;
      break;
    case opc_eDataType_unsignedShort:
      (*(pwr_tUInt32 *) bufp) = ((xsd__unsignedShort *) value)->__item;
      break;
    case opc_eDataType_unsignedByte:
      (*(pwr_tUInt32 *) bufp) = ((xsd__unsignedByte *) value)->__item;
      break;
    case opc_eDataType_dateTime:
    case opc_eDataType_string:
    case opc_eDataType_decimal:
    default:
      return false;
    }
    break;
  case pwr_eType_UInt64:
    switch (opc_type) {
    case opc_eDataType_boolean:
      (*(pwr_tUInt64 *) bufp) = ((xsd__boolean *) value)->__item;
      break;
    case opc_eDataType_float:
      (*(pwr_tUInt64 *) bufp) = (pwr_tUInt32)((xsd__float *) value)->__item;
      break;
    case opc_eDataType_double:
      (*(pwr_tUInt64 *) bufp) = (pwr_tUInt32)((xsd__double *) value)->__item;
      break;
    case opc_eDataType_long:
      (*(pwr_tUInt64 *) bufp) = ((xsd__long *) value)->__item;
      break;
    case opc_eDataType_int:
      (*(pwr_tUInt64 *) bufp) = ((xsd__int *) value)->__item;
      break;
    case opc_eDataType_short:
      (*(pwr_tUInt64 *) bufp) = ((xsd__short *) value)->__item;
      break;
    case opc_eDataType_byte:
      (*(pwr_tUInt64 *) bufp) = ((xsd__byte *) value)->__item;
      break;
    case opc_eDataType_unsignedLong:
      (*(pwr_tUInt64 *) bufp) = ((xsd__unsignedLong *) value)->__item;
      break;
    case opc_eDataType_unsignedInt:
      (*(pwr_tUInt64 *) bufp) = ((xsd__unsignedInt *) value)->__item;
      break;
    case opc_eDataType_unsignedShort:
      (*(pwr_tUInt64 *) bufp) = ((xsd__unsignedShort *) value)->__item;
      break;
    case opc_eDataType_unsignedByte:
      (*(pwr_tUInt64 *) bufp) = ((xsd__unsignedByte *) value)->__item;
      break;
    case opc_eDataType_dateTime:
    case opc_eDataType_string:
    case opc_eDataType_decimal:
    default:
      return false;
    }
    break;
  case pwr_eType_String:
    switch (opc_type) {
    case opc_eDataType_string:
      strncpy((char *) bufp, ((xsd__string *) value)->__item.c_str(), size );
      break;
    case opc_eDataType_boolean:
      if (((xsd__boolean *) value)->__item)
	sprintf((char *) bufp, "true");
      else
	sprintf((char *) bufp, "false");
      break;
    case opc_eDataType_float:
      snprintf((char *)bufp, size, "%f", ((xsd__float *) value)->__item);
      break;
    case opc_eDataType_double:
      snprintf((char *)bufp, size, "%f", ((xsd__double *) value)->__item);
      break;
    case opc_eDataType_long:
#if defined (HW_X86_64)
      snprintf((char *)bufp, size, "%ld", ((xsd__long *) value)->__item);
#else
      snprintf((char *)bufp, size, "%lld", ((xsd__long *) value)->__item);
#endif
      break;
    case opc_eDataType_int:
      snprintf((char *)bufp, size, "%d", ((xsd__int *) value)->__item);
      break;
    case opc_eDataType_short:
      snprintf((char *)bufp, size, "%hd", ((xsd__short *) value)->__item);
      break;
    case opc_eDataType_byte:
      snprintf((char *)bufp, size, "%hhd", ((xsd__byte *) value)->__item);
      break;
    case opc_eDataType_unsignedLong:
#if defined (HW_X86_64)
      snprintf((char *)bufp, size, "%lu", ((xsd__unsignedLong *) value)->__item);
#else
      snprintf((char *)bufp, size, "%llu", ((xsd__unsignedLong *) value)->__item);
#endif
      break;
    case opc_eDataType_unsignedInt:
      snprintf((char *)bufp, size, "%u", ((xsd__unsignedInt *) value)->__item);
      break;
    case opc_eDataType_unsignedShort:
      snprintf((char *)bufp, size, "%hu", ((xsd__unsignedShort *) value)->__item);
      break;
    case opc_eDataType_unsignedByte:
      snprintf((char *)bufp, size, "%hhu", ((xsd__unsignedByte *) value)->__item);
      break;
    case opc_eDataType_dateTime:
      strncpy((char *) bufp, ((xsd__dateTime *) value)->__item.c_str(), size );
      break;
    case opc_eDataType_decimal:
    default:
      return false;
    }
    break;
  case pwr_eType_Time:
    switch (opc_type) {
    case opc_eDataType_string:
    case opc_eDataType_boolean:
    case opc_eDataType_float:
    case opc_eDataType_decimal:
    case opc_eDataType_double:
    case opc_eDataType_long:
    case opc_eDataType_int:
    case opc_eDataType_short:
    case opc_eDataType_byte:
    case opc_eDataType_unsignedLong:
    case opc_eDataType_unsignedInt:
    case opc_eDataType_unsignedShort:
    case opc_eDataType_unsignedByte:
      break;
    case opc_eDataType_dateTime:
      opc_time_OPCAsciiToA( (char *)((xsd__dateTime *)value)->__item.c_str(), (pwr_tTime *)bufp);
      break;
    default:
      return false;
    }
    break;
  case pwr_eType_DeltaTime:
    switch (opc_type) {
    case opc_eDataType_string:
    case opc_eDataType_boolean:
    case opc_eDataType_float:
    case opc_eDataType_decimal:
    case opc_eDataType_double:
    case opc_eDataType_long:
    case opc_eDataType_int:
    case opc_eDataType_short:
    case opc_eDataType_byte:
    case opc_eDataType_unsignedLong:
    case opc_eDataType_unsignedInt:
    case opc_eDataType_unsignedShort:
    case opc_eDataType_unsignedByte:
    case opc_eDataType_dateTime:
      break;
    default:
      return false;
    }
    break;
  default:
    return false;
  }
  
  return true;

}

//
// Return the corresponding opc type for a opc type string
//
bool opc_convert_pwrtype_to_opctype(void *bufin, void *bufout, int size, int opc_type, int pwr_type)
{
  if ( !bufout)
    bufout = bufin;
  switch (opc_type) {
    case opc_eDataType_string:
      switch (pwr_type) {
        case pwr_eType_Float64:
	  snprintf((char *)bufout, size, "%f", *(pwr_tFloat64 *) bufin );
	  break;
        case pwr_eType_Float32:
	  snprintf((char *)bufout, size, "%f", *(pwr_tFloat32 *) bufin );
	  break;
        case pwr_eType_Char:
	  snprintf((char *)bufout, size, "%c", *(pwr_tChar *) bufin );
          break;
        case pwr_eType_Int8:
	  snprintf((char *)bufout, size, "%hhd", *(pwr_tChar *) bufin );
          break;
        case pwr_eType_Int16:
	  snprintf((char *)bufout, size, "%hd", *(pwr_tInt16 *) bufin );
          break;
        case pwr_eType_Int32:
        case pwr_eType_Boolean:
	  snprintf((char *)bufout, size, "%d", *(pwr_tInt32 *) bufin );
          break;
        case pwr_eType_Int64:
	  snprintf((char *)bufout, size, pwr_dFormatInt64, *(pwr_tInt64 *) bufin );
          break;
        case pwr_eType_UInt8:
	  snprintf((char *)bufout, size, "%hhu", *(pwr_tUInt8 *) bufin );
          break;
        case pwr_eType_UInt16:
	  snprintf((char *)bufout, size, "%hu", *(pwr_tUInt16 *) bufin );
          break;
        case pwr_eType_UInt32:
        case pwr_eType_Enum:
        case pwr_eType_Mask:
        case pwr_eType_Status:
        case pwr_eType_NetStatus:
	  snprintf((char *)bufout, size, "%u", *(pwr_tInt32 *) bufin );
          break;
        case pwr_eType_UInt64:
	  snprintf((char *)bufout, size, pwr_dFormatUInt64, *(pwr_tInt64 *) bufin );
          break;
        case pwr_eType_String:
	  if ( bufout != bufin)
	    strncpy( (char *)bufout, (char *)bufin, size);
	  break;
        case pwr_eType_Time:
	  pwr_tTime  at;
	  at = (*(pwr_tTime *) bufin);
	  time_AtoOPCAscii(&at, (char *) bufout, size);
	  break;
        case pwr_eType_DeltaTime:
          time_DtoAscii ((pwr_tDeltaTime *) bufin, 0, (char *) bufout, size);
	  break;
      }
      break;
    case opc_eDataType_boolean:
      switch (pwr_type) {
        case pwr_eType_Float32:
	  (*(char *) bufout) = (*(pwr_tFloat32 *) bufin) ? 1 : 0;
	  break;
        case pwr_eType_Float64:
	  (*(char *) bufout) = (*(pwr_tFloat64 *) bufin) ? 1 : 0;
	  break;
        case pwr_eType_Char:
        case pwr_eType_Int8:
	  *(char *) bufout = (*(pwr_tInt8 *) bufin) ? 1 : 0;
	  break;
        case pwr_eType_Int16:
	  (*(char *) bufout) = (*(pwr_tInt16 *) bufin) ? 1 : 0;
	  break;
        case pwr_eType_Int32:
        case pwr_eType_Boolean:
	  (*(char *) bufout) = (*(pwr_tInt32 *) bufin) ? 1 : 0;
	  break;
        case pwr_eType_Int64:
	  (*(char *) bufout) = (*(pwr_tInt64 *) bufin) ? 1 : 0;
	  break;
        case pwr_eType_UInt8:
	  *(char *) bufout = *(unsigned char *) bufin;
	  break;
        case pwr_eType_UInt16:
	  (*(char *) bufout) = (*(pwr_tUInt16 *) bufin) ? 1 : 0;
	  break;
        case pwr_eType_UInt32:
	  (*(char *) bufout) = (*(pwr_tUInt32 *) bufin) ? 1 : 0;
	  break;
        case pwr_eType_UInt64:
	  (*(char *) bufout) = (*(pwr_tUInt64 *) bufin) ? 1 : 0;
	  break;
        case pwr_eType_String:
        case pwr_eType_Time:
        case pwr_eType_DeltaTime:
	  return false;
      }
      break;
    case opc_eDataType_float:
      switch (pwr_type) {
        case pwr_eType_Float32:
	  (*(float *) bufout) = (*(pwr_tFloat32 *) bufin);
	  break;
        case pwr_eType_Float64:
	  (*(float *) bufout) = (*(pwr_tFloat64 *) bufin);
	  break;
        case pwr_eType_Char:
        case pwr_eType_Int8:
	  (*(float *) bufout) = (*(pwr_tChar *) bufin);
	  break;
        case pwr_eType_Int16:
	  (*(float *) bufout) = (*(pwr_tInt16 *) bufin);
	  break;
        case pwr_eType_Int32:
        case pwr_eType_Boolean:
	  (*(float *) bufout) = (*(pwr_tInt32 *) bufin);
	  break;
        case pwr_eType_Int64:
	  (*(float *) bufout) = (*(pwr_tInt64 *) bufin);
	  break;
        case pwr_eType_UInt8:
	  (*(float *) bufout) = (*(pwr_tUInt8 *) bufin);
	  break;
        case pwr_eType_UInt16:
	  (*(float *) bufout) = (*(pwr_tUInt16 *) bufin);
	  break;
        case pwr_eType_UInt32:
	  (*(float *) bufout) = (*(pwr_tUInt32 *) bufin);
	  break;
        case pwr_eType_UInt64:
	  (*(float *) bufout) = (*(pwr_tUInt64 *) bufin);
	  break;
        case pwr_eType_String:
        case pwr_eType_Time:
        case pwr_eType_DeltaTime:
	  return false;
      }
      break;
    case opc_eDataType_double:
    case opc_eDataType_decimal:
      switch (pwr_type) {
        case pwr_eType_Float32:
	  (*(double *) bufout) = (*(pwr_tFloat32 *) bufin);
	  break;
        case pwr_eType_Float64:
	  (*(double *) bufout) = (*(pwr_tFloat64 *) bufin);
	  break;
        case pwr_eType_Char:
        case pwr_eType_Int8:
	  (*(double *) bufout) = (*(pwr_tChar *) bufin);
	  break;
        case pwr_eType_Int16:
	  (*(double *) bufout) = (*(pwr_tInt16 *) bufin);
	  break;
        case pwr_eType_Int32:
        case pwr_eType_Boolean:
	  (*(double *) bufout) = (*(pwr_tInt32 *) bufin);
	  break;
        case pwr_eType_Int64:
	  (*(double *) bufout) = (*(pwr_tInt64 *) bufin);
	  break;
        case pwr_eType_UInt8:
	  (*(double *) bufout) = (*(pwr_tUInt8 *) bufin);
	  break;
        case pwr_eType_UInt16:
	  (*(double *) bufout) = (*(pwr_tUInt16 *) bufin);
	  break;
        case pwr_eType_UInt32:
	  (*(double *) bufout) = (*(pwr_tUInt32 *) bufin);
	  break;
        case pwr_eType_UInt64:
	  (*(double *) bufout) = (*(pwr_tUInt64 *) bufin);
	  break;
        case pwr_eType_String:
        case pwr_eType_Time:
        case pwr_eType_DeltaTime:
	  return false;
      }
      break;
    case opc_eDataType_int:
      switch (pwr_type) {
        case pwr_eType_Float32:
	  (*(int *) bufout) = (int) (*(pwr_tFloat32 *) bufin);
	  break;
        case pwr_eType_Float64:
	  (*(int *) bufout) = (int) (*(pwr_tFloat64 *) bufin);
	  break;
        case pwr_eType_Char:
        case pwr_eType_Int8:
	  (*(int *) bufout) = (int) (*(pwr_tChar *) bufin);
	  break;
        case pwr_eType_Int16:
	  (*(int *) bufout) = (int) (*(pwr_tInt16 *) bufin);
	  break;
        case pwr_eType_Boolean:
        case pwr_eType_Int32:
	  (*(int *) bufout) = (int) (*(pwr_tInt32 *) bufin);
	  break;
        case pwr_eType_Int64:
	  (*(int *) bufout) = (int) (*(pwr_tInt64 *) bufin);
	  break;
        case pwr_eType_UInt8:
	  (*(int *) bufout) = (int) (*(pwr_tUInt8 *) bufin);
	  break;
        case pwr_eType_UInt16:
	  (*(int *) bufout) = (int) (*(pwr_tUInt16 *) bufin);
	  break;
        case pwr_eType_UInt32:
	  (*(int *) bufout) = (int) (*(pwr_tUInt32 *) bufin);
	  break;
        case pwr_eType_UInt64:
	  (*(int *) bufout) = (int) (*(pwr_tUInt64 *) bufin);
	  break;
        case pwr_eType_String:
        case pwr_eType_Time:
        case pwr_eType_DeltaTime:
	  return false;
      }
      break;
    case opc_eDataType_unsignedInt:
      switch (pwr_type) {
        case pwr_eType_Float32:
	  (*(unsigned int *) bufout) = (unsigned int) (*(pwr_tFloat32 *) bufin);
	  break;
        case pwr_eType_Float64:
	  (*(unsigned int *) bufout) = (unsigned int) (*(pwr_tFloat64 *) bufin);
	  break;
        case pwr_eType_Char:
        case pwr_eType_Int8:
	  (*(unsigned int *) bufout) = (unsigned int) (*(pwr_tChar *) bufin);
	  break;
        case pwr_eType_Int16:
	  (*(unsigned int *) bufout) = (unsigned int) (*(pwr_tInt16 *) bufin);
	  break;
        case pwr_eType_Boolean:
        case pwr_eType_Int32:
	  (*(unsigned int *) bufout) = (unsigned int) (*(pwr_tInt32 *) bufin);
	  break;
        case pwr_eType_Int64:
	  (*(unsigned int *) bufout) = (unsigned int) (*(pwr_tInt64 *) bufin);
	  break;
        case pwr_eType_UInt8:
	  (*(unsigned int *) bufout) = (unsigned int) (*(pwr_tUInt8 *) bufin);
	  break;
        case pwr_eType_UInt16:
	  (*(unsigned int *) bufout) = (unsigned int) (*(pwr_tUInt16 *) bufin);
	  break;
        case pwr_eType_UInt32:
	  (*(unsigned int *) bufout) = (unsigned int) (*(pwr_tUInt32 *) bufin);
	  break;
        case pwr_eType_UInt64:
	  (*(unsigned int *) bufout) = (unsigned int) (*(pwr_tUInt64 *) bufin);
	  break;
        case pwr_eType_String:
        case pwr_eType_Time:
        case pwr_eType_DeltaTime:
	  return false;
      }
      break;
    case opc_eDataType_short:
      switch (pwr_type) {
        case pwr_eType_Float32:
	  (*(short int *) bufout) = (short int) (*(pwr_tFloat32 *) bufin);
	  break;
        case pwr_eType_Float64:
	  (*(short int *) bufout) =  (short int) (*(pwr_tFloat64 *) bufin);
	  break;
        case pwr_eType_Char:
        case pwr_eType_Int8:
	  (*(short int *) bufout) =  (short int) (*(pwr_tChar *) bufin);
	  break;
        case pwr_eType_Int16:
	  (*(short int *) bufout) =  (short int) (*(pwr_tInt16 *) bufin);
	  break;
        case pwr_eType_Int32:
        case pwr_eType_Boolean:
	  (*(short int *) bufout) =  (short int) (*(pwr_tInt32 *) bufin);
	  break;
        case pwr_eType_Int64:
	  (*(short int *) bufout) =  (short int) (*(pwr_tInt64 *) bufin);
	  break;
        case pwr_eType_UInt8:
	  (*(short int *) bufout) =  (short int) (*(pwr_tUInt8 *) bufin);
	  break;
        case pwr_eType_UInt16:
	  (*(int *) bufout) =  (short int) (*(pwr_tUInt16 *) bufin);
	  break;
        case pwr_eType_UInt32:
	  (*(short int *) bufout) =  (short int) (*(pwr_tUInt32 *) bufin);
	  break;
        case pwr_eType_UInt64:
	  (*(short int *) bufout) =  (short int) (*(pwr_tUInt64 *) bufin);
	  break;
        case pwr_eType_String:
        case pwr_eType_Time:
        case pwr_eType_DeltaTime:
	  return false;
      }
      break;
    case opc_eDataType_unsignedShort:
      switch (pwr_type) {
        case pwr_eType_Float32:
	  (*(unsigned short int *) bufout) = (unsigned short int) (*(pwr_tFloat32 *) bufin);
	  break;
        case pwr_eType_Float64:
	  (*(unsigned short int *) bufout) =  (unsigned short int) (*(pwr_tFloat64 *) bufin);
	  break;
        case pwr_eType_Char:
        case pwr_eType_Int8:
	  (*(unsigned short int *) bufout) =  (unsigned short int) (*(pwr_tChar *) bufin);
	  break;
        case pwr_eType_Int16:
	  (*(unsigned short int *) bufout) =  (unsigned short int) (*(pwr_tInt16 *) bufin);
	  break;
        case pwr_eType_Int32:
        case pwr_eType_Boolean:
	  (*(unsigned short int *) bufout) =  (unsigned short int) (*(pwr_tInt32 *) bufin);
	  break;
        case pwr_eType_Int64:
	  (*(unsigned short int *) bufout) =  (unsigned short int) (*(pwr_tInt64 *) bufin);
	  break;
        case pwr_eType_UInt8:
	  (*(unsigned short int *) bufout) =  (unsigned short int) (*(pwr_tUInt8 *) bufin);
	  break;
        case pwr_eType_UInt16:
	  (*(unsigned short int *) bufout) =  (unsigned short int) (*(pwr_tUInt16 *) bufin);
	  break;
        case pwr_eType_UInt32:
	  (*(unsigned short int *) bufout) =  (unsigned short int) (*(pwr_tUInt32 *) bufin);
	  break;
        case pwr_eType_UInt64:
	  (*(unsigned short int *) bufout) =  (unsigned short int) (*(pwr_tUInt64 *) bufin);
	  break;
        case pwr_eType_String:
        case pwr_eType_Time:
        case pwr_eType_DeltaTime:
	  return false;
      }
      break;
    case opc_eDataType_byte:
      switch (pwr_type) {
        case pwr_eType_Float32:
	  (*(char *) bufout) = (char) (*(pwr_tFloat32 *) bufin);
	  break;
        case pwr_eType_Float64:
	  (*(char *) bufout) = (char) (*(pwr_tFloat64 *) bufin);
	  break;
        case pwr_eType_Char:
        case pwr_eType_Int8:
	  *(char *) bufout = *(char *)bufin;
	  break;
        case pwr_eType_Int16:
	  (*(char *) bufout) = (char) (*(pwr_tInt16 *) bufin);
	  break;
        case pwr_eType_Int32:
        case pwr_eType_Boolean:
	  (*(char *) bufout) = (char) (*(pwr_tInt32 *) bufin);
	  break;
        case pwr_eType_Int64:
	  (*(char *) bufout) = (char) (*(pwr_tInt64 *) bufin);
	  break;
        case pwr_eType_UInt8:
	  *(char *) bufout = (char) (*(pwr_tUInt8 *)bufin);
	  break;
        case pwr_eType_UInt16:
	  (*(char *) bufout) = (char) (*(pwr_tUInt16 *) bufin);
	  break;
        case pwr_eType_UInt32:
	  (*(char *) bufout) = (char) (*(pwr_tUInt32 *) bufin);
	  break;
        case pwr_eType_UInt64:
	  (*(char *) bufout) = (char) (*(pwr_tUInt64 *) bufin);
	  break;
        case pwr_eType_String:
        case pwr_eType_Time:
        case pwr_eType_DeltaTime:
	  return false;
      }
      break;
    case opc_eDataType_unsignedByte:
      switch (pwr_type) {
        case pwr_eType_Float32:
	  (*(unsigned char *) bufout) = (unsigned char) (*(pwr_tFloat32 *) bufin);
	  break;
        case pwr_eType_Float64:
	  (*(unsigned char *) bufout) = (unsigned char) (*(pwr_tFloat64 *) bufin);
	  break;
        case pwr_eType_Char:
        case pwr_eType_Int8:
	  *(unsigned char *) bufout = *(unsigned char *)bufin;
	  break;
        case pwr_eType_Int16:
	  (*(unsigned char *) bufout) = (unsigned char) (*(pwr_tInt16 *) bufin);
	  break;
        case pwr_eType_Int32:
        case pwr_eType_Boolean:
	  (*(unsigned char *) bufout) = (unsigned char) (*(pwr_tInt32 *) bufin);
	  break;
        case pwr_eType_Int64:
	  (*(unsigned char *) bufout) = (unsigned char) (*(pwr_tInt64 *) bufin);
	  break;
        case pwr_eType_UInt8:
	  *(unsigned char *) bufout = (unsigned char) (*(pwr_tUInt8 *)bufin);
	  break;
        case pwr_eType_UInt16:
	  (*(unsigned char *) bufout) = (unsigned char) (*(pwr_tUInt16 *) bufin);
	  break;
        case pwr_eType_UInt32:
	  (*(unsigned char *) bufout) = (unsigned char) (*(pwr_tUInt32 *) bufin);
	  break;
        case pwr_eType_UInt64:
	  (*(unsigned char *) bufout) = (unsigned char) (*(pwr_tUInt64 *) bufin);
	  break;
        case pwr_eType_String:
        case pwr_eType_Time:
        case pwr_eType_DeltaTime:
	  return false;
      }
      break;
    case opc_eDataType_long:
      switch (pwr_type) {
        case pwr_eType_Float32:
	  (*(pwr_tInt64 *) bufout) = (pwr_tInt64) (*(pwr_tFloat32 *) bufin);
	  break;
        case pwr_eType_Float64:
	  (*(pwr_tInt64 *) bufout) = (pwr_tInt64) (*(pwr_tFloat64 *) bufin);
	  break;
        case pwr_eType_Char:
        case pwr_eType_Int8:
	  (*(pwr_tInt64 *) bufout) = (pwr_tInt64) (*(pwr_tChar *) bufin);
	  break;
        case pwr_eType_Int16:
	  (*(pwr_tInt64 *) bufout) = (pwr_tInt64) (*(pwr_tInt16 *) bufin);
	  break;
        case pwr_eType_Int32:
        case pwr_eType_Boolean:
	  (*(pwr_tInt64 *) bufout) = (pwr_tInt64) (*(pwr_tInt32 *) bufin);
	  break;
        case pwr_eType_Int64:
	  (*(pwr_tInt64 *) bufout) = (pwr_tInt64) (*(pwr_tInt64 *) bufin);
	  break;
        case pwr_eType_UInt8:
	  (*(pwr_tInt64 *) bufout) = (pwr_tInt64) (*(pwr_tUInt8 *) bufin);
	  break;
        case pwr_eType_UInt16:
	  (*(pwr_tInt64 *) bufout) = (pwr_tInt64) (*(pwr_tUInt16 *) bufin);
	  break;
        case pwr_eType_UInt32:
	  (*(pwr_tInt64 *) bufout) = (pwr_tInt64) (*(pwr_tUInt32 *) bufin);
	  break;
        case pwr_eType_UInt64:
	  (*(pwr_tInt64 *) bufout) = (pwr_tInt64) (*(pwr_tUInt64 *) bufin);
	  break;
        case pwr_eType_String:
        case pwr_eType_Time:
        case pwr_eType_DeltaTime:
	  return false;
      }
      break;
    case opc_eDataType_unsignedLong:
      switch (pwr_type) {
        case pwr_eType_Float32:
	  (*(pwr_tUInt64 *) bufout) = (pwr_tUInt64) (*(pwr_tFloat32 *) bufin);
	  break;
        case pwr_eType_Float64:
	  (*(pwr_tUInt64 *) bufout) = (pwr_tUInt64) (*(pwr_tFloat64 *) bufin);
	  break;
        case pwr_eType_Char:
        case pwr_eType_Int8:
	  (*(pwr_tUInt64 *) bufout) = (pwr_tUInt64) (*(pwr_tChar *) bufin);
	  break;
        case pwr_eType_Int16:
	  (*(pwr_tUInt64 *) bufout) = (pwr_tUInt64) (*(pwr_tInt16 *) bufin);
	  break;
        case pwr_eType_Int32:
        case pwr_eType_Boolean:
	  (*(pwr_tUInt64 *) bufout) = (pwr_tUInt64) (*(pwr_tInt32 *) bufin);
	  break;
        case pwr_eType_Int64:
	  (*(pwr_tUInt64 *) bufout) = (pwr_tUInt64) (*(pwr_tInt64 *) bufin);
	  break;
        case pwr_eType_UInt8:
	  (*(pwr_tUInt64 *) bufout) = (pwr_tUInt64) (*(pwr_tUInt8 *) bufin);
	  break;
        case pwr_eType_UInt16:
	  (*(pwr_tUInt64 *) bufout) = (pwr_tUInt64) (*(pwr_tUInt16 *) bufin);
	  break;
        case pwr_eType_UInt32:
	  (*(pwr_tUInt64 *) bufout) = (pwr_tUInt64) (*(pwr_tUInt32 *) bufin);
	  break;
        case pwr_eType_UInt64:
	  (*(pwr_tUInt64 *) bufout) = (pwr_tUInt64) (*(pwr_tUInt64 *) bufin);
	  break;
        case pwr_eType_String:
        case pwr_eType_Time:
        case pwr_eType_DeltaTime:
	  return false;
      }
      break;
    case opc_eDataType_dateTime:
      switch (pwr_type) {
      case pwr_eType_Time:
	memcpy( bufout, bufin, sizeof(pwr_tTime));
	//time_AtoOPCAscii( (pwr_tTime *)bufin, (char *)bufout, size);
	break;
      default:
	return false;
      }
      break;
    case opc_eDataType_duration:
    case opc_eDataType_base64Binary:
    case opc_eDataType_time:
    case opc_eDataType_date:
    case opc_eDataType_QName:
    case opc_eDataType_anyType :
      return false;
  }
  
  return true;
}

//
// Return the corresponding opc type for a opc type string
//
bool opc_string_to_opctype(const char *str, int *type)
{
  int ii;

  for (ii = 0; ii < opc_cDataTypeSize; ii++) {
    if (strncmp(str_dt[ii], str, sizeof(str_dt[ii])) == 0) {
      *type  = ii;
      return true;
    }
  }
  
  return false;
}
//
// Return the corresponding opc type string for a opc type
//
char *opc_opctype_to_string(int type)
{

  return str_dt[type];
  
}

bool opc_pwrtype_to_opctype(int pwrtype, int *opctype) 
{
  switch (pwrtype) {
  case pwr_eType_String:
  case pwr_eType_Objid:
  case pwr_eType_AttrRef:
    *opctype = opc_eDataType_string;
    break;
  case pwr_eType_Boolean:
    *opctype = opc_eDataType_boolean;
    break;
  case pwr_eType_Float32:
    *opctype = opc_eDataType_float;
    break;
  case pwr_eType_Float64:
    *opctype = opc_eDataType_double;
    break;
  case pwr_eType_Enum:
  case pwr_eType_Mask:
  case pwr_eType_Status:
  case pwr_eType_NetStatus:
  case pwr_eType_Int32:
    *opctype = opc_eDataType_int;
    break;
  case pwr_eType_Int64:
    *opctype = opc_eDataType_long;
    break;
  case pwr_eType_Int16:
    *opctype = opc_eDataType_short;
    break;
  case pwr_eType_Int8:
    *opctype = opc_eDataType_byte;
    break;
  case pwr_eType_UInt32:
    *opctype = opc_eDataType_unsignedInt;
    break;
  case pwr_eType_UInt64:
    *opctype = opc_eDataType_unsignedLong;
    break;
  case pwr_eType_UInt16:
    *opctype = opc_eDataType_unsignedShort;
    break;
  case pwr_eType_UInt8:
    *opctype = opc_eDataType_unsignedByte;
    break;
  case pwr_eType_Time:
    *opctype = opc_eDataType_dateTime;
    break;
  case pwr_eType_DeltaTime:
    *opctype = opc_eDataType_duration;
    break;
  default:
    *opctype = opc_eDataType_;
    return false;
  }
  return true;
}
bool opc_opctype_to_pwrtype(int type, int *pwrtype) 
{
  switch (type) {

  case opc_eDataType_string:
    *pwrtype = pwr_eType_String;
    break;
  case opc_eDataType_boolean:
    *pwrtype = pwr_eType_Boolean;
    break;
  case opc_eDataType_float:
    *pwrtype = pwr_eType_Float32;
    break;
  case opc_eDataType_double:
    *pwrtype = pwr_eType_Float64;
    break;
  case opc_eDataType_int:
    *pwrtype = pwr_eType_Int32;
    break;
  case opc_eDataType_short:
    *pwrtype = pwr_eType_Int16;
    break;
  case opc_eDataType_byte:
    *pwrtype = pwr_eType_Int8;
    break;
  case opc_eDataType_unsignedInt:
    *pwrtype = pwr_eType_UInt32;
    break;
  case opc_eDataType_unsignedShort:
    *pwrtype = pwr_eType_UInt16;
    break;
  case opc_eDataType_unsignedByte:
    *pwrtype = pwr_eType_UInt8;
    break;
  case opc_eDataType_dateTime:
    *pwrtype = pwr_eType_Time;
    break;
  case opc_eDataType_duration:
    *pwrtype = pwr_eType_DeltaTime;
    break;
  case opc_eDataType_long:
    *pwrtype = pwr_eType_Int64;
    break;
  case opc_eDataType_unsignedLong:
    *pwrtype = pwr_eType_UInt64;
    break;
  case opc_eDataType_QName:
  case opc_eDataType_anyType: 
  case opc_eDataType_decimal:
  case opc_eDataType_base64Binary:
  case opc_eDataType_time:
  case opc_eDataType_date:
  default:
    *pwrtype = 0;
    return false;
  }
  
  return true;
}

//
// Return the corresponding opc type string for a pwr_eType
//
bool opc_pwrtype_to_string( int type, char **str)
{
  int opctype;

  if ( !opc_pwrtype_to_opctype( type, &opctype))
    return false;

  *str = str_dt[opctype];
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

bool opc_get_property( std::vector<s0__ItemProperty *> properties, unsigned int mask,
		       xsd__anyType **valp)
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
  char name[200];
  char *np;

  *mask = 0;

  for ( int i = 0; i < (int) pnames.size(); i++) {
    strcpy( name, pnames[i].c_str());
    if ( (np = strrchr( name, ':')))
      np++;
    else
      np = name;

    switch ( *np) {
    case 'a':
      *mask |= opc_mProperty_AccessRights;
      break;
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
  case s0__qualityBits__bad:
    strcpy( *str, "bad");
    break;
  case s0__qualityBits__badConfigurationError:
    strcpy( *str, "badConfigurationError");
    break;
  case s0__qualityBits__badNotConnected:
    strcpy( *str, "badNotConnected");
    break;
  case s0__qualityBits__badDeviceFailure:
    strcpy( *str, "badDeviceFailure");
    break;
  case s0__qualityBits__badSensorFailure:
    strcpy( *str, "badSensorFailure");
    break;
  case s0__qualityBits__badLastKnownValue:
    strcpy( *str, "badLastKnownValue");
    break;
  case s0__qualityBits__badCommFailure:
    strcpy( *str, "badCommFailure");
    break;
  case s0__qualityBits__badOutOfService:
    strcpy( *str, "badOutOfService");
    break;
  case s0__qualityBits__badWaitingForInitialData:
    strcpy( *str, "badWaitingForInitialData");
    break;
  case s0__qualityBits__uncertain:
    strcpy( *str, "uncertain");
    break;
  case s0__qualityBits__uncertainLastUsableValue:
    strcpy( *str, "uncertainLastUsableValue");
    break;
  case s0__qualityBits__uncertainSensorNotAccurate:
    strcpy( *str, "uncertainSensorNotAccurate");
    break;
  case s0__qualityBits__uncertainEUExceeded:
    strcpy( *str, "uncertainEUExceede");
    break;
  case s0__qualityBits__uncertainSubNormal:
    strcpy( *str, "uncertainSubNormal");
    break;
  case s0__qualityBits__good:
    strcpy( *str, "good");
    break;
  case s0__qualityBits__goodLocalOverride:
    strcpy( *str, "goodLocalOverride");
    break;
  default:
    return false;
  }
  return true;
}

bool opc_cmp_pwr( void *p1, void *p2, int size, int type, float deadband)
{
  switch ( type) {
  case pwr_eType_Boolean:
  case pwr_eType_Int32:
  case pwr_eType_UInt32:
    if ( *(pwr_tUInt32 *)p1 == *(pwr_tUInt32 *)p2)
      return true;
    break;
  case pwr_eType_Float32:
    if ( deadband == 0) {
      if ( fabs( *(pwr_tFloat32 *)p1 - *(pwr_tFloat32 *)p2) < FLT_EPSILON)
	return true;
    }
    else {
      if ( fabs( *(pwr_tFloat32 *)p1 - *(pwr_tFloat32 *)p2) < deadband)
	return true;
    }
    break;
  default: ;
  }
  return false;
}

