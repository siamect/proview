/* rt_io_convert.c -- <short description>

   PROVIEW/R
   Copyright (C) 1996, 2000 by Mandator AB.

   IO conversion PROVIEW/R.  */

#if defined(OS_ELN)
# include string
# include stdlib
# include stdio
#else
# include <string.h>
# include <stdlib.h>
# include <stdio.h>
#endif

#include "pwr.h"
#include "pwr_class.h"
#include "pwr_baseclasses.h"
#include "rt_io_msg.h"
#include "co_cdh.h"
#include "rt_errh.h"


static pwr_tStatus io_ConvertAI (
  void		*ChanObjP,
  pwr_tClassId   ChanType,
  void          *ConfigObjP,
  pwr_tUInt16   *RawValueP,
  pwr_tFloat32  *SigValueP,
  pwr_tFloat32  *ActValueP
);

static pwr_tStatus io_ConvertAO (
  void                    *ChanObjP,
  pwr_sClass_Ao_7455_30   *ConfigObjP,
  pwr_tUInt16             *RawValueP,
  pwr_tFloat32            *SigValueP,
  pwr_tFloat32            *ActValueP
);

static pwr_tStatus io_ConvSensor (
  pwr_sClass_ChanAi   *ChanObjP,
  pwr_tClassId         ChanType,
  pwr_tFloat32        *SigValueP,
  pwr_tFloat32        *ActValueP
);

static pwr_tStatus io_ConvSensorType (
  void          *ChanObjP,
  pwr_tUInt16   SensorTypeCode,
  pwr_tFloat32	*SigValueP
);

static void io_ConvSensorPol (
  pwr_sClass_ChanAi *ChanObjP,
  pwr_tFloat32      *SigValueP,
  pwr_tFloat32      *ActValueP
);

static void io_ConvSensorTab (
  pwr_sClass_ChanAit  *ChanObjP,
  pwr_tFloat32        *SigValueP,
  pwr_tFloat32        *ActValueP
);

static pwr_tStatus io_ConvDevAI (
  pwr_sClass_Ai_7436  *ConfigObjP,
  pwr_tUInt16         *RawValueP,
  pwr_tFloat32        *SigValueP
);

static void io_ConvDevPolAI (
  pwr_sClass_Ai_7436  *ConfigObjP,
  pwr_tUInt16         *RawValueP,
  pwr_tFloat32        *SigValueP
);

static void io_ConvSensorType_0 (
  pwr_sClass_ChanAi   *ChanObjP,
  pwr_tFloat32        *SigValueP
);

static void io_ConvSensorType_1 (
  pwr_sClass_ChanAi   *ChanObjP,
  pwr_tFloat32        *SigValueP
);

static void io_ConvSensorType_2 (
  pwr_sClass_ChanAi   *ChanObjP,
  pwr_tFloat32        *SigValueP
);

static void io_ConvSensorType_3 (
  pwr_sClass_ChanAi   *ChanObjP,
  pwr_tFloat32        *SigValueP
);

static pwr_tStatus io_ConvOutput(
  pwr_sClass_ChanAo   *ChanObjP,
  pwr_tFloat32        *SigValueP,
  pwr_tFloat32        *ActValueP
);

static void io_ConvOutputPol (
  pwr_sClass_ChanAo   *ChanObjP,
  pwr_tFloat32        *SigValueP,
  pwr_tFloat32        *ActValueP
);

static void io_ConvDevPolAO (
  pwr_sClass_Ao_7455_30	*ConfigObjP,
  pwr_tUInt16           *RawValueP,
  pwr_tFloat32          *SigValueP
);



/************************************************************************
*
* Name:		io_Convert
*
* Type:		pwr_tInt32	return ParamStatus
*
* TYPE		PARAMETER	IOGF	DESCRIPTION
* void          *ChanObjP       I	Pointer to channel object
* void		*ConfigObjP     I	Pointer to card object
* pwr_tUInt16	*RawValueP      I	Pointer to raw value
* pwr_tFloat32	*SigValueP      I	Pointer to signal value
* pwr_tFloat32	*ActValueP	I	Pointer to actual value
* pwr_tUInt16	ChanTypeCode    I	Channel object type.
*				        pwr_cClass_ChanAi, pwr_cClass_ChanAit 
*					or pwr_cClass_ChanAo 
*				        (see pinc$:ot_def.h.)
* 
*
* Description:	Convert AI-signals from raw value to actual value 
*		and AO-signals from actual value to raw value.
*
*		Object AI:
*		- Convert raw value to signal value and store it in RTDB.
*		- Convert signal value to actual value and store it in RTDB.
*
*		Object AO:
*		- Convert actual value to signal value and store it in RTDB.
*		- Convert signal value to raw value and store it in RTDB.
*************************************************************************/

pwr_tStatus io_Convert (
  void		*ChanObjP,
  void		*ConfigObjP,
  pwr_tUInt16	*RawValueP,
  pwr_tFloat32	*SigValueP,
  pwr_tFloat32	*ActValueP,
  pwr_tClassId	ChanTypeCode
)
{
  pwr_tStatus	sts = IO__SUCCCONV;

  switch (ChanTypeCode) {
  case pwr_cClass_ChanAi:
    sts = io_ConvertAI (ChanObjP, ChanTypeCode, ConfigObjP, 
      RawValueP, SigValueP, ActValueP);
    break;

  case pwr_cClass_ChanAit:
    sts = io_ConvertAI (ChanObjP, ChanTypeCode, ConfigObjP, 
      RawValueP, SigValueP, ActValueP);
    break;

  case pwr_cClass_ChanAo:
    sts = io_ConvertAO (ChanObjP, ConfigObjP, RawValueP, 
       SigValueP, ActValueP);
    break;

  default:
    return (IO__WROOBJTYPE);
    break;
  }

  return (sts);
}

/************************************************************************
*
* Namn          :   io_ConvertAI
*
* Typ           : pwr_tInt32	    returnerar ParamStatus
*
* TYP		   PARAMETER        IUGF    BESKRIVNING
* void             *ChanObjP        I	    Pointer to channel object
* pwr_tUInt16	   ChanType         I	    Channel object type.
*				            pwr_cClass_ChanAi, 
*					    pwr_cClass_ChanAit or 
*					    pwr_cClass_ChanAo 
*				            (see pinc$:ot_def.h.)
* void		   *ConfigObjP      I	    Pointer to card object
* pwr_tUInt16	   *RawValueP       I	    Pointer to raw value
* pwr_tFloat32	   *SigValueP       I	    Pointer to signal value
* pwr_tFloat32	   *ActValueP	    I	    Pointer to actual value
*
* Upphov        : Ingemar Ljungdahl		 Hämtad från COMTEST
*						 (Revision, se filhuvud)
*
* Beskrivning   : Omvandlar AI från råvärde (Rtdb) till signalvärde och 
*                 signalvärde till ärvärde och lägger signalvärde och 
*		  ärvärde i Rtdb.
*************************************************************************/

static pwr_tStatus io_ConvertAI (
  void		*ChanObjP,
  pwr_tClassId	ChanType,
  void		*ConfigObjP,
  pwr_tUInt16	*RawValueP,
  pwr_tFloat32	*SigValueP,
  pwr_tFloat32	*ActValueP
)
{
  pwr_tStatus	sts = IO__SUCCCONV;

  /*  Rawvalue -> Sigvalue  */

  sts = io_ConvDevAI(ConfigObjP, RawValueP, SigValueP);

  /*  Sigvalue -> Actualvalue  */

  sts = io_ConvSensor(ChanObjP, ChanType, SigValueP, ActValueP);

  return (sts);
}

/************************************************************************
*
* Namn          : io_ConvDevAI
*
* Typ           : int		returnerar ParamStatus
*
* TYP		   PARAMETER       IUGF    BESKRIVNING
* int              ChanIndex       I	   Anger kanal på kortet
*					   (0 - (antal kanaler-1)).  
* Config_AI_Object *ConfigObjP       G	   Pekare till config objekt
* uns int	   *RawValueP        G	   Pekare till råvärdet
* float		   *SigValueP        G     Pekare till signalvärdet
*
* Upphov        : Ingemar Ljungdahl		(Revision, se filhuvud)
*
* Beskrivning   : Kortpolynomtyp bestäms och omvandling sker enligt beskrivning
*                 för kortet
*************************************************************************/

static pwr_tStatus io_ConvDevAI (
  pwr_sClass_Ai_7436  *ConfigObjP,
  pwr_tUInt16	      *RawValueP,
  pwr_tFloat32	      *SigValueP
)
{
  pwr_tStatus	sts = IO__SUCCCONV;

  switch (ConfigObjP->DevPolyType) {
  case 0:	  /* No Conversion */
    break;

  case 1:	  /* Conv normal device polynomial */
    io_ConvDevPolAI (ConfigObjP, RawValueP, SigValueP);
    break;

  default:	  /* Device poly type out of range */
    errh_CErrLog(IO__INFOCONVDEV, errh_ErrArgL(ConfigObjP->DevPolyType), NULL);
    return (IO__CONVAI);
    break;
  }

  return (sts);

}


/************************************************************************
*
* Namn          :   io_ConvDevPolAI
*
* Typ           : void
*
* TYP              PARAMETER       IUGF    BESKRIVNING
* int              ChanIndex       I	   Anger kanal på kortet
*					   (0 - (antal kanaler-1)).  
* Config_AI_Object *ConfigObjP       G	   Pekare till config objekt
* uns int	   *RawValueP        G	   Pekare till råvärdet
* float		   *SigValueP        G     Pekare till signalvärdet
*
* Upphov        : Ingemar Ljungdahl         Hämtad från COMTEST
*					    (Revision, se filhuvud)
*
* Beskrivning   : Omvandlar värdet genom ett definierat polynom för
*                 aktuellt kort
*************************************************************************/

static void io_ConvDevPolAI (
  pwr_sClass_Ai_7436  *ConfigObjP,
  pwr_tUInt16	      *RawValueP,
  pwr_tFloat32	      *SigValueP)
{
  pwr_tUInt16	i; 
  pwr_tUInt16	VectLen; 
  pwr_tFloat32	*PolCoeffP;
  pwr_tFloat64	Result;

  VectLen = 2;	/* Max polynomdegree = 2 */
  PolCoeffP = &(ConfigObjP->DevPolyCoef1);
  Result = 0;

  for (i = VectLen; i > 0; i--) {
    Result = ((pwr_tInt16 )*RawValueP * Result) + *PolCoeffP;
    PolCoeffP--;
  }

  *SigValueP = Result;

}

/************************************************************************
*
* Namn          :   io_ConvSensor
*
* Typ           : int		returnerar ParamStatus
*
* TYP		    PARAMETER       IUGF    BESKRIVNING
* int               ChanIndex       I	    Anger kanal på kortet
*					    (0 - (antal kanaler-1)).  
* Config_AI_Object  *ConfigObjP	      G     Pekare till config objekt
* void	            *ConvTabObjP      G	    Pekare till omvandlingstabell objekt
*					    vid tabellslagning  
* float		    *SigValueP        G     Pekare till signalvärdet
* float		    *ActValueP        G     Pekare till signalvärdet
*
* Upphov        : Ingemar Ljungdahl	    Hämtad från COMTEST
*					    (Revision, se filhuvud)
*
* Beskrivning   :Omvandlingskod bestäms och omvandling sker enligt beskrivning
*                för givaren
*************************************************************************/

static	pwr_tStatus io_ConvSensor (
  pwr_sClass_ChanAi *ChanObjP,
  pwr_tClassId	    ChanType,
  pwr_tFloat32	    *SigValueP,
  pwr_tFloat32	    *ActValueP
) {
  pwr_tStatus	    sts = IO__SUCCCONV;

  sts = io_ConvSensorType(ChanObjP, ChanObjP->SensorTypeCode, SigValueP);

  switch (ChanType) {
  case pwr_cClass_ChanAi:    /* Conv sensor pol */
    io_ConvSensorPol(ChanObjP, SigValueP, ActValueP);
    break;

  case pwr_cClass_ChanAit:    /* Conv table */
    io_ConvSensorTab((pwr_sClass_ChanAit *)ChanObjP, SigValueP, ActValueP);	
    break;

  default:
    errh_CErrLog(IO__INFOCONVSENS,errh_ErrArgL(ChanType), NULL);
    return (IO__CONVAI);
    break;
  }

  return (sts);

} /* END io_ConvSensor */



/************************************************************************
*
* Namn          :   io_ConvSensorType
*
* Typ           :   int		returnerar ParamStatus
*
* TYP           PARAMETER       IUGF    BESKRIVNING
* int           ChanIndex       I	Anger kanal på kortet
*					(0 - (antal kanaler-1)).  
* uns int	SensorTypeCode  I
* void	        *ConfigObjP	  G     Pekare till config objekt
* float		*SigValueP        G     Pekare till signalvärdet
*
* Upphov        : Ingemar Ljungdahl	    Hämtad från COMTEST
*					    (Revision, se filhuvud)
*
* Beskrivning   : Anrop till Hårdkodade omvandlingsfunktioner för speciella
*                 givare, t.ex. termoelement.
*
*************************************************************************/

static pwr_tStatus io_ConvSensorType (
  void		*ChanObjP,
  pwr_tUInt16	SensorTypeCode,
  pwr_tFloat32	*SigValueP
) {
  pwr_tStatus	sts = IO__SUCCCONV;

  switch (SensorTypeCode) {
  case 0:	/* AI normal */
    io_ConvSensorType_0 (ChanObjP, SigValueP);
    break;

  case 1:	/* TK-low */
    io_ConvSensorType_1 (ChanObjP, SigValueP);
    break;

  case 2:	/* TK-high */
    io_ConvSensorType_2 (ChanObjP, SigValueP);
    break;

  case 3:	/* TR-ref */
    io_ConvSensorType_3 (ChanObjP, SigValueP);
    break;

  default: 
    errh_CErrLog(IO__INFOSENSTYPE,errh_ErrArgL(SensorTypeCode), NULL);
    return (IO__CONVAI);
    break;
  }

  return (sts);

} /* END io_ConvSensorType */


/************************************************************************
*
* Namn          : io_ConvSensorType_0
*
* Typ           :
*
* Upphov        : Ingemar Ljungdahl	    Hämtad från io_ConvSensorType
*					    (Revision, se filhuvud)
*
* Beskrivning   : omvandling av givartype nummer 0, normal
*************************************************************************/

static	void io_ConvSensorType_0 (
  pwr_sClass_ChanAi *ChanObjP,
  pwr_tFloat32	    *SigValueP
) {
} /* END io_ConvSensorType_0 */

/************************************************************************
*
* Namn          : io_ConvSensorType_1
*
* Typ           :
*
* TYP           PARAMETER       IUGFR   BESKRIVNING
*
* Upphov        : Ingemar Ljungdahl	    Hämtad från io_ConvSensorType
*					    (Revision, se filhuvud)
*
* Beskrivning   : omvandling av givartype nummer 1, TK-low
*************************************************************************/

static	void io_ConvSensorType_1 (
  pwr_sClass_ChanAi *ChanObjP,
  pwr_tFloat32	    *SigValueP
) {
} /* END io_ConvSensorType_1 */


/************************************************************************
*
* Namn          : io_ConvSensorType_2
*
* Typ           :
*
* TYP           PARAMETER       IUGFR   BESKRIVNING
*
* Upphov        : Ingemar Ljungdahl	    Hämtad från io_ConvSensorType
*					    (Revision, se filhuvud)
*
* Beskrivning   : Omvandling av givartype nummer 2, TK-high
*************************************************************************/

static void io_ConvSensorType_2 (
  pwr_sClass_ChanAi *ChanObjP,
  pwr_tFloat32	    *SigValueP
) {
} /* END io_ConvSensorType_2  */


/************************************************************************
*
* Namn          : io_ConvSensorType_3
*
* Typ           :
*
* TYP           PARAMETER       IUGFR   BESKRIVNING
*
* Upphov        : Ingemar Ljungdahl	    Hämtad från io_ConvSensorType
*					    (Revision, se filhuvud)
*
* Beskrivning   : omvandling av givartype nummer 3, TR-ref
*************************************************************************/

static void io_ConvSensorType_3 (
  pwr_sClass_ChanAi   *ChanObjP,
  pwr_tFloat32	      *SigValueP
) {
} /* END io_ConvSensorType_3  */


/************************************************************************
*
* Namn          :   io_ConvSensorPol
*
* Typ           : void
*
* TYP              PARAMETER       IUGF    BESKRIVNING
* int              ChanIndex       I	   Anger kanal på kortet
*					   (0 - (antal kanaler-1)).  
* Config_AI_Object *ConfigObjP       G	   Pekare till config objekt
* float		   *SigValueP        G	   Pekare till signalvärdet
* float		   *ActValueP        G     Pekare till ärvärdet
*
* Upphov        : Ingemar Ljungdahl         Hämtad från COMTEST
*					    (Revision, se filhuvud)
*
* Beskrivning   : Omvandlar signalvärde till ärvärde genom ett definierat 
*		  polynom för aktuell givare
*
*************************************************************************/

static void io_ConvSensorPol (
  pwr_sClass_ChanAi *ChanObjP,
  pwr_tFloat32	    *SigValueP,
  pwr_tFloat32	    *ActValueP
) {
  pwr_tUInt16	i; 
  pwr_tUInt16	VectLen; 
  pwr_tFloat32	*PolCoeffP;
  pwr_tFloat64	Result;


  VectLen = 3;	/* Max polynomdegree = 2 */
  PolCoeffP = &(ChanObjP->SensorPolyCoef2);
  Result = 0;

  for (i = VectLen; i > 0; i--) {
    Result = (*SigValueP * Result) + *PolCoeffP;
    PolCoeffP--;
  }

  *ActValueP = Result;

} /* END io_ConvSensorPol */


/************************************************************************
*
* Namn          :   io_ConvSensorTab
*
* Typ           : void
*
* TYP               PARAMETER       IUGF    BESKRIVNING
* Conv_Table_Object *ConvTabObjP      G	    Pekare till config objekt
* float		    *SigValueP        G	    Pekare till signalvärdet
* float		    *ActValueP        G     Pekare till ärvärdet
*
* Upphov        : Ingemar Ljungdahl	    Hämtad från COMTEST
*					    (Revision, se filhuvud)
*
* Beskrivning   : Omvandlar signalvärde till ärvärde genom tabellslagning.
*		  Sökningen i tabellen sker genom binärsökning.
*
*************************************************************************/

static void io_ConvSensorTab (
  pwr_sClass_ChanAit  *ChanObjP,
  pwr_tFloat32	      *SigValueP,
  pwr_tFloat32	      *ActValueP
) {
  pwr_tFloat32	Slope;
  pwr_tFloat32  Intercept;
  pwr_tFloat32  TransTabValue;
  pwr_tUInt16	First = 0;
  pwr_tUInt16	Middle;
  pwr_tUInt16	Last;

  Last = ChanObjP->NoOfCoordinates;

  while ((Last - First) > 1) {
    Middle = (First+Last) / 2;
    TransTabValue = ChanObjP->InValue[Middle];
    if (*SigValueP < TransTabValue)
      Last = Middle;
    else
      First = Middle;
  }
  
  Intercept = ChanObjP->Intercept[First];
  Slope = ChanObjP->Slope[First];
  *ActValueP = Intercept + *SigValueP * Slope;
 
} /* END io_ConvSensorTab */


/************************************************************************
*
* Namn          :   io_ConvertAO
*
* Typ           : INT	returnerar ParamStatus
*
* TYP		   PARAMETER        IUGF    BESKRIVNING
* int              ChanIndex        I	   Anger kanal på kortet
*					   (0 - (antal kanaler-1)).  
* void             *ConfigObjP        G     Pekare till config objekt
* uns int	   *RawValueP         G     Pekare till råvärdet
* float		   *SigValueP         G     Pekare till signalvärdet
* float		   *ActValueP         G     Pekare till ärvärdet
*
* Upphov        : Ingemar Ljungdahl		 Hämtad från io_ConvertAO
*						 (Revision, se filhuvud)
*
* Beskrivning   : Omvandlar AO-Objekt från ärvärde (Rtdb) till signalvärde och 
*                 signalvärde till råvärde och lägger signalvärde och 
*		  råvärde i Rtdb.
*************************************************************************/
static pwr_tStatus io_ConvertAO (
  void			  *ChanObjP,
  pwr_sClass_Ao_7455_30	  *ConfigObjP,
  pwr_tUInt16		  *RawValueP,
  pwr_tFloat32		  *SigValueP,
  pwr_tFloat32		  *ActValueP
) {
  pwr_tStatus	sts = IO__SUCCCONV;

  /*  Actualvalue -> Sigvalue  */

  sts = io_ConvOutput (ChanObjP, SigValueP, ActValueP);

  /*  Sigvalue -> Rawvalue  */

  switch (ConfigObjP->DevPolyType) {
  case 0:    
    /* No Conversion */
    break;

  case 1:    
    /* Conv normal device polynomial */
    io_ConvDevPolAO(ConfigObjP, RawValueP, SigValueP);
    break;

  default:
    /* Device poly type out of range */
    errh_CErrLog(IO__INFOCONVDEV, errh_ErrArgL(ConfigObjP->DevPolyType), NULL);
    return (IO__CONVAO);
    break;
  }

  return (sts);
} /* END io_ConvertAO */


/* Omvandling sker enligt beskrivning för utsignalen.
   Övervakning av signalvärde (max och min).  */

static	pwr_tStatus io_ConvOutput (
  pwr_sClass_ChanAo   *ChanObjP,
  pwr_tFloat32	      *SigValueP,
  pwr_tFloat32	      *ActValueP
) {
  pwr_tInt32	sts = IO__SUCCCONV;

  /* Conv output pol */

  io_ConvOutputPol (ChanObjP, SigValueP, ActValueP);

  if (*SigValueP < ChanObjP->ChannelSigValRangeLow) 
    *SigValueP = ChanObjP->ChannelSigValRangeLow;	   

  if (*SigValueP > ChanObjP->ChannelSigValRangeHigh) 
    *SigValueP = ChanObjP->ChannelSigValRangeHigh;

  return (sts);

} /* END io_ConvOutput */


/* Omvandlar ärvärde till signalvärde genom ett definierat 
   polynom för aktuell utsignal.  */

static void
io_ConvOutputPol (
  pwr_sClass_ChanAo *ChanObjP,
  pwr_tFloat32	    *SigValueP,
  pwr_tFloat32	    *ActValueP
)
{
  pwr_tUInt16	    i; 
  pwr_tUInt16	    VectLen; 
  pwr_tFloat32	    *PolCoeffP;
  pwr_tFloat64	    Result = 0;


  VectLen = 2;				/* Max polynomdegree = 1 */
  PolCoeffP = &(ChanObjP->OutPolyCoef1);

  for (i = VectLen; i > 0; i--) {
    Result = (*ActValueP * Result) + *PolCoeffP;
    PolCoeffP--;
  }

  *SigValueP = Result;
}

/* Omvandlar signalvärde till råvärde värdet genom ett 
   definierat polynom för aktuellt kort.  */

static void
io_ConvDevPolAO (
  pwr_sClass_Ao_7455_30  *ConfigObjP,
  pwr_tUInt16		 *RawValueP,
  pwr_tFloat32		 *SigValueP
)
{
  pwr_tUInt16	i; 
  pwr_tUInt16	VectLen; 
  pwr_tFloat32	*PolCoeffP;
  pwr_tFloat64	Result;

  VectLen = 2;				/* Max polynomdegree = 1 */

  PolCoeffP = &(ConfigObjP->DevPolyCoef1);

  Result = 0;

  for (i = VectLen; i > 0; i--) {
    Result = (*SigValueP * Result) + *PolCoeffP;
    PolCoeffP--;
  }

  *RawValueP = Result;
}
