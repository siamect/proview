/*
  Dummy for libcifx.a
*/

#include <stdint.h>

typedef void * CIFXHANDLE;
typedef struct {
  char p[10];
} CIFX_PACKET;

struct CIFX_LINUX_INIT {
  char p[10];
};

int32_t cifXDriverInit(const struct CIFX_LINUX_INIT* init_params) { return -1;}
int32_t xSysdeviceOpen( CIFXHANDLE  hDriver, char* szBoard, CIFXHANDLE* phSysdevice) { return -1;}
int32_t xSysdeviceInfo( CIFXHANDLE  hSysdevice, uint32_t ulCmd, uint32_t ulSize, void* pvInfo) { return -1;}
int32_t xChannelCommonStatusBlock( CIFXHANDLE  hChannel, uint32_t ulCmd, uint32_t ulOffset, uint32_t ulDataLen, void* pvData) { return -1;}
int32_t xChannelHostState( CIFXHANDLE  hChannel, uint32_t ulCmd, uint32_t* pulState, uint32_t ulTimeout) { return -1;}
int32_t xChannelBusState( CIFXHANDLE  hChannel, uint32_t ulCmd, uint32_t* pulState, uint32_t ulTimeout) { return -1;}
int32_t xDriverOpen( CIFXHANDLE* phDriver) { return -1;}
int32_t xChannelOpen( CIFXHANDLE  hDriver,  char* szBoard, uint32_t ulChannel, CIFXHANDLE* phChannel) { return -1; }
int32_t xChannelReset( CIFXHANDLE  hChannel, uint32_t ulResetMode, uint32_t ulTimeout) { return -1; }
int32_t xDriverEnumBoards( CIFXHANDLE  hDriver, uint32_t ulBoard, uint32_t ulSize, void* pvBoardInfo) { return -1;}
int32_t xDriverEnumChannels( CIFXHANDLE  hDriver, uint32_t ulBoard, uint32_t ulChannel, uint32_t ulSize, void* pvChannelInfo) { return -1; }
int32_t xChannelPutPacket( CIFXHANDLE  hChannel, CIFX_PACKET*  ptSendPkt, uint32_t ulTimeout) { return -1;}
int32_t xChannelGetPacket( CIFXHANDLE  hChannel, uint32_t ulSize, CIFX_PACKET* ptRecvPkt, uint32_t ulTimeout) { return -1; }
int32_t xChannelIORead( CIFXHANDLE  hChannel, uint32_t ulAreaNumber, uint32_t ulOffset, uint32_t ulDataLen, void* pvData, uint32_t ulTimeout) { return -1;}
int32_t xDriverGetErrorDescription( int32_t lError, char* szBuffer, uint32_t ulBufferLen) { return -1;}
int32_t xChannelIOWrite( CIFXHANDLE  hChannel, uint32_t ulAreaNumber, uint32_t ulOffset, uint32_t ulDataLen, void* pvData, uint32_t ulTimeout) { return -1;}
int32_t xDriverClose( CIFXHANDLE  hDriver) { return -1;}










