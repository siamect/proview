/* 
 * Proview   $Id: ge_plant.h,v 1.3 2005-09-01 14:57:53 claes Exp $
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

#ifndef ge_plant_h
#define ge_plant_h

/* ge_plant.h -- Simple navigator */

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
