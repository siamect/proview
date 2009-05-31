/* 
 * Proview   $Id: rt_rtt_edit.c,v 1.3 2005-09-01 14:57:56 claes Exp $
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

/* rt_rtt_edit.c
   This module contains routines for the rtt editor. */


/*_Include files_________________________________________________________*/

#if defined(OS_VMS)
 #include <stdio.h>
 #include <ctype.h>
 #include <descrip.h>
 #include <lib$routines.h>
 #include <clidef.h>
 #include <ssdef.h>
 #include <string.h>
 #include <chfdef.h>
 #include <stdlib.h>
#elif defined(OS_LYNX) || defined(OS_LINUX)
 #include <stdio.h>
 #include <ctype.h>
 #include <string.h>
 #include <stdlib.h>
#endif

#include "pwr.h"
#include "pwr_class.h"
#include "rt_gdh.h"
#include "rt_rtt_dir.h"
#include "rt_rtt.h"
#include "rt_rtt_functions.h"
#include "rt_rtt_global.h"
#include "rt_rtt_edit.h"
#include "rt_rtt_msg.h"
#include "rt_rtt_edit_helptext.h"

/* Nice functions */
#define ODD(a)	(((int)(a) & 1) != 0)
#define EVEN(a)	(((int)(a) & 1) == 0)
#define max(Dragon,Eagle) ((Dragon) > (Eagle) ? (Dragon) : (Eagle))
#define min(Dragon,Eagle) ((Dragon) < (Eagle) ? (Dragon) : (Eagle))
#ifndef __ALPHA
#define abs(Dragon) ((Dragon) >= 0 ? (Dragon) : (-(Dragon)))
#endif

#define CHARSET_ASCII(a) \
	if ( a & RTT_CHARSET_LINE) a -= RTT_CHARSET_LINE;
#define CHARSET_LINE(a) \
	a |= RTT_CHARSET_LINE;
#define CHARSET_NOINVERSE(a) \
	if ( a & RTT_CHARSET_INVERSE) a -= RTT_CHARSET_INVERSE;
#define CHARSET_INVERSE(a) \
	a |= RTT_CHARSET_INVERSE;


#define	MENU_ATTRIBUTES_TITLE "Menu attributes"

typedef enum {
	eAction_Link 	= 1,
	eAction_Compile = 2
	} dtt_eAction;
	

extern 	unsigned long	rtt_chn[4];
extern 	rtt_t_recall 	*rtt_recallbuff;
extern	rtt_t_helptext	*rtt_command_helptext;
extern	rtt_t_helptext	rtt_appl_helptext[];
extern	menu_ctx	rtt_collectionmenuctx;
static	dtt_t_store_ctx	*dtt_ctx_store = 0;
static	dtt_t_store_menuctx	*dtt_menuctx_store = 0;
static	menu_ctx	dtt_root_ctx = 0;
static 	char		dtt_programname[80];
static 	char		dtt_maintitle[80];
static 	char		dtt_title_prefix[80];

static	int		dtt_gdh_initialized = 0;
static	rtt_t_backgr	dtt_paste_chartable;
static	dtt_item	dtt_paste_itemlist = 0;
static	int		dtt_paste_size_x;
static	int		dtt_paste_size_y;

static	long int       	dtt_current_index = 2;
static 	int		dtt_is_rttsys;
static	int		dtt_opsys = 0;
static	int		dtt_current_opsys;
static	char		dtt_source_dir[80];
static	char		dtt_build_dir[80];
static	char		dtt_exe_dir[80];

/*_Local function prototypes_____________________________________________*/

static int	set_func(	edit_ctx	ctx,
				int		flag);

static int	write_func(	edit_ctx	ctx,
				int		flag);
static int	syntax_func(	edit_ctx	ctx,
				int		flag);
static int	save_func(	edit_ctx	ctx,
				int		flag);
static int	generate_func(	edit_ctx	ctx,
				int		flag);
static int	dtt_exit_func(	edit_ctx	ctx,
				int		flag);
static int	dtt_quit_func(	edit_ctx	ctx,
				int		flag);
static int	dtt_export_func(	edit_ctx	ctx,
					int		flag);
static int	dtt_list_func(	edit_ctx	ctx,
				int		flag);
static int	dtt_clear_func(	edit_ctx	ctx,
				int		flag);
static int	dtt_connect_func(	edit_ctx	ctx,
				int		flag);
static int	dtt_dualconnect_func(	edit_ctx	ctx,
				int		flag);
static int	dtt_help_func(	menu_ctx	ctx,
				int		flag);
static int	dtt_link_func(	edit_ctx	ctx,
				int		flag);
static int	dtt_compile_func(	edit_ctx	ctx,
				int		flag);
static int	dtt_edit_func(	edit_ctx	ctx,
				int		flag);
static int	dtt_run_func(	edit_ctx	ctx,
				int		flag);
static int	dtt_include_func(	menu_ctx	ctx,
				int		flag);
static int	dtt_undo_func(	menu_ctx	ctx,
				int		flag);
static int	dtt_delete_func(	edit_ctx	ctx,
				int		flag);
static int	dtt_paste_func(	edit_ctx	ctx,
				int		flag);
static int	dtt_cut_func(	edit_ctx	ctx,
				int		flag);
static int	dtt_copy_func(	edit_ctx	ctx,
				int		flag);
static int	dtt_select_func(	edit_ctx	ctx,
				int		flag);
static int	dtt_unselect_func(	edit_ctx	ctx,
				int		flag);
static int	create_func(	edit_ctx	ctx,
				int		flag);
static int	modify_func(	edit_ctx	ctx,
				int		flag);
static int	dtt_rtt_func(	menu_ctx	ctx,
				int		flag);
static int	dtt_show_func(	menu_ctx	ctx,
				int		flag);
static int	dtt_edit_draw( edit_ctx	ctx);
static int	dtt_edit_draw_itemlist( edit_ctx	ctx);
static int	dtt_edit_restore_settings( edit_ctx ctx);
static int	dtt_edit_normal_settings();
static int	dtt_store_ctx( 	edit_ctx	ctx,
				char		*key,
				int		idx);
#if 0
static int dtt_remove_stored_ctx( char		*key,
				  int		idx);
#endif
static int dtt_get_stored_ctx( 	edit_ctx	*ctx,
				char		*key,
				int		idx);
static int	dtt_store_menuctx(	menu_ctx	ctx,
					int		key);
static int	dtt_get_stored_menuctx( menu_ctx	*ctx, 
					int		key);
static void	dtt_free_ctx( 	edit_ctx	ctx);
static int	dtt_create_ctx( 	edit_ctx	*ctx,
					edit_ctx	parent_ctx,
					char		*title,
					int		index);
static int	dtt_edit_write( edit_ctx	ctx,
				char		*filename,
				char		*menu_filename,
				int		userspec_filename,
				int		nocompile,
				int		generate_only,
				int		opsys,
				int		return_wait,
				int		*messages);
static int	dtt_edit_read( 	edit_ctx	ctx,
				char		*name,
				int		userspec_name);
static int	dtt_upd_item_add( 	dtt_item	*updlist,
					int		x,
					int		y,
					char		*text);
static int	dtt_get_upd_item( 	dtt_item	itemlist,
					int		x,
					int		y,
					dtt_item	*item_ptr);
static int	dtt_upd_item_show( 	edit_ctx	parent_ctx,
					int		arg1,
					int		arg2,
					int		arg3,
					int		arg4);
static int	dtt_number_sort( dtt_item	itemlist);
static int	dtt_item_delete(	dtt_item	itemlist,
					dtt_item	item_ptr);
static int	dtt_edit_select( edit_ctx	ctx);
static int	dtt_edit_unselect( edit_ctx	ctx);
static int	rtt_edit_draw_section( 	rtt_t_backgr	*chartable,
					int		x1,
					int		y1,
					int		x2,
					int		y2,
					int		inverse);
static int	dtt_edit_delete_section(rtt_t_backgr	*chartable,
					dtt_item	itemlist,
					int		x1,
					int		y1,
					int		x2,
					int		y2);
static int	dtt_paste_insert(	rtt_t_backgr	*chartable,
					dtt_item	itemlist,
					int		x1,
					int		y1,
					int		x2,
					int		y2);
static int	dtt_paste_copy( rtt_t_backgr	*chartable,
				dtt_item	*itemlist_ptr,
				int		x,
				int		y);
static int	dtt_paste_item_insert(	dtt_item	item,
					int		x,
					int		y);
static int	dtt_paste_item_copy(	dtt_item	paste_item,
					dtt_item	*itemlist_ptr,
					int		x,
					int		y);
static int	dtt_edit_save_menues(	char		*filename,
					int		generate_only);
static int	dtt_edit_save_one_menu(	FILE		*fout,
					FILE		*fout_c,
					FILE		*fout_h,
					FILE		*fout_db1,
					FILE		*fout_db2,
					FILE		*fout_decl,
					menu_ctx	ctx,
					int		index,
					int		generate_only,
					int		nocompile);
static int	dtt_edit_read_menues(	char		*filename);
static int	dtt_edit_read_one_menu(	FILE		*fin,
					menu_ctx	parent_ctx,
					int		parent_item);
#if 0
static int	dtt_get_menu_name(	int		index,
					char		*name);
#endif
static int	dtt_get_picturefilename(	int		index,
						char		*filename);
static int	dtt_get_menufilename( char		*filename);
static int	dtt_get_menuname(	int		index,
					char		*menu_name);
static int	dtt_get_picturename(	int		index,
				char		*picture_name);
static int dtt_cc( int opsys, int action, int debug);
static int dtt_compile_picture( char	*filename,
				int	opsys);
static int dtt_read_line(	char	*line,
				int	maxsize,
				FILE	*file);
static int	dtt_menu_draw_item( 	menu_ctx	ctx,
					int		item);
static int	dtt_menu_draw( menu_ctx	ctx);
static int	dtt_menu_select( menu_ctx	ctx);
static int	dtt_menu_unselect( menu_ctx	ctx);
static int	dtt_edit_process_menues(
			int		(* func) (),
			unsigned long	arg1,
			unsigned long	arg2,
			unsigned long	arg3,
			unsigned long	arg4,
			unsigned long	arg5);
static int	dtt_edit_process_one_menu(
			int		(* func) (),
			unsigned long		arg1,
			unsigned long		arg2,
			unsigned long		arg3,
			unsigned long		arg4,
			unsigned long		arg5,
			menu_ctx	ctx,
			int		index);
static int	dtt_edit_write_gdhrefs( char *filename);
static int	dtt_edit_write_menu_gdhrefs( 	menu_ctx ctx, 
					int menu_idx, 
					FILE *outfile, 
					int dum1, 
					int dum2, 
					int dum3, 
					int dum4);
static int	dtt_edit_pwrplc_gdhrefs( char *filename);
static int	dtt_edit_pwrplc_menu_gdhrefs( 	menu_ctx ctx, 
					int menu_idx, 
					FILE *outfile, 
					int *externref_index, 
					int dum2, 
					int dum3, 
					int dum4);
static int	dtt_edit_list_items_all( char *filename);
static int	dtt_edit_list_items_picture( 	menu_ctx ctx, 
					int menu_idx, 
					FILE *outfile, 
					int dum1, 
					int dum2, 
					int dum3, 
					int dum4);
static int	dtt_edit_list_items( 	edit_ctx	picture_ctx,
					FILE		*outfile);
static int	dtt_edit_read_items( 	edit_ctx	ctx,
					char		*filename);
static int	dtt_edit_read_picture_items( 	edit_ctx	ctx,
						FILE		*infile);
static int	dtt_edit_save_all(	int	generate,
					int	nocompile,
					int	opsys);
static int	dtt_edit_convert_all( void);
static int	dtt_edit_save_picture( 	menu_ctx ctx,
					int menu_idx, 
					int keep_ctx,
					int index,
					int generate_only, 
					int opsys, 
					int dum4);
static int	dtt_edit_convert_picture( 	menu_ctx ctx,
					int menu_idx,
					int keep_ctx,
					int index,
					int dum2,
					int dum3,
					int dum4);
static int	dtt_edit_write_menue( 	menu_ctx	ctx,
					char		*filename,
					int		all);
static int	dtt_edit_write_one_menu(	FILE		*fout,
						menu_ctx	ctx,
						int		index);
static int	dtt_edit_include_menue(	menu_ctx	ctx,
					char		*filename);
static int	dtt_edit_include_one_menu( 	FILE		*fin,
						menu_ctx	parent_ctx,
						int		parent_item);
static int 	dtt_check_if_function_menu(	menu_ctx ctx,
						int menu_idx, 
						int *function_found,
						int dum1, 
						int dum2, 
						int dum3, 
						int dum4);
static int dtt_function_menues_exist();
static int	dtt_show_menu( 	menu_ctx	ctx, 
				char 		*menu_name);
static int dtt_command_get_input(
			char		*chn,
			char		*out_string,
			unsigned long	*out_terminator,
			int		out_maxlen,
			unsigned long	option,
			int		timeout);
static char	*dtt_quotes_convert( char *str);
static int dtt_edit_print_syntaxerror(
			dtt_item 	item_ptr,
			char		*error_text,
			int		error,
			int		*error_count,
			int		*warning_count);
static int	dtt_edit_check_items_syntax( 	edit_ctx	picture_ctx,
						int		*error_count,
						int		*warning_count);
static int	dtt_edit_save_by_index( int	index, 
					int	generate_only,
					int	nocompile,
					int	opsys);
static int	dtt_edit_draw_position( edit_ctx	ctx);
static int	dtt_menu_item_show( 	menu_ctx	ctx,
					int		arg1,
					int		arg2,
					int		arg3,
					int		arg4);
static int	dtt_setup( menu_ctx	parent_ctx);
static void	dtt_exit_now( pwr_tStatus exit_sts);
static char *dtt_opsys_to_name( int opsys);


/*************************************************************************
*
* Name:		dtt_initialize()
*
* Type		int
*
* Type		Parameter	IOGF	Description
*
* Description:
*	This function initializes dtt.
*
**************************************************************************/

int	dtt_initialize()
{
	int	sts;

	rtt_priv = RTT_PRV_SYS;
	rtt_init_state_table();
	qio_assign( "stdin", (int *) &rtt_chn);
	sts = rtt_recall_create( &rtt_recallbuff);
	if (EVEN(sts)) return sts;
	sts = rtt_recall_create( &rtt_value_recallbuff);
	if (EVEN(sts)) return sts;

	sts = rtt_get_platform( rtt_platform);
	sts = rtt_get_hw( rtt_hw);

	return RTT__SUCCESS;
}


/*************************************************************************
*
* Name:		set_func()
*
* Type		int
*
* Type		Parameter	IOGF	Description
* menu_ctx	ctx		I	rtt context.
*
* Description:
*	This function is called when a "collect" command i recieved.
*	
**************************************************************************/

static int	set_func(	edit_ctx	ctx,
				int		flag)
{
	char	arg1_str[80];
	char	arg2_str[80];

	if ( EVEN( rtt_get_qualifier( "rtt_arg1", arg1_str)))
	{
	  rtt_message('E', "Qualifier is missing");
	  return RTT__NOPICTURE;
	}
	if ( strncmp( arg1_str, "FILE", strlen( arg1_str)) == 0)
	{
	  char str[80];

	  if ( ODD( rtt_get_qualifier( "/ON", str)))
	  {
	    if ( rtt_file_on)
	    {
	      rtt_message('E', "File is already on");
	      return RTT__NOPICTURE;
	    }
	    if ( EVEN( rtt_get_qualifier( "/NAME", str)))
	    {
	      rtt_message('E', "Enter name");
	      return RTT__HOLDCOMMAND;
	    }

	    rtt_outfile = fopen( str, "w");
	    if ( rtt_outfile == 0)
	    {
	      rtt_message('E', "Unable to open file");
	      return RTT__HOLDCOMMAND;
	    }
	    if ( ODD( rtt_get_qualifier( "/MESSAGE", str)))
	      rtt_print_message = 1;
	    else
	      rtt_print_message = 0;
	      
	    if ( ODD( rtt_get_qualifier( "/COMMAND", str)))
	      rtt_print_command = 1;
	    else
	      rtt_print_command = 0;
	      
	    rtt_file_on = 1;
	    return RTT__NOPICTURE;
	  }
	  else if ( ODD( rtt_get_qualifier( "/OFF", str)))
	  {
	    if ( !rtt_file_on)
	    {
	      rtt_message('E', "File is not on");
	      return RTT__NOPICTURE;
	    }
	    fclose( rtt_outfile);
	    rtt_print_message = 0;
	    rtt_print_command = 0;
	    rtt_file_on = 0;
	    return RTT__NOPICTURE;
	  }
	  else
	  {
	    rtt_message('E', "Syntax error");
	    return RTT__HOLDCOMMAND;
	  }
	}
	else if ( strncmp( arg1_str, "VERIFY", strlen( arg1_str)) == 0)
	{
	  rtt_verify = 1;
	  rtt_message('I', "Verify set on");
	  return RTT__NOPICTURE;
	}
	else if ( strncmp( arg1_str, "NOVERIFY", strlen( arg1_str)) == 0)
	{
	  rtt_verify = 0;
	  rtt_message('I', "Verify set off");
	  return RTT__NOPICTURE;
	}
	else if ( strncmp( arg1_str, "LINE", strlen( arg1_str)) == 0)
	{
	  rtt_charset_linedrawing();
	  CHARSET_LINE(ctx->charset);
	  return RTT__NOPICTURE;
	}
	else if ( strncmp( arg1_str, "ASCII", strlen( arg1_str)) == 0)
	{
	  rtt_charset_ascii();
	  CHARSET_ASCII( ctx->charset);
	  return RTT__NOPICTURE;
	}
	else if ( strncmp( arg1_str, "INVERSE", strlen( arg1_str)) == 0)
	{
	  rtt_char_inverse_start();
	  CHARSET_INVERSE(ctx->charset);
	  return RTT__NOPICTURE;
	}
	else if ( strncmp( arg1_str, "NOINVERSE", strlen( arg1_str)) == 0)
	{
	  rtt_char_inverse_end();
	  CHARSET_NOINVERSE( ctx->charset);
	  return RTT__NOPICTURE;
	}
	else if ( strncmp( arg1_str, "MODE", strlen( arg1_str)) == 0)
	{
	  if ( EVEN( rtt_get_qualifier( "rtt_arg2", arg2_str)))
	  {
	    rtt_message('E', "Qualifier is missing");
	    return RTT__NOPICTURE;
	  }
	  if ( strncmp( arg2_str, "FORMAT", strlen( arg2_str)) == 0)
	  {
	    if ( ctx->ctx_type != DTT_CTX_EDIT)
	    {
	      rtt_message('E', "Invalid command in this context");
	      return RTT__NOPICTURE;
	    }

	    ctx->display_mode = DTT_DISPLAY_MODE_FORMAT;
	    return RTT__SUCCESS;
	  }
	  else if ( strncmp( arg2_str, "NUMBER", strlen( arg2_str)) == 0)
	  {
	    if ( ctx->ctx_type != DTT_CTX_EDIT)
	    {
	      rtt_message('E', "Invalid command in this context");
	      return RTT__NOPICTURE;
	    }

	    ctx->display_mode = DTT_DISPLAY_MODE_NUMBER;
	    return RTT__SUCCESS;
	  }
	  else
	  {
	    rtt_message('E', "Unknown qualifier");
	    return RTT__HOLDCOMMAND;
	  }
	}
	else
	{
	  rtt_message('E', "Unknown qualifier");
	  return RTT__HOLDCOMMAND;
	}

	return RTT__SUCCESS;	
}
/*************************************************************************
*
* Name:		write_func()
*
* Type		int
*
* Type		Parameter	IOGF	Description
* menu_ctx	ctx		I	rtt context.
*
* Description:
*	This function is called when a "collect" command i recieved.
*	
**************************************************************************/

static int	write_func(	edit_ctx	ctx,
				int		flag)
{
	int		sts;
	char		arg1_str[80];
	char		arg2_str[80];
	char		filename[80];
	char		menu_filename[80];
	int		messages;

	if ( ODD( rtt_get_qualifier( "rtt_arg1", arg1_str)))
	{
	  if ( strncmp( arg1_str, "ITEMS", strlen( arg1_str)) == 0)
	  {
	    char		outfilename[80];
	    char		message[120];
	    FILE		*outfile;

	    /* Command is "WRITE ITEMS" */
	    if ( ODD( rtt_get_qualifier( "/ALL", arg2_str)))
	    {
	      if ( ODD( rtt_get_qualifier( "rtt_arg2", arg2_str)))
	      {
	        /* arg2 is the filename */
	        sts = dtt_edit_list_items_all( arg2_str);
	        if ( EVEN(sts)) return sts;
	    
	        return RTT__NOPICTURE;
	      }
	      else
	      {
	        rtt_message('E', "Enter filename");
	        return RTT__HOLDCOMMAND;
	      }
	    }
	    else
	    {
	      if (( ctx->ctx_type == DTT_CTX_EDIT) || 
		((ctx->parent_ctx != 0) && 
		(((edit_ctx)(ctx->parent_ctx))->ctx_type == DTT_CTX_EDIT)))
	      {
	        /* This is a picture or update item menu, save the picture */
	        if ( ctx->ctx_type != DTT_CTX_EDIT)
	          ctx = ctx->parent_ctx;
	        /* Take the default filename */
	        if ( ctx->ctx_type != DTT_CTX_EDIT)
	        ctx = ctx->parent_ctx;

	        if ( ODD( rtt_get_qualifier( "rtt_arg2", arg2_str)))
	        {
	          outfile = fopen( arg2_str, "w");
	          if ( !outfile)
	          {
	            rtt_message('E', "Unable to open file");
	            return RTT__NOPICTURE;
	          }

	          sts = dtt_edit_list_items( ctx, outfile);

	          rtt_fgetname( outfile, outfilename, arg2_str);
	          fclose( outfile);
	          if ( EVEN(sts)) return sts;

	          sprintf( message, "%s created", outfilename);
	          rtt_message('I', message);
	          return RTT__NOPICTURE;
	        }
	        else
	        {
	          rtt_message('E', "Enter filename");
	          return RTT__HOLDCOMMAND;
	        }
	      }
	      else
	      {
	        rtt_message('E', "Function not defined in this picture");
	        return RTT__NOPICTURE;
	      }
	    }
	  }
	  else if ( strncmp( arg1_str, "PICTURE", strlen( arg1_str)) == 0)
	  {
	    if (( ctx->ctx_type == DTT_CTX_EDIT) ||
		((ctx->parent_ctx != 0) &&
		(((edit_ctx)(ctx->parent_ctx))->ctx_type == DTT_CTX_EDIT)))
	    {
	      if ( ctx->ctx_type != DTT_CTX_EDIT)
	        ctx = ctx->parent_ctx;
	      if ( EVEN( rtt_get_qualifier( "rtt_arg2", arg2_str)))
	      {
	        rtt_message('E', "Enter filename");
	        return RTT__HOLDCOMMAND;
	      }

	      /* Take the argument as filename */
	      strcpy( filename, arg2_str);
	      dtt_get_menufilename( menu_filename);
	  
	      sts = dtt_edit_write( ctx, filename, menu_filename, 1, 1, 0, 0,
			0, &messages);
	      if ( EVEN(sts))
	      {
	        rtt_message('E', "Unable to open file, check filename");
	        return RTT__NOPICTURE;
	      }
	      else
	        rtt_message('I', "Picture written");
	      if ( messages)
	        return RTT__SUCCESS;
	      else
	        return RTT__NOPICTURE;
	    }
	  }
	  else if ( strncmp( arg1_str, "MENU", strlen( arg1_str)) == 0)
	  {
	    int		all;

  	    /* Check that this is not an edit context */
	    if ( ctx->ctx_type == DTT_CTX_EDIT)
	    {
	      rtt_message('E',"Position in a menue");
	      return RTT__NOPICTURE;
	    }

	    if ( EVEN( rtt_get_qualifier( "rtt_arg2", arg2_str)))
	    {
	      rtt_message('E', "Enter filname");
	      return RTT__HOLDCOMMAND;
	    }

	    /* Take the argument as filename */
	    strcpy( filename, arg2_str);

	    all = ODD( rtt_get_qualifier( "/ALL", arg2_str));
	  
	    sts = dtt_edit_write_menue( (menu_ctx) ctx, filename, all);
	    if ( sts == RTT__PICTURENOTSAVED)
	    {
	      return RTT__NOPICTURE;
	    }
	    else if ( EVEN(sts))
	    {
	      rtt_message('E', "Unable to open file, check filename");
	      return RTT__NOPICTURE;
	    }
	    else
	      rtt_message('I', "Menue written");
	
	    return RTT__NOPICTURE;
	  }
	  else
	  {
	    rtt_message('E', "Syntax error");
	    return RTT__HOLDCOMMAND;
	  }
	}
	else
	{
	  rtt_message('E', "Syntax error");
	  return RTT__HOLDCOMMAND;
	}
	return RTT__SUCCESS;
}
/*************************************************************************
*
* Name:		convert_func()
*
* Type		int
*
* Type		Parameter	IOGF	Description
* menu_ctx	ctx		I	rtt context.
*
* Description:
*	This function is called when a "convert" command i recieved.
*	
**************************************************************************/

static int	convert_func(	edit_ctx	ctx,
				int		flag)
{
	int		sts;

	sts = dtt_edit_convert_all();
	if ( EVEN(sts))
	{
	  rtt_message('E', "Conversion error");
	  return RTT__NOPICTURE;
	}
	return RTT__NOPICTURE;
}

/*************************************************************************
*
* Name:		save_func()
*
* Type		int
*
* Type		Parameter	IOGF	Description
* menu_ctx	ctx		I	rtt context.
*
* Description:
*	This function is called when a "save" command i recieved.
*	
**************************************************************************/

static int	save_func(	edit_ctx	ctx,
				int		flag)
{
	int		sts;
	char		arg1_str[80];
	char		filename[80];
	char		menu_filename[80];
	int		messages;
	int		opsys;
	int		nocompile;

	nocompile = ODD( rtt_get_qualifier( "/NOCOMPILE", arg1_str));

	if ( ODD( rtt_get_qualifier( "/ALL", arg1_str)))
	{
	  if ( EVEN( rtt_get_qualifier( "rtt_arg1", arg1_str)))
	  {
	    sts = dtt_edit_save_all( 0, nocompile, dtt_opsys);
	  }
	  else
	  {
            if ( strncmp( arg1_str, "VAX_VMS", strlen( arg1_str)) == 0)
	      opsys = pwr_mOpSys_VAX_VMS;
	    else if ( strncmp( arg1_str, "VAX_ELN", strlen( arg1_str)) == 0)
	      opsys = pwr_mOpSys_VAX_ELN;
	    else if ( strncmp( arg1_str, "AXP_VMS", strlen( arg1_str)) == 0)
	      opsys = pwr_mOpSys_AXP_VMS;
	    else if ( strncmp( arg1_str, "PPC_LYNX", strlen( arg1_str)) == 0)
	      opsys = pwr_mOpSys_PPC_LYNX;
	    else if ( strncmp( arg1_str, "X86_LYNX", strlen( arg1_str)) == 0)
	      opsys = pwr_mOpSys_X86_LYNX;
	    else if ( strncmp( arg1_str, "PPC_LINUX", strlen( arg1_str)) == 0)
	      opsys = pwr_mOpSys_PPC_LINUX;
	    else if ( strncmp( arg1_str, "X86_LINUX", strlen( arg1_str)) == 0)
 	      opsys = pwr_mOpSys_X86_LINUX;
	    else if ( strncmp( arg1_str, "X86_64_LINUX", strlen( arg1_str)) == 0)
 	      opsys = pwr_mOpSys_X86_64_LINUX;
	    else
	    {
	      rtt_message('E', "Unknown platform");
	      return RTT__NOPICTURE;
	    }
	    sts = dtt_edit_save_all( 0, nocompile, opsys);
	  }
	  if ( sts == RTT__PICTURENOTSAVED)
	  {
	    return RTT__NOPICTURE;
	  }
	  else if ( EVEN(sts))
	  {
	    rtt_message('E', "Unable to open file");
	    return RTT__NOPICTURE;
	  }
	  rtt_message('I', "All pictures and menues saved");
	  return RTT__NOPICTURE;
	}
	else
	{
	  if (( ctx->ctx_type == DTT_CTX_EDIT) || 
		((ctx->parent_ctx != 0) && 
		(((edit_ctx)(ctx->parent_ctx))->ctx_type == DTT_CTX_EDIT)))
	  {
	    /* This is a picture or update item menu, save the picture */
	    if ( ctx->ctx_type != DTT_CTX_EDIT)
	      ctx = (edit_ctx) ctx->parent_ctx;

	    /* Take the default filename */
	    dtt_get_picturefilename( ctx->index, filename); 
	    dtt_get_menufilename( menu_filename);
	  
	    if ( EVEN( rtt_get_qualifier( "rtt_arg1", arg1_str)))
	    {
	      sts = dtt_edit_write( ctx, filename, menu_filename, 0, nocompile,
			0, dtt_opsys, 1, &messages);
	    }
	    else
	    {
              if ( strncmp( arg1_str, "VAX_VMS", strlen( arg1_str)) == 0)
	        opsys = pwr_mOpSys_VAX_VMS;
	      else if ( strncmp( arg1_str, "VAX_ELN", strlen( arg1_str)) == 0)
	        opsys = pwr_mOpSys_VAX_ELN;
	      else if ( strncmp( arg1_str, "AXP_VMS", strlen( arg1_str)) == 0)
	        opsys = pwr_mOpSys_AXP_VMS;
	      else if ( strncmp( arg1_str, "PPC_LYNX", strlen( arg1_str)) == 0)
	        opsys = pwr_mOpSys_PPC_LYNX;
	      else if ( strncmp( arg1_str, "X86_LYNX", strlen( arg1_str)) == 0)
	        opsys = pwr_mOpSys_X86_LYNX;
	      else if ( strncmp( arg1_str, "PPC_LINUX", strlen( arg1_str)) == 0)
	        opsys = pwr_mOpSys_PPC_LINUX;
	      else if ( strncmp( arg1_str, "X86_LINUX", strlen( arg1_str)) == 0)
	        opsys = pwr_mOpSys_X86_LINUX;
	      else if ( strncmp( arg1_str, "X86_64_LINUX", strlen( arg1_str)) == 0)
	        opsys = pwr_mOpSys_X86_64_LINUX;
	      else
	      {
	        rtt_message('E', "Unknown platform");
	        return RTT__NOPICTURE;
	      }
	      sts = dtt_edit_write( ctx, filename, menu_filename, 0, nocompile,
			0, opsys, 1, &messages);
	    }
	    if ( EVEN(sts))
	    {
	      rtt_message('E', "Unable to open file");
	      return RTT__NOPICTURE;
	    }
	    /* Save the menues also */
	    sts = dtt_edit_save_menues( menu_filename, 0);
	    if ( sts == RTT__PICTURENOTSAVED)
	    {
	      return RTT__NOPICTURE;
	    }
	    else if ( EVEN(sts))
	    {
	      rtt_message('E', "Unable to open file");
	      return RTT__NOPICTURE;
	    }

	    rtt_message('I', "Picture saved");
	    if ( messages)
	      return RTT__SUCCESS;
	    else
	      return RTT__NOPICTURE;
	  }
	  else
	  {
	    /* Take the default filename */
	    dtt_get_menufilename( filename);
	  
	    sts = dtt_edit_save_menues( filename, 0);
	    if ( sts == RTT__PICTURENOTSAVED)
	    {
	      return RTT__NOPICTURE;
	    }
	    else if ( EVEN(sts))
	    {
	      rtt_message('E', "Unable to open file");
	      return RTT__NOPICTURE;
	    }

	    rtt_message('I', "Menues saved");
	    return RTT__NOPICTURE;
	  }
	}
	return RTT__SUCCESS;
}
/*************************************************************************
*
* Name:		generate_func()
*
* Type		int
*
* Type		Parameter	IOGF	Description
* menu_ctx	ctx		I	rtt context.
*
* Description:
*	This function is called when a "generate" command i recieved.
*	
**************************************************************************/

static int	generate_func(	edit_ctx	ctx,
				int		flag)
{
	int		sts;
	char		arg1_str[80];
	char		filename[80];
	char		*s, *t;
	char		index_str[10];
	int		index;
	int		found;
	int		opsys;

	if ( EVEN( rtt_get_qualifier( "rtt_arg1", arg1_str)))
	{
	  opsys = dtt_opsys;
	}
	else
	{
          if ( strncmp( arg1_str, "VAX_VMS", strlen( arg1_str)) == 0)
	    opsys = pwr_mOpSys_VAX_VMS;
	  else if ( strncmp( arg1_str, "VAX_ELN", strlen( arg1_str)) == 0)
	    opsys = pwr_mOpSys_VAX_ELN;
	  else if ( strncmp( arg1_str, "AXP_VMS", strlen( arg1_str)) == 0)
	    opsys = pwr_mOpSys_AXP_VMS;
	  else if ( strncmp( arg1_str, "PPC_LYNX", strlen( arg1_str)) == 0)
	    opsys = pwr_mOpSys_PPC_LYNX;
	  else if ( strncmp( arg1_str, "X86_LYNX", strlen( arg1_str)) == 0)
	    opsys = pwr_mOpSys_X86_LYNX;
	  else if ( strncmp( arg1_str, "PPC_LINUX", strlen( arg1_str)) == 0)
	    opsys = pwr_mOpSys_PPC_LINUX;
 	  else if ( strncmp( arg1_str, "X86_LINUX", strlen( arg1_str)) == 0)
 	    opsys = pwr_mOpSys_X86_LINUX;
	  else if ( strncmp( arg1_str, "X86_64_LINUX", strlen( arg1_str)) == 0)
	    opsys = pwr_mOpSys_X86_64_LINUX;
	  else
	  {
	    rtt_message('E', "Unknown platform");
	    return RTT__NOPICTURE;
	  }
	}

	if ( ODD( rtt_get_qualifier( "/ALL", arg1_str)))
	{
	  sts = dtt_edit_save_all( 1, 0, opsys);
	  if ( sts == RTT__PICTURENOTSAVED)
	  {
	    return RTT__NOPICTURE;
	  }
	  else if ( EVEN(sts))
	  {
	    rtt_message('E', "Unable to open file");
	    return RTT__NOPICTURE;
	  }
	  rtt_message('I', "All pictures and menues generated");
	  return RTT__NOPICTURE;
	}
	else if ( ODD( rtt_get_qualifier( "/ID", arg1_str)))
	{
	  /* Get index from the id */
	  t = index_str;
	  found = 0;
	  for ( s = arg1_str; *s != 0; s++)
	  {
	    if ( isdigit( *s))
	    {
	      found = 1;
	      *t = *s;
	      t++;
	      *t = 0;
	    }
	    else
	      if ( found)
	        break;
	  }
	  if ( !found)
	  {
	    rtt_message('E', "Syntax error in id");
	    return RTT__NOPICTURE;
	  }
	  sts = sscanf( index_str, "%d", &index);
	  if ( sts != 1)
	  {
	    rtt_message('E', "Syntax error in id");
	    return RTT__NOPICTURE;
	  }

	  sts = dtt_edit_save_by_index( index, 1, 0, opsys);
	  if ( sts == RTT__PICTURENOTSAVED)
	  {
	    return RTT__NOPICTURE;
	  }
	  else if ( EVEN(sts))
	  {
	    rtt_message('E', "Unable to open file");
	    return RTT__NOPICTURE;
	  }
	  rtt_message('I', "Picture generated");
	  return RTT__NOPICTURE;
	}
	else if ( ODD( rtt_get_qualifier( "/MENU", arg1_str)))
	{
	  /* Save the menues */
	  /* Take the default filename */
	  dtt_get_menufilename( filename);
	  
	  sts = dtt_edit_save_menues( filename, 1);
	  if ( sts == RTT__PICTURENOTSAVED)
	  {
	    return RTT__NOPICTURE;
	  }
	  else if ( EVEN(sts))
	  {
	    rtt_message('E', "Unable to open file");
	    return RTT__NOPICTURE;
	  }
	}

	return RTT__SUCCESS;
}
/*************************************************************************
*
* Name:		setup_func()
*
* Type		int
*
* Type		Parameter	IOGF	Description
* menu_ctx	ctx		I	rtt context.
*
* Description:
*	This function is called when a "setup" command i recieved.
*
**************************************************************************/

static int	dtt_setup_func(	menu_ctx	ctx,
				int		*flag)
{
	int	sts;

	/* Command is "SETUP" */
	sts = dtt_setup( ctx);
	return sts;
}

/*************************************************************************
*
* Name:		syntax_func()
*
* Type		int
*
* Type		Parameter	IOGF	Description
* menu_ctx	ctx		I	rtt context.
*
* Description:
*	This function is called when a "syntax" command i recieved.
*	
**************************************************************************/

static int	syntax_func(	edit_ctx	ctx,
				int		flag)
{
	int		sts;
	int		error_count, warning_count;

	if (( ctx->ctx_type == DTT_CTX_EDIT) || 
		((ctx->parent_ctx != 0) && 
		(((edit_ctx)(ctx->parent_ctx))->ctx_type == DTT_CTX_EDIT)))
	{
	  /* This is a picture or update item menu, save the picture */
	  if ( ctx->ctx_type != DTT_CTX_EDIT)
	    ctx = (edit_ctx) ctx->parent_ctx;

	  sts = dtt_edit_check_items_syntax( ctx, &error_count, &warning_count);
	  if ( (error_count || warning_count) &&
	     !(rtt_commandmode & RTT_COMMANDMODE_FILE))
	  {
	    rtt_wait_for_return();
	    return RTT__SUCCESS;
	  }
	  if ( !error_count && !warning_count)
	    rtt_message('I', "Syntax successfull completion");
	  return RTT__NOPICTURE;
	}
	else
	{
	  rtt_message('E', "Syntax is only defined in a picture");
	  return RTT__NOPICTURE;
	}
	return RTT__SUCCESS;
}
/*************************************************************************
*
* Name:		dtt_exit_func()
*
* Type		int
*
* Type		Parameter	IOGF	Description
* menu_ctx	ctx		I	rtt context.
*
* Description:
*	This function is called when a "exit" command i recieved.
*	
**************************************************************************/

static int	dtt_exit_func(	edit_ctx	ctx,
				int		flag)
{
	int		sts;
	char		filename[80];
	char		menu_filename[80];
	int		messages;

	if (( ctx->ctx_type == DTT_CTX_EDIT) || 
		((ctx->parent_ctx != 0) && 
		(((edit_ctx)(ctx->parent_ctx))->ctx_type == DTT_CTX_EDIT)))
	{
	  /* This is a picture or update item menu, save the picture */
	  if ( ctx->ctx_type != DTT_CTX_EDIT)
	    ctx = (edit_ctx) ctx->parent_ctx;
	  /* Take the default filename */
	  dtt_get_picturefilename( ctx->index, filename); 
	  dtt_get_menufilename( menu_filename);

	  sts = dtt_edit_write( ctx, filename, menu_filename, 0, 0, 0, 
			dtt_opsys, 1, &messages);
	  if ( EVEN(sts))
	  {
	    rtt_message('E', "Unable to open file");
	    return RTT__NOPICTURE;
	  }
	  /* Save the menues also */
	  /* Take the default filename */
	  
	  sts = dtt_edit_save_menues( menu_filename, 0);
	  if ( sts == RTT__PICTURENOTSAVED)
	  {
	    return RTT__NOPICTURE;
	  }
	  else if ( EVEN(sts))
	  {
	    rtt_message('E', "Unable to open file");
	    return RTT__NOPICTURE;
	  }

	  rtt_message('I', "Picture saved");
	}
	else
	{
	  /* Take the default filename */
	  dtt_get_menufilename( filename);
	  
	  sts = dtt_edit_save_menues( filename, 0);
	  if ( EVEN(sts))
	  {
	    rtt_message('E', "Unable to open file");
	    return RTT__NOPICTURE;
	  }

	  rtt_message('I', "Menues saved");
	}
	dtt_exit_now( RTT__SUCCESS);
	return 0;
}
/*************************************************************************
*
* Name:		dtt_quit_func()
*
* Type		int
*
* Type		Parameter	IOGF	Description
* menu_ctx	ctx		I	rtt context.
*
* Description:
*	This function is called when a "quit" command i recieved.
*	
**************************************************************************/

static int	dtt_quit_func(	edit_ctx	ctx,
				int		flag)
{
	dtt_exit_now( RTT__SUCCESS);
	return RTT__SUCCESS;
}

/*************************************************************************
*
* Name:		write_func()
*
* Type		int
*
* Type		Parameter	IOGF	Description
* menu_ctx	ctx		I	rtt context.
*
* Description:
*	This function is called when a "collect" command i recieved.
*	
**************************************************************************/

static int	dtt_export_func(	edit_ctx	ctx,
					int		flag)
{
	int		sts;
	char		arg1_str[80];
	char		arg2_str[80];

	if ( ODD( rtt_get_qualifier( "rtt_arg1", arg1_str)))
	{
	  if ( strncmp( arg1_str, "GDHREFLIST", strlen( arg1_str)) == 0)
	  {
	    /* Command is "EXPORT GDHREFLIST" */
	    if ( ODD( rtt_get_qualifier( "rtt_arg2", arg2_str)))
	    {
	      /* Take the default filename */
	      sts = dtt_edit_write_gdhrefs( arg2_str);
	      if ( EVEN(sts)) return sts;
	    
	      return RTT__NOPICTURE;
	    }
	    else
	    {
	      rtt_message('E', "Enter filename");
	      return RTT__NOPICTURE;
	    }
	  }
	  else if ( strncmp( arg1_str, "EXTERNREF", strlen( arg1_str)) == 0)
	  {
	    /* Command is "EXPORT EXTERNREF" */
	    if ( ODD( rtt_get_qualifier( "rtt_arg2", arg2_str)))
	    {
	      /* Take the default filename */
	      sts = dtt_edit_pwrplc_gdhrefs( arg2_str);
	      if ( EVEN(sts)) return sts;
	    
	      return RTT__NOPICTURE;
	    }
	    else
	    {
	      rtt_message('E', "Enter filename");
	      return RTT__NOPICTURE;
	    }
	  }
	  else
	  {
	    rtt_message('E', "Syntax error");
	    return RTT__NOPICTURE;
	  }
	}
	else
	{
	  rtt_message('E', "Syntax error");
	  return RTT__NOPICTURE;
	}

}

/*************************************************************************
*
* Name:		list_func()
*
* Type		int
*
* Type		Parameter	IOGF	Description
* menu_ctx	ctx		I	rtt context.
*
* Description:
*	This function is called when a "list" command i recieved.
*	
**************************************************************************/

static int	dtt_list_func(	edit_ctx	ctx,
				int		flag)
{
	int		sts;
	char		arg1_str[80];
	char		arg2_str[80];
	char		outfilename[80];
	char		message[120];
	FILE		*outfile;

	if ( ODD( rtt_get_qualifier( "rtt_arg1", arg1_str)))
	{
	  if ( strncmp( arg1_str, "ITEMS", strlen( arg1_str)) == 0)
	  {
	    /* Command is "LIST ITEMS" */
	    if ( ODD( rtt_get_qualifier( "/ALL", arg2_str)))
	    {
	      if ( ODD( rtt_get_qualifier( "rtt_arg2", arg2_str)))
	      {
	        /* arg2 is the filename */
	        sts = dtt_edit_list_items_all( arg2_str);
	        if ( EVEN(sts)) return sts;
	    
	        return RTT__NOPICTURE;
	      }
	      else
	      {
	        rtt_message('E', "Enter filename");
	        return RTT__NOPICTURE;
	      }
	    }
	    else
	    {
	      if (( ctx->ctx_type == DTT_CTX_EDIT) || 
		((ctx->parent_ctx != 0) && 
		(((edit_ctx)(ctx->parent_ctx))->ctx_type == DTT_CTX_EDIT)))
	      {
	        /* This is a picture or update item menu, save the picture */
	        if ( ctx->ctx_type != DTT_CTX_EDIT)
	          ctx = (edit_ctx) ctx->parent_ctx;

	        if ( ODD( rtt_get_qualifier( "rtt_arg2", arg2_str)))
	        {
	          outfile = fopen( arg2_str, "w");
	          if ( !outfile)
	          {
	            rtt_message('E', "Unable to open file");
	            return RTT__NOPICTURE;
	          }

	          sts = dtt_edit_list_items( ctx, outfile);

	          rtt_fgetname( outfile, outfilename, arg2_str);
	          fclose( outfile);
	          if ( EVEN(sts)) return sts;

	          sprintf( message, "%s created", outfilename);
	          rtt_message('I', message);
	          return RTT__NOPICTURE;
	        }
	        else
	        {
	          rtt_message('E', "Enter filename");
	          return RTT__NOPICTURE;
	        }
	      }
	      else
	      {
	        rtt_message('E', "This i not a picture");
	        return RTT__NOPICTURE;
	      }
	    }
	  }
	  else
	  {
	    rtt_message('E', "Syntax error");
	    return RTT__NOPICTURE;
	  }
	}
	else
	{
	  rtt_message('E', "Syntax error");
	  return RTT__NOPICTURE;
	}

}

/*************************************************************************
*
* Name:		clear_func()
*
* Type		int
*
* Type		Parameter	IOGF	Description
* menu_ctx	ctx		I	rtt context.
*
* Description:
*	This function is called when a "clear" command i recieved.
*	
**************************************************************************/

static int	dtt_clear_func(	edit_ctx	ctx,
				int		flag)
{
	char		arg1_str[80];

	if ( ODD( rtt_get_qualifier( "rtt_arg1", arg1_str)))
	{
	  if ( strncmp( arg1_str, "ITEMS", strlen( arg1_str)) == 0)
	  {
	    if ( ctx->ctx_type == DTT_CTX_EDIT)
	    {
	      /* Clear the itemlist */
	      free( ctx->upd_items);
	      ctx->upd_items = 0;
	      return RTT__SUCCESS;
	    }
	    else
	    {
	      rtt_message('E', "This i not a picture");
	      return RTT__NOPICTURE;
	    }
	  }
	  else if ( strncmp( arg1_str, "PICTURE", strlen( arg1_str)) == 0)
	  {
	    if ( ctx->ctx_type == DTT_CTX_EDIT)
	    {
	      /* Clear the itemlist */
	      free( ctx->upd_items);
	      ctx->upd_items = 0;
	      memset( &(ctx->chartable), 0, sizeof( ctx->chartable));
	      return RTT__SUCCESS;
	    }
	    else
	    {
	      rtt_message('E', "This i not a picture");
	      return RTT__NOPICTURE;
	    }
	  }
	  else
	  {
	    rtt_message('E', "Syntax error");
	    return RTT__NOPICTURE;
	  }
	}
	else
	{
	  rtt_message('E', "Syntax error");
	  return RTT__NOPICTURE;
	}

}
/*************************************************************************
*
* Name:		dtt_connect_func()
*
* Type		int
*
* Type		Parameter	IOGF	Description
* menu_ctx	ctx		I	rtt context.
*
* Description:
*	This function is called when a "connect" command i recieved.
*	
**************************************************************************/

static int	dtt_connect_func(	edit_ctx	ctx,
					int		flag)
{
	dtt_item 	item_ptr;
	rtt_t_menu_upd	*menu_ptr;
	int		sts;
	char		message[80];

	/* Check that this is a picure ctx */
	if ( ctx->ctx_type == DTT_CTX_EDIT)
	{
	  /* Get update item */
	  sts = dtt_get_upd_item( ctx->upd_items, ctx->cursor_x, ctx->cursor_y,
		 &item_ptr);
	  if ( EVEN(sts)) 
	  {
	    rtt_message('E', "Position on an update item to connect it");
	    return RTT__NOPICTURE;
	  }
	  /* Get selected item in collection picture */
	  if ( rtt_collectionmenuctx == 0)
	  {
	    rtt_message('E', "No objects in collection picture");
	    return RTT__NOPICTURE;
	  }
	  if ( rtt_collectionmenuctx->menu == 0)
	  {
	    rtt_message('E', "No objects in collection picture");
	    return RTT__NOPICTURE;
	  }
	    
	  menu_ptr = (rtt_t_menu_upd *) rtt_collectionmenuctx->menu;
	  menu_ptr += rtt_collectionmenuctx->current_item;
	  /* Copy selected object and parameter to update item */
	  strcpy( item_ptr->parameter, menu_ptr->parameter_name);

	  strcpy( message, "Parameter connected ");
	  strcat( message, menu_ptr->parameter_name);
	  rtt_message('I', message);
	}

	return RTT__NOPICTURE;
}

/*************************************************************************
*
* Name:		dtt_dualconnect_func()
*
* Type		int
*
* Type		Parameter	IOGF	Description
* menu_ctx	ctx		I	rtt context.
*
* Description:
*	This function is called when a "connect" command i recieved.
*	
**************************************************************************/

static int	dtt_dualconnect_func(	edit_ctx	ctx,
					int		flag)
{
	dtt_item 	item_ptr;
	rtt_t_menu_upd	*menu_ptr;
	int		sts;
	char		message[80];

	/* Check that this is a picure ctx */
	if ( ctx->ctx_type == DTT_CTX_EDIT)
	{
	  /* Get update item */
	  sts = dtt_get_upd_item( ctx->upd_items, ctx->cursor_x, ctx->cursor_y,
		 &item_ptr);
	  if ( EVEN(sts)) 
	  {
	    rtt_message('E', "Position on an update item to connect it");
	    return RTT__NOPICTURE;
	  }
	  /* Get selected item in collection picture */
	  if ( rtt_collectionmenuctx == 0)
	  {
	    rtt_message('E', "No objects in collection picture");
	    return RTT__NOPICTURE;
	  }
	  if ( rtt_collectionmenuctx->menu == 0)
	  {
	    rtt_message('E', "No objects in collection picture");
	    return RTT__NOPICTURE;
	  }
	    
	  menu_ptr = (rtt_t_menu_upd *) rtt_collectionmenuctx->menu;
	  menu_ptr += rtt_collectionmenuctx->current_item;
	  /* Copy selected object and parameter to update item */
	  strcpy( item_ptr->dualparameter, menu_ptr->parameter_name);

	  strcpy( message, "Dualparameter connected ");
	  strcat( message, menu_ptr->parameter_name);
	  rtt_message('I', message);
	}

	return RTT__NOPICTURE;
}
/*************************************************************************
*
* Name:		help_func()
*
* Type		int
*
* Type		Parameter	IOGF	Description
* menu_ctx	ctx		I	
*
* Description:
*	This function is called when a "help" command i recieved.
*
**************************************************************************/

static int	dtt_help_func(	menu_ctx	ctx,
				int		flag)
{
	int	sts;
	char	arg1_str[80];
	char	arg2_str[80];
	char	arg3_str[80];
	char	arg4_str[80];
	char 	help_str[100] = "";

	while ( 1)
	{
	  if ( EVEN( rtt_get_qualifier( "rtt_arg1", arg1_str)))
	    break;
	  strcpy( help_str, arg1_str);
	  if ( EVEN( rtt_get_qualifier( "rtt_arg2", arg2_str)))
	    break;
	  strncat( help_str, " ", 
			sizeof(help_str) - strlen(help_str) - 1 );
	  strncat( help_str, arg2_str, 
			sizeof(help_str) - strlen(help_str) - 1 );
	  if ( EVEN( rtt_get_qualifier( "rtt_arg3", arg3_str)))
	    break;
	  strncat( help_str, " ", 
			sizeof(help_str) - strlen(help_str) - 1 );
	  strncat( help_str, arg3_str, 
			sizeof(help_str) - strlen(help_str) - 1 );
	  if ( EVEN( rtt_get_qualifier( "rtt_arg4", arg4_str)))
	    break;
	  strncat( help_str, " ", 
			sizeof(help_str) - strlen(help_str) - 1 );
	  strncat( help_str, arg4_str, 
			sizeof(help_str) - strlen(help_str) - 1 );
	}
	
	if ( help_str[0] == '\0')
	  sts = rtt_help( ctx, "HELP", rtt_edit_helptext);
	else
	{
	  sts = rtt_help( ctx, help_str, rtt_edit_helptext);
	  if ( EVEN(sts)) 
	  {
	    rtt_message('E', "No help on this subject");
	    return RTT__HOLDCOMMAND;
	  }
	}
	return RTT__SUCCESS;
}
/*************************************************************************
*
* Name:		dtt_link_func()
*
* Type		int
*
* Type		Parameter	IOGF	Description
* menu_ctx	ctx		I	rtt context.
*
* Description:
*	This function is called when a "quit" command i recieved.
*	
**************************************************************************/

static int	dtt_link_func(	edit_ctx	ctx,
				int		flag)
{
	char		arg1_str[80];
	int		debug;
	int		opsys;
	char		msg[80];

	debug = ODD( rtt_get_qualifier( "/DEBUG", arg1_str));

	if ( EVEN( rtt_get_qualifier( "rtt_arg1", arg1_str)))
	{
 	  if ( !dtt_opsys)
	  {
	    /* Link current opsys */
	    sprintf( msg, "Linking %s%s...", debug ? "with debug " : "", 
			dtt_opsys_to_name(dtt_current_opsys));
	    rtt_message('I', msg);
	    rtt_cursor_abs( 0, 21);
	    r_print_buffer();
	    dtt_cc( dtt_current_opsys, eAction_Link, debug);

	    sprintf( msg, "Program linked %s", debug ? "with debug" : "");
	    rtt_message('I',msg);
	  }
	  else
	  {
	    for ( opsys = 1; opsys < pwr_mOpSys_; opsys = opsys << 1)
	    {
	      if ( dtt_opsys & opsys)
	      {
	        sprintf( msg, "Linking %s%s...", debug ? "with debug " : "", 
				dtt_opsys_to_name(opsys));
	        rtt_message('I', msg);
	        rtt_cursor_abs( 0, 21);
	        r_print_buffer();
	        dtt_cc( opsys, eAction_Link, debug);
	      }
	    }
	    
	    sprintf( msg, "Program linked %s", debug ? "with debug" : "");
	    rtt_message('I',msg);
	  }
	}
	else 
	{
          if ( strncmp( arg1_str, "VAX_VMS", strlen( arg1_str)) == 0)
	    opsys = pwr_mOpSys_VAX_VMS;
	  else if ( strncmp( arg1_str, "VAX_ELN", strlen( arg1_str)) == 0)
	    opsys = pwr_mOpSys_VAX_ELN;
	  else if ( strncmp( arg1_str, "AXP_VMS", strlen( arg1_str)) == 0)
	    opsys = pwr_mOpSys_AXP_VMS;
	  else if ( strncmp( arg1_str, "PPC_LYNX", strlen( arg1_str)) == 0)
	    opsys = pwr_mOpSys_PPC_LYNX;
	  else if ( strncmp( arg1_str, "X86_LYNX", strlen( arg1_str)) == 0)
	    opsys = pwr_mOpSys_X86_LYNX;
	  else if ( strncmp( arg1_str, "PPC_LINUX", strlen( arg1_str)) == 0)
	    opsys = pwr_mOpSys_PPC_LINUX;
	  else if ( strncmp( arg1_str, "X86_LINUX", strlen( arg1_str)) == 0)
	    opsys = pwr_mOpSys_X86_LINUX;
	  else if ( strncmp( arg1_str, "X86_64_LINUX", strlen( arg1_str)) == 0)
	    opsys = pwr_mOpSys_X86_64_LINUX;
	  else
	  {
	    rtt_message('E', "Unknown platform");
	    return RTT__NOPICTURE;
	  }
	  if ( dtt_opsys & opsys ||
	       (!dtt_opsys && dtt_current_opsys == opsys))
	  {
	    sprintf( msg, "Linking %s%s...", debug ? "with debug " : "", 
				dtt_opsys_to_name(opsys));
	    rtt_message('I', msg);
	    rtt_cursor_abs( 0, 21);
	    r_print_buffer();
	    dtt_cc( opsys, eAction_Link, debug);

	    sprintf( msg, "Program linked for %s %s", 
			dtt_opsys_to_name(opsys), debug ? "with debug" : "");
	    rtt_message('I',msg);
	  }
	  else
	  {
	    rtt_message('E', "Platform is not configured");
	    return RTT__NOPICTURE;
	  }
	}
	
	return RTT__NOPICTURE;
}
/*************************************************************************
*
* Name:		dtt_compile_func()
*
* Type		int
*
* Type		Parameter	IOGF	Description
* menu_ctx	ctx		I	rtt context.
*
* Description:
*	This function is called when a "compile" command i recieved.
*	
**************************************************************************/

static int	dtt_compile_func(	edit_ctx	ctx,
					int		flag)
{
	char		arg1_str[80];
	int		debug;
	int		opsys;
	char		msg[80];

	debug = ODD( rtt_get_qualifier( "/DEBUG", arg1_str));

	if ( EVEN( rtt_get_qualifier( "rtt_arg1", arg1_str)))
	{
 	  if ( !dtt_opsys)
	  {
	    /* Compile current opsys */
	    sprintf( msg, "Compiling %s%s...", debug ? "with debug " : "", 
			dtt_opsys_to_name(dtt_current_opsys));
	    rtt_message('I', msg);
	    rtt_cursor_abs( 0, 21);
	    r_print_buffer();
	    dtt_cc( dtt_current_opsys, eAction_Compile, debug);

	    sprintf( msg, "Program compiled %s", debug ? "with debug" : "");
	    rtt_message('I',msg);
	  }
	  else
	  {
	    for ( opsys = 1; opsys < pwr_mOpSys_; opsys = opsys << 1)
	    {
	      if ( dtt_opsys & opsys)
	      {
	        sprintf( msg, "Compiling %s%s...", debug ? "with debug " : "", 
				dtt_opsys_to_name(opsys));
	        rtt_message('I', msg);
	        rtt_cursor_abs( 0, 21);
	        r_print_buffer();
	        dtt_cc( opsys, eAction_Compile, debug);
	      }
	    }
	    
	    sprintf( msg, "Program compiled %s", debug ? "with debug" : "");
	    rtt_message('I',msg);
	  }
	}
	else 
	{
          if ( strncmp( arg1_str, "VAX_VMS", strlen( arg1_str)) == 0)
	    opsys = pwr_mOpSys_VAX_VMS;
	  else if ( strncmp( arg1_str, "VAX_ELN", strlen( arg1_str)) == 0)
	    opsys = pwr_mOpSys_VAX_ELN;
	  else if ( strncmp( arg1_str, "AXP_VMS", strlen( arg1_str)) == 0)
	    opsys = pwr_mOpSys_AXP_VMS;
	  else if ( strncmp( arg1_str, "PPC_LYNX", strlen( arg1_str)) == 0)
	    opsys = pwr_mOpSys_PPC_LYNX;
	  else if ( strncmp( arg1_str, "X86_LYNX", strlen( arg1_str)) == 0)
	    opsys = pwr_mOpSys_X86_LYNX;
	  else if ( strncmp( arg1_str, "PPC_LINUX", strlen( arg1_str)) == 0)
	    opsys = pwr_mOpSys_PPC_LINUX;
	  else if ( strncmp( arg1_str, "X86_LINUX", strlen( arg1_str)) == 0)
	    opsys = pwr_mOpSys_X86_LINUX;
	  else if ( strncmp( arg1_str, "X86_64_LINUX", strlen( arg1_str)) == 0)
	    opsys = pwr_mOpSys_X86_64_LINUX;
	  else
	  {
	    rtt_message('E', "Unknown platform");
	    return RTT__NOPICTURE;
	  }
	  if ( dtt_opsys & opsys ||
	       (!dtt_opsys && dtt_current_opsys == opsys))
	  {
	    sprintf( msg, "Compiling %s%s...", debug ? "with debug " : "", 
				dtt_opsys_to_name(opsys));
	    rtt_message('I', msg);
	    rtt_cursor_abs( 0, 21);
	    r_print_buffer();
	    dtt_cc( opsys, eAction_Compile, debug);
	  }
	  else
	  {
	    rtt_message('E', "Platform is not configured");
	    return RTT__NOPICTURE;
	  }
	}
	return RTT__NOPICTURE;
}
/*************************************************************************
*
* Name:		dtt_edit_func()
*
* Type		int
*
* Type		Parameter	IOGF	Description
* menu_ctx	ctx		I	rtt context.
*
* Description:
*	This function is called when a "quit" command i recieved.
*	
**************************************************************************/

static int	dtt_edit_func(	edit_ctx	ctx,
				int		flag)
{
	char		arg1_str[80];
	char		filename[80];
	char		menuname[80];
	char	cmd[100];

	if ( EVEN( rtt_get_qualifier( "rtt_arg1", arg1_str)))
	{
	  dtt_get_menufilename( menuname);
	  /* Edit help is default */

	  sprintf( filename, "%s%s.rhlp", dtt_source_dir, menuname);
#if defined(OS_VMS)
	  sprintf( cmd, "spawn edit/edt %s", filename); 
#elif defined(OS_LYNX) || defined(OS_LINUX)
	  sprintf( cmd, "vi %s", filename); 
#endif
	  system( cmd);
	}
	else if ( strncmp( arg1_str, "HELP", strlen( arg1_str)) == 0)
	{
	  /* Edit the help file */
	  dtt_get_menufilename( menuname);
	  sprintf( filename, "%s%s.rhlp", dtt_source_dir, menuname);
#if defined(OS_VMS)
	  sprintf( cmd, "spawn edit/edt %s", filename); 
#elif defined(OS_LYNX) || defined(OS_LINUX)
	  sprintf( cmd, "vi %s", filename); 
#endif
	  system( cmd);
	}
	else if ( strncmp( arg1_str, "FUNCTIONS", strlen( arg1_str)) == 0)
	{
	  /* Edit the function file */
#if defined(OS_VMS)
	  sprintf( cmd, "spawn edit/edt %ra_rtt_%s.c", 
		dtt_source_dir, dtt_programname); 
#elif defined(OS_LYNX) || defined(OS_LINUX)
	  sprintf( cmd, "vi %sra_rtt_%s.c", dtt_source_dir, dtt_programname); 
#endif
	  system( cmd);
	}
	else
	{
	  /* syntax error */
	  rtt_message('E',"Syntax error");
	  return RTT__HOLDCOMMAND;
	}
	return RTT__SUCCESS;
}
/*************************************************************************
*
* Name:		dtt_run_func()
*
* Type		int
*
* Type		Parameter	IOGF	Description
* menu_ctx	ctx		I	rtt context.
*
* Description:
*	This function is called when a "quit" command i recieved.
*	
**************************************************************************/

static int	dtt_run_func(	edit_ctx	ctx,
				int		flag)
{
#if defined(OS_VMS)
	char	cmd[100];
	sprintf( cmd, "spawn run pwrp_exe:rt_rtt_%s", 
		dtt_programname); 
	system( cmd);
#endif

	return RTT__SUCCESS;
}
/*************************************************************************
*
* Name:		dtt_include_func()
*
* Type		int
*
* Type		Parameter	IOGF	Description
* menu_ctx	ctx		I	rtt context.
*
* Description:
*	This function is called when a "include" command i recieved.
*	
**************************************************************************/

static int	dtt_include_func(	menu_ctx	ctx,
					int		flag)
{
	int		sts;
	char		arg1_str[80];
	char		arg2_str[80];
	char		dum_str[80];


	if ( ODD( rtt_get_qualifier( "rtt_arg1", arg1_str)))
	{
	  if ( strncmp( arg1_str, "PICTURE", strlen( arg1_str)) == 0)
	  {
	    edit_ctx	e_ctx;

	    /* Check that this is an edit context */
	    if ( ctx->ctx_type != DTT_CTX_EDIT)
	    {
	      rtt_message('E',"Function not defined in this picture");
	      return RTT__NOPICTURE;
	    }
	    e_ctx = (edit_ctx) ctx;

	    if ( EVEN( rtt_get_qualifier( "rtt_arg2", arg2_str)))
 	    {
	      rtt_message('E',"Enter file to include");
	      return RTT__HOLDCOMMAND;
	    }
	    else
	    {
	      sts = dtt_edit_read( (edit_ctx) ctx, arg2_str, 1);
	      if ( EVEN(sts))
	      {
	        rtt_message('E', "File not found");
	        return RTT__NOPICTURE;
	      }
	    }
	  }
	  else if ( strncmp( arg1_str, "ITEMS", strlen( arg1_str)) == 0)
	  {
	    edit_ctx	e_ctx;

  	    /* Check that this is an edit context */
	    if ( ctx->ctx_type != DTT_CTX_EDIT)
	    {
	      rtt_message('E',"Function not defined in this picture");
	      return RTT__NOPICTURE;
	    }
	    e_ctx = (edit_ctx) ctx;

	    if ( EVEN( rtt_get_qualifier( "rtt_arg2", arg2_str)))
 	    {
	      rtt_message('E',"Enter file to include");
	      return RTT__HOLDCOMMAND;
	    }

	    if ( EVEN( rtt_get_qualifier( "/ADD", dum_str)))
 	    {
	      /* Check that the itemlist is cleared */
	      if ( e_ctx->upd_items)
	      {
	        rtt_message('E',"Either clear items first or use /add qualifier");
	        return RTT__HOLDCOMMAND;
	      }
	    }

	    sts = dtt_edit_read_items( (edit_ctx) ctx, arg2_str);
	    return sts;
	  }
	  else if ( strncmp( arg1_str, "MENU", strlen( arg1_str)) == 0)
	  {
	    char	filename[80];

  	    /* Check that this is not an edit context */
	    if ( ctx->ctx_type == DTT_CTX_EDIT)
	    {
	      rtt_message('E',"Position in a menue");
	      return RTT__NOPICTURE;
	    }
	    if ( EVEN( rtt_get_qualifier( "rtt_arg2", arg2_str)))
	    {
	      rtt_message('E', "Enter filname");
	      return RTT__HOLDCOMMAND;
	    }

	    /* Take the argument as filename */
	    strcpy( filename, arg2_str);

	    sts = dtt_edit_include_menue( ctx, filename);
	    if ( EVEN(sts))
	    {
	      rtt_message('E', "Unable to open file, check filename");
	      return RTT__NOPICTURE;
	    }
	    return RTT__SUCCESS;
	  }
	  else
	  {
	    rtt_message('E', "Syntax error");
	    return RTT__HOLDCOMMAND;
	  }
	}
	else
	{
	  rtt_message('E', "Syntax error");
	  return RTT__HOLDCOMMAND;
	}

	return RTT__SUCCESS;
}


/*************************************************************************
*
* Name:		dtt_delete_func()
*
* Type		int
*
* Type		Parameter	IOGF	Description
* menu_ctx	ctx		I	rtt context.
*
* Description:
*	This function is called when a "undelete" command i recieved.
*	
**************************************************************************/

static int	dtt_undo_func(	menu_ctx	ctx,
				int		flag)
{
	char		arg1_str[80];
	int		new_item;
	int		sts;

	if ( ODD( rtt_get_qualifier( "rtt_arg1", arg1_str)))
	{
	  if ( strncmp( arg1_str, "DELETE", strlen( arg1_str)) == 0)
	  {
	    if ( ctx->ctx_type != DTT_CTX_EDIT)
	    {
	      new_item = ctx->current_item + 1;
	      sts = rtt_menu_item_undelete( &ctx->menu, new_item);
	      if ( EVEN(sts))
	      {
	        rtt_message('E', "Nothing to undo");
	        return RTT__NOPICTURE;
	      }
	      rtt_menu_configure( ctx);
	      return RTT__SUCCESS;
	    }
	    else
	    {
	      rtt_message('E',"Function not defined in this picture");
	      return RTT__NOPICTURE;
	    }
	  }
	  else
	  {
	    rtt_message('E',"Syntax error");
	    return RTT__HOLDCOMMAND;
	  }
	}
	else
	{
	  rtt_message('E',"Syntax error");
	  return RTT__HOLDCOMMAND;
	}
}
	
/*************************************************************************
*
* Name:		dtt_delete_func()
*
* Type		int
*
* Type		Parameter	IOGF	Description
* menu_ctx	ctx		I	rtt context.
*
* Description:
*	This function is called when a "delete" command i recieved.
*	
**************************************************************************/

static int	dtt_delete_func(	edit_ctx	ctx,
					int		flag)
{
	int		sts;
	dtt_item	item_ptr;
	char		arg1_str[80];

	/* Check that this is an edit context */
	if ( ctx->ctx_type != DTT_CTX_EDIT)
	{
	  rtt_message('E',"Function not defined in this picture");
	  return RTT__NOPICTURE;
	}

	if ( EVEN( rtt_get_qualifier( "rtt_arg1", arg1_str)))
	{
	  if ( ctx->select_active)
	  {
	    sts = dtt_edit_delete_section( (rtt_t_backgr *) ctx->chartable, 
		ctx->upd_items, 
		ctx->cursor_x, ctx->cursor_y, ctx->select_x, ctx->select_y);
	    dtt_edit_unselect( ctx);
	    return RTT__NOPICTURE;
	  }
	  else
	  {
	    rtt_message('E',"Select an area");
	    return RTT__NOPICTURE;
	  }
	}
	else if ( strncmp( arg1_str, "ITEM", strlen( arg1_str)) == 0)
	{
	  /* Get the update item */
	  sts = dtt_get_upd_item( ctx->upd_items, ctx->cursor_x, ctx->cursor_y,
		 &item_ptr);
	  if ( EVEN(sts)) 
	  {
	    rtt_message('E', "Position on an update item to delete it");
	    return RTT__NOPICTURE;
	  }
	  sts = dtt_item_delete( ctx->upd_items, item_ptr);
	}
	else
	{
	  rtt_message('E', "Undefined qualifier");
	  return RTT__HOLDCOMMAND;
	}
	return RTT__SUCCESS;
}
/*************************************************************************
*
* Name:		dtt_paste_func()
*
* Type		int
*
* Type		Parameter	IOGF	Description
* menu_ctx	ctx		I	rtt context.
*
* Description:
*	This function is called when a "delete" command i recieved.
*	
**************************************************************************/

static int	dtt_paste_func(	edit_ctx	ctx,
				int		flag)
{
	int		sts;

	/* Check that this is an edit context */
	if ( ctx->ctx_type != DTT_CTX_EDIT)
	{
	  rtt_message('E',"Function not defined in this picture");
	  return RTT__NOPICTURE;
	}

	sts = dtt_paste_copy( (rtt_t_backgr *) ctx->chartable, 
		&(ctx->upd_items), 
		ctx->cursor_x, ctx->cursor_y);
	rtt_edit_draw_section( &(ctx->chartable), ctx->cursor_x,
		ctx->cursor_y, ctx->cursor_x + dtt_paste_size_x, 
		ctx->cursor_y + dtt_paste_size_y, 0);
	dtt_edit_draw_itemlist( ctx);	
	return RTT__NOPICTURE;
}
/*************************************************************************
*
* Name:		dtt_cut_func()
*
* Type		int
*
* Type		Parameter	IOGF	Description
* menu_ctx	ctx		I	rtt context.
*
* Description:
*	This function is called when a "cut" command i recieved.
*	
**************************************************************************/

static int	dtt_cut_func(	edit_ctx	ctx,
				int		flag)
{
	int		sts;

	/* Check that this is an edit context */
	if ( ctx->ctx_type != DTT_CTX_EDIT)
	{
	  rtt_message('E',"Function not defined in this picture");
	  return RTT__NOPICTURE;
	}

	if ( ctx->select_active)
	{
	  sts = dtt_paste_insert( (rtt_t_backgr *) ctx->chartable, 
		ctx->upd_items, 
		ctx->cursor_x, ctx->cursor_y, ctx->select_x, ctx->select_y);
	  sts = dtt_edit_delete_section( (rtt_t_backgr *) ctx->chartable, 
		ctx->upd_items,
		ctx->cursor_x, ctx->cursor_y, ctx->select_x, ctx->select_y);
	  dtt_edit_unselect( ctx);
	  return RTT__NOPICTURE;
	}
	else
	{
	  rtt_message('E',"Select an area");
	  return RTT__NOPICTURE;
	}
	return RTT__SUCCESS;
}
/*************************************************************************
*
* Name:		dtt_copy_func()
*
* Type		int
*
* Type		Parameter	IOGF	Description
* menu_ctx	ctx		I	rtt context.
*
* Description:
*	This function is called when a "copy" command i recieved.
*	
**************************************************************************/

static int	dtt_copy_func(	edit_ctx	ctx,
				int		flag)
{
	int		sts;

	/* Check that this is an edit context */
	if ( ctx->ctx_type != DTT_CTX_EDIT)
	{
	  rtt_message('E',"Function not defined in this picture");
	  return RTT__NOPICTURE;
	}

	if ( ctx->select_active)
	{
	  sts = dtt_paste_insert( (rtt_t_backgr *) ctx->chartable, 
		ctx->upd_items, 
		ctx->cursor_x, ctx->cursor_y, ctx->select_x, ctx->select_y);
	  dtt_edit_unselect( ctx);
	  return RTT__NOPICTURE;
	}
	else
	{
	  rtt_message('E',"Select an area");
	  return RTT__NOPICTURE;
	}
	return RTT__SUCCESS;
}
/*************************************************************************
*
* Name:		dtt_select_func()
*
* Type		int
*
* Type		Parameter	IOGF	Description
* menu_ctx	ctx		I	rtt context.
*
* Description:
*	This function is called when a "select" command i recieved.
*	
**************************************************************************/

static int	dtt_select_func(	edit_ctx	ctx,
					int		flag)
{
	int	active;

	/* Check that this is an edit context */
	if ( ctx->ctx_type != DTT_CTX_EDIT)
	{
	  rtt_message('E',"Function not defined in this picture");
	  return RTT__NOPICTURE;
	}
	active = ctx->select_active;
	dtt_edit_select(ctx);
	if ( active)
	  return RTT__SUCCESS;
	else
	  return RTT__NOPICTURE;
}
/*************************************************************************
*
* Name:		dtt_unselect_func()
*
* Type		int
*
* Type		Parameter	IOGF	Description
* menu_ctx	ctx		I	rtt context.
*
* Description:
*	This function is called when a "unselect" command i recieved.
*	
**************************************************************************/

static int	dtt_unselect_func(	edit_ctx	ctx,
					int		flag)
{
	/* Check that this is an edit context */
	if ( ctx->ctx_type != DTT_CTX_EDIT)
	{
	  rtt_message('E',"Function not defined in this picture");
	  return RTT__NOPICTURE;
	}
	dtt_edit_unselect(ctx);
	return RTT__SUCCESS;
}

/*************************************************************************
*
* Name:		create_func()
*
* Type		int
*
* Type		Parameter	IOGF	Description
* edit_ctx	ctx		I	rtt context.
*
* Description:
*	This function is called when a "create" command i recieved.
*	
**************************************************************************/

static int	create_func(	edit_ctx	ctx,
				int		flag)
{
	int		sts;
	char		arg1_str[80];
	char		arg2_str[80];
	char		text_str[80];
	menu_ctx	m_ctx;
	int		size;
	rtt_t_menu	*menulist = 0;
	rtt_t_menu	*parent_menu_ptr;
	rtt_t_menu	*menu_ptr;
	menu_ctx	newctx;
	char		*s;
	int		new_item;
	char		*command;
	char		*title;
	dtt_item	item_ptr;

	/* Get the text */
	if ( EVEN( rtt_get_qualifier( "/TEXT", text_str)))
	  if ( EVEN( rtt_get_qualifier( "rtt_arg1", text_str)))
	    strcpy( text_str, "%");

	/* Subtitute # to space */	    
	for ( s = text_str; *s != 0; s++)
	  if ( *s == '#') *s = ' ';

	/* Check that this is an edit context */
	if ( ctx->ctx_type == DTT_CTX_EDIT)
	{
	  /* Check that i doesn't exist */
	  sts = dtt_get_upd_item( ctx->upd_items, ctx->cursor_x, ctx->cursor_y,
		 &item_ptr);
	  if ( ODD(sts))
          {
	    rtt_message('E', "Update field already exist on this coordinate");
	    return RTT__NOPICTURE;
          }

	  if ( ODD( rtt_get_qualifier( "/TIME", arg1_str)))
	  {
	    /* Create a time upditem on current koordinates */
	    sts = dtt_upd_item_add( &(ctx->upd_items), ctx->cursor_x, 
			ctx->cursor_y, text_str);
	    if ( EVEN(sts)) return sts;
	    sts = dtt_get_upd_item( ctx->upd_items, ctx->cursor_x, ctx->cursor_y,
		 &item_ptr);
	    if ( EVEN(sts)) return sts;
	    strcpy( item_ptr->text, "%");
	    strcpy( item_ptr->priv, "NO");
	    strcpy( item_ptr->parameter, "RTT_TIME");
	    strcpy( item_ptr->database, "RTT");
	    strcpy( item_ptr->declaration, "STRING");	 
	    item_ptr->characters = 8;
	  }
	  else if ( ODD( rtt_get_qualifier( "/FULLTIME", arg1_str)))
	  {
	    /* Create a time upditem on current koordinates */
	    sts = dtt_upd_item_add( &(ctx->upd_items), ctx->cursor_x, 
			ctx->cursor_y, text_str);
	    if ( EVEN(sts)) return sts;
	    sts = dtt_get_upd_item( ctx->upd_items, ctx->cursor_x, ctx->cursor_y,
		 &item_ptr);
	    if ( EVEN(sts)) return sts;
	    strcpy( item_ptr->text, "%");
	    strcpy( item_ptr->priv, "NO");
	    strcpy( item_ptr->parameter, "RTT_TIME_FULL");
	    strcpy( item_ptr->database, "RTT");
	    strcpy( item_ptr->declaration, "STRING");	 
	    item_ptr->characters = 20;
	  }
	  else
	  {
	    /* Create a new upditem on current koordinates */
	    sts = dtt_upd_item_add( &(ctx->upd_items), ctx->cursor_x, 
			ctx->cursor_y, text_str);
	    if ( EVEN(sts)) return sts;
	  }

	  return RTT__NOBACKGROUND;
	}
	else
	{
	  /* This is a menu ctx */
	  m_ctx = (menu_ctx) ctx;
	  if ( text_str[0] == 0)
	  {
	    rtt_message('E', "Enter text");
	    return RTT__HOLDCOMMAND;
	  }

	  if ( EVEN( rtt_get_qualifier( "/CHILD", arg1_str)))
	  {
	    /* Create an menu item in the current ctx */
	    /* Get the size of the meny */
	    rtt_get_menusize( m_ctx, &size);  
	    new_item = m_ctx->current_item + 1;

	    /* Create a new entry in the root meny */
	    sts = rtt_menu_list_insert( &(m_ctx->menu), 
		new_item, text_str, 
		0, 0, 0, pwr_cNObjid, 0,0,0,0);
	    if ( EVEN(sts)) return sts;

	    /* Set type and index */
	    menu_ptr = m_ctx->menu;
	    menu_ptr += m_ctx->current_item + 1;
	    menu_ptr->index = dtt_current_index;
	    menu_ptr->arg3 = (void *) dtt_current_index;
	    menu_ptr->func2 = &dtt_menu_item_show;

	    dtt_current_index++;

	    if ( ODD( rtt_get_qualifier( "/MENU", arg1_str)))
	    {
	      menu_ptr->type = DTT_MENUITEM_MENU;
	    }
	    if ( ODD( rtt_get_qualifier( "/KEYS", arg1_str)))
	    {
	      menu_ptr->type = DTT_MENUITEM_KEYS;
	    }
	    else if ( ODD( rtt_get_qualifier( "/PICTURE", arg1_str)))
	    {
	      /* Check if function */
	      if ( ODD( rtt_get_qualifier( "/FUNCTION", arg1_str)))
	      {
	        if ( arg1_str[0] == 0)
	        {
	          rtt_message('E',"Enter function");
	          return RTT__HOLDCOMMAND;
	        }
	        menu_ptr->type = DTT_MENUITEM_PIFUNC;
	        /* save the function name string somewhere */
	        command = calloc(  80, sizeof( char));
	        strcpy( command, arg1_str);
	        menu_ptr->arg1 =  command;
	        menu_ptr->func = &dtt_edit_new;
	      }
	      else
	      {
	        menu_ptr->type = DTT_MENUITEM_EDIT;
	        menu_ptr->func = &dtt_edit_new;
	      }
	    }
	    else if ( ODD( rtt_get_qualifier( "/PERMPICTURE", arg1_str)))
	    {
	      /* Check if function */
	      if ( ODD( rtt_get_qualifier( "/FUNCTION", arg1_str)))
	      {
	        if ( arg1_str[0] == 0)
	        {
	          rtt_message('E',"Enter function");
	          return RTT__HOLDCOMMAND;
	        }
	        menu_ptr->type = DTT_MENUITEM_PIFUNCPERM;
	        /* save the function name string somewhere */
	        command = calloc(  80, sizeof( char));
	        strcpy( command, arg1_str);
	        menu_ptr->arg1 = command;
	        menu_ptr->func = &dtt_edit_new;
	      }
	      else
	      {
	        menu_ptr->type = DTT_MENUITEM_PIPERM;
	        menu_ptr->func = &dtt_edit_new;
	      }
	    }
	    else if ( ODD( rtt_get_qualifier( "/EXIT", arg1_str)))
	    {
	      menu_ptr->type = DTT_MENUITEM_EXIT;
	    }
	    else if ( ODD( rtt_get_qualifier( "/OBJECTHIERARCHY", arg1_str)))
	    {
	      menu_ptr->type = DTT_MENUITEM_OBJECTS;
	    }
	    else if ( ODD( rtt_get_qualifier( "/FILESPEC", arg1_str)))
	    {
	      if ( arg1_str[0] == 0)
	      {
	        rtt_message('E',"Enter filespec");
	        return RTT__HOLDCOMMAND;
	      }
	      if ( ODD( rtt_get_qualifier( "/COMMAND", arg2_str)))
	      {
	        if ( arg2_str[0] == 0)
	        {
	          rtt_message('E',"Enter command");
	          return RTT__HOLDCOMMAND;
	        }
	        /* save the command string somewhere */
	        command = calloc(  80, sizeof( char));
	        strcpy( command, arg2_str);
	        menu_ptr->arg2 = command;
	      }
	      else
	      {
	        rtt_message('E',"Enter command");
	        return RTT__HOLDCOMMAND;
	      }
	      menu_ptr->type = DTT_MENUITEM_FILESPEC;
	      /* save the command string somewhere */
	      command = calloc(  80, sizeof( char));
	      strcpy( command, arg1_str);
	      menu_ptr->arg1 = command;
	    }
	    else if ( ODD( rtt_get_qualifier( "/COMMAND", arg1_str)))
	    {
	      if ( arg1_str[0] == 0)
	      {
	        rtt_message('E',"Enter command");
	        return RTT__HOLDCOMMAND;
	      }
	      menu_ptr->type = DTT_MENUITEM_COMMAND;
	      /* save the command string somewhere */
	      command = calloc(  80, sizeof( char));
	      strcpy( command, arg1_str);
	      menu_ptr->arg1 = command;
	    }
	    else if ( ODD( rtt_get_qualifier( "/COMMANDHOLD", arg1_str)))
	    {
	      if ( arg1_str[0] == 0)
	      {
	        rtt_message('E',"Enter command");
	        return RTT__HOLDCOMMAND;
	      }
	      menu_ptr->type = DTT_MENUITEM_COMMANDHOLD;
	      /* save the command string somewhere */
	      command = calloc(  80, sizeof( char));
	      strcpy( command, arg1_str);
	      menu_ptr->arg1 = command;
	    }
	    else if ( ODD( rtt_get_qualifier( "/VMSCOMMAND", arg1_str)))
	    {
	      if ( arg1_str[0] == 0)
	      {
	        rtt_message('E',"Enter vms command");
	        return RTT__HOLDCOMMAND;
	      }
	      menu_ptr->type = DTT_MENUITEM_VMSCOMMAND;
	      /* save the command string somewhere */
	      command = calloc(  80, sizeof( char));
	      strcpy( command, arg1_str);
	      menu_ptr->arg1 = command;
	    }
	    else if ( ODD( rtt_get_qualifier( "/VMSNOWAIT", arg1_str)))
	    {
	      if ( arg1_str[0] == 0)
	      {
	        rtt_message('E',"Enter vms command");
	        return RTT__HOLDCOMMAND;
	      }
	      menu_ptr->type = DTT_MENUITEM_VMSCOMMANDNOWAIT;
	      /* save the command string somewhere */
	      command = calloc(  80, sizeof( char));
	      strcpy( command, arg1_str);
	      menu_ptr->arg1 = command;
	    }
	    else if ( ODD( rtt_get_qualifier( "/VMSCONFIRM", arg1_str)))
	    {
	      if ( arg1_str[0] == 0)
	      {
	        rtt_message('E',"Enter vms command");
	        return RTT__HOLDCOMMAND;
	      }
	      menu_ptr->type = DTT_MENUITEM_VMSCOMMANDCONF;
	      /* save the command string somewhere */
	      command = calloc(  80, sizeof( char));
	      strcpy( command, arg1_str);
	      menu_ptr->arg1 = command;
	    }
	    else if ( ODD( rtt_get_qualifier( "/VMSHOLD", arg1_str)))
	    {
	      if ( arg1_str[0] == 0)
	      {
	        rtt_message('E',"Enter vms command");
	        return RTT__HOLDCOMMAND;
	      }
	      menu_ptr->type = DTT_MENUITEM_VMSCOMMANDHOLD;
	      /* save the command string somewhere */
	      command = calloc(  80, sizeof( char));
	      strcpy( command, arg1_str);
	      menu_ptr->arg1 = command;
	    }
	    else if ( ODD( rtt_get_qualifier( "/SYSPICTURE", arg1_str)))
	    {
	      if ( arg1_str[0] == 0)
	      {
	        rtt_message('E',"Enter syspicture");
	        return RTT__HOLDCOMMAND;
	      }
	      menu_ptr->type = DTT_MENUITEM_SYSPICTURE;
	      /* save the command string somewhere */
	      command = calloc(  80, sizeof( char));
	      strcpy( command, arg1_str);
	      menu_ptr->arg1 = command;
	    }
	    else if ( ODD( rtt_get_qualifier( "/OBJPICTURE", arg1_str)))
	    {
	      if ( arg1_str[0] == 0)
	      {
	        rtt_message('E',"Enter objpicture");
	        return RTT__HOLDCOMMAND;
	      }
	      if ( ODD( rtt_get_qualifier( "/NAME", arg2_str)))
	      {
	        if ( arg2_str[0] == 0)
	        {
	          rtt_message('E',"Enter object name");
	          return RTT__HOLDCOMMAND;
	        }
	        /* save the command string somewhere */
	        command = calloc(  80, sizeof( char));
	        strcpy( command, arg2_str);
	        menu_ptr->arg2 = command;
	      }
	      else
	      {
	        rtt_message('E',"Enter object name");
	        return RTT__HOLDCOMMAND;
	      }
	      menu_ptr->type = DTT_MENUITEM_OBJPICTURE;
	      /* save the command string somewhere */
	      command = calloc(  80, sizeof( char));
	      strcpy( command, arg1_str);
	      menu_ptr->arg1 = command;
	    }
	    else
	    {
	      /* Menu is default */
	      menu_ptr->type = DTT_MENUITEM_MENU;
	    }	    

	    /* Reconfigure */
	    rtt_menu_configure( m_ctx);
	    /* Select the new entry */
	    m_ctx->current_item = new_item;

	    return RTT__SUCCESS;
	  }
	  else
	  {
	    /* Create a child context with one item to this item */
	    parent_menu_ptr = m_ctx->menu;
	    parent_menu_ptr += m_ctx->current_item;

	    /* Check that parent is a menuitem */
	    if ( !( parent_menu_ptr->type == DTT_MENUITEM_MENU ||
	            parent_menu_ptr->type == DTT_MENUITEM_KEYS))
	    {
	      rtt_message('E', "This item is not of menutype");
	      return RTT__NOPICTURE;
	    }

	    /* Look for a context */
	    sts = dtt_get_stored_menuctx( &newctx, parent_menu_ptr->index);
	    if ( EVEN(sts))
	    {
	      /* Create a new menu */
	      sts = rtt_menu_list_add( &menulist, 0, 0, text_str, 
			0, 0, 0, pwr_cNObjid, 0,0,0,0);
	      if ( EVEN(sts)) return sts;

	      /* Set type and index */
	      menu_ptr = menulist;
	      menu_ptr->index = dtt_current_index;
	      menu_ptr->arg3 = (void *) dtt_current_index;
	      menu_ptr->func2 = &dtt_menu_item_show;
	      dtt_current_index++;

	      if ( ODD( rtt_get_qualifier( "/MENU", arg1_str)))
	      {
	        menu_ptr->type = DTT_MENUITEM_MENU;
	      }
	      else if ( ODD( rtt_get_qualifier( "/PICTURE", arg1_str)))
	      {
	        /* Check if function */
	        if ( ODD( rtt_get_qualifier( "/FUNCTION", arg1_str)))
	        {
	          if ( arg1_str[0] == 0)
	          {
	            rtt_message('E',"Enter function");
	            return RTT__HOLDCOMMAND;
	          }
	          menu_ptr->type = DTT_MENUITEM_PIFUNC;
	          /* save the function name string somewhere */
	          command = calloc(  80, sizeof( char));
	          strcpy( command, arg1_str);
	          menu_ptr->arg1 = command;
	          menu_ptr->func = &dtt_edit_new;
	        }
	        else
	        {
	          menu_ptr->type = DTT_MENUITEM_EDIT;
	          menu_ptr->func = &dtt_edit_new;
	        }
	      }
	      else if ( ODD( rtt_get_qualifier( "/PERMPICTURE", arg1_str)))
	      {
	        /* Check if function */
	        if ( ODD( rtt_get_qualifier( "/FUNCTION", arg1_str)))
	        {
	          if ( arg1_str[0] == 0)
	          {
	            rtt_message('E',"Enter function");
	            return RTT__HOLDCOMMAND;
	          }
	          menu_ptr->type = DTT_MENUITEM_PIFUNCPERM;
	          /* save the function name string somewhere */
	          command = calloc(  80, sizeof( char));
	          strcpy( command, arg1_str);
	          menu_ptr->arg1 = command;
	          menu_ptr->func = &dtt_edit_new;
	        }
	        else
	        {
	          menu_ptr->type = DTT_MENUITEM_PIPERM;
	          menu_ptr->func = &dtt_edit_new;
	        }
	      }
	      else if ( ODD( rtt_get_qualifier( "/EXIT", arg1_str)))
	      {
	        menu_ptr->type = DTT_MENUITEM_EXIT;
	      }
	      else if ( ODD( rtt_get_qualifier( "/OBJECTHIERARCHY", arg1_str)))
	      {
	        menu_ptr->type = DTT_MENUITEM_OBJECTS;
	      }
	      else if ( ODD( rtt_get_qualifier( "/FILESPEC", arg1_str)))
	      {
	        if ( arg1_str[0] == 0)
	        {
	          rtt_message('E',"Enter filespec");
	          return RTT__HOLDCOMMAND;
	        }
	        if ( ODD( rtt_get_qualifier( "/COMMAND", arg2_str)))
	        {
	          if ( arg2_str[0] == 0)
	          {
	            rtt_message('E',"Enter command");
	            return RTT__HOLDCOMMAND;
	          }
	          /* save the command string somewhere */
	          command = calloc(  80, sizeof( char));
	          strcpy( command, arg2_str);
	          menu_ptr->arg2 = command;
	        }
	        else
	        {
	          rtt_message('E',"Enter command");
	          return RTT__HOLDCOMMAND;
	        }
	        menu_ptr->type = DTT_MENUITEM_FILESPEC;
	        /* save the command string somewhere */
	        command = calloc(  80, sizeof( char));
	        strcpy( command, arg1_str);
	        menu_ptr->arg1 = command;
	      }
	      else if ( ODD( rtt_get_qualifier( "/COMMAND", arg1_str)))
	      {
	        if ( arg1_str[0] == 0)
	        {
	          rtt_message('E',"Enter command");
	          return RTT__HOLDCOMMAND;
	        }
	        menu_ptr->type = DTT_MENUITEM_COMMAND;
	        /* save the command string somewhere */
	        command = calloc(  80, sizeof( char));
	        strcpy( command, arg1_str);
	        menu_ptr->arg1 = command;
	      }
	      else if ( ODD( rtt_get_qualifier( "/COMMANDHOLD", arg1_str)))
	      {
	        if ( arg1_str[0] == 0)
	        {
	          rtt_message('E',"Enter command");
	          return RTT__HOLDCOMMAND;
	        }
	        menu_ptr->type = DTT_MENUITEM_COMMANDHOLD;
	        /* save the command string somewhere */
	        command = calloc(  80, sizeof( char));
	        strcpy( command, arg1_str);
	        menu_ptr->arg1 = command;
	      }
	      else if ( ODD( rtt_get_qualifier( "/VMSCOMMAND", arg1_str)))
	      {
	        if ( arg1_str[0] == 0)
	        {
	          rtt_message('E',"Enter vms command");
	          return RTT__HOLDCOMMAND;
	        }
	        menu_ptr->type = DTT_MENUITEM_VMSCOMMAND;
	        /* save the command string somewhere */
	        command = calloc(  80, sizeof( char));
	        strcpy( command, arg1_str);
	        menu_ptr->arg1 = command;
	      }
	      else if ( ODD( rtt_get_qualifier( "/VMSNOWAIT", arg1_str)))
	      {
	        if ( arg1_str[0] == 0)
	        {
	          rtt_message('E',"Enter vms command");
	          return RTT__HOLDCOMMAND;
	        }
	        menu_ptr->type = DTT_MENUITEM_VMSCOMMANDNOWAIT;
	        /* save the command string somewhere */
	        command = calloc(  80, sizeof( char));
	        strcpy( command, arg1_str);
	        menu_ptr->arg1 = command;
	      }
	      else if ( ODD( rtt_get_qualifier( "/VMSCONFIRM", arg1_str)))
	      {
	        if ( arg1_str[0] == 0)
	        {
	          rtt_message('E',"Enter vms command");
	          return RTT__HOLDCOMMAND;
	        }
	        menu_ptr->type = DTT_MENUITEM_VMSCOMMANDCONF;
	        /* save the command string somewhere */
	        command = calloc(  80, sizeof( char));
	        strcpy( command, arg1_str);
	        menu_ptr->arg1 = command;
	      }
	      else if ( ODD( rtt_get_qualifier( "/VMSHOLD", arg1_str)))
	      {
	        if ( arg1_str[0] == 0)
	        {
	          rtt_message('E',"Enter vms command");
	          return RTT__HOLDCOMMAND;
	        }
	        menu_ptr->type = DTT_MENUITEM_VMSCOMMANDHOLD;
	        /* save the command string somewhere */
	        command = calloc(  80, sizeof( char));
	        strcpy( command, arg1_str);
	        menu_ptr->arg1 = command;
	      }
	      else if ( ODD( rtt_get_qualifier( "/SYSPICTURE", arg1_str)))
	      {
	        if ( arg1_str[0] == 0)
	        {
	          rtt_message('E',"Enter syspicture");
	          return RTT__HOLDCOMMAND;
	        }
	        menu_ptr->type = DTT_MENUITEM_SYSPICTURE;
	        /* save the command string somewhere */
	        command = calloc(  80, sizeof( char));
	        strcpy( command, arg1_str);
	        menu_ptr->arg1 = command;
	      }
	      else if ( ODD( rtt_get_qualifier( "/OBJPICTURE", arg1_str)))
	      {
	        if ( arg1_str[0] == 0)
	        {
	          rtt_message('E',"Enter objpicture");
	          return RTT__HOLDCOMMAND;
	        }
	        if ( ODD( rtt_get_qualifier( "/NAME", arg2_str)))
	        {
	          if ( arg2_str[0] == 0)
	          {
	            rtt_message('E',"Enter object name");
	            return RTT__HOLDCOMMAND;
	          }
	          /* save the command string somewhere */
	          command = calloc(  80, sizeof( char));
	          strcpy( command, arg2_str);
	          menu_ptr->arg2 = command;
	        }
	        else
	        {
	          rtt_message('E',"Enter object name");
	          return RTT__HOLDCOMMAND;
	        }
	        menu_ptr->type = DTT_MENUITEM_OBJPICTURE;
	        /* save the command string somewhere */
	        command = calloc(  80, sizeof( char));
	        strcpy( command, arg1_str);
	        menu_ptr->arg1 = command;
	      }
	      else
	      {
	        /* Menu is default */
	        menu_ptr->type = DTT_MENUITEM_MENU;
	      }	    
	      /* Create a context */
	      sts = rtt_menu_create_ctx( &newctx, m_ctx, menulist,
			parent_menu_ptr->text,
			RTT_MENUTYPE_MENU | RTT_MENUTYPE_DYN);
	      if ( EVEN(sts)) return sts;
	      /* Configure */
	      rtt_menu_configure( newctx);

	      /* Store the context */
	      dtt_store_menuctx( newctx, parent_menu_ptr->index);

	      /* Set function in parent item */
	      title = calloc(  80, sizeof( char));
	      strcpy( title, parent_menu_ptr->text);
	      parent_menu_ptr->arg1 = title;
	      parent_menu_ptr->func = &dtt_menu_new;
	    }
	    else
	    {
	      /* The context exists, insert a new item */
	      rtt_get_menusize( newctx, &size);
	      sts = rtt_menu_list_add( &(newctx->menu), size, 0, text_str, 
			0, 0, 0, pwr_cNObjid, 0,0,0,0);
	      if ( EVEN(sts)) return sts;

	      /* Set type and index */
	      menu_ptr = newctx->menu;
	      menu_ptr += size;
	      menu_ptr->index = dtt_current_index;
	      menu_ptr->arg3 = (void *) dtt_current_index;
	      menu_ptr->func2 = &dtt_menu_item_show;
	      dtt_current_index++;
	      if ( ODD( rtt_get_qualifier( "/MENU", arg1_str)))
	      {
	        menu_ptr->type = DTT_MENUITEM_MENU;
	      }
	      else if ( ODD( rtt_get_qualifier( "/PICTURE", arg1_str)))
	      {
	        menu_ptr->type = DTT_MENUITEM_EDIT;
	        menu_ptr->func = &dtt_edit_new;
	      }
	      else if ( ODD( rtt_get_qualifier( "/EXIT", arg1_str)))
	      {
	        menu_ptr->type = DTT_MENUITEM_EXIT;
	      }
	      else if ( ODD( rtt_get_qualifier( "/OBJECTHIERARCHY", arg1_str)))
	      {
	        menu_ptr->type = DTT_MENUITEM_OBJECTS;
	      }
	    }
	    return RTT__NOPICTURE;
	  }
	  return RTT__NOPICTURE;
	}
	return RTT__SUCCESS;
}


/*************************************************************************
*
* Name:		modify_func()
*
* Type		int
*
* Type		Parameter	IOGF	Description
* edit_ctx	ctx		I	rtt context.
*
* Description:
*	This function is called when a "create" command i recieved.
*	
**************************************************************************/

static int	modify_func(	edit_ctx	ctx,
				int		flag)
{
	int		sts;
	char		arg1_str[80];
	menu_ctx 	m_ctx;
	menu_ctx 	parent_ctx;
	menu_ctx 	newctx;
	dtt_item	item_ptr;
	int		number;
	int		nr;
	rtt_t_menu	*menu_ptr;
	rtt_t_menu	*parent_menu_ptr;
	char		message[80] = "";
	int		change = 0;
	char		*s;
	float		some_float;

	if ( ODD( rtt_get_qualifier( "/TITLEPREFIX", arg1_str)))
	{
	  if ( arg1_str[0] == 0)
	  {
	    rtt_message('E', "Enter title prefix");
	    return RTT__HOLDCOMMAND;
	  }
	  strcpy( dtt_title_prefix, arg1_str);
	  strcpy( rtt_title_prefix, dtt_title_prefix);
	  return RTT__SUCCESS;
	}

	if ( ODD( rtt_get_qualifier( "/MAINTITLE", arg1_str)))
	{
	  if ( arg1_str[0] == 0)
	  {
	    rtt_message('E', "Enter maintitle");
	    return RTT__HOLDCOMMAND;
	  }
	
	  /* Subtitute # to space */	    
	  for ( s = arg1_str; *s != 0; s++)
	    if ( *s == '#') *s = ' ';

	  strcpy( dtt_maintitle, arg1_str);
	  if ( (s = strchr( dtt_maintitle, '/')))
	    *s = 0;
	  /* Change the title in the root context */
	  strcpy( dtt_root_ctx->title, arg1_str);
	  return RTT__SUCCESS;
	}

	/* Check that this is an edit context */
	if ( ctx->ctx_type == DTT_CTX_EDIT)
	{
	  /* Edit context */
	  /* Get the update item */
	  sts = dtt_get_upd_item( ctx->upd_items, ctx->cursor_x, ctx->cursor_y,
		 &item_ptr);
	  if ( EVEN(sts)) 
	  {
	    rtt_message('E', "Position on an update item");
	    return RTT__NOPICTURE;
	  }
	  if ( ODD( rtt_get_qualifier( "/NUMBER", arg1_str)))
	  {
	    /* Convert to number */
	    nr = sscanf( arg1_str, "%d", &number);
	    if ( (nr != 1) || ( number < 1))
	    {
	      rtt_message('E', "Syntax error in number");
	      return RTT__HOLDCOMMAND;
	    }
	    item_ptr->number = number;
	    change += 10;
	    strcat( message, "Number ");
	  }
	  if ( ODD( rtt_get_qualifier( "/TEXT", arg1_str)))
	  {
	    /* Subtitute # to space */	    
	    for ( s = arg1_str; *s != 0; s++)
	      if ( *s == '#') *s = ' ';
	    strcpy( item_ptr->text, arg1_str);	 
	    change += 10;
	    strcat( message, "Text ");
	  }
	  if ( ODD( rtt_get_qualifier( "/TYPE", arg1_str)))
	  {
	    if ( (strcmp( arg1_str, "UPDATE") == 0) ||
		(strcmp( arg1_str, "SET") == 0) ||
		(strcmp( arg1_str, "RESET") == 0) ||
		(strcmp( arg1_str, "SET_RESET") == 0) ||
		(strcmp( arg1_str, "TOGGLE") == 0) ||
		(strcmp( arg1_str, "DUAL_SET") == 0) ||
		(strcmp( arg1_str, "DUAL_RESET") == 0) ||
		(strcmp( arg1_str, "DUAL_SET_RESET") == 0) ||
		(strcmp( arg1_str, "DUAL_TOGGLE") == 0) ||
		(strcmp( arg1_str, "SET_DUALSET") == 0) ||
		(strcmp( arg1_str, "TOGGLE_DUALTOGGLE") == 0) ||
		(strcmp( arg1_str, "COMMAND") == 0))
	    {
	      strcpy( item_ptr->type, arg1_str);	 
	      change++;
	      strcat( message, "Type ");
	    }
	    else
	    {
	      rtt_message('E', "Syntax error");
	      return RTT__HOLDCOMMAND;
	    }
	  }
	  if ( ODD( rtt_get_qualifier( "/PARAMETER", arg1_str)))
	  {
	    strcpy( item_ptr->parameter, arg1_str);	 
	    change++;
	    strcat( message, "Parameter ");
	  }
	  if ( ODD( rtt_get_qualifier( "/DUALPARAMETER", arg1_str)))
	  {
	    strcpy( item_ptr->dualparameter, arg1_str);	 
	    change++;
	    strcat( message, "Dualparameter ");
	  }
	  if ( ODD( rtt_get_qualifier( "/PRIVILEGES", arg1_str)))
	  {
	    if ( (strcmp( arg1_str, "OP") == 0) ||
		(strcmp( arg1_str, "NO") == 0) ||
		(strcmp( arg1_str, "EL") == 0) ||
		(strcmp( arg1_str, "PROC") == 0) ||
		(strcmp( arg1_str, "SYS") == 0) ||
		(strcmp( arg1_str, "NOOP") == 0))
	    {
	      strcpy( item_ptr->priv, arg1_str);	 
	      change++;
	      strcat( message, "Priv ");
	    }
	    else
	    {
	      rtt_message('E', "Syntax error");
	      return RTT__HOLDCOMMAND;
	    }
	  }
	  if ( ODD( rtt_get_qualifier( "/OUTPUTFLAGS", arg1_str)))
	  {
	    if ( (strcmp( arg1_str, "ONOFF") == 0) ||
		(strcmp( arg1_str, "NO") == 0) ||
		(strcmp( arg1_str, "TRUEFALSE") == 0) ||
		(strcmp( arg1_str, "AUTOMAN") == 0) ||
		(strcmp( arg1_str, "") == 0) ||
		(strcmp( arg1_str, "BAR") == 0) ||
		(strcmp( arg1_str, "TEXT") == 0) ||
                (strcmp( arg1_str, "FLASHTEXT") == 0) ||
                (strcmp( arg1_str, "FLASH") == 0) ||
		(strcmp( arg1_str, "OPENCLOSED") == 0))
	    {
	      strcpy( item_ptr->outflags, arg1_str);	 
	      change++;
	      strcat( message, "Outflags ");
	    }
	    else
	    {
	      rtt_message('E', "Syntax error");
	      return RTT__HOLDCOMMAND;
	    }
	  }
	  if ( ODD( rtt_get_qualifier( "/CHARACTERS", arg1_str)))
	  {	  
	    /* Convert to number */
	    nr = sscanf( arg1_str, "%d", &number);
	    if ( (nr != 1) || ( number < 0))
	    {
	      rtt_message('E', "Syntax error in characters");
	      return RTT__HOLDCOMMAND;
	    }
	    item_ptr->characters = number;
	    change++;
	    strcat( message, "Characters ");
	  }
	  if ( ODD( rtt_get_qualifier( "/DECIMALS", arg1_str)))
	  {	  
	    /* Convert to number */
	    nr = sscanf( arg1_str, "%d", &number);
	    if ( (nr != 1) || ( number < 0))
	    {
	      rtt_message('E', "Syntax error in decimals");
	      return RTT__HOLDCOMMAND;
	    }
	    item_ptr->decimals = number;
	    change++;
	    strcat( message, "Decimals ");
	  }
	  if ( ODD( rtt_get_qualifier( "/MAXLIMIT", arg1_str)))
	  {	  
	    /* Convert to float */
	    nr = sscanf( arg1_str, "%f", &some_float);
	    if (nr != 1)
	    {
	      rtt_message('E', "Syntax error in maxlimit");
	      return RTT__HOLDCOMMAND;
	    }
	    item_ptr->maxlimit = some_float;
	    change++;
	    strcat( message, "Maxlimit ");
	  }
	  if ( ODD( rtt_get_qualifier( "/MINLIMIT", arg1_str)))
	  {	  
	    /* Convert to float */
	    nr = sscanf( arg1_str, "%f", &some_float);
	    if (nr != 1)
	    {
	      rtt_message('E', "Syntax error in minlimit");
	      return RTT__HOLDCOMMAND;
	    }
	    item_ptr->minlimit = some_float;
	    change++;
	    strcat( message, "Minlimit ");
	  }
	  if ( ODD( rtt_get_qualifier( "/DATABASE", arg1_str)))
	  {
	    if ( (strcmp( arg1_str, "GDH") == 0) ||
		(strcmp( arg1_str, "RTT") == 0) ||
		(strcmp( arg1_str, "RTTSYS") == 0) ||
		(strcmp( arg1_str, "USER") == 0))
	    {
	      strcpy( item_ptr->database, arg1_str);	 
	      change++;
	      strcat( message, "Database ");
	    }
	    else
	    {
	      rtt_message('E', "Syntax error");
	      return RTT__HOLDCOMMAND;
	    }
	  }
	  if ( ODD( rtt_get_qualifier( "/DECLARATION", arg1_str)))
	  {
	    if ( (strcmp( arg1_str, "INT") == 0) ||
		(strcmp( arg1_str, "SHORT") == 0) ||
		(strcmp( arg1_str, "BOOLEAN") == 0) ||
		(strcmp( arg1_str, "FLOAT") == 0) ||
		(strcmp( arg1_str, "STRING") == 0) ||
		(strcmp( arg1_str, "STRING4") == 0) ||
		(strcmp( arg1_str, "STRING10") == 0) ||
		(strcmp( arg1_str, "STRING20") == 0) ||
		(strcmp( arg1_str, "STRING40") == 0) ||
		(strcmp( arg1_str, "OBJID") == 0) ||
		(strcmp( arg1_str, "ATTRREF") == 0) ||
		(strcmp( arg1_str, "TIME") == 0) ||
		(strcmp( arg1_str, "CHAR") == 0))
	    {
	      strcpy( item_ptr->declaration, arg1_str);	 
	      change++;
	      strcat( message, "Decl ");
	    }
	    else
	    {
	      rtt_message('E', "Syntax error");
	      return RTT__HOLDCOMMAND;
	    }
	  }
	  if ( change > 0)
	    strcat( message, "modified");
	  else
	    strcat( message, "Nothing modified");
	  rtt_message('I', message);
	  if ( change >= 10 )
	    return RTT__NOBACKGROUND;
	  else
	    return RTT__NOPICTURE;

	}
	else if ( (ctx->parent_ctx != 0) && 
		(((edit_ctx)(ctx->parent_ctx))->ctx_type == DTT_CTX_EDIT))
	{
	  /* This is the update item picture */

	  m_ctx = (menu_ctx) ctx;
	  menu_ptr = m_ctx->menu;
	  menu_ptr += m_ctx->current_item;
	  item_ptr = (dtt_item) menu_ptr->arg1;

	  if ( m_ctx->current_item == 0)
	  {
	    /* Change the number */
	    if ( EVEN( rtt_get_qualifier( "rtt_arg1", arg1_str)))
	    {
	      if ( EVEN( rtt_get_qualifier( "/NUMBER", arg1_str)))
	      {
	        rtt_message('E',"Enter number");
	        return RTT__NOPICTURE;
	      }  
	    }	  
	    /* Convert to number */
	    nr = sscanf( arg1_str, "%d", &number);
	    if ( (nr != 1) || ( number < 1))
	    {
	      rtt_message('E', "Syntax error in number");
	      return RTT__HOLDCOMMAND;
	    }

	    item_ptr->number = number;
	    sprintf( menu_ptr->text, 
		"Number:        %d", number);
	    return RTT__SUCCESS;
	  }
	  else if ( m_ctx->current_item == 1)
	  {
	    /* Change the text */
	    if ( EVEN( rtt_get_qualifier( "rtt_arg1", arg1_str)))
	    {
	      if ( EVEN( rtt_get_qualifier( "/TEXT", arg1_str)))
	      {
	        rtt_message('E',"Enter text");
	        return RTT__NOPICTURE;
	      }  
	    }	  
	    /* Subtitute # to space */	    
	    for ( s = arg1_str; *s != 0; s++)
	      if ( *s == '#') *s = ' ';
	    strcpy( item_ptr->text, arg1_str);	 
	    sprintf( menu_ptr->text, 
		"Text:          %s", arg1_str);
	    return RTT__SUCCESS;
	  }
	  else if ( m_ctx->current_item == 2)
	  {
	    /* Change the type */
	    if ( EVEN( rtt_get_qualifier( "rtt_arg1", arg1_str)))
	    {
	      if ( EVEN( rtt_get_qualifier( "/TYPE", arg1_str)))
	      {
	        rtt_message('E',"Enter type");
	        return RTT__NOPICTURE;
	      }  
	    }	  
	    if ( (strcmp( arg1_str, "UPDATE") == 0) ||
		(strcmp( arg1_str, "SET") == 0) ||
		(strcmp( arg1_str, "RESET") == 0) ||
		(strcmp( arg1_str, "SET_RESET") == 0) ||
		(strcmp( arg1_str, "TOGGLE") == 0) ||
		(strcmp( arg1_str, "DUAL_SET") == 0) ||
		(strcmp( arg1_str, "DUAL_RESET") == 0) ||
		(strcmp( arg1_str, "DUAL_SET_RESET") == 0) ||
		(strcmp( arg1_str, "DUAL_TOGGLE") == 0) ||
		(strcmp( arg1_str, "SET_DUALSET") == 0) ||
		(strcmp( arg1_str, "TOGGLE_DUALTOGGLE") == 0) ||
		(strcmp( arg1_str, "COMMAND") == 0))
	    {
	      strcpy( item_ptr->type, arg1_str);	 
	      sprintf( menu_ptr->text, 
		"Type:          %s", arg1_str);
	      return RTT__SUCCESS;
	    }
	    else
	    {
	      rtt_message('E', "Syntax error");
	      return RTT__HOLDCOMMAND;
	    }
	  }
	  else if ( m_ctx->current_item == 3)
	  {
	    /* Change the parameter */
	    if ( EVEN( rtt_get_qualifier( "rtt_arg1", arg1_str)))
	    {
	      if ( EVEN( rtt_get_qualifier( "/PARAMETER", arg1_str)))
	      {
	        rtt_message('E',"Enter parameter");
	        return RTT__NOPICTURE;
	      }  
	    }	  
	    strcpy( item_ptr->parameter, arg1_str);	 
	    sprintf( menu_ptr->text, 
		"Parameter      %s", arg1_str);
	    return RTT__SUCCESS;
	  }
	  else if ( m_ctx->current_item == 4)
	  {
	    /* Change the Dualparameter */
	    if ( EVEN( rtt_get_qualifier( "rtt_arg1", arg1_str)))
	    {
	      if ( EVEN( rtt_get_qualifier( "/DUALPARAMETER", arg1_str)))
	      {
	        rtt_message('E',"Enter dualparameter");
	        return RTT__NOPICTURE;
	      }  
	    }	  
	    strcpy( item_ptr->dualparameter, arg1_str);	 
	    sprintf( menu_ptr->text, 
		"Dualparameter: %s", arg1_str);
	    return RTT__SUCCESS;
	  }
	  else if ( m_ctx->current_item == 5)
	  {
	    /* Change the privs */
	    if ( EVEN( rtt_get_qualifier( "rtt_arg1", arg1_str)))
	    {
	      if ( EVEN( rtt_get_qualifier( "/PRIVILEGES", arg1_str)))
	      {
	        rtt_message('E',"Enter privileges");
	        return RTT__NOPICTURE;
	      }  
	    }	  
	    if ( (strcmp( arg1_str, "OP") == 0) ||
		(strcmp( arg1_str, "NO") == 0) ||
		(strcmp( arg1_str, "EL") == 0) ||
		(strcmp( arg1_str, "PROC") == 0) ||
		(strcmp( arg1_str, "SYS") == 0) ||
		(strcmp( arg1_str, "NOOP") == 0))
	    {
	      strcpy( item_ptr->priv, arg1_str);	 
	      sprintf( menu_ptr->text, 
		"Privileges:    %s", arg1_str);
	      return RTT__SUCCESS;
	    }
	    else
	    {
	      rtt_message('E', "Syntax error");
	      return RTT__HOLDCOMMAND;
	    }
	  }
	  else if ( m_ctx->current_item == 6)
	  {
	    /* Change the privs */
	    if ( EVEN( rtt_get_qualifier( "rtt_arg1", arg1_str)))
	    {
	      if ( EVEN( rtt_get_qualifier( "/OUTPUTFLAGS", arg1_str)))
	      {
	        rtt_message('E',"Enter outputflags");
	        return RTT__NOPICTURE;
	      }  
	    }	  
	    if ( (strcmp( arg1_str, "ONOFF") == 0) ||
		(strcmp( arg1_str, "NO") == 0) ||
		(strcmp( arg1_str, "TRUEFALSE") == 0) ||
		(strcmp( arg1_str, "AUTOMAN") == 0) ||
		(strcmp( arg1_str, "") == 0) ||
		(strcmp( arg1_str, "BAR") == 0) ||
		(strcmp( arg1_str, "TEXT") == 0) ||
                (strcmp( arg1_str, "FLASHTEXT") == 0) ||
                (strcmp( arg1_str, "FLASH") == 0) ||
		(strcmp( arg1_str, "OPENCLOSED") == 0))
	    {
	      strcpy( item_ptr->outflags, arg1_str);	 
	      sprintf( menu_ptr->text,  
		"Outputflags:   %s", arg1_str);
  	      return RTT__SUCCESS;
	    }
	    else
	    {
	      rtt_message('E', "Syntax error");
	      return RTT__HOLDCOMMAND;
	    }
	  }
	  if ( m_ctx->current_item == 7)
	  {
	    /* Change the number */
	    if ( EVEN( rtt_get_qualifier( "rtt_arg1", arg1_str)))
	    {
	      if ( EVEN( rtt_get_qualifier( "/CHARACTERS", arg1_str)))
	      {
	        rtt_message('E',"Enter number of characters");
	        return RTT__NOPICTURE;
	      }  
	    }	  
	    /* Convert to number */
	    nr = sscanf( arg1_str, "%d", &number);
	    if ( (nr != 1) || ( number < 0))
	    {
	      rtt_message('E', "Syntax error in characters");
	      return RTT__HOLDCOMMAND;
	    }

	    item_ptr->characters = number;
	    sprintf( menu_ptr->text, "Characters:    %d", number);
	    return RTT__SUCCESS;
	  }
	  if ( m_ctx->current_item == 8)
	  {
	    /* Change the decimals */
	    if ( EVEN( rtt_get_qualifier( "rtt_arg1", arg1_str)))
	    {
	      if ( EVEN( rtt_get_qualifier( "/DECIMALS", arg1_str)))
	      {
	        rtt_message('E',"Enter number of decimals");
	        return RTT__NOPICTURE;
	      }  
	    }	  
	    /* Convert to number */
	    nr = sscanf( arg1_str, "%d", &number);
	    if ( (nr != 1) || ( number < 0))
	    {
	      rtt_message('E', "Syntax error in decimals");
	      return RTT__HOLDCOMMAND;
	    }

	    item_ptr->decimals = number;
	    sprintf( menu_ptr->text, 
		"Decimals:      %d", number);
	    return RTT__SUCCESS;
	  }
	  if ( m_ctx->current_item == 9)
	  {
	    /* Change the decimals */
	    if ( EVEN( rtt_get_qualifier( "rtt_arg1", arg1_str)))
	    {
	      if ( EVEN( rtt_get_qualifier( "/MAXLIMIT", arg1_str)))
	      {
	        rtt_message('E',"Enter maxlimit");
	        return RTT__NOPICTURE;
	      }  
	    }	  
	    /* Convert to number */
	    nr = sscanf( arg1_str, "%f", &some_float);
	    if (nr != 1)
	    {
	      rtt_message('E', "Syntax error");
	      return RTT__HOLDCOMMAND;
	    }

	    item_ptr->maxlimit = some_float;
	    sprintf( menu_ptr->text, 
		"Maxlimit:      %f", some_float);
	    return RTT__SUCCESS;
	  }
	  if ( m_ctx->current_item == 10)
	  {
	    /* Change the decimals */
	    if ( EVEN( rtt_get_qualifier( "rtt_arg1", arg1_str)))
	    {
	      if ( EVEN( rtt_get_qualifier( "/MINLIMIT", arg1_str)))
	      {
	        rtt_message('E',"Enter minlimit");
	        return RTT__NOPICTURE;
	      }  
	    }	  
	    /* Convert to number */
	    nr = sscanf( arg1_str, "%f", &some_float);
	    if (nr != 1)
	    {
	      rtt_message('E', "Syntax error");
	      return RTT__HOLDCOMMAND;
	    }

	    item_ptr->minlimit = some_float;
	    sprintf( menu_ptr->text, 
		"Minlimit:      %f", some_float);
	    return RTT__SUCCESS;
	  }
	  else if ( m_ctx->current_item == 11)
	  {
	    /* Change the database */
	    if ( EVEN( rtt_get_qualifier( "rtt_arg1", arg1_str)))
	    {
	      if ( EVEN( rtt_get_qualifier( "/DATABASE", arg1_str)))
	      {
	        rtt_message('E',"Enter database");
	        return RTT__NOPICTURE;
	      }  
	    }	  
	    if ( (strcmp( arg1_str, "GDH") == 0) ||
		(strcmp( arg1_str, "RTT") == 0) ||
		(strcmp( arg1_str, "RTTSYS") == 0) ||
		(strcmp( arg1_str, "USER") == 0))
	    {
	      strcpy( item_ptr->database, arg1_str);	 
	      sprintf( menu_ptr->text,  
		"Database:      %s", arg1_str);
  	      return RTT__SUCCESS;
	    }
	    else
	    {
	      rtt_message('E', "Syntax error");
	      return RTT__HOLDCOMMAND;
	    }
	  }
	  else if ( m_ctx->current_item == 12)
	  {
	    /* Change the declaration */
	    if ( EVEN( rtt_get_qualifier( "rtt_arg1", arg1_str)))
	    {
	      if ( EVEN( rtt_get_qualifier( "/DECLARATION", arg1_str)))
	      {
	        rtt_message('E',"Enter declaration");
	        return RTT__NOPICTURE;
	      }  
	    }	  
	    if ( (strcmp( arg1_str, "INT") == 0) ||
		(strcmp( arg1_str, "SHORT") == 0) ||
		(strcmp( arg1_str, "BOOLEAN") == 0) ||
		(strcmp( arg1_str, "FLOAT") == 0) ||
		(strcmp( arg1_str, "STRING") == 0) ||
		(strcmp( arg1_str, "STRING4") == 0) ||
		(strcmp( arg1_str, "STRING10") == 0) ||
		(strcmp( arg1_str, "STRING20") == 0) ||
		(strcmp( arg1_str, "STRING40") == 0) ||
		(strcmp( arg1_str, "OBJID") == 0) ||
		(strcmp( arg1_str, "ATTRREF") == 0) ||
		(strcmp( arg1_str, "TIME") == 0) ||
		(strcmp( arg1_str, "CHAR") == 0))
	    {
	      strcpy( item_ptr->declaration, arg1_str);	 
	      sprintf( menu_ptr->text,  
		"Declaration:   %s", arg1_str);
  	      return RTT__SUCCESS;
	    }
	    else
	    {
	      rtt_message('E', "Syntax error");
	      return RTT__HOLDCOMMAND;
	    }
	  }
	}
	else
	{
	  /* This it a menu */
	  m_ctx = (menu_ctx) ctx;	
	  if ( strcmp( m_ctx->title, MENU_ATTRIBUTES_TITLE) == 0)
	  {
	    /* This is a menu attributes menu */
	    menu_ptr = m_ctx->menu;
	    menu_ptr += m_ctx->current_item;
	    parent_ctx = (menu_ctx) m_ctx->parent_ctx;
	    parent_menu_ptr = parent_ctx->menu;
	    parent_menu_ptr += parent_ctx->current_item;

	    if ( m_ctx->current_item == 0)
	    {
	      /* Change the text */
	      if ( EVEN( rtt_get_qualifier( "rtt_arg1", arg1_str)))
	      {
	        if ( EVEN( rtt_get_qualifier( "/TEXT", arg1_str)))
	        {
	          rtt_message('E',"Enter text");
	          return RTT__NOPICTURE;
	        }  
	      }
	      /* Subtitute # to space */	    
	      for ( s = arg1_str; *s != 0; s++)
	        if ( *s == '#') *s = ' ';
	      strcpy( parent_menu_ptr->text, arg1_str);
	      strcpy( menu_ptr->text, "Text:    ");
	      strncat( menu_ptr->text, arg1_str, sizeof(menu_ptr->text)-strlen(menu_ptr->text)-1);

	      if ( ODD( dtt_get_stored_menuctx( &newctx, parent_menu_ptr->index)))
	      {
	        /* Change the title in the child ctx also */
	        strcpy( newctx->title, arg1_str);	 
	      }
	      return RTT__SUCCESS;
	    }
	    else if ( m_ctx->current_item == 1)
	    {
	      rtt_message('E', "Type can not be modified");
	      return RTT__NOPICTURE;
	    }
	    else if ( m_ctx->current_item == 2)
	    {
	      rtt_message('E', "Index can not be modified");
	      return RTT__NOPICTURE;
	    }
	    else if ( m_ctx->current_item == 3)
	    {
	      switch ( parent_menu_ptr->type)
	      {
	        case DTT_MENUITEM_COMMAND:
	        case DTT_MENUITEM_COMMANDHOLD:
	        case DTT_MENUITEM_VMSCOMMAND:
	        case DTT_MENUITEM_VMSCOMMANDHOLD:
	        case DTT_MENUITEM_VMSCOMMANDCONF:
	        case DTT_MENUITEM_VMSCOMMANDNOWAIT:
	          /* Change the command */
	          if ( EVEN( rtt_get_qualifier( "rtt_arg1", arg1_str)))
	          {
	            rtt_message('E',"Enter command");
	            return RTT__NOPICTURE;
	          }
	          strcpy( (char *)parent_menu_ptr->arg1, arg1_str);
	          strcpy( menu_ptr->text, "Command: ");
	          strncat( menu_ptr->text, arg1_str, sizeof(menu_ptr->text)-strlen(menu_ptr->text)-1);
	          break;
	        case DTT_MENUITEM_FILESPEC:
	          /* Change the command */
	          if ( EVEN( rtt_get_qualifier( "rtt_arg1", arg1_str)))
	          {
	            rtt_message('E',"Enter command");
	            return RTT__NOPICTURE;
	          }
	          strcpy( (char *)parent_menu_ptr->arg2, arg1_str);
	          strcpy( menu_ptr->text, "Command: ");
	          strncat( menu_ptr->text, arg1_str, sizeof(menu_ptr->text)-strlen(menu_ptr->text)-1);
	          break;
	        case DTT_MENUITEM_OBJPICTURE:
	          /* Change the object picture */
	          if ( EVEN( rtt_get_qualifier( "rtt_arg1", arg1_str)))
	          {
	            rtt_message('E',"Enter object picture");
	            return RTT__NOPICTURE;
	          }
	          strcpy( (char *)parent_menu_ptr->arg1, arg1_str);
	          strcpy( menu_ptr->text, "Object picture: ");
	          strncat( menu_ptr->text, arg1_str, sizeof(menu_ptr->text)-strlen(menu_ptr->text)-1);
	          break;
	        case DTT_MENUITEM_SYSPICTURE:
	          /* Change the system picture */
	          if ( EVEN( rtt_get_qualifier( "rtt_arg1", arg1_str)))
	          {
	            rtt_message('E',"Enter system picture");
	            return RTT__NOPICTURE;
	          }
	          strcpy( (char *)parent_menu_ptr->arg1, arg1_str);
	          strcpy( menu_ptr->text, "System picture: ");
	          strncat( menu_ptr->text, arg1_str, sizeof(menu_ptr->text)-strlen(menu_ptr->text)-1);
	          break;
	        case DTT_MENUITEM_PIFUNC:
	        case DTT_MENUITEM_PIFUNCPERM:
	          /* Change the system picture */
	          if ( EVEN( rtt_get_qualifier( "rtt_arg1", arg1_str)))
	          {
	            rtt_message('E',"Enter function");
	            return RTT__NOPICTURE;
	          }
	          strcpy( (char *)parent_menu_ptr->arg1, arg1_str);
	          strcpy( menu_ptr->text, "Function: ");
	          strncat( menu_ptr->text, arg1_str, sizeof(menu_ptr->text)-strlen(menu_ptr->text)-1);
	          break;
	      }
	      return RTT__SUCCESS;
	    }
	    else if ( m_ctx->current_item == 4)
	    {
	      switch ( parent_menu_ptr->type)
	      {
	        case DTT_MENUITEM_OBJPICTURE:
	          /* Change the object name */
	          if ( EVEN( rtt_get_qualifier( "rtt_arg1", arg1_str)))
	          {
	            rtt_message('E',"Enter object picture");
	            return RTT__NOPICTURE;
	          }
	          strcpy( (char *)parent_menu_ptr->arg2, arg1_str);
	          strcpy( menu_ptr->text, "Object name: ");
	          strncat( menu_ptr->text, arg1_str, sizeof(menu_ptr->text)-strlen(menu_ptr->text)-1);
	          break;
	        case DTT_MENUITEM_FILESPEC:
	          /* Change the object name */
	          if ( EVEN( rtt_get_qualifier( "rtt_arg1", arg1_str)))
	          {
	            rtt_message('E',"Enter file specification");
	            return RTT__NOPICTURE;
	          }
	          strcpy( (char *)parent_menu_ptr->arg1, arg1_str);
	          strcpy( menu_ptr->text, "File spec: ");
	          strncat( menu_ptr->text, arg1_str, sizeof(menu_ptr->text)-strlen(menu_ptr->text)-1);
	          break;
	      }
	      return RTT__SUCCESS;
	    }
	  }
	  else
	  {
	    if ( EVEN( rtt_get_qualifier( "/TEXT", arg1_str)))
	      if ( EVEN( rtt_get_qualifier( "rtt_arg1", arg1_str)))
	      {
	        rtt_message('E',"Syntax error");
	        return RTT__HOLDCOMMAND;
	      }

	    if ( arg1_str[0] == 0)
	    {
	      rtt_message('E', "Enter text");
	      return RTT__HOLDCOMMAND;
	    }

	    /* Subtitute # to space */	    
	    for ( s = arg1_str; *s != 0; s++)
	      if ( *s == '#') *s = ' ';

	    /* Get the current item */
	    m_ctx = (menu_ctx) ctx;	
	    menu_ptr = m_ctx->menu;
	    menu_ptr += m_ctx->current_item;
	    strcpy( menu_ptr->text, arg1_str);

	    if ( ODD( dtt_get_stored_menuctx( &newctx, menu_ptr->index)))
	    {
	      /* Change the title in the child ctx also */
	      strcpy( newctx->title, arg1_str);	 
	    }
	  }
	}
	return RTT__SUCCESS;
}



/*************************************************************************
*
* Name:		show_func()
*
* Type		int
*
* Type		Parameter	IOGF	Description
* menu_ctx	ctx		I	rtt context.
*
* Description:
*	This function is called when a "show" command i recieved.
*
**************************************************************************/

static int	dtt_rtt_func(	menu_ctx	ctx,
				int		flag)
{
	int	sts;

	if ( !dtt_gdh_initialized )
	{
	  sts = rtt_gdh_init();
	  if ( EVEN(sts)) 
	  {
	    rtt_message('E', "Unable to reach Nethandler");
	    return RTT__NOPICTURE;
	  }
	  dtt_gdh_initialized  = 1;
	}
	sts = rtt_hierarchy( ctx, pwr_cNObjid, 0, 0, 0, 0);
	return sts;

	return RTT__SUCCESS;	
}

/*************************************************************************
*
* Name:		dtt_show_func()
*
* Type		int
*
* Type		Parameter	IOGF	Description
* menu_ctx	ctx		I	rtt context.
*
* Description:
*	This function is called when a "show" command i recieved.
*
**************************************************************************/

static int	dtt_show_func(	menu_ctx	ctx,
				int		flag)
{
	int	sts;
	char	arg1_str[80];
	int	arg1_sts;
	char	message[80];
	rtt_t_menu	*menu_ptr;
	edit_ctx	ctx_e;

	arg1_sts = rtt_get_qualifier( "rtt_arg1", arg1_str);

	if ( strncmp( arg1_str, "SYMBOL", strlen( arg1_str)) == 0)
	{
	  /* Command is "SHOW SYMBOL" */
	  char	arg2_str[80];
	  char	message_str[80];
	  char  value[80];
	 
	  if ( EVEN( rtt_get_qualifier( "rtt_arg2", arg2_str)))
	  {
	    rtt_message('E', "Enter symbol");
	    return RTT__HOLDCOMMAND;
	  }
	  sts = rtt_get_symbol( arg2_str, value);
	  if (EVEN(sts))
	  {
	    rtt_message('E', "Symbol not found");
	    return RTT__HOLDCOMMAND;
	  }
	  sprintf( message_str, "Symbol %s = %s", arg2_str, value);
	  rtt_message('I', message_str);
	  return RTT__NOPICTURE;
	}
	else if ( strncmp( arg1_str, "TITLEPREFIX", strlen( arg1_str)) == 0)
	{
	  /* Command is "SHOW TITLEPREFIX" */
	  char	message_str[80];
	 
	  sprintf( message_str, "Titleprefix = %s", dtt_title_prefix);
	  rtt_message('I', message_str);
	  return RTT__NOPICTURE;
	}

	else if ( strncmp( arg1_str, "MENU", strlen( arg1_str)) == 0)
	{
	  /* Command is "SHOW MENU" */
	  char	name_str[80];

	  if ( ODD( rtt_get_qualifier( "rtt_arg2", name_str)))
	  {
	    if ( name_str[0] == '/')
	    {
	      rtt_message('E', "Syntax error");
	      return RTT__HOLDCOMMAND;
	    }
	  }
	  else
	  {
	    if ( EVEN( rtt_get_qualifier( "/NAME", name_str)))
	    {
	      rtt_message('E', "Syntax error, name not found");
	      return RTT__HOLDCOMMAND;
	    }
	  }
	  sts = dtt_show_menu( ctx, name_str);
	  return sts;
	}

	else if ( strncmp( arg1_str, "COLLECTION", strlen( arg1_str)) == 0)
	{
	  sts = rtt_collect_show( ctx);
	  return sts;
	}

	else if ( strncmp( arg1_str, "COMMAND", strlen( arg1_str)) == 0)
	{
	  /* Show the command string in arg1 */
	  /* Check that this is a command menu item */
	  ctx_e = (edit_ctx) ctx;
	  if ( ctx_e->ctx_type == DTT_CTX_EDIT)
	  {
	    /* Edit context */
	    rtt_message('E', "This is not a command menu item");
	    return RTT__NOPICTURE;
	  }
	  menu_ptr = ctx->menu;
	  menu_ptr += ctx->current_item;
	  if ( !( menu_ptr->type != DTT_MENUITEM_COMMAND ||
	          menu_ptr->type != DTT_MENUITEM_COMMANDHOLD ||
	          menu_ptr->type != DTT_MENUITEM_FILESPEC))
	  {
	    rtt_message('E', "This is not a command menu item");
	    return RTT__NOPICTURE;
	  }
	  if ( menu_ptr->type == DTT_MENUITEM_FILESPEC)
	    sprintf( message, "Command: %s", (char *)menu_ptr->arg2);
	  else
	    sprintf( message, "Command: %s", (char *)menu_ptr->arg1);
	  rtt_message('I', message);
	  return RTT__NOPICTURE;	
	}

	else if ( strncmp( arg1_str, "VMS", strlen( arg1_str)) == 0)
	{
	  /* Show the command string in arg1 */
	  /* Check that this is a command menu item */
	  ctx_e = (edit_ctx) ctx;
	  if ( ctx_e->ctx_type == DTT_CTX_EDIT)
	  {
	    /* Edit context */
	    rtt_message('E', "This is not a command menu item");
	    return RTT__NOPICTURE;
	  }
	  menu_ptr = ctx->menu;
	  menu_ptr += ctx->current_item;
	  if ( !(menu_ptr->type == DTT_MENUITEM_VMSCOMMAND ||
	  	 menu_ptr->type == DTT_MENUITEM_VMSCOMMANDNOWAIT ||
	  	 menu_ptr->type == DTT_MENUITEM_VMSCOMMANDCONF ||
	  	 menu_ptr->type == DTT_MENUITEM_VMSCOMMANDHOLD ))
	  {
	    rtt_message('E', "This is not a vms command menu item");
	    return RTT__NOPICTURE;
	  }
	  sprintf( message, "VMS command: %s", (char *)menu_ptr->arg1);
	  rtt_message('I', message);
	  return RTT__NOPICTURE;	
	}

	else if ( strncmp( arg1_str, "SYSPICTURE", strlen( arg1_str)) == 0)
	{
	  /* Show the command string in arg1 */
	  /* Check that this is a command menu item */
	  ctx_e = (edit_ctx) ctx;
	  if ( ctx_e->ctx_type == DTT_CTX_EDIT)
	  {
	    /* Edit context */
	    rtt_message('E', "This is not a system picture menu item");
	    return RTT__NOPICTURE;
	  }
	  menu_ptr = ctx->menu;
	  menu_ptr += ctx->current_item;
	  if ( menu_ptr->type != DTT_MENUITEM_SYSPICTURE)
	  {
	    rtt_message('E', "This is not a syspicture menu item");
	    return RTT__NOPICTURE;
	  }
	  sprintf( message, "System picture: %s", (char *)menu_ptr->arg1);
	  rtt_message('I', message);
	  return RTT__NOPICTURE;	
	}

	else if ( strncmp( arg1_str, "OBJPICTURE", strlen( arg1_str)) == 0)
	{
	  /* Show the command string in arg1 */
	  /* Check that this is a command menu item */
	  ctx_e = (edit_ctx) ctx;
	  if ( ctx_e->ctx_type == DTT_CTX_EDIT)
	  {
	    /* Edit context */
	    rtt_message('E', "This is not a object picture menu item");
	    return RTT__NOPICTURE;
	  }
	  menu_ptr = ctx->menu;
	  menu_ptr += ctx->current_item;
	  if ( menu_ptr->type != DTT_MENUITEM_OBJPICTURE)
	  {
	    rtt_message('E', "This is not a objpicture menu item");
	    return RTT__NOPICTURE;
	  }
	  sprintf( message, "Object picture: %s", (char *)menu_ptr->arg1);
	  rtt_message('I', message);
	  return RTT__NOPICTURE;	
	}

	else if ( strncmp( arg1_str, "FILESPEC", strlen( arg1_str)) == 0)
	{
	  /* Show the command string in arg1 */
	  /* Check that this is a command menu item */
	  ctx_e = (edit_ctx) ctx;
	  if ( ctx_e->ctx_type == DTT_CTX_EDIT)
	  {
	    /* Edit context */
	    rtt_message('E', "This is not a object picture menu item");
	    return RTT__NOPICTURE;
	  }
	  menu_ptr = ctx->menu;
	  menu_ptr += ctx->current_item;
	  if ( menu_ptr->type != DTT_MENUITEM_FILESPEC)
	  {
	    rtt_message('E', "This is not a filespec menu item");
	    return RTT__NOPICTURE;
	  }
	  sprintf( message, "File specification: %s", (char *)menu_ptr->arg1);
	  rtt_message('I', message);
	  return RTT__NOPICTURE;	
	}

	else if ( strncmp( arg1_str, "NAME", strlen( arg1_str)) == 0)
	{
	  /* Show the command string in arg1 */
	  /* Check that this is a command menu item */
	  ctx_e = (edit_ctx) ctx;
	  if ( ctx_e->ctx_type == DTT_CTX_EDIT)
	  {
	    /* Edit context */
	    rtt_message('E', "This is not a object picture menu item");
	    return RTT__NOPICTURE;
	  }
	  menu_ptr = ctx->menu;
	  menu_ptr += ctx->current_item;
	  if ( menu_ptr->type != DTT_MENUITEM_OBJPICTURE)
	  {
	    rtt_message('E', "This is not a object picture menu item");
	    return RTT__NOPICTURE;
	  }
	  sprintf( message, "Object name: %s", (char *)menu_ptr->arg2);
	  rtt_message('I', message);
	  return RTT__NOPICTURE;	
	}

	else if ( strncmp( arg1_str, "FUNCTION", strlen( arg1_str)) == 0)
	{
	  /* Show the command string in arg1 */
	  /* Check that this is a function menu item */
	  ctx_e = (edit_ctx) ctx;
	  if ( ctx_e->ctx_type == DTT_CTX_EDIT)
	  {
	    /* Edit context */
	    rtt_message('E', "This is not a function menu item");
	    return RTT__NOPICTURE;
	  }
	  menu_ptr = ctx->menu;
	  menu_ptr += ctx->current_item;
	  if ( !(menu_ptr->type == DTT_MENUITEM_PIFUNC || 
	       menu_ptr->type == DTT_MENUITEM_PIFUNCPERM))
	  {
	    rtt_message('E', "This is not a function menu item");
	    return RTT__NOPICTURE;
	  }
	  sprintf( message, "Function: %s", (char *)menu_ptr->arg1);
	  rtt_message('I', message);
	  return RTT__NOPICTURE;	
	}
	else
	{
	  rtt_message('E',"Syntax error");
	  return RTT__HOLDCOMMAND;
	}

	return RTT__SUCCESS;	
}

/*************************************************************************
*
* Description:
*	Command table.
*	The commands in pwr_rtt is defined here.
*	First parameter is the command verb.
*	Second is the function to be called.
*	Third and on are the qualifiers.
*	If the first arguments are named rtt_arg1, rtt_arg2 etc
*	parameters are allowed and can be fetched by the user.
*
**************************************************************************/

static rtt_t_comtbl	dtt_command_table[] = {
		{
			"LEARN",
			&rttcmd_learn_func,
			{ "rtt_arg1", "/FILE", ""}
		},
		{
			"PRINT",
			&rttcmd_print_func,
			{ "rtt_arg1", "/FILE", "/APPEND", "/TSIZE",
			"/PSIZE", "/TEXT", ""}
		},
		{
			"SAY",
			&rttcmd_say_func,
			{ "rtt_arg1", "/TEXT", ""}
		},
		{
			"DEFINE",
			&rttcmd_define_func,
			{ "rtt_arg1", "rtt_arg2", "rtt_arg4", "rtt_arg4", ""}
		},
		{	
			"SET",
			&set_func,
			{ "rtt_arg1", "rtt_arg2", "/NAME", "/ON", "/OFF", 
			"/MESSAGE", "/COMMAND", ""}
		},
		{
			"WRITE",
			&write_func,
			{ "rtt_arg1", "rtt_arg2", "/ALL", ""}
		},
		{
			"SAVE",
			&save_func,
			{ "rtt_arg1", "/ALL", "/NOCOMPILE", ""}
		},
		{
			"CONVERT",
			&convert_func,
			{""}
		},
		{
			"GENERATE",
			&generate_func,
			{ "rtt_arg1", "/ALL", "/ID", "/MENU", ""}
		},
		{
			"SYNTAX",
			&syntax_func,
			{ ""}
		},
		{
			"SETUP",
			&dtt_setup_func,
			{ ""}
		},
		{
			"CREATE",
			&create_func,
			{ "rtt_arg1","/TEXT", "/MENU", "/PICTURE", 
			"/CHILD", "/EXIT", "/OBJECTHIERARCHY",
			"/COMMAND", "/COMMANDHOLD", "/FUNCTION", "/TIME", "/FULLTIME", 
			"/SYSPICTURE", "/OBJPICTURE", "/VMSCOMMAND",
			"/VMSCONFIRM", "/VMSHOLD", "/VMSNOWAIT",
			"/NAME", "/PERMPICTURE", "/KEYS", "/FILESPEC", ""}
		},
		{
			"MODIFY",
			&modify_func,
			{ "rtt_arg1", "/NUMBER", "/TEXT", "/TYPE", 
			"/PARAMETER", "/DUALPARAMETER", "/PRIVILEGES", 
			"/OUTPUTFLAGS", "/MAINTITLE", "/MAXLIMIT", 
			"/MINLIMIT", "/CHARACTERS",
			"/DECIMALS", "/DATABASE", "/DECLARATION", 
			"/TITLEPREFIX", ""}
		},
		{
			"EXIT",
			&dtt_exit_func,
			{ "rtt_arg1", ""}
		},
		{
			"QUIT",
			&dtt_quit_func,
			{ "", }
		},
		{
			"HELP",
			&dtt_help_func,
			{ "rtt_arg1", "rtt_arg2", "rtt_arg3", "rtt_arg4",""}
		},
		{
			"LINK",
			&dtt_link_func,
			{ "rtt_arg1", "/DEBUG", ""}
		},
		{
			"COMPILE",
			&dtt_compile_func,
			{ "rtt_arg1", "/DEBUG"}
		},
		{
			"EDIT",
			&dtt_edit_func,
			{ "rtt_arg1", ""}
		},
		{
			"RUN",
			&dtt_run_func,
			{ ""}
		},
		{
			"DELETE",
			&dtt_delete_func,
			{ "rtt_arg1", ""}
		},
		{
			"UNDO",
			&dtt_undo_func,
			{ "rtt_arg1", "rtt_arg2", ""}
		},
		{
			"CUT",
			&dtt_cut_func,
			{ "",}
		},
		{
			"COPY",
			&dtt_copy_func,
			{ "",}
		},
		{
			"PASTE",
			&dtt_paste_func,
			{ "",}
		},
		{
			"SELECT",
			&dtt_select_func,
			{ "",}
		},
		{
			"UNSELECT",
			&dtt_unselect_func,
			{ "",}
		},
		{
			"INCLUDE",
			&dtt_include_func,
			{ "rtt_arg1", "rtt_arg2", "/ADD", ""}
		},
		{
			"RTT",
			&dtt_rtt_func,
			{ "",}
		},
		{
			"CONNECT",
			&dtt_connect_func,
			{ "",}
		},
		{
			"DUALCONNECT",
			&dtt_dualconnect_func,
			{ "",}
		},
		{
			"SHOW",
			&dtt_show_func,
			{ "rtt_arg1", "rtt_arg2", "/NAME", ""}
		},
		{
			"EXPORT",
			&dtt_export_func,
			{ "rtt_arg1", "rtt_arg2", ""}
		},
		{
			"LIST",
			&dtt_list_func,
			{ "rtt_arg1", "rtt_arg2", "/ALL", ""}
		},
		{
			"CLEAR",
			&dtt_clear_func,
			{ "rtt_arg1", ""}
		},
		{"",}};


/*************************************************************************
*
* Name:		dtt_edit_draw_position()
*
* Type		int
*
* Type		Parameter	IOGF	Description
* menu_ctx	*ctx		O	
*
* Description:
*	Draw the current coordinates in the message line.
*
**************************************************************************/

static int	dtt_edit_draw_position( edit_ctx	ctx)
{
	rtt_cursor_abs( 73, 24);
	r_print("(%2d,%2d)", ctx->cursor_x, ctx->cursor_y);
	return 1;
}
/*************************************************************************
*
* Name:		dtt_edit_draw()
*
* Type		int
*
* Type		Parameter	IOGF	Description
* menu_ctx	*ctx		O	
* menu_ctx	parent_ctx	I	
* char		*title		I	
*
* Description:
*	Creates a dtt context.
*
**************************************************************************/

static int	dtt_edit_draw( edit_ctx	ctx)
{

	rtt_display_erase();
	rtt_edit_draw_background( &(ctx->chartable));

	if ( ctx->select_active)
	  rtt_edit_draw_section( &(ctx->chartable), ctx->cursor_x,
		ctx->cursor_y, ctx->select_x, ctx->select_y, 1);

	/* Draw the update items */
        dtt_edit_draw_itemlist( ctx);
	dtt_edit_draw_position( ctx);

	return RTT__SUCCESS;
}


/*************************************************************************
*
* Name:		dtt_edit_draw_itemlist()
*
* Type		int
*
* Type		Parameter	IOGF	Description
* menu_ctx	*ctx		O	
* menu_ctx	parent_ctx	I	
* char		*title		I	
*
* Description:
*	Creates a dtt context.
*
**************************************************************************/

static int	dtt_edit_draw_itemlist( edit_ctx	ctx)
{
	dtt_item	item_ptr;
	int		i;

	/* Draw the update items */
	item_ptr = ctx->upd_items;
	if ( item_ptr != 0)
	{
	  rtt_charset_ascii();
	  rtt_char_inverse_start();
	  while ( item_ptr->number != 0)
	  {
	    rtt_cursor_abs( item_ptr->x + 1, item_ptr->y + 1);
	    switch ( ctx->display_mode)
	    {
	      case DTT_DISPLAY_MODE_FORMAT:
	        if ( item_ptr->text[0] != 0 &&
		     strcmp( item_ptr->text, "%") != 0)
	          r_print("%s", &(item_ptr->text));
	        if ( strcmp( item_ptr->outflags, "NO") != 0 &&
		     strcmp( item_ptr->text, "%") != 0)
	          r_print(" ");
		if ( strcmp( item_ptr->text, "%") == 0 &&
	             item_ptr->characters == 0)
	          r_print("<");
	        if ( item_ptr->characters > 0)
	        r_print("<");
	        for ( i = 1; i < item_ptr->characters; i++)
	        {
	          if ( item_ptr->decimals > 0 &&
		       i == item_ptr->characters - item_ptr->decimals - 1)
	            r_print(".");
	          else
	            r_print("F");
	        }
	        break;
	      case DTT_DISPLAY_MODE_NUMBER:
	        if ( item_ptr->text[0] != 0)
	          r_print("%s", &(item_ptr->text));
	        r_print(" U%d", item_ptr->number);
	        break;
	    }
	    item_ptr++;
	  }
	  rtt_char_inverse_end();
	}

	dtt_edit_restore_settings( ctx);
	return RTT__SUCCESS;
}

/*************************************************************************
*
* Name:		dtt_edit_restore_settings()
*
* Type		int
*
* Type		Parameter	IOGF	Description
* ctx		ctx		I	
*
* Description:
*	Restore settings in edit mode, ascii-line, inverse, cursorposition.
*
**************************************************************************/

static int	dtt_edit_restore_settings( edit_ctx ctx)
{
	if ( ctx->charset & RTT_CHARSET_LINE)
	  rtt_charset_linedrawing();
	else
	  rtt_charset_ascii();
	if ( ctx->charset & RTT_CHARSET_INVERSE)
	  rtt_char_inverse_start();
	else
	  rtt_char_inverse_end();
	rtt_cursor_abs( ctx->cursor_x + 1, ctx->cursor_y + 1);

	return RTT__SUCCESS;
}

/*************************************************************************
*
* Name:		dtt_edit_normal_settings()
*
* Type		int
*
* Type		Parameter	IOGF	Description
*
* Description:
*	Set mod ascii and noinverse.
*
**************************************************************************/

static int	dtt_edit_normal_settings()
{
	rtt_charset_ascii();
	rtt_char_inverse_end();
	return RTT__SUCCESS;
}

/*************************************************************************
*
* Name:		dtt_store_ctx()
*
* Type		int
*
* Type		Parameter	IOGF	Description
* ctx		ctx		I	
* char		*key		I	
*
* Description:
*	Stores a ctx.
*
**************************************************************************/

static int	dtt_store_ctx( 	edit_ctx	ctx,
				char		*key,
				int		idx)
{
#define DTT_STORE_SIZE 100
	dtt_t_store_ctx	*store_ptr;
	int	i = 0;
	int	sts;
	edit_ctx some_ctx;

	/* Check if already stored */
	sts = dtt_get_stored_ctx( &some_ctx, key, idx);
	if ( ODD(sts)) return sts;
	
	if (dtt_ctx_store == 0)
	{
	  dtt_ctx_store = calloc(  DTT_STORE_SIZE, sizeof(dtt_t_store_ctx));
	}
	store_ptr = dtt_ctx_store;

	while ( store_ptr->ctx != 0)
	{
	  store_ptr++;
	  i++;
	}
	if ( i > DTT_STORE_SIZE - 2)
	  return 0;

	store_ptr->ctx = ctx;
	store_ptr->idx = idx;
	strcpy( store_ptr->key, key);

	return RTT__SUCCESS;
}
/*************************************************************************
*
* Name:		dtt_remove_stored_ctx()
*
* Type		int
*
* Type		Parameter	IOGF	Description
* ctx		ctx		I	
* char		*key		I	
*
* Description:
*	Removes a stored ctx.
*
**************************************************************************/
#if 0
static int dtt_remove_stored_ctx( char		*key,
				int		idx)
{
	dtt_t_store_ctx	*store_ptr;
	int		found;

	if (dtt_ctx_store == 0)
	  return 0;

	found = 0;
	store_ptr = dtt_ctx_store;
	while ( store_ptr->ctx != 0)
	{
	  if ( !found )
	  {
	    if ( store_ptr->idx == idx)
	    {
	      found = 1;
	      strcpy( store_ptr->key, (store_ptr + 1)->key);
	      store_ptr->ctx = (store_ptr + 1)->ctx;
	    }
	  }
	  else
	  {
	    strcpy( store_ptr->key, (store_ptr + 1)->key);
	    store_ptr->ctx = (store_ptr + 1)->ctx;
	  }
	  store_ptr++;	
	}
	if (!found)
	  return 0;
	return RTT__SUCCESS;
}
#endif
/*************************************************************************
*
* Name:		dtt_get_stored_ctx()
*
* Type		int
*
* Type		Parameter	IOGF	Description
* ctx		ctx		I	
* char		*key		I	
*
* Description:
*	Returns a stored ctx.
*
**************************************************************************/

static int dtt_get_stored_ctx( 	edit_ctx	*ctx,
				char		*key,
				int		idx)
{
	dtt_t_store_ctx	*store_ptr;

	if (dtt_ctx_store == 0)
	  return 0;

	store_ptr = dtt_ctx_store;
	while ( store_ptr->ctx != 0)
	{
	  if ( store_ptr->idx == idx)
	  {
	    *ctx = store_ptr->ctx;
	    return RTT__SUCCESS;
	  }
	  store_ptr++;	
	}
	return 0;
}

/*************************************************************************
*
* Name:		dtt_store_menuctx()
*
* Type		int
*
* Type		Parameter	IOGF	Description
* ctx		ctx		I	
*
* Description:
*	Stores a ctx.
*
**************************************************************************/

static int	dtt_store_menuctx(	menu_ctx	ctx,
					int		key)
{
	dtt_t_store_menuctx	*store_ptr;
	int	i = 0;
	int	sts;
	menu_ctx some_ctx;

	/* Check if already stored */
	sts = dtt_get_stored_menuctx( &some_ctx, key);
	if ( ODD(sts)) return sts;

	if (dtt_menuctx_store == 0)
	{
	  dtt_menuctx_store = calloc(  400, sizeof(dtt_t_store_ctx));
	}
	store_ptr = dtt_menuctx_store;

	while ( store_ptr->ctx != 0)
	{
	  store_ptr++;	
	  i++;
	}
	if ( i > 398)
	  return 0;

	store_ptr->ctx = ctx;
	store_ptr->key = key;

	return RTT__SUCCESS;
}


/*************************************************************************
*
* Name:		dtt_get_stored_menuctx()
*
* Type		int
*
* Type		Parameter	IOGF	Description
* ctx		ctx		I	
* char		*key		I	
*
* Description:
*	Returns a stored ctx.
*
**************************************************************************/

static int	dtt_get_stored_menuctx( menu_ctx	*ctx, 
					int		key)
{
	dtt_t_store_menuctx	*store_ptr;

	if (dtt_menuctx_store == 0)
	  return 0;
	/* Zero is not defined as a key */
	if ( key == 0)
	  return 0;

	store_ptr = dtt_menuctx_store;
	while ( store_ptr->ctx != 0)
	{
	  if ( store_ptr->key == key)
	  {
	    *ctx = store_ptr->ctx;
	    return RTT__SUCCESS;
	  }
	  store_ptr++;	
	}
	return 0;
}


/*************************************************************************
*
* Name:		dtt_free_ctx()
*
* Type		int
*
* Type		Parameter	IOGF	Description
* edit_ctx	ctx		O	
*
* Description:
*	Free a dtt context.
*
**************************************************************************/

static void	dtt_free_ctx( 	edit_ctx	ctx)
{
	if ( ctx->upd_items)
	  free( (char *) ctx->upd_items);
	free( (char *) ctx);
}
/*************************************************************************
*
* Name:		dtt_create_ctx()
*
* Type		int
*
* Type		Parameter	IOGF	Description
* menu_ctx	*ctx		O	
* menu_ctx	parent_ctx	I	
* char		*title		I	
*
* Description:
*	Creates a dtt context.
*
**************************************************************************/

static int	dtt_create_ctx( 	edit_ctx	*ctx,
					edit_ctx	parent_ctx,
					char		*title,
					int		index)
{
	*ctx = calloc(  1, sizeof(dtt_t_edit_ctx));
	if ( *ctx == 0)
	  return RTT__NOMEMORY;
	(*ctx)->ctx_type = DTT_CTX_EDIT;
	(*ctx)->parent_ctx = parent_ctx;
	(*ctx)->index = index;
	CHARSET_ASCII( (*ctx)->charset);
	strncpy( (*ctx)->title, title, 80); 

	return RTT__SUCCESS;
}

/*************************************************************************
*
* Name:		dtt_edit_new()
*
* Type		int
*
* Type		Parameter	IOGF	Description
* unsigned long	parent_ctx	I	parents rtt context.
* char		*title		I  	buffername.
*
* Description:
*	Create a new edit buffer.
*
**************************************************************************/

int	dtt_edit_new( 	menu_ctx	parent_ctx, 
					unsigned long	arg1,
					unsigned long	arg2,
					unsigned long	index,
					unsigned long	arg4)
{
	int		sts;
	edit_ctx	ctx;
	unsigned long	terminator;
	unsigned long	option;
	char		input_str[80];
	int		maxlen = 1;
	char		filename[80];
	unsigned char	c;
	rtt_t_menu	*menu_ptr;
	char		*title;

	/* Get the title from the parent items text */
	menu_ptr = parent_ctx->menu;
	menu_ptr += parent_ctx->current_item;
	title = menu_ptr->text;


	/* Try to find this context, it may be stored */
	sts = dtt_get_stored_ctx( &ctx, title, menu_ptr->index);
	if ( ODD(sts))
	{
	  dtt_edit_draw( ctx);
	}
	else
	{
	  /* New buffer */
	  sts = dtt_create_ctx( &ctx, (edit_ctx) parent_ctx, title, index);
	  if ( EVEN(sts)) return sts;
	  sts = dtt_store_ctx( ctx, title, index);
	  if ( EVEN(sts)) return sts;
	  /* Try to read it */
	  rtt_display_erase();
	  dtt_get_picturefilename( ctx->index, filename); 
	  sts = dtt_edit_read( ctx, filename, 0);
	  if ( EVEN(sts))
	  {
	    rtt_message('I',"New buffer");
	  }
	  else
	    dtt_edit_draw( ctx);
	}

	option = RTT_OPT_NORECALL | RTT_OPT_NOEDIT | RTT_OPT_NOECHO |
		RTT_OPT_NOSCROLL;

	rtt_cursor_abs( ctx->cursor_x + 1, ctx->cursor_y + 1);

	while (1)
	{
	  dtt_edit_draw_position( ctx);
	  rtt_cursor_abs( ctx->cursor_x + 1, ctx->cursor_y + 1);
	  r_print_buffer();
	  dtt_command_get_input( (char *) rtt_chn, input_str, &terminator, 
		maxlen, option, 0);
	  rtt_message('S',"");
	  c = input_str[0];
	  rtt_cursor_abs( ctx->cursor_x + 1, ctx->cursor_y + 1);

	  if ( terminator == RTT_K_ARROW_LEFT)
	  {
	    if ( ctx->cursor_x > 0)
	    {
	      ctx->cursor_x--;
	      rtt_cursor_rel( 0, -1);
	      if ( ctx->select_active)
	      {
	        if ( ctx->cursor_x >= ctx->select_x )
	          rtt_edit_draw_section( &(ctx->chartable), ctx->cursor_x + 1,
	 		ctx->cursor_y, ctx->cursor_x + 1, ctx->select_y, 0);
		else
	          rtt_edit_draw_section( &(ctx->chartable), ctx->cursor_x,
	 		ctx->cursor_y, ctx->cursor_x, ctx->select_y, 1);
	        rtt_cursor_abs( ctx->cursor_x + 1, ctx->cursor_y + 1);
	      }
	    }
	  }
	  else if ( terminator == RTT_K_ARROW_RIGHT)
	  {
	    if ( ctx->cursor_x < 79)
	    {
	      ctx->cursor_x++;
	      rtt_cursor_rel( 0, 1);
	      if ( ctx->select_active)
	      {
	        if ( ctx->cursor_x > ctx->select_x )
	          rtt_edit_draw_section( &(ctx->chartable), ctx->cursor_x,
	 		ctx->cursor_y, ctx->cursor_x, ctx->select_y, 1);
	        else
	          rtt_edit_draw_section( &(ctx->chartable), ctx->cursor_x - 1,
	 		ctx->cursor_y, ctx->cursor_x - 1, ctx->select_y, 0);
	        rtt_cursor_abs( ctx->cursor_x + 1, ctx->cursor_y + 1);
	      }
	    }
	  }
	  if ( terminator == RTT_K_ARROW_UP)
	  {
	    if ( ctx->cursor_y > 0)
	    {
	      ctx->cursor_y--;
	      rtt_cursor_rel( 1, 0);
	      if ( ctx->select_active)
	      {
	        if ( ctx->cursor_y >= ctx->select_y )
	          rtt_edit_draw_section( &(ctx->chartable), ctx->cursor_x,
	 		ctx->cursor_y + 1, ctx->select_x, ctx->cursor_y + 1, 0);
	        else
	          rtt_edit_draw_section( &(ctx->chartable), ctx->cursor_x,
	 		ctx->cursor_y, ctx->select_x, ctx->cursor_y, 1);
	        rtt_cursor_abs( ctx->cursor_x + 1, ctx->cursor_y + 1);
	      }
	    }
	  }
	  else if ( terminator == RTT_K_ARROW_DOWN)
	  {
	    if ( ctx->cursor_y < 21)
	    {
	      ctx->cursor_y++;
	      rtt_cursor_rel( -1, 0);
	      if ( ctx->select_active)
	      {
	        if ( ctx->cursor_y > ctx->select_y )
	          rtt_edit_draw_section( &(ctx->chartable), ctx->cursor_x,
	 		ctx->cursor_y, ctx->select_x, ctx->cursor_y, 1);
	        else
	          rtt_edit_draw_section( &(ctx->chartable), ctx->cursor_x,
	 		ctx->cursor_y - 1, ctx->select_x, ctx->cursor_y - 1, 0);
	        rtt_cursor_abs( ctx->cursor_x + 1, ctx->cursor_y + 1);
	      }
	    }
	  }
	  else if ( terminator == RTT_K_BACKSPACE)
	  {
	  }
	  else if ( terminator == RTT_K_PF4)
	  {
	    break;
	  }
	  else if ( terminator == RTT_K_PF1)
	  {
	    dtt_edit_normal_settings();
	    sts = dtt_upd_item_show( ctx, 0, 0, 0, 0);
	    if ( EVEN(sts)) return sts;
	    if ( sts == RTT__FASTBACK)
	    {
	      if ( ctx->parent_ctx != 0)
	      {	
	        return RTT__FASTBACK;
	      }
	    }
	    if ( sts == RTT__BACK)
	    {
	      if ( ctx->parent_ctx != 0)
	      {	
	        return RTT__SUCCESS;
	      }
	    }
	    if ( sts != RTT__NOPICTURE)
	    {
	      dtt_edit_draw( ctx);
	    } 
	    dtt_edit_restore_settings( ctx);
	  }
	  else if ( terminator == RTT_K_DELETE)
	  {
	    if ( ctx->cursor_x > 0)
	    {
	      ctx->cursor_x--;
	      rtt_cursor_rel( 0, -1);
	      rtt_char_inverse_end();
	      r_print(" ");
	      if ( ctx->charset & RTT_CHARSET_INVERSE)
	        rtt_char_inverse_start();
	      rtt_cursor_rel( 0, -1);
	      ctx->chartable[0][ ctx->cursor_x][ ctx->cursor_y] = 0;
	      ctx->chartable[1][ ctx->cursor_x][ ctx->cursor_y] = 0;
  	    }
	  }
	  if ( terminator == RTT_K_COMMAND)
	  {
	    dtt_edit_normal_settings();
	    sts = rtt_get_command( (menu_ctx) ctx, (char *) &rtt_chn, 
		rtt_recallbuff,  0, 0, ctx, 
	    	"rtt_edit> ", 0, RTT_ROW_COMMAND, dtt_command_table);
	    /* menu_ptr might have been changed */
	    if ( EVEN(sts)) return sts;
	    if ( sts == RTT__FASTBACK)
	    {
	      if ( ctx->parent_ctx != 0)
	      {	
	        return RTT__FASTBACK;
	      }
	    }
	    if ( sts == RTT__BACK)
	    {
	      if ( ctx->parent_ctx != 0)
	      {	
	        return RTT__SUCCESS;
	      }
	    }
	    if ( sts == RTT__NOBACKGROUND)
	    {
	      dtt_edit_draw_itemlist( ctx);
	    } 
	    else if ( sts != RTT__NOPICTURE)
	    {
	      dtt_edit_draw( ctx);
	    } 
	    dtt_edit_restore_settings( ctx);
	  } 
	  if ( terminator == RTT_K_HELP)
	  {
	    /* Try to find subject in application help */
	    dtt_edit_normal_settings();
	    sts = rtt_help( parent_ctx, title, rtt_edit_helptext);
	    if ( sts == RTT__NOHELPSUBJ)
	      /* Not found, show the 'object menu' help */
	      rtt_help( parent_ctx, "PICTURE", rtt_edit_helptext);

	    dtt_edit_draw( ctx);
	  } 
	  if ( terminator == RTT_K_CTRLW)
	  {
	    dtt_edit_draw( ctx);
	  }
	  if ( terminator == RTT_K_PF2)
	  {
	    sts = dtt_select_func( ctx, 0);
	    if ( sts != RTT__NOPICTURE)	
	      dtt_edit_draw( ctx);
	  }
	  if ( terminator == RTT_K_PF3)
	  {
	    sts = dtt_copy_func( ctx, 0);
	    if ( sts != RTT__NOPICTURE)	
	      dtt_edit_draw( ctx);
	  }
	  if ( terminator == RTT_K_CTRLV)
	  {
	    sts = dtt_connect_func( ctx, 0);
	    if ( sts != RTT__NOPICTURE)	
	      dtt_edit_draw( ctx);
	  }
	  if ( terminator == RTT_K_CTRLN)
	  {
	    dtt_edit_normal_settings();
	    sts = rtt_collect_show( (menu_ctx) ctx);
	    if ( sts != RTT__NOPICTURE)	
	      dtt_edit_draw( ctx);
	    dtt_edit_restore_settings( ctx);
	  }
	  if ( terminator == RTT_K_CTRLZ)
	  {
	    return RTT__FASTBACK;
	  }
	  if ( terminator == RTT_K_NEXTPAGE)
	  {
	    sts = dtt_paste_func( ctx, 0);
	    if ( sts != RTT__NOPICTURE)	
	      dtt_edit_draw( ctx);
	  } 
	  if ( terminator == RTT_K_PREVPAGE)
	  {
	  } 
	  else if ( c >= 31)
	  {
	    ctx->chartable[0][ ctx->cursor_x][ ctx->cursor_y] = input_str[0];
	    ctx->chartable[1][ ctx->cursor_x][ ctx->cursor_y] = ctx->charset;
	    r_print("%c", input_str[0]);
	    ctx->cursor_x++;
	  }	
	}
	rtt_charset_ascii();
	rtt_char_inverse_end();
	return RTT__SUCCESS;
}


/*************************************************************************
*
* Name:		dtt_edit_write()
*
* Type		int
*
* Type		Parameter	IOGF	Description
*
* Description:
*	Write an edit buffer.
*
**************************************************************************/

static int	dtt_edit_write( edit_ctx	ctx,
				char		*filename,
				char		*menu_filename,
				int		userspec_filename,
				int		nocompile,
				int		generate_only,
				int		opsys,
				int		return_wait,
				int		*messages)
{
	int		sts;
	FILE		*fout;
	FILE		*fout_db1;
	FILE		*fout_db2;
	char		*s;
	int		i, j, k;
	char		*char_ptr;
	dtt_item	item_ptr;
	char		fname[200];
	char		fnamebld[200];
	char		picture_name[80];
	int		x, y;
	int		error_count;
	int		warning_count;

	/* Sort the itemlist */
	sts = dtt_number_sort( ctx->upd_items);
	
	if ( userspec_filename)
	{
	  strcpy( fname, filename);
	  /* Check if there is any extention in the filename */
	  s = strchr( fname, '.');
	  if ( s == 0)
	    /* No extention given */
	    strcat( fname, ".dtt");

	  nocompile = 1;
	}
	else
	{
	  sprintf( fname, "%s%s.dtt", dtt_source_dir, filename);
	  sprintf( fnamebld, "%s%s.", dtt_build_dir, filename);
	}
	if ( dtt_is_rttsys && !generate_only)
	  /* If rttsys the files must be generated by make */
	  nocompile = 1;

	/* Open and write */
	if ( !generate_only)
	{
	  fout = fopen( fname, "w");

	  if ( fout == 0)
	    return 0;

#if 0
	  fwrite( &(ctx->chartable), sizeof( ctx->chartable), 1, fout);
#endif
	  for ( i = 0; 
	    i < (int)(sizeof(ctx->chartable) / sizeof(ctx->chartable[0]));
	    i++)
	  {
	    for ( j = 0; 
	      j < (int)(sizeof(ctx->chartable[0]) / sizeof(ctx->chartable[0][0]));
	      j++)
	    {
	      for ( k = 0; 
	        k < (int)(sizeof(ctx->chartable[0][0]) / sizeof(ctx->chartable[0][0][0]));
	        k++)
	      {
	        fprintf( fout, "%d ", ctx->chartable[i][j][k]);
	      }
	      fprintf( fout, "\n");
	    }
	  }

	  /* Write update items */
#if 0
	  item_ptr = ctx->upd_items;
	  if ( item_ptr != 0)
	  {
	    while ( item_ptr->number != 0)
	    {
	      char_ptr = (char *) item_ptr;
	      fwrite( item_ptr, sizeof( dtt_t_upd_item), 1, fout);
	      item_ptr++;
	    }
	  }
#endif
	  sts = dtt_edit_list_items( ctx, fout);
	  fclose(fout);
	  if ( EVEN(sts)) return sts;

	}

	sts = dtt_edit_check_items_syntax( ctx, &error_count, &warning_count);
	if ( (error_count || warning_count) &&
	     return_wait &&
	     !(rtt_commandmode & RTT_COMMANDMODE_FILE))
	{
	  rtt_wait_for_return();
	  *messages = 1;
	}
	else
	  *messages = 0;

	if ( error_count)
	  return RTT__SUCCESS;

	if ( nocompile)
	  return RTT__SUCCESS;

	/* Write an includefile */
	s = strrchr( fnamebld, '.');
	*s = 0;
	strcat( fnamebld, ".c");
	fout = fopen( fnamebld, "w");

	if ( fout == 0)
	  return 0;

	/* Write an rtt database file */
	s = strrchr( fnamebld, '.');
	*s = 0;
	strcat( fnamebld, ".rdb1");
	fout_db1 = fopen( fnamebld, "w");
	if ( fout_db1 == 0)
	  return 0;

	/* Write an rtt database file */
	s = strrchr( fnamebld, '.');
	*s = 0;
	strcat( fnamebld, ".rdb2");
	fout_db2 = fopen( fnamebld, "w");
	if ( fout_db2 == 0)
	  return 0;

	dtt_get_picturename( ctx->index, picture_name);
	fprintf( fout, "/* c file for picture %s */\n", picture_name);
	fprintf( fout, "#include \"rt_rtt.h\"\n");
	fprintf( fout, "#include \"%s.rh\"\n", menu_filename);
	fprintf( fout, "#if defined(OS_ELN)\n");
	fprintf( fout, "globaldef rtt_t_backgr %s_bg = {\n", picture_name);
	fprintf( fout, "#else\n");
	fprintf( fout, "rtt_t_backgr %s_bg = {\n", picture_name);
	fprintf( fout, "#endif\n");
	for ( i = 0; i < 2; i++)
	{
	  fprintf( fout, "{\n");
	  for ( j = 0; j < 80; j++)
	  {
	    fprintf( fout, "{");
	    for ( k = 0; k < 22; k++)
	    {
	      fprintf( fout, "%d" ,ctx->chartable[i][j][k]);
	      if ( k != 21) 
	        fprintf( fout, ",");
	    }
	    fprintf( fout, "}\n");
	    if ( j != 79) 
	      fprintf( fout, ",");
	  }
	  fprintf( fout, "}");
	  if ( i != 1) 
	    fprintf( fout, ",\n");
	}
	fprintf( fout, "};\n");

	/* Write update items */
	item_ptr = ctx->upd_items;
	fprintf( fout, "#if defined(OS_ELN)\n");
	fprintf( fout, "globaldef rtt_t_menu_update %s_eu[] = {\n", picture_name);
	fprintf( fout, "#else\n");
	fprintf( fout, "rtt_t_menu_update %s_eu[] = {\n", picture_name);
	fprintf( fout, "#endif\n");
	if ( item_ptr != 0)
	{
	  while ( item_ptr->number != 0)
	  {
	    char_ptr = (char *) item_ptr;
	    /* text */
	    fprintf( fout, "{\"%s\",\n", item_ptr->text);
	    /* first and second function */
	    if (strcmp( item_ptr->type, "UPDATE") == 0)
	      fprintf( fout, "0,0,\n");
	    else if (strcmp( item_ptr->type, "SET") == 0)
	      fprintf( fout, "&rtt_menu_parameter_set,0,\n");
	    else if (strcmp( item_ptr->type, "RESET") == 0)
	      fprintf( fout, "&rtt_menu_parameter_reset,0,\n");
	    else if (strcmp( item_ptr->type, "SET_RESET") == 0)
	      fprintf( fout,
		"&rtt_menu_parameter_set,&rtt_menu_parameter_reset,\n"); 
	    else if (strcmp( item_ptr->type, "TOGGLE") == 0)
	      fprintf( fout, "&rtt_menu_parameter_toggle,0,\n");
	    else if (strcmp( item_ptr->type, "DUAL_SET") == 0)
	      fprintf( fout, "&rtt_menu_parameter_dual_set,0,\n");
	    else if (strcmp( item_ptr->type, "DUAL_RESET") == 0)
	      fprintf( fout, "&rtt_menu_parameter_dual_reset,0,\n");
	    else if (strcmp( item_ptr->type, "DUAL_SET_RESET") == 0)
	      fprintf( fout,
		"&rtt_menu_parameter_dual_set,&rtt_menu_parameter_dual_reset,\n"); 
	    else if (strcmp( item_ptr->type, "DUAL_TOGGLE") == 0)
	      fprintf( fout, "&rtt_menu_parameter_dual_toggle,0,\n");
	    else if (strcmp( item_ptr->type, "SET_DUALSET") == 0)
	      fprintf( fout,
		"&rtt_menu_parameter_set,&rtt_menu_parameter_dual_set,\n"); 
	    else if (strcmp( item_ptr->type, "TOGGLE_DUALTOGGLE") == 0)
	      fprintf( fout,
		"&rtt_menu_parameter_toggle,&rtt_menu_parameter_dual_toggle,\n"); 
	    else if (strcmp( item_ptr->type, "COMMAND") == 0)
	    {
	      fprintf( fout, "&rtt_menu_parameter_command,0,\n");
	      /* Database has to be user and outflags no */
	      strcpy( item_ptr->database, "USER");
	      strcpy( item_ptr->outflags, "NO");
	    }	      
	    /* Print parameter */
	    fprintf( fout, "\"%s\",\n", item_ptr->parameter);
	    /* Print dualparameter */
	    fprintf( fout, "\"%s\",\n", 
			dtt_quotes_convert(item_ptr->dualparameter));
	    /* Print privileges */	
	    if ( !strcmp( item_ptr->text, "%"))
	      strcpy( item_ptr->priv, "NO");
	    fprintf( fout, "RTT_PRIV_%s ", item_ptr->priv);
	    /* Print outflags */
	    if ( item_ptr->outflags[0] != 0)
	      fprintf( fout, "| RTT_OUTPUT_%s", item_ptr->outflags);
	    /* Print inputflags */
	    if (strcmp( item_ptr->type, "UPDATE") != 0)
	      fprintf( fout, "| RTT_MENU_NOINPUT");
	    fprintf( fout, ",\n");
	    /* Print koordinates f|r update */
	    x = item_ptr->x + strlen( item_ptr->text) + 2;
	    y = item_ptr->y + 1;
	    fprintf( fout, "%d,", x);
	    fprintf( fout, " %d, ", y);
	    /* Print characters */
	    fprintf( fout, "%d,", item_ptr->characters);
	    /* Print decimals */
	    fprintf( fout, "%d,", item_ptr->decimals);
	    /* Print maxlimit */
	    fprintf( fout, "%f,", item_ptr->maxlimit);
	    /* Print minlimit */
	    fprintf( fout, "%f, ", item_ptr->minlimit);
	    /* Print database */	
	    fprintf( fout, "RTT_DATABASE_%s, ", item_ptr->database);
	    /* Print declaration */	
	    fprintf( fout, "RTT_DECL_%s},\n", item_ptr->declaration);
	    
	    if (( strcmp( item_ptr->database, "RTT") == 0) ||
	        ( strcmp( item_ptr->database, "RTTSYS") == 0))
	    {
	      if (!( ( strcmp( item_ptr->parameter, "RTT_TIME") == 0) ||
	      	     ( strcmp( item_ptr->parameter, "RTT_TIME_FULL") == 0) ||
	      	     ( strcmp( item_ptr->parameter, "RTT_ALARMTEXT1") == 0)||
	      	     ( strcmp( item_ptr->parameter, "RTT_ALARMTEXT2") == 0)||
	      	     ( strcmp( item_ptr->parameter, "RTT_ALARMTEXT3") == 0)
			)) 
	      {
	        /* Print in rtt database */
	        if ( strcmp( item_ptr->declaration, "INT") == 0)
	          fprintf( fout_db1, "static int %s;\n", item_ptr->parameter);
	        else if ( strcmp( item_ptr->declaration, "SHORT") == 0)
	          fprintf( fout_db1, "static short %s;\n", item_ptr->parameter);
	        else if ( strcmp( item_ptr->declaration, "FLOAT") == 0)
	          fprintf( fout_db1, "static float %s;\n", item_ptr->parameter);
	        else if ( strcmp( item_ptr->declaration, "BOOLEAN") == 0)
	          fprintf( fout_db1, "static unsigned int %s;\n", item_ptr->parameter);
	        else if ( strcmp( item_ptr->declaration, "CHAR") == 0)
	          fprintf( fout_db1, "static char %s;\n", item_ptr->parameter);
	        else if ( strcmp( item_ptr->declaration, "STRING") == 0)
	          fprintf( fout_db1, "static char %s[80];\n", item_ptr->parameter);
	        else if ( strcmp( item_ptr->declaration, "STRING4") == 0)
	          fprintf( fout_db1, "static char %s[4];\n", item_ptr->parameter);
	        else if ( strcmp( item_ptr->declaration, "STRING10") == 0)
	          fprintf( fout_db1, "static char %s[10];\n", item_ptr->parameter);
	        else if ( strcmp( item_ptr->declaration, "STRING20") == 0)
	          fprintf( fout_db1, "static char %s[20];\n", item_ptr->parameter);
	        else if ( strcmp( item_ptr->declaration, "STRING40") == 0)
	          fprintf( fout_db1, "static char %s[40];\n", item_ptr->parameter);
	        else if ( strcmp( item_ptr->declaration, "OBJID") == 0)
	          fprintf( fout_db1, "static pwr_tObjid %s;\n", item_ptr->parameter);
	        else if ( strcmp( item_ptr->declaration, "ATTRREF") == 0)
	          fprintf( fout_db1, "static pwr_sAttrRef %s;\n", item_ptr->parameter);
	        else if ( strcmp( item_ptr->declaration, "TIME") == 0)
	          fprintf( fout_db1, "static pwr_tTime %s;\n", item_ptr->parameter);

	        fprintf( fout_db2, "{\"%s\", (char *) &%s},\n", item_ptr->parameter, 
			item_ptr->parameter);
	      }
	    }
	    item_ptr++;
	  }
	}
	fprintf( fout, "{ \"\",}};\n");
	fprintf( fout, "pwr_dExport rtt_t_menu_update *%s_euptr = (rtt_t_menu_update *)  &%s_eu;\n", 
		picture_name, picture_name);
	fclose(fout);
	fclose(fout_db1);
	fclose(fout_db2);

	/* Compile the created file and put into library */
	s = strrchr( fname, '.');
	*s = 0;
	dtt_compile_picture( filename, opsys);

	return	RTT__SUCCESS;
}

/*************************************************************************
*
* Name:		dtt_edit_read()
*
* Type		int
*
* Type		Parameter	IOGF	Description
*
* Description:
*	Read an edit buffer.
*
**************************************************************************/

static int	dtt_edit_read( 	edit_ctx	ctx,
				char		*name,
				int		userspec_name)
{
	FILE		*fin;
	char		*s;
	char		filename[80];
	int		sts;
	char		line[200];
	int		i, j;

	if ( userspec_name)
	{
	  strcpy( filename, name);
	  /* Check if there is any extention in the filename */
	  s = strchr( filename, '.');
	  if ( s == 0)
	  {
	    /* No extention given */
	    strcat( filename, ".dtt");
	  }
	}
	else
	  sprintf( filename, "%s%s.dtt", dtt_source_dir, name);

	/* Open and read */
	fin = fopen( filename, "r");

	if ( fin == 0)
	  return 0;

#if 0
	if (fread( &(ctx->chartable), sizeof( ctx->chartable), 1, fin) == 0)
	  return 0;
#endif
	for ( i = 0; 
	    i < (int)(sizeof(ctx->chartable) / sizeof(ctx->chartable[0]));
	    i++)
	{
	  for ( j = 0; 
	      j < (int)(sizeof(ctx->chartable[0]) / sizeof(ctx->chartable[0][0]));
	      j++)
	  {
	    sts = dtt_read_line( line, sizeof(line), fin);
	    if ( EVEN(sts)) return sts;
	    sscanf( line, "%d %d %d %d %d %d %d %d %d %d \
			%d %d %d %d %d %d %d %d %d %d %d %d", 
		&ctx->chartable[i][j][0],
		&ctx->chartable[i][j][1],
		&ctx->chartable[i][j][2],
		&ctx->chartable[i][j][3],
		&ctx->chartable[i][j][4],
		&ctx->chartable[i][j][5],
		&ctx->chartable[i][j][6],
		&ctx->chartable[i][j][7],
		&ctx->chartable[i][j][8],
		&ctx->chartable[i][j][9],
		&ctx->chartable[i][j][10],
		&ctx->chartable[i][j][11],
		&ctx->chartable[i][j][12],
		&ctx->chartable[i][j][13],
		&ctx->chartable[i][j][14],
		&ctx->chartable[i][j][15],
		&ctx->chartable[i][j][16],
		&ctx->chartable[i][j][17],
		&ctx->chartable[i][j][18],
		&ctx->chartable[i][j][19],
		&ctx->chartable[i][j][20],
		&ctx->chartable[i][j][21]);
	  }
	}

	/* Read update items */
	sts = dtt_edit_read_picture_items( ctx, fin);
	fclose(fin);
	if ( EVEN(sts)) return sts;
#if 0
	while ( 1)
	{
	  if ( fread( &item_buffer, sizeof( dtt_t_upd_item), 1, fin) == 0)
	    break;

	  /* Insert the buffer in the upd item list */
	  sts = dtt_upd_item_add( &(ctx->upd_items), 0, 0, "");
	  item_ptr = ctx->upd_items;
	  while ( item_ptr->number != 0)
	    item_ptr++;
	  item_ptr--;
	  memcpy( item_ptr, &item_buffer, sizeof(item_buffer));
	}
#endif

	return	RTT__SUCCESS;
}

/*************************************************************************
*
* Name:		dtt_edit_read_v27()
*
* Type		int
*
* Type		Parameter	IOGF	Description
*
* Description:
*	Read an edit buffer in V2.7 syntax. Used to convert to current
*	syntax.
*
**************************************************************************/

static int	dtt_edit_read_v27( 	edit_ctx	ctx,
				char		*name,
				int		userspec_name)
{
	FILE		*fin;
	char		*s;
	char		filename[80];
	dtt_t_upd_item 	item_buffer;
	dtt_item	item_ptr;
	int		sts;

	if ( userspec_name)
	{
	  strcpy( filename, name);
	  /* Check if there is any extention in the filename */
	  s = strchr( filename, '.');
	  if ( s == 0)
	  {
	    /* No extention given */
	    strcat( filename, ".dtt");
	  }
	}
	else
	  sprintf( filename, "%s%s.dtt", dtt_source_dir, name);

	/* Open and read */
	fin = fopen( filename, "r");

	if ( fin == 0)
	  return 0;

	if (fread( &(ctx->chartable), sizeof( ctx->chartable), 1, fin) == 0)
	  return 0;

	/* Read update items */
	while ( 1)
	{
	  if ( fread( &item_buffer, sizeof( dtt_t_upd_item), 1, fin) == 0)
	    break;

	  /* Insert the buffer in the upd item list */
	  sts = dtt_upd_item_add( &(ctx->upd_items), 0, 0, "");
	  item_ptr = ctx->upd_items;
	  while ( item_ptr->number != 0)
	    item_ptr++;
	  item_ptr--;
	  memcpy( item_ptr, &item_buffer, sizeof(item_buffer));
	}

	fclose(fin);
	return	RTT__SUCCESS;
}


/*************************************************************************
*
* Name:		dtt_upd_item_add()
*
* Type		int
*
* Type		Parameter	IOGF	Description
*
* Description:
*	Adds an item to a itemlist.
*
**************************************************************************/

static int	dtt_upd_item_add( 	dtt_item	*updlist,
					int		x,
					int		y,
					char		*text)
{
	dtt_item	item_ptr;
	dtt_item	new_updlist;
	int		index = 0;

	item_ptr = *updlist;
	if ( item_ptr == 0)
	  index = 0;
	else
	{
	  while ( item_ptr->number != 0)
	  {
	    index++;
	    item_ptr++;
	  }
	}

	if ( index == 0)
	{
	  *updlist = calloc( 1 , 2 * sizeof(dtt_t_upd_item));
	  if ( *updlist == 0)
	    return RTT__NOMEMORY;
	}
	else
	{
	  new_updlist = calloc( index + 2 , sizeof(dtt_t_upd_item));
	  if ( new_updlist == 0)
	    return RTT__NOMEMORY;
	  memcpy( new_updlist, *updlist, (index + 1) * sizeof(dtt_t_upd_item));
	  free( *updlist);
	  *updlist = new_updlist;
	}
	item_ptr = *updlist + index;
	item_ptr->number = index + 1;
	item_ptr->x = x;
	item_ptr->y = y;
	strcpy( item_ptr->text, text);
	strcpy( item_ptr->type, "UPDATE");
	strcpy( item_ptr->priv, "OP");
	strcpy( item_ptr->database, "GDH");

	return RTT__SUCCESS;
}

/*************************************************************************
*
* Name:		dtt_get_upd_item()
*
* Type		int
*
* Type		Parameter	IOGF	Description
* edit_ctx	ctx		I	dtt context.
* upd_item	*item_ptr	O	item pointer.
*
* Description:
*	Return a pointer to current update item.
*
**************************************************************************/

static int	dtt_get_upd_item( 	dtt_item	itemlist,
					int		x,
					int		y,
					dtt_item	*item_ptr)
{
	dtt_item	item_p;
	
	if ( itemlist == 0)
	  return 0;

	item_p = itemlist;
	while ( item_p->number != 0)
	{
	  if ( (item_p->x == x) && (item_p->y == y))
	  {
	    *item_ptr = item_p;
	    return RTT__SUCCESS;
	  }
	  item_p++;
	}
	return 0;
}

/*************************************************************************
*
* Name:		dtt_upd_item_show()
*
* Type		int
*
* Type		Parameter	IOGF	Description
* unsigned long	parent_ctx	I	parent rtt context.
* int		arg1		I	
* int		arg2		I	
* int		arg3		I	
* int		arg4		I	
*
* Description:
*	Displays the content of a upd item.
*
**************************************************************************/

static int	dtt_upd_item_show( 	edit_ctx	parent_ctx,
					int		arg1,
					int		arg2,
					int		arg3,
					int		arg4)
{
	int		sts;
	char		title[80];
	char		text[80];
	rtt_t_menu	*menulist = 0;
	int		i;
	dtt_item	item_ptr;

	/* Check that parent is a edit picture */
	if ( parent_ctx->ctx_type != DTT_CTX_EDIT)
	{
	  rtt_message('E',"Not edit mode");
	  return RTT__NOPICTURE;
	}

	/* Identify the update item */
	sts = dtt_get_upd_item( parent_ctx->upd_items, parent_ctx->cursor_x, 
		parent_ctx->cursor_y, &item_ptr);
	if ( EVEN( sts))
	{
	  rtt_message('E',"Position on a update item");
	  return RTT__NOPICTURE;
	}

	/* Create a title */
	sprintf( title, "%s U%d", item_ptr->text, item_ptr->number);

	i = 0;
	/* Create a menu item for the number */
	sprintf( text, "Number:        %d", item_ptr->number);
	sts = rtt_menu_list_add( &menulist, i, 0, text, 
		0, 0, 0,
		pwr_cNObjid, item_ptr,0,0,0);
	if ( EVEN(sts)) return sts;
	i++;

	/* Create a menu item for the text */
	sprintf( text, "Text:          %s", item_ptr->text);
	sts = rtt_menu_list_add( &menulist, i, 0, text, 
		0, 0, 0,
		pwr_cNObjid, item_ptr,0,0,0);
	if ( EVEN(sts)) return sts;
	i++;

	/* Create a menu item for the text */
	sprintf( text, "Type:          %s", item_ptr->type);
	sts = rtt_menu_list_add( &menulist, i, 0, text, 
		0, 0, 0,
		pwr_cNObjid, item_ptr,0,0,0);
	if ( EVEN(sts)) return sts;
	i++;

	/* Create a menu item for the text */
	sprintf( text, "Parameter:     %s", item_ptr->parameter);
	sts = rtt_menu_list_add( &menulist, i, 0, text, 
		0, 0, 0,
		pwr_cNObjid, item_ptr,0,0,0);
	if ( EVEN(sts)) return sts;
	i++;

	/* Create a menu item for the text */
	sprintf( text, "Text/Dualpar:  %s", item_ptr->dualparameter);
	sts = rtt_menu_list_add( &menulist, i, 0, text, 
		0, 0, 0,
		pwr_cNObjid, item_ptr,0,0,0);
	if ( EVEN(sts)) return sts;
	i++;

	/* Create a menu item for the text */
	sprintf( text, "Privileges:    %s", item_ptr->priv);
	sts = rtt_menu_list_add( &menulist, i, 0, text, 
		0, 0, 0,
		pwr_cNObjid, item_ptr,0,0,0);
	if ( EVEN(sts)) return sts;
	i++;

	/* Create a menu item for the text */
	sprintf( text, "Outputflags:   %s", item_ptr->outflags);
	sts = rtt_menu_list_add( &menulist, i, 0, text, 
		0, 0, 0,
		pwr_cNObjid, item_ptr,0,0,0);
	if ( EVEN(sts)) return sts;
	i++;

	/* Create a menu item for the characters */
	sprintf( text, "Characters:    %d", item_ptr->characters);
	sts = rtt_menu_list_add( &menulist, i, 0, text, 
		0, 0, 0,
		pwr_cNObjid, item_ptr,0,0,0);
	if ( EVEN(sts)) return sts;
	i++;

	/* Create a menu item for the decimals */
	sprintf( text, "Decimals:      %d", item_ptr->decimals);
	sts = rtt_menu_list_add( &menulist, i, 0, text, 
		0, 0, 0,
		pwr_cNObjid, item_ptr,0,0,0);
	if ( EVEN(sts)) return sts;
	i++;

	/* Create a menu item for the maxlimit */
	sprintf( text, "Maxlimit:      %f", item_ptr->maxlimit);
	sts = rtt_menu_list_add( &menulist, i, 0, text, 
		0, 0, 0,
		pwr_cNObjid, item_ptr,0,0,0);
	if ( EVEN(sts)) return sts;
	i++;

	/* Create a menu item for the maxlimit */
	sprintf( text, "Minlimit:      %f", item_ptr->minlimit);
	sts = rtt_menu_list_add( &menulist, i, 0, text, 
		0, 0, 0,
		pwr_cNObjid, item_ptr,0,0,0);
	if ( EVEN(sts)) return sts;
	i++;

	/* Create a menu item for the database */
	sprintf( text, "Database:      %s", item_ptr->database);
	sts = rtt_menu_list_add( &menulist, i, 0, text, 
		0, 0, 0,
		pwr_cNObjid, item_ptr,0,0,0);
	if ( EVEN(sts)) return sts;
	i++;

	/* Create a menu item for the declaration */
	sprintf( text, "Declaration:   %s", item_ptr->declaration);
	sts = rtt_menu_list_add( &menulist, i, 0, text, 
		0, 0, 0,
		pwr_cNObjid, item_ptr,0,0,0);
	if ( EVEN(sts)) return sts;
	i++;

	sts = dtt_menu_new( (menu_ctx) parent_ctx, &menulist, title, 0, 
			RTT_MENUTYPE_DYN);
	if ( sts == RTT__FASTBACK) return sts;
	else if (EVEN(sts)) return sts;

	return RTT__SUCCESS;
}

/*************************************************************************
*
* Name:		dtt_upd_item_show()
*
* Type		int
*
* Type		Parameter	IOGF	Description
* unsigned long	parent_ctx	I	parent rtt context.
* int		arg1		I	
* int		arg2		I	
* int		arg3		I	
* int		arg4		I	
*
* Description:
*	Displays the content of a menu item.
*
**************************************************************************/

static int	dtt_menu_item_show( 	menu_ctx	ctx,
					int		arg1,
					int		arg2,
					int		arg3,
					int		arg4)
{
	int		sts;
	char		title[80];
	char		text[80];
	rtt_t_menu	*menulist = 0;
	int		i;
	rtt_t_menu	*menu_ptr;

	menu_ptr = ctx->menu;
	menu_ptr += ctx->current_item;

	strcpy( title, MENU_ATTRIBUTES_TITLE);


	i = 0;
	/* Create a menu item for the index */
	strcpy( text, "Text:    ");
	strncat( text, menu_ptr->text, sizeof(text)-strlen(text)-1);
	sts = rtt_menu_list_add( &menulist, i, 0, text, 
		0, 0, 0,
		pwr_cNObjid, menu_ptr,0,0,0);
	if ( EVEN(sts)) return sts;
	i++;

	/* Create a menu item for the type */
	switch ( menu_ptr->type)
	{
	  case DTT_MENUITEM_EDIT:
	    sprintf( text, "Type:    Picture");
	    break;
	  case DTT_MENUITEM_MENU:
	    sprintf( text, "Type:    Menu");
	    break;
	  case DTT_MENUITEM_OBJECTS:
	    sprintf( text, "Type:    Database");
	    break;
	  case DTT_MENUITEM_EXIT:
	    sprintf( text, "Type:    Exit");
	    break;
	  case DTT_MENUITEM_PIFUNC:
	    sprintf( text, "Type:    Function picture");
	    break;
	  case DTT_MENUITEM_SYSPICTURE:
	    sprintf( text, "Type:    System picture");
	    break;
	  case DTT_MENUITEM_OBJPICTURE:
	    sprintf( text, "Type:    Object picture");
	    break;
	  case DTT_MENUITEM_VMSCOMMAND:
	    sprintf( text, "Type:    VMS command");
	    break;
	  case DTT_MENUITEM_KEYS:
	    sprintf( text, "Type:    Function keys");
	    break;
	  case DTT_MENUITEM_COMMAND:
	    sprintf( text, "Type:    Command");
	    break;
	  case DTT_MENUITEM_COMMANDHOLD:
	    sprintf( text, "Type:    Command hold");
	    break;
	  case DTT_MENUITEM_VMSCOMMANDHOLD:
	    sprintf( text, "Type:    VMS command hold");
	    break;
	  case DTT_MENUITEM_VMSCOMMANDCONF:
	    sprintf( text, "Type:    VMS command confirm");
	    break;
	  case DTT_MENUITEM_VMSCOMMANDNOWAIT:
	    sprintf( text, "Type:    VMS command wait");
	    break;
	  case DTT_MENUITEM_FILESPEC:
	    sprintf( text, "Type:    File specification");
	    break;
	  case DTT_MENUITEM_PIPERM :
	    sprintf( text, "Type:    Permanent picture");
	    break;
	  case DTT_MENUITEM_PIFUNCPERM:
	    sprintf( text, "Type:    Permanent function picture");
	    break;
	}
	sts = rtt_menu_list_add( &menulist, i, 0, text, 
		0, 0, 0,
		pwr_cNObjid, menu_ptr,0,0,0);
	if ( EVEN(sts)) return sts;
	i++;

	/* Create a menu item for the index */
	sprintf( text, "Index:   %d", menu_ptr->index);
	sts = rtt_menu_list_add( &menulist, i, 0, text, 
		0, 0, 0,
		pwr_cNObjid, menu_ptr,0,0,0);
	if ( EVEN(sts)) return sts;
	i++;

	switch ( menu_ptr->type)
	{
	  case DTT_MENUITEM_EDIT:
	  case DTT_MENUITEM_PIPERM:
	  case DTT_MENUITEM_MENU:
	  case DTT_MENUITEM_OBJECTS:
	  case DTT_MENUITEM_EXIT:
	  case DTT_MENUITEM_KEYS:
	    break;

	  case DTT_MENUITEM_COMMAND:
	  case DTT_MENUITEM_COMMANDHOLD:
	  case DTT_MENUITEM_VMSCOMMAND:
	  case DTT_MENUITEM_VMSCOMMANDHOLD:
	  case DTT_MENUITEM_VMSCOMMANDCONF:
	  case DTT_MENUITEM_VMSCOMMANDNOWAIT:
	    /* Create a menu item for the command */
	    strcpy( text, "Command: ");
	    strncat( text, (char *)menu_ptr->arg1, sizeof(text)-strlen(text)-1);
	    sts = rtt_menu_list_add( &menulist, i, 0, text, 
		0, 0, 0,
		pwr_cNObjid, menu_ptr,0,0,0);
	    if ( EVEN(sts)) return sts;
	    i++;
	    break;

	  case DTT_MENUITEM_FILESPEC:
	    /* Create a menu item for the command */
	    strcpy( text, "Command: ");
	    strncat( text, (char *)menu_ptr->arg2, sizeof(text)-strlen(text)-1);
	    sts = rtt_menu_list_add( &menulist, i, 0, text, 
		0, 0, 0,
		pwr_cNObjid, menu_ptr,0,0,0);
	    if ( EVEN(sts)) return sts;
	    i++;

	    /* Create a menu item for the file specification */
	    strcpy( text, "File spec: ");
	    strncat( text, (char *)menu_ptr->arg1, sizeof(text)-strlen(text)-1);
	    sts = rtt_menu_list_add( &menulist, i, 0, text, 
		0, 0, 0,
		pwr_cNObjid, menu_ptr,0,0,0);
	    if ( EVEN(sts)) return sts;
	    i++;
	    break;

	  case DTT_MENUITEM_OBJPICTURE:
	    /* Create a menu item for the picture */
	    strcpy( text, "Object picture: ");
	    strncat( text, (char *)menu_ptr->arg1, sizeof(text)-strlen(text)-1);
	    sts = rtt_menu_list_add( &menulist, i, 0, text, 
		0, 0, 0,
		pwr_cNObjid, menu_ptr,0,0,0);
	    if ( EVEN(sts)) return sts;
	    i++;

	    /* Create a menu item for the name */
	    strcpy( text, "Object name: ");
	    strncat( text, (char *)menu_ptr->arg2, sizeof(text)-strlen(text)-1);
	    sts = rtt_menu_list_add( &menulist, i, 0, text, 
		0, 0, 0,
		pwr_cNObjid, menu_ptr,0,0,0);
	    if ( EVEN(sts)) return sts;
	    i++;
	    break;

	  case DTT_MENUITEM_SYSPICTURE:
	    /* Create a menu item for the picture */
	    strcpy( text, "System picture: ");
	    strncat( text, (char *)menu_ptr->arg1, sizeof(text)-strlen(text)-1);
	    sts = rtt_menu_list_add( &menulist, i, 0, text, 
		0, 0, 0,
		pwr_cNObjid, menu_ptr,0,0,0);
	    if ( EVEN(sts)) return sts;
	    i++;
	    break;

	  case DTT_MENUITEM_PIFUNC:
	  case DTT_MENUITEM_PIFUNCPERM:
	    /* Create a menu item for function */
	    strcpy( text, "Function: ");
	    strncat( text, (char *)menu_ptr->arg1, sizeof(text)-strlen(text)-1);
	    sts = rtt_menu_list_add( &menulist, i, 0, text, 
		0, 0, 0,
		pwr_cNObjid, menu_ptr,0,0,0);
	    if ( EVEN(sts)) return sts;
	    i++;
	    break;
	}

	sts = dtt_menu_new( ctx, &menulist, title, 0, 
			RTT_MENUTYPE_DYN);
	if ( sts == RTT__FASTBACK) return sts;
	else if (EVEN(sts)) return sts;

	return RTT__SUCCESS;
}


/*************************************************************************
*
* Name:		dtt_menu_new()
*
* Type		int
*
* Type		Parameter	IOGF	Description
* unsigned long	parent_ctx	I	parents rtt context.
* rtt_t_menu	**menu_p	I	menu list.
* char		*title		I	menu title.
* unsigned long	userdata	I	...
* unsigned long	flag		I	menu type
*
* Description:
*	Create a new menu. The only difference between dtt_menu_new and
*	rtt_menu_new is the commandtable in get_command and the collection
*	picture stuff.
*
**************************************************************************/

int	dtt_menu_new( 	menu_ctx	parent_ctx,
			rtt_t_menu	**menu_p,
			char		*title,
			unsigned long	userdata,
			unsigned long	flag)
{
	rtt_t_menu	*menu;
	menu_ctx	ctx;
	unsigned long	terminator;
	char		input_str[80];
	int		maxlen = 30;
	unsigned long	option;
	rtt_t_menu	*menu_ptr;
	int		sts;
	unsigned long	menutype;
	
	if ( EVEN( dtt_get_stored_menuctx( &ctx, (int) userdata)) )
	{
	  menu = *menu_p;
	  menutype = RTT_MENUTYPE_MENU | flag;
	  sts = rtt_menu_create_ctx( &ctx, parent_ctx, menu, title, menutype);
	  if ( EVEN(sts)) return sts;
	}
	rtt_menu_configure( ctx);
	dtt_menu_draw( ctx);
	dtt_menu_select( ctx);
	menu_ptr = ctx->menu;

	/* Store the root menu context */
	if ( dtt_root_ctx == 0)
	  dtt_root_ctx = ctx;	

	option = RTT_OPT_NORECALL | RTT_OPT_NOEDIT | RTT_OPT_NOECHO;

	while (1)
	{
	  rtt_command_get_input_string( (char *) &rtt_chn, input_str, 
		&terminator, maxlen, rtt_recallbuff, option, 0, 0, 
		ctx,
		NULL, RTT_COMMAND_PICTURE);
/*
	  rtt_get_input_string( &rtt_chn, &input_str, &terminator, maxlen, 
		rtt_recallbuff, option, 0, NULL, 0, NULL);
*/
	  rtt_message('S',"");

	  if ( terminator == RTT_K_ARROW_UP)
	  {
	    dtt_menu_unselect( ctx);
	    rtt_get_next_item_up( ctx);
	    dtt_menu_select( ctx);
	  } 
	  if ( terminator == RTT_K_ARROW_DOWN)
	  {
	    dtt_menu_unselect( ctx);
	    rtt_get_next_item_down( ctx);
	    dtt_menu_select( ctx);
	  } 
	  if ( terminator == RTT_K_ARROW_LEFT)
	  {
	    dtt_menu_unselect( ctx);
	    rtt_get_next_item_left( ctx);
	    dtt_menu_select( ctx);
	  } 
	  if ( terminator == RTT_K_ARROW_RIGHT)
	  {
	    dtt_menu_unselect( ctx);
	    rtt_get_next_item_right( ctx);
	    dtt_menu_select( ctx);
	  } 
	  if ( terminator == RTT_K_NEXTPAGE)
	  {
	    /* Next page */
	    sts = rtt_get_next_page( ctx);	
	    if ( ODD(sts))
	    {
	      dtt_menu_draw( ctx);
	      dtt_menu_select( ctx);
	    }
	  } 
	  if ( terminator == RTT_K_PREVPAGE)
	  {
	    /* Previous page */
	    sts = rtt_get_previous_page( ctx);	
	    if ( ODD(sts))
	    {
	      dtt_menu_draw( ctx);
	      dtt_menu_select( ctx);
	    }
	  } 
	  if ( terminator == RTT_K_RETURN)
	  {
	    if ( (menu_ptr + ctx->current_item)->func != NULL)
	    {
	      sts = ((menu_ptr + ctx->current_item)->func) ( ctx,
		(menu_ptr + ctx->current_item)->arg1,
		(menu_ptr + ctx->current_item)->arg2,
		(menu_ptr + ctx->current_item)->arg3,
		(menu_ptr + ctx->current_item)->arg4);
	      if ( EVEN(sts)) return sts;
	      menu_ptr = ctx->menu;
	      if ( sts == RTT__FASTBACK)
	      {
	        if ( ctx->parent_ctx != 0)
	        {	
/*	          rtt_menu_delete(ctx);
*/
	          return RTT__FASTBACK;
	        }
	      }
	      if ( sts != RTT__NOPICTURE)
	      {
	        dtt_menu_draw( ctx);
	        dtt_menu_select( ctx);
	      }
	    }
	    else 
	      rtt_message('E', "Function not defined");
	  } 
	  if ( terminator == RTT_K_PF1)
	  {
	    if ( (menu_ptr + ctx->current_item)->func2 != NULL)
	    {
	      sts = ((menu_ptr + ctx->current_item)->func2) ( ctx,
		(menu_ptr + ctx->current_item)->arg1,
		(menu_ptr + ctx->current_item)->arg2,
		(menu_ptr + ctx->current_item)->arg3,
		(menu_ptr + ctx->current_item)->arg4);
	      if ( EVEN(sts)) return sts;
	      if ( sts == RTT__FASTBACK)
	      {
	        if ( ctx->parent_ctx != 0)
	        {	
/*	          rtt_menu_delete(ctx);
*/
	          return RTT__FASTBACK;
	        }
	      }
	      if ( sts != RTT__NOPICTURE)
	      {
	        dtt_menu_draw( ctx);
	        dtt_menu_select( ctx);
	      }
	    } 
	    else 
	      rtt_message('E', "Function not defined");
	  } 
	  if ( terminator == RTT_K_PF2)
	  {
	    if ( (menu_ptr + ctx->current_item)->func3 != NULL)
	    {
	      sts = ((menu_ptr + ctx->current_item)->func3) ( ctx,
		(menu_ptr + ctx->current_item)->arg1,
		(menu_ptr + ctx->current_item)->arg2,
		(menu_ptr + ctx->current_item)->arg3,
		(menu_ptr + ctx->current_item)->arg4);
	      if ( EVEN(sts)) return sts;
	      if ( sts == RTT__FASTBACK)
	      {
	        if ( ctx->parent_ctx != 0)
	        {	
/*	          rtt_menu_delete(ctx);
*/
	          return RTT__FASTBACK;
	        }
	      }
	      if ( sts != RTT__NOPICTURE)
	      {
	        dtt_menu_draw( ctx);
	        dtt_menu_select( ctx);
	      }
	    } 
	    else 
	      rtt_message('E', "Function not defined");
	  } 
	  if ( terminator == RTT_K_PF3)
	  {
	    rtt_message('E', "Function not defined");
	  } 
	  if ( terminator == RTT_K_PF4)
	  {
	    if ( ctx->parent_ctx != 0)
	    {	
/*	      rtt_menu_delete(ctx);
*/
	      break;
	    }
	  } 
	  if ( terminator == RTT_K_CTRLZ)
	  {
	    if ( ctx->parent_ctx != 0)
	    {	
	      return RTT__FASTBACK;
	    }
	    else
	    {
	      dtt_menu_unselect( ctx);
	      ctx->current_page = 0;
	      ctx->current_item = 0;
	      dtt_menu_select( ctx);
	    }
	  }
	  if ( terminator == RTT_K_CTRLW)
	  {
	    dtt_menu_draw( ctx);
	    dtt_menu_select( ctx);
	  }
	  if ( terminator == RTT_K_DELETE)
	  {
	    /* Delete current item */
	    /* Check that this is a dynamic menu */
	    if ( ctx->menutype && RTT_MENUTYPE_DYN)
	    {
	      /* Delete current item */
	      rtt_menu_item_delete( ctx, ctx->current_item);
	      /* Reconfigure the menu */
	      rtt_menu_configure( ctx);
	      dtt_menu_draw( ctx);
	      dtt_menu_select( ctx);
	    }
	    else
	      rtt_message('E',"Unable to delete an item in a static menu");
	  }
	  if ( terminator == RTT_K_COMMAND)
	  {
	    sts = rtt_get_command( ctx, (char *) &rtt_chn, rtt_recallbuff,  0,
		 0, ctx, 
	    	"rtt_edit> ", 0, RTT_ROW_COMMAND, dtt_command_table);
	    if ( EVEN(sts)) return sts;
	    /* menu_ptr, prefix, title might have been changed */
	    menu_ptr = ctx->menu;
	    strcpy( rtt_title_prefix, dtt_title_prefix);
	    strcpy( dtt_root_ctx->title, dtt_maintitle);
	    if ( sts == RTT__FASTBACK)
	    {
	      if ( ctx->parent_ctx != 0)
	      {	
/*	        rtt_menu_delete(ctx);
*/
	        return RTT__FASTBACK;
	      }
	    }
	    if ( sts == RTT__BACK)
	    {
	      if ( ctx->parent_ctx != 0)
	      {	
/*	        rtt_menu_delete(ctx);
*/
	        return RTT__SUCCESS;
	      }
	    }
	    if ( sts != RTT__NOPICTURE)
	    {
	      dtt_menu_draw( ctx);
	      dtt_menu_select( ctx);
	    } 
	  } 
	  if ( terminator == RTT_K_HELP)
	  {
	    if ((ctx->parent_ctx != 0) && 
			(((edit_ctx) (ctx->parent_ctx))->ctx_type == DTT_CTX_EDIT))
	      rtt_help( ctx, "UPDATE PARAMETERS", rtt_edit_helptext);
	    else
	      rtt_help( ctx, "MENU", rtt_edit_helptext);

	    dtt_menu_draw( ctx);
	    dtt_menu_select( ctx);
	  } 
	}

	return RTT__SUCCESS;
}

/*************************************************************************
*
* Name:		dtt_number_sort()
*
* Type		int
*
* Type		Parameter	IOGF	Description
* rtt_t_menu	*menulist	I	menulist.
*
* Description:
*	This function sorts the update items after number.
*
**************************************************************************/

static int	dtt_number_sort( dtt_item	itemlist)
{
	int	i, j, size;
	int	number1, number2;
	dtt_t_upd_item 	dum;
	dtt_item	item_ptr;

	/* Get the size of the menu */
	item_ptr = itemlist;
	size = 0;
	if ( itemlist == 0)
	  return RTT__SUCCESS;

	while ( item_ptr->number != 0)
	{
	  item_ptr++;
	  size++;
	}

	for ( i = size - 1; i > 0; i--)
	{
	  item_ptr = itemlist;
	  for ( j = 0; j < i; j++)
	  {
	    number1 = item_ptr->number;
	    number2 = (item_ptr + 1)->number;
	    if ( number2 == 0)
	      break;
	    if ( number1 > number2)
	    {
	      /* Change order */
	      memcpy( &dum, item_ptr + 1, sizeof( dtt_t_upd_item));
	      memcpy( item_ptr + 1, item_ptr, sizeof( dtt_t_upd_item));
	      memcpy( item_ptr, &dum, sizeof( dtt_t_upd_item));
	    }
	    item_ptr++;
	  }
	}
	return RTT__SUCCESS;
}


/*************************************************************************
*
* Name:		dtt_item_delete()
*
* Type		int
*
* Type		Parameter	IOGF	Description
* menu_ctx	ctx		I	
* dtt_item	item_ptr	I	
*
* Description:
*	Delete an update item.
*
**************************************************************************/

static int	dtt_item_delete(	dtt_item	itemlist,
					dtt_item	item_ptr)
{
	dtt_item	item_end;
	dtt_item	item_next;
	int		size;

	/* Get the size of itemlist */
	item_end = itemlist;
	while ( item_end->number != 0)
	  item_end++;

	size = (char *)item_end - (char *)item_ptr;
	item_next = item_ptr + 1;	
	memcpy( item_ptr, item_next, size);	

	return RTT__SUCCESS;
}

/*************************************************************************
*
* Name:		dtt_edit_select()
*
* Type		int
*
* Type		Parameter	IOGF	Description
* menu_ctx	ctx		I	
*
* Description:
*	Set select.
*
**************************************************************************/

static int	dtt_edit_select( edit_ctx	ctx)
{
	ctx->select_x = ctx->cursor_x;
	ctx->select_y = ctx->cursor_y;
	ctx->select_active = 1;
	rtt_edit_draw_section( &(ctx->chartable), ctx->cursor_x,
		ctx->cursor_y, ctx->cursor_x, ctx->cursor_y, 1);
	return RTT__SUCCESS;
}

/*************************************************************************
*
* Name:		dtt_edit_unselect()
*
* Type		int
*
* Type		Parameter	IOGF	Description
* menu_ctx	ctx		I	
*
* Description:
*	Set unselect.
*
**************************************************************************/

static int	dtt_edit_unselect( edit_ctx	ctx)
{
	if ( ctx->select_active == 0)
	  return RTT__SUCCESS;
	
	ctx->select_active = 0;
	rtt_edit_draw_section( &(ctx->chartable), ctx->cursor_x,
		ctx->cursor_y, ctx->select_x, ctx->select_y, 0);
	dtt_edit_draw_itemlist( ctx);

	return RTT__SUCCESS;
}

/*************************************************************************
*
* Name:		rtt_edit_draw_section()
*
* Type		int
*
* Type		Parameter	IOGF	Description
* rtt_t_backgr	*chartable	I	character table of picture.
*
* Description:
*	Draw a section of background picture.
*
**************************************************************************/

static int	rtt_edit_draw_section( 	rtt_t_backgr	*chartable,
					int		x1,
					int		y1,
					int		x2,
					int		y2,
					int		inverse)
{
	int		i, j;
	unsigned long	current_charset;
	unsigned long	current_inverse;
	int		current_x = 0;
	int		dum;
	int		c0, c1;

	if ( y1 > y2)
	{
	  dum = y1;
	  y1 = y2;
	  y2 = dum;
	}
	if ( x1 > x2)
	{
	  dum = x1;
	  x1 = x2;
	  x2 = dum;
	}
	rtt_charset_ascii();
	rtt_char_inverse_end();
	current_charset = RTT_CHARSET_ASCII;
	current_inverse = RTT_CHARSET_NOINVERSE;
	rtt_cursor_abs( x1 + 1, y1 + 1);
	for ( i = y1; i < y2 + 1; i++)
	{
	  for ( j = x1; j < x2 + 1; j++)
	  {
	    if ( j < 80 && i < 22)
	    {
	      c0 = (*chartable)[0][ j][ i];
	      c1 = (*chartable)[1][ j][ i];
	      if ( c0 == 0)
	        c0 = ' ';
	      /* Set cursor if you really have to */
	      if ( current_x++ != j)
	      {
	        rtt_cursor_abs( j + 1, i + 1);
	        current_x = j + 1;
	      }
	      if (( c1 & RTT_CHARSET_LINE) &&
	         ( current_charset != RTT_CHARSET_LINE))	         
	      {
	        rtt_charset_linedrawing();
	        current_charset = RTT_CHARSET_LINE;
	      }
	      else if ( !(c1 & RTT_CHARSET_LINE) &&
	         ( current_charset != RTT_CHARSET_ASCII))	         
	      {
	        rtt_charset_ascii();
	        current_charset = RTT_CHARSET_ASCII;
	      }
	      if ( !inverse )
	      {
	        if (( c1 & RTT_CHARSET_INVERSE) &&
	         ( current_inverse != RTT_CHARSET_INVERSE))	         
	        {
	          rtt_char_inverse_start();
	          current_inverse = RTT_CHARSET_INVERSE;
	        }
	        else if ( !(c1 & RTT_CHARSET_INVERSE) &&
	         ( current_inverse != RTT_CHARSET_NOINVERSE))	         
	        {
	          rtt_char_inverse_end();
	          current_inverse = RTT_CHARSET_NOINVERSE;
	        }
	      }
	      else
	      {
	        if (!(c1 & RTT_CHARSET_INVERSE) &&
	         ( current_inverse != RTT_CHARSET_INVERSE))	         
	        {
	          rtt_char_inverse_start();
	          current_inverse = RTT_CHARSET_INVERSE;
	        }
	        else if ( (c1 & RTT_CHARSET_INVERSE) &&
	         ( current_inverse != RTT_CHARSET_NOINVERSE))	         
	        {
	          rtt_char_inverse_end();
	          current_inverse = RTT_CHARSET_NOINVERSE;
	        }
	      }
	      r_print("%c", c0);
	    }
	  }
	}
	rtt_charset_ascii();
	rtt_char_inverse_end();
	return RTT__SUCCESS;
}


/*************************************************************************
*
* Name:		rtt_edit_delete_section()
*
* Type		int
*
* Type		Parameter	IOGF	Description
* rtt_t_backgr	*chartable	I	character table of picture.
*
* Description:
*	Delete a section of background picture.
*
**************************************************************************/

static int	dtt_edit_delete_section(rtt_t_backgr	*chartable,
					dtt_item	itemlist,
					int		x1,
					int		y1,
					int		x2,
					int		y2)
{
	int		i, j;
	int		dum;
	dtt_item	item_ptr;
	int		sts;

	if ( y1 > y2)
	{
	  dum = y1;
	  y1 = y2;
	  y2 = dum;
	}
	if ( x1 > x2)
	{
	  dum = x1;
	  x1 = x2;
	  x2 = dum;
	}
	for ( i = y1; i < y2 + 1; i++)
	{
	  for ( j = x1; j < x2 + 1; j++)
	  {
	    {
	      (*chartable)[0][ j][ i] = 0;
	      (*chartable)[1][ j][ i] = 0;
	      sts = dtt_get_upd_item( itemlist, j, i, &item_ptr);
	      if ( ODD(sts))
	        sts = dtt_item_delete( itemlist, item_ptr);
	    }
	  }
	}
	return RTT__SUCCESS;
}

/*************************************************************************
*
* Name:		dtt_paste_insert()
*
* Type		int
*
* Type		Parameter	IOGF	Description
* rtt_t_backgr	*chartable	I	character table of picture.
*
* Description:
*	copy a section of background picture to paste buffer.
*
**************************************************************************/

static int	dtt_paste_insert(	rtt_t_backgr	*chartable,
					dtt_item	itemlist,
					int		x1,
					int		y1,
					int		x2,
					int		y2)
{
	int		sts;
	int		i, j;
	int		dum;
	dtt_item	item_ptr;

	/* Clear the paste itemlist */
	if ( dtt_paste_itemlist != 0)
	{
	  free ( dtt_paste_itemlist);
	  dtt_paste_itemlist = 0;
	}

	if ( y1 > y2)
	{
	  dum = y1;
	  y1 = y2;
	  y2 = dum;
	}
	if ( x1 > x2)
	{
	  dum = x1;
	  x1 = x2;
	  x2 = dum;
	}
	dtt_paste_size_x = x2 - x1 + 1;
	dtt_paste_size_y = y2 - y1 + 1;
	for ( i = y1; i < y2 + 1; i++)
	{
	  for ( j = x1; j < x2 + 1; j++)
	  {
	    {
	      dtt_paste_chartable[0][ j - x1][ i - y1] = 
			(*chartable)[0][ j][ i];
	      dtt_paste_chartable[1][ j - x1][ i - y1] = 
			(*chartable)[1][ j][ i];
	      sts = dtt_get_upd_item( itemlist, j, i, &item_ptr);
	      if ( ODD(sts))
	        dtt_paste_item_insert( item_ptr, j - x1, i - y1);
	    }
	  }
	}
	return RTT__SUCCESS;
}

/*************************************************************************
*
* Name:		dtt_paste_copy()
*
* Type		int
*
* Type		Parameter	IOGF	Description
* rtt_t_backgr	*chartable	I	character table of picture.
*
* Description:
*	copy a section of background picture to paste buffer.
*
**************************************************************************/

static int	dtt_paste_copy( rtt_t_backgr	*chartable,
				dtt_item	*itemlist_ptr,
				int		x,
				int		y)
{
	int		sts;
	int		i, j;
	dtt_item	item_ptr;

	/* Remove all items in the paste area */
	item_ptr = *itemlist_ptr;
	while ( item_ptr && item_ptr->number != 0)
	{
	  if ( item_ptr->x >= x && item_ptr->x < x + dtt_paste_size_x &&
	       item_ptr->y >= y && item_ptr->y < y + dtt_paste_size_y)
	  {
	     sts = dtt_item_delete( *itemlist_ptr, item_ptr);
	     item_ptr--;
	  }
	  item_ptr++;
	}

	for ( i = 0; i < dtt_paste_size_y; i++)
	{
	  for ( j = 0; j < dtt_paste_size_x; j++)
	  {
	    if ( i + y < 22 && j + x < 80)
	    {
	      (*chartable)[0][ j + x ][ i + y] = 
	 		dtt_paste_chartable[0][ j ][ i ];
	      (*chartable)[1][ j + x ][ i + y] = 
	 		dtt_paste_chartable[1][ j ][ i ];
	      sts = dtt_get_upd_item( dtt_paste_itemlist, j, i, &item_ptr);
	      if ( ODD(sts))
	        dtt_paste_item_copy( item_ptr, itemlist_ptr, j + x, i + y);
	    }
	  }
	}

	return RTT__SUCCESS;
}


/*************************************************************************
*
* Name:		dtt_paste_item_insert()
*
* Type		int
*
* Type		Parameter	IOGF	Description
* dtt_item	item		I	item.
*
* Description:
*	Insert an update item in paste.
*
**************************************************************************/

static int	dtt_paste_item_insert(	dtt_item	item,
					int		x,
					int		y)
{
	int		sts;
	dtt_item	item_ptr;

	/* Insert the buffer in the upd item list */
	sts = dtt_upd_item_add( &dtt_paste_itemlist, 0, 0, "");
	item_ptr = dtt_paste_itemlist;
	while ( item_ptr->number != 0)
	  item_ptr++;
	item_ptr--;

	memcpy( item_ptr, item, sizeof( dtt_t_upd_item));
	item_ptr->x = x;
	item_ptr->y = y;
	return RTT__SUCCESS;
}


/*************************************************************************
*
* Name:		dtt_paste_item_copy()
*
* Type		int
*
* Type		Parameter	IOGF	Description
* dtt_item	item		I	item.
*
* Description:
*	Insert an update item in paste.
*
**************************************************************************/

static int	dtt_paste_item_copy(	dtt_item	paste_item,
					dtt_item	*itemlist_ptr,
					int		x,
					int		y)
{
	dtt_item	item_ptr;
	int		sts;

	/* Insert the buffer in the upd item list */
	sts = dtt_upd_item_add( itemlist_ptr, 0, 0, "");
	item_ptr = *itemlist_ptr;
	while ( item_ptr->number != 0)
	  item_ptr++;
	item_ptr--;

	memcpy( item_ptr, paste_item, sizeof( dtt_t_upd_item));
	item_ptr->x = x;
	item_ptr->y = y;
	return RTT__SUCCESS;
}



/*************************************************************************
*
* Name:		dtt_menu_write()
*
* Type		int
*
* Type		Parameter	IOGF	Description
*
* Description:
*	Write the menues.
*
**************************************************************************/

static int	dtt_edit_save_menues(	char		*filename,
					int		generate_only)
{
	int		sts;
	FILE		*fout;
	FILE		*fout_c;
	FILE		*fout_h;
	FILE		*fout_db1;
	FILE		*fout_db2;
	FILE		*fout_decl;
	FILE		*fout_hlp = 0;
	char		fname[200];
	char		fnamebld_noext[200];
	char		fname_noext[200];
	char		cmd[200];
	char		tmpstr[80];
	int		nocompile = 0;

	if ( dtt_is_rttsys && !generate_only)
	  nocompile = 1;

	sprintf( fname, "%s%s.dtt_m", dtt_source_dir, filename);
	sprintf( fname_noext, "%s%s", dtt_source_dir, filename);
	sprintf( fnamebld_noext, "%s%s", dtt_build_dir, filename);

	/* Open the datafile and write */
	if ( !generate_only)
	{
	  fout = fopen( fname, "w");

	  if ( fout == 0)
	    return 0;
	}

	if ( !nocompile)
	{
	  /* Export help file */
#if defined(OS_VMS)
	  sprintf( cmd, "copy /nolog %s.rhlp %s.rhlp", fname_noext, 
		fnamebld_noext); 
#elif defined(OS_LYNX)
	  sprintf( cmd, "cp %s.rhlp %s.rhlp", fname_noext, 
		fnamebld_noext); 
#elif defined(OS_LINUX)
	  sprintf( cmd, "cp -f %s.rhlp %s.rhlp", fname_noext, 
		fnamebld_noext); 
#endif
	  system( cmd);

	  /* Create a c file */
	  strcpy( fname, fnamebld_noext);
	  strcat( fname, ".c");
	  fout_c = fopen( fname, "w");

	  /* Create a h file for the picture include files */
	  strcpy( fname, fnamebld_noext);
	  strcat( fname, ".rh");
	  fout_h = fopen( fname, "w");

	  /* Create db files for the rtt local database */
	  strcpy( fname, fnamebld_noext);
	  strcat( fname, ".rdb1");
	  fout_db1 = fopen( fname, "w");
	  strcpy( fname, fnamebld_noext);
	  strcat( fname, ".rdb2");
	  fout_db2 = fopen( fname, "w");

	  /* Create declaratio file for applications function prototypes */
	  strcpy( fname, fnamebld_noext);
	  strcat( fname, ".rdecl");
	  fout_decl = fopen( fname, "w");

	  /* Create a help file if there is no */
	  strcpy( fname, fname_noext);
	  strcat( fname, ".rhlp");
	  sts = rtt_search_file( fname, tmpstr, 1);
	  if ( EVEN(sts))
	  {
	    /* There is no help file create one */
	    fout_hlp = fopen( fname, "w");
	    fprintf( fout_hlp, "\
/* HELP FILE FOR %s */\n\
\n\
\n\
RTT_HELP_START\n\
/* Put the help texts for your menues an pictures here */\n\
\n\
RTT_HELP_SUBJ(\"%s\")\n\
RTT_HELP_INFO(\"Select a menu item with the arrow keys and press RETURN\")\n\
RTT_HELP_TEXT(\"Pwr_rtt shows information about the realtime database.\")\n\
\n\
RTT_HELP_END\n",
	    dtt_programname, dtt_maintitle);

	    fclose( fout_hlp);
	  }

	  /* C file: write include files for rtt, help and pictures */
	  fprintf( fout_c, "\
#include \"rt_rtt_menu.h\"\n");

	  if ( !dtt_is_rttsys)
	    /* Write rttsys database */
	    fprintf( fout_c, "\
#include \"dtt_appl_rttsys_m.rdecl\"\n");

	  if ( !dtt_function_menues_exist())
	  {
	    /* Declare the application database here */
	    fprintf( fout_c, "\
RTT_DB_START\n\
RTT_DB_CONTINUE\n\
RTT_DB_END\n");
	  }

	  fprintf( fout_c, "\
#include \"%s.rdecl\"\n\
#include \"%s.rhlp\"\n\
#include \"%s.rh\"\n", filename, filename, filename);
	}

	/* Datafile: Start to write the current index and the main title */
	if ( !generate_only)
	{
	  fprintf( fout, "%ld\n", dtt_current_index);
	  fprintf( fout, "%d\n", dtt_opsys);
	  fprintf( fout, "%s/%s\n", dtt_maintitle, dtt_title_prefix);
	}
	/* Write the root menu */
	sts = dtt_edit_save_one_menu( fout, fout_c, fout_h, fout_db1, 
		fout_db2, fout_decl, dtt_root_ctx, 0, generate_only, 
		nocompile);

	/* close the files */
	if ( !generate_only)
	  fclose(fout);
	if ( !nocompile)
	{
	  fclose(fout_c);
	  fclose(fout_h);
	  fclose(fout_db1);
	  fclose(fout_db2);
	  fclose(fout_decl);

	}

	return RTT__SUCCESS;
}

/*************************************************************************
*
* Name:		dtt_edit_save_one_menu()
*
* Type		int
*
* Type		Parameter	IOGF	Description
*
* Description:
*	Write the menues.
*
**************************************************************************/

static int	dtt_edit_save_one_menu(	FILE		*fout,
					FILE		*fout_c,
					FILE		*fout_h,
					FILE		*fout_db1,
					FILE		*fout_db2,
					FILE		*fout_decl,
					menu_ctx	ctx,
					int		index,
					int		generate_only,
					int		nocompile)
{
	int		sts;
	menu_ctx	childctx;
	rtt_t_menu	*menu_ptr;
	char		picture_name[80];	
	char		menu_name[80];	
	char		item_menu_name[80];	
	char		fname[200];
	FILE		*fout_test = 0;
	char		tmpstr[200];

	menu_ptr = ctx->menu;

	while ( menu_ptr->text[0] != 0)
	{
	  if ( !generate_only)
	  {
	    if ( menu_ptr != ctx->menu )
	      fprintf( fout, "next\n");

	    /* Print the text, type, and index */
	    fprintf( fout, "%s\n", menu_ptr->text);
	    fprintf( fout, "%d\n", menu_ptr->type);
	    fprintf( fout, "%d\n", menu_ptr->index);
	    /* Print command if this is a command item */
	    if (( menu_ptr->type == DTT_MENUITEM_COMMAND) ||
	        ( menu_ptr->type == DTT_MENUITEM_COMMANDHOLD) ||
	        ( menu_ptr->type == DTT_MENUITEM_VMSCOMMAND) ||
	        ( menu_ptr->type == DTT_MENUITEM_VMSCOMMANDNOWAIT) ||
	        ( menu_ptr->type == DTT_MENUITEM_VMSCOMMANDCONF) ||
	        ( menu_ptr->type == DTT_MENUITEM_VMSCOMMANDHOLD) ||
	        ( menu_ptr->type == DTT_MENUITEM_SYSPICTURE) ||
	        ( menu_ptr->type == DTT_MENUITEM_PIFUNCPERM) ||
	        ( menu_ptr->type == DTT_MENUITEM_PIFUNC))
	    {
	      fprintf( fout, "%s\n", (char *) menu_ptr->arg1);
	    }
	    if ( menu_ptr->type == DTT_MENUITEM_OBJPICTURE)
	    {
	      fprintf( fout, "%s\n", (char *) menu_ptr->arg1);
	      fprintf( fout, "%s\n", (char *) menu_ptr->arg2);
	    }
	    if ( menu_ptr->type == DTT_MENUITEM_FILESPEC)
	    {
	      fprintf( fout, "%s\n", (char *) menu_ptr->arg1);
	      fprintf( fout, "%s\n", (char *) menu_ptr->arg2);
	    }
	  }
	  /* Look for a submenu to this item */
	  if ( ODD( dtt_get_stored_menuctx( &childctx, menu_ptr->index)) )
	  {
	    if ( !generate_only)
	      fprintf( fout, "down\n");
	    /* Write this menu */
	    sts = dtt_edit_save_one_menu( fout, fout_c, fout_h, fout_db1,
			fout_db2, fout_decl, 
			childctx, menu_ptr->index, generate_only, nocompile);
	    if ( EVEN(sts)) return sts;
	    if ( !generate_only)
	      fprintf( fout, "up\n");
	  }
	  menu_ptr++;
	}

	if ( nocompile)
	  return RTT__SUCCESS;


	/* c file: write the current menu */
	if ( ctx == dtt_root_ctx)
	  /* Main menu */
	  fprintf( fout_c, "RTT_MAINMENU_START( \"%s/%s\")\n", 
		dtt_maintitle, dtt_title_prefix);	
	else
	{
	  dtt_get_menuname( index, menu_name);
	  fprintf( fout_c, "RTT_MENU_START( %s)\n", menu_name);	
	}
	menu_ptr = ctx->menu;
	while ( menu_ptr->text[0] != 0)
	{
	  if ( menu_ptr->type == DTT_MENUITEM_MENU)
	  {
	    dtt_get_menuname( menu_ptr->index, item_menu_name);
	    if ( menu_ptr->func != 0)
	      fprintf( fout_c, 
		"RTT_MENU_NEW( \"%s\", %s)\n", menu_ptr->text, item_menu_name);	
	    else
	      /* No submenu is created */
	      fprintf( fout_c, "RTT_MENU_NO( \"%s\")\n", menu_ptr->text);	
	  }
	  else if ( menu_ptr->type == DTT_MENUITEM_KEYS)
	  {
	    dtt_get_menuname( menu_ptr->index, item_menu_name);
	    if ( menu_ptr->func != 0)
	      fprintf( fout_c, 
		"RTT_MENU_KEYS_NEW( \"%s\", %s)\n", menu_ptr->text, item_menu_name);	
	    else
	      /* No submenu is created */
	      fprintf( fout_c, "RTT_MENU_NO( \"%s\")\n", menu_ptr->text);	
	  }
	  else if ( menu_ptr->type == DTT_MENUITEM_EDIT ||
	            menu_ptr->type == DTT_MENUITEM_PIPERM)
	  {
	    dtt_get_picturename( menu_ptr->index, item_menu_name);
	    if ( menu_ptr->type == DTT_MENUITEM_EDIT)
	      fprintf( fout_c, 
		"RTT_MENU_UPEDIT_NEW( \"%s\", %s, NULL)\n", 
		menu_ptr->text, item_menu_name);	
	    else
	      fprintf( fout_c, 
		"RTT_MENU_UPEDITPERM_NEW( \"%s\", %s, NULL)\n", 
		menu_ptr->text, item_menu_name);	
	    /* h file: write in include for this picture file */
	    fprintf( fout_h, "#if defined(OS_ELN)\n");
	    fprintf( fout_h, "globalref rtt_t_menu_update %s_eu[];\n", 
		item_menu_name);
	    fprintf( fout_h, "globalref rtt_t_backgr %s_bg;\n", item_menu_name);
	    fprintf( fout_h, "globalref rtt_t_menu_update *%s_euptr;\n", 
		item_menu_name);
	    fprintf( fout_h, "#else\n");
	    fprintf( fout_h, "extern rtt_t_menu_update %s_eu[];\n", 
		item_menu_name);
	    fprintf( fout_h, "extern rtt_t_backgr %s_bg;\n", item_menu_name);
	    fprintf( fout_h, "extern rtt_t_menu_update *%s_euptr;\n", 
		item_menu_name);
	    fprintf( fout_h, "#endif\n");

	    /* db1 and db2 file: write include files for database */
	    dtt_get_picturefilename( menu_ptr->index, picture_name);
	    fprintf( fout_db1, "#include \"%s.rdb1\"\n", picture_name);
	    fprintf( fout_db2, "#include \"%s.rdb2\"\n", picture_name);
	  }
	  else if ( menu_ptr->type == DTT_MENUITEM_PIFUNC || 
	  	    menu_ptr->type == DTT_MENUITEM_PIFUNCPERM)
	  {
	    dtt_get_picturename( menu_ptr->index, item_menu_name);
	    if ( menu_ptr->type == DTT_MENUITEM_PIFUNC)
	      fprintf( fout_c,
		"RTT_MENU_UPEDIT_NEW( \"%s\", %s, &%s)\n",
		menu_ptr->text, item_menu_name, (char *) menu_ptr->arg1);
	    else
	      fprintf( fout_c,
		"RTT_MENU_UPEDITPERM_NEW( \"%s\", %s, &%s)\n", 
		menu_ptr->text, item_menu_name, (char *) menu_ptr->arg1);	
	    /* h file: write in include for this picture file */
	    fprintf( fout_h, "#if defined(OS_ELN)\n");
	    fprintf( fout_h, "globalref rtt_t_menu_update %s_eu[];\n",item_menu_name);
	    fprintf( fout_h, "globalref rtt_t_backgr %s_bg;\n", item_menu_name);
	    fprintf( fout_h, "globalref rtt_t_menu_update *%s_euptr;\n", 
		item_menu_name);
	    fprintf( fout_h, "#else\n");
	    fprintf( fout_h, "extern rtt_t_menu_update %s_eu[];\n",item_menu_name);
	    fprintf( fout_h, "extern rtt_t_backgr %s_bg;\n", item_menu_name);
	    fprintf( fout_h, "extern rtt_t_menu_update *%s_euptr;\n", 
		item_menu_name);
	    fprintf( fout_h, "#endif\n");

	    /* db1 and db2 file: write include files for database */
	    dtt_get_picturefilename( menu_ptr->index, picture_name);
	    fprintf( fout_db1, "#include \"%s.rdb1\"\n", picture_name);
	    fprintf( fout_db2, "#include \"%s.rdb2\"\n", picture_name);

	    /* decl file: write a function prototype declaration */
	    fprintf( fout_decl, "int %s();\n", (char *) menu_ptr->arg1);

	    if ( !dtt_is_rttsys)
	    {
	      /* Check that the application function file exists, if it doesn't 
		 create it */
	      sprintf( fname, "%sra_rtt_%s.c", dtt_source_dir, 
			dtt_programname);
	      sts = rtt_search_file( fname, tmpstr, 1);
	      if ( EVEN(sts))
	      {
	        fout_test = fopen( fname, "w");
	        dtt_get_menufilename( fname);
	        fprintf( fout_test, "\
/************************************************************************\n\
*************************************************************************\n\
*\n\
*		 P S S - 9 0 0 0\n\
*		=================\n\
*************************************************************************\n\
*\n\
* Filename:		RT_RTT_%s.C\n\
*			Date	Pgm.	Read.	Remark\n\
* Modified					Initial creation\n\
*\n\
* Description:\n\
*	Application functions in rtt.\n\
*\n\
**************************************************************************\n\
**************************************************************************/\n\
\n\
/*_Include files_________________________________________________________*/\n\
#include \"rt_rtt_menu.h\"\n\
#include \"rt_rtt_msg.h\"\n\
\n\
/*_Local rtt database____________________________________________________*/\n\
RTT_DB_START\n\
#include \"%s.rdb1\"\n\
RTT_DB_CONTINUE\n\
#include \"%s.rdb2\"\n\
RTT_DB_END\n\
\n\
/*************************************************************************\n\
*\n\
* Name:		pwrp_rtt:%s\n\
*\n\
* Type		int\n\
*\n\
* Type		Parameter	IOGF	Description\n\
* menu_ctx	ctx		I	context of the picture.\n\
* int		event		I 	type of event.\n\
* char		*parameter_ptr	I	pointer to the parameter which value\n\
*					has been changed.\n\
*\n\
* Description:\n\
*	Application function of a picture.\n\
*\n\
**************************************************************************/\n\
\n\
int %s ( ctx, event, parameter_ptr)\n\
menu_ctx	ctx;\n\
int		event;\n\
char		*parameter_ptr;\n\
{ \n\
\n\
", dtt_programname, fname, fname, (char *)menu_ptr->arg1, (char *)menu_ptr->arg1);
	      fprintf( fout_test, "\
  switch ( event)\n\
  {\n\
    /**********************************************************\n\
    *	Initialization of the picture\n\
    ***********************************************************/\n\
    case RTT_APPL_INIT:\n\
      break;\n\
    /**********************************************************\n\
    *	Udate of the picture\n\
    ***********************************************************/\n\
    case RTT_APPL_UPDATE:\n\
      break;\n\
    /**********************************************************\n\
    *	Exit of the picture\n\
    ***********************************************************/\n\
    case RTT_APPL_EXIT:\n\
      break;\n\
    /**********************************************************\n\
    *	The value of a parameter is changed.\n\
    ***********************************************************/\n\
    case RTT_APPL_VALUECHANGED:\n\
      break;\n\
    /**********************************************************\n\
    *	Next page key activated.\n\
    ***********************************************************/\n\
    case RTT_APPL_NEXTPAGE:\n\
      break;\n\
    /**********************************************************\n\
    *	Pevious page key activated.\n\
    ***********************************************************/\n\
    case RTT_APPL_PREVPAGE:\n\
      break;\n\
  }\n\
  return RTT__SUCCESS;\n\
}\n");

	        fclose( fout_test);
	      }
	    }
	  }
	  else if ( menu_ptr->type == DTT_MENUITEM_SYSPICTURE)
	  {
	    fprintf( fout_c, 
		"RTT_MENU_SYSEDIT_NEW( \"%s\", 0, &%s)\n", 
		menu_ptr->text, (char *) menu_ptr->arg1);	
	  }
	  else if ( menu_ptr->type == DTT_MENUITEM_OBJPICTURE)
	  {
	    fprintf( fout_c, 
		"RTT_MENU_SYSEDIT_NEW( \"%s\", \"%s\", &%s)\n", 
		menu_ptr->text, (char *) menu_ptr->arg2, (char *) menu_ptr->arg1);	
	  }
	  else if ( menu_ptr->type == DTT_MENUITEM_FILESPEC)
	  {
	    fprintf( fout_c, 
		"RTT_MENUITEM_FILECOMMAND( \"%s\", \"%s\", \"%s\")\n", 
		menu_ptr->text, (char *) menu_ptr->arg1, (char *) menu_ptr->arg2);	
	  }
	  else if ( menu_ptr->type == DTT_MENUITEM_EXIT)
	  {
	    fprintf( fout_c, "RTT_MENUITEM_EXIT(\"%s\")\n", menu_ptr->text);	
	  }
	  else if ( menu_ptr->type == DTT_MENUITEM_OBJECTS)
	  {
	    fprintf( fout_c, "RTT_MENUITEM_OBJECTS(\"%s\")\n", 
		menu_ptr->text);	
	  }
	  else if ( menu_ptr->type == DTT_MENUITEM_COMMAND)
	  {
	    fprintf( fout_c, "RTT_MENUITEM_COMMAND(\"%s\", \"%s\")\n", 
		menu_ptr->text, (char *) menu_ptr->arg1);	
	  }
	  else if ( menu_ptr->type == DTT_MENUITEM_COMMANDHOLD)
	  {
	    fprintf( fout_c, "RTT_MENUITEM_COMMANDHOLD(\"%s\", \"%s\")\n", 
		menu_ptr->text, (char *) menu_ptr->arg1);	
	  }
	  else if ( menu_ptr->type == DTT_MENUITEM_VMSCOMMAND)
	  {
	    fprintf( fout_c, "RTT_MENUITEM_VMSCOMMAND(\"%s\", \"%s\")\n", 
		menu_ptr->text, (char *) menu_ptr->arg1);	
	  }
	  else if ( menu_ptr->type == DTT_MENUITEM_VMSCOMMANDNOWAIT)
	  {
	    fprintf( fout_c, "RTT_MENUITEM_VMSCOMMAND_NOWAIT(\"%s\", \"%s\")\n", 
		menu_ptr->text, (char *) menu_ptr->arg1);	
	  }
	  else if ( menu_ptr->type == DTT_MENUITEM_VMSCOMMANDCONF)
	  {
	    fprintf( fout_c, "RTT_MENUITEM_VMSCOMMANDCONF(\"%s\", \"%s\")\n", 
		menu_ptr->text, (char *) menu_ptr->arg1);	
	  }
	  else if ( menu_ptr->type == DTT_MENUITEM_VMSCOMMANDHOLD)
	  {
	    fprintf( fout_c, "RTT_MENUITEM_VMSCOMMANDHOLD(\"%s\", \"%s\")\n", 
		menu_ptr->text , (char *) menu_ptr->arg1);	
	  }
	  menu_ptr++;
	}
	if ( ctx == dtt_root_ctx)
	  fprintf( fout_c, "RTT_MAINMENU_END\n");
	else
	  fprintf( fout_c, "RTT_MENU_END( %s)\n", menu_name);	

	return RTT__SUCCESS;
}


/*************************************************************************
*
* Name:		dtt_edit_read_menues()
*
* Type		int
*
* Type		Parameter	IOGF	Description
*
* Description:
*	Read the menues.
*
**************************************************************************/

static int	dtt_edit_read_menues(	char		*filename)
{
	int		sts;
	FILE		*fin;
	char		*s;
	char		fname[200];
	char		line[80];

	sprintf( fname, "%s%s.dtt_m", dtt_source_dir, filename);

	/* Open and read */
	fin = fopen( fname, "r");

	if ( fin == 0)
	  return 0;

	/* Start to read the current index and maintitle*/
	sts = dtt_read_line( line, sizeof(line), fin);
	if ( EVEN(sts)) return sts;
	sts = sscanf( line, "%ld", &dtt_current_index);
	if ( sts != 1) return 0;
	sts = dtt_read_line( line, sizeof(line), fin);
	if ( EVEN(sts)) return sts;
	sts = sscanf( line, "%d", &dtt_opsys);
	if ( sts != 1) 
        { 
	  /* Not converted to V3.0 */ 
	}
	else
	{
	  sts = dtt_read_line( dtt_maintitle, sizeof(dtt_maintitle), fin);
	  if ( EVEN(sts)) return sts;
	}
	if ( (s = strchr( dtt_maintitle, '/')))
	{
	  *s = 0;
	  strcpy( dtt_title_prefix, s + 1);
	  strcpy( rtt_title_prefix, dtt_title_prefix);
	}
	else
	{
	  strcpy( dtt_title_prefix, "");
	  strcpy( rtt_title_prefix, dtt_title_prefix);
	}
	/* Read the root menu */
	sts = dtt_edit_read_one_menu( fin, 0, 0);
	if (EVEN(sts)) return sts;

	fclose(fin);
	return RTT__SUCCESS;
}

/*************************************************************************
*
* Name:		dtt_edit_read_one_menu()
*
* Type		int
*
* Type		Parameter	IOGF	Description
*
* Description:
*	Reads the menues.
*
**************************************************************************/

static int	dtt_edit_read_one_menu(	FILE		*fin,
					menu_ctx	parent_ctx,
					int		parent_item)
{
	int		sts;
	menu_ctx	ctx;
	rtt_t_menu	*menu_ptr;
	rtt_t_menu	*menulist = 0;
	rtt_t_menu	*parent_menu_ptr;
	long int       	index;
	int		type;
	int		first = 0;
	char		text[80];
	char		title[80];
	char		line[80];
	char		command[80];
	char		command2[80];
	char		*command_ptr;
	int		i = 0;
	char		*parent_text;

	if ( parent_ctx != 0)
	{
	  parent_menu_ptr = parent_ctx->menu;
	  parent_menu_ptr += parent_item;
	}	
	sts = dtt_read_line( text, sizeof(text), fin);
	while ( 1)
	{
	  sts = dtt_read_line( line, sizeof(line), fin);
	  sts = sscanf( line, "%d", &type);
	  sts = dtt_read_line( line, sizeof(line), fin);
	  sts = sscanf( line, "%ld", &index);
	  /* If command type read the command */
	  if (( type == DTT_MENUITEM_COMMAND) ||
	      ( type == DTT_MENUITEM_COMMANDHOLD) ||
	      ( type == DTT_MENUITEM_VMSCOMMAND) ||
	      ( type == DTT_MENUITEM_VMSCOMMANDNOWAIT) ||
	      ( type == DTT_MENUITEM_VMSCOMMANDCONF) ||
	      ( type == DTT_MENUITEM_VMSCOMMANDHOLD) ||
	      ( type == DTT_MENUITEM_SYSPICTURE) ||
	      ( type == DTT_MENUITEM_PIFUNCPERM) ||
	      ( type == DTT_MENUITEM_PIFUNC))
	  {
	    sts = dtt_read_line( command, sizeof(command), fin);
	  }
	  if ( type == DTT_MENUITEM_OBJPICTURE)
	  {
	    sts = dtt_read_line( command, sizeof(command), fin);
	    sts = dtt_read_line( command2, sizeof(command2), fin);
	  }
	  if ( type == DTT_MENUITEM_FILESPEC)
	  {
	    sts = dtt_read_line( command, sizeof(command), fin);
	    sts = dtt_read_line( command2, sizeof(command2), fin);
	  }

	  if ( !first)
	  {
	    first = 1;

	    /* Create a new menu */
	    sts = rtt_menu_list_add( &menulist, i, 0, text, 
			0, 0, 0, pwr_cNObjid, 0,0,0,0);
	    if ( EVEN(sts)) return sts;

	    /* Set type and index */
	    menu_ptr = menulist;
	    menu_ptr->index = index;
	    menu_ptr->arg3 = (void *) index;
	    menu_ptr->type = type;
	    menu_ptr->func2 = &dtt_menu_item_show;

	    if ( type == DTT_MENUITEM_EDIT ||
	         type == DTT_MENUITEM_PIPERM)
	    {
	      menu_ptr->func = &dtt_edit_new;
	    }
	    else if (( type == DTT_MENUITEM_COMMAND) ||
	    	     ( type == DTT_MENUITEM_COMMANDHOLD) ||
	    	     ( type == DTT_MENUITEM_VMSCOMMAND) ||
	    	     ( type == DTT_MENUITEM_VMSCOMMANDCONF) ||
	    	     ( type == DTT_MENUITEM_VMSCOMMANDNOWAIT) ||
	    	     ( type == DTT_MENUITEM_VMSCOMMANDHOLD) ||
	    	     ( type == DTT_MENUITEM_SYSPICTURE))
	    {
	      /* Put a pointer to the command in arg1 */
	      command_ptr = calloc(  80, sizeof( char));
	      strcpy( command_ptr, command);
	      menu_ptr->arg1 = command_ptr;
	    }
	    else if ( type == DTT_MENUITEM_OBJPICTURE ||
	              type == DTT_MENUITEM_FILESPEC)
	    {
	      /* Put a pointer to the command in arg1 */
	      command_ptr = calloc(  80, sizeof( char));
	      strcpy( command_ptr, command);
	      menu_ptr->arg1 = command_ptr;
	      command_ptr = calloc(  80, sizeof( char));
	      strcpy( command_ptr, command2);
	      menu_ptr->arg2 = command_ptr;
	    }
	    else if ( type == DTT_MENUITEM_PIFUNC ||
	              type == DTT_MENUITEM_PIFUNCPERM)
	    {
	      /* Put a pointer to the function name in arg1 */
	      command_ptr = calloc(  80, sizeof( char));
	      strcpy( command_ptr, command);
	      menu_ptr->arg1 = command_ptr;
	      menu_ptr->func = &dtt_edit_new;
	    }
	    
	    if ( parent_ctx != 0)
	      strcpy( title, parent_menu_ptr->text);
	    else
	      strcpy( title, dtt_maintitle);

	    /* Create a context */
	    sts = rtt_menu_create_ctx( &ctx, parent_ctx, menulist,
			title,
			RTT_MENUTYPE_MENU | RTT_MENUTYPE_DYN);
	    if ( EVEN(sts)) return sts;

	    /* Store the context */
	    if ( parent_ctx == 0)
	    {
	      dtt_root_ctx = ctx;
	      dtt_store_menuctx( ctx, 1);
	    }
	    else
	      dtt_store_menuctx( ctx, parent_menu_ptr->index);
	  }
	  else
	  {
	    /* Add an item to the menulist */
	    sts = rtt_menu_list_add( &(ctx->menu), i, 0, text, 
			0, 0, 0, pwr_cNObjid, 0,0,0,0);
	    if ( EVEN(sts)) return sts;

	    /* Set type and index */
	    menu_ptr = ctx->menu;
	    menu_ptr += i;
	    menu_ptr->index = index;
	    menu_ptr->arg3 = (void *) index;
	    menu_ptr->type = type;
	    menu_ptr->func2 = &dtt_menu_item_show;

	    if ( type == DTT_MENUITEM_EDIT ||
	         type == DTT_MENUITEM_PIPERM)
	    {
	      menu_ptr->func = &dtt_edit_new;
	    }
	    else if (( type == DTT_MENUITEM_COMMAND) ||
	             ( type == DTT_MENUITEM_COMMANDHOLD) ||
	             ( type == DTT_MENUITEM_VMSCOMMAND) ||
	             ( type == DTT_MENUITEM_VMSCOMMANDCONF) ||
	             ( type == DTT_MENUITEM_VMSCOMMANDNOWAIT) ||
	             ( type == DTT_MENUITEM_VMSCOMMANDHOLD) ||
	             ( type == DTT_MENUITEM_SYSPICTURE))
	    {
	      /* Put a pointer to the command in arg1 */
	      command_ptr = calloc(  80, sizeof( char));
	      strcpy( command_ptr, command);
	      menu_ptr->arg1 = command_ptr;
	    }
	    else if ( type == DTT_MENUITEM_OBJPICTURE ||
		      type == DTT_MENUITEM_FILESPEC)
	    {
	      /* Put a pointer to the command in arg1 */
	      command_ptr = calloc(  80, sizeof( char));
	      strcpy( command_ptr, command);
	      menu_ptr->arg1 = command_ptr;
	      command_ptr = calloc(  80, sizeof( char));
	      strcpy( command_ptr, command2);
	      menu_ptr->arg2 = command_ptr;
	    }
	    else if ( type == DTT_MENUITEM_PIFUNC ||
	              type == DTT_MENUITEM_PIFUNCPERM)
	    {
	      /* Put a pointer to the function name in arg1 */
	      command_ptr = calloc(  80, sizeof( char));
	      strcpy( command_ptr, command);
	      menu_ptr->arg1 = command_ptr;
	      menu_ptr->func = &dtt_edit_new;
	    }
	  }	    
	  /* Read next item */
	  sts = dtt_read_line( text, sizeof(text), fin);
	  if ( EVEN(sts))
	     break;
	  if ( strcmp( text, "down") == 0)
	  {
	    /* Down to a submenu */
	    sts = dtt_edit_read_one_menu( fin, ctx, i);
	    if ( EVEN(sts)) return sts;
	    sts = dtt_read_line( text, sizeof(text), fin);
	    if ( EVEN(sts))
	       break;
	  }
	  if ( strcmp( text, "up") == 0)
	  {
	    break;
	  }
	  if ( strcmp( text, "next") == 0)
	  {
	    sts = dtt_read_line( text, sizeof(text), fin);
	    if ( EVEN(sts))
	       break;
	  }
          i++;
	}	
	/* Configure */
	rtt_menu_configure( ctx);

	/* Set function in parent item */
	if ( parent_ctx != 0)
	{
	  parent_text = calloc(  80, sizeof( char));
	  strcpy( parent_text, parent_menu_ptr->text);
	  parent_menu_ptr->arg1 = parent_text;
	  parent_menu_ptr->func = &dtt_menu_new;
	}
	return RTT__SUCCESS;
}
/*************************************************************************
*
* Name:		dtt_get_menu_name()
*
* Type		int
*
* Type		Parameter	IOGF	Description
*
* Description:
*	Write the menues.
*
**************************************************************************/
#if 0
static int	dtt_get_menu_name(	int		index,
					char		*name)
{
	sprintf( name, "dttappl__u%d", index);
	return RTT__SUCCESS;
}
#endif
/*************************************************************************
*
* Name:		dtt_start()
*
* Type		int
*
* Type		Parameter	IOGF	Description
*
* Description:
*	Write the menues.
*
**************************************************************************/

int	dtt_start( char		*programname)
{
	int	sts;
	char	filename[80];
	rtt_t_menu	*menulist;
	char		first_item[80];
	char		*s;

	strcpy( dtt_programname, programname);	
	dtt_is_rttsys = (( strcmp( dtt_programname, "RTTSYS") == 0) ||
	                 ( strcmp( dtt_programname, "rttsys") == 0));


	/* Figure out the current opsys */
#if defined(OS_VMS)
	if ( strcmp( rtt_hw, "axp") ==0)
	  dtt_current_opsys = pwr_mOpSys_AXP_VMS;
	else
	  dtt_current_opsys = pwr_mOpSys_VAX_VMS;
#elif defined(OS_LYNX)
	if ( strcmp( rtt_hw, "x86") == 0)
	  dtt_current_opsys = pwr_mOpSys_X86_LYNX;
	else
	  dtt_current_opsys = pwr_mOpSys_PPC_LYNX;
#elif defined(OS_LINUX)
	if ( strcmp( rtt_hw, "x86") == 0)
	  dtt_current_opsys = pwr_mOpSys_X86_LINUX;
	else if ( strcmp( rtt_hw, "x86_64") == 0)
	  dtt_current_opsys = pwr_mOpSys_X86_LINUX;
	else
	  dtt_current_opsys = pwr_mOpSys_PPC_LINUX;
#endif	  

	/* Create path for source and build directories */
#if defined(OS_VMS)
	if ( dtt_is_rttsys)
	{
	  sprintf( dtt_build_dir, "pwrb_bldroot:[lib.dtt]");
	  strcpy( dtt_source_dir, "pwr_root:[src.lib.dtt.src]");
	  strcpy( dtt_exe_dir, "pwr_exe:");
	}
	else
	{
	  strcpy( dtt_source_dir, "pwrp_rtt:");
	  strcpy( dtt_build_dir, "pwrp_rttbld:");
	  strcpy( dtt_exe_dir, "pwr_exe:");
	}
#elif defined(OS_LYNX) || defined(OS_LINUX)
	if ( dtt_is_rttsys)
	{
	  if ( (s = getenv( "pwre_sroot")) == NULL)
	  {
	    printf( "** Rtt source directory pwrp_rtt is not defined\n");
	    dtt_exit_now(0);
	  }
	  sprintf( dtt_source_dir, "%s/lib/dtt/src/", s);
	  if ( (s = getenv( "pwre_broot")) == NULL)
	  {
	    printf( "** Rtt build directory pwrp_rttbld is not defined\n");
	    dtt_exit_now(0);
	  }
	  sprintf( dtt_build_dir, "%s/os_%s/hw_%s/bld/lib/dtt/", s, rtt_os,
		rtt_hw);
	  if ( (s = getenv( "pwr_exe")) == NULL)
	  {
	    printf( "** Rtt execute directory pwr_exe is not defined\n");
	    dtt_exit_now(0);
	  }
	  sprintf( dtt_exe_dir, "%s/", s);
	}
	else
	{
	  if ( (s = getenv( "pwrp_rtt")) == NULL)
	  {
	    printf( "** Rtt source directory pwrp_rtt is not defined\n");
	    dtt_exit_now(0);
	  }
	  sprintf( dtt_source_dir, "%s/", s);
	  if ( (s = getenv( "pwrp_rttbld")) == NULL)
	  {
	    printf( "** Rtt build directory pwrp_rttbld is not defined\n");
	    dtt_exit_now(0);
	  }
	  sprintf( dtt_build_dir, "%s/", s);
	  if ( (s = getenv( "pwr_exe")) == NULL)
	  {
	    printf( "** Rtt execute directory pwr_exe is not defined\n");
	    dtt_exit_now(0);
	  }
	  sprintf( dtt_exe_dir, "%s/", s);
	}
#endif

	dtt_get_menufilename( filename);
	sts = dtt_edit_read_menues( filename);
	if ( EVEN(sts))
	{
	  /* New program, create a hook menu */
	  sprintf( dtt_maintitle, "RTT IN %s", dtt_programname);
	  sprintf( first_item, "%s MENU", dtt_programname);
	  sts = rtt_menu_list_add( &menulist, 0, 0, first_item, 
		0, 0, 0,
		pwr_cNObjid, 0,0,0,0);
	  if ( EVEN(sts)) return sts;

	  /* Set type and index */
	  menulist->index = dtt_current_index;
	  menulist->arg3 = (void *) dtt_current_index;
	  menulist->type = DTT_MENUITEM_MENU;
	  menulist->func2 = &dtt_menu_item_show;
	  dtt_current_index++;

	  sts = dtt_menu_new( 0, &menulist, dtt_maintitle, 
		1, RTT_MENUTYPE_DYN); 
	  if ( EVEN(sts)) return sts;
	}
	else
	{
	  sts = dtt_menu_new( 0, &dtt_root_ctx->menu, 
		dtt_root_ctx->title,
	  	1 , RTT_MENUTYPE_DYN); 
	  if ( EVEN(sts)) return sts;
	}
	return RTT__SUCCESS;
}

/*************************************************************************
*
* Name:		dtt_get_picturefilename()
*
* Type		int
*
* Type		Parameter	IOGF	Description
*
* Description:
*	Write the menues.
*
**************************************************************************/

static int	dtt_get_picturefilename(	int		index,
						char		*filename)
{

	sprintf( filename, "dtt_appl_%s_p%d", dtt_programname, index);
	return RTT__SUCCESS;
}

/*************************************************************************
*
* Name:		dtt_get_menufilename()
*
* Type		int
*
* Type		Parameter	IOGF	Description
*
* Description:
*	Write the menues.
*
**************************************************************************/

static int	dtt_get_menufilename( char		*filename)
{

	sprintf( filename, "dtt_appl_%s_m", dtt_programname);
	return RTT__SUCCESS;
}

/*************************************************************************
*
* Name:		dtt_get_menuname()
*
* Type		int
*
* Type		Parameter	IOGF	Description
*
* Description:
*	Write the menues.
*
**************************************************************************/

static int	dtt_get_menuname(	int		index,
					char		*menu_name)
{
	sprintf( menu_name, "dtt_menu_m%d", index);
	return RTT__SUCCESS;
}
/*************************************************************************
*
* Name:		dtt_get_picturename()
*
* Type		int
*
* Type		Parameter	IOGF	Description
*
* Description:
*	Write the menues.
*
**************************************************************************/

static int	dtt_get_picturename(	int		index,
					char		*picture_name)
{

	if ( dtt_is_rttsys)
	  /* this is rttsys */
  	  sprintf( picture_name, "dtt_systempicture_p%d", index);
	else
  	  sprintf( picture_name, "dtt_picture_p%d", index);

	return RTT__SUCCESS;
}

/*************************************************************************
*
* Name:		gcg_cc( )
*
* Type		void
*
* Type		Parameter	IOGF	Description
*
* Description:
*	Create an objectfile from the generated c-code.
*
**************************************************************************/

static int dtt_cc(	int	opsys,
			int	action,
			int	debug)
{ 
	char	cmd[100];

#if defined(OS_VMS)
	sprintf( cmd, "@%swb_rtt_appl %s %d %d %d", dtt_exe_dir, 
		dtt_programname, action, opsys, debug);

#elif defined(OS_LYNX) || defined(OS_LINUX)
	sprintf( cmd, "%s/wb_rtt_appl.sh %s %d %d %d", dtt_exe_dir, 
		dtt_programname, action, dtt_is_rttsys, opsys);
#endif
	system( cmd);
	return RTT__SUCCESS;
}


/*************************************************************************
*
* Name:		gcg_compile_picture( )
*
* Type		void
*
* Type		Parameter	IOGF	Description
*
* Description:
*	Create an objectfile from the generated c-code from a picture.
*
**************************************************************************/

static int dtt_compile_picture( char	*filename,
				int	opsys)
{ 
	char	cmd[200];
	char	msg[80];
	int	os;

 	if ( !opsys)
	{
	  /* Save current opsys */
	  sprintf( msg, "Saving %s...", dtt_opsys_to_name(dtt_current_opsys));
	  rtt_message('I', msg);
	  rtt_cursor_abs( 0, 21);
	  r_print_buffer();
#if defined(OS_VMS)
	  sprintf( cmd, "@%swb_rtt_comppicture %s %s %d %d", dtt_exe_dir, filename,
		dtt_programname, dtt_is_rttsys, dtt_current_opsys);

	  system( cmd);
#elif defined(OS_LYNX) || defined(OS_LINUX)
	  sprintf( cmd, "%swb_rtt_comppicture.sh %s %s %d %d", dtt_exe_dir, filename,
		dtt_programname, dtt_is_rttsys, dtt_current_opsys);

	  system( cmd);
#endif
	}
	else
	{
	  for ( os = 1; os < pwr_mOpSys_; os = os << 1)
	  {
	    if ( opsys & os)
	    {
	      sprintf( msg, "Saving %s...", dtt_opsys_to_name(os));
	      rtt_message('I', msg);
	      rtt_cursor_abs( 0, 21);
	      r_print_buffer();
#if defined(OS_VMS)
	      sprintf( cmd, "@%swb_rtt_comppicture %s %s %d %d", dtt_exe_dir, filename,
		dtt_programname, dtt_is_rttsys, os);

	      system( cmd);
#elif defined(OS_LYNX) || defined(OS_LINUX)
	      sprintf( cmd, "%swb_rtt_comppicture.sh %s %s %d %d", dtt_exe_dir, filename,
		dtt_programname, dtt_is_rttsys, os);

	      system( cmd);
#endif
	    }
	  }
	}
	return RTT__SUCCESS;
}



/*************************************************************************
*
* Name:		dtt_read_line()
*
* Type		void
*
* Type		Parameter	IOGF	Description
*
* Description:
*	Read a line for a file.
*
**************************************************************************/

static int dtt_read_line(	char	*line,
				int	maxsize,
				FILE	*file)
{ 
	char	*s;

	if (fgets( line, maxsize, file) == NULL)
	  return 0;
	s = strchr( line, 10);
	if ( s != 0)
	  *s = 0;

	return RTT__SUCCESS;
}


/*************************************************************************
*
* Name:		dtt_menu_draw_item()
*
* Type		int
*
* Type		Parameter	IOGF	Description
* menu_ctx	ctx		I	rtt context.
* int		item		I	index of menu item.
*
* Description:
*	Draw a menu item.
*
**************************************************************************/

static int	dtt_menu_draw_item( 	menu_ctx	ctx,
					int		item)
{
	int		x, y, row, col;
	rtt_t_menu	*menu_ptr;
	char		*menu_charptr;

	if ( (item < ctx->current_page * ctx->page_len) ||
	     (item > (ctx->current_page + 1) * ctx->page_len) ||
	     (item > ctx->no_items))
	  return 0;

	col = (item - ctx->current_page * ctx->page_len) / ctx->rows;
	row = item - ctx->current_page * ctx->page_len - col * ctx->rows;
	x = ctx->left_margin + col * ctx->col_size;
	y = (22 - RTT_MENU_MAXROWS) + ctx->up_margin + row * ctx->row_size;
	rtt_cursor_abs( x, y);

	menu_charptr = (char *) ctx->menu;
	if ( ctx->menutype & RTT_MENUTYPE_MENU )
	  menu_charptr += item * sizeof(rtt_t_menu);
	else if ( ctx->menutype & RTT_MENUTYPE_UPD)
	  menu_charptr += item * sizeof(rtt_t_menu_upd);

	menu_ptr = (rtt_t_menu *) menu_charptr;
	r_print("%s", &(menu_ptr->text));
	if ( menu_ptr->type == DTT_MENUITEM_EDIT)
	  r_print(" p");
	else if ( menu_ptr->type == DTT_MENUITEM_PIPERM)
	  r_print(" pp");
	else if ( menu_ptr->type == DTT_MENUITEM_MENU)
	  r_print(" m");
	else if ( menu_ptr->type == DTT_MENUITEM_KEYS)
	  r_print(" k");
	else if ( menu_ptr->type == DTT_MENUITEM_OBJECTS)
	  r_print(" o");
	else if ( menu_ptr->type == DTT_MENUITEM_EXIT)
	  r_print(" e");
	else if ( menu_ptr->type == DTT_MENUITEM_COMMAND)
	  r_print(" c");
	else if ( menu_ptr->type == DTT_MENUITEM_COMMANDHOLD)
	  r_print(" ch");
	else if ( menu_ptr->type == DTT_MENUITEM_VMSCOMMAND)
	  r_print(" v");
	else if ( menu_ptr->type == DTT_MENUITEM_VMSCOMMANDCONF)
	  r_print(" vc");
	else if ( menu_ptr->type == DTT_MENUITEM_VMSCOMMANDNOWAIT)
	  r_print(" vn");
	else if ( menu_ptr->type == DTT_MENUITEM_VMSCOMMANDHOLD)
	  r_print(" vh");
	else if ( menu_ptr->type == DTT_MENUITEM_PIFUNC)
	  r_print(" f");
	else if ( menu_ptr->type == DTT_MENUITEM_PIFUNCPERM)
	  r_print(" fp");
	else if ( menu_ptr->type == DTT_MENUITEM_SYSPICTURE)
	  r_print(" sp");
	else if ( menu_ptr->type == DTT_MENUITEM_OBJPICTURE)
	  r_print(" op");
	else if ( menu_ptr->type == DTT_MENUITEM_FILESPEC)
	  r_print(" fs");
	
	return RTT__SUCCESS;
}


/*************************************************************************
*
* Name:		dtt_menu_draw()
*
* Type		int
*
* Type		Parameter	IOGF	Description
* menu_ctx	ctx		I	menu ctx.
*
* Description:
*	Draw a menu.
*
**************************************************************************/

static int	dtt_menu_draw( menu_ctx	ctx)
{
	rtt_t_menu	*menu_ptr;
	int		i;

	rtt_display_erase();
	rtt_menu_draw_title( ctx);
	menu_ptr = ctx->menu;
	for ( i = ctx->current_page * ctx->page_len; 
		(i < ctx->no_items) && ( i < (ctx->current_page + 1) *
		ctx->page_len); i++)
	{
	  dtt_menu_draw_item( ctx, i);
	}
	return RTT__SUCCESS;
}

/*************************************************************************
*
* Name:		dtt_menu_select()
*
* Type		int
*
* Type		Parameter	IOGF	Description
* menu_ctx	ctx		I	rtt context.
*
* Description:
*	Draw the current item in inverse mode.
*
**************************************************************************/

static int	dtt_menu_select( menu_ctx	ctx)
{
	rtt_char_inverse_start();
	dtt_menu_draw_item( ctx, ctx->current_item);
	rtt_char_inverse_end();

	return RTT__SUCCESS;
}

/*************************************************************************
*
* Name:		dtt_menu_unselect()
*
* Type		int
*
* Type		Parameter	IOGF	Description
* menu_ctx	ctx		I	
*
* Description:
*	Draw the current item in no inverse mode.
*
**************************************************************************/

static int	dtt_menu_unselect( menu_ctx	ctx)
{
	dtt_menu_draw_item( ctx, ctx->current_item);
	return RTT__SUCCESS;
}




/*************************************************************************
*
* Name:		dtt_edit_process_menues()
*
* Type		int
*
* Type		Parameter	IOGF	Description
*
* Description:
*	Process the menues.
*
**************************************************************************/

static int	dtt_edit_process_menues(
			int		(* func) (),
			unsigned long	arg1,
			unsigned long	arg2,
			unsigned long	arg3,
			unsigned long	arg4,
			unsigned long	arg5)
{
	int		sts;

	/* Process the root menu and all submenues */
	sts = dtt_edit_process_one_menu( func, arg1, arg2, arg3, arg4, arg5, 
		dtt_root_ctx, 0);
	if ( EVEN(sts)) return sts;

	return RTT__SUCCESS;
}

/*************************************************************************
*
* Name:		dtt_edit_process_one_menu()
*
* Type		int
*
* Type		Parameter	IOGF	Description
*
* Description:
*	Write the menues.
*
**************************************************************************/

static int	dtt_edit_process_one_menu(
			int		(* func) (),
			unsigned long  	arg1,
			unsigned long  	arg2,
			unsigned long  	arg3,
			unsigned long  	arg4,
			unsigned long  	arg5,
			menu_ctx	ctx,
			int		index)
{
	int		sts;
	menu_ctx	childctx;
	rtt_t_menu	*menu_ptr;
	int		menu_idx;

	menu_ptr = ctx->menu;

	menu_idx = 0;
	while ( menu_ptr->text[0] != 0)
	{
	  sts = (func) ( ctx, menu_idx, arg1, arg2, arg3, arg4, arg5);
	  if ( EVEN(sts)) return sts;

	  /* Look for a submenu to this item */
	  if ( ODD( dtt_get_stored_menuctx( &childctx, menu_ptr->index)) )
	  {
	    /* Process this menu */
	    sts = dtt_edit_process_one_menu( func, arg1, arg2, arg3, arg4, arg5,
			childctx, menu_ptr->index);
	    if ( EVEN(sts)) return sts;
	  }
	  menu_idx++;
	  menu_ptr++;
	}

	return RTT__SUCCESS;
}

static int	dtt_edit_write_gdhrefs( char *filename)
{
	FILE	*outfile;
	int	sts;
	char	outfilename[80];
	char	message[80];

	/* Open file */
	outfile = fopen( filename, "w");
	if ( !outfile)
	  return RTT__NOFILE;

	sts = dtt_edit_process_menues( &dtt_edit_write_menu_gdhrefs,
		(unsigned long) outfile, 0,0,0,0);
	rtt_fgetname( outfile, outfilename, filename);
	fclose( outfile);
	if ( EVEN(sts)) return sts;

	sprintf( message, "%s created", outfilename);
	rtt_message('I', message);
	return RTT__SUCCESS;

}

static int	dtt_edit_write_menu_gdhrefs( 	menu_ctx ctx, 
					int menu_idx, 
					FILE *outfile, 
					int dum1, 
					int dum2, 
					int dum3, 
					int dum4)
{
	int	sts;
	rtt_t_menu	*menu_ptr;
	edit_ctx	picture_ctx;
	char		picturefilename[80];
	char		*title;
	dtt_item 	item_ptr;
	int		gdh_item_found;

	menu_ptr = ctx->menu;
	menu_ptr += menu_idx;

	/* Which type of menue is this */
	if (
		( menu_ptr->type == DTT_MENUITEM_EDIT) ||
		( menu_ptr->type == DTT_MENUITEM_PIPERM) ||
		( menu_ptr->type == DTT_MENUITEM_PIFUNCPERM) ||
	        ( menu_ptr->type == DTT_MENUITEM_PIFUNC))
	{
	  /* Get this picture */

	  /* Get the title from the parent items text */
	  title = menu_ptr->text;

	  /* Try to find this context, it may be stored */
	  sts = dtt_get_stored_ctx( &picture_ctx, title, menu_ptr->index);
	  if ( EVEN(sts))
	  {
	    /* Not loaded */
	    sts = dtt_create_ctx( &picture_ctx, (edit_ctx) ctx, title, 
			menu_ptr->index);
	    if ( EVEN(sts)) return sts;
	    sts = dtt_store_ctx( picture_ctx, title, menu_ptr->index);
	    if ( EVEN(sts)) return sts;
	    /* Try to read it */
	    dtt_get_picturefilename( picture_ctx->index, picturefilename); 
	    sts = dtt_edit_read( picture_ctx, picturefilename, 0);
	    if ( EVEN(sts)) return sts;
	  }
	  /* Write the items with database gdh */
	  item_ptr = picture_ctx->upd_items;
	  if ( item_ptr != 0)
	  {
	    gdh_item_found = 0;
	    while ( item_ptr->number != 0)
	    {
	      if ( !strcmp( item_ptr->database, "GDH"))
	      {
	        if ( !gdh_item_found)
	        {
	          fprintf( outfile, "%s\n", menu_ptr->text);
	          gdh_item_found = 1;
		}
	        fprintf( outfile, "%d	%s\n", item_ptr->number, 
			item_ptr->parameter);
	        if ( !strncmp( item_ptr->type, "DUAL", 4))
	          fprintf( outfile, "%d	%s\n", item_ptr->number, item_ptr->dualparameter);
	      }
	      item_ptr++;
	    }
	  }
	}
	return RTT__SUCCESS;
}

static int	dtt_edit_pwrplc_gdhrefs( char *filename)
{
	FILE	*outfile;
	int	sts;
	int	externref_index;
	char	outfilename[80];
	char	message[80];

	externref_index = 0;

	/* Open file */
	outfile = fopen( filename, "w");
	if ( !outfile)
	  return RTT__NOFILE;

	fprintf( outfile, "$set verify\n");
	fprintf( outfile, "$pwr_cmd\n");
	fprintf( outfile, "!\n");
	fprintf( outfile, "! Skapa Planthierarki\n");
	fprintf( outfile, "!\n");
	fprintf( outfile, 
		"create object/dest=\"\"/class=$planthier/name=\"RttDum\"\n");
	fprintf( outfile, 
		"create object/dest=rttdum/class=$planthier/name=\"Rtt\"\n");

	sts = dtt_edit_process_menues( &dtt_edit_pwrplc_menu_gdhrefs,
		(unsigned long) outfile, (unsigned long) &externref_index,
		0,0,0);
	fprintf( outfile, "save\n");
	fprintf( outfile, "quit\n");
	fprintf( outfile, "$set noverify\n");
	rtt_fgetname( outfile, outfilename, filename);
	fclose( outfile);
	if ( EVEN(sts)) return sts;

	sprintf( message, "%s created", outfilename);
	rtt_message('I', message);
	return RTT__SUCCESS;

}

static int	dtt_edit_pwrplc_menu_gdhrefs( 	menu_ctx ctx, 
					int menu_idx, 
					FILE *outfile, 
					int *externref_index, 
					int dum2, 
					int dum3, 
					int dum4)
{
	int	sts;
	rtt_t_menu	*menu_ptr;
	edit_ctx	picture_ctx;
	char		picturefilename[80];
	char		*title;
	dtt_item 	item_ptr;
	int		gdh_item_found;
	char		objname[80];
	char		parname[120];
	char		description[130];
	char		*s;

	menu_ptr = ctx->menu;
	menu_ptr += menu_idx;

	/* Which type of menue is this */
	if (
		( menu_ptr->type == DTT_MENUITEM_EDIT) ||
		( menu_ptr->type == DTT_MENUITEM_PIPERM) ||
		( menu_ptr->type == DTT_MENUITEM_PIFUNCPERM) ||
	        ( menu_ptr->type == DTT_MENUITEM_PIFUNC))
	{
	  /* Get this picture */

	  /* Get the title from the parent items text */
	  title = menu_ptr->text;

	  /* Try to find this context, it may be stored */
	  sts = dtt_get_stored_ctx( &picture_ctx, title, menu_ptr->index);
	  if ( EVEN(sts))
	  {
	    /* Not loaded */
	    sts = dtt_create_ctx( &picture_ctx, (edit_ctx) ctx, title, 
			menu_ptr->index);
	    if ( EVEN(sts)) return sts;
	    sts = dtt_store_ctx( picture_ctx, title, menu_ptr->index);
	    if ( EVEN(sts)) return sts;
	    /* Try to read it */
	    dtt_get_picturefilename( picture_ctx->index, picturefilename); 
	    sts = dtt_edit_read( picture_ctx, picturefilename, 0);
	    if ( EVEN(sts)) return sts;
	  }
	  /* Write the items with database gdh */
	  item_ptr = picture_ctx->upd_items;
	  if ( item_ptr != 0)
	  {
	    gdh_item_found = 0;
	    while ( item_ptr->number != 0)
	    {
	      if ( !strcmp( item_ptr->database, "GDH"))
	      {
	        if ( !gdh_item_found)
	        {
	          strcpy( objname, menu_ptr->text);
		  for ( s = objname; *s; s++)
	 	  {
		    if ( !(isalpha( *s) || isdigit( *s))) *s = '_';
	   	  }
		  fprintf( outfile, 
		  "create object/dest=rttdum-rtt/class=$planthier/name=\"%s\"\n",
		  	 objname);
	          gdh_item_found = 1;
		}
		/* Remove attribute from parameter */
		strcpy( parname, item_ptr->parameter);
		if ( (s = strchr( parname, '.')))
		  *s = 0;
		fprintf( outfile, 
"create object/dest=rttdum-rtt-%s/class=ExternRef/name=\"RttExt%d\"\n",
		  	 objname, *externref_index);
		fprintf( outfile, 
"set attr/noco/name=rttdum-rtt-%s-rttext%d/attr=Object/value=%s\n",
		  	 objname, *externref_index, parname);
		if ( (s = strchr( item_ptr->parameter, '.')))
		{
		  s++;
		  sprintf( description, "Num %d Attr %s", item_ptr->number, s);
		}
		else
		  sprintf( description, "Num %d", item_ptr->number);
		description[40] = 0;
		fprintf( outfile, 
"set attr/noco/name=rttdum-rtt-%s-rttext%d/attr=Description/value=\"%s\"\n",
		  	objname, *externref_index, description);
		if ( !(	!strcmp( item_ptr->priv, "NO") ||
			!strcmp( item_ptr->text, "%")) &&
			 strncmp( item_ptr->type, "DUAL", 4))
		{
		  fprintf( outfile, 
"set attr/noco/name=rttdum-rtt-%s-rttext%d/attr=Write/value=1\n",
		  	 objname, *externref_index);
		}
	        (*externref_index)++;

		if ( !strncmp( item_ptr->type, "DUAL", 4))
	 	{
		  /* Create extref for dualparameter also */
	  	  /* Remove attribute from dualparameter */
	  	  strcpy( parname, item_ptr->dualparameter);
		  if ( (s = strchr( parname, '.')))
		    *s = 0;
		  fprintf( outfile, 
"create object/dest=rttdum-rtt-%s/class=ExternRef/name=\"RttExt%d\"\n",
		  	 objname, *externref_index);
		  fprintf( outfile, 
"set attr/noco/name=rttdum-rtt-%s-rttext%d/attr=Object/value=%s\n",
		  	 objname, *externref_index, parname);
		  fprintf( outfile, 
"set attr/noco/name=rttdum-rtt-%s-rttext%d/attr=Write/value=1\n",
		  	 objname, *externref_index);

		  if ( (s = strchr( item_ptr->dualparameter, '.')))
		  {
		    s++;
		    sprintf( description, "Num %d Attr %s", item_ptr->number, s);
		  }
		  else
		    sprintf( description, "Num %d", item_ptr->number);
		  description[40] = 0;

		  fprintf( outfile, 
"set attr/noco/name=rttdum-rtt-%s-rttext%d/attr=Description/value=\"%s\"\n",
		  	objname, *externref_index, description);
	          (*externref_index)++;
		}
	      }
	      item_ptr++;
	    }
	  }
	}
	return RTT__SUCCESS;
}

static int	dtt_edit_list_items_all( char *filename)
{
	FILE	*outfile;
	int	sts;
	char	outfilename[80];
	char	message[80];

	/* Open file */
	outfile = fopen( filename, "w");
	if ( !outfile)
	  return RTT__NOFILE;

	sts = dtt_edit_process_menues( &dtt_edit_list_items_picture,
		(unsigned long) outfile, 0,0,0,0);
	rtt_fgetname( outfile, outfilename, filename);
	fclose( outfile);
	if ( EVEN(sts)) return sts;

	sprintf( message, "%s created", outfilename);
	rtt_message('I', message);
	return RTT__SUCCESS;

}

static int	dtt_edit_list_items_picture( 	menu_ctx ctx, 
					int menu_idx, 
					FILE *outfile, 
					int dum1, 
					int dum2, 
					int dum3, 
					int dum4)
{
	int	sts;
	rtt_t_menu	*menu_ptr;
	edit_ctx	picture_ctx;
	char		picturefilename[80];
	char		*title;

	menu_ptr = ctx->menu;
	menu_ptr += menu_idx;

	/* Which type of menue is this */
	if (
		( menu_ptr->type == DTT_MENUITEM_EDIT) ||
		( menu_ptr->type == DTT_MENUITEM_PIPERM) ||
		( menu_ptr->type == DTT_MENUITEM_PIFUNCPERM) ||
	        ( menu_ptr->type == DTT_MENUITEM_PIFUNC))
	{
	  /* Get this picture */

	  /* Get the title from the parent items text */
	  title = menu_ptr->text;

	  /* Try to find this context, it may be stored */
	  sts = dtt_get_stored_ctx( &picture_ctx, title, menu_ptr->index);
	  if ( EVEN(sts))
	  {
	    /* Not loaded */
	    sts = dtt_create_ctx( &picture_ctx, (edit_ctx) ctx, title, 
			menu_ptr->index);
	    if ( EVEN(sts)) return sts;
	    sts = dtt_store_ctx( picture_ctx, title, menu_ptr->index);
	    if ( EVEN(sts)) return sts;
	    /* Try to read it */
	    dtt_get_picturefilename( picture_ctx->index, picturefilename);
	    sts = dtt_edit_read( picture_ctx, picturefilename, 0);
	    if ( EVEN(sts)) return sts;
	  }
	  /* Write the items */
	  sts = dtt_edit_list_items( picture_ctx, outfile);
	  if ( EVEN(sts)) return sts;
	}
	return RTT__SUCCESS;
}

static int	dtt_edit_list_items( 	edit_ctx	picture_ctx,
					FILE		*outfile)
{
	dtt_item 	item_ptr;
	int		item_found;

	/* List the items */
	item_ptr = picture_ctx->upd_items;
	if ( item_ptr != 0)
	{
	  item_found = 0;
	  while ( item_ptr->number != 0)
	  {
	    if ( !item_found)
	    {
	      fprintf( outfile, 
"\n*******************************************************************\n");
	      fprintf( outfile, "%s\n", picture_ctx->title);
	      fprintf( outfile, 
"*******************************************************************\n");
	      item_found = 1;
	    }
	    fprintf( outfile,      "Number		%d\n", item_ptr->number);
	    fprintf( outfile,      "Text		\"%s\"\n", item_ptr->text);
	    fprintf( outfile,      "Type		%s\n", item_ptr->type);
	    fprintf( outfile,      "Parameter	%s\n", item_ptr->parameter);
	    fprintf( outfile,      "Text/Dualpar	\"%s\"\n", item_ptr->dualparameter);
	    fprintf( outfile,      "Privileges	%s\n", item_ptr->priv);
	    fprintf( outfile,      "Outputflags	%s\n", item_ptr->outflags);
	    fprintf( outfile,      "Characters	%d\n", item_ptr->characters);
	    fprintf( outfile,      "Decimals	%d\n", item_ptr->decimals);
	    fprintf( outfile,      "MaxLimit	%f\n", item_ptr->maxlimit);
	    fprintf( outfile,      "MinLimit	%f\n", item_ptr->minlimit);
	    fprintf( outfile,      "Database	%s\n", item_ptr->database);
	    fprintf( outfile,      "Declaration	%s\n", item_ptr->declaration);
	    fprintf( outfile,      "x,y		%d	%d\n", item_ptr->x,
				item_ptr->y);
	    fprintf( outfile, 
"-------------------------------------------------------------------\n");
	    item_ptr++;
	  }
	}
	return RTT__SUCCESS;
}

static int	dtt_edit_read_items( 	edit_ctx	ctx,
					char		*filename)
{
	dtt_item 	item_ptr;
	FILE		*infile;
	char		line[100];
	int		sts;
	char		attribute[100];
	int		number;
	char		text[80];
	char		type[80];
	char		parameter[80];
	char		text_dualpar[80];
	char		privileges[80];
	char		outputflags[80];
	int		characters;
	int		decimals;
	float		maxlimit;
	float		minlimit;
	char		database[80];
	char		declaration[80];
	int		x;
	int		y;
	int		row;
	int		i;
	int		check_state;
	char		message[80];
	char		value_str[3][80];

	/* Open the file */
	infile = fopen( filename, "r");
	if ( !infile)
	{
	  rtt_message('E', "Unable to open file");
	  return RTT__NOPICTURE;
	}

	check_state = 1;
	for ( i = 0; i < 2; i++)
	{
	  row = 0;
	  while( ODD( sts = dtt_read_line( line, sizeof(line), infile)))
	  {
	    row++;
	    if ( strncmp( line, "Number", 6))
	      /* Read next line */
	      continue;

	    /* Read Number */
	    sts = sscanf( line, "%s%d", attribute, &number);
	    if ( sts != 2) goto error;
	    if ( number < 1) goto error;
	    /* Read Text */
	    strcpy( text, "");
	    sts = dtt_read_line( line, sizeof(line), infile);
	    if ( EVEN(sts)) goto error;
	    row++;
	    sts = rtt_parse( line, " 	", "",
		(char *) value_str, sizeof( value_str)/sizeof( value_str[0]), 
		sizeof( value_str[0]), 0);
	    if ( !(sts == 1 || sts == 2)) goto error;
	    strcpy( attribute, value_str[0]);
	    if ( sts == 2)
	      strcpy( text, value_str[1]);
	    if ( strcmp( attribute, "Text"))
	      goto error;
	    /* Remove '"' in the text string */
	    if ( text[0] == '"')
	      strcpy( text, &text[1]);
	    if ( strlen(text) > 0 && text[strlen(text)-1] == '"')
	      text[strlen(text)-1] = 0;
	    /* Read Type */
	    strcpy( type, "");
	    sts = dtt_read_line( line, sizeof(line), infile);
	    if ( EVEN(sts)) goto error;
	    row++;
	    sts = sscanf( line, "%s%s", attribute, type);
	    if ( !(sts == 1 || sts == 2)) goto error;
	    if ( strcmp( attribute, "Type"))
	      goto error;
	    /* Read Parameter */
	    sts = dtt_read_line( line, sizeof(line), infile);
	    if ( EVEN(sts)) goto error;
	    sts = sscanf( line, "%s%s", attribute, parameter);
	    if ( sts != 2) goto error;
	    if ( strcmp( attribute, "Parameter"))
	      goto error;
	    /* Read Text/Dualpar */
	    strcpy( text_dualpar, "");
	    sts = dtt_read_line( line, sizeof(line), infile);
	    if ( EVEN(sts)) goto error;
	    row++;
	    sts = rtt_parse( line, " 	", "",
		(char *) value_str, sizeof( value_str)/sizeof( value_str[0]), 
		sizeof( value_str[0]), 0);
	    if ( !(sts == 1 || sts == 2)) goto error;
	    strcpy( attribute, value_str[0]);
	    if ( sts == 2)
	      strcpy( text_dualpar, value_str[1]);
	    if ( strcmp( attribute, "Text/Dualpar"))
	      goto error;
	    /* Remove '"' in the text string */
	    if ( text_dualpar[0] == '"')
	      strcpy( text_dualpar, &text_dualpar[1]);
	    if ( strlen(text_dualpar) > 0 && 
		 text_dualpar[strlen(text_dualpar)-1] == '"')
	      text_dualpar[strlen(text_dualpar)-1] = 0;
	    /* Read Priveleges */
	    strcpy( privileges, "");
	    sts = dtt_read_line( line, sizeof(line), infile);
	    if ( EVEN(sts)) goto error;
	    row++;
	    sts = sscanf( line, "%s%s", attribute, privileges);
	    if ( !(sts == 1 || sts == 2)) goto error;
	    if ( strcmp( attribute, "Privileges"))
	      goto error;
	    /* Read Outputflags */
	    strcpy( outputflags, "");
	    sts = dtt_read_line( line, sizeof(line), infile);
	    if ( EVEN(sts)) goto error;
	    row++;
	    sts = sscanf( line, "%s%s", attribute, outputflags);
	    if ( !(sts == 1 || sts == 2)) goto error;
	    if ( strcmp( attribute, "Outputflags"))
	      goto error;
	    /* Read Characters */
	    sts = dtt_read_line( line, sizeof(line), infile);
	    if ( EVEN(sts)) goto error;
	    sts = sscanf( line, "%s%d", attribute, &characters);
	    if ( sts != 2) goto error;
	    if ( strcmp( attribute, "Characters"))
	      goto error;
	    /* Read Decimals */
	    sts = dtt_read_line( line, sizeof(line), infile);
	    if ( EVEN(sts)) goto error;
	    row++;
	    sts = sscanf( line, "%s%d", attribute, &decimals);
	    if ( sts != 2) goto error;
	    if ( strcmp( attribute, "Decimals"))
	      goto error;
	    /* Read MaxLimit */
	    sts = dtt_read_line( line, sizeof(line), infile);
	    if ( EVEN(sts)) goto error;
	    row++;
	    sts = sscanf( line, "%s%f", attribute, &maxlimit);
	    if ( sts != 2) goto error;
	    if ( strcmp( attribute, "MaxLimit"))
	      goto error;
	    /* Read MinLimit */
	    sts = dtt_read_line( line, sizeof(line), infile);
	    if ( EVEN(sts)) goto error;
	    row++;
	    sts = sscanf( line, "%s%f", attribute, &minlimit);
	    if ( sts != 2) goto error;
	    if ( strcmp( attribute, "MinLimit"))
	      goto error;
	    /* Read Database */
	    strcpy( database, "");
	    sts = dtt_read_line( line, sizeof(line), infile);
	    if ( EVEN(sts)) goto error;
	    row++;
	    sts = sscanf( line, "%s%s", attribute, database);
	    if ( !(sts == 1 || sts == 2)) goto error;
	    if ( strcmp( attribute, "Database"))
	      goto error;
	    /* Read Declaration */
	    strcpy( declaration, "");
	    sts = dtt_read_line( line, sizeof(line), infile);
	    if ( EVEN(sts)) goto error;
	    row++;
	    sts = sscanf( line, "%s%s", attribute, declaration);
	    if ( !(sts == 1 || sts == 2)) goto error;
	    if ( strcmp( attribute, "Declaration"))
	      goto error;
	    /* Read x,y */
	    sts = dtt_read_line( line, sizeof(line), infile);
	    if ( EVEN(sts)) goto error;
	    row++;
	    sts = sscanf( line, "%s%d%d", attribute, &x, &y);
	    if ( sts != 3) goto error;
	    if ( strcmp( attribute, "x,y"))
	      goto error;

	    /* Check for collisions */
	    sts = dtt_get_upd_item( ctx->upd_items, x, y, &item_ptr);
	    if ( ODD(sts))
	      goto collision_error;

	    if ( !check_state)
	    {
	      /* Add this item */
	      sts = dtt_upd_item_add( &(ctx->upd_items), x, y, text);
	      if ( EVEN(sts)) return sts;
	      sts = dtt_get_upd_item( ctx->upd_items, x, y, &item_ptr);
	      if ( EVEN(sts)) return sts;
	      item_ptr->number = number;	
	      strcpy( item_ptr->type, type);
	      strcpy( item_ptr->parameter, parameter);
	      strcpy( item_ptr->dualparameter, text_dualpar);
	      strcpy( item_ptr->priv, privileges);
	      strcpy( item_ptr->outflags, outputflags);
	      item_ptr->characters = characters;
	      item_ptr->decimals = decimals;
	      item_ptr->maxlimit = maxlimit;
	      item_ptr->minlimit = minlimit;
	      strcpy( item_ptr->database, database);
	      strcpy( item_ptr->declaration, declaration);	 
	    }
	  }
	  if ( check_state)
	  {
	    rewind( infile);
	    check_state = 0;
	    row = 0;
	  }
 	}
	fclose( infile);
	return RTT__SUCCESS;

error:
	fclose( infile);
	sprintf( message, "Error in line %d, %s", row, line);
	rtt_message('E', message);
	return RTT__NOPICTURE;

collision_error:
	fclose( infile);
	sprintf( message, "Field collision in line %d, %s", row, line);
	rtt_message('E', message);
	return RTT__NOPICTURE;
}

static int	dtt_edit_read_picture_items( 	edit_ctx	ctx,
						FILE		*infile)
{
	dtt_item 	item_ptr;
	char		line[100];
	int		sts;
	char		attribute[100];
	int		number;
	char		text[80];
	char		type[80];
	char		parameter[80];
	char		text_dualpar[80];
	char		privileges[80];
	char		outputflags[80];
	int		characters;
	int		decimals;
	float		maxlimit;
	float		minlimit;
	char		database[80];
	char		declaration[80];
	int		x;
	int		y;
	int		row;
	char		message[80];
	char		value_str[3][80];

	  row = 0;
	  while( ODD( sts = dtt_read_line( line, sizeof(line), infile)))
	  {
	    row++;
	    if ( strncmp( line, "Number", 6))
	      /* Read next line */
	      continue;

	    /* Read Number */
	    sts = sscanf( line, "%s%d", attribute, &number);
	    if ( sts != 2) goto error;
	    if ( number < 1) goto error;
	    /* Read Text */
	    strcpy( text, "");
	    sts = dtt_read_line( line, sizeof(line), infile);
	    if ( EVEN(sts)) goto error;
	    row++;
	    sts = rtt_parse( line, " 	", "",
		(char *) value_str, sizeof( value_str)/sizeof( value_str[0]), 
		sizeof( value_str[0]), 0);
	    if ( !(sts == 1 || sts == 2)) goto error;
	    strcpy( attribute, value_str[0]);
	    if ( sts == 2)
	      strcpy( text, value_str[1]);
	    if ( strcmp( attribute, "Text"))
	      goto error;
	    /* Remove '"' in the text string */
	    if ( text[0] == '"')
	      strcpy( text, &text[1]);
	    if ( strlen(text) > 0 && text[strlen(text)-1] == '"')
	      text[strlen(text)-1] = 0;
	    /* Read Type */
	    strcpy( type, "");
	    sts = dtt_read_line( line, sizeof(line), infile);
	    if ( EVEN(sts)) goto error;
	    row++;
	    sts = sscanf( line, "%s%s", attribute, type);
	    if ( !(sts == 1 || sts == 2)) goto error;
	    if ( strcmp( attribute, "Type"))
	      goto error;
	    /* Read Parameter */
	    sts = dtt_read_line( line, sizeof(line), infile);
	    if ( EVEN(sts)) goto error;
	    sts = sscanf( line, "%s%s", attribute, parameter);
	    if ( sts != 2) goto error;
	    if ( strcmp( attribute, "Parameter"))
	      goto error;
	    /* Read Text/Dualpar */
	    strcpy( text_dualpar, "");
	    sts = dtt_read_line( line, sizeof(line), infile);
	    if ( EVEN(sts)) goto error;
	    row++;
	    sts = rtt_parse( line, " 	", "",
		(char *) value_str, sizeof( value_str)/sizeof( value_str[0]), 
		sizeof( value_str[0]), 0);
	    if ( !(sts == 1 || sts == 2)) goto error;
	    strcpy( attribute, value_str[0]);
	    if ( sts == 2)
	      strcpy( text_dualpar, value_str[1]);
	    if ( strcmp( attribute, "Text/Dualpar"))
	      goto error;
	    /* Remove '"' in the text string */
	    if ( text_dualpar[0] == '"')
	      strcpy( text_dualpar, &text_dualpar[1]);
	    if ( strlen(text_dualpar) > 0 && 
		 text_dualpar[strlen(text_dualpar)-1] == '"')
	      text_dualpar[strlen(text_dualpar)-1] = 0;
	    /* Read Priveleges */
	    strcpy( privileges, "");
	    sts = dtt_read_line( line, sizeof(line), infile);
	    if ( EVEN(sts)) goto error;
	    row++;
	    sts = sscanf( line, "%s%s", attribute, privileges);
	    if ( !(sts == 1 || sts == 2)) goto error;
	    if ( strcmp( attribute, "Privileges"))
	      goto error;
	    /* Read Outputflags */
	    strcpy( outputflags, "");
	    sts = dtt_read_line( line, sizeof(line), infile);
	    if ( EVEN(sts)) goto error;
	    row++;
	    sts = sscanf( line, "%s%s", attribute, outputflags);
	    if ( !(sts == 1 || sts == 2)) goto error;
	    if ( strcmp( attribute, "Outputflags"))
	      goto error;
	    /* Read Characters */
	    sts = dtt_read_line( line, sizeof(line), infile);
	    if ( EVEN(sts)) goto error;
	    sts = sscanf( line, "%s%d", attribute, &characters);
	    if ( sts != 2) goto error;
	    if ( strcmp( attribute, "Characters"))
	      goto error;
	    /* Read Decimals */
	    sts = dtt_read_line( line, sizeof(line), infile);
	    if ( EVEN(sts)) goto error;
	    row++;
	    sts = sscanf( line, "%s%d", attribute, &decimals);
	    if ( sts != 2) goto error;
	    if ( strcmp( attribute, "Decimals"))
	      goto error;
	    /* Read MaxLimit */
	    sts = dtt_read_line( line, sizeof(line), infile);
	    if ( EVEN(sts)) goto error;
	    row++;
	    sts = sscanf( line, "%s%f", attribute, &maxlimit);
	    if ( sts != 2) goto error;
	    if ( strcmp( attribute, "MaxLimit"))
	      goto error;
	    /* Read MinLimit */
	    sts = dtt_read_line( line, sizeof(line), infile);
	    if ( EVEN(sts)) goto error;
	    row++;
	    sts = sscanf( line, "%s%f", attribute, &minlimit);
	    if ( sts != 2) goto error;
	    if ( strcmp( attribute, "MinLimit"))
	      goto error;
	    /* Read Database */
	    strcpy( database, "");
	    sts = dtt_read_line( line, sizeof(line), infile);
	    if ( EVEN(sts)) goto error;
	    row++;
	    sts = sscanf( line, "%s%s", attribute, database);
	    if ( !(sts == 1 || sts == 2)) goto error;
	    if ( strcmp( attribute, "Database"))
	      goto error;
	    /* Read Declaration */
	    strcpy( declaration, "");
	    sts = dtt_read_line( line, sizeof(line), infile);
	    if ( EVEN(sts)) goto error;
	    row++;
	    sts = sscanf( line, "%s%s", attribute, declaration);
	    if ( !(sts == 1 || sts == 2)) goto error;
	    if ( strcmp( attribute, "Declaration"))
	      goto error;
	    /* Read x,y */
	    sts = dtt_read_line( line, sizeof(line), infile);
	    if ( EVEN(sts)) goto error;
	    row++;
	    sts = sscanf( line, "%s%d%d", attribute, &x, &y);
	    if ( sts != 3) goto error;
	    if ( strcmp( attribute, "x,y"))
	      goto error;

	    /* Check for collisions */
	    sts = dtt_get_upd_item( ctx->upd_items, x, y, &item_ptr);
	    if ( ODD(sts))
	      goto collision_error;

	    /* Add this item */
	    sts = dtt_upd_item_add( &(ctx->upd_items), x, y, text);
	    if ( EVEN(sts)) return sts;
	    sts = dtt_get_upd_item( ctx->upd_items, x, y, &item_ptr);
	    if ( EVEN(sts)) return sts;
	    item_ptr->number = number;	
	    strcpy( item_ptr->type, type);
	    strcpy( item_ptr->parameter, parameter);
	    strcpy( item_ptr->dualparameter, text_dualpar);
	    strcpy( item_ptr->priv, privileges);
	    strcpy( item_ptr->outflags, outputflags);
	    item_ptr->characters = characters;
	    item_ptr->decimals = decimals;
	    item_ptr->maxlimit = maxlimit;
	    item_ptr->minlimit = minlimit;
	    strcpy( item_ptr->database, database);
	    strcpy( item_ptr->declaration, declaration);	 
	  }
	  return RTT__SUCCESS;

error:
	sprintf( message, "Error in line %d, %s", row, line);
	rtt_message('E', message);
	return RTT__NOPICTURE;

collision_error:
	fclose( infile);
	sprintf( message, "Field collision in line %d, %s", row, line);
	rtt_message('E', message);
	return RTT__NOPICTURE;
}


static int	dtt_edit_convert_all( void)
{
	int	sts;

	/* Save all pictures */
	sts = dtt_edit_process_menues( &dtt_edit_convert_picture,
		0,0,0,0,0);
	if ( EVEN(sts)) return sts;

	return RTT__SUCCESS;
}

static int	dtt_edit_save_all( 	int	generate_only, 
					int	nocompile,
					int 	opsys)
{
	int	sts;
	char	filename[80];

	/* Save all pictures */
	sts = dtt_edit_process_menues( &dtt_edit_save_picture,
		0,0, generate_only, opsys, nocompile);
	if ( EVEN(sts)) return sts;

	/* Save the menues */
	/* Take the default filename */
	dtt_get_menufilename( filename);
	  
	sts = dtt_edit_save_menues( filename, generate_only);
	if ( EVEN(sts)) return sts;

	return RTT__SUCCESS;
}

static int	dtt_edit_save_by_index( int	index, 
					int	generate_only,
					int	nocompile,
					int	opsys)
{
	int	sts;

	/* Save all pictures */
	sts = dtt_edit_process_menues( &dtt_edit_save_picture,
		0, index, generate_only, opsys, nocompile);
	if ( EVEN(sts)) return sts;

	return RTT__SUCCESS;
}

static int	dtt_edit_convert_picture( 	menu_ctx ctx,
					int menu_idx,
					int keep_ctx,
					int index,
					int dum2,
					int dum3,
					int dum4)
{
	int	sts;
	char	picturefilename[80];
	char	menufilename[80];
	rtt_t_menu	*menu_ptr;
	edit_ctx	picture_ctx;
	char		*title;
	char		message[80];
	int		unload;
	int		messages;
	int		generate_only = 0;
	int		nocompile = 1;

	menu_ptr = ctx->menu;
	menu_ptr += menu_idx;
	unload = 0;

	/* Which type of menue is this */
	if (
		( menu_ptr->type == DTT_MENUITEM_EDIT) ||
		( menu_ptr->type == DTT_MENUITEM_PIPERM) ||
		( menu_ptr->type == DTT_MENUITEM_PIFUNCPERM) ||
	        ( menu_ptr->type == DTT_MENUITEM_PIFUNC))
	{
	  if ( index)
	  {
	    /* Save only this index */
	    if ( index != menu_ptr->index)
	      return RTT__SUCCESS;
	  }

	  sprintf( message, "Converting picture %s", menu_ptr->text);
	  rtt_message('I', message);

	  /* Get this picture */

	  /* Get the title from the parent items text */
	  title = menu_ptr->text;

	  /* Try to find this context, it may be stored */
	  sts = dtt_get_stored_ctx( &picture_ctx, title, menu_ptr->index);
	  if ( EVEN(sts))
	  {
	    /* Not loaded */
	    sts = dtt_create_ctx( &picture_ctx, (edit_ctx) ctx, title, 
			menu_ptr->index);
	    if ( EVEN(sts)) return sts;
	    if ( keep_ctx)
	    {
	      sts = dtt_store_ctx( picture_ctx, title, menu_ptr->index);
	      if ( EVEN(sts)) return sts;
	    }
	    else
	      unload = 1;
	    /* Try to read it */
	    dtt_get_picturefilename( picture_ctx->index, picturefilename);
	    sts = dtt_edit_read_v27( picture_ctx, picturefilename, 0);
	    if ( EVEN(sts)) return sts;
	  }
	  else
	    dtt_get_picturefilename( picture_ctx->index, picturefilename);
	  
	  dtt_get_menufilename( menufilename);
	  sts = dtt_edit_write( picture_ctx, picturefilename, menufilename, 0,
		nocompile, generate_only, 0, 0, &messages);
	  if ( EVEN(sts)) return sts;

	  if ( unload)
	    dtt_free_ctx( picture_ctx);
	}
	return RTT__SUCCESS;
}

static int	dtt_edit_save_picture( 	menu_ctx ctx,
					int menu_idx, 
					int keep_ctx,
					int index,
					int generate_only, 
					int opsys,
					int nocompile)
{
	int	sts;
	char	picturefilename[80];
	char	menufilename[80];
	rtt_t_menu	*menu_ptr;
	edit_ctx	picture_ctx;
	char		*title;
	char		message[80];
	int		unload;
	int		messages;

	menu_ptr = ctx->menu;
	menu_ptr += menu_idx;
	unload = 0;

	/* Which type of menue is this */
	if (
		( menu_ptr->type == DTT_MENUITEM_EDIT) ||
		( menu_ptr->type == DTT_MENUITEM_PIPERM) ||
		( menu_ptr->type == DTT_MENUITEM_PIFUNCPERM) ||
	        ( menu_ptr->type == DTT_MENUITEM_PIFUNC))
	{
	  if ( index)
	  {
	    /* Save only this index */
	    if ( index != menu_ptr->index)
	      return RTT__SUCCESS;
	  }

	  if ( !generate_only)
	    sprintf( message, "Saving picture %s", menu_ptr->text);
	  else
	    sprintf( message, "Generating picture %s", menu_ptr->text);
	  rtt_message('I', message);

	  /* Get this picture */

	  /* Get the title from the parent items text */
	  title = menu_ptr->text;

	  /* Try to find this context, it may be stored */
	  sts = dtt_get_stored_ctx( &picture_ctx, title, menu_ptr->index);
	  if ( EVEN(sts))
	  {
	    /* Not loaded */
	    sts = dtt_create_ctx( &picture_ctx, (edit_ctx) ctx, title, 
			menu_ptr->index);
	    if ( EVEN(sts)) return sts;
	    if ( keep_ctx)
	    {
	      sts = dtt_store_ctx( picture_ctx, title, menu_ptr->index);
	      if ( EVEN(sts)) return sts;
	    }
	    else
	      unload = 1;
	    /* Try to read it */
	    dtt_get_picturefilename( picture_ctx->index, picturefilename);
	    sts = dtt_edit_read( picture_ctx, picturefilename, 0);
	    if ( EVEN(sts)) return sts;
	  }
	  else
	    dtt_get_picturefilename( picture_ctx->index, picturefilename);
	  
	  dtt_get_menufilename( menufilename);
	  sts = dtt_edit_write( picture_ctx, picturefilename, menufilename, 0,
		nocompile, generate_only, opsys, 0, &messages);
	  if ( EVEN(sts)) return sts;

	  if ( unload)
	    dtt_free_ctx( picture_ctx);
	}
	return RTT__SUCCESS;
}
/*************************************************************************
*
* Name:		dtt_edit_write_menue()
*
* Type		int
*
* Type		Parameter	IOGF	Description
*
* Description:
*	Write the menues.
*
**************************************************************************/

static int	dtt_edit_write_menue( 	menu_ctx	ctx,
					char		*filename,
					int		all)
{
	int		sts;
	FILE		*fout;
	char		*s;
	char		fname[200];
	menu_ctx	childctx;
	rtt_t_menu	*menu_ptr;

	strcpy( fname, filename);
	/* Check if there is any extention in the filename */
	s = strchr( fname, '.');
	if ( s == 0)
	{
	  /* No extention given */
	  strcat( fname, ".dtt_m");
	}

	/* Open the datafile and write */
	fout = fopen( fname, "w");

	if ( fout == 0)
	  return 0;

/*	fprintf( fout, "%d\n", dtt_current_index);
	fprintf( fout, "%s\n", &dtt_maintitle);
*/

	if (all)
	{
	  /* Write the whole current menue */
	  menu_ptr = ctx->menu;
	  while ( menu_ptr->text[0] != 0)
	  {
	    if ( menu_ptr != ctx->menu)
	      fprintf( fout, "next\n");

	    /* Print the text, type, and index */
	    fprintf( fout, "%s\n", menu_ptr->text);
	    fprintf( fout, "%d\n", menu_ptr->type);
	    fprintf( fout, "%d\n", menu_ptr->index);
	    /* Print command if this is a command item */
	    if (( menu_ptr->type == DTT_MENUITEM_COMMAND) ||
	      ( menu_ptr->type == DTT_MENUITEM_COMMANDHOLD) ||
	      ( menu_ptr->type == DTT_MENUITEM_VMSCOMMAND) ||
	      ( menu_ptr->type == DTT_MENUITEM_VMSCOMMANDNOWAIT) ||
	      ( menu_ptr->type == DTT_MENUITEM_VMSCOMMANDCONF) ||
	      ( menu_ptr->type == DTT_MENUITEM_VMSCOMMANDHOLD) ||
	      ( menu_ptr->type == DTT_MENUITEM_SYSPICTURE) ||
	      ( menu_ptr->type == DTT_MENUITEM_PIFUNCPERM) ||
	      ( menu_ptr->type == DTT_MENUITEM_PIFUNC))
	    {
	      fprintf( fout, "%s\n", (char *) menu_ptr->arg1);
	    }
	    if ( menu_ptr->type == DTT_MENUITEM_OBJPICTURE ||
	         menu_ptr->type == DTT_MENUITEM_FILESPEC)
	    {
	      fprintf( fout, "%s\n", (char *) menu_ptr->arg1);
	      fprintf( fout, "%s\n", (char *) menu_ptr->arg2);
	    }

	    /* Look for a submenu to this item */
	    if ( ODD( dtt_get_stored_menuctx( &childctx, menu_ptr->index)) )
	    {
	      fprintf( fout, "down\n");
	      /* Write this menu */
	      sts = dtt_edit_write_one_menu( fout, childctx, menu_ptr->index);
	      if ( EVEN(sts)) return sts;
	      fprintf( fout, "up\n");
	    }
	    menu_ptr++;
	  }
	}
	else
	{
	  /* Just write the current menu item */
	  menu_ptr = ctx->menu;
	  menu_ptr += ctx->current_item;

	  /* Print the text, type, and index */
	  fprintf( fout, "%s\n", menu_ptr->text);
	  fprintf( fout, "%d\n", menu_ptr->type);
	  fprintf( fout, "%d\n", menu_ptr->index);
	  /* Print command if this is a command item */
	  if (( menu_ptr->type == DTT_MENUITEM_COMMAND) ||
	      ( menu_ptr->type == DTT_MENUITEM_COMMANDHOLD) ||
	      ( menu_ptr->type == DTT_MENUITEM_VMSCOMMAND) ||
	      ( menu_ptr->type == DTT_MENUITEM_VMSCOMMANDNOWAIT) ||
	      ( menu_ptr->type == DTT_MENUITEM_VMSCOMMANDCONF) ||
	      ( menu_ptr->type == DTT_MENUITEM_VMSCOMMANDHOLD) ||
	      ( menu_ptr->type == DTT_MENUITEM_SYSPICTURE) ||
	      ( menu_ptr->type == DTT_MENUITEM_PIFUNCPERM) ||
	      ( menu_ptr->type == DTT_MENUITEM_PIFUNC))
	  {
	    fprintf( fout, "%s\n", (char *) menu_ptr->arg1);
	  }
	  if ( menu_ptr->type == DTT_MENUITEM_OBJPICTURE ||
	       menu_ptr->type == DTT_MENUITEM_FILESPEC)
	  {
	    fprintf( fout, "%s\n", (char *) menu_ptr->arg1);
	    fprintf( fout, "%s\n", (char *) menu_ptr->arg2);
	  }

	  /* Look for a submenu to this item */
	  if ( ODD( dtt_get_stored_menuctx( &childctx, menu_ptr->index)) )
	  {
	    fprintf( fout, "down\n");
	    /* Write this menu */
	    sts = dtt_edit_write_one_menu( fout, childctx, menu_ptr->index);
	    if ( EVEN(sts)) return sts;
	    fprintf( fout, "up\n");
	  }
	}

	/* close the files */
	fclose(fout);

	return RTT__SUCCESS;
}

/*************************************************************************
*
* Name:		dtt_edit_write_one_menu()
*
* Type		int
*
* Type		Parameter	IOGF	Description
*
* Description:
*	Write the menues.
*
**************************************************************************/

static int	dtt_edit_write_one_menu(	FILE		*fout,
						menu_ctx	ctx,
						int		index)
{
	int		sts;
	menu_ctx	childctx;
	rtt_t_menu	*menu_ptr;

	menu_ptr = ctx->menu;

	while ( menu_ptr->text[0] != 0)
	{
	  if ( menu_ptr != ctx->menu )
	    fprintf( fout, "next\n");

	  /* Print the text, type, and index */
	  fprintf( fout, "%s\n", menu_ptr->text);
	  fprintf( fout, "%d\n", menu_ptr->type);
	  fprintf( fout, "%d\n", menu_ptr->index);
	  /* Print command if this is a command item */
	  if (( menu_ptr->type == DTT_MENUITEM_COMMAND) ||
	      ( menu_ptr->type == DTT_MENUITEM_COMMANDHOLD) ||
	      ( menu_ptr->type == DTT_MENUITEM_VMSCOMMAND) ||
	      ( menu_ptr->type == DTT_MENUITEM_VMSCOMMANDNOWAIT) ||
	      ( menu_ptr->type == DTT_MENUITEM_VMSCOMMANDCONF) ||
	      ( menu_ptr->type == DTT_MENUITEM_VMSCOMMANDHOLD) ||
	      ( menu_ptr->type == DTT_MENUITEM_SYSPICTURE) ||
	      ( menu_ptr->type == DTT_MENUITEM_PIFUNCPERM) ||
	      ( menu_ptr->type == DTT_MENUITEM_PIFUNC))
	  {
	    fprintf( fout, "%s\n", (char *) menu_ptr->arg1);
	  }
	  if ( menu_ptr->type == DTT_MENUITEM_OBJPICTURE ||
	       menu_ptr->type == DTT_MENUITEM_FILESPEC)
	  {
	    fprintf( fout, "%s\n", (char *) menu_ptr->arg1);
	    fprintf( fout, "%s\n", (char *) menu_ptr->arg2);
	  }

	  /* Look for a submenu to this item */
	  if ( ODD( dtt_get_stored_menuctx( &childctx, menu_ptr->index)) )
	  {
	    fprintf( fout, "down\n");
	    /* Write this menu */
	    sts = dtt_edit_write_one_menu( fout, childctx, menu_ptr->index);
	    if ( EVEN(sts)) return sts;
	    fprintf( fout, "up\n");
	  }
	  menu_ptr++;
	}

	return RTT__SUCCESS;
}


/*************************************************************************
*
* Name:		dtt_edit_include_menues()
*
* Type		int
*
* Type		Parameter	IOGF	Description
*
* Description:
*	Read the menues.
*
**************************************************************************/

static int	dtt_edit_include_menue(	menu_ctx	ctx,
					char		*filename)
{
	int		sts;
	FILE		*fin;
	char		*s;
	char		fname[200];
	rtt_t_menu	*menu_ptr;
	int		index;
	int		type;
	char		text[80];
	char		line[80];
	char		command[80];
	char		command2[80];
	char		*command_ptr;
	int		i = 0;


	strcpy( fname, filename);
	/* Check if there is any extention in the filename */
	s = strchr( fname, '.');
	if ( s == 0)
	{
	  /* No extention given */
	  strcat( fname, ".dtt_m");
	}

	/* Open and read */
	fin = fopen( fname, "r");

	if ( fin == 0)
	  return 0;

	menu_ptr = ctx->menu;
	menu_ptr += ctx->current_item;

	/* Include the items on the top level */
	sts = dtt_read_line( text, sizeof(text), fin);

	i = ctx->current_item + 1;

	while ( 1)
	{
	  sts = dtt_read_line( line, sizeof(line), fin);
	  sts = sscanf( line, "%d", &type);
	  sts = dtt_read_line( line, sizeof(line), fin);
	  sts = sscanf( line, "%d", &index);
	  /* If command type read the command */
	  if (( type == DTT_MENUITEM_COMMAND) ||
	      ( type == DTT_MENUITEM_COMMANDHOLD) ||
	      ( type == DTT_MENUITEM_VMSCOMMAND) ||
	      ( type == DTT_MENUITEM_VMSCOMMANDNOWAIT) ||
	      ( type == DTT_MENUITEM_VMSCOMMANDCONF) ||
	      ( type == DTT_MENUITEM_VMSCOMMANDHOLD) ||
	      ( type == DTT_MENUITEM_SYSPICTURE) ||
	      ( type == DTT_MENUITEM_PIFUNCPERM) ||
	      ( type == DTT_MENUITEM_PIFUNC))
	  {
	    sts = dtt_read_line( command, sizeof(command), fin);
	  }
	  if ( type == DTT_MENUITEM_OBJPICTURE ||
	       type == DTT_MENUITEM_FILESPEC)
	  {
	    sts = dtt_read_line( command, sizeof(command), fin);
	    sts = dtt_read_line( command2, sizeof(command2), fin);
	  }

	  {
	    /* Add an item to the menulist */
	    sts = rtt_menu_list_insert( &(ctx->menu),
		i, text,
		0, 0, 0, pwr_cNObjid, 0,0,0,0);
	    if ( EVEN(sts)) return sts;

	    /* Set type and index */
	    menu_ptr = ctx->menu;
	    menu_ptr += i;
	    menu_ptr->index = dtt_current_index;
	    menu_ptr->arg3 = (void *) dtt_current_index;
	    menu_ptr->type = type;
	    menu_ptr->func2 = &dtt_menu_item_show;
	    dtt_current_index++;

	    if ( type == DTT_MENUITEM_EDIT ||
	         type == DTT_MENUITEM_PIPERM)
	    {
	      menu_ptr->func = &dtt_edit_new;
	    }
	    else if (( type == DTT_MENUITEM_COMMAND) ||
	             ( type == DTT_MENUITEM_COMMANDHOLD) ||
	             ( type == DTT_MENUITEM_VMSCOMMAND) ||
	             ( type == DTT_MENUITEM_VMSCOMMANDCONF) ||
	             ( type == DTT_MENUITEM_VMSCOMMANDNOWAIT) ||
	             ( type == DTT_MENUITEM_VMSCOMMANDHOLD) ||
	             ( type == DTT_MENUITEM_SYSPICTURE))
	    {
	      /* Put a pointer to the command in arg1 */
	      command_ptr = calloc(  80, sizeof( char));
	      strcpy( command_ptr, command);
	      menu_ptr->arg1 = command_ptr;
	    }
	    else if ( type == DTT_MENUITEM_OBJPICTURE ||
	              type == DTT_MENUITEM_FILESPEC)
	    {
	      /* Put a pointer to the command in arg1 */
	      command_ptr = calloc(  80, sizeof( char));
	      strcpy( command_ptr, command);
	      menu_ptr->arg1 = command_ptr;
	      command_ptr = calloc(  80, sizeof( char));
	      strcpy( command_ptr, command2);
	      menu_ptr->arg2 = command_ptr;
	    }
	    else if ( type == DTT_MENUITEM_PIFUNC ||
	              type == DTT_MENUITEM_PIFUNCPERM)
	    {
	      /* Put a pointer to the function name in arg1 */
	      command_ptr = calloc(  80, sizeof( char));
	      strcpy( command_ptr, command);
	      menu_ptr->arg1 = command_ptr;
	      menu_ptr->func = &dtt_edit_new;
	    }
	  }	    
	  /* Read next item */
	  sts = dtt_read_line( text, sizeof(text), fin);
	  if ( EVEN(sts))
	     break;
	  if ( strcmp( text, "down") == 0)
	  {
	    /* Down to a submenu */
	    sts = dtt_edit_include_one_menu( fin, ctx, i);
	    if ( EVEN(sts)) return sts;
	    sts = dtt_read_line( text, sizeof(text), fin);
	    if ( EVEN(sts))
	       break;
	  }
	  if ( strcmp( text, "up") == 0)
	  {
	    break;
	  }
	  if ( strcmp( text, "next") == 0)
	  {
	       sts = dtt_read_line( text, sizeof(text), fin);
	    if ( EVEN(sts))
	       break;
	  }
          i++;
	}
	/* Configure */
	rtt_menu_configure( ctx);

	fclose(fin);
	return RTT__SUCCESS;
}

/*************************************************************************
*
* Name:		dtt_edit_include_one_menu()
*
* Type		int
*
* Type		Parameter	IOGF	Description
*
* Description:
*	Reads the menues.
*
**************************************************************************/

static int	dtt_edit_include_one_menu( 	FILE		*fin,
						menu_ctx	parent_ctx,
						int		parent_item)
{
	int		sts;
	menu_ctx	ctx;
	rtt_t_menu	*menu_ptr;
	rtt_t_menu	*menulist = 0;
	rtt_t_menu	*parent_menu_ptr;
	int		index;
	int		type;
	int		first = 0;
	char		text[80];
	char		title[80];
	char		line[80];
	char		command[80];
	char		command2[80];
	char		*command_ptr;
	int		i = 0;
	char		*parent_text;

	if ( parent_ctx != 0)
	{
	  parent_menu_ptr = parent_ctx->menu;
	  parent_menu_ptr += parent_item;
	}	
	sts = dtt_read_line( text, sizeof(text), fin);

	while ( 1)
	{
	  sts = dtt_read_line( line, sizeof(line), fin);
	  sts = sscanf( line, "%d", &type);
	  sts = dtt_read_line( line, sizeof(line), fin);
	  sts = sscanf( line, "%d", &index);
	  /* If command type read the command */
	  if (( type == DTT_MENUITEM_COMMAND) ||
	      ( type == DTT_MENUITEM_COMMANDHOLD) ||
	      ( type == DTT_MENUITEM_VMSCOMMAND) ||
	      ( type == DTT_MENUITEM_VMSCOMMANDNOWAIT) ||
	      ( type == DTT_MENUITEM_VMSCOMMANDCONF) ||
	      ( type == DTT_MENUITEM_VMSCOMMANDHOLD) ||
	      ( type == DTT_MENUITEM_SYSPICTURE) ||
	      ( type == DTT_MENUITEM_PIFUNCPERM) ||
	      ( type == DTT_MENUITEM_PIFUNC))
	  {
	    sts = dtt_read_line( command, sizeof(command), fin);
	  }
	  if ( type == DTT_MENUITEM_OBJPICTURE ||
	       type == DTT_MENUITEM_FILESPEC)
	  {
	    sts = dtt_read_line( command, sizeof(command), fin);
	    sts = dtt_read_line( command2, sizeof(command2), fin);
	  }

	  if ( !first)
	  {
	    first = 1;

	    /* Create a new menu */
	    sts = rtt_menu_list_add( &menulist, i, 0, text,
			0, 0, 0, pwr_cNObjid, 0,0,0,0);
	    if ( EVEN(sts)) return sts;

	    /* Set type and index */
	    menu_ptr = menulist;
	    menu_ptr->index = dtt_current_index;
	    menu_ptr->arg3 = (void *) dtt_current_index;
	    menu_ptr->type = type;
	    menu_ptr->func2 = &dtt_menu_item_show;
	    dtt_current_index++;

	    if ( type == DTT_MENUITEM_EDIT ||
	         type == DTT_MENUITEM_PIPERM)
	    {
	      menu_ptr->func = &dtt_edit_new;
	    }
	    else if (( type == DTT_MENUITEM_COMMAND) ||
	    	     ( type == DTT_MENUITEM_COMMANDHOLD) ||
	    	     ( type == DTT_MENUITEM_VMSCOMMAND) ||
	    	     ( type == DTT_MENUITEM_VMSCOMMANDCONF) ||
	    	     ( type == DTT_MENUITEM_VMSCOMMANDNOWAIT) ||
	    	     ( type == DTT_MENUITEM_VMSCOMMANDHOLD) ||
	    	     ( type == DTT_MENUITEM_SYSPICTURE))
	    {
	      /* Put a pointer to the command in arg1 */
	      command_ptr = calloc(  80, sizeof( char));
	      strcpy( command_ptr, command);
	      menu_ptr->arg1 = command_ptr;
	    }
	    else if ( type == DTT_MENUITEM_OBJPICTURE ||
	              type == DTT_MENUITEM_FILESPEC)
	    {
	      /* Put a pointer to the command in arg1 */
	      command_ptr = calloc(  80, sizeof( char));
	      strcpy( command_ptr, command);
	      menu_ptr->arg1 = command_ptr;
	      command_ptr = calloc(  80, sizeof( char));
	      strcpy( command_ptr, command2);
	      menu_ptr->arg2 = command_ptr;
	    }
	    else if ( type == DTT_MENUITEM_PIFUNC ||
	              type == DTT_MENUITEM_PIFUNCPERM)
	    {
	      /* Put a pointer to the function name in arg1 */
	      command_ptr = calloc(  80, sizeof( char));
	      strcpy( command_ptr, command);
	      menu_ptr->arg1 = command_ptr;
	      menu_ptr->func = &dtt_edit_new;
	    }
	    
	    if ( parent_ctx != 0)
	      strcpy( title, parent_menu_ptr->text);
	    else
	      strcpy( title, dtt_maintitle);

	    /* Create a context */
	    sts = rtt_menu_create_ctx( &ctx, parent_ctx, menulist,
			title,
			RTT_MENUTYPE_MENU | RTT_MENUTYPE_DYN);
	    if ( EVEN(sts)) return sts;

	    /* Store the context */
	    if ( parent_ctx == 0)
	    {
	      dtt_root_ctx = ctx;
	      dtt_store_menuctx( ctx, 1);
	    }
	    else
	      dtt_store_menuctx( ctx, parent_menu_ptr->index);
	  }
	  else
	  {
	    /* Add an item to the menulist */
	    sts = rtt_menu_list_add( &(ctx->menu),
		i, 0, text,
		0, 0, 0, pwr_cNObjid, 0,0,0,0);
	    if ( EVEN(sts)) return sts;

	    /* Set type and index */
	    menu_ptr = ctx->menu;
	    menu_ptr += i;
	    menu_ptr->index = dtt_current_index;
	    menu_ptr->arg3 = (void *) dtt_current_index;
	    menu_ptr->type = type;
	    menu_ptr->func2 = &dtt_menu_item_show;
	    dtt_current_index++;

	    if ( type == DTT_MENUITEM_EDIT ||
	         type == DTT_MENUITEM_PIPERM)
	    {
	      menu_ptr->func = &dtt_edit_new;
	    }
	    else if (( type == DTT_MENUITEM_COMMAND) ||
	             ( type == DTT_MENUITEM_COMMANDHOLD) ||
	             ( type == DTT_MENUITEM_VMSCOMMAND) ||
	             ( type == DTT_MENUITEM_VMSCOMMANDCONF) ||
	             ( type == DTT_MENUITEM_VMSCOMMANDNOWAIT) ||
	             ( type == DTT_MENUITEM_VMSCOMMANDHOLD) ||
	             ( type == DTT_MENUITEM_SYSPICTURE))
	    {
	      /* Put a pointer to the command in arg1 */
	      command_ptr = calloc(  80, sizeof( char));
	      strcpy( command_ptr, command);
	      menu_ptr->arg1 = command_ptr;
	    }
	    else if ( type == DTT_MENUITEM_OBJPICTURE ||
	              type == DTT_MENUITEM_FILESPEC)
	    {
	      /* Put a pointer to the command in arg1 */
	      command_ptr = calloc(  80, sizeof( char));
	      strcpy( command_ptr, command);
	      menu_ptr->arg1 = command_ptr;
	      command_ptr = calloc(  80, sizeof( char));
	      strcpy( command_ptr, command2);
	      menu_ptr->arg2 = command_ptr;
	    }
	    else if ( type == DTT_MENUITEM_PIFUNC ||
	              type == DTT_MENUITEM_PIFUNCPERM)
	    {
	      /* Put a pointer to the function name in arg1 */
	      command_ptr = calloc(  80, sizeof( char));
	      strcpy( command_ptr, command);
	      menu_ptr->arg1 = command_ptr;
	      menu_ptr->func = &dtt_edit_new;
	    }
	  }	    
	  /* Read next item */
	  sts = dtt_read_line( text, sizeof(text), fin);
	  if ( EVEN(sts))
	     break;
	  if ( strcmp( text, "down") == 0)
	  {
	    /* Down to a submenu */
	    sts = dtt_edit_include_one_menu( fin, ctx, i);
	    if ( EVEN(sts)) return sts;
	    sts = dtt_read_line( text, sizeof(text), fin);
	    if ( EVEN(sts))
	       break;
	  }
	  if ( strcmp( text, "up") == 0)
	  {
	    break;
	  }
	  if ( strcmp( text, "next") == 0)
	  {
	    sts = dtt_read_line( text, sizeof(text), fin);
	    if ( EVEN(sts))
	       break;
	  }
          i++;
	}
	/* Configure */
	rtt_menu_configure( ctx);

	/* Set function in parent item */
	if ( parent_ctx != 0)
	{
	  parent_text = calloc(  80, sizeof( char));
	  strcpy( parent_text, parent_menu_ptr->text);
	  parent_menu_ptr->arg1 = parent_text;
	  parent_menu_ptr->func = &dtt_menu_new;
	}
	return RTT__SUCCESS;
}

/*************************************************************************
*
* Name:		dtt_if_function_menues()
*
* Type		int
*
* Type		Parameter	IOGF	Description
*
* Description:
*	Reads the menues.
*
**************************************************************************/

static int dtt_function_menues_exist()
{
	int	function_found;
	int	sts;

	function_found = 0;
	sts = dtt_edit_process_menues( &dtt_check_if_function_menu,
		(unsigned long) &function_found, 0,0,0,0);

	return function_found;
}

static int 	dtt_check_if_function_menu(	menu_ctx ctx,
						int menu_idx, 
						int *function_found,
						int dum1, 
						int dum2, 
						int dum3, 
						int dum4)
{
	rtt_t_menu	*menu_ptr;

	menu_ptr = ctx->menu;
	menu_ptr += menu_idx;

	/* Which type of menue is this */
	if (
		( menu_ptr->type == DTT_MENUITEM_PIFUNCPERM) ||
	        ( menu_ptr->type == DTT_MENUITEM_PIFUNC))
	{
	  *function_found = 1;
	}
	return RTT__SUCCESS;
}

/*************************************************************************
*
* Name:		dtt_show_menu()
*
* Type		int
*
* Type		Parameter	IOGF	Description
*
* Description:
*	Show a menu specified by a string.
*
**************************************************************************/

static int	dtt_show_menu( 	menu_ctx	ctx, 
				char 		*menu_name)
{
	char		name_array[20][32];
	int		i;
	rtt_t_menu	*menu_ptr;
	char		title[80];
	int		nr;
	int		found;
	int		sts;
	menu_ctx	menuctx;

	/* Parse the menu_name */
	nr = rtt_parse( menu_name, "-", "",
		(char *) name_array, 
		sizeof( name_array)/sizeof( name_array[0]), 
		sizeof( name_array[0]), 0);

	menu_ptr = dtt_root_ctx->menu;
	for ( i = 0; i < nr; i++)
	{

	  rtt_toupper( name_array[i], name_array[i]);
	  found = 0;
	  while ( menu_ptr->text[0])
	  {
	    rtt_toupper( title, menu_ptr->text);
	    if ( !strcmp( title, name_array[i]))
	    {
	      if ( i < nr -1)
	      {
	        /* Check that the menu type is correct */
	        if ( menu_ptr->func != dtt_menu_new )
	        {
	          rtt_message('E',"Error in menu type");
	          return RTT__NOPICTURE;
	        }
	        sts = dtt_get_stored_menuctx( &menuctx, menu_ptr->index);
	        if ( EVEN(sts))
	        {
	          rtt_message('E',"Error in menu");
	          return RTT__NOPICTURE;
	        }
	        menu_ptr = menuctx->menu;
	        found = 1;
	        break;
	      }
	      else
	      {
	        found = 1;
	        break;
	      }
	    }
	    menu_ptr++;
	  }
	  if ( !found)
	  {
	    rtt_message('E',"Menu not found");
	    return RTT__HOLDCOMMAND;
	  }
	}

	if ( menu_ptr->func)
	{
	  sts = (menu_ptr->func)( menuctx, menu_ptr->arg1, menu_ptr->arg2, 
			menu_ptr->arg3, menu_ptr->arg4);
	  return sts;
	}
	else
	{
	  rtt_message('E',"Function in menu not defined");
	  return RTT__NOPICTURE;
	}
}

/*************************************************************************
*
* Name:		dtt_command_get_input()
*
* Type		int
*
* Type		Parameter	IOGF	Description
* char		*chn		I	channel.
* char		*out_string	O	input string
* unsigned long	*out_terminator	O	terminator
* int		out_maxlen	I	max charachters.
* unsigned long	option		I	option mask.
* int		timeout		I	timeout time
*
* Description:
*	Read a input char in edit mode.
*
**************************************************************************/

static int dtt_command_get_input(
			char		*chn,
			char		*out_string,
			unsigned long	*out_terminator,
			int		out_maxlen,
			unsigned long	option,
			int		timeout)
{
	int		sts;
	unsigned long	terminator;
	char		command[80];
	char		str[80];

	if ( rtt_commandmode & RTT_COMMANDMODE_FILE)
	{
	  sts = rtt_commandmode_getnext( command, &terminator);
	  if ( EVEN(sts))
	  {
	    *out_terminator = RTT_K_COMMAND;
	    return RTT__SUCCESS;
	  }

	  *out_string = 0;
	  if ( strcmp( command, "K_RETURN") == 0)
	    *out_terminator = RTT_K_RETURN;
	  else if ( strcmp( command, "K_PF1") == 0)
	    *out_terminator = RTT_K_PF1;
  	  else if ( strcmp( command, "K_PF2") == 0)
	    *out_terminator = RTT_K_PF2;
	  else if ( strcmp( command, "K_PF3") == 0)
	    *out_terminator = RTT_K_PF3;
	  else if ( strcmp( command, "K_PF4") == 0)
	    *out_terminator = RTT_K_PF4;
	  else if ( strcmp( command, "K_ARROW_UP") == 0)
	    *out_terminator = RTT_K_ARROW_UP;
	  else if ( strcmp( command, "K_ARROW_DOWN") == 0)
	    *out_terminator = RTT_K_ARROW_DOWN;
	  else if ( strcmp( command, "K_ARROW_RIGHT") == 0)
	    *out_terminator = RTT_K_ARROW_RIGHT;
	  else if ( strcmp( command, "K_ARROW_LEFT") == 0)
	    *out_terminator = RTT_K_ARROW_LEFT;
	  else if ( strcmp( command, "K_PREVIOUS_PAGE") == 0)
	    *out_terminator = RTT_K_PREVPAGE;
	  else if ( strcmp( command, "K_NEXT_PAGE") == 0)
	    *out_terminator = RTT_K_NEXTPAGE;
	  else if ( strcmp( command, "K_CTRLV") == 0)
	    *out_terminator = RTT_K_CTRLV;
	  else if ( strcmp( command, "K_DELETE") == 0)
	    *out_terminator = RTT_K_DELETE;
	  else if ( strcmp( command, "K_CTRLN") == 0)
	    *out_terminator = RTT_K_CTRLN;
	  else if ( strcmp( command, "K_CTRLZ") == 0)
	    *out_terminator = RTT_K_CTRLZ;
	  else if ( strcmp( command, "K_CTRLW") == 0)
	    *out_terminator = RTT_K_CTRLW;
	  else if ( strcmp( command, "K_HELP") == 0)
	    *out_terminator = RTT_K_HELP;
	  else if ( strncmp( command, "K_CHAR ", 7) == 0)
	  {
	    *out_terminator = RTT_K_MAXLEN;
	    sscanf( &command[8], "%c", out_string);
	  }
	  else
	  {
	    /* Nothing for us */	  
	    rtt_commandmode_rewind();
	    *out_terminator = RTT_K_COMMAND;
	  }
	  sts = RTT__SUCCESS;
	}
	else
	{
	  sts = rtt_get_input( chn, out_string, out_terminator, out_maxlen,
		option, timeout);
	}

	if ( rtt_commandmode & RTT_COMMANDMODE_LEARN)
	{
	    if ( *out_terminator == RTT_K_RETURN)
	      rtt_learn_store( "K_RETURN");
	    if ( *out_terminator == RTT_K_PF1)
	      rtt_learn_store( "K_PF1");
	    if ( *out_terminator == RTT_K_PF2)
    	      rtt_learn_store( "K_PF2");
	    if ( *out_terminator == RTT_K_PF3)
	      rtt_learn_store( "K_PF3");
	    if ( *out_terminator == RTT_K_PF4)
	      rtt_learn_store( "K_PF4");
	    if ( *out_terminator == RTT_K_ARROW_UP)
	      rtt_learn_store( "K_ARROW_UP");
	    if ( *out_terminator == RTT_K_ARROW_DOWN)
	      rtt_learn_store( "K_ARROW_DOWN");
	    if ( *out_terminator == RTT_K_ARROW_RIGHT)
	      rtt_learn_store( "K_ARROW_RIGHT");
	    if ( *out_terminator == RTT_K_ARROW_LEFT)
	      rtt_learn_store( "K_ARROW_LEFT");
	    if ( *out_terminator == RTT_K_PREVPAGE)
	      rtt_learn_store( "K_PREVIOUS_PAGE");
	    if ( *out_terminator == RTT_K_NEXTPAGE)
	      rtt_learn_store( "K_NEXT_PAGE");
	    if ( *out_terminator == RTT_K_CTRLV)
	      rtt_learn_store( "K_CTRLV");
	    if ( *out_terminator == RTT_K_DELETE)
	      rtt_learn_store( "K_DELETE");
	    if ( *out_terminator == RTT_K_CTRLN)
	      rtt_learn_store( "K_CTRLN");
	    if ( *out_terminator == RTT_K_CTRLZ)
	      rtt_learn_store( "K_CTRLZ");
	    if ( *out_terminator == RTT_K_CTRLW)
	      rtt_learn_store( "K_CTRLW");
	    if ( *out_terminator == RTT_K_HELP)
	      rtt_learn_store( "K_HELP");
	    else if ( *out_string >= 31)
	    {
	      sprintf( str, "K_CHAR \"%c\"", *out_string);
	      rtt_learn_store( str);
	    }
	}

	return sts;
}

/*************************************************************************
*
* Name:		dtt_quotes_convert()
*
* Type		char *
*
* Type		Parameter	IOGF	Description
*
* Description:
*	Adds a '\' before every '"'.
*	
**************************************************************************/
static char	*dtt_quotes_convert( char *str)
{
	static char outstr[200];
	char		*in_p;
	unsigned char	*out_p;

	in_p  = str;
	out_p = (unsigned char *) outstr;
	while ( *in_p != '\0')
	{
	  if ( *in_p == '"')
	    *out_p++ = '\\';
	  *out_p++ = *in_p++;
	}
	*out_p = *in_p;
	return outstr;
}

static int	dtt_edit_check_items_syntax( 	edit_ctx	picture_ctx,
						int		*error_count,
						int		*warning_count)
{
	dtt_item 	item_ptr, item_p;
	int		errors;
	int		warnings;
	int		size;

	errors = 0;
	warnings = 0;

	/* List the items */
	item_ptr = picture_ctx->upd_items;
	if ( item_ptr != 0)
	{
	  while ( item_ptr->number != 0)
	  {
	    /* Check number */
	    if ( item_ptr->number < 1)
	      dtt_edit_print_syntaxerror( item_ptr, "Number out of range", 1, &errors, 
			&warnings);

	    /* Check text */
	    if ( strcmp( item_ptr->text, "") == 0)
	      dtt_edit_print_syntaxerror( item_ptr, "Text i missing", 1, &errors, 
			&warnings);
	    if ( strcmp( item_ptr->text, "%") == 0)
	      strcpy( item_ptr->priv, "NO");

	    /* Check Parameter */
	    if ( strcmp( item_ptr->parameter, "") == 0 &&
		 strcmp( item_ptr->type, "COMMAND") != 0)
	      dtt_edit_print_syntaxerror( item_ptr, "Parameter i missing", 1, &errors, 
			&warnings);

	    if ( strcmp (item_ptr->parameter, "RTT_ALARMTEXT1") == 0 ||
	         strcmp (item_ptr->parameter, "RTT_ALARMTEXT2") == 0 ||
	         strcmp (item_ptr->parameter, "RTT_ALARMTEXT3") == 0 ||
	         strcmp (item_ptr->parameter, "RTT_ALARMTEXT4") == 0 ||
	         strcmp (item_ptr->parameter, "RTT_ALARMTEXT5") == 0)
	    {
	      if ( strcmp (item_ptr->text, "%") != 0)
	        dtt_edit_print_syntaxerror( item_ptr, "Text should be % in alarmtext entry", 1,
			&errors, &warnings);
	      strcpy( item_ptr->priv, "NO");
	      strcpy( item_ptr->type, "UPDATE");
	      strcpy( item_ptr->outflags, "");
	      strcpy( item_ptr->database, "RTT");
	      strcpy( item_ptr->declaration, "STRING");
	    }
	    if ( strcmp (item_ptr->parameter, "RTT_TIME") == 0)
	    {
	      if ( strcmp (item_ptr->text, "%") != 0)
	        dtt_edit_print_syntaxerror( item_ptr, "Text should be % in time entry", 1, 
			&errors, &warnings);
	      strcpy( item_ptr->priv, "NO");
	      strcpy( item_ptr->database, "RTT");
	      strcpy( item_ptr->declaration, "STRING");	 
	      item_ptr->characters = 8;
	      strcpy( item_ptr->type, "UPDATE");
	      strcpy( item_ptr->outflags, "");
	    }
	    if ( strcmp (item_ptr->parameter, "RTT_TIME_FULL") == 0)
	    {
	      if ( strcmp (item_ptr->text, "%") != 0)
	        dtt_edit_print_syntaxerror( item_ptr, "Text should be % in time entry", 1, 
			&errors, &warnings);
	      strcpy( item_ptr->priv, "NO");
	      strcpy( item_ptr->database, "RTT");
	      strcpy( item_ptr->declaration, "STRING");	 
	      item_ptr->characters = 20;
	      strcpy( item_ptr->type, "UPDATE");
	      strcpy( item_ptr->outflags, "");
	    }
	

	    /* Check Parameter */
	    if ( strcmp( item_ptr->outflags, "TEXT") == 0 &&
		 strcmp( item_ptr->dualparameter, "") == 0)
	      dtt_edit_print_syntaxerror( item_ptr, "Dualparameter i missing", 1, &errors, 
			&warnings);

	    if ( strcmp( item_ptr->outflags, "FLASHTEXT") == 0 &&
		 strcmp( item_ptr->dualparameter, "") == 0)
	      dtt_edit_print_syntaxerror( item_ptr, "Dualparameter i missing", 1, &errors, 
			&warnings);

	    /* Check that it doesn't collide with another item */
	    item_p = picture_ctx->upd_items;
	    while ( item_p->number != 0)
	    {
	      if ( item_p == item_ptr)
	      {
	        item_p++;
	        continue;
	      }
	      if ( strcmp( item_ptr->outflags, "NO") == 0)
	        size = strlen( item_ptr->text);
	      else if ( strcmp( item_ptr->text, "%") == 0)
	        size = item_ptr->characters;
	      else
	        size = item_ptr->characters + strlen( item_ptr->text) + 1;

	      if ( item_ptr->y == item_p->y &&
	           item_ptr->x <= item_p->x && item_p->x < item_ptr->x + size)
	        dtt_edit_print_syntaxerror( item_ptr, "Field overlap", 0,
			&errors, &warnings);
	      item_p++;
	    }

	    item_ptr++;
	  }
	  if ( errors || warnings)
	    printf("\n\n\n");

	}
	*error_count = errors;
	*warning_count = warnings;

	return RTT__SUCCESS;
}

static int dtt_edit_print_syntaxerror(
			dtt_item 	item_ptr,
			char		*error_text,
			int		error,
			int		*error_count,
			int		*warning_count)
{
	if ( !*error_count && !*warning_count)
	  printf( "\n\n");

	if ( error)
	{
	  printf( "RTT-E-SYNTAX, %s\n", error_text);
	  (*error_count)++;
	}
	else
	{
	  printf( "RTT-W-SYNTAX, %s\n", error_text);
	  (*warning_count)++;
	}
	printf( "	in entry (%2d,%2d) \"%s U%d\"\n", item_ptr->x,
		item_ptr->y, item_ptr->text, item_ptr->number);

	return RTT__SUCCESS;	
}

/*************************************************************************
*
* Name:		dtt_setup()
*
* Type		int
*
* Type		Parameter	IOGF	Description
* menu_ctx	parent_ctx	I	parent rtt context.
*
* Description:
*	Display setup parameters.
*
**************************************************************************/

static int	dtt_setup( menu_ctx	parent_ctx)
{
	int		sts;
	char		title[80];
	rtt_t_menu_upd	*menulist = 0;
	int		i;
	unsigned long	elements;
	char 		*parameter_ptr;
	char		text[80];
	int		size;
	unsigned int	type;
	unsigned int	priv;
	float		maxlimit;
	float		minlimit;

	elements = 8;

	strcpy( title, "Rtt Editor Setup");

	/* Allocate memory for menu list */
	sts = rtt_menu_upd_list_add_malloc( &menulist, elements);
	if ( EVEN(sts)) return sts;

	for ( i = 0; i < (int)elements; i++)
	{
	  maxlimit = 0;
	  minlimit = 0;
	  if ( i == 0)
	  {
	    strcpy( text, "Program name");
	    parameter_ptr = dtt_programname;
	    size = sizeof( dtt_programname);
	    type = pwr_eType_String;
	    priv = RTT_PRIV_OP;
	  }
	  else if ( i == 1)
	  {
	    strcpy( text, "Main title");
	    parameter_ptr = dtt_maintitle;
	    size = sizeof( dtt_maintitle);
	    type = pwr_eType_String;
	    priv = RTT_PRIV_OP;
	  }
	  else if ( i == 2)
	  {
	    strcpy( text, "Title prefix");
	    parameter_ptr = dtt_title_prefix;
	    size = sizeof( dtt_title_prefix);
	    type = pwr_eType_String;
	    priv = RTT_PRIV_OP;
	  }
	  else if ( i == 3)
	  {
	    strcpy( text, "Operating system");
	    parameter_ptr = (char *) &dtt_opsys;
	    size = sizeof( dtt_opsys);
	    type = pwr_eType_Int32;
	    priv = RTT_PRIV_OP;
	  }	  
	  else if ( i == 4)
	  {
	    strcpy( text, "DefaulDirectory");
	    parameter_ptr = (char *) &rtt_default_directory;
	    size = sizeof( rtt_default_directory);
	    type = pwr_eType_String;
	    priv = RTT_PRIV_OP;
	  }	  
	  else if ( i == 5)
	  {
	    strcpy( text, "Verify");
	    parameter_ptr = (char *) &rtt_verify;
	    size = sizeof( rtt_verify);
	    type = pwr_eType_Boolean;
	    priv = RTT_PRIV_OP;
	  }	  
	  else if ( i == 6)
	  {
	    strcpy( text, "Source directory");
	    parameter_ptr = dtt_source_dir;
	    size = sizeof( dtt_source_dir);
	    type = pwr_eType_String;
	    priv = RTT_PRIV_OP;
	  }	  
	  else if ( i == 7)
	  {
	    strcpy( text, "Build directory");
	    parameter_ptr = dtt_build_dir;
	    size = sizeof( dtt_build_dir);
	    type = pwr_eType_String;
	    priv = RTT_PRIV_OP;
	  }	  

	  sts = rtt_menu_upd_list_add( &menulist, i, elements,
		text, 
		0, 
		0,
		0, pwr_cNObjid, 0, 0, 0, 0,
		text, priv, parameter_ptr, type, 
		0, size, pwr_cNDlid, 0, 0, 0, 0,
		maxlimit, minlimit, RTT_DATABASE_USER, 0);
	  if ( EVEN(sts)) return sts;
	}

	sts = rtt_menu_upd_new( parent_ctx, pwr_cNObjid, &menulist, title, 0, 
		RTT_MENUTYPE_DYN);
	if ( sts == RTT__FASTBACK) return sts;
	else if ( sts == RTT__BACKTOCOLLECT) return sts;
	else if ( EVEN(sts)) return sts;

	return RTT__SUCCESS;
}

static void	dtt_exit_now( pwr_tStatus exit_sts)
{
	qio_reset((int *) rtt_chn);
#if defined(OS_LYNX) || defined(OS_LINUX)
	/* Returnstatus 0 is OK for UNIX commands */
	if (EVEN(exit_sts))
	  exit( exit_sts);
	else
	  exit( 0);
#endif
#if defined(OS_VMS)
	exit( exit_sts);
#endif
}


static char *dtt_opsys_to_name( int opsys)
{
	static char name[20];
	switch (opsys)
	{
	  case pwr_mOpSys_VAX_ELN: strcpy( name, "VAX_ELN"); break;
	  case pwr_mOpSys_VAX_VMS: strcpy( name, "VAX_VMS"); break;
	  case pwr_mOpSys_AXP_VMS: strcpy( name, "AXP_VMS"); break;
	  case pwr_mOpSys_PPC_LYNX: strcpy( name, "PPC_LYNX"); break;
	  case pwr_mOpSys_X86_LYNX: strcpy( name, "X86_LYNX"); break;
	  case pwr_mOpSys_PPC_LINUX: strcpy( name, "PPC_LINUX"); break;
	  case pwr_mOpSys_X86_LINUX: strcpy( name, "X86_LINUX"); break;
	  case pwr_mOpSys_X86_64_LINUX: strcpy( name, "X86_64_LINUX"); break;
	  default: strcpy( name, "Unknwn");
	}
	return name;
}
