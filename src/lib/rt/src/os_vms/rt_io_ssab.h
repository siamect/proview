/* rt_io_ssab.h -- init, read and write ssab cards.

   PROVIEW/R
   Copyright (C) 1994 by Comator Process AB.  */

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
