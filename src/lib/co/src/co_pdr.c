/* co_pdr.c -- pdr routines

   PROVIEW/R
   Copyright (C) 2002 by Cell Network AB.

   This module contains the PDR-routines */

#include <string.h>

#include "co_pdr.h"
#include "co_float.h"


#include <netinet/in.h>




#define PWR_PDR_BYTES(pdrs, addr, len)				\
{								\
  if (((pdrs)->x_handy -= len) < 0)				\
    return (FALSE);						\
                                                                \
  if ((caddr_t)addr != (pdrs)->x_private) {                     \
    if ((pdrs)->x_op == PDR_DECODE)				\
      memmove((caddr_t)addr, (pdrs)->x_private, len);		\
    else					 		\
      memmove((pdrs)->x_private, (caddr_t)addr, len);		\
  }								\
  (pdrs)->x_private += len;	  				\
}

#define PWR_PDR_STRING(pdrs, addr, len) PWR_PDR_BYTES(pdrs, addr, len)


#define PWR_PDR_INT(pdrs, objp)                			\
{								\
  if (((pdrs)->x_handy -= sizeof(int)) < 0)			\
    return (FALSE);						\
    								\
  if ((pdrs)->x_op == PDR_DECODE) {				\
    if ((pdrs)->trg.b.bo != (pdrs)->src.b.bo) {                 \
      ENDIAN_SWAP_INT(objp, (int *)(pdrs)->x_private);          \
    } else {                                                    \
      *objp = *(int *)(pdrs)->x_private;                        \
    }                                                           \
  } else {				 			\
    if ((pdrs)->trg.b.bo != (pdrs)->src.b.bo) {                 \
      ENDIAN_SWAP_INT((int *)(pdrs)->x_private, objp);          \
    } else {                                                    \
      *(int *)(pdrs)->x_private = *(objp);                      \
    }                                                           \
 }                                                              \
								\
 (pdrs)->x_private += sizeof(int);	  			\
}


#define PWR_PDR_SHORT(pdrs, objp)                               \
{								\
  if (((pdrs)->x_handy -= sizeof(short)) < 0)			\
    return (FALSE);						\
    								\
  if ((pdrs)->x_op == PDR_DECODE) {				\
    if ((pdrs)->trg.b.bo != (pdrs)->src.b.bo) {                 \
      ENDIAN_SWAP_SHORT(objp, (short *)(pdrs)->x_private);      \
    } else {                                                    \
      *objp = *(short *)(pdrs)->x_private;                      \
    }                                                           \
  } else {				 			\
    if ((pdrs)->trg.b.bo != (pdrs)->src.b.bo) {                 \
      ENDIAN_SWAP_INT((short *)(pdrs)->x_private, objp);        \
    } else {                                                    \
      *(short *)(pdrs)->x_private = *(objp);                    \
    }                                                           \
 }                                                              \
								\
 (pdrs)->x_private += sizeof(short);	  			\
}


#define PWR_PDR_CHAR(pdrs, objp)                                \
{								\
  if (((pdrs)->x_handy -= sizeof(char)) < 0)			\
    return (FALSE);						\
    								\
  if ((pdrs)->x_op == PDR_DECODE)				\
      *objp = *(char *)(pdrs)->x_private;                       \
  else   				 			\
      *(char *)(pdrs)->x_private = *(objp);                     \
								\
 (pdrs)->x_private += sizeof(char);	  			\
}





/*
 * The procedure pdrmem_create initializes a stream descriptor for a
 * memory buffer.  
 */
void
pdrmem_create(PDR *pdrs,
              void *addr,
              u_int size,
              enum pdr_op op,
              co_mFormat src,
              co_mFormat trg)

{
    pdrs->x_op = op;
    pdrs->x_private = pdrs->x_base = addr;
    pdrs->x_handy = size;
    pdrs->src = src;
    pdrs->trg = trg;
}


pwr_tBoolean
pdr_char(PDR *pdrs, char *objp)
{
  PWR_PDR_CHAR(pdrs, objp);
  return TRUE;  
}

pwr_tBoolean 
pdr_enum(PDR *pdrs, pdr_tEnum *ep)
{
  PWR_PDR_INT(pdrs, (int *)ep);
  return TRUE;
}


pwr_tBoolean
pdr_float(PDR *pdrs,
          float *fp)
{
    if ((pdrs->x_handy -= sizeof(float)) < 0)
        return FALSE;

    if (pdrs->x_op == PDR_DECODE) {

        if (pdrs->src.b.ft != pdrs->trg.b.ft) {
            if (pdrs->src.b.ft == co_eFT_ieeeS)
                co_vaxf2ieee(pdrs->src.b.bo, pdrs->trg.b.bo, pdrs->x_private, (char *)fp);
            else
                co_ieee2vaxf(pdrs->src.b.bo, pdrs->trg.b.bo, pdrs->x_private, (char *)fp);
                
        }
        else if (pdrs->trg.b.bo != pdrs->src.b.bo) {
                ENDIAN_SWAP_INT((int *)fp, (int *)pdrs->x_private);
        } else {
                *(int *)fp = *(int *)pdrs->x_private;
        }
    }


    /* Encode */
    else {
        if (pdrs->src.b.ft != pdrs->trg.b.ft) {
            if (pdrs->src.b.ft == co_eFT_ieeeS)
                co_vaxf2ieee(pdrs->src.b.bo, pdrs->trg.b.bo, (const char*)fp, pdrs->x_private);
            else
                co_ieee2vaxf(pdrs->src.b.bo, pdrs->trg.b.bo, (const char*)fp, pdrs->x_private);
                
        }
        else if (pdrs->trg.b.bo != pdrs->src.b.bo) {
            ENDIAN_SWAP_INT((int *)pdrs->x_private, (int *)fp);
        } else {
            *(int *)pdrs->x_private = *(int *)fp;
        }
        
    }
    

    pdrs->x_private += sizeof(float);

    return TRUE;    
}


/**
 *
 * PDR a fixed length array. Unlike variable-length arrays,
 * the storage of fixed length arrays is static and unfreeable.
 * > basep: base of the array
 * > size: size of the array
 * > elemsize: size of each element
 * > pdr_elem: routine to PDR each element
 */
pwr_tBoolean
pdr_vector(PDR *pdrs,
           char *basep,
           u_int nelem,
           u_int elemsize,
           pdrproc_t pdr_elem)
{
    u_int i;
    char *elptr;

    elptr = basep;
    for (i = 0; i < nelem; i++) {
        if (! (*pdr_elem)(pdrs, elptr, LASTUNSIGNED)) {
            return FALSE ;
        }
        elptr += elemsize;
    }
    return TRUE;
}



/*
 * When encoded, pdr_mFormat should always be stored as big endian.
 * When decoded, the source must be big endian.
 *
 */
pwr_tBoolean
pdr_co_mFormat(PDR *pdrs, co_mFormat *objp)
{
    if ((pdrs->x_handy -= sizeof(int)) < 0)
        return (FALSE);

    if (pdrs->x_op == PDR_DECODE) {
        if (pdrs->trg.b.bo != co_eBO_little) {
            ENDIAN_SWAP_INT(objp, (int *)pdrs->x_private);
        } else {
           *(int *)objp = *(int *)pdrs->x_private;
        }
    } else {
        if (pdrs->src.b.bo != co_eBO_big) {
            ENDIAN_SWAP_INT((int *)pdrs->x_private, objp);
        } else {
            *(int *)pdrs->x_private = *(int *)(objp);
        }
    }

    pdrs->x_private += sizeof(int);

    return TRUE;
}






/*
* pdr_IsPdrNeeded
*/
pwr_tBoolean pdr_IsPdrNeeded(co_sPlatform *plat1, co_sPlatform *plat2)
{
  if (plat1->fm.m == plat2->fm.m)
      return FALSE;
  else
      return TRUE;
}


/*
* PDR Routines
*/

pwr_tBoolean 
pdr_co_sPlatform(PDR *pdrs, co_sPlatform *objp)
{
  PWR_PDR_INT(pdrs, &objp->os);
  PWR_PDR_INT(pdrs, &objp->hw);
  PWR_PDR_INT(pdrs, &objp->fm.m);

  return TRUE;
}

pwr_tBoolean
pdr_cdh_sObjName(PDR *pdrs, cdh_sObjName *objp)
{
	if (!pdr_pwr_tObjName(pdrs, &objp->orig)) {
		return (FALSE);
	}
	if (!pdr_pwr_tObjName(pdrs, &objp->norm)) {
		return (FALSE);
	}
	if (!pdr_pwr_tUInt32(pdrs, &objp->pack.key)) {
		return (FALSE);
	}
	return TRUE;
}

pwr_tBoolean
pdr_cdh_sFamily(PDR *pdrs, cdh_sFamily *objp)
{
	if (!pdr_cdh_sObjName(pdrs, &objp->name)) {
		return (FALSE);
	}
	if (!pdr_pwr_tObjid(pdrs, &objp->poid)) {
		return (FALSE);
	}
	return TRUE;
}

pwr_tBoolean
pdr_pwr_tBit(PDR *pdrs, pwr_tBit *objp)
{
  PWR_PDR_INT(pdrs, objp);
  return TRUE;  
}


pwr_tBoolean
pdr_pwr_tBitMask(PDR *pdrs, pwr_tBitMask *objp)
{
  PWR_PDR_INT(pdrs, objp);
  return TRUE;  
}


pwr_tBoolean
pdr_pwr_tBoolean(PDR *pdrs, pwr_tBoolean *objp)
{
  PWR_PDR_INT(pdrs, objp);
  return TRUE;  
}


pwr_tBoolean
pdr_pwr_tGeneration(PDR *pdrs, pwr_tGeneration *objp)
{
  PWR_PDR_INT(pdrs, objp);
  return TRUE;  
}

pwr_tBoolean
pdr_pwr_tInt8(PDR *pdrs, pwr_tInt8 *objp)
{
  PWR_PDR_CHAR(pdrs, objp);
  return TRUE;  
}


pwr_tBoolean
pdr_pwr_tInt16(PDR *pdrs, pwr_tInt16 *objp)
{
  PWR_PDR_SHORT(pdrs, objp);
  return TRUE;  
}


pwr_tBoolean
pdr_pwr_tInt32(PDR *pdrs, pwr_tInt32 *objp)
{
  PWR_PDR_INT(pdrs, objp);
  return TRUE;  
}


pwr_tBoolean
pdr_pwr_tInt64(PDR *pdrs, pwr_tInt64 *objp)
{
  PWR_PDR_INT(pdrs, &objp->low);
  PWR_PDR_INT(pdrs, &objp->high);

  return TRUE;
}


pwr_tBoolean
pdr_pwr_tUInt8(PDR *pdrs, pwr_tUInt8 *objp)
{
  PWR_PDR_CHAR(pdrs, objp);
  return TRUE;  
}


pwr_tBoolean
pdr_pwr_tUInt16(PDR *pdrs, pwr_tUInt16 *objp)
{
  PWR_PDR_SHORT(pdrs, objp);
  return TRUE;  
}


pwr_tBoolean
pdr_pwr_tUInt32(PDR *pdrs, pwr_tUInt32 *objp)
{
  PWR_PDR_INT(pdrs, objp);
  return TRUE;  
}


pwr_tBoolean
pdr_pwr_tUInt64(PDR *pdrs, pwr_tUInt64 *objp)
{
  PWR_PDR_INT(pdrs, &objp->low);
  PWR_PDR_INT(pdrs, &objp->high);

  return TRUE;
}


pwr_tBoolean
pdr_pwr_tStatus(PDR *pdrs, pwr_tStatus *objp)
{
  PWR_PDR_INT(pdrs, objp);
  return TRUE;
}


pwr_tBoolean
pdr_pwr_tVolumeId(PDR *pdrs, pwr_tVolumeId *objp)
{
  PWR_PDR_INT(pdrs, objp);
  return TRUE;  
}


pwr_tBoolean
pdr_pwr_tObjectIx(PDR *pdrs, pwr_tObjectIx *objp)
{
  PWR_PDR_INT(pdrs, objp);
  return TRUE;  
}


pwr_tBoolean
pdr_pwr_tObjid(PDR *pdrs, pwr_tObjid *objp)
{
  PWR_PDR_INT(pdrs, &objp->oix);
  PWR_PDR_INT(pdrs, &objp->vid);

  return TRUE;
}


pwr_tBoolean
pdr_pwr_tClassId(PDR *pdrs, pwr_tClassId *objp)
{
  PWR_PDR_INT(pdrs, objp);
  return TRUE;  
}


pwr_tBoolean
pdr_pwr_tTypeId(PDR *pdrs, pwr_tTypeId *objp)
{
  PWR_PDR_INT(pdrs, objp);
  return TRUE;  
}


pwr_tBoolean
pdr_pwr_tVersion(PDR *pdrs, pwr_tVersion *objp)
{
  PWR_PDR_INT(pdrs, objp);
  return TRUE;  
}


pwr_tBoolean
pdr_pwr_tPwrVersion(PDR *pdrs, pwr_tPwrVersion *objp)
{
  PWR_PDR_INT(pdrs, objp);
  return TRUE;  
}


pwr_tBoolean
pdr_pwr_tProjVersion(PDR *pdrs, pwr_tProjVersion *objp)
{
  PWR_PDR_INT(pdrs, objp);
  return TRUE;  
}


pwr_tBoolean
pdr_pwr_tUserId(PDR *pdrs, pwr_tUserId *objp)
{
  PWR_PDR_INT(pdrs, objp);
  return TRUE;  
}


pwr_tBoolean
pdr_pwr_tDbId(PDR *pdrs, pwr_tDbId *objp)
{
  PWR_PDR_INT(pdrs, objp);
  return TRUE;  
}


pwr_tBoolean
pdr_pwr_tNodeId(PDR *pdrs, pwr_tNodeId *objp)
{
  PWR_PDR_INT(pdrs, objp);
  return TRUE;  
}


pwr_tBoolean
pdr_pwr_tNodeIndex(PDR *pdrs, pwr_tNodeIndex *objp)
{
  PWR_PDR_INT(pdrs, objp);
  return TRUE;  
}


pwr_tBoolean
pdr_pwr_tRefId(PDR *pdrs, pwr_tRefId *objp)
{
  PWR_PDR_INT(pdrs, &objp->rix);
  PWR_PDR_INT(pdrs, &objp->nid);

  return TRUE;
}


pwr_tBoolean
pdr_pwr_tDlid(PDR *pdrs, pwr_tDlid *objp)
{
  PWR_PDR_INT(pdrs, &objp->rix);
  PWR_PDR_INT(pdrs, &objp->nid);

  return TRUE;
}


pwr_tBoolean
pdr_pwr_tSubid(PDR *pdrs, pwr_tSubid *objp)
{
  PWR_PDR_INT(pdrs, &objp->rix);
  PWR_PDR_INT(pdrs, &objp->nid);

  return TRUE;
}


pwr_tBoolean
pdr_pwr_tTime(PDR *pdrs, pwr_tTime *objp)
{
  PWR_PDR_INT(pdrs, &objp->tv_sec);
  PWR_PDR_INT(pdrs, &objp->tv_nsec);

  return TRUE;
}


pwr_tBoolean
pdr_pwr_tDeltaTime(PDR *pdrs, pwr_tDeltaTime *objp)
{
  PWR_PDR_INT(pdrs, &objp->tv_sec);
  PWR_PDR_INT(pdrs, &objp->tv_nsec);

  return TRUE;
}


pwr_tBoolean
pdr_pwr_tObjName(PDR *pdrs, pwr_tObjName *objp)
{
  PWR_PDR_STRING(pdrs, *objp, sizeof(*objp));
  return TRUE;  
}


pwr_tBoolean
pdr_pwr_tPgmName(PDR *pdrs, pwr_tPgmName *objp)
{
  PWR_PDR_BYTES(pdrs, objp, sizeof(*objp));
  return TRUE;  
}


pwr_tBoolean
pdr_pwr_tXRef(PDR *pdrs, pwr_tXRef *objp)
{
  PWR_PDR_BYTES(pdrs, objp, sizeof(*objp));
  return TRUE;  
}


pwr_tBoolean
pdr_pwr_tGraphName(PDR *pdrs, pwr_tGraphName *objp)
{
  PWR_PDR_BYTES(pdrs, objp, sizeof(*objp));
  return TRUE;  
}


pwr_tBoolean
pdr_pwr_tStructName(PDR *pdrs, pwr_tStructName *objp)
{
  PWR_PDR_BYTES(pdrs, objp, sizeof(*objp));
  return TRUE;  
}


pwr_tBoolean
pdr_pwr_tAttrName(PDR *pdrs, pwr_tAttrName *objp)
{
  PWR_PDR_BYTES(pdrs, objp, sizeof(*objp));
  return TRUE;  
}

pwr_tBoolean
pdr_pwr_tPathName(PDR *pdrs, pwr_tPathName *objp)
{
  PWR_PDR_BYTES(pdrs, objp, sizeof(*objp));
  return TRUE;  
}


pwr_tBoolean
pdr_pwr_tFullName(PDR *pdrs, pwr_tFullName *objp)
{
  PWR_PDR_BYTES(pdrs, objp, sizeof(*objp));
  return TRUE;  
}


pwr_tBoolean
pdr_pwr_tString256(PDR *pdrs, pwr_tString256 *objp)
{
  PWR_PDR_BYTES(pdrs, objp, sizeof(*objp));
  return TRUE;  
}


pwr_tBoolean
pdr_pwr_tString132(PDR *pdrs, pwr_tString132 *objp)
{
  PWR_PDR_BYTES(pdrs, objp, sizeof(*objp));
  return TRUE;  
}


pwr_tBoolean
pdr_pwr_tString80(PDR *pdrs, pwr_tString80 *objp)
{
  PWR_PDR_BYTES(pdrs, objp, sizeof(*objp));
  return TRUE;  
}


pwr_tBoolean
pdr_pwr_tString40(PDR *pdrs, pwr_tString40 *objp)
{
  PWR_PDR_BYTES(pdrs, objp, sizeof(*objp));
  return TRUE;  
}


pwr_tBoolean
pdr_pwr_tString32(PDR *pdrs, pwr_tString32 *objp)
{
  PWR_PDR_BYTES(pdrs, objp, sizeof(*objp));
  return TRUE;  
}


pwr_tBoolean
pdr_pwr_tString16(PDR *pdrs, pwr_tString16 *objp)
{
  PWR_PDR_BYTES(pdrs, objp, sizeof(*objp));
  return TRUE;  
}


pwr_tBoolean
pdr_pwr_tString8(PDR *pdrs, pwr_tString8 *objp)
{
  PWR_PDR_BYTES(pdrs, objp, sizeof(*objp));
  return TRUE;  
}


pwr_tBoolean
pdr_pwr_mAttrRef(PDR *pdrs, pwr_mAttrRef *objp)
{
  PWR_PDR_INT(pdrs, &objp->m);
  return TRUE;  
}


pwr_tBoolean 
pdr_pwr_sAttrRef(PDR *pdrs, pwr_sAttrRef *objp)
{
  PWR_PDR_INT(pdrs, &objp->Objid.oix);
  PWR_PDR_INT(pdrs, &objp->Objid.vid);
  PWR_PDR_INT(pdrs, &objp->Body);
  PWR_PDR_INT(pdrs, &objp->Offset);
  PWR_PDR_INT(pdrs, &objp->Size);
  PWR_PDR_INT(pdrs, &objp->Flags.m);

  return TRUE;
}
