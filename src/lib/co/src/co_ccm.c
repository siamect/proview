/** 
 * Proview   $Id: co_ccm.c,v 1.8 2008-10-31 12:51:30 claes Exp $
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


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <float.h>
#include <ctype.h>
#include <time.h>

#include "co_cdh.h"
#include "co_ccm.h"
#include "co_ccm_msg.h"
#include "co_time.h"
#include "co_dcli.h"
#include "co_cnf.h"
#include "co_syi.h"
#include "co_api.h"

#define r_toupper(c) (((c) >= 'a' && (c) <= 'z') ? (c) & 0xDF : (c))
#define r_tolower(c) (((c) >= 'A' && (c) <= 'Z') ? (c) | 0x20 : (c))


/* Nice functions */
#define ODD(a)	(((int)(a) & 1) != 0)
#define EVEN(a)	(((int)(a) & 1) == 0)
#define max(Dragon,Eagle) ((Dragon) > (Eagle) ? (Dragon) : (Eagle))
#define min(Dragon,Eagle) ((Dragon) < (Eagle) ? (Dragon) : (Eagle))
#ifndef __ALPHA
#define abs(Dragon) ((Dragon) >= 0 ? (Dragon) : (-(Dragon)))
#endif

#define K_LINE_SIZE	400

#define K_ACTION_NO	0
#define K_ACTION_DOWN	1
#define K_ACTION_UP	2
#define K_ACTION_MUL	3
#define K_ACTION_DIV	4
#define K_ACTION_ADD	5
#define K_ACTION_SUB	6
#define K_ACTION_EQL	7
#define K_ACTION_LT	8
#define K_ACTION_GT	9
#define K_ACTION_GE	10
#define K_ACTION_LE	11
#define K_ACTION_EQ	12
#define K_ACTION_NE	13
#define K_ACTION_INV	14
#define K_ACTION_INCR	15
#define K_ACTION_DECR	16
#define K_ACTION_AND	17
#define K_ACTION_OR	18
#define K_ACTION_BITAND	19
#define K_ACTION_BITOR	20
#define K_ACTION_VAR	21
#define K_ACTION_NUM	22
#define K_ACTION_FLOAT	23
#define K_ACTION_END	24
#define K_ACTION_EQLADD	25
#define K_ACTION_EQLSUB 26
#define K_ACTION_DELIM	27
#define K_ACTION_EQLMUL	28
#define K_ACTION_EQLDIV	29
#define K_ACTION_CREALOCINT 30
#define K_ACTION_CREALOCFLOAT 31
#define K_ACTION_CREALOCSTRING 32
#define K_ACTION_CREAGBLINT 33
#define K_ACTION_CREAGBLFLOAT 34
#define K_ACTION_CREAGBLSTRING 35
#define K_ACTION_CREAEXTINT 36
#define K_ACTION_CREAEXTFLOAT 37
#define K_ACTION_CREAEXTSTRING 38
#define K_ACTION_STRING	39
#define K_ACTION_LSHIFT	40
#define K_ACTION_RSHIFT	41
#define K_ACTION_LIST	42
#define K_ACTION_FUNCTION 43
#define K_ACTION_RETURN 44
#define K_ACTION_BACKSLASH 45
#define K_ACTION_DELETE 46

#define K_OPERAND_NO	0
#define K_OPERAND_NAME	1
#define K_OPERAND_VALUE	2
#define K_OPERAND_DECL	3
#define K_OPERAND_GLOBAL 4
#define K_OPERAND_EXTERN 5
#define K_OPERAND_FUNCTION 6
#define K_OPERAND_RETURN 7
#define K_OPERAND_DELETE 8

#define K_LOCTYPE_NO	0
#define K_LOCTYPE_BEFORE 1
#define K_LOCTYPE_AFTER	2

#define K_TYPE_OPERATOR	0
#define K_TYPE_OPERAND	1

#define K_DECL_STRING	1
#define K_DECL_INT	2
#define K_DECL_FLOAT	3
#define K_DECL_UNKNOWN	4

#define K_STATE_VAR		1
#define K_STATE_NUM		2
#define K_STATE_STRING		3
#define K_STATE_OPERATOR	4
#define K_STATE_DELIM		5
#define K_STATE_PAR		6
#define K_STATE_END		7
#define K_STATE_ERROR		8
#define K_STATE_LIST		9
#define K_STATE_NUM_NEG		10

#define K_LINE_STATMENT		0
#define K_LINE_FUNCTION		1
#define K_LINE_ENDFUNCTION	2
#define K_LINE_MAIN		3
#define K_LINE_ENDMAIN		4
#define K_LINE_IF		5
#define K_LINE_ELSE		6
#define K_LINE_ENDIF		7
#define K_LINE_FOR		8
#define K_LINE_ENDFOR		9
#define K_LINE_WHILE		10
#define K_LINE_ENDWHILE		11
#define K_LINE_BREAK		12
#define K_LINE_CONTINUE		13
#define K_LINE_LABEL		14
#define K_LINE_GOTO		15



typedef struct ccm_sLine_ {
	char			line[K_LINE_SIZE];
	int			row;
	int			type;
	struct ccm_sLine_		*next;
	struct ccm_sLine_		*prev;
} ccm_sLine;

typedef struct s_intvar_ {
	char			name[32];
	ccm_tInt       		*value;
	int			elements;
	int			array;
	struct s_intvar_	*next;
} ccm_sIntvar;

typedef struct s_floatvar_ {
	char			name[32];
	ccm_tFloat     		*value;
	int			elements;
	int			array;
	struct s_floatvar_	*next;
} ccm_sFloatvar;

typedef struct s_stringvar_ {
	char			name[32];
	char    		*value;
	int			elements;
	int			array;
	struct s_stringvar_	*next;
} ccm_sStringvar;

typedef struct ccm_sOperand_ {
	int			type;
	char			name[100];
	int			value_decl;
	ccm_tInt       		value_int;
	ccm_tFloat	       	value_float;
	ccm_tString    		value_string;
	int			result_decl;
	ccm_tInt       		result_int;
	ccm_tFloat     		result_float;
	ccm_tString	       	result_string;
	int			result_done;
	int			operator;
	int			local_type;
	int			local_operator;
	int			prio;
	int			parlevel;
	int			done;
	struct ccm_sOperand_	*next;
	struct ccm_sOperand_	*prev;
	} ccm_sOperand;

typedef struct ccm_sFunc_ {
	char			name[32];
	int			decl;
	ccm_sLine		*start_line;
	ccm_sLine		*end_line;
	struct ccm_sFunc_	*next;
	} ccm_sFunc;

typedef struct {
	ccm_sLine      	*main_start_line;
	ccm_sLine      	*main_end_line;
	int		verify;
	int		current_row;
	int		error_row;
	char		error_line[160];
	int		break_before;
	int		(* externcmd_func) ( char *, void *);
	int		(* deffilename_func) ( char *, char *, void *);
	int		(* errormessage_func) ( char *, int, void *);
	ccm_sFunc      	*func_list;
	ccm_sIntvar	*gblint_list;
	ccm_sFloatvar	*gblfloat_list;
	ccm_sStringvar	*gblstring_list;
	ccm_sLine      	*line_list;
	void		*main_funcctx;
	int		extfunc_return_mode;
	char		extfunc_line[256];
	char		last_fgets[1024];
	ccm_sArg	*main_arg_list;
	int		main_arg_count;
  	void		*client_data;
	} *ccm_tFileCtx;

typedef struct {
	char		line[256];
	ccm_tFileCtx	filectx;
	int		pos;
	int		delim_pos;
	int		state;
	int		level;
	int		num_decl;
	int		num_neg;
	char		msg[80];
	int		last_type;
	ccm_sOperand	*curr_operand;
	ccm_sOperand	*list;
	} *ccm_tRowCtx;

typedef struct {
	char		msg[80];
	ccm_tFileCtx	filectx;
	ccm_sIntvar	*locint_list;
	ccm_sFloatvar	*locfloat_list;
	ccm_sStringvar	*locstring_list;
	ccm_sOperand	*list;
	ccm_sLine      	*current_line;
	int		is_main;
	int		for_init;
	ccm_sArg       	*arg_list;
	int		arg_count;
	} *ccm_tFuncCtx;

typedef struct {
	char		name[32];
	int 		(* sysfunc) ( void *, ccm_sArg *, int, int *, ccm_tFloat *,
				      ccm_tInt *, char *);
	} ccm_sSysFunc;

static  int		ccm_testmode = 0;

ccm_sIntvar	*extint_list = 0;
ccm_sFloatvar	*extfloat_list = 0;
ccm_sStringvar	*extstring_list = 0;

static int ctable[256];
static int ptable[256];

static int ccm_element( char *element, int num, char *str, char delim);
static int ccm_getvar( 
  ccm_tFuncCtx		ctx,
  const char	       	*name,
  int			*decl,
  ccm_tFloat	       	*value_float,
  ccm_tInt     		*value_int,
  char  		*value_string);
static int ccm_setvar( 
  ccm_tFuncCtx	ctx,
  const char   	*name,
  int		decl,
  ccm_tFloat   	value_float,
  ccm_tInt     	value_int,
  char  	*value_string);
static int ccm_createvar(
  const char   		*name,
  int			decl,
  ccm_tFloat	       	value_float,
  ccm_tInt     		value_int,
  char  		*value_string,
  ccm_sIntvar		**int_list,
  ccm_sFloatvar		**float_list,
  ccm_sStringvar	**string_list
);
static int ccm_deletevar(
  const char	       	*name,
  ccm_sIntvar		**intlist,
  ccm_sFloatvar		**floatlist,
  ccm_sStringvar	**stringlist
);
static int ccm_function_exec( 
  ccm_tFileCtx		ctx,
  char		*name,
  ccm_sArg		*arg_list,
  int		arg_count,
  int		*return_decl,
  ccm_tFloat   	*return_float,
  ccm_tInt     	*return_int,
  char  	*return_string,
  int		resume);
static int ccm_func_printf( void *filectx, ccm_sArg *arg_list, int arg_count, int *return_decl,
  ccm_tFloat *return_float, ccm_tInt *return_int, char *return_string);
static int ccm_func_say( void *filectx, ccm_sArg *arg_list, int arg_count, int *return_decl,
  ccm_tFloat *return_float, ccm_tInt *return_int, char *return_string);
static int ccm_func_scanf( void *filectx, ccm_sArg *arg_list, int arg_count, int *return_decl,
  ccm_tFloat *return_float, ccm_tInt *return_int, char *return_string);
static int ccm_func_ask( void *filectx, ccm_sArg *arg_list, int arg_count, int *return_decl,
  ccm_tFloat *return_float, ccm_tInt *return_int, char *return_string);
static int ccm_func_fprintf( void *filectx, ccm_sArg *arg_list, int arg_count, int *return_decl,
  ccm_tFloat *return_float, ccm_tInt *return_int, char *return_string);
static int ccm_func_sprintf( void *filectx, ccm_sArg *arg_list, int arg_count, int *return_decl,
  ccm_tFloat *return_float, ccm_tInt *return_int, char *return_string);
static int ccm_func_fgets( void *filectx, ccm_sArg *arg_list, int arg_count, int *return_decl,
  ccm_tFloat *return_float, ccm_tInt *return_int, char *return_string);
static int ccm_func_fopen( void *filectx, ccm_sArg *arg_list, int arg_count, int *return_decl,
  ccm_tFloat *return_float, ccm_tInt *return_int, char *return_string);
static int ccm_func_fclose( void *filectx, ccm_sArg *arg_list, int arg_count, int *return_decl,
  ccm_tFloat *return_float, ccm_tInt *return_int, char *return_string);
static int ccm_func_exit( void *filectx, ccm_sArg *arg_list, int arg_count, int *return_decl,
  ccm_tFloat *return_float, ccm_tInt *return_int, char *return_string);
static int ccm_func_element( void *filectx, ccm_sArg *arg_list, int arg_count, int *return_decl,
  ccm_tFloat *return_float, ccm_tInt *return_int, char *return_string);
static int ccm_func_felement( void *filectx, ccm_sArg *arg_list, int arg_count, int *return_decl,
  ccm_tFloat *return_float, ccm_tInt *return_int, char *return_string);
static int ccm_func_extract( void *filectx, ccm_sArg *arg_list, int arg_count, int *return_decl, 
	ccm_tFloat *return_float, ccm_tInt *return_int, char *return_string);
static int ccm_func_edit( void *filectx, ccm_sArg *arg_list, int arg_count, int *return_decl, 
	ccm_tFloat *return_float, ccm_tInt *return_int, char *return_string);
static int ccm_func_verify( void *filectx, ccm_sArg *arg_list, int arg_count, int *return_decl, 
	ccm_tFloat *return_float, ccm_tInt *return_int, char *return_string);
static int ccm_func_time( void *filectx, ccm_sArg *arg_list, int arg_count, int *return_decl, 
	ccm_tFloat *return_float, ccm_tInt *return_int, char *return_string);
static int ccm_func_system( void *filectx, ccm_sArg *arg_list, int arg_count, int *return_decl, 
	ccm_tFloat *return_float, ccm_tInt *return_int, char *return_string);
static int ccm_func_strlen( void *filectx, ccm_sArg *arg_list, int arg_count, int *return_decl, 
	ccm_tFloat *return_float, ccm_tInt *return_int, char *return_string);
static int ccm_func_strchr( void *filectx, ccm_sArg *arg_list, int arg_count, int *return_decl, 
	ccm_tFloat *return_float, ccm_tInt *return_int, char *return_string);
static int ccm_func_strrchr( void *filectx, ccm_sArg *arg_list, int arg_count, int *return_decl, 
	ccm_tFloat *return_float, ccm_tInt *return_int, char *return_string);
static int ccm_func_strstr( void *filectx, ccm_sArg *arg_list, int arg_count, int *return_decl, 
	ccm_tFloat *return_float, ccm_tInt *return_int, char *return_string);
static int ccm_func_toupper( void *filectx, ccm_sArg *arg_list, int arg_count, int *return_decl, 
	ccm_tFloat *return_float, ccm_tInt *return_int, char *return_string);
static int ccm_func_tolower( void *filectx, ccm_sArg *arg_list, int arg_count, int *return_decl, 
	ccm_tFloat *return_float, ccm_tInt *return_int, char *return_string);
static int ccm_func_translate_filename( void *filectx, ccm_sArg *arg_list, int arg_count, int *return_decl, 
	ccm_tFloat *return_float, ccm_tInt *return_int, char *return_string);
static int ccm_func_get_pwr_config( void *filectx, ccm_sArg *arg_list, int arg_count, int *return_decl, 
	ccm_tFloat *return_float, ccm_tInt *return_int, char *return_string);
static int ccm_func_get_node_name( void *filectx, ccm_sArg *arg_list, int arg_count, int *return_decl, 
	ccm_tFloat *return_float, ccm_tInt *return_int, char *return_string);
static int ccm_func_get_language( void *filectx, ccm_sArg *arg_list, int arg_count, int *return_decl, 
	ccm_tFloat *return_float, ccm_tInt *return_int, char *return_string);

#define CCM_SYSFUNC_MAX	100

static ccm_sSysFunc	ccm_sysfunc[CCM_SYSFUNC_MAX] = {
		{"printf", 	&ccm_func_printf},
		{"scanf", 	&ccm_func_scanf},
		{"ask", 	&ccm_func_ask},
		{"say", 	&ccm_func_say},
		{"fprintf", 	&ccm_func_fprintf},
		{"sprintf", 	&ccm_func_sprintf},
		{"fgets", 	&ccm_func_fgets},
		{"fclose", 	&ccm_func_fclose},
		{"fopen", 	&ccm_func_fopen},
		{"exit", 	&ccm_func_exit},
		{"element", 	&ccm_func_element},
		{"felement", 	&ccm_func_felement},
		{"extract", 	&ccm_func_extract},
		{"edit", 	&ccm_func_edit},
		{"verify", 	&ccm_func_verify},
		{"time", 	&ccm_func_time},
		{"system", 	&ccm_func_system},
		{"strlen", 	&ccm_func_strlen},
		{"strchr", 	&ccm_func_strchr},
		{"strrchr", 	&ccm_func_strrchr},
		{"strstr", 	&ccm_func_strstr},
		{"toupper", 	&ccm_func_toupper},
		{"tolower", 	&ccm_func_tolower},
		{"translate_filename", 	&ccm_func_translate_filename},
		{"get_pwr_config",  &ccm_func_get_pwr_config},
		{"get_node_name",  &ccm_func_get_node_name},
		{"get_language",  &ccm_func_get_language},
		{"", 		0}};

/************* TEST *********************/

static int	rtt_parse( 	char	*string,
			char	*parse_char,
			char	*inc_parse_char,
			char	*outstr,
			int	max_rows,
			int 	max_cols,
			int	keep_quota)
{
	int	row;
	int	col;
	char	*char_ptr;	
	char	*inc_char_ptr;	
	int	parsechar_found;
	int	inc_parsechar_found;
	int	next_token;
	int	char_found;
	int	one_token = 0;
	unsigned char	prev_char;
	int	nullstr;

	prev_char = 0;
	row = 0;
	col = 0;
	char_found = 0;
	next_token = 0;
	nullstr = 0;
	while ( *string != '\0')	
	{
	  char_ptr = parse_char;
	  inc_char_ptr = inc_parse_char;
	  parsechar_found = 0;
	  inc_parsechar_found = 0;
	  if ( *string == '"' && prev_char != '\\')
	  {
	    one_token = !one_token;
	    prev_char = (unsigned char) *string;
 	    if ( !one_token && col == 0)
 	      nullstr = 1;
	    else
	      nullstr = 0;
	    if ( !keep_quota)
	    {
	      string++; 
	      continue;
	    }
	  }
	  else if ( *string == '"' && prev_char == '\\')
	    col--;
	  if ( !one_token)
	  {
	    while ( *char_ptr != '\0')
	    {
	      /* Check if this is a parse charachter */
	      if ( *string == *char_ptr)
	      {
	        parsechar_found = 1;
	        /* Next token */
	        if ( col > 0)	
	        {
	          *(outstr + row * max_cols + col) = '\0';
	          row++;
	          if ( row >= max_rows )
	            return row;
	          col = 0;
	          next_token = 0;
	        }
	        break;
	      }
	      char_ptr++;	    
	    }
	    while ( *inc_char_ptr != '\0')
	    {
	      /* Check if this is a parse charachter */
	      if ( *string == *inc_char_ptr)
	      {
	        parsechar_found = 1;
	        inc_parsechar_found = 1;
	        /* Next token */
	        if ( col > 0 || nullstr)	
	        {
	          *(outstr + row * max_cols + col) = '\0';
	          row++;
	          if ( row >= max_rows )
	            return row;
	          col = 0;
	          next_token = 0;
	        }
	        break;
	      }
	      inc_char_ptr++;	    
	    }
	  }
	  if ( !parsechar_found && !next_token)
	  {
	    char_found++;
	    *(outstr + row * max_cols + col) = *string;
	    col++;
	  }
	  if ( inc_parsechar_found)
	  {
	    *(outstr + row * max_cols + col) = *inc_char_ptr;
	    col++;
	  }
	  prev_char = (unsigned char) *string;
	  string++; 
	  if ( col >= (max_cols - 1))
	    next_token = 1;	    
	  nullstr = 0;
	}
	*(outstr + row * max_cols + col) = '\0';
	row++;

	if ( char_found == 0)
	  return 0;

	return row;
}

static int rtt_toupper( 	char	*str_upper,
				char	*str)
{
	char	namechar;
	char	*u;
	char	*t; 
	int	convert;
	unsigned char prev_char;

	/* Convert to upper case */
	u = str;
	t = str_upper;
	prev_char = 0;
	convert = 1;
	while ( *u != '\0')
	{
	  namechar = *(u++);
/*
	  if ( namechar == '"' && prev_char != '\\')
	    convert = !convert;
*/
	  if (convert)
	  {
	    *t = (char) r_toupper( namechar);
	    if (*t == 'ö') *t = (char) 'Ö';
	    else if (*t == 'ä') *t = (char) 'Ä';
	    else if (*t == 'å') *t = (char) 'Å';
	  }
	  else
	    *t = namechar;
	  prev_char = (unsigned char) *t;
	  t++;
	}
	*t = '\0';

	return 1;
}

/************* SLUT TEST *********************/

static int ccm_element( char *element, int num, char *str, char delim)
{
  char	*s1, *s2;
  int	i;
  int	max_size;

  if ( num <= 0)
    return 0;

  s1 = strchr( str, delim);
  if ( num == 1)
  {
    if (s1 == 0)
    {
      strncpy( element, str, K_STRING_SIZE);
      element[K_STRING_SIZE-1] = 0;
      return 1;
    }
    else
    {
      max_size = min( s1 - str, K_STRING_SIZE-1);
      strncpy( element, str, max_size);
      element[max_size] = 0;
      return 1;
    }
  }
  else if ( s1 == 0)
  {
    strcpy( element, "");
    return 0;
  }
  for ( i = 2; i <= num; i++)
  {
    s1++;
    s2 = strchr( s1, delim);
    if ( s2 == 0)
    {
      if ( i == num)
      {
        strncpy( element, s1, K_STRING_SIZE);
        element[K_STRING_SIZE-1] = 0;
        return 1;
      }
      else
      {
        strcpy( element, "");
        return 0;
      }
    }  
    else if ( i == num)
    {
      max_size = min( s2 - s1, K_STRING_SIZE-1);
      strncpy( element, s1, max_size);
      element[max_size] = 0;
      return 1;
    }
    s1 = s2;
  }
  return 0;
}

int	ccm_remove_blank( char *out_str, char *in_str)
{
	char *s;

	s = in_str;
	/* Find first not blank */
	while ( *s)
	{
	  if ( !(*s == 9 || *s == 32)) 
	    break;
	  s++;
	}
	strcpy( out_str, s);
	/* Remove at end */
	s = out_str + strlen(out_str);
	s--;
	while( s >= out_str)
	{
	  if ( !(*s == 9 || *s == 32 || *s == 10)) 
	    break;
	  s--;
	}
	s++;
	*s = 0;
	return 1;
}


int ccm_varname_parse( 
	ccm_tFuncCtx	funcctx,
	const char 	*name, 
	char	*varname,
	int	*array,
	int	*element)
{
	char	*s, *t;
	char	elementstr[80];
	char	vname[80];
	int	len;
	int	value_decl;
	ccm_tInt	value_int;
	ccm_tFloat	value_float;
	ccm_tString	value_string;
	int	sts;

	/* Check index in parameter */
	strcpy( varname, name);
	s = strchr( varname, '[');
	if ( s == 0)
	{
	  *array = 0;
	  *element = 0;
	}
	else
	{
	  t = strchr( varname, ']');
	  if ( t == 0)
            return 0;
	  else
	  {
	    len = t - s - 1;
	    strncpy( elementstr, s + 1, len);
	    elementstr[ len] = 0;
	    if ( !isdigit(elementstr[0]))
	    {
	      if ( funcctx == NULL)
	        return 0;
              ccm_remove_blank( vname, elementstr);
              sts = ccm_getvar( funcctx, vname, &value_decl,
			&value_float, &value_int,
			value_string);
              if ( EVEN(sts)) return sts;
	      if ( value_decl != K_DECL_INT)
	        return 0;
	      *element = value_int;
	    }
	    sscanf( elementstr, "%d", element);
	    *s = '\0';
	    if ( (*element < 0) || (*element > 1000) )
	    {
	      return 0;
	    }
	    *array = 1;
	  }
	}
	return 1;
}

int ccm_register_function( 
	const char 	*name,
	int 	(* sysfunc) ( void *, ccm_sArg *, int, int *, ccm_tFloat *,
			  ccm_tInt *, char *)
)
{
  ccm_sSysFunc	*sysfunc_p;
  int		i;

  i = 0;
  for ( sysfunc_p = ccm_sysfunc; sysfunc_p->sysfunc; sysfunc_p++) {
    if ( strcmp( sysfunc_p->name, name) == 0)
      return CCM__ALREADYREG;
    i++;    
  }

  if ( i >= CCM_SYSFUNC_MAX - 1)
    return CCM__SYSFUNCEXCEED;

  strcpy( sysfunc_p->name, name);
  sysfunc_p->sysfunc = sysfunc;

  return 1;
}

void ccm_float_to_string( char *string, ccm_tFloat f)
{
  int		i;

  /* If value is close to integer, round it */
  if ( fabs( f - floor( f)) <= FLT_EPSILON) {
    if ( f >= 0)
      i = f + FLT_EPSILON;
    else
      i = f - FLT_EPSILON;
    sprintf( string, "%d", i);
  }
  else if ( fabs( f - floor( f) + 1) <= FLT_EPSILON) {
    if ( f >= 0)
      i = f + FLT_EPSILON;
    else
      i = f - FLT_EPSILON;
    sprintf( string, "%d", i);
  }
  else
    sprintf( string, "%f", f);
}

static void ccm_print_error( ccm_tFileCtx filectx, int sts)
{
  char	text[80];

  switch ( sts)
  {
    case CCM__EXPRESSION : 	strcpy( text, "Syntax error in expression"); return;
    case CCM__VARTYPE :		strcpy( text, "Variable type mismatch"); break;
    case CCM__VARALREXIST :	strcpy( text, "Variable already declared"); break;
    case CCM__VARNOTFOUND :	strcpy( text, "Variable not found"); break;
    case CCM__OPENFILE :	strcpy( text, "Unable to open file"); break;
    case CCM__SYNTAX :		strcpy( text, "Syntax error"); break;
    case CCM__FUNCNOTFOUND :	strcpy( text, "Function not declared"); break;
    case CCM__UNKNVARTYPE :	strcpy( text, "Unknown varable type"); break;
    case CCM__FUNCMISM :	strcpy( text, "Function-endfunction mismatch"); break;
    case CCM__MAINMISM :	strcpy( text, "Main-endmain mismatch"); break;
    case CCM__IFMISM :		strcpy( text, "If-else-endif mismatch"); break;
    case CCM__WHILEMISM :	strcpy( text, "While-endwhile mismatch"); break;
    case CCM__FORMISM :		strcpy( text, "For-endfor mismatch"); break;
    case CCM__ARGMISM :		strcpy( text, "Argument mismatch"); break;
    case CCM__CONTMISM :	strcpy( text, "Continue-for/while mismatch"); break;
    case CCM__BREAKMISM :	strcpy( text, "Break-if/for/while mismatch"); break;
    case CCM__NOEXTVAR :	strcpy( text, "No external variables defined"); break;
    case CCM__GOTOMISM :	strcpy( text, "Label in goto-expression not found"); break;
    case CCM__SYSFUNCEXCEED :	strcpy( text, "Max number of functions exceeded"); break;
    case CCM__LONGLINE :	strcpy( text, "Maximum line size exceeded"); break;
    case CCM__STRINGEXCEED :	strcpy( text, "Maximum string size exceeded"); break;
    case CCM__ARGEXCEED :	strcpy( text, "Maximum number of arguments exceeded"); break;
    case CCM__ARRAYBOUNDS :	strcpy( text, "Array index out of bounds"); break;
    default :			strcpy( text, "Unknown error code");
  }
  if ( filectx->error_row)
  {
    sprintf( &text[strlen(text)], ", at line %d \"", filectx->error_row);
    strncat( text, filectx->error_line, sizeof(text)-strlen(text)-1);
    text[sizeof(text)-1] = 0;
  }
  (filectx->errormessage_func)( text, 0, filectx->client_data);
}

static int	ccm_replace_symbol( ccm_tFuncCtx funcctx, char *command, char *newcommand)
{
	char	*s;
	char	*t;
	char	*u;
	int	symbolmode;
	long int size;
	char	value[K_STRING_SIZE];
	char	symbol[80];
	int	sts;
	char	new[160];
	int	ignore_symbolmode;
	int	value_decl;
	ccm_tInt	value_int;
	ccm_tFloat	value_float;
	ccm_tString	value_string;

	symbolmode = 0;
	ignore_symbolmode = 0;
	s = command;
	t = new;

	while ( *s != 0)
	{
	  if ( (unsigned char) *s == '\\' &&
               (unsigned char) *(s+1) == '\'')
	  {	
	    if ( ignore_symbolmode)
	      ignore_symbolmode = 0;
	    else
	      ignore_symbolmode = 1;
	    strcpy( t, "'");
	    t++;
            s++;
	  }
	  else if (*s == '\'')
	  {
	    if ( symbolmode)
	    {
	      /* End of potential symbol */
	      size = (long int) s - (long int) u;
	      strncpy( symbol, u, size);
	      symbol[size] = 0;
              sts = ccm_getvar( funcctx, symbol, &value_decl, &value_float,
			&value_int, value_string);
	      if ( EVEN(sts))
	      {
	        /* It was no symbol */
	        strcpy( t, "'");
	        t++;
	        strcat( t, symbol);
	        t += strlen(symbol);
	        strcat( t, "'");
	        t++;
	      }
	      else
	      {
	        if ( value_decl == K_DECL_FLOAT)
	          ccm_float_to_string( value, value_float);
	        else if ( value_decl == K_DECL_INT)
	          sprintf( value, ccm_cIntFormat, value_int);
	        else
	          strcpy( value, value_string);
	        /* Symbol found */
	        strcpy( t, value);
	        t += strlen(value);
	      }
	      symbolmode = 0;
	    }
	    else
	    {
	      if ( ignore_symbolmode)
	      {
	        strcpy( t, "'");
	        t++;
	      }
	      else
	      {
	        symbolmode = 1;
	        u = s + 1; 
	      }
	    }
	    ignore_symbolmode = 0;
	  }
	  else
	  {
	    if ( !symbolmode)
	    {
	      *t = *s;
	      t++;
	    }
	    ignore_symbolmode = 0;
	  }
	  s++;
	}
	if ( symbolmode)
	{
	  strcpy( t, u);
	}
	else
	  *t = 0;

	strcpy( newcommand, new);
	return 1;
}

static void init_ctable( void)
{
	memset( ctable, 0, sizeof(ctable));
	memset( ptable, 0, sizeof(ptable));

	ptable[ K_ACTION_DOWN] = 	1;
	ptable[ K_ACTION_UP] = 		1;
	ptable[ K_ACTION_MUL] = 	8;
	ptable[ K_ACTION_DIV] = 	8;
	ptable[ K_ACTION_ADD] = 	10;
	ptable[ K_ACTION_SUB] = 	10;
	ptable[ K_ACTION_EQL] = 	20;
	ptable[ K_ACTION_LT] = 		13;
	ptable[ K_ACTION_GT] = 		13;
	ptable[ K_ACTION_GE] = 		13;
	ptable[ K_ACTION_LE] = 		13;
	ptable[ K_ACTION_EQ] = 		14;
	ptable[ K_ACTION_NE] = 		14;
	ptable[ K_ACTION_INV] = 	6;
	ptable[ K_ACTION_INCR] = 	2;
	ptable[ K_ACTION_DECR] = 	2;
	ptable[ K_ACTION_AND] = 	17;
	ptable[ K_ACTION_OR] = 		18;
	ptable[ K_ACTION_BITAND] = 	16;
	ptable[ K_ACTION_BITOR] = 	15;
	ptable[ K_ACTION_EQLADD] = 	19;
	ptable[ K_ACTION_EQLSUB] = 	19;
	ptable[ K_ACTION_EQLMUL] = 	19;
	ptable[ K_ACTION_EQLDIV] = 	19;
	ptable[ K_ACTION_CREALOCINT] = 	19;
	ptable[ K_ACTION_CREALOCFLOAT] = 19;
	ptable[ K_ACTION_CREALOCSTRING] = 19;
	ptable[ K_ACTION_CREAGBLINT] = 	19;
	ptable[ K_ACTION_CREAGBLFLOAT] = 19;
	ptable[ K_ACTION_CREAGBLSTRING] = 19;
	ptable[ K_ACTION_CREAEXTINT] = 	19;
	ptable[ K_ACTION_CREAEXTFLOAT] = 19;
	ptable[ K_ACTION_CREAEXTSTRING] = 19;
	ptable[ K_ACTION_LSHIFT] = 	12;
	ptable[ K_ACTION_RSHIFT] = 	12;
	ptable[ K_ACTION_FUNCTION] = 	2;

	ctable[0] = K_ACTION_END;
	ctable[';'] = K_ACTION_END;
	ctable['	'] = K_ACTION_DELIM;
	ctable[' '] = K_ACTION_DELIM;
	ctable[10]  = K_ACTION_DELIM;
	ctable['!'] = K_ACTION_INV;
	ctable['='] = K_ACTION_EQL;
	ctable['>'] = K_ACTION_GT;
	ctable['<'] = K_ACTION_LT;
	ctable[')'] = K_ACTION_DOWN;
	ctable['('] = K_ACTION_UP;
	ctable['*'] = K_ACTION_MUL;
	ctable['/'] = K_ACTION_DIV;
	ctable['+'] = K_ACTION_ADD;
	ctable['-'] = K_ACTION_SUB;
	ctable['|'] = K_ACTION_OR;
	ctable['&'] = K_ACTION_AND;
	ctable['_'] = K_ACTION_VAR;
	ctable['A'] = K_ACTION_VAR;
	ctable['B'] = K_ACTION_VAR;
	ctable['C'] = K_ACTION_VAR;
	ctable['D'] = K_ACTION_VAR;
	ctable['E'] = K_ACTION_VAR;
	ctable['F'] = K_ACTION_VAR;
	ctable['G'] = K_ACTION_VAR;
	ctable['H'] = K_ACTION_VAR;
	ctable['I'] = K_ACTION_VAR;
	ctable['J'] = K_ACTION_VAR;
	ctable['K'] = K_ACTION_VAR;
	ctable['L'] = K_ACTION_VAR;
	ctable['M'] = K_ACTION_VAR;
	ctable['N'] = K_ACTION_VAR;
	ctable['O'] = K_ACTION_VAR;
	ctable['P'] = K_ACTION_VAR;
	ctable['Q'] = K_ACTION_VAR;
	ctable['R'] = K_ACTION_VAR;
	ctable['S'] = K_ACTION_VAR;
	ctable['T'] = K_ACTION_VAR;
	ctable['U'] = K_ACTION_VAR;
	ctable['V'] = K_ACTION_VAR;
	ctable['W'] = K_ACTION_VAR;
	ctable['X'] = K_ACTION_VAR;
	ctable['Y'] = K_ACTION_VAR;
	ctable['Z'] = K_ACTION_VAR;
	ctable[(unsigned char)'Å'] = K_ACTION_VAR;
	ctable[(unsigned char)'Ä'] = K_ACTION_VAR;
	ctable[(unsigned char)'Ö'] = K_ACTION_VAR;
	ctable['a'] = K_ACTION_VAR;
	ctable['b'] = K_ACTION_VAR;
	ctable['c'] = K_ACTION_VAR;
	ctable['d'] = K_ACTION_VAR;
	ctable['e'] = K_ACTION_VAR;
	ctable['f'] = K_ACTION_VAR;
	ctable['g'] = K_ACTION_VAR;
	ctable['h'] = K_ACTION_VAR;
	ctable['i'] = K_ACTION_VAR;
	ctable['j'] = K_ACTION_VAR;
	ctable['k'] = K_ACTION_VAR;
	ctable['l'] = K_ACTION_VAR;
	ctable['m'] = K_ACTION_VAR;
	ctable['n'] = K_ACTION_VAR;
	ctable['o'] = K_ACTION_VAR;
	ctable['p'] = K_ACTION_VAR;
	ctable['q'] = K_ACTION_VAR;
	ctable['r'] = K_ACTION_VAR;
	ctable['s'] = K_ACTION_VAR;
	ctable['t'] = K_ACTION_VAR;
	ctable['u'] = K_ACTION_VAR;
	ctable['v'] = K_ACTION_VAR;
	ctable['w'] = K_ACTION_VAR;
	ctable['x'] = K_ACTION_VAR;
	ctable['y'] = K_ACTION_VAR;
	ctable['z'] = K_ACTION_VAR;
	ctable[(unsigned char)'å'] = K_ACTION_VAR;
	ctable[(unsigned char)'ä'] = K_ACTION_VAR;
	ctable[(unsigned char)'ö'] = K_ACTION_VAR;
	ctable[(unsigned char)'['] = K_ACTION_VAR;
	ctable[(unsigned char)']'] = K_ACTION_VAR;
	ctable['0'] = K_ACTION_NUM;
	ctable['1'] = K_ACTION_NUM;
	ctable['2'] = K_ACTION_NUM;
	ctable['3'] = K_ACTION_NUM;
	ctable['4'] = K_ACTION_NUM;
	ctable['5'] = K_ACTION_NUM;
	ctable['6'] = K_ACTION_NUM;
	ctable['7'] = K_ACTION_NUM;
	ctable['8'] = K_ACTION_NUM;
	ctable['9'] = K_ACTION_NUM;
	ctable['.'] = K_ACTION_FLOAT;
	ctable['"'] = K_ACTION_STRING;
	ctable[','] = K_ACTION_LIST;
	ctable['\\'] = K_ACTION_BACKSLASH;
}

static void set_operand_parlevel( ccm_tRowCtx rowctx)
{
  ccm_sOperand 	*curr_operand;
  ccm_sOperand 	*operand;
  
  if ( rowctx->curr_operand == NULL)
    return;
  curr_operand = rowctx->curr_operand;
  operand = curr_operand->prev;
  if ( curr_operand->prev == NULL)
    curr_operand->parlevel = 1;
  else if ( !curr_operand->parlevel )
    curr_operand->parlevel = operand->parlevel;

}

static void set_operand_parlevel_down( ccm_tRowCtx rowctx)
{
  ccm_sOperand 	*curr_operand;
  ccm_sOperand 	*operand;
  
  curr_operand = rowctx->curr_operand;
  operand = curr_operand->prev;
  if ( curr_operand->prev == NULL)
  {
    curr_operand->parlevel = 1;
    return;
  }

  if ( operand->parlevel == 1)
  {
    rowctx->state = K_STATE_ERROR;
    strcpy( rowctx->msg, "Parentheses missmatch");
  }
  else
  {
    curr_operand->parlevel = operand->parlevel - 1;
  }
}

static void set_operand_parlevel_up( ccm_tRowCtx rowctx)
{
  ccm_sOperand 	*curr_operand;
  ccm_sOperand 	*operand;
  
  curr_operand = rowctx->curr_operand;
  operand = curr_operand->prev;
  if ( curr_operand->prev == NULL)
  {
    curr_operand->parlevel = 2;
    return;
  }

  curr_operand->parlevel = operand->parlevel + 1;
}

static void create_no_operand( ccm_tRowCtx rowctx)
{
  ccm_sOperand 	*operand_p;
  ccm_sOperand 	*prev_operand;
 
  operand_p = calloc( 1, sizeof(*operand_p));

  /* Insert in list */
  if ( rowctx->list == NULL)
  {
    rowctx->list = operand_p;
    rowctx->curr_operand = operand_p;
  }
  else
  {
    prev_operand = rowctx->curr_operand;
    prev_operand->next = operand_p;
    rowctx->curr_operand = operand_p;
    operand_p->prev = prev_operand;
  }
  operand_p->type = K_OPERAND_NO;
}

static int operand_found( ccm_tRowCtx rowctx)
{
  ccm_sOperand 	*operand_p;
  ccm_sOperand 	*prev_operand;
  int		nr;
  char		upname[80];
 
  if ( rowctx->last_type != K_TYPE_OPERATOR)
  {
    rowctx->state = K_STATE_ERROR;
    return CCM__SYNTAX;
  }

  operand_p = calloc( 1, sizeof(*operand_p));

  if ( rowctx->state == K_STATE_VAR)
  {
    /* Variable found */
    operand_p->type = K_OPERAND_NAME;
    strncpy( operand_p->name, &rowctx->line[rowctx->delim_pos], 
		rowctx->pos - rowctx->delim_pos);

    rowctx->last_type = K_TYPE_OPERAND;

    /* Check if reserved word */
    rtt_toupper( upname, operand_p->name);
    if ( strcmp( upname, "INT") == 0)
    {
      if ( rowctx->curr_operand && 
           rowctx->curr_operand->type == K_OPERAND_GLOBAL)
        operand_p->operator = K_ACTION_CREAGBLINT;
      else if ( rowctx->curr_operand && 
                rowctx->curr_operand->type == K_OPERAND_EXTERN)
        operand_p->operator = K_ACTION_CREAEXTINT;
      else
        operand_p->operator = K_ACTION_CREALOCINT;
      operand_p->type = K_OPERAND_DECL;
      rowctx->last_type = K_TYPE_OPERATOR;
    }
    else if ( strcmp( upname, "FLOAT") == 0)
    {
      if ( rowctx->curr_operand && 
           rowctx->curr_operand->type == K_OPERAND_GLOBAL)
        operand_p->operator = K_ACTION_CREAGBLFLOAT;
      else if ( rowctx->curr_operand && 
                rowctx->curr_operand->type == K_OPERAND_EXTERN)
        operand_p->operator = K_ACTION_CREAEXTFLOAT;
      else
        operand_p->operator = K_ACTION_CREALOCFLOAT;
      operand_p->type = K_OPERAND_DECL;
      rowctx->last_type = K_TYPE_OPERATOR;
    }
    else if ( strcmp( upname, "STRING") == 0)
    {
      if ( rowctx->curr_operand && 
           rowctx->curr_operand->type == K_OPERAND_GLOBAL)
        operand_p->operator = K_ACTION_CREAGBLSTRING;
      else if ( rowctx->curr_operand && 
                rowctx->curr_operand->type == K_OPERAND_EXTERN)
        operand_p->operator = K_ACTION_CREAEXTSTRING;
      else
        operand_p->operator = K_ACTION_CREALOCSTRING;
      operand_p->type = K_OPERAND_DECL;
      rowctx->last_type = K_TYPE_OPERATOR;
    }
    else if ( strcmp( upname, "GLOBAL") == 0)
    {
      operand_p->type = K_OPERAND_GLOBAL;
      operand_p->operator = K_ACTION_NO;
      rowctx->last_type = K_TYPE_OPERATOR;
    }
    else if ( strcmp( upname, "EXTERN") == 0)
    {
      operand_p->type = K_OPERAND_EXTERN;
      operand_p->operator = K_ACTION_NO;
      rowctx->last_type = K_TYPE_OPERATOR;
    }
    else if ( strcmp( upname, "DELETE") == 0)
    {
      operand_p->type = K_OPERAND_DELETE;
      operand_p->operator = K_ACTION_DELETE;
      rowctx->last_type = K_TYPE_OPERATOR;
    }
    else if ( strcmp( upname, "RETURN") == 0)
    {
      operand_p->type = K_OPERAND_RETURN;
      operand_p->operator = K_ACTION_RETURN;
      rowctx->last_type = K_TYPE_OPERATOR;
    }

  }
  else if ( rowctx->state == K_STATE_NUM)
  {
    /* Numeric found */
    operand_p = calloc( 1, sizeof(*operand_p));
    operand_p->type = K_OPERAND_VALUE;
    strncpy( operand_p->name, &rowctx->line[rowctx->delim_pos], 
	rowctx->pos - rowctx->delim_pos);
    if ( rowctx->num_decl == K_DECL_FLOAT)
    {
      nr = sscanf( operand_p->name, "%f", &operand_p->value_float);
      operand_p->value_decl = K_DECL_FLOAT;
      if ( rowctx->num_neg)
        operand_p->value_float = -operand_p->value_float;
    }
    else
    {
      nr = sscanf( operand_p->name, ccm_cIntFormat, &operand_p->value_int);
      operand_p->value_decl = K_DECL_INT;
      if ( rowctx->num_neg)
        operand_p->value_int = -operand_p->value_int;
    }
    rowctx->num_decl = 0;
    rowctx->last_type = K_TYPE_OPERAND;
  }
  else if ( rowctx->state == K_STATE_STRING)
  {
    /* String found */
    operand_p->type = K_OPERAND_VALUE;
    if ( rowctx->pos - rowctx->delim_pos > K_STRING_SIZE - 1)
    {
      rowctx->state = K_STATE_ERROR;
      strcpy( rowctx->msg, "String too long");
      return CCM__SYNTAX;
    }
    strncpy( operand_p->name, &rowctx->line[rowctx->delim_pos], 
		rowctx->pos - rowctx->delim_pos);
    strncpy( operand_p->value_string, &rowctx->line[rowctx->delim_pos], 
		rowctx->pos - rowctx->delim_pos);
    operand_p->value_decl = K_DECL_STRING;
    rowctx->last_type = K_TYPE_OPERAND;
  }

  /* Insert in list */
  if ( rowctx->list == NULL)
  {
    rowctx->list = operand_p;
    rowctx->curr_operand = operand_p;
  }
  else
  {
    prev_operand = rowctx->curr_operand;
    prev_operand->next = operand_p;
    rowctx->curr_operand = operand_p;
    operand_p->prev = prev_operand;
  }
  rowctx->num_neg = 0;

  return CCM__SUCCESS;
}

static void function_found( ccm_tRowCtx rowctx)
{
  ccm_sOperand 	*operand_p;
  ccm_sOperand 	*prev_operand;
 
  operand_p = calloc( 1, sizeof(*operand_p));

  operand_p->type = K_OPERAND_FUNCTION;
  strncpy( operand_p->name, &rowctx->line[rowctx->delim_pos], 
		rowctx->pos - rowctx->delim_pos);
  operand_p->operator = K_ACTION_FUNCTION;
  rowctx->last_type = K_TYPE_OPERATOR;

  /* Insert in list */
  if ( rowctx->list == NULL)
  {
    rowctx->list = operand_p;
    rowctx->curr_operand = operand_p;
  }
  else
  {
    prev_operand = rowctx->curr_operand;
    prev_operand->next = operand_p;
    rowctx->curr_operand = operand_p;
    operand_p->prev = prev_operand;
  }

}

void ccm_free_list(  ccm_sOperand *list)
{
  ccm_sOperand	*op;
  ccm_sOperand	*next;

  for ( op = list; op; op = next)
  {
    next = op->next;
    free( (char *) op);    
  }
}

int	ccm_create_list( 
  ccm_tFileCtx filectx,
  char *line,
  ccm_sOperand **list)
{
  ccm_tRowCtx rowctx;
  char		msg[80];
  int		sts;

  rowctx = calloc( 1, sizeof(*rowctx));
  strcpy( rowctx->line, line);
  rowctx->filectx = filectx;

  init_ctable();

  for (;;)
  {
    if ( rowctx->pos > (int)sizeof(rowctx->line))
    {
      rowctx->state = K_STATE_ERROR;
      strcpy( rowctx->msg, "Expression to large");
      break;
    }
    if ( rowctx->state == K_STATE_STRING)
    {
      if ( ctable[ (unsigned char) rowctx->line[rowctx->pos]] != K_ACTION_STRING ||
           (ctable[ (unsigned char) rowctx->line[rowctx->pos]] == K_ACTION_STRING &&
            ctable[ (unsigned char) rowctx->line[rowctx->pos-1]] == K_ACTION_BACKSLASH))
      {
        rowctx->pos++;
        continue;
      }
    }
    switch( ctable[ (unsigned char) rowctx->line[rowctx->pos]])
    {
      case K_ACTION_VAR:
        if ( rowctx->state == K_STATE_NUM)
        {
          rowctx->state = K_STATE_ERROR;
	  strcpy( rowctx->msg, "Syntax error");
        }
        else if ( rowctx->state == K_STATE_NUM_NEG)
	{
          rowctx->state = K_STATE_VAR;
	  rowctx->delim_pos = rowctx->pos;

          /* Create an -1 operand with multiplication operator */
	  rowctx->last_type = K_TYPE_OPERATOR;
	  rowctx->delim_pos = rowctx->pos;
          create_no_operand( rowctx);
	  rowctx->curr_operand->operator = K_ACTION_MUL;
          rowctx->curr_operand->type = K_OPERAND_VALUE;
          strcpy( rowctx->curr_operand->name, "-1");
          rowctx->curr_operand->value_int = -1;
          rowctx->curr_operand->value_decl = K_DECL_INT;
	  rowctx->curr_operand->prio = ptable[rowctx->curr_operand->operator];
	  set_operand_parlevel( rowctx);
	}
        else if ( !(rowctx->state == K_STATE_VAR || rowctx->state == K_STATE_STRING))
	{
          rowctx->state = K_STATE_VAR;
	  rowctx->delim_pos = rowctx->pos;
	}
        rowctx->pos++;
        break;

      case K_ACTION_NUM:
        if ( rowctx->state == K_STATE_NUM_NEG)
	{
          rowctx->state = K_STATE_NUM;
	  rowctx->delim_pos = rowctx->pos;
	  rowctx->num_neg = 1;
	}
        else if ( !(rowctx->state == K_STATE_VAR || rowctx->state == K_STATE_NUM))
	{
          rowctx->state = K_STATE_NUM;
	  rowctx->delim_pos = rowctx->pos;
	}
        rowctx->pos++;
        break;

      case K_ACTION_FLOAT:
        if ( rowctx->state == K_STATE_NUM)
          rowctx->num_decl = K_DECL_FLOAT;
        else if ( rowctx->state == K_STATE_VAR)
	  ;
	else
        {
          rowctx->state = K_STATE_ERROR;
	  strcpy( rowctx->msg, "Syntax error");
        }
        rowctx->pos++;
        break;

      case K_ACTION_STRING:
        if ( rowctx->state == K_STATE_NUM)
        {
          rowctx->state = K_STATE_ERROR;
	  strcpy( rowctx->msg, "Syntax error");
        }
        else if ( rowctx->state != K_STATE_STRING)
	{
          rowctx->state = K_STATE_STRING;
	  rowctx->delim_pos = rowctx->pos + 1;
	}
        else
	{
	  sts = operand_found( rowctx);
	  if ( EVEN(sts)) break;
          rowctx->state = K_STATE_DELIM;
	}
        rowctx->pos++;
        break;

      case K_ACTION_LIST:
        if ( rowctx->state == K_STATE_NUM ||
             rowctx->state == K_STATE_VAR)
        {
	  sts = operand_found( rowctx);
	  if ( EVEN(sts)) break;
          rowctx->last_type = K_TYPE_OPERATOR;
	}
	else if ( !(rowctx->state == K_STATE_DELIM &&
	            rowctx->last_type == K_TYPE_OPERAND))
	{
          rowctx->state = K_STATE_ERROR;
	  strcpy( rowctx->msg, "Syntax error");
        }

	rowctx->curr_operand->operator = K_ACTION_LIST;
        rowctx->state = K_STATE_LIST;
        rowctx->last_type = K_TYPE_OPERATOR;
	set_operand_parlevel( rowctx);
        rowctx->pos++;
        break;

      case K_ACTION_DOWN:
        if ( rowctx->state == K_STATE_NUM ||
             rowctx->state == K_STATE_VAR)
        {
	  sts = operand_found( rowctx);
	  if ( EVEN(sts)) break;
        }
	else if ( rowctx->last_type != K_TYPE_OPERAND)
        {
          create_no_operand( rowctx);
          rowctx->last_type = K_TYPE_OPERAND;
        }
        rowctx->pos++;
	set_operand_parlevel_down( rowctx);
        rowctx->state = K_STATE_PAR;
        break;

      case K_ACTION_UP:
        if ( rowctx->state == K_STATE_VAR &&
             rowctx->last_type == K_TYPE_OPERATOR)
          function_found( rowctx);
        else if ( rowctx->state == K_STATE_NUM_NEG)
	{
          /* Create an -1 operand with multiplication operator */
	  rowctx->last_type = K_TYPE_OPERATOR;
	  rowctx->delim_pos = rowctx->pos;
          create_no_operand( rowctx);
	  rowctx->curr_operand->operator = K_ACTION_MUL;
          rowctx->curr_operand->type = K_OPERAND_VALUE;
          strcpy( rowctx->curr_operand->name, "-1");
          rowctx->curr_operand->value_int = -1;
          rowctx->curr_operand->value_decl = K_DECL_INT;
	  rowctx->curr_operand->prio = ptable[rowctx->curr_operand->operator];
	  set_operand_parlevel( rowctx);

          create_no_operand( rowctx);
	}
        else if ( rowctx->state == K_STATE_NUM ||
             rowctx->state == K_STATE_VAR)
        {
	  sts = operand_found( rowctx);
	  if ( EVEN(sts)) break;
        }
	else if ( rowctx->last_type != K_TYPE_OPERAND)
	{
          create_no_operand( rowctx);
          rowctx->last_type = K_TYPE_OPERATOR;
 	}
        rowctx->pos++;
	set_operand_parlevel_up( rowctx);
        rowctx->state = K_STATE_PAR;
        break;

      case K_ACTION_INV:
	if ( rowctx->line[rowctx->pos+1] == '=')
	{
          if ( rowctx->state == K_STATE_NUM ||
               rowctx->state == K_STATE_VAR)
          {
	    sts = operand_found( rowctx);
	    if ( EVEN(sts)) break;
          }
	
	  if ( rowctx->last_type != K_TYPE_OPERAND)
          {
            rowctx->state = K_STATE_ERROR;
	    strcpy( rowctx->msg, "Syntax error");
	    break;
          }

	  rowctx->curr_operand->operator = K_ACTION_NE;
	  rowctx->pos++;
	  rowctx->pos++;
	  rowctx->last_type = K_TYPE_OPERATOR;
          rowctx->state = K_STATE_OPERATOR;
	  rowctx->curr_operand->prio = ptable[rowctx->curr_operand->operator];
	  set_operand_parlevel( rowctx);
        }
	else
	{
          if ( rowctx->state == K_STATE_NUM ||
               rowctx->state == K_STATE_VAR)
          {
            rowctx->state = K_STATE_ERROR;
	    strcpy( rowctx->msg, "Syntax error");
	  }
	  else
	  {
            create_no_operand( rowctx);
            rowctx->last_type = K_TYPE_OPERATOR;
	    rowctx->curr_operand->operator = K_ACTION_INV;
            rowctx->pos++;
	    set_operand_parlevel( rowctx);
          }
 	}
        break;

      case K_ACTION_ADD:
        if ( rowctx->state == K_STATE_NUM ||
             rowctx->state == K_STATE_VAR)
	{
	  sts = operand_found( rowctx);
	  if ( EVEN(sts)) break;
	}
	
	if ( rowctx->line[rowctx->pos+1] == '+')
	{
	  if ( rowctx->last_type != K_TYPE_OPERAND)
          {
            rowctx->state = K_STATE_ERROR;
	    strcpy( rowctx->msg, "Syntax error");
	    break;
          }
	  rowctx->curr_operand->local_operator = K_ACTION_INCR;
	  rowctx->curr_operand->local_type = K_LOCTYPE_AFTER;
	  rowctx->pos++;
	  rowctx->pos++;
	  rowctx->state = K_STATE_DELIM;
	}
	else if ( rowctx->line[rowctx->pos+1] == '=')
	{
	  if ( rowctx->last_type != K_TYPE_OPERAND)
          {
            rowctx->state = K_STATE_ERROR;
	    strcpy( rowctx->msg, "Syntax error");
	    break;
          }
	  rowctx->curr_operand->operator = K_ACTION_EQLADD;
	  rowctx->pos++;
	  rowctx->pos++;
	  rowctx->last_type = K_TYPE_OPERATOR;
          rowctx->state = K_STATE_OPERATOR;
	}
	else
	{
	  if ( rowctx->last_type != K_TYPE_OPERAND)
          {
            rowctx->state = K_STATE_ERROR;
	    strcpy( rowctx->msg, "Syntax error");
	    break;
          }
	  rowctx->curr_operand->operator = K_ACTION_ADD;
	  rowctx->pos++;
	  rowctx->last_type = K_TYPE_OPERATOR;
          rowctx->state = K_STATE_OPERATOR;
	}
	rowctx->curr_operand->prio = ptable[rowctx->curr_operand->operator];
	set_operand_parlevel( rowctx);
        break;

      case K_ACTION_SUB:
        if ( rowctx->state == K_STATE_NUM ||
             rowctx->state == K_STATE_VAR)
	{
	  sts = operand_found( rowctx);
	  if ( EVEN(sts)) break;
	}

	if ( rowctx->line[rowctx->pos+1] == '-')
	{
	  if ( rowctx->last_type != K_TYPE_OPERAND)
          {
            rowctx->state = K_STATE_ERROR;
	    strcpy( rowctx->msg, "Syntax error");
	    break;
          }
	  rowctx->curr_operand->local_operator = K_ACTION_DECR;
	  rowctx->curr_operand->local_type = K_LOCTYPE_AFTER;
	  rowctx->state = K_STATE_DELIM;
	  rowctx->pos++;
	  rowctx->pos++;
	}
	else if ( rowctx->line[rowctx->pos+1] == '=')
	{
	  if ( rowctx->last_type != K_TYPE_OPERAND)
          {
            rowctx->state = K_STATE_ERROR;
	    strcpy( rowctx->msg, "Syntax error");
	    break;
          }
	  rowctx->curr_operand->operator = K_ACTION_EQLSUB;
	  rowctx->pos++;
	  rowctx->pos++;
	  rowctx->last_type = K_TYPE_OPERATOR;
          rowctx->state = K_STATE_OPERATOR;
	}
	else if ( rowctx->last_type != K_TYPE_OPERAND)
        {
          rowctx->state = K_STATE_NUM_NEG;
	  rowctx->pos++;
        }
	else
	{
	  rowctx->curr_operand->operator = K_ACTION_SUB;
	  rowctx->pos++;
	  rowctx->last_type = K_TYPE_OPERATOR;
          rowctx->state = K_STATE_OPERATOR;
	}
	rowctx->curr_operand->prio = ptable[rowctx->curr_operand->operator];
	set_operand_parlevel( rowctx);
        break;

      case K_ACTION_MUL:
        if ( rowctx->state == K_STATE_NUM ||
             rowctx->state == K_STATE_VAR)
	{
	  sts = operand_found( rowctx);
	  if ( EVEN(sts)) break;
	}
	
	if ( rowctx->last_type != K_TYPE_OPERAND)
        {
          rowctx->state = K_STATE_ERROR;
	  strcpy( rowctx->msg, "Syntax error");
	  break;
        }

	else if ( rowctx->line[rowctx->pos+1] == '=')
	{
	  rowctx->curr_operand->operator = K_ACTION_EQLMUL;
	  rowctx->pos++;
	  rowctx->pos++;
	}
	else
	{
	  rowctx->curr_operand->operator = K_ACTION_MUL;
	  rowctx->pos++;
	}
	rowctx->last_type = K_TYPE_OPERATOR;
        rowctx->state = K_STATE_OPERATOR;
	rowctx->curr_operand->prio = ptable[rowctx->curr_operand->operator];
	set_operand_parlevel( rowctx);
        break;

      case K_ACTION_DIV:
        if ( rowctx->state == K_STATE_NUM ||
             rowctx->state == K_STATE_VAR)
	{
	  sts = operand_found( rowctx);
	  if ( EVEN(sts)) break;
	}
	
	if ( rowctx->last_type != K_TYPE_OPERAND)
        {
          rowctx->state = K_STATE_ERROR;
	  strcpy( rowctx->msg, "Syntax error");
	  break;
        }

	else if ( rowctx->line[rowctx->pos+1] == '=')
	{
	  rowctx->curr_operand->operator = K_ACTION_EQLDIV;
	  rowctx->pos++;
	  rowctx->pos++;
	}
	else
	{
	  rowctx->curr_operand->operator = K_ACTION_DIV;
	  rowctx->pos++;
	}
	rowctx->last_type = K_TYPE_OPERATOR;
        rowctx->state = K_STATE_OPERATOR;
	rowctx->curr_operand->prio = ptable[rowctx->curr_operand->operator];
	set_operand_parlevel( rowctx);
        break;

      case K_ACTION_EQL:
        if ( rowctx->state == K_STATE_NUM ||
             rowctx->state == K_STATE_VAR)
	{
	  sts = operand_found( rowctx);
	  if ( EVEN(sts)) break;
	}

	if ( rowctx->last_type != K_TYPE_OPERAND)
        {
          rowctx->state = K_STATE_ERROR;
	  strcpy( rowctx->msg, "Syntax error");
	  break;
        }

	else if ( rowctx->line[rowctx->pos+1] == '=')
	{
	  rowctx->curr_operand->operator = K_ACTION_EQ;
	  rowctx->pos++;
	  rowctx->pos++;
	}
	else
	{
	  rowctx->curr_operand->operator = K_ACTION_EQL;
	  rowctx->pos++;
	}
	rowctx->last_type = K_TYPE_OPERATOR;
        rowctx->state = K_STATE_OPERATOR;
	rowctx->curr_operand->prio = ptable[rowctx->curr_operand->operator];
	set_operand_parlevel( rowctx);
        break;

      case K_ACTION_LT:
        if ( rowctx->state == K_STATE_NUM ||
             rowctx->state == K_STATE_VAR)
	{
	  sts = operand_found( rowctx);
	  if ( EVEN(sts)) break;
	}
	
	if ( rowctx->last_type != K_TYPE_OPERAND)
        {
          rowctx->state = K_STATE_ERROR;
	  strcpy( rowctx->msg, "Syntax error");
	  break;
        }

	else if ( rowctx->line[rowctx->pos+1] == '=')
	{
	  rowctx->curr_operand->operator = K_ACTION_LE;
	  rowctx->pos++;
	  rowctx->pos++;
	}
	else if ( rowctx->line[rowctx->pos+1] == '<')
	{
	  rowctx->curr_operand->operator = K_ACTION_LSHIFT;
	  rowctx->pos++;
	  rowctx->pos++;
	}
	else
	{
	  rowctx->curr_operand->operator = K_ACTION_LT;
	  rowctx->pos++;
	}
	rowctx->last_type = K_TYPE_OPERATOR;
        rowctx->state = K_STATE_OPERATOR;
	rowctx->curr_operand->prio = ptable[rowctx->curr_operand->operator];
	set_operand_parlevel( rowctx);
        break;

      case K_ACTION_GT:
        if ( rowctx->state == K_STATE_NUM ||
             rowctx->state == K_STATE_VAR)
	{
	  sts = operand_found( rowctx);
	  if ( EVEN(sts)) break;
	}
	
	if ( rowctx->last_type != K_TYPE_OPERAND)
        {
          rowctx->state = K_STATE_ERROR;
	  strcpy( rowctx->msg, "Syntax error");
	  break;
        }

	else if ( rowctx->line[rowctx->pos+1] == '=')
	{
	  rowctx->curr_operand->operator = K_ACTION_GE;
	  rowctx->pos++;
	  rowctx->pos++;
	}
	else if ( rowctx->line[rowctx->pos+1] == '>')
	{
	  rowctx->curr_operand->operator = K_ACTION_RSHIFT;
	  rowctx->pos++;
	  rowctx->pos++;
	}
	else
	{
	  rowctx->curr_operand->operator = K_ACTION_GT;
	  rowctx->pos++;
	}
	rowctx->last_type = K_TYPE_OPERATOR;
        rowctx->state = K_STATE_OPERATOR;
	rowctx->curr_operand->prio = ptable[rowctx->curr_operand->operator];
	set_operand_parlevel( rowctx);
        break;

      case K_ACTION_AND:
        if ( rowctx->state == K_STATE_NUM ||
             rowctx->state == K_STATE_VAR)
	{
	  sts = operand_found( rowctx);
	  if ( EVEN(sts)) break;
	}	

	if ( rowctx->last_type != K_TYPE_OPERAND)
        {
          rowctx->state = K_STATE_ERROR;
	  strcpy( rowctx->msg, "Syntax error");
	  break;
        }

	else if ( rowctx->line[rowctx->pos+1] == '&')
	{
	  rowctx->curr_operand->operator = K_ACTION_AND;
	  rowctx->pos++;
	  rowctx->pos++;
	}
	else
	{
	  rowctx->curr_operand->operator = K_ACTION_BITAND;
	  rowctx->pos++;
	}
	rowctx->last_type = K_TYPE_OPERATOR;
        rowctx->state = K_STATE_OPERATOR;
	rowctx->curr_operand->prio = ptable[rowctx->curr_operand->operator];
	set_operand_parlevel( rowctx);
        break;

      case K_ACTION_OR:
        if ( rowctx->state == K_STATE_NUM ||
             rowctx->state == K_STATE_VAR)
	{
	  sts = operand_found( rowctx);
	  if ( EVEN(sts)) break;
	}
	
	if ( rowctx->last_type != K_TYPE_OPERAND)
        {
          rowctx->state = K_STATE_ERROR;
	  strcpy( rowctx->msg, "Syntax error");
	  break;
        }

	else if ( rowctx->line[rowctx->pos+1] == '|')
	{
	  rowctx->curr_operand->operator = K_ACTION_OR;
	  rowctx->pos++;
	  rowctx->pos++;
	}
	else
	{
	  rowctx->curr_operand->operator = K_ACTION_BITOR;
	  rowctx->pos++;
	}
	rowctx->last_type = K_TYPE_OPERATOR;
        rowctx->state = K_STATE_OPERATOR;
	rowctx->curr_operand->prio = ptable[rowctx->curr_operand->operator];
	set_operand_parlevel( rowctx);
        break;

      case K_ACTION_END:
        if ( rowctx->state == K_STATE_VAR ||
             rowctx->state == K_STATE_NUM)
	  /* Operand found */
	{
	  sts = operand_found( rowctx);
	  if ( EVEN(sts)) break;
	}
	
	set_operand_parlevel( rowctx);
        rowctx->state = K_STATE_END;
        break;

      case K_ACTION_DELIM:
        if ( rowctx->state == K_STATE_VAR ||
             rowctx->state == K_STATE_NUM)
	  /* Operand found */
	{
	  sts = operand_found( rowctx);
	  if ( EVEN(sts)) break;
	}
	
        if ( rowctx->state != K_STATE_NUM_NEG)
          rowctx->state = K_STATE_DELIM;
        rowctx->pos++;
        break;

      default:
        rowctx->pos++;
    }
    if ( rowctx->state == K_STATE_ERROR ||
         rowctx->state == K_STATE_END)
      break;
  }
  if ( rowctx->state == K_STATE_ERROR)
  {
    sprintf( msg, "%s, line %d pos %d \"", rowctx->msg, 
	filectx->current_row, rowctx->pos - 1); 
    strncat( msg, rowctx->line, sizeof(msg)-strlen(msg)-1);
    msg[sizeof(msg)-2] = 0;
    strcat(msg,"\"");
    (filectx->errormessage_func)(msg, 0, filectx->client_data);
    ccm_free_list( rowctx->list);
    return CCM__EXPRESSION;
  }
  *list = rowctx->list;
  free( (char *) rowctx);
  return 1;
}

int ccm_operate_exec(
  ccm_tFuncCtx	funcctx,
  ccm_sOperand 	*op,
  ccm_sOperand 	*next)
{
  int	next_decl;
  ccm_tInt	next_int;
  ccm_tFloat	next_float;
  ccm_tString	next_string;
  ccm_tString	tmp_str;
  int	sts;
  ccm_sOperand 	*op_arg;
  ccm_sArg		*arg_list;
  ccm_sArg		*arg_p, *a_p, *next_arg;
  int		arg_count;
  int		exit_function_found;


  exit_function_found = 0;
  if ( next == NULL)
  {
    if ( !op->result_done)
    {
      if ( op->value_decl == K_DECL_FLOAT)
        op->result_float = op->value_float;
      else if ( op->value_decl == K_DECL_INT)
        op->result_int = op->value_int;
      else
        strcpy( op->result_string, op->value_string);
      op->result_decl = op->value_decl;
    }
  }
  else
  {
    if ( op->type == K_OPERAND_NO)
    {
      if ( !next->result_done)
      {
        if ( next->type == K_OPERAND_NAME )
        {
          sts = ccm_getvar( funcctx, next->name, &next->value_decl, 
		&next->value_float, &next->value_int, 
		next->value_string);
          if ( EVEN(sts)) return sts;
        }
        if ( next->value_decl == K_DECL_FLOAT)
          next->result_float = next->value_float;
        else if ( next->value_decl == K_DECL_INT)
          next->result_int = next->value_int;
        else
          strcpy( next->result_string, next->value_string);
        next->result_decl = next->value_decl;
      }
    }
    else if ( op->type == K_OPERAND_NAME && !op->result_done)
    {
      sts = ccm_getvar( funcctx, op->name, &op->value_decl, &op->value_float, 
		&op->value_int, op->value_string);
      if ( EVEN(sts)) return sts;
    }
    else if ( op->type == K_OPERAND_FUNCTION)
    {
      /* Create an argument list */
      arg_list = 0;
      arg_count = 0;
      for ( op_arg = op->next; op_arg; op_arg = op_arg->next)
      {
        if ( op_arg->type == K_OPERAND_NO &&
             op_arg->parlevel == op->parlevel - 1)
          break;
        if ( op_arg->type == K_OPERAND_NAME)
        {
          sts = ccm_getvar( funcctx, op_arg->name, &op_arg->value_decl, 
		&op_arg->value_float, &op_arg->value_int, op_arg->value_string);
          if ( EVEN(sts)) return sts;
        }
        arg_p = calloc( 1, sizeof( ccm_sArg));
        if ( op_arg->value_decl == K_DECL_INT)
          arg_p->value_int = op_arg->value_int;
        else if ( op_arg->value_decl == K_DECL_FLOAT)
          arg_p->value_float = op_arg->value_float;
        else if ( op_arg->value_decl == K_DECL_STRING)
          strcpy( arg_p->value_string, op_arg->value_string);
        arg_p->value_decl = op_arg->value_decl;
	arg_p->value_type = op_arg->type;
	strcpy( arg_p->value_name, op_arg->name);
        if ( arg_list == 0)
          arg_list = arg_p;
        else
        {
          for ( a_p = arg_list; a_p->next; a_p = a_p->next) ;
          a_p->next = arg_p;
        }
        arg_count++;
        op_arg->type = K_OPERAND_NO;
        if ( op_arg->parlevel == op->parlevel - 1)
          break;
      }

      sts = ccm_function_exec( funcctx->filectx, op->name, arg_list, arg_count,  
	&op->result_decl, &op->result_float, &op->result_int, 
	op->result_string, 0);

      /* Set returned argument values and free argumentlist */
      for ( arg_p = arg_list; arg_p; arg_p = next_arg)
      {
        if ( arg_p->value_type == K_OPERAND_NAME &&
	     arg_p->value_returned)
	{
          sts = ccm_setvar( funcctx, arg_p->value_name, arg_p->value_decl, 
		arg_p->value_float, arg_p->value_int, arg_p->value_string);
	  if ( EVEN(sts)) return sts;
        }
        next_arg = arg_p->next;
        free( (char *)arg_p);
      }

      if ( EVEN(sts)) return sts;
      if ( sts == CCM__EXITFUNC) exit_function_found = 1;
    }

    if ( next->type == K_OPERAND_NAME && !next->result_done &&
         op->type != K_OPERAND_DECL)
    {
      sts = ccm_getvar( funcctx, next->name, &next->value_decl, &next->value_float, 
		&next->value_int, next->value_string);
      if ( EVEN(sts)) return sts;
    }
    if ( next->result_done)
    {
      /* Use result, not value for next-operand */
      next_decl = next->result_decl;
      next_int = next->result_int;
      next_float = next->result_float;
      strcpy( next_string, next->result_string);
    }
    else
    {
      /* Use value for next-operand */
      next_decl = next->value_decl;
      next_int = next->value_int;
      next_float = next->value_float;
      strcpy( next_string, next->value_string);
    }

    if ( next->type == K_OPERAND_NO && !next->result_done)
    {
      next->result_float = op->result_float;
      next->result_int = op->result_int;
      strcpy( next->result_string, op->result_string);
      next->result_decl = op->result_decl;
    }
    else if ( next_decl == K_DECL_UNKNOWN)
    {
      next_decl = op->value_decl;
    }

    switch ( op->operator)
    {
      case K_ACTION_EQL:
        sts = ccm_setvar( funcctx, op->name, next_decl, next_float, next_int, 
		next_string);
        if ( EVEN(sts)) return sts;
        break;
      case K_ACTION_RETURN:
        if ( !next->result_done)
       {
          next->result_decl = next_decl;
         if ( next_decl == K_DECL_INT)
            next->result_int = next_int;
          else if ( next_decl == K_DECL_FLOAT)
            next->result_float = next_float;
          else if ( next_decl == K_DECL_STRING)
            strcpy( next->result_string, next_string);
          break;
        }
      case K_ACTION_MUL:
        if ( op->result_decl == K_DECL_INT && next_decl == K_DECL_INT)
        {
          next->result_int = op->result_int * next_int;
          next->result_decl = K_DECL_INT;
        }
        else if ( op->result_decl == K_DECL_FLOAT && next_decl == K_DECL_INT)
        {
          next->result_float = op->result_float * next_int;
          next->result_decl = K_DECL_FLOAT;
        }
        else if ( op->result_decl == K_DECL_INT && next_decl == K_DECL_FLOAT)
        {
          next->result_float = op->result_int * next_float;
          next->result_decl = K_DECL_FLOAT;
        }
        else if ( op->result_decl == K_DECL_FLOAT && next_decl == K_DECL_FLOAT)
        {
          next->result_float = op->result_float * next_float;
          next->result_decl = K_DECL_FLOAT;
        }
        break;
      case K_ACTION_DIV:
        if ( op->result_decl == K_DECL_INT && next_decl == K_DECL_INT)
        {
          next->result_int = op->result_int / next_int;
          next->result_decl = K_DECL_INT;
        }
        else if ( op->result_decl == K_DECL_FLOAT && next_decl == K_DECL_INT)
        {
          next->result_float = op->result_float / next_int;
          next->result_decl = K_DECL_FLOAT;
        }
        else if ( op->result_decl == K_DECL_INT && next_decl == K_DECL_FLOAT)
        {
          next->result_float = op->result_int / next_float;
          next->result_decl = K_DECL_FLOAT;
        }
        else if ( op->result_decl == K_DECL_FLOAT && next_decl == K_DECL_FLOAT)
        {
          next->result_float = op->result_float / next_float;
          next->result_decl = K_DECL_FLOAT;
        }
        break;
      case K_ACTION_ADD:
        if ( op->result_decl == K_DECL_INT && next_decl == K_DECL_INT)
        {
          next->result_int = op->result_int + next_int;
          next->result_decl = K_DECL_INT;
        }
        else if ( op->result_decl == K_DECL_FLOAT && next_decl == K_DECL_INT)
        {
          next->result_float = op->result_float + next_int;
          next->result_decl = K_DECL_FLOAT;
        }
        else if ( op->result_decl == K_DECL_INT && next_decl == K_DECL_FLOAT)
        {
          next->result_float = op->result_int + next_float;
          next->result_decl = K_DECL_FLOAT;
        }
        else if ( op->result_decl == K_DECL_FLOAT && next_decl == K_DECL_FLOAT)
        {
          next->result_float = op->result_float + next_float;
          next->result_decl = K_DECL_FLOAT;
        }
        else if ( op->result_decl == K_DECL_STRING && next_decl == K_DECL_STRING)
        {
          strcpy( next->result_string, op->result_string);
          strncat( next->result_string, next_string, K_STRING_SIZE);
          next->result_string[K_STRING_SIZE-1] = 0;
          next->result_decl = K_DECL_STRING;
        }
        else if ( op->result_decl == K_DECL_INT && next_decl == K_DECL_STRING)
        {
          sprintf( next->result_string + strlen(next->result_string), 
 		ccm_cIntFormat, op->result_int);
          strncat( next->result_string, next_string, K_STRING_SIZE);
          next->result_string[K_STRING_SIZE-1] = 0;
          next->result_decl = K_DECL_STRING;
        }
        else if ( op->result_decl == K_DECL_STRING && next_decl == K_DECL_INT)
        {
          strcpy( next->result_string, op->result_string);
          sprintf( next->result_string + strlen(next->result_string), 
		ccm_cIntFormat, next_int);
          next->result_decl = K_DECL_STRING;
        }
        else if ( op->result_decl == K_DECL_FLOAT && next_decl == K_DECL_STRING)
        {
	  ccm_float_to_string( tmp_str, op->result_float);
	  strcpy( next->result_string, tmp_str);
          strncat( next->result_string, next_string, K_STRING_SIZE);
          next->result_string[K_STRING_SIZE-1] = 0;
          next->result_decl = K_DECL_STRING;
        }
        else if ( op->result_decl == K_DECL_STRING && next_decl == K_DECL_FLOAT)
        {
          strcpy( next->result_string, op->result_string);
	  ccm_float_to_string( tmp_str, next_float);
          strncat( next->result_string, tmp_str, K_STRING_SIZE);
          next->result_string[K_STRING_SIZE-1] = 0;
          next->result_decl = K_DECL_STRING;
        }
        break;
      case K_ACTION_EQLADD:
        if ( op->result_decl == K_DECL_INT && next_decl == K_DECL_INT)
        {
          next->result_int = op->result_int + next_int;
          next->result_decl = K_DECL_INT;
        }
        else if ( op->result_decl == K_DECL_FLOAT && next_decl == K_DECL_INT)
        {
          next->result_float = op->result_float + next_int;
          next->result_decl = K_DECL_FLOAT;
        }
        else if ( op->result_decl == K_DECL_INT && next_decl == K_DECL_FLOAT)
        {
          next->result_float = op->result_int + next_float;
          next->result_decl = K_DECL_FLOAT;
        }
        else if ( op->result_decl == K_DECL_FLOAT && next_decl == K_DECL_FLOAT)
        {
          next->result_float = op->result_float + next_float;
          next->result_decl = K_DECL_FLOAT;
        }
        else if ( op->result_decl == K_DECL_STRING && next_decl == K_DECL_STRING)
        {
          strcpy( next->result_string, op->result_string);
          strncat( next->result_string, next_string, K_STRING_SIZE);
          next->result_string[K_STRING_SIZE-1] = 0;
          next->result_decl = K_DECL_STRING;
        }
        else if ( op->result_decl == K_DECL_INT && next_decl == K_DECL_STRING)
        {
          sprintf( next->result_string + strlen(next->result_string), 
 		ccm_cIntFormat, op->result_int);
          strncat( next->result_string, next_string, K_STRING_SIZE);
          next->result_string[K_STRING_SIZE-1] = 0;
          next->result_decl = K_DECL_STRING;
        }
        else if ( op->result_decl == K_DECL_STRING && next_decl == K_DECL_INT)
        {
          strcpy( next->result_string, op->result_string);
          sprintf( next->result_string + strlen(next->result_string), 
		ccm_cIntFormat, next_int);
          next->result_decl = K_DECL_STRING;
        }
        else if ( op->result_decl == K_DECL_FLOAT && next_decl == K_DECL_STRING)
        {
	  ccm_float_to_string( tmp_str, op->result_float);
          strcpy( next->result_string, tmp_str);
          strncat( next->result_string, next_string, K_STRING_SIZE);
          next->result_string[K_STRING_SIZE-1] = 0;
          next->result_decl = K_DECL_STRING;
        }
        else if ( op->result_decl == K_DECL_STRING && next_decl == K_DECL_FLOAT)
        {
          strcpy( next->result_string, op->result_string);
	  ccm_float_to_string( tmp_str, next_float);
          strncat( next->result_string, tmp_str, K_STRING_SIZE);
          next->result_string[K_STRING_SIZE-1] = 0;
          next->result_decl = K_DECL_STRING;
        }
        sts = ccm_setvar( funcctx, op->name, next->result_decl, 
		next->result_float, next->result_int, next->result_string);
        if ( EVEN(sts)) return sts;
        break;
      case K_ACTION_EQLSUB:
        if ( op->result_decl == K_DECL_INT && next_decl == K_DECL_INT)
        {
          next->result_int = op->result_int - next_int;
          next->result_decl = K_DECL_INT;
        }
        else if ( op->result_decl == K_DECL_FLOAT && next_decl == K_DECL_INT)
        {
          next->result_float = op->result_float - next_int;
          next->result_decl = K_DECL_FLOAT;
        }
        else if ( op->result_decl == K_DECL_INT && next_decl == K_DECL_FLOAT)
        {
          next->result_float = op->result_int - next_float;
          next->result_decl = K_DECL_FLOAT;
        }
        else if ( op->result_decl == K_DECL_FLOAT && next_decl == K_DECL_FLOAT)
        {
          next->result_float = op->result_float - next_float;
          next->result_decl = K_DECL_FLOAT;
        }
        sts = ccm_setvar( funcctx, op->name, next->result_decl,
		next->result_float, next->result_int, next->result_string);
        if ( EVEN(sts)) return sts;
        break;
      case K_ACTION_SUB:
        if ( op->result_decl == K_DECL_INT && next_decl == K_DECL_INT)
        {
          next->result_int = op->result_int - next_int;
          next->result_decl = K_DECL_INT;
        }
        else if ( op->result_decl == K_DECL_FLOAT && next_decl == K_DECL_INT)
        {
          next->result_float = op->result_float - next_int;
          next->result_decl = K_DECL_FLOAT;
        }
        else if ( op->result_decl == K_DECL_INT && next_decl == K_DECL_FLOAT)
        {
          next->result_float = op->result_int - next_float;
          next->result_decl = K_DECL_FLOAT;
        }
        else if ( op->result_decl == K_DECL_FLOAT && next_decl == K_DECL_FLOAT)
        {
          next->result_float = op->result_float - next_float;
          next->result_decl = K_DECL_FLOAT;
        }
        break;
      case K_ACTION_AND:
        if ( op->result_decl == K_DECL_INT && next_decl == K_DECL_INT)
        {
          next->result_int = op->result_int && next_int;
          next->result_decl = K_DECL_INT;
        }
        else if ( op->result_decl == K_DECL_FLOAT && next_decl == K_DECL_INT)
        {
          next->result_float = op->result_float && next_int;
          next->result_decl = K_DECL_FLOAT;
        }
        else if ( op->result_decl == K_DECL_INT && next_decl == K_DECL_FLOAT)
        {
          next->result_float = op->result_int && next_float;
          next->result_decl = K_DECL_FLOAT;
        }
        else if ( op->result_decl == K_DECL_FLOAT && next_decl == K_DECL_FLOAT)
        {
          next->result_float = op->result_float && next_float;
          next->result_decl = K_DECL_FLOAT;
        }
        break;
      case K_ACTION_OR:
        if ( op->result_decl == K_DECL_INT && next_decl == K_DECL_INT)
        {
          next->result_int = op->result_int || next_int;
          next->result_decl = K_DECL_INT;
        }
        else if ( op->result_decl == K_DECL_FLOAT && next_decl == K_DECL_INT)
        {
          next->result_float = op->result_float || next_int;
          next->result_decl = K_DECL_FLOAT;
        }
        else if ( op->result_decl == K_DECL_INT && next_decl == K_DECL_FLOAT)
        {
          next->result_float = op->result_int || next_float;
          next->result_decl = K_DECL_FLOAT;
        }
        else if ( op->result_decl == K_DECL_FLOAT && next_decl == K_DECL_FLOAT)
        {
          next->result_float = op->result_float || next_float;
          next->result_decl = K_DECL_FLOAT;
        }
        break;
      case K_ACTION_EQ:
        if ( op->result_decl == K_DECL_INT && next_decl == K_DECL_INT)
        {
          next->result_int = ( op->result_int == next_int);
          next->result_decl = K_DECL_INT;
        }
        else if ( op->result_decl == K_DECL_FLOAT && next_decl == K_DECL_INT)
        {
          next->result_int = ( fabs( op->result_float - next_int) < FLT_EPSILON);
          next->result_decl = K_DECL_INT;
        }
        else if ( op->result_decl == K_DECL_INT && next_decl == K_DECL_FLOAT)
        {
          next->result_int = ( fabs( op->result_int - next_float) < FLT_EPSILON);
          next->result_decl = K_DECL_INT;
        }
        else if ( op->result_decl == K_DECL_FLOAT && next_decl == K_DECL_FLOAT)
        {
          next->result_int = ( fabs( op->result_float - next_float) < FLT_EPSILON);
          next->result_decl = K_DECL_INT;
        }
        else if ( op->result_decl == K_DECL_STRING && next_decl == K_DECL_STRING)
        {
          next->result_int = ( strcmp( op->result_string, next_string) == 0);
          next->result_decl = K_DECL_INT;
        }
        break;
      case K_ACTION_NE:
        if ( op->result_decl == K_DECL_INT && next_decl == K_DECL_INT)
        {
          next->result_int = ( op->result_int != next_int);
          next->result_decl = K_DECL_INT;
        }
        else if ( op->result_decl == K_DECL_FLOAT && next_decl == K_DECL_INT)
        {
          next->result_int = !( fabs( op->result_float - next_int) < FLT_EPSILON);
          next->result_decl = K_DECL_INT;
        }
        else if ( op->result_decl == K_DECL_INT && next_decl == K_DECL_FLOAT)
        {
          next->result_int = !( fabs( op->result_int - next_float) < FLT_EPSILON);
          next->result_decl = K_DECL_INT;
        }
        else if ( op->result_decl == K_DECL_FLOAT && next_decl == K_DECL_FLOAT)
        {
          next->result_int = !( fabs( op->result_float - next_float) < FLT_EPSILON);
          next->result_decl = K_DECL_INT;
        }
        else if ( op->result_decl == K_DECL_STRING && next_decl == K_DECL_STRING)
        {
          next->result_int = !( strcmp( op->result_string, next_string) == 0);
          next->result_decl = K_DECL_INT;
        }
        break;
      case K_ACTION_LT:
        if ( op->result_decl == K_DECL_INT && next_decl == K_DECL_INT)
        {
          next->result_int = ( op->result_int < next_int);
          next->result_decl = K_DECL_INT;
        }
        else if ( op->result_decl == K_DECL_FLOAT && next_decl == K_DECL_INT)
        {
          next->result_int = ( op->result_float < next_int);
          next->result_decl = K_DECL_INT;
        }
        else if ( op->result_decl == K_DECL_INT && next_decl == K_DECL_FLOAT)
        {
          next->result_int = ( op->result_int < next_float);
          next->result_decl = K_DECL_INT;
        }
        else if ( op->result_decl == K_DECL_FLOAT && next_decl == K_DECL_FLOAT)
        {
          next->result_int = ( op->result_float < next_float);
          next->result_decl = K_DECL_INT;
        }
        break;
      case K_ACTION_GT:
        if ( op->result_decl == K_DECL_INT && next_decl == K_DECL_INT)
        {
          next->result_int = ( op->result_int > next_int);
          next->result_decl = K_DECL_INT;
        }
        else if ( op->result_decl == K_DECL_FLOAT && next_decl == K_DECL_INT)
        {
          next->result_int = ( op->result_float > next_int);
          next->result_decl = K_DECL_INT;
        }
        else if ( op->result_decl == K_DECL_INT && next_decl == K_DECL_FLOAT)
        {
          next->result_int = ( op->result_int > next_float);
          next->result_decl = K_DECL_INT;
        }
        else if ( op->result_decl == K_DECL_FLOAT && next_decl == K_DECL_FLOAT)
        {
          next->result_int = ( op->result_float > next_float);
          next->result_decl = K_DECL_INT;
        }
        break;
      case K_ACTION_LE:
        if ( op->result_decl == K_DECL_INT && next_decl == K_DECL_INT)
        {
          next->result_int = ( op->result_int <= next_int);
          next->result_decl = K_DECL_INT;
        }
        else if ( op->result_decl == K_DECL_FLOAT && next_decl == K_DECL_INT)
        {
          next->result_int = ( op->result_float <= next_int);
          next->result_decl = K_DECL_INT;
        }
        else if ( op->result_decl == K_DECL_INT && next_decl == K_DECL_FLOAT)
        {
          next->result_int = ( op->result_int <= next_float);
          next->result_decl = K_DECL_INT;
        }
        else if ( op->result_decl == K_DECL_FLOAT && next_decl == K_DECL_FLOAT)
        {
          next->result_int = ( op->result_float <= next_float);
          next->result_decl = K_DECL_INT;
        }
        break;
      case K_ACTION_GE:
        if ( op->result_decl == K_DECL_INT && next_decl == K_DECL_INT)
        {
          next->result_int = ( op->result_int >= next_int);
          next->result_decl = K_DECL_INT;
        }
        else if ( op->result_decl == K_DECL_FLOAT && next_decl == K_DECL_INT)
        {
          next->result_int = ( op->result_float >= next_int);
          next->result_decl = K_DECL_INT;
        }
        else if ( op->result_decl == K_DECL_INT && next_decl == K_DECL_FLOAT)
        {
          next->result_int = ( op->result_int >= next_float);
          next->result_decl = K_DECL_INT;
        }
        else if ( op->result_decl == K_DECL_FLOAT && next_decl == K_DECL_FLOAT)
        {
          next->result_int = ( op->result_float >= next_float);
          next->result_decl = K_DECL_INT;
        }
        break;
      case K_ACTION_INV:
        if ( next_decl == K_DECL_INT)
        {
          next->result_int = ! next_int;
          next->result_decl = K_DECL_INT;
        }
        else
        {
          next->result_float = next_float;
          next->result_decl = K_DECL_FLOAT;
        }
        break;
      case K_ACTION_BITAND:
        if ( op->result_decl == K_DECL_INT && next_decl == K_DECL_INT)
        {
          next->result_int = op->result_int & next_int;
          next->result_decl = K_DECL_INT;
        }
        break;
      case K_ACTION_BITOR:
        if ( op->result_decl == K_DECL_INT && next_decl == K_DECL_INT)
        {
          next->result_int = op->result_int | next_int;
          next->result_decl = K_DECL_INT;
        }
        break;
      case K_ACTION_LSHIFT:
        if ( op->result_decl == K_DECL_INT && next_decl == K_DECL_INT)
        {
          next->result_int = op->result_int << next_int;
          next->result_decl = K_DECL_INT;
        }
        break;
      case K_ACTION_RSHIFT:
        if ( op->result_decl == K_DECL_INT && next_decl == K_DECL_INT)
        {
          next->result_int = op->result_int >> next_int;
          next->result_decl = K_DECL_INT;
        }
        break;
      case K_ACTION_CREALOCINT:
        sts = ccm_createvar( next->name, K_DECL_INT, 0, 0, NULL,
		&funcctx->locint_list, &funcctx->locfloat_list, &funcctx->locstring_list);
        if ( EVEN(sts)) return sts;
        break;
      case K_ACTION_CREALOCFLOAT:
        sts = ccm_createvar( next->name, K_DECL_FLOAT, 0, 0, NULL,
		&funcctx->locint_list, &funcctx->locfloat_list, &funcctx->locstring_list);
        if ( EVEN(sts)) return sts;
        break;
      case K_ACTION_CREALOCSTRING:
        sts = ccm_createvar( next->name, K_DECL_STRING, 0, 0, "",
		&funcctx->locint_list, &funcctx->locfloat_list, &funcctx->locstring_list);
        if ( EVEN(sts)) return sts;
        break;
      case K_ACTION_CREAGBLINT:
        sts = ccm_createvar( next->name, K_DECL_INT, 0, 0, NULL,
		&funcctx->filectx->gblint_list, &funcctx->filectx->gblfloat_list, &funcctx->filectx->gblstring_list);
        if ( EVEN(sts)) return sts;
        break;
      case K_ACTION_CREAGBLFLOAT:
        sts = ccm_createvar( next->name, K_DECL_FLOAT, 0, 0, NULL,
		&funcctx->filectx->gblint_list, &funcctx->filectx->gblfloat_list, &funcctx->filectx->gblstring_list);
        if ( EVEN(sts)) return sts;
        break;
      case K_ACTION_CREAGBLSTRING:
        sts = ccm_createvar( next->name, K_DECL_STRING, 0, 0, "",
		&funcctx->filectx->gblint_list, &funcctx->filectx->gblfloat_list, &funcctx->filectx->gblstring_list);
        if ( EVEN(sts)) return sts;
        break;
      case K_ACTION_CREAEXTINT:
        sts = ccm_createvar( next->name, K_DECL_INT, 0, 0, NULL,
		&extint_list, &extfloat_list, &extstring_list);
	if ( sts == CCM__VARALREXIST)
	  sts = CCM__SUCCESS;
        if ( EVEN(sts)) return sts;
        break;
      case K_ACTION_CREAEXTFLOAT:
        sts = ccm_createvar( next->name, K_DECL_FLOAT, 0, 0, NULL,
		&extint_list, &extfloat_list, &extstring_list);
	if ( sts == CCM__VARALREXIST)
	  sts = CCM__SUCCESS;
        if ( EVEN(sts)) return sts;
        break;
      case K_ACTION_CREAEXTSTRING:
        sts = ccm_createvar( next->name, K_DECL_STRING, 0, 0, "",
		&extint_list, &extfloat_list, &extstring_list);
	if ( sts == CCM__VARALREXIST)
	  sts = CCM__SUCCESS;
        if ( EVEN(sts)) return sts;
        break;
      case K_ACTION_DELETE:
        sts = ccm_deletevar( next->name,
		&extint_list, &extfloat_list, &extstring_list);
        if ( EVEN(sts)) return sts;
        break;
    }
    next->result_done = 1;
    if ( ccm_testmode)
      printf(
"     Processing \"%s\"	\"%s\"	decl: %d, float: %f, int: " ccm_cIntFormat " str: \"%s\"\n", 
			op->name, next->name, 
			next->result_decl, next->result_float, 
			next->result_int, next->result_string);

  }
  op->done = 1;

  if ( op->local_type == K_LOCTYPE_AFTER)
  {
    switch ( op->local_operator)
    {
      case K_ACTION_INCR:
        if ( op->value_decl == K_DECL_INT || op->value_decl == K_DECL_FLOAT)
        {
          op->value_int++;
          op->value_float++;
          sts = ccm_setvar( funcctx, op->name, op->value_decl, op->value_float, 
		op->value_int, "");
          if ( EVEN(sts)) return sts;
	}
        break;
      case K_ACTION_DECR:
        if ( op->value_decl == K_DECL_INT || op->value_decl == K_DECL_FLOAT)
        {
          op->value_int--;
          op->value_float--;
          sts = ccm_setvar( funcctx, op->name, op->value_decl, op->value_float, 
		op->value_int, "");
          if ( EVEN(sts)) return sts;
	}
        break;
    }
  }
  if ( exit_function_found)
    return CCM__EXITFUNC;
  return 1;
}

int	ccm_execute_list( 
  ccm_tFuncCtx	funcctx,
  ccm_sOperand *list,
  int	*result_decl,
  ccm_tFloat	*result_float,
  ccm_tInt	*result_int,
  char		*result_string
)
{
  int		sts;
  ccm_sOperand	*op;
  ccm_sOperand	*bp;
  int		exit_func_found;


  exit_func_found = 0;
  for ( op = funcctx->list; op; op = op->next)
  {
    if ( !op->prev)
    {
      /* First in list */
      if ( op->type == K_OPERAND_NAME)
      {
        sts = ccm_getvar( funcctx, op->name, &op->value_decl, 
			&op->value_float, &op->value_int, op->value_string);
        if ( EVEN(sts)) return sts;
      }
      if ( op->value_decl == K_DECL_FLOAT)
        op->result_float = op->value_float;
      else if ( op->value_decl == K_DECL_INT)
        op->result_int = op->value_int;
      else
        strcpy( op->result_string, op->value_string);
      op->result_decl = op->value_decl;
      op->result_done = 1;
      if ( !op->next)
      {
        sts = ccm_operate_exec( funcctx, op, NULL);
        if ( EVEN(sts)) return sts;
        if ( sts == CCM__EXITFUNC) exit_func_found = 1;
      }
    }
    if ( !op->next)
    {
      /* Last in list */
      for ( bp = op->prev; bp; bp = bp->prev)
      {
        if ( !bp->done)
        {
          sts = ccm_operate_exec( funcctx, bp, op);
          if ( EVEN(sts)) return sts;
          if ( sts == CCM__EXITFUNC) exit_func_found = 1;
        }
      }
      *result_decl = op->result_decl;
      if ( op->result_decl == K_DECL_FLOAT)
        *result_float = op->result_float;
      else if ( op->result_decl == K_DECL_INT)
        *result_int = op->result_int;
      else if ( op->result_decl == K_DECL_STRING)
        strcpy( result_string, op->result_string);
    }
    else
    {
      if ( op->next->parlevel > op->parlevel)
      {
        /* Left parenthesis */
        if ( op->next->prio < op->prio)
	{
          for ( bp = op->prev; bp; bp = bp->prev)
          {
            if ( bp->parlevel < op->parlevel)
              break;
            if ( bp->parlevel > op->parlevel)
              continue;
            if ( bp->prio <= op->prio && !bp->done)
            {
              sts = ccm_operate_exec( funcctx, bp, op);
              if ( EVEN(sts)) return sts;
              if ( sts == CCM__EXITFUNC) exit_func_found = 1;
            }
          }
        }
        continue;
      }
      else if ( op->next->parlevel < op->parlevel)
      {
        /* Right parentheseis, sum sinc last right parenthes */
        for ( bp = op; bp; bp = bp->prev)
        {
          if ( bp->parlevel < op->parlevel)
            break;
          if ( bp->parlevel > op->parlevel)
            continue;
          if ( !bp->done)
          {
            sts = ccm_operate_exec( funcctx, bp, op->next);
            if ( EVEN(sts)) return sts;
            if ( sts == CCM__EXITFUNC) exit_func_found = 1;
          }
        }

      }
      else
      {
        if ( op->next->prio < op->prio)
	{
          for ( bp = op->prev; bp; bp = bp->prev)
          {
            if ( bp->parlevel < op->parlevel)
              break;
            if ( bp->parlevel > op->parlevel)
              continue;
            if ( bp->prio <= op->prio && !bp->done)
            {
              sts = ccm_operate_exec( funcctx, bp, op);
              if ( EVEN(sts)) return sts;
              if ( sts == CCM__EXITFUNC) exit_func_found = 1;
            }
          }
	  if ( op->next->type == K_OPERAND_NAME)
          {
            sts = ccm_getvar( funcctx, op->next->name, &op->next->value_decl, 
			&op->next->value_float, &op->next->value_int,
			op->next->value_string);
            if ( EVEN(sts)) return sts;
          }
          if ( op->next->value_decl == K_DECL_FLOAT)
            op->next->result_float = op->next->value_float;
          else if ( op->next->value_decl == K_DECL_INT)
            op->next->result_int = op->next->value_int;
          else if ( op->next->value_decl == K_DECL_STRING)
            strcpy( op->next->result_string, op->next->value_string);
          op->next->result_decl = op->next->value_decl;
          op->next->result_done = 1;
          continue;
        }
	else
        {
          sts = ccm_operate_exec( funcctx, op, op->next);
          if ( EVEN(sts)) return sts;
          if ( sts == CCM__EXITFUNC) exit_func_found = 1;
        }
      }
    }

  }
  if ( exit_func_found)
    return CCM__EXITFUNC;
  return 1;
}

int ccm_line_exec( 
  ccm_tFuncCtx	funcctx,
  char 		*line, 
  int		*result_decl,
  ccm_tFloat	*result_float,
  ccm_tInt	*result_int,
  char		*result_string
)
{
  int		sts;
  ccm_sOperand	*list;
  ccm_sOperand	*op;

  sts = ccm_create_list( funcctx->filectx, line, &list);
  if ( EVEN(sts)) return sts;

  funcctx->list = list;

  for ( op = list; op; op = op->next)
  {
    if ( ccm_testmode)
      printf( "operator %d prio %d   parlevel %d Name %s\n", 
		op->operator, op->prio, op->parlevel, 
		op->name);
  }

  sts = ccm_execute_list( funcctx, list, result_decl, result_float, result_int, 
	result_string);
  ccm_free_list( list);
  if ( EVEN(sts) || (sts == CCM__EXITFUNC)) return sts;

  return 1;
}


static void ccm_free_varlists(
  ccm_sIntvar		*int_list,
  ccm_sFloatvar		*float_list,
  ccm_sStringvar	*string_list
)
{
  ccm_sIntvar		*int_p,  *next_int;
  ccm_sFloatvar		*float_p, *next_float;
  ccm_sStringvar	*string_p, *next_string;

  for ( int_p = int_list; int_p; int_p = next_int)
  {
    next_int = int_p->next;
    free( (char *) int_p->value);
    free( (char *) int_p);
  }
  for ( float_p = float_list; float_p; float_p = next_float)
  {
    next_float = float_p->next;
    free( (char *) float_p->value);
    free( (char *) float_p);
  }
  for ( string_p = string_list; string_p; string_p = next_string)
  {
    next_string = string_p->next;
    free( (char *) string_p->value);
    free( (char *) string_p);
  }
}

static int ccm_getvar( 
  ccm_tFuncCtx		funcctx,
  const char	       	*name,
  int			*decl,
  ccm_tFloat	       	*value_float,
  ccm_tInt	       	*value_int,
  char		       	*value_string)
{
  ccm_sIntvar		*int_p;
  ccm_sFloatvar	*float_p;
  ccm_sStringvar	*string_p;
  ccm_sIntvar		*int_list;
  ccm_sFloatvar	*float_list;
  ccm_sStringvar	*string_list;
  int			found;
  int			i;
  int			sts;
  int			array;
  int			element;
  char			varname[80];

  sts = ccm_varname_parse( funcctx, name, varname, &array, &element);
  if ( EVEN(sts)) return sts;

  /* Search int */
  found = 0;
  for ( i = 0; i < 3; i++)
  {
    if ( i == 0)
    {
      int_list = funcctx->locint_list;
      float_list = funcctx->locfloat_list;
      string_list = funcctx->locstring_list;
    }
    else if ( i == 1)
    {
      int_list = funcctx->filectx->gblint_list;
      float_list = funcctx->filectx->gblfloat_list;
      string_list = funcctx->filectx->gblstring_list;
    }
    else
    {
      int_list = extint_list;
      float_list = extfloat_list;
      string_list = extstring_list;
    }

    for ( int_p = int_list; int_p; int_p = int_p->next)
    {
      if ( strcmp( int_p->name, varname) == 0)
      {
        found = 1;
        break;
      }
    }
    if ( found)
    {
      if ( int_p->array != array)
        return CCM__VARNOTFOUND;
      if ( int_p->array && ( element < 0 || element >= int_p->elements))
        return CCM__ARRAYBOUNDS;

      *value_int = *(int_p->value + element);
      *decl = K_DECL_INT;
    }
    else
    {
      /* Search float */
      for ( float_p = float_list; float_p; float_p = float_p->next)
      {
        if ( strcmp( float_p->name, varname) == 0)
        {
          found = 1;
          break;
        }
      }
      if ( found)
      {
        if ( float_p->array != array)
          return CCM__VARNOTFOUND;
        if ( float_p->array && ( element < 0 || element >= float_p->elements))
          return CCM__ARRAYBOUNDS;

        *value_float = *(float_p->value + element);
        *decl = K_DECL_FLOAT;
      }
      else
      {
        /* Search string */
        for ( string_p = string_list; string_p; string_p = string_p->next)
        {
          if ( strcmp( string_p->name, varname) == 0)
          {
            found = 1;
            break;
          }
        }
        if ( found)
        {
          if ( string_p->array != array)
            return CCM__VARNOTFOUND;
          if ( string_p->array && ( element < 0 || element >= string_p->elements))
            return CCM__ARRAYBOUNDS;

          strncpy( value_string, string_p->value + element*K_STRING_SIZE, K_STRING_SIZE);
          *decl = K_DECL_STRING;
        }
      }
    }
    if ( found)
      break;
  }
  if ( !found)
    return CCM__VARNOTFOUND;

  return 1;
}

static int ccm_setvar( 
  ccm_tFuncCtx	funcctx,
  const char   	*name,
  int		decl,
  ccm_tFloat   	value_float,
  ccm_tInt     	value_int,
  char  	*value_string)
{
  ccm_sIntvar		*int_p;
  ccm_sFloatvar	*float_p;
  ccm_sStringvar	*string_p;
  ccm_sIntvar		*int_list;
  ccm_sFloatvar	*float_list;
  ccm_sStringvar	*string_list;
  int			found;
  int			nr;
  float			f_var;
  int			i;
  int			sts;
  int			array;
  int			element;
  char			varname[80];

  sts = ccm_varname_parse( funcctx, name, varname, &array, &element);
  if ( EVEN(sts)) return sts;

  found = 0;
  for ( i = 0; i < 3; i++)
  {
    if ( i == 0)
    {
      int_list = funcctx->locint_list;
      float_list = funcctx->locfloat_list;
      string_list = funcctx->locstring_list;
    }
    else if ( i == 1)
    {
      int_list = funcctx->filectx->gblint_list;
      float_list = funcctx->filectx->gblfloat_list;
      string_list = funcctx->filectx->gblstring_list;
    }
    else
    {
      int_list = extint_list;
      float_list = extfloat_list;
      string_list = extstring_list;
    }

    for ( int_p = int_list; int_p; int_p = int_p->next)
    {
      if ( strcmp( int_p->name, varname) == 0)
      {
        found = 1;
        break;
      }
    }
    if ( found)
    {
      if ( int_p->array != array)
        return CCM__VARNOTFOUND;
      if ( int_p->array && ( element < 0 || element >= int_p->elements))
        return CCM__ARRAYBOUNDS;

      if ( decl == K_DECL_INT)
        *(int_p->value + element) = value_int;
      else if ( decl == K_DECL_FLOAT)
        *(int_p->value + element) = value_float;
      else
      {
        if ( strchr( value_string, '.'))
        {
          nr = sscanf( value_string, "%f", &f_var);
	  if ( nr != 1)
	    return CCM__VARTYPE;
          *(int_p->value + element) = f_var;
        }
        else
        {
          nr = sscanf( value_string, ccm_cIntFormat, int_p->value + element);
	  if ( nr != 1)
	    return CCM__VARTYPE;
        }
      }
    }
    else
    {
      /* Search float */
      for ( float_p = float_list; float_p; float_p = float_p->next)
      {
        if ( strcmp( float_p->name, varname) == 0)
        {
          found = 1;
          break;
        }
      }
      if ( found)
      {
        if ( float_p->array != array)
          return CCM__VARNOTFOUND;
        if ( float_p->array && ( element < 0 || element >= float_p->elements))
          return CCM__ARRAYBOUNDS;

        if ( decl == K_DECL_INT)
          *(float_p->value + element) = value_int;
        else if ( decl == K_DECL_FLOAT)
          *(float_p->value + element) = value_float;
        else
        {
          nr = sscanf( value_string, "%f", float_p->value + element);
    	  if ( nr != 1)
	    return CCM__VARTYPE;
        }
      }
      else
      {
        /* Search string */
        for ( string_p = string_list; string_p; string_p = string_p->next)
        {
          if ( strcmp( string_p->name, varname) == 0)
          {
            found = 1;
            break;
          }
        }
        if ( found)
        {
          if ( string_p->array != array)
            return CCM__VARNOTFOUND;
          if ( string_p->array && ( element < 0 || element >= string_p->elements))
            return CCM__ARRAYBOUNDS;

          if ( decl == K_DECL_INT)
            sprintf( string_p->value + element*K_STRING_SIZE, ccm_cIntFormat, value_int);
          else if ( decl == K_DECL_FLOAT)
	    ccm_float_to_string( string_p->value + element*K_STRING_SIZE, value_float);
          else
            strncpy( string_p->value + element*K_STRING_SIZE, value_string, K_STRING_SIZE);
        }
      }
    }
    if ( found)
      break;
  }
  if ( !found)
    return CCM__VARNOTFOUND;
  return 1;
}

int ccm_set_external_var( 
  const char   	*name,
  int		decl,
  ccm_tFloat   	value_float,
  ccm_tInt     	value_int,
  char  	*value_string)
{
  ccm_sIntvar		*int_p;
  ccm_sFloatvar	*float_p;
  ccm_sStringvar	*string_p;
  ccm_sIntvar		*int_list;
  ccm_sFloatvar	*float_list;
  ccm_sStringvar	*string_list;
  int			found;
  int			sts;
  int			array;
  int			element;
  char			varname[80];

  sts = ccm_varname_parse( NULL, name, varname, &array, &element);
  if ( EVEN(sts)) return sts;

  found = 0;

  if ( decl == K_DECL_INT)
  { 
    int_list = extint_list;

    for ( int_p = int_list; int_p; int_p = int_p->next)
    {
      if ( strcmp( int_p->name, varname) == 0)
      {
        found = 1;
        break;
      }
    }
    if ( found)
      *(int_p->value + element) = value_int;
  }
  else if ( decl == K_DECL_FLOAT)
  { 
    /* Search float */
    float_list = extfloat_list;

    for ( float_p = float_list; float_p; float_p = float_p->next)
    {
      if ( strcmp( float_p->name, varname) == 0)
      {
        found = 1;
        break;
      }
    }
    if ( found)
      *(float_p->value + element) = value_float;
  }
  else if ( decl == K_DECL_STRING)
  { 
    /* Search string */
    string_list = extstring_list;

    for ( string_p = string_list; string_p; string_p = string_p->next)
    {
      if ( strcmp( string_p->name, varname) == 0)
      {
        found = 1;
        break;
      }
    }
    if ( found)
      strncpy( string_p->value + element*K_STRING_SIZE, value_string, K_STRING_SIZE);
  }
  if ( !found)
    return CCM__VARNOTFOUND;
  return 1;
}

int ccm_get_external_var( 
  const char   	*name,
  int		decl,
  ccm_tFloat   	*value_float,
  ccm_tInt     	*value_int,
  char  	*value_string)
{
  ccm_sIntvar		*int_p;
  ccm_sFloatvar	*float_p;
  ccm_sStringvar	*string_p;
  ccm_sIntvar		*int_list;
  ccm_sFloatvar	*float_list;
  ccm_sStringvar	*string_list;
  int			found;
  int			sts;
  int			array;
  int			element;
  char			varname[80];

  sts = ccm_varname_parse( NULL, name, varname, &array, &element);
  if ( EVEN(sts)) return sts;

  found = 0;

  if ( decl == K_DECL_INT)
  { 
    int_list = extint_list;

    for ( int_p = int_list; int_p; int_p = int_p->next)
    {
      if ( strcmp( int_p->name, varname) == 0)
      {
        found = 1;
        break;
      }
    }
    if ( found)
      *value_int = *(int_p->value + element);
  }
  else if ( decl == K_DECL_FLOAT)
  { 
    /* Search float */
    float_list = extfloat_list;

    for ( float_p = float_list; float_p; float_p = float_p->next)
    {
      if ( strcmp( float_p->name, varname) == 0)
      {
        found = 1;
        break;
      }
    }
    if ( found)
      *value_float = *(float_p->value + element);
  }
  else if ( decl == K_DECL_STRING)
  { 
    /* Search string */
    string_list = extstring_list;

    for ( string_p = string_list; string_p; string_p = string_p->next)
    {
      if ( strcmp( string_p->name, varname) == 0)
      {
        found = 1;
        break;
      }
    }
    if ( found)
      strncpy( value_string, string_p->value + element*K_STRING_SIZE, K_STRING_SIZE);
  }
  if ( !found)
    return CCM__VARNOTFOUND;
  return 1;
}

int ccm_ref_external_var( 
  const char   	*name,
  int		decl,
  void		**valuep)
{
  ccm_sIntvar		*int_p;
  ccm_sFloatvar	*float_p;
  ccm_sStringvar	*string_p;
  ccm_sIntvar		*int_list;
  ccm_sFloatvar	*float_list;
  ccm_sStringvar	*string_list;
  int			found;
  int			sts;
  int			array;
  int			element;
  char			varname[80];

  sts = ccm_varname_parse( NULL, name, varname, &array, &element);
  if ( EVEN(sts)) return sts;

  found = 0;

  if ( decl == K_DECL_INT)
  { 
    int_list = extint_list;

    for ( int_p = int_list; int_p; int_p = int_p->next)
    {
      if ( strcmp( int_p->name, varname) == 0)
      {
        found = 1;
        break;
      }
    }
    if ( found)
      *valuep = (void *)(int_p->value + element);
  }
  else if ( decl == K_DECL_FLOAT)
  { 
    /* Search float */
    float_list = extfloat_list;

    for ( float_p = float_list; float_p; float_p = float_p->next)
    {
      if ( strcmp( float_p->name, varname) == 0)
      {
        found = 1;
        break;
      }
    }
    if ( found)
      *valuep = (void *)(float_p->value + element);
  }
  else if ( decl == K_DECL_STRING)
  { 
    /* Search string */
    string_list = extstring_list;

    for ( string_p = string_list; string_p; string_p = string_p->next)
    {
      if ( strcmp( string_p->name, varname) == 0)
      {
        found = 1;
        break;
      }
    }
    if ( found)
      *valuep = (void *) (string_p->value + element*K_STRING_SIZE);
  }
  if ( !found)
    return CCM__VARNOTFOUND;
  return 1;
}


static int ccm_createvar(
  const char   		*name,
  int			decl,
  ccm_tFloat	       	value_float,
  ccm_tInt     		value_int,
  char  		*value_string,
  ccm_sIntvar		**int_list,
  ccm_sFloatvar		**float_list,
  ccm_sStringvar	**string_list
)
{
  ccm_sIntvar		*int_p;
  ccm_sFloatvar		*float_p;
  ccm_sStringvar	*string_p;
  int			array;
  int			elements;
  char			varname[80];
  int			sts;

  sts = ccm_varname_parse( NULL, name, varname, &array, &elements);
  if ( EVEN(sts)) return sts;
  elements++;

  if ( decl == K_DECL_INT)
  {
    for ( int_p = *int_list; int_p; int_p = int_p->next)
    {
      if ( strcmp( int_p->name, varname) == 0)
	return CCM__VARALREXIST;
    }

    int_p = calloc( 1, sizeof(ccm_sIntvar));
    strcpy( int_p->name, varname);
    int_p->value = calloc( elements, sizeof(ccm_tInt));
    if ( !array)
      *(int_p->value) = value_int;
    int_p->elements = elements;
    int_p->array = array;
    int_p->next = *int_list;
    *int_list = int_p;
  }
  else if ( decl == K_DECL_FLOAT)
  {
    for ( float_p = *float_list; float_p; float_p = float_p->next)
    {
      if ( strcmp( float_p->name, varname) == 0)
	return CCM__VARALREXIST;
    }

    float_p = calloc( 1, sizeof(ccm_sFloatvar));
    strcpy( float_p->name, varname);
    float_p->value = calloc( elements, sizeof(ccm_tFloat));
    if ( !array)
      *(float_p->value) = value_float;
    float_p->elements = elements;
    float_p->array = array;
    float_p->next = *float_list;
    *float_list = float_p;
  }
  else if ( decl == K_DECL_STRING)
  {
    for ( string_p = *string_list; string_p; string_p = string_p->next)
    {
      if ( strcmp( string_p->name, varname) == 0)
	return CCM__VARALREXIST;
    }

    string_p = calloc( 1, sizeof(ccm_sStringvar));
    strcpy( string_p->name, varname);
    string_p->value = calloc( elements, sizeof(ccm_tString));
    if ( !array && value_string != NULL)
      strncpy( string_p->value, value_string, sizeof(ccm_tString));
    string_p->elements = elements;
    string_p->array = array;
    string_p->next = *string_list;
    *string_list = string_p;
  }
  return 1;
}

static int ccm_deletevar(
  const char	       	*name,
  ccm_sIntvar		**int_list,
  ccm_sFloatvar		**float_list,
  ccm_sStringvar	**string_list
)
{
  ccm_sIntvar		*int_p, *int_prev;
  ccm_sFloatvar		*float_p, *float_prev;
  ccm_sStringvar	*string_p, *string_prev;
  int			array;
  int			elements;
  char			varname[80];
  int			sts;
  int			found;

  sts = ccm_varname_parse( NULL, name, varname, &array, &elements);
  if ( EVEN(sts)) return sts;
  elements++;

  int_prev = 0;
  found = 0;
  for ( int_p = *int_list; int_p; int_p = int_p->next) {
    if ( strcmp( int_p->name, varname) == 0) {	
      found = 1;
      
      if ( !int_prev)
	*int_list = int_p->next;
      else 
	int_prev->next = int_p->next;
      free( int_p->value);
      free( int_p);
      break;
    }
    int_prev = int_p;
  }

  if ( found)
    return 1;

  float_prev = 0;
  for ( float_p = *float_list; float_p; float_p = float_p->next) {
    if ( strcmp( float_p->name, varname) == 0) {
      found = 1;
    
      if ( !float_prev)
	*float_list = float_p->next;
      else 
	float_prev->next = float_p->next;
      free( float_p->value);
      free( float_p);
      break;
    }
    float_prev = float_p;
  }
  if ( found)
    return 1;

  string_prev = 0;
  for ( string_p = *string_list; string_p; string_p = string_p->next) {
    if ( strcmp( string_p->name, varname) == 0) {
      found = 1;

      if ( !string_prev)
	*string_list = string_p->next;
      else 
	string_prev->next = string_p->next;
      free( string_p->value);
      free( string_p);
      break;
    }
    string_prev = string_p;
  }
  if ( !found)
    return CCM__VARNOTFOUND;
  return 1;
}

int ccm_create_external_var(
  const char	       	*name,
  int			decl,
  ccm_tFloat	       	value_float,
  ccm_tInt	       	value_int,
  char  		*value_string
)
{
  int	sts;

  sts = ccm_createvar( name, decl, value_float, value_int, value_string,
	&extint_list, &extfloat_list, &extstring_list);
  if ( sts == CCM__VARALREXIST)
    return CCM__SUCCESS;
  return sts;
}

int ccm_delete_external_var(
  const char	       	*name,
  ccm_tFloat   		value_float,
  ccm_tInt	       	value_int,
  char  		*value_string
)
{
  return ccm_deletevar( name, &extint_list, &extfloat_list, &extstring_list);
}

static int	ccm_read_file(
		ccm_tFileCtx	filectx,
		char		*filename,
		ccm_sLine      	**line_list)
{
  char		str[K_LINE_SIZE+40];
  char		str2[K_LINE_SIZE+40];
  FILE 		*infile;
  char		*rsts = 0;
  ccm_sLine 	*line_p;
  ccm_sLine 	*line_list_p;
  int		sts;
  char		*s, *se;
  int		row;
  char		fname[80];

  if ( filectx->deffilename_func)
    sts = ( filectx->deffilename_func)( fname, filename, filectx->client_data);
  else
    strcpy( fname, filename);

  infile = fopen( fname, "r");
  if ( !infile)
    return CCM__OPENFILE;

  row = 0;
  line_list_p = *line_list;
  while ( (rsts = fgets( str, sizeof(str), infile)) != NULL)
  {
    row++;
    ccm_remove_blank( str, str);
    if ( str[0] == '!' || (str[0] == '#' && strncmp( str, "#include", 8) != 0))
      continue;
    /* If last char i backslash, concatenate next line */
    while( str[strlen(str)-1] == '\\')
    {
      rsts = fgets( str2, sizeof(str2), infile);
      if ( rsts == NULL)
      {
        filectx->error_row = row;
	strcpy( filectx->error_line, "EOF");
        return CCM__SYNTAX;
      }
      row++;
      ccm_remove_blank( str2, str2);
      if ( str2[0] == '!' || str2[0] == '#')
        continue;
      str[strlen(str)-1] = 0;
      if ( strlen(str)+strlen(str2) > K_LINE_SIZE-1)
      {
        filectx->error_row = row;
	strcpy( filectx->error_line, str2);
        return CCM__LONGLINE;
      }
      strcat( str, str2);
    }
    if ( strlen(str) > K_LINE_SIZE-1)
    {
      filectx->error_row = row;
      strcpy( filectx->error_line, str);
      return CCM__LONGLINE;
    }
    if ( strncmp( str, "#include", 8) == 0)
    {
      /* Read the include file */
      s = strchr( str, '<');
      if ( s == 0)
      {
        filectx->error_row = row;
	strcpy( filectx->error_line, str);
        return CCM__SYNTAX;
      }
      se = strrchr( str, '>');
      if ( se == 0)
      {
        filectx->error_row = row;
	strcpy( filectx->error_line, str);
        return CCM__SYNTAX;
      }
      *se = 0;
      sts = ccm_read_file( filectx, ++s, &line_list_p);
      if ( EVEN(sts)) return sts;
      if ( *line_list == NULL)
	*line_list = line_list_p;

      for ( ;line_list_p->next; line_list_p = line_list_p->next) ;
      continue;
    }

    line_p = calloc( 1, sizeof(ccm_sLine));
    strcpy( line_p->line, str);
    line_p->row = row;
    if ( line_list_p == NULL)    
      *line_list = line_p;
    else
    {
      line_p->prev = line_list_p;
      line_list_p->next = line_p;
    }
    line_list_p = line_p;
  }
  fclose( infile);
  return 1;
}

static int ccm_init_filectx( 	ccm_tFileCtx	filectx)
{
  ccm_sLine 	*line_p;
  ccm_sLine 	*l_p;
  char		out_str[5][32];
  int		in_function;
  int		func_found;
  int		main_start_found;
  int		main_end_found;
  int		nr;
  ccm_sFunc	*func_p;
  char		*s;
  int		ok;

  func_found = 0;
  in_function = 0;
  main_start_found = 0;
  main_end_found = 0;
  for ( line_p = filectx->line_list; line_p; line_p = line_p->next)
  {
    if ( strncmp( line_p->line, "function", 8) == 0 && 
	 (line_p->line[8] == ' ' ||line_p->line[8] == '	'))
    {
      if ( in_function)
      {
        filectx->error_row = line_p->row;
	strcpy( filectx->error_line, line_p->line);
        return CCM__FUNCNOTFOUND;
      }
      func_found = 1;

      /* Parse the command string */
      nr = rtt_parse( line_p->line, " 	(", "", (char *)out_str, 
		sizeof( out_str) / sizeof( out_str[0]), sizeof( out_str[0]), 0);
      if ( nr < 4)
      {
        filectx->error_row = line_p->row;
	strcpy( filectx->error_line, line_p->line);
        return CCM__SYNTAX;
      }

      func_p = calloc( 1, sizeof( ccm_sFunc));
      strcpy( func_p->name, out_str[2]);
      func_p->start_line = line_p;
      if ( strcmp( out_str[1], "int") == 0)
        func_p->decl = K_DECL_INT;
      else if ( strcmp( out_str[1], "float") == 0)
        func_p->decl = K_DECL_FLOAT;
      else if ( strcmp( out_str[1], "string") == 0)
        func_p->decl = K_DECL_STRING;
      else
      {
        filectx->error_row = line_p->row;
	strcpy( filectx->error_line, line_p->line);
        return CCM__UNKNVARTYPE;
      }

      func_p->next = filectx->func_list;
      filectx->func_list = func_p;
      in_function = 1;
      line_p->type = K_LINE_FUNCTION;
    }
    else if ( strncmp( line_p->line, "endfunction", 11) == 0)
    {
      if ( !in_function)
      {
        filectx->error_row = line_p->row;
	strcpy( filectx->error_line, line_p->line);
        return CCM__FUNCMISM;
      }

      func_p = filectx->func_list;
      func_p->end_line = line_p;
      in_function = 0;
      line_p->type = K_LINE_ENDFUNCTION;
    }
    else if ( strncmp( line_p->line, "main", 4) == 0)
    {

      /* Parse the command string */
       nr = rtt_parse( line_p->line, " 	(", "", (char *)out_str, 
		sizeof( out_str) / sizeof( out_str[0]), sizeof( out_str[0]), 0);
       if ( nr < 1)
      {
        filectx->error_row = line_p->row;
	strcpy( filectx->error_line, line_p->line);
        return CCM__SYNTAX;
      }

      if ( strcmp( out_str[0], "main") == 0)
      {
        if ( main_start_found)
        {
          filectx->error_row = line_p->row;
	strcpy( filectx->error_line, line_p->line);
          return CCM__MAINMISM;
        }


        filectx->main_start_line = line_p;
        main_start_found = 1;
        line_p->type = K_LINE_MAIN;
      }
    }
    else if ( strncmp( line_p->line, "endmain", 7) == 0)
    {
      if ( !main_start_found)
      {
        filectx->error_row = line_p->row;
	strcpy( filectx->error_line, line_p->line);
        return CCM__MAINMISM;
      }

      filectx->main_end_line = line_p;
      main_end_found = 1;
      line_p->type = K_LINE_ENDMAIN;
    }
    else if ( strncmp( line_p->line, "if", 2) == 0 &&
	      (line_p->line[2] == ' ' || line_p->line[2] == '	' ||
               line_p->line[2] == '('))
    {
      line_p->type = K_LINE_IF;
    }
    else if ( strncmp( line_p->line, "else", 4) == 0 &&
	      (line_p->line[4] == ' ' || line_p->line[4] == '	' ||
               line_p->line[4] == 10  || line_p->line[4] == 0))
    {
      line_p->type = K_LINE_ELSE;
    }
    else if ( strncmp( line_p->line, "endif", 5) == 0 &&
	      (line_p->line[5] == ' ' || line_p->line[5] == '	' ||
               line_p->line[5] == 10  || line_p->line[5] == 0 ||
               line_p->line[5] == ';'))
    {
      line_p->type = K_LINE_ENDIF;
    }
    else if ( strncmp( line_p->line, "while", 5) == 0 &&
	      (line_p->line[5] == ' ' || line_p->line[5] == '	' ||
               line_p->line[5] == '('))
    {
      line_p->type = K_LINE_WHILE;
    }
    else if ( strncmp( line_p->line, "endwhile", 8) == 0 &&
	      (line_p->line[8] == ' ' || line_p->line[8] == '	' ||
               line_p->line[8] == 10  || line_p->line[8] == 0 ||
               line_p->line[8] == ';'))
    {
      line_p->type = K_LINE_ENDWHILE;
    }
    else if ( strncmp( line_p->line, "for", 3) == 0 &&
	      (line_p->line[3] == ' ' || line_p->line[3] == '	' ||
               line_p->line[3] == '('))
    {
      line_p->type = K_LINE_FOR;
    }
    else if ( strncmp( line_p->line, "endfor", 6) == 0 &&
	      (line_p->line[6] == ' ' || line_p->line[6] == '	' ||
               line_p->line[6] == 10  || line_p->line[6] == 0 ||
               line_p->line[6] == ';'))
    {
      line_p->type = K_LINE_ENDFOR;
    }
    else if ( strncmp( line_p->line, "break", 5) == 0 &&
	      (line_p->line[5] == ' ' || line_p->line[5] == '	' ||
               line_p->line[5] == 10  || line_p->line[5] == 0 ||
               line_p->line[5] == ';'))
    {
      line_p->type = K_LINE_BREAK;
    }
    else if ( strncmp( line_p->line, "continue", 8) == 0 &&
	      (line_p->line[8] == ' ' || line_p->line[8] == '	' ||
               line_p->line[8] == 10  || line_p->line[8] == 0 ||
               line_p->line[8] == ';'))
    {
      line_p->type = K_LINE_CONTINUE;
    }
    else if ( strncmp( line_p->line, "goto", 4) == 0 &&
	      (line_p->line[4] == ' ' || line_p->line[4] == '	' ))
    {
      line_p->type = K_LINE_GOTO;
    }
    else if ( strlen(line_p->line) != 0 &&
              line_p->line[strlen(line_p->line)-1] == ':')
    {
      /* Check label name */
      ok = 1;
      for ( s = line_p->line; *s; s++)
      {
        if ( ! (isalnum(*s) || *s == '_'))
        {
          if ( s != &line_p->line[strlen(line_p->line)-1])
          {
            ok = 0;
            break;
          }
        }
      }
      if ( ok)
        line_p->type = K_LINE_LABEL;
    }
  }
  if ( filectx->line_list && 
       !main_start_found && !main_end_found && !func_found)
  {
    /* No main and no function found, insert mainstart first and main end last
       in list */
    for ( line_p = filectx->line_list; line_p->next; line_p = line_p->next) ;
    l_p = calloc( 1, sizeof(ccm_sLine));
    filectx->main_end_line = l_p;
    l_p->type = K_LINE_ENDMAIN;
    l_p->row = line_p->row;
    l_p->prev = line_p;
    line_p->next = l_p;
    main_end_found = 1;

    l_p = calloc( 1, sizeof(ccm_sLine));
    filectx->main_start_line = l_p;
    l_p->type = K_LINE_MAIN;
    l_p->row = 1;
    filectx->line_list->prev = l_p;
    l_p->next = filectx->line_list;
    filectx->line_list = l_p;
    main_start_found = 1;
  }

  if ( !main_start_found)
    return CCM__MAINMISM;
  if ( !main_end_found)
    return CCM__MAINMISM;
  if ( in_function)
    return CCM__FUNCMISM;

  return 1;
}

static void ccm_free_filectx( 	ccm_tFileCtx	filectx)
{
  ccm_sLine 	*line_p, *next_line;

  for ( line_p = filectx->line_list; line_p; line_p = next_line)
  {
    next_line = line_p->next;
    free( (char *) line_p);
  }
  /* Free variablelists and funcctx */
  ccm_free_varlists( filectx->gblint_list, filectx->gblfloat_list,
	filectx->gblstring_list);

  free( (char *) filectx);
}

static int ccm_func_printf( 
  void *filectx,
  ccm_sArg *arg_list, 
  int arg_count,
  int *return_decl, 
  ccm_tFloat *return_float, 
  ccm_tInt *return_int, 
  char *return_string)
{
  ccm_sArg	*arg_p;
  char	format[K_STRING_SIZE];
  char	frm[K_STRING_SIZE];
  int	sts;
  char	*s, *t;
  char	*pos, *last_pos;
  int	i;

  if ( arg_count == 0)
    return CCM__ARGMISM;
  if ( arg_list->value_decl != K_DECL_STRING)
    return CCM__VARTYPE;
  /* Replace any '\x' characters */
  for (s = arg_list->value_string, t = format; *s; s++)
  {
    if ( *s == '\\')
    {
      if ( *(s+1) == 'n')
        *t = '\n';
      else if ( *(s+1) == '"')
        *t = '\"';
      else if ( *(s+1) == '\\')
        *t = '\\';
      else
      {
        *t = *s;
        s--;
      }
      s++;
    }
    else
      *t = *s;
    t++;
  }
  *t = 0;

  if ( arg_count == 1)
    sts = printf( format);
  else
  {
    pos = format;
    last_pos = format;
    sts = 0;
    arg_p = arg_list->next;
    for ( i = 0; i < arg_count; i++)
    {
      pos = strchr( pos, '%');
      while ( pos && *(pos+1) == '%')
        pos = strchr( pos+2, '%');

      if ( !pos && i != arg_count - 1)
        return CCM__ARGMISM;
      if ( pos && i == arg_count - 1)
        return CCM__ARGMISM;
      if ( !pos)
        pos = format + strlen(format);

      if ( i == 0)
      {
        pos++;
        continue;
      }
      strncpy( frm, last_pos, pos - last_pos);
      frm[pos-last_pos]=0;
      switch( arg_p->value_decl) {
        case K_DECL_FLOAT: sts += printf( frm, arg_p->value_float); break;
        case K_DECL_INT:   sts += printf( frm, arg_p->value_int); break;
        case K_DECL_STRING:sts += printf( frm, arg_p->value_string); break;
      }
      last_pos = pos;
      pos++;
      arg_p = arg_p->next;
    }
  }
#if defined OS_LYNX || defined OS_LINUX
  fflush( stdout);
#endif
  *return_decl = K_DECL_INT;
  *return_int = sts;
  return 1;
}

static int ccm_func_say( 
  void *filectx,
  ccm_sArg *arg_list, 
  int arg_count,
  int *return_decl, 
  ccm_tFloat *return_float, 
  ccm_tInt *return_int, 
  char *return_string)
{
  ccm_sArg	*arg_p;
  int	sts;

  if ( arg_count != 1)
    return CCM__ARGMISM;
  if ( arg_list->value_decl != K_DECL_STRING)
    return CCM__VARTYPE;

  arg_p = arg_list;
  if ( arg_p->value_decl == K_DECL_FLOAT)
    sts = printf( "%f", arg_p->value_float);
  else if ( arg_p->value_decl == K_DECL_INT)
    sts = printf( "%ld", arg_p->value_int);
  else if ( arg_p->value_decl == K_DECL_STRING)
    sts = printf( "%s\n", arg_p->value_string);

  *return_decl = K_DECL_INT;
  *return_int = sts;
  return 1;
}


static int ccm_func_scanf( 
  void *filectx,
  ccm_sArg *arg_list, 
  int arg_count,
  int *return_decl, 
  ccm_tFloat *return_float, 
  ccm_tInt *return_int, 
  char *return_string)
{
  ccm_sArg	*arg_p;
  char	format[K_STRING_SIZE];
  int	sts;

  if ( arg_count != 2)
    return CCM__ARGMISM;
  if ( arg_list->value_decl != K_DECL_STRING)
    return CCM__VARTYPE;
  strcpy( format, arg_list->value_string);

  arg_p = arg_list->next;
  if ( arg_p->value_decl == K_DECL_FLOAT)
    sts = scanf( format, &arg_p->value_float);
  else if ( arg_p->value_decl == K_DECL_INT)
    sts = scanf( format, &arg_p->value_int);
  else if ( arg_p->value_decl == K_DECL_STRING)
    sts = scanf( format, arg_p->value_string);
  arg_p->value_returned = 1;
  arg_p->var_decl = arg_p->value_decl;
  *return_decl = K_DECL_INT;
  *return_int = sts;
  return 1;
}

static int ccm_func_ask(
  void *filectx,
  ccm_sArg *arg_list,
  int arg_count,
  int *return_decl,
  ccm_tFloat *return_float,
  ccm_tInt *return_int,
  char *return_string)
{
  ccm_sArg *arg_p;
  int   sts;

  if ( arg_count != 2)
    return CCM__ARGMISM;
  if ( arg_list->value_decl != K_DECL_STRING)
    return CCM__VARTYPE;

  printf("%s", arg_list->value_string);
#if defined OS_LYNX || defined OS_LINUX
  fflush( stdout);
#endif

  arg_p = arg_list->next;
  if ( arg_p->value_decl == K_DECL_FLOAT)
    sts = scanf( "%f", &arg_p->value_float);
  else if ( arg_p->value_decl == K_DECL_INT)
    sts = scanf( "%ld", &arg_p->value_int);
  else if ( arg_p->value_decl == K_DECL_STRING)
    sts = scanf( "%s", arg_p->value_string);
  arg_p->value_returned = 1;
  arg_p->var_decl = arg_p->value_decl;
  *return_decl = K_DECL_INT;
  *return_int = sts;
  return 1;
}


static int ccm_func_fprintf( 
  void *filectx,
  ccm_sArg *arg_list, 
  int arg_count,
  int *return_decl, 
  ccm_tFloat *return_float, 
  ccm_tInt *return_int, 
  char *return_string)
{
  ccm_sArg	*arg_p1, *arg_p2, *arg_p;
  char	format[K_STRING_SIZE];
  char	frm[K_STRING_SIZE];
  int	sts;
  FILE	*file;
  char	*s, *t;
  char	*pos, *last_pos;
  int	i;

  if ( arg_count < 2)
    return CCM__ARGMISM;
  arg_p1 = arg_list;
  arg_p2 = arg_list->next;
  if ( arg_p1->value_decl != K_DECL_INT)
    return CCM__VARTYPE;
  if ( arg_p2->value_decl != K_DECL_STRING)
    return CCM__VARTYPE;
  file = (FILE *) arg_p1->value_int;
  /* Replace any '\x' characters */
  for (s = arg_p2->value_string, t = format; *s; s++)
  {
    if ( *s == '\\')
    {
      if ( *(s+1) == 'n')
        *t = '\n';
      else if ( *(s+1) == '"')
        *t = '\"';
      else if ( *(s+1) == '\\')
        *t = '\\';
      else
      {
        *t = *s;
        s--;
      }
      s++;
    }
    else
      *t = *s;
    t++;
  }
  *t = 0;

  if ( arg_count == 2)
    sts = fprintf( file, format);
  else
  {
    pos = format;
    last_pos = format;
    sts = 0;
    arg_p = arg_p2->next;
    for ( i = 0; i < arg_count-1; i++)
    {
      pos = strchr( pos, '%');
      while ( pos && *(pos+1) == '%')
        pos = strchr( pos+2, '%');

      if ( !pos && i != arg_count - 2)
        return CCM__ARGMISM;
      if ( pos && i == arg_count - 2)
        return CCM__ARGMISM;
      if ( !pos)
        pos = format + strlen(format);

      if ( i == 0)
      {
        pos++;
        continue;
      }
      strncpy( frm, last_pos, pos - last_pos);
      frm[pos-last_pos]=0;
      switch( arg_p->value_decl) {
        case K_DECL_FLOAT: sts += fprintf( file, frm, arg_p->value_float); break;
        case K_DECL_INT:   sts += fprintf( file, frm, arg_p->value_int); break;
        case K_DECL_STRING:sts += fprintf( file, frm, arg_p->value_string); break;
      }
      last_pos = pos;
      pos++;
      arg_p = arg_p->next;
    }
  }

#if 0
  if ( arg_count == 2)
    fprintf( file, format);
  else if ( arg_count == 3)
  {
    arg_p3 = arg_p2->next;
    if ( arg_p3->value_decl == K_DECL_FLOAT)
      sts = fprintf( file, format, arg_p3->value_float);
    else if ( arg_p3->value_decl == K_DECL_INT)
      sts = fprintf( file, format, arg_p3->value_int);
    else if ( arg_p3->value_decl == K_DECL_STRING)
      sts = fprintf( file, format, arg_p3->value_string);
  }
  else if ( arg_count == 4)
  {
    arg_p3 = arg_p2->next;
    arg_p4 = arg_p3->next;
    if ( arg_p3->value_decl == K_DECL_FLOAT && arg_p4->value_decl == K_DECL_FLOAT)
      sts = fprintf( file, format, arg_p3->value_float, arg_p4->value_float);
    else if ( arg_p3->value_decl == K_DECL_FLOAT && arg_p4->value_decl == K_DECL_INT)
      sts = fprintf( file, format, arg_p3->value_float, arg_p4->value_int);
    else if ( arg_p3->value_decl == K_DECL_FLOAT && arg_p4->value_decl == K_DECL_STRING)
      sts = fprintf( file, format, arg_p3->value_float, arg_p4->value_string);
    else if ( arg_p3->value_decl == K_DECL_INT && arg_p4->value_decl == K_DECL_FLOAT)
      sts = fprintf( file, format, arg_p3->value_int, arg_p4->value_float);
    else if ( arg_p3->value_decl == K_DECL_INT && arg_p4->value_decl == K_DECL_INT)
      sts = fprintf( file, format, arg_p3->value_int, arg_p4->value_int);
    else if ( arg_p3->value_decl == K_DECL_INT && arg_p4->value_decl == K_DECL_STRING)
      sts = fprintf( file, format, arg_p3->value_int, arg_p4->value_string);
    else if ( arg_p3->value_decl == K_DECL_STRING && arg_p4->value_decl == K_DECL_FLOAT)
      sts = fprintf( file, format, arg_p3->value_string, arg_p4->value_float);
    else if ( arg_p3->value_decl == K_DECL_STRING && arg_p4->value_decl == K_DECL_INT)
      sts = fprintf( file, format, arg_p3->value_string, arg_p4->value_int);
    else if ( arg_p3->value_decl == K_DECL_STRING && arg_p4->value_decl == K_DECL_STRING)
      sts = fprintf( file, format, arg_p3->value_string, arg_p4->value_string);
  }
  else
    return CCM__ARGMISM;
#endif
  *return_decl = K_DECL_INT;
  *return_int = sts;
  return 1;
}

static int ccm_func_sprintf( 
  void *filectx,
  ccm_sArg *arg_list, 
  int arg_count,
  int *return_decl, 
  ccm_tFloat *return_float, 
  ccm_tInt *return_int, 
  char *return_string)
{
  ccm_sArg	*arg_p1, *arg_p2, *arg_p;
  char	format[K_STRING_SIZE];
  char	frm[K_STRING_SIZE];
  int	sts;
  char	*s, *t;
  char	*pos, *last_pos;
  int	i;
  char	tmp[K_STRING_SIZE];

  if ( arg_count < 2)
    return CCM__ARGMISM;
  arg_p1 = arg_list;
  arg_p2 = arg_list->next;
  if ( arg_p1->value_decl != K_DECL_STRING)
    return CCM__VARTYPE;
  if ( arg_p2->value_decl != K_DECL_STRING)
    return CCM__VARTYPE;
  /* Replace any '\x' characters */
  for (s = arg_p2->value_string, t = format; *s; s++)
  {
    if ( *s == '\\')
    {
      if ( *(s+1) == 'n')
        *t = '\n';
      else if ( *(s+1) == '"')
        *t = '\"';
      else if ( *(s+1) == '\\')
        *t = '\\';
      else
      {
        *t = *s;
        s--;
      }
      s++;
    }
    else
      *t = *s;
    t++;
  }
  *t = 0;

  if ( arg_count == 2)
    sts = sprintf( arg_p1->value_string, format);
  else
  {
    pos = format;
    last_pos = format;
    sts = 0;
    arg_p = arg_p2->next;
    strcpy( arg_p1->value_string, "");
    for ( i = 0; i < arg_count-1; i++)
    {
      pos = strchr( pos, '%');
      while ( pos && *(pos+1) == '%')
        pos = strchr( pos+2, '%');

      if ( !pos && i != arg_count - 2)
        return CCM__ARGMISM;
      if ( pos && i == arg_count - 2)
        return CCM__ARGMISM;
      if ( !pos)
        pos = format + strlen(format);

      if ( i == 0)
      {
        pos++;
        continue;
      }
      strncpy( frm, last_pos, pos - last_pos);
      frm[pos-last_pos]=0;
      switch( arg_p->value_decl) {
        case K_DECL_FLOAT: sts += sprintf( tmp, frm, arg_p->value_float); break;
        case K_DECL_INT:   sts += sprintf( tmp, frm, arg_p->value_int); break;
        case K_DECL_STRING:sts += sprintf( tmp, frm, arg_p->value_string); break;
      }
      if ( strlen( arg_p1->value_string) + strlen(tmp) > K_STRING_SIZE - 1)
        return CCM__STRINGEXCEED;
      strcat( arg_p1->value_string, tmp);
      last_pos = pos;
      pos++;
      arg_p = arg_p->next;
    }
  }
#if 0
  if ( arg_count == 2)
    sprintf( arg_p1->value_string, format);
  else if ( arg_count == 3)
  {
    arg_p3 = arg_p2->next;
    if ( arg_p3->value_decl == K_DECL_FLOAT)
      sts = sprintf( arg_p1->value_string, format, arg_p3->value_float);
    else if ( arg_p3->value_decl == K_DECL_INT)
      sts = sprintf( arg_p1->value_string, format, arg_p3->value_int);
    else if ( arg_p3->value_decl == K_DECL_STRING)
      sts = sprintf( arg_p1->value_string, format, arg_p3->value_string);
  }
  else if ( arg_count == 4)
  {
    arg_p3 = arg_p2->next;
    arg_p4 = arg_p3->next;
    if ( arg_p3->value_decl == K_DECL_FLOAT && arg_p4->value_decl == K_DECL_FLOAT)
      sts = sprintf( arg_p1->value_string, format, arg_p3->value_float, arg_p4->value_float);
    else if ( arg_p3->value_decl == K_DECL_FLOAT && arg_p4->value_decl == K_DECL_INT)
      sts = sprintf( arg_p1->value_string, format, arg_p3->value_float, arg_p4->value_int);
    else if ( arg_p3->value_decl == K_DECL_FLOAT && arg_p4->value_decl == K_DECL_STRING)
      sts = sprintf( arg_p1->value_string, format, arg_p3->value_float, arg_p4->value_string);
    else if ( arg_p3->value_decl == K_DECL_INT && arg_p4->value_decl == K_DECL_FLOAT)
      sts = sprintf( arg_p1->value_string, format, arg_p3->value_int, arg_p4->value_float);
    else if ( arg_p3->value_decl == K_DECL_INT && arg_p4->value_decl == K_DECL_INT)
      sts = sprintf( arg_p1->value_string, format, arg_p3->value_int, arg_p4->value_int);
    else if ( arg_p3->value_decl == K_DECL_INT && arg_p4->value_decl == K_DECL_STRING)
      sts = sprintf( arg_p1->value_string, format, arg_p3->value_int, arg_p4->value_string);
    else if ( arg_p3->value_decl == K_DECL_STRING && arg_p4->value_decl == K_DECL_FLOAT)
      sts = sprintf( arg_p1->value_string, format, arg_p3->value_string, arg_p4->value_float);
    else if ( arg_p3->value_decl == K_DECL_STRING && arg_p4->value_decl == K_DECL_INT)
      sts = sprintf( arg_p1->value_string, format, arg_p3->value_string, arg_p4->value_int);
    else if ( arg_p3->value_decl == K_DECL_STRING && arg_p4->value_decl == K_DECL_STRING)
      sts = sprintf( arg_p1->value_string, format, arg_p3->value_string, arg_p4->value_string);
  }
  else
    return CCM__ARGMISM;
#endif
  arg_p1->value_returned = 1;
  *return_decl = K_DECL_INT;
  *return_int = sts;
  return 1;
}

static int ccm_func_fgets( 
  void *filectx,
  ccm_sArg *arg_list, 
  int arg_count,
  int *return_decl, 
  ccm_tFloat *return_float, 
  ccm_tInt *return_int, 
  char *return_string)
{
  ccm_sArg	*arg_p1, *arg_p2;
  FILE	*file;
  char	*rsts = 0;

  if ( arg_count != 2)
    return CCM__ARGMISM;
  arg_p1 = arg_list;
  arg_p2 = arg_list->next;
  if ( arg_p2->value_decl != K_DECL_INT)
    return CCM__VARTYPE;
  if ( arg_p1->value_decl != K_DECL_STRING)
    return CCM__VARTYPE;
  file = (FILE *) arg_p2->value_int;
  rsts = fgets( ((ccm_tFileCtx)filectx)->last_fgets, 
	sizeof(((ccm_tFileCtx)filectx)->last_fgets), file);
  if ( rsts != NULL)
  {
    if ( ((ccm_tFileCtx)filectx)->last_fgets[strlen(((ccm_tFileCtx)filectx)->last_fgets)-1] == 10)
      ((ccm_tFileCtx)filectx)->last_fgets[ strlen(((ccm_tFileCtx)filectx)->last_fgets)-1] = 0;
    strncpy( arg_p1->value_string, ((ccm_tFileCtx)filectx)->last_fgets, K_STRING_SIZE);
    arg_p1->value_string[K_STRING_SIZE-1] = 0;
    arg_p1->value_returned = 1;
    arg_p1->var_decl = arg_p1->value_decl;
    *return_int = 1;
  }
  else
    *return_int = 0;
  *return_decl = K_DECL_INT;
  return 1;
}

static int ccm_func_element( 
  void *filectx,
  ccm_sArg *arg_list, 
  int arg_count,
  int *return_decl, 
  ccm_tFloat *return_float, 
  ccm_tInt *return_int, 
  char *return_string)
{
  ccm_sArg	*arg_p1, *arg_p2, *arg_p3;

  if ( arg_count != 3)
    return CCM__ARGMISM;
  arg_p1 = arg_list;
  arg_p2 = arg_list->next;
  arg_p3 = arg_p2->next;
  if ( arg_p1->value_decl != K_DECL_INT)
    return CCM__VARTYPE;
  if ( arg_p2->value_decl != K_DECL_STRING)
    return CCM__VARTYPE;
  if ( arg_p3->value_decl != K_DECL_STRING)
    return CCM__VARTYPE;

  ccm_element( return_string, arg_p1->value_int, arg_p3->value_string, 
	arg_p2->value_string[0]);

  *return_decl = K_DECL_STRING;
  return 1;
}

static int ccm_func_felement( 
  void *filectx,
  ccm_sArg *arg_list, 
  int arg_count,
  int *return_decl, 
  ccm_tFloat *return_float, 
  ccm_tInt *return_int, 
  char *return_string)
{
  ccm_sArg	*arg_p1, *arg_p2;

  if ( arg_count != 2)
    return CCM__ARGMISM;
  arg_p1 = arg_list;
  arg_p2 = arg_list->next;
  if ( arg_p1->value_decl != K_DECL_INT)
    return CCM__VARTYPE;
  if ( arg_p2->value_decl != K_DECL_STRING)
    return CCM__VARTYPE;

  ccm_element( return_string, arg_p1->value_int, 
	((ccm_tFileCtx)filectx)->last_fgets, arg_p2->value_string[0]);
  *return_decl = K_DECL_STRING;
  return 1;
}

static int ccm_func_extract( 
  void *filectx,
  ccm_sArg *arg_list, 
  int arg_count,
  int *return_decl, 
  ccm_tFloat *return_float, 
  ccm_tInt *return_int, 
  char *return_string)
{
  ccm_sArg	*arg_p1, *arg_p2, *arg_p3;

  if ( arg_count != 3)
    return CCM__ARGMISM;
  arg_p1 = arg_list;
  arg_p2 = arg_list->next;
  arg_p3 = arg_p2->next;
  if ( arg_p1->value_decl != K_DECL_INT)
    return CCM__VARTYPE;
  if ( arg_p2->value_decl != K_DECL_INT)
    return CCM__VARTYPE;
  if ( arg_p3->value_decl != K_DECL_STRING)
    return CCM__VARTYPE;

  if ( arg_p1->value_int > (int)strlen( arg_p3->value_string) ||
       arg_p1->value_int < 1)
    strcpy( return_string, "");
  else
  {
    strncpy( return_string, &arg_p3->value_string[arg_p1->value_int-1], 
	arg_p2->value_int);
    if ( arg_p2->value_int < K_STRING_SIZE)
      return_string[arg_p2->value_int] = 0;
  }

  *return_decl = K_DECL_STRING;
  return 1;
}

static int ccm_func_edit( 
  void *filectx,
  ccm_sArg *arg_list, 
  int arg_count,
  int *return_decl, 
  ccm_tFloat *return_float, 
  ccm_tInt *return_int, 
  char *return_string)
{
  ccm_sArg	*arg_p1;
  char	*s, *t, *last_no_space;
  int	first_elm, first_space;

  if ( arg_count != 1)
    return CCM__ARGMISM;
  arg_p1 = arg_list;
  if ( arg_p1->value_decl != K_DECL_STRING)
    return CCM__VARTYPE;

  t = return_string;
  first_elm = 1;
  first_space = 1;
  last_no_space = 0;
  for ( s = arg_p1->value_string; *s; s++)
  {
    if (*s == ' ' || *s == '	')
    {
      if ( first_elm)
        continue;
      if ( !first_space)
        continue;
      *t = ' ';        
      first_space = 0;
    }
    else
    {
      *t = *s;
      first_elm = 0;
      first_space = 1;
      last_no_space = t;
    }
    t++;
  }
  if ( last_no_space)
   *(++last_no_space) = 0;
  else
   *t = 0;

  *return_decl = K_DECL_STRING;
  return 1;
}

static int ccm_func_fopen( 
  void *filectx,
  ccm_sArg *arg_list, 
  int arg_count,
  int *return_decl, 
  ccm_tFloat *return_float, 
  ccm_tInt *return_int, 
  char *return_string)
{
  ccm_sArg	*arg_p1, *arg_p2;
  FILE	*file;
  char filename[120];

  if ( arg_count != 2)
    return CCM__ARGMISM;
  arg_p1 = arg_list;
  arg_p2 = arg_list->next;
  if ( arg_p1->value_decl != K_DECL_STRING)
    return CCM__VARTYPE;
  if ( arg_p2->value_decl != K_DECL_STRING)
    return CCM__VARTYPE;
  dcli_translate_filename( filename, arg_p1->value_string);
  file = fopen( filename, arg_p2->value_string);
  *return_decl = K_DECL_INT;
  *return_int = (long int) file;
  return 1;
}

static int ccm_func_fclose( 
  void *filectx,
  ccm_sArg *arg_list, 
  int arg_count,
  int *return_decl, 
  ccm_tFloat *return_float, 
  ccm_tInt *return_int, 
  char *return_string)
{
  ccm_sArg	*arg_p1;
  int	sts;
  FILE	*file;

  if ( arg_count != 1)
    return CCM__ARGMISM;
  arg_p1 = arg_list;
  if ( arg_p1->value_decl != K_DECL_INT)
    return CCM__VARTYPE;
  file = (FILE *) arg_p1->value_int;
  sts = fclose( file);
  *return_decl = K_DECL_INT;
  *return_int = sts;
  return 1;
}

static int ccm_func_exit( 
  void *filectx,
  ccm_sArg *arg_list, 
  int arg_count,
  int *return_decl, 
  ccm_tFloat *return_float, 
  ccm_tInt *return_int, 
  char *return_string)
{

  if ( arg_count != 0 && arg_list->value_decl == K_DECL_INT)
    *return_int = arg_list->value_int;
  else
    *return_int = 1;
  *return_decl = K_DECL_INT;

  return CCM__EXITFUNC;
}

static int ccm_func_verify( 
  void *filectx,
  ccm_sArg *arg_list, 
  int arg_count,
  int *return_decl, 
  ccm_tFloat *return_float, 
  ccm_tInt *return_int, 
  char *return_string)
{

  if ( arg_count == 0)
  {
    *return_int = ((ccm_tFileCtx)filectx)->verify;
    *return_decl = K_DECL_INT;
  }
  else if ( arg_list->value_decl == K_DECL_INT)
  {
    if ( arg_list->value_int)
      ((ccm_tFileCtx)filectx)->verify = 1;
    else
      ((ccm_tFileCtx)filectx)->verify = 0;

    *return_int = ((ccm_tFileCtx)filectx)->verify;
    *return_decl = K_DECL_INT;
  }
  else
    return CCM__ARGMISM;

  return 1;
}


static int ccm_func_time( 
  void *filectx,
  ccm_sArg *arg_list, 
  int arg_count,
  int *return_decl, 
  ccm_tFloat *return_float, 
  ccm_tInt *return_int, 
  char *return_string)
{
  int	sts;
  char	value_string[K_STRING_SIZE];

  if ( arg_count != 0)
    return CCM__ARGMISM;

  sts = time_AtoAscii(NULL,
		time_eFormat_DateAndTime, value_string,
		sizeof(value_string));
  if (ODD(sts))
    value_string[20] = '\0';
  else
    value_string[0] = '\0';
	    
  strcpy( return_string, value_string);
  *return_decl = K_DECL_STRING;
  return 1;
}

static int ccm_func_system( 
  void *filectx,
  ccm_sArg *arg_list, 
  int arg_count,
  int *return_decl, 
  ccm_tFloat *return_float, 
  ccm_tInt *return_int, 
  char *return_string)
{
  int	sts;

  if ( arg_count != 1)
    return CCM__ARGMISM;
  if ( arg_list->value_decl != K_DECL_STRING)
    return CCM__VARTYPE;

#if !defined( OS_ELN)
  sts = system( arg_list->value_string);
#else
  sts = 0;
#endif
  *return_int = sts;
  *return_decl = K_DECL_INT;
  return 1;
}

static int ccm_func_strlen( 
  void *filectx,
  ccm_sArg *arg_list, 
  int arg_count,
  int *return_decl, 
  ccm_tFloat *return_float, 
  ccm_tInt *return_int, 
  char *return_string)
{

  if ( arg_count != 1)
    return CCM__ARGMISM;
  if ( arg_list->value_decl != K_DECL_STRING)
    return CCM__VARTYPE;

  *return_int = strlen( arg_list->value_string);
  *return_decl = K_DECL_INT;
  return 1;
}

static int ccm_func_strchr( 
  void *filectx,
  ccm_sArg *arg_list, 
  int arg_count,
  int *return_decl, 
  ccm_tFloat *return_float, 
  ccm_tInt *return_int, 
  char *return_string)
{
  ccm_sArg *arg_p2; 
  char 	*s;

  if ( arg_count != 2)
    return CCM__ARGMISM;
  arg_p2 = arg_list->next;
  if ( arg_list->value_decl != K_DECL_STRING)
    return CCM__VARTYPE;
  if ( arg_p2->value_decl != K_DECL_STRING)
    return CCM__VARTYPE;

  s = strchr( arg_list->value_string, arg_p2->value_string[0]);
  if ( s == 0)
    *return_int = 0;
  else
    *return_int = s - arg_list->value_string + 1;

  *return_decl = K_DECL_INT;
  return 1;
}

static int ccm_func_strrchr( 
  void *filectx,
  ccm_sArg *arg_list, 
  int arg_count,
  int *return_decl, 
  ccm_tFloat *return_float, 
  ccm_tInt *return_int, 
  char *return_string)
{
  ccm_sArg *arg_p2; 
  char 	*s;

  if ( arg_count != 2)
    return CCM__ARGMISM;
  arg_p2 = arg_list->next;
  if ( arg_list->value_decl != K_DECL_STRING)
    return CCM__VARTYPE;
  if ( arg_p2->value_decl != K_DECL_STRING)
    return CCM__VARTYPE;

  s = strrchr( arg_list->value_string, arg_p2->value_string[0]);
  if ( s == 0)
    *return_int = 0;
  else
    *return_int = s - arg_list->value_string + 1;

  *return_decl = K_DECL_INT;
  return 1;
}

static int ccm_func_strstr( 
  void *filectx,
  ccm_sArg *arg_list, 
  int arg_count,
  int *return_decl, 
  ccm_tFloat *return_float, 
  ccm_tInt *return_int, 
  char *return_string)
{
  ccm_sArg *arg_p2; 
  char 	*s;

  if ( arg_count != 2)
    return CCM__ARGMISM;
  arg_p2 = arg_list->next;
  if ( arg_list->value_decl != K_DECL_STRING)
    return CCM__VARTYPE;
  if ( arg_p2->value_decl != K_DECL_STRING)
    return CCM__VARTYPE;

  s = strstr( arg_list->value_string, arg_p2->value_string);
  if ( s == 0)
    *return_int = 0;
  else
    *return_int = s - arg_list->value_string + 1;

  *return_decl = K_DECL_INT;
  return 1;
}

static int ccm_func_toupper( 
  void *filectx,
  ccm_sArg *arg_list, 
  int arg_count,
  int *return_decl, 
  ccm_tFloat *return_float, 
  ccm_tInt *return_int, 
  char *return_string)
{

  if ( arg_count != 1)
    return CCM__ARGMISM;
  if ( arg_list->value_decl != K_DECL_STRING)
    return CCM__VARTYPE;

  cdh_ToUpper( return_string, arg_list->value_string);
  *return_decl = K_DECL_STRING;
  return 1;
}

static int ccm_func_tolower( 
  void *filectx,
  ccm_sArg *arg_list, 
  int arg_count,
  int *return_decl, 
  ccm_tFloat *return_float, 
  ccm_tInt *return_int, 
  char *return_string)
{

  if ( arg_count != 1)
    return CCM__ARGMISM;
  if ( arg_list->value_decl != K_DECL_STRING)
    return CCM__VARTYPE;

  cdh_ToLower( return_string, arg_list->value_string);
  *return_decl = K_DECL_STRING;
  return 1;
}

static int ccm_func_translate_filename( 
  void *filectx,
  ccm_sArg *arg_list, 
  int arg_count,
  int *return_decl, 
  ccm_tFloat *return_float, 
  ccm_tInt *return_int, 
  char *return_string)
{
  char fname[200];

  if ( arg_count != 1)
    return CCM__ARGMISM;
  if ( arg_list->value_decl != K_DECL_STRING)
    return CCM__VARTYPE;

  dcli_translate_filename( fname, arg_list->value_string);
  strncpy( return_string, fname, K_STRING_SIZE);
  return_string[K_STRING_SIZE-1] = 0;
  *return_decl = K_DECL_STRING;
  return 1;
}

static int ccm_func_get_pwr_config( 
  void *filectx,
  ccm_sArg *arg_list, 
  int arg_count,
  int *return_decl, 
  ccm_tFloat *return_float, 
  ccm_tInt *return_int, 
  char *return_string)
{
  char value[512];

  if ( arg_count != 1)
    return CCM__ARGMISM;
  if ( arg_list->value_decl != K_DECL_STRING)
    return CCM__VARTYPE;

  if ( cnf_get_value( arg_list->value_string, value)) {
    strncpy( return_string, value, K_STRING_SIZE);
    return_string[K_STRING_SIZE-1] = 0;
  }
  else
    strcpy( return_string, "");
  *return_decl = K_DECL_STRING;
  return 1;
}

static int ccm_func_get_language( 
  void *filectx,
  ccm_sArg *arg_list, 
  int arg_count,
  int *return_decl, 
  ccm_tFloat *return_float, 
  ccm_tInt *return_int, 
  char *return_string)
{
  if ( arg_count != 0)
    return CCM__ARGMISM;

  strncpy( return_string, lng_get_language_str(), K_STRING_SIZE);
  return_string[K_STRING_SIZE-1] = 0;
  *return_decl = K_DECL_STRING;
  return 1;
}

static int ccm_func_get_node_name( 
  void *filectx,
  ccm_sArg *arg_list, 
  int arg_count,
  int *return_decl, 
  ccm_tFloat *return_float, 
  ccm_tInt *return_int, 
  char *return_string)
{
  char name[80];
  pwr_tStatus sts;

  if ( arg_count != 0)
    return CCM__ARGMISM;

  syi_NodeName( &sts, name, sizeof(name));
  if ( ODD(sts)) {
    strncpy( return_string, name, K_STRING_SIZE);
    return_string[K_STRING_SIZE-1] = 0;
  }
  else
    strcpy( return_string, "");
  *return_decl = K_DECL_STRING;
  return 1;
}

static int ccm_extract_parenthes_expr( char *expr, char *line)
{
  char *s;

  s = strchr( line, '(');
  if ( s == 0)
    return CCM__SYNTAX;
    
  strcpy( expr, ++s);
  s = strrchr( expr, ')');
  if ( s == 0)
    return CCM__SYNTAX;
  *s = 0;
  ccm_remove_blank( expr, expr);
  return 1;
}

static int ccm_function_exec( 
  ccm_tFileCtx		filectx,
  char		*name,
  ccm_sArg		*arg_list,
  int		arg_count,
  int		*return_decl,
  ccm_tFloat   	*return_float,
  ccm_tInt     	*return_int,
  char		*return_string,
  int		resume)
{
  int		return_found;
  int		decl;
  ccm_tInt     	int_val;
  ccm_tFloat   	float_val;
  ccm_tString  	string_val;
  int		i;
  ccm_sLine 	*line_p;
  ccm_sLine 	*l_p;
  int		sts;
  ccm_sLine	*start_line;
  ccm_sLine	*end_line;
  int		main_found;
  int		found;
  ccm_sFunc	*func_p;
  char		arg_str[20][32];
  char		elm_str[4][K_LINE_SIZE];
  ccm_sArg     	*arg_p;
  int		nr;
  char		expr[K_LINE_SIZE];
  ccm_tFuncCtx	funcctx;
  int		if_level;
  int		while_level;
  int		for_level;
  char		arg_name[8];
  ccm_sSysFunc	*sysfunc_p;
  char		label[80];
  char		goto_label[80];

  if ( resume)
  {
    funcctx = filectx->main_funcctx;
    start_line = ((ccm_tFuncCtx)(filectx->main_funcctx))->current_line->next;
    end_line = filectx->main_end_line;
  }
  else
  {
    main_found = 0;
    if ( strcmp( name, "main") == 0)
    {
      start_line = filectx->main_start_line->next;
      end_line = filectx->main_end_line;
      main_found = 1;
    }
    else
    {
      /* Search i system function list */
      for ( sysfunc_p = ccm_sysfunc; sysfunc_p->sysfunc; sysfunc_p++)
      {
        if ( strcmp( name, sysfunc_p->name) == 0)
        {
          sts = (sysfunc_p->sysfunc)( (void *)filectx, arg_list, arg_count, 
		return_decl, return_float, return_int, return_string);
          return sts;
       }
      }

      /* Search for an application function */
     found = 0;
      for( func_p = filectx->func_list; func_p; func_p = func_p->next)
      {
        if ( strcmp( func_p->name, name) == 0)
        {
          start_line = func_p->start_line->next;
          end_line = func_p->end_line;
          found = 1;
          break;
        }
      }
      if ( !found)
        return CCM__FUNCNOTFOUND;
    }


    funcctx = calloc( 1, sizeof(*funcctx));
    funcctx->filectx = filectx;
    funcctx->arg_list = arg_list;
    funcctx->arg_count = arg_count;

    if ( main_found)
    {
      funcctx->is_main = 1;
      filectx->main_funcctx = funcctx;
    }
    /* Match the argument list and insert arguments as local variables */
    if ( !funcctx->is_main)
    {
      sts = ccm_extract_parenthes_expr( expr, func_p->start_line->line);
      nr = rtt_parse( expr, ",", "", (char *)arg_str, 
		sizeof( arg_str) / sizeof( arg_str[0]), sizeof( arg_str[0]), 0);
      if ( nr != arg_count)
        return CCM__ARGMISM;
      arg_p = arg_list;
      for ( i = 0; i < arg_count; i++)
      {
        ccm_remove_blank( arg_str[i], arg_str[i]);
        nr = rtt_parse( arg_str[i], " 	", "", (char *)elm_str, 
		sizeof( elm_str) / sizeof( elm_str[0]), sizeof( elm_str[0]), 0);
        if ( nr != 2)
          return CCM__SYNTAX;
        if ( strcmp( elm_str[0], "float") == 0)
        {
          sts = ccm_createvar( elm_str[1], K_DECL_FLOAT, 0, 0, NULL,
		&funcctx->locint_list, &funcctx->locfloat_list, &funcctx->locstring_list);
          if ( EVEN(sts)) return sts;
          sts = ccm_setvar( funcctx, elm_str[1], arg_p->value_decl, arg_p->value_float,
		arg_p->value_int, arg_p->value_string);
          if ( EVEN(sts)) return sts;
          strcpy( arg_p->var_name, elm_str[1]);
          arg_p->var_decl = K_DECL_FLOAT;
        }
        else if ( strcmp( elm_str[0], "int") == 0)
        {
          sts = ccm_createvar( elm_str[1], K_DECL_INT, 0, 0, NULL,
		&funcctx->locint_list, &funcctx->locfloat_list, &funcctx->locstring_list);
          if ( EVEN(sts)) return sts;
          sts = ccm_setvar( funcctx, elm_str[1], arg_p->value_decl, arg_p->value_float,
		arg_p->value_int, arg_p->value_string);
          if ( EVEN(sts)) return sts;
          strcpy( arg_p->var_name, elm_str[1]);
          arg_p->var_decl = K_DECL_INT;
        }
        else if ( strcmp( elm_str[0], "string") == 0)
        {
          sts = ccm_createvar( elm_str[1], K_DECL_STRING, 0, 0, NULL,
		&funcctx->locint_list, &funcctx->locfloat_list, &funcctx->locstring_list);
          if ( EVEN(sts)) return sts;
          sts = ccm_setvar( funcctx, elm_str[1], arg_p->value_decl, arg_p->value_float,
		arg_p->value_int, arg_p->value_string);
          if ( EVEN(sts)) return sts;
          strcpy( arg_p->var_name, elm_str[1]);
          arg_p->var_decl = K_DECL_STRING;
        }
        else
          return CCM__UNKNVARTYPE;
        arg_p = arg_p->next;
      }
    }
    else
    {
      /* Save the arguments as p1 - p9 */
      arg_p = arg_list;
      for ( i = 1; i <= 9; i++)
      {
        sprintf( arg_name, "p%d", i);
        if ( arg_p)
        {
          sts = ccm_createvar( arg_name, K_DECL_STRING, 0, 0, 
		arg_p->value_string, &funcctx->locint_list, 
		&funcctx->locfloat_list, &funcctx->locstring_list);
          if ( EVEN(sts)) return sts;
          arg_p = arg_p->next;
        }
        else
        {
          sts = ccm_createvar( arg_name, K_DECL_STRING, 0, 0, 
		"", &funcctx->locint_list, 
		&funcctx->locfloat_list, &funcctx->locstring_list);
          if ( EVEN(sts)) return sts;
        }
      }
    }

    funcctx->for_init = 1;
  }
  return_found = 0;
  for ( line_p = start_line; line_p != end_line && line_p; 
		line_p = line_p->next)
  {
    if ( filectx->verify && line_p->type != K_LINE_STATMENT)
    {
      if ( filectx->break_before)
        printf( "\nCCM-I-VERIFY, \"%s\"", line_p->line);
      else
        printf( "CCM-I-VERIFY, \"%s\"\n", line_p->line);
    }

    filectx->current_row = line_p->row;

    if ( ccm_testmode)
      printf( "%s", line_p->line);

    switch ( line_p->type)
    {
      case K_LINE_MAIN:
      case K_LINE_FUNCTION:
	break;
      case K_LINE_IF:
        sts = ccm_extract_parenthes_expr( expr, line_p->line);
	if ( EVEN(sts)) return sts;
        sts = ccm_line_exec( funcctx, expr, &decl, &float_val, &int_val, string_val);
        if ( EVEN(sts))
	{
 	  if ( ! funcctx->filectx->error_row)
	  {
 	    funcctx->filectx->error_row = line_p->row;
	    strcpy( funcctx->filectx->error_line, line_p->line);
	  }
          return sts;
        }
        if (sts == CCM__EXITFUNC) return sts;
	if ( (decl == K_DECL_INT && int_val == 0) ||
	     (decl == K_DECL_FLOAT && fabs(float_val) < FLT_EPSILON))
        {
	  /* Find next matching endif */
          if_level = 0;
          found = 0;
          for ( l_p = line_p->next; l_p; l_p = l_p->next)
          {
            if ( l_p->type == K_LINE_IF)
              if_level++;
            else if ( l_p->type == K_LINE_ENDIF)
	    {
              if ( if_level == 0)
              {
	        found = 1;
                break;
              }
              else
                if_level--;
            }
            else if ( l_p->type == K_LINE_ELSE)
	    {
              if ( if_level == 0)
              {
	        found = 1;
                break;
              }
            }
          }
          if ( !found)
            return CCM__IFMISM;
          line_p = l_p;
        }
        break;
      case K_LINE_ELSE:
	/* Find next matching endif */
        if_level = 0;
        found = 0;
        for ( l_p = line_p->next; l_p; l_p = l_p->next)
        {
          if ( l_p->type == K_LINE_IF)
            if_level++;
          else if ( l_p->type == K_LINE_ENDIF)
	  {
            if ( if_level == 0)
            {
	      found = 1;
              break;
            }
            else
              if_level--;
          }
        }
        if ( !found)
          return CCM__IFMISM;
        line_p = l_p;
        break;
      case K_LINE_ENDIF:
        break;
      case K_LINE_WHILE:
        sts = ccm_extract_parenthes_expr( expr, line_p->line);
	if ( EVEN(sts)) return sts;
        sts = ccm_line_exec( funcctx, expr, &decl, &float_val, &int_val, string_val);
        if ( EVEN(sts))
	{
 	  if ( ! funcctx->filectx->error_row)
	  {
 	    funcctx->filectx->error_row = line_p->row;
	    strcpy( funcctx->filectx->error_line, line_p->line);
	  }
          return sts;
        }
        if (sts == CCM__EXITFUNC) return sts;
	if ( (decl == K_DECL_INT && int_val == 0) ||
	     (decl == K_DECL_FLOAT && fabs(float_val) < FLT_EPSILON))
        {
	  /* Find next matching endwhile */
          while_level = 0;
          found = 0;
          for ( l_p = line_p->next; l_p; l_p = l_p->next)
          {
            if ( l_p->type == K_LINE_WHILE)
              while_level++;
            else if ( l_p->type == K_LINE_ENDWHILE)
	    {
              if ( while_level == 0)
              {
	        found = 1;
                break;
              }
              else
                while_level--;
            }
          }
          if ( !found)
            return CCM__WHILEMISM;
          line_p = l_p;
        }
        break;
      case K_LINE_ENDWHILE:
	/* Find previous matching while */
        while_level = 0;
        found = 0;
        for ( l_p = line_p->prev; l_p; l_p = l_p->prev)
        {
          if ( l_p->type == K_LINE_ENDWHILE)
            while_level++;
          else if ( l_p->type == K_LINE_WHILE)
	  {
            if ( while_level == 0)
            {
	      found = 1;
              break;
            }
            else
              while_level--;
          }
        }
        if ( !found)
          return CCM__WHILEMISM;
        line_p = l_p->prev;
        break;
      case K_LINE_FOR:
        sts = ccm_extract_parenthes_expr( expr, line_p->line);
	if ( EVEN(sts)) return sts;
        memset( elm_str, 0, sizeof( elm_str));
        nr = rtt_parse( expr, ";", "", (char *)elm_str, 
		sizeof( elm_str) / sizeof( elm_str[0]), sizeof( elm_str[0]), 0);
        if ( nr > 3)
          return CCM__SYNTAX;
        if ( nr < 3)
        {
          /* The parser doesn't count the null-strings */
          if ( expr[0] == ';')
          {
            strcpy( elm_str[2], elm_str[1]);
            strcpy( elm_str[1], elm_str[0]);
            strcpy( elm_str[0], "");
            if ( expr[1] == ';')
            {
              strcpy( elm_str[2], elm_str[1]);
              strcpy( elm_str[1], elm_str[0]);
              strcpy( elm_str[0], "");
            }
          }
        }

        if ( funcctx->for_init)
          sts = ccm_line_exec( funcctx, elm_str[0], &decl, &float_val, &int_val, string_val);
        else
          sts = ccm_line_exec( funcctx, elm_str[2], &decl, &float_val, &int_val, string_val);
        if ( EVEN(sts))
	{
 	  if ( ! funcctx->filectx->error_row)
	  {
 	    funcctx->filectx->error_row = line_p->row;
	    strcpy( funcctx->filectx->error_line, line_p->line);
	  }
          return sts;
        }
        if (sts == CCM__EXITFUNC) return sts;
        funcctx->for_init = 1;
        ccm_remove_blank( elm_str[1], elm_str[1]);
        if ( strcmp( elm_str[1], "") == 0)
        {
          /* Null-string is always true */
          decl = K_DECL_INT;
          int_val = 1;
        }
        else
        {
          sts = ccm_line_exec( funcctx, elm_str[1], &decl, &float_val, &int_val, string_val);
          if ( EVEN(sts))
	  {
 	    if ( ! funcctx->filectx->error_row)
	    {
 	      funcctx->filectx->error_row = line_p->row;
	      strcpy( funcctx->filectx->error_line, line_p->line);
	    }
            return sts;
          }
          if (sts == CCM__EXITFUNC) return sts;
        }
	if ( (decl == K_DECL_INT && int_val == 0) ||
	     (decl == K_DECL_FLOAT && fabs(float_val) < FLT_EPSILON))
        {
	  /* Find next matching endfor */
          for_level = 0;
          found = 0;
          for ( l_p = line_p->next; l_p; l_p = l_p->next)
          {
            if ( l_p->type == K_LINE_FOR)
              for_level++;
            else if ( l_p->type == K_LINE_ENDFOR)
	    {
              if ( for_level == 0)
              {
	        found = 1;
                break;
              }
              else
                for_level--;
            }
          }
          if ( !found)
            return CCM__FORMISM;
          line_p = l_p;
        }
        break;
      case K_LINE_ENDFOR:
	/* Find previous matching for */
        for_level = 0;
        found = 0;
        for ( l_p = line_p->prev; l_p; l_p = l_p->prev)
        {
          if ( l_p->type == K_LINE_ENDFOR)
            for_level++;
          else if ( l_p->type == K_LINE_FOR)
	  {
            if ( for_level == 0)
            {
	      found = 1;
              break;
            }
            else
              for_level--;
          }
        }
        if ( !found)
          return CCM__FORMISM;
        funcctx->for_init = 0;
        line_p = l_p->prev;
        break;
      case K_LINE_CONTINUE:
	/* Find previous matching for or while statement */
        found = 0;
        for_level = 0;
        while_level = 0;
        for ( l_p = line_p->prev; l_p; l_p = l_p->prev)
        {
          if ( l_p->type == K_LINE_ENDFOR)
            for_level++;
          else if ( l_p->type == K_LINE_FOR)
	  {
            if ( for_level == 0)
            {
	      found = 1;
              funcctx->for_init = 0;
              break;
            }
            else
              for_level--;
          }
          if ( l_p->type == K_LINE_ENDWHILE)
            while_level++;
          else if ( l_p->type == K_LINE_WHILE)
	  {
            if ( while_level == 0)
            {
	      found = 1;
              break;
            }
            else
              while_level--;
          }
        }
        if ( !found)
          return CCM__CONTMISM;
        line_p = l_p->prev;
        break;
      case K_LINE_BREAK:
	/* Find next matching endfor or endwhile statement */
        found = 0;
        for_level = 0;
        while_level = 0;
        for ( l_p = line_p->next; l_p; l_p = l_p->next)
        {
          if ( l_p->type == K_LINE_FOR)
            for_level++;
          else if ( l_p->type == K_LINE_ENDFOR)
	  {
            if ( for_level == 0)
            {
	      found = 1;
              break;
            }
            else
              for_level--;
          }
          else if ( l_p->type == K_LINE_WHILE)
            while_level++;
          else if ( l_p->type == K_LINE_ENDWHILE)
	  {
            if ( while_level == 0)
            {
	      found = 1;
              break;
            }
            else
              while_level--;
          }
        }
        if ( !found)
          return CCM__BREAKMISM;
        line_p = l_p;
        break;
      case K_LINE_LABEL:
        break;
      case K_LINE_GOTO:
	/* Find matching label inside this function */
	strcpy( goto_label, &line_p->line[5]);
        goto_label[strlen(goto_label)-1] = 0;
        ccm_remove_blank( goto_label, goto_label);

        found = 0;
        for ( l_p = start_line; l_p != end_line && l_p; 
		l_p = l_p->next)
        {
          if ( l_p->type == K_LINE_LABEL)
          {
	    strcpy( label, l_p->line);
            label[strlen(label)-1] = 0;
            ccm_remove_blank( label, label);
            if ( strcmp( label, goto_label) == 0)
            {
              found = 1;
              break;
            }
          }
        }
        if ( !found)
          return CCM__GOTOMISM;
        line_p = l_p;
        break;
      default:

        ccm_remove_blank( line_p->line, line_p->line);
	if ( strcmp( line_p->line, "") == 0)
          break;
        else if ( line_p->line[strlen(line_p->line)-1] != ';')
        {
          /* This is an external command */
          ccm_replace_symbol( funcctx, line_p->line, expr);

          if ( filectx->verify)
          { 
            if ( filectx->break_before)
              printf( "\nCCM-I-VERIFY, \"%s\"", expr);
            else
              printf( "CCM-I-VERIFY, \"%s\"\n", expr);
          }

          if ( funcctx->is_main && filectx->extfunc_return_mode)
          {
            funcctx->current_line = line_p;
	    strcpy( filectx->extfunc_line, expr);
            return CCM__EXTERNFUNC;
          }
          if ( funcctx->filectx->externcmd_func)
            sts = (funcctx->filectx->externcmd_func)( 
			expr, funcctx->filectx->client_data);
        }
        else
        {
          if ( filectx->verify)
          {
            if ( filectx->break_before)
              printf( "\nCCM-I-VERIFY, \"%s\"", line_p->line);
            else
              printf( "CCM-I-VERIFY, \"%s\"\n", line_p->line);
          }
          sts = ccm_line_exec( funcctx, line_p->line, &decl, &float_val, &int_val, string_val);
          if ( EVEN(sts))
	  {
 	    if ( ! funcctx->filectx->error_row)
	    {
 	      funcctx->filectx->error_row = line_p->row;
	      strcpy( funcctx->filectx->error_line, line_p->line);
	    }
            return sts;
          }
          if ( EVEN(sts)) return sts;
          if ( ccm_testmode)
            printf( "%s\n decl: %d, float: %f, int: " ccm_cIntFormat " str: %s\n", line_p->line, decl,
       	  float_val, int_val, string_val);

          if ( sts == CCM__EXITFUNC)
          {
            *return_int = int_val;
            *return_decl = decl;
            return_found = 1;
          }
          else if ( strncmp( line_p->line, "return", 6) == 0 &&
               (line_p->line[6] == ' ' || line_p->line[6] == '	'))
          {
            /* Return statement, set return value and exit function */
            if ( func_p->decl == K_DECL_INT &&  decl == K_DECL_INT)
              *return_int = int_val;
            else if ( func_p->decl == K_DECL_INT &&  decl == K_DECL_FLOAT)
              *return_int = float_val;
            else if ( func_p->decl == K_DECL_FLOAT &&  decl == K_DECL_INT)
              *return_float = int_val;
            else if ( func_p->decl == K_DECL_FLOAT &&  decl == K_DECL_FLOAT)
              *return_float = float_val;
            else if ( func_p->decl == K_DECL_STRING &&  decl == K_DECL_STRING)
              strcpy( return_string, string_val);
            else if ( func_p->decl == K_DECL_STRING)
              strcpy( return_string, "");
            *return_decl = func_p->decl;
            return_found = 1;
          }
        }
    }
    if ( return_found)
      break;
  }

  /* Modify values in argument list */
  if ( !funcctx->is_main)
  {
    arg_p = funcctx->arg_list;
    for ( i = 0; i < funcctx->arg_count; i++)
    {
      if ( arg_p->value_type == K_OPERAND_NAME)
      {
         sts = ccm_getvar( funcctx, arg_p->var_name, &decl, 
			&float_val, &int_val, string_val);
         if ( EVEN(sts)) return sts;

	 if ( arg_p->var_decl == arg_p->value_decl)
	 {
	   if ( decl == K_DECL_FLOAT)
             arg_p->value_float = float_val;
	   else if ( decl == K_DECL_INT)
             arg_p->value_int = int_val;
	   else if ( decl == K_DECL_STRING)
             strcpy( arg_p->value_string, string_val);
	   arg_p->value_returned = 1;
        }
      }
      arg_p = arg_p->next;
    }
  }

  /* Free variablelists and funcctx */
  ccm_free_varlists( funcctx->locint_list, funcctx->locfloat_list,
	funcctx->locstring_list);
  free( (char *) funcctx);

  if ( sts == CCM__EXITFUNC)
    return sts;
  return 1;
}


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
)
{
  int		decl;
  ccm_tInt     	int_val;
  ccm_tFloat   	float_val;
  int		i;
  ccm_tString  	string_val;
  int		sts;
  ccm_tFileCtx filectx;
  char		elm_str[9][K_LINE_SIZE];
  int		nr;
  ccm_sArg	*arg_list, *arg_p, *a_p, *next_arg;
  int		arg_count;

  if ( resume)
  {
    filectx = *ctx;
  }
  else
  {
    /* Parse the commandline */
    nr = rtt_parse( cmd, " 	", "", (char *)elm_str, 
	sizeof( elm_str) / sizeof( elm_str[0]), sizeof( elm_str[0]), 0);
    if ( nr < 1)
      return CCM__SYNTAX;

    /* Create an argumentlist */
    arg_count = 0;
    arg_list = 0;
    for ( i = 1; i < nr; i++)
    {
      arg_p = calloc( 1, sizeof( ccm_sArg));
      strcpy( arg_p->value_string, elm_str[i]);
      arg_p->value_decl = K_DECL_STRING;
      if ( arg_list == 0)
        arg_list = arg_p;
      else
      {
        for ( a_p = arg_list; a_p->next; a_p = a_p->next) ;
        a_p->next = arg_p;
      }
      arg_count++;
    }

    filectx = calloc( 1, sizeof( *filectx));
    filectx->deffilename_func = deffilename_func;
    filectx->externcmd_func = externcmd_func;
    filectx->errormessage_func = errormessage_func;
    filectx->verify = verify;
    filectx->break_before = break_before;
    filectx->extfunc_return_mode = extfunc_return_mode;
    filectx->main_arg_list = arg_list;
    filectx->main_arg_count = arg_count;
    filectx->client_data = client_data;
    if ( ctx)
      *ctx = filectx;

    sts = ccm_read_file( filectx, elm_str[0], &filectx->line_list);
    if ( EVEN(sts)) goto file_exec_exit;

    sts = ccm_init_filectx( filectx);
    if ( EVEN(sts)) goto file_exec_exit;
  }

  sts = ccm_function_exec( filectx, "main", arg_list, arg_count, 
	&decl, &float_val, &int_val, string_val, resume);
  if ( sts == CCM__EXTERNFUNC)
  {
    strcpy( extfunc_line, filectx->extfunc_line);
    return sts;
  }
  if ( sts == CCM__EXITFUNC)
    *appl_sts = int_val;
  else
    *appl_sts = 1;

file_exec_exit:
  if ( EVEN(sts))
    ccm_print_error( filectx, sts);

  for ( arg_p = filectx->main_arg_list; arg_p; arg_p = next_arg)
  {
    next_arg = arg_p->next;
    free( (char *)arg_p);
  }
  ccm_free_filectx( filectx);

  return sts;
}

#if 0
/************************* TEST ***********************************/
static int test_deffilename_func( char *outfile, char *infile, void *client_data)
{
  strcpy( outfile, infile);
  printf( "Default filename: %s\n", outfile);
  return 1;
}

static int test_externcmd_func( char *cmd, void *client_data)
{
  printf( "Extern cmd: %s\n", cmd);
  return 1;
}

main()
{
  char		str[K_LINE_SIZE];
  int		sts;
  int		appl_sts;
  void		*ccm_ctx;
  char		cmd[256];
  int		resume;

  strcpy( str, "ccm.txt argument1  argument2 argument3");
/****
  sts = ccm_file_exec( str, test_externcmd_func, 
		test_deffilename_func, NULL, &appl_sts, 1, NULL, 0, 0, NULL);

  printf( "Appl sts : %d\n", appl_sts);
***/
  resume = 0;
  for (;;)
  {
    sts = ccm_file_exec( str, test_externcmd_func, 
		test_deffilename_func, NULL, &appl_sts, 1, &ccm_ctx, 1, 
		resume, cmd, NULL);
    if ( sts == CCM__EXTERNFUNC)
    {
      printf( "Extern cmd returnmode: %s\n", cmd);
      resume = 1;
    }
    else
    {
      printf( "Appl sts : %d\n", appl_sts);
      break;
    }
  }
  exit(1);
}
#endif

