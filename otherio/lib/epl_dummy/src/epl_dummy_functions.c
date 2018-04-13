/*
  Dummy for libpowerlink.a
*/

#include <stdint.h>

typedef void *tEplKernel;
typedef int tEplObdSize;
typedef void *tEplApiInitParam;
typedef void *tEplNmtEvent;
typedef void *tEplEventSource;
typedef void *tEplApiProcessImageCopyJob;
typedef void *EplObdInitParam;
#define EPLDLLEXPORT
#define PUBLIC
#define MEM
#define WORD int
#define BOOL int

tEplKernel PUBLIC EplTgtInit(void) {return 0;}
EPLDLLEXPORT tEplKernel PUBLIC EplApiInitialize(tEplApiInitParam* pInitParam_p) {return 0;}
EPLDLLEXPORT tEplKernel PUBLIC EplApiSetCdcFilename(char* pszCdcFilename_p) {return 0;}
EPLDLLEXPORT tEplKernel PUBLIC EplApiProcessImageAlloc(
    unsigned int uiSizeProcessImageIn_p,
    unsigned int uiSizeProcessImageOut_p,
    unsigned int uiQueueEntriesLo_p,
    unsigned int uiQueueEntriesHi_p) {return 0;}
EPLDLLEXPORT tEplKernel PUBLIC EplApiExecNmtCommand(tEplNmtEvent NmtEvent_p) {return 0;}
EPLDLLEXPORT char * PUBLIC EplGetNmtEventStr(tEplNmtEvent nmtEvent_p) {return 0;}
EPLDLLEXPORT tEplKernel PUBLIC EplApiWriteLocalObject(
            unsigned int      uiIndex_p,
            unsigned int      uiSubindex_p,
            void*             pSrcData_p,
            unsigned int      uiSize_p) {return 0;}
EPLDLLEXPORT tEplKernel PUBLIC EplApiReadLocalObject(
            unsigned int      uiIndex_p,
            unsigned int      uiSubindex_p,
            void*             pDstData_p,
            unsigned int*     puiSize_p) {return 0;}
EPLDLLEXPORT char * PUBLIC EplGetEplKernelStr( tEplKernel EplKernel_p ) {return 0;}
EPLDLLEXPORT char * PUBLIC EplGetEventSourceStr(tEplEventSource eventSrc_p) {return 0;}
EPLDLLEXPORT const char * PUBLIC EplGetEmergErrCodeStr( WORD EmergErrCode_p ) {return 0;}
EPLDLLEXPORT tEplKernel PUBLIC EplApiProcessImageFree(void) {return 0;}
EPLDLLEXPORT tEplKernel PUBLIC EplApiShutdown(void) {return 0;}
EPLDLLEXPORT tEplKernel PUBLIC EplApiProcessImageLinkObject(
    unsigned int    uiObjIndex_p,
    unsigned int    uiFirstSubindex_p,
    unsigned int    uiOffsetPI_p,
    BOOL            fOutputPI_p,
    tEplObdSize     EntrySize_p,
    unsigned int*   puiVarEntries_p) {return 0;}
EPLDLLEXPORT tEplKernel PUBLIC EplApiProcessImageExchange(
    tEplApiProcessImageCopyJob* pCopyJob_p) {return 0;}
tEplKernel PUBLIC  EplObdInitRam (EplObdInitParam MEM * pInitParam_p) {return 0;}










