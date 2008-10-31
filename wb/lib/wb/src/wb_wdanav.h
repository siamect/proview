/* 
 * Proview   $Id: wb_wdanav.h,v 1.6 2008-10-31 12:51:32 claes Exp $
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

#ifndef wb_wattnav_h
#define wb_wattnav_h

#if defined __cplusplus
extern "C" {
#endif

#ifndef pwr_h
# include "pwr.h"
#endif

#ifndef wb_ldh_h
# include "wb_ldh.h"
#endif

#ifndef flow_h
#include "flow.h"
#endif

#ifndef flow_browapi_h
#include "flow_browapi.h"
#endif

#ifndef wb_wnav_brow_h
#include "wb_wnav_brow.h"
#endif

#define wdanav_cVersion	"X3.3b"
#define WDANAV_BROW_MAX	25

class WdaNav {
  public:
    WdaNav(
	void 		*wa_parent_ctx,
	const char     	*wa_name,
	ldh_tSesContext wa_ldhses,
	pwr_tObjid 	wa_objid,
	pwr_tClassId 	wa_classid,
        char            *wa_attribute,
	int 		wa_editmode,
	int 		wa_advanced_user,
	int		wa_display_objectname,
	wb_eUtility	wa_utility,
	pwr_tStatus 	*status);
    virtual ~WdaNav();

    void 		*parent_ctx;
    char 		name[80];
    WNavBrow		*brow;
    ldh_tSesContext	ldhses;
    pwr_tObjid		objid;
    pwr_tClassId	classid;
    char                attribute[80];
    int			editmode;
    int			advanced_user;
    int			display_objectname;
    int			bypass;
    int			trace_started;
    void 		(*message_cb)( void *, char, const char *);
    void 		(*change_value_cb)( void *);
    wb_eUtility		utility;
    int			displayed;
    int			attrobjects;

    virtual void set_inputfocus() {}

    void start_trace( pwr_tObjid Objid, char *object_str);
    int set_attr_value( brow_tObject node, char *name, char *value_str);
    int check_attr( int *multiline, brow_tObject *node, char *name,
		char **init_value, int *size);
    int get_select( pwr_sAttrRef *attrref, int *is_attr);
    void message( char sev, const char *text);
    void force_trace_scan();
    int get_attr();
    int object_exist( brow_tObject object);
    void redraw();
    void enable_events();
    void set_editmode( int editmode, ldh_tSesContext ldhses)
	{ this->editmode = editmode; this->ldhses = ldhses;};
    int select_by_name( char *name);
    int update( pwr_tObjid new_objid, pwr_tClassId new_classid,
		char *new_attribute, int new_attrobjects);
    int find_by_objid( pwr_tObjid oi, brow_tObject *object);
    int print( char *filename);

    static int brow_cb( FlowCtx *ctx, flow_tEvent event);
    static int init_brow_cb( FlowCtx *fctx, void *client_data);

};


#if defined __cplusplus
}
#endif
#endif


