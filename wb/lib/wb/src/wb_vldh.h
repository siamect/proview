#ifndef wb_vldh_h
#define wb_vldh_h

/* wb_vldh.h -- very local datahandler

   PROVIEW/R
   Copyright (C) 1996 by Comator Process AB.

   <Description>.  */

#ifndef _XtIntrinsic_h
#include <X11/Intrinsic.h>
#endif

#ifndef wb_ldh_h
#include "wb_ldh.h"
#endif

#include "flow_api.h"

#define	OT_POINT	16
#define	OT_SUBORDER	200

/* Class index for quick reference */
#define	VLDH_CLASS_ORDER	0
#define	VLDH_CLASS_SORDER	1
#define	VLDH_CLASS_LORDER	2
#define	VLDH_CLASS_CORDER	3
#define	VLDH_CLASS_DORDER	4
#define	VLDH_CLASS_TRANS	5
#define	VLDH_CLASS_STEP		6
#define	VLDH_CLASS_INITSTEP	7
#define	VLDH_CLASS_SUBSTEP	8
#define VLDH_CLASS_CONANALOG	9
#define VLDH_CLASS_CONDIGITAL	10
#define VLDH_CLASS_CONGRAFCET	11
#define	VLDH_CLASS_SSBEGIN	12
#define	VLDH_CLASS_SSEND	13
#define	VLDH_CLASS_POINT	14
#define	VLDH_CLASS_DI		15
#define	VLDH_CLASS_DO		16
#define	VLDH_CLASS_DV		17
#define	VLDH_CLASS_AI		18
#define	VLDH_CLASS_AO		19
#define	VLDH_CLASS_AV		20
#define	VLDH_CLASS_CO		21
#define	VLDH_CLASS_GETDP	22
#define	VLDH_CLASS_GETAP	23
#define	VLDH_CLASS_ORDERACT	24
#define	VLDH_CLASS_SHOWPLCATTR	25
#define	VLDH_CLASS_PORDER	26
#define	VLDH_CLASS_GETDI	27
#define	VLDH_CLASS_GETDO	28
#define	VLDH_CLASS_GETDV	29
#define	VLDH_CLASS_GETAI	30
#define	VLDH_CLASS_GETAO	31
#define	VLDH_CLASS_GETAV	32
#define	VLDH_CLASS_AND		33
#define	VLDH_CLASS_STODO	34
#define	VLDH_CLASS_STODV	35
#define	VLDH_CLASS_STOAO	36
#define	VLDH_CLASS_STOAV	37
#define	VLDH_CLASS_SETDO	38
#define	VLDH_CLASS_SETDV	39
#define	VLDH_CLASS_RESDO	40
#define	VLDH_CLASS_RESDV	41
#define	VLDH_CLASS_STODP	42
#define	VLDH_CLASS_STOAP	43
#define	VLDH_CLASS_GETPI	44
#define	VLDH_CLASS_SETDP	45
#define	VLDH_CLASS_RESDP	46
#define	VLDH_CLASS_RTNODE	47
#define	VLDH_CLASS_PLCPGM	48
#define	VLDH_CLASS_PLANTHIER	49
#define	VLDH_CLASS_WINDOWPLC	50
#define	VLDH_CLASS_WINDOWCOND	51
#define	VLDH_CLASS_WINDOWORDERACT 52
#define	VLDH_CLASS_RESET_SO 	53
#define	VLDH_CLASS_AANALYSE 	54
#define	VLDH_CLASS_DANALYSE 	55
#define	VLDH_CLASS_CONTRACE 	56
#define	VLDH_CLASS_CSTOAO	57
#define	VLDH_CLASS_CSTOAV	58
#define	VLDH_CLASS_CSTOAP	59
#define	VLDH_CLASS_NODEHIER	60
#define	VLDH_CLASS_POS3P	61
#define	VLDH_CLASS_INC3P	62
#define	VLDH_CLASS_WINDOWSUBSTEP 63
#define	VLDH_CLASS_STOIP	64
#define	VLDH_CLASS_CSTOIP	65
#define	VLDH_CLASS_GETIP	66
#define	VLDH_CLASS_EXTERNREF	67
#define	VLDH_CLASS_TEXT		68
#define	VLDH_CLASS_LIBHIER	69
#define VLDH_CLASS_CONDATA	70
#define	VLDH_CLASS_PO		71
#define	VLDH_CLASS_STODI	72
#define	VLDH_CLASS_SETDI	73
#define	VLDH_CLASS_RESDI	74
#define	VLDH_CLASS_GETDATA	75
#define	VLDH_CLASS_PLCTHREAD	76
#define VLDH_CLASS_STEPDIV	77
#define VLDH_CLASS_STEPCONV	78
#define VLDH_CLASS_TRANSDIV	79
#define VLDH_CLASS_TRANSCONV	80

#define MAXNO_NODES	500
#define MAX_OBJECTTYPES	300
#define VLDH_NODE_SOURCE	1
#define VLDH_NODE_DESTINATION	2
#define	VLDH_MAX_CONPOINTS	64
#define	VLDH_MAX_SUBWINDOWS	2

/* object type in vldh */
#define VLDH_NODE	0
#define VLDH_CON	1
#define VLDH_WIND	3
#define VLDH_PLC	4

/* Compilation directions */
#define	VLDH_HORIZONTAL	0
#define	VLDH_VERTICAL	1

/* Object status */
#define	VLDH_DELETE	1	/* Deleted in vldh during this session */
#define	VLDH_CREATE	2	/* Created in this session */
#define	VLDH_LOAD	4	/* Loaded from ldh or saved during this sess */
#define	VLDH_MODIFY	8	/* Modified in this session */
#define	VLDH_LDHDELETE	16	/* Deleted in ldh during this session */

/* Macros called when the ldh part of the vldhobject is modified */
#define vldh_nodemodified(node) (node)->hn.status |= VLDH_MODIFY
#define vldh_conmodified(con) (con)->hc.status |= VLDH_MODIFY
#define vldh_windmodified(wind) (wind)->hw.status |= VLDH_MODIFY
#define vldh_plcmodified(plc) (plc)->hp.status |= VLDH_MODIFY

#define	VLDH_PASTEMODE_OBJECT		0
#define	VLDH_PASTEMODE_AREA		1


/* struct for koordinates */
typedef struct {
	float	x;
	float	y;
	} vldh_point;

/* vldh window object */
typedef	struct	{
  struct 	{
    int			status; 	/* deleted , created, loaded etc */
    char		object_name[32];	/* ldh segment name */
    char		full_name[80];		/* ldh hierarchy name */
    Widget		widget_id;		/* ? */
    struct nodetag   	*node_list_pointer;	/* pointer to nodelist */
    struct contag 	*con_list_pointer;	/* pointer to connection list*/
    struct nodetag  	*parent_node_pointer;	/* zero if parent is a plc obj*/
    struct plc_objtag   *plcobject_pointer;	/* always defined */
    void		*foectx;
    ldh_tSesContext	ldhsession;
  } hw;
/*  struct	{
    pwr_tObjid		objdid;
    unsigned long	object_type;
    pwr_tClassId	class;
    float		x;	* widget coord not yet implemented *
    float		y;	* widget coord not yet implemented *
    float		width;	* widget geom.. not yet implemented *
    float		height; * widget geom.. not yet implemented * 
    pwr_tObjid	 	parent_node_did;
    unsigned int	subwindowindex;		* not used for a plcsubwind * 
  } lw;
*/
  pwr_sPlcWindow	lw;
}	vldh_t_windobject, *vldh_t_wind;

typedef	struct	plc_objtag { 
  struct 	{
    int			status;		/* deleted , created, loaded etc */
    struct plc_objtag	*plcobj_pointer;	/* link to next plcobject in vldh */
    vldh_t_windobject 	*windowobject;		/* if zero wind not loaded */
    char		object_name[80];	
    ldh_tSesContext	ldhsesctx;		/* ldh session ctx from hied */
    ldh_tWBContext	ldhwbctx;		/* workbench ctx from hied */
    void		*hinactx;		/* navigator ctx */
  } hp;
  pwr_sPlcProgram	lp;
}	vldh_t_plcobject, *vldh_t_plc;

typedef	struct nodetag	{ 
  struct	{ 
    unsigned int	vldhtype;		/* node or con, first position!*/
    char		name[32];		/* segment name */
    flow_tNode		node_id;		/* flow node id */
    int			status;			/* created, loaded... */
    vldh_t_windobject 	*window_pointer;
    struct nodetag	*object_pointer;	/* pointer to next node */
    vldh_t_windobject 	*subwindowobject[VLDH_MAX_SUBWINDOWS]; /* pointer to 
						  subwindow in vldh */
    unsigned long	con_count[VLDH_MAX_CONPOINTS];	/* nr of cons to every
							conpoint in vldh */
    struct contag	**con_list[VLDH_MAX_CONPOINTS];	/* pointer to cons to
							every conpoint */
    unsigned long	executeorder;		
    pwr_tSubid		trace_subid;
  } hn;
/*  struct	{ 
    unsigned long	object_type;
    pwr_tClassId	class;
    pwr_tObjid		object_did;
    float		x;
    float		y;
    float		width;
    float		height;
    pwr_tObjid	window_did;
    pwr_tObjid	subwind_objdid[2];
    char		subwindow;
    char		graphtype;
    unsigned long	mask[3];
    unsigned long	compdirection;
    int			nodewidth;
  } ln;
*/
  pwr_sPlcNode		ln;
  struct	{ 
    char		annot[3][80]; 	/* not in use */
  } an;
}	vldh_t_nodeobject, *vldh_t_node;

typedef	struct contag	{ 
	struct	{ 
    	  int		vldhtype;		/* node or con, first position!*/
	  int		status;
	  vldh_t_node	source_node_pointer;
	  vldh_t_windobject *window_pointer; 	/* vldh */
	  char		name[32];		/* segment name */
	  flow_tCon	con_id;			/* flow */
	  vldh_t_node	dest_node_pointer;	/* vldh */
	  struct contag	*object_pointer;	/* next connection obj vldh */
	  int		redrawn;		/* flag for redrawing refcons */
	  pwr_tSubid	trace_subid;
		} hc;
/*	struct	{ 
			pwr_tObjid	objdid;
			unsigned long	object_type;
			pwr_tClassId	class;
			unsigned long	curvature;
			unsigned long	drawtype;
			unsigned long	attributes;
			unsigned long	refnr;
			unsigned long	source_point;
			pwr_tObjid	source_node_did;
			unsigned long	dest_point;
			pwr_tObjid	dest_node_did;
			unsigned long	point_count;
			vldh_point	point[10];
			pwr_tObjid	window_did;
		} lc;
*/
  pwr_sPlcConnection	lc;
		}	vldh_t_conobject, *vldh_t_con;

typedef	struct	{
		vldh_t_node	node;
		unsigned long	conpoint;
		} 	vldh_t_conpoint; 




unsigned long vldh_check_document ( 
  ldh_tSesContext ldhses,
  pwr_tObjid objdid
);

unsigned long vldh_check_plcpgm (
  ldh_tSesContext ldhses,
  pwr_tObjid objdid
);

pwr_tClassId	vldh_class ( 
  ldh_tSesContext ldhses,
  int index
);

pwr_tClassId	vldh_uclass (
  ldh_tSesContext ldhses,
  char	*classname
);

pwr_tClassId	vldh_eclass (
  ldh_tSesContext ldhses,
  char	*classname
);

int vldh_node_con_insert (
  vldh_t_node	node,
  unsigned long	con_point,
  vldh_t_con	con,
  int		type
);

int vldh_node_con_delete (
  vldh_t_node	node,
  unsigned long	con_point,
  vldh_t_con	con
);

int vldh_node_subwindow_created (
  vldh_t_node	node,
  vldh_t_wind	subwind,
  unsigned long	windowindex
);

int vldh_node_delete (
  vldh_t_node	node
);

int vldh_node_undelete (
  vldh_t_node	node
);

int vldh_con_create (
  vldh_t_wind	wind,
  pwr_tClassId	classid,
  unsigned long	drawtype,
  vldh_t_node	source_node,
  unsigned long	source_point,
  vldh_t_node	dest_node,
  unsigned long	dest_point,
  vldh_t_con	*con
);

int vldh_con_load (
  vldh_t_wind	wind,
  pwr_tObjid objdid
);

int vldh_con_delete (
  vldh_t_con	con
);

int vldh_con_undelete (
  vldh_t_con	con
);

int vldh_get_con_nodes (
  vldh_t_con	con,
  vldh_t_node	*source_node,
  vldh_t_node	*dest_node
);

int vldh_con_getrefnr (
  vldh_t_con	con
);

int vldh_wind_quit (
  vldh_t_wind	wind
);

int vldh_wind_quit_all (
  vldh_t_wind	wind
);

int vldh_wind_save (
  vldh_t_wind	wind
);

int vldh_wind_delete_all (
  vldh_t_wind	wind
);

int vldh_wind_load_all (
  vldh_t_wind wind
);

int vldh_wind_create ( 
  vldh_t_plc	plc,
  vldh_t_node	parentnode,
  void	*foectx,
  pwr_tClassId	classid,
  Widget	widget_id,
  unsigned long	subwindowindex,
  vldh_t_wind	*wind,
  ldh_eAccess	access
);

int vldh_wind_load (
  vldh_t_plc	plc,
  vldh_t_node	parentnode,
  pwr_tObjid 	objdid,
  void		*foectx,
  Widget	widget_id,
  vldh_t_wind	*wind,
  ldh_eAccess	access
);

int vldh_wind_delete (
  vldh_t_wind	wind
);

int vldh_plc_create (
  pwr_tObjid	  objdid,
  ldh_tWBContext  ldhwbctx,
  ldh_tSesContext ldhsesctx,
  vldh_t_plc	*plc
);

int vldh_plc_load (
  pwr_tObjid	  objdid,
  ldh_tWBContext  ldhwbctx,
  ldh_tSesContext ldhsesctx,
  vldh_t_plc	*plc
);

int vldh_plc_delete (
  vldh_t_plc	plc
);

int vldh_get_wind_subwindows ( 
  vldh_t_wind		wind,
  unsigned long		*wind_count,
  vldh_t_wind		**intern_windlist
);

int vldh_get_subwindows_all ( 
  vldh_t_wind		wind,
  unsigned long		*wind_count,
  vldh_t_wind		**intern_windlist
);

int vldh_get_wind_windows (
  vldh_t_wind		wind,
  unsigned long		*wind_count,
  vldh_t_wind		**intern_windlist
);

int vldh_get_plc_windows (
  vldh_t_plc		plc,
  unsigned long		*wind_count,
  vldh_t_wind		**intern_windlist
);

int vldh_get_nodes (
  vldh_t_wind		wind,
  unsigned long		*node_count,
  vldh_t_node		**intern_nodelist
);

int vldh_get_nodes_del (
  vldh_t_wind		wind,
  unsigned long		*node_count,
  vldh_t_node           **intern_nodelist
);

int vldh_get_nodes_class (
  vldh_t_wind		wind,
  unsigned long		object_type,
  unsigned long		*node_count,
  vldh_t_node		**intern_nodelist
);

int vldh_get_cons (
  vldh_t_wind		wind,
  unsigned long		*con_count,
  vldh_t_con		**intern_conlist
);

int vldh_get_cons_del ( 
  vldh_t_wind		wind,
  unsigned long		*con_count,
  vldh_t_con		**intern_conlist
);

int vldh_get_conpoints_next (
  vldh_t_node		node,
  unsigned long		point,
  unsigned long		*point_count,
  vldh_t_conpoint	**pointlist,
  unsigned long		attributemask
);

int vldh_get_conpoint_nodes ( 
  vldh_t_node		node,
  unsigned long		point,
  unsigned long		*point_count,
  vldh_t_conpoint	**pointlist,
  unsigned long		attributemask
);

int vldh_get_conpoint_nodes_close (
  vldh_t_node		node,
  unsigned long		point,
  unsigned long		*point_count,
  vldh_t_conpoint	**pointlist,
  unsigned long		attributemask
);

int vldh_get_conpoint_cons (
  vldh_t_node		node,
  unsigned long		point,
  unsigned long		*con_count,
  vldh_t_con		**intern_conlist
);

int vldh_get_nodes_direction (
  vldh_t_wind		wind,
  unsigned long		*node_count_horizontal,
  vldh_t_node		**nodelist_horizontal,
  unsigned long		*node_count_vertical,
  vldh_t_node		**nodelist_vertical
);

int vldh_get_cons_node (
  vldh_t_node		node,
  unsigned long		*con_count,
  vldh_t_con		**intern_conlist
);

int vldh_get_nodes_node (
  vldh_t_node		node,
  unsigned long		*node_count,
  vldh_t_node		**intern_nodelist,
  unsigned long		conmask,
  unsigned long		nodemask
);

int vldh_get_plc_objdid (
  pwr_tObjid objdid,
  vldh_t_plc	*plc
);

int vldh_get_plcs (
  unsigned long		*plc_count,
  vldh_t_plc		**plclist
);

int vldh_get_node_objdid (
  pwr_tObjid		objdid,
  vldh_t_wind		wind,
  vldh_t_node		*node
);

int vldh_get_wind_objdid (
  pwr_tObjid	objdid,
  vldh_t_wind	*wind
);







int vldh_paste_setmode ( 
  int mode
);

int vldh_paste_getmode (
);

int vldh_paste_setrectinfo (
  void	*ctx,
  float	x,
  float	y,
  float	width,
  float	height
);

int vldh_paste_getrectinfo (
  void	**ctx,
  float	*x,
  float	*y,
  float	*width,
  float	*height
);

int vldh_paste_singlenode_insert (
  vldh_t_wind	send_window,
  vldh_t_node	send_node
);

int vldh_paste_node_insert (
  vldh_t_wind	send_window,
  vldh_t_node	send_node
);

int vldh_paste_con_insert (
  vldh_t_wind	send_window,
  vldh_t_con	send_con
);

int vldh_paste_init ( 
);

int vldh_paste_singlenode_copy (
  vldh_t_wind	wind,
  vldh_t_node	*node
);

int vldh_paste_copy (
  vldh_t_wind	wind,
  float		x,
  float		y,
  unsigned long	*node_count,
  vldh_t_node	**intern_nodelist,
  unsigned long	*con_count,
  vldh_t_con	**intern_conlist
);

int vldh_node_update_spec (
  vldh_t_node	node
);

int vldh_object_update_spec (
  ldh_tSesContext ldhses,
  pwr_tObjid Objdid
);

int vldh_node_create (
  vldh_t_wind	wind,
  pwr_tClassId	classid,
  vldh_t_node	*node
);

int vldh_node_create_spec (
  vldh_t_node	node
);

int vldh_node_ldhdel_spec (
  vldh_t_node	node
);

int vldh_get_node_coordinates ( 
  vldh_t_wind		wind,
  float			x,
  float			y,
  vldh_t_node		*node
);

int vldh_get_wind_modification (
  vldh_t_wind		wind,
  int			*modified
);

char	*vldh_IdToStr(
  int		idx,
  pwr_tObjid 	objid
);

char *vldh_VolumeIdToStr( 
  pwr_tVolumeId volumeid
);

pwr_tStatus vldh_StrToId( 
  char 		*instr, 
  pwr_tObjid 	*objid
);

#endif
