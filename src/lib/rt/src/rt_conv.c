/**
 * @file rt_conv.c
 *
 * Routines for converting attributes with different data types.
 *
 */

#include <string.h>
#include "pwr_class.h"
#include "rt_conv.h"


#define CONV_ARGS \
  int		tcount,	\
  int		tasize, \
  char		*tp,	\
  int		*tsize,	\
  int		scount,	\
  int		sasize,	\
  const char	*sp,	\
  pwr_mAdef	sadef

#define BOOL_TO_OTHER(TTYPE)						\
  for (; tcount > 0 && *tsize >= sizeof(TTYPE); tcount--, scount--) {	\
    if (scount > 0) {							\
      *(TTYPE *)tp =  (*(pwr_tBoolean *)sp) ? 1 : 0;			\
      sp += sizeof(pwr_tBoolean);					\
    } else								\
      *(TTYPE *)tp = 0;							\
									\
    tp += sizeof(TTYPE);						\
    *tsize -= sizeof(TTYPE);						\
  }

#define FLOAT_TO_BOOL(STYPE)						\
  for (; tcount > 0 && *tsize >= sizeof(pwr_tBoolean); tcount--, scount--) {	\
    if (scount > 0 && !sadef.b.privatepointer) { 			\
      *(pwr_tBoolean *)tp =  (*(STYPE *)sp != 0) ? TRUE : FALSE;	\
      sp += sizeof(STYPE);						\
    } else								\
      *(pwr_tBoolean*)tp = 0;						\
									\
    tp += sizeof(pwr_tBoolean);						\
    *tsize -= sizeof(pwr_tBoolean);					\
  }

#define FLOAT_TO_NOT_BOOL(TTYPE, STYPE)					\
  for (; tcount > 0 && *tsize >= sizeof(TTYPE); tcount--, scount--) {	\
    if (scount > 0 && !sadef.b.privatepointer) { 			\
      *(TTYPE *)tp =  *(STYPE *)sp;					\
      sp += sizeof(STYPE);						\
    } else								\
      *(TTYPE*)tp = 0;							\
									\
    tp += sizeof(TTYPE);						\
    *tsize -= sizeof(TTYPE);						\
  }

#define INT_TO_BOOL(STYPE)						\
  for (; tcount > 0 && *tsize >= sizeof(pwr_tBoolean); tcount--, scount--) {	\
    if (scount > 0) { 							\
      *(pwr_tBoolean *)tp =  (*(STYPE *)sp) ? TRUE : FALSE;		\
      sp += sizeof(STYPE);						\
    } else								\
      *(pwr_tBoolean*)tp = 0;						\
									\
    tp += sizeof(pwr_tBoolean);						\
    *tsize -= sizeof(pwr_tBoolean);					\
  }

#define INT_TO_NOT_BOOL(TTYPE, STYPE)					\
  for (; tcount > 0 && *tsize >= sizeof(TTYPE); tcount--, scount--) {	\
    if (scount > 0) { 							\
      *(TTYPE *)tp =  *(STYPE *)sp;					\
      sp += sizeof(STYPE);						\
    } else								\
      *(TTYPE*)tp = 0;							\
									\
    tp += sizeof(TTYPE);						\
    *tsize -= sizeof(TTYPE);						\
  }




static pwr_tBoolean convIllegal(CONV_ARGS);
static pwr_tBoolean convCopy(CONV_ARGS);
static pwr_tBoolean convZero(CONV_ARGS);

static pwr_tBoolean boolToFloat32(CONV_ARGS);
static pwr_tBoolean boolToFloat64(CONV_ARGS);
static pwr_tBoolean boolToInt8(CONV_ARGS);
static pwr_tBoolean boolToInt16(CONV_ARGS);
static pwr_tBoolean boolToInt32(CONV_ARGS);

static pwr_tBoolean float32ToBool(CONV_ARGS);
static pwr_tBoolean float32ToFloat64(CONV_ARGS);
static pwr_tBoolean float32ToInt8(CONV_ARGS);
static pwr_tBoolean float32ToInt16(CONV_ARGS);
static pwr_tBoolean float32ToInt32(CONV_ARGS);

static pwr_tBoolean float64ToBool(CONV_ARGS);
static pwr_tBoolean float64ToFloat32(CONV_ARGS);
static pwr_tBoolean float64ToInt8(CONV_ARGS);
static pwr_tBoolean float64ToInt16(CONV_ARGS);
static pwr_tBoolean float64ToInt32(CONV_ARGS);

static pwr_tBoolean int8ToBool(CONV_ARGS);
static pwr_tBoolean int8ToFloat32(CONV_ARGS);
static pwr_tBoolean int8ToFloat64(CONV_ARGS);
static pwr_tBoolean int8ToInt16(CONV_ARGS);
static pwr_tBoolean int8ToInt32(CONV_ARGS);
static pwr_tBoolean int8ToUInt16(CONV_ARGS);
static pwr_tBoolean int8ToUInt32(CONV_ARGS);

static pwr_tBoolean int16ToBool(CONV_ARGS);
static pwr_tBoolean int16ToFloat32(CONV_ARGS);
static pwr_tBoolean int16ToFloat64(CONV_ARGS);
static pwr_tBoolean int16ToInt8(CONV_ARGS);
static pwr_tBoolean int16ToInt32(CONV_ARGS);
static pwr_tBoolean int16ToUInt8(CONV_ARGS);
static pwr_tBoolean int16ToUInt32(CONV_ARGS);

static pwr_tBoolean int32ToBool(CONV_ARGS);
static pwr_tBoolean int32ToFloat32(CONV_ARGS);
static pwr_tBoolean int32ToFloat64(CONV_ARGS);
static pwr_tBoolean int32ToInt8(CONV_ARGS);
static pwr_tBoolean int32ToInt16(CONV_ARGS);
static pwr_tBoolean int32ToUInt8(CONV_ARGS);
static pwr_tBoolean int32ToUInt16(CONV_ARGS);

static pwr_tBoolean uint8ToBool(CONV_ARGS);
static pwr_tBoolean uint8ToFloat32(CONV_ARGS);
static pwr_tBoolean uint8ToFloat64(CONV_ARGS);
static pwr_tBoolean uint8ToInt16(CONV_ARGS);
static pwr_tBoolean uint8ToInt32(CONV_ARGS);
static pwr_tBoolean uint8ToUInt16(CONV_ARGS);
static pwr_tBoolean uint8ToUInt32(CONV_ARGS);

static pwr_tBoolean uint16ToBool(CONV_ARGS);
static pwr_tBoolean uint16ToFloat32(CONV_ARGS);
static pwr_tBoolean uint16ToFloat64(CONV_ARGS);
static pwr_tBoolean uint16ToInt8(CONV_ARGS);
static pwr_tBoolean uint16ToInt32(CONV_ARGS);
static pwr_tBoolean uint16ToUInt8(CONV_ARGS);
static pwr_tBoolean uint16ToUInt32(CONV_ARGS);

static pwr_tBoolean uint32ToBool(CONV_ARGS);
static pwr_tBoolean uint32ToFloat32(CONV_ARGS);
static pwr_tBoolean uint32ToFloat64(CONV_ARGS);
static pwr_tBoolean uint32ToInt8(CONV_ARGS);
static pwr_tBoolean uint32ToInt16(CONV_ARGS);
static pwr_tBoolean uint32ToUInt8(CONV_ARGS);
static pwr_tBoolean uint32ToUInt16(CONV_ARGS);

static pwr_tBoolean stringToString(CONV_ARGS);


/** @note The member order must match conv_eIdx */
const convFunction conv_Fctn[] = {
  convIllegal,
  convCopy,
  convZero,

  boolToFloat32,
  boolToFloat64,
  boolToInt8,
  boolToInt16,
  boolToInt32,

  float32ToBool,
  float32ToFloat64,
  float32ToInt8,
  float32ToInt16,
  float32ToInt32,

  float64ToBool,
  float64ToFloat32,
  float64ToInt8,
  float64ToInt16,
  float64ToInt32,

  int8ToBool,
  int8ToFloat32,
  int8ToFloat64,
  int8ToInt16,
  int8ToInt32,
  int8ToUInt16,
  int8ToUInt32,

  int16ToBool,
  int16ToFloat32,
  int16ToFloat64,
  int16ToInt8,
  int16ToInt32,
  int16ToUInt8,
  int16ToUInt32,

  int32ToBool,
  int32ToFloat32,
  int32ToFloat64,
  int32ToInt8,
  int32ToInt16,
  int32ToUInt8,
  int32ToUInt16,

  uint8ToBool,
  uint8ToFloat32,
  uint8ToFloat64,
  uint8ToInt16,
  uint8ToInt32,
  uint8ToUInt16,
  uint8ToUInt32,

  uint16ToBool,
  uint16ToFloat32,
  uint16ToFloat64,
  uint16ToInt8,
  uint16ToInt32,
  uint16ToUInt8,
  uint16ToUInt32,

  uint32ToBool,
  uint32ToFloat32,
  uint32ToFloat64,
  uint32ToInt8,
  uint32ToInt16,
  uint32ToUInt8,
  uint32ToUInt16,

  stringToString
};



/**
 * Returns the index to the conversion routine, which will convert the
 * source type to the target type.
 */
conv_eIdx 
conv_GetIdx(pwr_eType src, pwr_eType trg)
{

    if (src == trg) {
      if (src == pwr_eType_String || src == pwr_eType_Text)
        return conv_eIdx_stringToString;
      else
        return conv_eIdx_copy;
    }
    
    
    switch (src) {
    case pwr_eType_Boolean:
        switch (trg) {
        case pwr_eType_Float32:
            return conv_eIdx_boolToFloat32;
        case pwr_eType_Float64:
            return conv_eIdx_boolToFloat64;
        case pwr_eType_Char:
        case pwr_eType_Int8:
        case pwr_eType_UInt8:
            return conv_eIdx_boolToInt8;
        case pwr_eType_Int16:
        case pwr_eType_UInt16:
            return conv_eIdx_boolToInt16;
        case pwr_eType_Int32:
        case pwr_eType_UInt32:
            return conv_eIdx_boolToInt32;
        default:
            break;            
        }

    case pwr_eType_Float32:
        switch (trg) {
        case pwr_eType_Boolean:
            return conv_eIdx_float32ToBoolean;
        case pwr_eType_Float64:
            return conv_eIdx_float32ToFloat64;
        case pwr_eType_Char:
        case pwr_eType_Int8:
        case pwr_eType_UInt8:
            return conv_eIdx_float32ToInt8;
        case pwr_eType_Int16:
        case pwr_eType_UInt16:
            return conv_eIdx_float32ToInt16;
        case pwr_eType_Int32:
        case pwr_eType_UInt32:
            return conv_eIdx_float32ToInt32;            
        default:
            break;
        }

    case pwr_eType_Float64:
        switch (trg) {
        case pwr_eType_Boolean:
            return conv_eIdx_float64ToBoolean;
        case pwr_eType_Float64:
            return conv_eIdx_float64ToFloat32;
        case pwr_eType_Char:
        case pwr_eType_Int8:
        case pwr_eType_UInt8:
            return conv_eIdx_float64ToInt8;
        case pwr_eType_Int16:
        case pwr_eType_UInt16:
            return conv_eIdx_float64ToInt16;
        case pwr_eType_Int32:
        case pwr_eType_UInt32:
            return conv_eIdx_float64ToInt32;            
        default:
            break;
        }
        
    case pwr_eType_Int8:
    case pwr_eType_Char:
        switch (trg) {
        case pwr_eType_Boolean:
            return conv_eIdx_int8ToBoolean;
        case pwr_eType_Float32:
            return conv_eIdx_int8ToFloat32;
        case pwr_eType_Float64:
            return conv_eIdx_int8ToFloat64;
        case pwr_eType_Char:
        case pwr_eType_Int8:
            return conv_eIdx_copy;
        case pwr_eType_Int16:
            return conv_eIdx_int8ToInt16;
        case pwr_eType_Int32:
            return conv_eIdx_int8ToInt32;
        case pwr_eType_UInt8:
            return conv_eIdx_copy;
        case pwr_eType_UInt16:
            return conv_eIdx_int8ToUInt16;
        case pwr_eType_UInt32:
            return conv_eIdx_int8ToUInt32;
        default:
            break;
        }

    case pwr_eType_Int16:
        switch (trg) {
        case pwr_eType_Boolean:
            return conv_eIdx_int16ToBoolean;
        case pwr_eType_Float32:
            return conv_eIdx_int16ToFloat32;
        case pwr_eType_Float64:
            return conv_eIdx_int16ToFloat64;
        case pwr_eType_Char:
        case pwr_eType_Int8:
            return conv_eIdx_int16ToInt8;
        case pwr_eType_Int32:
            return conv_eIdx_int16ToInt32;
        case pwr_eType_UInt8:
            return conv_eIdx_int16ToUInt8;
        case pwr_eType_UInt16:
            return conv_eIdx_copy;
        case pwr_eType_UInt32:
            return conv_eIdx_int16ToUInt32;
        default:
            break;
        }

    case pwr_eType_Int32:
        switch (trg) {
        case pwr_eType_Boolean:
            return conv_eIdx_int32ToBoolean;
        case pwr_eType_Float32:
            return conv_eIdx_int32ToFloat32;
        case pwr_eType_Float64:
            return conv_eIdx_int32ToFloat64;
        case pwr_eType_Char:
        case pwr_eType_Int8:
            return conv_eIdx_int32ToInt8;
        case pwr_eType_Int16:
            return conv_eIdx_int32ToInt16;
        case pwr_eType_UInt8:
            return conv_eIdx_int32ToUInt8;
        case pwr_eType_UInt16:
            return conv_eIdx_int32ToUInt16;
        case pwr_eType_UInt32:
        case pwr_eType_Enum:
        case pwr_eType_Mask:
            return conv_eIdx_copy;
        default:
            break;
        }

    case pwr_eType_UInt8:
        switch (trg) {
        case pwr_eType_Boolean:
            return conv_eIdx_uint8ToBoolean;
        case pwr_eType_Float32:
            return conv_eIdx_uint8ToFloat32;
        case pwr_eType_Float64:
            return conv_eIdx_uint8ToFloat64;
        case pwr_eType_Char:
        case pwr_eType_Int8:
            return conv_eIdx_copy;
        case pwr_eType_Int16:
            return conv_eIdx_uint8ToInt16;
        case pwr_eType_Int32:
            return conv_eIdx_uint8ToInt32;
        case pwr_eType_UInt16:
            return conv_eIdx_uint8ToUInt16;
        case pwr_eType_UInt32:
            return conv_eIdx_uint8ToUInt32;
        default:
            break;
        }

    case pwr_eType_UInt16:
        switch (trg) {
        case pwr_eType_Boolean:
            return conv_eIdx_uint16ToBoolean;
        case pwr_eType_Float32:
            return conv_eIdx_uint16ToFloat32;
        case pwr_eType_Float64:
            return conv_eIdx_uint16ToFloat64;
        case pwr_eType_Char:
        case pwr_eType_Int8:
            return conv_eIdx_uint16ToInt8;
        case pwr_eType_Int16:
            return conv_eIdx_copy;
        case pwr_eType_Int32:
            return conv_eIdx_uint16ToInt32;
        case pwr_eType_UInt8:
            return conv_eIdx_uint16ToUInt8;
        case pwr_eType_UInt32:
            return conv_eIdx_uint16ToUInt32;
        default:
            break;
        }

    case pwr_eType_UInt32:
        switch (trg) {
        case pwr_eType_Boolean:
            return conv_eIdx_uint32ToBoolean;
        case pwr_eType_Float32:
            return conv_eIdx_uint32ToFloat32;
        case pwr_eType_Float64:
            return conv_eIdx_uint32ToFloat64;
        case pwr_eType_Char:
        case pwr_eType_Int8:
            return conv_eIdx_uint32ToInt8;
        case pwr_eType_Int16:
            return conv_eIdx_uint32ToInt16;
        case pwr_eType_Int32:
        case pwr_eType_Enum:
        case pwr_eType_Mask:
            return conv_eIdx_copy;
        case pwr_eType_UInt8:
            return conv_eIdx_uint32ToUInt8;
        case pwr_eType_UInt16:
            return conv_eIdx_uint32ToUInt16;
        default:
            break;
        }

    case pwr_eType_Enum:
    case pwr_eType_Mask:
        switch (trg) {
        case pwr_eType_Int32:
        case pwr_eType_UInt32:
            return conv_eIdx_copy;
        default:
            break;
        }

    default:
        break;
        
    }


    return conv_eIdx_invalid;
}





static pwr_tBoolean 
convIllegal(CONV_ARGS)
{
  return FALSE;
}

static pwr_tBoolean 
convCopy(CONV_ARGS)
{
  int tatot = tcount * tasize;
  int satot = scount * sasize;
  int size = MIN(MIN(*tsize, tatot), satot);

  memcpy(tp, sp, size);
  tp += size;
  *tsize -= size;

  if (tatot > satot) {
    size = MIN(tatot - satot, *tsize);
    memset(tp, 0, size);
    tp += size;
    *tsize -= size;
  }

  return TRUE;
}

static pwr_tBoolean 
convZero(CONV_ARGS)
{
  int size = MIN(tcount * tasize, *tsize);

  memset(tp, 0, size);
  *tsize -= size;

  return TRUE;
}




/*
 * bool
 */
static pwr_tBoolean 
boolToFloat32(CONV_ARGS)
{
  BOOL_TO_OTHER(pwr_tFloat32);
  return TRUE;
}

static pwr_tBoolean 
boolToFloat64(CONV_ARGS)
{
  BOOL_TO_OTHER(pwr_tFloat64);
  return TRUE;
}

static pwr_tBoolean 
boolToInt8(CONV_ARGS)
{
  BOOL_TO_OTHER(pwr_tInt8);
  return TRUE;
}

static pwr_tBoolean 
boolToInt16(CONV_ARGS)
{
  BOOL_TO_OTHER(pwr_tInt16);
  return TRUE;
}

static pwr_tBoolean 
boolToInt32(CONV_ARGS)
{
  BOOL_TO_OTHER(pwr_tInt32);
  return TRUE;
}



/*
 * float32
 */
static pwr_tBoolean 
float32ToBool(CONV_ARGS)
{
  FLOAT_TO_BOOL(pwr_tFloat32);
  return TRUE;
}    

static pwr_tBoolean 
float32ToFloat64(CONV_ARGS)
{
  FLOAT_TO_NOT_BOOL(pwr_tFloat64, pwr_tFloat32);
  return TRUE;
}    

static pwr_tBoolean 
float32ToInt8(CONV_ARGS)
{
  FLOAT_TO_NOT_BOOL(pwr_tInt8, pwr_tFloat32);
  return TRUE;
}
    
static pwr_tBoolean 
float32ToInt16(CONV_ARGS)
{
  FLOAT_TO_NOT_BOOL(pwr_tInt16, pwr_tFloat32);
  return TRUE;
}    

static pwr_tBoolean 
float32ToInt32(CONV_ARGS)
{
  FLOAT_TO_NOT_BOOL(pwr_tInt32, pwr_tFloat32);
  return TRUE;
}


/*
 * float64
 */
static pwr_tBoolean 
float64ToBool(CONV_ARGS)
{
  FLOAT_TO_BOOL(pwr_tFloat64);
  return TRUE;
}    

static pwr_tBoolean 
float64ToFloat32(CONV_ARGS)
{
  FLOAT_TO_NOT_BOOL(pwr_tFloat32, pwr_tFloat64);
  return TRUE;
}    

static pwr_tBoolean 
float64ToInt8(CONV_ARGS)
{
  FLOAT_TO_NOT_BOOL(pwr_tInt8, pwr_tFloat64);
  return TRUE;
}
    
static pwr_tBoolean 
float64ToInt16(CONV_ARGS)
{
  FLOAT_TO_NOT_BOOL(pwr_tInt16, pwr_tFloat64);
  return TRUE;
}    

static pwr_tBoolean 
float64ToInt32(CONV_ARGS)
{
  FLOAT_TO_NOT_BOOL(pwr_tInt32, pwr_tFloat64);
  return TRUE;
}


/*
 * int8
 */

static pwr_tBoolean 
int8ToBool(CONV_ARGS)
{
  INT_TO_BOOL(pwr_tInt8);
  return TRUE;
}

static pwr_tBoolean 
int8ToFloat32(CONV_ARGS)
{
  INT_TO_NOT_BOOL(pwr_tFloat32, pwr_tInt8)
  return TRUE;
}

static pwr_tBoolean 
int8ToFloat64(CONV_ARGS)
{
  INT_TO_NOT_BOOL(pwr_tFloat64, pwr_tInt8)
  return TRUE;
}

static pwr_tBoolean 
int8ToInt16(CONV_ARGS)
{
  INT_TO_NOT_BOOL(pwr_tInt16, pwr_tInt8)
  return TRUE;
}

static pwr_tBoolean 
int8ToInt32(CONV_ARGS)
{
  INT_TO_NOT_BOOL(pwr_tInt32, pwr_tInt8)
  return TRUE;
}

static pwr_tBoolean 
int8ToUInt16(CONV_ARGS)
{
  INT_TO_NOT_BOOL(pwr_tUInt16, pwr_tInt8)
  return TRUE;
}

static pwr_tBoolean 
int8ToUInt32(CONV_ARGS)
{
  INT_TO_NOT_BOOL(pwr_tUInt32, pwr_tInt8)
  return TRUE;
}


/*
 * int16
 */

static pwr_tBoolean 
int16ToBool(CONV_ARGS)
{
  INT_TO_BOOL(pwr_tInt16);
  return TRUE;
}

static pwr_tBoolean 
int16ToFloat32(CONV_ARGS)
{
  INT_TO_NOT_BOOL(pwr_tFloat32, pwr_tInt16)
  return TRUE;
}

static pwr_tBoolean 
int16ToFloat64(CONV_ARGS)
{
  INT_TO_NOT_BOOL(pwr_tFloat64, pwr_tInt16)
  return TRUE;
}

static pwr_tBoolean 
int16ToInt8(CONV_ARGS)
{
  INT_TO_NOT_BOOL(pwr_tInt8, pwr_tInt16)
  return TRUE;
}

static pwr_tBoolean 
int16ToInt32(CONV_ARGS)
{
  INT_TO_NOT_BOOL(pwr_tInt32, pwr_tInt16)
  return TRUE;
}

static pwr_tBoolean 
int16ToUInt8(CONV_ARGS)
{
  INT_TO_NOT_BOOL(pwr_tUInt8, pwr_tInt16)
  return TRUE;
}

static pwr_tBoolean 
int16ToUInt32(CONV_ARGS)
{
  INT_TO_NOT_BOOL(pwr_tUInt32, pwr_tInt16)
  return TRUE;
}



/*
 * int32
 */

static pwr_tBoolean 
int32ToBool(CONV_ARGS)
{
  INT_TO_BOOL(pwr_tInt32);
  return TRUE;
}

static pwr_tBoolean 
int32ToFloat32(CONV_ARGS)
{
  INT_TO_NOT_BOOL(pwr_tFloat32, pwr_tInt32)
  return TRUE;
}

static pwr_tBoolean 
int32ToFloat64(CONV_ARGS)
{
  INT_TO_NOT_BOOL(pwr_tFloat64, pwr_tInt32)
  return TRUE;
}

static pwr_tBoolean 
int32ToInt8(CONV_ARGS)
{
  INT_TO_NOT_BOOL(pwr_tInt8, pwr_tInt32)
  return TRUE;
}

static pwr_tBoolean 
int32ToInt16(CONV_ARGS)
{
  INT_TO_NOT_BOOL(pwr_tInt16, pwr_tInt32)
  return TRUE;
}


static pwr_tBoolean 
int32ToUInt8(CONV_ARGS)
{
  INT_TO_NOT_BOOL(pwr_tUInt8, pwr_tInt32)
  return TRUE;
}

static pwr_tBoolean 
int32ToUInt16(CONV_ARGS)
{
  INT_TO_NOT_BOOL(pwr_tUInt16, pwr_tInt32)
  return TRUE;
}




/*
 * uint8
 */

static pwr_tBoolean 
uint8ToBool(CONV_ARGS)
{
  INT_TO_BOOL(pwr_tUInt8);
  return TRUE;
}

static pwr_tBoolean 
uint8ToFloat32(CONV_ARGS)
{
  INT_TO_NOT_BOOL(pwr_tFloat32, pwr_tUInt8)
  return TRUE;
}

static pwr_tBoolean 
uint8ToFloat64(CONV_ARGS)
{
  INT_TO_NOT_BOOL(pwr_tFloat64, pwr_tUInt8)
  return TRUE;
}

static pwr_tBoolean 
uint8ToInt16(CONV_ARGS)
{
  INT_TO_NOT_BOOL(pwr_tInt16, pwr_tUInt8)
  return TRUE;
}

static pwr_tBoolean 
uint8ToInt32(CONV_ARGS)
{
  INT_TO_NOT_BOOL(pwr_tInt32, pwr_tUInt8)
  return TRUE;
}

static pwr_tBoolean 
uint8ToUInt16(CONV_ARGS)
{
  INT_TO_NOT_BOOL(pwr_tUInt16, pwr_tUInt8)
  return TRUE;
}

static pwr_tBoolean 
uint8ToUInt32(CONV_ARGS)
{
  INT_TO_NOT_BOOL(pwr_tUInt32, pwr_tUInt8)
  return TRUE;
}


/*
 * uint16
 */

static pwr_tBoolean 
uint16ToBool(CONV_ARGS)
{
  INT_TO_BOOL(pwr_tUInt16);
  return TRUE;
}

static pwr_tBoolean 
uint16ToFloat32(CONV_ARGS)
{
  INT_TO_NOT_BOOL(pwr_tFloat32, pwr_tUInt16)
  return TRUE;
}

static pwr_tBoolean 
uint16ToFloat64(CONV_ARGS)
{
  INT_TO_NOT_BOOL(pwr_tFloat64, pwr_tUInt16)
  return TRUE;
}

static pwr_tBoolean 
uint16ToInt8(CONV_ARGS)
{
  INT_TO_NOT_BOOL(pwr_tInt8, pwr_tUInt16)
  return TRUE;
}

static pwr_tBoolean 
uint16ToInt32(CONV_ARGS)
{
  INT_TO_NOT_BOOL(pwr_tInt32, pwr_tUInt16)
  return TRUE;
}

static pwr_tBoolean 
uint16ToUInt8(CONV_ARGS)
{
  INT_TO_NOT_BOOL(pwr_tUInt8, pwr_tUInt16)
  return TRUE;
}

static pwr_tBoolean 
uint16ToUInt32(CONV_ARGS)
{
  INT_TO_NOT_BOOL(pwr_tUInt32, pwr_tUInt16)
  return TRUE;
}



/*
 * uint32
 */

static pwr_tBoolean 
uint32ToBool(CONV_ARGS)
{
  INT_TO_BOOL(pwr_tUInt32);
  return TRUE;
}

static pwr_tBoolean 
uint32ToFloat32(CONV_ARGS)
{
  INT_TO_NOT_BOOL(pwr_tFloat32, pwr_tUInt32)
  return TRUE;
}

static pwr_tBoolean 
uint32ToFloat64(CONV_ARGS)
{
  INT_TO_NOT_BOOL(pwr_tFloat64, pwr_tUInt32)
  return TRUE;
}

static pwr_tBoolean 
uint32ToInt8(CONV_ARGS)
{
  INT_TO_NOT_BOOL(pwr_tInt8, pwr_tUInt32)
  return TRUE;
}

static pwr_tBoolean 
uint32ToInt16(CONV_ARGS)
{
  INT_TO_NOT_BOOL(pwr_tInt16, pwr_tUInt32)
  return TRUE;
}

static pwr_tBoolean 
uint32ToUInt8(CONV_ARGS)
{
  INT_TO_NOT_BOOL(pwr_tUInt8, pwr_tUInt32)
  return TRUE;
}

static pwr_tBoolean 
uint32ToUInt16(CONV_ARGS)
{
  INT_TO_NOT_BOOL(pwr_tUInt16, pwr_tUInt32)
  return TRUE;
}


static pwr_tBoolean
stringToString(CONV_ARGS)
{

  for (; tcount > 0 && *tsize >= tasize; tcount--, scount--) {
    if (scount > 0) {
      memcpy(tp, sp, MIN(tasize, sasize));
      *(tp + tasize - 1) = 0;
      sp += sasize;
    } else
      *tp = 0;

    tp += tasize;
    *tsize -= tasize;
  }
  return TRUE;
}    
