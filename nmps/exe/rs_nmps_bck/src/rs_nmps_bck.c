/*************************************************************************
*
*		SSAB OXELÖSUND
*		==============
*************************************************************************
*
* 	PROGRAM		rs_nmps_bck
*	SYSTEM		SSAB 
*
*       Modifierad
*		960205	Claes Sjöfors	Skapad
*
*
*
*	Funktion:	Backup av NMpsCell objekt och data objekt.
*
*
*
**************************************************************************
**************************************************************************/

/*_Include filer_________________________________________________________*/

#ifdef OS_VMS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <float.h>
#include <libdef.h>
#include <starlet.h>
#include <lib$routines.h>
#endif

#if defined OS_LYNX || defined OS_LINUX
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <float.h>
#endif

#ifdef OS_ELN
#include stdio
#include stdlib
#include string
#include math
#include float
#include libdef
#include starlet
#include lib$routines
#endif

#include "pwr.h"
#include "pwr_baseclasses.h"
#include "pwr_nmpsclasses.h"
#include "co_cdh.h"
#include "co_time.h"
#include "co_dcli.h"
#include "rt_gdh.h"
#include "rt_errh.h"
#include "rt_gdh_msg.h"
#include "rt_hash_msg.h"
#include "rs_nmps.h"
#include "rs_nmps_msg.h"
#include "rs_sutl.h"
#include "rt_qcom.h"
#include "rt_qcom_msg.h"
#include "rt_ini_event.h"

/*_Globala variabler______________________________________________________*/

/*_Local functions________________________________________________________*/
/* Global functions________________________________________________________*/

#define	NMPS_BCK_FILEVERSION		1
#define	NMPSBCK_MAX_RECORDS		500

#define NMPSBCK_FILE_EXT1	".bck1"
#define NMPSBCK_FILE_EXT2	".bck2"

#define ODD(a)	(((int)(a) & 1) != 0)
#define EVEN(a)	(((int)(a) & 1) == 0)
#define max(Dragon,Eagle) ((Dragon) > (Eagle) ? (Dragon) : (Eagle))
#define min(Dragon,Eagle) ((Dragon) < (Eagle) ? (Dragon) : (Eagle))
#ifndef __ALPHA
#define abs(Dragon) ((Dragon) >= 0 ? (Dragon) : (-(Dragon)))
#endif
#define	LogAndExit( status) \
{\
  errh_CErrLog(NMPS__BCKEXIT, errh_ErrArgMsg(status), NULL);\
  exit( status);\
}

#define	LogAndReturn( status1, status2) \
{\
  errh_CErrLog(status1, errh_ErrArgMsg(status2), NULL);\
  return status2;\
}

#define	Log( status1, status2) \
{\
  errh_CErrLog(status1, errh_ErrArgMsg(status2), NULL);\
}

typedef struct {
	pwr_tTime			creationtime;
	int				version;
	} nmpsbck_t_fileheader;

typedef struct {
	pwr_tTime			time;
	char				type[32];
	} nmpsbck_t_recordheader;

typedef struct {
	char				type[32];
	pwr_tObjid			objid;
	pwr_tClassId			class;
	int				size;
	int				last;
	} nmpsbck_t_cellheader;

typedef struct {
	char				type[32];
	pwr_tObjid			objid;
	char				data_name[120];
	pwr_tClassId			class;
	int				size;
	int				last;
	} nmpsbck_t_dataheader;

#define HEADERTYPE_RECORDSTART	"Headertype record start"
#define HEADERTYPE_RECORDEND	"Headertype record end"
#define HEADERTYPE_CELL		"Headertype cell"
#define HEADERTYPE_CELLEND	"Headertype cell end"
#define HEADERTYPE_DATA		"Headertype data"
#define HEADERTYPE_DATAEND	"Headertype data end"

typedef struct nmpsbck_clist_tag{
	pwr_tObjid			objid;
	pwr_tClassId			class;
	char				*buffer;
	int				buffer_size;
	char				*objectp;
	unsigned int			object_size;
	struct nmpsbck_clist_tag	*next_ptr;
	} nmpsbck_t_clist;

typedef struct nmpsbck_data_tag{
	pwr_tObjid			objid;
	pwr_tObjid			old_objid;
	pwr_tClassId			class;
	int				size;
	char				data_name[120];
	char				*data_ptr;
	gdh_tDlid			data_subid;
	unsigned char			found;
	unsigned char			new;
	unsigned char			created;
	struct nmpsbck_data_tag		*next_ptr;
	struct nmpsbck_data_tag		*prev_ptr;
	} nmpsbck_t_data_list;

typedef struct nmpsbck_cell_tag {
	pwr_tObjid		objid;
	pwr_tClassId		class;
	pwr_sClass_NMpsMirrorCell *cell;
	gdh_tDlid		subid;
	int			size;
	int			backup_now;
	struct nmpsbck_cell_tag	*next_ptr;
	} nmpsbck_t_cell_list;

typedef struct {
	int				CellObjectCount;
	int				DataObjectCount;
	pwr_tUInt32			LoopCount;
	pwr_sClass_NMpsBackupConfig	*bckconfig;
	gdh_tDlid			bckconfig_dlid;
	nmpsbck_t_cell_list		*cellist;
	int				cell_count;
	nmpsbck_t_data_list		*data_list;
	int				init_done;
	char				*buffer;
	int				buffer_size;
	int				cellist_size;
	FILE				*bckfile1;
	FILE				*bckfile2;
	int				file1_pos;
	int				file2_pos;
	int				file_num;
	int				increment;
	int				record_count;
	} *bck_ctx;

static pwr_tStatus	nmpsbck_data_handler( bck_ctx	bckctx);
static pwr_tStatus	nmpsbck_set_cell_backup_done();
static pwr_tStatus	nmpsbck_data_db_delete( 
			nmpsbck_t_data_list	**data_list, 
			nmpsbck_t_data_list	*data_ptr);
static pwr_tStatus	nmpsbck_data_db_create(  
			bck_ctx			bckctx,
			pwr_tObjid		objid,
			nmpsbck_t_data_list	**datalist_ptr);
static pwr_tStatus	nmpsbck_data_db_find(  
			nmpsbck_t_data_list	*data_list,
			pwr_tObjid		objid,
			nmpsbck_t_data_list	**datalist_ptr);
static pwr_tStatus	nmpsbck_cellist_add( 
			bck_ctx			bckctx,
			pwr_tObjid 		objid, 
			nmpsbck_t_cell_list 	**cellist,
			int			*cellist_count);
static pwr_tStatus	nmps_get_bckconfig( bck_ctx	bckctx);
static pwr_tStatus	nmps_cell_init( bck_ctx	bckctx);
static pwr_tStatus	nmpsbck_get_cellbuffer( bck_ctx	bckctx);
static pwr_tStatus	nmpsbck_fill_buffer( 	bck_ctx	bckctx);
static pwr_tStatus	nmpsbck_write_cells( 	bck_ctx	bckctx,
						FILE	*bckfile);
static pwr_tStatus	nmpsbck_write_data( 	bck_ctx	bckctx,
						FILE	*bckfile);
static pwr_tStatus	nmpsbck_read( 		bck_ctx	bckctx);
static pwr_tStatus	nmpsbck_cell_handler( bck_ctx	bckctx);
static pwr_tStatus	nmpsbck_free( bck_ctx bckctx);

/*************************************************************************
*
* Name:		nmps_fgetname()
*
* Type		int
*
* Type		Parameter	IOGF	Description
*
* Description:
*	Get filename for a filedescriptor.
*	This function is not implementet on all os, therefor a defaultname
*	should be supplied which is returned for this os.
*
**************************************************************************/
static char	*nmps_fgetname( FILE *fp, char *name, char *def_name)
{
#if defined(OS_VMS) || defined(OS_ELN)
	return fgetname( fp, name);
#else
	strcpy( name, def_name);
	return name;
#endif	
}

/*************************************************************************
*
* Name:		nmpsbck_get_filename
*
* Typ		int
*
* Typ		Parameter	IOGF	Beskrivning
*
* Beskrivning: 
*	Backup of data objects.
*
**************************************************************************/
int	nmpsbck_get_filename(
			char	*inname,
			char	*outname,
			char	*ext)
{
	char	*s;
	char	*s2;

	strcpy( outname, inname);

	/* Look for extention in filename */
	if ( ext != NULL)
	{
	  s = strrchr( outname, ':');
	  if ( s == 0)
	    s = outname;

	  s2 = strrchr( s, '>');
	  if ( s2 == 0)
	  {
	    s2 = strrchr( s, ']');
	    if ( s2 == 0)
	      s2 = s;
	  }

	  s = strrchr( s2, '.');
	  if ( s != 0)
	    *s = 0;
	  strcat( outname, ext);
	}
	dcli_translate_filename( outname, outname);
	return NMPS__SUCCESS;
}

/*************************************************************************
*
* Name:		nmpsbck_data_handler
*
* Typ		int
*
* Typ		Parameter	IOGF	Beskrivning
*
* Beskrivning: 
*	Backup of data objects.
*
**************************************************************************/

static pwr_tStatus	nmpsbck_data_handler( bck_ctx	bckctx)
{
	nmpsbck_t_data_list	*data_ptr;
	nmpsbck_t_data_list	*next_ptr;
	pwr_tStatus		sts;
	int			data_count;
	int			offset;
	int			i, k;
	nmpsbck_t_cellheader	*cellheader;

	/* Reset the found and new flag in all data entries */
	data_ptr = bckctx->data_list;
	while ( data_ptr != NULL)
	{
	  data_ptr->found = 0;
	  data_ptr = data_ptr->next_ptr;
	}

	/* Loop trough the cells to identify the new ones */
	offset = 0;
	for ( i = 0; i < bckctx->cell_count; i++)
	{
	  cellheader = (nmpsbck_t_cellheader *) (bckctx->buffer + offset);
	  offset += sizeof( *cellheader);
	  switch( cellheader->class)
	  {
	    case pwr_cClass_NMpsCell:
	    case pwr_cClass_NMpsStoreCell:
	    {
	      pwr_sClass_NMpsCell 	*cell_ptr;
	      plc_t_DataInfo		*data_block_ptr;

	      cell_ptr = (pwr_sClass_NMpsCell *) (bckctx->buffer + offset);
	      data_block_ptr = (plc_t_DataInfo *) &cell_ptr->Data1P;
	      for ( k = 0; k < cell_ptr->LastIndex; k++)
	      {
	        /* Check if the objid already is in the data_db */
	        sts = nmpsbck_data_db_find(  bckctx->data_list,
			data_block_ptr->Data_ObjId, &data_ptr);
	        if ( ODD(sts))
	          data_ptr->found = 1;
	        else
	        {
	          /* New data object, insert it */
		  sts = nmpsbck_data_db_create( bckctx, 
			data_block_ptr->Data_ObjId, &data_ptr);
	          if ( EVEN(sts)) return sts;
	          data_ptr->found = 1;
	          data_ptr->new = 1;
	        }
	        data_block_ptr++;
	      }
	      break;
	    }
	    case pwr_cClass_NMpsMirrorCell:
	    {
	      pwr_sClass_NMpsMirrorCell *cell_ptr;
	      plc_t_DataInfoMirCell	*data_block_ptr;

	      cell_ptr = (pwr_sClass_NMpsMirrorCell *) 
			(bckctx->buffer + offset);
	      data_block_ptr = (plc_t_DataInfoMirCell *) &cell_ptr->Data1P;
	      for ( k = 0; k < cell_ptr->LastIndex; k++)
	      {
	        /* Check if the objid already is in the data_db */
	        sts = nmpsbck_data_db_find(  bckctx->data_list,
			data_block_ptr->Data_ObjId, &data_ptr);
	        if ( ODD(sts))
	          data_ptr->found = 1;
	        else
	        {
	          /* New data object, insert it */
		  sts = nmpsbck_data_db_create( bckctx,
			data_block_ptr->Data_ObjId, &data_ptr);
	          if ( EVEN(sts))
	          {
	            /* The object does not exist... */
	            continue;
	          }
	          data_ptr->found = 1;
	          data_ptr->new = 1;
	        }
	        data_block_ptr++;
	      }
	      break;
	    }
	  }
	  offset += cellheader->size;
	}

	/* Remove data objects not found in a cell */
	data_count = 0;
	data_ptr = bckctx->data_list;
	while ( data_ptr != NULL)
	{
	  if ( !data_ptr->found)
	  {
	    /* Remove subscriptions */
	    sts = gdh_DLUnrefObjectInfo( data_ptr->data_subid);
	    if (EVEN(sts)) Log(NMPS__MIRUNREF, sts);

	    /* Delete the data entry */
	    next_ptr = data_ptr->next_ptr;
	    sts = nmpsbck_data_db_delete( &bckctx->data_list, data_ptr);
	    data_ptr = next_ptr;
	    continue;
	  }
	  else
	    data_count++;
	  data_ptr = data_ptr->next_ptr;
	}
	bckctx->bckconfig->DataCount = data_count;
	return NMPS__SUCCESS;
}


/****************************************************************************
* Name:		nmpsbck_data_db_delete()
*
* Type		pwr_tStatus
*
* Type		Parameter	IOGF	Description
*
* Description:
*		Delete an entry in the datalist.
*
**************************************************************************/
static pwr_tStatus	nmpsbck_data_db_delete( 
			nmpsbck_t_data_list	**data_list, 
			nmpsbck_t_data_list	*data_ptr)
{
	if ( data_ptr == *data_list)
	{
	  /* Change the root */
	  *data_list = data_ptr->next_ptr;
	  if ( *data_list)
	    (*data_list)->prev_ptr = 0;
	}

	if ( data_ptr->prev_ptr )
	  data_ptr->prev_ptr->next_ptr = data_ptr->next_ptr;
	if ( data_ptr->next_ptr )
	  data_ptr->next_ptr->prev_ptr = data_ptr->prev_ptr;

	free( data_ptr);

	return NMPS__SUCCESS;
}


/****************************************************************************
* Name:		nmpsbck_data_db_create()
*
* Type		pwr_tStatus
*
* Type		Parameter	IOGF	Description
*
* Description:
*		Create an entry in the datalist for an objid.
*
**************************************************************************/
static pwr_tStatus	nmpsbck_data_db_create(  
			bck_ctx			bckctx,
			pwr_tObjid		objid,
			nmpsbck_t_data_list	**datalist_ptr)
{
	nmpsbck_t_data_list	*next_ptr;
	pwr_tStatus		sts;
	pwr_sAttrRef		attrref;
	pwr_tClassId		class;
	unsigned int		size;
	char			data_name[120];
	char			*data_ptr;
	gdh_tDlid		data_subid;

	sts = gdh_GetObjectClass ( objid, &class);
	if (EVEN(sts)) LogAndReturn(NMPS__BCKDATA, sts);
	sts = gdh_GetObjectSize ( objid, &size);
	if (EVEN(sts)) LogAndReturn(NMPS__BCKDATA, sts);
	sts = gdh_ObjidToName ( objid, data_name, sizeof(data_name), 
			cdh_mName_volumeStrict);
	if (EVEN(sts)) LogAndReturn(NMPS__BCKDATA, sts);

	/* Get a direct link to the object */
	memset( &attrref, 0, sizeof(attrref));
	attrref.Objid = objid;
	sts = gdh_DLRefObjectInfoAttrref ( &attrref,
		(pwr_tAddress *) &data_ptr, &data_subid);
	if (EVEN(sts)) LogAndReturn(NMPS__BCKDATA, sts);

	*datalist_ptr = calloc( 1, sizeof( nmpsbck_t_data_list));
	if ( *datalist_ptr == 0) return NMPS__NOMEMORY;

	/* Insert first in list */
	next_ptr = bckctx->data_list;
	if ( next_ptr != NULL)
	  next_ptr->prev_ptr = *datalist_ptr;
	(*datalist_ptr)->next_ptr = next_ptr;

	bckctx->data_list = *datalist_ptr;

	(*datalist_ptr)->objid = objid;
	(*datalist_ptr)->class = class;
	(*datalist_ptr)->size = size;
	strcpy( (*datalist_ptr)->data_name, data_name);
	(*datalist_ptr)->data_ptr = data_ptr;
	(*datalist_ptr)->data_subid = data_subid;

	return NMPS__SUCCESS;
}


/****************************************************************************
* Name:		nmpsbck_data_db_find()
*
* Type		pwr_tStatus
*
* Type		Parameter	IOGF	Description
*
* Description:
*		Find an entry in the datalist for an objid.
*
**************************************************************************/
static pwr_tStatus	nmpsbck_data_db_find(  
			nmpsbck_t_data_list	*data_list,
			pwr_tObjid		objid,
			nmpsbck_t_data_list	**datalist_ptr)
{
	nmpsbck_t_data_list	*data_ptr;
	int			found;

	/* Insert first in list */
	found = 0;
	data_ptr = data_list;
	while ( data_ptr != NULL)
	{
	  if ( cdh_ObjidIsEqual( data_ptr->objid, objid))
	  {
	    *datalist_ptr = data_ptr;
	    found = 1;
	    break;
	  }
	  data_ptr = data_ptr->next_ptr;
	}
	
	if ( !found)
	  return NMPS__DATANOTFOUND;

	return NMPS__SUCCESS;
}


/****************************************************************************
* Name:		nmpsbck_data_db_find_old()
*
* Type		pwr_tStatus
*
* Type		Parameter	IOGF	Description
*
* Description:
*		Find an entry in the datalist for an old objid.
*
**************************************************************************/
static pwr_tStatus	nmpsbck_data_db_find_old(  
			nmpsbck_t_data_list	*data_list,
			pwr_tObjid		old_objid,
			nmpsbck_t_data_list	**datalist_ptr)
{
	nmpsbck_t_data_list	*data_ptr;
	int			found;

	/* Insert first in list */
	found = 0;
	data_ptr = data_list;
	while ( data_ptr != NULL)
	{
	  if ( cdh_ObjidIsEqual( data_ptr->old_objid, old_objid))
	  {
	    *datalist_ptr = data_ptr;
	    found = 1;
	    break;
	  }
	  data_ptr = data_ptr->next_ptr;
	}
	
	if ( !found)
	  return NMPS__DATANOTFOUND;

	return NMPS__SUCCESS;
}


/****************************************************************************
* Name:		nmpsbck_data_db_create()
*
* Type		pwr_tStatus
*
* Type		Parameter	IOGF	Description
*
* Description:
*		Create an entry in the datalist for an objid.
*
**************************************************************************/
static pwr_tStatus	nmpsbck_clist_store(
			nmpsbck_t_clist		**clist,
			pwr_tObjid		objid,
			nmpsbck_t_clist		**clist_ptr)
{
	*clist_ptr = calloc( 1, sizeof( nmpsbck_t_clist));
	if ( clist_ptr == 0) return NMPS__NOMEMORY;

	/* Insert first in list */
	(*clist_ptr)->next_ptr = *clist;

	*clist = *clist_ptr;
	(*clist_ptr)->objid = objid;
	return NMPS__SUCCESS;
}


/****************************************************************************
* Name:		nmpsbck_clist_find()
*
* Type		pwr_tStatus
*
* Type		Parameter	IOGF	Description
*
* Description:
*		Find an entry in clist.
*
**************************************************************************/
static pwr_tStatus	nmpsbck_clist_find(  
				nmpsbck_t_clist		*clist,
				pwr_tObjid		objid,
				nmpsbck_t_clist		**clist_ptr)
{
	while ( clist != NULL)
	{
	  if ( cdh_ObjidIsEqual( clist->objid, objid))
	  {
	    *clist_ptr = clist;
	    return NMPS__SUCCESS;
	  }
	  clist = clist->next_ptr;
	}
	return NMPS__DATANOTFOUND;
}


/****************************************************************************
* Name:		nmpsbck_clist_free()
*
* Type		pwr_tStatus
*
* Type		Parameter	IOGF	Description
*
* Description:
*		Free the clist.
*
**************************************************************************/
static pwr_tStatus	nmpsbck_clist_free( 	
				nmpsbck_t_clist		*clist)
{
	nmpsbck_t_clist		*clist_ptr;

	while ( clist != NULL)
	{
	  clist_ptr = clist;
	  clist = clist->next_ptr;
	  if ( clist_ptr->buffer)
	    free( clist_ptr->buffer);
	  free( clist_ptr);
	}
	return NMPS__SUCCESS;
}


/****************************************************************************
* Name:		nmpsbck_cellist_add()
*
* Type		pwr_tStatus
*
* Type		Parameter	IOGF	Description
*
* Description:
*		Add a cell to the cellist
*
**************************************************************************/
static pwr_tStatus	nmpsbck_cellist_add( 
			bck_ctx			bckctx,
			pwr_tObjid 		objid, 
			nmpsbck_t_cell_list 	**cellist,
			int			*cellist_count)
{
	nmpsbck_t_cell_list	*cellist_ptr;
	pwr_sAttrRef		attrref;
	pwr_tStatus		sts;
	pwr_tClassId		class;
	char			*objectp;

	sts = gdh_GetObjectClass ( objid, &class);
	if (EVEN(sts)) LogAndReturn(NMPS__BCKCELL, sts);

	sts = gdh_ObjidToPointer ( objid, (pwr_tAddress *) &objectp);
	if (EVEN(sts)) LogAndReturn(NMPS__BCKCELL, sts);

	/* Check if backup should be done on this cell */
	switch ( class)
	{
	  case pwr_cClass_NMpsCell:
	  case pwr_cClass_NMpsStoreCell:
	    if ( !(((pwr_sClass_NMpsCell *)objectp)->Function & 
				NMPS_CELLFUNC_BACKUP))
	      return NMPS__SUCCESS;
	    break;
	  case pwr_cClass_NMpsMirrorCell:
	    if ( !(((pwr_sClass_NMpsMirrorCell *)objectp)->Function & 
				NMPS_CELLFUNC_BACKUP))
	      return NMPS__SUCCESS;
	    break;
	  default:
	    return NMPS__SUCCESS;
	}

	cellist_ptr = calloc( 1, sizeof( nmpsbck_t_cell_list));
	if ( cellist_ptr == 0) return NMPS__NOMEMORY;

	/* Insert first in list */
	cellist_ptr->next_ptr = *cellist;
	*cellist = cellist_ptr;

	cellist_ptr->objid = objid;
	cellist_ptr->class = class;

	/* Direct link to the cell */
	memset( &attrref, 0, sizeof(attrref));
	attrref.Objid = objid;
	sts = gdh_DLRefObjectInfoAttrref ( &attrref, 
		(pwr_tAddress *) &cellist_ptr->cell,
		&cellist_ptr->subid);
	if (EVEN(sts)) LogAndReturn(NMPS__BCKCELL, sts);

	switch( cellist_ptr->class)
	{
	  case pwr_cClass_NMpsCell:
	  case pwr_cClass_NMpsStoreCell:
	    cellist_ptr->size = 
	        (char *)&(((pwr_sClass_NMpsCell *)(cellist_ptr->cell))->Data1P) -
	        (char *)(cellist_ptr->cell) +
	        ((pwr_sClass_NMpsCell *)(cellist_ptr->cell))->MaxSize *
		sizeof( plc_t_DataInfo);
	    break;
	  case pwr_cClass_NMpsMirrorCell:
	    cellist_ptr->size = 
	        (char *)&(((pwr_sClass_NMpsMirrorCell *)(cellist_ptr->cell))->Data1P) -
	        (char *)(cellist_ptr->cell) +
	        ((pwr_sClass_NMpsMirrorCell *)(cellist_ptr->cell))->MaxSize *
		sizeof( plc_t_DataInfoMirCell);
	    break;
	}

	(*cellist_count)++;
	return NMPS__SUCCESS;
}


/*************************************************************************
*
* Name:		nmps_get_bckconfig
*
* Typ		int
*
* Typ		Parameter	IOGF	Beskrivning
*
* Beskrivning: 
*	Hämta pekare till backup konfig objektet.
*
**************************************************************************/

static pwr_tStatus	nmps_get_bckconfig( bck_ctx	bckctx)
{
	pwr_tStatus	sts;
	pwr_tObjid	objid;
	pwr_sAttrRef	attrref;

	/* Get pointer to the bckconfig object */
	sts = gdh_GetClassList ( pwr_cClass_NMpsBackupConfig, &objid);
	if (EVEN(sts)) return NMPS__BCKCONFIG;

	/* Direct link to the cell */
	memset( &attrref, 0, sizeof(attrref));
	attrref.Objid = objid;
	sts = gdh_DLRefObjectInfoAttrref ( &attrref,
		(pwr_tAddress *) &bckctx->bckconfig,
		&bckctx->bckconfig_dlid);
	if ( EVEN(sts)) return sts;
	return NMPS__SUCCESS;
}


/*************************************************************************
*
* Name:		nmps_cell_init
*
* Typ		int
*
* Typ		Parameter	IOGF	Beskrivning
*
* Beskrivning: 
*	Initiering av cell funktionen.
*
**************************************************************************/

static pwr_tStatus	nmps_cell_init( bck_ctx	bckctx)
{
	pwr_tStatus	sts;
	pwr_tObjid	objid;

	bckctx->init_done = 1;

	/* Get the cell objects on this node */
	sts = gdh_GetClassList ( pwr_cClass_NMpsCell, &objid);
	while ( ODD(sts))
	{
          /* Store and direct link the cells */
          sts = nmpsbck_cellist_add( bckctx, objid, &bckctx->cellist, 
		&bckctx->cell_count);
	  if ( EVEN(sts)) return sts;

	  sts = gdh_GetNextObject( objid, &objid);
	}
	/* Get the cell objects on this node */
	sts = gdh_GetClassList ( pwr_cClass_NMpsStoreCell, &objid);
	while ( ODD(sts))
	{
          /* Store and direct link the cells */
          sts = nmpsbck_cellist_add( bckctx, objid, &bckctx->cellist, 
		&bckctx->cell_count);
	  if ( EVEN(sts)) return sts;

	  sts = gdh_GetNextObject( objid, &objid);
	}
	/* Get the cell objects on this node */
	sts = gdh_GetClassList ( pwr_cClass_NMpsMirrorCell, &objid);
	while ( ODD(sts))
	{
          /* Store and direct link the cells */
          sts = nmpsbck_cellist_add( bckctx, objid, &bckctx->cellist, 
		&bckctx->cell_count);
	  if ( EVEN(sts)) return sts;

	  sts = gdh_GetNextObject( objid, &objid);
	}

	bckctx->bckconfig->CellCount = bckctx->cell_count;

	return NMPS__SUCCESS;
}


/*************************************************************************
*
* Name:		nmpsbck_cell_handler
*
* Typ		int
*
* Typ		Parameter	IOGF	Beskrivning
*
* Beskrivning: 
*	Mirroring.
*
**************************************************************************/

static pwr_tStatus	nmpsbck_get_cellbuffer( bck_ctx	bckctx)
{
	char	*old_buffer;
	char	old_buffer_size;
	int	size;
	nmpsbck_t_cell_list	*cell_ptr;

	old_buffer = bckctx->buffer;
	old_buffer_size = bckctx->buffer_size;

	/* Allocate the local cell buffer */
	size = 0;
	cell_ptr = bckctx->cellist;
	while ( cell_ptr)
	{
	  size += sizeof(nmpsbck_t_cellheader);
	  size += cell_ptr->size;
	  cell_ptr = cell_ptr->next_ptr;
	}
	bckctx->buffer = calloc( 1 , size);
	if ( bckctx->buffer == 0 ) return NMPS__NOMEMORY;
	bckctx->buffer_size = size;

	/* Copy and free old buffer if there is one */
	if ( old_buffer != 0 && old_buffer_size != 0)
	{
	  memcpy( bckctx->buffer, old_buffer, old_buffer_size);
	  free( old_buffer);
	}
	return NMPS__SUCCESS;
}


/*************************************************************************
*
* Name:		nmpsbck_fill_buffer
*
* Typ		int
*
* Typ		Parameter	IOGF	Beskrivning
*
* Beskrivning: 
*	Fill the local buffer of cell objects.
*
**************************************************************************/

static pwr_tStatus	nmpsbck_fill_buffer( 	bck_ctx	bckctx)
{
	pwr_tStatus		sts;
	nmpsbck_t_cell_list	*cell_ptr;
	nmpsbck_t_cellheader	cellheader;
	int			offset;
	
	/* Copy cells into local area */
	offset = 0;
	cell_ptr = bckctx->cellist;
	while ( cell_ptr)
	{
	  strcpy( cellheader.type, HEADERTYPE_CELL);
	  cellheader.objid = cell_ptr->objid;
	  cellheader.class = cell_ptr->class;
	  cellheader.size = cell_ptr->size;

	  /* Check that there is size enough */
	  if ( offset + (int) sizeof(cellheader) + cellheader.size >
		bckctx->buffer_size)
	  {
	    sts = nmpsbck_get_cellbuffer( bckctx);
	    if (EVEN(sts)) return sts;
	  }

	  memcpy( bckctx->buffer + offset, &cellheader, sizeof( cellheader));
	  offset += sizeof( cellheader);
	  memcpy( bckctx->buffer + offset, cell_ptr->cell, cellheader.size);
	  offset += cellheader.size;

	  cell_ptr = cell_ptr->next_ptr;
	}
	bckctx->cellist_size = offset;

	return NMPS__SUCCESS;
}


/*************************************************************************
*
* Name:		nmpsbck_write_cells
*
* Typ		int
*
* Typ		Parameter	IOGF	Beskrivning
*
* Beskrivning: 
*	Write the cell objects in the local buffer on file.
*
**************************************************************************/

static pwr_tStatus	nmpsbck_write_cells( 	bck_ctx	bckctx,
						FILE	*bckfile)
{
	nmpsbck_t_cellheader	cellheader;
	nmpsbck_t_cellheader	*cellheader_ptr;
	pwr_tUInt32		csts;
	int			offset;
	nmpsbck_t_cell_list	*cell_ptr;

	if ( bckctx->increment)
	{
	  /* Write only cells with backup_now flag */
	  offset = 0;
	  cell_ptr = bckctx->cellist;
	  while ( cell_ptr)
	  {
	    cellheader_ptr = (nmpsbck_t_cellheader *) (bckctx->buffer + offset);
	    if ( cell_ptr->backup_now)
	    {
	      csts = fwrite( cellheader_ptr, sizeof(*cellheader_ptr) + 
			cellheader_ptr->size, 1, bckfile);
	      if (csts == 0) return csts;
	      cell_ptr->backup_now = 0;
	    }
	    offset += sizeof(*cellheader_ptr) + cellheader_ptr->size;
	    cell_ptr = cell_ptr->next_ptr;
	  }
	}
	else
	{
	  /* Write the whole buffer */
	  if ( bckctx->cellist_size > 0)
	  {
	    csts = fwrite( bckctx->buffer, bckctx->cellist_size, 1, bckfile);
	    if (csts == 0) return csts;
	  }

	  /* Reset backup_now flag */
	  cell_ptr = bckctx->cellist;
	  while ( cell_ptr)
	  {
	    cell_ptr->backup_now = 0;
	    cell_ptr = cell_ptr->next_ptr;
	  }
	}
	strcpy( cellheader.type, HEADERTYPE_CELLEND);
	csts = fwrite( &cellheader, sizeof( cellheader), 1, bckfile);
	if (csts == 0) return csts;

	return NMPS__SUCCESS;
}


/*************************************************************************
*
* Name:		nmpsbck_write_data
*
* Typ		int
*
* Typ		Parameter	IOGF	Beskrivning
*
* Beskrivning: 
*	Write the data objects in the local buffer on file.
*
**************************************************************************/

static pwr_tStatus	nmpsbck_write_data(
				bck_ctx	bckctx,
				FILE	*bckfile)
{
	nmpsbck_t_dataheader	dataheader;
	pwr_tUInt32		csts;
	nmpsbck_t_data_list	*data_ptr;
	
	data_ptr = bckctx->data_list;
	while ( data_ptr != NULL)
	{
	  if ( !bckctx->increment || data_ptr->new)
	  {
	    strcpy( dataheader.type, HEADERTYPE_DATA);
	    dataheader.objid = data_ptr->objid;
	    strcpy( dataheader.data_name, data_ptr->data_name);
	    dataheader.class = data_ptr->class;
	    dataheader.size = data_ptr->size;

	    csts = fwrite( &dataheader, sizeof( dataheader), 1, bckfile);
	    if (csts == 0) return csts;
	    csts = fwrite( data_ptr->data_ptr, data_ptr->size, 1, bckfile);
	    if (csts == 0) return csts;

	    data_ptr->found = 0;
	    data_ptr->new = 0;
	  }
	  data_ptr = data_ptr->next_ptr;
	}

	strcpy( dataheader.type, HEADERTYPE_DATAEND);
	csts = fwrite( &dataheader, sizeof( dataheader), 1, bckfile);
	if (csts == 0) return csts;

	return NMPS__SUCCESS;
}

/*************************************************************************
*
* Name:		nmpsbck_open_file
*
* Typ		int
*
* Typ		Parameter	IOGF	Beskrivning
*
* Beskrivning: 
*	Open current backup file and write a file header.
*
**************************************************************************/

static pwr_tStatus	nmpsbck_open_file( 	bck_ctx	bckctx)
{
	nmpsbck_t_fileheader	fileheader;
	pwr_tUInt32		csts;
	char			filename[200];
	float			reopen_time = 10;
	int			first_time;
	int			wait_some_time;

	if ( bckctx->file_num == 1)
	{
	  if ( bckctx->bckfile1 != 0)
	  {
	    /* Write error has occured */
	    first_time = 0;
	    bckctx->bckconfig->File1WriteErrorCount++;
	    nmps_fgetname( bckctx->bckfile1, filename, "");
	    errh_CErrLog( NMPS__BCKWRITE, errh_ErrArgAF(filename), NULL);
	    fclose( bckctx->bckfile1);
	    wait_some_time = 1;
	  }
	  else
	  {
	    first_time = 1;
	    wait_some_time = 0;
	  }

	  for (;;)
	  {
	    if ( wait_some_time)
	      sutl_sleep( reopen_time);
	    else
	      wait_some_time = 1;

	    /* Work with file 1 */
	    /* Open file */
	    nmpsbck_get_filename( bckctx->bckconfig->BackupFile,
			filename, NMPSBCK_FILE_EXT1);


#if defined(OS_ELN)
	    bckctx->bckfile1 = fopen( filename, "w+", "shr=get");
#elif defined(OS_VMS)
	    bckctx->bckfile1 = fopen( filename, "r+", "shr=get");
	    if (bckctx->bckfile1 == NULL)
	      bckctx->bckfile1 = fopen( filename, "w+", "shr=get");
#else
	    bckctx->bckfile1 = fopen( filename, "w+");
#endif
	    if (bckctx->bckfile1 != NULL)
	    {
	      /* Write a file header */
	      clock_gettime( CLOCK_REALTIME, &fileheader.creationtime);
	      fileheader.version = NMPS_BCK_FILEVERSION;
	      csts = fwrite( &fileheader, sizeof( fileheader), 1,
				bckctx->bckfile1);
	      if (csts != 0)
	      {
	        /* File 1 is open */
	        bckctx->bckconfig->File1OpenCount++;
	        nmps_fgetname( bckctx->bckfile1, filename, "");
	        errh_CErrLog( NMPS__BCKFILEOPEN, errh_ErrArgAF(filename), NULL);
	        if ( first_time)
	          return NMPS__SUCCESS;
	        else
	          return NMPS__FILEREOPEN;
	      }
	      fclose( bckctx->bckfile1);
	    }
	  }
	}
	else
	{
	  if ( bckctx->bckfile2 != 0)
	  {
	    /* Write error has occured */
	    first_time = 0;
	    bckctx->bckconfig->File2WriteErrorCount++;
	    nmps_fgetname( bckctx->bckfile2, filename, "");
	    errh_CErrLog( NMPS__BCKWRITE, errh_ErrArgAF(filename), NULL);
	    fclose( bckctx->bckfile2);
	    wait_some_time = 1;
	  }
	  else
	  {
	    first_time = 1;
	    wait_some_time = 0;
	  }

	  for (;;)
	  {
	    if ( wait_some_time)
	      sutl_sleep( reopen_time);
	    else
	      wait_some_time = 1;

	    /* Work with file 2 */
	    /* Open file */
	    nmpsbck_get_filename( bckctx->bckconfig->BackupFile,
			filename, NMPSBCK_FILE_EXT2);

#if defined(OS_ELN)
	    bckctx->bckfile2 = fopen( filename, "w+", "shr=get");
#elif defined(OS_VMS)
	    bckctx->bckfile2 = fopen( filename, "r+", "shr=get");
	    if (bckctx->bckfile2 == NULL) 
	      bckctx->bckfile2 = fopen( filename, "w+", "shr=get");
#else
	    bckctx->bckfile2 = fopen( filename, "w+");
#endif
	    if (bckctx->bckfile2 != NULL) 
	    {
	      /* Write a file header */
	      clock_gettime( CLOCK_REALTIME, &fileheader.creationtime);
	      fileheader.version = NMPS_BCK_FILEVERSION;
	      csts = fwrite( &fileheader, sizeof( fileheader), 1,
				bckctx->bckfile2);
	      if (csts != 0)
	      {
	        /* File 2 is open */
	        bckctx->bckconfig->File2OpenCount++;
	        nmps_fgetname( bckctx->bckfile2, filename, "");
	        errh_CErrLog( NMPS__BCKFILEOPEN, errh_ErrArgAF(filename), NULL);
	        if ( first_time)
	          return NMPS__SUCCESS;
	        else
	          return NMPS__FILEREOPEN;
	      }
	      fclose( bckctx->bckfile2);
	    }
	  }
	}
}

/*************************************************************************
*
* Name:		nmpsbck_write
*
* Typ		int
*
* Typ		Parameter	IOGF	Beskrivning
*
* Beskrivning: 
*	Write the nmps objects in the backup file.
*
**************************************************************************/

static pwr_tStatus	nmpsbck_write( bck_ctx	bckctx)
{
	pwr_tStatus		sts;
	nmpsbck_t_recordheader	recordheader;
	pwr_tUInt32		csts;
	pwr_tUInt32		actpos;
	FILE			*bckfile;
	
	if ( !bckctx->bckconfig->BackupOn)
	  return NMPS__SUCCESS;

	if ( (bckctx->file_num == 1  &&  bckctx->bckfile1 == 0) ||
	       (bckctx->file_num == 2  &&  bckctx->bckfile2 == 0))
	  /* This is the first time, open the file */
	  sts = nmpsbck_open_file( bckctx);
	      
	if ( bckctx->file_num == 1)
	  bckfile = bckctx->bckfile1;
	else
	  bckfile = bckctx->bckfile2;

	clock_gettime( CLOCK_REALTIME, &recordheader.time);
	strcpy( recordheader.type, HEADERTYPE_RECORDSTART);

	if ( !bckctx->increment)
	{
	  /* Start at the beginning of the file */
	  actpos = ftell( bckfile);
	  csts = fseek( bckfile, -actpos + sizeof(nmpsbck_t_fileheader), 1);
	  if (csts != 0) goto bck_write_error;
	  bckctx->record_count = 0;
	}
	else
	{
	  /* Go to the position of the last written record. */
	  /* This is necessary in ELN (bugg or feature ?) */
	  actpos = ftell( bckfile);
	  if ( bckctx->file_num == 1)
	    csts = fseek( bckfile, -actpos + bckctx->file1_pos, 1);
	  else
	    csts = fseek( bckfile, -actpos + bckctx->file2_pos, 1);
	  if (csts != 0) goto bck_write_error;
	  bckctx->record_count++;
	}

	csts = fwrite( &recordheader, sizeof( recordheader), 1, bckfile);
	if (csts == 0) goto bck_write_error;

	/* Write the cell objects */
	csts = nmpsbck_write_cells( bckctx, bckfile);
	if  (csts == 0) goto bck_write_error;

	/* Write the data objects */
	csts = nmpsbck_write_data( bckctx, bckfile);
	if (csts == 0) goto bck_write_error;

	/* Write record end header */
	clock_gettime( CLOCK_REALTIME, &recordheader.time);
	strcpy( recordheader.type, HEADERTYPE_RECORDEND);
	csts = fwrite( &recordheader, sizeof( recordheader), 1, bckfile);
	if (csts == 0) goto bck_write_error;

	/* Store current position */
	/* This is necessary in ELN (bugg or feature?) */
	if ( bckctx->file_num == 1)
	  bckctx->file1_pos = ftell( bckfile);
	else
	  bckctx->file2_pos = ftell( bckfile);

	csts = fflush( bckfile);
	if (csts != 0) goto bck_write_error;

	if ( bckctx->increment)
	{
	  bckctx->bckconfig->IncrementLoopCount++;
	  clock_gettime( CLOCK_REALTIME, &bckctx->bckconfig->LastIncrementBackup);
	}
	else
	{
	  bckctx->bckconfig->FullLoopCount++;
	  clock_gettime( CLOCK_REALTIME, &bckctx->bckconfig->LastFullBackup);
	}

	return NMPS__SUCCESS;

bck_write_error:

	/* Some error has occured */
	sts = nmpsbck_open_file( bckctx);
	return sts;
}


/*************************************************************************
*
* Name:		nmpsbck_cells
*
* Typ		int
*
* Typ		Parameter	IOGF	Beskrivning
*
* Beskrivning: 
*	Read the cell objects from backup file.
*
**************************************************************************/

static int		nmpsbck_timecmp( pwr_tTime 	*time_old,
					pwr_tTime	*time_new)
{
#if 0
/********************
	pwr_tTime	testtime;
	int		sts;

	if ( time_new->high == time_old->high &&
	     time_new->low == time_old->low)
	  return 1;
	sts = lib$sub_times( time_new, time_old, &testtime);
	if ( sts == LIB$_NEGTIM)
	  return 0;
	return 1;
***************/
#endif
	int	sts;

	sts = time_Acomp( time_new, time_old);
	if ( sts >= 0)
	  return 1;
	else
	  return 0;	
}

/*************************************************************************
*
* Name:		nmpsbck_check_file
*
* Typ		int
*
* Typ		Parameter	IOGF	Beskrivning
*
* Beskrivning: 
*	Check the consistency of a backup file.
*
**************************************************************************/

static pwr_tStatus nmpsbck_check_file( 	bck_ctx		bckctx,
					FILE		*bckfile,
					int		*record_count,
					unsigned int	*record_start,
					unsigned int	*cellarea_start,
					unsigned int	*dataarea_start,
					pwr_tTime	*first_record_time,
					pwr_tTime	*last_record_time,
					int		time_only)
{
	nmpsbck_t_fileheader	fileheader;
	nmpsbck_t_recordheader	recordheader;
	nmpsbck_t_recordheader	recordheaderend;
	nmpsbck_t_cellheader	cellheader;
	nmpsbck_t_dataheader	dataheader;
	pwr_tUInt32		csts;
	pwr_tUInt32		actpos;
	int			cell_read_success;
	int			data_read_success;

	actpos = ftell( bckfile);
	csts = fseek( bckfile, - actpos , 1);
	if (csts != 0) return NMPS__FILEREAD;

	/* Read file header */
	csts = fread( &fileheader, sizeof(fileheader), 1, bckfile);
	if (csts == 0) return NMPS__FILEREAD;

	/* Check version */
	if (fileheader.version != NMPS_BCK_FILEVERSION)
	  return NMPS__BCKVERSION;

	*record_count = 0;
	cell_read_success = 0;
	data_read_success = 0;
	for (;;)
	{
	  /* Read record header */
	  /* Store the start of the record */
	  *record_start = ftell( bckfile);
	  csts = fread( &recordheader, sizeof(recordheader), 1, bckfile);
	  if (csts == 0)
	    break;

	  if (strcmp( recordheader.type, HEADERTYPE_RECORDSTART))
	    break;
	  if (!nmpsbck_timecmp( &fileheader.creationtime, &recordheader.time))
	    break;

	  if ( *record_count == 0)
	  {
	    memcpy( first_record_time, &recordheader.time, 
			sizeof( recordheader.time));
	    if ( time_only)
	      return NMPS__SUCCESS;
	  }
	  else
	  {
	    if (!nmpsbck_timecmp( first_record_time, &recordheader.time))
	      break;
          }

	  /* Read cell area */
	  cell_read_success = 0;
	  /* Store the start of the cell area */
	  *cellarea_start = ftell( bckfile);
	  for (;;)
	  {
	    csts = fread( &cellheader, sizeof(cellheader), 1, bckfile);
	    if (csts == 0)
	      break;

	    if ( !strcmp( cellheader.type, HEADERTYPE_CELLEND))
	    {
	      /* End of cell area */
	      cell_read_success = 1;
	      break;
	    }
	    else if ( strcmp( cellheader.type, HEADERTYPE_CELL))
	      break;

	    csts = fseek( bckfile, cellheader.size, 1);
	    if (csts != 0)
	      break;
	  }

	  if ( !cell_read_success)
	    break;

	  /* Read data area */
	  data_read_success = 0;
	  /* Store the start of the data area */
	  *dataarea_start = ftell( bckfile);
	  for (;;)
	  {
	    csts = fread( &dataheader, sizeof(dataheader), 1, bckfile);
	    if (csts == 0)
	      break;

	    if ( !strcmp( dataheader.type, HEADERTYPE_DATAEND))
	    {
	      /* End of data area */
	      data_read_success = 1;
	      break;
	    }
	    else if ( strcmp( dataheader.type, HEADERTYPE_DATA))
	      break;

	    csts = fseek( bckfile, dataheader.size, 1);
	    if (csts != 0)
	      break;
	  }

	  if ( !data_read_success)
	    break;

	  /* Read record header */
	  csts = fread( &recordheaderend, sizeof(recordheaderend), 1, bckfile);
	  if (csts == 0)
	    break;

	  if (strcmp( recordheaderend.type, HEADERTYPE_RECORDEND))
	    break;

	  if (!nmpsbck_timecmp( &recordheader.time, &recordheaderend.time))
	    break;

	  (*record_count)++;
	  record_start++;
	  cellarea_start++;
	  dataarea_start++;
	}

	if ( *record_count == 0)
	{
	  /* No valid record found */
	  return NMPS__BCKFILECORRUPT;
	}

	memcpy( last_record_time, &recordheader.time,
			sizeof( recordheader.time));

	return NMPS__SUCCESS;
}


/*************************************************************************
*
* Name:		nmpsbck_read
*
* Typ		int
*
* Typ		Parameter	IOGF	Beskrivning
*
* Beskrivning: 
*	Read the cell objects from backup file.
*
**************************************************************************/

static pwr_tStatus	nmpsbck_read( bck_ctx	bckctx)
{
	pwr_tStatus		sts;
	nmpsbck_t_cellheader	cellheader;
	nmpsbck_t_dataheader	dataheader;
	FILE			*bckfile;
	FILE			*bckfile1;
	FILE			*bckfile2;
	pwr_tUInt32		csts;
	pwr_tUInt32		actpos;
	int			k;
	char			*databuff = 0;
	int			databuff_size = 0;
	nmpsbck_t_data_list	*data_ptr;
	nmpsbck_t_data_list	*next_ptr;
	char			*objectp;
	char			filename[200];
	int			file_num = 1;
	int			record_count;
	pwr_tUInt32		cellarea_start[NMPSBCK_MAX_RECORDS];
	pwr_tUInt32		dataarea_start[NMPSBCK_MAX_RECORDS];
	pwr_tUInt32		record_start[NMPSBCK_MAX_RECORDS];
	nmpsbck_t_clist		*clist = 0;
	nmpsbck_t_clist		*clist_ptr;
	int			cell_read_success;
	int			data_read_success;
	int			i;
	pwr_tObjid		objid;
	int			data_count;
	int			created;
	pwr_tTime		first_record_time;
	pwr_tTime		last_record_time;
	pwr_tTime		bckfile1_time;
	pwr_tTime		bckfile2_time;
	pwr_tStatus		bckfile1_sts;
	pwr_tStatus		bckfile2_sts;
	char			*ptr;
	int			return_sts;

	/* Open file 1 */
	file_num = 1;
	bckfile1_sts = NMPS__SUCCESS;
	nmpsbck_get_filename( bckctx->bckconfig->BackupFile,
			filename, NMPSBCK_FILE_EXT1);

#if defined(OS_ELN)
	bckfile1 = fopen( filename, "r+", "shr=get");
#elif defined(OS_VMS)
	bckfile1 = fopen( filename, "r+", "shr=get");
#else
	bckfile1 = fopen( filename, "r+");
#endif
	if (bckfile1 == NULL)
	  bckfile1_sts = NMPS__BCKOPEN;
	else
	{
	  /* Get time of first record */
	  bckfile1_sts = nmpsbck_check_file( bckctx, bckfile1, &record_count, 
			record_start, cellarea_start, dataarea_start, 
			&bckfile1_time, &last_record_time, 1);
	  if ( EVEN(bckfile1_sts))
	  {
	    Log( NMPS__RELOAD_FILE1, bckfile1_sts);
	    fclose( bckfile1);
	  }
	}

	/* Open file 2 */
	file_num = 2;
	bckfile2_sts = NMPS__SUCCESS;

	nmpsbck_get_filename( bckctx->bckconfig->BackupFile,
			filename, NMPSBCK_FILE_EXT2);

#if defined(OS_ELN)
	bckfile2 = fopen( filename, "r+", "shr=get");
#elif defined(OS_VMS)
	bckfile2 = fopen( filename, "r+", "shr=get");
#else
	bckfile2 = fopen( filename, "r+");
#endif
	if (bckfile2 == NULL)
	  bckfile2_sts = NMPS__BCKOPEN;
	else
	{
	  /* Get time of first record */
	  bckfile2_sts = nmpsbck_check_file( bckctx, bckfile2, &record_count, record_start,
			cellarea_start, dataarea_start, &bckfile2_time,
			&last_record_time, 1);
	  if ( EVEN(bckfile2_sts))
	  {
	    Log( NMPS__RELOAD_FILE2, bckfile2_sts);
	    fclose( bckfile2);
	  }
	}

	if ( ODD( bckfile1_sts) && ODD( bckfile2_sts))
	{
	  /* Both is ok so far */
	  /* Compare time and choose the latest file */
	  if ( nmpsbck_timecmp( &bckfile1_time, &bckfile2_time))
	  {
	    /* Number 2 is the lastest, check consistency of file 2 */
	    bckfile2_sts = nmpsbck_check_file( bckctx, bckfile2, &record_count, 
			record_start, 
			cellarea_start, dataarea_start, &first_record_time,
			&last_record_time, 0);
	    if ( EVEN(bckfile2_sts))
	    {
	      /* File number 2 is currupt, try file number 1 */
	      Log( NMPS__RELOAD_FILE2, bckfile2_sts);
	      fclose( bckfile2);
	      bckfile1_sts = nmpsbck_check_file( bckctx, bckfile1, &record_count, 
			record_start, 
			cellarea_start, dataarea_start, &first_record_time,
			&last_record_time, 0);
	      if ( EVEN(bckfile1_sts))
	      {
	        /* Both files are corrupt, log and return */
	        Log( NMPS__RELOAD_FILE1, bckfile1_sts);
	        fclose( bckfile1);
	        bckfile = 0;
	        sts = NMPS__RELOAD;
	        goto nmpsbck_read_abort;
	      }
	      /* File 1 is OK, read file 1 */
	      bckfile = bckfile1;
	    }
	    else
	    {
	      /* File 2 is OK, read file 2 */
	      bckfile = bckfile2;
	      fclose( bckfile1);
	    }
	  }
	  else
	  {
	    /* Number 1 is the lastest, check consistency of file 1 */
	    bckfile1_sts = nmpsbck_check_file( bckctx, bckfile1, &record_count, 
			record_start, 
			cellarea_start, dataarea_start, &first_record_time,
			&last_record_time, 0);
	    if ( EVEN(bckfile1_sts))
	    {
	      /* File number 1 is currupt, try file number 2 */
	      Log( NMPS__RELOAD_FILE1, bckfile1_sts);
	      fclose( bckfile1);
	      bckfile2_sts = nmpsbck_check_file( bckctx, bckfile2, &record_count, 
			record_start, 
			cellarea_start, dataarea_start, &first_record_time,
			&last_record_time, 0);
	      if ( EVEN(bckfile2_sts))
	      {
	        /* Both files are corrupt, log and return */
	        Log( NMPS__RELOAD_FILE2, bckfile1_sts);
	        fclose( bckfile2);
	        bckfile = 0;
	        sts = NMPS__RELOAD;
	        goto nmpsbck_read_abort;
	      }
	      /* File 2 is OK, read file 2 */
	      bckfile = bckfile2;
	    }
	    else
	    {
	      /* File 1 is OK, read file 1 */
	      bckfile = bckfile1;
	      fclose( bckfile2);
	    }
	  }
	}
	else if ( ODD( bckfile1_sts))
	{
	  /* File 2 is currupt, go for file 1 */
	  bckfile1_sts = nmpsbck_check_file( bckctx, bckfile1, &record_count, 
			record_start, 
			cellarea_start, dataarea_start, &first_record_time,
			&last_record_time, 0);
	  if ( EVEN(bckfile1_sts))
	  {
	    /* Both files are corrupt, log and return */
	    Log( NMPS__RELOAD_FILE1, bckfile1_sts);
	    fclose( bckfile1);
	    bckfile = 0;
	    sts = NMPS__RELOAD;
	    goto nmpsbck_read_abort;
	  }
	  /* File 1 is OK, read file 1 */
	  bckfile = bckfile1;
	}
	else if ( ODD( bckfile2_sts))
	{
	  /* File 1 is currupt, go for file2 */
	  bckfile2_sts = nmpsbck_check_file( bckctx, bckfile2, &record_count, 
			record_start, 
			cellarea_start, dataarea_start, &first_record_time,
			&last_record_time, 0);
	  if ( EVEN(bckfile2_sts))
	  {
	    /* Both files are corrupt, log and return */
	    Log( NMPS__RELOAD_FILE2, bckfile2_sts);
	    fclose( bckfile2);
	    bckfile = 0;
	    sts = NMPS__RELOAD;
	    goto nmpsbck_read_abort;
	  }
	  /* File 2 is OK, read file 2 */
	  bckfile = bckfile2;
	}
	else
	{
	  /* Both files are corrupt, abort */
	  if ( bckfile2_sts != NMPS__BCKOPEN)
	    sts = bckfile2_sts;
	  else 
	    sts = bckfile1_sts;
	  bckfile = 0;
	  goto nmpsbck_read_abort;
	}

	nmps_fgetname( bckfile, filename, "");
	errh_CErrLog( NMPS__RESTORING, errh_ErrArgAF(filename), NULL);

	/* Read dataareas records backwards */
	for ( i = 0; i < record_count; i++)
	{
	  /* Position on cell area on record */
	  actpos = ftell( bckfile);
	  csts = fseek( bckfile, dataarea_start[record_count-i-1] - actpos , 1);
	  if (csts != 0)
	  {
	    sts = NMPS__FILEREAD;
	    goto nmpsbck_read_abort;
	  }

	  data_read_success = 0;
	  for (;;)
	  {
	    csts = fread( &dataheader, sizeof(dataheader), 1, bckfile);
	    if (csts == 0) break;
	
	    if ( !strcmp( dataheader.type, HEADERTYPE_DATAEND))
	    {
	      /* End of data area */
	      data_read_success = 1;
	      break;
	    }

	    /* Allocate or increment the read buffer size */
	    if ( databuff == 0 || dataheader.size > databuff_size)
	    {
	      if ( databuff)
	        free( databuff);
	      databuff = calloc( 1 , dataheader.size);
	      if ( databuff == 0) 
	      {
	        sts = NMPS__NOMEMORY;
	        goto nmpsbck_read_abort;
	      }
	      databuff_size = dataheader.size;
	    }
	    csts = fread( databuff, dataheader.size, 1, bckfile);
	    if (csts == 0) break;

	    /* Check if the objid already is in the data_db */
	    sts = nmpsbck_data_db_find_old(  bckctx->data_list,
	  		dataheader.objid, &data_ptr);
	    if ( ODD(sts))
	      continue;

	    created = 1;
	    sts = gdh_CreateObject( dataheader.data_name, dataheader.class, 0,
			&objid, pwr_cNObjid, 0, pwr_cNObjid);
	    if ( sts == GDH__DUPLNAME )
	    {
	      /* The object already exist, this might be a static object */
	      sts = gdh_NameToObjid( dataheader.data_name, &objid);
	      if ( EVEN(sts))
	      {
	        Log(NMPS__RELOADCREA, sts);
		sts = NMPS__RELOADCREA;
	        goto nmpsbck_read_abort;
	      }
	      created = 0;
	    }
	    else if ( EVEN(sts))
	    {
	      Log(NMPS__RELOADCREA, sts);
	      sts = NMPS__RELOADCREA;
	      goto nmpsbck_read_abort;
	    }

	    /* Insert the object in the data list */
	    sts = nmpsbck_data_db_create( bckctx, objid, &data_ptr);
	    if ( EVEN(sts))
	      goto nmpsbck_read_abort;
	    data_ptr->old_objid = dataheader.objid;
	    data_ptr->new = 1;
	    data_ptr->created = created;

	    memcpy( data_ptr->data_ptr, databuff, min( dataheader.size,
		data_ptr->size));
	  }
	  if ( !data_read_success)
	    break;
	}

	if ( databuff)
        {
	  free( databuff);
          databuff = 0;
        }

	if ( !data_read_success)
	{
	  sts = NMPS__FILEREAD;
	  goto nmpsbck_read_abort;
	}

	/* Read the cells backwards */
	for ( i = 0; i < record_count; i++)
	{
	  /* Position on cell area on record */
	  actpos = ftell( bckfile);
	  csts = fseek( bckfile, cellarea_start[record_count-i-1] - actpos , 1);
	  if (csts != 0)
	  {
	    sts = NMPS__FILEREAD;
	    goto nmpsbck_read_abort;
	  }

	  cell_read_success = 0;
	  for (;;)
	  {
	    csts = fread( &cellheader, sizeof(cellheader), 1, bckfile);
	    if (csts == 0) break;
	
	    if ( !strcmp( cellheader.type, HEADERTYPE_CELLEND))
	    {
	      /* End of data area */
	      cell_read_success = 1;
	      break;
	    }

	    sts = nmpsbck_clist_find( clist, cellheader.objid, &clist_ptr);
	    if (ODD(sts))
	    {
	      /* This cell is already found */
	      csts = fseek( bckfile, cellheader.size, 1);
	      if (csts != 0) break;
	      continue;
	    }

	    sts = nmpsbck_clist_store( &clist, cellheader.objid, &clist_ptr);
	    if (EVEN(sts))
	      goto nmpsbck_read_abort;

	    clist_ptr->buffer = calloc( 1 , cellheader.size);
	    if ( clist_ptr->buffer == 0) 
	    {
	      sts = NMPS__NOMEMORY;
	      goto nmpsbck_read_abort;
	    }
	    clist_ptr->buffer_size = cellheader.size;
	    clist_ptr->class = cellheader.class;

	    sts = gdh_ObjidToPointer( clist_ptr->objid, 
			(pwr_tAddress  *) &clist_ptr->objectp);
	    if ( EVEN(sts))
	    {
	      sts = NMPS__RELOADINCONS;
	      goto nmpsbck_read_abort;
	    }

	    sts = gdh_GetObjectSize( clist_ptr->objid, &clist_ptr->object_size);
	    if ( EVEN(sts))
	    {
	      sts = NMPS__RELOADINCONS;
	      goto nmpsbck_read_abort;
	    }

	    csts = fread( clist_ptr->buffer, clist_ptr->buffer_size,
		1, bckfile);
	    if (csts == 0) break;

	    switch( cellheader.class)
	    {
	      case pwr_cClass_NMpsCell:
	      case pwr_cClass_NMpsStoreCell:
	      {
	        pwr_sClass_NMpsCell 	*cell_ptr;
	        pwr_sClass_NMpsCell 	*object_ptr;
	        plc_t_DataInfo		*data_block_ptr;

	        cell_ptr = (pwr_sClass_NMpsCell *) clist_ptr->buffer;
	        object_ptr = (pwr_sClass_NMpsCell *) clist_ptr->objectp;
	        data_block_ptr = (plc_t_DataInfo *) &cell_ptr->Data1P;
	        for ( k = 0; k < cell_ptr->LastIndex; k++)
	        {
	          /* Check if the objid already is in the data_db */
	          sts = nmpsbck_data_db_find_old(  bckctx->data_list,
			data_block_ptr->Data_ObjId, &data_ptr);
	          if ( EVEN(sts))
	          {
	            sts = NMPS__RELOADINCONS;
	            goto nmpsbck_read_abort;
	          }
	          /* Replace the objid */
	          data_ptr->found = 1;
	          data_block_ptr->Data_ObjId = data_ptr->objid;
	          /* Put NULL in pointer */
	          data_block_ptr->DataP = NULL;
	          data_block_ptr->Data_Dlid = pwr_cNDlid;
	          data_block_ptr++;
	        }
	        /* Replace DataLast and DataL */
	        if ( cell_ptr->LastIndex > 0 && 
		     cdh_ObjidIsNotNull( cell_ptr->DataL_ObjId))
	        {
	          sts = nmpsbck_data_db_find_old(  bckctx->data_list,
			cell_ptr->DataL_ObjId, &data_ptr);
	          if ( EVEN(sts))
	          {
	            sts = NMPS__RELOADINCONS;
	            goto nmpsbck_read_abort;
	          }
	          /* Replace the objid */
	          cell_ptr->DataL_ObjId = data_ptr->objid;
	          /* Put NULL in pointer */
	          cell_ptr->DataLP = NULL;
	        	
	          sts = nmpsbck_data_db_find_old(  bckctx->data_list,
			cell_ptr->DataLast_ObjId, &data_ptr);
	          if ( EVEN(sts))
	          {
	            sts = NMPS__RELOADINCONS;
	            goto nmpsbck_read_abort;
	          }
	          /* Replace the objid */
	          cell_ptr->DataLast_ObjId = data_ptr->objid;
	          /* Put NULL in pointer */
	          cell_ptr->DataLastP = NULL;
	        }
	        /* Reset flags */
	        cell_ptr->InFlag = 0;
	        cell_ptr->OutFrontFlag = 0;
	        cell_ptr->OutRearFlag = 0;
	        cell_ptr->ExternFlag = 0;
	        cell_ptr->FrontNew = 0;
	        cell_ptr->RearNew = 0;
	        cell_ptr->ResetObject = object_ptr->ResetObject;
	        cell_ptr->ResetObjectP = object_ptr->ResetObjectP;
	        break;
	      }
	      case pwr_cClass_NMpsMirrorCell:
	      {
	        pwr_sClass_NMpsMirrorCell *cell_ptr;
	        pwr_sClass_NMpsMirrorCell *object_ptr;
	        plc_t_DataInfoMirCell	*data_block_ptr;

	        cell_ptr = (pwr_sClass_NMpsMirrorCell *) clist_ptr->buffer;
	        object_ptr = (pwr_sClass_NMpsMirrorCell *) clist_ptr->objectp;
	        data_block_ptr = (plc_t_DataInfoMirCell *) &cell_ptr->Data1P;
	        for ( k = 0; k < cell_ptr->LastIndex; k++)
	        {
	          /* Check if the objid already is in the data_db */
	          sts = nmpsbck_data_db_find_old(  bckctx->data_list,
			data_block_ptr->Data_ObjId, &data_ptr);
	          if ( EVEN(sts))
	          {
	            sts = NMPS__RELOADINCONS;
	            goto nmpsbck_read_abort;
	          }
	          data_ptr->found = 1;
	          data_block_ptr->Data_ObjId = data_ptr->objid;
	          /* Put NULL in pointer */
	          data_block_ptr->DataP = NULL;
	          data_block_ptr->Data_Dlid = pwr_cNDlid;
	          data_block_ptr++;
	        }
	        /* Replace DataLast */
	        if ( cell_ptr->LastIndex > 0 && 
		     cdh_ObjidIsNotNull( cell_ptr->DataLast_ObjId))
	        {
	          sts = nmpsbck_data_db_find_old(  bckctx->data_list,
			cell_ptr->DataLast_ObjId, &data_ptr);
	          if ( EVEN(sts))
	          {
	            sts = NMPS__RELOADINCONS;
	            goto nmpsbck_read_abort;
	          }
	          /* Replace the objid */
	          cell_ptr->DataLast_ObjId = data_ptr->objid;
	          /* Put NULL in pointer */
	          cell_ptr->DataLastP = NULL;
	        }
	        /* Reset flags */
	        object_ptr->TempLastIndex = cell_ptr->LastIndex;
	        break;
	      }
	    }
	  }
	  if ( !cell_read_success)
	    break;
	}

	if ( !cell_read_success)
	{
	  sts = NMPS__FILEREAD;
	  goto nmpsbck_read_abort;
	}

	/* Remove data objects that were not found in a cell */
	data_count = 0;
	data_ptr = bckctx->data_list;
	while ( data_ptr != NULL)
	{
	  if ( !data_ptr->found)
	  {
	    /* Remove subscriptions */
	    sts = gdh_DLUnrefObjectInfo( data_ptr->data_subid);
	    if (EVEN(sts)) Log(NMPS__BCKUNREF, sts);

	    /* Delete the data entry */
	    next_ptr = data_ptr->next_ptr;
	    objid = data_ptr->objid;
	    created = data_ptr->created;
	    sts = nmpsbck_data_db_delete( &bckctx->data_list, data_ptr);
	    if ( created)
	      sts = gdh_DeleteObject( objid);
	    data_ptr = next_ptr;
	    continue;
	  }
	  else
	    data_count++;
	  data_ptr = data_ptr->next_ptr;
	}
	bckctx->bckconfig->DataCount = data_count;

	/* Copy cell buffer into the cell object */
	clist_ptr = clist;
	while ( clist_ptr != NULL)
	{
	  /* Copy current function in cell object and reset reload bit */
	  switch( clist_ptr->class)
	  {
	    case pwr_cClass_NMpsCell:
	    case pwr_cClass_NMpsStoreCell:
	      ((pwr_sClass_NMpsCell *) clist_ptr->buffer)->ReloadDone = 0;
	      ((pwr_sClass_NMpsCell *) clist_ptr->buffer)->MirrorRestart = 1;
	      ((pwr_sClass_NMpsCell *) clist_ptr->buffer)->Function = 
	        ((pwr_sClass_NMpsCell *) clist_ptr->objectp)->Function;
	      memcpy( clist_ptr->objectp, clist_ptr->buffer,
			min( (int)clist_ptr->object_size, clist_ptr->buffer_size));
	      break;
	    case pwr_cClass_NMpsMirrorCell:
	      /* Copy to temporary area for the mirror job to read */
	      ptr = (char *) &((pwr_sClass_NMpsMirrorCell *) 
			clist_ptr->buffer)->Data1P;
	      memcpy( 
		((pwr_sClass_NMpsMirrorCell *) (clist_ptr->objectp))->TempArea,
		 ptr, ptr - clist_ptr->buffer);
	      ((pwr_sClass_NMpsMirrorCell *) 
			(clist_ptr->objectp))->ReloadDone = NMPS_CELL_RELOADDONE;
	      ((pwr_sClass_NMpsMirrorCell *) 
			(clist_ptr->objectp))->MirrorRestart = 1;
	      break;
	  }

	  clist_ptr = clist_ptr->next_ptr;
	}

	/* Release new cells with backup function, by setting the
	   backup done flag */
	sts = gdh_GetClassList ( pwr_cClass_NMpsCell, &objid);
	while ( ODD(sts))
	{
	  sts = nmpsbck_clist_find( clist, cellheader.objid, &clist_ptr);
	  if (EVEN(sts))
	  {
	    sts = gdh_ObjidToPointer( objid, (pwr_tAddress *) &objectp);
	    if (EVEN(sts))
	    {
	      nmpsbck_set_cell_backup_done();
	      LogAndReturn(NMPS__RESTOREERROR, sts);
	    }

	    if ( ((pwr_sClass_NMpsCell *) objectp)->Function & 
			NMPS_CELLFUNC_BACKUP)
	    {
	      ((pwr_sClass_NMpsCell *) objectp)->ReloadDone = NMPS_CELL_RELOADDONE;
	      ((pwr_sClass_NMpsCell *) objectp)->InitTime = 1;
	    }
	  }
	  sts = gdh_GetNextObject( objid, &objid);
	}
	sts = gdh_GetClassList ( pwr_cClass_NMpsStoreCell, &objid);
	while ( ODD(sts))
	{
	  sts = nmpsbck_clist_find( clist, cellheader.objid, &clist_ptr);
	  if (EVEN(sts))
	  {
	    sts = gdh_ObjidToPointer( objid, (pwr_tAddress *) &objectp);
	    if (EVEN(sts))
	    {
	      nmpsbck_set_cell_backup_done();
	      LogAndReturn(NMPS__RESTOREERROR, sts);
	    }

	    if ( ((pwr_sClass_NMpsStoreCell *) objectp)->Function & 
			NMPS_CELLFUNC_BACKUP)
	    {
	      ((pwr_sClass_NMpsStoreCell *) objectp)->ReloadDone = NMPS_CELL_RELOADDONE;
	      ((pwr_sClass_NMpsStoreCell *) objectp)->InitTime = 1;
	    }
	  }
	  sts = gdh_GetNextObject( objid, &objid);
	}
	sts = gdh_GetClassList ( pwr_cClass_NMpsMirrorCell, &objid);
	while ( ODD(sts))
	{
	  sts = nmpsbck_clist_find( clist, cellheader.objid, &clist_ptr);
	  if (EVEN(sts))
	  {
	    sts = gdh_ObjidToPointer( objid, (pwr_tAddress *) &objectp);
	    if (EVEN(sts))
	    {
	      nmpsbck_set_cell_backup_done();
	      LogAndReturn(NMPS__RESTOREERROR, sts);
	    }

	    if ( ((pwr_sClass_NMpsMirrorCell *) objectp)->Function & 
			NMPS_CELLFUNC_BACKUP)
	      ((pwr_sClass_NMpsMirrorCell *) objectp)->ReloadDone = NMPS_CELL_RELOADDONE;
	  }
	  sts = gdh_GetNextObject( objid, &objid);
	}

	nmpsbck_clist_free( clist);

	errh_CErrLog( NMPS__RESTORED, NULL);
	fclose( bckfile);
	bckctx->bckconfig->ReloadFailure = 0;
	return NMPS__SUCCESS;

nmpsbck_read_abort:
	/* Abort reload and cleanup */
	return_sts = sts;
	Log(NMPS__RESTOREABORT, return_sts);

	/* Remove data objects that were created */
	data_count = 0;
	data_ptr = bckctx->data_list;
	while ( data_ptr != NULL)
	{
	  /* Remove subscriptions */
	  sts = gdh_DLUnrefObjectInfo( data_ptr->data_subid);
	  if ( data_ptr->created)
	    sts = gdh_DeleteObject( data_ptr->objid);
	  data_ptr = data_ptr->next_ptr;
	}

	data_ptr = bckctx->data_list;
	while ( data_ptr != NULL)
	{
	  next_ptr = data_ptr->next_ptr;
	  sts = nmpsbck_data_db_delete( &bckctx->data_list, data_ptr);
	  data_ptr = next_ptr;
	}	

	if ( databuff)
	  free( databuff);

	/* Release the cell objects by setting backup done flag */
	sts = nmpsbck_set_cell_backup_done();
	if (EVEN(sts)) return sts;

	nmpsbck_clist_free( clist);
	if ( bckfile != NULL)
	  fclose( bckfile);
	bckctx->bckconfig->ReloadFailure = 1;

	return return_sts;
}


/*************************************************************************
*
* Name:		nmpsbck_set_cell_backup_done
*
* Typ		int
*
* Typ		Parameter	IOGF	Beskrivning
*
* Beskrivning: 
*	Set backup done flag in all cell objects with backup function.
*
**************************************************************************/

static pwr_tStatus	nmpsbck_set_cell_backup_done()
{
	pwr_tStatus 	sts;
	pwr_tObjid	objid;
	char		*objectp;

	/* Release all cells with backup function, by setting the
	   backup done flag */
	sts = gdh_GetClassList ( pwr_cClass_NMpsCell, &objid);
	while ( ODD(sts))
	{
	  sts = gdh_ObjidToPointer( objid, (pwr_tAddress *) &objectp);
	  if (EVEN(sts)) LogAndReturn(NMPS__RESTOREERROR, sts);

	  if ( ((pwr_sClass_NMpsCell *) objectp)->Function & 
			NMPS_CELLFUNC_BACKUP)
	  {
	    ((pwr_sClass_NMpsCell *) objectp)->ReloadDone = NMPS_CELL_RELOADDONE;
	    ((pwr_sClass_NMpsCell *) objectp)->InitTime = 1;
	  }
	  sts = gdh_GetNextObject( objid, &objid);
	}
	sts = gdh_GetClassList ( pwr_cClass_NMpsStoreCell, &objid);
	while ( ODD(sts))
	{
	  sts = gdh_ObjidToPointer( objid, (pwr_tAddress *) &objectp);
	  if (EVEN(sts)) LogAndReturn(NMPS__RESTOREERROR, sts);

	  if ( ((pwr_sClass_NMpsStoreCell *) objectp)->Function & 
			NMPS_CELLFUNC_BACKUP)
	  {
	    ((pwr_sClass_NMpsStoreCell *) objectp)->ReloadDone = NMPS_CELL_RELOADDONE;
	    ((pwr_sClass_NMpsStoreCell *) objectp)->InitTime = 1;
	  }
	  sts = gdh_GetNextObject( objid, &objid);
	}
	sts = gdh_GetClassList ( pwr_cClass_NMpsMirrorCell, &objid);
	while ( ODD(sts))
	{
	  sts = gdh_ObjidToPointer( objid, (pwr_tAddress *) &objectp);
	  if (EVEN(sts)) LogAndReturn(NMPS__RESTOREERROR, sts);

	  if ( ((pwr_sClass_NMpsMirrorCell *) objectp)->Function & 
			NMPS_CELLFUNC_BACKUP)
	    ((pwr_sClass_NMpsMirrorCell *) objectp)->ReloadDone = NMPS_CELL_RELOADDONE;
	  sts = gdh_GetNextObject( objid, &objid);
	}
	return NMPS__SUCCESS;
}

/*************************************************************************
*
* Name:		nmpsbck_cell_handler
*
* Typ		int
*
* Typ		Parameter	IOGF	Beskrivning
*
* Beskrivning: 
*	Mirroring.
*
**************************************************************************/

static pwr_tStatus	nmpsbck_cell_handler( bck_ctx	bckctx)
{
	pwr_tStatus		sts;
	nmpsbck_t_cell_list	*cell_ptr;
	int			backup_now;
	
	/* Loop through the cell objects */
	backup_now = 0;
	cell_ptr = bckctx->cellist;
	while ( cell_ptr)
	{
	  switch ( cell_ptr->class)
	  {
	    case pwr_cClass_NMpsCell:
	    case pwr_cClass_NMpsStoreCell:
	      if (((pwr_sClass_NMpsCell *)(cell_ptr->cell))->BackupNow)
	      {
	        backup_now = 1;
	        cell_ptr->backup_now = 1;
	        ((pwr_sClass_NMpsCell *)(cell_ptr->cell))->BackupNow = 0;
	      }
	      break;
	    case pwr_cClass_NMpsMirrorCell:
	      if (((pwr_sClass_NMpsMirrorCell *)(cell_ptr->cell))->BackupNow)
	      {
	        backup_now = 1;
	        cell_ptr->backup_now = 1;
	        ((pwr_sClass_NMpsMirrorCell *)(cell_ptr->cell))->BackupNow = 0;
	      }
	      break;
	  }
	  cell_ptr = cell_ptr->next_ptr;
	}
	if ( backup_now || !bckctx->increment)
	{
	  sts = nmpsbck_fill_buffer( bckctx);
	  if (EVEN(sts)) return sts;

	  /* Update the data list */
	  sts = nmpsbck_data_handler( bckctx);
	  if (EVEN(sts)) return sts;

	  sts = nmpsbck_write( bckctx);
	  if (EVEN(sts)) return sts;
	}
	return NMPS__SUCCESS;
}


/****************************************************************************
* Name:		nmpsbck_free()
*
* Type		pwr_tStatus
*
* Type		Parameter	IOGF	Description
*
* Description:
*		Free the bck context.
*
**************************************************************************/
static pwr_tStatus	nmpsbck_free( bck_ctx bckctx)
{
	nmpsbck_t_data_list	*data_ptr;
	nmpsbck_t_data_list	*next_ptr;

	/* Free the object database */
	data_ptr = bckctx->data_list;
	gdh_DLUnrefObjectInfoAll();
	while ( data_ptr != NULL)
	{
	  next_ptr = data_ptr->next_ptr;
	  free( data_ptr);
	  data_ptr = next_ptr;
	}

	if ( bckctx->buffer)
	  free( bckctx->buffer);

	/* Free the context */
	free( bckctx);
	return NMPS__SUCCESS;
}

int main()
{
	bck_ctx		bckctx;
	pwr_tStatus	sts;
	float		scantime;
	int		full_scan;
	int		count;
	int		first_scan = 1;
	int		file_reopen = 0;
	qcom_sQid 	qid = qcom_cNQid;
	qcom_sQattr 	qAttr;
	qcom_sQid 	qini;
	int		tmo;
	int		swap = 0;
	char 		mp[2000];
	qcom_sGet 	get;

	errh_Init("rs_nmps_bck", 0);

	if (!qcom_Init(&sts, 0, "rs_nmps_bck")) {
	  errh_Fatal("qcom_Init, %m", sts);
	  exit(sts);
	} 

	qAttr.type = qcom_eQtype_private;
	qAttr.quota = 100;
	if (!qcom_CreateQ(&sts, &qid, &qAttr, "events")) {
	  errh_Fatal("qcom_CreateQ, %m", sts);
	  exit(sts);
	} 

	qini = qcom_cQini;
	if (!qcom_Bind(&sts, &qid, &qini)) {
	  errh_Fatal("qcom_Bind(Qini), %m", sts);
	  exit(-1);
	}

	sts = gdh_Init("rs_nmps_bck");
	if (EVEN(sts)) LogAndExit( sts);

	for (;;) {
	  bckctx = calloc( 1 , sizeof( *bckctx));
	  if ( bckctx == 0 ) return NMPS__NOMEMORY;

	  bckctx->file_num = 1;

	  sts = nmps_get_bckconfig( bckctx);
	  if (EVEN(sts)) LogAndExit( sts);

	  if ( !bckctx->bckconfig->NoRead) {
	    /* Restore the old backup file */
	    sts = nmpsbck_read( bckctx);
	  }
	  else {
	    /* Release the cells by setting backup done flag */
	    sts = nmpsbck_set_cell_backup_done();
	    if (EVEN(sts)) LogAndExit( sts);
	  }

	  sts = nmps_cell_init( bckctx);
	  if (EVEN(sts)) LogAndExit( sts);

	  sts = nmpsbck_get_cellbuffer( bckctx);
	  if (EVEN(sts)) LogAndExit( sts);

	  scantime = bckctx->bckconfig->IncrementCycleTime;
	  full_scan = bckctx->bckconfig->FullCycleTime / scantime + FLT_EPSILON;
	  tmo = (int) scantime * 1000;

	  count = 0;
	  for (;;) {
	    get.maxSize = sizeof(mp);
	    get.data = mp;
	    qcom_Get(&sts,&qid, &get, tmo);
	    if (sts == QCOM__TMO || sts == QCOM__QEMPTY) {
	      if ( swap)
		continue;

	      if ( bckctx->bckconfig->Initialize) {
		bckctx->bckconfig->Initialize = 0;
		nmpsbck_free( bckctx);
		break;
	      }

	      count++;
	      if ( count > full_scan || first_scan || file_reopen ||
		   bckctx->record_count >= NMPSBCK_MAX_RECORDS - 1 ||
		   bckctx->bckconfig->ForceFullBackup) {
		/* Time for full backup */
		if ( bckctx->bckconfig->ForceFullBackup)
		  bckctx->bckconfig->ForceFullBackup = 0;
		bckctx->increment = 0;
		count = 0;

		/* Toggle file */
		if ( !file_reopen) {
		  if ( bckctx->file_num == 1)
		    bckctx->file_num = 2;
		  else
		    bckctx->file_num = 1;
		}
		file_reopen = 0;
	      }
	      else
		/* Incremental backup */
		bckctx->increment = 1;

	      sts = nmpsbck_cell_handler( bckctx);
	      if ( sts == NMPS__FILEREOPEN) {
		/* New file is opened, write a full backup */
		file_reopen = 1;
		continue;
	      }
	      if ( ODD(sts))
		bckctx->bckconfig->LoopCount++;

#if 0
	      else if (EVEN(sts)) LogAndExit( sts);
#endif


	      // sutl_sleep( scantime);

	      first_scan = 0;
	    }
	    else {
	      ini_mEvent  new_event;
	      qcom_sEvent *ep = (qcom_sEvent*) get.data;

	      new_event.m  = ep->mask;
	      if (new_event.b.oldPlcStop && !swap) {
		swap = 1;
		nmpsbck_free( bckctx);
		break;
	      } 
	      else if (new_event.b.swapDone && swap) {
		swap = 0;
	      } 
	      else if (new_event.b.terminate) {
		exit(0);
	      }
	    }
	  }
	}
}




