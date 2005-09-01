/** 
 * Proview   $Id: wb_cmd.h,v 1.2 2005-09-01 14:57:57 claes Exp $
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

#ifndef wb_cmd_h
#define wb_cmd_h

/* wb_cmd.h -- command file processing */

int	cmd_readcmdfile( char		*incommand,
			 show_ctx 	showctx);
int	cmd_remove_blank( char *out_str, char *in_str);
void	cmd_set_status( pwr_tStatus	sts);
void 	cmd_msg( pwr_tStatus sts);
#endif
