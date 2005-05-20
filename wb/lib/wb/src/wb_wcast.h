#ifndef wb_wcast_h
#define wb_wcast_h

/* wb_wcast.h -- Attribute object casting window.

   PROVIEW/R
   Copyright (C) 1996 by Comator Process AB.

   <Description>.  */

#include <vector>

#if defined __cplusplus
extern "C" {
#endif

#ifndef pwr_h
# include "pwr.h"
#endif


#ifndef wb_ldh
# include "wb_ldh.h"
#endif

#ifndef co_wow_h
#include "co_wow.h"
#endif

using namespace std;


class WCast {
  public:
    WCast( 
	void	*wc_parent_ctx,
	Widget 	wc_parent_wid,
	char 	*wc_name,
	ldh_tSesContext wc_ldhses,
	pwr_sAttrRef wc_aref,
	pwr_tStatus *status);

    void 	*parent_ctx;
    Widget	parent_wid;
    char 	name[80];
    ldh_tSesContext ldhses;
    pwr_sAttrRef aref;
    vector<pwr_tCid> cidlist;

    pwr_tStatus open_castlist();
    void get_subcid( pwr_tCid cid);

    ~WCast();

    static void selected_cb( void *ctx, char *text);
};


#if defined __cplusplus
}
#endif
#endif



