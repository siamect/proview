/** 
 * Proview   $Id: co_ccm.h,v 1.6 2008-10-31 12:51:30 claes Exp $
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

#ifndef co_ccm_h
#define co_ccm_h

#ifdef __cplusplus
extern "C" {
#endif

/* co_ccm.h -- script interpreter */

#define CCM_DECL_STRING	1
#define CCM_DECL_INT	2
#define CCM_DECL_FLOAT	3
#define CCM_DECL_UNKNOWN 4

#define K_STRING_SIZE	400

typedef struct s_arg_ {
	char			value_name[80];
	int			value_decl;
	int			value_int;
	float			value_float;
	char			value_string[K_STRING_SIZE];
	int			value_type;
	int			var_decl;
	char			var_name[32];
	int			value_returned;
	struct s_arg_		*next;
} ccm_s_arg;



int ccm_register_function( 
	const char 	*name,
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
  const char	       	*name,
  int			decl,
  float			value_float,
  int			value_int,
  char			*value_string
);

int ccm_delete_external_var(
  const char	       	*name,
  float			value_float,
  int			value_int,
  char			*value_string
);

int ccm_set_external_var(
  const char   		*name,
  int			decl,
  float			value_float,
  int			value_int,
  char			*value_string
);

int ccm_get_external_var(
  const char	       	*name,
  int			decl,
  float			*value_float,
  int			*value_int,
  char			*value_string
);

int ccm_ref_external_var(
  const char	       	*name,
  int			decl,
  void 			**valuep
);

#ifdef __cplusplus
}
#endif

#endif
