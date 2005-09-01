/** 
 * Proview   $Id: co_nav_crr.h,v 1.2 2005-09-01 14:57:52 claes Exp $
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

#ifndef co_nav_crr_h
#define co_nav_crr_h

/* co_nav_crr.h -- Display crossreferences */

#if defined __cplusplus
extern "C" {
#endif


typedef enum {
	navc_eItemType_Crossref,
	navc_eItemType_Header,
	navc_eItemType_Text
	} navc_eItemType;


class NavCrr {
  public:
    NavCrr( void *c_parent_ctx, void *c_parent_node) 
      : parent_ctx(c_parent_ctx), parent_node(c_parent_node) 
      {};
    void 	        *parent_ctx;
    void                *parent_node;
    void 		(*insert_cb)( void *, void *, navc_eItemType,
			     char *, char *, int);
    int                 (*name_to_objid_cb)( void *, char *, pwr_tObjid *);
    int                 (*get_volume_cb)( void *, pwr_tVolumeId *);
    int	crr_signal( char *filename, char *signalname);
    int	crr_object( char *filename, char *objectname);
    int	crr_code( char *filename, char *str, int brief, int func,
		  int case_sensitive);
};


#if defined __cplusplus
}
#endif
#endif








