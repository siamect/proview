#ifndef wb_cmdc_h
#define wb_cmdc_h

/* wb_cmdc.h -- 

   PROVIEW/R
   Copyright (C) 1996 by Comator Process AB.

   <Description>.  */

#if defined __cplusplus
extern "C" {
#endif

#ifndef pwr_h
# include "pwr.h"
#endif

#ifndef wb_ldh_h
# include "wb_ldh.h"
#endif

#ifndef wb_wnav_h
# include "wb_wnav.h"
#endif

#ifndef co_dcli_input_h
# include "co_dcli_input.h"
#endif

class Cmd {
  public:
    Cmd();
    
    ldh_tSesContext 	ldhses;
    ldh_tWBContext 	wbctx;
    ldh_tVolContext 	volctx;
    pwr_tVolumeId	volid;
    WNav 		*wnav;
    int			wb_type;
    dcli_sChannel 	chn;
    dcli_sRecall 	*recall_buf;


    int detach_volume();

};

#if defined __cplusplus
}
#endif
#endif
