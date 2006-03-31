/* 
 * Proview   $Id: wb_pb_gsd_attr.h,v 1.1 2006-03-31 05:42:44 claes Exp $
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

#ifndef wb_pb_gsd_attr_h
#define wb_pb_gsd_attr_h

/* wb_pb_gsd_attr.h -- Profibus gsd attribute editor */

#include "wb_pb_gsd.h"

#if defined __cplusplus
extern "C" {
#endif

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

class GsdAttr {
  public:
    GsdAttr(
      Widget                    a_parent_wid,
      void			*a_parent_ctx,
      void 			*a_object,
      // void			(*close_cb) (attr_tCtx),
      // void			(*redraw_cb) (attr_tCtx),
      pb_gsd			*a_gsd,
      int			a_edit_mode);
    void 	*parent_ctx;
    Widget	parent_wid;
    char 	name[80];
    pb_gsd	*gsd;
    int		edit_mode;
    Widget	brow_widget;
    Widget	form_widget;
    Widget	toplevel;
    GsdAttrNav	*attrnav;
    Widget	msg_label;
    Widget	cmd_prompt;
    Widget	cmd_input;
    Widget	attrnav_form;
    Widget	cmd_ok;
    Widget	cmd_cancel;
    int		input_open;
    void 	*object;
    void       	(*close_cb) (void *);
    int		(*save_cb) (void *);
    void	*client_data;
    int         recall_idx;
    static char	value_recall[30][160];
    int		value_current_recall;

    void message( char severity, char *message);
    void set_prompt( char *prompt);
    void change_value();
    
    ~GsdAttr();
};


/*@}*/
#if defined __cplusplus
}
#endif
#endif








