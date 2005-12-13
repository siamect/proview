/* 
 * Proview   $Id: wb_adminnav.h,v 1.5 2005-12-13 15:15:53 claes Exp $
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

#ifndef ge_adminnav_h
#define ge_adminnav_h

#include <iostream.h>

#if defined __cplusplus
extern "C" {
#endif

#ifndef pwr_h
# include "pwr.h"
#endif

#ifndef co_user_h
# include "co_user.h"
#endif

#ifndef flow_h
#include "flow.h"
#endif

#ifndef flow_browapi_h
#include "flow_browapi.h"
#endif

#ifndef wb_wnav_brow_h
#include "wb_wnav_brow.h"
#endif

#define adminnav_cVersion	"X3.0b"

typedef enum {
  adminnav_eMode_Projects,
  adminnav_eMode_Volumes,
  adminnav_eMode_Users
} adminnav_eMode;
	      

typedef struct adminnav_s_ProjMenu {
	char			name[80];
	int			item_type;
	char			project[120];
	char			base[120];
	char			root[120];
	char			hierarchy[120];
	char			description[120];
 	int			pixmap;
	struct adminnav_s_ProjMenu *child_list;
	struct adminnav_s_ProjMenu *parent;
	struct adminnav_s_ProjMenu *next;
} adminnav_sProjMenu;

typedef struct adminnav_s_Base {
	char			name[80];
	char			root[120];
	struct adminnav_s_Base	*next;
} adminnav_sBase;

typedef enum {
	adminnav_eItemType_ProjHier,
	adminnav_eItemType_Project,
	adminnav_eItemType_ProjDb,
	adminnav_eItemType_ProjVolume,
	adminnav_eItemType_Volume,
	adminnav_eItemType_VolComment,
	adminnav_eItemType_User,
	adminnav_eItemType_UsersGroup
	} adminnav_eItemType;

typedef enum {
	adminnav_mOpen_All	= ~0,
	adminnav_mOpen_Children	= 1 << 0,
	adminnav_mOpen_Attributes = 1 << 1,
	adminnav_mOpen_Crossref	= 1 << 2
	} adminnav_mOpen;

class AdminNav {
  public:
    AdminNav(
	void *xn_parent_ctx,
	Widget	xn_parent_wid,
	char *xn_name,
	Widget *w,
	pwr_tStatus *status);
    ~AdminNav();

    void 		*parent_ctx;
    Widget		parent_wid;
    char 		name[80];
    Widget		brow_widget;
    Widget		form_widget;
    Widget		toplevel;
    WNavBrow		*brow;
    XtIntervalId	trace_timerid;
    int			trace_started;
    void 		(*message_cb)( void *, char, char *);
    int 		(*set_focus_cb)( void *, void *);
    adminnav_sProjMenu	*proj_menu_tree;
    adminnav_sBase	*baselist;
    int			displayed;
    adminnav_eMode	mode;
    GeUser		*gu;

    void message( char sev, char *text);
    void proj_tree_build();
    void proj_tree_load();
    void proj_tree_free();
    void volumes_tree_build();
    void volumes_tree_free();
    void users_tree_build();
    void users_tree_free();
    int volumes_save();
    int volumes_add( char *volumename, char *volumeid, char *projectname,
		     brow_tNode node, flow_eDest dest);
    void volumes_delete( brow_tNode node);
    void set_inputfocus( int focus);
    void insert_tree( adminnav_sProjMenu *menu, adminnav_sProjMenu *parent,
		      char *hierarchy);
    void enable_events();
    int get_select( void **item);
    int basename_to_baseroot( char *name, char *root);
    void refresh();
    int print( char *filename);
    void zoom( double zoom_factor);
    void get_zoom( double *zoom_factor);
    void unzoom();
    void view( adminnav_eMode m);

};

class Item {
  public:
    Item( adminnav_eItemType	item_type) :
	type( item_type) {};
    adminnav_eItemType	type;
    virtual int	open_children( AdminNav *adminnav, double x, double y) 
      { return 0;}
    virtual int	close( AdminNav *adminnav, double x, double y)
      { return 0;}
    virtual char *identity() { return "";}
    virtual brow_tNode get_node() = 0;
    virtual void print( ofstream& fp) {}

    virtual ~Item() {}
};

class ItemProjHier : public Item {
  public:
    ItemProjHier( AdminNav *adminnav, char *item_name, char *item_hier,
	adminnav_sProjMenu *item_child_list,
	brow_tNode dest, flow_eDest dest_code);
    brow_tNode		node;
    char	 	name[120];
    char		hierarchy[120];
    adminnav_sProjMenu	*child_list;

    int			open_children( AdminNav *adminnav, double x, double y);
    int			close( AdminNav *adminnav, double x, double y);
    char		*identity() { return hierarchy;}
    brow_tNode		get_node() { return node;}
};

class ItemProject : public Item {
  public:
    ItemProject( AdminNav *adminnav, char *item_name,
	char *item_project, char *item_base, char *item_root, 
	char *item_hierarchy, char *item_descr,
	brow_tNode dest, flow_eDest dest_code);
    brow_tNode		node;
    char	 	name[120];
    char	 	project[120];
    char	 	base[120];
    char	 	root[120];
    char	 	hierarchy[120];
    char	 	description[120];
    int			open_children( AdminNav *adminnav, double x, double y);
    int     		close( AdminNav *adminnav, double x, double y);
    char		*identity() { return hierarchy;}
    brow_tNode		get_node() { return node;}
};

class ItemProjDb : public Item {
  public:
    ItemProjDb( AdminNav *adminnav, char *item_name,
	char *item_project, char *item_db, char *item_root,
	brow_tNode dest, flow_eDest dest_code);
    brow_tNode		node;
    char	 	name[120];
    char	 	project[120];
    char	 	db[120];
    char	 	hierarchy[120];
    char	 	root[120];
    int			open_children( AdminNav *adminnav, double x, double y);
    int     		close( AdminNav *adminnav, double x, double y);
    char		*identity() { return hierarchy;}
    brow_tNode		get_node() { return node;}
};

class ItemProjVolume : public Item {
  public:
    ItemProjVolume( AdminNav *adminnav, char *item_name,
	char *item_project, char *item_db, char *item_volume, char *item_root,
	brow_tNode dest, flow_eDest dest_code);
    brow_tNode		node;
    char	 	name[120];
    char	 	project[120];
    char	 	db[120];
    char	 	hierarchy[120];
    char	 	volume[120];
    char	 	root[120];
    char		*identity() { return hierarchy;}
    brow_tNode		get_node() { return node;}
};

class ItemVolume : public Item {
  public:
    ItemVolume( AdminNav *adminnav,
	char *item_name, char *item_vid, char *item_project,
	brow_tNode dest, flow_eDest dest_code);
    brow_tNode		node;
    pwr_tVolumeId	vid;
    char	 	name[120];
    char	 	project[120];

    char		*identity() { return name;}
    brow_tNode		get_node() { return node;}
    void		modify( char *item_name, char *item_vid, char *item_project);
    void		print( ofstream& fp);
};

class ItemVolComment : public Item {
  public:
    ItemVolComment( AdminNav *adminnav,
	char *item_text,
	brow_tNode dest, flow_eDest dest_code);
    brow_tNode		node;
    char		text[120];

    char		*identity() { return text;}
    brow_tNode		get_node() { return node;}
    void		print( ofstream& fp);
};

class ItemUsersGroup : public Item {
  public:
    ItemUsersGroup( AdminNav *adminnav,
	SystemList *item_group,
	brow_tNode dest, flow_eDest dest_code);
    brow_tNode		node;
    SystemList		*group;
    char		name[120];

    int			open_children( AdminNav *adminnav, double x, double y);
    int     		close( AdminNav *adminnav, double x, double y);
    char		*identity() { return name;}
    brow_tNode		get_node() { return node;}
};

class ItemUser : public Item {
  public:
    ItemUser( AdminNav *adminnav,
	UserList *item_user,
	brow_tNode dest, flow_eDest dest_code);
    brow_tNode		node;
    UserList		*user;
    char		name[120];

    char		*identity() { return name;}
    brow_tNode		get_node() { return node;}
};

#if defined __cplusplus
}
#endif
#endif
















