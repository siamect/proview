#ifndef co_dcli_h
#define co_dcli_h

/* co_dcli.c -- <short description>

   PROVIEW/R
   Copyright (C) 1996-98 by Mandator AB.

   Command line interpreter. */

typedef	struct	{
	char	command[20];
	int	(*func) ( void *, void *);
	char	qualifier[50][40];
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
				char	*value);
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
int	dcli_translate_filename( char *out, char *in);


/* Functions in module co_dcli_dir */

#define DCLI_DIR_SEARCH_NEXT 0
#define DCLI_DIR_SEARCH_INIT 1
#define DCLI_DIR_SEARCH_END 2


int dcli_search_file( 		char 	*file_name , 
				char	*found_file,
				int	new_search);

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


#endif

