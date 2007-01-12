/* 
 * Proview   $Id: rt_io_profiboard.c,v 1.2 2007-01-12 13:28:32 claes Exp $
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

#pragma pack(1)

#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <sys/file.h>
#include <sys/ioctl.h>
#include <unistd.h>

#include "keywords.h"

#include "pb_type.h"
#include "pb_conf.h"
#include "pb_if.h"
#include "pb_err.h"
#include "pb_fmb.h"
#include "pb_dp.h"

#include "pwr.h"
#include "rt_io_profiboard.h"

unsigned short swap16(unsigned short in) 
{
  unsigned short result = 0;
  result = (in << 8) & 0xFF00;
  result |= (in >> 8) & 0x00FF;
  return(result);
}

unsigned int swap32(unsigned int in) 
{
  unsigned int result = 0;;
  result  = (in << 24) & 0xFF000000;
  result |= (in <<  8) & 0x00FF0000;
  result |= (in >>  8) & 0x0000FF00;
  result |= (in >> 24) & 0x000000FF;
  return(result);
}

