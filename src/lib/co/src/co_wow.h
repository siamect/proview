#ifndef co_wow_h
#define co_wow_h

/* co_wow -- useful windows

   PROVIEW/R
   Copyright (C) 1996 by Comator Process AB.

   <Description>.  */

typedef enum {
  wow_eFileSelType_All,
  wow_eFileSelType_Dbs,
  wow_eFileSelType_Wbl
} wow_eFileSelType;

typedef struct {
  Widget    dialog;
  void      (*file_selected_cb)( void *, char *, wow_eFileSelType);
  void      *parent_ctx;
  wow_eFileSelType file_type;
} *wow_tFileSelCtx;

typedef struct {
  Widget    toplevel;
  Widget    list;
  char      *texts;
  void      (* action_cb) ( void *, char *);
  void      *parent_ctx;
} *wow_tListCtx;

void wow_DisplayQuestion (
  void	    *ctx,
  Widget    father,
  char	    *title,
  char	    *text,
  void	    (* questionbox_ok) ( void *, void *),
  void	    (* questionbox_cancel) ( void *, void *),
  void	    *data
);

void wow_DisplayError (
  Widget    father,
  char	    *title,
  char	    *text
);

void wow_CreatePushButton (
  Widget    father,
  char	    *text,
  char	    *widget_name,
  void	    (callback)(),
  void	    *ctx
);

wow_tListCtx wow_CreateList (
  Widget    parent,
  char	    *title,
  char      *texts,
  void	    (action_cb)( void *, char *),
  void	    *ctx
);

void wow_CreateFileSelDia( Widget parent_wid,
			   char *title,
			   void *parent_ctx,
			   void (*file_selected_cb)(void *, char *, wow_eFileSelType),
			   wow_eFileSelType file_type);

#endif








