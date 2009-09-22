/** 
 * Proview   $Id$
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

#ifndef co_logw_h
#define co_logw_h

/* co_glow.h -- History logfile window */

#ifndef pwr_h
# include "pwr.h"
#endif

#ifndef co_logwnav_h
# include "co_logwnav.h"
#endif

class CoWow;
class CoWowFocusTimer;

class CoLogW {
  public:
    CoLogW(
	void *msg_parent_ctx,
	const char *logw_name,
	pwr_tStatus *status);
    virtual ~CoLogW() {}

    void show( char categories[][20], char *item);

    void 		*parent_ctx;
    char 		name[80];
    CoLogWNav		*logwnav;
    int		        displayed;
    int			size;
    int			max_size;
    CoWow		*wow;

};

#endif









