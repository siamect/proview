/* rt_io_ssab.c -- init, read and write ssab cards.

   PROVIEW/R
   Copyright (C) 1994 by Comator Process AB.  */

#include "pwr.h"
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
  return 1;
}

unsigned short ssabai_read(	
		void	*reg_diP,
		int	grupp)
{
  return 1;
}

int ssabao_ini(	
		char	*inidev,
		void	*adressini,
		void	*hv_device,
		int	relvec)
{
  return 1;
}
void ssabao_write(	
		short	data,
		void	*reg_diP,
		int	channel)
{
}

int ssabd_ini(	
		char	*inidev,
		void	*adressini,
		void	*hv_device,
		int	relvec)
{
  return 1;
}

unsigned short ssabdi_read(	
		void	*reg_diP,
		int	grupp)
{
  return 1;
}

void ssabdo_write(	
		short	data,
		void	*reg_diP,
		int	grupp)
{
}

void ssabco_read(	
		void	*reg_diP,
		int	numofword,
		int	*co_id,
		unsigned short	*data)
{

}

void ssabco_write(
		unsigned short	*data,
		void	*reg_P)
{

}

unsigned short ssabaiup_read(	
		void	*reg_diP,
		int	grupp)
{
  return 1;
}
