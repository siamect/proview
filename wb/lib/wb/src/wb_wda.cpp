/* 
 * Proview   $Id: wb_wda.cpp,v 1.10 2007-01-04 07:29:04 claes Exp $
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

/* wb_wda.cpp -- spreadsheet editor */

#if defined OS_VMS && defined __ALPHA
# pragma message disable (NOSIMPINT,EXTROUENCUNNOBJ)
#endif

#if defined OS_VMS && !defined __ALPHA
# pragma message disable (LONGEXTERN)
#endif

#include "glow_std.h"

#include <stdio.h>
#include <stdlib.h>

#include "co_cdh.h"
#include "co_dcli.h"
#include "co_time.h"
#include "wb_wda_msg.h"
#include "co_wow.h"
#include "flow.h"
#include "flow_browctx.h"
#include "flow_browapi.h"
#include "wb_wda.h"
#include "wb_wdanav.h"
#include "wb_wtt.h"
#include "wb_wnav.h"
#include "co_xhelp.h"


void Wda::message_cb( void *wda, char severity, char *message)
{
  ((Wda *)wda)->message( severity, message);
}

void Wda::change_value_cb( void *wda)
{
  ((Wda *)wda)->change_value(1);
}

void Wda::print()
{
  char filename[80] = "pwrp_tmp:wda.ps";
  char cmd[200];
  int sts;

  dcli_translate_filename( filename, filename);
  wdanav->print(filename);

  sprintf( cmd, "wb_gre_print.sh %s", filename); 
  sts = system( cmd);
}

void Wda::set_editmode( int editmode, ldh_tSesContext ldhses)
{
  this->ldhses = ldhses;
  this->editmode = editmode;
  ((WdaNav *)wdanav)->set_editmode( editmode, ldhses);
}

int Wda::open_changevalue( char *name)
{
  int sts;

  sts = ((WdaNav*)wdanav)->select_by_name( name);
  if ( EVEN(sts)) return sts;

  change_value(0);
  return WDA__SUCCESS;
}

void Wda::open_class_dialog()
{
  pwr_tOName hierstr;
  char classstr[80];
  int size;
  int sts;
  char *s;

  if ( cdh_ObjidIsNotNull( objid)) {
    sts = ldh_ObjidToName( ldhses, objid, ldh_eName_Hierarchy,
		hierstr, sizeof(hierstr), &size);
    if ( EVEN(sts))
      strcpy( hierstr, "");
  }
  else
    strcpy( hierstr, "");
  
  if ( classid != 0) {
    sts = ldh_ClassIdToName( ldhses, classid, classstr, sizeof(classstr),
			     &size);
    if ( EVEN(sts))
      strcpy( classstr, "");
    else if ( (s = strchr( classstr, '-')))
      strcpy( classstr, s + 1);
  }
  else
    strcpy( classstr, "");

  open_class_dialog( hierstr, classstr);
}

void Wda::open_attr_dialog()
{
  // Get all attributes for this class
  int	i, j;
  char	body[20];
  ldh_sParDef 	*bodydef;
  int sts;
  int rows;
  int attr_cnt;
  char (*attr_vect)[80];

  // Count the attributes
  attr_cnt = 0;
  for ( i = 0; i < 3; i++)
  {
    if ( i == 0)
      strcpy( body, "RtBody");
    else if ( i == 1)
      strcpy( body, "DevBody");
    else
      strcpy( body, "SysBody");

    sts = ldh_GetObjectBodyDef( ldhses, classid, body, 1,
		&bodydef, &rows);
    if ( EVEN(sts))
      continue;
    for ( j = 0; j < rows; j++)
      attr_cnt++;

    free((char *) bodydef);
  }

  attr_vect = (char (*)[80]) calloc( attr_cnt + 1, 80);

  attr_cnt = 0;
  for ( i = 0; i < 3; i++)
  {
    if ( i == 0)
      strcpy( body, "RtBody");
    else if ( i == 1)
      strcpy( body, "DevBody");
    else
      strcpy( body, "SysBody");

    sts = ldh_GetObjectBodyDef( ldhses, classid, body, 1,
		&bodydef, &rows);
    if ( EVEN(sts))
      continue;
    for ( j = 0; j < rows; j++)
      strcpy( attr_vect[attr_cnt++], bodydef[j].ParName);

    free((char *) bodydef);
  }
  strcpy( attr_vect[attr_cnt], "");


  wow->CreateList( "Attributes", (char *)attr_vect,
		   set_attr_cb, (void *)this);

}

int Wda::next_attr()
{
  // Get all attributes for this class
  int	i, j;
  char	body[20];
  ldh_sParDef 	*bodydef;
  int sts;
  int rows;
  int get_next;

  if ( strcmp( attribute, "") == 0)
    // Get first attribute
    get_next = 1;
  else
    get_next = 0;
  for ( i = 0; i < 3; i++)
  {
    if ( i == 0)
      strcpy( body, "RtBody");
    else if ( i == 1)
      strcpy( body, "DevBody");
    else
      strcpy( body, "SysBody");

    sts = ldh_GetObjectBodyDef( ldhses, classid, body, 1,
		&bodydef, &rows);
    if ( EVEN(sts))
      continue;
    for ( j = 0; j < rows; j++) {
      if ( cdh_NoCaseStrcmp( attribute, bodydef[j].ParName) == 0)
        get_next = 1;
      else if ( get_next) {
        strcpy( attribute, bodydef[j].ParName);
        sts = ((WdaNav *)wdanav)->update( objid, classid, attribute, attrobjects);
        free((char *) bodydef);
        return WDA__SUCCESS;
      }
    }
    free((char *) bodydef);
  }
  return WDA__NONEXTATTR;
}

int Wda::prev_attr()
{
  // Get all attributes for this class
  int	i, j;
  char	body[20];
  ldh_sParDef 	*bodydef;
  int sts;
  int rows;
  int get_last;
  char prev_attr[40];

  strcpy( prev_attr, "");
  if ( strcmp( attribute, "") == 0)
    // Get last attribute
    get_last = 1;
  else
    get_last = 0;
  for ( i = 0; i < 3; i++)
  {
    if ( i == 0)
      strcpy( body, "RtBody");
    else if ( i == 1)
      strcpy( body, "DevBody");
    else
      strcpy( body, "SysBody");

    sts = ldh_GetObjectBodyDef( ldhses, classid, body, 1,
		&bodydef, &rows);
    if ( EVEN(sts))
      continue;
    for ( j = 0; j < rows; j++) {
      if ( cdh_NoCaseStrcmp( attribute, bodydef[j].ParName) == 0) {
        if ( strcmp( prev_attr, "") == 0) {
          // get_last = 1;
          free((char *) bodydef);
          return WDA__NOPREVATTR;
        }
        else {
          strcpy( attribute, prev_attr);
          sts = ((WdaNav *)wdanav)->update( objid, classid, attribute, attrobjects);
          free((char *) bodydef);
          return WDA__SUCCESS;
        }
      }
      strcpy( prev_attr, bodydef[j].ParName);
    }
    free((char *) bodydef);
  }

  if ( get_last && strcmp( prev_attr, "") != 0) {
    strcpy( attribute, prev_attr);
    sts = ((WdaNav *)wdanav)->update( objid, classid, attribute, attrobjects);
    return WDA__SUCCESS;
  }
  return WDA__NOPREVATTR;
}

void Wda::set_attr_cb( void *ctx, char *text)
{
  Wda *wda = (Wda *)ctx;
  int sts;

  strcpy( wda->attribute, text);
  sts = ((WdaNav *)wda->wdanav)->update( wda->objid, wda->classid,
		wda->attribute, wda->attrobjects);
  if ( EVEN(sts))
    wda->wow->DisplayError("Spreadsheet error", wnav_get_message( sts));
}

Wda::~Wda()
{
}

Wda::Wda( 
	void 		*wa_parent_ctx, 
	ldh_tSesContext wa_ldhses, 
	pwr_tObjid 	wa_objid,
	pwr_tClassId 	wa_classid,
	char            *wa_attribute,
	int 		wa_editmode,
	int 		wa_advanced_user,
	int		wa_display_objectname) :
  parent_ctx(wa_parent_ctx), 
  ldhses(wa_ldhses), objid(wa_objid), classid(wa_classid),
  editmode(wa_editmode), 
  input_open(0), input_multiline(0), 
  close_cb(0), redraw_cb(0), client_data(0),
  attrobjects(0)
{
  strcpy( attribute, wa_attribute);
}

