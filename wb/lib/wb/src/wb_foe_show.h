#ifndef wb_foe_show_h
#define wb_foe_show_h

/* wb_foe_show.h -- <short description>

   PROVIEW/R
   Copyright (C) 1996 by Comator Process AB.

   <Description>.  */

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
