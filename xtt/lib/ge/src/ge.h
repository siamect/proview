#ifndef ge_h
#define ge_h

/* ge.h -- Simple graphic editor

   PROVIEW/R
   Copyright (C) 1996 by Comator Process AB.

   <Description>.  */

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
