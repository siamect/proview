#if defined OS_VMS || defined OS_LINUX

#if defined OS_VMS && defined __ALPHA
# pragma message disable (NOSIMPINT,EXTROUENCUNNOBJ)
#endif

#if defined OS_VMS && !defined __ALPHA
# pragma message disable (LONGEXTERN)
#endif

#include <stdio.h>
#include <stdlib.h>
#include <Xm/Xm.h>
#include <Mrm/MrmPublic.h>
#ifndef _XtIntrinsic_h
#include <X11/Intrinsic.h>
#endif
#include <X11/IntrinsicP.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xatom.h>
#include <X11/Xmu/Atoms.h>
#include <X11/Xmu/StdSel.h>

extern "C" {
#include "wb_ldh.h"
#include "wb_ldh_msg.h"
#include "co_cdh.h"
#include "flow_x.h"
}

#include "flow.h"
#include "flow_browctx.h"
#include "flow_browapi.h"
#include "flow_browwidget.h"
#include "wb_nav.h"

#include "ge_plant.h"

void *plant_new(
	void *parent_ctx,
	Widget	parent_wid,
	char *name,
	ldh_tSesContext ldhses,
	char *root_name,
	Widget *w,
	pwr_tStatus *status,
        int (*get_plant_select_cb)( void *, char *)
	)
{
  Nav *nav = new Nav( parent_ctx, parent_wid, name, ldhses, root_name,
	w, status);
  nav->get_plant_select_cb = get_plant_select_cb;
  return (void *) nav;
}

void plant_del( void *plantctx)
{
  delete (Nav *) plantctx;
}

int plant_get_select( void *plantctx, pwr_sAttrRef *attrref, int *is_attr)
{
  return ((Nav *)plantctx)->get_select( attrref, is_attr);
}

#endif


