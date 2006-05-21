/* 
 * Proview   $Id: ge.h,v 1.4 2006-05-21 22:30:50 lw Exp $
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

#ifndef ge_h
#define ge_h

#ifndef wb_ldh_h
#include "wb_ldh.h"
#endif

/* ge.h -- Simple graphic editor */

#if defined __cplusplus
extern "C" {
#endif


void *ge_new( 	void 	*parent_ctx, 
		Widget 	parent_widget,
		ldh_tSesContext	ldhses,
		int 	exit_when_close,
		char	*graph_name);

void ge_del( void *ge_ctx);
void ge_open( void *ge_ctx, char *name);
void ge_save( void *ge_ctx, char *name);
void ge_clear( void *ge_ctx);
int ge_generate_web( ldh_tSesContext ldhses);
int ge_command( void *ge_ctx, char *cmd);
void ge_message( void *ge_ctx, pwr_tStatus sts);

#if defined __cplusplus
}
#endif
#endif
