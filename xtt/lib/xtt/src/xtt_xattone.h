/* 
 * Proview   $Id: xtt_xattone.h,v 1.3 2007-01-04 08:22:47 claes Exp $
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

#ifndef xtt_xattone_h
#define xtt_xattone_h

/* xtt_xattone.h -- Single attribute editor */

#ifndef pwr_h
# include "pwr.h"
#endif


class XAttOne {
 public:
  void 	*parent_ctx;
  pwr_sAttrRef aref;
  char 	name[80];
  unsigned int priv;
  int		input_open;
  int		access_rw;
  int         input_multiline;
  void 	*object;
  void	(*close_cb) ( void *, XAttOne *);
  int		(*is_authorized_cb)(void *, unsigned int);
  void	*client_data;
  brow_tObject input_node;
  char	input_name[80];
  pwr_tTid	atype;
  unsigned int asize;
  unsigned int aoffs;
  unsigned int aelem;
  
  XAttOne( void *xa_parent_ctx, 
	   pwr_sAttrRef *xa_objar,
	   char *xa_title,
	   unsigned int xa_priv,
	   int *xa_sts);
  virtual ~XAttOne();

  virtual void message( char severity, char *message) {}
  virtual void set_prompt( char *prompt) {}
  virtual int change_value( int set_focus) {return 0;}
  virtual void change_value_close() {}
  virtual void pop() {}
  virtual int set_value() {return 0;}

  int open_changevalue( char *name);
};

#endif







