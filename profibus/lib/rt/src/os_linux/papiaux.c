/*****************************************************************************
*                                                                            *
*                                 SOFTING AG                                 *
*                          Richard-Reitzner-Allee 6                          *
*                                D-85540 Haar                                *
*                        Phone: ++49-89-4 56 56-0                            *
*                          Fax: ++49-89-4 56 56-3 99                         *
*                                                                            *
*                     Copyright (C) SOFTING AG 1999-2003                     *
*                             All Rights Reserved                            *
*                                                                            *
******************************************************************************

FILE_NAME               PAPIAUX.C

PROJECT_NAME            PROFIBUS

MODULE                  PAPIAUX

COMPONENT_LIBRARY       PAPI Lib
                        PAPI DLL

AUTHOR                  SOFTING GmbH

VERSION                 5.22.0.00.release

DATE                    26-February-1999

STATUS                  finished

FUNCTIONAL_MODULE_DESCRIPTION

This modul contains auxiliary functions to

- get registry entries
- to swap data from little endian to big endian

RELATED_DOCUMENTS
=============================================================================*/

#include "keywords.h"

INCLUDES

#if defined (WIN32) || defined (_WIN32) || defined (WIN16) || defined (_WIN16)
#include <windows.h>
#endif
#include <stdio.h>

#ifdef _LINUX
#include <sys/select.h>
#endif

#include "pb_type.h"

#if defined (WIN32) || defined (_WIN32)
#include "pb_ntdrv.h"
#endif


FUNCTION_DECLARATIONS


LOCAL_DEFINES

LOCAL_TYPEDEFS

EXPORT_DATA

IMPORT_DATA

LOCAL_DATA



// ***************************************************************************
// * FUNCTIONs to access system registry
// ***************************************************************************


#if defined (WIN32) || defined (_WIN32)
FUNCTION PUBLIC BOOL ReadBoardRegistryEntries
         (
           IN  USIGN8    BoardNumber,
           OUT USIGN32 * OperationMode
         )
/*-----------------------------------------------------------------------------
FUNCTIONAL_DESCRIPTION

This function is used read the OperationMode from the system registry specified
by the BoardNumber.

return value:
- TRUE     read registry entry is successfully
- FALSE    read registry entry is not successfully
-----------------------------------------------------------------------------*/
{
  LOCAL_VARIABLES

  LONG  RetVal;
  HKEY  keyParameters = NULL;
  HKEY  keyBoard      = NULL;
  char  szKeyBoard[100];
  char  szBoardType[100];
  DWORD sizeULONG = sizeof(ULONG);
  DWORD SizeofBoardType;

  FUNCTION_BODY

  if (ERROR_SUCCESS == RegOpenKeyEx(HKEY_LOCAL_MACHINE,REGPATH_PARAMETER_FROM_HLM,0,KEY_READ,&keyParameters))
  {
    sprintf (szKeyBoard, "%s\\%d", REGPATH_PARAMETER_FROM_HLM,BoardNumber);
    if (ERROR_SUCCESS == RegOpenKeyEx(HKEY_LOCAL_MACHINE,szKeyBoard,0,KEY_READ,&keyBoard))
    {
      SizeofBoardType = sizeof(szBoardType);
      szBoardType[0]    = '\0';

      if ((RetVal = RegQueryValueEx(keyBoard,"Type",NULL,NULL,(LPBYTE) szBoardType,&SizeofBoardType)) == ERROR_SUCCESS)
      {
        RetVal |= RegQueryValueEx(keyBoard,"OperationMode",NULL,NULL,(LPBYTE) OperationMode,&sizeULONG);

        if (!RetVal) return(TRUE);
      }
    }
  }
  return(FALSE);
}

#endif

/****************************************************************************/

#ifdef _LINUX

FUNCTION PUBLIC int kbhit (VOID)
  /*------------------------------------------------------------------------*/
  /* FUNCTIONAL_DESCRIPTION                                                 */
  /*------------------------------------------------------------------------*/
  /*                                                                        */
  /*------------------------------------------------------------------------*/
{
LOCAL_VARIABLES

  fd_set          rd_fds;
  struct timeval  timeout; 

FUNCTION_BODY

  FD_ZERO (&rd_fds);
  FD_SET (0, &rd_fds);
  
  timeout.tv_sec  = 0L;
	timeout.tv_usec = 1000L; /* wait 1 ms */
	
	if (select (1, &rd_fds, NULL, NULL, &timeout) > 0)
	{
	  return (1);
	}

	return (0);
} /* kbhit */

#endif

// ***************************************************************************
// * FUNCTIONs to convert data from LITTLE ENDIAN to BIG-ENDIAN format
// ***************************************************************************


FUNCTION PUBLIC USIGN16 swap_16_intel_motorola
         (
           IN USIGN16  input16   /* value to swap */
         )

/*-----------------------------------------------------------------------------
FUNCTIONAL_DESCRIPTION

This function is used to swap a INTEGER16 or USIGN16 value from LITTLE ENDIAN to
BIG-ENDIAN format.

IN:  input16  -> 16-Bit value to swap

return value:
- swapped INTEGER16 or USIGN16 value
-----------------------------------------------------------------------------*/
{
  LOCAL_VARIABLES

  USIGN16 inp16;

  FUNCTION_BODY

  ((USIGN8*) &inp16) [0] = ((USIGN8*) &input16) [1];
  ((USIGN8*) &inp16) [1] = ((USIGN8*) &input16) [0];

  return (inp16);

}


FUNCTION PUBLIC USIGN32 swap_32_intel_motorola
         (
           IN USIGN32  input32   /* value to swap */
         )

/*-----------------------------------------------------------------------------
FUNCTIONAL_DESCRIPTION

This function is used to swap a INTEGER32 or USIGN32 value from LITTLE ENDIAN to
BIG-ENDIAN format.

IN:  input32  -> 32-Bit value to swap

return value:
- swapped INTEGER32 or USIGN32 value
-----------------------------------------------------------------------------*/
{
  LOCAL_VARIABLES

  USIGN32 inp32;

  FUNCTION_BODY

  ((USIGN8*) &inp32) [0] = ((USIGN8*) &input32) [3];
  ((USIGN8*) &inp32) [1] = ((USIGN8*) &input32) [2];
  ((USIGN8*) &inp32) [2] = ((USIGN8*) &input32) [1];
  ((USIGN8*) &inp32) [3] = ((USIGN8*) &input32) [0];

  return (inp32);

}


