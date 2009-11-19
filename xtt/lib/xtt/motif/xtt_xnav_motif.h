/* 
 * Proview   $Id: xtt_xnav_motif.h,v 1.4 2008-10-31 12:51:36 claes Exp $
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

#ifndef xtt_xnav_motif_h
#define xtt_xnav_motif_h

/* xtt_xnav_motif.h -- Simple navigator */

#ifndef xtt_xnav_h
# include "xtt_xnav.h"
#endif

class XNavMotif : public XNav {
  public:
    XNavMotif(
	void *xn_parent_ctx,
	Widget	xn_parent_wid,
	const char *xn_name,
	Widget *w,
	xnav_sStartMenu *root_menu,
	char *xn_opplace_name,
	int op_close_button,
	pwr_tStatus *status);
    ~XNavMotif();

    void set_inputfocus();
    void pop();
    void create_popup_menu( pwr_sAttrRef attrref,
			    xmenu_eItemType item_type,
			    xmenu_mUtility caller, unsigned int priv,
			    char *arg, int x, int y);
    RtTrace *plctrace_new( pwr_tOid oid, pwr_tStatus *sts);
    XAtt *xatt_new( pwr_tAttrRef *arp, int advanced_user, pwr_tStatus *sts);
    XCrr *xcrr_new( pwr_tAttrRef *arp, int advanced_user, pwr_tStatus *sts);
    Ev *ev_new( char *eve_name, char *ala_name, char *blk_name,
		pwr_tObjid ev_user, int display_ala, int display_eve,
		int display_blk, int display_return, int display_ack,
		int ev_beep, pwr_tMask ev_pop_mask, pwr_tStatus *status);
    Hist *hist_new( char *title, pwr_tOid oid, pwr_tStatus *sts);
    Block *block_new( pwr_tAttrRef *arp, char *name, unsigned int priv,
		      pwr_tStatus *sts);
    Op *op_new( char *opplace, pwr_tStatus *sts);
    XttTrend *xtttrend_new( char *name, pwr_tAttrRef *objar, pwr_tAttrRef *plotgroup,
			    pwr_tStatus *sts);
    XttFast *xttfast_new( char *name, pwr_tAttrRef *objar, pwr_tStatus *sts);
    XAttOne *xattone_new( pwr_tAttrRef *objar, char *title, unsigned int priv,
			  pwr_tStatus *sts);
    CLog *clog_new( const char *name, pwr_tStatus *sts);
    XttGe *xnav_ge_new( const char *name, const char *filename, int scrollbar, int menu, 
			int navigator, int width, int height, int x, int y, 
			double scan_time, const char *object_name, 
			int use_default_access, unsigned int access,
			int (*xg_command_cb) (XttGe *, char *),
			int (*xg_get_current_objects_cb) (void *, pwr_sAttrRef **, int **),
			int (*xg_is_authorized_cb) (void *, unsigned int));
    GeCurve *gecurve_new( char *name, char *filename, GeCurveData *data,
			  int pos_right);
    CoLogin *login_new( const char     	*wl_name,
			const char     	*wl_groupname,
			void		(* wl_bc_success)( void *),
			void		(* wl_bc_cancel)( void *),
			pwr_tStatus  	*status);
    void bell( int time);
    void get_popup_menu( pwr_sAttrRef attrref,
			 xmenu_eItemType item_type, 
			 xmenu_mUtility caller, 
			 unsigned int priv, char *arg, int x, int y);

    static Widget build_menu( Widget Parent,
			      int   MenuType,
			      const char *MenuTitle,
			      void *MenuUserData,
			      void (*Callback)( Widget, XNav *, XmAnyCallbackStruct *),
			      void *CallbackData,
			      xmenu_sMenuItem *Items,
			      int *idx);
    static void popup_button_cb( Widget w, XNav *xnav, XmAnyCallbackStruct *data);
    static void popup_unmap_cb(Widget w, xmenu_sMenuCall *ip, XmAnyCallbackStruct *data);


    Widget		parent_wid;
    Widget		brow_widget;
    Widget		form_widget;
    Widget		toplevel;

};

#endif


