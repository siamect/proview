/* 
 * Proview   $Id: wb_cmdc.h,v 1.2 2008-10-15 06:06:14 claes Exp $
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
 */

#ifndef wb_cmdc_h
#define wb_cmdc_h

#ifndef pwr_h
# include "pwr.h"
#endif

#ifndef wb_h
# include "wb.h"
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
    
    wb_eUtility		ctx_type;
    ldh_tSesContext 	ldhses;
    ldh_tWBContext 	wbctx;
    ldh_tVolContext 	volctx;
    pwr_tVolumeId	volid;
    WNav 		*wnav;
    int			wb_type;
    dcli_sChannel 	chn;
    dcli_sRecall 	*recall_buf;
    static char cmd_volume[80];
    static char *cmd_volume_p;
    static unsigned int cmd_options;

    int detach_volume();
    static void usage();
    static int get_wbctx( void *ctx, ldh_tWBContext *wbctx);
    static int attach_volume_cb( void *ctx,
				 pwr_tVolumeId	volid,
				 int pop);
    static int detach_volume_cb( void *ctx);
    static void save_cb( void *ctx, int quiet);
    static void revert_ok( Cmd *cmd);
    static void revert_cb( void *ctx, int confirm);
    static void close_cb( void *ctx);
  
};

#endif
