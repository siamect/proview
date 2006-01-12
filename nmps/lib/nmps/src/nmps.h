/* 
 * Proview   $Id: nmps.h,v 1.1 2006-01-12 05:57:43 claes Exp $
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



#define	NMPS_CELL_SIZE			30	/* Number of  dataobject in
			 			   NMpsCell och NMpsSelectCell */
#define	NMPS_CELLMIR_SIZE		30	/* Number of dataobject in 
						   NMpsMirrorCell */
#define MMPS_CELLMIR_ORIGCELL_SIZE	10	/* Size of the origcell-array
						   in NMpsMirrorCell */
#define	NMPS_OPTYPE_EXTINSERT		0
#define	NMPS_OPTYPE_EXTINSERT_IDX	1
#define	NMPS_OPTYPE_EXTDELETE		2
#define	NMPS_OPTYPE_EXTDELETE_IDX	3
#define	NMPS_OPTYPE_EXTDELETE_OBJID	4
#define	NMPS_OPTYPE_EXTSELECT_OBJID	5
#define	NMPS_OPTYPE_EXTINSERTSELECT	6
#define	NMPS_OPTYPE_EXTUNSELECT_OBJID	7
#define	NMPS_OPTYPE_EXTMOVEFORW_OBJID	8
#define	NMPS_OPTYPE_EXTMOVEBACKW_OBJID	9
#define	NMPS_OPTYPE_EXTREMOVE_OBJID	10

#define	NMPS_CELL_RELOADDONE		1
#define	NMPS_CELL_INITIALIZED		2

#define	NMPS_CELLFUNC_SINGELSELECT	1
#define	NMPS_CELLFUNC_ONESELECT		2
#define	NMPS_CELLFUNC_DELETE		4
#define	NMPS_CELLFUNC_RESETDELETE	8
#define	NMPS_CELLFUNC_BACKUP		16

#define	NMPS_TRPFUNC_UNIT		1
#define	NMPS_TRPFUNC_ALL		2
#define	NMPS_TRPFUNC_ALLEMPTY		4
#define	NMPS_TRPFUNC_TRIGGRESET		8
#define NMPS_TRPFUNC_SLOW               16
#define NMPS_TRPFUNC_LEVEL              32
 
/* Data structure for one dataobjekt in a NMpsCell and NMpsSelectCell */
typedef struct {
	pwr_tFloat32	*DataP;
	pwr_tObjid	Data_ObjId;
	pwr_tBoolean	Data_Front;
	pwr_tBoolean	Data_Back;
	gdh_tDlid	Data_Dlid;	
	pwr_tBoolean	Data_Select;
	pwr_tBoolean	Data_OldSelect;
	} plc_t_DataInfo;

/* Data structure for the DLpointer and the objid in a Cell */
typedef struct {
	pwr_tFloat32	*DataP;
	pwr_tObjid	Data_ObjId;
	} plc_t_DataInfoBrief;

/* Data structure for one dataobjekt in a NMpsMirrorCell */
typedef struct {
	pwr_tFloat32	*DataP;
	pwr_tObjid	Data_ObjId;
	gdh_tDlid	Data_Dlid;	
	} plc_t_DataInfoMirCell;

/* Data structure for the copyarea in a  NMpsMirrorCell */
typedef struct {
	plc_t_DataInfoMirCell	datainfo[ NMPS_CELLMIR_SIZE];
	} nmps_t_mircell_copyarea;

int	nmps_RemoveData( pwr_tObjid objid);
int	nmps_RemoveAndDeleteData( pwr_tObjid objid);
