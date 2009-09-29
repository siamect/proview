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

#ifndef pn_viewer_h
#define pn_viewer_h

/* pn_viewer.h -- Profinet viewer */

#include <vector>

#ifndef pwr_h
# include "pwr.h"
#endif

#ifndef pn_viewernav_h
# include "pn_viewernav.h"
#endif

class CoWow;
class CoWowFocusTimer;
class PnViewerPNAC;

class PnViewer {
  public:
    PnViewer(
	void *v_parent_ctx,
	const char *v_name,
	pwr_tStatus *status);
    virtual ~PnViewer();

    virtual void message( char severity, const char *msg) {}
    virtual void set_prompt( const char *prompt) {}
    virtual void open_change_value() {}

    void update_devices();
    void set_device_properties( unsigned char *macaddress, unsigned char *ipaddress,
				char *devname);
    void activate_update();
    void activate_setdevice();
    void activate_changevalue();
    void activate_close();
    void activate_help();

    static void change_value( void *ctx);
    static void message_cb( void *ctx, int severity, const char *msg);

    void 		*parent_ctx;
    char 		name[80];
    PnViewerNav		*viewernav;
    CoWow		*wow;
    int			input_open;
    vector<PnDevice>	dev_vect;
    PnViewerPNAC	*pnet;
    void		(*close_cb)( void *);

};

#endif









