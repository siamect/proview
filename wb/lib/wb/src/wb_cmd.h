#ifndef wb_cmd_h
#define wb_cmd_h

/* wb_cmd.h -- command file processing

   PROVIEW/R
   Copyright (C) 1996 by Comator Process AB.

   <Description>.  */


int	cmd_readcmdfile( char		*incommand,
			 show_ctx 	showctx);
int	cmd_remove_blank( char *out_str, char *in_str);
void	cmd_set_status( pwr_tStatus	sts);
void 	cmd_msg( pwr_tStatus sts);
#endif
