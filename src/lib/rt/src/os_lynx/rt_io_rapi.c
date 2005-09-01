/* 
 * Proview   $Id: rt_io_rapi.c,v 1.2 2005-09-01 14:57:57 claes Exp $
 * Copyright (C) 2005 SSAB Oxelösund AB.
 *
 * This program is free software; you can redistribute it and/or 
 * modify it under the terms of the GNU General Public License as 
 * published by the Free Software Foundation, either version 2 of 
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful 
 * but WITHOUT ANY WARRANTY; without even the implied warranty of 
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the 
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License 
 * along with the program, if not, write to the Free Software 
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */


/*------------------------------------------------------------------------------
//
// File: rt_io_rapi.c
//
// Description: RAPI Host interface routines
//
//
// Revision history
// ----------------
// Date   Name Text
// 970202 BHo  Ported from vxWorks demo source code.
//
// Last change:   970604 BHo
//------------------------------------------------------------------------------
*/

/*------------- D E F I N E S ------------------------------------------------*/

#define RAPI_LIB
#define RAPI_HOST

//#define DEBUG

/*------------- I N C L U D E S ----------------------------------------------*/

#ifdef WIN32
#include <windows.h>
#endif

#include "stdio.h"
#include "rt_io_rapi.h"

/*------------- D A T A T Y P E S --------------------------------------------*/
/*------------- V A R I A B L E S --------------------------------------------*/

char rapiDevName[][15][7] =	{
 { " " },
 { "BIT   ","IN    ","OUT   ","COS   ","COUNT ","PULSE ","PWM   ","DAC   ",
                        "EVCAP ","EVCOMP","      ","      ","      ","      ","      "},
 { "PORT  ","IN-8  ","IN-16 ","IN-32 ","OUT-8 ","OUT-16","OUT-32","      ",
                        "      ","      ","      ","      ","      ","      ","      "},
 { "ADC   ","8-BIT ","10-BIT","12-BIT","14-BIT","16-BIT","18-BIT","20-BIT",
                        "      ","      ","      ","      ","      ","      ","      "},
 { "DAC   ","8-BIT ","10-BIT","12-BIT","14-BIT","16-BIT","18-BIT","20-BIT",
                        "      ","      ","      ","      ","      ","      ","      "},
 { "SERIAL","TRM   ","REC   ","RAWREC","      ","      ","      ","      ",
                        "      ","      ","      ","      ","      ","      ","      "},
 { "CAN   ","REGS  ","FILTER","TRMBUF","RECBUF","      ","      ","      ",
                        "      ","      ","      ","      ","      ","      ","      "},
 { "COUNT ","TIM16 ","TIM32 ","CNT16 ","CNT32 ","      ","      ","      ",
                        "      ","      ","      ","      ","      ","      ","      "},
 { "LVAR  ","IN-8  ","IN-16 ","IN-32 ","OUT-8 ","OUT-16","OUT-32","      ",
                        "      ","      ","      ","      ","      ","      ","      "},
 { "GVAR  ","IN-8  ","IN-16 ","IN-32 ","OUT-8 ","OUT-16","OUT-32","      ",
                        "      ","      ","      ","      ","      ","      ","      "},
 { "MUXMOD","IN-8  ","IN-16 ","OUT-8 ","OUT-16","REG-8 ","REG-16","REG-32",
                        "STP-8 ","STP-16","STP-32","BOARD ","      ","      ","      "},
 { "MOTOR ","GCTRL ","GSTAT ","MCTRL ","MSTAT ","ACC   ","SPEED ","PARM  ",
                        "SPOS  ","APOS  ","IN_REG","OUTREG","      ","      ","      "}
};

char rapiDevSize[][15] =	{
 { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },     /*          */
 { 0, 1, 1, 1, 2, 2, 2, 2, 2, 2, 0, 0, 0, 0, 0 },     /* BIT      */
 { 0, 1, 2, 4, 1, 2, 4, 0, 0, 0, 0, 0, 0, 0, 0 },     /* PORT     */
 { 0, 1, 2, 2, 2, 2, 4, 4, 4, 4, 0, 0, 0, 0, 0 },     /* ADC      */
 { 0, 1, 2, 2, 2, 2, 4, 4, 4, 4, 0, 0, 0, 0, 0 },     /* DAC      */
 { 0, 1, 1, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },     /* SERIAL   */
 { 0,32,16,10,10, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },     /* CAN      */
 { 0, 2, 4, 2, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },     /* COUNT    */
 { 0, 1, 2, 4, 1, 2, 4, 0, 0, 0, 0, 0, 0, 0, 0 },     /* LVAR     */
 { 0, 1, 2, 4, 1, 2, 4, 0, 0, 0, 0, 0, 0, 0, 0 },     /* GVAR     */
 { 0, 1, 2, 1, 2, 1, 2, 4, 1, 2, 4, 0, 0, 0, 0 },     /* MUXMOD   */
 { 0, 1, 1, 2, 1, 2, 2, 1, 4, 4, 1, 1, 0, 0, 0 }      /* MOTOR    */
};

RAPI_MOD		rapiModule[NUM_RAPI];                     /* GLOBAL RAPI modul control blocks. One for each baseaddress */
int         cRapi = 0;                                /* Counter how many control blocks used */

RAPI_HEADER*   pRapiHeader;
RAPI_DEV*      pRapiDevtab;
RAPI_IMREG*    pRapiImReg;


/*------------- P R O T O T Y P E S ------------------------------------------*/
/*----------------------------------------------------------------------------*/


/*
      Routines private to this module.
*/

#ifdef WIN32
/*---------------------------------------------------------------------------
* Name : nanosleep
* 
*  In:
*     struct timespec* rgtp   Requested time delay
*
*  Out:
*     struct timespec* rmtp   Remaining time 
*
*  Returns:
*     Error flag              0 = OK, >< 0 = Fail
*
* Description	: simulates Posix nanosleep call
* 
*--------------------------------------------------------------------------*/

int nanosleep(struct timespec* rgtp, struct timespec* rmtp)
{
   int milliSeconds;

   milliSeconds = rgtp->tv_sec * 1000;
   milliSeconds+= rgtp->tv_nsec/10000;

   Sleep(milliSeconds);

   return 0;
}
#endif

/*---------------------------------------------------------------------------
* Name		: taskDelay
* 
*  In:
*     int delay      Delay in milliseconds( may be it's in ticks !?)    
*
*  Out:
*     ---
*
*  Returns:
*     ---
*
* Description	: simulates VXworks(?) taskDelay call
* 
---------------------------------------------------------------------------*/

void taskDelay(int delay)
{
   struct timespec nanoDelay;

   nanoDelay.tv_sec = delay / 1000;                  /* How many seconds to wait */
   nanoDelay.tv_nsec = (delay % 1000) * 1000000000;  /* How many nanoseconds to wait */
   nanosleep(&nanoDelay, NULL);                       /* Sleep a short while bewteen status checking */

}
#if 0
/***************************************************************************
* rapiInt - Interrupt service routine
*
*  int rapiId  Index for RAPI module control block
*
* RETURNS: N/A.
*/

static void rapiInt(int rapiId)
{
   RAPI_IMREG  *pImReg;
   pwr_tUInt16 mail;
   int         dev;

   pImReg = rapiModule[rapiId].pRapiImReg;

   if((mail=pImReg->hostMailbox) != 0)       /* Clear DPM interrupt */
   {   
      pImReg->hostMailbox = 0;               /* Clear mailbox */
   }

   dev = rapiDevIrqGet(rapiId);

#ifdef DEBUG
/*	logMsg("rapiInt: Id=%d, dev=%d\n", rapiId, dev, 3, 4, 5, 6); */
#endif	

}
#endif
/****************************************************************************
* get_devsize - ???
*
*	pwr_tUInt8		   Device ?
*
* RETURNS:
* Device size
*
* Moved from"rapimon" to here
*/

int get_devsize(int dev)
{
   return (int)rapiDevSize[pRapiDevtab[dev].clas][pRapiDevtab[dev].type&0x0F];
}

/****************************************************************************
* wordSwapChk - Swap word bytes if host and device have differnt byte ordering
*
*	pwr_tUInt16 w		   16-bit word to swap
*
* RETURNS:
* Input (swapped) word. 
*/

static __inline pwr_tUInt16 wordSwapChk(pwr_tUInt16 w)
{

#if HOST_WORD != RAPI_WORD          /* If cpu and device byte order is different.. */

   pwr_tUInt8 buf1[2], buf2[2];     /* ..we'll have to do the swapping */

   *(pwr_tUInt16*) buf1 = w;
   buf2[0]=buf1[1];
   buf2[1]=buf1[0];
   return(*(pwr_tUInt16*) buf2);

#else                               /* If cpu and device byte order is the same.. */
   return(w);                       /* ..we don't have to do anything */
#endif
}

/****************************************************************************
* rapiCrcCalc - Calculate CRC checksum
*
* DESCRIPTION
* Calculation routine for industry standary CRC code of the first 15 bytes of
* the RAPI header.
*
*	pwr_tUInt8 *pBase		Pointer to start of RAPI header
*
* RETURNS:
* The lower 8-bits of the FSC described in CCITT T.30, section 5.3.7
*/
static pwr_tUInt8 rapiCrcCalc(pwr_tUInt8 *pBase)
{
   pwr_tUInt8 i;
   pwr_tUInt8 b;

   unsigned long crc;

   crc=0xffff;                               /* Default CRC value     */
   for(i=0; i<15; i++)                      /* CRC of first 15 bytes */
   {
      for(b=0x80; b; b>>=1)                 /* Process all 8 bits    */
      {
         if(b & pBase[i])
            crc^=0x8000;

         crc<<=1;

         if(crc & 0xffff0000L)
            crc = (crc & 0xffff) ^ 0x1021;
      }
   }

   crc = ~crc & 0xff;

   return((pwr_tUInt8)crc);
}

/*
      Public routines
*/

/****************************************************************************
* rapiInit - Initialize RAPI interface
*
*	pwr_tUInt8 *pRapiDPMBase  Pointer to start of RAPI DPM
*	int   irq				Interrupt vector
*
*
* RETURNS:
* Id of rapi structure on succes, or RAPI_ERR.
*/

int rapiInit(pwr_tUInt8* pRapiDPMBase, int irq)
{
#if 0
   RAPI_HEADER*   pRapiHeader;
   RAPI_DEV*      pRapiDevtab;
   RAPI_IMREG*    pRapiImReg;
#endif
   if(cRapi >= NUM_RAPI - 1)	                          /* No free rapi module block's */
   {

#ifdef DEBUG
      printf("rapiInit: no free rapi module block\n");
#endif
      return(RAPI_ERR);
   }

   pRapiHeader = (RAPI_HEADER *) pRapiDPMBase;          /* Setup RAPI header pointer */

#ifdef DEBUG
   printf("rapiInit: pRapiDPMBase = 0x%x\n", (unsigned int) pRapiDPMBase);
   printf("Id 0 : %d\n", pRapiHeader->rapiId[0]);     /* Printout of RAPI ID */
   printf("Id 0 : %d\n", pRapiHeader->rapiId[1]);
   printf("Id 0 : %d\n", pRapiHeader->rapiId[2]);
   printf("Id 0 : %d\n", pRapiHeader->rapiId[3]);
#endif

   if((pRapiHeader->rapiId[0] != 'R') ||   	         /* Test for valid RAPI ID structure */
      (pRapiHeader->rapiId[1] != 'A') ||
      (pRapiHeader->rapiId[2] != 'P') ||
      (pRapiHeader->rapiId[3] != 'I') )
   {

#ifdef DEBUG
      printf("rapiInit: ""RAPI"" string missing in header\n");
#endif
      return(RAPI_ERR);
   }

   if(pRapiHeader->crc != rapiCrcCalc(pRapiDPMBase))  /* Check CRC */
   {

#ifdef DEBUG
      printf("rapiInit: crc error\n");
#endif
      return(RAPI_ERR);
   }

#ifdef DEBUG
   printf("RAPI rev %d\n", pRapiHeader->rapiRev);
   printf("Module ID %d, Nr %d, Rev %d\n", pRapiHeader->modId,
           pRapiHeader->modNr, pRapiHeader->modRev);
#endif

   pRapiDevtab = (RAPI_DEV *)&pRapiDPMBase[wordSwapChk(pRapiHeader->devtab)];   /* Get pointer to device table */
   pRapiImReg = (RAPI_IMREG *)&pRapiDPMBase[wordSwapChk(pRapiHeader->vectab)];  /* Get pointer to interrupt vector table */

#ifdef DEBUG
   printf("rapiInit: pRapiDevtab = 0x%x\n", (unsigned int) pRapiDevtab);
   printf("rapiInit: pRapiImReg = 0x%x\n", (unsigned int) pRapiImReg);
#endif

   rapiModule[cRapi].pRapiDPM    = pRapiDPMBase;      /* Store pointer to Dual Ported Memory */
   rapiModule[cRapi].pRapiHeader = pRapiHeader;       /* Store pointer to RAPI_HEADER */
   rapiModule[cRapi].pRapiDevtab = pRapiDevtab;       /* Store pointer to RAPI_DEV */
   rapiModule[cRapi].pRapiImReg  = pRapiImReg;        /* Store pointer to RAPI_IMREG */

#if 0    /* Obsolete in Lynx/SSPA case !?!? */

   pRapiImReg->irqVec1 = irq;
   pRapiImReg->irqVec2 = irq;

   intConnect(INUM_TO_IVEC(irq), rapiInt, (unsigned int) cRapi);

#endif   /* Obsolete in Lynx/SSPA case !?!? */

   rapiModeSet(cRapi, RAPI_RESET);                    /* Initialize to RESET mode */

   cRapi++;                                           /* Increment initialized module count */

   return(cRapi - 1);                                 /* Return index for the module we just initialized */
}

/****************************************************************************
* rapiExit - 
*
*	int rapiId     RAPI id (index for controlblock)
*
* RETURNS:
* RAPI_OK on succes, or RAPI_ERR.
*/

int rapiExit(int rapiId)
{
   if(rapiId < 0 || rapiId >= cRapi)            /* Valid control module index ? */
      return(RAPI_ERR);

   return(rapiModeSet(rapiId, RAPI_RESET));     /* Stop all RAPI activity */

}

/***************************************************************************
* rapiModeGet - Get RAPI mode
*
*	int rapiId     RAPI id
*
* RETURNS:
* Present RAPI mode, or RAPI_ERR if invalid rapiId.
*/
int rapiModeGet(int rapiId)
{

   if(rapiId < 0 || rapiId >= cRapi)                        /* Valid control module index ? */
      return(RAPI_ERR);

   return(rapiModule[rapiId].pRapiHeader->statReg & 0x07);  /* Read status register and mask mode bits */
}

/***************************************************************************
* rapiModeSet - Set new RAPI mode
*
*	int rapiId     RAPI id
*	int mode			New RAPI mode
*
* RETURNS:
* RAPI_OK on succes, or RAPI_ERR.
*/
int rapiModeSet(int rapiId, int mode)
{
   int         i;
   RAPI_HEADER *pRapiHeader;

   if(rapiId < 0 || rapiId >= cRapi)                              /* Valid control module index ? */
      return(RAPI_ERR);

   if(mode < 0 || mode > 7)                                       /* Mode within valid range ? */
      return(RAPI_ERR);

   pRapiHeader = rapiModule[rapiId].pRapiHeader;                  /* Get pointer to RAPI_HEADER */

   pRapiHeader->ctrlReg = (pRapiHeader->ctrlReg & ~0x07) | mode;  /* Read current status reg, mask mode, insert new mode and write ctrl reg */

#if 0
   for(i=0; i < mSecToTick(1000); i++)                            /* Wait a while for new mode to show up */
#else
   for(i=0; i < 10000000; i++)                                        /* Wait a while for new mode to show up */
#endif
   {
      if((pRapiHeader->statReg & 0x07) == mode)                   /* New mode show up in status reg ? */
      {
#ifdef DEBUG
         printf("Waited %d laps for mode to be set\n", i);        /* Try to get a grip on the time frame involved here ;-) */
#endif
         return(RAPI_OK);                                         /* Allright, it's set ! */
      }

      taskDelay(1);      /* (is it one millisecond ??) */         /* Sleep a short while bewteen status checking (nanosleep ??) */

   }

   return(RAPI_ERR);                                              /* We didn't succeed in setting new mode */

}

/****************************************************************************
* rapiDevSearch
*
* int rapiId      RAPI id 
* int class
* int type
* int node
* int numb
*
* RETURNS:
* Device number on success, or RAPI_ERR.
*/

int rapiDevSearch(int rapiId, int class, int type, int node, int numb)
{
   int i;
   int cnt;
   RAPI_DEV *pRapiDevtab;

   if(rapiId < 0 || rapiId >= cRapi)                      /* Valid control module index ? */
      return(RAPI_ERR);

   pRapiDevtab = rapiModule[rapiId].pRapiDevtab;          /* Get pointer to RAPI_DEV(ice table) */

   for(i = cnt = 0; pRapiDevtab[i].clas != 0xFF; i++)    /* Until end of table */
   {
#if 0
#ifdef DEBUG
      printf("Searching for           Node %d Class %d Type %d Numb %d\n", node, class, type, numb);
      printf("Device table entry %4d Node %d Class %d Type %d\n",
              i,
              pRapiDevtab[i].node,
              pRapiDevtab[i].class,
              pRapiDevtab[i].type & 0x0F);
#endif
#endif
      if((pRapiDevtab[i].clas == class)        &&        /* Check class */
         ((pRapiDevtab[i].type & 0x0F) == type) &&        /* Check type  */
         ((node == -1)                          ||
         (pRapiDevtab[i].node == node)))  	              /* Check node  */
      {

         if(cnt++ == numb)                                /* Check numb(what's that !?) */
            return(i);  	                                /* Return device number */
      }
   }

   return(RAPI_ERR);                                      /* Device not found */
}

/****************************************************************************
* rapiIoRegGet - Get pointer to I/O register space
*
*  int rapiId    RAPI id  
*  int dev
*
* RETURNS:
* Pointer to RAPI I/O register space, or NULL if rapiId invalid.
*/

RAPI_IOREG* rapiIoRegGet(int rapiId, int dev)
{
   RAPI_IOREG *pIoReg;

   if(rapiId < 0 || rapiId >= cRapi)                              /* Valid control module index ? */
      return(NULL);

   pIoReg = (RAPI_IOREG *)                                        /* Casting */
            &rapiModule[rapiId].pRapiDPM                          /* Address in Dual Ported Memory.. */
            [
            wordSwapChk(rapiModule[rapiId].pRapiDevtab[dev].addr) /* ..where this device resides in device table */
            ];

   return(pIoReg);
}

/****************************************************************************
* rapiDevParamSet - Set parameter in device
*
*  int rapiId       RAPI id
*  int dev          Device
*  int parm         Parameter number
*  int val          Parameter value
*
* RETURNS:
* RAPI_OK on success, or RAPI_ERR.
*
*/

int rapiDevParamSet(int rapiId, int dev, int parm, int val)
{
   RAPI_IOREG  *pIoReg;
   pwr_tUInt8  oldCtrl;
   int         i;


   if((pIoReg = rapiIoRegGet(rapiId, dev)) == NULL)            /* Get pointer to device I/O registers (Ctrl/Status) */
      return(RAPI_ERR);

   if (parm < 1 || parm > 7)
      return(RAPI_ERR); 	                                    /* Parm out of range  */

   oldCtrl = pIoReg->ctrl;       	                            /* Save old ctrl reg  */

   pIoReg->ctrl = (pIoReg->ctrl & ~0x07) | parm;              /* Set parameter mode */

   rapiDevIrqSet(rapiId, dev);  /* (What ??) */               /* Send device irq    */

#if 0
   for(i=0; i < mSecToTick(200); i++)                         /* Check for timeout  */
#else
   for(i=0; i < 10000; i++)                                   /* Check for timeout  */
#endif
   {
      if((pIoReg->stat & 0x07) == parm)                        /* New mode set ?? (i.e Ready to receive parameter value ?) */
      {
         pIoReg->dreg.wdata = wordSwapChk((pwr_tUInt16) val);  /* Store parameter   */

         pIoReg->ctrl = oldCtrl;                               /* Restore ctrl reg   */
         rapiDevIrqSet(rapiId, dev);                           /* Send device irq    */

         for (i=0; i<mSecToTick(200); i++)                     /* Check for timeout  */
         {
            if((pIoReg->stat & 0x07)==(oldCtrl & 0x07))        /* Old mode restored ? */
            {
#ifdef DEBUG
               printf("Waited %d laps for old ctrl to be set\n",i); /* Try to get a grip on the time frame involved here ;-) */
#endif
               return(RAPI_OK);
            }
            taskDelay(1);                                      /* Sleep a short while waiting for old mode to turn up */
         }
         return(RAPI_ERR);                                     /* Time out! Wasn't able to restore old mode */
      }
      taskDelay(1);
   }

   return(RAPI_ERR);                                           /* Time out! */

}

/****************************************************************************
* rapiDevParamGet - Read parameter in device
*
*  int rapiId     RAPI id          
*  int dev        Device           
*  int parm       Parameter number 
*  int *val       Will receive parameter value  
*
* RETURNS:
* RAPI_OK on success, or RAPI_ERR.
*/

int rapiDevParamGet(int rapiId, int dev, int parm, int *val)
{
   RAPI_IOREG *pIoReg;
   pwr_tUInt8 oldCtrl;
   int        i;

   if((pIoReg = rapiIoRegGet(rapiId, dev)) == NULL)       /* Get pointer to device I/O registers (Ctrl/Status) */
      return(RAPI_ERR);

   if((parm < 1) || (parm > 7))
      return(RAPI_ERR);		     	                          /* Parm out of range  */

   oldCtrl = pIoReg->ctrl;                                /* Save old ctrl reg  */

   pIoReg->ctrl = (pIoReg->ctrl & ~0x07) | parm;          /* Set parameter mode */

   rapiDevIrqSet(rapiId, dev);                            /* Send device irq    */

#if 0
   for(i=0; i<mSecToTick(200); i++)                       /* Check for timeout  */
#else
   for(i=0; i< 10000; i++)                                /* Check for timeout  */
#endif
   {
      if((pIoReg->stat & 0x07) == parm)                   /* New mode set ?? (i.e Ready to deliver parameter value ?) */
      {
         *val = wordSwapChk(pIoReg->dreg.wdata);          /* Read parameter     */

         pIoReg->ctrl = oldCtrl;                          /* Restore ctrl reg   */
         rapiDevIrqSet(rapiId, dev);                      /* Send device irq    */

#if 0
         for(i=0; i<mSecToTick(200); i++)           	    /* Check for timeout */ 
#else
         for(i=0; i< 10000; i++)           	              /* Check for timeout */ 
#endif
         {
            if((pIoReg->stat & 0x07)==(oldCtrl & 0x07))  /* Old mode restored ? */
            {
#ifdef DEBUG
               printf("Waited %d laps for old ctrl to be set\n",i); /* Try to get a grip on the time frame involved here ;-) */
#endif
               return(RAPI_OK);
            }
            taskDelay(1);
         }
         return(RAPI_ERR);                               /* Time out! Wasn't able to restore old mode */
      }
      taskDelay(1);
   }
   return(RAPI_ERR);                                     /* Time out! */
}

/****************************************************************************
* rapiDevAttrBitSet
*
* DESCRIPTION
* Sets the attribute bits specified by <attrBitMsk>
*
* Not:
* rapiModeSet(rapiId, RAPI_DEVINIT) must be called after the attribute
* bit are set to make the changes take effect.
*
* RETURNS:
* RAPI_OK on success, or RAPI_ERR.
*/
int rapiDevAttrBitSet(int rapiId, int dev, int attrBitMsk)
{
   pwr_tUInt8 value;

   if(rapiId < 0 || rapiId >= cRapi)
      return(RAPI_ERR);

   value = rapiModule[rapiId].pRapiDevtab[dev].type;
   value |= attrBitMsk & 0xF0;
   rapiModule[rapiId].pRapiDevtab[dev].type = value;

   return(RAPI_OK);	
}

/****************************************************************************
* rapiDevAttrBitClr
*
* DESCRIPTION
* Clears the attribute bits specified by <attrBitMsk>.
*
* Not:
* rapiModeSet(rapiId, RAPI_DEVINIT) must be called after the attribute
* bit are set to make the changes take effect.
*
*  int rapiId
*  int dev
*  int attrBitMsk
*
* RETURNS:
* RAPI_OK on success, or RAPI_ERR.
*/

int rapiDevAttrBitClr(int rapiId, int dev, int attrBitMsk)
{
   pwr_tUInt8 value;

   if(rapiId < 0 || rapiId >= cRapi)                        /* Valid control module index ? */
      return(RAPI_ERR);

   value = rapiModule[rapiId].pRapiDevtab[dev].type;
   attrBitMsk &= 0xF0;
   value &= ~attrBitMsk;
   rapiModule[rapiId].pRapiDevtab[dev].type = value;

   return(RAPI_OK);	
}

/****************************************************************************
* rapiDevCosChk - Check device COS bit
*
*  int rapiId
*  int dev
*
* RETURNS:
* 1 of COS detected, 0 if no COS or RAPI_ERR if invalid rapiId.
*/

int rapiDevCosChk(int rapiId, int dev)
{
   RAPI_IOREG *pIoReg;

   if((pIoReg = rapiIoRegGet(rapiId, dev)) == NULL)      /* Get pointer to device I/O registers (Ctrl/Status) */
      return(RAPI_ERR);

   if((pIoReg->ctrl ^ pIoReg->stat) & RAPI_DEV_COS)      /* Compare COS bits */
   {
      pIoReg->ctrl ^= RAPI_DEV_COS;                      /* Toggle COS bit   */
      return(1);                             	         /* Return input COS */
   }

   return(0);  		                       	            /* No input COS     */
}

/****************************************************************************
* rapiUpdateWait - Wait for RAPI device update
*
*  int rapiId
*  int dev
*
* RETURNS:
* RAPI_OK, or RAPI_ERR on timeout.
*/

int rapiUpdateWait(int rapiId, int dev)
{
   RAPI_IOREG *pIoReg;                                      /* Pointer to device registers (Ctrl/Status) */
   int i;

   if((pIoReg = rapiIoRegGet(rapiId, dev)) == NULL)         /* Get pointer to device registers (Ctrl/Status) */
      return(RAPI_ERR);

   if(pIoReg->stat & RAPI_DEV_UPDATE)
      pIoReg->ctrl &= ~RAPI_DEV_UPDATE;                     /* Toggle UPDATE bit */
   else
      pIoReg->ctrl |= RAPI_DEV_UPDATE;

   rapiDevIrqSet(rapiId, dev);               	              /* Send interrupt */

#if 0
   for(i=0; i<mSecToTick(200); i++)		                      /* Wait for UPDATE bit */
#else
   for(i=0; i< 10000; i++)		                              /* Wait for UPDATE bit */
#endif
   {
      if(!((pIoReg->ctrl ^ pIoReg->stat) & RAPI_DEV_UPDATE))
      {
#ifdef DEBUG
         printf("Waited %d laps for dev update\n",i);       /* Try to get a grip on the time frame involved here ;-) */
#endif
         return (RAPI_OK);
      }
      taskDelay(1);
   }

   return(RAPI_ERR);
}

/****************************************************************************
* rapiDevFifoGet - Get data from device FIFO
*
*  int  rapiId    RAPI id                      
*  int  dev       Device                       
*  void *pData    Will receive dat from FIFO
*  int  size      Number of bytes to read
*
* RETURNS:
* RAPI_OK on success, or RAPI_ERR.
*/

int rapiDevFifoGet(int rapiId, int dev, void *pData, int size)
{
   int            i;
   int            rp;
   int            wp;

   pwr_tUInt8           *pByteBuff;
   volatile pwr_tUInt8  *pDataReg;
   RAPI_DEV             *pDev;
   RAPI_IOREG           *pIoReg;


   if((pIoReg = rapiIoRegGet(rapiId, dev)) == NULL)      /* Get pointer to device I/O registers (Ctrl/Status) */
   {
#ifdef DEBUG
      printf("rapiDevFifoGet: rapiIoRegGet() failed\n");
#endif

      return(RAPI_ERR);
   }

   pDev = &rapiModule[rapiId].pRapiDevtab[dev]; 	      /* Get dev table entry */

   if (!(pDev->type & RAPI_FIFO))      	               /* FIFO mode ? */
   {
#ifdef DEBUG
      printf("rapiDevFifoGet: device not in fifo mode\n");
#endif
      return(RAPI_ERR);  			

   }

   rp = pIoReg->dreg.bbuf[0];                            /* Get FIFO read pointer  */
   wp = pIoReg->dreg.bbuf[1];                            /* Get FIFO write pointer */

   if (wp == rp)                                         /* FIFO empty ? */
   {

#ifdef DEBUG
      printf("rapiDevFifoGet: fifo empty\n");
#endif

      return(RAPI_ERR);			                  
   }

   pByteBuff = (pwr_tUInt8 *) pData;                     /* Convert to byte array */
   pDataReg = &pIoReg->dreg.bbuf[2+(size*rp)];           /* Calculate FIFO entry */

   for (i=0; i<size; i++)
      *pByteBuff++ = *pDataReg++;    	                  /* Copy data from FIFO */

   if (++rp >= pDev->attr)
      rp = 0;           		                           /* Calc next read pointer */

   pIoReg->dreg.bbuf[0] = rp;                            /* Restore read pointer */

   return(RAPI_OK);
}

/****************************************************************************
* rapiDevFifoPut - Write data to device FIFO
*
*  int rapiId     RAPI id                   
*  int dev        Device                    
*  void *buf      Data to write to FIFO
*  int size       Number of bytes to write
*
* RETURNS:
* RAPI_OK on success, or RAPI_ERR if device not in FIFO mode or FIFO full.
*/

int rapiDevFifoPut(int rapiId, int dev, void *buf, int size)
{
   int rp;                                               /* FIFO read pointer */
   int wp;                                               /* FIFO write pointer */
   int i;

   pwr_tUInt8          *pByteBuf;                        /* Pointer to buf(as a byte array) */
   volatile pwr_tUInt8 *pDataReg;                        /* FIFO entry */
   RAPI_DEV             *pDev;                           /* Pointer to dev table entry */
   RAPI_IOREG           *pIoReg;                         /* Pointer to device registers (Ctrl/Status) */


   if((pIoReg = rapiIoRegGet(rapiId, dev)) == NULL)      /* Get pointer to device registers (Ctrl/Status) */
   {

#ifdef DEBUG
      printf("rapiDevFifoPut: rapiIoRegGet() failed\n");
#endif

      return(RAPI_ERR);
   }

   pDev = &rapiModule[rapiId].pRapiDevtab[dev];          /* Get dev table entry   */


   if(!(pDev->type & RAPI_FIFO))                         /* FIFO mode ? */
   {
#ifdef DEBUG
      printf("rapiDevFifoPut: device not in fifo mode\n");
#endif
      return(RAPI_ERR);
   }

   rp = pIoReg->dreg.bbuf[0];             	            /* Get FIFO read pointer  */
   wp = pIoReg->dreg.bbuf[1];                            /* Get FIFO write pointer */

   pDataReg = &pIoReg->dreg.bbuf[2+(size*wp)];           /* Calculate FIFO entry */

   if(++wp >= pDev->attr)
      wp = 0;    			                                 /* Calc next read pointer */

   if(wp == rp)     			                              /* FIFO full ? */
   {
#ifdef DEBUG
      printf("rapiDevFifoPut: fifo full\n");
#endif
      return(RAPI_ERR);
   }

   pByteBuf = (pwr_tUInt8*) buf;                         /* Convert to byte array  */

   for (i=0; i<size; i++)
      *pDataReg++ = *pByteBuf++;   	                     /* Copy data to FIFO */

   pIoReg->dreg.bbuf[1] = wp;                            /* Restore write pointer  */

   return(RAPI_OK);
}

/****************************************************************************
* rapiDevCosGet - Get device from COS FIFO
*
* RETURNS:
* Device number, or RAPI_ERR if no device in FIFO buffer.
*/

int rapiDevCosGet(int rapiId)
{
   int rp;
   int dev;
   RAPI_IMREG *pImReg;

   if(rapiId < 0 || rapiId >= cRapi)                           /* Valid control module index ? */
      return(RAPI_ERR);

   pImReg = rapiModule[rapiId].pRapiImReg;

   if((rp=pImReg->cosFifoReadPtr) != pImReg->cosFifoWritePtr)
   {
      dev = pImReg->cosFifoBuf[rp];                            /* Get COS device number */
      pImReg->cosFifoReadPtr = (rp+1) & 0x0F;	               /* Update read pointer   */
      return(dev);                                             /* Return device number  */
   }

   return(RAPI_ERR);                                           /* No devices in buffer  */
}

/****************************************************************************
* rapiDevIrqGet - Get device from IRQ FIFO
*
* RETURNS:
* Device number, or RAPI_ERR if no device in FIFO buffer.
*/

int rapiDevIrqGet(int rapiId)
{
   int rp;
   int dev;
   RAPI_IMREG *pImReg;

   if(rapiId < 0 || rapiId >= cRapi)                                    /* Valid control module index ? */
      return(RAPI_ERR);

   pImReg = rapiModule[rapiId].pRapiImReg;

   if((rp=pImReg->irqHostFifoReadPtr) != pImReg->irqHostFifoWritePtr)
   {                                                                    
      dev = pImReg->irqHostFifoBuf[rp];      	                        /* Get COS device number */
      pImReg->irqHostFifoReadPtr = (rp+1) & 0x0F;                       /* Update read pointer   */
      return(dev);                                                      /* Return device number  */
   }

   return(RAPI_ERR);                               	                  /* No devices in buffer  */
}

/****************************************************************************
* rapiDevIrqSet - Send a device IRQ to the IRQ FIFO
*
* RETURNS:
* RAPI_OK on succes, or RAPI_ERR if FIFO is full.
*/

int rapiDevIrqSet(int rapiId, int dev)
{
   int wp;
   RAPI_IMREG *pImReg;

   if(rapiId < 0 || rapiId >= cRapi)                        /* Valid control module index ? */
      return(RAPI_ERR);

   pImReg = rapiModule[rapiId].pRapiImReg;

#ifdef DEBUG
   printf("rapiDevIrqSet: pImReg=0x%x, &wp=%x, &rp=%x\n",
   (unsigned int) pImReg,
   (unsigned int)	&pImReg->irqModFifoWritePtr,
   (unsigned int) &pImReg->irqModFifoReadPtr);

   printf("rapiDevIrqSet: wp=%x, rp=%x\n",
   pImReg->irqModFifoWritePtr, pImReg->irqModFifoReadPtr);
#endif

   wp = pImReg->irqModFifoWritePtr;       	               /* Get Fifo write pointer  */
   pImReg->irqModFifoBuf[wp] = dev;       	               /* Put dev in FIFO         */
   wp = (wp+1) & 0x0F;                                      /* Increment write pointer */
   if (wp == pImReg->irqModFifoReadPtr)     		            /* Fifo Full ??     */
   {
#ifdef DEBUG
      printf("rapiDevIrqSet: Fifo full\n");
#endif
      return (RAPI_ERR);
   }

   pImReg->irqModFifoWritePtr = wp;       	               /* Update write pointer    */

   pImReg->modMailbox = 1;                	               /* Send IRQ to RAPI module */

#ifdef DEBUG
   printf("rapiDevIrqSet: &pImReg->modMailbox=%x\n",
   (unsigned int) &pImReg->modMailbox);
#endif

   return(RAPI_OK);
}

/****************************************************************************
* rapiDevSizeGet - Get device size
*
* RETURNS:
* Size of specified device or RAPI_ERR if invalid rapiId.
*/

int rapiDevSizeGet(int rapiId, int dev)
{
   RAPI_DEV *pDev;                                       /* Device table entry */


   if(rapiId < 0 || rapiId >= cRapi)                     /* Valid control module index ? */
      return(RAPI_ERR);

   pDev = &rapiModule[rapiId].pRapiDevtab[dev]; 	      /* Get dev table entry */

   return(rapiDevSize[pDev->clas][pDev->type & 0x0F]);
}


