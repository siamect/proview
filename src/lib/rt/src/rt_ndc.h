#ifndef rt_ndc_h
#define rt_ndc_h
/* rt_ndc.h -- Network data conversion.

   PROVIEW/R
   Copyright (C) 1997 by Comator Process AB.

   .  */

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
  const pwr_sAttrRef	*arp,
  void			*tp,	/* Address of target.  */
  const void		*sp,	/* Address of source.  */
  pwr_tUInt32		size,	/* Size of source.  */
  ndc_eOp		op
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
  pwr_tUInt32		size	/**< Size of target buffer.  */
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
  pwr_tUInt32		size,	/* Size of source.  */
  ndc_eOp		op
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
  pwr_tUInt32		size	/**< Size of target buffer.  */
);

/**
 * Converts remote data that has a different class version.
 * The data has already been converted to native data format
 */
pwr_tBoolean
ndc_ConvertRemoteToNativeTable (
  pwr_tStatus			*sts,
  const gdb_sCclass		*ccp,	/**< Cached class */
  const ndc_sRemoteToNative	*tbl,
  const pwr_sAttrRef		*rarp,  /**< Remote attribute reference */
  const pwr_sAttrRef		*narp,  /**< Native attribute reference */
  void				*tp,	/**< Address of target.  */
  const void			*sp,	/**< Address of source.  */
  pwr_tUInt32			size	/**< Size of target buffer.  */
  );


/**
 * Converts a native attribute reference to a remote attribute.
 * 
 * @return The argument rarp or NULL if an error
 */
pwr_sAttrRef *
ndc_NarefToRaref(
  pwr_tStatus 		*sts,  /**< Status */
  const mvol_sAttribute	*ap,   /**< Native mvol attribute */
  const pwr_sAttrRef	*narp, /**< Native attribute reference */ 
  const gdb_sCclass	*ccp,  /**< Cached class */
  pwr_tUInt32		*ridx, /**< Attribute index in ccp or UINT_LONG if whole object*/
  pwr_sAttrRef		*rarp, /**< Remote attribute reference */
  pwr_tBoolean		*equal /**< Set if the attribute references are equal */
);

ndc_sRemoteToNative *
ndc_UpdateRemoteToNativeTable(
  pwr_tStatus		*sts, 
  ndc_sRemoteToNative	*tbl,  
  pwr_tUInt32		tcnt, /**< # table entries */ 
  const gdb_sClass	*cp,
  const gdb_sCclass	*ccp
  );


#endif
		
