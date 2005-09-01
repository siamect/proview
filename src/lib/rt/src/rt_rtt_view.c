/* 
 * Proview   $Id: rt_rtt_view.c,v 1.2 2005-09-01 14:57:56 claes Exp $
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
 */

/* rt_rtt_view.c 
   This module contains routines for the view function in rtt. */

/*_Include files_________________________________________________________*/

#if defined(OS_ELN)
# include $vaxelnc
# include stdio
# include stdlib
# include string
# include ctype
# include chfdef
#else
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <ctype.h>
#endif

#include "pwr.h"
#include "pwr_class.h"
#include "rt_gdh.h"
#include "rt_rtt.h"
#include "rt_rtt_global.h"
#include "rt_rtt_functions.h"
#include "rt_rtt_msg.h"

/* Nice functions */
#define ODD(a)	(((int)(a) & 1) != 0)
#define EVEN(a)	(((int)(a) & 1) == 0)
#define max(Dragon,Eagle) ((Dragon) > (Eagle) ? (Dragon) : (Eagle))
#define min(Dragon,Eagle) ((Dragon) < (Eagle) ? (Dragon) : (Eagle))
#ifndef __ALPHA
#define abs(Dragon) ((Dragon) >= 0 ? (Dragon) : (-(Dragon)))
#endif

#define RTTVIEW_ARROW_INC 5
#define RTTVIEW_PAGE_INC 20
#define RTTVIEW_BUFF_SIZE 512000
#define RTTVIEW_BUFFROW_SIZE 2000

/*__Local function prototypes___________________________________________*/

#if 0
static void	rtt_scroll_up( int nr);
static void	rtt_scroll_down( int nr);
#endif
static int	rtt_view_get_row_size( 
				char *row, 
				int size, 
				int *number_of_char,
				char *last_char);
static int	rtt_read_buff( 	view_ctx ctx,
			char 	*buff, 
			int	buff_size,
			int 	*buffrow, 
			int	buffrow_size,
			int 	*buffrow_count, 
			int	direction, 
			int	page_size,
			int	*start_row);
static int	rtt_read_file( 	view_ctx ctx,
			char 	*buff, 
			int	buff_size,
			int 	*buffrow, 
			int	buffrow_size,
			int 	*buffrow_count, 
			int	direction, 
			int	page_size,
			int	*start_row);

static char *rttview_gets( char *str, int size, char  *inbuff, int *str_offset);


#if 0
static void	rtt_scroll_up( int nr)
{
	char	scroll[]={27, 91, 0, 0, 'S', 0};
	scroll[2] = nr / 10 + 48;
	scroll[3] = nr - (nr / 10) * 10 + 48;
	r_print("%s", scroll);
}
static void	rtt_scroll_down( int nr)
{
	char	scroll[]={27, 91, 0, 0, 'T', 0};
	scroll[2] = nr / 10 + 48;
	scroll[3] = nr - (nr / 10) * 10 + 48;
	r_print("%s", scroll);
}
#endif

static int	rtt_view_get_row_size( 
				char *row, 
				int size, 
				int *number_of_char,
				char *last_char)
		
{
	char	*s;
	int	display_size, i;

	s = row;
	display_size = 0;
	for ( i = 0; i < size; i++)
	{
	  if ( !*s || *s == 12 || *s == 10)
	    break;

	  if ( *s == 9)
	    display_size = display_size / 8 * 8 + 8;
	  else
	    display_size++;
	  if ( display_size > size)
	    break;
	  s++;
	}
	*number_of_char = i + 1;
	*last_char = *s;
	return RTT__SUCCESS;
}

int	rtt_view_search( view_ctx ctx,
			 char	*search_string)
{
	int	i;
	char	*s;
	int	offset;

	if ( !ctx->buff_read_complete)
	{
	  rtt_message('E',"File too large");
	  return RTT__NOPICTURE;
	}
	else
	{
	  /* Find the current offset */
	  if ( ctx->start_row >= ctx->buffrow_count -1)
	  {
	    rtt_message('E',"String not found");
	    return RTT__NOPICTURE;
	  }
	
	  offset = ctx->buffrow[ctx->start_row +1];

	  s = strstr( &ctx->buff[offset], search_string);
	  if ( !s)
	  {
	    rtt_message('E',"String not found");
	    return RTT__NOPICTURE;
	  }

	  /* Find the line */
	  offset = s - ctx->buff;

	  for ( i = 0; i < ctx->buffrow_count; i ++)
	  {
	    if ( ctx->buffrow[i] > offset )
	    {
	      ctx->start_row = i - 1;
	      if ( ctx->start_row < 0) 
	        ctx->start_row = 0;
	      break;
	    }
	  }
	}
	return RTT__SUCCESS;
}

static int	rtt_read_file( 	view_ctx ctx,
			char 	*buff, 
			int	buff_size,
			int 	*buffrow, 
			int	buffrow_size,
			int 	*buffrow_count, 
			int	direction, 
			int	page_size,
			int	*start_row)
{
	int	len;
	char	str[2000];
	int		i;
	int		back_jump_rows;
	int		old_buffstart_row;


	back_jump_rows = buff_size - 50;
	old_buffstart_row = ctx->buffstart_row;

	if ( ctx->first)
	{
	  ctx->first = 0;
	}
	if ( direction == 0)
	{
	  /* Forward */
	  if (ctx->read_sts == 0 || ctx->buff_read_complete == 1)
	  {
	    if ( *start_row + page_size > *buffrow_count)
	    {
	      *start_row = *buffrow_count - page_size;
	    }
	    if ( *start_row < 0)
	      *start_row = 0;
	    return 1;
	  }

	  if ( ctx->start_row + page_size > ctx->buffrow_count )
	  {
	    if (  *start_row + page_size > *buffrow_count)
	    {
	      ctx->buffstart_row += *start_row;
	      *start_row = 0;
	    }
          }
	  if ( fseek( ctx->infile, 0,0) != 0)
	    return 0;
/*	  sts = fseek( infile, buffstart_offset, 1);*/
	  for ( i = 0; i < ctx->buffstart_row; i++)
	  {
	    if ( fgets( str, sizeof(str), ctx->infile) == NULL)
	      break;
	  }
	  *buffrow_count = 0;
	}
	else
	{
	  /* Backward */
	  if ( ctx->buff_read_complete == 1)
	  {
	    if ( *start_row < 0)
	      *start_row = 0;
	    return 1;
	  }
	  ctx->read_sts = (char *) 1;
	  if ( *start_row < 0)
	  {
	    if ( back_jump_rows <= ctx->buffstart_row)
	    {
	      ctx->buffstart_row -= back_jump_rows;
	      *start_row += back_jump_rows;
	    }
	    else
	    {
	      *start_row += ctx->buffstart_row;
	      if ( *start_row < 0)
	        *start_row = 0;
	      ctx->buffstart_row = 0;
	    }
	  }

	  if ( fseek( ctx->infile, 0,0) != 0)
	    return 0;
/*	  sts = fseek( infile, buffstart_offset, 1);*/
	  for ( i = 0; i < ctx->buffstart_row; i++)
	  {
	    if ( fgets( str, sizeof(str), ctx->infile) == NULL)
	      break;
	  }
	  *buffrow_count = 0;
	}

	len = 0;
	while ( ctx->read_sts != NULL &&
		len < buff_size - (int)sizeof(str) &&
		*buffrow_count < buffrow_size)
	{
	  ctx->read_sts = fgets( str, sizeof(str), ctx->infile);
	  if ( ctx->read_sts == NULL)
	    break;
	  buffrow[*buffrow_count] = len;
	  strcpy( buff + len, str);
	  len += strlen(str);
	  (*buffrow_count)++;
	}
	if ( *start_row > *buffrow_count)
	  *start_row = *buffrow_count - page_size;

	if (ctx->read_sts == NULL)
	{
	  if ( *start_row + page_size > *buffrow_count)
	    *start_row = *buffrow_count - page_size;
	  if ( ctx->buffstart_row == 0)
	    ctx->buff_read_complete = 1;
	}

	if ( *start_row < 0)
	  *start_row = 0;
	return 1;
}

static int	rtt_read_buff(  view_ctx ctx,
			char 	*buff, 
			int	buff_size,
			int 	*buffrow, 
			int	buffrow_size,
			int 	*buffrow_count, 
			int	direction, 
			int	page_size,
			int	*start_row)
{
	int	len;
	char	str[2000];
	int		i;
	int		back_jump_rows;
	int		old_buffstart_row;
	int		inbuff_offset;


	inbuff_offset = 0;
	back_jump_rows = buff_size - 50;
	old_buffstart_row = ctx->buffstart_row;

	if ( ctx->first)
	{
	  ctx->first = 0;
	}
	if ( direction == 0)
	{
	  /* Forward */
	  if (ctx->read_sts == 0 || ctx->buff_read_complete == 1)
	  {
	    if ( *start_row + page_size > *buffrow_count)
	    {
	      *start_row = *buffrow_count - page_size;
	    }
	    if ( *start_row < 0)
	      *start_row = 0;
	    return 1;
	  }

	  if (  *start_row + page_size > *buffrow_count)
	  {
	    ctx->buffstart_row += *start_row;
	    *start_row = 0;
	  }
/*	  sts = fseek( infile, buffstart_offset, 1);*/
	  for ( i = 0; i < ctx->buffstart_row; i++)
	  {
	    if ( rttview_gets( str, sizeof(str), ctx->inbuff, &inbuff_offset) 
				== NULL)
	      break;
	  }
	  *buffrow_count = 0;
	}
	len = 0;
	while ( ctx->read_sts != NULL &&
		len < buff_size - (int)sizeof(str) &&
		*buffrow_count < buffrow_size)
	{
	  ctx->read_sts = rttview_gets( str, sizeof(str), ctx->inbuff, 
			&inbuff_offset);
	  buffrow[*buffrow_count] = len;
	  strcpy( buff + len, str);
	  len += strlen(str);
	  (*buffrow_count)++;
	}
	if ( *start_row > *buffrow_count)
	  *start_row = *buffrow_count - page_size;

	if (ctx->read_sts == NULL)
	{
	  if ( *start_row + page_size > *buffrow_count)
/*	    *start_row = *buffrow_count - page_size;
*/
	  if ( ctx->buffstart_row == 0)
	    ctx->buff_read_complete = 1;
	}
	if ( *start_row < 0)
	  *start_row = 0;

	return 1;
}

static char *rttview_gets( char *str, int size, char  *inbuff, 
		int *inbuff_offset)
{
	char *s;

	s = inbuff + *inbuff_offset;
	while( ! (*s == 10 || *s== 0))
	{
	  *str = *s;
	  s++;
	  str++;
	  (*inbuff_offset)++;
	}
	*str = 10;
	str++;
	*str = 0;
	if (*s == 0)
	  return NULL;
	s++;
	(*inbuff_offset)++;
	if (*s == 13)
	{
	  (*inbuff_offset)++;
	}
	return (char *) 1;
}

int	rtt_view_buffer(
			menu_ctx	parent_ctx,
			pwr_tObjid	objid,
			char		*filename, 
			char		*inbuff,
			char		*intitle,
			int		type)
{
	int	sts;

	sts = rtt_view(	parent_ctx, filename, inbuff, intitle, type);
	return sts;
}

/*************************************************************************
*
* Name:		rtt_view()
*
* Type		int
*
* Type		Parameter	IOGF	Description
* unsigned long	parent_ctx	I	parents rtt context.
* rtt_t_menu	**menu_p	I	menu list.
* char		*title		I	menu title.
* unsigned long	userdata	I	...
* unsigned long	flag		I	menu type
*
* Description:
*	Create a view window.
*
**************************************************************************/

int	rtt_view(	menu_ctx	parent_ctx,
			char		*filename, 
			char		*inbuff,
			char		*intitle,
			int		type)
{
	view_ctx	ctx;
	unsigned long	terminator;
	char		input_str[80];
	int		maxlen = 30;
	unsigned long	option;
	int		sts;
	int	i, start_i, end_i;
	int	size, offset;
	int	left_marg;
	int	page_size = 21;
	int	row_change;
	int	redraw = 1;
	char	str[100];
	char	title[80];
	FILE	*infile;
	char	last_char;
	char	pagestr[80];

	
	if ( type == RTT_VIEWTYPE_FILE)
	{
	  infile = fopen( filename, "r");
	  if ( !infile)
	  {
	    rtt_message('E',"Unable to open file");
	    return RTT__NOPICTURE;
	  }
	  rtt_fgetname( infile, title, filename);
	}
	else
	{
	  strcpy( title, intitle);
	}

	ctx = calloc( 1, sizeof( *ctx));
	if ( !ctx)
	{
	  rtt_message('E',"Unable to allocate memory");
	  return RTT__NOPICTURE;
	}
	rtt_ctx_push( (menu_ctx) ctx);
	ctx->ctx_type = RTT_CTXTYPE_VIEW;
	strcpy( ctx->title, title); 
	ctx->parent_ctx = parent_ctx;
	ctx->first = 1;
	if ( type == RTT_VIEWTYPE_FILE)
	  ctx->infile = infile;
	else
	  ctx->inbuff = inbuff;
	ctx->read_sts = (char *) 1;
	ctx->buff = calloc( 1, RTTVIEW_BUFF_SIZE);
	if ( !ctx->buff)
	{
	  rtt_ctx_pop();
	  rtt_message('E',"Unable to allocate memory");
	  return RTT__NOPICTURE;
	}
	ctx->buffrow = calloc( RTTVIEW_BUFFROW_SIZE, sizeof(*ctx->buffrow));
	if ( !ctx->buffrow)
	{
	  rtt_ctx_pop();
	  rtt_message('E',"Unable to allocate memory");
	  return RTT__NOPICTURE;
	}
	ctx->buffrow_count = 0;
	ctx->start_row = 0;
	left_marg = 0;
	row_change = 0;

	if ( type == RTT_VIEWTYPE_FILE)
	  rtt_read_file( ctx, ctx->buff, RTTVIEW_BUFF_SIZE, ctx->buffrow,
		RTTVIEW_BUFFROW_SIZE, &ctx->buffrow_count,
		0, page_size, &ctx->start_row);
	else
	  rtt_read_buff( ctx, ctx->buff, RTTVIEW_BUFF_SIZE, ctx->buffrow,
		RTTVIEW_BUFFROW_SIZE, &ctx->buffrow_count,
		0, page_size, &ctx->start_row);

	option = RTT_OPT_NORECALL | RTT_OPT_NOEDIT | RTT_OPT_NOECHO | 
		RTT_OPT_TIMEOUT;

	while (1)
	{

	  if ( ctx->start_row + page_size > ctx->buffrow_count )
	  {
	    if ( type == RTT_VIEWTYPE_FILE)
	      rtt_read_file( ctx, ctx->buff, RTTVIEW_BUFF_SIZE, ctx->buffrow,
		RTTVIEW_BUFFROW_SIZE, &ctx->buffrow_count,
		0, page_size, &ctx->start_row);
	    else
	      ctx->start_row = max( 0, ctx->buffrow_count - page_size);
/*
	      rtt_read_buff( ctx, ctx->buff, RTTVIEW_BUFF_SIZE, ctx->buffrow,
		RTTVIEW_BUFFROW_SIZE, &ctx->buffrow_count,
		0, page_size, &ctx->start_row);
*/
	    redraw = 1;
	  }
	  else if ( ctx->start_row < 0)
	  {
	    if ( type == RTT_VIEWTYPE_FILE)
	      rtt_read_file( ctx, ctx->buff, RTTVIEW_BUFF_SIZE, ctx->buffrow,
		RTTVIEW_BUFFROW_SIZE, &ctx->buffrow_count,
		1, page_size, &ctx->start_row);
	    else
	      ctx->start_row = 0;
/*
	      rtt_read_buff( ctx, ctx->buff, RTTVIEW_BUFF_SIZE, ctx->buffrow,
		RTTVIEW_BUFFROW_SIZE, &ctx->buffrow_count,
		1, page_size, &ctx->start_row);
*/
	    redraw = 1;
	  }
	  if ( redraw || row_change != 0)
	  {
	    redraw = 0;
	    rtt_display_erase();
	    start_i = ctx->start_row;
	    end_i = min(ctx->start_row+page_size, ctx->buffrow_count);
	    rtt_cursor_abs( 1, 22-page_size);
	    for ( i = ctx->start_row; i < end_i; i++)
	    {
	      if ( i == ctx->buffrow_count - 1)
	      {
	        offset = ctx->buffrow[i] + left_marg;
		rtt_view_get_row_size( &ctx->buff[offset], 80, &size, &last_char);
	        size = min( size, 80);
	        if ( last_char != 10)
	          r_print("%.*s\n\r", size,  &ctx->buff[offset]);
	        else
	          r_print("%.*s\r", size,  &ctx->buff[offset]);
/*
	        str[80] = 0;
	        r_print("%80s\n", str);
*/
	      }
	      else
	      {
	        offset = min( ctx->buffrow[i+1] - 1, ctx->buffrow[i] + left_marg);
	        size = min( ctx->buffrow[i+1] - offset, 80);
		rtt_view_get_row_size( &ctx->buff[offset], 80, &size, &last_char);
	        size = min( size, 80);
	        if ( last_char != 10)
	          r_print("%.*s\n\r", size,  &ctx->buff[offset]);
	        else
	          r_print("%.*s\r", size,  &ctx->buff[offset]);
	      }
	    }
	    rtt_cursor_abs( 1, 22);
	    rtt_char_inverse_start();
	    if ( end_i == ctx->buffrow_count)
	      sprintf( pagestr, "%d-EOF(%d)", start_i+1, ctx->buffrow_count);
	    else
	      sprintf( pagestr, "%d-%d(%d)", start_i+1, end_i, ctx->buffrow_count);
	    sprintf( str, 
"                                                    |%12s | PF4 back", pagestr);
	    strncpy( &str[1], title, min( strlen( title), 52));
	    r_print( "%80s", str);
	    rtt_char_inverse_end();
	    
	    rtt_cursor_abs( 1, 23);
	    rtt_eofline_erase();
	    rtt_cursor_abs( 1, 24);
	    rtt_eofline_erase();
	  }

	  row_change = 0;
	  r_print_buffer();


	  rtt_command_get_input_string( (char *) &rtt_chn, 
		input_str, &terminator, maxlen, 
		rtt_recallbuff, option, rtt_scantime, &rtt_scan, ctx, 
		NULL, RTT_COMMAND_PICTURE);
	  rtt_message('S',"");

	  switch ( terminator)
	  {
	    case RTT_K_ARROW_UP:
	      if ( type == RTT_VIEWTYPE_BUF)
	      {
	        if ( ctx->start_row - RTTVIEW_ARROW_INC < 0) 
	        {
		  ctx->start_row = 0;
	          row_change = RTTVIEW_ARROW_INC - ctx->start_row;
	        }
	        else
	        {
	          ctx->start_row -= RTTVIEW_ARROW_INC;
	          row_change = -RTTVIEW_ARROW_INC;
	        }
	        redraw = 0;
	      }
	      else
	      {
	        ctx->start_row -= RTTVIEW_ARROW_INC;
	        row_change = -RTTVIEW_ARROW_INC;
	        redraw = 0;
	      }
	      break;
	    case RTT_K_ARROW_DOWN:
	      ctx->start_row += RTTVIEW_ARROW_INC;
	      row_change = RTTVIEW_ARROW_INC;
	      redraw = 0;
	      break;
	    case RTT_K_ARROW_RIGHT:
	      left_marg += 8;
	      redraw = 1;
	      break;
	    case RTT_K_ARROW_LEFT:
	      redraw = 1;
	      left_marg -= 8;
	      if ( left_marg < 0)
	        left_marg = 0;
	      break;
	    case RTT_K_NEXTPAGE:
	      /* Next page */
	      ctx->start_row += RTTVIEW_PAGE_INC;
	      row_change = RTTVIEW_PAGE_INC;
	      redraw = 0;
	      break;
	    case RTT_K_PREVPAGE:
	      /* Previous page */
	      ctx->start_row -= RTTVIEW_PAGE_INC;
	      row_change = -RTTVIEW_PAGE_INC;
	      redraw = 0;
	      break;
	    case RTT_K_RETURN:
	      break;
	    case RTT_K_PF1:
	      /* Top */
	      ctx->buffstart_row = 0;
	      ctx->start_row = 0;
	      rtt_read_buff( ctx, ctx->buff, RTTVIEW_BUFF_SIZE, ctx->buffrow,
		  RTTVIEW_BUFFROW_SIZE, &ctx->buffrow_count,
		  1, page_size, &ctx->start_row);
	      redraw = 1;
	      break;
	    case RTT_K_PF2:
	      /* Bottom */
	      ctx->start_row = max( 0, ctx->buffrow_count - page_size);
	      if ( type == RTT_VIEWTYPE_FILE)
	      {
	        while( ctx->read_sts != NULL)
	        {
	          rtt_read_buff( ctx, ctx->buff, RTTVIEW_BUFF_SIZE, ctx->buffrow,
		    RTTVIEW_BUFFROW_SIZE, &ctx->buffrow_count,
		    0, page_size, &ctx->start_row);
	          ctx->start_row = ctx->buffrow_count - page_size + 1;
	        }
	      }
	      redraw = 1;
	      break;
	    case RTT_K_PF3:
	      rtt_message('E', "Function not defined");
	      break;
	    case RTT_K_PF4:
	      if ( type == RTT_VIEWTYPE_FILE)
	        fclose( ctx->infile);
	      free( ctx->buff);
	      free( ctx->buffrow);
	      free( ctx);
	      rtt_ctx_pop();
	      return RTT__SUCCESS;
	    case RTT_K_FAST_1:
	    case RTT_K_FAST_2:
	    case RTT_K_FAST_3:
	    case RTT_K_FAST_4:
	    case RTT_K_FAST_5:
	    case RTT_K_FAST_6:
	    case RTT_K_FAST_7:
	    case RTT_K_FAST_8:
	    case RTT_K_FAST_9:
	    case RTT_K_FAST_10:
	    case RTT_K_FAST_11:
	    case RTT_K_FAST_12:
	    case RTT_K_FAST_13:
	    case RTT_K_FAST_14:
	    case RTT_K_FAST_15:
	      rtt_fastkey = terminator - RTT_K_FAST;
	      sts = rtt_get_fastkey_type();
	      if ( sts == RTT__NOPICTURE)
	      {
	        sts = rtt_get_fastkey_picture( (menu_ctx) ctx);
	        if ( EVEN(sts)) return sts;
	        break;
	      }
	    case RTT_K_CTRLZ:
	      if ( type == RTT_VIEWTYPE_FILE)
	        fclose( ctx->infile);
	      free( ctx->buff);
	      free( ctx->buffrow);
	      free( ctx);
	      rtt_ctx_pop();
	      return RTT__FASTBACK;
	    case RTT_K_CTRLW:
	      redraw = 1;
	      break;
	    case RTT_K_CTRLK:
	      /* Acknowledge last alarm */
	      sts = rtt_alarm_ack_last();
	      break;
	    case RTT_K_CTRLL:
	      /* Change description mode */
	      if ( !rtt_description_on)
	        sts = rtt_cli( rtt_command_table, "SET DESCRIPTION", (void *) ctx, 0);
	      else
	        sts = rtt_cli( rtt_command_table, "SET NODESCRIPTION", (void *) ctx, 0);
	      break;
	    case RTT_K_CTRLV:
	      break;
	    case RTT_K_CTRLN:
	      break;
	    case RTT_K_DELETE:
	      break;
	    case RTT_K_COMMAND:
	      sts = rtt_get_command( (menu_ctx) ctx, (char *) &rtt_chn, 
		rtt_recallbuff,  
		0, 0, ctx, 
	    	"pwr_rtt> ", 0, RTT_ROW_COMMAND, rtt_command_table);
	      /* menu_ptr might have been changed */
	      if ( EVEN(sts)) return sts;
	      if ( sts == RTT__FASTBACK)
	      {
	        if ( type == RTT_VIEWTYPE_FILE)
	          fclose( ctx->infile);
	        free( ctx->buff);
	        free( ctx->buffrow);
	        free( ctx);
	        rtt_ctx_pop();
	        return RTT__FASTBACK;
	      }
	      if ( sts == RTT__BACK)
	      {
	        if ( type == RTT_VIEWTYPE_FILE)
	          fclose( ctx->infile);
	        free( ctx->buff);
	        free( ctx->buffrow);
	        free( ctx);
	        rtt_ctx_pop();
	        return RTT__SUCCESS;
	      }
	      if ( sts == RTT__BACKTOCOLLECT)
	      {
	        if ( type == RTT_VIEWTYPE_FILE)
	          fclose( ctx->infile);
	        free( ctx->buff);
	        free( ctx->buffrow);
	        free( ctx);
	        rtt_ctx_pop();
	        return RTT__BACKTOCOLLECT;
	      }
	      if ( sts != RTT__NOPICTURE)
	      {
	        redraw = 1;
	      } 
	      break;
	    case RTT_K_HELP:
	      /* Try to find subject in application help */
	      sts = rtt_help( parent_ctx, "VIEW WINDOW", rtt_appl_helptext);
	      if ( sts == RTT__NOHELPSUBJ)
	          rtt_help( parent_ctx, "OBJECT MENU", 
			(rtt_t_helptext *) rtt_command_helptext);
	      redraw = 1;
	      break;
	  } 
	}

	return RTT__SUCCESS;
}
