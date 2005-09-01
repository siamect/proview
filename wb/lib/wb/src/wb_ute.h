/** 
 * Proview   $Id: wb_ute.h,v 1.2 2005-09-01 14:57:59 claes Exp $
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

#ifndef wb_ute_h
#define wb_ute_h

#ifndef wb_ldh_h
#include "wb_ldh.h"
#endif

#ifndef wb_foe_macros_h
#include "wb_foe_macros.h"
#endif

#ifndef _XtIntrinsic_h
#include <Xm/Intrinsic.h>
#endif

#ifndef wb_foe_show_h
#include "wb_foe_show.h"
#endif

#define UTE_MODE_EDIT 	0
#define UTE_MODE_VIEW 	1

#define	UTE_QUALS	10

#define UTE_PROC_PWRPLC	0
#define	UTE_PROC_DCL	1

#define UTE_QUAL_QUAL	0
#define UTE_QUAL_PAR	1
#define UTE_QUAL_DEFQUAL 2

#define UTE_INS_PLANT	0
#define UTE_INS_NODE		1
#define UTE_INS_PLNO		2
#define UTE_INS_NOPL		3

#define	UTE_BATCH_BATCH		0
#define UTE_BATCH_CURRSESS	1

#define UTE_MAX_COMMANDS	40

struct ute_widgets 
{
  Widget	ute_window;
  Widget	label;
  Widget        adb;
  Widget	file_entry;
  Widget	quit;
  Widget	save;
  Widget	revert;
  Widget	edit_togg;
  Widget	batchoptmenu;
  Widget	commandlabel;
  Widget	batch;
  Widget	currsess;
  Widget	help;
  Widget	timelabel;
  Widget	timevalue;
  Widget	qualifier[UTE_QUALS];
  Widget	value[UTE_QUALS];
  Widget	present[UTE_QUALS];
  Widget	optmenubuttons[UTE_MAX_COMMANDS];
  Widget	command_window;
  Widget	questionbox;
  Widget	commandwind_button;
};

typedef struct s_ute_ctx
{
  t_commonpart		cp;
  struct		ute_widgets widgets ;
  int			current_index;
  int			present_sts[UTE_QUALS];
  int			batch_sts;
  ldh_tWBContext	ldhwb;
  ldh_tSesContext	ldhses;
  int			mode;
  void			(* questionbox_yes) ();
  void			(* questionbox_no) ();
  void			(* questionbox_cancel) ();
  show_ctx		showctx;
  void 			(*quit_cb)(void *parent_ctx);
  int			dummy[20];
} t_ute_ctx, *ute_ctx;

pwr_tStatus ute_new (
  void			*parent_ctx,
  Widget		parent_wid,
  char			*name,
  char			*iconname,
  ldh_tWBContext	ldhwb,
  ldh_tSesContext	ldhsession,
  ute_ctx		*return_utectx,
  void 			(*quit_cb)( void *)
);

void ute_raise_window ( 
  ute_ctx utectx
);

void ute_delete(
  ute_ctx	utectx
);

#endif
