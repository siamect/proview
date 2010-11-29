/*************************************************************
*                                                            *
* COPYRIGHT © 2007 Motion Control i Västerås AB, Sweden	     *
*                                                            *
* All rights including ownership and copyright to the 	     *
* software herein are held by Motion Control i Västerås AB.  *
* The software may be used and/ or copied only with the	     *
* written permission from Motion Control i Västerås AB or    *
* in accordance with the terms and conditions stipulated     *
* agreement/ contract under which the software has been      *
* supplied.                                                  *
*------------------------------------------------------------*
* Programenhet :   USBIO driver for Linux 2.6                *
* Filnamn :        libusbio.h                                *
* Tillhörande      libusbio.c                                *
* filer :                                                    *
*----------------------------------------------------------- *
* Beskrivning :    Detta är H-filen för supportbiblioteket   *
*                  till USB I/O enheten.                     *
*------------------------------------------------------------*
* Programhistoria :                                          *
* Ver, Datum,    Utförd av,                                  *
* 1.0  07-01-29  Marcus Tönnäng                              *
* 1.2  07-02-06  Johan Zetterlund portad till Linux.         *
*                                                            *
**************************************************************/

//Communication
int USBIO_Open(int *Handle);
// int USBIO_OpenBySerialNr(int *Handle, int Serial[]);
// int USBIO_OpenByIdNr(int *Handle, int IdNr);
int USBIO_Close(int *Handle);
int USBIO_IsConnected(int *Handle);	
// int USBIO_SetTimeOut(int *Handle, unsigned int TimeOut);
// int USBIO_ListAll(int *Antal, int IdNr[]);  //Ändra, mer info

//Read
int USBIO_ReadDI( int *Handle, int Port, int *Value);
int USBIO_ReadChannelDI( int *Handle, int Port, int Channel, int *Value);
int USBIO_ReadAI( int *Handle, int Channel, float *Value);
int USBIO_ReadAllAI( int *Handle, float AIValue[]);		
int USBIO_ReadADVal( int *Handle, int Channel, int *Value);
int USBIO_ReadAllADVal( int *Handle, int ADValue[]);
int USBIO_ReadCounter( int *Handle, unsigned int *Value, int *Overflow);
int USBIO_ReadUART( int *Handle, int *Nr, unsigned char Value[], int *Overflow);
// int USBIO_ReadFreq( int *Handle, float *Value, float *Width);

//Write				
int USBIO_WriteDO(int *Handle, int Port, int Value);
int USBIO_WriteChannelDO(int *Handle, int Port, int Channel, int Value);
int USBIO_WriteAO( int *Handle, int Channel, float Value);
int USBIO_WritePWM( int *Handle, int Channel, int Value);
int USBIO_WriteLowFreq( int *Handle, int Freq, unsigned char PortA, unsigned char PortB, unsigned char PortC);
int USBIO_WriteUART( int *Handle, unsigned char Value);
int USBIO_WriteLED( int *Handle, int Value); 

//Configure				
int USBIO_ConfigDIO(int *Handle, int Port, int Value);
int USBIO_ConfigAI(int *Handle, int Value);	
int USBIO_ConfigAO(int *Handle, int Value);
int USBIO_ConfigPWM(int *Handle, int Value, int Freq);	
int USBIO_ConfigCounter(int *Handle, int Value);	
int USBIO_ConfigUART(int *Handle, int BaudRate);
//int USBIO_Reset(int *Handle);
int USBIO_SoftReset(int *Handle);
int USBIO_SetIdNr(int *Handle, int IdNr, char Description[]);
int USBIO_ConfigWatchdog(int *Handle, int Active, int TimeOut, int Reset, unsigned char PortMask[], unsigned char Port[], int AOfunction);
int USBIO_ResetWatchdog(int *Handle);

//Status
int USBIO_GetStatus(int *Handle, int Value[]);
int USBIO_GetFirmwareVersion(int *Handle, char *Version);
int USBIO_GetHardwareVersion(int *Handle, char *Version);
int USBIO_GetSerialNr(int *Handle, unsigned int *Serial);
int USBIO_GetLibVersion(char *Version);
int USBIO_GetIdNr(int *Handle, int *IdNr, char *Description);

//Error
int USBIO_GetErrorString(int Error, char *ErrorMessage , int *StringLength);	

//Advanced functions
int USBIO_WriteAndRead(int *Handle, unsigned char WriteBuffer[], unsigned char ReadBuffer[]);












