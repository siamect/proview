#ifndef co_float_h
#define co_float_h

#ifdef __cplusplus
extern "C" {
#endif

/**
  @file co_float.h

  Converts an IEEE float to a VAX float and vice verca.
 */

#include "co_pdr.h"


void co_vaxf2ieee(co_eBO sbo,     /**< Source byte order, VAX float */
                  co_eBO tbo,     /**< Target byte order, IEEE float */
                  const char *sp, /**< Address to the source, VAX float */
                  char *tp);      /**< Address to the target, IEEE float */


void co_ieee2vaxf(co_eBO sbo,     /**< Source byte order, IEEE float */
                  co_eBO tbo,     /**< Target byte order, VAX float */
                  const char *sp, /**< Address to the source, IEEE float */
                  char *tp);      /**< Address to the target, VAX float */


#ifdef __cplusplus
}
#endif

#endif
