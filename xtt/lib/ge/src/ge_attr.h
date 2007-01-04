/* 
 * Proview   $Id: ge_attr.h,v 1.4 2007-01-04 08:18:34 claes Exp $
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

#include "glow.h"

/* ge_attr.h -- Ge attribute editor */

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

class AttrNav;

//! Attribute editor.
/*! Displays grow attributes and dynamic attributes for an object or graph. */
class Attr {
  public:
    Attr(
      void			*a_parent_ctx,
      void 			*a_object,
      attr_sItem  		*itemlist,
      int			item_cnt);
    void 	*parent_ctx;
    char 	name[80];
    AttrNav	*attrnav;
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

    virtual void message( char severity, char *message) {}
    virtual void set_prompt( char *prompt) {}
    virtual void change_value() {}
    virtual int reconfigure_attr() {return 1;}
    virtual void store();
    virtual void recall_next();
    virtual void recall_prev();
    static int get_plant_select_c( void *attr_ctx, char *value);
    static int get_current_colors_c( void *attr_ctx, glow_eDrawType *fill_color, 
				      glow_eDrawType *border_color, 
				      glow_eDrawType *text_color);    
    static int get_subgraph_info_c( void *attr_ctx, char *name, 
				     attr_sItem **itemlist, int *item_cnt);
    static int get_dyn_info_c( void *attr_ctx, GeDyn *dyn, 
				attr_sItem **itemlist, int *item_cnt);
    static void change_value_c( void *attr);
    static int reconfigure_attr_c( void *attr);
    static void message( void *attr, char severity, char *message);
    virtual ~Attr();
};


/*@}*/
#endif








