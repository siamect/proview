/* 
 * Proview   $Id: wb_foe_show.h,v 1.3 2005-09-06 10:43:31 claes Exp $
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

#ifndef wb_foe_show_h
#define wb_foe_show_h

#ifndef wb_ldh_h
#include "wb_ldh.h"
#endif

#ifndef _XtIntrinsic_h
#include <X11/Intrinsic.h>
#endif

typedef struct {
	ldh_tWBContext	ldhwb;
	ldh_tVolContext	ldhvol;
	ldh_tSesContext	ldhses;
	unsigned int 	argc;
	char 		**argv;
	Widget		parentwidget;
	ldh_tSesContext	*seslst;
	int		session_owner;
	pwr_tObjid	created_object;
	int		db_set;
	} *show_ctx;

extern int foe_show_verify;
extern int foe_show_message;

pwr_tStatus foe_show_set_ldhses( 
  show_ctx	showctx
);

pwr_tStatus	foe_show_reset_ldhses();

pwr_tStatus	foe_get_ldhses(
  show_ctx	showctx
);

int  foe_get_qualval ( 
    char *qual,
    char *qual_val,
    int	sendlen
);

int  foe_get_qual (
    char *qual,
    pwr_tStatus *condval_ptr
);


int  foe_get_attr_value(
	char		*attrname,
	char		*text
);

pwr_tStatus foe_show_create_ctx(
  show_ctx		*showctx,
  ldh_tSesContext	ldhses,
  int			argc,
  char			**argv,
  Widget		parentwidget,
  int			session_owner
);

void	foe_show_delete_ctx(
  show_ctx	showctx
);

pwr_tStatus foe_ldh_event_bc(
			  void *ctx,
			  ldh_sEvent *event);


pwr_tStatus foe_show_get_created_object(
			pwr_tObjid *objid);

#endif
