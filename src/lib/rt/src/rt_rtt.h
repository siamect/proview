/* 
 * Proview   $Id: rt_rtt.h,v 1.4 2005-09-01 14:57:56 claes Exp $
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

#ifndef rt_rtt_h
#define rt_rtt_h

#if 0
#ifdef OS_VMS
# include <stdio.h>
# define int32 ___int32
# define uint32 ___uint32
# define int64 ___int64
# define uint64 ___uint64
# include <pthread.h>
# undef int32
# undef uint32
# undef int64
# undef uint64
#endif
#endif

#ifdef OS_VMS
# include <stdio.h>
# include <pthread.h>
#endif

#ifdef OS_ELN
#include stdio
#endif

#ifndef rt_gdh_h
#include "rt_gdh.h"
#endif
#ifndef rt_mh_outunit_h
#include "rt_mh_outunit.h"
#endif

#define RTT_CTXTYPE_MENU	0	
#define RTT_CTXTYPE_VIEW	1	

#define	RTT_CHARSET_ASCII	0
#define	RTT_CHARSET_NOINVERSE	0
#define	RTT_CHARSET_LINE	1
#define	RTT_CHARSET_INVERSE	2
#define	RTT_CHARSET_ITEM	4

#define	RTT_COMMANDMODE_FILE	1
#define	RTT_COMMANDMODE_LEARN	2
#define	RTT_COMMANDMODE_SINGLE	4

#define	RTT_COMMAND_COMMAND 	0
#define	RTT_COMMAND_PICTURE 	1
#define	RTT_COMMAND_VALUE	2

#define	RTT_PRV_OP		1
#define	RTT_PRV_EL		3
#define	RTT_PRV_PROC		5
#define	RTT_PRV_SYS		15

#define	RTT_PRIV_NO		0
#define	RTT_PRIV_OP		1
#define	RTT_PRIV_EL		2
#define	RTT_PRIV_PROC		4
#define	RTT_PRIV_SYS		8
#define RTT_PRIV_NOOP		6
#define	RTT_MENU_NOINPUT	16
#define RTT_OUTPUT_ONOFF	32
#define RTT_OUTPUT_TRUEFALSE	64
#define RTT_OUTPUT_OPENCLOSED	128
#define RTT_OUTPUT_AUTOMAN	256
#define RTT_OUTPUT_NO		512
#define RTT_OUTPUT_BAR		1024
#define RTT_OUTPUT_TEXT		2048
#define RTT_OUTPUT_FLASHTEXT    6144
#define RTT_OUTPUT_FLASH        4096

#define RTT_PRIV_MASK		15

#define RTT_DATABASE_GDH	0
#define RTT_DATABASE_RTT	1
#define RTT_DATABASE_USER	2
#define RTT_DATABASE_RTTSYS	3

#define RTT_APPL_INIT		0
#define RTT_APPL_EXIT		1
#define RTT_APPL_VALUECHANGED	2
#define RTT_APPL_UPDATE		3
#define RTT_APPL_NEXTPAGE	4
#define RTT_APPL_PREVPAGE	5
#define RTT_APPL_MENU		6
#define RTT_APPL_PICTURE	7

#define RTT_LOGG_CONT		1
#define RTT_LOGG_MOD		2

#define RTT_DECL_		0
#define RTT_DECL_INT		1
#define RTT_DECL_BOOLEAN	2
#define RTT_DECL_FLOAT		3
#define RTT_DECL_CHAR		4
#define RTT_DECL_STRING		5
#define RTT_DECL_STRING4	5
#define RTT_DECL_STRING10	5
#define RTT_DECL_STRING20	5
#define RTT_DECL_STRING40	5
#define RTT_DECL_SHORT		6
#define RTT_DECL_OBJID		7
#define RTT_DECL_TIME		8
#define RTT_DECL_ATTRREF	9

#define	RTT_MENU_CREATE		0
#define	RTT_MENU_ADD		1

#define	RTT_MENUTYPE_MENU	1
#define	RTT_MENUTYPE_UPD	2
#define	RTT_MENUTYPE_STAT	4
#define	RTT_MENUTYPE_DYN	8
#define	RTT_MENUTYPE_ALARM	16
#define	RTT_MENUTYPE_EDIT	32

#define	RTT_VIEWTYPE_FILE	1
#define	RTT_VIEWTYPE_BUF	2

#define RTT_ERASE		9999

#define	RTT_K_MAXLEN		0

#define	RTT_ROW_COMMAND		23
#define	RTT_ROW_MESSAGE		24
#define	RTT_ROW_INFO		22


#define RTT_TERM		1000
#define	RTT_K_NONE	 	0
#define	RTT_K_ARROW_UP 		274
#define	RTT_K_ARROW_DOWN 	275
#define	RTT_K_ARROW_RIGHT 	276
#define	RTT_K_ARROW_LEFT 	277
#define	RTT_K_PF1		278
#define	RTT_K_PF2		279
#define	RTT_K_PF3		280
#define	RTT_K_PF4		281
#define	RTT_K_TIMEOUT		282
#define	RTT_K_PREVPAGE		283
#define	RTT_K_NEXTPAGE		284
#define	RTT_K_HELP		285
#define	RTT_K_COMMAND		286
#define	RTT_K_SHIFT_ARROW_RIGHT 287
#define	RTT_K_SHIFT_ARROW_LEFT 	288
#if defined(OS_LYNX) || defined(OS_LINUX)
# define RTT_K_RETURN 		10
#else
# define RTT_K_RETURN 		13
#endif
#define	RTT_K_DELETE		127
#define	RTT_K_BACKSPACE		8
#define	RTT_K_CTRLA		1
#define	RTT_K_CTRLB		2
#define	RTT_K_CTRLC		3
#define	RTT_K_CTRLD		4
#define	RTT_K_CTRLE		5
#define	RTT_K_CTRLF		6
#define	RTT_K_CTRLH		8
#define	RTT_K_CTRLK		11
#define	RTT_K_CTRLL		12
#define	RTT_K_CTRLN		14
#define	RTT_K_CTRLR		18
#define	RTT_K_CTRLT		20
#define	RTT_K_CTRLV		22
#define	RTT_K_CTRLW		23
#define	RTT_K_CTRLZ		26
#define	RTT_K_ESCAPE		27
#define	RTT_K_FAST		300
#define	RTT_K_FAST_1		301
#define	RTT_K_FAST_2		302
#define	RTT_K_FAST_3		303
#define	RTT_K_FAST_4		304
#define	RTT_K_FAST_5		305
#define	RTT_K_FAST_6		306
#define	RTT_K_FAST_7		307
#define	RTT_K_FAST_8		308
#define	RTT_K_FAST_9		309
#define	RTT_K_FAST_10		310
#define	RTT_K_FAST_11		311
#define	RTT_K_FAST_12		312
#define	RTT_K_FAST_13		313
#define	RTT_K_FAST_14		314
#define	RTT_K_FAST_15		315
#define	RTT_K_FAST_16		316
#define	RTT_K_FAST_17		317
#define	RTT_K_FAST_18		318
#define	RTT_K_FAST_19		319
#define	RTT_K_FAST_20		320
#define	RTT_K_FAST_21		321
#define	RTT_K_FAST_22		322

#define RTT_OPT_NORECALL	1
#define RTT_OPT_NOEDIT		2
#define RTT_OPT_NOECHO		4
#define RTT_OPT_NOPFTAN		8
#define RTT_OPT_TIMEOUT		16
#define RTT_OPT_NOSCROLL	32

#define RTT_QUIET_ALL		1
#define RTT_QUIET_MESSAGE	2

#define RTT_RECALL_MAX		30

#define RTT_MENU_MAXCOLS	79
#define RTT_MENU_MAXROWS	20

#define RTT_RTC_SHOW		1
#define RTT_RTC_GET		2
#define RTT_RTC_SET		3

#define RTT_CLASSORT_SIZE	4


typedef struct {
	int	first_command;
	int	last_command;
	char	command[RTT_RECALL_MAX][200];
	} rtt_t_recall;

typedef struct {
	char		text[80+RTT_CLASSORT_SIZE];
	int		(* func) ();
	int		(* func2) ();
	int		(* func3) ();
	pwr_tObjid	argoi;
	void		*arg1;
	void		*arg2;
	void		*arg3;
	void		*arg4;
	char		argstr[80];
	int		index;
	int		type;
	} rtt_t_menu;

typedef struct {
	char		text[80];
	int		(* func) ();
	int		(* func2) ();
	int		(* func3) ();
	pwr_tObjid	argoi;
	void		*arg1;
	void		*arg2;
	void		*arg3;
	void		*arg4;
	unsigned long	priv;
	char		parameter_name[110];
	char		*value_ptr;
	unsigned long	value_type;
	unsigned long	flags;
	unsigned long	size;
	gdh_tSubid	subid;
	unsigned long	value_x;
	unsigned long	value_y;
	char		old_value[80];
	char		characters;
	char		decimals;
	float		maxlimit;
	float		minlimit;
	int		database;
	char		output_text[110];
	} rtt_t_menu_upd;

typedef struct {
	char		text[80];
	int		(* func) ();
	int		(* func2) ();
	int		(* func3) ();
	pwr_tObjid	argoi;
	void		*arg1;
	void		*arg2;
	void		*arg3;
	void		*arg4;
	int		id;
	pwr_tTime	time;
	char		eventtext[80];
	char		eventname[80];
	int		eventflags;
	unsigned long	type;
	unsigned long	eventprio;
	mh_sEventId	eventid;
	pwr_tObjid	object;
	unsigned long	status;
	} rtt_t_menu_alarm;

typedef struct {
	unsigned long	ctx_type;
	void 		*parent_ctx;
	unsigned long	commandmode_id;
	rtt_t_menu	*menu;
	int		menutype;
	unsigned long	update_init;
	char		title[80];
	int		current_page;
	int		current_item;
	int		current_row;
	int		current_col;
	int		no_items;
	int		no_pages;
	int		rows;
	int		cols;
	int		max_item_size;
	int		page_len;
	int		row_len;
	int		col_len;
	int		left_margin;
	int		up_margin;
	int		col_size;
	int		row_size;
	int		(* appl_func) ();
	} rtt_t_menu_ctx, *menu_ctx;

typedef struct {
	unsigned long	ctx_type;
	menu_ctx	parent_ctx;
	char		title[80];
	FILE		*infile;
	char		*inbuff;
	char		*buff;
	int		*buffrow;
	int		buffrow_count;
	int		buffstart_row;
	int		start_row;
	int		first;
	char		*read_sts;
	int		buff_read_complete;
	} *view_ctx;



typedef	struct	{
	char	command[20];
	int	(*func) ();
	char	qualifier[30][40];
	} rtt_t_comtbl;

typedef	struct	{
	int	view_in_list;
	char	subject[80];
	char	infoline[80];
	char	text[1500];
	} rtt_t_helptext;

typedef struct {
	char		text[80];
	int		(*func1) ();
	int		(*func2) ();
	char		parameter_name[110];
	char		dualparameter_name[110];
	unsigned long	priv;
	int		x;
	int		y;
	char		characters;
	char		decimals;
	float		maxlimit;
	float		minlimit;
	int		database;
	int		declaration;
	} rtt_t_menu_update;

typedef struct {
	int		number;
	char		text[80];
	char		type[80];
	char		parameter[120];
	char		dualparameter[120];
	char		priv[80];
	char		outflags[80];
	char		characters;
	char		decimals;
	float		maxlimit;
	float		minlimit;
	char		database[10];
	char		declaration[10];
	int		x;
	int		y;
	} rtt_t_eup;

typedef	int	rtt_t_backgr[2][80][22];

typedef struct {
	char		parameter[80];
	char		*parameter_ptr;
	} rtt_t_db;

#define	RTT_LOGG_MAXENTRY	10
#define	RTT_LOGG_MAXPAR		100
#define RTT_BUFFER_DEFSIZE	100
#define RTT_LOGG_LINE_DEFSIZE	512

typedef struct {
	char		active;
	char		occupied;
	char		intern;
	char		stop;
	int		logg_type;
	int		logg_priority;
	char		parameterstr[RTT_LOGG_MAXPAR][120];
	char		shortname[RTT_LOGG_MAXPAR][120];
	char		*parameter_ptr[RTT_LOGG_MAXPAR];
	gdh_tDlid	parameter_subid[RTT_LOGG_MAXPAR];
	unsigned long	parameter_type[RTT_LOGG_MAXPAR];
	unsigned long	parameter_size[RTT_LOGG_MAXPAR];
	char		conditionstr[120];
	char		*condition_ptr;
	gdh_tDlid	condition_subid;
	int		logg_time;
	char		logg_filename[80];
	void		*logg_file;	
	char		old_value[RTT_LOGG_MAXPAR][8];
	pwr_tTime	starttime;
#ifdef OS_ELN
	PROCESS		process_id;
#endif
#ifdef OS_VMS
	pthread_t 	thread;
	unsigned int    event_flag;
#endif
#if defined  OS_LYNX || defined OS_LINUX
	pthread_t 	thread;
#endif
	int		line_size;
	int		parameter_count;
	int		print_shortname;
	int		buffer_size;
	int		buffer_count;
	char		*buffer_ptr;
	} rtt_t_loggtable;

#ifndef rt_rtt_functions_h
#include "rt_rtt_functions.h"
#endif

/*
int	rtt_menu_new();
int	rtt_menu_keys_new();
int	rtt_menu_new_update();
int	rtt_menu_upd_update();
int	rtt_menu_new_upedit();
int	rtt_menu_new_upeditperm();
int	rtt_menu_new_sysedit();
int	rtt_scan();
int	rtt_menu_command();
int	rtt_menu_commandhold();
int	rtt_menu_vmscommand();
int	rtt_menu_vmscommand_nowait();
int	rtt_menu_vmscommandconf();
int	rtt_menu_vmscommandhold();
int	rtt_hierarchy();
int	rtt_exit();
int	dummy();
int	rtt_hierarchy_child();
int	rtt_debug_child();
int	rtt_object_parameters();
int	rtt_menu_parameter_set();
int	rtt_menu_parameter_reset();
int	rtt_menu_parameter_toggle();
int	rtt_menu_parameter_dual_set();
int	rtt_menu_parameter_dual_reset();
int	rtt_menu_parameter_dual_toggle();
int	rtt_alarm_update();
int	rtt_edit_debug_signals();
int	rtt_crossref_signal();
*/

#endif
