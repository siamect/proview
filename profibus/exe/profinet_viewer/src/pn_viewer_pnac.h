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

#ifndef pn_viewer_pnac_h
#define pn_viewer_pnac_h

/* pn_viewer_pnac.h -- Profinet viewer PNAC interface */


#ifndef pwr_h
# include "pwr.h"
#endif

#ifndef pn_viewernav_h
# include "pn_viewernav.h"
#endif

# include "rt_io_base.h"

#ifndef rt_io_pn_locals_h
# include "rt_io_pn_locals.h"
#endif

#ifndef rt_pn_gsdml_data_h
# include "rt_pn_gsdml_data.h"
#endif




class PnViewerPNAC {
 public:
  PnViewerPNAC( pwr_tStatus *sts);
  ~PnViewerPNAC();

  void fetch_devices( vector<PnDevice>& dev_vect);
  void set_device_properties( unsigned char *macaddress, unsigned char *ipaddress,
			      char *devname);
  GsdmlDeviceData  dev_data;
  io_sAgentLocal  *local; 

};


#endif
