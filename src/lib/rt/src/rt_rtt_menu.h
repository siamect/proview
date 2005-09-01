/* 
 * Proview   $Id: rt_rtt_menu.h,v 1.3 2005-09-01 14:57:56 claes Exp $
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

#ifndef rt_rtt_menu_h
#define rt_rtt_menu_h

#ifdef OS_VMS
# include <stdio.h>
# include <string.h>
# include <chfdef.h>
# include <signal.h>
#elif OS_ELN
# include stdio
# include string
# include chfdef
# include signal
#else
# include <stdio.h>
# include <string.h>
#endif

#include "rt_gdh.h"
#include "rt_rtt.h"
#include "rt_rtt_global.h"
#include "rt_rtt_functions.h"
/* ?????
#include "dtt_appl_rttsys_m.rdecl"
*/
#include "rt_rtt_msg.h"

#define NULLOI {0,0}

#if defined(OS_LYNX) || defined(OS_LINUX)
# define VAXC$ESTABLISH( t)	{}
#endif

#define RTT_HELP_START	\
extern	rtt_t_helptext	rtt_appl_helptext[];	\
rtt_t_helptext	rtt_appl_helptext[] = {

#define	RTT_HELP_SUBJ(subject)	\
{0, subject,

#define	RTT_HELP_INFO(infoline)	\
infoline,

#define RTT_HELP_TEXT(text)		\
text },

#define RTT_HELP_END 	\
{0,	"",}};

#define RTT_DB_START

#define RTT_DB_CONTINUE	\
rtt_t_db rtt_appl_db[] = {	\
{ "RTT_TIME", rtt_time},

#define RTT_DB_END	\
{ "", },};

#define RTT_RTTSYSDB_START

#define RTT_RTTSYSDB_CONTINUE	\
rtt_t_db rtt_rttsys_db[] = {

#define RTT_RTTSYSDB_END	\
{ "", },};

#define	RTT_MENU_START( menu_name)	\
rtt_t_menu	menu_name[] = {		

#ifdef OS_ELN
#define RTT_MENU_END( menu_name)	\
{"",0,0,0,NULLOI,0,0,0,0} };				\
rtt_t_menu	*menu_name/**/_ptr = (rtt_t_menu *) &menu_name;
#else
#define RTT_MENU_END( menu_name)	\
{"",0,0,0,NULLOI,0,0,0,0} };				\
rtt_t_menu	*menu_name##_ptr = (rtt_t_menu *) &menu_name;
#endif


#define	RTT_MENU_UPDATE_START( menu_name)	\
rtt_t_menu_update	menu_name[] = {		

#ifdef OS_ELN
#define RTT_MENU_UPDATE_END( menu_name)	\
{"",} };				\
rtt_t_menu_update	*menu_name/**/_ptr = (rtt_t_menu_update *) &menu_name;
#else
#define RTT_MENU_UPDATE_END( menu_name)	\
{"",} };				\
rtt_t_menu_update	*menu_name##_ptr = (rtt_t_menu_update *) &menu_name;
#endif

/***********************************************************
* macroname	RTT_MENU_UPDATE
*
* Parameters
* string	title		text in menu item
* string	parameter	full rtdb name of parameter to show
* unsigned long priv		privileges and input/output flags
*
* Description:	Show the value of the parameter and set
*	the value when PF1 is pressed.
*************************************************************/

#define	RTT_MENU_UPDATE( title, parameter, priv)	\
{ title, 0, 0, parameter, "", priv, 0, 0, 0, 0, 0.0, 0.0},

/***********************************************************
* macroname	RTT_MENU_SET
*
* Parameters
* string	title		text in menu item
* string	parameter	full rtdb name of parameter
* unsigned long priv		privileges and input/output flags
*
* Description:	Show the value of the parameter and set
*	the value when PF1 is pressed.
*************************************************************/

#define	RTT_MENU_SET( title, parameter, priv)	\
{ title, &rtt_menu_parameter_set, 0, parameter, "", priv | RTT_MENU_NOINPUT,\
0, 0, 0, 0, 0.0, 0.0},
		
/***********************************************************
* macroname	RTT_MENU_RESET
*
* Parameters
* string	title		text in menu item
* string	parameter	full rtdb name of parameter
* unsigned long priv		privileges and input/output flags
*
* Description:	Show the value of the parameter and reset
*	the value when PF1 is pressed.
*************************************************************/

#define	RTT_MENU_RESET( title, parameter, priv)	\
{ title, &rtt_menu_parameter_reset, 0, parameter, "",\
priv | RTT_MENU_NOINPUT, 0, 0, 0, 0, 0.0, 0.0},
		
/***********************************************************
* macroname	RTT_MENU_SET_RESET
*
* Parameters
* string	title		text in menu item
* string	parameter	full rtdb name of parameter
* unsigned long priv		privileges and input/output flags
*
* Description:	Show the value of the parameter and set
*	the value when PF1 is pressed and reset when PF2 
*	is pressed.
*************************************************************/

#define	RTT_MENU_SET_RESET( title, parameter, priv)	\
{ title, &rtt_menu_parameter_set, &rtt_menu_parameter_reset, parameter, "",\
priv | RTT_MENU_NOINPUT, 0, 0, 0, 0, 0.0, 0.0},
		
/***********************************************************
* macroname	RTT_MENU_TOGGLE
*
* Parameters
* string	title		text in menu item
* string	parameter	full rtdb name of parameter
* unsigned long priv		privileges and input/output flags
*
* Description:	Show the value of the parameter and toggle
*	the value when PF1 is pressed.
*************************************************************/

#define	RTT_MENU_TOGGLE( title, parameter, priv)	\
{ title, &rtt_menu_parameter_toggle, 0, parameter, "", priv | RTT_MENU_NOINPUT,\
0, 0, 0, 0, 0.0, 0.0},
		
/***********************************************************
* macroname	RTT_MENU_DUAL_SET
*
* Parameters
* string	title		text in menu item
* string	show_parameter	full rtdb name of parameter to show
* string	set_parameter	full rtdb name of parameter to set
* unsigned long priv		privileges and input/output flags
*
* Description:	Show the value of one parameter and set
*	the value of another parameter when PF1 is pressed.
*************************************************************/

#define	RTT_MENU_DUAL_SET( title, show_parameter, set_parameter, priv)	\
{ title, &rtt_menu_parameter_dual_set, 0, show_parameter, set_parameter,\
priv | RTT_MENU_NOINPUT, 0, 0, 0, 0, 0.0, 0.0},
		
/***********************************************************
* macroname	RTT_MENU_DUAL_RESET
*
* Parameters
* string	title		text in menu item
* string	show_parameter	full rtdb name of parameter to show
* string	reset_parameter	full rtdb name of parameter to reset
* unsigned long priv		privileges and input/output flags
*
* Description:	Show the value of one parameter and reset
*	the value of another parameter when PF1 is pressed.
*************************************************************/

#define	RTT_MENU_DUAL_RESET( title, show_parameter, reset_parameter, priv) \
{ title, &rtt_menu_parameter_dual_reset, 0, show_parameter, reset_parameter, \
priv | RTT_MENU_NOINPUT, 0, 0, 0, 0, 0.0, 0.0},
		
/***********************************************************
* macroname	RTT_MENU_DUAL_SET_RESET
*
* Parameters
* string	title		text in menu item
* string	show_parameter	full rtdb name of parameter to show
* string	sr_parameter	full rtdb name of parameter to set or reset
* unsigned long priv		privileges and input/output flags
*
* Description:	Show the value of one parameter and set
*	the value of another parameter when PF1 is pressed
*	and reset it with PF2.
*************************************************************/

#define	RTT_MENU_DUAL_SET_RESET( title, show_parameter, sr_parameter, priv) \
{ title, &rtt_menu_parameter_dual_set, &rtt_menu_parameter_dual_reset,\
show_parameter, sr_parameter, priv | RTT_MENU_NOINPUT, 0, 0, 0, 0, 0.0, 0.0},
		
/***********************************************************
* macroname	RTT_MENU_DUAL_TOGGLE
*
* Parameters
* string	title		text in menu item
* string	show_parameter	full rtdb name of parameter to show
* string	toggle_parameter full rtdb name of parameter to toggle
* unsigned long priv		privileges and input/output flags
*
* Description:	Show the value of one parameter and toggle
*	the value of another parameter when PF1 is pressed.
*************************************************************/

#define	RTT_MENU_DUAL_TOGGLE( title, show_parameter, toggle_parameter, priv) \
{ title, &rtt_menu_parameter_dual_toggle, 0, show_parameter, toggle_parameter,\
priv | RTT_MENU_NOINPUT, 0, 0, 0, 0, 0.0, 0.0},
		
#define	RTT_MENUITEM_OBJECTS( title)	\
{ title, &rtt_hierarchy, 0, 0, NULLOI, 0, 0, 0, 0},

#define	RTT_MENUITEM_EXIT( title)	\
{ title, &rtt_exit, &rtt_exit, 0, NULLOI, 0, 0, 0, 0},

#define	RTT_MENUITEM_COMMAND( title, command)	\
{ title, &rtt_menu_command, 0, 0, NULLOI, (void *) command, 0, 0, 0},

#define	RTT_MENUITEM_COMMANDHOLD( title, command)	\
{ title, &rtt_menu_commandhold, 0, 0, NULLOI, (void *) command, 0, 0, 0},

#define	RTT_MENUITEM_FILECOMMAND( title, filespec, command)	\
{ title, &rtt_menu_filecommand, 0, 0, NULLOI, (void *) filespec, (void *)command, 0, 0},

#define	RTT_MENUITEM_VMSCOMMAND( title, command)	\
{ title, &rtt_menu_vmscommand, 0, 0, NULLOI, (void *) command, 0, 0, 0},

#define	RTT_MENUITEM_VMSCOMMAND_NOWAIT( title, command)	\
{ title, &rtt_menu_vmscommand_nowait, 0, 0, NULLOI, (void *) command, 0, 0, 0},

#define	RTT_MENUITEM_VMSCOMMANDCONF( title, command)	\
{ title, &rtt_menu_vmscommandconf, 0, 0, NULLOI, (void *) command, 0, 0, 0},

#define	RTT_MENUITEM_VMSCOMMANDHOLD( title, command)	\
{ title, &rtt_menu_vmscommandhold, 0, 0, NULLOI, (void *) command, 0, 0, 0},

#define	RTT_MENU_NO( title)	\
{ title, 0, 0, 0, NULLOI, 0, 0, 0, 0},

#ifdef OS_ELN
#define	RTT_MENU_NEW( title, menu_name)	\
{ title, &rtt_menu_new, 0, 0, NULLOI, (void *) &menu_name/**/_ptr, \
(void *) title, 0, (void *) RTT_MENUTYPE_STAT},
#else
#define	RTT_MENU_NEW( title, menu_name)	\
{ title, &rtt_menu_new, 0, 0, NULLOI, (void *) &menu_name##_ptr, \
(void *) title, 0, (void *) RTT_MENUTYPE_STAT},
#endif

#ifdef OS_ELN
#define	RTT_MENU_KEYS_NEW( title, menu_name)	\
{ title, &rtt_menu_keys_new, 0, 0, NULLOI, (void *) &menu_name/**/_ptr, \
(void *) title, 0, (void *) RTT_MENUTYPE_STAT},
#else
#define	RTT_MENU_KEYS_NEW( title, menu_name)	\
{ title, &rtt_menu_keys_new, 0, 0, NULLOI, (void *) &menu_name##_ptr, \
(void *) title, 0, (void *) RTT_MENUTYPE_STAT},
#endif

#ifdef OS_ELN
#define	RTT_MENU_UPDATE_NEW( title, menu_name)	\
{ title, &rtt_menu_new_update, 0, 0, NULLOI, (void *) &menu_name/**/_ptr, \
(void *) title, 0, (void *) RTT_MENUTYPE_DYN},
#else
#define	RTT_MENU_UPDATE_NEW( title, menu_name)	\
{ title, &rtt_menu_new_update, 0, 0, NULLOI, (void *) &menu_name##_ptr, \
(void *) title, 0, (void *) RTT_MENUTYPE_DYN},
#endif

#ifdef OS_ELN
#define	RTT_MENU_UPEDIT_NEW( title, picture_name, function)	\
{ title, &rtt_menu_new_upedit, 0, 0, NULLOI, \
(void *) &picture_name/**/_euptr, \
(void *)  title, (void *) &picture_name/**/_bg, (void *) function },
#else
#define	RTT_MENU_UPEDIT_NEW( title, picture_name, function)	\
{ title, &rtt_menu_new_upedit, 0, 0, NULLOI, \
(void *) &picture_name##_euptr, \
(void *)  title, (void *) &picture_name##_bg, (void *) function },
#endif

#ifdef OS_ELN
#define	RTT_MENU_UPEDITPERM_NEW( title, picture_name, function)	\
{ title, &rtt_menu_new_upeditperm, 0, 0, NULLOI, \
(void *) &picture_name/**/_euptr, \
(void *)  title, (void *) &picture_name/**/_bg, (void *) function },
#else
#define	RTT_MENU_UPEDITPERM_NEW( title, picture_name, function)	\
{ title, &rtt_menu_new_upeditperm, 0, 0, NULLOI, \
(void *) &picture_name##_euptr,\
(void *)  title, (void *) &picture_name##_bg, (void *) function },
#endif

#define	RTT_MENU_SYSEDIT_NEW( title, objectname, function)	\
{ title, &rtt_menu_new_sysedit, 0, 0, NULLOI, (void *) objectname, \
(void *) title, 0, (void *) function },

#define	RTT_MENU( title, fun1, fun2, fun3, NULLOI, arg1, arg2, arg3, arg4) \
{ title, fun1, fun2, fun3, arg1, arg2, arg3, arg4},

#define	RTT_MAINMENU_START( main_title)	\
char		mainmenu_title[80] = main_title;	\
rtt_t_menu	rtt_mainmenu[] = {	

#define RTT_MAINMENU_END	\
{"",0,0,0, NULLOI, 0,0,0,0}				\
};						\
rtt_t_menu	*mainmenu_ptr = (rtt_t_menu *) &rtt_mainmenu; \
						\
unsigned int	rtt_exception(	void	*signalP, void	*mechanismP); \
						\
int main(int argc, char *argv[])		\
{						\
int 	sts, i;					\
char	login[20];				\
char	commandfile[80];			\
						\
VAXC$ESTABLISH( rtt_exception);			\
rtt_args = argc - 1;				\
for ( i = 0; i < rtt_args; i++)			\
{						\
  if ( i >= (int)(sizeof(rtt_arg)/sizeof(rtt_arg[0])))	\
    break;					\
  strncpy( rtt_arg[i], argv[i+1], sizeof(rtt_arg[i]));\
  rtt_toupper( rtt_arg[i], rtt_arg[i]);		\
}						\
if ( argc >= 2 )				\
{						\
  strcpy( login, argv[1]);			\
  rtt_toupper( login, login);			\
  if ( argc >= 3 )				\
  {						\
    strcpy( commandfile, argv[2]);		\
    if (argc >= 4)				\
    {						\
      strcpy(rtt_ConfigureObject,argv[3]);	\
      if (argc >= 5)				\
      {						\
        strcpy( mainmenu_title, argv[4]);	\
      }						\
    }						\
  }						\
  else						\
    strcpy( commandfile, "");			\
}						\
else						\
{						\
  strcpy( login, "SYS");	       		\
  strcpy( commandfile, "");			\
}						\
sts = rtt_initialize( login, commandfile, mainmenu_title);	\
sts = rtt_menu_new( 0, pwr_cNObjid, &mainmenu_ptr, rtt_mainmenu_title,\
	0, RTT_MENUTYPE_STAT); \
rtt_error_msg(sts);				\
return 1;					\
}

#endif
