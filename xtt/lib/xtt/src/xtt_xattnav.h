/* 
 * Proview   $Id: xtt_xattnav.h,v 1.4 2005-09-01 14:57:48 claes Exp $
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

#ifndef wb_xattnav_h
#define wb_xattnav_h

/* wb_xattnav.h -- Simple navigator */

#if defined __cplusplus
extern "C" {
#endif

#ifndef pwr_h
# include "pwr.h"
#endif

#ifndef flow_h
#include "flow.h"
#endif

#ifndef flow_browapi_h
#include "flow_browapi.h"
#endif

#ifndef xtt_xnav_brow_h
#include "xtt_xnav_brow.h"
#endif

#define xattnav_cVersion	"X3.3b"
#define XATTNAV_BROW_MAX	25

typedef enum {
  xattnav_eType_Object,
  xattnav_eType_CrossRef
} xattnav_eType;

class XAttNav {
  public:
    XAttNav(
	void 		*xa_parent_ctx,
	Widget		xa_parent_wid,
        xattnav_eType   xa_type,
	char 		*xa_name,
	pwr_sAttrRef 	*xa_objar,
	int 		xa_advanced_user,
	Widget 		*w,
	pwr_tStatus 	*status);
    ~XAttNav();

    void 		*parent_ctx;
    Widget		parent_wid;
    xattnav_eType       type;
    char 		name[80];
    Widget		brow_widget;
    Widget		form_widget;
    Widget		toplevel;
    XNavBrow		*brow;
    pwr_sAttrRef       	objar;
    int			advanced_user;
    int			bypass;
    XtIntervalId	trace_timerid;
    int			trace_started;
    void 		(*message_cb)( void *, char, char *);
    void 		(*close_cb)( void *);
    void 		(*change_value_cb)( void *);
    void 		(*popup_menu_cb)( void *, pwr_sAttrRef, unsigned long,
					  unsigned long, char *, Widget *);
    void 		(*start_trace_cb)( void *, pwr_tObjid, char *);
    int		        (*is_authorized_cb)(void *, unsigned int);
    int			displayed;

    void start_trace( pwr_tObjid Objid, char *object_str);
    int set_attr_value( brow_tObject node, char *name, char *value_str);
    int check_attr( int *multiline, brow_tObject *node, char *name,
		char **init_value, int *size);
    int get_select( pwr_sAttrRef *attrref, int *is_attr);
    void message( char sev, char *text);
    void set_inputfocus();
    void force_trace_scan();
    int object_attr();
    int crossref();
    int object_exist( brow_tObject object);
    void redraw();
    void enable_events();
    int select_by_name( char *name);
    void start_trace();
    void swap( int mode);
};


#if defined __cplusplus
}
#endif
#endif
