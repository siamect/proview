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

/* pn_viewer.cpp -- Profinet viewer */

#include "glow_std.h"

#include <stdio.h>
#include <stdlib.h>

#include "co_cdh.h"
#include "co_time.h"
#include "co_dcli.h"

#include "co_error.h"
#include "co_lng.h"
#include "co_xhelp.h"
#include "pn_viewer_pnac.h"
#include "pn_viewer.h"


PnViewer::PnViewer(
	void *v_parent_ctx,
	const char *v_name,
	pwr_tStatus *status) :
  parent_ctx(v_parent_ctx), viewernav(NULL), wow(0), input_open(0), pnet(0), close_cb(0)
{
  strcpy( name, v_name);

  pnet = new PnViewerPNAC( status);
}

PnViewer::~PnViewer()
{
  delete pnet;
}

void PnViewer::update_devices()
{
  dev_vect.clear();
  pnet->fetch_devices( dev_vect);

  viewernav->set( dev_vect);    
}

void PnViewer::set_device_properties( unsigned char *macaddress, unsigned char *ipaddress, 
				      char *devname)
{
  pnet->set_device_properties( macaddress, ipaddress, devname);
}

void PnViewer::change_value( void *ctx)
{
  PnViewer *viewer = (PnViewer *) ctx;

  viewer->open_change_value();
}

void PnViewer::message_cb( void *ctx, int severity, const char *msg)
{
  PnViewer *viewer = (PnViewer *) ctx;

  viewer->message( severity, msg);
}

void PnViewer::activate_update()
{
  try {
    update_devices();
  }
  catch( co_error &e) {
    printf( "** Update exception: %s\n", e.what().c_str());
  }
}
 
void PnViewer::activate_setdevice()
{
  pwr_tStatus sts;
  ItemDevice *dev;

  sts = viewernav->get_selected_device( &dev);
  if ( EVEN(sts)) {
    message( 'E', "Select a device");
    return;
  }

  try {
    set_device_properties( dev->macaddress, dev->ipaddress, 
			   dev->devname);  
  }
  catch ( co_error &e) {
    printf( "** Exception, %s\n", e.what().c_str());
  }
}

void PnViewer::activate_changevalue()
{
  open_change_value();
}

void PnViewer::activate_close()
{
  if ( close_cb)
    close_cb( parent_ctx);
  else {
    delete this;
    exit(0);
  }
}

void PnViewer::activate_help()
{
  CoXHelp::dhelp( "profinet_viewer", 0, navh_eHelpFile_Other, "$pwr_lang/man_pb.dat", 
		  true);
}




