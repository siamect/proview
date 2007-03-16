/* 
 * Proview   $Id: opc_utl.cpp,v 1.11 2007-03-16 10:19:45 claes Exp $
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
#include "co_time.h"
#include "co_time_msg.h"
#include "opc_utl.h"
#include "opc_soap_Stub.h"

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

static char opc_ResultCodes[23][32] = {"s:S_CLAMP",
				       "s:S_DATAQUEUEOVERFLOW",
				       "s:S_UNSUPPORTEDRATE",
				       "s:E_ACCESS_DENIED",
				       "s:E_BUSY",
				       "s:E_FAIL",
				       "s:E_INVALIDCONTINUATIONPOINT",
				       "s:E_INVALIDFILTER",
				       "s:E_INVALIDHOLDTIME",
				       "s:E_INVALIDITEMNAME",
				       "s:E_INVALIDITEMPATH",
				       "s:E_INVALIDPID",
				       "s:E_NOSUBSCRIPTION",
				       "s:E_NOTSUPPORTED",
				       "s:E_OUTOFMEMORY",
				       "s:E_RANGE",
				       "s:E_READONLY",
				       "s:E_SERVERSTATE",
				       "s:E_TIMEDOUT",
				       "s:E_UNKNOWNITEMNAME",
				       "s:E_UNKNOWNITEMPATH",
				       "s:E_WRITEONLY",
				       "s:E_BADTYPE"};

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

void opcsrv_returnerror(std::vector<s0__OPCError *>& errors, std::string **rc, int err_code, unsigned int options)
{
  int ii;
  bool exists = false;

	
  if ( rc)
    *rc = new std::string( opc_resultcode_to_string( err_code));
  
  for (ii = 0; ii < (int) errors.size(); ii++) {
    if (strcmp( errors[ii]->ID.c_str(), opc_resultcode_to_string( err_code)) == 0) {
      exists = true;
    }
  }
  
  if (!exists) {
    s0__OPCError *oe = new s0__OPCError();
    oe->ID = std::string(opc_resultcode_to_string( err_code));
    
    if (options & opc_mRequestOption_ReturnErrorText) {
      oe->Text = new std::string( opc_resultcode_to_text( err_code));
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

std::string& opc_datetime( pwr_tTime *tp)
{
  static std::string timstr;
  char str[40];

  time_AtoOPCAscii( tp, str, sizeof(str));
  timstr = std::string( str);
  return timstr;
}

pwr_tStatus time_AtoOPCAscii (pwr_tTime *tp, char *buf, int bufsize)
{

   struct tm *tmpTm;
   int        buflen;
   char       tmpStr[16];
   pwr_tTime  t;

   if ( !tp) {
     clock_gettime( CLOCK_REALTIME, &t);
     tp = &t;
   }
  
   tmpTm = localtime(&tp->tv_sec);
   strftime(buf, bufsize, "%Y-%m-%dT%H:%M:%S", tmpTm);

   sprintf(tmpStr, ".%07d", (int)(tp->tv_nsec / 100));
   buflen = strlen(buf);
   if (strlen(tmpStr) + buflen < (unsigned int) bufsize)
     strcpy(&buf[buflen], tmpStr);
 
   return TIME__SUCCESS;
}
//
// Return the corresponding opc type for a opc type string
//
bool opc_opctype_to_value(void *bufp, int size, int opc_type)
{
  switch (opc_type) {
    case opc_eDataType_string:
      break;
    case opc_eDataType_boolean:
      if (*(char *) bufp) {
        sprintf((char *) bufp, "true");
      } else {
        sprintf((char *) bufp, "false");
      }
      break;
    case opc_eDataType_float:
      snprintf((char *)bufp, size, "%f", *(pwr_tFloat32 *) bufp );
      break;
    case opc_eDataType_decimal:
    case opc_eDataType_double:
      snprintf((char *)bufp, size, "%f", *(pwr_tFloat64 *) bufp );
      break;
    case opc_eDataType_long:
      snprintf((char *)bufp, size, "%lli", *(long long int *) bufp );
      break;
    case opc_eDataType_int:
      snprintf((char *)bufp, size, "%i", *(pwr_tInt32 *) bufp );
      break;
    case opc_eDataType_short:
      snprintf((char *)bufp, size, "%hi", *(pwr_tInt16 *) bufp );
      break;
    case opc_eDataType_byte:
      snprintf((char *)bufp, size, "%hhi", *(pwr_tChar *) bufp );
      break;
    case opc_eDataType_unsignedLong:
      snprintf((char *)bufp, size, "%llu", *(long long unsigned *) bufp );
      break;
    case opc_eDataType_unsignedInt:
      snprintf((char *)bufp, size, "%u", *(pwr_tInt32 *) bufp );
      break;
    case opc_eDataType_unsignedShort:
      snprintf((char *)bufp, size, "%hu", *(pwr_tUInt16 *) bufp );
      break;
    case opc_eDataType_unsignedByte:
      snprintf((char *)bufp, size, "%hhu", *(pwr_tUInt8 *) bufp );
      break;
    case opc_eDataType_dateTime:
      pwr_tTime  at;
      at = (*(pwr_tTime *) bufp);
      time_AtoOPCAscii(&at, (char *) bufp, size);
      break;
    case opc_eDataType_duration:
      time_DtoAscii ((pwr_tDeltaTime *) bufp, 0, (char *) bufp, size);
      break;
    default:
      break;
  }
  
  return true;
}
//
// Return the corresponding opc type for a opc type string
//
bool opc_convert_pwrtype_to_opctype(void *bufp, int size, int opc_type, int pwr_type)
{
  switch (opc_type) {
    case opc_eDataType_string:
      switch (pwr_type) {
        case pwr_eType_Float64:
	  snprintf((char *)bufp, size, "%f", *(pwr_tFloat64 *) bufp );
	  break;
        case pwr_eType_Float32:
	  snprintf((char *)bufp, size, "%f", *(pwr_tFloat32 *) bufp );
	  break;
        case pwr_eType_Char:
	  snprintf((char *)bufp, size, "%c", *(pwr_tChar *) bufp );
          break;
        case pwr_eType_Int8:
	  snprintf((char *)bufp, size, "%hhi", *(pwr_tChar *) bufp );
          break;
        case pwr_eType_Int16:
	  snprintf((char *)bufp, size, "%hi", *(pwr_tInt16 *) bufp );
          break;
        case pwr_eType_Int32:
        case pwr_eType_Boolean:
	  snprintf((char *)bufp, size, "%i", *(pwr_tInt32 *) bufp );
          break;
        case pwr_eType_UInt8:
	  snprintf((char *)bufp, size, "%hhu", *(pwr_tUInt8 *) bufp );
          break;
        case pwr_eType_UInt16:
	  snprintf((char *)bufp, size, "%hu", *(pwr_tUInt16 *) bufp );
          break;
        case pwr_eType_UInt32:
	  snprintf((char *)bufp, size, "%u", *(pwr_tInt32 *) bufp );
          break;
        case pwr_eType_String:
	  break;
        case pwr_eType_Time:
	  pwr_tTime  at;
	  at = (*(pwr_tTime *) bufp);
	  time_AtoOPCAscii(&at, (char *) bufp, size);
	  break;
        case pwr_eType_UInt64:
	  snprintf((char *)bufp, size, "%llu", *(long long unsigned *) bufp );
          break;
        case pwr_eType_Int64:
	  snprintf((char *)bufp, size, "%lli", *(long long int *) bufp );
	  break;
        case pwr_eType_DeltaTime:
          time_DtoAscii ((pwr_tDeltaTime *) bufp, 0, (char *) bufp, size);
	  break;
      }
      break;
    case opc_eDataType_boolean:
      switch (pwr_type) {
        case pwr_eType_Float32:
	  (*(char *) bufp) = (*(pwr_tFloat32 *) bufp) ? 1 : 0;
	  break;
        case pwr_eType_Float64:
	  (*(char *) bufp) = (*(pwr_tFloat64 *) bufp) ? 1 : 0;
	  break;
        case pwr_eType_Char:
        case pwr_eType_Int8:
//	  *bufp = (char) (char *bufp);
	  break;
        case pwr_eType_Int16:
	  (*(char *) bufp) = (*(pwr_tInt16 *) bufp) ? 1 : 0;
	  break;
        case pwr_eType_Int32:
        case pwr_eType_Boolean:
	  (*(char *) bufp) = (*(pwr_tInt32 *) bufp) ? 1 : 0;
	  break;
        case pwr_eType_UInt8:
//	  *bufp = (char) (unsigned char *bufp);
	  break;
        case pwr_eType_UInt16:
	  (*(char *) bufp) = (*(pwr_tUInt16 *) bufp) ? 1 : 0;
	  break;
        case pwr_eType_UInt32:
	  (*(char *) bufp) = (*(pwr_tUInt32 *) bufp) ? 1 : 0;
	  break;
        case pwr_eType_String:
        case pwr_eType_Time:
        case pwr_eType_UInt64:
        case pwr_eType_Int64:
        case pwr_eType_DeltaTime:
	  return false;
	  break;
      }
      break;
    case opc_eDataType_float:
      switch (pwr_type) {
        case pwr_eType_Float32:
//	  (*(float *) bufp) = (float) (*(pwr_tFloat32 *) bufp);
	  break;
        case pwr_eType_Float64:
	  (*(float *) bufp) = (float) (*(pwr_tFloat64 *) bufp);
	  break;
        case pwr_eType_Char:
        case pwr_eType_Int8:
	  (*(float *) bufp) = (float) (*(pwr_tChar *) bufp);
	  break;
        case pwr_eType_Int16:
	  (*(float *) bufp) = (float) (*(pwr_tInt16 *) bufp);
	  break;
        case pwr_eType_Int32:
        case pwr_eType_Boolean:
	  (*(float *) bufp) = (float) (*(pwr_tInt32 *) bufp);
	  break;
        case pwr_eType_UInt8:
	  (*(float *) bufp) = (float) (*(pwr_tUInt8 *) bufp);
	  break;
        case pwr_eType_UInt16:
	  (*(float *) bufp) = (float) (*(pwr_tUInt16 *) bufp);
	  break;
        case pwr_eType_UInt32:
	  (*(float *) bufp) = (float) (*(pwr_tUInt32 *) bufp);
	  break;
        case pwr_eType_String:
        case pwr_eType_Time:
        case pwr_eType_UInt64:
        case pwr_eType_Int64:
        case pwr_eType_DeltaTime:
	  return false;
	  break;
      }
      break;
    case opc_eDataType_double:
    case opc_eDataType_decimal:
      switch (pwr_type) {
        case pwr_eType_Float32:
	  (*(double *) bufp) = (double) (*(pwr_tFloat32 *) bufp);
	  break;
        case pwr_eType_Float64:
	  (*(double *) bufp) = (double) (*(pwr_tFloat64 *) bufp);
	  break;
        case pwr_eType_Char:
        case pwr_eType_Int8:
	  (*(double *) bufp) = (double) (*(pwr_tChar *) bufp);
	  break;
        case pwr_eType_Int16:
	  (*(double *) bufp) = (double) (*(pwr_tInt16 *) bufp);
	  break;
        case pwr_eType_Int32:
        case pwr_eType_Boolean:
	  (*(double *) bufp) = (double) (*(pwr_tInt32 *) bufp);
	  break;
        case pwr_eType_UInt8:
	  (*(double *) bufp) = (double) (*(pwr_tUInt8 *) bufp);
	  break;
        case pwr_eType_UInt16:
	  (*(double *) bufp) = (double) (*(pwr_tUInt16 *) bufp);
	  break;
        case pwr_eType_UInt32:
	  (*(double *) bufp) = (double) (*(pwr_tUInt32 *) bufp);
	  break;
        case pwr_eType_String:
        case pwr_eType_Time:
        case pwr_eType_UInt64:
        case pwr_eType_Int64:
        case pwr_eType_DeltaTime:
	  return false;
	  break;
      }
      break;
    case opc_eDataType_int:
    case opc_eDataType_unsignedInt:
      switch (pwr_type) {
        case pwr_eType_Float32:
	  (*(int *) bufp) = (int) (*(pwr_tFloat32 *) bufp);
	  break;
        case pwr_eType_Float64:
	  (*(int *) bufp) = (int) (*(pwr_tFloat64 *) bufp);
	  break;
        case pwr_eType_Char:
        case pwr_eType_Int8:
	  (*(int *) bufp) = (int) (*(pwr_tChar *) bufp);
	  break;
        case pwr_eType_Int16:
	  (*(int *) bufp) = (int) (*(pwr_tInt16 *) bufp);
	  break;
        case pwr_eType_Boolean:
        case pwr_eType_Int32:
//	  (*(double *) bufp) = (double) (*(pwr_tInt32 *) bufp);
	  break;
        case pwr_eType_UInt8:
	  (*(int *) bufp) = (int) (*(pwr_tUInt8 *) bufp);
	  break;
        case pwr_eType_UInt16:
	  (*(int *) bufp) = (int) (*(pwr_tUInt16 *) bufp);
	  break;
        case pwr_eType_UInt32:
//	  (*(int *) bufp) = (int) (*(pwr_tUInt32 *) bufp);
	  break;
        case pwr_eType_String:
        case pwr_eType_Time:
        case pwr_eType_UInt64:
        case pwr_eType_Int64:
        case pwr_eType_DeltaTime:
	  return false;
	  break;
      }
      break;
    case opc_eDataType_short:
    case opc_eDataType_unsignedShort:
      switch (pwr_type) {
        case pwr_eType_Float32:
	  (*(short int *) bufp) = (short int) (*(pwr_tFloat32 *) bufp);
	  break;
        case pwr_eType_Float64:
	  (*(short int *) bufp) = (short int) (*(pwr_tFloat64 *) bufp);
	  break;
        case pwr_eType_Char:
        case pwr_eType_Int8:
	  (*(short int *) bufp) = (short int) (*(pwr_tChar *) bufp);
	  break;
        case pwr_eType_Int16:
//	  (*(short int *) bufp) = (short int) (*(pwr_tInt16 *) bufp);
	  break;
        case pwr_eType_Int32:
        case pwr_eType_Boolean:
	  (*(short int *) bufp) = (short int) (*(pwr_tInt32 *) bufp);
	  break;
        case pwr_eType_UInt8:
	  (*(short int *) bufp) = (short int) (*(pwr_tUInt8 *) bufp);
	  break;
        case pwr_eType_UInt16:
//	  (*(int *) bufp) = (int) (*(pwr_tUInt16 *) bufp);
	  break;
        case pwr_eType_UInt32:
	  (*(short int *) bufp) = (short int) (*(pwr_tUInt32 *) bufp);
	  break;
        case pwr_eType_String:
        case pwr_eType_Time:
        case pwr_eType_UInt64:
        case pwr_eType_Int64:
        case pwr_eType_DeltaTime:
	  return false;
	  break;
      }
      break;
    case opc_eDataType_unsignedByte:
    case opc_eDataType_byte:
      switch (pwr_type) {
        case pwr_eType_Float32:
	  (*(char *) bufp) = (char) (*(pwr_tFloat32 *) bufp);
	  break;
        case pwr_eType_Float64:
	  (*(char *) bufp) = (char) (*(pwr_tFloat64 *) bufp);
	  break;
        case pwr_eType_Char:
        case pwr_eType_Int8:
//	  *bufp = (char) (char *bufp);
	  break;
        case pwr_eType_Int16:
	  (*(char *) bufp) = (char) (*(pwr_tInt16 *) bufp);
	  break;
        case pwr_eType_Int32:
        case pwr_eType_Boolean:
	  (*(char *) bufp) = (char) (*(pwr_tInt32 *) bufp);
	  break;
        case pwr_eType_UInt8:
//	  *bufp = (char) (unsigned char *bufp);
	  break;
        case pwr_eType_UInt16:
	  (*(char *) bufp) = (char) (*(pwr_tUInt16 *) bufp);
	  break;
        case pwr_eType_UInt32:
	  (*(char *) bufp) = (char) (*(pwr_tUInt32 *) bufp);
	  break;
        case pwr_eType_String:
        case pwr_eType_Time:
        case pwr_eType_UInt64:
        case pwr_eType_Int64:
        case pwr_eType_DeltaTime:
	  return false;
	  break;
      }
      break;
    case opc_eDataType_unsignedLong:
    case opc_eDataType_long:
      switch (pwr_type) {
        case pwr_eType_Float32:
	  (*(long long *) bufp) = (long long) (*(pwr_tFloat32 *) bufp);
	  break;
        case pwr_eType_Float64:
	  (*(long long *) bufp) = (long long) (*(pwr_tFloat64 *) bufp);
	  break;
        case pwr_eType_Char:
        case pwr_eType_Int8:
	  (*(long long *) bufp) = (long long) (*(pwr_tChar *) bufp);
	  break;
        case pwr_eType_Int16:
	  (*(long long *) bufp) = (long long) (*(pwr_tInt16 *) bufp);
	  break;
        case pwr_eType_Int32:
        case pwr_eType_Boolean:
	  (*(long long *) bufp) = (long long) (*(pwr_tInt32 *) bufp);
	  break;
        case pwr_eType_UInt8:
	  (*(long long *) bufp) = (long long) (*(pwr_tUInt8 *) bufp);
	  break;
        case pwr_eType_UInt16:
	  (*(long long *) bufp) = (long long) (*(pwr_tUInt16 *) bufp);
	  break;
        case pwr_eType_UInt32:
	  (*(long long *) bufp) = (long long) (*(pwr_tUInt32 *) bufp);
	  break;
        case pwr_eType_String:
        case pwr_eType_Time:
        case pwr_eType_UInt64:
        case pwr_eType_Int64:
        case pwr_eType_DeltaTime:
	  return false;
	  break;
      }
      break;
    case opc_eDataType_dateTime:
    case opc_eDataType_duration:
    case opc_eDataType_base64Binary:
    case opc_eDataType_time:
    case opc_eDataType_date:
    case opc_eDataType_QName:
    case opc_eDataType_anyType :
      return false;
      break;
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
  case pwr_eType_Int16:
    *opctype = opc_eDataType_short;
    break;
  case pwr_eType_Int8:
    *opctype = opc_eDataType_byte;
    break;
  case pwr_eType_UInt32:
    *opctype = opc_eDataType_unsignedInt;
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
bool opc_type_to_pwrtype(int type, int *pwrtype) 
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
  case opc_eDataType_QName:
  case opc_eDataType_anyType: 
  case opc_eDataType_decimal:
  case opc_eDataType_long:
  case opc_eDataType_unsignedLong:
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

