/* 
 * Proview   $Id: wb_utl.h,v 1.8 2005-11-22 12:26:58 claes Exp $
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

#ifndef wb_utl_h
#define wb_utl_h

#ifndef wb_ldh_h
#include "wb_ldh.h"
#endif

#ifndef _XtIntrinsic_h
#include <X11/Intrinsic.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif


typedef struct utl_s_objidlist {
	pwr_tObjid  objid;
	struct utl_s_objidlist *next;
	} utl_t_objidlist;

typedef struct {
	pwr_tString40	volume_name;
	pwr_tVolumeId	volume_id;
	pwr_tString40	p1;
	pwr_tString40	p2;
	pwr_tString40	p3;
	pwr_tString40	p4;
	} utl_t_volumelist;


void utl_objidlist_free (
  utl_t_objidlist *list
);

int utl_objidlist_insert (
  pwr_sAttrRef  *arp,
  void		*l,
  void		*c,
  void		*dum1,
  void		*dum2,
  void		*dum3
);

int utl_toupper ( 
  char	*str_upper,
  char	*str
);

int utl_parse (
  char	*instring,
  char	*parse_char,
  char	*inc_parse_char,
  char	*outstr,
  int	max_rows,
  int 	max_cols
);

int utl_parse_indexstring (
  char	*indexstring,
  char	*indexarray,
  int	size
);




int utl_show_node (
  ldh_tSesContext ldhses,
  int		terminal,
  char		*filename
);

int utl_show_plcpgm (
  ldh_tSesContext ldhses,
  int		terminal,
  char		*filename
);

int utl_show_window (
  ldh_tSesContext ldhses,
  char		*plcstring,
  int		terminal,
  char		*filename
);

int utl_show_modules (
  ldh_tSesContext ldhses,
  char 		*objdidstr,
  char 		*name,
  char 		*hiername,
  int		terminal,
  char		*filename
);

int utl_show_object (
  ldh_tSesContext ldhses,
  char		*windowstring,
  pwr_tObjid	sendobjdid,
  int		terminal,
  char		*filename
);

int utl_set_object_parameter ( 
  ldh_tSesContext ldhses,
  char		*hiername,
  char		*classname,
  char		*name,
  char		*parameter,
  char		*valuestr,
  int		terminal,
  char		*filename,
  int		confirm,
  int		log
);

int utl_show_obj_hier_class_name (
  ldh_tSesContext ldhses,
  char		*hiername,
  char		*classname,
  char		*name,
  char		*parameter,
  char		*volumes,
  int		terminal,
  char		*filename,
  int		full,
  int		allvolumes,
  int		append,
  int		exactorder
);

int utl_show_object_objdid (
  ldh_tSesContext ldhses,
  char		*objdidstring,
  int		terminal,
  char		*filename
);

int utl_show_hierarchy (
  ldh_tSesContext ldhses,
  int		terminal,
  char		*filename
);



int utl_show_connection (
  ldh_tSesContext ldhses,
  char		*windowstring,
  int		terminal,
  char		*filename
);

int utl_link ( 
  ldh_tSesContext ldhses,
  char		*nodename,
  int		debug
);

int utl_compile ( 
  ldh_tSesContext ldhses,
  ldh_tWBContext  ldhwb,
  char		*plcname,
  char		*windowname,
  char		*hiername,
  char		*fromname,
  int		modified,
  int		debug,
  int		allvolume,
  char		*volumes
);

int utl_crossref_object ( 
  ldh_tSesContext ldhses,
  char		*objectname,
  int		terminal,
  char		*filename
);

int utl_crossref_hier_class_name ( 
  ldh_tSesContext ldhses,
  char		*hiername,
  char		*classname,
  char		*name,
  int		terminal,
  char		*filename
);



int utl_create_mainwindow (
  int argc,
  char **argv,
  Widget *widget
);

int utl_destroy_mainwindow (
Widget widget
);

int utl_print_plc ( 
  Widget	  parentwidget,
  ldh_tSesContext ldhses,
  ldh_tWBContext  ldhwb,
  char		  *plcstring,
  int		  document,
  int		  overview
);

int utl_print_plc_hier (
  Widget	parentwidget,
  ldh_tSesContext ldhses,
  ldh_tWBContext  ldhwb,
  char		*hiername,
  char		*fromname,
  int		document,
  int		overview,
  int		all
);

int utl_print_document (
  pwr_tObjid	Objdid,
  Widget	parent_widget,
  ldh_tSesContext ldhses,
  ldh_tWBContext  ldhwb,
  unsigned long	document,
  unsigned long	overview
);

int utl_print_repage (
  ldh_tSesContext ldhses
);

int utl_print_content (
  ldh_tSesContext ldhses,
  int		terminal,
  char		*filename
);

int utl_repage (
  pwr_tObjid  Objdid,
  ldh_tSesContext ldhses,
  unsigned long	plcpgm_page,
  unsigned long	*doc_page,
  unsigned long	dum3,
  unsigned long	dum4
);

int utl_redraw_plc (
  Widget	  parentwidget,
  ldh_tSesContext ldhses,
  ldh_tWBContext  ldhwb,
  char		*plcstring
);

int utl_redraw_plc_hier (
  Widget	  parentwidget,
  ldh_tSesContext ldhses,
  ldh_tWBContext  ldhwb,
  char		*hiername,
  char		*fromname,
  int		all
);

int utl_redraw_windows ( 
  pwr_tObjid	  Objdid,
  Widget	  parent_widget,
  ldh_tSesContext ldhses,
  ldh_tWBContext  ldhwb
);


int utl_list (
  ldh_tSesContext ldhses,
  char		*list_str,
  char		*hier_str,
  char		*object_str,
  char		*filename,
  int		print,
  int		allvolumes,
  char		*volumes,
  int		keep_crosslist
);


int utl_cut_segments (
  char	*outname,
  char	*name,
  int	segments
);

int utl_configure_card (
  ldh_tSesContext ldhses,
  char		*rackname,
  char		*cardname,
  char		*cardclassname,
  char		*channame,
  char		*chanidentity,
  char		*chandescription,
  char		*table
);

int utl_get_filename (
  char	*filename
);

int utl_get_projectname(
  char		*projectname
);

int utl_get_systemname(
  char		*systemname,
  char		*systemgroup
);

int utl_get_systemobject(
  ldh_tSesContext ldhses,
  pwr_tObjid	*system_objid,
  char		*systemname,
  char		*systemgroup
);

int utl_sortchildren (
  ldh_tSesContext ldhses,
  char		*parent_name,
  int		classort
);

int utl_create_object (
  ldh_tSesContext ldhses,
  char		*object_name,
  char		*class_name,
  char		*destination_name,
  int		first,
  int		last,
  int		after,
  int		before
);

int utl_move_object ( 
  ldh_tSesContext ldhses,
  char		*source_name,
  char		*destination_name,
  char		*name,
  int		first,
  int		last,
  int		after,
  int		before
);

int utl_copy_objects ( 
  ldh_tSesContext ldhses,
  char		*source_name,
  char		*destination_name,
  char		*name,
  int		hier,
  int		first,
  int		last,
  int		after,
  int		before
);

int utl_move_window (
  ldh_tSesContext ldhses,
  char		*source_name,
  char		*destination_name
);

int utl_connect ( 
  ldh_tSesContext ldhses,
  char		*object_name,
  char		*connect_name,
  int		reconnect
);


int utl_disconnect (
  ldh_tSesContext ldhses,
  char		*object_name
);

int utl_delete_objects (
  ldh_tSesContext ldhses,
  char		*hiername,
  char		*classname,
  char		*name,
  int		confirm,
  int		log
);

int utl_delete_tree (
  ldh_tSesContext ldhses,
  char		*name,
  int		confirm,
  int		log
);

int utl_delete_volume (
  ldh_tWBContext  ldhwb,
  char		*name,
  int		confirm,
  int		log
);

int utl_export_object ( 
  ldh_tSesContext ldhses,
  char		*name,
  char		*prefix,
  int		debugparameter,
  int		gms,
  char		*filename,
  int		append
);

int utl_realloc_s( 	
  char	**list_ptr, 
  int	count, 
  int	size, 
  int	*alloc
);

int utl_realloc (
  char	**list_ptr,
  int	old_size,
  int	new_size
);

int utl_create_loadfiles (
  ldh_tSesContext ldhses,
  char		*volumes,
  int		allvolumes
);

int utl_create_bootfile (
  ldh_tSesContext ldhses
);


int utl_set_template( 	
  ldh_tSesContext	ldhses, 
  int			signalobjectsegments,
  int			showsigchancon,
  int			sigchanconsegments,
  int			showdetecttext
);
pwr_tStatus utl_show_class_classhier (
  ldh_tSesContext ldhses,
  char		*hiername,
  char		*name,
  int		terminal,
  char		*filename,
  int		full,
  int		contents,
  int		all
);

pwr_tStatus utl_get_module_time (
    char		*libr_name,
    char		*module_name,
    pwr_tTime		*time
);

pwr_tStatus utl_read_line(		char	*line,
					int	maxsize,
					FILE	*file,
					int	*line_count);


pwr_tStatus utl_create_volume( 
		ldh_tWBContext  ldhwb,
		char 	*volumename, 
		char	*volumeclass_str, 
		char	*volumeid_str);

pwr_tStatus	utl_show_volumes( 
  ldh_tSesContext	ldhses,
  int			allvolumes
);

int utl_revert ( 
  ldh_tSesContext ldhses,
  int		confirm
);

#ifdef __cplusplus
}
#endif

#endif
