#ifndef co_dcli_input_h
#define co_dcli_input_h

#ifdef __cplusplus
extern "C" {
#endif

/* co_dcli_input.h -- <short description>

   PROVIEW/R
   Copyright (C) 1996-98 by Mandator AB.

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
int	dcli_get_input_command( dcli_sChannel *chn, char *prompt, char *cmd, 
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
				char		*prompt);

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
