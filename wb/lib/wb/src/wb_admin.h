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

typedef enum {
	admin_eDiaUserFunction_New,
	admin_eDiaUserFunction_Prop
	} admin_eDiaUserFunction;

typedef enum {
	admin_eDiaSysFunction_New,
	admin_eDiaSysFunction_Prop
	} admin_eDiaSysFunction;

typedef enum {
	admin_eDiaVolFunction_New,
	admin_eDiaVolFunction_Prop
	} admin_eDiaVolFunction;

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
    Widget	proj_entry;
    Widget	volume_entry;
    Widget	user_entry;
    Widget	menu_project;
    Widget      menu_edit;

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

    Widget	adminuser_dia;
    Widget	adminuser_username;
    Widget	adminuser_password;
    Widget	adminuser_system;
    Widget	adminuser_username_noe;
    Widget	adminuser_system_noe;
    Widget	adminuser_priv_rtread;
    Widget	adminuser_priv_rtwrite;
    Widget	adminuser_priv_rtevents;
    Widget	adminuser_priv_system;
    Widget	adminuser_priv_maintenance;
    Widget	adminuser_priv_process;
    Widget	adminuser_priv_instrument;
    Widget	adminuser_priv_operator1;
    Widget	adminuser_priv_operator2;
    Widget	adminuser_priv_operator3;
    Widget	adminuser_priv_operator4;
    Widget	adminuser_priv_operator5;
    Widget	adminuser_priv_operator6;
    Widget	adminuser_priv_operator7;
    Widget	adminuser_priv_operator8;
    Widget	adminuser_priv_operator9;
    Widget	adminuser_priv_operator10;
    Widget	adminuser_priv_devread;
    Widget	adminuser_priv_devplc;
    Widget	adminuser_priv_devconfig;
    Widget	adminuser_priv_devclass;

    Widget	adminsys_dia;
    Widget	adminsys_systemname;
    Widget	adminsys_systemname_noe;
    Widget	adminsys_attr_userinherit;

    Widget	adminvol_dia;
    Widget	adminvol_volumename;
    Widget	adminvol_volumename_noe;
    Widget	adminvol_volumeid;
    Widget	adminvol_volumeid_noe;
    Widget	adminvol_projname;
    Widget	adminvol_projname_noe;

    int		input_open;
    void 	*object;
    void	(*close_cb) ( void *);
    brow_tObject input_node;
    char	input_name[80];
    int		set_focus_disabled;
    XtIntervalId focus_timerid;
    int		dia_function;
    int		diauser_function;
    int		diasys_function;
    int		diavol_function;
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
    Item	*adminvol_item;
    int		loggedin;

    void message( char severity, char *message);
    void set_prompt( char *prompt);
    void set_title( char *title);
    int open_input();
    int open_project();
    int create_project();
    int delete_project();
    int copy_project();
    int project_properties(int projname_edit, int base_edit,
	int path_edit, int hier_edit, int descr_edit);
    int user_properties( admin_eDiaUserFunction function, int username_edit, int password_edit,
	int system_edit, int priv_edit);
    int system_properties( admin_eDiaSysFunction function, int systemname_edit, int attr_edit);
    int volume_properties( admin_eDiaVolFunction function, int volumename_edit, 
			   int volumeid_edit, int projectname_edit);
    void adminprop_setup( char *title, int projname_edit, int base_edit, 
	int path_edit, int hier_edit, int descript_edit,
	char *projname_value, char *base_value, char *path_value,
	char *hier_value, char *descript_value);
    void adminuser_setup( char *title, int username_edit, int password_edit, 
			  int system_edit, int priv_edit,
			  char *username_value, char *system_value, unsigned int priv_value);
    void adminsys_setup( char *title, int systemname_edit, int attr_edit,
			  char *systemname_value, unsigned int attr_value);
    void adminvol_setup( char *title, int volumename_edit, int volumeid_edit,
			 int projectname_edit, char *volumename_value, char *volumeidvalue,
			 char *projectnamevalue);
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
