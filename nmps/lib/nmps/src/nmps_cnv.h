/* 
 * Proview   $Id: nmps_cnv.h,v 1.1 2006-01-12 05:57:43 claes Exp $
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



typedef enum {
  cnv_eConvType_memcpy,
  cnv_eConvType_CharToString,
  cnv_eConvType_Int16ToInt32,
  cnv_eConvType_Int32ToInt16,
  cnv_eConvType_Int32ToFloat32,
  cnv_eConvType_Int32ToString,
  cnv_eConvType_Int32ToAscii,
  cnv_eConvType_Float32ToInt32,
  cnv_eConvType_Float32ToString,
  cnv_eConvType_Float32ToAscii,
  cnv_eConvType_StringToInt32,
  cnv_eConvType_StringToFloat32,
  cnv_eConvType_AsciiToInt32,
  cnv_eConvType_AsciiToFloat32
} cnv_eConvType;

typedef enum {
  cnv_eParType__ 	= 0,
  cnv_eParType_Char,
  cnv_eParType_Boolean,
  cnv_eParType_Int8,
  cnv_eParType_UInt8,
  cnv_eParType_Int16,
  cnv_eParType_UInt16,
  cnv_eParType_Int32,
  cnv_eParType_UInt32,
  cnv_eParType_Float32,
  cnv_eParType_Float64,
  cnv_eParType_String,
  cnv_eParType_Ascii,
  cnv_eParType_Binary,
  cnv_eParType_Unknown,
  cnv_eParType_Time,
  cnv_eParType_
} cnv_eParType;

typedef	struct {
	int		from;
	int		to;
	int		size;
	cnv_eConvType	type;
	char		format[20];
	} cnv_t_conv_item;

#define CNV_CONV_TABLE_SIZE 200

typedef cnv_t_conv_item	cnv_t_conv_table [CNV_CONV_TABLE_SIZE];

int	cnv_ConvertData( 
			cnv_t_conv_table *conv_table,
			int		conv_table_count,
			char		*from_data,
			char		*to_data);

int cvn_ConvInit( 
	pwr_tInt32	ToConvdefType,
	pwr_tObjid	ToConvdef,
	char		*ToConvdefFile,
	pwr_tInt32	FromConvdefType,
	pwr_tObjid	FromConvdef,
	char		*FromConvdefFile,
	pwr_tInt32	*ConvTableSize,
	pwr_tInt32	AllocConvTable,
	char		**ConvTable);

