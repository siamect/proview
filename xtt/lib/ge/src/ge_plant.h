#ifndef ge_plant_h
#define ge_plant_h

/* wb_nav.h -- Simple navigator

   PROVIEW/R
   Copyright (C) 1996 by Comator Process AB.

   <Description>.  */

#if defined __cplusplus
extern "C" {
#endif

/*! \file ge_plant.h
    \brief API to the Nav class which displayes the plant hierarchy. */
/*! \addtogroup Ge */
/*@{*/

void *plant_new(
	void *parent_ctx,
	Widget	parent_wid,
	char *name,
	ldh_tSesContext ldhses,
	char *root_name,
	Widget *w,
	pwr_tStatus *status,
        int (*get_plant_select_cb)( void *, char *)
	);
void plant_del( void *plantctx);
int plant_get_select( void *plantctx, pwr_sAttrRef *attrref, int *is_attr);

/*@}*/
#if defined __cplusplus
}
#endif
#endif
