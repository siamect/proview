/* 
 * Proview   $Id: wb_cmdc.h,v 1.2 2005-09-01 14:57:48 claes Exp $
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
