/* 
 * Proview   $Id: rt_rtt_functions.h,v 1.4 2008-06-25 07:50:14 claes Exp $
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

#ifndef rt_rtt_functions_h
#define rt_rtt_functions_h

#include "co_ccm.h"

/* Module rt_rtt_menu.c */

int	rtt_gdh_init( void);
int	rtt_qcom_init( void);
int	rtt_initialize( char	*username,
			char	*password,
			char	*commandfile,
			char	*mainmenu_title);


int	rtt_parse( 	char	*string,
			char	*parse_char,
			char	*inc_parse_char,
			char	*outstr,
			int	max_rows,
			int 	max_cols,
			int	keep_quota);

int	rtt_cli( 	rtt_t_comtbl	*command_table,
			char		*string,
			void		*userdata1,
			void		*userdata2);

int	rtt_get_qualifier( 	char	*qualifier,
				char	*value);

int	rtt_get_input( 	char		*chn,
			char		*input_str,
			unsigned long	*terminator,
			int		maxlen,
			unsigned long	option,
			int		timeout);

void	rtt_cursor_rel( 	int	x,
				int	y);
void	rtt_cursor_abs( 	int	x,
				int	y);
void	rtt_cursor_abs_force( 	int	x,
				int	y);
void	rtt_char_delete( 	int	n);
void	rtt_char_insert( 	int 	n);
void	rtt_char_insert_nob( 	int	n);
void	rtt_eofline_erase();
void	rtt_display_erase();
void	rtt_print_screen();
void	rtt_char_inverse_start();
void	rtt_char_inverse_end();
void	rtt_store_cursorpos();
void	rtt_restore_cursorpos();
void	rtt_charset_ascii();
void	rtt_charset_linedrawing();
void	rtt_charset_mosaic();

int	rtt_get_input_string( 	char		*chn,
				char		*out_string,
				unsigned long	*out_terminator,
				int		out_maxlen,
				rtt_t_recall 	*recall,
				unsigned long	option,
				int		timeout,
				int		(* timeout_func) (),
				void		*timeout_arg,
				char		*prompt);

int	rtt_menu_create_ctx( 
			menu_ctx	*ctx,
			menu_ctx	parent_ctx,
			rtt_t_menu	*menu,
			char		*title,
			int		menutype);

int	rtt_menu_configure( menu_ctx	ctx);

int	rtt_menu_upd_configure( menu_ctx	ctx);

int	rtt_menu_draw_item( 
			menu_ctx	ctx,
			int		item);

int	rtt_menu_draw_title( menu_ctx	ctx);

int	rtt_menu_select( menu_ctx	ctx);
int	rtt_menu_unselect( menu_ctx	ctx);
int	rtt_edit_draw( 		menu_ctx	ctx,
					rtt_t_backgr	*picture);
int	rtt_edit_select( menu_ctx	ctx);
int	rtt_edit_unselect( menu_ctx	ctx);

int	rtt_get_next_item_down( menu_ctx	ctx);
int	rtt_get_next_item_up( menu_ctx	ctx);
int	rtt_get_next_item_left( menu_ctx	ctx);
int	rtt_get_next_item_right( menu_ctx	ctx);
int	rtt_get_previous_page( menu_ctx	ctx);
int	rtt_get_next_page( menu_ctx	ctx);
int	rtt_item_to_page( 
			menu_ctx	ctx,
			int		item,
			int		*page);
int	rtt_menu_delete( menu_ctx	ctx);
int	rtt_menu_keys_new(
			menu_ctx	parent_ctx,
			pwr_tObjid	argoi,
			rtt_t_menu	**menu_p,
			char		*title,
			void		*userdata,
			unsigned long	flag);
int	rtt_menu_new(
			menu_ctx	parent_ctx,
			pwr_tObjid	argoi,
			rtt_t_menu	**menu_p,
			char		*title,
			void		*userdata,
			unsigned long	flag);
int	rtt_menu_upd_new(
			menu_ctx	parent_ctx,
			pwr_tObjid	argoi,
			rtt_t_menu_upd	**menu_p,
			char		*title,
			void		*userdata,
			unsigned long	flag);
int	rtt_menu_edit_new(
			menu_ctx	parent_ctx,
			pwr_tObjid	argoi,
			rtt_t_menu_upd	**menu_p,
			char		*title,
			rtt_t_backgr	*picture,
			int		(* appl_func) ());
int	rtt_menu_sysedit_new(
			menu_ctx	parent_ctx,
			pwr_tObjid	argoi,
			rtt_t_menu_upd	**menu_p,
			char		*title,
			char		*objectname,
			int		(* appl_func) ());
int	rtt_menu_list_add_malloc(
			rtt_t_menu	**menulist,
			int		index);
int	rtt_menu_list_add(
			rtt_t_menu	**menulist,
			int		index,
			int		allocated,
			char		*text,
			int		(* func) (),
			int		(* func2) (),
			int		(* func3) (),
			pwr_tObjid	argoi,
			void		*arg1,
			void		*arg2,
			void		*arg3,
			void		*arg4);
int	rtt_menu_list_insert(
			rtt_t_menu	**menulist,
			int		index,
			char		*text,
			int		(* func) (),
			int		(* func2) (),
			int		(* func3) (),
			pwr_tObjid	argoi,
			void		*arg1,
			void		*arg2,
			void		*arg3,
			void		*arg4);
int	rtt_menu_upd_list_add_malloc(
			rtt_t_menu_upd	**menulist,
			int		index);
int	rtt_menu_upd_list_add( 
			rtt_t_menu_upd	**menulist,
			int		index,
			int		allocated,
			char		*text,
			int		(* func) (),
			int		(* func2) (),
			int		(* func3) (),
			pwr_tObjid	argoi,
			void		*arg1,
			void		*arg2,
			void		*arg3,
			void		*arg4,
			char		*parameter_name,
			unsigned long	priv,
			char		*value_ptr,
			unsigned long	value_type,
			unsigned long	flags,
			unsigned long	size,
			gdh_tSubid	subid,
			int		x,
			int		y,
			char		characters,
			char		decimals,
			float		maxlimit,
			float		minlimit,
			int		database,
			char		*output_text);

int	rtt_menu_upd_update(
			menu_ctx	ctx);
int	rtt_menu_edit_update(
			menu_ctx	ctx);
int	rtt_show_object_as_struct( 
			menu_ctx	parent_ctx, 
			pwr_tObjid	objid, 
			char		*type_str, 
			char		*file_str);
int	rtt_object_parameters(
			menu_ctx	parent_ctx,
			pwr_tObjid	objid,
			void		*arg1,
			void		*arg2,
			void		*arg3,
			void		*arg4);
int	rtt_hierarchy_child(
			menu_ctx	parent_ctx,
			pwr_tObjid	parent_objid,
			void		*arg1,
			void		*arg2,
			void		*arg3,
			void		*arg4);
int	rtt_show_file(
			menu_ctx	parent_ctx,
			char		*filename,
			char		*command,
			char		*title);
int	rtt_hierarchy(
			menu_ctx	parent_ctx,
			pwr_tObjid	argoi,
			void		*arg1,
			void		*arg2,
			void		*arg3,
			void		*arg4);
int	rtt_class_hierarchy(
			menu_ctx	parent_ctx,
			pwr_tObjid	argoi,
			void		*arg1,
			void		*arg2,
			void		*arg3,
			void		*arg4);
int	rtt_debug_child(
			menu_ctx	parent_ctx,
			pwr_tObjid	parent_objid,
			void		*arg1,
			void		*arg2,
			void		*arg3,
			void		*arg4);
int 	rtt_exit( int ctx, pwr_tObjid argoi, void *arg1, void *arg2, 
			void *arg3, void *arg4);
int	rtt_message(	char		severity,
			char	*message);
int	rtt_message_sts( int 		sts);
int	rtt_help(	
			menu_ctx	parent_ctx,
			char		*subject,
			rtt_t_helptext	*helptext);
int	rtt_get_menusize(
			menu_ctx	ctx,
			int		*size);
int	rtt_menu_item_undelete(
			rtt_t_menu	**menulist,
			int		index);
int	rtt_menu_item_delete(
			menu_ctx	ctx,
			int		item);
void rtt_error_msg( unsigned long 	sts);
int	rtt_objidtoclassname(
			pwr_tObjid	objid,
			char		*name);
int	rtt_menu_new_update(
			menu_ctx	parent_ctx,
			pwr_tObjid	argoi,
			rtt_t_menu_update **menu_p,
			char		*title,
			void		*userdata,
			unsigned long	flag);
int	rtt_menu_new_upedit(
			menu_ctx	parent_ctx,
			pwr_tObjid	argoi,
			rtt_t_menu_update **menu_p,
			char		*title,
			rtt_t_backgr	*picture,
			int		(* function) ());
int	rtt_menu_new_upeditperm(
			menu_ctx	parent_ctx,
			pwr_tObjid	argoi,
			rtt_t_menu_update **menu_p,
			char		*title,
			rtt_t_backgr	*picture,
			int		(* function) ());
int	rtt_menu_new_sysedit(
			menu_ctx	parent_ctx,
			pwr_tObjid	argoi,
			char		*objectname,
			char		*title,
			void		*dummy,
			int		(* function) ());
int	rtt_menu_parameter_set(
			menu_ctx	ctx,
			pwr_tObjid	argoi,
			char		*parameter_name,
			void		*arg2,
			unsigned long	priv,
			void		*arg4);
int	rtt_menu_parameter_reset(
			menu_ctx	ctx,
			pwr_tObjid	argoi,
			char		*parameter_name,
			void		*arg2,
			unsigned long	priv,
			void		*arg4);
int	rtt_menu_parameter_toggle(
			menu_ctx	ctx,
			pwr_tObjid	argoi,
			char		*parameter_name,
			void		*arg2,
			unsigned long	priv,
			void		*arg4);
int	rtt_menu_parameter_dual_set(
			menu_ctx	parent_ctx,
			pwr_tObjid	argoi,
			void		*arg1,
			char		*parameter_name,
			unsigned long	priv,
			void		*arg4);
int	rtt_menu_parameter_dual_reset(
			menu_ctx	parent_ctx,
			pwr_tObjid	argoi,
			void		*arg1,
			char		*parameter_name,
			unsigned long	priv,
			void		*arg4);
int	rtt_menu_parameter_dual_toggle(
			menu_ctx	parent_ctx,
			pwr_tObjid	argoi,
			void		*arg1,
			char		*parameter_name,
			unsigned long	priv,
			void		*arg4);
int	rtt_menu_parameter_command(
				menu_ctx	parent_ctx,
				pwr_tObjid	argoi,
				void		*arg1,
				char		*command,
				unsigned long	priv,
				void		*arg4);
int	rtt_edit_draw_background(
			rtt_t_backgr	*chartable);
int	r_print(	char		*format, ...);
int	r_print_buffer();

int	rtt_wait_for_return();
int	rtt_clear_screen();
int	rtt_scan(	menu_ctx	ctx);
int	rtt_sleep(
			menu_ctx	ctx,
			int	time);
int	rtt_get_defaultfilename(
			char	*inname,
			char	*outname,
			char	*ext);
void	rtt_exit_now( int disconnected, pwr_tStatus exit_sts);
int	rtt_printf(	char	*format, ...);
int	rtt_get_fastkey_picture( menu_ctx ctx);
int	rtt_get_fastkey_type( );
unsigned int	rtt_exception(	void	*signalP, void	*mechanismP);
int	rtt_update_time ();
int rtt_cut_segments (
  char	*outname,
  char	*name,
  int	segments);
int	rtt_menu_get_parent_text( 	menu_ctx parent_ctx, 
					char *text);
int	rtt_setup( menu_ctx	parent_ctx);
int	rtt_init_state_table();
int	rtt_recall_create();
char	*rtt_pwr_dir( char *dir);
int rtt_ctx_push( menu_ctx ctx);
int rtt_ctx_pop();
menu_ctx rtt_current_ctx();
int	rtt_logon_pict( unsigned long *chn,
			unsigned long *priv);

/* Module rt_rtt_crr.c */

int	rtt_crossref_signal(
			unsigned long	ctx,
			pwr_tObjid	objid,
			void		*arg1,
			void		*arg2,
			void		*arg3,
			void		*arg4);
int	rtt_crossref_channel(
			unsigned long	ctx,
			pwr_tObjid	objid,
			void		*arg1,
			void		*arg2,
			void		*arg3,
			void		*arg4);
int	rtt_crr_signal(
			char	*filename,
			char	*signalname);
int	rtt_crr_object(
			char	*filename,
			char	*objectname);
int	rtt_crr_code(
			char	*filename,
			char	*str,
			int	brief,
			int	func,
			int	case_sensitive);
int	rtt_show_signals(
			menu_ctx	parent_ctx,
			char		*filename,
			char		*windowname,
			int		debug);

/* Module rt_rtt_command.c */

int	rtt_get_do_test(	pwr_tObjid	objid,
				int		*on);
int	rtt_set_do_test(	pwr_tObjid	objid,
			int		on,
			int		show_only);
int	rtt_get_do_testvalue(	pwr_tObjid	objid,
				int		*on);
int	rtt_set_do_testvalue(	pwr_tObjid	objid,
			int		on,
			int		show_only);
int rtt_wildcard(	char	*wildname,
			char	*name);

int rtt_toupper( 	char	*str_upper,
			char	*str);
int	rtt_get_command(
			menu_ctx	ctx,
			char		*chn,
			rtt_t_recall 	*recall,
			int		timeout,
			int		(* timeout_func) (),
			void		*timeout_arg,
			char		*prompt,
			int		x,
			int		y,
			rtt_t_comtbl	*command_table);
int	rtt_menu_execute_file(
			menu_ctx	ctx,
			pwr_tObjid	argoi,
			char		*filename,
			void		*arg2,
			void		*arg3,
			void		*arg4);
int	rtt_menu_exec_filecommand(
				menu_ctx	parent_ctx,
				pwr_tObjid	argoi,
				char		*command,
				void		*arg2,
				void		*arg3,
				void		*arg4);
int	rtt_menu_filecommand(
				menu_ctx	parent_ctx,
				pwr_tObjid	argoi,
				char		*filespec,
				char		*command,
				void		*arg3,
				void		*arg4);
int	rtt_menu_command(
			menu_ctx	ctx,
			pwr_tObjid	argoi,
			char		*command,
			void		*arg2,
			void		*arg3,
			void		*arg4);
int	rtt_menu_commandhold(
			menu_ctx	ctx,
			pwr_tObjid	argoi,
			char		*command,
			void		*arg2,
			void		*arg3,
			void		*arg4);
int	rtt_menu_vmscommand(
			menu_ctx	ctx,
			pwr_tObjid	argoi,
			char		*command,
			void		*arg2,
			void		*arg3,
			void		*arg4);
int	rtt_menu_vmscommand_nowait(
			menu_ctx	ctx,
			pwr_tObjid	argoi,
			char		*command,
			void		*arg2,
			void		*arg3,
			void		*arg4);
int	rtt_menu_vmscommandconf(
			menu_ctx	ctx,
			pwr_tObjid	argoi,
			char		*command,
			void		*arg2,
			void		*arg3,
			void		*arg4);
int	rtt_menu_vmscommandhold(
			menu_ctx	ctx,
			pwr_tObjid	argoi,
			char		*command,
			void		*arg2,
			void		*arg3,
			void		*arg4);
int	rtt_debug_child_add(
			pwr_tObjid	objid,
			rtt_t_menu_upd	**menulist,
			int		*index,
			int		*allocated,
			void		*dum3,
			void		*dum4);
int	rtt_debug_object_add(
			pwr_tObjid	objid,
			rtt_t_menu_upd	**menulist,
			int		*index,
			int		*crossref,
			void		*dum3,
			void		*dum4);
int	rtt_debug_child_check(
			pwr_tObjid	objid);
int	rtt_show_obj_hier_class_name(
			menu_ctx	parent_ctx,
			char		*hiername,
			char		*classname,
			char		*name,
			int		global,
			int		max_objects);
int	rtt_collect_insert(
			menu_ctx	ctx,
			char		*full_name);
int	rtt_get_objects_hier_class_name(
			menu_ctx	ctx,
			pwr_tObjid	hierobjid,
			pwr_tClassId	class,
			char		*name,
			int		max_count,
			int		global,
			int		(*backcall)(),
			void		*arg1,
			void		*arg2,
			void		*arg3,
			void		*arg4,
			void		*arg5);
int	rtt_menu_classort(
			rtt_t_menu	*menulist,
			int		redo);
int	rtt_menu_bubblesort(
			rtt_t_menu	*menulist);
int	rtt_menu_upd_bubblesort(
			rtt_t_menu_upd	*menulist);
int	rtt_collect_show(
			menu_ctx	ctx);
int	rtt_commandmode_start(
			char			*filename,
			int			quit);
int	rtt_commandmode_single(
			char			*command);
int rtt_read_line(
			char	*line,
			int	maxsize,
			FILE	*file);
int	rtt_command_get_input_string(
			char		*chn,
			char		*out_string,
			unsigned long	*out_terminator,
			int		out_maxlen,
			rtt_t_recall	*recall,
			unsigned long	option,
			int		timeout,
			int		(* timeout_func) (),
			void		*timeout_arg,
			char		*prompt,
			int		function);
int	rtt_edit_debug_signals(
			menu_ctx	ctx,
			pwr_tObjid	objid,
			void		*dum1,
			void		*dum2,
			void		*dum3,
			void		*dum4);

int	rttcmd_print_func(	menu_ctx	ctx,
				int		*flag);
int	rttcmd_say_func(	menu_ctx	ctx,
				int		*flag);
int	rttcmd_define_func(	menu_ctx	ctx,
				int		*flag);
int	rtt_show_menu( 	menu_ctx	ctx, 
			char 		*menu_name);
int	rtt_learn_store( char	*command);
int	rtt_learn_start( char		*filename);
int	rtt_learn_stop();
int	rtt_commandmode_getnext(
			char		*command,
			unsigned long	*terminator);
int	rtt_commandmode_rewind();
int	rttcmd_learn_func(	menu_ctx	ctx,
				int		*flag);
int	rtt_remove_blank( char *out_str, char *in_str);

/* Module rt_rtt_alarm.c */

int	rtt_alarm_send(
			char	*alarm_text,
			int	alarm_prio);
int	rtt_alarm_disconnect();
int	rtt_alarm_connect (
			pwr_tObjid	UserObject,
			int		maxalarm,
			int		maxevent,
			int		acknowledge,
			int		returned,
			int		beep);
int	rtt_alarm_update ( 
			menu_ctx	ctx);
int	rtt_menu_alarm_new(
			menu_ctx		parent_ctx,
			menu_ctx		ctx);
int	rtt_event_print(
			char		*filename,
			int		notext,
			int		noname);
int	rtt_alarm_ack(
			menu_ctx 	ctx);
int	rtt_alarm_ack_last();
int	rtt_alarmlog_start( char *filename);
int	rtt_alarmlog_stop();

/* Module rt_rtt_logging.c */

int	rtt_logging_create(
			menu_ctx	ctx,
			int		entry,
			int		logg_time,
			char		*filename,
			char		*parameterstr,
			char		*conditionstr,
			int		logg_type,
			int		insert,
			int		buffer_size,
			int		stop,
			int		priority,
			int		line_size,
			int		shortname);
int	rtt_logging_set(
			menu_ctx	ctx,
			int		entry,
			int		logg_time,
			char		*filename,
			char		*parameterstr,
			char		*conditionstr,
			int		logg_type,
			int		insert,
			int		buffer_size,
			int		stop,
			int		priority,
			int		create,
			int		line_size,
			int		shortname);
int	rtt_logging_show(
			menu_ctx	ctx,
			int		entry);
int	rtt_logging_store_entry(
			int		entry,
			char		*filename);
int	rtt_logging_store_all(
			char		*filename);
int	rtt_logging_start(
			menu_ctx	ctx,
			int		entry);
int	rtt_logging_stop(
			menu_ctx	ctx,
			int		entry);
int	rtt_logging_delete(
			menu_ctx	ctx,
			int		entry,
			char		*parameterstr);
int	rtt_logging_close_files();

/* Module rt_rtt_view.c */

int	rtt_view(	menu_ctx	parent_ctx,
			char		*filename, 
			char		*inbuff,
			char		*intitle,
			int		type);
int	rtt_view_search( view_ctx ctx,
			 char	*search_string);
int	rtt_view_buffer(
			menu_ctx	parent_ctx,
			pwr_tObjid	objid,
			char		*filename, 
			char		*inbuff,
			char		*intitle,
			int		type);

/* Module rt_rtt_os.c */

int	rtt_get_nodename( char *nodename, int size);
char	*rtt_fgetname( FILE *fp, char *name, char *def_name);
int	rtt_set_prio( int prio);
int	rtt_set_default_prio();
#if defined OS_VMS
int	rttvms_get_nodename( char *nodename, int size);
int	rttvms_get_uic( int *uic);
int	rttvms_get_identname( int id, char *name);
int	rttvms_get_procinfo( int prcnum, char *state, int *pri, int *cputim);
int	rttvms_get_procinfo_full( 	int 		prcnum, 
					char		*state,
				 	int 		*pri,
					int		*prib,
				 	int 		*cputim,
					int		*logintim,
					int		*uic,
					int		*bufio,
					char		*imagname,
					int		*gpgcnt,
					int		*jobprccnt,
					int		*pageflts,
					int		*pagfilcnt,
					int		*pgflquota,
					char		*prcnam,
					int		*fillm,
					int		*virtpeak,
					int		*wspeak,
					int		*wsquota,
					int		*wsextent,
					int		*wssize,
					int		*ppgcnt,
					int		*dirio);
int	rttvms_get_pwr_proc( 
		int		*proc_pid_p,
		pwr_tString32	*proc_name_p,
		pwr_tString16	*proc_pidstr_p,
		int		*proc_pri_p,
		pwr_tString16	*proc_state_p,
		float		*proc_cputim_p,
		int		max_proc,
		int		*proc_count);
#elif defined OS_ELN
int	rtteln_get_jobs( 
		int		*proc_pid_p,
		pwr_tString32	*proc_name_p,
		int		*proc_pri_p,
		pwr_tString16	*proc_state_p,
		float		*proc_cputim_p,
		int		max_proc,
		int		*proc_count);
int	rtteln_get_job_info( 
		int		job_generation,
		pwr_tString32	*proc_name_p,
		int		*proc_pri_p,
		pwr_tString16	*proc_state_p,
		float		*proc_cputim_p,
		pwr_tString	*proc_filename_p,
		char		*pgm_mode,
		int		*user_stack,
		int		*kernel_stack,
		char		*optionflags);
int	rtteln_get_job_proc( 
		int		job_generation,
		int		*proc_generation_p,
		pwr_tString32	*proc_name_p,
		int		*proc_pri_p,
		pwr_tString16	*proc_state_p,
		float		*proc_cputim_p,
		int		max_proc,
		int		*proc_count);
#elif defined OS_LYNX || defined OS_LINUX || defined OS_MACOS
int     rtt_replace_env( char *str, char *newstr);
#endif

int	rtt_get_platform( char *platform);
int	rtt_get_hw( char *hw);

/* Module rt_rtt_qiomod.c */

int qio_assign( char *s, int *chn);
int qio_set_attr( int *chn);
int qio_reset( int *chn);
int qio_readw( int *chn, char *buf, int len);
int qio_read( int *chn, int tmo, char *buf, int len);
int qio_writew( int *chn, char *buf, int len);
int qio_write( int *chn, int tmo, char *buf, int len);

#endif

/* Module rt_rtt_cmdmode.c */
int	rtt_store_symbols( char	*filename);
int	rtt_show_symbols( menu_ctx	ctx);
int	rtt_replace_symbol( char *command, char *newcommand);
int	rtt_get_symbol( char *key, char *value);
int	rtt_get_symbol_cmd( char *key, char *value);
int	rtt_define_symbol( char *key, char *arg1, char *arg2, char *arg3);
int 	rtt_attribute_func ( 
  char		*name,
  int		*return_decl,
  ccm_tFloat   	*return_float,
  ccm_tInt     	*return_int,
  char		*return_string);

/* Module rt_rtt_rtc.c */
#ifdef OS_ELN
int rtt_rtc( int function);
#endif


