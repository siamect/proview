/** 
 * Proview   $Id: wb_wnav_selformat.h,v 1.2 2005-09-01 14:57:59 claes Exp $
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

#ifndef wb_wnav_selformat_h
#define wb_wnav_selformat_h

#if defined __cplusplus
extern "C" {
#endif

#ifndef pwr_h
# include "pwr.h"
#endif

#ifndef wb_ldh_h
# include "wb_ldh.h"
#endif

// Defines for primary selection mode
typedef enum {
  	wnav_eSelectionMode_GMS,
  	wnav_eSelectionMode_Normal,
  	wnav_eSelectionMode_Extern
	} wnav_eSelectionMode;


pwr_tBoolean wnav_format_selection( ldh_tSesContext ldhses, pwr_sAttrRef attrref, 
				   pwr_tBoolean is_class, pwr_tBoolean is_attr,
				   int select_syntax, int select_volume, 
				   int select_attr, int select_type, char *buff);



#if defined __cplusplus
}
#endif
#endif




