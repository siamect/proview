/*****************************************************************************
*                                                                            *
*                                 SOFTING AG                                 *
*                          Richard-Reitzner-Allee 6                          *
*                                D-85540 Haar                                *
*                        Phone: ++49-89-4 56 56-0                            *
*                          Fax: ++49-89-4 56 56-3 99                         *
*                                                                            *
*                      Copyright (C) SOFTING AG 1995-2003                    *
*                             All Rights Reserved                            *
*                                                                            *
******************************************************************************

FILE_NAME               FM7GDL.C

PROJECT_NAME            PROFIBUS

MODULE                  FM7GDL

COMPONENT_LIBRARY       PAPI Lib
                        PAPI DLL

AUTHOR                  SOFTING AG

VERSION                 5.21.0.00.release

DATE                    27-February-1998

STATUS                  finished

FUNCTIONAL_MODULE_DESCRIPTION

This modul contains the FM7-Service-Specific-Functions which return the length
length of the Request- or Response-Datas.



CHANGE_NOTES

Date      Name      Change
------------------------------------------------------------------------------


RELATED_DOCUMENTS
=============================================================================*/
#include "keywords.h"

INCLUDES

#if defined (WIN16) || defined (WIN32)
#include <windows.h>
#endif

#include "pb_type.h"
#include "pb_if.h"
#include "pb_err.h"
#include "pb_fm7.h"


GLOBAL_DEFINES

LOCAL_DEFINES

EXPORT_TYPEDEFS

LOCAL_TYPEDEFS

FUNCTION_DECLARATIONS

EXPORT_DATA

IMPORT_DATA

LOCAL_DATA


#if defined (WIN32) || defined (_WIN32) || defined (WIN16) || defined (_WIN16)
#pragma check_stack (off)
#endif

FUNCTION LOCAL INT16 fm7gdl_get_ctxt_data_len
         (
          IN    USIGN8     service,          /* Service             */
          IN    USIGN8     primitive         /* Service-Primitive   */
         )

/*----------------------------------------------------------------------------
FUNCTIONAL_DESCRIPTION
This function is used to return the length of request-datas or response-datas
of the following FM7-Context-Management-Services.

- FM7_INITIATE
- FM7_ABORT

possible return values:
- Data-length
----------------------------------------------------------------------------*/
{
  LOCAL_VARIABLES

  FUNCTION_BODY

  if (service == FM7_ABORT && primitive == REQ)
  {
    return(sizeof(T_FM7_ABORT_REQ));
  }
  else
  {
    return(0);
  }
}



FUNCTION LOCAL INT16 fm7gdl_get_crl_data_len
         (
          IN    USIGN8     service,          /* Service             */
          IN    USIGN8     primitive,        /* Service-Primitive   */
          IN    USIGN8 FAR *data_ptr         /* pointer to data     */
         )

/*----------------------------------------------------------------------------
FUNCTIONAL_DESCRIPTION
This function is used to return the length of request-datas or response-datas
of the following CRL-Services.

- FM7_INIT_LOAD_CRL_LOC
- FM7_INIT_LOAD_CRL_REM
- FM7_LOAD_CRL_LOC
- FM7_LOAD_CRL_REM
- FM7_TERM_LOAD_CRL_LOC
- FM7_TERM_LOAD_CRL_REM

- FM7_READ_CRL_LOC
- FM7_READ_CRL_REM


possible return values:
- Data-length


----------------------------------------------------------------------------*/
{
  LOCAL_VARIABLES

  FUNCTION_BODY

  switch(service)
  {
     case FM7_LOAD_CRL_LOC:
     case FM7_LOAD_CRL_REM:
          if (primitive == REQ) return(sizeof(T_LOAD_CRL_REQ));
          else                  return(0);

     case FM7_READ_CRL_LOC:
     case FM7_READ_CRL_REM:
          if (primitive == REQ)
          {
            return(sizeof(T_READ_CRL_REQ));
          }
          else
          {
            T_READ_CRL_CNF FAR *rsp = (T_READ_CRL_CNF FAR*) data_ptr;

            if (rsp->desired_cr == 0) return(sizeof(T_READ_CRL_CNF));
            else                      return(sizeof(T_READ_CRL_CNF) +
                                      rsp->id.crl_entry.crl_status_len);
          }

     default:
         return(0);
  }
}




FUNCTION LOCAL INT16 fm7gdl_get_s_r_value_data_len
         (
          IN    USIGN8     service,          /* Service             */
          IN    USIGN8     primitive,        /* Service-Primitive   */
          IN    USIGN8 FAR *data_ptr         /* pointer to data     */
         )

/*-----------------------------------------------------------------------------
FUNCTIONAL_DESCRIPTION
This function is used to return the length of request-datas or response-datas
of the following SET- / Read-Value-Services.

- FM7_SET_BUSPARAMETER
- FM7_READ_BUSPARAMETER

- FM7_SET_VALUE_LOC
- FM7_SET_VALUE_REM
- FM7_READ_VALUE_LOC
- FM7_READ_VALUE_REM

possible return values:
- Data-length


-----------------------------------------------------------------------------*/
{
  LOCAL_VARIABLES

  FUNCTION_BODY

  switch (service)
  {
       case FM7_SET_VALUE_LOC:
       case FM7_SET_VALUE_REM:
            if (primitive == REQ)
            {
              T_SET_VALUE_REQ FAR *req = (T_SET_VALUE_REQ FAR*) data_ptr;
              return(sizeof (T_SET_VALUE_REQ) + req->length);
            }
            else  return(0);

       case FM7_SET_BUSPARAMETER:
            if (primitive == REQ) return(sizeof(T_SET_BUSPARAMETER_REQ));
            else                  return(0);


       case FM7_READ_VALUE_LOC:
       case FM7_READ_VALUE_REM:
            if (primitive == REQ)
            {
              return(sizeof(T_READ_VALUE_REQ));
            }
            else
            {
               T_READ_VALUE_CNF FAR *rsp = (T_READ_VALUE_CNF FAR*) data_ptr;
               return(sizeof(T_READ_VALUE_CNF) + rsp->length);
            }

       default:
            return(0);
  }
}




FUNCTION LOCAL INT16 fm7gdl_get_ident_data_len
         (
          IN    USIGN8     primitive         /* Service-Primitive   */
         )

/*-----------------------------------------------------------------------------
FUNCTIONAL_DESCRIPTION

This function is used to return the length of request-datas or response-datas
of the following Ident-Services.

- IDENT_LOC
- IDENT_REM

possible return values:
- Data-length

-----------------------------------------------------------------------------*/
{
  LOCAL_VARIABLES

  FUNCTION_BODY

  if (primitive == REQ) return(sizeof(T_IDENT_REQ));
  else                  return(sizeof(T_IDENT_CNF));
}





FUNCTION LOCAL INT16 fm7gdl_get_live_list_data_len
         (
          IN    VOID
         )

/*-----------------------------------------------------------------------------
FUNCTIONAL_DESCRIPTION
This function is used to return the length of request-datas
of the Get-Live-List Service.

possible return values:
- Data-length

-----------------------------------------------------------------------------*/
{
  LOCAL_VARIABLES

  FUNCTION_BODY

  return(0);
}





FUNCTION LOCAL INT16 fm7gdl_get_lsap_status_data_len
         (
          IN    USIGN8     primitive
         )

/*-----------------------------------------------------------------------------
FUNCTIONAL_DESCRIPTION
This function is used to return the length of request-datas or response-datas
of the following LSAP-Status Services.

- LSAP_STATUS_LOC
- LSAP_STATUS_REM

possible return values:
- Data-length

-----------------------------------------------------------------------------*/
{
  LOCAL_VARIABLES

  FUNCTION_BODY

  if (primitive == REQ) return(sizeof (T_LSAP_STATUS_REQ));
  else                  return(sizeof (T_LSAP_STATUS_CNF));
}






FUNCTION LOCAL INT16 fm7gdl_get_fault_mngt_data_len
         (
          IN    VOID
         )

/*-----------------------------------------------------------------------------
FUNCTIONAL_DESCRIPTION
This function is used to return the length of request-datas
of the following Fault-Management Services.

- RESET
- EXIT
- FM7_EVENT

possible return values:
- Data-length

-----------------------------------------------------------------------------*/
{
  LOCAL_VARIABLES

  FUNCTION_BODY

  return(0);
}



FUNCTION LOCAL INT16 fm7gdl_get_error_data_len
         (
          IN    USIGN8     service           /* Service  */
         )

/*-----------------------------------------------------------------------------
FUNCTIONAL_DESCRIPTION
This function is used to return the length of response-error-datas
of the following FM7-Services.

- FM7_INITIATE
- FM7_READ_CRL_REM
- FM7_INIT_LOAD_CRL_REM
- FM7_LOAD_CRL_REM
- FM7_TERM_LOAD_CRL_REM
- FM7_SET_VALUE_REM
- FM7_READ_VALUE_REM
- FM7_LSAP_STATUS_REM
- FM7_IDENT_REM
- FM7_READ_CRL_LOC
- FM7_INIT_LOAD_CRL_LOC
- FM7_LOAD_CRL_LOC
- FM7_TERM_LOAD_CRL_LOC
- FM7_SET_VALUE_LOC
- FM7_READ_VALUE_LOC
- FM7_LSAP_STATUS_LOC
- FM7_IDENT_LOC
- FM7_EVENT
- FM7_RESET
- FM7_EXIT
- FM7_SET_BUSPARAMETER
- FM7_READ_BUSPARAMETER
- FM7_GET_LIVE_LIST

possible return values:
- Data-length

-----------------------------------------------------------------------------*/
{
  LOCAL_VARIABLES

  FUNCTION_BODY

  switch(service)
  {
     case FM7_INITIATE:
          return(sizeof(T_FM7_INIT_ERR_CNF));

     case FM7_READ_CRL_REM:
     case FM7_INIT_LOAD_CRL_REM:
     case FM7_LOAD_CRL_REM:
     case FM7_SET_VALUE_REM:
     case FM7_READ_VALUE_REM:
     case FM7_LSAP_STATUS_REM:
     case FM7_IDENT_REM:
     case FM7_READ_CRL_LOC:
     case FM7_INIT_LOAD_CRL_LOC:
     case FM7_LOAD_CRL_LOC:
     case FM7_SET_VALUE_LOC:
     case FM7_READ_VALUE_LOC:
     case FM7_LSAP_STATUS_LOC:
     case FM7_IDENT_LOC:

     case FM7_EVENT:
     case FM7_RESET:
     case FM7_EXIT:
     case FM7_SET_BUSPARAMETER:
     case FM7_READ_BUSPARAMETER:
     case FM7_GET_LIVE_LIST:
          return(sizeof(T_ERROR));


     case FM7_TERM_LOAD_CRL_LOC:
     case FM7_TERM_LOAD_CRL_REM:
          return(sizeof(T_CRL_ERROR));

     default:
          return(0);

  }
}



FUNCTION PUBLIC INT16 fm7gdl_get_data_len
          (
            IN  INT16      result,                          /* Service-Result */
            IN  USIGN8     service,                                /* Service */
            IN  USIGN8     primitive,                    /* Service-Primitive */
            IN  USIGN8 FAR *data_ptr,                      /* pointer to data */
            OUT INT16      *data_len_ptr                    /* length of data */
          )

/*-----------------------------------------------------------------------------
FUNCTIONAL_DESCRIPTION

this function is used to return the data length of FM7-SERVICES

possible return values:
- Data-length

-----------------------------------------------------------------------------*/
{
  LOCAL_VARIABLES


  FUNCTION_BODY

  *data_len_ptr = 0;

  switch (service)
  {
     case FM7_READ_CRL_LOC:
     case FM7_INIT_LOAD_CRL_LOC:
     case FM7_LOAD_CRL_LOC:
     case FM7_TERM_LOAD_CRL_LOC:
     case FM7_READ_CRL_REM:
     case FM7_INIT_LOAD_CRL_REM:
     case FM7_LOAD_CRL_REM:
     case FM7_TERM_LOAD_CRL_REM:
          if (result == POS) *data_len_ptr = fm7gdl_get_crl_data_len(service,primitive,data_ptr);
          else               *data_len_ptr = fm7gdl_get_error_data_len(service);
          break;

     case FM7_SET_VALUE_LOC:
     case FM7_READ_VALUE_LOC:
     case FM7_SET_BUSPARAMETER:
     case FM7_READ_BUSPARAMETER:
     case FM7_SET_VALUE_REM:
     case FM7_READ_VALUE_REM:
          if (result == POS) *data_len_ptr = fm7gdl_get_s_r_value_data_len(service,primitive,data_ptr);
          else               *data_len_ptr = fm7gdl_get_error_data_len(service);
          break;

     case FM7_LSAP_STATUS_LOC:
     case FM7_LSAP_STATUS_REM:
          if (result == POS) *data_len_ptr = fm7gdl_get_lsap_status_data_len(primitive);
          else               *data_len_ptr = fm7gdl_get_error_data_len(service);
          break;

     case FM7_IDENT_LOC:
     case FM7_IDENT_REM:
          if (result == POS) *data_len_ptr = fm7gdl_get_ident_data_len(primitive);
          else               *data_len_ptr = fm7gdl_get_error_data_len(service);
          break;

     case FM7_EVENT:
     case FM7_RESET:
     case FM7_EXIT:
          if (result == POS) *data_len_ptr = fm7gdl_get_fault_mngt_data_len();
          else               *data_len_ptr = fm7gdl_get_error_data_len(service);
          break;

     case FM7_GET_LIVE_LIST:
          if (result == POS) *data_len_ptr = fm7gdl_get_live_list_data_len();
          else               *data_len_ptr = fm7gdl_get_error_data_len(service);
          break;

     case FM7_INITIATE:
     case FM7_ABORT:
          if (result == POS) *data_len_ptr = fm7gdl_get_ctxt_data_len(service,primitive);
          else               *data_len_ptr = fm7gdl_get_error_data_len(service);
          break;

     default:
          return(E_IF_INVALID_SERVICE);
          break;
   }


  return(E_OK);
}

#if defined (WIN32) || defined (_WIN32) || defined (WIN16) || defined (_WIN16)
#pragma check_stack
#endif
