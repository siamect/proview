/** 
 * Proview   $Id: co_api.h,v 1.9 2008-10-31 12:51:30 claes Exp $
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

#ifndef cow_api_h
#define cow_api_h

#if defined __cplusplus
extern "C" {
#endif

#if !defined cow_msgwindow_h
typedef enum {
  msgw_ePop_No,
  msgw_ePop_Yes,
  msgw_ePop_Default
} msgw_ePop;
#endif

void msgw_message( int severity, const char *text, msgw_ePop pop);
void msgw_message_sts( pwr_tStatus sts, const char *text1, const char *text2);
void msgw_message_object( pwr_tStatus sts, const char *text1, const char *text2, pwr_tOid oid);
void msgw_message_plcobject( pwr_tStatus sts, const char *text1, const char *text2, pwr_tOid oid);
void msgw_set_nodraw();
void msgw_reset_nodraw();
int xhelp_help( char *key, char *help_bookmark, navh_eHelpFile file_type,
		char *file_name, int strict);
#if defined __cplusplus
}
#endif
#endif








