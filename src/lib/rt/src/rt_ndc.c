/* rt_ndc.c -- Network data conversion.

   PROVIEW/R
   Copyright (C) 1997 by Comator Process AB.

   .  */

#ifdef	OS_ELN
# include $vaxelnc
#else
# include <stdio.h>
# include <string.h>
#endif

#include "pwr.h"
#include "pwr_class.h"
#include "rt_gdh_msg.h"
#include "rt_errh.h"
#include "rt_pool.h"
#include "rt_vol.h"
#include "rt_net.h"
#include "rt_cvol.h"
#include "rt_cvolc.h"
#include "rt_ndc.h"
#include "rt_ndc_msg.h"


/* Vax f-float on a little endian machine.


   3             2 2             1 1 1           0 0 0           0
   1             4 3             6 5 4           8 7 6           0
  +---------------+---------------+-+-------------+-+-------------+
  |       fraction 15 <- 0        |s|  exponent   : | f 22 <- 16  |
  +---------------+---------------+-+-------------+-+-------------+
   1             0 0             0   0           0 0 2           1
   5             8 7             0   7           1 0 2           6


*/

union vax_f_le {
  unsigned int	i;
  struct {
    unsigned int    f22_16	:  7;
    unsigned int    exp		:  8;
    unsigned int    sign	:  1;
    unsigned int    f15_0	: 16;
  } b;
};

/* Vax f-float on a big endian machine.



  +-+-------------+   +-+-------------+   +---------------+   +---------------+
  : | f 22 <- 16  |   |s|  exponent   :   : 15 <- 0       |   |      fraction :
  +-+-------------+   +-+-------------+   +---------------+   +---------------+
   0 2           1       0           0     0             0     1             0
   0 2           6       7           1     7             0     5             8

   0             0 0             1 1 1           2 2             3
   0             7 8             5 6 7           3 4             1
  +---------------+---------------+-+-------------+-+-------------+
  |       fraction 0 -> 15        |s|  exponent   : | f 16 -> 22  |
  +---------------+---------------+-+-------------+-+-------------+
   0             0 0             1   0             0 1           2
   0             7 8             5   0             7 6           2

*/

union vax_f_be {
  unsigned int	i;
  struct {
    unsigned int    f0_15	: 16;
    unsigned int    sign	:  1;
    unsigned int    exp		:  8;
    unsigned int    f16_22	:  7;
  } b;
};

/* IEEE single on a little endian machine.

   3             2 2             1 1             0 0             0
   1             4 3             6 5             8 7             0
  +-+-------------+-+-------------+---------------+---------------+
  |s|  exponent   : |        fraction 22 <- 0     :               |
  +-+-------------+-+-------------+---------------+---------------+
     0             0 2           1 1                             0
     7             0 2           6 5                             0

*/

union i3e_s_le {
  unsigned int	i;
  struct {
    unsigned int    f22_0	: 23;
    unsigned int    exp		:  8;
    unsigned int    sign	:  1;
  } b;
  struct {
    unsigned int    f15_0	: 16;
    unsigned int    f22_16	:  7;
    unsigned int    exp		:  8;
    unsigned int    sign	:  1;
  } v;
};

/* IEEE single on a big endian machine.

   0             0 0             1 1             2 2             3
   0             7 8             5 6             3 4             1
  +-+-------------+-+-------------+---------------+---------------+
  |s|  exponent   : |        fraction 0 -> 22     :               |
  +-+-------------+-+-------------+---------------+---------------+
     0           0 0 0           0 0                             2
     0           6 7 0           6 7                             2

*/

union i3e_s_be {
  unsigned int	i;
  struct {
    unsigned int    sign	:  1;
    unsigned int    exp		:  8;
    unsigned int    f0_22	: 23;
  } b;
  struct {
    unsigned int    sign	:  1;
    unsigned int    exp		:  8;
    unsigned int    f0_6	:  7;
    unsigned int    f7_22	: 16;
  } v;
};

#define VAX_F_BIAS    0x81
#define I3E_S_BIAS    0x7f
#define VAX_D_BIAS    0x81
#define VAX_G_BIAS    0x401
#define I3E_D_BIAS    0x3ff

#define IBYTE0(i) ((i >> 0x18) & 0x000000ff) 
#define IBYTE1(i) ((i >> 0x08) & 0x0000ff00) 
#define IBYTE2(i) ((i << 0x08) & 0x00ff0000) 
#define IBYTE3(i) ((i << 0x18) & 0xff000000) 

#define ENDIAN_SWAP_INT(t, s)\
  {int i = *(int *)s; *(int *)t = (IBYTE0(i) | IBYTE1(i) | IBYTE2(i) | IBYTE3(i));}

#define SBYTE0(s) ((s >> 0x08) & 0x00ff) 
#define SBYTE1(s) ((s << 0x08) & 0xff00) 

#define ENDIAN_SWAP_SHORT(t, s)\
  {short int i = *(short *)s; *(short *)t = (SBYTE0(i) | SBYTE1(i));}

#if 0
# define ENDIAN_SWAP_BOOL(t, s) (*(int *)t = (0 != *(int *)s))
#else
# define ENDIAN_SWAP_BOOL(t, s) ENDIAN_SWAP_INT(t, s)
#endif



#ifndef __powerpc__
/* .  */

static pwr_tBoolean
decode_aref (
  int	count,
  int	asize,
  char	*tp,
  char	*sp,
  int	*size
)
{

  for (; count > 0 && *size >= sizeof(pwr_sAttrRef); count--) {
    ENDIAN_SWAP_INT(tp, sp);
    tp += sizeof(int);
    sp += sizeof(int);
    ENDIAN_SWAP_INT(tp, sp);
    tp += sizeof(int);
    sp += sizeof(int);

    ENDIAN_SWAP_INT(tp, sp);
    tp += sizeof(int);
    sp += sizeof(int);

    ENDIAN_SWAP_INT(tp, sp);
    tp += sizeof(int);
    sp += sizeof(int);

    ENDIAN_SWAP_INT(tp, sp);
    tp += sizeof(int);
    sp += sizeof(int);

    ENDIAN_SWAP_INT(tp, sp);
    tp += sizeof(int);
    sp += sizeof(int);

    *size -= sizeof(pwr_sAttrRef);
  }

  return TRUE;
}    

/* .  */

static pwr_tBoolean
decode_bool (
  int	count,
  int	asize,
  char	*tp,
  char	*sp,
  int	*size
)
{

  for (; count > 0 && *size >= sizeof(int); count--) {
    ENDIAN_SWAP_BOOL(tp, sp);
    tp += sizeof(int);
    sp += sizeof(int);
    *size -= sizeof(int);
  }

  return TRUE;
}    
#endif


/* .  */

static pwr_tBoolean
decode_copy (
  int	count,
  int	asize,
  char	*tp,
  char	*sp,
  int	*size
)
{

  if (tp != sp)
    memcpy(tp, sp, MIN(asize, *size));
  *size = (*size >= asize ? *size - asize : 0);
  
  return TRUE;
}    

/* .  */

static pwr_tBoolean
decode_null (
  int	count,
  int	asize,
  char	*tp,
  char	*sp,
  int	size
)
{

  return FALSE;
}    

#if defined(OS_VMS) || defined(OS_ELN)
/* .  */

static pwr_tBoolean
encode_sfloat (
  int	count,
  int	asize,
  char	*tp,
  char	*sp,
  int	*size
)
{
  union vax_f_le	*vp;
  union i3e_s_le	i3e;

  for (; count > 0 && *size >= sizeof(float); count--) {
    vp = ((union vax_f_le *)sp);
    
    if (vp->b.f22_16 == 0x7f && vp->b.exp == 0xff && vp->b.f15_0 == 0xffff) {  /* High value.  */
      i3e.i = 0, i3e.v.exp = 0xff;
    } else if (vp->b.f22_16 == 0 && vp->b.exp == 0 && vp->b.f15_0 == 0) {  /* Low value.  */
      i3e.i = 0;
    } else {
      i3e.v.exp	    = vp->b.exp - VAX_F_BIAS + I3E_S_BIAS;
      i3e.v.f15_0   = vp->b.f15_0;
      i3e.v.f22_16  = vp->b.f22_16;
    }

    i3e.b.sign = vp->b.sign;

    ENDIAN_SWAP_INT(tp, &i3e.i);
    tp += sizeof(float);
    sp += sizeof(float);
    *size -= sizeof(float);
  }

  return TRUE;
}    

#elif (defined(OS_LYNX) || defined(OS_LINUX)) && defined(HW_X86)

static pwr_tBoolean
encode_sfloat (
  int	count,
  int	asize,
  char	*tp,
  char	*sp,
  int	*size
)
{

  for (; count > 0 && *size >= sizeof(float); count--) {
    ENDIAN_SWAP_INT(tp, sp);
    tp += sizeof(float);
    sp += sizeof(float);
    *size -= sizeof(float);
  }

  return TRUE;
}
#endif

#if defined(OS_VMS) || defined(OS_ELN)
/* .  */

static pwr_tBoolean
decode_sfloat (
  int	count,
  int	asize,
  char	*tp,
  char	*sp,
  int	*size
)
{
  union vax_f_le	*vp;
  union i3e_s_le	i3e;

  for (; count > 0 && *size >= sizeof(float); count--) {
    vp = ((union vax_f_le *)tp);
    ENDIAN_SWAP_INT(&i3e.i, sp);
    
    /* PPC some times sets the sign bit for zero (-0), which isn't valid
     * for f-float. ML 971025
     */ 
    if (i3e.i == 0x80000000)
      i3e.i = 0; /* Clear sign bit */

    if (i3e.b.f22_0 == 0x0 && i3e.b.exp == 0xff) {  /* High value.  */
      vp->b.f22_16 = 0x7f;
      vp->b.exp	   = 0xff;
      vp->b.f15_0  = 0xffff;
    } else if (i3e.b.f22_0 == 0x0 && i3e.b.exp == 0x00) {  /* Low value.  */
      vp->i = 0;
    } else {
      vp->b.exp	   = i3e.v.exp - I3E_S_BIAS + VAX_F_BIAS;
      vp->b.f22_16 = i3e.v.f22_16;
      vp->b.f15_0  = i3e.v.f15_0;
    }

    vp->b.sign = i3e.b.sign;

    tp += sizeof(float);
    sp += sizeof(float);
    *size -= sizeof(float);
  }

  return TRUE;
}    

#elif (defined(OS_LYNX) || defined(OS_LINUX)) && defined(HW_X86)

static pwr_tBoolean
decode_sfloat(
  int	count,
  int	asize,
  char	*tp,
  char	*sp,
  int	*size
)
{
  for (; count > 0 && *size >= sizeof(float); count--) {
    ENDIAN_SWAP_INT(tp, sp);
    tp += sizeof(float);
    sp += sizeof(float);
    *size -= sizeof(float);
  }

  return TRUE;
}
#endif

#ifndef __powerpc__
/* .  */

static pwr_tBoolean
decode_int (
  int	count,
  int	asize,
  char	*tp,
  char	*sp,
  int	*size
)
{

  for (; count > 0 && *size >= sizeof(int); count--) {
    ENDIAN_SWAP_INT(tp, sp);
    tp += sizeof(int);
    sp += sizeof(int);
    *size -= sizeof(int);
  }

  return TRUE;
}    
#endif

#ifndef __powerpc__
/* .  */

static pwr_tBoolean
decode_2_int (
  int	count,
  int	asize,
  char	*tp,
  char	*sp,
  int	*size
)
{

  for (count *= 2; count > 0 && *size >= sizeof(int); count--) {
    ENDIAN_SWAP_INT(tp, sp);
    tp += sizeof(int);
    sp += sizeof(int);
    *size -= sizeof(int);
  }

  return TRUE;
}    
#endif

#ifndef __powerpc__
/* Convert a short integer.

  NOTA BENE   A short occupies 4 bytes in the Proview rtdb
              and thus this routine is not currently used.  */

static pwr_tBoolean
decode_short (
  int	count,
  int	asize,
  char	*tp,
  char	*sp,
  int	*size
)
{

  for (; count > 0 && *size >= sizeof(short); count--) {
    ENDIAN_SWAP_SHORT(tp, sp);
    tp += sizeof(short);
    sp += sizeof(short);
    *size -= sizeof(short);
  }

  return TRUE;
}    
#endif

#ifndef __powerpc__
/* Convert a tiny integer (8 bits).

  NOTA BENE   A tiny occupies 4 bytes in the Proview rtdb
              and thus this routine is not currently used.  */

static pwr_tBoolean
decode_tiny (
  int	count,
  int	asize,
  char	*tp,
  char	*sp,
  int	*size
)
{

  for (; count > 0 && *size >= sizeof(char); count--) {
    *tp++ = *sp++;
    (*size)--;
  }

  return TRUE;
}    
#endif

#ifndef __powerpc__
static pwr_tBoolean (*decode[pwr_eTix_])() = {
  decode_null,		/* pwr_eTix__		*/
  decode_bool,		/* pwr_eTix_Boolean	*/
  decode_sfloat,	/* pwr_eTix_Float32	*/
  decode_copy,		/* pwr_eTix_Float64	*/
  decode_tiny,		/* pwr_eTix_Char	*/
  decode_tiny,		/* pwr_eTix_Int8	*/
  decode_short,		/* pwr_eTix_Int16	*/
  decode_int,		/* pwr_eTix_Int32	*/
  decode_tiny,		/* pwr_eTix_UInt8	*/
  decode_short,		/* pwr_eTix_UInt16	*/
  decode_int,		/* pwr_eTix_UInt32	*/
  decode_2_int,		/* pwr_eTix_Objid	*/
  decode_copy,		/* pwr_eTix_Buffer	*/
  decode_copy,		/* pwr_eTix_String	*/
  decode_int,		/* pwr_eTix_Enum	*/
  decode_null,		/* pwr_eTix_Struct	*/
  decode_int,		/* pwr_eTix_Mask	*/
  decode_null,		/* pwr_eTix_Array	*/
  decode_2_int,		/* pwr_eTix_Time	*/
  decode_copy,		/* pwr_eTix_Text	*/
  decode_aref,		/* pwr_eTix_AttrRef	*/
  decode_2_int,		/* pwr_eTix_UInt64	*/
  decode_2_int,		/* pwr_eTix_Int64	*/
  decode_int,		/* pwr_eTix_ClassId	*/
  decode_int,		/* pwr_eTix_TypeId	*/
  decode_int,		/* pwr_eTix_VolumeId	*/
  decode_int,		/* pwr_eTix_ObjectIx	*/
  decode_2_int,		/* pwr_eTix_RefId	*/
  decode_2_int,		/* pwr_eTix_DeltaTime	*/
};
static pwr_tBoolean (*encode[pwr_eTix_])() = {
  decode_null,		/* pwr_eTix__		*/
  decode_bool,		/* pwr_eTix_Boolean	*/
  encode_sfloat,	/* pwr_eTix_Float32	*/
  decode_null,		/* pwr_eTix_Float64	*/
  decode_tiny,		/* pwr_eTix_Char	*/
  decode_tiny,		/* pwr_eTix_Int8	*/
  decode_short,		/* pwr_eTix_Int16	*/
  decode_int,		/* pwr_eTix_Int32	*/
  decode_tiny,		/* pwr_eTix_UInt8	*/
  decode_short,		/* pwr_eTix_UInt16	*/
  decode_int,		/* pwr_eTix_UInt32	*/
  decode_2_int,		/* pwr_eTix_Objid	*/
  decode_copy,		/* pwr_eTix_Buffer	*/
  decode_copy,		/* pwr_eTix_String	*/
  decode_int,		/* pwr_eTix_Enum	*/
  decode_null,		/* pwr_eTix_Struct	*/
  decode_int,		/* pwr_eTix_Mask	*/
  decode_null,		/* pwr_eTix_Array	*/
  decode_2_int,		/* pwr_eTix_Time	*/
  decode_copy,		/* pwr_eTix_Text	*/
  decode_aref,		/* pwr_eTix_AttrRef	*/
  decode_2_int,		/* pwr_eTix_UInt64	*/
  decode_2_int,		/* pwr_eTix_Int64	*/
  decode_int,		/* pwr_eTix_ClassId	*/
  decode_int,		/* pwr_eTix_TypeId	*/
  decode_int,		/* pwr_eTix_VolumeId	*/
  decode_int,		/* pwr_eTix_ObjectIx	*/
  decode_2_int,		/* pwr_eTix_RefId	*/
  decode_2_int,		/* pwr_eTix_DeltaTime	*/
};
#else
static pwr_tBoolean (*decode[pwr_eTix_])() = {
  decode_null,		/* pwr_eTix__		*/
  decode_copy,		/* pwr_eTix_Boolean	*/
  decode_copy,		/* pwr_eTix_Float32	*/
  decode_copy,		/* pwr_eTix_Float64	*/
  decode_copy,		/* pwr_eTix_Char	*/
  decode_copy,		/* pwr_eTix_Int8	*/
  decode_copy,		/* pwr_eTix_Int16	*/
  decode_copy,		/* pwr_eTix_Int32	*/
  decode_copy,		/* pwr_eTix_UInt8	*/
  decode_copy,		/* pwr_eTix_UInt16	*/
  decode_copy,		/* pwr_eTix_UInt32	*/
  decode_copy,		/* pwr_eTix_Objid	*/
  decode_copy,		/* pwr_eTix_Buffer	*/
  decode_copy,		/* pwr_eTix_String	*/
  decode_copy,		/* pwr_eTix_Enum	*/
  decode_null,		/* pwr_eTix_Struct	*/
  decode_copy,		/* pwr_eTix_Mask	*/
  decode_null,		/* pwr_eTix_Array	*/
  decode_copy,		/* pwr_eTix_Time	*/
  decode_copy,		/* pwr_eTix_Text	*/
  decode_copy,		/* pwr_eTix_AttrRef	*/
  decode_copy,		/* pwr_eTix_UInt64	*/
  decode_copy,		/* pwr_eTix_Int64	*/
  decode_copy,		/* pwr_eTix_ClassId	*/
  decode_copy,		/* pwr_eTix_TypeId	*/
  decode_copy,		/* pwr_eTix_VolumeId	*/
  decode_copy,		/* pwr_eTix_ObjectIx	*/
  decode_copy,		/* pwr_eTix_RefId	*/
  decode_copy,		/* pwr_eTix_DeltaTime	*/
};
static pwr_tBoolean (*encode[pwr_eTix_])() = {
  decode_null,		/* pwr_eTix__		*/
  decode_copy,		/* pwr_eTix_Boolean	*/
  decode_copy,		/* pwr_eTix_Float32	*/
  decode_null,		/* pwr_eTix_Float64	*/
  decode_copy,		/* pwr_eTix_Char	*/
  decode_copy,		/* pwr_eTix_Int8	*/
  decode_copy,		/* pwr_eTix_Int16	*/
  decode_copy,		/* pwr_eTix_Int32	*/
  decode_copy,		/* pwr_eTix_UInt8	*/
  decode_copy,		/* pwr_eTix_UInt16	*/
  decode_copy,		/* pwr_eTix_UInt32	*/
  decode_copy,		/* pwr_eTix_Objid	*/
  decode_copy,		/* pwr_eTix_Buffer	*/
  decode_copy,		/* pwr_eTix_String	*/
  decode_copy,		/* pwr_eTix_Enum	*/
  decode_null,		/* pwr_eTix_Struct	*/
  decode_copy,		/* pwr_eTix_Mask	*/
  decode_null,		/* pwr_eTix_Array	*/
  decode_copy,		/* pwr_eTix_Time	*/
  decode_copy,		/* pwr_eTix_Text	*/
  decode_copy,		/* pwr_eTix_AttrRef	*/
  decode_copy,		/* pwr_eTix_UInt64	*/
  decode_copy,		/* pwr_eTix_Int64	*/
  decode_copy,		/* pwr_eTix_ClassId	*/
  decode_copy,		/* pwr_eTix_TypeId	*/
  decode_copy,		/* pwr_eTix_VolumeId	*/
  decode_copy,		/* pwr_eTix_ObjectIx	*/
  decode_copy,		/* pwr_eTix_RefId	*/
  decode_copy,		/* pwr_eTix_DeltaTime	*/
};
#endif

/**
 * @note There is no support for double. 
 *       if we want to implement it we have too look at the OS to decide what 
 *       format to use. Because for backward compatibility is the float format set 
 *       to vaxF for VAX to AXP and not vaxD and vaxG.
 */

pwr_tBoolean
ndc_ConvertData (
  pwr_tStatus		*sts,
  const gdb_sNode	*np,
  const pwr_sAttrRef	*arp,
  void			*tp,	/* Address of target.  */
  const void		*sp,	/* Address of source.  */
  pwr_tUInt32		size,	/* Size of source.  */
  ndc_eOp		op
)
{
  gdb_sClass		*cp;
  cdh_uTypeId		cid;
  int			i;
  int			base;
  gdb_sAttribute	*ap;

 /* The new way, convert if different co_mFormat
  * The old way, always convert if different OS 
  */

  if ((np->netver >= net_cFirstCclassVersion && np->fm.m == gdbroot->my_node->fm.m) 
      || (np->netver < net_cFirstCclassVersion && 
          np->os == gdbroot->my_node->os && np->fm.b.bo == gdbroot->my_node->fm.b.bo)) {
      if (tp != sp)
        memcpy(tp, sp, size);
      return TRUE;
  }


  cid.pwr = arp->Body;
  cid.c.bix = 0;	/* To get the class id.  */

  cp = hash_Search(sts, gdbroot->cid_ht, &cid.pwr);
  if (cp == NULL) return FALSE;
  
  /* Find attribute.  */

  for (i = 0, ap = cp->attr; i < cp->acount; i++, ap++)
    if (arp->Offset <= ap->moffset)
      break;

  if (i >= cp->acount) pwr_Return(NO, sts, NDC__OFFSET);

  if (arp->Offset == 0)
    base = 0;
  else
    base = ap->offs;

  switch (op) {
  case ndc_eOp_encode:
    for (; i < cp->acount && size > 0; i++, ap++) {
      if(!encode[pwr_Tix(ap->type)](ap->elem, ap->size, (char *)tp + (ap->offs - base),
	(char *)sp + (ap->offs - base), &size))
	pwr_Return(NO, sts, NDC__CONVERT);
    }
    break;
  case ndc_eOp_decode:
    for (; i < cp->acount && size > 0; i++, ap++) {
      if(!decode[pwr_Tix(ap->type)](ap->elem, ap->size, (char *)tp + (ap->offs - base),
	(char *)sp + (ap->offs - base), &size))
	pwr_Return(NO, sts, NDC__CONVERT);
    }
    break;
  default:
    {
      char ebuf[80];
      sprintf(ebuf, "unknown op: %d", op);
      errh_Bugcheck(NDC__OP, ebuf);
    }
    break;
  }

  pwr_Return(YES, sts, NDC__SUCCESS);
}

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
)
{
  const net_sCattribute	*cap;
  conv_eIdx		cidx;
  int			i,j;
  int 			tasize,sasize;
  pwr_mAdef		adef;
  
  gdb_AssumeUnlocked;

  if (nap->aop == NULL) { /* whole object */
    const gdb_sClass		*cp;
    const gdb_sAttribute	*ap;
    

    cp = hash_Search(sts, gdbroot->cid_ht, &ccp->key.cid);
    if (cp == NULL) 
      errh_Bugcheck(GDH__WEIRD, "can't find native class");
    

    for (i = 0, cap = ccp->attr; i < ccp->acount && size > 0; i++, cap++) {
      for (j = 0, ap = cp->attr; j < cp->acount; j++, ap++) {
        if (ap->aix == cap->aix) {
          tasize = cap->size / cap->elem;
          sasize = ap->size / ap->elem;

          cidx = conv_GetIdx(ap->type, cap->type);
          if (cidx == conv_eIdx_invalid)
            cidx = conv_eIdx_zero; /* Zero the attribute */

          /* Prevent conversion of pointers in objects. 
           * If we are unlucky we can get a floating point exception.
           */
          adef.m = ap->flags.m;
          adef.b.privatepointer = 1; 
          

          if (!conv_Fctn[cidx](cap->elem, tasize, (char *)tp + cap->offs, (int *)&size, ap->elem, sasize, (const char *)sp + ap->offs, adef)) {
            pwr_Return(NO, sts, NDC__CONVERT);
          }
          break;
        }
      }

      if (j >= cp->acount) {/* the remote attribute doesn't exist locally */
        if (!conv_Fctn[conv_eIdx_zero](cap->elem, tasize, (char *)tp + cap->offs, (int *)&size, 0, 0, 0, adef)) {
          pwr_Return(NO, sts, NDC__CONVERT);
        }
      }
    }
    
    
    
  } else { /* single attribute */

    pwr_Assert(nap->adef != NULL);
    pwr_Assert(ridx < ccp->acount);
    
    cap = &ccp->attr[ridx];
    sasize = nap->size / nap->elem;
    tasize = cap->size / cap->elem;

    cidx = conv_GetIdx(nap->adef->Info.Type, cap->type);
    if (cidx == conv_eIdx_invalid) {
      pwr_Return(NO, sts, NDC__NOCONV);    
    } 

    /* Prevent conversion of pointers if it's not a single pointer. 
     * If we are unlucky we can get a floating point exception.
     */
    adef.m = nap->adef->Info.Flags;
    if (adef.b.array && size > cap->size/cap->elem)
      adef.b.privatepointer = 1; 


    if (!conv_Fctn[cidx](cap->elem, tasize, tp, (int *)&size, 
                       nap->elem, sasize, sp, adef)
    ) {
      pwr_Return(NO, sts, NDC__CONVERT);
    }
    
  }

  pwr_Return(YES, sts, NDC__SUCCESS);

}


/**
 * Encodes/decodes remote data by means of the cached class.
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
)
{
  int			i;
  int			base;
  const net_sCattribute	*cap;


  if (np->fm.m == gdbroot->my_node->fm.m) {
    if (tp != sp)
      memcpy(tp, sp, size);
    return TRUE;
  }


  /* Find attribute.  */

  for (i = 0, cap = ccp->attr; i < ccp->acount; i++, cap++)
    if (arp->Offset <= cap->offs + cap->size - 1) /* maximum attribute offset */
      break;

  if (i >= ccp->acount) pwr_Return(NO, sts, NDC__OFFSET);

  if (arp->Offset == 0)
    base = 0;
  else
    base = cap->offs;

  switch (op) {
  case ndc_eOp_encode:
    for (; i < ccp->acount && size > 0; i++, cap++) {
      if(!encode[pwr_Tix(cap->type)](cap->elem, cap->size, (char *)tp + (cap->offs - base),
	(char *)sp + (cap->offs - base), &size))
	pwr_Return(NO, sts, NDC__CONVERT);
    }
    break;
  case ndc_eOp_decode:
    for (; i < ccp->acount && size > 0; i++, cap++) {
      if(!decode[pwr_Tix(cap->type)](cap->elem, cap->size, (char *)tp + (cap->offs - base),
	(char *)sp + (cap->offs - base), &size))
	pwr_Return(NO, sts, NDC__CONVERT);
    }
    break;
  default:
    {
      char ebuf[80];
      sprintf(ebuf, "unknown op: %d", op);
      errh_Bugcheck(NDC__OP, ebuf);
    }
    break;
  }

  pwr_Return(YES, sts, NDC__SUCCESS);
}

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
)
{
  const net_sCattribute	*cap;
  conv_eIdx		cidx;
  int			i,j;
  int 			tasize,sasize;
  pwr_mAdef		adef;

  gdb_AssumeUnlocked;

  if (nap->aop == NULL) { /* whole object */
    const gdb_sClass		*cp;
    const gdb_sAttribute	*ap;
    

    pwr_Assert(narp->Offset == 0);

    cp = hash_Search(sts, gdbroot->cid_ht, &ccp->key.cid);
    if (cp == NULL) 
      return FALSE;

    ap = cp->attr;

    for (i = 0; i < cp->acount && size > 0; i++, ap++) {
      for (j = 0, cap = ccp->attr; j < ccp->acount; j++, cap++) {
        if (ap->aix == cap->aix) {
          tasize = ap->size / ap->elem;
          sasize = cap->size / cap->elem;

          cidx = conv_GetIdx(cap->type, ap->type);
          if (cidx == conv_eIdx_invalid)
            cidx = conv_eIdx_zero; /* Zero the attribute */

          /* Prevent conversion of pointers in objects. 
           * If we are unlucky we can get a floating point exception.
           */
          adef.m = cap->flags.m;
          adef.b.privatepointer = 1;          

          if (!conv_Fctn[cidx](ap->elem, tasize, tp, (int *)&size, cap->elem, sasize, sp, adef)) {
            pwr_Return(NO, sts, NDC__CONVERT);
          }
        }
      }

      if (j >= ccp->acount) {/* the native attribute doesn't exist remotely */
        if (!conv_Fctn[cidx](ap->elem, tasize, tp, (int *)&size, 0, 0, 0, adef)) {
          pwr_Return(NO, sts, NDC__CONVERT);
        }
      }
    }
    
    
    
  } else { /* single attribute */

    pwr_Assert(nap->adef != NULL);
    pwr_Assert(ridx < ccp->acount);
    
    cap = &ccp->attr[ridx];
    tasize = nap->size / nap->elem;
    sasize = cap->size / cap->elem;

    cidx = conv_GetIdx(cap->type, nap->adef->Info.Type);
    if (cidx == conv_eIdx_invalid) {
      pwr_Return(NO, sts, NDC__NOCONV);    
    } 

    /* Prevent conversion of pointers if it's not a single attribute. 
     * If we are unlucky we can get a floating point exception.
     */
    size = MIN(size, nap->size);
    adef = cap->flags;
    if (adef.b.array && size > nap->size/nap->elem)
      adef.b.privatepointer = 1;


    if (!conv_Fctn[cidx](nap->elem, tasize, tp, (int *)&size, 
                       cap->elem, sasize, sp, adef)
    ) {
      pwr_Return(NO, sts, NDC__CONVERT);
    }
      
  }

  pwr_Return(YES, sts, NDC__SUCCESS);
}

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
)
{
  const gdb_sClass	*cp;
  const gdb_sAttribute	*ap;
  const net_sCattribute	*cap;
  int			i;
  int			base;
  int 			zsize;
  int 			idx;
  int  			relem;
  pwr_tUInt32		roffs;
  pwr_tUInt32		raidx;
  conv_eIdx		cidx;
  pwr_tBoolean		first;
  pwr_mAdef		adef;
  

  gdb_AssumeLocked;

  cp = hash_Search(sts, gdbroot->cid_ht, &ccp->key.cid);
  if (cp == NULL) 
    return FALSE;

  /* Find attribute.  */

  for (i = 0, ap = cp->attr; i < cp->acount; i++, ap++)
    if (narp->Offset <= ap->moffset)
      break;

  if (i >= cp->acount) pwr_Return(NO, sts, NDC__OFFSET);

  if (narp->Offset == 0)
    base = 0;
  else
    base = ap->offs;


  for (first = 1; i < cp->acount && size > 0; i++, ap++) {
    cidx = tbl[i].cidx;
    raidx =  tbl[i].raidx;
    pwr_Assert(raidx < ccp->acount || raidx == ULONG_MAX);    

    if ( raidx == ULONG_MAX || cidx == conv_eIdx_invalid) {
      /* Attribute doesn't exist on remote node or there is no valid conversion
       * Zero the local attribute 
       */
      zsize = MIN(size, ap->size);
      memset((char *)tp + (ap->offs - base), 0, zsize);
      size -= zsize;
      if (first)
        first = 0;
    } else {
      cap = &ccp->attr[raidx];

      /** @note Pointers are only handled correctly for a single pointer, 
       * not arrays. See, vol_AttributeToAddress. Set private for all other
       * cases. 
       * It's quite tricky to find out if it's a single array element. Let's 
       * hope that the size has the exact size of one element. Maybe we should
       * add a flag to the attribute reference that indicates single array element.
       */
      adef = cap->flags;
      if (!first || (adef.b.array && size > ap->size/ap->elem))
        adef.b.privatepointer = 1; /* prevent floating point exceptions */


      if (first) {
        first = 0;
        roffs = 0;

        /* Check if the first attribute is an array element with index > 0 
         * and that the index exist in the remote attribute.
         */
        if (base != 0 && narp->Offset > ap->offs) {        
          pwr_Assert(ap->elem > 1);

          idx = (narp->Offset - ap->offs) / (ap->size/ap->elem);

          /* Calm down, the convert routine will only use the source if relem > 0 */
          relem = cap->elem  - idx; 
        } else
          relem = cap->elem;

      } else {
        roffs = cap->offs;
        relem = cap->elem;
      }
      


      if(!conv_Fctn[cidx](ap->elem, ap->size/ap->elem, (char *)tp + (ap->offs - base), (int *)&size,
                        relem, cap->size/cap->elem, (const char *)sp + roffs, adef))
        pwr_Return(NO, sts, NDC__CONVERT);      
      
    }
  }

  pwr_Return(YES, sts, NDC__SUCCESS);
}

/**
 * Converts a native attribute reference to a remote attribute reference.
 * 
 * @return The argument rarp or NULL if an error
 */
pwr_sAttrRef *
ndc_NarefToRaref(
  pwr_tStatus 		*sts,  /**< Status */
  const mvol_sAttribute	*ap,   /**< Native mvol attribute */
  const pwr_sAttrRef	*narp, /**< Native attribute reference */ 
  const gdb_sCclass	*ccp,  /**< Cached class */
  pwr_tUInt32		*ridx, /**< Attribute index in ccp or UINT_LONG if whole object */
  pwr_sAttrRef		*rarp, /**< Remote attribute reference */
  pwr_tBoolean		*equal /**< Set if the attribute references are equal, not checked if whole object */
)
{
  pwr_tUInt32		i;
  const net_sCattribute	*cap;


  gdb_AssumeLocked;

  *sts = GDH__SUCCESS;
  *equal = 0;  
  *ridx = ULONG_MAX;

  if (ap->aop == NULL) { /* whole object */
    *rarp = *narp;
    rarp->Size = ccp->size;
    return rarp;
  }
  

  /* It's a single attribute */

  pwr_Assert(ap->aix != ULONG_MAX);

  for (i = 0, cap = ccp->attr; i < ccp->acount; i++, cap++) {      

    if (ap->aix == cap->aix) {
      pwr_Assert(ap->adef != NULL);
      *ridx = i;

      if (ap->idx == ULONG_MAX) {
        if (ap->adef->Info.Type == cap->type && 
            ap->offs            == cap->offs &&
            ap->size            == cap->size &&
            ap->elem            == cap->elem &&
            ap->flags.b.Indirect == (cap->flags.b.pointer && !cap->flags.b.privatepointer)
        ) {
          *rarp = *narp;
          *equal = 1;
        } else {
          rarp->Objid = narp->Objid;
          rarp->Body = narp->Body;
          rarp->Offset = cap->offs;
          rarp->Size = cap->size;
          rarp->Flags.b.Indirect = (cap->flags.b.pointer && !cap->flags.b.privatepointer);
        } 
      } else { /* It's an array element */

          
        if (ap->adef->Info.Type     == cap->type && 
            ap->adef->Info.Offset   == cap->offs &&
            ap->adef->Info.Size     == cap->size &&
            ap->adef->Info.Elements == cap->elem &&
            ap->flags.b.Indirect    == (cap->flags.b.pointer && !cap->flags.b.privatepointer)
        ) {
          *rarp = *narp;
          *equal = 1;
        } else {

          if (ap->idx >= cap->elem) {
            *sts = NDC__NRELEM_IDX;
            return NULL;
          }            

          rarp->Objid = narp->Objid;
          rarp->Body = narp->Body;
          rarp->Size = cap->size / cap->elem;
          rarp->Offset = cap->offs + rarp->Size * ap->idx;
          rarp->Flags.b.Indirect = (cap->flags.b.pointer && !cap->flags.b.privatepointer);
        }
      }
      return rarp;
    }
  } /* for acount */

  /* Attribute doesn't exist on remote node */
  *sts = NDC__NRATTRIBUTE;
  return NULL;
}

ndc_sRemoteToNative *
ndc_UpdateRemoteToNativeTable(
  pwr_tStatus		*sts, 
  ndc_sRemoteToNative	*tbl,  
  pwr_tUInt32		tcnt, /**< # table entries */ 
  const gdb_sClass	*cp,
  const gdb_sCclass	*ccp
  )
{
  const gdb_sAttribute	*ap;
  const net_sCattribute	*cap;
  int			i,j;


  if (tcnt < cp->acount) {
    *sts = NDC__BUFOVRUN;
    return NULL;
  }
  


  for (i = 0, ap = cp->attr; i < cp->acount; i++, ap++) {
    for (j = 0, cap = ccp->attr; j < ccp->acount; j++, cap++) {
      if (ap->aix == cap->aix) {
        tbl[i].cidx = conv_GetIdx(cap->type, ap->type);
        tbl[i].raidx = j;
        break;
      }
    }

    if (j >= ccp->acount) {
      tbl[i].cidx = conv_eIdx_invalid;
      tbl[i].raidx = ULONG_MAX;
    } 
  }

  *sts = NDC__SUCCESS;
  
  return tbl;
  
}

