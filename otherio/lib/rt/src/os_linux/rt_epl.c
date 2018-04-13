#if defined PWRE_CONF_EPL && defined PWRE_CONF_LIBPCAP

#include "Epl.h"

//---------------------------------------------------------------------------
// const defines
//---------------------------------------------------------------------------

#define EPLAPI_PI_SUBINDEX_COUNT    252

//---------------------------------------------------------------------------
// local types
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// modul globale vars
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// local function prototypes
//---------------------------------------------------------------------------


/***************************************************************************/
/*                                                                         */
/*                                                                         */
/*          C L A S S  EplApi                                              */
/*                                                                         */
/*                                                                         */
/***************************************************************************/
//
// Description:
//
//
/***************************************************************************/


//=========================================================================//
//                                                                         //
//          P R I V A T E   D E F I N I T I O N S                          //
//                                                                         //
//=========================================================================//

//---------------------------------------------------------------------------
// const defines
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// local types
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// local vars
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// local function prototypes
//---------------------------------------------------------------------------

static tEplKernel EplApiProcessImageLinkRange(
    unsigned int    uiObjIndexStart_p,
    unsigned int    uiObjIndexEnd_p,
    unsigned int    uiOffsetPI_p,
    BOOL            fOutputPI_p,
    tEplObdSize     EntrySize_p,
    unsigned int    uiSubindexCountPerIndex_p);


//=========================================================================//
//                                                                         //
//          P U B L I C   F U N C T I O N S                                //
//                                                                         //
//=========================================================================//


//---------------------------------------------------------------------------
//
// Function:    EplApiProcessImageSetup()
//
// Description: sets up static process image
//
// Parameters:  (none)
//
// Returns:     tEplKernel              = error code
//
//
// State:
//
//---------------------------------------------------------------------------

tEplKernel PUBLIC EplApiProcessImageSetup(void)
{
tEplKernel      Ret = kEplSuccessful;

    Ret = EplApiProcessImageLinkRange(0xA000, 0xA00F, 0, FALSE, 1, EPLAPI_PI_SUBINDEX_COUNT);
    if (Ret != kEplSuccessful)
    {
        goto Exit;
    }

    Ret = EplApiProcessImageLinkRange(0xA040, 0xA04F, 0, FALSE, 1, EPLAPI_PI_SUBINDEX_COUNT);
    if (Ret != kEplSuccessful)
    {
        goto Exit;
    }

    Ret = EplApiProcessImageLinkRange(0xA0C0, 0xA0C7, 0, FALSE, 2, EPLAPI_PI_SUBINDEX_COUNT);
    if (Ret != kEplSuccessful)
    {
        goto Exit;
    }

    Ret = EplApiProcessImageLinkRange(0xA100, 0xA107, 0, FALSE, 2, EPLAPI_PI_SUBINDEX_COUNT);
    if (Ret != kEplSuccessful)
    {
        goto Exit;
    }

    Ret = EplApiProcessImageLinkRange(0xA1C0, 0xA1C3, 0, FALSE, 4, EPLAPI_PI_SUBINDEX_COUNT);
    if (Ret != kEplSuccessful)
    {
        goto Exit;
    }

    Ret = EplApiProcessImageLinkRange(0xA200, 0xA203, 0, FALSE, 4, EPLAPI_PI_SUBINDEX_COUNT);
    if (Ret != kEplSuccessful)
    {
        goto Exit;
    }

    Ret = EplApiProcessImageLinkRange(0xA480, 0xA48F, 0, TRUE, 1, EPLAPI_PI_SUBINDEX_COUNT);
    if (Ret != kEplSuccessful)
    {
        goto Exit;
    }

    Ret = EplApiProcessImageLinkRange(0xA4C0, 0xA4CF, 0, TRUE, 1, EPLAPI_PI_SUBINDEX_COUNT);
    if (Ret != kEplSuccessful)
    {
        goto Exit;
    }

    Ret = EplApiProcessImageLinkRange(0xA540, 0xA547, 0, TRUE, 2, EPLAPI_PI_SUBINDEX_COUNT);
    if (Ret != kEplSuccessful)
    {
        goto Exit;
    }

    Ret = EplApiProcessImageLinkRange(0xA580, 0xA587, 0, TRUE, 2, EPLAPI_PI_SUBINDEX_COUNT);
    if (Ret != kEplSuccessful)
    {
        goto Exit;
    }

    Ret = EplApiProcessImageLinkRange(0xA640, 0xA643, 0, TRUE, 4, EPLAPI_PI_SUBINDEX_COUNT);
    if (Ret != kEplSuccessful)
    {
        goto Exit;
    }

    Ret = EplApiProcessImageLinkRange(0xA680, 0xA683, 0, TRUE, 4, EPLAPI_PI_SUBINDEX_COUNT);
    if (Ret != kEplSuccessful)
    {
        goto Exit;
    }

Exit:
    return Ret;
}



//=========================================================================//
//                                                                         //
//          P R I V A T E   F U N C T I O N S                              //
//                                                                         //
//=========================================================================//

//---------------------------------------------------------------------------
//
// Function:    EplApiProcessImageSetup()
//
// Description: sets up static process image
//
// Parameters:  (none)
//
// Returns:     tEplKernel              = error code
//
//
// State:
//
//---------------------------------------------------------------------------

static tEplKernel EplApiProcessImageLinkRange(
    unsigned int    uiObjIndexStart_p,
    unsigned int    uiObjIndexEnd_p,
    unsigned int    uiOffsetPI_p,
    BOOL            fOutputPI_p,
    tEplObdSize     EntrySize_p,
    unsigned int    uiSubindexCountPerIndex_p)
{
tEplKernel      Ret = kEplSuccessful;
unsigned int    uiVarEntries;

    for (; uiObjIndexStart_p <= uiObjIndexEnd_p;
        uiObjIndexStart_p++, uiOffsetPI_p += EntrySize_p * uiSubindexCountPerIndex_p)
    {
        uiVarEntries = uiSubindexCountPerIndex_p;
        Ret = EplApiProcessImageLinkObject(
						uiObjIndexStart_p,
						1,
                        uiOffsetPI_p,
                        fOutputPI_p,
                        EntrySize_p,
                        &uiVarEntries);
        if (((Ret == kEplSuccessful) && (uiVarEntries < uiSubindexCountPerIndex_p))
            || (Ret == kEplApiPISizeExceeded))
        {
            Ret = kEplSuccessful;
            break;
        }
        if (Ret != kEplSuccessful)
        {
            TRACE("EplApiProcessImageLinkObject returned: %xh for index %xh\n", Ret, uiObjIndexStart_p);
            goto Exit;
        }
    }

Exit:
    return Ret;
}

#endif
