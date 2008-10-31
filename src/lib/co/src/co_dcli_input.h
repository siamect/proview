/** 
 * Proview   $Id: co_dcli_input.h,v 1.4 2008-10-31 12:51:30 claes Exp $
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

#ifndef co_dcli_input_h
#define co_dcli_input_h

#ifdef __cplusplus
extern "C" {
#endif

/* co_dcli_input.h 
   Command line input. */

#define DCLI_OPT_NORECALL	1
#define DCLI_OPT_NOEDIT		2
#define DCLI_OPT_NOECHO		4
#define DCLI_OPT_NOPFTAN	8
#define DCLI_OPT_TIMEOUT	16
#define DCLI_OPT_NOSCROLL	32

#define DCLI_RECALL_MAX		30

typedef struct {
	int	first_command;
	int	last_command;
	char	command[DCLI_RECALL_MAX][200];
	} dcli_sRecall;

#if defined OS_ELN
typedef unsigned long dcli_sChannel[4];
#else
typedef int dcli_sChannel;
#endif

int	dcli_input_init( dcli_sChannel *chn, dcli_sRecall **recall_buf);
int	dcli_input_end( dcli_sChannel *chn, dcli_sRecall *recall_buf);
int	dcli_get_input_command( dcli_sChannel *chn, const char *prompt,char *cmd, 
		int maxlen, dcli_sRecall *recall_buf);
int	dcli_get_input_string( 	dcli_sChannel	*chn,
				char		*out_string,
				unsigned long	*out_terminator,
				int		out_maxlen,
				dcli_sRecall 	*recall,
				unsigned long	option,
				int		timeout,
				int		(* timeout_func) (),
				void		*timeout_arg,
				const char     	*prompt);

int dcli_qio_assign( char *s, dcli_sChannel *chn);
int dcli_qio_set_attr( dcli_sChannel *chn);
int dcli_qio_reset( dcli_sChannel *chn);
int dcli_qio_readw( dcli_sChannel *chn, char *buf, int len);
int dcli_qio_read( dcli_sChannel *chn, int tmo, char *buf, int len);
int dcli_qio_writew( dcli_sChannel *chn, char *buf, int len);
int dcli_qio_write( dcli_sChannel *chn, int tmo, char *buf, int len);


#ifdef __cplusplus
}
#endif

#endif
