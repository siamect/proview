/* 
 * Proview   $Id$
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

#ifndef cow_pn_gsdml_attr_h
#define cow_pn_gsdml_attr_h

/* cow_pn_gsdml_attr.h -- Profinet gsdml attribute editor */

#include "cow_pn_gsdml.h"
#include "cow_pn_gsdml_data.h"

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

class GsdmlAttrNav;
class CoWow;

class GsdmlAttr {
 public:
  void 		*parent_ctx;
  char 		name[80];
  pn_gsdml	*gsdml;
  int		edit_mode;
  GsdmlAttrNav	*attrnav;
  int		input_open;
  void 		*object;
  void       	(*close_cb) (void *);
  int		(*save_cb) (void *);
  int		(*help_cb) (void *, const char *);
  void		*client_data;
  int         	recall_idx;
  static char	value_recall[30][160];
  int		value_current_recall;
  pwr_tFileName data_filename;
  CoWow		*wow;

  GsdmlAttr( void *a_parent_ctx,
	     void *a_object,
	     pn_gsdml *a_gsdml,
	     int a_edit_mode,
	     const char *a_data_filename);
  virtual ~GsdmlAttr();

  virtual void message( char severity, const char *message) {}
  virtual void set_prompt( const char *prompt) {}
  virtual void change_value() {}

  void activate_exit();
  void activate_help();
  void activate_copy();
  void activate_cut();
  void activate_paste();
  void activate_print();
  void activate_viewio( int set);
  void activate_zoom_in();
  void activate_zoom_out();
  void activate_zoom_reset();
  void activate_cmd_ok();
  void activate_cmd_ca();
    
  static void gsdmlattr_message( void *attr, char severity, const char *message);
  static void gsdmlattr_change_value_cb( void *attr_ctx);
  static void cmd_close_apply_cb( void *ctx, void *data);
  static void cmd_close_no_cb( void *ctx, void *data);

};

#endif








