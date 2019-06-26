/******************************************************************************
*                                                                             *
*                      SOFTING Industrial Automation GmbH                     *
*                          Richard-Reitzner-Allee 6                           *
*                                D-85540 Haar                                 *
*                        Phone: (++49)-(0)89-45656-0                          *
*                        Fax:   (++49)-(0)89-45656-399                        *
*                                                                             *
*            Copyright (C) SOFTING Industrial Automation GmbH 1995-2012       *
*                              All Rights Reserved                            *
*                                                                             *
*******************************************************************************

FILE_NAME               DPSGDL.C

PROJECT_NAME            PROFIBUS

MODULE                  DPSGDL

COMPONENT_LIBRARY       PAPI Lib
                        PAPI DLL

AUTHOR                  SOFTING

VERSION                 1.30.0.00.release (DP-Slave Stand-Alone for DOS)
                        5.22.0.00.release (WIN95/WIN98 and WinNT)

DATE                    February-1999

STATUS                  finished

FUNCTIONAL_MODULE_DESCRIPTION

This modul contains DP-Slave-Service-Specific-Functions which return the length
length of the Request- or Response-Datas.


RELATED_DOCUMENTS
=============================================================================*/

#include "keywords.h"

INCLUDES

#if defined(WIN16) || defined(WIN32)
#include <windows.h>
#endif

#include "pb_type.h"
#include "pb_if.h"

#ifndef DPS_STANDALONE_MODE
#include "pb_err.h"
#include "pb_dp.h"
#endif

#include "pb_dps.h"

FUNCTION_DECLARATIONS

LOCAL_DEFINES

LOCAL_TYPEDEFS

EXPORT_DATA

IMPORT_DATA

LOCAL_DATA

#if defined(WIN32) || defined(_WIN32) || defined(WIN16) || defined(_WIN16)
#pragma check_stack(off)
#endif
FUNCTION PUBLIC INT16 dpsgdl_get_data_len(IN INT16 result, IN USIGN8 service,
                                          IN USIGN8 primitive,
                                          IN USIGN8 FAR* data_ptr,
                                          OUT INT16 FAR* data_len_ptr)
/*------------------------------------------------------------------------*/
/* FUNCTIONAL_DESCRIPTION                                                 */
/*------------------------------------------------------------------------*/
/* - returns the data length of any called PROFIBUS DP-Slave service      */
/*------------------------------------------------------------------------*/
{
  LOCAL_VARIABLES

  T_DPS_INIT_SLAVE_REQ FAR* init_slave_req_ptr;
  T_DPS_SLAVE_DIAG_REQ FAR* slave_diag_req_ptr;

  FUNCTION_BODY

  switch (primitive)
  {
  /*--- USER REQUESTS ----------------------------------------------------*/

  case REQ:
  {
    switch (service)
    {
    case DPS_INIT_SLAVE:
    {
      init_slave_req_ptr = (T_DPS_INIT_SLAVE_REQ FAR*)data_ptr;

      if ((init_slave_req_ptr->cfg_data_len == 0) ||
          (init_slave_req_ptr->cfg_data_len > DP_MAX_CFG_DATA_LEN))
      {
        return (E_IF_INVALID_DATA_SIZE);
      }

      if (init_slave_req_ptr->enhanced_init_data_len >
          sizeof(init_slave_req_ptr->enhanced_init_data))
      {
        return (E_IF_INVALID_DATA_SIZE);
      }

      *data_len_ptr = sizeof(T_DPS_INIT_SLAVE_REQ) -
                      (sizeof(init_slave_req_ptr->enhanced_init_data) -
                       init_slave_req_ptr->enhanced_init_data_len);
      break;
    } /* case DPS_INIT_SLAVE */

    case DPS_EXIT_SLAVE:
      *data_len_ptr = 0;
      break;

    case DPS_SLAVE_DIAG:
    {
      slave_diag_req_ptr = (T_DPS_SLAVE_DIAG_REQ FAR*)data_ptr;

      if (slave_diag_req_ptr->ext_diag_data_len > DP_MAX_EXT_DIAG_DATA_LEN)
      {
        return (E_IF_INVALID_DATA_SIZE);
      }

      *data_len_ptr =
          sizeof(T_DPS_SLAVE_DIAG_REQ) -
          (DP_MAX_EXT_DIAG_DATA_LEN - slave_diag_req_ptr->ext_diag_data_len);
      break;
    }

    case DPS_GET_STATUS:
      *data_len_ptr = 0;
      break;

    case DPS_CHK_CFG:
    case DPS_SET_PRM:
    case DPS_SET_SLAVE_ADD:
      return (E_IF_INVALID_PRIMITIVE);

    default:
      return (E_IF_INVALID_SERVICE);

    } /* switch serivce */

    break;
  } /* case REQ */

  /*---- USER RESPONSES --------------------------------------------------*/

  case RES:
  {
    switch (service)
    {
    case DPS_CHK_CFG:
      *data_len_ptr = sizeof(T_DPS_CHK_CFG_RES);
      break;
    case DPS_SET_PRM:
      *data_len_ptr = sizeof(T_DPS_SET_PRM_RES);
      break;

    default:
      return (E_IF_INVALID_SERVICE);

    } /* switch serivce */

    break;
  } /* case RES */

  /*---- WRONG PRIMITIVE -------------------------------------------------*/

  default:
    return (E_IF_INVALID_PRIMITIVE);

  } /* switch primitive */

  return (E_OK);

} /* dpsgdl_get_data_len */

#if defined(WIN32) || defined(_WIN32) || defined(WIN16) || defined(_WIN16)
#pragma check_stack
#endif
