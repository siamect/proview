/** 
 * Proview   $Id: co_nav_help.cpp,v 1.11 2008-02-27 06:24:37 claes Exp $
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

/* xtt_xnav_help.cpp 
   This module contains routines for displaying help in xtt. */

/*_Include files_________________________________________________________*/

#include "flow_std.h"

#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

extern "C" {
#include "co_cdh.h"
#include "co_dcli.h"
}

#include "co_nav_help.h"
#include "co_dcli_msg.h"
#include "co_nav_msg.h"
#include "co_lng.h"

/* Nice functions */
#define ODD(a)	(((int)(a) & 1) != 0)
#define EVEN(a)	(((int)(a) & 1) == 0)
#define max(Dragon,Eagle) ((Dragon) > (Eagle) ? (Dragon) : (Eagle))
#define min(Dragon,Eagle) ((Dragon) < (Eagle) ? (Dragon) : (Eagle))
#ifndef __ALPHA
#define abs(Dragon) ((Dragon) >= 0 ? (Dragon) : (-(Dragon)))
#endif

/*** Local funktions ***************************************************/
static int	help_remove_spaces(
			char	*in,
			char	*out);

/*************************************************************************
*
* Name:		help_remove_spaces()
*
* Type		int
*
* Type		Parameter	IOGF	Description
*
* Description:
*	Removes spaces and tabs at the begining and at the end of a string.
*
**************************************************************************/

static int	help_remove_spaces(
			char	*in,
			char	*out)
{
	char    *s;

	for ( s = in; !((*s == 0) || ((*s != ' ') && (*s != 9))); s++);

	strcpy( out, s);
        
	s = out;
        if ( strlen(s) != 0)
        {
	  for ( s += strlen(s) - 1;
                !((s == out) || ((*s != ' ') && (*s != 9))); s--) ;
	  s++;
	  *s = 0;
        }

	return NAV__SUCCESS;
}


/*************************************************************************
*
* Name:		help()
*
**************************************************************************/

int	NavHelp::help( char *help_key, char *help_bookmark, 
		       navh_eHelpFile file_type, char *file_name, void **book_mark,
		       bool strict)
{
  char	filestr[200];
  FILE	*file;
  char	line[200];
  char	text1[200];
  char	text2[200];
  char	text3[200];
  char	key[80];
  char	key_part[4][40];
  char	include_file[80];
  char	subject[80];
  char	subject_part[4][40];
  int	key_nr;
  int	subject_nr;
  int	hit = 0;
  int 	sts;
  int	i;  
  char	link[200];
  char	link_bookmark[80];
  char	link_filename[80];
  char	*link_filename_p;
  int	link_nr;
  char	link_part[4][200];
  char	bookmark[80];
  char	search_bookmark[80];
  char	*s;
  int	header1;
  int	header2;
  int	bold;
  int	code;
  int	horizontal_line;
  int	style = 0;
  char  style_str[80];
  int   image;
  char  imagefile[80];
  int	register_bookmark = 0;
  int	bookmark_found = 0;
  void  *bookmark_node = 0;
  int   index_link;
  void  *node;
  int   print_all;
  char  *bookmark_p;

  print_all = (help_key == NULL);
  if ( help_key) {
    cdh_ToLower( key, help_key);
    cdh_ToLower( search_bookmark, help_bookmark);
  }

  // Open file
  if ( file_type == navh_eHelpFile_Base)
    dcli_get_defaultfilename( base_file, filestr, NULL);
  else if ( file_type == navh_eHelpFile_Project)
    dcli_get_defaultfilename( project_file, filestr, NULL);
  else if ( file_name)
    dcli_get_defaultfilename( file_name, filestr, NULL);
  else
    dcli_get_defaultfilename( base_file, filestr, NULL);

  // Replace symbol for language
  if ( strncmp( filestr, "$pwr_lang/", 10) == 0) {
    char lng_filestr[200];
    
    sprintf( lng_filestr, "$pwr_exe/%s/%s", Lng::get_language_str(),
	     &filestr[10]);
    strcpy( filestr, lng_filestr);
  }
  dcli_translate_filename( filestr, filestr);

  file = fopen( filestr, "r");
  if ( file == 0)
    return NAV__NOFILE;
	
  if ( !print_all)
    key_nr = dcli_parse( key, " 	", "", (char *)key_part,
			 sizeof( key_part) / sizeof( key_part[0]), 
			 sizeof( key_part[0]), 0);


  sts = dcli_read_line( line, sizeof( line), file);
  while ( ODD(sts)) {
    if ( cdh_NoCaseStrncmp( line, "<include>", 9) == 0) {      
      help_remove_spaces( &line[9], include_file);
      // Replace symbol for language
      if ( strncmp( include_file, "$pwr_lang/", 10) == 0) {
        char lng_include_file[200];

	sprintf( lng_include_file, "$pwr_exe/%s/%s", Lng::get_language_str(),
		 &include_file[10]);
	strcpy( include_file, lng_include_file);
      }

      if ( !noprop) {
	sts = help( help_key, help_bookmark, navh_eHelpFile_Other, 
		    include_file, book_mark, strict);
	if ( ODD(sts) && !print_all) {
	  fclose( file);
	  return sts;
	}
      }
      hit = 0;
    }
    else if ( cdh_NoCaseStrncmp( line, "<chapter>", 9) == 0) {      
      (insert_cb)( parent_ctx, navh_eItemType_Chapter, "",
		   NULL, NULL, NULL, NULL, NULL, navh_eHelpFile_, 0, NULL);
    }
    else if ( cdh_NoCaseStrncmp( line, "</chapter>", 10) == 0) {
      (insert_cb)( parent_ctx, navh_eItemType_EndChapter, "",
		   NULL, NULL, NULL, NULL, NULL, navh_eHelpFile_, 0, NULL);
    }
    else if ( cdh_NoCaseStrncmp( line, "<headerlevel>", 13) == 0) {
      (insert_cb)( parent_ctx, navh_eItemType_HeaderLevel, "",
		   NULL, NULL, NULL, NULL, NULL, navh_eHelpFile_, 0, NULL);
    }
    else if ( cdh_NoCaseStrncmp( line, "</headerlevel>", 14) == 0) {
      (insert_cb)( parent_ctx, navh_eItemType_EndHeaderLevel, "",
		   NULL, NULL, NULL, NULL, NULL, navh_eHelpFile_, 0, NULL);
    }
    else if ( cdh_NoCaseStrncmp( line, "<pagebreak>", 11) == 0) {
      (insert_cb)( parent_ctx, navh_eItemType_PageBreak, "",
		   NULL, NULL, NULL, NULL, NULL, navh_eHelpFile_, 0, NULL);
    }
    else if ( cdh_NoCaseStrncmp( line, "<option>", 8) == 0) {
      char option[80];

      help_remove_spaces( &line[8], option);
      cdh_ToLower( option, option);
      (insert_cb)( parent_ctx, navh_eItemType_Option, option,
		   NULL, NULL, NULL, NULL, NULL, navh_eHelpFile_, 0, NULL);
    }
    else if ( cdh_NoCaseStrncmp( line, "<topic>", 7) == 0) {
      if ( (s = strstr( line, "<style>")) || (s = strstr( line, "<STYLE>"))) {
	style = 1;
	help_remove_spaces( s + 7, style_str);
	*s = 0;
      }

      help_remove_spaces( &line[7], subject);
      cdh_ToLower( subject, subject);
      subject_nr = dcli_parse( subject, " 	", "", (char *)subject_part,
                	sizeof( subject_part) / sizeof( subject_part[0]),
			sizeof( subject_part[0]), 0);
      hit = 0;
      if ( !print_all) {
        if ( key_nr == subject_nr ) {
          for ( i = 0; i < key_nr; i++) {
            if ( (!strict && strncmp( subject_part[i], key_part[i], strlen(key_part[i])) == 0) ||
		 (strict && strcmp( subject_part[i], key_part[i]) == 0)) {
              if ( i == key_nr - 1)
                hit = 1;
            }
            else
              break;
          }
        }
      }
      else {
	if ( cdh_NoCaseStrcmp( subject_part[0], "__documenttitlepage") == 0)
	  (insert_cb)( parent_ctx, navh_eItemType_DocTitlePage, subject,
		       NULL, NULL, NULL, NULL, NULL, navh_eHelpFile_, 0, NULL);
	else if ( cdh_NoCaseStrcmp( subject_part[0], "__documentinfopage") == 0)
	  (insert_cb)( parent_ctx, navh_eItemType_DocInfoPage, subject,
		       NULL, NULL, NULL, NULL, NULL, navh_eHelpFile_, 0, NULL);
	else
	  (insert_cb)( parent_ctx, navh_eItemType_Topic, subject,
		       NULL, NULL, NULL, NULL, NULL, navh_eHelpFile_, 0, NULL);
	if ( style)
	  node = (insert_cb)( parent_ctx, navh_eItemType_Style, style_str,
		      NULL, NULL, NULL, NULL, NULL, navh_eHelpFile_, 0, NULL);
        hit = 1;
      }
    }
    if ( hit)
    {
      sts = dcli_read_line( line, sizeof( line), file);
      (insert_cb)( parent_ctx, navh_eItemType_HelpHeader, line, NULL, NULL, 
		 NULL, NULL, NULL, navh_eHelpFile_, 0, NULL);
      sts = dcli_read_line( line, sizeof( line), file);
      while( ODD(sts))
      {
        bookmark_p = 0;

        if ( strncmp( line, "</topic>", 8) == 0 ||
             strncmp( line, "</TOPIC>", 8) == 0) {
          if ( print_all)
            (insert_cb)( parent_ctx, navh_eItemType_EndTopic, subject,
		      NULL, NULL, NULL, NULL, NULL, navh_eHelpFile_, 0, NULL);
          break;
        }

	// Ignore all other tags
        if ( (s = strstr( line, "<ib>")) || (s = strstr( line, "<IB>"))) {
	  strcpy( text1, s + 4);
          (insert_cb)( parent_ctx, navh_eItemType_HelpBold, 
			    text1,  "", "", "", "", NULL, file_type, 0, NULL);
          sts = dcli_read_line( line, sizeof( line), file);
          continue;
	}
        if ( (s = strstr( line, "<i>")) || (s = strstr( line, "<I>"))) {
	  strcpy( text1, s + 3);
          (insert_cb)( parent_ctx, navh_eItemType_Help, text1,  "", "", "", 
		"", NULL, file_type, 0, bookmark_p);
          sts = dcli_read_line( line, sizeof( line), file);
          continue;
	}
	if ( cdh_NoCaseStrncmp( line, "<option>", 8) == 0) {
	  char option[80];

	  help_remove_spaces( &line[8], option);
	  cdh_ToLower( option, option);
	  (insert_cb)( parent_ctx, navh_eItemType_Option, option,
		      NULL, NULL, NULL, NULL, NULL, navh_eHelpFile_, 0, NULL);
          sts = dcli_read_line( line, sizeof( line), file);
          continue;
	}

        if ( (s = strstr( line, "<link>")) || (s = strstr( line, "<LINK>")))
	{
          help_remove_spaces( s + 6, link);
	  *s = 0;

          link_nr = dcli_parse( link, ",", "", (char *)link_part,
                	sizeof( link_part) / sizeof( link_part[0]),
			sizeof( link_part[0]), 0);
          if ( link_nr == 1)
	  {
	    help_remove_spaces( link_part[0], link);
            strcpy( link_bookmark, "");
            link_filename_p = file_name;
	  }
          else if ( link_nr == 2)
	  {
	    help_remove_spaces( link_part[0], link);
	    help_remove_spaces( link_part[1], link_bookmark);
            link_filename_p = file_name;
          }
          else if ( link_nr > 2)
	  {
	    help_remove_spaces( link_part[0], link);
	    help_remove_spaces( link_part[1], link_bookmark);
	    help_remove_spaces( link_part[2], link_filename);
            link_filename_p = link_filename;
          }
	}
        else if ( (s = strstr( line, "<weblink>")) || (s = strstr( line, "<WEBLINK>")))
	{
          help_remove_spaces( s + 9, link);
	  *s = 0;

          link_nr = dcli_parse( link, ",", "", (char *)link_part,
                	sizeof( link_part) / sizeof( link_part[0]),
			sizeof( link_part[0]), 0);
          if ( link_nr == 1)
	  {
	    strcpy( link, "$web:");
	    help_remove_spaces( link_part[0], &link[5]);
            strcpy( link_bookmark, "");
            link_filename_p = file_name;
	  }
          else if ( link_nr == 2)
	  {
	    strcpy( link, "$web:");
	    help_remove_spaces( link_part[0], &link[5]);
	    help_remove_spaces( link_part[1], link_bookmark);
            link_filename_p = file_name;
          }
          else if ( link_nr > 2)
	  {
	    strcpy( link, "$web:");
	    help_remove_spaces( link_part[0], &link[5]);
	    help_remove_spaces( link_part[1], link_bookmark);
	    help_remove_spaces( link_part[2], link_filename);
            link_filename_p = link_filename;
          }
	}
        else
	{
          strcpy( link, "");
          strcpy( link_bookmark, "");
          link_filename_p = link_filename;
	}

        if ( (s = strstr( line, "<bookmark>")) || (s = strstr( line, "<BOOKMARK>")))
	{
          help_remove_spaces( s + 10, bookmark);
	  *s = 0;
	  cdh_ToLower( bookmark, bookmark);
          if ( !bookmark_found && !print_all &&
	       strcmp( search_bookmark, bookmark) == 0)
	  {
            bookmark_found = 1;
	    register_bookmark = 1;
	  }
          if ( print_all)
	    bookmark_p = bookmark;
	}

        if ( (s = strstr( line, "<index>")) || (s = strstr( line, "<INDEX>")))
        {
	  index_link = 1;
          *s = 0;
	}
        else
          index_link = 0;

	image = 0;
        header1 = 0;
        header2 = 0;
        bold = 0;
	code = 0;
	horizontal_line = 0;
	style = 0;
        if ( (s = strstr( line, "<h1>")) || (s = strstr( line, "<H1>")))
        {
          header1 = 1;
          strcpy( text1, s + 4);
        }
        else if ( (s = strstr( line, "<h2>")) || (s = strstr( line, "<H2>")))
        {
          header2 = 1;
          strcpy( text1, s + 4);
        }
        else if ( (s = strstr( line, "<b>")) || (s = strstr( line, "<B>")))
        {
          bold = 1;
          strcpy( text1, s + 3);
        }
        else if ( (s = strstr( line, "<c>")) || (s = strstr( line, "<C>")))
        {
          code = 1;
          strcpy( text1, s + 3);
        }
        else if ( (s = strstr( line, "<hr>")) || (s = strstr( line, "<HR>")))
        {
          horizontal_line = 1;
        }
        else if ( (s = strstr( line, "<image>")) || (s = strstr( line, "<IMAGE>")))
	{
          help_remove_spaces( s + 7, imagefile);
          image = 1;
	}

        else
        {
          strcpy( text1, line);
        }

        if ( (s = strstr( text1, "<t>")) || (s = strstr( text1, "<T>")) )
	{
          strcpy( text2, s + 3);
	  *s = 0;
          if ( (s = strstr( text2, "<t>")) || (s = strstr( text2, "<T>")) )
	  {
            strcpy( text3, s + 3);
	    *s = 0;
	  }
          else
            strcpy( text3, "");
	}
        else
        {
          strcpy( text2, "");
          strcpy( text3, "");
        }
        if ( header1)
	{
          node = (insert_cb)( parent_ctx, navh_eItemType_HeaderLarge,
			      text1, NULL, NULL, NULL, NULL, NULL,
			      navh_eHelpFile_, 0, bookmark_p);
	  if ( register_bookmark)
	  {
	    bookmark_node = node;
	    register_bookmark = 0;
	  }
	}
        else if ( header2)
	{
          node = (insert_cb)( parent_ctx, navh_eItemType_Header, text1,
			      NULL, NULL, NULL, NULL, NULL, 
			      navh_eHelpFile_, 0, bookmark_p);
	  if ( register_bookmark)
	  {
	    bookmark_node = node;
	    register_bookmark = 0;
	  }
	}
	else if ( bold)
	{
          node = (insert_cb)( parent_ctx, navh_eItemType_HelpBold, text1, 
		text2, text3, link, 
		link_bookmark, link_filename_p, file_type, index_link,
		bookmark_p);
	  if ( register_bookmark)
	  {
	    bookmark_node = node;
	    register_bookmark = 0;
	  }
	}
	else if ( code)
	{
          node = (insert_cb)( parent_ctx, navh_eItemType_HelpCode, text1, 
		text2, text3, link, 
		link_bookmark, link_filename_p, file_type, index_link,
		bookmark_p);
	  if ( register_bookmark)
	  {
	    bookmark_node = node;
	    register_bookmark = 0;
	  }
	}
	else if ( horizontal_line) {
	  node = (insert_cb)( parent_ctx, navh_eItemType_HorizontalLine, NULL,
		      NULL, NULL, NULL, NULL, NULL, navh_eHelpFile_, 0, NULL);
	}
        else if ( image)
	{
          node = (insert_cb)( parent_ctx, navh_eItemType_Image,
			      imagefile, NULL, NULL, link, 
			      link_bookmark, link_filename_p, file_type, index_link,
			      bookmark_p);
	  if ( register_bookmark)
	  {
	    bookmark_node = node;
	    register_bookmark = 0;
	  }
	}
	else
	{
          node = (insert_cb)( parent_ctx, navh_eItemType_Help, text1,  text2,
		text3, link, 
		link_bookmark, link_filename_p, file_type, index_link,
		bookmark_p);
	  if ( register_bookmark)
	  {
	    bookmark_node = node;
	    register_bookmark = 0;
	  }
	}
        sts = dcli_read_line( line, sizeof( line), file);
      }

      if ( bookmark_found)
        *book_mark = bookmark_node;
      else
        *book_mark = 0;
      if ( !print_all)
        break;
      else
        hit = 0;
    }
    sts = dcli_read_line( line, sizeof( line), file);
  }
  fclose( file);

  if ( !print_all && !hit)
    return NAV__TOPICNOTFOUND;
  return NAV__SUCCESS;
}


int	NavHelp::get_next_key( char *help_key, navh_eHelpFile file_type, char *file_name,
			       bool strict, char *next_key)
{
  char	filestr[200];
  FILE	*file;
  char	line[200];
  char	key[80];
  char	key_part[4][40];
  char	include_file[80];
  char	subject[80];
  char	subject_part[4][40];
  int	key_nr;
  int	subject_nr;
  int	hit = 0;
  int   next_hit = 0;
  int 	sts;
  int	i;  
  char	*s;

  if ( help_key) {
    cdh_ToLower( key, help_key);
  }

  // Open file
  if ( file_type == navh_eHelpFile_Base)
    dcli_get_defaultfilename( base_file, filestr, NULL);
  else if ( file_type == navh_eHelpFile_Project)
    dcli_get_defaultfilename( project_file, filestr, NULL);
  else if ( file_name)
    dcli_get_defaultfilename( file_name, filestr, NULL);
  else
    dcli_get_defaultfilename( base_file, filestr, NULL);

  // Replace symbol for language
  if ( strncmp( filestr, "$pwr_lang/", 10) == 0) {
    char lng_filestr[200];
    
    sprintf( lng_filestr, "$pwr_exe/%s/%s", Lng::get_language_str(),
	     &filestr[10]);
    strcpy( filestr, lng_filestr);
  }
  dcli_translate_filename( filestr, filestr);

  file = fopen( filestr, "r");
  if ( file == 0)
    return NAV__NOFILE;
	
  key_nr = dcli_parse( key, " 	", "", (char *)key_part,
		       sizeof( key_part) / sizeof( key_part[0]), 
		       sizeof( key_part[0]), 0);


  sts = dcli_read_line( line, sizeof( line), file);
  while ( ODD(sts)) {
    if ( cdh_NoCaseStrncmp( line, "<include>", 9) == 0) {      
      help_remove_spaces( &line[9], include_file);
      // Replace symbol for language
      if ( strncmp( include_file, "$pwr_lang/", 10) == 0) {
        char lng_include_file[200];

	sprintf( lng_include_file, "$pwr_exe/%s/%s", Lng::get_language_str(),
		 &include_file[10]);
	strcpy( include_file, lng_include_file);
      }

      if ( !noprop) {
	sts = get_next_key( help_key, navh_eHelpFile_Other, 
			    include_file, strict, next_key);
	if ( ODD(sts)) {
	  fclose( file);
	  return sts;
	}
      }
      hit = 0;
    }
    if ( cdh_NoCaseStrncmp( line, "<topic>", 7) == 0) {
      if ( (s = strstr( line, "<style>")) || (s = strstr( line, "<STYLE>"))) {
	*s = 0;
      }

      help_remove_spaces( &line[7], subject);
      if ( hit) {
	strcpy( next_key, subject);
	next_hit = 1;
	break;
      }
      cdh_ToLower( subject, subject);
      subject_nr = dcli_parse( subject, " 	", "", (char *)subject_part,
                	sizeof( subject_part) / sizeof( subject_part[0]),
			sizeof( subject_part[0]), 0);
      if ( key_nr == subject_nr ) {
	for ( i = 0; i < key_nr; i++) {
	  if ( (!strict && strncmp( subject_part[i], key_part[i], strlen(key_part[i])) == 0) ||
	       (strict && strcmp( subject_part[i], key_part[i]) == 0)) {
	    if ( i == key_nr - 1)
	      hit = 1;
	  }
	  else
	    break;
	}
      }
    }
    sts = dcli_read_line( line, sizeof( line), file);
  }
  fclose( file);

  if ( !next_hit)
    return NAV__TOPICNOTFOUND;
  return NAV__SUCCESS;
}

int	NavHelp::get_previous_key( char *help_key, navh_eHelpFile file_type, char *file_name,
				   bool strict, char *prev_key)
{
  char	filestr[200];
  FILE	*file;
  char	line[200];
  char	key[80];
  char	key_part[4][40];
  char	include_file[80];
  char	subject[80];
  char	subject_part[4][40];
  int	key_nr;
  int	subject_nr;
  int	hit = 0;
  int 	sts;
  int	i;  
  char	*s;
  char  prev[80] = "";

  if ( help_key) {
    cdh_ToLower( key, help_key);
  }

  // Open file
  if ( file_type == navh_eHelpFile_Base)
    dcli_get_defaultfilename( base_file, filestr, NULL);
  else if ( file_type == navh_eHelpFile_Project)
    dcli_get_defaultfilename( project_file, filestr, NULL);
  else if ( file_name)
    dcli_get_defaultfilename( file_name, filestr, NULL);
  else
    dcli_get_defaultfilename( base_file, filestr, NULL);

  // Replace symbol for language
  if ( strncmp( filestr, "$pwr_lang/", 10) == 0) {
    char lng_filestr[200];
    
    sprintf( lng_filestr, "$pwr_exe/%s/%s", Lng::get_language_str(),
	     &filestr[10]);
    strcpy( filestr, lng_filestr);
  }
  dcli_translate_filename( filestr, filestr);

  file = fopen( filestr, "r");
  if ( file == 0)
    return NAV__NOFILE;
	
  key_nr = dcli_parse( key, " 	", "", (char *)key_part,
		       sizeof( key_part) / sizeof( key_part[0]), 
		       sizeof( key_part[0]), 0);


  sts = dcli_read_line( line, sizeof( line), file);
  while ( ODD(sts)) {
    if ( cdh_NoCaseStrncmp( line, "<include>", 9) == 0) {      
      help_remove_spaces( &line[9], include_file);
      // Replace symbol for language
      if ( strncmp( include_file, "$pwr_lang/", 10) == 0) {
        char lng_include_file[200];

	sprintf( lng_include_file, "$pwr_exe/%s/%s", Lng::get_language_str(),
		 &include_file[10]);
	strcpy( include_file, lng_include_file);
      }

      if ( !noprop) {
	sts = get_next_key( help_key, navh_eHelpFile_Other, 
			    include_file, strict, prev_key);
	if ( ODD(sts)) {
	  fclose( file);
	  return sts;
	}
      }
      hit = 0;
    }
    if ( cdh_NoCaseStrncmp( line, "<topic>", 7) == 0) {
      if ( (s = strstr( line, "<style>")) || (s = strstr( line, "<STYLE>"))) {
	*s = 0;
      }

      help_remove_spaces( &line[7], subject);
      cdh_ToLower( subject, subject);
      subject_nr = dcli_parse( subject, " 	", "", (char *)subject_part,
                	sizeof( subject_part) / sizeof( subject_part[0]),
			sizeof( subject_part[0]), 0);
      if ( key_nr == subject_nr ) {
	for ( i = 0; i < key_nr; i++) {
	  if ( (!strict && strncmp( subject_part[i], key_part[i], strlen(key_part[i])) == 0) ||
	       (strict && strcmp( subject_part[i], key_part[i]) == 0)) {
	    if ( i == key_nr - 1) {
	      hit = 1;
	      strcpy( prev_key, prev);
	    }
	  }
	  else
	    break;
	}
      }
      if ( hit) 
	break;

      strcpy( prev, subject);
    }
    sts = dcli_read_line( line, sizeof( line), file);
  }
  fclose( file);

  if ( !hit || strcmp( prev, "") == 0)
    return NAV__TOPICNOTFOUND;
  return NAV__SUCCESS;
}


int	NavHelp::help_index( navh_eHelpFile file_type, char *file_name)
{
  char	filestr[80];
  FILE	*file;
  char	line[200];
  char	include_file[80];
  char	subject[80];
  int 	sts;
  char  *s;

  // Open file
  if ( file_type == navh_eHelpFile_Base)
    dcli_get_defaultfilename( base_file, filestr, NULL);
  else if ( file_type == navh_eHelpFile_Project)
    dcli_get_defaultfilename( project_file, filestr, NULL);
  else if ( file_name)
    dcli_get_defaultfilename( file_name, filestr, NULL);
  else
    dcli_get_defaultfilename( base_file, filestr, NULL);

  if ( strncmp( filestr, "$pwr_lang/", 10) == 0) {
    char lng_file[200];

    sprintf( lng_file, "$pwr_exe/%s/%s", Lng::get_language_str(),
	     &filestr[10]);
    strcpy( filestr, lng_file);
  }
  dcli_translate_filename( filestr, filestr);

  file = fopen( filestr, "r");
  if ( file == 0)
    return NAV__NOFILE;
	
  sts = dcli_read_line( line, sizeof( line), file);
  while ( ODD(sts))
  {
    if ( strncmp( line, "<include>", 9) == 0 || 
         strncmp( line, "<INCLUDE>", 9) == 0)
    {
      help_remove_spaces( &line[9], include_file);
      sts = help_index( navh_eHelpFile_Other, include_file);
    }
    else if ( strncmp( line, "<topic>", 7) == 0 || 
              strncmp( line, "<TOPIC>", 7) == 0)
    {
      if ( (s = strstr( line, "<style>")) || (s = strstr( line, "<STYLE>")))
	*s = 0;
      help_remove_spaces( &line[7], subject);

      (insert_cb)( parent_ctx, navh_eItemType_HelpBold,
		   subject,  "", "", subject, 
		   "", file_name, file_type, 0, NULL);
    }
    sts = dcli_read_line( line, sizeof( line), file);
  }
  fclose( file);
  return NAV__SUCCESS;
}



















































