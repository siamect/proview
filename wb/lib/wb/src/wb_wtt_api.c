/* api.cpp -- api for cpp

   PROVIEW/R  
   Copyright (C) 1996 by Comator Process AB.

   <Description>.  */

#include "wb_foe.h"
#include "wb_wtt_api.h"


int wnav_foe_new(
	void	*parent_ctx,
	Widget	parent_wid,
	char	*name,
	pwr_tObjDId	plcprogram,
	ldh_tWBContext 	ldhwbctx,
	ldh_tSesContext	ldhsesctx,
	void		**return_foectx,
	int		map_window,
	ldh_eAccess	access)
{
  return foe_new( 
	parent_ctx,
	parent_wid,
	name,
	plcprogram,
	ldhwbctx,
	ldhsesctx,
	(foe_ctx *)return_foectx,
	map_window,
	access);
}

