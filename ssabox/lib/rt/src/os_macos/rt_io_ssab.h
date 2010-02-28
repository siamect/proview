/* 
 * Proview   $Id: rt_io_ssab.h,v 1.1 2005-12-30 15:52:05 claes Exp $
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

/* rt_io_ssab.h -- init, read and write ssab cards. */

typedef int DEVICE;


int ssab_close(	
		void	*hv_device);

int ssabai_ini(	
		char	*inidev,
		void	*adressini,
		void	*hv_device,
		int	relvec);

unsigned short ssabai_read(	
		void	*reg_diP,
		int	grupp);

int ssabao_ini(	
		char	*inidev,
		void	*adressini,
		void	*hv_device,
		int	relvec);
void ssabao_write(	
		short	data,
		void	*reg_diP,
		int	channel);

int ssabd_ini(	
		char	*inidev,
		void	*adressini,
		void	*hv_device,
		int	relvec);

unsigned short ssabdi_read(	
		void	*reg_diP,
		int	grupp);

void ssabdo_write(	
		short	data,
		void	*reg_diP,
		int	grupp);

void ssabco_read(	
		void	*reg_diP,
		int	numofword,
		int	*co_id,
		unsigned short	*data,
		int 	channel);

void ssabco_write(
		unsigned short	*data,
		void	*reg_P,
		int	channel);

unsigned short ssabaiup_read(	
		void	*reg_diP,
		int	grupp);
