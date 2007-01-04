/* 
 * Proview   $Id: rt_pb_gsd_attr.h,v 1.4 2007-01-04 08:42:20 claes Exp $
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

#ifndef rt_pb_gsd_attr_h
#define rt_pb_gsd_attr_h

/* rt_pb_gsd_attr.h -- Profibus gsd attribute editor */

#include "rt_pb_gsd.h"

typedef struct {
	void	*value;
	char	name[80];
	int	type;
	int	size;
	double	minlimit;
	double	maxlimit;
	int	noedit;
	int	multiline;
    	int	mask;
} attr_sItem;

class GsdAttrNav;
class CoWow;

class GsdAttr {
 public:
  void 		*parent_ctx;
  char 		name[80];
  pb_gsd	*gsd;
  int		edit_mode;
  GsdAttrNav	*attrnav;
  int		input_open;
  void 		*object;
  void       	(*close_cb) (void *);
  int		(*save_cb) (void *);
  int		(*help_cb) (void *, char *);
  void		*client_data;
  int         	recall_idx;
  static char	value_recall[30][160];
  int		value_current_recall;
  CoWow		*wow;

  GsdAttr( void *a_parent_ctx,
	   void *a_object,
	   pb_gsd *a_gsd,
	   int a_edit_mode);
  virtual ~GsdAttr();

  virtual void message( char severity, char *message) {}
  virtual void set_prompt( char *prompt) {}
  virtual void change_value() {}

  void activate_exit();
  void activate_help();
  void activate_copy();
  void activate_cut();
  void activate_paste();
  void activate_print();
  void activate_zoom_in();
  void activate_zoom_out();
  void activate_zoom_reset();
  void activate_cmd_ok();
  void activate_cmd_ca();
    
  static void gsdattr_message( void *attr, char severity, char *message);
  static void gsdattr_change_value_cb( void *attr_ctx);
  static void cmd_close_apply_cb( void *ctx, void *data);
  static void cmd_close_no_cb( void *ctx, void *data);

};

#endif








