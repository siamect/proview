/*
  Dummy for libusbio.a
 */

//Communication
int USBIO_Open(int *Handle) { return -1;}
// int USBIO_OpenBySerialNr(int *Handle, int Serial[]);
// int USBIO_OpenByIdNr(int *Handle, int IdNr);
int USBIO_Close(int *Handle) { return -1;}
int USBIO_IsConnected(int *Handle) { return -1;}	
// int USBIO_SetTimeOut(int *Handle, unsigned int TimeOut);
// int USBIO_ListAll(int *Antal, int IdNr[]);  //Ändra, mer info

//Read
int USBIO_ReadDI( int *Handle, int Port, int *Value) { return -1;}
int USBIO_ReadChannelDI( int *Handle, int Port, int Channel, int *Value) { return -1;}
int USBIO_ReadAI( int *Handle, int Channel, float *Value) { return -1;}
int USBIO_ReadAllAI( int *Handle, float AIValue[]) { return -1;}
int USBIO_ReadADVal( int *Handle, int Channel, int *Value) { return -1;}
int USBIO_ReadAllADVal( int *Handle, int ADValue[]) { return -1;}
int USBIO_ReadCounter( int *Handle, unsigned int *Value, int *Overflow) { return -1;}
int USBIO_ReadUART( int *Handle, int *Nr, unsigned char Value[], int *Overflow) { return -1;}
// int USBIO_ReadFreq( int *Handle, float *Value, float *Width);

//Write				
int USBIO_WriteDO(int *Handle, int Port, int Value) { return -1;}
int USBIO_WriteChannelDO(int *Handle, int Port, int Channel, int Value) { return -1;}
int USBIO_WriteAO( int *Handle, int Channel, float Value) { return -1;}
int USBIO_WritePWM( int *Handle, int Channel, int Value) { return -1;}
int USBIO_WriteLowFreq( int *Handle, int Freq, unsigned char PortA, unsigned char PortB, unsigned char PortC) { return -1;}
int USBIO_WriteUART( int *Handle, unsigned char Value) { return -1;}
int USBIO_WriteLED( int *Handle, int Value) { return -1;}

//Configure				
int USBIO_ConfigDIO(int *Handle, int Port, int Value) { return -1;}
int USBIO_ConfigAI(int *Handle, int Value) { return -1;}	
int USBIO_ConfigAO(int *Handle, int Value) { return -1;}
int USBIO_ConfigPWM(int *Handle, int Value, int Freq) { return -1;}
int USBIO_ConfigCounter(int *Handle, int Value) { return -1;}	
int USBIO_ConfigUART(int *Handle, int BaudRate) { return -1;}
//int USBIO_Reset(int *Handle);
int USBIO_SoftReset(int *Handle) { return -1;}
int USBIO_SetIdNr(int *Handle, int IdNr, char Description[]) { return -1;}
int USBIO_ConfigWatchdog(int *Handle, int Active, int TimeOut, int Reset, unsigned char PortMask[], unsigned char Port[], int AOfunction) { return -1;}
int USBIO_ResetWatchdog(int *Handle) { return -1;}

//Status
int USBIO_GetStatus(int *Handle, int Value[]) { return -1;}
int USBIO_GetFirmwareVersion(int *Handle, char *Version) { return -1;}
int USBIO_GetHardwareVersion(int *Handle, char *Version) { return -1;}
int USBIO_GetSerialNr(int *Handle, unsigned int *Serial) { return -1;}
int USBIO_GetLibVersion(char *Version) { return -1;}
int USBIO_GetIdNr(int *Handle, int *IdNr, char *Description) { return -1;}

//Error
int USBIO_GetErrorString(int Error, char *ErrorMessage , int *StringLength) { return -1;}	

//Advanced functions
int USBIO_WriteAndRead(int *Handle, unsigned char WriteBuffer[], unsigned char ReadBuffer[]) { return -1;}












