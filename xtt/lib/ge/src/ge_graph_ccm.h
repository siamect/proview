/* 
 * Proview   $Id: ge_graph_ccm.h,v 1.2 2005-09-01 14:57:53 claes Exp $
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

#ifndef ge_graph_ccm_h
#define ge_graph_ccm_h

/* graph_gccm.h -- script interpreter */

#define CCM_DECL_STRING	1
#define CCM_DECL_INT	2
#define CCM_DECL_FLOAT	3
#define CCM_DECL_UNKNOWN 4


typedef struct s_garg_ {
	char			value_name[80];
	int			value_decl;
	int			value_int;
	float			value_float;
	char			value_string[80];
	int			value_type;
	int			var_decl;
	char			var_name[32];
	int			value_returned;
	struct s_garg_		*next;
} gccm_s_arg;

typedef struct s_intvar_ {
	char			name[32];
	int			*value;
	int			elements;
	int			array;
	struct s_intvar_	*next;
} gccm_s_intvar;

typedef struct s_floatvar_ {
	char			name[32];
	float			*value;
	int			elements;
	int			array;
	struct s_floatvar_	*next;
} gccm_s_floatvar;

typedef struct s_stringvar_ {
	char			name[32];
	char			*value;
	int			elements;
	int			array;
	struct s_stringvar_	*next;
} gccm_s_stringvar;

typedef struct {
	gccm_s_intvar	*int_list;
	gccm_s_floatvar	*float_list;
	gccm_s_stringvar *string_list;
} gccm_s_arglist;	


int gccm_register_function( 
	char 	*name,
	int 	(* sysfunc) ( void *, gccm_s_arg *, int, int *, float *,
			  int *, char *)
);

int gccm_file_exec(
  char	*buff,
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
  void	*client_data,
  gccm_s_arglist *pwr_arglist
);

int gccm_create_external_var(
  char			*name,
  int			decl,
  float			value_float,
  int			value_int,
  char			*value_string
);

int gccm_set_external_var(
  char			*name,
  int			decl,
  float			value_float,
  int			value_int,
  char			*value_string
);

int	gccm_get_pwr_argnames(
		char	*inbuff,
		int	(* errormessage_func) ( char *, int, void *),
		char	*pwr_argnames,
		int	*pwr_argtypes,
		int	*pwr_argcnt);

int	gccm_create_pwr_args(
		char	*pwr_argnames,
		int	*pwr_argtypes,
		char	**pwr_argvalues,
		int	pwr_argcnt,
		int	(* errormessage_func) ( char *, int, void *),
		gccm_s_arglist	*arglist,
		gccm_s_arglist	*parent_arglist);

void	gccm_free_pwr_args(
		gccm_s_arglist	*arglist);
#endif
