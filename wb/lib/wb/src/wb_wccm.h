/* wb_wccm_fcn.h -- Buildtin script functions for wb

   PROVIEW/R
   Copyright (C) 1996 by Comator Process AB.

*/

void wccm_store_ldhses( void *client_data, ldh_tSesContext ldhsession);
void wccm_store_client( void *client_data);
int wccm_get_ldhses( ldh_tSesContext *ldhses);
int	wccm_register( 
	int (* get_wbctx_cb)( void *, ldh_tWBContext *),
	int (* get_ldhsession_cb)( void *, ldh_tSesContext *));
void	wccm_set_status( pwr_tStatus	sts);
