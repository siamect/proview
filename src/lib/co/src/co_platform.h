#ifndef co_platform_h
#define co_platform_h

/* co_platform.h -- platform routines

   PROVIEW/R
   Copyright (C)  by Cell Network AB.

   This module contains data structures that descibes a PROVIEW platform  
   */

#ifdef __cplusplus
extern "C" {
#endif


#ifndef pwr_h
# include "pwr.h"
#endif

#define LASTUNSIGNED	((u_int)0-1)


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



#define IBYTEP0(s) (((*(int *)s) >> 0x18) & 0x000000ff) 
#define IBYTEP1(s) (((*(int *)s) >> 0x08) & 0x0000ff00) 
#define IBYTEP2(s) (((*(int *)s) << 0x08) & 0x00ff0000) 
#define IBYTEP3(s) (((*(int *)s) << 0x18) & 0xff000000) 

#define ENDIAN_SWAP_INTP(s)\
  (*(int *)s = (IBYTEP0(s) | IBYTEP1(s) | IBYTEP2(s) | IBYTEP3(s)))

#define SBYTEP0(s) (((*(short *)s) >> 0x08) & 0x00ff) 
#define SBYTEP1(s) (((*(short *)s) << 0x08) & 0xff00) 

#define ENDIAN_SWAP_SHORTP(s)\
  (*(short *)s = (SBYTEP0(s) | SBYTEP1(s)))

#define ENDIAN_SWAP_BOOLP(s) (*(int *)s = (0 != *(int *)s))


/** OS type
 * @note Do not change the order. Must be backward compatible
 */
typedef enum {
    co_eOS__ = 0,
    co_eOS_Lynx,
    co_eOS_VMS,
    co_eOS_ELN,
    co_eOS_WNT,   /**< Pay me and I will do it */
    co_eOS_W95,   /**< Never!! */
    co_eOS_Amiga, /**< We are working on it ;-) */
    co_eOS_Linux,
    co_eOS_
} co_eOS;

#if defined(OS_LYNX) 
#   define co_dHostOS co_eOS_Lynx
#elif defined(OS_VMS) 
#   define co_dHostOS co_eOS_VMS
#elif defined(OS_ELN) 
#   define co_dHostOS co_eOS_ELN
#elif defined(OS_WNT) 
#   define co_dHostOS co_eOS_WNT
#elif defined(OS_LINUX) 
#   define co_dHostOS co_eOS_Linux
#else 
#   define co_dHostOS co_eOS__
#endif


/** Hardware type 
  * @note Do not change the order. Must be backward compatible
  */
 
typedef enum {
  co_eHW__ = 0,
  co_eHW_x86,
  co_eHW_68k,
  co_eHW_VAX,
  co_eHW_Alpha,
  co_eHW_PPC,
  co_eHW_
} co_eHW;

#if defined(HW_X86) 
#   define co_dHostHW co_eHW_x86
#elif defined(HW_VAX) 
#   define co_dHostHW co_eHW_VAX
#elif defined(HW_AXP) 
#   define co_dHostHW co_eHW_Alpha
#elif defined(HW_PPC) 
#   define co_dHostHW co_eHW_PPC
#else 
#   define co_dHostHW co_eHW__
#endif


/** Integer and floating-point byte order.
 *
 *  Network is big    endian
 *  68k     is big    endian
 *  x86     is little endian
 *  PPC     is both   endian
 *  VAX     is little endian
 *
 * @note Do not change the order. Must be backward compatible
 */

typedef enum {
    co_eBO__ = 0,
    co_eBO_big,
    co_eBO_little,
    co_eBO_middle /**< Not used, historical reasons */
} co_eBO;


#if (pwr_dHost_byteOrder == pwr_dLittleEndian)
#   define co_dHostByteOrder co_eBO_little
#else
#   define co_dHostByteOrder co_eBO_big
#endif


/** Character representation type. Always set to ascii
 *
 *  @note The enum must not exceed 15 
 */
typedef enum {
    co_eCT__ = 0,
    co_eCT_ascii,
    co_eCT_
} co_eCT;

#define co_dHostAsciiType co_eCT_ascii



/** Floating-point representation type.
 *
 * @note Do not change the order. Must be backward compatible
 */
typedef enum {
    co_eFT__ = 0,
    co_eFT_ieeeS,
    co_eFT_ieeeT, /**< Not used */
    co_eFT_ieeeK, /**< Not used */
    co_eFT_vaxF,  /**< Not set, implicit for both vaxD and vaxG */
    co_eFT_vaxD,
    co_eFT_vaxG,
    co_eFT_vaxH,  /**< Not used */
    co_eFT_
} co_eFT;

#if (defined(OS_VMS) || defined(OS_ELN))
#   if defined(HW_AXP)
#       define co_dHostFloatType co_eFT_vaxG
#   else
#       define co_dHostFloatType co_eFT_vaxD
#   endif
#else
#   define co_dHostFloatType co_eFT_ieeeS
#endif


typedef union {
    pwr_tBitMask m;
    pwr_32Bits (
        pwr_Bits( bo       , 4),,,,     /**< Byte order */
        pwr_Bits( ct	   , 4),,,,     /**< Not used. Character representation */
        pwr_Bits( ft	   , 8),,,,,,,, /**< Floting-point representation */
        pwr_Bits( reserved1, 8),,,,,,,,
        pwr_Bits( reserved2, 8),,,,,,,
      ) b;

} co_mFormat;


typedef struct {
    co_eOS     os;
    co_eHW     hw;
    co_mFormat fm;
} co_sPlatform;


co_mFormat*   co_GetOwnFormat(co_mFormat *format);
co_sPlatform* co_GetOwnPlatform(co_sPlatform *platform);
co_mFormat*   co_SetFormat(co_mFormat *format, co_eBO bo, co_eFT ft);


#ifdef __cplusplus
}
#endif

#endif
