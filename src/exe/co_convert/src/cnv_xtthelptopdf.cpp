/* 
 * Proview   $Id: cnv_xtthelptopdf.cpp,v 1.4 2008-11-03 09:50:24 claes Exp $
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

/* cnv_xtthelptopdf.cpp --
   Convert xtt help file to PDF. */

/*_Include files_________________________________________________________*/

#include <iostream>
#include <fstream>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

extern "C" {
#include "co_cdh.h"
#include "co_dcli.h"
}

#include "co_nav_help.h"
#include "co_lng.h"
#include "cnv_ctx.h"
#include "cnv_readxtthelp.h"
#include "cnv_xtthelptopdf.h"
#include "cnv_image.h"

#define pdf_cCellSize 110
/* Nice functions */
#define ODD(a)	(((int)(a) & 1) != 0)
#define EVEN(a)	(((int)(a) & 1) == 0)
#define max(Dragon,Eagle) ((Dragon) > (Eagle) ? (Dragon) : (Eagle))
#define min(Dragon,Eagle) ((Dragon) < (Eagle) ? (Dragon) : (Eagle))
#ifndef __ALPHA
#define abs(Dragon) ((Dragon) >= 0 ? (Dragon) : (-(Dragon)))
#endif

void CnvXtthelpToPdf::subject_to_fname( char *fname, const char *subject, int path)
{

  if ( path) {
    strcpy( fname, ctx->dir);
    strcat( fname, ctx->rx->name);
  }
  else
    strcpy( fname, ctx->rx->name);
  strcat( fname, ".pdf");
  cdh_ToLower( fname, fname);
}

CnvXtthelpToPdf::~CnvXtthelpToPdf()
{
  if ( !first_topic) {
    if ( status & pdf_mStatus_xtthelp) {
      status &= ~pdf_mStatus_xtthelp;
    }
  }
  topdf.close();
}

void *CnvXtthelpToPdf::insert( navh_eItemType item_type, const char *text1,
			       const char *text2, const char *text3, const char *alink, 
			       const char *link_bookmark, const char *file_name,
			       navh_eHelpFile file_type, int help_index, 
			       const char *bookmark)
{
  char link[80];
  if ( alink)
    strcpy( link, alink);

  if ( option & pdf_mOption_printDisable &&
       item_type != navh_eItemType_Option)
    return NULL;


  if ( (text2 && strcmp(text2, "") != 0) || 
       (text3 && strcmp(text3, "") != 0) ) {
    if ( !(status & pdf_mStatus_table))
      status |= pdf_mStatus_table;
  }
  else {
    if ( status & pdf_mStatus_table) {
      // Close table (keep if empty line) 
      if ( !( text1 && strcmp( text1, "") == 0 && 
	      (item_type == navh_eItemType_Help || 
	       item_type == navh_eItemType_HelpCode || 
	       item_type == navh_eItemType_HelpBold)))
        status &= ~pdf_mStatus_table;
    }
  }
  switch ( item_type) {
    case navh_eItemType_DocTitlePage:
    case navh_eItemType_DocInfoPage:
    case navh_eItemType_Topic:
    {
      strcpy( current_subject, text1);

      if ( item_type == navh_eItemType_DocTitlePage) {
	topdf.set_ci( pdf_eId_TitlePage);
      }
      else if ( item_type == navh_eItemType_DocInfoPage) {
	topdf.set_ci( pdf_eId_InfoPage);
	topdf.print_pagebreak( 0);
      }
      else {
	if ( topdf.ci == pdf_eId_Chapter) {
	  if ( !first_chaptertopic)
	    topdf.set_ci( pdf_eId_TopicL1);
	  else {
	    first_chaptertopic = 0;
	    topdf.y = pdf_cPageHeight - pdf_cTopMargin;
	  }
	}
      }

      if ( first_topic) {
	pwr_tFileName fname;

	subject_to_fname( fname, text1, 1);
	topdf.set_filename( pdf_eFile_Body, fname);
	topdf.open();

	status |= pdf_mStatus_xtthelp;
	first_topic = 0;
      }
      status |= pdf_mStatus_topic;
      return NULL;
    } 
    case navh_eItemType_EndTopic: {
      if ( status & pdf_mStatus_table)
	status &= ~pdf_mStatus_table;
      if ( status & pdf_mStatus_paragraph)
	status &= ~pdf_mStatus_paragraph;
      if ( status & pdf_mStatus_topic)
	status &= ~pdf_mStatus_topic;
      if ( user_style) {
	user_style = 0;
	topdf.set_ci( base_ci);
      }
      return NULL;
    }
    case navh_eItemType_Style: {
      if ( cdh_NoCaseStrcmp( text1, "function") == 0) {
	base_ci = topdf.ci;
	topdf.set_ci( pdf_eId_Function);
	user_style = 1;
      }
      return NULL;
    }
    case navh_eItemType_EndChapter: {
      if ( status & pdf_mStatus_table)
	status &= ~pdf_mStatus_table;
      if ( status & pdf_mStatus_paragraph)
	status &= ~pdf_mStatus_paragraph;
      if ( status & pdf_mStatus_topic)
	status &= ~pdf_mStatus_topic;

      topdf.set_ci( pdf_eId_TopicL1);
      user_style = 0;
      return NULL;
    }
    case navh_eItemType_Chapter: {
      if ( status & pdf_mStatus_table)
	status &= ~pdf_mStatus_table;
      if ( status & pdf_mStatus_paragraph)
	status &= ~pdf_mStatus_paragraph;
      if ( status & pdf_mStatus_topic)
	status &= ~pdf_mStatus_topic;

      topdf.set_ci( pdf_eId_Chapter);
      first_chaptertopic = 1;
      user_style = 0;

      topdf.reset_headernumbers( 1);
      return NULL;
    }
    case navh_eItemType_HeaderLevel: {
      if ( user_style) {
	user_style = 0;
	topdf.set_ci( base_ci);
      }
      topdf.incr_headerlevel();
      return NULL;
    }
    case navh_eItemType_EndHeaderLevel: {
      if ( user_style) {
	user_style = 0;
	topdf.set_ci( base_ci);
      }
      topdf.decr_headerlevel();
      return NULL;
    }
    case navh_eItemType_PageBreak: {
      topdf.print_pagebreak(0);
      return NULL;
    }
    case navh_eItemType_Help:
    case navh_eItemType_HelpCode:
    case navh_eItemType_HelpBold:
    {      
      int printmode;
      CnvStyle *hstyle;

      // Unable to calculate lenght because link i not known in confpass
      strcpy( link, "");
      
      if ( item_type == navh_eItemType_Help)
	hstyle = &topdf.style[topdf.ci].text;
      else if ( item_type == navh_eItemType_HelpBold)
	hstyle = &topdf.style[topdf.ci].boldtext;
      else if ( item_type == navh_eItemType_HelpCode)
	hstyle = &topdf.style[topdf.ci].code;

      if ( strcmp( link, "") != 0)
	printmode = pdf_mPrintMode_Start;
      else
	printmode = pdf_mPrintMode_Pos;
      if ( !(status & pdf_mStatus_table)) {
	topdf.x = pdf_cLeftMargin;
	topdf.print_text( text1, *hstyle, printmode);
      }
      else {
	topdf.x = pdf_cLeftMargin;
	topdf.print_text( text1, *hstyle);
        if ( text2 && strcmp( text2, "") != 0) {
	  topdf.x = pdf_cLeftMargin + pdf_cCellSize;
	  topdf.print_text( text2, *hstyle, pdf_mPrintMode_KeepY | pdf_mPrintMode_FixX);
	}
	if ( text3 && strcmp( text3, "") != 0) {
	  topdf.x = pdf_cLeftMargin + 2 * pdf_cCellSize;
	  topdf.print_text( text3, *hstyle, pdf_mPrintMode_KeepY | pdf_mPrintMode_FixX);
	}
      }
      if ( strcmp( link, "") != 0 && !conf_pass) {
        pwr_tFileName fname;
	char str[200];
	int page;

	if ( strncmp( link, "$web:", 5) == 0) {
	  if ( strncmp( &link[5], "$pwrp_web/", 10) == 0)
	    strcpy( fname, &link[15]);
	  else
	    strcpy( fname, &link[5]);
	  sprintf( str, " (%s %s)", Lng::translate("See"), fname);
	  if ( !(status & pdf_mStatus_table))
	    topdf.print_text( str, topdf.style[topdf.ci].link, 
			     pdf_mPrintMode_End | pdf_mPrintMode_FixX);	  
	  else {
	    topdf.x = pdf_cLeftMargin + 3 * pdf_cCellSize;
	    if ( pdf_cLeftMargin + 2 * pdf_cCellSize + strlen(text3) * hstyle->font_size * 0.65 > topdf.x)
	      topdf.x = pdf_cLeftMargin + 2 * pdf_cCellSize + strlen(text3) * hstyle->font_size * 0.65;
	    topdf.print_text( str, topdf.style[topdf.ci].link, 
			     pdf_mPrintMode_End | pdf_mPrintMode_FixX);	  
	  }
	} 
        else if ( (strstr( link, ".htm") != 0) || 
		  (strstr( link, ".pdf") != 0)) {
          strcpy( fname, link);
	  sprintf( str, " (%s %s)", Lng::translate("See"), fname);
	  if ( !(status & pdf_mStatus_table))
	    topdf.print_text( str, topdf.style[topdf.ci].link, 
			     pdf_mPrintMode_End | pdf_mPrintMode_FixX);	  
	  else {
	    topdf.x = pdf_cLeftMargin + 3 * pdf_cCellSize;
	    if ( pdf_cLeftMargin + 2 * pdf_cCellSize + strlen(text3) * hstyle->font_size * 0.65 > topdf.x)
	      topdf.x = pdf_cLeftMargin + 2 * pdf_cCellSize + strlen(text3) * hstyle->font_size * 0.65;
	    topdf.print_text( str, topdf.style[topdf.ci].link, 
			     pdf_mPrintMode_End | pdf_mPrintMode_FixX);	  
	  }
        }
        else {
	  char text[80];
	  int sts = topdf.content.find_link( link, text, &page);
	  if ( ODD(sts)) {
 	    sprintf( str, " (%s %s ", Lng::translate("See"), text);
	    sprintf( &str[strlen(str)], "%s %d)", Lng::translate("page"), page);
	    if ( !(status & pdf_mStatus_table))
	      topdf.print_text( str, topdf.style[topdf.ci].link, 
			       pdf_mPrintMode_End | pdf_mPrintMode_FixX);	  
	    else {
#if 0
	      if ( !(text3 && strcmp(text3, "") != 0)) {
		topdf.x = pdf_cLeftMargin + 2 * pdf_cCellSize;
		if ( pdf_cLeftMargin + pdf_cCellSize + strlen(text2) * hstyle->font_size * 0.5 > topdf.x)
		  topdf.x = pdf_cLeftMargin + pdf_cCellSize + strlen(text2) * hstyle->font_size * 0.5;
	      }
	      else {
		topdf.x = pdf_cLeftMargin + 3 * pdf_cCellSize;
		if ( pdf_cLeftMargin + pdf_cCellSize + strlen(text3) * hstyle->font_size * 0.5 > topdf.x)
		  topdf.x = pdf_cLeftMargin + 2 * pdf_cCellSize + strlen(text3) * hstyle->font_size * 0.5;
	      }
	      topdf.print_text( str, topdf.style[topdf.ci].link, 
			       pdf_mPrintMode_KeepY | pdf_mPrintMode_FixX);
#endif
	    }
	  }
	  else
	    if ( !(status & pdf_mStatus_table))
	      topdf.print_text( "", topdf.style[topdf.ci].link, 
			       pdf_mPrintMode_End | pdf_mPrintMode_FixX);	  
        }
      }
      else if ( bookmark) {
        // fp[cf] << tags[pdf_eTag_link].start << " name=\"" << bookmark << "\">";
      }

      return NULL;
    }
    case navh_eItemType_HelpHeader:
    {
      int hlevel;

      if ( !user_style)
	hlevel = topdf.ci - (int) pdf_eId_Chapter;
      else
	hlevel = base_ci - (int) pdf_eId_Chapter;
      topdf.print_h1( text1, hlevel, current_subject);
      return NULL;
    }
    case navh_eItemType_Header:
    {      
      topdf.print_h3( text1);
      return NULL;
    }
    case navh_eItemType_HeaderLarge:
    {      
      topdf.print_h2( text1);
      return NULL;
    }
    case navh_eItemType_HorizontalLine:
    {      
      topdf.print_horizontal_line();
      return NULL;
    }
    case navh_eItemType_Image:
    {      
      int sts = topdf.print_image( text1);
      if ( EVEN(sts))
	printf( "Image: %s not found\n", text1);
      return NULL;
    }
    case navh_eItemType_Option:
    {      
      if ( strcmp( text1, "printdisable") == 0)
	option |= pdf_mOption_printDisable;
      else if ( strcmp( text1, "printenable") == 0)
	option &= ~pdf_mOption_printDisable;
      return NULL;
    }
    default:
      return 0;
  }
  return 0;
}







