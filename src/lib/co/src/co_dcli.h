/** 
 * Proview   $Id: co_dcli.h,v 1.12 2008-06-24 07:02:56 claes Exp $
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

#ifndef co_dcli_h
#define co_dcli_h

#include <stdio.h>

#include "pwr.h"

#ifdef __cplusplus
extern "C" {
#endif

/* co_dcli.c
   Command line interpreter. */


#define DCLI_CMD_SIZE 400
#define DCLI_QUAL_SIZE 400
#define DCLI_SYM_KEY_SIZE 200
#define DCLI_SYM_VALUE_SIZE 200


typedef	struct	{
	char	command[20];
	int	(*func) ( void *, void *);
	char	qualifier[60][40];
	} dcli_tCmdTable;


int	dcli_parse( 	char	*string,
			char	*parse_char,
			char	*inc_parse_char,
			char	*outstr,
			int	max_rows,
			int 	max_cols,
			int	keep_quota);
int	dcli_cli( 	dcli_tCmdTable	*command_table,
			char		*string,
			void		*userdata1,
			void		*userdata2);
int	dcli_get_qualifier( 	char	*qualifier,
				char	*value,
				size_t  size);
int	dcli_store_symbols( char	*filename);
int	dcli_replace_symbol( char *command, char *newcommand, int newsize);
int	dcli_get_symbol( char *key, char *value);
int	dcli_get_symbol_by_index( int index, char *key, char *value);
int	dcli_get_symbol_cmd( char *key, char *value);
int	dcli_define_symbol( char *key, char *arg1, char *arg2, char *arg3);
int 	dcli_toupper( 	char	*str_upper,
				char	*str);
int	dcli_trim( char *out_str, char *in_str);
int	dcli_remove_blank( char *out_str, char *in_str);
char	*dcli_pwr_dir( char *dir);
int 	dcli_wildcard(	char	*wildname,
			char	*name);
int dcli_read_line( char *line, int maxsize, FILE *file);


/* Functions in module co_dcli_file */

void 	dcli_set_default_directory( char *dir);
int	dcli_get_defaultfilename(
			char	*inname,
			char	*outname,
			char	*ext);
#if defined (OS_LYNX) || defined(OS_LINUX)
int     dcli_replace_env( char *str, char *newstr);
#endif
char	*dcli_fgetname( FILE *fp, char *name, char *def_name);
int	dcli_translate_filename( char *out, const char *in);
pwr_tStatus dcli_file_time( char *filename, pwr_tTime *time);
pwr_tStatus dcli_file_ctime( char *filename, pwr_tTime *time);


/* Functions in module co_dcli_dir */

#define DCLI_DIR_SEARCH_NEXT 0
#define DCLI_DIR_SEARCH_INIT 1
#define DCLI_DIR_SEARCH_END 2


int dcli_search_file( 		char 	*file_name , 
				char	*found_file,
				int	new_search);

int dcli_get_files( 		char *dir,
		   		char *pattern, 
				pwr_tString40 *filelist[], 
				int *filecnt);

int dcli_parse_filename( 	char	*filename,
				char	*dev,
				char	*dir,
				char	*file,
				char	*type,
				int	*version);

/* Functions i module co_dcli_struct */

typedef struct s_element {
	int		type;
	int		size;
	int		elements;
	int		undefined;
	char		typestr[40];
	char		struct_begin;
	char		name[40];
	unsigned int	mask;
        char            filename[120];
        int             line_nr;
	struct s_element *next;
	struct s_element *prev;
	} dcli_sStructElement;

int	dcli_readstruct_find( 	char *filename,
				char *struct_name,
				dcli_sStructElement **e_list);

void	dcli_readstruct_free(	dcli_sStructElement *e_list);

int dcli_readstruct_get_message( char **message);


#ifdef __cplusplus
}
#endif

#endif
