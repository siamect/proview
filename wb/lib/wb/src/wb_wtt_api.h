/* api.h */

int wnav_foe_new(
	void	*parent_ctx,
	Widget	parent_wid,
	char	*name,
	pwr_tObjDId	plcprogram,
	ldh_tWBContext 	ldhwbctx,
	ldh_tSesContext	ldhsesctx,
	void		**return_foectx,
	int		map_window,
	ldh_eAccess	access,
	pwr_tOid	oid);
