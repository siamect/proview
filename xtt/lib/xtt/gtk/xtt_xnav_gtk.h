/* 
 * Proview   $Id: xtt_xnav_gtk.h,v 1.6 2008-09-18 14:56:57 claes Exp $
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

#ifndef xtt_xnav_gtk_h
#define xtt_xnav_gtk_h

/* xtt_xnav_gtk.h -- Simple navigator */

#ifndef xtt_xnav_h
# include "xtt_xnav.h"
#endif

class XNavGtk : public XNav {
  public:
    XNavGtk(
	void *xn_parent_ctx,
	GtkWidget *xn_parent_wid,
	char *xn_name,
	GtkWidget **w,
	xnav_sStartMenu *root_menu,
	char *xn_opplace_name,
	int xn_op_close_button,
	pwr_tStatus *status);
    ~XNavGtk();

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
		int ev_beep, pwr_tStatus *status);
    Hist *hist_new( char *title, pwr_tOid oid, pwr_tStatus *sts);
    Block *block_new( pwr_tAttrRef *arp, char *name, unsigned int priv,
		      pwr_tStatus *sts);
    Op *op_new( char *opplace, pwr_tStatus *sts);
    XttTrend *xtttrend_new( char *name, pwr_tAttrRef *objar, pwr_tAttrRef *plotgroup,
			    pwr_tStatus *sts);
    XttSevHist *xttsevhist_new( char *name, pwr_tOid *oid, pwr_tOName *aname,
				sevcli_tCtx scctx, pwr_tStatus *sts);
    XttFast *xttfast_new( char *name, pwr_tAttrRef *objar, pwr_tStatus *sts);
    XAttOne *xattone_new( pwr_tAttrRef *objar, char *title, unsigned int priv,
			  pwr_tStatus *sts);
    CLog *clog_new( char *name, pwr_tStatus *sts);
    XttGe *xnav_ge_new( char *name, char *filename, int scrollbar, int menu, 
			int navigator, int width, int height, int x, int y, 
			double scan_time, char *object_name, 
			int use_default_access, unsigned int access,
			int (*xg_command_cb) (XttGe *, char *),
			int (*xg_get_current_objects_cb) (void *, pwr_sAttrRef **, int **),
			int (*xg_is_authorized_cb) (void *, unsigned int));
    GeCurve *gecurve_new( char *name, char *filename, GeCurveData *data,
			  int pos_right);
    XttFileview *fileview_new( pwr_tOid oid, char *title, char *dir, char *pattern,
			       int type, char *target_attr, char *trigger_attr, char *filetype);
    CoLogin *login_new( char		*wl_name,
			char		*wl_groupname,
			void		(* wl_bc_success)( void *),
			void		(* wl_bc_cancel)( void *),
			pwr_tStatus  	*status);
    void bell( int time);
    void get_popup_menu( pwr_sAttrRef attrref,
			 xmenu_eItemType item_type, 
			 xmenu_mUtility caller, 
			 unsigned int priv, char *arg, int x, int y);
    static void menu_position_func( GtkMenu *menu, gint *x, gint *y, gboolean *push_in,
				    gpointer data);
    static GtkWidget *build_menu( GtkWidget *Parent,
				  int   MenuType,
				  char *MenuTitle,
				  void *MenuUserData,
				  void (*Callback)( GtkWidget *, gpointer),
				  void *CallbackData,
				  xmenu_sMenuItem *Items,
				  int *idx);
    static void popup_button_cb( GtkWidget *w, gpointer data);

    GtkWidget		*parent_wid;
    GtkWidget		*brow_widget;
    GtkWidget		*form_widget;
    GtkWidget		*toplevel;
    int			popupmenu_x;
    int			popupmenu_y;
};

#endif


