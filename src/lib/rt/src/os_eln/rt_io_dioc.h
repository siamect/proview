#ifndef rt_io_dioc_h
#define rt_io_dioc_h

/* rt_io_dioc.h -- <short description>

   PROVIEW/R
   Copyright (C) 1996 by Comator Process AB.

   <Description>.  */

#include "pwr.h"
#ifndef pwr_eClass_Node
#include "pwr_class.h"
#endif
#ifndef pwr_cClass_RTP_DIOC
#include "pwr_baseclasses.h"
#endif

/* Message types  */

#define	DIOC_MT_CMD		    1
#define	DIOC_MT_OUT		    2
#define	DIOC_MT_CMD_OUT		    3
#define	DIOC_MT_OUT_IN_ON_RDY	    4
#define	DIOC_MT_CMD_OUT_IN_ON_RDY   5
#define DIOC_MT_CBO_CMD		    6
#define	DIOC_MT_CBO_OUT		    7
#define	DIOC_MT_CBO_CMD_OUT	    8
#define	DIOC_MT_TEST		    0x11
#define	DIOC_MT_INPUT		    0x12
#define	DIOC_MT_CMD_IN		    0x13
#define	DIOC_MT_CMD_TEST	    0x14
#define	DIOC_MT_INTR_QUERY	    0x1E
#define	DIOC_MT_ASG_SCAN_LIST	    9
#define	DIOC_MT_XQT_OUT_SCAN	    0x0A
#define	DIOC_MT_CBO_OUT_SCAN	    0x0B
#define	DIOC_MT_READ_SCAN_LIST	    0x15
#define	DIOC_MT_DEL_SCAN_LIST	    0x16
#define	DIOC_MT_XQT_IN_SCAN	    0x17
#define	DIOC_MT_START_CONT_SCAN	    0x18
#define	DIOC_MT_STOP_CONT_SCAN	    0x19
#define	DIOC_MT_ECHO_CHECK	    0x0C
#define	DIOC_MT_SITE_STATUS_CHECK   0x1A
#define	DIOC_MT_CBO_OPERATE	    0x1B
#define	DIOC_MT_REXMIT		    0x1C
#define	DIOC_MT_RESET		    0x1D
#define	DIOC_MT_RMT_RESPONSE	    0x0D
#define	DIOC_MT_RMT_BLK_RESPONSE    0x0E
#define	DIOC_MT_ACK		    0x10
#define	DIOC_MT_NACK		    0x1F
#define	DIOC_MT_ILLCODE_0	    0
#define	DIOC_MT_ILLCODE_F	    0x0F

/* Scan codes  */

#define	DIOC_SC_SINGLE_DO	    2
#define	DIOC_SC_MULTIPLE_DO	    3
#define	DIOC_SC_SINGLE_DI	    4
#define	DIOC_SC_MULTIPLE_DI	    5
#define	DIOC_SC_UNIVERSAL_AI	    6
#define	DIOC_SC_CONTROLLER_AI	    7

/* This structure describes the small dioc header */

typedef struct {
  unsigned int	FuncCode :5;
  unsigned int	DevAddr	 :3;
  unsigned int	SubFunc	 :4;
  unsigned int	SiteAddr :4;
  pwr_tInt16	Crc16;
} DIOC_4B_HEADER;

/* This structure describes the large dioc header */

typedef struct {
  unsigned int	FuncCode  :5;
  unsigned int	DevAddr   :3;
  unsigned int	SubFunc   :4;
  unsigned int	SiteAddr  :4;
  unsigned int	ScanType  :3;
  unsigned int		  :1;
  unsigned int	UniAiAddr :4;
  pwr_tUInt8	WordCount;
  pwr_tInt16	Crc16;
} DIOC_6B_HEADER;


/* This structure describes a scan list entry for an AI signal */

typedef struct {
  unsigned int	GainSel  :4;
  unsigned int	Channel  :3;
  unsigned int	CardAddr :4;
  unsigned int	         :1;
  unsigned int	OddChan  :1;
  unsigned int	Diff     :1;
  unsigned int	OTD      :1;
  unsigned int	         :1;
} DIOC_AI_LISTENT;

/* This structure describes a scan list entry for a DI/DO or AO signal */

typedef struct {
  unsigned int	CardAddr    :4;
  unsigned int	CardDep     :4;
  unsigned int	CtrlAccMode :2;
  unsigned int	SomeBits    :2;
  unsigned int		    :1;
  unsigned int	CtrlAddr    :3;
} DIOC_DIDOAO_LISTENT;

/* This union descibes a scan list entry that can be a DI/DO/AO, AI or
   check sum entry */

typedef union  {
  DIOC_AI_LISTENT	AIEntry;
  DIOC_DIDOAO_LISTENT	IOEntry;
  pwr_tUInt16		Crc16;
} DIOC_SCAN_LIST_ENTRY;

 
/* This union is used to describe the short DIOC header */

typedef union {
  DIOC_4B_HEADER	HdrStr;
  char			CharBuf[4];
} DIOC_4B_UNION;


/* These following structures is used to describe the DIOC hierarchy */


/* Analog input, polynominal conversion */

typedef struct {
  pwr_sClass_Ai_7436  *CardPtr;	
  pwr_sClass_ChanAi   *ChanPtr;
  pwr_sClass_Ai	      *SigPtr;
} DIOC_LIST_AI_ENT;

/* Analog input, table conversion */

typedef struct {
  pwr_sClass_Ai_7436  *CardPtr;	
  pwr_sClass_ChanAit  *ChanPtr;
  pwr_sClass_Ai	      *SigPtr;
} DIOC_LIST_AIT_ENT;

/* Analog output, single channel card */

typedef struct {
  pwr_sClass_Ao_7455_20 *CardPtr;	
  pwr_sClass_ChanAo     *ChanPtr;
  pwr_sClass_Ao		*SigPtr;
} DIOC_LIST_AO1_ENT;

/* Analog output, four channel card */

typedef struct {
  pwr_sClass_Ao_7455_30 *CardPtr;	
  pwr_sClass_ChanAo     *ChanPtr;
  pwr_sClass_Ao		*SigPtr;
} DIOC_LIST_AO4_ENT;

/* Programmable delay card, single channel card */

typedef struct {
  pwr_sClass_Pd_7435_26 *CardPtr;	
  pwr_sClass_ChanAo     *ChanPtr;
  pwr_sClass_Ao		*SigPtr;
} DIOC_LIST_PD_ENT;


/* Digital input */

typedef struct {
  pwr_sClass_Di_7437_37 *CardPtr;	
  pwr_sClass_ChanDi     *(*ChanPtr[16]);
  pwr_sClass_Di		*(*SigPtr[16]);
} DIOC_LIST_DI_ENT;


/* Digital output */

typedef struct {
  pwr_sClass_Do_7437_83	*CardPtr;	
  pwr_sClass_ChanDo     *(*ChanPtr[16]);
  pwr_sClass_Do		*(*SigPtr[16]);
} DIOC_LIST_DO_ENT;

/* Counter card, single channel card , polynominal conversion */

typedef struct {
  pwr_sClass_Co_7435_33 *CardPtr;	
  pwr_sClass_ChanCo     *ChanPtr;
  pwr_sClass_Co		*SigPtr;
} DIOC_LIST_CO1_ENT;

#if 0
/* Counter card, single channel card , table conversion algorithm */

typedef struct {
  pwr_sClass_Co_7435_33 *CardPtr;	
  pwr_sClass_ChanCot    *ChanPtr;
  pwr_sClass_Co		*SigPtr;
} DIOC_LIST_CO1T_ENT;
#endif

typedef struct {
  pwr_sClass_Co_7437_33 *CardPtr;	
  pwr_sClass_ChanCo     *ChanPtr;
  pwr_sClass_Co		*SigPtr;
} DIOC_LIST_CO4_ENT;

#if 0
typedef struct {
  pwr_sClass_Co_7437_33 *CardPtr;	
  pwr_sClass_ChanCot    *ChanPtr;
  pwr_sClass_Co		*SigPtr;
} DIOC_LIST_CO4T_ENT;
#endif


typedef union {
  DIOC_LIST_AI_ENT	Ai;
  DIOC_LIST_AIT_ENT	Ait;
  DIOC_LIST_AO1_ENT	Ao1;
  DIOC_LIST_AO4_ENT	Ao4;
  DIOC_LIST_DI_ENT	Di;
  DIOC_LIST_DO_ENT	Do;
  DIOC_LIST_CO1_ENT	Co1;
#if 0
  DIOC_LIST_CO1T_ENT	Co1t;
  DIOC_LIST_CO4T_ENT	Co4t;
#endif
  DIOC_LIST_CO4_ENT	Co4;
  DIOC_LIST_PD_ENT	Pd;
} DIOC_LIST_UNION;

/* This is an enumeration type  describing what kind of an entry this is */

typedef enum {
  _eDiocLEtype_illegal,
  _eDiocLEtype_ai,
  _eDiocLEtype_ait,
  _eDiocLEtype_ao1,
  _eDiocLEtype_ao4,
  _eDiocLEtype_di,
  _eDiocLEtype_do,
  _eDiocLEtype_co1,
  _eDiocLEtype_co1t,
  _eDiocLEtype_co4,
  _eDiocLEtype_co4t,
  _eDiocLEtype_pd
} DIOC_LENTTYP_ENUM;

/* This entry is used to describe a list entry */ 

typedef struct {
  DIOC_LENTTYP_ENUM	EntryType;
  DIOC_LIST_UNION	Data;
} DIOC_DESC_LIST_ENTRY;

/* This structure describes a buffer containing either a scan list or some 
   data that corresponds to such a list. This buffer should be dynamically
   allocated as large as needed. Therefore the slightly idiosynchratic 
   array declaration  */

typedef struct {
  DIOC_6B_HEADER	Header;
  DIOC_SCAN_LIST_ENTRY	Entries[1];
} DIOC_SCAN_BUFFER;


/* This enumeration is used to describe what kind of a list that this
   header heads (gawkkk!) */

typedef enum {
  _eDiocIOLtype_illegal,
  _eDiocIOLtype_di,
  _eDiocIOLtype_ai,
  _eDiocIOLtype_do,
  _eDiocIOLtype_ao
} DIOC_LISTTYP_ENUM;

/* This header is used to collate all the information needed to make a 
   DIOC scan list work in a Proview/R environment */

typedef struct s_DiocScLiDe {
  DIOC_LISTTYP_ENUM	ListType;
  struct s_DiocScLiDe	*Next;
  int			ScanListNo;
  int			NoOfEntries;
  DIOC_SCAN_BUFFER	*ListBuffer;
  DIOC_SCAN_BUFFER	*DataBuffer;
  DIOC_LIST_UNION	*DescList;
} DIOC_SCAN_LIST_DESCR;

extern pwr_tInt32 io_dioc_init();
extern pwr_tInt32 io_dioc_start();
extern pwr_tBoolean io_dioc_ancestor_is_dioc(pwr_tObjid Object);
#endif
