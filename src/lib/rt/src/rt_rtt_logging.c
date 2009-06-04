/* 
 * Proview   $Id: rt_rtt_logging.c,v 1.6 2007-07-05 07:28:36 claes Exp $
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

/* rt_rtt_logging.c 
   This module contains routines for handling of logging in rtt. */

/*_Include files_________________________________________________________*/



#if defined(OS_ELN)
# include stdio
# include string
# include stdlib
# include descrip
# include chfdef
# include stdarg
# include $vaxelnc
# include $kerneldef
# include $mutex
# include prdef
# include $kernelmsg
# include $elnmsg
# include $get_message_text
#elif defined(OS_VMS)
# include <stdio.h>
# include <string.h>
# include <stdlib.h>
# include <descrip.h>
# include <chfdef.h>
# include <stdarg.h>
# include <processes.h>
# include <lib$routines.h>
# include <libdef.h>
# include <libdtdef.h>
# include <starlet.h>
#else
# include <stdarg.h>
# include <stdio.h>
# include <string.h>
# include <stdlib.h>
# include <pthread.h>
#endif
#if defined OS_LINUX
# include <time.h>
#endif

#include "pwr.h"
#include "pwr_class.h"
#include "pwr_baseclasses.h"
#include "co_cdh.h"
#include "co_time.h"
#include "rt_gdh.h"
#include "rt_gdh_msg.h"
#include "rt_rtt.h"
#include "rt_rtt_edit.h"
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

/* Local static variables */
#if defined OS_ELN
static int      psts;
#endif

/*_Local function prototypes____________________________________________*/

static int	rtt_logging_show_entry(
			int		entry,
			char		*buff,
			int		*buff_cnt);
static int	rtt_logging_entry_stop(
			rtt_t_loggtable	*entry_ptr);
#if defined(OS_ELN)
pwr_tStatus	rtt_logging_logproc(
			rtt_t_loggtable	*entry_ptr);
#else
void	*rtt_logging_logproc(void *arg);
#endif
static int	rtt_get_parinfo(
			char		*parameter_name,
			pwr_sParInfo	*parinfo);
static int	log_print( 
		rtt_t_loggtable	*entry_ptr,
		char		*format,
		...);
static int	log_print_buffer(
			rtt_t_loggtable	*entry_ptr);



/*************************************************************************
*
* Name:		rtt_logging_create()
*
* Type		int
*
* Type		Parameter	IOGF	Description
*
* Description:
*	Create an entry in the logging table.
*
**************************************************************************/

int	rtt_logging_create(
			menu_ctx	ctx,
			int		entry,
			int		logg_time,
			char		*filename,
			char		*parameterstr,
			char		*conditionstr,
			int		logg_type,
			int		insert,
			int		buffer_size,
			int		stop,
			int		priority,
			int		line_size,
			int		shortname)
{
	int		sts;
	int		i;
	int		found;
	rtt_t_loggtable	*entry_ptr;
	char		buffer[4];
	pwr_sParInfo	parinfo;
	rtt_t_menu_upd	*menu_ptr;
	char		msg[80];
	int		type_error;

	if ( entry == 0)
	{
	  /* Find a not occupied entry */
	  found = 0;
	  entry_ptr = rtt_loggtable;

	  for ( i = 0; i < RTT_LOGG_MAXENTRY; i++)
	  {
	    if ( !entry_ptr->occupied )	
	    {
	      found = 1;
	      break;
	    }
	    entry_ptr++;
	  }
	  if ( !found)
	  {
	    rtt_message('E', "Logging table is full");
	    return RTT__HOLDCOMMAND;
	  }
	}
	else
	{
	  /* Create the selected entry */
	  if ( ( entry < 1) || ( entry > RTT_LOGG_MAXENTRY))
	  {
	    rtt_message('E',"Entry out of range");
	    return RTT__HOLDCOMMAND;
	  } 

	  /* Get the entry */
	  entry_ptr = rtt_loggtable + entry - 1;

	  if ( entry_ptr->occupied )	
	  {
	    rtt_message('E',"Entry is already created");
	    return RTT__HOLDCOMMAND;
	  } 
	}

	if ( parameterstr != NULL)
	{
	  /* Check that parameter exists */
	  sts = gdh_GetObjectInfo ( parameterstr, &buffer, sizeof(buffer)); 
	  if (EVEN(sts))
	  {
	    rtt_message('E',"Parameter doesn't exist");
	    return RTT__HOLDCOMMAND;
	  }

	  sts = rtt_get_parinfo( parameterstr, &parinfo);
	  if (EVEN(sts))
	  {
	    rtt_message('E',"Parameter doesn't exist");
	    return RTT__HOLDCOMMAND;
	  }
	  strcpy ( entry_ptr->parameterstr[0], parameterstr); 
	  entry_ptr->parameter_type[0] = parinfo.Type; 	  
	}

	if ( buffer_size != 0)
	  entry_ptr->buffer_size = buffer_size;
	else
	  /* Default value */
	  entry_ptr->buffer_size = RTT_BUFFER_DEFSIZE;
	
	/* Allocate a buffer to store logging info in */
	entry_ptr->buffer_ptr = (char *) calloc( 1, 
			entry_ptr->buffer_size * 512);
	if (entry_ptr->buffer_ptr == 0)
	{
	  rtt_message('E', 
		"Buffer is to large, entry is not created");
	  return RTT__HOLDCOMMAND;
	}

	entry_ptr->occupied = 1;
	rtt_message('I', "Logging entry created");

	/* Insert in the entry */
	if ( filename != NULL)
	{
	  rtt_get_defaultfilename( filename, entry_ptr->logg_filename, 
		".dat");
	}
	else
	{
	  /* User default file name */
	  rtt_get_defaultfilename( "rtt_logging", entry_ptr->logg_filename,
		".dat");
	}

	if ( conditionstr != NULL)
	{
	  /* Check that parameter exists */
	  sts = gdh_GetObjectInfo ( conditionstr, &buffer, sizeof(buffer)); 
	  if (EVEN(sts))
	  {
	    rtt_message('E',"Condition doesn't exist");
	    return RTT__HOLDCOMMAND;
	  }
	  strcpy ( entry_ptr->conditionstr, conditionstr); 
	}
	if ( logg_time != 0)
	  entry_ptr->logg_time = logg_time;
	else
	  /* Default value */
	  entry_ptr->logg_time = 2000;
	
	if ( logg_type != 0)
	  entry_ptr->logg_type = logg_type;
	else
	  /* Default value */
	  entry_ptr->logg_type = RTT_LOGG_CONT;

	if ( line_size != 0)
	  entry_ptr->line_size = line_size;
	else
	  entry_ptr->line_size = RTT_LOGG_LINE_DEFSIZE;

	if ( shortname != -1)
	  entry_ptr->print_shortname = shortname;

	if ( priority < -1 || priority > 32)
	{
	    rtt_message('E',"Priority out of range");
	    return RTT__HOLDCOMMAND;
	}
	else if ( priority == -1)
	  entry_ptr->logg_priority = 0;
	else
	  entry_ptr->logg_priority = priority;

	if ( stop != -1)
	  entry_ptr->intern = stop;
	else
	  entry_ptr->intern = 0;

	if ( insert)
	{
	  /* Insert from collection picture */

	  /* Get items in collection picture */
	  if ( rtt_collectionmenuctx == 0)
	  {
	    rtt_message('E', "No objects in collection picture");
	    return RTT__NOPICTURE;
	  }
	  if ( rtt_collectionmenuctx->menu == 0)
	  {
	    rtt_message('E', "No objects in collection picture");
	    return RTT__NOPICTURE;
	  }
	    
	  i = 0;
	  type_error = 0;
	  menu_ptr = (rtt_t_menu_upd *) rtt_collectionmenuctx->menu;
	  while ( menu_ptr->text[0] != '\0')
	  {
	    if ( i >= RTT_LOGG_MAXPAR )
	    {
	      rtt_message('E', "Max number of parameters exceeded");
	      break;
	    }
	    strcpy(  entry_ptr->parameterstr[i], menu_ptr->parameter_name);
	    entry_ptr->parameter_type[i] = menu_ptr->value_type;
	    switch( entry_ptr->parameter_type[i])
	    {
	      case pwr_eType_Float32:
	      case pwr_eType_Float64:
	      case pwr_eType_UInt8:
	      case pwr_eType_Boolean:
	      case pwr_eType_Char:
	      case pwr_eType_Int8:
	      case pwr_eType_Int16:
	      case pwr_eType_UInt16:
	      case pwr_eType_Int32:
	      case pwr_eType_UInt32:
	      case pwr_eType_Int64:
	      case pwr_eType_UInt64:
	      case pwr_eType_Objid:
	      case pwr_eType_AttrRef:
	      case pwr_eType_Time:
	        break;
	      default:
	        sprintf( msg, "Error in parameter nr %d: type is not supported", i+1);
	        rtt_message('E', msg);
                type_error = 1;
	    }
	    menu_ptr++;
	    i++;
	  }
	  if ( i < RTT_LOGG_MAXPAR && !type_error)
	    rtt_message('I', "Parameters copied");
	}

	/* Count the parameters */
	entry_ptr->parameter_count = 0;
	for ( i = 0; i < RTT_LOGG_MAXPAR; i++)
	{
	  if ( entry_ptr->parameterstr[i][0] != 0)
	    entry_ptr->parameter_count++;
	}

#ifdef OS_VMS
	sts = lib$get_ef(&entry_ptr->event_flag);
	if (EVEN(sts)) return sts;
#endif
	return RTT__NOPICTURE;
}

/*************************************************************************
*
* Name:		rtt_logging_set()
*
* Type		int
*
* Type		Parameter	IOGF	Description
*
* Description:
*	Modify parameter in the logging table.
*
**************************************************************************/

int	rtt_logging_set(
			menu_ctx	ctx,
			int		entry,
			int		logg_time,
			char		*filename,
			char		*parameterstr,
			char		*conditionstr,
			int		logg_type,
			int		insert,
			int		buffer_size,
			int		stop,
			int		priority,
			int		create,
			int		line_size,
			int		shortname)
{
	int		i, sts;
	int		found, par_index;
	rtt_t_loggtable	*entry_ptr;
	char		buffer[8];
	pwr_sParInfo	parinfo;
	rtt_t_menu_upd	*menu_ptr;
	char		msg[80];
	int		type_error;
	
	/* Check the entry */
	if ( ( entry < 1) || ( entry > RTT_LOGG_MAXENTRY))
	{
	  rtt_message('E',"Entry out of range");
	  return RTT__HOLDCOMMAND;
	} 

	/* Get the entry */
	entry_ptr = rtt_loggtable + entry - 1;
	if ( create)
	{
	  if ( entry_ptr->occupied)
	  {
	    /* Reset the entry */
	    if ( entry_ptr->buffer_ptr != 0)
	      free( entry_ptr->buffer_ptr);
	    memset( entry_ptr, 0, sizeof( *entry_ptr));
	  }
          sts = rtt_logging_create( ctx, entry, 0, NULL, NULL, NULL, 0, 0, 0, 
		-1, -1, 0, -1);
	}
	else
	{
	  if ( !entry_ptr->occupied )	
	  {
	    rtt_message('E',"Entry is not created");
	    return RTT__HOLDCOMMAND;
	  }
	} 
	if ( entry_ptr->active )	
	{
	  rtt_message('E',"Unable to modify entry, entry is started");
	  return RTT__HOLDCOMMAND;
	} 

	if ( buffer_size != 0)
	{
	  entry_ptr->buffer_size = buffer_size;
	
	  /* Reallocate the buffer to store logging info in */
	  if ( entry_ptr->buffer_ptr != 0)
	    free( entry_ptr->buffer_ptr);
	  entry_ptr->buffer_ptr = calloc( 1, entry_ptr->buffer_size * 512);
	  if (entry_ptr->buffer_ptr == 0)
	  {
	    rtt_message('E', "Buffer is to large");
	    /* set default buffer */
	    entry_ptr->buffer_size = RTT_BUFFER_DEFSIZE;
	
	    /* Reallocate the buffer to store logging info in */
	    entry_ptr->buffer_ptr = calloc( 1, entry_ptr->buffer_size * 512);
	    if (entry_ptr->buffer_ptr == 0)
	      exit( RTT__NOMEMORY);
	  }
	}

	/* Insert in the entry */
	if ( filename != NULL)
	{
	  rtt_get_defaultfilename( filename, entry_ptr->logg_filename, 
		".dat");
	}
	if ( parameterstr != NULL)
	{
	  /* Get a free parameter index */
	  found = 0;
	  for ( i = 0; i < RTT_LOGG_MAXPAR; i++)
	  {
	    if ( entry_ptr->parameterstr[i][0] == 0)
	    {
	      found = 1;
	      par_index = i;
	      break;
	    }
	  }
	  if ( !found) 
	  {
	    rtt_message('E', "Max number of parameters exceeded");
	    return RTT__HOLDCOMMAND;
	  }

	  /* Check that parameter exists */
	  sts = gdh_GetObjectInfo ( parameterstr, &buffer, sizeof(buffer));
	  if (EVEN(sts))
	  {
	    rtt_message('E',"Parameter doesn't exist");
	    return RTT__HOLDCOMMAND;
	  }

	  sts = rtt_get_parinfo( parameterstr, &parinfo);
	  if (EVEN(sts))
	  {
	    rtt_message('E',"Parameter doesn't exist");
	    return RTT__HOLDCOMMAND;
	  }
	  strcpy ( entry_ptr->parameterstr[ par_index], parameterstr); 
	  entry_ptr->parameter_type[ par_index] = parinfo.Type; 	  
	  entry_ptr->parameter_size[ par_index] = parinfo.Size/parinfo.Elements;
	}

	if ( conditionstr != NULL)
	{
	  /* Check that parameter exists */
	  sts = gdh_GetObjectInfo ( conditionstr, &buffer, sizeof(buffer)); 
	  if (EVEN(sts))
	  {
	    rtt_message('E',"Condition doesn't exist");
	    return RTT__HOLDCOMMAND;
	  }
	  strcpy ( entry_ptr->conditionstr, conditionstr);
	}
	if ( logg_time != 0)
	  entry_ptr->logg_time = logg_time;

	if ( buffer_size != 0)
	  entry_ptr->buffer_size = buffer_size;

	if ( logg_type != 0)
	  entry_ptr->logg_type = logg_type;

	if ( priority < -1 || priority > 32)
	{
	    rtt_message('E',"Priority out of range");
	    return RTT__HOLDCOMMAND;
	}
	else if ( priority != -1)
	  entry_ptr->logg_priority = priority;

	if ( line_size != 0)
	  entry_ptr->line_size = line_size;

	if ( shortname != -1)
	  entry_ptr->print_shortname = shortname;

	if ( stop != -1)
	  entry_ptr->intern = stop;

	if ( insert)
	{
	  /* Insert from collection picture */

	  /* Get items in collection picture */
	  if ( rtt_collectionmenuctx == 0)
	  {
	    rtt_message('E', "No objects in collection picture");
	    return RTT__NOPICTURE;
	  }
	  if ( rtt_collectionmenuctx->menu == 0)
	  {
	    rtt_message('E', "No objects in collection picture");
	    return RTT__NOPICTURE;
	  }
	    
	  /* Clear all parameters */
	  memset( &(entry_ptr->parameterstr), 0, 
			sizeof( entry_ptr->parameterstr));
	  i = 0;
	  type_error = 0;
	  menu_ptr = (rtt_t_menu_upd *) rtt_collectionmenuctx->menu;
	  while ( menu_ptr->text[0] != '\0')
	  {
	    if ( i >= RTT_LOGG_MAXPAR )
	    {
	      rtt_message('E', "Max number of parameters exceeded");
	      break;
	    }
	    strcpy(  entry_ptr->parameterstr[i], menu_ptr->parameter_name);
	    entry_ptr->parameter_type[i] = menu_ptr->value_type; 
	    switch( entry_ptr->parameter_type[i])
	    {
	      case pwr_eType_Float32:
	      case pwr_eType_Float64:
	      case pwr_eType_UInt8:
	      case pwr_eType_Boolean:
	      case pwr_eType_Char:
	      case pwr_eType_Int8:
	      case pwr_eType_Int16:
	      case pwr_eType_UInt16:
	      case pwr_eType_Int32:
	      case pwr_eType_UInt32:
	      case pwr_eType_Int64:
	      case pwr_eType_UInt64:
	      case pwr_eType_Objid:
	      case pwr_eType_AttrRef:
	      case pwr_eType_Time:
	        break;
	      default:
	        sprintf( msg, "Error in parameter nr %d: type is not supported", i+1);
	        rtt_message('E', msg);
	        type_error = 1;
	    }
	    menu_ptr++;
	    i++;
	  }
	  if ( i < RTT_LOGG_MAXPAR && !type_error)
	    rtt_message('I', "Parameters copied");
	}

	/* Count the parameters */
	entry_ptr->parameter_count = 0;
	for ( i = 0; i < RTT_LOGG_MAXPAR; i++)
	{
	  if ( entry_ptr->parameterstr[i][0] != 0)
	    entry_ptr->parameter_count++;
	}


	return RTT__NOPICTURE;
}

/*************************************************************************
*
* Name:		rtt_logging_show()
*
* Type		int
*
* Type		Parameter	IOGF	Description
*
* Description:
*	Show the entry.
*
**************************************************************************/

int	rtt_logging_show(
			menu_ctx	ctx,
			int		entry)
{
	int		sts;
	int		i;
	int		found;
	rtt_t_loggtable	*entry_ptr;
	char		*buff;
	int		buff_cnt;
	
	/* Check the entry */
	if ( ( entry < 0) || ( entry > RTT_LOGG_MAXENTRY))
	{
	  rtt_message('E',"Entry out of range");
	  return RTT__HOLDCOMMAND;
	} 

	if ( entry > 0)
	{
	  /* Show this entry */
	  /* Get the entry */
	  entry_ptr = rtt_loggtable + entry - 1;
	  if ( !entry_ptr->occupied )	
	  {
	    rtt_message('E',"Entry is not created");
	    return RTT__HOLDCOMMAND;
	  }   
	  rtt_clear_screen();

	  buff = calloc( 1, 120000);
	  if ( buff == 0)
	    return RTT__NOMEMORY;
	  buff_cnt = 0;
	  rtt_logging_show_entry( entry, buff, &buff_cnt);
	  sts = rtt_view( 0, 0, buff, "Show logging entry", 
			RTT_VIEWTYPE_BUF);
	  free( buff);
	  return sts;
	}
	else
	{
	  /* Show all entries */
	  /* Check that there is an occupied entry */
	  found = 0;
	  entry_ptr = rtt_loggtable;
	  for ( i = 0; i < RTT_LOGG_MAXENTRY; i++)
	  {
	    if ( entry_ptr->occupied )	
	    {
	      found = 1;
	    }
	    entry_ptr++;
	  }
	  if ( !found)
	  {
	    rtt_message('E', "No entries found");
	    return RTT__HOLDCOMMAND;
	  }
	  
	  buff = calloc( 1, 120000);
	  if ( buff == 0)
	    return RTT__NOMEMORY;
	  buff_cnt = 0;
	  entry_ptr = rtt_loggtable;
	  for ( i = 0; i < RTT_LOGG_MAXENTRY; i++)
	  {
	    if ( entry_ptr->occupied )	
	    {
	      rtt_logging_show_entry( i + 1, buff, &buff_cnt);
	    }
	    entry_ptr++;
	  }
	  sts = rtt_view( 0, 0, buff, "Show logging entry", 
			RTT_VIEWTYPE_BUF);
	  free( buff);
	  return sts;
	}
}

/*************************************************************************
*
* Name:		rtt_logging_show_entry()
*
* Type		int
*
* Type		Parameter	IOGF	Description
*
* Description:
*	Show one entry.
*
**************************************************************************/

static int	rtt_logging_show_entry(
			int		entry,
			char		*buff,
			int		*buff_cnt)
{
	rtt_t_loggtable	*entry_ptr;
	int		i;
	int		par_cnt;

	entry_ptr = rtt_loggtable + entry - 1;
	(*buff_cnt) += sprintf( buff + *buff_cnt, 
		"   Entry:     %d", entry);

	if ( entry_ptr->active)
	  (*buff_cnt) += sprintf( buff + *buff_cnt, "       ACTIVE\n");
	else
	  (*buff_cnt) += sprintf( buff + *buff_cnt, "       NOT ACTIVE\n");

	if ( entry_ptr->logg_type == RTT_LOGG_MOD)                      
	  (*buff_cnt) += sprintf( buff + *buff_cnt, 
		"   Type:      Event\n");
	else if ( entry_ptr->logg_type == RTT_LOGG_CONT)
	  (*buff_cnt) += sprintf( buff + *buff_cnt, 
		"   Type:      Cont\n");

	(*buff_cnt) += sprintf( buff + *buff_cnt, 
		"   Time:      %d ms\n", entry_ptr->logg_time);
	(*buff_cnt) += sprintf( buff + *buff_cnt, 
		"   Buffer:    %d pages\n", entry_ptr->buffer_size);
	(*buff_cnt) += sprintf( buff + *buff_cnt, 
		"   Priority:  %d\n", entry_ptr->logg_priority);
	(*buff_cnt) += sprintf( buff + *buff_cnt, 
		"   Line size: %d\n", entry_ptr->line_size);
	if ( entry_ptr->intern) 
	  (*buff_cnt) += sprintf( buff + *buff_cnt, 
		"   Stop when buffer is full\n");
	(*buff_cnt) += sprintf( buff + *buff_cnt, 
		"   Filename:  %s\n", entry_ptr->logg_filename);
	(*buff_cnt) += sprintf( buff + *buff_cnt, 
		"   Number of parameters: %d\n", entry_ptr->parameter_count);
	par_cnt = 0;
	for ( i = 0; i < RTT_LOGG_MAXPAR; i++)
	{
	  if ( entry_ptr->parameterstr[i][0] != 0)
	  {
	    par_cnt++;
	    (*buff_cnt) += sprintf( buff + *buff_cnt, "Parameter%d :	%s\n", 
		par_cnt, entry_ptr->parameterstr[i]);
	  }
	}
	(*buff_cnt) += sprintf( buff + *buff_cnt, "Condition:	%s\n", entry_ptr->conditionstr);
	(*buff_cnt) += sprintf( buff + *buff_cnt, "\n");

	return RTT__SUCCESS;
}

/*************************************************************************
*
* Name:		rtt_logging_store_entry()
*
* Type		int
*
* Type		Parameter	IOGF	Description
*
* Description:
*	Store one entry.
*
**************************************************************************/

int	rtt_logging_store_entry(
			int		entry,
			char		*filename)
{
	rtt_t_loggtable	*entry_ptr;
	int		i;
	pwr_tFileName  	filename_str;
	FILE		*outfile;
	char		message[120];
	int		found_parameter;

	/* Check the entry */
	if ( ( entry < 1) || ( entry > RTT_LOGG_MAXENTRY))
	{
	  rtt_message('E',"Entry out of range");
	  return RTT__HOLDCOMMAND;
	} 

	entry_ptr = rtt_loggtable + entry - 1;
	if ( !entry_ptr->occupied )
	{
	  rtt_message('E', "Logging entry is not created");
	  return RTT__NOPICTURE;
	}

	found_parameter = 0;
	for ( i = 0; i < RTT_LOGG_MAXPAR; i++)
	{
	  if ( entry_ptr->parameterstr[i][0] != 0)
	    found_parameter++;
	}
	if ( !found_parameter )
	{
	  rtt_message('E', "No parameters found in Logging entry");
	  return RTT__NOPICTURE;
	}

	rtt_get_defaultfilename( filename, filename_str, ".rtt_com");

	outfile = fopen( filename_str, "w");
	if ( outfile == 0)
	{
	  rtt_message('E', "Unable to open file");
	  return RTT__HOLDCOMMAND;
	}

	fprintf( outfile, "logging set/create/entry=current/file=\"%s\"\n",
		entry_ptr->logg_filename);
	if ( entry_ptr->logg_time != 0)
	  fprintf( outfile, "logging set/entry=current/time=%d\n",
		entry_ptr->logg_time);
	fprintf( outfile, "logging set/entry=current/buffer=%d\n", 
		entry_ptr->buffer_size);
	fprintf( outfile, "logging set/entry=current/line_size=%d\n", 
		entry_ptr->line_size);
	fprintf( outfile, "logging set/entry=current/priority=%d\n", 
		entry_ptr->logg_priority);
	if ( entry_ptr->print_shortname)
	  fprintf( outfile, "logging set/entry=current/shortname\n");
	else
	  fprintf( outfile, "logging set/entry=current/noshortname\n");

	if ( entry_ptr->logg_type == RTT_LOGG_MOD)                      
	  fprintf( outfile, "logging set/entry=current/type=event\n");
	else if ( entry_ptr->logg_type == RTT_LOGG_CONT)
	  fprintf( outfile, "logging set/entry=current/type=cont\n");

	for ( i = 0; i < RTT_LOGG_MAXPAR; i++)
	{
	  if ( entry_ptr->parameterstr[i][0] != 0)
	    fprintf( outfile, "logging set/entry=current/parameter=\"%s\"\n",
		entry_ptr->parameterstr[i]);
	}
	if ( entry_ptr->conditionstr[0] != 0)
	  fprintf( outfile, "logging set/entry=current/condition=\"%s\"\n", 
		entry_ptr->conditionstr);

	if ( entry_ptr->intern )
	  fprintf( outfile, "logging set/entry=current/stop\n");
	else
	  fprintf( outfile, "logging set/entry=current/nostop\n");

	rtt_fgetname( outfile, filename_str, filename_str);
	fclose( outfile);

	sprintf( message, "%s created", filename_str);
	rtt_message('I', message);
	return RTT__NOPICTURE;
}
/*************************************************************************
*
* Name:		rtt_logging_store_all()
*
* Type		int
*
* Type		Parameter	IOGF	Description
*
* Description:
*	Store all entries.
*
**************************************************************************/

int	rtt_logging_store_all(
			char		*filename)
{
	rtt_t_loggtable	*entry_ptr;
	int		i, j;
	pwr_tFileName  	filename_str;
	FILE		*outfile;
	char		message[120];
	int		entry;


	rtt_get_defaultfilename( filename, filename_str, ".rtt_com");

	outfile = fopen( filename_str, "w");
	if ( outfile == 0)
	{
	  rtt_message('E', "Unable to open file");
	  return RTT__HOLDCOMMAND;
	}

	for ( j = 0; j < RTT_LOGG_MAXENTRY; j++)
	{
	  entry = j + 1;
	  entry_ptr = rtt_loggtable + entry - 1;

	  if ( !entry_ptr->occupied )	
	    continue;

	  fprintf( outfile, "logging set/create/entry=%d/file=%s\n",
		entry, entry_ptr->logg_filename);
	  if ( entry_ptr->logg_time != 0)
	    fprintf( outfile, "logging set/entry=%d/time=%d\n",
		entry, entry_ptr->logg_time);
	  fprintf( outfile, "logging set/entry=%d/buffer=%d\n", 
		entry, entry_ptr->buffer_size);
	  fprintf( outfile, "logging set/entry=%d/priority=%d\n", 
		entry, entry_ptr->logg_priority);
	  if ( entry_ptr->print_shortname)
	    fprintf( outfile, "logging set/entry=%d/shortname\n", 
		entry);
	  else
	    fprintf( outfile, "logging set/entry=%d/noshortname\n", 
		entry);

	  if ( entry_ptr->logg_type == RTT_LOGG_MOD)
            fprintf( outfile, "logging set/entry=%d/type=event\n", entry);
  	  else if ( entry_ptr->logg_type == RTT_LOGG_CONT)
	    fprintf( outfile, "logging set/entry=%d/type=cont\n", entry);

	  for ( i = 0; i < RTT_LOGG_MAXPAR; i++)
	  {
	    if ( entry_ptr->parameterstr[i][0] != 0)
	      fprintf( outfile, "logging set/entry=%d/parameter=\"%s\"\n",
		entry, entry_ptr->parameterstr[i]);
	  }
	  if ( entry_ptr->conditionstr[0] != 0)
	    fprintf( outfile, "logging set/entry=%d/condition=\"%s\"\n", 
		entry, entry_ptr->conditionstr);
	  if ( entry_ptr->intern )
	    fprintf( outfile, "logging set/entry=%d/stop\n", entry);
	  else
	    fprintf( outfile, "logging set/entry=%d/nostop\n", entry);

	}
	rtt_fgetname( outfile, filename_str, filename_str);
	fclose( outfile);

	sprintf( message, "%s created", filename_str);
	rtt_message('I', message);
	return RTT__NOPICTURE;
}

/*************************************************************************
*
* Name:		rtt_logging_start()
*
* Type		int
*
* Type		Parameter	IOGF	Description
*
* Description:
*	Start the entry.
*
**************************************************************************/

int	rtt_logging_start(
			menu_ctx	ctx,
			int		entry)
{
	int		sts;
	int		i, found;
	rtt_t_loggtable	*entry_ptr;
	char		message[256];
	
	/* Check the entry */
	if ( ( entry < 1) || ( entry > RTT_LOGG_MAXENTRY))
	{
	  rtt_message('E',"Entry out of range");
	  return RTT__HOLDCOMMAND;
	} 

	entry_ptr = rtt_loggtable + entry - 1;
	if ( !entry_ptr->occupied )	
	{
	  rtt_message('E',"Entry is not created");
	  return RTT__HOLDCOMMAND;
	}   

	if ( entry_ptr->active )	
	{
	  rtt_message('E',"Entry is already started");
	  return RTT__HOLDCOMMAND;
	} 

	/* Get the parameters */
	found = 0;
	for ( i = 0; i < RTT_LOGG_MAXPAR; i++)
	{
	  if ( entry_ptr->parameterstr[i][0] != 0)
	  {
	    found = 1;

	    sts = gdh_RefObjectInfo(
		entry_ptr->parameterstr[i],	
		(pwr_tAddress *) &entry_ptr->parameter_ptr[i],
		&(entry_ptr->parameter_subid[i]),
		entry_ptr->parameter_size[i]);
	    if ( EVEN(sts)) 
	    {
	      rtt_message('E', "Parameter not found");
	      return RTT__HOLDCOMMAND;
	    }
	  }
	}
	if ( !found)
	{
	  rtt_message('E', "Parameter is missing");
	  return RTT__HOLDCOMMAND;
	}

	/* Get the condition */
	if ( entry_ptr->conditionstr[0] != 0)
	{
	  sts = gdh_RefObjectInfo(
		entry_ptr->conditionstr,	
		(pwr_tAddress *) &entry_ptr->condition_ptr,
		&(entry_ptr->condition_subid), 1);
	  if ( EVEN(sts)) 
	  {
	    rtt_message('E', "Condition parameter not found");
	    return RTT__HOLDCOMMAND;
	  }
	}
	else
	{
	  entry_ptr->condition_ptr = 0;
	}
	
	/* Open the file */
	if ( entry_ptr->logg_filename[0] != 0)
	{
	  entry_ptr->logg_file = fopen( entry_ptr->logg_filename, "w");
	  if ( entry_ptr->logg_file == 0)
	  {
	    rtt_message('E', "Unable to open file");
	    return RTT__HOLDCOMMAND;
	  }
	}
	else
	{
	  rtt_message('E', "File is missing");	
	  return RTT__HOLDCOMMAND;
	}

	/* Check time */
	if ( entry_ptr->logg_time == 0)
	{
	  rtt_message('E', "Time is missing");
	  return RTT__HOLDCOMMAND;
	}

	/* Clear buffer */
	entry_ptr->buffer_count = 0;
	*(entry_ptr->buffer_ptr) = 0;
	
	entry_ptr->active = 1;
	entry_ptr->stop = 0;

	/* Create a subprocess */
#if defined(OS_ELN)
	ker$create_process( &sts, &(entry_ptr->process_id), 
		&rtt_logging_logproc, &psts, entry_ptr);
        if ( EVEN(sts)) return sts;
#elif defined(OS_VMS) || defined(OS_LINUX) || defined(OS_LYNX) && !defined(PWR_LYNX_30)
	sts = pthread_create (
		&entry_ptr->thread,
		NULL,				/* attr */
		rtt_logging_logproc,		/* start_routine */
		entry_ptr);			/* arg */
        if ( sts != 0) return sts;
#elif defined(OS_LYNX)
	sts = pthread_create (
		&entry_ptr->thread,
		pthread_attr_default,		/* attr */
		rtt_logging_logproc,		/* start_routine */
		entry_ptr);			/* arg */
        if ( sts != 0) return sts;
#endif

	strcpy( message, "Logg start ");
	rtt_fgetname( entry_ptr->logg_file, message + strlen(message),
		entry_ptr->logg_filename);
	rtt_message('I', message);
	return RTT__NOPICTURE;
}
/*************************************************************************
*
* Name:		rtt_logging_stop()
*
* Type		int
*
* Type		Parameter	IOGF	Description
*
* Description:
*	Stop the entry.
*
**************************************************************************/

int	rtt_logging_stop(
			menu_ctx	ctx,
			int		entry)
{
	rtt_t_loggtable	*entry_ptr;
	
	/* Check the entry */
	if ( ( entry < 1) || ( entry > RTT_LOGG_MAXENTRY))
	{
	  rtt_message('E',"Entry out of range");
	  return RTT__HOLDCOMMAND;
	} 

	entry_ptr = rtt_loggtable + entry - 1;
	if ( !entry_ptr->occupied )	
	{
	  rtt_message('E',"Entry is not created");
	  return RTT__HOLDCOMMAND;
	}   

	if ( !entry_ptr->active )	
	{
	  rtt_message('E',"Entry is already stopped");
	  return RTT__HOLDCOMMAND;
	} 

	rtt_logging_entry_stop( entry_ptr);

	rtt_message('I', "Logging stopped");
	return RTT__NOPICTURE;
}

/*************************************************************************
*
* Name:		rtt_logging_entry_stop()
*
* Type		int
*
* Type		Parameter	IOGF	Description
*
* Description:
*	Stop the entry.
*
**************************************************************************/

static int	rtt_logging_entry_stop(
			rtt_t_loggtable	*entry_ptr)
{
	int		i;
	int		sts;
	
	/* This will stop the subprocess */
	entry_ptr->active = 0;
	entry_ptr->stop = 1;

	/* Unref from gdh */
	for ( i = 0; i < RTT_LOGG_MAXPAR; i++)
	{
	  if ( entry_ptr->parameterstr[i][0] != 0)
	  {
	    sts = gdh_UnrefObjectInfo ( entry_ptr->parameter_subid[i]);
	  }
	}
	if ( entry_ptr->condition_ptr != 0)
	  sts = gdh_UnrefObjectInfo ( entry_ptr->condition_subid);

	return RTT__SUCCESS;
}

/*************************************************************************
*
* Name:		rtt_logging_delete()
*
* Type		int
*
* Type		Parameter	IOGF	Description
*
* Description:
*	Delete the entry.
*
**************************************************************************/

int	rtt_logging_delete(
			menu_ctx	ctx,
			int		entry,
			char		*parameterstr)
{
	int		i;
	int		found;
	rtt_t_loggtable	*entry_ptr;
	
	/* Check the entry */
	if ( ( entry < 1) || ( entry > RTT_LOGG_MAXENTRY))
	{
	  rtt_message('E',"Entry out of range");
	  return RTT__HOLDCOMMAND;
	} 

	entry_ptr = rtt_loggtable + entry - 1;
	if ( !entry_ptr->occupied )	
	{
	  rtt_message('E',"Entry is not created");
	  return RTT__HOLDCOMMAND;
	}   

	if ( parameterstr == NULL)
	{
	  /* Delete the entire entry */
	  /* Free the buffer */
	  free ( entry_ptr->buffer_ptr);

	  memset( entry_ptr, 0, sizeof( *entry_ptr));
	  rtt_message('E', "Entry deleted");
	}
	else
	{
	  /* Remove this parameter only */
	  found = 0;
	  for ( i = 0; i < RTT_LOGG_MAXPAR; i++)
	  {
	    if ( strcmp( entry_ptr->parameterstr[i], parameterstr) == 0)
	    {
	      /* Parmeter is found, remove it */
	      entry_ptr->parameterstr[i][0] = 0;
	      rtt_message('I', "Parameter removed");
	      found = 1;
	      break;
	    }
	  }
	  if ( !found)
	  {
	    rtt_message('E',"Parameter not found");
	    return RTT__HOLDCOMMAND;
	  }
	}

	return RTT__NOPICTURE;
}

/*************************************************************************
*
* Name:		rtt_logging_logproc()
*
* Type		int
*
* Type		Parameter	IOGF	Description
*
* Description:
*	Logging subprocess.
*
**************************************************************************/

#if defined(OS_ELN)
pwr_tStatus	rtt_logging_logproc(
	rtt_t_loggtable	*entry_ptr)
#else
void	*rtt_logging_logproc( void *arg)
#endif
{
	int		sts;
	int		i, j, k;
	int		char_cnt;
	pwr_tTime	time;
	pwr_tDeltaTime	timediff;
	char		time_str[80];
	float		time_float;
	char		*value_ptr;
	char		*old_value_ptr;
	int		first_scan;
	pwr_tObjid		objid;
	pwr_sAttrRef		*attrref;
	pwr_tAName     		hiername;
	char			timstr[64];
	char			parname[40];
	char			*s;
	pwr_tTime	nextime;
	pwr_tTime	restime;
	pwr_tDeltaTime	deltatime;
	pwr_tDeltaTime	wait_time;
#ifdef OS_ELN
 	pwr_tVaxTime 	vmstime;
	TIME_RECORD	time_rec;
#endif
#ifdef OS_VMS
 	pwr_tVaxTime 	vmstime;
#endif
#if defined(OS_VMS) || defined(OS_LYNX) || defined(OS_LINUX)
	rtt_t_loggtable	*entry_ptr;

	entry_ptr = (rtt_t_loggtable *) arg;
#endif
	

	char_cnt = 0;
	first_scan = 1;

        time_GetTime( &nextime);

	entry_ptr->starttime = nextime;

	/* Print starttime and logged parameters on the file */
 	time_AtoAscii( &entry_ptr->starttime, time_eFormat_DateAndTime, 
			time_str, sizeof(time_str));

	switch ( entry_ptr->logg_type)
	{
	  case RTT_LOGG_CONT:
	    if ( entry_ptr->logg_file)
	      fprintf( entry_ptr->logg_file, "\"\"");
	    /* Find a unic shortname for each parameter */
	    for ( i = 0; i < RTT_LOGG_MAXPAR; i++)
	    {
	      if ( entry_ptr->print_shortname)
	      {
	        if ( entry_ptr->parameterstr[i][0] != 0)
	        {
		  /* Print only last segment and not ActualValue */
	          rtt_cut_segments( entry_ptr->shortname[i], entry_ptr->parameterstr[i], 1);
	          if ( (s = strchr( entry_ptr->shortname[i], '.')) != 0)
                  {
	            rtt_toupper( parname, s+1);
	            if ( strcmp( parname, "ACTUALVALUE") == 0)
                      *s = 0;
                  }
	          /* Check that this name is unic */
	          for ( j = 0; j < RTT_LOGG_MAXPAR; j++)
	          {
	            if ( j != i &&
	                 !strcmp( entry_ptr->shortname[i], entry_ptr->shortname[j]))
	            {
	              for ( k = 2; k < 7; k++)
	              {                  
	                /* Increase number of segments */
	                rtt_cut_segments( entry_ptr->shortname[i], entry_ptr->parameterstr[i], k);
	                if ( (s = strchr( entry_ptr->shortname[i], '.')) != 0)
                        {
	                  rtt_toupper( parname, s+1);
	                  if ( strcmp( parname, "ACTUALVALUE") == 0)
                            *s = 0;
                        }
	                rtt_cut_segments( entry_ptr->shortname[j], entry_ptr->parameterstr[j], k);
	                if ( (s = strchr( entry_ptr->shortname[j], '.')) != 0)
                        {
	                  rtt_toupper( parname, s+1);
	                  if ( strcmp( parname, "ACTUALVALUE") == 0)
                            *s = 0;
                        }
	                if ( strcmp( entry_ptr->shortname[i], entry_ptr->shortname[j]))
	                  break;
	              }
	            }
	          }
	        }
	      }
	      else
	        strcpy( entry_ptr->shortname[i], entry_ptr->parameterstr[i]);
	    }
	    for ( i = 0; i < RTT_LOGG_MAXPAR; i++)
	    {
	      if ( entry_ptr->parameterstr[i][0] != 0)
	      {
	        if ( entry_ptr->logg_file)
	        {
	          char_cnt += fprintf( entry_ptr->logg_file, "	%s",
			entry_ptr->shortname[i]);
	          if (char_cnt + 120 > entry_ptr->line_size)
	          {
	            fprintf( entry_ptr->logg_file, "\n");
	            char_cnt = 0;    
	          }
	        }
	      }
	    }
	    if ( entry_ptr->logg_file)
	    {
	      fprintf( entry_ptr->logg_file, "\n");
	      char_cnt = 0;    
	    }
	    break;

	  case RTT_LOGG_MOD:
	    if ( entry_ptr->logg_file)
	      fprintf( entry_ptr->logg_file, "RTT LOGGING STARTED AT %s\n", 
		time_str);
	    for ( i = 0; i < RTT_LOGG_MAXPAR; i++)
	    {
	      if ( entry_ptr->parameterstr[i][0] != 0)
	      {
	        if ( entry_ptr->logg_file)
	          fprintf( entry_ptr->logg_file, "Parameter: %s\n",
		entry_ptr->parameterstr[i]);
	      }
	    }
	    break;
	}

	time_MsToD( &deltatime, entry_ptr->logg_time);

	if ( entry_ptr->logg_priority != 0)
	{
	  sts = rtt_set_prio( entry_ptr->logg_priority);
	}

	for (;;)
	{

	  /* Calculation of starttime for next loop */
	  time_Aadd( &restime, &nextime, &deltatime);
	  nextime = restime;

	  if ( entry_ptr->condition_ptr != 0)
	  {
	    if ( entry_ptr->active && !entry_ptr->stop)
	    {
	      if ( !*(entry_ptr->condition_ptr))
	      {
	        /*  Don't log, wait until next scan */
#ifdef OS_ELN
	        time_PwrToVms( &nextime, &vmstime);
	        ker$wait_any( NULL, NULL, &vmstime);
#endif
#ifdef OS_VMS
	        time_PwrToVms( &nextime, &vmstime);
	        sys$clref( entry_ptr->event_flag);
	        sys$setimr( entry_ptr->event_flag, &vmstime, 0, 0, 0);
	        sys$waitfr( entry_ptr->event_flag);
#endif
#if defined OS_LYNX || defined OS_LINUX
	        time_GetTime( &time);
	        time_Adiff( &wait_time, &nextime, &time);
                nanosleep( (struct timespec *) &wait_time, NULL);
#endif
	        continue;
	      }
	    }
	  }

	  time_GetTime( &time);
	  switch ( entry_ptr->logg_type)
	  {
	    case RTT_LOGG_CONT:
	      /* Convert time to seconds since start */
	      time_Adiff( &timediff, &time, &entry_ptr->starttime);
	      time_float = time_DToFloat( NULL, &timediff);
	
	      /* fix */
	      if ( first_scan)
	        time_float = 0.;
	      /* Print time and the value of the parameter on the file */
	      char_cnt += log_print( entry_ptr, "%11.3f", time_float);
	      for ( i = 0; i < RTT_LOGG_MAXPAR; i++)
	      {
	        if ( entry_ptr->parameterstr[i][0] != 0)
	        {
	          value_ptr = entry_ptr->parameter_ptr[i];
	          switch ( entry_ptr->parameter_type[i])
	          {
	            case pwr_eType_Float32:
	              char_cnt += log_print( entry_ptr, "	%f", *(pwr_tFloat32 *)value_ptr);
	              break;

	            case pwr_eType_Float64:
	              char_cnt += log_print( entry_ptr, "	%f", *(pwr_tFloat64 *)value_ptr);
	              break;

	            case pwr_eType_UInt8:
	              char_cnt += log_print( entry_ptr, "	%d", 
					*(pwr_tUInt8 *)value_ptr);
	              break;
	            case pwr_eType_Boolean:
	              char_cnt += log_print( entry_ptr, "	%d", 
					*(pwr_tBoolean *)value_ptr);
	              break;
	            case pwr_eType_Char:
	              char_cnt += log_print( entry_ptr, "	%c", *(pwr_tChar *)value_ptr);
	              break;
	            case pwr_eType_Int8:
	              char_cnt += log_print( entry_ptr, "	%d", *(pwr_tInt8 *)value_ptr);
	              break;
	            case pwr_eType_Int16:
	              char_cnt += log_print( entry_ptr, "	%d", *(pwr_tInt16 *)value_ptr);
	              break;
	            case pwr_eType_UInt16:
	              char_cnt += log_print( entry_ptr, "	%d", *(pwr_tUInt16 *)value_ptr);
	              break;
	            case pwr_eType_Int32:
	              char_cnt += log_print( entry_ptr, "	%d", *(pwr_tInt32 *)value_ptr);
	              break;
	            case pwr_eType_UInt32:
	              char_cnt += log_print( entry_ptr, "	%d", *(pwr_tUInt32 *)value_ptr);
	              break;
	            case pwr_eType_Int64:
	              char_cnt += log_print( entry_ptr, "	%lld", *(pwr_tInt64 *)value_ptr);
	              break;
	            case pwr_eType_UInt64:
	              char_cnt += log_print( entry_ptr, "	%llu", *(pwr_tUInt64 *)value_ptr);
	              break;
	            case pwr_eType_Objid:
	              objid = *(pwr_tObjid *)value_ptr;
	              if ( !objid.oix)
	                sts = gdh_ObjidToName ( objid, hiername, sizeof(hiername), 
				 cdh_mName_volumeStrict);
	              else
	                sts = gdh_ObjidToName ( objid, hiername, sizeof(hiername), 
				cdh_mNName);
	              if (EVEN(sts)) strcpy( hiername, "** Unknown objid");
	              char_cnt += log_print( entry_ptr, "	%s", hiername);
	              break;
	            case pwr_eType_AttrRef:
	              attrref = (pwr_sAttrRef *) value_ptr;
	              sts = gdh_AttrrefToName ( attrref, hiername, sizeof(hiername), cdh_mNName);
	              if ( EVEN(sts)) strcpy( hiername, "** Unknown attrref");
	              char_cnt += log_print( entry_ptr, "	%s", hiername);
	              break;
	            case pwr_eType_Time:
	              sts = time_AtoAscii( (pwr_tTime *) value_ptr, 
			    time_eFormat_DateAndTime, timstr, sizeof(timstr));
	              if ( EVEN(sts))
	                strcpy( timstr, "Undefined time");
	              char_cnt += log_print( entry_ptr, "	%s", timstr);
	              break;
	            case pwr_eType_DeltaTime:
	              sts = time_DtoAscii( (pwr_tDeltaTime *) value_ptr, 
			    	1, timstr, sizeof(timstr));
	              if ( EVEN(sts))
	                strcpy( timstr, "Undefined time");
	              char_cnt += log_print( entry_ptr, "	%s", timstr);
	              break;
	            default:
	              char_cnt += log_print( entry_ptr, "	%s", "Type error");
	          }
	          if (char_cnt + 10 > entry_ptr->line_size)
	          {
	            log_print( entry_ptr, "\n");
	            char_cnt = 0;    
	          }
	        }
	      }
	      log_print( entry_ptr, "\n");
	      char_cnt = 0;    
	   
	      break;

	    case RTT_LOGG_MOD:
	      /* Write only if value is changed */
	      for ( i = 0; i < RTT_LOGG_MAXPAR; i++)
	      {
	        if ( entry_ptr->parameterstr[i][0] != 0)
	        {
	          value_ptr = entry_ptr->parameter_ptr[i];
	          old_value_ptr = (char *) &entry_ptr->old_value[i];
	          switch ( entry_ptr->parameter_type[i])
	          {
	            case pwr_eType_Float32:
	              if ( (*(pwr_tFloat32 *)value_ptr != 
				*(pwr_tFloat32 *)old_value_ptr) || first_scan)
	              {
	                /* Value is changed, print */
	                time_AtoAscii( &time, time_eFormat_DateAndTime, 
				time_str, sizeof(time_str));
	                log_print( entry_ptr, "%s", &time_str);
	                log_print( entry_ptr, "	%s", 
				&(entry_ptr->parameterstr[i]));
	                log_print( entry_ptr, "	%f\n", *(pwr_tFloat32 *)value_ptr);
	                *(pwr_tFloat32 *)old_value_ptr = *(pwr_tFloat32 *)value_ptr;
	              }
	              break;

	            case pwr_eType_Float64:
	              if ( (*(pwr_tFloat64 *)value_ptr != 
				*(pwr_tFloat64 *)old_value_ptr) || first_scan)
	              {
	                log_print( entry_ptr, "	%s", 
				&(entry_ptr->parameterstr[i]));
	                log_print( entry_ptr, "	%f\n", *(pwr_tFloat64 *)value_ptr);
	                  *(pwr_tFloat64 *)old_value_ptr = *(pwr_tFloat64 *)value_ptr;
	              }
	              break;

	            case pwr_eType_Boolean:
	              if ( (*(pwr_tBoolean *)value_ptr != 
				*(pwr_tBoolean *)old_value_ptr) || first_scan)
	              {
	                /* Value is changed, print */
	                time_AtoAscii( &time, time_eFormat_DateAndTime, 
				time_str, sizeof(time_str));
	                log_print( entry_ptr, "%s", &time_str);
	                log_print( entry_ptr, "	%s",  
				&(entry_ptr->parameterstr[i]));
	                log_print( entry_ptr, "	%d\n", *(pwr_tBoolean *)value_ptr);
	                *(pwr_tBoolean *)old_value_ptr = 
				*(pwr_tBoolean *)value_ptr;
	              }
	              break;
	            case pwr_eType_Char:
	              if ( (*(pwr_tChar *)value_ptr != 
				*(pwr_tChar *)old_value_ptr) || first_scan)
	              {
	                /* Value is changed, print */
	                time_AtoAscii( &time, time_eFormat_DateAndTime, 
				time_str, sizeof(time_str));
	                log_print( entry_ptr, "%s", &time_str);
	                log_print( entry_ptr, "	%s",  
				&(entry_ptr->parameterstr[i]));
	                log_print( entry_ptr, "	%c\n", *(pwr_tChar *)value_ptr);
	                *(pwr_tChar *)old_value_ptr = 
				*(pwr_tChar *)value_ptr;
	              }
	              break;
	            case pwr_eType_UInt8:
	              if ( (*(pwr_tUInt8 *)value_ptr != 
				*(pwr_tUInt8 *)old_value_ptr) || first_scan)
	              {
	                /* Value is changed, print */
	                time_AtoAscii( &time, time_eFormat_DateAndTime, 
				time_str, sizeof(time_str));
	                log_print( entry_ptr, "%s", &time_str);
	                log_print( entry_ptr, "	%s",  
				&(entry_ptr->parameterstr[i]));
	                log_print( entry_ptr, "	%d\n", *(pwr_tUInt8 *)value_ptr);
	                *(pwr_tUInt8 *)old_value_ptr = 
				*(pwr_tUInt8 *)value_ptr;
	              }
	              break;
	            case pwr_eType_Int8:
	              if ( (*(pwr_tInt8 *)value_ptr != 
				*(pwr_tInt8 *) old_value_ptr) || first_scan)
	              {
	                /* Value is changed, print */
	                time_AtoAscii( &time, time_eFormat_DateAndTime, 
				time_str, sizeof(time_str));
	                log_print( entry_ptr, "%s", &time_str);
	                log_print( entry_ptr, "	%s",  
				&(entry_ptr->parameterstr[i]));
	                log_print( entry_ptr, "	%d\n", *value_ptr);
	                *(pwr_tInt8 *)old_value_ptr = *(pwr_tInt8 *)value_ptr;
	              }
	              break;
	            case pwr_eType_UInt16:
	              if ( (*(pwr_tUInt16 *)value_ptr != 
				*(pwr_tUInt16 *)old_value_ptr) || first_scan)
	              {
	                /* Value is changed, print */
	                time_AtoAscii( &time, time_eFormat_DateAndTime, 
				time_str, sizeof(time_str));
	                log_print( entry_ptr, "%s", &time_str);
	                log_print( entry_ptr, "	%s",  
				&(entry_ptr->parameterstr[i]));
	                log_print( entry_ptr, "	%d\n", *(pwr_tUInt16 *)value_ptr);
	                *(pwr_tUInt16 *)old_value_ptr = 
				*(pwr_tUInt16 *)value_ptr;
	              }
	              break;
	            case pwr_eType_Int16:
	              if ( (*(pwr_tInt16 *)value_ptr != 
				*(pwr_tInt16 *)old_value_ptr) || first_scan)
	              {
	                /* Value is changed, print */
	                time_AtoAscii( &time, time_eFormat_DateAndTime, 
				time_str, sizeof(time_str));
	                log_print( entry_ptr, "%s", &time_str);
	                log_print( entry_ptr, "	%s",  
				&(entry_ptr->parameterstr[i]));
	                log_print( entry_ptr, "	%d\n", *(pwr_tInt16 *)value_ptr);
	                *(pwr_tInt16 *)old_value_ptr = *(pwr_tInt16 *)value_ptr;
	              }
	              break;
	            case pwr_eType_UInt32:
	              if ( (*(pwr_tUInt32 *)value_ptr != 
				*(pwr_tUInt32 *)old_value_ptr) || first_scan)
	              {
	                /* Value is changed, print */
	                time_AtoAscii( &time, time_eFormat_DateAndTime, 
				time_str, sizeof(time_str));
	                log_print( entry_ptr, "%s", &time_str);
	                log_print( entry_ptr, "	%s",  
				&(entry_ptr->parameterstr[i]));
	                log_print( entry_ptr, "	%d\n", *(pwr_tUInt32 *)value_ptr);
	                *(pwr_tUInt32 *)old_value_ptr = 
				*(pwr_tUInt32 *)value_ptr;
	              }
	              break;
	            case pwr_eType_Int32:
	              if ( (*(pwr_tInt32 *)value_ptr != 
				*(pwr_tInt32 *)old_value_ptr) || first_scan)
	              {
	                /* Value is changed, print */
	                time_AtoAscii( &time, time_eFormat_DateAndTime, 
				time_str, sizeof(time_str));
	                log_print( entry_ptr, "%s", &time_str);
	                log_print( entry_ptr, "	%s",  
				&(entry_ptr->parameterstr[i]));
	                log_print( entry_ptr, "	%d\n", *(pwr_tInt32 *)value_ptr);
	                *(pwr_tInt32 *)old_value_ptr = 
				*(pwr_tInt32 *)value_ptr;
	              }
	              break;
	            case pwr_eType_UInt64:
	              if ( (*(pwr_tUInt64 *)value_ptr != 
				*(pwr_tUInt64 *)old_value_ptr) || first_scan)
	              {
	                /* Value is changed, print */
	                time_AtoAscii( &time, time_eFormat_DateAndTime, 
				time_str, sizeof(time_str));
	                log_print( entry_ptr, "%s", &time_str);
	                log_print( entry_ptr, "	%s",  
				&(entry_ptr->parameterstr[i]));
	                log_print( entry_ptr, "	%llu\n", *(pwr_tUInt64 *)value_ptr);
	                *(pwr_tUInt64 *)old_value_ptr = 
				*(pwr_tUInt64 *)value_ptr;
	              }
	              break;
	            case pwr_eType_Int64:
	              if ( (*(pwr_tInt64 *)value_ptr != 
				*(pwr_tInt64 *)old_value_ptr) || first_scan)
	              {
	                /* Value is changed, print */
	                time_AtoAscii( &time, time_eFormat_DateAndTime, 
				time_str, sizeof(time_str));
	                log_print( entry_ptr, "%s", &time_str);
	                log_print( entry_ptr, "	%s",  
				&(entry_ptr->parameterstr[i]));
	                log_print( entry_ptr, "	%lld\n", *(pwr_tInt64 *)value_ptr);
	                *(pwr_tInt64 *)old_value_ptr = 
				*(pwr_tInt64 *)value_ptr;
	              }
	              break;
	            case pwr_eType_Objid:
	              if ( memcmp( value_ptr, old_value_ptr, 
				sizeof( pwr_tObjid)) != 0 || first_scan)
	              {
	                /* Value is changed, print */
	                objid = *(pwr_tObjid *)value_ptr;
	                if ( !objid.oix)
	                  sts = gdh_ObjidToName ( objid, hiername, sizeof(hiername), 
				 cdh_mName_volumeStrict);
	                else
	                  sts = gdh_ObjidToName ( objid, hiername, sizeof(hiername), 
				cdh_mNName);
	                if (EVEN(sts)) strcpy( hiername, "** Unknown objid");
	                time_AtoAscii( &time, time_eFormat_DateAndTime, 
				time_str, sizeof(time_str));
	                log_print( entry_ptr, "%s", &time_str);
	                log_print( entry_ptr, "	%s",  
				&(entry_ptr->parameterstr[i]));
	                log_print( entry_ptr, "	%s\n", hiername);
	                memcpy( old_value_ptr, value_ptr, sizeof(pwr_tObjid));
	              }
	              break;
	            case pwr_eType_AttrRef:
	              attrref = (pwr_sAttrRef *) value_ptr;
	              /* There is only space for the objid of the attrref in oldvalue .. */
	              if ( memcmp( (char *) &attrref->Objid, old_value_ptr,
				sizeof( pwr_tObjid)) != 0 || first_scan)
	              {
	                /* At least the objid is changed */
	                sts = gdh_AttrrefToName ( attrref, hiername, sizeof(hiername), cdh_mNName);
	                if (EVEN(sts)) strcpy( hiername, "** Unknown attrref");
	                time_AtoAscii( &time, time_eFormat_DateAndTime, 
				time_str, sizeof(time_str));
	                log_print( entry_ptr, "%s", &time_str);
	                log_print( entry_ptr, "	%s",  
				&(entry_ptr->parameterstr[i]));
	                log_print( entry_ptr, "	%s\n", hiername);
	                memcpy( old_value_ptr, (char *) &attrref->Objid, sizeof(pwr_tObjid));
	              }
	              break;
	            case pwr_eType_Time:
	              if ( memcmp( value_ptr, old_value_ptr, 
				sizeof( pwr_tTime)) != 0 || first_scan)
	              {
	                /* Value is changed, print */
	                sts = time_AtoAscii( (pwr_tTime *) value_ptr, 
		           time_eFormat_DateAndTime, timstr, sizeof(timstr));
	                if ( EVEN(sts))
	                  strcpy( timstr, "Undefined time");

	                time_AtoAscii( &time, time_eFormat_DateAndTime, 
				time_str, sizeof(time_str));
	                log_print( entry_ptr, "%s", &time_str);
	                log_print( entry_ptr, "	%s",
				&(entry_ptr->parameterstr[i]));
	                log_print( entry_ptr, "	%s\n", timstr);
	                memcpy( old_value_ptr, value_ptr, sizeof(pwr_tTime));
	              }
	              break;
	            case pwr_eType_DeltaTime:
	              if ( memcmp( value_ptr, old_value_ptr, 
				sizeof( pwr_tTime)) != 0 || first_scan)
	              {
	                /* Value is changed, print */
	                sts = time_DtoAscii( (pwr_tDeltaTime *) value_ptr, 
		           1, timstr, sizeof(timstr));
	                if ( EVEN(sts))
	                  strcpy( timstr, "Undefined time");

	                time_AtoAscii( &time, time_eFormat_DateAndTime, 
				time_str, sizeof(time_str));
	                log_print( entry_ptr, "%s", &time_str);
	                log_print( entry_ptr, "	%s",
				&(entry_ptr->parameterstr[i]));
	                log_print( entry_ptr, "	%s\n", timstr);
	                memcpy( old_value_ptr, value_ptr, sizeof(pwr_tDeltaTime));
	              }
	              break;
	          }
	        }
	      }
	      
	      break;
	  }
	  first_scan = 0;

	  /*  Wait "cytime" ms */

	  if ( !entry_ptr->active || entry_ptr->stop)
	  {
	    rtt_logging_entry_stop( entry_ptr);
	    log_print_buffer( entry_ptr);
	    if ( entry_ptr->logg_file)
	      fclose( entry_ptr->logg_file);
	    entry_ptr->logg_file = 0;

	    if ( entry_ptr->logg_priority != 0)
	      sts = rtt_set_default_prio();
#ifdef OS_ELN
	    ker$exit(NULL,RTT__SUCCESS);
#endif
#if defined(OS_VMS) || defined(OS_LYNX) || defined(OS_LINUX)
/*	    sts = pthread_detach( &entry_ptr->thread); */
	    pthread_exit( (void *) 1);
#endif
	  }
          time_GetTime( &time);
	  while( time_Acomp( &time, &nextime) > 0)
	  {
	    /* To late for next lap, skip it */
	    time_Aadd( &restime, &nextime, &deltatime);
	    nextime = restime;
	  }

#ifdef OS_VMS
	  time_PwrToVms( &nextime, &vmstime);
	  sys$clref( entry_ptr->event_flag);
	  sys$setimr( entry_ptr->event_flag, &vmstime, 0, 0, 0);
	  sys$waitfr( entry_ptr->event_flag);
#endif
#ifdef OS_ELN
	  time_PwrToVms( &nextime, &vmstime);
	  ker$wait_any( NULL, NULL, &vmstime);
#endif
#if defined OS_LYNX || defined OS_LINUX
	  time_Adiff( &wait_time, &nextime, &time);
          nanosleep( (struct timespec *) &wait_time, NULL);
#endif

	}
#ifdef OS_ELN
	ker$exit(NULL,KER$_BAD_STATE);
#endif
#if defined(OS_VMS) || defined(OS_LYNX) || defined(OS_LINUX)
	pthread_exit(0);
#endif
	return NULL;
}


/*************************************************************************
*
* Name:		rtt_ger_parinfo()
*
* Type		int
*
* Type		Parameter	IOGF	Description
*
* Description:
*	Get parameter info.
*
**************************************************************************/

static int	rtt_get_parinfo(
			char		*parameter_name,
			pwr_sParInfo	*parinfo)
{
	pwr_tOName     	hiername;
	pwr_tOName     	parname;
	pwr_tOName     	name_array[2];
	int		nr;
	int		sts;
	pwr_tObjid	objid;
	pwr_tObjid	parameter;
	pwr_tClassId	class;
	pwr_tOName     	objname;
	char		classname[80];
	char 		*s;

	/* Get object name */
	/* Parse the parameter name into a object and a parameter name */
	nr = rtt_parse( parameter_name, ".", "",
		(char *)name_array, 
		sizeof( name_array)/sizeof( name_array[0]), 
		sizeof( name_array[0]), 0);
	if ( nr < 2)
	  return RTT__OBJNOTFOUND;

	strcpy( objname, name_array[0]);
	strcpy( parname, name_array[1]);

	/* Get objid */
	sts = gdh_NameToObjid ( objname, &objid);
	if ( EVEN(sts)) return sts;

	/* Get class name */	
	sts = rtt_objidtoclassname( objid, classname);
	if ( EVEN(sts)) return sts;

	/* Get objid of rtbody */
	sts = gdh_GetObjectClass ( objid, &class);
	if ( EVEN(sts)) return sts;
	sts = gdh_ObjidToName ( cdh_ClassIdToObjid(class), hiername, 
			sizeof(hiername), cdh_mName_volumeStrict);
	if ( EVEN(sts)) return sts;
	strcat( hiername, "-RtBody");
	sts = gdh_NameToObjid ( hiername, &parameter);
	if ( EVEN(sts)) 
	{
	  /* Try with sysbody */
	  sts = gdh_ObjidToName ( cdh_ClassIdToObjid(class), hiername,
                        sizeof(hiername), cdh_mName_volumeStrict);
	  if ( EVEN(sts)) return sts;
	  strcat( hiername, "-SysBody");
	  sts = gdh_NameToObjid ( hiername, &parameter);
	  if ( EVEN(sts)) 
	  {
	    rtt_message('E', "Unable to open object");	  
	    return RTT__NOPICTURE;
	  }
	}

	if ( (s = strrchr( parname, '[')))
	  *s = 0;

	strcat( hiername, "-");
	strcat( hiername, parname);

	sts = gdh_GetObjectInfo ( hiername, parinfo, sizeof( *parinfo)); 
	if (EVEN(sts)) return sts;

	return RTT__SUCCESS;
}

/*************************************************************************
*
* Name:		log_print()
*
* Type		int
*
* Type		Parameter	IOGF	Description
*
* Description:
*	Print. Equivalent to fprintf but the character string is put
*	in a buffer and printed when the buffer size is exceeded or 
*	when r_print_buffer is called.
*	The max size of the character string is 200.
*
**************************************************************************/

static int	log_print( 
		rtt_t_loggtable	*entry_ptr,
		char		*format,
		... )
{
	char	buff[200];
	int	sts;
	char	*s;
	va_list ap;

	va_start( ap, format);
	sts = vsprintf( buff, format, ap);
	va_end( ap);

	s = entry_ptr->buffer_ptr + entry_ptr->buffer_count;
	strcpy( s, buff);
	entry_ptr->buffer_count += strlen( buff);

	if ( entry_ptr->buffer_count > ( entry_ptr->buffer_size * 512 -
		(int)sizeof( buff)))
	{
	  if ( entry_ptr->intern)
	  {
	    entry_ptr->stop = 1;
	    return 1;
	  }
	  log_print_buffer( entry_ptr);
	}
	return sts;
}

/*************************************************************************
*
* Name:		log_print_buffer()
*
* Type		int
*
* Type		Parameter	IOGF	Description
*
* Description:
*
**************************************************************************/

static int	log_print_buffer(
			rtt_t_loggtable	*entry_ptr)
{

	if ( entry_ptr->logg_priority != 0)
	  rtt_set_default_prio();
/*
	fputs( entry_ptr->buffer_ptr, entry_ptr->logg_file);
*/
	if ( entry_ptr->logg_file)
	  fwrite( entry_ptr->buffer_ptr, 1, entry_ptr->buffer_count, 
		entry_ptr->logg_file);

	if ( entry_ptr->logg_priority != 0)
	  rtt_set_prio( entry_ptr->logg_priority);

	entry_ptr->buffer_count = 0;
	*(entry_ptr->buffer_ptr) = 0;

	return RTT__SUCCESS;
}


/*************************************************************************
*
* Name:		rtt_logging_close_files()
*
* Type		int
*
* Type		Parameter	IOGF	Description
*
* Description:
*	Close all open files.
*	This file is called at execute termintation to close the files.
*
**************************************************************************/
int	rtt_logging_close_files()
{
	int		i;
	rtt_t_loggtable	*entry_ptr;

	entry_ptr = rtt_loggtable;
	for ( i = 0; i < RTT_LOGG_MAXENTRY; i++)
	{
	  if ( entry_ptr->occupied && entry_ptr->active)
	  {
	    if ( entry_ptr->logg_file)
	    {
	      log_print_buffer( entry_ptr);
	      fclose( entry_ptr->logg_file);
	    }
	  }
	  entry_ptr++;
	}
	return RTT__SUCCESS;
}
