/** 
 * Proview   $Id: co_nav_crr.cpp,v 1.3 2005-10-21 16:11:22 claes Exp $
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

/* co_nav_crr.cpp 
   This module contains routines for displaying crossreferences. */

/*_Include files_________________________________________________________*/

#include "flow_std.h"

#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

extern "C" {
#include "co_cdh.h"
#include "co_ccm.h"
#include "co_time.h"
#include "co_dcli.h"
#include "co_ccm_msg.h"
}
#include "co_nav_crr.h"
#include "co_dcli_msg.h"
#include "co_nav_msg.h"

/* Nice functions */
#define ODD(a)	(((int)(a) & 1) != 0)
#define EVEN(a)	(((int)(a) & 1) == 0)
#define max(Dragon,Eagle) ((Dragon) > (Eagle) ? (Dragon) : (Eagle))
#define min(Dragon,Eagle) ((Dragon) < (Eagle) ? (Dragon) : (Eagle))
#ifndef __ALPHA
#define abs(Dragon) ((Dragon) >= 0 ? (Dragon) : (-(Dragon)))
#endif

/*** Local funktions ***************************************************/

static int	nav_get_signal_line(
			FILE	*file,
			char	*line,
			int	size,
			int	*spaces,
			char	*text,
			int	*lines);
static int	nav_remove_spaces(
			char	*in,
			char	*out);
static char	*nav_VolumeIdToStr( pwr_tVolumeId volumeid);


/*************************************************************************
*
* Name:		nav_get_signal_line()
*
* Type		int
*
* Type		Parameter	IOGF	Description
*
* Description:
*	Reads a line i a proview signal or plcmodule list.
*	Returns the read line (line), number of spaces to the first charachter,
*	the number of lines read( empty lines are not returned), 
*	and the first word (text).
*	Returns NAV__EOF if end of file.
*
**************************************************************************/

static int	nav_get_signal_line(
			FILE	*file,
			char	*line,
			int	size,
			int	*spaces,
			char	*text,
			int	*lines)
{
	char    *s;
	char	*f;

	*lines = 0;
	while( 1)
	{
	  if ( dcli_read_line( line, size, file) == 0)
	    return NAV__EOF;
	  (*lines)++;
	
	  /* Get number of spaces before text */
	  *spaces = 0;
	  for ( s = line; !((*s == 0) || ((*s != ' ') && (*s != 9))); s++)
	  {
	    (*spaces)++;
	    if ( *s == 9)
	      (*spaces) += 7;
	  }
	  strcpy( text, s);

	  /* Get end of text */
	  for ( f = text; !((*f == 0) || (*f == ' ') || (*f == 9)); f++);
	  *f = 0;

	  /* Check the text, if no text read next line */
	  if ( text[0] == 0) continue;
	  if ( text[0] == '-')
	  {
	    if ( *spaces > 30)
	    {
	      /* Next line is a header */
	      if ( dcli_read_line( line, size, file) == 0)
	        return NAV__EOF;
	      (*lines)++;
	    }
	    continue;
	  }
	  break;
	}
	return NAV__SUCCESS;
}

/*************************************************************************
*
* Name:		nav_remove_spaces()
*
* Type		int
*
* Type		Parameter	IOGF	Description
*
* Description:
*	Removes spaces and tabs at the begining and at the end of a string.
*
**************************************************************************/

static int	nav_remove_spaces(
			char	*in,
			char	*out)
{
	char    *s;

	for ( s = in; !((*s == 0) || ((*s != ' ') && (*s != 9))); s++);

	strcpy( out, s);
        
        if ( strlen(s) != 0)
        {
	  for ( s += strlen(s) - 1; 
                !((s == in) || ((*s != ' ') && (*s != 9))); s--) ;
	  s++;
	  *s = 0;
        }

	return NAV__SUCCESS;
}


/*************************************************************************
*
* Name:		nav_crr_signal()
*
* Type		int
*
* Type		Parameter	IOGF	Description
*
* Description:
*		Prints the cross references to a signal.
*
**************************************************************************/

int	NavCrr::crr_signal(
			char	*filename,
			char	*signalname)
{
	pwr_tFileName default_filename;
	FILE	*file;
	char	line[1000];
	int	hierarchy_spaces;
	pwr_tAName hierarchy;
	int	object_spaces;
	pwr_tAName object;
	pwr_tAName objname;
	pwr_tAName show_objname;
	int	spaces;
	int	first;
	int	sts;	
	char	*s;
	int	wildcard;
	int	signalcount = 0;
	int	crossref_count = 0;
	pwr_tFileName	filestr;
	int	lines;
	pwr_tVolumeId	volid;
	pwr_tObjid	objid;
	pwr_tAName	line_part[8];
	int	nr;
	int	write;

	cdh_ToUpper( signalname, signalname);

	/* Check if wildcard */
	s = strchr( signalname, '*');
	if ( s == 0)
	  wildcard = 0;
	else
	  wildcard = 1;

	/* Open file */
	if ( filename == NULL)
	{
	  /* Open file, first get the volume id */
	  if ( !wildcard)
	  {
            sts = (name_to_objid_cb)( parent_ctx, signalname, &objid);
	    if ( EVEN(sts))
	      return NAV__OBJECTNOTFOUND;
	    volid = objid.vid;
	  }
	  else
	  {
            sts = (get_volume_cb)( parent_ctx, &volid);
            if ( EVEN(sts)) return sts;
	  }

	  sprintf( default_filename, "%srtt_crr_%s.dat", 
		dcli_pwr_dir("pwrp_load"), nav_VolumeIdToStr( volid));
	  dcli_get_defaultfilename( default_filename, filestr, NULL);
	  file = fopen( filestr, "r");
	}
	else
	{
	  dcli_get_defaultfilename( filename, filestr, ".lis");
	  file = fopen( filestr, "r");
	}
	  
	if ( file == 0)
	  return NAV__NOFILE;
	
	/* First line is a header, skip it */
	sts = nav_get_signal_line( file, line, sizeof( line), 
				&hierarchy_spaces, hierarchy, &lines);
	if ( EVEN(sts)) goto finish;

	/* Get the hierarchy */
	sts = nav_get_signal_line( file, line, sizeof( line), 
				&spaces, object, &lines);
	if ( EVEN(sts)) goto finish;
	hierarchy_spaces = spaces;


	first = 1;
	while ( 1)
	{
	  while ( spaces != hierarchy_spaces)
	  {
	    sts = nav_get_signal_line( file, line, sizeof( line),
				&spaces, object, &lines);
	    if ( EVEN(sts)) goto finish;
	  }
	  strcpy( hierarchy, object);

	  /* Next line is an object */
	  sts = nav_get_signal_line( file, line, sizeof( line), 
				&spaces, object, &lines);
	  if ( EVEN(sts)) goto finish;
	  if ( first)
	  {
	    object_spaces = spaces;
	    first = 0;
	  }

	  while ( spaces == object_spaces)
	  {
	    /* Put object and hierarchy together and check if this is 
			the object */
	    strcpy( objname, hierarchy);
	    strcat( objname, "-");
	    strcat( objname, object);
	    strcpy( show_objname, objname);
	    cdh_ToUpper( objname, objname);

	    sts = dcli_wildcard( signalname, objname);
	    if ( !sts )
	    {
	      /* Hit, print this object */
	      signalcount++;

	      sts = nav_get_signal_line( file, line, sizeof( line),
				&spaces, object, &lines);
	      if ( EVEN(sts)) goto finish;
	      while( spaces > object_spaces)
	      {
	        nav_remove_spaces( line, line);

	        if ( line[0] == '#')
	        {
	          write = 1;
	          strcpy( line, &line[2]);
	        }
	        else
	          write = 0;

	        nr = dcli_parse( line, " 	", "", (char *)line_part,
                	sizeof( line_part) / sizeof( line_part[0]), 
			sizeof( line_part[0]), 0);
                if ( strcmp( line_part[nr-1], "") == 0 && nr > 2)
                  nr--;
                (insert_cb)( parent_ctx, parent_node,
			     navc_eItemType_Crossref, 
			     line_part[nr-2], line_part[nr-1],
			     write);
	        crossref_count++;
	        sts = nav_get_signal_line( file, line, sizeof( line),
				&spaces, object, &lines);
	        if ( EVEN(sts)) goto finish;
	      }
	      if ( !wildcard)
	        goto finish;
	    }
	    else
	    {
	      sts = nav_get_signal_line( file, line, sizeof( line),
				&spaces, object, &lines);
	      if ( EVEN(sts)) goto finish;
	      while( spaces > object_spaces)
	      {
	        sts = nav_get_signal_line( file, line, sizeof( line),
				&spaces, object, &lines);
	        if ( EVEN(sts)) goto finish;
	      }
	    }
	  }
	}

finish:
	fclose( file);

	if ( signalcount == 0)
	  return NAV__OBJECTNOTFOUND;

	if ( crossref_count == 0)
	  return NAV__NOCROSSREF;

	return NAV__SUCCESS;

}

/*************************************************************************
*
* Name:		nav_crr_object()
*
* Type		int
*
* Type		Parameter	IOGF	Description
*
* Description:
*		Prints the cross references to a object that is not a signal.
*
**************************************************************************/

int	NavCrr::crr_object(
			char	*filename,
			char	*objectname)
{
	pwr_tFileName default_filename;
	FILE	*file;
	char	line[1000];
	int	hierarchy_spaces;
	pwr_tAName hierarchy;
	int	object_spaces;
	pwr_tAName object;
	pwr_tAName objname;
	pwr_tAName show_objname;
	int	spaces;
	int	first;
	int	sts;	
	char	*s;
	int	wildcard;
	int	signalcount = 0;
	pwr_tFileName filestr;
	int	lines;
	pwr_tVolumeId	volid;
	pwr_tObjid	objid;
	int	crossref_count = 0;
	pwr_tAName line_part[8];
	int	nr;
	int	write;

	cdh_ToUpper( objectname, objectname);

	/* Check if wildcard */
	s = strchr( objectname, '*');
	if ( s == 0)
	  wildcard = 0;
	else
	  wildcard = 1;

	/* Open file */
	if ( filename == NULL)
	{
	  /* Open file, first get the volume id */
	  if ( !wildcard)
	  {
            sts = (name_to_objid_cb)( parent_ctx, objectname, &objid);
	    if ( EVEN(sts))
	      return NAV__OBJECTNOTFOUND;
	    volid = objid.vid;
	  }
	  else
	  {
            sts = (get_volume_cb)( parent_ctx, &volid);
            if ( EVEN(sts)) return sts;
	  }

	  sprintf( default_filename, "%srtt_crro_%s.dat", 
		dcli_pwr_dir("pwrp_load"), nav_VolumeIdToStr( volid));
	  dcli_get_defaultfilename( default_filename, filestr, NULL);
	  file = fopen( filestr, "r");
	}
	else
	{
	  dcli_get_defaultfilename( filename, filestr, ".lis");
	  file = fopen( filestr, "r");
	}
	  
	if ( file == 0)
	  return NAV__NOFILE;
	
	/* First line is a header, skip it */
	sts = nav_get_signal_line( file, line, sizeof( line), 
				&hierarchy_spaces, hierarchy, &lines);
	if ( EVEN(sts)) goto finish;
	sts = nav_get_signal_line( file, line, sizeof( line), 
				&hierarchy_spaces, hierarchy, &lines);
	if ( EVEN(sts)) goto finish;
	sts = nav_get_signal_line( file, line, sizeof( line), 
				&hierarchy_spaces, hierarchy, &lines);
	if ( EVEN(sts)) goto finish;
	sts = nav_get_signal_line( file, line, sizeof( line), 
				&hierarchy_spaces, hierarchy, &lines);
	if ( EVEN(sts)) goto finish;

	/* Get the hierarchy */
	sts = nav_get_signal_line( file, line, sizeof( line), 
				&spaces, object, &lines);
	if ( EVEN(sts)) goto finish;
	object_spaces = spaces;


	first = 1;
	while ( 1)
	{
	  while ( spaces != object_spaces)
	  {
	    sts = nav_get_signal_line( file, line, sizeof( line),
				&spaces, object, &lines);
	    if ( EVEN(sts)) goto finish;
	  }
	  strcpy( objname, object);

	  strcpy( show_objname, objname);
	  cdh_ToUpper( objname, objname);

	  sts = dcli_wildcard( objectname, objname);
	  if ( !sts )
	  {
	    /* Hit, print this object */
	    signalcount++;

	    sts = nav_get_signal_line( file, line, sizeof( line),
				&spaces, object, &lines);
	    if ( EVEN(sts)) goto finish;
	    while( spaces > object_spaces)
	    {
	      nav_remove_spaces( line, line);

	      if ( line[0] == '#')
	      {
	        write = 1;
	        strcpy( line, &line[2]);
	      }
	      else if ( line[0] == '&')
	      {
	        write = 2;
	        strcpy( line, &line[2]);
	      }
	      else
	        write = 0;

	      nr = dcli_parse( line, " 	", "", (char *)line_part,
                	sizeof( line_part) / sizeof( line_part[0]), 
			sizeof( line_part[0]), 0);
              if ( strcmp( line_part[nr-1], "") == 0 && nr > 2)
                nr--;
              if ( nr > 2 && line_part[nr-3][0] == '#')
                write = 1;
              else if ( nr > 2 && line_part[nr-3][0] == '&')
                write = 2;
	      (insert_cb)( parent_ctx, parent_node, 
			   navc_eItemType_Crossref,
			   line_part[nr-2], line_part[nr-1],  write);
	      crossref_count++;

	      sts = nav_get_signal_line( file, line, sizeof( line),
				&spaces, object, &lines);
	      if ( EVEN(sts)) goto finish;
	    }
	    if ( !wildcard)
	      goto finish;
	  }
	  else
	  {
	    sts = nav_get_signal_line( file, line, sizeof( line),
				&spaces, object, &lines);
	    if ( EVEN(sts)) goto finish;
	  }
	}

finish:
	fclose( file);

	if ( signalcount == 0)
	  return NAV__OBJECTNOTFOUND;

	if ( crossref_count == 0)
	  return NAV__NOCROSSREF;

	return NAV__SUCCESS;

}

/*************************************************************************
*
* Name:		nav_crr_code()
*
* Type		int
*
* Type		Parameter	IOGF	Description
*
* Description:
*		Prints the cross references to a fuction or string
*		in arithm objects.
*
**************************************************************************/

int	NavCrr::crr_code(
			char	*filename,
			char	*str,
			int	brief,
			int	func,
			int	case_sensitive)
{
	pwr_tFileName default_filename;
	FILE	*file;
	char	line[1000];
	char	tst_line[1000];
	int	hierarchy_spaces;
	pwr_tAName hierarchy;
	int	object_spaces;
	pwr_tAName object;
	pwr_tAName objname;
	int	spaces;
	int	first;
	int	sts;	
	char	*s;
	int	signalcount = 0;
	pwr_tFileName filestr;
	int	lines;
	char	title[80];
	pwr_tVolumeId	volid;
	int	objname_written;
	int	hit;
	char	*tst_char;

	/* Open file */
	if ( filename == NULL)
	{
          sts = (get_volume_cb)( parent_ctx, &volid);
          if ( EVEN(sts)) return sts;

	  sprintf( default_filename, "%srtt_crrc_%s.dat", 
		dcli_pwr_dir("pwrp_load"), nav_VolumeIdToStr( volid));
	  dcli_get_defaultfilename( default_filename, filestr, NULL);
	  file = fopen( filestr, "r");
	}
	else
	{
	  dcli_get_defaultfilename( filename, filestr, ".lis");
	  file = fopen( filestr, "r");
	}
	  
	if ( file == 0)
	  return NAV__NOFILE;
	
	/* Case sensitive if any lowercase */
	if ( !case_sensitive)
	  for ( s = str; *s != 0; s++)
	  {
	    if ( *s != '_' && !isupper(*s))
	      case_sensitive = 1;
	  }

	/* First line is a header, skip it */
	sts = nav_get_signal_line( file, line, sizeof( line), 
				&hierarchy_spaces, hierarchy, &lines);
	if ( EVEN(sts)) goto finish;
	sts = nav_get_signal_line( file, line, sizeof( line), 
				&hierarchy_spaces, hierarchy, &lines);
	if ( EVEN(sts)) goto finish;
	sts = nav_get_signal_line( file, line, sizeof( line), 
				&hierarchy_spaces, hierarchy, &lines);
	if ( EVEN(sts)) goto finish;
	sts = nav_get_signal_line( file, line, sizeof( line), 
				&hierarchy_spaces, hierarchy, &lines);
	if ( EVEN(sts)) goto finish;

	/* Get the hierarchy */
	sts = nav_get_signal_line( file, line, sizeof( line), 
				&spaces, object, &lines);
	if ( EVEN(sts)) goto finish;
	object_spaces = spaces;


	first = 1;
	while ( 1)
	{

	  while ( strncmp( line, " _Obj_ ", 7) != 0)
	  {
	    sts = nav_get_signal_line( file, line, sizeof( line),
				&spaces, object, &lines);
	    if ( EVEN(sts)) goto finish;
	  }
	  strcpy( objname, &line[7]);
	  for ( s = objname; !(*s == 32 || *s == 9 || *s == 0); s++);
	  *s = 0;

	  sts = nav_get_signal_line( file, line, sizeof( line),
				&spaces, object, &lines);
	  objname_written = 0;
	  while ( strncmp( line, " _Obj_ ", 7) != 0)
	  {
	    if ( !case_sensitive)
	      cdh_ToUpper( tst_line, line);
	    else
	      strcpy( tst_line, line);

	    hit = 0;
	    if ( !func)
	    {
	      if ( strstr( tst_line, str) != 0)
	        hit = 1;
	    }
	    else
	    {
	      if ( (s = strstr( tst_line, str)) != 0)
	      {
	        hit = 1;
	        /* Check char after */
	        tst_char = s + strlen(str);
	        if ( isalpha( *tst_char) || isdigit( *tst_char) ||
		     *tst_char == '_')
	          hit = 0;
	        /* Check char before */
	        if ( s != tst_line)
	        {
	          tst_char = s - 1;
	          if ( isalpha( *tst_char) || isdigit( *tst_char) ||
		       *tst_char == '_')
	            hit = 0;
	        }
	      }
	    }
	    if ( hit)
	    {
	      /* Hit, print this object */
	      if ( signalcount == 0)
	      {
	        if ( func)
	        {
	          sprintf( title, "Crossreferens list Function  \"%s\"\n\n", str);
                  (insert_cb)( parent_ctx, parent_node, 
			       navc_eItemType_Header, title, NULL, 0);
	        }
	        else
	        {
	          sprintf( title, "Crossreferens list String    \"%s\"\n\n", str);
                  (insert_cb)( parent_ctx, parent_node, 
			       navc_eItemType_Header, title, NULL, 0);
	        }
	      }
	      signalcount++;

	      if ( !objname_written)
	      {
                (insert_cb)( parent_ctx, parent_node, 
			     navc_eItemType_Crossref, objname, "", 2);
	        objname_written = 1;
	      }

	      strcpy( tst_line, "     ");
	      strcat( tst_line, line);
              (insert_cb)( parent_ctx, parent_node, 
			   navc_eItemType_Text, tst_line, NULL, 0);

	      if ( brief)
	      {
	        while ( strncmp( line, " _Obj_ ", 7) != 0)
	        {	      
	          sts = nav_get_signal_line( file, line, sizeof( line),
				&spaces, object, &lines);
	          if ( EVEN(sts)) goto finish;
	        }
	      }
	      else
	      {
	        sts = nav_get_signal_line( file, line, sizeof( line),
				&spaces, object, &lines);
	        if ( EVEN(sts)) goto finish;
	      }
	    }
	    else
	    {
	      sts = nav_get_signal_line( file, line, sizeof( line),
				&spaces, object, &lines);
	      if ( EVEN(sts)) goto finish;
	    }
	  }
	}

finish:
	fclose( file);

	if ( signalcount > 0)
	{
	}
	else
	{
	  if ( func)
	    return NAV__STRINGNOTFOUND;
	  else
	    return NAV__FUNCTIONNOTFOUND;
	} 
	return NAV__SUCCESS;

}



/*************************************************************************
*
* Name:		nav_VolumeIdToStr()
*
* Type		* char
*
* Type		Parameter	IOGF	Description
*
* Description:
*	Converts an VolumeId to a string.
*	The returned string is static and must be used befor next call
*	of the function.
*
**************************************************************************/

static char	*nav_VolumeIdToStr( pwr_tVolumeId volumeid)
{
	static char	str[80];
	unsigned char	volid[4];

	memcpy( &volid, &volumeid, sizeof(volid));
	sprintf( str, "%3.3u_%3.3u_%3.3u_%3.3u",
		volid[3], volid[2], volid[1], volid[0]);
	return str;
}


