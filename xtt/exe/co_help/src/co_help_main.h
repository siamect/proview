/* 
 * Proview   $Id: co_help_main.h,v 1.1 2008-10-28 09:36:12 claes Exp $
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

#ifndef co_help_main_h
#define co_help_main_h

/* co_help_main.h -- Help window */


#ifndef pwr_h
# include "pwr.h"
#endif

#ifndef flow_h
#include "flow.h"
#endif

#ifndef flow_browctx_h
#include "flow_browapi.h"
#endif

#ifndef co_nav_help_h
#include "co_nav_help.h"
#endif

class CoHelpMain {
  public:
    CoHelpMain( int argc, char *argv[], int *return_sts);
    virtual ~CoHelpMain() {}
    void exec_help();

    char topic[80];
    pwr_tFileName helpfile;
    char bookmark[80];
    int strict;
    navh_eHelpFile type;
};

#endif




















