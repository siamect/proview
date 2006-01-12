/* 
 * Proview   $Id: rs_showqbus.c,v 1.1 2006-01-12 05:54:27 claes Exp $
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


#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <sys/file.h>
#include <time.h>
#include <smem.h> 

#include "qbus_io.h"

#define C0 0x400000
#define C1 0x800000


typedef struct {
        unsigned int    CtrlStatus;
        unsigned int    reserved1;
        unsigned int    BusData;
        unsigned int    BusVector;
        unsigned int    BusPriority;
        unsigned int    QbusMaintenance;
        unsigned int    QbusAddress;
        unsigned int    Offset;
        unsigned int    reserved2;
        unsigned int    TestPoint;
        } t_QbusReg;

int main ()
{
  int	fp;
//  qbus_io_write wb;
  qbus_io_read rb;
  int	sts;
  unsigned int start_address = 017760000;
  unsigned int stop_address = 017777776;
  unsigned int address;

  fp = open( "/dev/qbus", O_RDWR);

  address = start_address;
  for ( ;;)
  {

    /* Read */
    rb.Address = address;
    sts = read( fp, &rb, sizeof(rb)); 
    if ( sts != -1)
      printf( "Address: %o (%d) sts: %d read\n", address, address, sts);
//    else
//    {
//      /* Write */
//      wb.Data = 0;
//      wb.Address = address;
//      sts = write( fp, &wb, sizeof(wb)); 
//      if ( sts != -1)
//        printf( "Address: %o (%d) sts: %d write\n", address, address, sts);
//    }

    address += 2;
    if ( address > stop_address)
      break;
  }

  return 1;
}
