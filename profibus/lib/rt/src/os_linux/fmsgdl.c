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

FILE_NAME               FMSGDL.C

PROJECT_NAME            PROFIBUS

MODULE                  FMSGDL

COMPONENT_LIBRARY       PAPI Lib
                        PAPI DLL

AUTHOR                  SOFTING AG

VERSION                 5.21.0.00.release

DATE                    27-February-1998

STATUS                  finished

FUNCTIONAL_MODULE_DESCRIPTION

This modul contains FMS-Service-Specific-Functions which return the length
length of the Request- or Response-Datas.



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
#include "pb_fms.h"


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

FUNCTION LOCAL  INT16  fmsgdl_get_ctxt_data_len
          (
            IN  USIGN8     service,          /* Service              */
            IN  USIGN8     primitive         /* Service-Primitive    */
          )

/*-----------------------------------------------------------------------------
FUNCTIONAL_DESCRIPTION
This function is used to return the length of request-datas or response-datas
of the following FMS-Context-Management-Services.

- FMS_INITIATE
- FMS_ABORT

possible return values:
- Data-length

-----------------------------------------------------------------------------*/
{
  LOCAL_VARIABLES

  FUNCTION_BODY

  switch (service)
  {
    case FMS_INITIATE:
         if (primitive == REQ) return(sizeof(T_CTXT_INIT_REQ));
         else                  return(sizeof(T_CTXT_INIT_CNF));

    case FMS_ABORT:
         if (primitive == REQ) return(sizeof(T_CTXT_ABORT_REQ));
         else                  return(0);

    default:
         return(0);
  }
}




FUNCTION LOCAL INT16 fmsgdl_get_vfd_data_len
          (
            IN  USIGN8     service,          /* Service              */
            IN  USIGN8     primitive         /* Service-Primitive    */
          )

/*-----------------------------------------------------------------------------
FUNCTIONAL_DESCRIPTION
This function is used to return the length of request-datas or response-datas
of the following VFD-Services.

- FMS_STATUS
- FMS_IDENTIFY
- FMS_UNSOLICITEDSTATUS
- FMS_CREATE_VFD_LOC
- FMS_VFD_SET_PHYS_STATUS_LOC

possible return values:
- Data-length

-----------------------------------------------------------------------------*/
{
  LOCAL_VARIABLES

  FUNCTION_BODY

  switch (service)
  {
    case FMS_STATUS:
         if (primitive == RES) return(sizeof(T_VFD_STATUS_CNF));
         else                  return(0);

    case FMS_IDENTIFY:
         if (primitive == RES) return(sizeof(T_VFD_IDENTIFY_CNF));
         else                  return(0);

    case FMS_UNSOLICITEDSTATUS:
         if (primitive == REQ) return(sizeof(T_VFD_UNSOL_STATUS_REQ));
         else                  return(0);

    case FMS_CREATE_VFD_LOC:
         if (primitive == REQ) return(sizeof(T_VFD_CREATE_REQ));
         else                  return(0);

    case FMS_VFD_SET_PHYS_STATUS_LOC:
         if (primitive == REQ) return(sizeof(T_VFD_SET_PHYS_STATUS_REQ));
         else                  return(0);

    default:
         return(0);
  }
}






FUNCTION LOCAL INT16  fmsgdl_get_od_data_len
          (
            IN  USIGN8     service,          /* Service              */
            IN  USIGN8     primitive,        /* Service-Primitive    */
            IN  USIGN8 FAR *data_ptr         /* pointer to data      */
          )

/*-----------------------------------------------------------------------------
FUNCTIONAL_DESCRIPTION
This function is used to return the length of request-datas or response-datas
of the following OD-Services.

- FMS_INIT_PUT_OD
- FMS_PUT_OD
- FMS_TERM_PUT_OD
- FMS_INIT_LOAD_OD_LOC
- FMS_LOAD_OD_LOC
- FMS_TERM_LOAD_OD_LOC
- FMS_GET_OD
- FMS_OD_READ_LOC


possible return values:
- Data-length

-----------------------------------------------------------------------------*/
{
  LOCAL_VARIABLES

  FUNCTION_BODY

  switch(service)
  {
     case FMS_GET_OD:
          if (primitive == REQ)
          {
            return(sizeof(T_GET_OD_REQ));
          }
          else
          {
            T_GET_OD_CNF FAR* rsp           = (T_GET_OD_CNF FAR*) data_ptr;
            USIGN8       FAR* obj_descr_ptr = (USIGN8       FAR*) (rsp+1);
            USIGN16           offset        = 0;
            USIGN16           size          = 0;
            USIGN8            i;

            if (rsp->no_of_od_descr != 0)
            {
              for (i=0; i<rsp->no_of_od_descr; i++)
              {
                size   += (obj_descr_ptr[offset] + 1);
                offset =  size;
              }
              return(sizeof(T_GET_OD_CNF) + size);
            }
            else
            {
              return(0);
            }
          }

     case FMS_INIT_PUT_OD:
          if (primitive == REQ) return(sizeof(T_INIT_PUT_OD_REQ));
          else                  return(0);

     case FMS_PUT_OD:
          if (primitive == REQ)
          {
            T_PUT_OD_REQ FAR *req              = (T_PUT_OD_REQ FAR*) data_ptr;
            USIGN8       FAR *obj_descr_ptr    = (USIGN8       FAR*) (req+1);
            USIGN16          offset            = 0;
            USIGN16          size              = 0;
            USIGN8           i;

            for (i=0; i<req->no_of_od_descr; i++)
            {
              size   += (obj_descr_ptr[offset] + 1);
              offset =  size;
            }
            return(sizeof(T_PUT_OD_REQ) + size);
          }
          else  return(0);

     case FMS_INIT_LOAD_OD_LOC:
          if (primitive == REQ) return(sizeof(T_INIT_LOAD_OD_REQ));
          else                  return(0);

     case FMS_TERM_LOAD_OD_LOC:
          if (primitive == REQ) return(sizeof(T_TERM_LOAD_OD_REQ));
          else                  return(0);

     case FMS_LOAD_OD_LOC:
          if (primitive == REQ)
          {
            T_LOAD_OD_REQ    FAR *req    = (T_LOAD_OD_REQ    FAR *) data_ptr;
            T_OD_NULL_OBJECT FAR *object = (T_OD_NULL_OBJECT FAR*)
                                        &req->obj_descr.id.null_obj_descr;
            switch (object->obj_code)
            {
              case TYPE_STRUCT_OBJECT:
                   return(sizeof(T_LOAD_OD_REQ) +
                          (req->obj_descr.id.ds_obj_descr.no_of_elements *
                           sizeof(T_OD_DT_LIST))
                         );

               case RECORD_OBJECT:
                    return(sizeof(T_LOAD_OD_REQ) +
                           (req->obj_descr.id.r_var_obj_descr.no_of_address *
                           sizeof(USIGN32))
                          );

              case VAR_LIST_OBJECT:
                   return(sizeof(T_LOAD_OD_REQ) +
                          (req->obj_descr.id.vlist_obj_descr.no_of_var * sizeof(USIGN16))
                         );

              case INVOCATION_OBJECT:
                   return(sizeof(T_LOAD_OD_REQ) +
                          (req->obj_descr.id.pi_obj_descr.cnt_dom * sizeof(USIGN16))
                         );


              default:
                   return(sizeof(T_LOAD_OD_REQ));
            }
          }
          else  return(0);

     case FMS_OD_READ_LOC:
          if (primitive == REQ) return(sizeof(T_OD_READ_LOC_REQ));
          else                  return(0);

     default:
          return(0);

  }
}



FUNCTION LOCAL INT16 fmsgdl_get_var_data_len
          (
            IN  USIGN8      service,          /* Service              */
            IN  USIGN8      primitive,        /* Service-Primitive    */
            IN  USIGN8  FAR *data_ptr         /* pointer to data      */
          )

/*-----------------------------------------------------------------------------
FUNCTIONAL_DESCRIPTION
This function is used to return the length of request-datas or response-datas
of the following Variable-Access-Services.

- FMS_READ
- FMS_READ_WITH_TYPE
- FMS_WRITE
- FMS_WRITE_WITH_TYPE
- FMS_INFO_RPT
- FMS_INFO_RPT_WITH_TYPE
- FMS_PHYS_READ
- FMS_PHYS_WRITE
- FMS_DEF_VAR_LIST
- FMS_DEL_VAR_LIST

possible return values:
- Data-length

-----------------------------------------------------------------------------*/
{
  LOCAL_VARIABLES

  FUNCTION_BODY

  switch(service)
  {
    case FMS_READ:
         if (primitive == REQ)
         {
           return(sizeof(T_VAR_READ_REQ));
         }
         else
         {
            T_VAR_READ_CNF FAR *rsp = (T_VAR_READ_CNF FAR *) data_ptr;
            return(sizeof(T_VAR_READ_CNF) + rsp->length);
         }

    case FMS_READ_WITH_TYPE:
         if (primitive == REQ)
         {
           return(sizeof(T_VAR_READ_WITH_TYPE_REQ));
         }
         else
         {
           T_VAR_READ_WITH_TYPE_CNF FAR *rsp = (T_VAR_READ_WITH_TYPE_CNF FAR*) data_ptr;
           return(sizeof(T_VAR_READ_WITH_TYPE_CNF)               +
                  (sizeof(T_TYPE_DESCR) * rsp->no_of_type_descr) +
                  rsp->length
                 );
         }

    case FMS_WRITE:
         if (primitive == REQ)
         {
           T_VAR_WRITE_REQ FAR *req = (T_VAR_WRITE_REQ FAR *) data_ptr;
           return(sizeof(T_VAR_WRITE_REQ) + req->length);
         }
         else  return(0);

    case FMS_WRITE_WITH_TYPE:
         if (primitive == REQ)
         {
           T_VAR_WRITE_WITH_TYPE_REQ FAR *req = (T_VAR_WRITE_WITH_TYPE_REQ FAR *) data_ptr;
           return(sizeof(T_VAR_WRITE_WITH_TYPE_REQ)              +
                  (sizeof(T_TYPE_DESCR) * req->no_of_type_descr) +
                  req->length
                 );
         }
         else  return(0);

    case FMS_INFO_RPT:
         if (primitive == REQ)
         {
           T_VAR_INFO_RPT_REQ FAR *req = (T_VAR_INFO_RPT_REQ FAR *) data_ptr;
           return(sizeof(T_VAR_INFO_RPT_REQ) + req->length);
         }
         else  return(0);

    case FMS_INFO_RPT_WITH_TYPE:
         if (primitive == REQ)
         {
           T_VAR_INFO_RPT_WITH_TYPE_REQ FAR *req = (T_VAR_INFO_RPT_WITH_TYPE_REQ FAR*) data_ptr;
           return(sizeof(T_VAR_INFO_RPT_WITH_TYPE_REQ)           +
                  (sizeof(T_TYPE_DESCR) * req->no_of_type_descr) +
                  req->length
                 );
         }
         else  return(0);

    case FMS_PHYS_READ:
         if (primitive == REQ)
         {
           return(sizeof(T_VAR_PHYS_READ_REQ));
         }
         else
         {
           T_VAR_PHYS_READ_CNF FAR *rsp = (T_VAR_PHYS_READ_CNF FAR *) data_ptr;
           return(sizeof(T_VAR_PHYS_READ_CNF) + rsp->length);
         }

    case FMS_PHYS_WRITE:
         if (primitive == REQ)
         {
           T_VAR_PHYS_WRITE_REQ FAR *req = (T_VAR_PHYS_WRITE_REQ FAR*) data_ptr;
           return(sizeof(T_VAR_PHYS_WRITE_REQ) + req->length);
         }
         else  return(0);

    case FMS_DEF_VAR_LIST:
         if (primitive == REQ)
         {
           T_VAR_DEFINE_VAR_LIST_REQ FAR *req = (T_VAR_DEFINE_VAR_LIST_REQ FAR *) data_ptr;

           USIGN8     FAR *d_acc_spec_list   = (USIGN8     FAR*) (req+1);
           USIGN16        offset             = 0;
           USIGN16        acc_spec_list_size = 0;
           USIGN8         i                  = 0;

           for (i=0; i<req->no_of_var; i++)
           {
                  offset++;                    /* offset to length of acc_spec */
                  acc_spec_list_size += sizeof(T_DYN_ACC_SPEC) +
                                        d_acc_spec_list[offset];
                  offset = acc_spec_list_size;
           }
           return(sizeof(T_VAR_DEFINE_VAR_LIST_REQ) + acc_spec_list_size);
         }
         else
         {
           return(sizeof(T_VAR_DEFINE_VAR_LIST_CNF));
         }

    case FMS_DEL_VAR_LIST:
         if (primitive == REQ) return(sizeof(T_VAR_DELETE_VAR_LIST_REQ));
         else                  return(0);

    default:
         return(0);
  }
}



FUNCTION LOCAL  INT16  fmsgdl_get_evn_data_len
          (
            IN  USIGN8      service,          /* Service              */
            IN  USIGN8      primitive,        /* Service-Primitive    */
            IN  USIGN8  FAR *data_ptr         /* pointer to data      */
          )

/*-----------------------------------------------------------------------------
FUNCTIONAL_DESCRIPTION
This function is used to return the length of request-datas or response-datas
of the following Event-Management-Services.

- FMS_EVN_NOTIFY
- FMS_EVN_NOTIFY_WITH_TYPE
- FMS_ALT_EVN_CND_MNT
- FMS_ACK_EVN_NOTIFY

possible return values:
- Data-length

-----------------------------------------------------------------------------*/
{
  LOCAL_VARIABLES

  FUNCTION_BODY

  switch (service)
  {
     case FMS_EVN_NOTIFY:
          if (primitive == REQ)
          {
            T_EVENT_NOTIFY_REQ FAR *req = (T_EVENT_NOTIFY_REQ FAR *) data_ptr;
            return(sizeof(T_EVENT_NOTIFY_REQ) + req->data_length);
          }
          else  return(0);

     case FMS_EVN_NOTIFY_WITH_TYPE:
          if (primitive == REQ)
          {
            T_EVENT_NOTIFY_WITH_TYPE_REQ FAR *req = (T_EVENT_NOTIFY_WITH_TYPE_REQ FAR *) data_ptr;
            return(sizeof(T_EVENT_NOTIFY_WITH_TYPE_REQ) + req->data_length);
          }
          else  return(0);

     case FMS_ALT_EVN_CND_MNT:
          if (primitive == REQ) return(sizeof(T_ALT_EVN_CND_MNT_REQ));
          else                  return(0);

     case FMS_ACK_EVN_NOTIFY:
          if (primitive == REQ) return(sizeof(T_ACK_EVN_NOTIFY_REQ));
          else                  return(0);

     default:
          return(0);

  }
}



FUNCTION LOCAL INT16 fmsgdl_get_dom_data_len
          (
            IN  USIGN8      service,          /* Service              */
            IN  USIGN8      primitive,        /* Service-Primitive    */
            IN  USIGN8  FAR *data_ptr         /* pointer to data      */
          )

/*-----------------------------------------------------------------------------
FUNCTIONAL_DESCRIPTION
This function is used to return the length of request-datas or response-datas
of the following Domain-Management-Services.

- FMS_INIT_DOWNL_SEQ
- FMS_DOWNL_SEG
- FMS_TERM_DOWNL_SEQ

- FMS_GEN_INIT_DOWNL_SEQ
- FMS_GEN_DOWNL_SEG
- FMS_GEN_TERM_DOWNL_SEQ

- FMS_INI_UPL_SEQ
- FMS_UPL_SEG
- FMS_TERM_UPL_SEQ

- FMS_REQ_DOM_DOWNL
- FMS_REQ_DOM_UPL

possible return values:
- Data-length

-----------------------------------------------------------------------------*/
{
  LOCAL_VARIABLES

  FUNCTION_BODY

  switch (service)
  {
      case FMS_INIT_DOWNL_SEQ:
      case FMS_GEN_INIT_DOWNL_SEQ:
      case FMS_INIT_UPL_SEQ:
      case FMS_TERM_UPL_SEQ:
           if (primitive == REQ) return(sizeof(T_DOM_REQ));
           else                  return(0);

      case FMS_DOWNL_SEG:
      case FMS_UPL_SEG:
           if (primitive == REQ)
           {
             return(sizeof(T_DOM_REQ));
           }
           else
           {
             T_DNL_UPL_SEG_CNF FAR *rsp = (T_DNL_UPL_SEG_CNF FAR *) data_ptr;
             return(sizeof(T_DNL_UPL_SEG_CNF) + rsp->data_len);
           }

      case FMS_TERM_DOWNL_SEQ:
           if (primitive == REQ) return(sizeof(T_TERM_DNL_REQ));
           else                  return(0);

      case FMS_GEN_DOWNL_SEG:
           if (primitive == REQ)
           {
             T_GEN_DNL_SEG_REQ FAR *req = (T_GEN_DNL_SEG_REQ FAR *) data_ptr;
             return(sizeof(T_GEN_DNL_SEG_REQ ) + req->data_len);
           }
           else return(0);


      case FMS_GEN_TERM_DOWNL_SEQ:
           if (primitive == REQ) return(sizeof(T_DOM_REQ));
           else                  return(sizeof(T_GEN_TERM_DNL_CNF));


      case FMS_REQ_DOM_DOWNL:
      case FMS_REQ_DOM_UPL:
           if (primitive == REQ)
           {
             T_REQUEST_DOM_REQ FAR *req = (T_REQUEST_DOM_REQ FAR *) data_ptr;
             return(sizeof(T_REQUEST_DOM_REQ) + req->add_info_length);
           }
           else return(0);

      default:
           return(0);

  }
}



FUNCTION LOCAL INT16 fmsgdl_get_pi_data_len
          (
            IN  USIGN8      service,          /* Service              */
            IN  USIGN8      primitive,        /* Service-Primitive    */
            IN  USIGN8  FAR *data_ptr         /* pointer to data      */
          )

/*-----------------------------------------------------------------------------
FUNCTIONAL_DESCRIPTION
This function is used to return the length of request-datas or response-datas
of the following Program-Invocation-Management-Services.

- FMS_PI_CREATE
- FMS_PI_DELETE
- FMS_PI_START
- FMS_PI_STOP
- FMS_PI_RESUME
- FMS_PI_RESET
- FMS_PI_KILL
- FMS_PI_SET_STATE_LOC

possible return values:
- Data-length

-----------------------------------------------------------------------------*/
{
  LOCAL_VARIABLES

  FUNCTION_BODY

  switch (service)
  {
     case FMS_PI_CREATE:
          if (primitive == REQ)
          {
            T_PI_CR8_REQ FAR *req               = (T_PI_CR8_REQ FAR*) data_ptr;
            USIGN8       FAR *d_acc_spec_list   = (USIGN8       FAR*) (req+1);
            USIGN16          offset             = 0;
            USIGN16          acc_spec_list_size = 0;
            USIGN8           i                  = 0;

            /* - dynamic access specification list --------------------------*/
            for (i=0; i<req->cnt_dom; i++)
            {
                  offset++;                    /* offset to length of acc_spec */
                  acc_spec_list_size += sizeof(T_DYN_ACC_SPEC) +
                                        d_acc_spec_list[offset];
                  offset = acc_spec_list_size;
            }
            return(sizeof(T_PI_CR8_REQ) + acc_spec_list_size);
          }
          else
          {
            return(sizeof(T_PI_CR8_CNF));
          }

     case FMS_PI_DELETE:
          if (primitive == REQ) return(sizeof(T_PI_DEL_REQ));
          else                  return(0);

     case FMS_PI_START:
          if (primitive == REQ) return(sizeof(T_PI_START_REQ));
          else                  return(0);

     case FMS_PI_STOP:
          if (primitive == REQ) return(sizeof(T_PI_STOP_REQ));
          else                  return(0);

     case FMS_PI_RESUME:
          if (primitive == REQ) return(sizeof(T_PI_RESUME_REQ));
          else                  return(0);

     case FMS_PI_RESET:
          if (primitive == REQ) return(sizeof(T_PI_RESET_REQ));
          else                  return(0);

     case FMS_PI_KILL:
          if (primitive == REQ) return(sizeof(T_PI_KILL_REQ));
          else                  return(0);

     case FMS_PI_SET_STATE_LOC:
          if (primitive == REQ) return(sizeof(T_PI_SET_STATE_REQ));
          else                  return(0);

     default:
          return(0);
  }
}




FUNCTION LOCAL INT16 fmsgdl_get_error_data_len
         (
          IN    USIGN8     service           /* Service  */
         )

/*-----------------------------------------------------------------------------
FUNCTIONAL_DESCRIPTION
This function is used to return the length of response-error-datas
of the following FMS-Services.


- FMS_INITIATE
- FMS_STATUS
- FMS_IDENTIFY
- FMS_READ
- FMS_WRITE
- FMS_GET_OD
- FMS_READ_WITH_TYPE
- FMS_WRITE_WITH_TYPE
- FMS_DEF_VAR_LIST
- FMS_DEL_VAR_LIST
- FMS_INIT_DOWNL_SEQ
- FMS_DOWNL_SEG
- FMS_TERM_DOWNL_SEQ
- FMS_GEN_INIT_DOWNL_SEQ
- FMS_GEN_DOWNL_SEG
- FMS_GEN_TERM_DOWNL_SEQ
- FMS_INIT_UPL_SEQ
- FMS_UPL_SEG
- FMS_TERM_UPL_SEQ
- FMS_REQ_DOM_DOWNL
- FMS_REQ_DOM_UPL
- FMS_PI_CREATE
- FMS_PI_DELETE
- FMS_PI_START
- FMS_PI_STOP
- FMS_PI_RESUME
- FMS_PI_RESET
- FMS_PI_KILL
- FMS_ALT_EVN_CND_MNT
- FMS_ACK_EVN_NOTIFY
- FMS_PHYS_READ
- FMS_PHYS_WRITE
- FMS_INIT_PUT_OD
- FMS_PUT_OD
- FMS_TERM_PUT_OD

possible return values:
- Data-length

-----------------------------------------------------------------------------*/
{
  LOCAL_VARIABLES

  FUNCTION_BODY

  switch(service)
  {
     case  FMS_INITIATE:
           return(sizeof(T_CTXT_INIT_ERR_CNF));

     case  FMS_STATUS:
     case  FMS_IDENTIFY:
     case  FMS_READ:
     case  FMS_WRITE:
     case  FMS_GET_OD:
     case  FMS_READ_WITH_TYPE:
     case  FMS_WRITE_WITH_TYPE:
     case  FMS_DEF_VAR_LIST:
     case  FMS_DEL_VAR_LIST:
     case  FMS_INIT_DOWNL_SEQ:
     case  FMS_DOWNL_SEG:
     case  FMS_TERM_DOWNL_SEQ:
     case  FMS_GEN_INIT_DOWNL_SEQ:
     case  FMS_GEN_DOWNL_SEG:
     case  FMS_GEN_TERM_DOWNL_SEQ:
     case  FMS_INIT_UPL_SEQ:
     case  FMS_UPL_SEG:
     case  FMS_TERM_UPL_SEQ:
     case  FMS_REQ_DOM_DOWNL:
     case  FMS_REQ_DOM_UPL:
     case  FMS_PI_CREATE:
     case  FMS_PI_DELETE:
     case  FMS_ALT_EVN_CND_MNT:
     case  FMS_ACK_EVN_NOTIFY:
     case  FMS_PHYS_READ:
     case  FMS_PHYS_WRITE:
     case  FMS_INIT_PUT_OD:
     case  FMS_PUT_OD:
           return(sizeof(T_ERROR));

     case  FMS_TERM_PUT_OD:
           return(sizeof(T_OD_ERROR));

     case  FMS_PI_START:
     case  FMS_PI_STOP:
     case  FMS_PI_RESUME:
     case  FMS_PI_RESET:
     case  FMS_PI_KILL:
           return(sizeof(T_PI_ERROR));

     default:
          return(0);

  }
}



FUNCTION PUBLIC INT16 fmsgdl_get_data_len
         (
            IN  INT16      result,                         /* Service-Result */
            IN  USIGN8     service,                               /* Service */
            IN  USIGN8     primitive,                   /* Service-Primitive */
            IN  USIGN8 FAR *data_ptr,                     /* pointer to data */
            OUT INT16      *data_len_ptr                   /* length of data */
         )

/*-----------------------------------------------------------------------------
FUNCTIONAL_DESCRIPTION

this function is used to return the data length of FMS-SERVICES

possible return values:
- Data-length

-----------------------------------------------------------------------------*/
{
  LOCAL_VARIABLES

  FUNCTION_BODY

  *data_len_ptr = 0;

  switch (service)
  {
      case FMS_INITIATE:
      case FMS_ABORT:
           if (result == POS) *data_len_ptr = fmsgdl_get_ctxt_data_len(service,primitive);
           else               *data_len_ptr = fmsgdl_get_error_data_len(service);
           break;

      case FMS_STATUS:
      case FMS_IDENTIFY:
      case FMS_UNSOLICITEDSTATUS:
      case FMS_CREATE_VFD_LOC:
      case FMS_VFD_SET_PHYS_STATUS_LOC:
           if (result == POS) *data_len_ptr = fmsgdl_get_vfd_data_len(service,primitive);
           else               *data_len_ptr = fmsgdl_get_error_data_len(service);
           break;


      case FMS_INIT_DOWNL_SEQ:
      case FMS_DOWNL_SEG:
      case FMS_TERM_DOWNL_SEQ:
      case FMS_GEN_INIT_DOWNL_SEQ:
      case FMS_GEN_DOWNL_SEG:
      case FMS_GEN_TERM_DOWNL_SEQ:
      case FMS_INIT_UPL_SEQ:
      case FMS_UPL_SEG:
      case FMS_TERM_UPL_SEQ:
      case FMS_REQ_DOM_DOWNL:
      case FMS_REQ_DOM_UPL:
           if (result == POS) *data_len_ptr = fmsgdl_get_dom_data_len(service,primitive,data_ptr);
           else               *data_len_ptr = fmsgdl_get_error_data_len(service);
           break;


      case FMS_EVN_NOTIFY:
      case FMS_EVN_NOTIFY_WITH_TYPE:
      case FMS_ALT_EVN_CND_MNT:
      case FMS_ACK_EVN_NOTIFY:
           if (result == POS) *data_len_ptr = fmsgdl_get_evn_data_len(service,primitive,data_ptr);
           else               *data_len_ptr = fmsgdl_get_error_data_len(service);
           break;


      case FMS_GET_OD:
      case FMS_INIT_PUT_OD:
      case FMS_PUT_OD:
      case FMS_TERM_PUT_OD:
      case FMS_OD_READ_LOC:
      case FMS_INIT_LOAD_OD_LOC:
      case FMS_LOAD_OD_LOC:
      case FMS_TERM_LOAD_OD_LOC:
           if (result == POS) *data_len_ptr = fmsgdl_get_od_data_len(service,primitive,data_ptr);
           else               *data_len_ptr = fmsgdl_get_error_data_len(service);
           break;


      case FMS_PI_CREATE:
      case FMS_PI_DELETE:
      case FMS_PI_START:
      case FMS_PI_STOP:
      case FMS_PI_RESUME:
      case FMS_PI_RESET:
      case FMS_PI_KILL:
      case FMS_PI_SET_STATE_LOC:
           if (result == POS) *data_len_ptr = fmsgdl_get_pi_data_len(service,primitive,data_ptr);
           else               *data_len_ptr = fmsgdl_get_error_data_len(service);
           break;


      case FMS_READ:
      case FMS_READ_WITH_TYPE:
      case FMS_WRITE:
      case FMS_WRITE_WITH_TYPE:
      case FMS_INFO_RPT:
      case FMS_INFO_RPT_WITH_TYPE:
      case FMS_DEF_VAR_LIST:
      case FMS_DEL_VAR_LIST:
      case FMS_PHYS_READ:
      case FMS_PHYS_WRITE:
           if (result == POS) *data_len_ptr = fmsgdl_get_var_data_len(service,primitive,data_ptr);
           else               *data_len_ptr = fmsgdl_get_error_data_len(service);
           break;

      default:
           return(E_IF_INVALID_SERVICE);
  }

  return(E_OK);
}

#if defined (WIN32) || defined (_WIN32) || defined (WIN16) || defined (_WIN16)
#pragma check_stack
#endif
