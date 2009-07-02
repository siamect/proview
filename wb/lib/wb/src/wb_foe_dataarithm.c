/* 
 * Proview   $Id: wb_foe_dataarithm.c,v 1.7 2006-04-26 07:30:54 claes Exp $
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

/* wb_foe_dataarithm.c
   Code generation for data dataarithm object.  */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "pwr.h"

#include "wb_foe_dataarithm.h"
#include "wb_foe_msg.h"

#ifndef max
#define max(Dragon,Eagle) ((Dragon) > (Eagle) ? (Dragon) : (Eagle))
#endif

#ifndef min
#define min(Dragon,Eagle) ((Dragon) < (Eagle) ? (Dragon) : (Eagle))
#endif

#ifndef abs
#define abs(Dragon) ((Dragon) >= 0 ? (Dragon) : (-(Dragon)))
#endif

typedef struct dataa_s_ctx	*dataa_ctx;

typedef	struct {
	char		item[80];
	char		delim_front[20];
	char		delim_back[20];
	pwr_tStatus	(* func) ( dataa_ctx, char *, char *, char *, int *,	
					char *);
	int		hit;
	int		hit_count;
} dataa_t_item;

struct dataa_s_ctx {
	int		line_count;
	int		comment;
	int		outdata1_eq_indata;
	int		outdata2_eq_indata;
	int		outdata3_eq_indata;
	int		outdata4_eq_indata;
	char		classdef_Da1[80];
	char		classdef_Da2[80];
	char		classdef_Da3[80];
	char		classdef_Da4[80];
	char		structdef_Da1[80];
	char		structdef_Da2[80];
	char		structdef_Da3[80];
	char		structdef_Da4[80];
	char		aliasdef_Da1[80];
	char		aliasdef_Da2[80];
	char		aliasdef_Da3[80];
	char		aliasdef_Da4[80];
	dataa_t_item	items[100];
};

/* Local function prototypes. */

static int dataa_parse (
  char	*string,
  char	*parse_char,
  char	*inc_parse_char,
  char	*outstr,
  int	max_rows,
  int 	max_cols
);
static int dataa_add_item (
  dataa_ctx	dataactx,
  char 		*item, 
  char		*delim_front,
  char		*delim_back,
  pwr_tStatus	(* func) ()
);

/*************************************************************************
*
* Name:		-
*
* Type		int
*
* Type		Parameter	IOGF	Description
* dataa_ctx	dataactx	I	dataa context
* char		*line		I	current line
* char		*pos		I	position in current line
* char		*out		IO	output buffer
* char		*var		I	name of pointer to dataarithm object
*
* Description:
*	Backcall funktions called when an item is found.
*
**************************************************************************/

/*************************************************************************
*
* Name:		dataa_comment_start
*
* Type		int
*
* Type		Parameter	IOGF	Description
* dataa_ctx	dataactx	I	dataa context
* char		*line		I	current line
* char		*pos		I	position in current line
* char		*out		IO	output buffer
* char		*var		I	name of pointer to dataarithm object
*
* Description:
*	Backcall funktions called when an comment start is found.
*	Set dataactx->comment to stop conversion.
*
**************************************************************************/

static pwr_tStatus dataa_comment_start (
  dataa_ctx dataactx,
  char *line,
  char *pos,
  char *out,
  int *incr,
  char *var
) 
{
	sprintf( out, "/*");
	*incr = strlen(out);
	dataactx->comment = 1;
	return GSX__SUCCESS;
}

/*************************************************************************
*
* Name:		dataa_comment_end
*
* Type		static int
*
* Type		Parameter	IOGF	Description
* dataa_ctx	dataactx	I	dataa context
* char		*line		I	current line
* char		*pos		I	position in current line
* char		*out		IO	output buffer
* char		*var		I	name of pointer to dataarithm object
*
* Description:
*	Backcall funktions called when an comment end is found.
*	Reset dataactx->comment to start conversion again.
*
**************************************************************************/
static pwr_tStatus dataa_comment_end (
  dataa_ctx dataactx,
  char *line,
  char *pos,
  char *out,
  int *incr,
  char *var
) 
{
	sprintf( out, "*/");
	*incr = strlen(out);
	dataactx->comment = 0;
	return GSX__SUCCESS;
}

/*************************************************************************
*
* Name:		dataa_Dax
*
* Type		int
*
* Type		Parameter	IOGF	Description
* dataa_ctx	dataactx	I	dataa context
* char		*line		I	current line
* char		*pos		I	position in current line
* char		*out		IO	output buffer
* char		*var		I	name of pointer to dataarithm object
*
* Description:
*	Backcall funktions called when a datapointer 'Da1' is found.
* 	If a 'ODay = Dax' is detected the 'dataactx->outdatax_eq_indata' flag
* 	is set previously by the ODay backcall function, and the code should
*	be a memcpy.
*
**************************************************************************/
static pwr_tStatus dataa_Da1 (
  dataa_ctx dataactx,
  char *line,
  char *pos,
  char *out,
  int *incr,
  char *var
) 
{
	if ( dataactx->outdata1_eq_indata )
	{
	  sprintf( out, 
		"Da1 */ memcpy( &%s->OutData1, %s->DataIn1P, 12)", var, var);
	  dataactx->outdata1_eq_indata = 0;
	  *incr = strlen(out);
	  return GSX__SUCCESS; 
	}
	if ( dataactx->outdata2_eq_indata )
	{
	  sprintf( out, 
		"Da1 */ memcpy( &%s->OutData2, %s->DataIn1P, 12)", var, var);
	  dataactx->outdata2_eq_indata = 0;
	  *incr = strlen(out);
	  return GSX__SUCCESS; 
	}
	if ( dataactx->outdata3_eq_indata )
	{
	  sprintf( out, 
		"Da1 */ memcpy( &%s->OutData3, %s->DataIn1P, 12)", var, var);
	  dataactx->outdata3_eq_indata = 0;
	  *incr = strlen(out);
	  return GSX__SUCCESS; 
	}
	if ( dataactx->outdata4_eq_indata )
	{
	  sprintf( out, 
		"Da1 */ memcpy( &%s->OutData4, %s->DataIn1P, 12)", var, var);
	  dataactx->outdata4_eq_indata = 0;
	  *incr = strlen(out);
	  return GSX__SUCCESS;
	}
	if ( !strncmp( pos, "->", 2) || !strncmp( pos, " ->", 3)) 
	{
	  if ( strcmp( dataactx->classdef_Da1, "") != 0) 
	    sprintf( out, "((pwr_sClass_%s *)(*(%s->DataIn1P)))",
		     dataactx->classdef_Da1, var);
	  else if ( strcmp( dataactx->structdef_Da1, "") != 0) 
	    sprintf( out, "((%s *)(*(%s->DataIn1P)))",
		     dataactx->structdef_Da1, var);
	  else
	    return GSX__DATAASYNTAX;
	}
	else
	  sprintf( out, " *(%s->DataIn1P)", var);
	*incr = strlen(out);
	return GSX__SUCCESS;
}

static pwr_tStatus dataa_Da2 (
  dataa_ctx dataactx,
  char *line,
  char *pos,
  char *out,
  int *incr,
  char *var
) 
{
	if ( dataactx->outdata1_eq_indata )
	{
	  sprintf( out,
		"Da2 */ memcpy( &%s->OutData1, %s->DataIn2P, 12)", var, var);
	  dataactx->outdata1_eq_indata = 0;
	  *incr = strlen(out);
	  return GSX__SUCCESS; 
	}
	if ( dataactx->outdata2_eq_indata )
	{
	  sprintf( out, 
		"Da2 */ memcpy( &%s->OutData2, %s->DataIn2P, 12)", var, var);
	  dataactx->outdata2_eq_indata = 0;
	  *incr = strlen(out);
	  return GSX__SUCCESS; 
	}
	if ( dataactx->outdata3_eq_indata )
	{
	  sprintf( out, 
		"Da2 */ memcpy( &%s->OutData3, %s->DataIn2P, 12)", var, var);
	  dataactx->outdata3_eq_indata = 0;
	  *incr = strlen(out);
	  return GSX__SUCCESS; 
	}
	if ( dataactx->outdata4_eq_indata )
	{
	  sprintf( out, 
		"Da2 */ memcpy( &%s->OutData4, %s->DataIn2P, 12)", var, var);
	  dataactx->outdata4_eq_indata = 0;
	  *incr = strlen(out);
	  return GSX__SUCCESS;
	}
	if ( !strncmp( pos, "->", 2) || !strncmp( pos, " ->", 3)) 
	{
	  if ( strcmp( dataactx->classdef_Da2, "") != 0) 
	    sprintf( out, "((pwr_sClass_%s *)(*(%s->DataIn2P)))",
		     dataactx->classdef_Da2, var);
	  else if ( strcmp( dataactx->structdef_Da2, "") != 0) 
	    sprintf( out, "((%s *)(*(%s->DataIn2P)))",
		     dataactx->structdef_Da2, var);
	  else 
	    return GSX__DATAASYNTAX;
	}
	else
	  sprintf( out, " *(%s->DataIn2P)", var); 
	*incr = strlen(out);
	return GSX__SUCCESS; 
}

static pwr_tStatus dataa_Da3 (
  dataa_ctx dataactx,
  char *line,
  char *pos,
  char *out,
  int *incr,
  char *var
) 
{
	if ( dataactx->outdata1_eq_indata )
	{
	  sprintf( out, 
		"Da3 */ memcpy( &%s->OutData1, %s->DataIn3P, 12)", var, var);
	  dataactx->outdata1_eq_indata = 0;
	  *incr = strlen(out);
	  return GSX__SUCCESS; 
	}
	if ( dataactx->outdata2_eq_indata )
	{
	  sprintf( out, 
		"Da3 */ memcpy( &%s->OutData2, %s->DataIn3P, 12)", var, var);
	  dataactx->outdata2_eq_indata = 0;
	  *incr = strlen(out);
	  return GSX__SUCCESS; 
	}
	if ( dataactx->outdata3_eq_indata )
	{
	  sprintf( out, 
		"Da3 */ memcpy( &%s->OutData3, %s->DataIn3P, 12)", var, var);
	  dataactx->outdata3_eq_indata = 0;
	  *incr = strlen(out);
	  return GSX__SUCCESS; 
	}
	if ( dataactx->outdata4_eq_indata )
	{
	  sprintf( out, 
		"Da3 */ memcpy( &%s->OutData4, %s->DataIn3P, 12)", var, var);
	  dataactx->outdata4_eq_indata = 0;
	  *incr = strlen(out);
	  return GSX__SUCCESS;
	}
	if ( !strncmp( pos, "->", 2) || !strncmp( pos, " ->", 3)) 
	{
	  if ( strcmp( dataactx->classdef_Da3, "") != 0) 
	    sprintf( out, "((pwr_sClass_%s *)(*(%s->DataIn3P)))",
		     dataactx->classdef_Da3, var);
	  else if ( strcmp( dataactx->structdef_Da3, "") != 0) 
	    sprintf( out, "((%s *)(*(%s->DataIn3P)))",
		     dataactx->structdef_Da3, var);
	  else
	    return GSX__DATAASYNTAX;
	}
	else
	  sprintf( out, " *(%s->DataIn3P)", var); 
	*incr = strlen(out);
	return GSX__SUCCESS; 
}

static pwr_tStatus dataa_Da4 (
  dataa_ctx dataactx,
  char *line,
  char *pos,
  char *out,
  int *incr,
  char *var
) 
{
	if ( dataactx->outdata1_eq_indata )
	{
	  sprintf( out, 
		"Da4 */ memcpy( &%s->OutData1, %s->DataIn4P, 12)", var, var);
	  dataactx->outdata1_eq_indata = 0;
	  *incr = strlen(out);
	  return GSX__SUCCESS; 
	}
	if ( dataactx->outdata2_eq_indata )
	{
	  sprintf( out, 
		"Da4 */ memcpy( &%s->OutData2, %s->DataIn4P, 12)", var, var);
	  dataactx->outdata2_eq_indata = 0;
	  *incr = strlen(out);
	  return GSX__SUCCESS; 
	}
	if ( dataactx->outdata3_eq_indata )
	{
	  sprintf( out, 
		"Da4 */ memcpy( &%s->OutData3, %s->DataIn4P, 12)", var, var);
	  dataactx->outdata3_eq_indata = 0;
	  *incr = strlen(out);
	  return GSX__SUCCESS; 
	}
	if ( dataactx->outdata4_eq_indata )
	{
	  sprintf( out, 
		"Da4 */ memcpy( &%s->OutData4, %s->DataIn4P, 12)", var, var);
	  dataactx->outdata4_eq_indata = 0;
	  *incr = strlen(out);
	  return GSX__SUCCESS;
	}
	if ( !strncmp( pos, "->", 2) || !strncmp( pos, " ->", 3)) 
	{
	  if ( strcmp( dataactx->classdef_Da4, "") != 0) 
	    sprintf( out, "((pwr_sClass_%s *)(*(%s->DataIn4P)))",
		     dataactx->classdef_Da4, var);
	  else if ( strcmp( dataactx->structdef_Da4, "") != 0) 
	    sprintf( out, "((%s *)(*(%s->DataIn4P)))",
		     dataactx->structdef_Da4, var);
	  else
	    return GSX__DATAASYNTAX;
	}
	else
	  sprintf( out, " *(%s->DataIn4P)", var);
	*incr = strlen(out);
	return GSX__SUCCESS; 
}

static pwr_tStatus dataa_ODa1 (
  dataa_ctx dataactx,
  char *line,
  char *pos,
  char *out,
  int *incr,
  char *var
) 
{
	char	out_str[5][80];
	int	nr;

	nr = dataa_parse( pos, "	 =;,", "", out_str[0],
		sizeof( out_str) / sizeof( out_str[0]), sizeof( out_str[0]));
	if ( nr > 0)
	{
	  if ( !strcmp( out_str[0], "Da1") ||
	       !strcmp( out_str[0], dataactx->aliasdef_Da1) ||
	       !strcmp( out_str[0], "Da2") ||
	       !strcmp( out_str[0], dataactx->aliasdef_Da2) ||
	       !strcmp( out_str[0], "Da3") ||
	       !strcmp( out_str[0], dataactx->aliasdef_Da3) ||
	       !strcmp( out_str[0], "Da4") ||
	       !strcmp( out_str[0], dataactx->aliasdef_Da4) )
	  {
	    dataactx->outdata1_eq_indata = 1;
	    sprintf( out, "/* ODa1");
	  }
	}
	if ( !dataactx->outdata1_eq_indata )
	  sprintf( out, "(%s->OutData1)", var);
	*incr = strlen(out);
	return GSX__SUCCESS; 
}

static pwr_tStatus dataa_ODa2 (
  dataa_ctx dataactx,
  char *line,
  char *pos,
  char *out,
  int *incr,
  char *var
) 
{
	char	out_str[5][80];
	int	nr;

	nr = dataa_parse( pos, "	 =;,", "", out_str[0],
		sizeof( out_str) / sizeof( out_str[0]), sizeof( out_str[0]));
	if ( nr > 0)
	{
	  if ( !strcmp( out_str[0], "Da1") ||
	       !strcmp( out_str[0], dataactx->aliasdef_Da1) ||
	       !strcmp( out_str[0], "Da2") ||
	       !strcmp( out_str[0], dataactx->aliasdef_Da2) ||
	       !strcmp( out_str[0], "Da3") ||
	       !strcmp( out_str[0], dataactx->aliasdef_Da3) ||
	       !strcmp( out_str[0], "Da4") ||
	       !strcmp( out_str[0], dataactx->aliasdef_Da4) )
	  {
	    dataactx->outdata2_eq_indata = 1;
	    sprintf( out, "/* ODa2");
	  }
	}
	if ( !dataactx->outdata2_eq_indata )
	  sprintf( out, "(%s->OutData2)", var);
	*incr = strlen(out);
	return GSX__SUCCESS; 
}

static pwr_tStatus dataa_ODa3 (
  dataa_ctx dataactx,
  char *line,
  char *pos,
  char *out,
  int *incr,
  char *var
) 
{
	char	out_str[5][80];
	int	nr;

	nr = dataa_parse( pos, "	 =;,", "", out_str[0],
		sizeof( out_str) / sizeof( out_str[0]), sizeof( out_str[0]));
	if ( nr > 0)
	{
	  if ( !strcmp( out_str[0], "Da1") ||
	       !strcmp( out_str[0], dataactx->aliasdef_Da1) ||
	       !strcmp( out_str[0], "Da2") ||
	       !strcmp( out_str[0], dataactx->aliasdef_Da2) ||
	       !strcmp( out_str[0], "Da3") ||
	       !strcmp( out_str[0], dataactx->aliasdef_Da3) ||
	       !strcmp( out_str[0], "Da4") ||
	       !strcmp( out_str[0], dataactx->aliasdef_Da4) )
	  {
	    dataactx->outdata3_eq_indata = 1;
	    sprintf( out, "/* ODa3");
	  }
	}
	if ( !dataactx->outdata3_eq_indata )
	  sprintf( out, "(%s->OutData3)", var);
	*incr = strlen(out);
	return GSX__SUCCESS; 
}

static pwr_tStatus dataa_ODa4 (
  dataa_ctx dataactx,
  char *line,
  char *pos,
  char *out,
  int *incr,
  char *var
) 
{
	char	out_str[5][80];
	int	nr;

	nr = dataa_parse( pos, "	 =;,", "", out_str[0],
		sizeof( out_str) / sizeof( out_str[0]), sizeof( out_str[0]));
	if ( nr > 0)
	{
	  if ( !strcmp( out_str[0], "Da1") ||
	       !strcmp( out_str[0], dataactx->aliasdef_Da1) ||
	       !strcmp( out_str[0], "Da2") ||
	       !strcmp( out_str[0], dataactx->aliasdef_Da2) ||
	       !strcmp( out_str[0], "Da3") ||
	       !strcmp( out_str[0], dataactx->aliasdef_Da3) ||
	       !strcmp( out_str[0], "Da4") ||
	       !strcmp( out_str[0], dataactx->aliasdef_Da4) )
	  {
	    dataactx->outdata4_eq_indata = 1;
	    sprintf( out, "/* ODa4");
	  }
	}
	if ( !dataactx->outdata4_eq_indata )
	  sprintf( out, "(%s->OutData4)", var);
	*incr = strlen(out);
	return GSX__SUCCESS; 
}
static pwr_tStatus dataa_Da1front( dataa_ctx dataactx, char *line, char *pos, 
	char *out, int *incr, char *var) {
	sprintf( out, "( *(char *)((char *)(%s->DataIn1P) + pwr_cAlignLW+2*pwr_cAlignW))", var); 
	*incr = strlen(out);
	return GSX__SUCCESS; }
static pwr_tStatus dataa_Da2front( dataa_ctx dataactx, char *line, char *pos, 
	char *out, int *incr, char *var) {
	sprintf( out, "( *(char *)((char *)(%s->DataIn2P) + pwr_cAlignLW+2*pwr_cAlignW))", var); 
	*incr = strlen(out);
	return GSX__SUCCESS; }
static pwr_tStatus dataa_Da3front( dataa_ctx dataactx, char *line, char *pos, 
	char *out, int *incr, char *var) {
	sprintf( out, "( *(char *)((char *)(%s->DataIn3P) + pwr_cAlignLW+2*pwr_cAlignW))", var); 
	*incr = strlen(out);
	return GSX__SUCCESS; }
static pwr_tStatus dataa_Da4front( dataa_ctx dataactx, char *line, char *pos, 
	char *out, int *incr, char *var) {
	sprintf( out, "( *(char *)((char *)(%s->DataIn4P) + pwr_cAlignLW+2*pwr_cAlignW))", var); 
	*incr = strlen(out);
	return GSX__SUCCESS; }
static pwr_tStatus dataa_Da1back( dataa_ctx dataactx, char *line, char *pos, 
	char *out, int *incr, char *var) {
	sprintf( out, "( *(char *)((char *)(%s->DataIn1P) + pwr_cAlignLW+3*pwr_cAlignW))", var);
	*incr = strlen(out);
	 return GSX__SUCCESS; }
static pwr_tStatus dataa_Da2back( dataa_ctx dataactx, char *line, char *pos,
	char *out, int *incr, char *var) {
	sprintf( out, "( *(char *)((char *)(%s->DataIn2P) + pwr_cAlignLW+3*pwr_cAlignW))", var);
	*incr = strlen(out);
	 return GSX__SUCCESS; }
static pwr_tStatus dataa_Da3back( dataa_ctx dataactx, char *line, char *pos, 
	char *out, int *incr, char *var) {
	sprintf( out, "( *(char *)((char *)(%s->DataIn3P) + pwr_cAlignLW+3*pwr_cAlignW))", var); 
	*incr = strlen(out);
	return GSX__SUCCESS; }
static pwr_tStatus dataa_Da4back( dataa_ctx dataactx, char *line, char *pos, 
	char *out, int *incr, char *var) {
	sprintf( out, "( *(char *)((char *)(%s->DataIn4P) + pwr_cAlignLW+3*pwr_cAlignW))", var); 
	*incr = strlen(out);
	return GSX__SUCCESS; }
static pwr_tStatus dataa_Da1objid( dataa_ctx dataactx, char *line, char *pos, 
	char *out, int *incr, char *var) {
	sprintf( out, "( *(pwr_tObjid *)((char *)(%s->DataIn1P) + pwr_cAlignLW))", var); 
	*incr = strlen(out);
	return GSX__SUCCESS; }
static pwr_tStatus dataa_Da2objid( dataa_ctx dataactx, char *line, char *pos, 
	char *out, int *incr, char *var) {
	sprintf( out, "( *(pwr_tObjid *)((char *)(%s->DataIn2P) + pwr_cAlignLW))", var); 
	*incr = strlen(out);
	return GSX__SUCCESS; }
static pwr_tStatus dataa_Da3objid( dataa_ctx dataactx, char *line, char *pos, 
	char *out, int *incr, char *var) {
	sprintf( out, "( *(pwr_tObjid *)((char *)(%s->DataIn3P) + pwr_cAlignLW))", var); 
	*incr = strlen(out);
	return GSX__SUCCESS; }
static pwr_tStatus dataa_Da4objid( dataa_ctx dataactx, char *line, char *pos, 
	char *out, int *incr, char *var) {
	sprintf( out, "( *(pwr_tObjid *)((char *)(%s->DataIn4P) + pwr_cAlignLW))", var); 
	*incr = strlen(out);
	return GSX__SUCCESS; }
static pwr_tStatus dataa_d1( dataa_ctx dataactx, char *line, char *pos, 
	char *out, int *incr, char *var) {
	sprintf( out, "(*(%s->DIn1P))", var); 
	*incr = strlen(out);
	return GSX__SUCCESS; }
static pwr_tStatus dataa_d2( dataa_ctx dataactx, char *line, char *pos, 
	char *out, int *incr, char *var) {
	sprintf( out, "(*(%s->DIn2P))", var); 
	*incr = strlen(out);
	return GSX__SUCCESS; }
static pwr_tStatus dataa_d3( dataa_ctx dataactx, char *line, char *pos, 
	char *out, int *incr, char *var) {
	sprintf( out, "(*(%s->DIn3P))", var); 
	*incr = strlen(out);
	return GSX__SUCCESS; }
static pwr_tStatus dataa_d4( dataa_ctx dataactx, char *line, char *pos, 
	char *out, int *incr, char *var) {
	sprintf( out, "(*(%s->DIn4P))", var); 
	*incr = strlen(out);
	return GSX__SUCCESS; }
static pwr_tStatus dataa_d5( dataa_ctx dataactx, char *line, char *pos, 
	char *out, int *incr, char *var) {
	sprintf( out, "(*(%s->DIn5P))", var); 
	*incr = strlen(out);
	return GSX__SUCCESS; }
static pwr_tStatus dataa_d6( dataa_ctx dataactx, char *line, char *pos, 
	char *out, int *incr, char *var) {
	sprintf( out, "(*(%s->DIn6P))", var); 
	*incr = strlen(out);
	return GSX__SUCCESS; }
static pwr_tStatus dataa_d7( dataa_ctx dataactx, char *line, char *pos, 
	char *out, int *incr, char *var) {
	sprintf( out, "(*(%s->DIn7P))", var); 
	*incr = strlen(out);
	return GSX__SUCCESS; }
static pwr_tStatus dataa_d8( dataa_ctx dataactx, char *line, char *pos, 
	char *out, int *incr, char *var) {
	sprintf( out, "(*(%s->DIn8P))", var); 
	*incr = strlen(out);
	return GSX__SUCCESS; }
static pwr_tStatus dataa_A1( dataa_ctx dataactx, char *line, char *pos, 
	char *out, int *incr, char *var) {
	sprintf( out, "(*(%s->AIn1P))", var); 
	*incr = strlen(out);
	return GSX__SUCCESS; }
static pwr_tStatus dataa_A2( dataa_ctx dataactx, char *line, char *pos, 
	char *out, int *incr, char *var) {
	sprintf( out, "(*(%s->AIn2P))", var); 
	*incr = strlen(out);
	return GSX__SUCCESS; }
static pwr_tStatus dataa_A3( dataa_ctx dataactx, char *line, char *pos, 
	char *out, int *incr, char *var) {
	sprintf( out, "(*(%s->AIn3P))", var); 
	*incr = strlen(out);
	return GSX__SUCCESS; }
static pwr_tStatus dataa_A4( dataa_ctx dataactx, char *line, char *pos, 
	char *out, int *incr, char *var) {
	sprintf( out, "(*(%s->AIn4P))", var); 
	*incr = strlen(out);
	return GSX__SUCCESS; }
static pwr_tStatus dataa_A5( dataa_ctx dataactx, char *line, char *pos, 
	char *out, int *incr, char *var) {
	sprintf( out, "(*(%s->AIn5P))", var); 
	*incr = strlen(out);
	return GSX__SUCCESS; }
static pwr_tStatus dataa_A6( dataa_ctx dataactx, char *line, char *pos, 
	char *out, int *incr, char *var) {
	sprintf( out, "(*(%s->AIn6P))", var); 
	*incr = strlen(out);
	return GSX__SUCCESS; }
static pwr_tStatus dataa_A7( dataa_ctx dataactx, char *line, char *pos, 
	char *out, int *incr, char *var) {
	sprintf( out, "(*(%s->AIn7P))", var); 
	*incr = strlen(out);
	return GSX__SUCCESS; }
static pwr_tStatus dataa_A8( dataa_ctx dataactx, char *line, char *pos, 
	char *out, int *incr, char *var) {
	sprintf( out, "(*(%s->AIn8P))", var); 
	*incr = strlen(out);
	return GSX__SUCCESS; }
static pwr_tStatus dataa_I1( dataa_ctx dataactx, char *line, char *pos, 
	char *out, int *incr, char *var) {
	sprintf( out, "(*(%s->IIn1P))", var); 
	*incr = strlen(out);
	return GSX__SUCCESS; }
static pwr_tStatus dataa_I2( dataa_ctx dataactx, char *line, char *pos, 
	char *out, int *incr, char *var) {
	sprintf( out, "(*(%s->IIn2P))", var); 
	*incr = strlen(out);
	return GSX__SUCCESS; }
static pwr_tStatus dataa_I3( dataa_ctx dataactx, char *line, char *pos, 
	char *out, int *incr, char *var) {
	sprintf( out, "(*(%s->IIn3P))", var); 
	*incr = strlen(out);
	return GSX__SUCCESS; }
static pwr_tStatus dataa_I4( dataa_ctx dataactx, char *line, char *pos, 
	char *out, int *incr, char *var) {
	sprintf( out, "(*(%s->IIn4P))", var); 
	*incr = strlen(out);
	return GSX__SUCCESS; }
static pwr_tStatus dataa_I5( dataa_ctx dataactx, char *line, char *pos, 
	char *out, int *incr, char *var) {
	sprintf( out, "(*(%s->IIn5P))", var); 
	*incr = strlen(out);
	return GSX__SUCCESS; }
static pwr_tStatus dataa_I6( dataa_ctx dataactx, char *line, char *pos, 
	char *out, int *incr, char *var) {
	sprintf( out, "(*(%s->IIn6P))", var); 
	*incr = strlen(out);
	return GSX__SUCCESS; }
static pwr_tStatus dataa_I7( dataa_ctx dataactx, char *line, char *pos, 
	char *out, int *incr, char *var) {
	sprintf( out, "(*(%s->IIn7P))", var); 
	*incr = strlen(out);
	return GSX__SUCCESS; }
static pwr_tStatus dataa_I8( dataa_ctx dataactx, char *line, char *pos, 
	char *out, int *incr, char *var) {
	sprintf( out, "(*(%s->IIn8P))", var); 
	*incr = strlen(out);
	return GSX__SUCCESS; }
static pwr_tStatus dataa_od1( dataa_ctx dataactx, char *line, char *pos, 
	char *out, int *incr, char *var) {
	sprintf( out, "(%s->OutD1)", var); 
	*incr = strlen(out);
	return GSX__SUCCESS; }
static pwr_tStatus dataa_od2( dataa_ctx dataactx, char *line, char *pos, 
	char *out, int *incr, char *var) {
	sprintf( out, "(%s->OutD2)", var); 
	*incr = strlen(out);
	return GSX__SUCCESS; }
static pwr_tStatus dataa_od3( dataa_ctx dataactx, char *line, char *pos, 
	char *out, int *incr, char *var) {
	sprintf( out, "(%s->OutD3)", var); 
	*incr = strlen(out);
	return GSX__SUCCESS; }
static pwr_tStatus dataa_od4( dataa_ctx dataactx, char *line, char *pos, 
	char *out, int *incr, char *var) {
	sprintf( out, "(%s->OutD4)", var); 
	*incr = strlen(out);
	return GSX__SUCCESS; }
static pwr_tStatus dataa_od5( dataa_ctx dataactx, char *line, char *pos, 
	char *out, int *incr, char *var) {
	sprintf( out, "(%s->OutD5)", var); 
	*incr = strlen(out);
	return GSX__SUCCESS; }
static pwr_tStatus dataa_od6( dataa_ctx dataactx, char *line, char *pos, 
	char *out, int *incr, char *var) {
	sprintf( out, "(%s->OutD6)", var); 
	*incr = strlen(out);
	return GSX__SUCCESS; }
static pwr_tStatus dataa_od7( dataa_ctx dataactx, char *line, char *pos, 
	char *out, int *incr, char *var) {
	sprintf( out, "(%s->OutD7)", var); 
	*incr = strlen(out);
	return GSX__SUCCESS; }
static pwr_tStatus dataa_od8( dataa_ctx dataactx, char *line, char *pos, 
	char *out, int *incr, char *var) {
	sprintf( out, "(%s->OutD8)", var); 
	*incr = strlen(out);
	return GSX__SUCCESS; }
static pwr_tStatus dataa_OA1( dataa_ctx dataactx, char *line, char *pos, 
	char *out, int *incr, char *var) {
	sprintf( out, "(%s->OutA1)", var); 
	*incr = strlen(out);
	return GSX__SUCCESS; }
static pwr_tStatus dataa_OA2( dataa_ctx dataactx, char *line, char *pos, 
	char *out, int *incr, char *var) {
	sprintf( out, "(%s->OutA2)", var); 
	*incr = strlen(out);
	return GSX__SUCCESS; }
static pwr_tStatus dataa_OA3( dataa_ctx dataactx, char *line, char *pos, 
	char *out, int *incr, char *var) {
	sprintf( out, "(%s->OutA3)", var); 
	*incr = strlen(out);
	return GSX__SUCCESS; }
static pwr_tStatus dataa_OA4( dataa_ctx dataactx, char *line, char *pos, 
	char *out, int *incr, char *var) {
	sprintf( out, "(%s->OutA4)", var); 
	*incr = strlen(out);
	return GSX__SUCCESS; }
static pwr_tStatus dataa_OA5( dataa_ctx dataactx, char *line, char *pos, 
	char *out, int *incr, char *var) {
	sprintf( out, "(%s->OutA5)", var); 
	*incr = strlen(out);
	return GSX__SUCCESS; }
static pwr_tStatus dataa_OA6( dataa_ctx dataactx, char *line, char *pos, 
	char *out, int *incr, char *var) {
	sprintf( out, "(%s->OutA6)", var); 
	*incr = strlen(out);
	return GSX__SUCCESS; }
static pwr_tStatus dataa_OA7( dataa_ctx dataactx, char *line, char *pos, 
	char *out, int *incr, char *var) {
	sprintf( out, "(%s->OutA7)", var); 
	*incr = strlen(out);
	return GSX__SUCCESS; }
static pwr_tStatus dataa_OA8( dataa_ctx dataactx, char *line, char *pos, 
	char *out, int *incr, char *var) {
	sprintf( out, "(%s->OutA8)", var); 
	*incr = strlen(out);
	return GSX__SUCCESS; }
static pwr_tStatus dataa_OI1( dataa_ctx dataactx, char *line, char *pos, 
	char *out, int *incr, char *var) {
	sprintf( out, "(%s->OutI1)", var); 
	*incr = strlen(out);
	return GSX__SUCCESS; }
static pwr_tStatus dataa_OI2( dataa_ctx dataactx, char *line, char *pos, 
	char *out, int *incr, char *var) {
	sprintf( out, "(%s->OutI2)", var); 
	*incr = strlen(out);
	return GSX__SUCCESS; }
static pwr_tStatus dataa_OI3( dataa_ctx dataactx, char *line, char *pos, 
	char *out, int *incr, char *var) {
	sprintf( out, "(%s->OutI3)", var); 
	*incr = strlen(out);
	return GSX__SUCCESS; }
static pwr_tStatus dataa_OI4( dataa_ctx dataactx, char *line, char *pos, 
	char *out, int *incr, char *var) {
	sprintf( out, "(%s->OutI4)", var); 
	*incr = strlen(out);
	return GSX__SUCCESS; }
static pwr_tStatus dataa_OI5( dataa_ctx dataactx, char *line, char *pos, 
	char *out, int *incr, char *var) {
	sprintf( out, "(%s->OutI5)", var); 
	*incr = strlen(out);
	return GSX__SUCCESS; }
static pwr_tStatus dataa_OI6( dataa_ctx dataactx, char *line, char *pos, 
	char *out, int *incr, char *var) {
	sprintf( out, "(%s->OutI6)", var); 
	*incr = strlen(out);
	return GSX__SUCCESS; }
static pwr_tStatus dataa_OI7( dataa_ctx dataactx, char *line, char *pos, 
	char *out, int *incr, char *var) {
	sprintf( out, "(%s->OutI7)", var); 
	*incr = strlen(out);
	return GSX__SUCCESS; }
static pwr_tStatus dataa_OI8( dataa_ctx dataactx, char *line, char *pos, 
	char *out, int *incr, char *var) {
	sprintf( out, "(%s->OutI8)", var); 
	*incr = strlen(out);
	return GSX__SUCCESS; }

/*************************************************************************
*
* Name:		dataa_classdef
*
* Type		int
*
* Type		Parameter	IOGF	Description
* dataa_ctx	dataactx	I	dataa context
* char		*line		I	current line
*
* Description:
*	Backcall function called when a classdef command is detected.
*
**************************************************************************/
static pwr_tStatus dataa_classdef (
  dataa_ctx dataactx,
  char *line
)
{
	char	out_str[5][80];
	char	comm_str[20][80];
	int	nr;
	int	comm_nr;
	int	i, j;

	/* Remove the last character (linefeed) */
	*(line + strlen(line) - 1)  = 0;

	/* Parse the command string */
	comm_nr = dataa_parse( line, ",;", "", comm_str[0],
		sizeof( comm_str) / sizeof( comm_str[0]), sizeof( comm_str[0]));

	for ( i = 0; i < comm_nr; i++)
	{
	  if ( !strcmp( comm_str[i], ""))
	    break;

	  nr = dataa_parse( comm_str[i], "	 ", "", out_str[0],
		sizeof( out_str) / sizeof( out_str[0]), sizeof( out_str[0]));
	  if ( nr == 0)
	    continue;

	  if ( i == 0)
	  {
	    if ( strcmp( out_str[0], "classdef"))
	      return GSX__DATAASYNTAX;

	    for ( j = 0; j < nr-1; j++)
	      strcpy( out_str[j], out_str[j+1]);
	    nr--;
	  }

	  if (!( nr == 2 || nr == 3))
	    return GSX__DATAASYNTAX;

	  if ( !strcmp( out_str[0], "Da1"))
	  {
	    if ( dataactx->classdef_Da1[0] != 0)
	      /* Already used */
	      return GSX__DATAASYNTAX;
	    strcpy( dataactx->classdef_Da1, out_str[1]);
	    if ( nr == 3)
	    {
	      dataa_add_item( dataactx,  out_str[2], "A", "A", &dataa_Da1);
	      strcpy( dataactx->aliasdef_Da1, out_str[2]);
	    }
	  }
	  else if ( !strcmp( out_str[0], "Da2"))
	  {
	    if ( dataactx->classdef_Da2[0] != 0)
	      /* Already used */
	      return GSX__DATAASYNTAX;
	    strcpy( dataactx->classdef_Da2, out_str[1]);
	    if ( nr == 3)
	    {
	      dataa_add_item( dataactx,  out_str[2], "A", "A", &dataa_Da2);
	      strcpy( dataactx->aliasdef_Da2, out_str[2]);
	    }
	  }
	  else if ( !strcmp( out_str[0], "Da3"))
	  {
	    if ( dataactx->classdef_Da3[0] != 0)
	      /* Already used */
	      return GSX__DATAASYNTAX;
	    strcpy( dataactx->classdef_Da3, out_str[1]);
	    if ( nr == 3)
	    {
	      dataa_add_item( dataactx,  out_str[2], "A", "A", &dataa_Da3);
	      strcpy( dataactx->aliasdef_Da3, out_str[2]);
	    }
	  }
	  else if ( !strcmp( out_str[0], "Da4"))
	  {
	    if ( dataactx->classdef_Da4[0] != 0)
	      /* Already used */
	      return GSX__DATAASYNTAX;
	    strcpy( dataactx->classdef_Da4, out_str[1]);
	    if ( nr == 3)
	    {
	      dataa_add_item( dataactx,  out_str[2], "A", "A", &dataa_Da4);
	      strcpy( dataactx->aliasdef_Da4, out_str[2]);
	    }
	  }
	  else
	    return GSX__DATAASYNTAX;
	}

	return GSX__SUCCESS;
}

/*************************************************************************
*
* Name:		dataa_structdef
*
* Type		int
*
* Type		Parameter	IOGF	Description
* dataa_ctx	dataactx	I	dataa context
* char		*line		I	current line
*
* Description:
*	Backcall function called when a structdef command is detected.
*
**************************************************************************/
static pwr_tStatus dataa_structdef (
  dataa_ctx dataactx,
  char *line
)
{
	char	out_str[5][80];
	char	comm_str[20][80];
	int	nr;
	int	comm_nr;
	int	i, j;

	/* Remove the last character (linefeed) */
	*(line + strlen(line) - 1)  = 0;

	/* Parse the command string */
	comm_nr = dataa_parse( line, ",;", "", comm_str[0],
		sizeof( comm_str) / sizeof( comm_str[0]), sizeof( comm_str[0]));

	for ( i = 0; i < comm_nr; i++)
	{
	  if ( !strcmp( comm_str[i], ""))
	    break;

	  nr = dataa_parse( comm_str[i], "	 ", "", out_str[0],
		sizeof( out_str) / sizeof( out_str[0]), sizeof( out_str[0]));
	  if ( nr == 0)
	    continue;

	  if ( i == 0)
	  {
	    if ( strcmp( out_str[0], "structdef"))
	      return GSX__DATAASYNTAX;

	    for ( j = 0; j < nr-1; j++)
	      strcpy( out_str[j], out_str[j+1]);
	    nr--;
	  }

	  if (!( nr == 2 || nr == 3))
	    return GSX__DATAASYNTAX;

	  if ( !strcmp( out_str[0], "Da1"))
	  {
	    if ( dataactx->structdef_Da1[0] != 0)
	      /* Already used */
	      return GSX__DATAASYNTAX;
	    strcpy( dataactx->structdef_Da1, out_str[1]);
	    if ( nr == 3)
	    {
	      dataa_add_item( dataactx,  out_str[2], "A", "A", &dataa_Da1);
	      strcpy( dataactx->aliasdef_Da1, out_str[2]);
	    }
	  }
	  else if ( !strcmp( out_str[0], "Da2"))
	  {
	    if ( dataactx->structdef_Da2[0] != 0)
	      /* Already used */
	      return GSX__DATAASYNTAX;
	    strcpy( dataactx->structdef_Da2, out_str[1]);
	    if ( nr == 3)
	    {
	      dataa_add_item( dataactx,  out_str[2], "A", "A", &dataa_Da2);
	      strcpy( dataactx->aliasdef_Da2, out_str[2]);
	    }
	  }
	  else if ( !strcmp( out_str[0], "Da3"))
	  {
	    if ( dataactx->structdef_Da3[0] != 0)
	      /* Already used */
	      return GSX__DATAASYNTAX;
	    strcpy( dataactx->structdef_Da3, out_str[1]);
	    if ( nr == 3)
	    {
	      dataa_add_item( dataactx,  out_str[2], "A", "A", &dataa_Da3);
	      strcpy( dataactx->aliasdef_Da3, out_str[2]);
	    }
	  }
	  else if ( !strcmp( out_str[0], "Da4"))
	  {
	    if ( dataactx->structdef_Da4[0] != 0)
	      /* Already used */
	      return GSX__DATAASYNTAX;
	    strcpy( dataactx->structdef_Da4, out_str[1]);
	    if ( nr == 3)
	    {
	      dataa_add_item( dataactx,  out_str[2], "A", "A", &dataa_Da4);
	      strcpy( dataactx->aliasdef_Da4, out_str[2]);
	    }
	  }
	  else
	    return GSX__DATAASYNTAX;
	}

	return GSX__SUCCESS;
}


/*************************************************************************
*
* Name:		dataa_aliasdef
*
* Type		int
*
* Type		Parameter	IOGF	Description
* dataa_ctx	dataactx	I	dataa context
* char		*line		I	current line
*
* Description:
*	Backcall function called when a aliasdef command is detected.
*
**************************************************************************/
static int	dataa_aliasdef( dataa_ctx dataactx, char *line)
{
	char	out_str[5][80];
	char	comm_str[20][80];
	int	nr;
	int	comm_nr;
	int	i;

	/* Remove the last character (linefeed) */
	*(line + strlen(line) - 1)  = 0;

	/* Parse the command string */
	comm_nr = dataa_parse( line, ",;", "", comm_str[0],
		sizeof( comm_str) / sizeof( comm_str[0]), sizeof( comm_str[0]));

	for ( i = 0; i < comm_nr; i++)
	{
	  if ( !strcmp( comm_str[i], ""))
	    break;

	  nr = dataa_parse( comm_str[i], "	 ", "", out_str[0],
		sizeof( out_str) / sizeof( out_str[0]), sizeof( out_str[0]));
	  if ( i == 0)
	  {
	    if ( nr != 3 )
	      return GSX__DATAASYNTAX;
	    if ( strcmp( out_str[0], "aliasdef"))
	      return GSX__DATAASYNTAX;

	    strcpy( out_str[0], out_str[1]);
	    strcpy( out_str[1], out_str[2]);
	  }
	  else
	  {
	    if ( nr != 2 )
	      return GSX__DATAASYNTAX;
	  }

	  if ( !strcmp( out_str[0], "d1"))
	    dataa_add_item( dataactx,  out_str[1], "A", "A", &dataa_d1);
	  else if ( !strcmp( out_str[0], "d2"))
	    dataa_add_item( dataactx,  out_str[1], "A", "A", &dataa_d2);
	  else if ( !strcmp( out_str[0], "d3"))
	    dataa_add_item( dataactx,  out_str[1], "A", "A", &dataa_d3);
	  else if ( !strcmp( out_str[0], "d4"))
	    dataa_add_item( dataactx,  out_str[1], "A", "A", &dataa_d4);
	  else if ( !strcmp( out_str[0], "d5"))
	    dataa_add_item( dataactx,  out_str[1], "A", "A", &dataa_d5);
	  else if ( !strcmp( out_str[0], "d6"))
	    dataa_add_item( dataactx,  out_str[1], "A", "A", &dataa_d6);
	  else if ( !strcmp( out_str[0], "d7"))
	    dataa_add_item( dataactx,  out_str[1], "A", "A", &dataa_d7);
	  else if ( !strcmp( out_str[0], "d8"))
	    dataa_add_item( dataactx,  out_str[1], "A", "A", &dataa_d8);
	  else if ( !strcmp( out_str[0], "A1"))
	    dataa_add_item( dataactx,  out_str[1], "A", "A", &dataa_A1);
	  else if ( !strcmp( out_str[0], "A2"))
	    dataa_add_item( dataactx,  out_str[1], "A", "A", &dataa_A2);
	  else if ( !strcmp( out_str[0], "A3"))
	    dataa_add_item( dataactx,  out_str[1], "A", "A", &dataa_A3);
	  else if ( !strcmp( out_str[0], "A4"))
	    dataa_add_item( dataactx,  out_str[1], "A", "A", &dataa_A4);
	  else if ( !strcmp( out_str[0], "A5"))
	    dataa_add_item( dataactx,  out_str[1], "A", "A", &dataa_A5);
	  else if ( !strcmp( out_str[0], "A6"))
	    dataa_add_item( dataactx,  out_str[1], "A", "A", &dataa_A6);
	  else if ( !strcmp( out_str[0], "A7"))
	    dataa_add_item( dataactx,  out_str[1], "A", "A", &dataa_A7);
	  else if ( !strcmp( out_str[0], "A8"))
	    dataa_add_item( dataactx,  out_str[1], "A", "A", &dataa_A8);
	  else if ( !strcmp( out_str[0], "I1"))
	    dataa_add_item( dataactx,  out_str[1], "A", "A", &dataa_I1);
	  else if ( !strcmp( out_str[0], "I2"))
	    dataa_add_item( dataactx,  out_str[1], "A", "A", &dataa_I2);
	  else if ( !strcmp( out_str[0], "I3"))
	    dataa_add_item( dataactx,  out_str[1], "A", "A", &dataa_I3);
	  else if ( !strcmp( out_str[0], "I4"))
	    dataa_add_item( dataactx,  out_str[1], "A", "A", &dataa_I4);
	  else if ( !strcmp( out_str[0], "I5"))
	    dataa_add_item( dataactx,  out_str[1], "A", "A", &dataa_I5);
	  else if ( !strcmp( out_str[0], "I6"))
	    dataa_add_item( dataactx,  out_str[1], "A", "A", &dataa_I6);
	  else if ( !strcmp( out_str[0], "I7"))
	    dataa_add_item( dataactx,  out_str[1], "A", "A", &dataa_I7);
	  else if ( !strcmp( out_str[0], "I8"))
	    dataa_add_item( dataactx,  out_str[1], "A", "A", &dataa_I8);
	  else if ( !strcmp( out_str[0], "od1"))
	    dataa_add_item( dataactx,  out_str[1], "A", "A", &dataa_od1);
	  else if ( !strcmp( out_str[0], "od2"))
	    dataa_add_item( dataactx,  out_str[1], "A", "A", &dataa_od2);
	  else if ( !strcmp( out_str[0], "od3"))
	    dataa_add_item( dataactx,  out_str[1], "A", "A", &dataa_od3);
	  else if ( !strcmp( out_str[0], "od4"))
	    dataa_add_item( dataactx,  out_str[1], "A", "A", &dataa_od4);
	  else if ( !strcmp( out_str[0], "od5"))
	    dataa_add_item( dataactx,  out_str[1], "A", "A", &dataa_od5);
	  else if ( !strcmp( out_str[0], "od6"))
	    dataa_add_item( dataactx,  out_str[1], "A", "A", &dataa_od6);
	  else if ( !strcmp( out_str[0], "od7"))
	    dataa_add_item( dataactx,  out_str[1], "A", "A", &dataa_od7);
	  else if ( !strcmp( out_str[0], "od8"))
	    dataa_add_item( dataactx,  out_str[1], "A", "A", &dataa_od8);
	  else if ( !strcmp( out_str[0], "OA1"))
	    dataa_add_item( dataactx,  out_str[1], "A", "A", &dataa_OA1);
	  else if ( !strcmp( out_str[0], "OA2"))
	    dataa_add_item( dataactx,  out_str[1], "A", "A", &dataa_OA2);
	  else if ( !strcmp( out_str[0], "OA3"))
	    dataa_add_item( dataactx,  out_str[1], "A", "A", &dataa_OA3);
	  else if ( !strcmp( out_str[0], "OA4"))
	    dataa_add_item( dataactx,  out_str[1], "A", "A", &dataa_OA4);
	  else if ( !strcmp( out_str[0], "OA5"))
	    dataa_add_item( dataactx,  out_str[1], "A", "A", &dataa_OA5);
	  else if ( !strcmp( out_str[0], "OA6"))
	    dataa_add_item( dataactx,  out_str[1], "A", "A", &dataa_OA6);
	  else if ( !strcmp( out_str[0], "OA7"))
	    dataa_add_item( dataactx,  out_str[1], "A", "A", &dataa_OA7);
	  else if ( !strcmp( out_str[0], "OA8"))
	    dataa_add_item( dataactx,  out_str[1], "A", "A", &dataa_OA8);
	  else if ( !strcmp( out_str[0], "OI1"))
	    dataa_add_item( dataactx,  out_str[1], "A", "A", &dataa_OI1);
	  else if ( !strcmp( out_str[0], "OI2"))
	    dataa_add_item( dataactx,  out_str[1], "A", "A", &dataa_OI2);
	  else if ( !strcmp( out_str[0], "OI3"))
	    dataa_add_item( dataactx,  out_str[1], "A", "A", &dataa_OI3);
	  else if ( !strcmp( out_str[0], "OI4"))
	    dataa_add_item( dataactx,  out_str[1], "A", "A", &dataa_OI4);
	  else if ( !strcmp( out_str[0], "OI5"))
	    dataa_add_item( dataactx,  out_str[1], "A", "A", &dataa_OI5);
	  else if ( !strcmp( out_str[0], "OI6"))
	    dataa_add_item( dataactx,  out_str[1], "A", "A", &dataa_OI6);
	  else if ( !strcmp( out_str[0], "OI7"))
	    dataa_add_item( dataactx,  out_str[1], "A", "A", &dataa_OI7);
	  else if ( !strcmp( out_str[0], "OI8"))
	    dataa_add_item( dataactx,  out_str[1], "A", "A", &dataa_OI8);
	  else if ( !strcmp( out_str[0], "ODa1"))
	    dataa_add_item( dataactx,  out_str[1], "A", "A", &dataa_ODa1);
	  else if ( !strcmp( out_str[0], "ODa2"))
	    dataa_add_item( dataactx,  out_str[1], "A", "A", &dataa_ODa2);
	  else if ( !strcmp( out_str[0], "ODa3"))
	    dataa_add_item( dataactx,  out_str[1], "A", "A", &dataa_ODa3);
	  else if ( !strcmp( out_str[0], "ODa4"))
	    dataa_add_item( dataactx,  out_str[1], "A", "A", &dataa_ODa4);
	  else
	    return GSX__DATAASYNTAX;
	}

	return GSX__SUCCESS;
}

/* Items includes also 32 dynamic items */
dataa_t_item	template_items[100] = {
	{"*/", "X","X", &dataa_comment_end, 0, 0},
	{"/*", "X","X", &dataa_comment_start, 0, 0},
	{"Da1", "A","A", &dataa_Da1, 0, 0},
	{"Da2", "A","A", &dataa_Da2, 0, 0},
	{"Da3", "A","A", &dataa_Da3, 0, 0},
	{"Da4", "A","A", &dataa_Da4, 0, 0},
	{"Da1back", "A","A", &dataa_Da1back, 0, 0},
	{"Da2back", "A","A", &dataa_Da2back, 0, 0},
	{"Da3back", "A","A", &dataa_Da3back, 0, 0},
	{"Da4back", "A","A", &dataa_Da4back, 0, 0},
	{"Da1front", "A","A", &dataa_Da1front, 0, 0},
	{"Da2front", "A","A", &dataa_Da2front, 0, 0},
	{"Da3front", "A","A", &dataa_Da3front, 0, 0},
	{"Da4front", "A","A", &dataa_Da4front, 0, 0},
	{"Da1objid", "A","A", &dataa_Da1objid, 0, 0},
	{"Da2objid", "A","A", &dataa_Da2objid, 0, 0},
	{"Da3objid", "A","A", &dataa_Da3objid, 0, 0},
	{"Da4objid", "A","A", &dataa_Da4objid, 0, 0},
	{"d1", "A","A", &dataa_d1, 0, 0},
	{"d2", "A","A", &dataa_d2, 0, 0},
	{"d3", "A","A", &dataa_d3, 0, 0},
	{"d4", "A","A", &dataa_d4, 0, 0},
	{"d5", "A","A", &dataa_d5, 0, 0},
	{"d6", "A","A", &dataa_d6, 0, 0},
	{"d7", "A","A", &dataa_d7, 0, 0},
	{"d8", "A","A", &dataa_d8, 0, 0},
	{"A1", "A","A", &dataa_A1, 0, 0},
	{"A2", "A","A", &dataa_A2, 0, 0},
	{"A3", "A","A", &dataa_A3, 0, 0},
	{"A4", "A","A", &dataa_A4, 0, 0},
	{"A5", "A","A", &dataa_A5, 0, 0},
	{"A6", "A","A", &dataa_A6, 0, 0},
	{"A7", "A","A", &dataa_A7, 0, 0},
	{"A8", "A","A", &dataa_A8, 0, 0},
	{"I1", "A","A", &dataa_I1, 0, 0},
	{"I2", "A","A", &dataa_I2, 0, 0},
	{"I3", "A","A", &dataa_I3, 0, 0},
	{"I4", "A","A", &dataa_I4, 0, 0},
	{"I5", "A","A", &dataa_I5, 0, 0},
	{"I6", "A","A", &dataa_I6, 0, 0},
	{"I7", "A","A", &dataa_I7, 0, 0},
	{"I8", "A","A", &dataa_I8, 0, 0},
	{"ODa1", "A","A", &dataa_ODa1, 0, 0},
	{"ODa2", "A","A", &dataa_ODa2, 0, 0},
	{"ODa3", "A","A", &dataa_ODa3, 0, 0},
	{"ODa4", "A","A", &dataa_ODa4, 0, 0},
	{"od1", "A","A", &dataa_od1, 0, 0},
	{"od2", "A","A", &dataa_od2, 0, 0},
	{"od3", "A","A", &dataa_od3, 0, 0},
	{"od4", "A","A", &dataa_od4, 0, 0},
	{"od5", "A","A", &dataa_od5, 0, 0},
	{"od6", "A","A", &dataa_od6, 0, 0},
	{"od7", "A","A", &dataa_od7, 0, 0},
	{"od8", "A","A", &dataa_od8, 0, 0},
	{"OA1", "A","A", &dataa_OA1, 0, 0},
	{"OA2", "A","A", &dataa_OA2, 0, 0},
	{"OA3", "A","A", &dataa_OA3, 0, 0},
	{"OA4", "A","A", &dataa_OA4, 0, 0},
	{"OA5", "A","A", &dataa_OA5, 0, 0},
	{"OA6", "A","A", &dataa_OA6, 0, 0},
	{"OA7", "A","A", &dataa_OA7, 0, 0},
	{"OA8", "A","A", &dataa_OA8, 0, 0},
	{"OI1", "A","A", &dataa_OI1, 0, 0},
	{"OI2", "A","A", &dataa_OI2, 0, 0},
	{"OI3", "A","A", &dataa_OI3, 0, 0},
	{"OI4", "A","A", &dataa_OI4, 0, 0},
	{"OI5", "A","A", &dataa_OI5, 0, 0},
	{"OI6", "A","A", &dataa_OI6, 0, 0},
	{"OI7", "A","A", &dataa_OI7, 0, 0},
	{"OI8", "A","A", &dataa_OI8, 0, 0},
	{""}};

/*************************************************************************
*
* Name:		dataa_parse()
*
* Type		int
*
* Type		Parameter	IOGF	Description
* char		*string		I	string to be parsed.
* char		*parse_char	I	parse charachter(s).
* char		*inc_parse_char	I	parse charachter(s) that will be
*					included in the parsed string.
* char		*outstr		O	parsed strings.
* int		max_rows	I	maximum number of chars in a parsed
*					string.
* int 		max_cols	I	maximum number of parsed elements.
*
* Description:
*	Parses a string.
*
**************************************************************************/

static int dataa_parse (
  char	*string,
  char	*parse_char,
  char	*inc_parse_char,
  char	*outstr,
  int	max_rows,
  int 	max_cols
)
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

	row = 0;
	col = 0;
	char_found = 0;
	next_token = 0;
	while ( *string != '\0')	
	{
	  char_ptr = parse_char;
	  inc_char_ptr = inc_parse_char;
	  parsechar_found = 0;
	  inc_parsechar_found = 0;
	  if ( *string == '"')
	  {
	    one_token = !one_token;
	    string++; 
	    continue;
	  }
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
	  string++; 
	  if ( col >= (max_cols - 1))
	    next_token = 1;	    
	}
	*(outstr + row * max_cols + col) = '\0';
	row++;

	if ( char_found == 0)
	  return 0;

	return row;
}

/*************************************************************************
*
* Name:		dataa_isascii
*
* Type		int
*
* char		c	I	a character
*
* Description:
*	Returns 1 if c is a ascii-character or digit, else return 0.
*
**************************************************************************/
static int	dataa_isascii( char c)
{
	if ( c >= '0' && c <= '9')
	  return 1;
	if ( c >= 'a' && c <= 'z')
	  return 1;
	if ( c >= 'A' && c <= 'Z')
	  return 1;
	if ( c >= '_' )
	  return 1;
	if ( c == 'å' || c == 'ä' || c == 'ö' || 
	     c == 'Å' || c == 'Ä' || c == 'Ö')
	  return 1;

	return 0;
}
/*************************************************************************
*
* Name:		dataa_isfrontA
*
* Type		int
*
* char		c	I	a character
*
* Description:
*	Returns 1 if c2 is a ascii-character, digit, or c1c2 is '->', else return 0.
*
**************************************************************************/
static int	dataa_isfrontA( char c1, char c2)
{
  if ( dataa_isascii( c2))
    return 1;
  if ( c1 == '-' && c2 == '>')
    return 1;

  return 0;
}
/*************************************************************************
*
* Name:		dataa_isbackA
*
* Type		int
*
* char		c	I	a character
*
* Description:
*	Returns 1 if c is a ascii-character, digit, '[' or '(', else return 0.
*
**************************************************************************/
static int	dataa_isbackA( char c)
{
  if ( dataa_isascii( c))
    return 1;
  if ( c == '[')
    return 1;
  if ( c == '(')
    return 1;

  return 0;
}

/*************************************************************************
*
* Name:		dataa_get_next_line
*
* Type		int
*
* Type		Parameter	IOGF	Description
* char		*str		I	buffer.
* char		**pos		IO	position in buffer for next line.
* char		*line		O	next line.
* char		size		I	max size of line.
* int		first		I	first call for this buffer.
*
* Description:
*	Get the next line of a buffer.
*
**************************************************************************/
static int	dataa_get_next_line( 	char *str, 
				char **pos, 
				char *line, 
				int size,
				int first)
{
	char	*s, *t;
	int	i;

	if ( first)
	  s = str;
	else
	  s = *pos;

	i = 0;
	t = line;
	while( i < size)
	{
	  if ( *s == 16 || *s == 10) break;
	  if ( *s == 0) break;
	  *t = *s;
	  i++;
	  t++;
	  s++;
	}
	*t = *s;
	if ( *s == 0) return 1;
	t++;
	*t = 0;
	*pos = s + 1;
	return 0;
}

/*************************************************************************
*
* Name:		dataa_add_item
*
* Type		int
*
* Type		Parameter	IOGF	Description
* dataa_ctx	dataactx	I	dataa context
* char		*item		I	new item
* char		*delim_front	I	front delimiter.
* char		*delim_back	I	back delimiter.
* int		*func()		I	callback function
*
* Description:
*	Insert a new item in the context's item list.
*
**************************************************************************/
static int dataa_add_item (
  dataa_ctx	dataactx,
  char 		*item, 
  char		*delim_front,
  char		*delim_back,
  pwr_tStatus	(* func) ()
)
{
	dataa_t_item	*item_ptr;

	item_ptr = dataactx->items;
	while( item_ptr->item[0] != 0)
	    item_ptr++;
	strcpy( item_ptr->item, item);
	strcpy( item_ptr->delim_front, delim_front);
	strcpy( item_ptr->delim_back, delim_back);
	item_ptr->hit = 0;
	item_ptr->hit_count = 0;
	item_ptr->func = func;
	item_ptr++;
	item_ptr->item[0] = 0;
	return GSX__SUCCESS;
}

/*************************************************************************
*
* Name:		dataa_create_ctx
*
* Type		int
*
* Type		Parameter	IOGF	Description
* dataa_ctx	*dataactx	I	dataa context
*
* Description:
*	Allocate memory and initiziate a new context.
*
**************************************************************************/
static pwr_tStatus dataa_create_ctx (
  dataa_ctx   *dataactx
)
{
	/* Create the context */
	*dataactx = (dataa_ctx) calloc( 1, sizeof( **dataactx ));
	if ( *dataactx == 0) 
	  return GSX__NOMEMORY;

	/* Initiziate the item list */
	memcpy( (*dataactx)->items, template_items, sizeof(template_items));

	return GSX__SUCCESS;
}


/*************************************************************************
*
* Name:		dataa_delete_ctx()
*
* Type		void
*
* Type		Parameter	IOGF	Description
* dataa_ctx	dataactx	I	dataa context.
*
* Description:
*	Delete a context.
*	Free's all allocated memory in the dataa context.
*
**************************************************************************/

static pwr_tStatus dataa_delete_ctx (
  dataa_ctx	dataactx
)
{

	free((char *) dataactx);
	return GSX__SUCCESS;
}
  
/*************************************************************************
*
* Name:		dataarithm_convert
*
* Type		int
*
* Type		Parameter	IOGF	Description
* char		*str		I	code in dataarithm object
* char		*newstr		I	converted code.
* char		*object		I	name of object pointer.
* char		*bufsize	I	size of newstr.
*
* Description:
*	Convert the code of a dataarithm object.
*
**************************************************************************/
pwr_tStatus dataarithm_convert (
  char *str,
  char *newstr,
  char *object, 
  int  bufsize,
  char *error_line,
  int  *error_line_size,
  int  *error_line_num,
  int  *outsize
)
{
	char		line[200];
	char		*p;
	dataa_t_item	*item_ptr;
	pwr_tStatus	sts;
	char		*t;
	char		*delim_p;
	int		hit, delim_hit;
	char		tmpstr[160];
	char		*write_from;
	int		first_line;
	int		end_of_text;
	dataa_ctx 	dataactx;
	int		pos;
	int		incr;


	sts = dataa_create_ctx( &dataactx);
	if ( EVEN(sts)) return sts;

	*newstr = 0;
	pos = 0;
	sprintf( newstr, "{\n");
	pos += strlen("{\n");
	first_line = 1;
	while( 1)
	{
	  end_of_text = dataa_get_next_line( str, &p, line, sizeof(line),
			first_line);
	  dataactx->line_count++;
	  first_line = 0;
	  item_ptr = dataactx->items;
	  while( item_ptr->item[0] != 0)
	  {
	    item_ptr->hit = 0;
	    item_ptr->hit_count = 0;
	    item_ptr++;
	  }
	  hit = 0;

	  if ( strstr( line, "classdef"))
	  {
	    sts = dataa_classdef( dataactx, line);
	    if ( EVEN(sts))
	    {
	      strncpy( error_line, line, *error_line_size);
	      *error_line_num = dataactx->line_count,
	      dataa_delete_ctx( dataactx);
	      return sts;
	    }
	  }
	  else if ( strstr( line, "structdef"))
	  {
	    sts = dataa_structdef( dataactx, line);
	    if ( EVEN(sts))
	    {
	      strncpy( error_line, line, *error_line_size);
	      *error_line_num = dataactx->line_count,
	      dataa_delete_ctx( dataactx);
	      return sts;
	    }
	  }
	  else if ( strstr( line, "aliasdef"))
	  {
	    sts = dataa_aliasdef( dataactx, line);
	    if ( EVEN(sts))
	    {
	      strncpy( error_line, line, *error_line_size);
	      *error_line_num = dataactx->line_count,
	      dataa_delete_ctx( dataactx);
	      return sts;
	    }
	  }
	  else
	  {
	    t = line;
	    write_from = t;
	    item_ptr = dataactx->items;
	    while( item_ptr->item[0] != 0)
	    {
	      if ( item_ptr->hit == 0)
	      {
	        /* Check front delimiter */
	        delim_p = &item_ptr->delim_front[0];
	        while( *delim_p != 0)
	        {
	          if ( *delim_p == 'A' || *delim_p == 'X')
		  {
	            item_ptr->hit = 1;
	            item_ptr->hit_count = 1;
	          }
	          delim_p++;
	        }
	      }
	      item_ptr++;
	    }

	    while( *t != 0)
	    {
	      item_ptr = dataactx->items;
	      while( item_ptr->item[0] != 0)
	      {
	        if ( item_ptr->hit == 0)
	        {
	          /* Check front delimiter */
	          delim_p = &item_ptr->delim_front[0];
	          while( *delim_p != 0)
	          {
	  	    if ( ( *delim_p == 'A' && !dataa_isfrontA( t == line ? '\0' : *(t-1), *t)) ||
		         ( *delim_p == 'X') ||
		         ( *delim_p == *t && *delim_p != 'A'))
		    {
	              item_ptr->hit = 1;
	              item_ptr->hit_count = 1;
	            }
	            delim_p++;
	          }
	        }
	        else
	        {
	          if ( item_ptr->hit_count >= 1 && 
		          item_ptr->hit_count < strlen( item_ptr->item) + 1)
		  {
		    if ( item_ptr->item[item_ptr->hit_count-1] == *t)
		    {
		      /* Still hit */
		      item_ptr->hit_count++;
		    }
	            else
		    {
		      delim_hit = 0;
	              if ( item_ptr->hit_count == 1)
	              {
	                /* More delimiter */
	                delim_p = &item_ptr->delim_front[0];
	                while( *delim_p != 0)
	                {
	  	          if (( *delim_p == 'A' && !dataa_isfrontA( t == line ? '\0' : *(t-1), *t)) ||
		              ( *delim_p == 'X'))
	                    delim_hit = 1;
		          else if ( *delim_p == *t)
	                    delim_hit = 1;
	                  delim_p++;
	                }
	              }
		      if ( !delim_hit)
	              {
	                item_ptr->hit = 0;
	                item_ptr->hit_count = 0;
	              }
	            }
		  }
	          else if ( item_ptr->hit_count == strlen( item_ptr->item) + 1)
		  {
	            delim_p = &item_ptr->delim_back[0];
	            while( *delim_p != 0)
	            {
	  	      if (( *delim_p == 'A' && !dataa_isbackA(*t)) ||
		          ( *delim_p == 'X'))
		      {
		        hit = 1;
	              }
		      else if ( *delim_p == *t)
		      {
		        hit = 1;
	              }
	              delim_p++;
	            }
	 	    if ( hit) 
		      break;

	            item_ptr->hit = 0;
	            item_ptr->hit_count = 0;
		  }
	        }
	        item_ptr++;
	      }
	      if ( hit)
	      {
	        if ( dataactx->comment && 
	 	   !(item_ptr->func == &dataa_comment_end))
 	        {
	          /* Reset all except the comment_end (first item)*/
	          hit = 0;
	          item_ptr = dataactx->items;
	          item_ptr++;
	          while( item_ptr->item[0] != 0)
	          {
	            item_ptr->hit = 0;
	            item_ptr->hit_count = 0;
	            item_ptr++;
	          }
	        }
	        else
                {
	          /* Write line */
		  strcpy( tmpstr, write_from);
	          tmpstr[ t - write_from - strlen(item_ptr->item)] = 0;
		  strcpy( newstr+pos, tmpstr);
		  pos += strlen(tmpstr);
	          sts = (item_ptr->func) (dataactx, line, t, newstr+pos, &incr,
				object);
	          if ( EVEN(sts))
	          {
	            strncpy( error_line, line, *error_line_size);
	            *error_line_num = dataactx->line_count,
	            dataa_delete_ctx( dataactx);
	            return sts;
	          }
		  pos += incr;
	          write_from = t;
	          item_ptr = dataactx->items;
	          while( item_ptr->item[0] != 0)
	          {
	            item_ptr->hit = 0;
	            item_ptr->hit_count = 0;
	            item_ptr++;
	          }
	          hit = 0;
	        }
	      }
	      else
	        t++;
	    }
	    strcpy( newstr+pos, write_from);
	    pos += strlen(write_from);
	    newstr[ pos] = 0;
	    if ( pos > bufsize - 100)
	    {
	      dataa_delete_ctx( dataactx);
	      return GSX__DATAABUFSIZE;
	    }
	  }
	  if ( end_of_text)
	    break;
	}

	sprintf( newstr + strlen(newstr), "}\n");
	pos += strlen("}\n");
	*outsize = pos;
	dataa_delete_ctx( dataactx);
	return GSX__SUCCESS;
}

#ifdef TEST

main()
{
	int	sts;
	char	object_var[] = "Z800022";
	char	newstr[16000];
	char	str[2000] = "classdef Da1 Mtrl\n\
A1 = 25;\n\
if ( !Da1 && d1)\n\
{\n\
  od1 = 1;\n\
  Da1->Temp = 22;\n\
}";
	FILE *infile;
	char	error_line[80];
	int	error_num;
	
	str[0] = 0;
	infile = fopen( "dataa.txt", "r");
	while ( sts != NULL)
	  sts = fgets( str + strlen(str), sizeof(str), infile);
	fclose( infile);

	sts = dataarithm_convert( str, newstr, object_var, sizeof(newstr),
		error_line, sizeof(error_line), &error_num);
	if (EVEN(sts))
	  printf( "Error in line %d,\n  %s\n", error_num, error_line);
	printf( "sts : %d\n", sts);
	printf( "%s", newstr);
	sts = dataarithm_convert( str, newstr, object_var, sizeof(newstr),
		error_line, sizeof(error_line), &error_num);
	if (EVEN(sts))
	  printf( "Error in line %d,\n  %s\n", error_num, error_line);

	printf( "sts : %d\n", sts);
	printf( "%s", newstr);
}
#endif
