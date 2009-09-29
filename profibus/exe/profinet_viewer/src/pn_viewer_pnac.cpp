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

/* pn_viewer_pnac.cpp -- Profinet viewer PNAC interface */



#include "co_error.h"
#include "pn_viewer_pnac.h"

PnViewerPNAC::PnViewerPNAC( pwr_tStatus *sts)
{
  // Init PNAC
  *sts = 1;
}


PnViewerPNAC::~PnViewerPNAC()
{
  // Close PNAC
}

void PnViewerPNAC::fetch_devices( vector<PnDevice>& dev_vect)
{
  PnDevice d1;
  strcpy( d1.devname, "D1");
  d1.ipaddress[3] = 168;
  d1.ipaddress[2] = 192;
  d1.ipaddress[1] = 101;
  d1.ipaddress[0] = 56;
  d1.macaddress[0] = 255;
  d1.macaddress[1] = 255;
  d1.macaddress[2] = 255;
  d1.macaddress[3] = 36;
  d1.macaddress[4] = 9;
  d1.macaddress[5] = 22;
  d1.vendorid = 42;
  d1.deviceid = 769;
  dev_vect.push_back(d1);

  PnDevice d2;
  strcpy( d2.devname, "D2");
  d2.ipaddress[3] = 168;
  d2.ipaddress[2] = 192;
  d2.ipaddress[1] = 101;
  d2.ipaddress[0] = 57;
  d2.macaddress[0] = 255;
  d2.macaddress[1] = 255;
  d2.macaddress[2] = 255;
  d2.macaddress[3] = 36;
  d2.macaddress[4] = 9;
  d2.macaddress[5] = 23;
  d2.vendorid = 42;
  d2.deviceid = 769;
  dev_vect.push_back(d2);

  PnDevice d3;
  strcpy( d3.devname, "D3");
  d3.ipaddress[3] = 168;
  d3.ipaddress[2] = 192;
  d3.ipaddress[1] = 101;
  d3.ipaddress[0] = 58;
  d3.macaddress[0] = 255;
  d3.macaddress[1] = 255;
  d3.macaddress[2] = 255;
  d3.macaddress[3] = 36;
  d3.macaddress[4] = 9;
  d3.macaddress[5] = 24;
  d3.vendorid = 42;
  d3.deviceid = 769;
  dev_vect.push_back(d3);

  PnDevice d4;
  strcpy( d4.devname, "D4");
  d4.ipaddress[3] = 168;
  d4.ipaddress[2] = 192;
  d4.ipaddress[1] = 101;
  d4.ipaddress[0] = 59;
  d4.macaddress[0] = 255;
  d4.macaddress[1] = 255;
  d4.macaddress[2] = 255;
  d4.macaddress[3] = 36;
  d4.macaddress[4] = 9;
  d4.macaddress[5] = 25;
  d4.vendorid = 42;
  d4.deviceid = 769;
  dev_vect.push_back(d4);

  PnDevice d5;
  strcpy( d5.devname, "D5");
  d5.ipaddress[3] = 168;
  d5.ipaddress[2] = 192;
  d5.ipaddress[1] = 101;
  d5.ipaddress[0] = 60;
  d5.macaddress[0] = 255;
  d5.macaddress[1] = 255;
  d5.macaddress[2] = 255;
  d5.macaddress[3] = 36;
  d5.macaddress[4] = 9;
  d5.macaddress[5] = 26;
  d5.vendorid = 266;
  d5.deviceid = 1;
  dev_vect.push_back(d5);

  PnDevice d6;
  strcpy( d6.devname, "D6");
  d6.ipaddress[3] = 168;
  d6.ipaddress[2] = 192;
  d6.ipaddress[1] = 100;
  d6.ipaddress[0] = 55;
  d6.macaddress[0] = 200;
  d6.macaddress[1] = 1;
  d6.macaddress[2] = 0;
  d6.macaddress[3] = 201;
  d6.macaddress[4] = 202;
  d6.macaddress[5] = 203;
  d6.vendorid = 285;
  d6.deviceid = 750;
  dev_vect.push_back(d6);


  // if ( ...error... ) throw co_error_str( "Somethings is wrong...");
}
 
void PnViewerPNAC::set_device_properties( unsigned char *macaddress, unsigned char *ipaddress,
					  char *devname)
{
  printf( "Set %s %hhu.%hhu.%hhu.%hhu\n", devname, ipaddress[3],
	  ipaddress[2], ipaddress[1], ipaddress[0]);
}
