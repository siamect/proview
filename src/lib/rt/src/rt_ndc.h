/* 
 * Proview   $Id: rt_ndc.h,v 1.4 2005-09-01 14:57:56 claes Exp $
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

#ifndef rt_ndc_h
#define rt_ndc_h
/* rt_ndc.h -- Network data conversion. */

#ifndef pwr_h
# include "pwr.h"
#endif

#ifndef pwr_class_h
# include "pwr_class.h"
#endif

#ifndef rt_conv_h
#include "rt_conv.h"
#endif

#ifndef rt_gdb_h
# include "rt_gdb.h"
#endif

#ifndef rt_mvol_h
# include "rt_mvol.h"
#endif

typedef enum {
  ndc_eOp__ = 0,
  ndc_eOp_encode = 1,
  ndc_eOp_decode = 2,
  ndc_eOp_
} ndc_eOp;


/** Used when converting remote data to native and different classes */
typedef struct {
  conv_eIdx	cidx;	/**< Index in the conversion routine table */
  pwr_tUInt32	raidx; 	/**< Native attribute index */
  /*  pwr_tBoolean  private; */ /**< Set if attribute is a private pointer */
} ndc_sRemoteToNative;





pwr_tBoolean
ndc_ConvertData (
  pwr_tStatus		*sts,
  const gdb_sNode	*np,
  gdb_sClass            *cp,
  const pwr_sAttrRef	*arp,
  void			*tp,	/* Address of target.  */
  const void		*sp,	/* Address of source.  */
  pwr_tUInt32		*size,	/* Size of source.  */
  ndc_eOp		op,
  pwr_tUInt32           offset,
  pwr_tUInt32           offs
);

/**
 * Converts native data that has a different class version.
 * 
 */
pwr_tBoolean
ndc_ConvertNativeToRemoteData (
  pwr_tStatus		*sts,
  const gdb_sCclass	*ccp,	/**< Cached class */
  pwr_tUInt32		ridx,	/**< Attribute index in ccp */
  const mvol_sAttribute *nap,   /**< Native attribute */
  const pwr_sAttrRef	*rarp,  /**< Remote attribute reference */
  const pwr_sAttrRef	*narp,  /**< Native attribute reference */
  void			*tp,	/**< Address of target.  */
  const void		*sp,	/**< Address of source.  */
  pwr_tUInt32		*size,	/**< Size of target buffer.  */
  pwr_tUInt32           offset, /**< Offset in class */
  pwr_tUInt32           toffs,  /**< Offset i ntarget buffer */
  pwr_tUInt32           soffs,  /**< Offset i source buffer */
  pwr_tNodeId           nid     /**< Node id */
);

/**
 * Encodes/decodes data which uses a cached class.
 */
pwr_tBoolean
ndc_ConvertRemoteData (
  pwr_tStatus		*sts,
  const gdb_sNode	*np,
  const gdb_sCclass	*ccp,
  const pwr_sAttrRef	*arp,
  void			*tp,	/* Address of target.  */
  const void		*sp,	/* Address of source.  */
  pwr_tUInt32		*size,	/* Size of source.  */
  ndc_eOp		op,
  pwr_tUInt32           offset, /**< Offset to attribute in class.  */
  pwr_tUInt32           offs    /**< Offset in buffer.  */
);



/**
 * Converts remote data that has a different class version.
 * The data has already been converted to native data format
 */
pwr_tBoolean
ndc_ConvertRemoteToNativeData (
  pwr_tStatus		*sts,
  const gdb_sCclass	*ccp,	/**< Cached class */
  pwr_tUInt32		ridx,	/**< Attribute index in ccp */
  const mvol_sAttribute *nap,   /**< Native attribute */
  const pwr_sAttrRef	*rarp,  /**< Remote attribute reference */
  const pwr_sAttrRef	*narp,  /**< Native attribute reference */
  void			*tp,	/**< Address of target.  */
  const void		*sp,	/**< Address of source.  */
  pwr_tUInt32		*size,	/**< Size of target buffer.  */
  pwr_tUInt32           offset, /**< Offset in class */
  pwr_tUInt32           toffs,  /**< Offset i ntarget buffer */
  pwr_tUInt32           soffs,  /**< Offset i source buffer */
  pwr_tNodeId           nid     /**< Node id */
);

/**
 * Converts remote data that has a different class version.
 * The data has already been converted to native data format
 */
pwr_tBoolean
ndc_ConvertRemoteToNativeTable (
  pwr_tStatus			*sts,
  const gdb_sCclass		*ccp,	 /**< Cached class */
  const ndc_sRemoteToNative	*tbl,
  const pwr_sAttrRef		*rarp,   /**< Remote attribute reference */
  const pwr_sAttrRef		*narp,   /**< Native attribute reference */
  void				*tp,	 /**< Address of target.  */
  const void			*sp,	 /**< Address of source.  */
  pwr_tUInt32			*size,	 /**< Size of target buffer.  */
  pwr_tUInt32			offset,  /**< Offset to attribute in class.  */
  pwr_tUInt32			toffs,	 /**< Offset in target buffer.  */
  pwr_tUInt32			soffs,	 /**< Offset in source buffer.  */
  pwr_tBoolean			*first,	 /**< First scan.  */
  pwr_tNodeId			nid	 /**< Node id  */
  );


/**
 * Converts a native attribute reference to a remote attribute.
 * 
 * @return The argument rarp or NULL if an error
 */
pwr_sAttrRef *
ndc_NarefToRaref(
  pwr_tStatus 		*sts,   /**< Status */
  mvol_sAttribute	*ap,    /**< Native mvol attribute */
  pwr_sAttrRef	        *narp,  /**< Native attribute reference */ 
  gdb_sCclass	        *ccp,   /**< Cached class */
  pwr_tUInt32		*ridx,  /**< Attribute index in ccp or UINT_LONG if whole object */
  pwr_sAttrRef		*rarp,  /**< Remote attribute reference */
  pwr_tBoolean		*equal, /**< Set if the attribute references are equal, not checked if whole object */
  cdh_sParseName        *pn,    /**< Not NULL if called from Get-/SetObjectInfo */
  gdb_sCclass           *ccpLocked,
  gdb_sVolume           *vp,
  gdb_sNode             *np
);

ndc_sRemoteToNative *
ndc_UpdateRemoteToNativeTable(
  pwr_tStatus		*sts, 
  ndc_sRemoteToNative	*tbl,  
  pwr_tUInt32		tcnt, /**< # table entries */ 
  const gdb_sClass	*cp,
  const gdb_sCclass	*ccp,
  pwr_tNodeId           nid
  );


#endif
		
