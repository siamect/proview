/* 
 * Proview   $Id: ge_attr.h,v 1.3 2005-09-01 14:57:52 claes Exp $
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

#ifndef ge_attr_h
#define ge_attr_h

/* ge_attr.h -- Ge attribute editor */

#if defined __cplusplus
extern "C" {
#endif

/*! \file ge_attr.h
    \brief Contains the Attr class. */
/*! \addtogroup Ge */
/*@{*/

class GeDyn;

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

//! Attribute editor.
/*! Displays grow attributes and dynamic attributes for an object or graph. */
class Attr {
  public:
    Attr(
      Widget                    a_parent_wid,
      void			*a_parent_ctx,
      void 			*a_object,
      // void			(*close_cb) (attr_tCtx),
      // void			(*redraw_cb) (attr_tCtx),
      attr_sItem  		*itemlist,
      int			item_cnt);
    void 	*parent_ctx;
    Widget	parent_wid;
    char 	name[80];
    Widget	brow_widget;
    Widget	form_widget;
    Widget	toplevel;
    void	*attrnav;
    Widget	msg_label;
    Widget	cmd_prompt;
    Widget	cmd_input;
    Widget	cmd_scrolledinput;
    Widget	cmd_scrolled_ok;
    Widget	cmd_scrolled_ca;
    Widget	attrnav_form;
    int		input_open;
    void 	*object;
    void	(*close_cb) (Attr *);
    void	(*redraw_cb) (Attr *);
    int		(*get_subgraph_info_cb)( void *, char *, attr_sItem  **, 
                          int *);
    int		(*get_dyn_info_cb)( void *, GeDyn *, attr_sItem  **, 
                          int *);
    int 	(*reconfigure_attr_cb)( void *, void *,
			    attr_sItem **, int *, void **);
    void	(*store_cb) (void *, void *);
    int		(*recall_cb) (void *, void *, int, GeDyn **);
    int		(*set_data_cb) (void *, void *, GeDyn *);
    int		(*get_plant_select_cb) (void *, char *);
    int 	(*get_current_colors_cb)( void *, glow_eDrawType *, glow_eDrawType *,
						  glow_eDrawType *);
    void	*client_data;
    int         recall_idx;
    GeDyn 	*original_data;
    static char	value_recall[30][160];
    int		value_current_recall;

    void message( char severity, char *message);
    void set_prompt( char *prompt);
    void change_value();
    
    ~Attr();
};


/*@}*/
#if defined __cplusplus
}
#endif
#endif








