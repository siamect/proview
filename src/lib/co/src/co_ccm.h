#ifndef co_ccm_h
#define co_ccm_h

/* co_ccm.h -- script interpreter

   PROVIEW/R
   Copyright (C) 1996 by Comator Process AB.

   <Description>.  */


#define CCM_DECL_STRING	1
#define CCM_DECL_INT	2
#define CCM_DECL_FLOAT	3
#define CCM_DECL_UNKNOWN 4


typedef struct s_arg_ {
	char			value_name[80];
	int			value_decl;
	int			value_int;
	float			value_float;
	char			value_string[80];
	int			value_type;
	int			var_decl;
	char			var_name[32];
	int			value_returned;
	struct s_arg_		*next;
} ccm_s_arg;



int ccm_register_function( 
	char 	*name,
	int 	(* sysfunc) ( void *, ccm_s_arg *, int, int *, float *,
			  int *, char *)
);

int ccm_file_exec(
  char	*cmd,
  int	(* externcmd_func) ( char *, void *),
  int	(* deffilename_func) ( char *, char *, void *),
  int	(* errormessage_func) ( char *, int, void *),
  int	*appl_sts,
  int	verify,
  int	break_before,
  void  **ctx,
  int	extfunc_return_mode,
  int	resume,
  char	*extfunc_line,
  void	*client_data
);

int ccm_create_external_var(
  char			*name,
  int			decl,
  float			value_float,
  int			value_int,
  char			*value_string
);

int ccm_set_external_var(
  char			*name,
  int			decl,
  float			value_float,
  int			value_int,
  char			*value_string
);

#endif
