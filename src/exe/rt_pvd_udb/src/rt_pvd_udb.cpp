/* 
 * Proview   $Id: rt_pvd_udb.cpp,v 1.1 2006-09-14 14:16:07 claes Exp $
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

#include "pwr.h"
#include "pwr_class.h"
#include "rt_procom.h"
#include "co_provider.h"
#include "rt_gdh_msg.h"
#include "co_pvd_udb.h"
#include "wb_ldh.h"

int main()
{
  co_pvd_udb provider( pvd_eEnv_Rt);
  rt_procom procom( &provider,
		    errh_eAnix_appl20, 	// Application index
		    "rt_pvd_udb",     	// Process name
		    200,	       	// Sid
		    ldh_cUserDatabaseVolume, // Vid _V254.254.254.246
		    "VolUserDatabase",  // Volume name
		    0);			// Global
  

  procom.init();
  procom.mainLoop();
}
