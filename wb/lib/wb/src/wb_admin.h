#ifndef wb_admin_h
#define wb_admin_h

/* wb_admin.h -- Object attribute editor

   PROVIEW/R
   Copyright (C) 1996 by Comator Process AB.

   <Description>.  */

#if defined __cplusplus
extern "C" {
#endif

#ifndef pwr_h
# include "pwr.h"
#endif

#ifndef wb_adminnav_h
# include "wb_adminnav.h"
#endif

typedef enum {
	admin_eDiaFunction_Create,
	admin_eDiaFunction_Copy,
	admin_eDiaFunction_Prop
	} admin_eDiaFunction;

class Admin {
  public:
    Admin( 
	Widget 		wa_parent_wid, 
	void 		*wa_parent_ctx);
    ~Admin();
    Widget	parent_wid;
    void 	*parent_ctx;
    char 	name[80];
    Widget	brow_widget;
    Widget	form_widget;
    Widget	toplevel;
    void	*adminnav;
    Widget	msg_label;
    Widget	cmd_prompt;
    Widget	cmd_input;
    Widget	adminnav_form;
    Widget	adminprop_dia;
    Widget	adminprop_projname;
    Widget	adminprop_base;
    Widget	adminprop_path;
    Widget	adminprop_hier;
    Widget	adminprop_descript;
    Widget	adminprop_projname_noe;
    Widget	adminprop_base_noe;
    Widget	adminprop_path_noe;
    Widget	adminprop_hier_noe;
    Widget	adminprop_descript_noe;
    int		input_open;
    void 	*object;
    void	(*close_cb) ( void *);
    brow_tObject input_node;
    char	input_name[80];
    int		set_focus_disabled;
    XtIntervalId focus_timerid;
    int		dia_function;
    char	copy_from_proj[80];
    char	modify_projname[120];
    char	modify_base[120];
    char	modify_root[120];
    char	modify_hier[120];
    char	modify_descript[120];
    int		projname_editable;
    int		base_editable;
    int		root_editable;
    int		hier_editable;
    int		descript_editable;
    char	cmd_recall[30][160];
    int		cmd_current_recall;
    int		ccm_func_registred;
    int		verify;
    int		scriptmode;
    int		edit_mode;
    int		clock_cursor;

    void message( char severity, char *message);
    void set_prompt( char *prompt);
    int open_input();
    int open_project();
    int create_project();
    int delete_project();
    int copy_project();
    int project_properties(int projname_edit, int base_edit,
	int path_edit, int hier_edit, int descr_edit);
    void adminprop_setup( char *title, int projname_edit, int base_edit, 
	int path_edit, int hier_edit, int descript_edit,
	char *projname_value, char *base_value, char *path_value,
	char *hier_value, char *descript_value);
    void set_clock_cursor();
    void reset_cursor();
    void set_clock_cursor_dia();
    void reset_cursor_dia();
    void free_cursor();

    // Command module
    int readcmdfile( 	char		*incommand);
    int command( char* input_str);
};

#if defined __cplusplus
}
#endif
#endif
