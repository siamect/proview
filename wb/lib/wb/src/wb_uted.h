/** 
 * Proview   $Id: wb_uted.h,v 1.2 2005-09-01 14:57:59 claes Exp $
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

#ifndef wb_uted_h
#define wb_uted_h

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

#define UTED_MODE_EDIT 	0
#define UTED_MODE_VIEW 	1

#define	UTED_QUALS	10

#define UTED_PROC_PWRPLC	0
#define	UTED_PROC_DCL	1

#define UTED_QUAL_QUAL	0
#define UTED_QUAL_PAR	1
#define UTED_QUAL_DEFQUAL 2

#define UTED_INS_PLANT	0
#define UTED_INS_NODE		1
#define UTED_INS_PLNO		2
#define UTED_INS_NOPL		3

#define	UTED_BATCH_BATCH		0
#define UTED_BATCH_CURRSESS	1

#define UTED_MAX_COMMANDS	40

struct uted_widgets 
{
  Widget	uted_window;
  Widget	label;
  Widget        adb;
  Widget	file_entry;
  Widget	quit;
  Widget	batchoptmenu;
  Widget	commandlabel;
  Widget	batch;
  Widget	currsess;
  Widget	help;
  Widget	timelabel;
  Widget	timevalue;
  Widget	qualifier[UTED_QUALS];
  Widget	value[UTED_QUALS];
  Widget	present[UTED_QUALS];
  Widget	optmenubuttons[UTED_MAX_COMMANDS];
  Widget	command_window;
  Widget	questionbox;
  Widget	commandwind_button;
};

typedef struct s_uted_ctx
{
  t_commonpart		cp;
  struct		uted_widgets widgets ;
  int			current_index;
  int			present_sts[UTED_QUALS];
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
} t_uted_ctx, *uted_ctx;

pwr_tStatus uted_new (
  void			*parent_ctx,
  Widget		parent_wid,
  char			*name,
  char			*iconname,
  ldh_tWBContext	ldhwb,
  ldh_tSesContext	ldhses,
  int			editmode,
  uted_ctx		*return_utedctx,
  void 			(*quit_cb)( void *)
);

void uted_raise_window ( 
  uted_ctx utedctx
);

void uted_delete(
  uted_ctx	utedctx
);

void uted_set_editmode (
  uted_ctx 	utectx,
  int		edit,
  ldh_tSesContext ldhses
);
#endif
