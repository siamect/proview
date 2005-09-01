/* 
 * Proview   $Id: rt_io_ssab.c,v 1.2 2005-09-01 14:57:57 claes Exp $
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

/* rt_io_ssab.c -- init, read and write ssab cards. */

# include <string.h>
# include <stdlib.h>
# include <stdio.h>

#include "pwr.h"
#include "rt_io_msg.h"
#include "co_cdh.h"
#include "rt_errh.h"
#include "rt_io_base.h"
#include "rt_io_ssab.h"


int ssab_close(	
		void	*hv_device)
{
  return 1;
}

int ssabai_ini(	
		char	*inidev,
		void	*adressini,
		void	*hv_device,
		int	relvec)
{
  pwr_tStatus sts;

  * (int *)adressini = 12345;
  sts = 1;
  return sts;
}

unsigned short ssabai_read(	
		void	*reg_diP,
		int	grupp)
{
  unsigned short di_data;

  di_data = 3020;
  return di_data;
}

unsigned short ssabaiup_read(	
		void	*reg_diP,
		int	grupp)
{
  unsigned short di_data;

  di_data = 3020;
  return di_data;
}

int ssabao_ini(	
		char	*inidev,
		void	*adressini,
		void	*hv_device,
		int	relvec)
{
  pwr_tStatus sts;

  * (int *)adressini = 12345;
  sts = 1;
  return sts;
}
void ssabao_write(	
		short	data,
		void	*reg_diP,
		int	channel)
{
  io_writeerr = 0;
  io_fatal_error = 0;
}

int ssabd_ini(	
		char	*inidev,
		void	*adressini,
		void	*hv_device,
		int	relvec)
{
  pwr_tStatus sts;

  * (int *)adressini = 12345;
  sts = 1;
  return sts;
}

unsigned short ssabdi_read(	
		void	*reg_diP,
		int	grupp)
{
  unsigned short di_data;

  di_data = 21845;
  return di_data;
}

void ssabdo_write(	
		short	data,
		void	*reg_diP,
		int	grupp)
{
  io_writeerr = 0;
  io_fatal_error = 0;
}

void ssabco_read(	
		void	*reg_diP,
		int	numofword,
		int	*co_id,
		unsigned short	*data,
		int	channel)
{
  unsigned short di_data;

  *data = 21845;
}
void ssabco_write(
		unsigned short	*data,
		void	*reg_P,
		int	channel)
{
  io_writeerr = 0;
  io_fatal_error = 0;
}

