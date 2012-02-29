/*
  Dummy for libnodave
*/

#include <stdint.h>

#define DECL2
#define EXPORTSPEC

typedef void * _daveOSserialType;
typedef void * daveInterface;
typedef void * daveConnection;

int openSocket(const int port, const char * peer) { return -1;}
int closeSocket(int h) { return -1;}
int setPort(char * name, char* baud, char parity) { return -1;}
int closePort(int port) { return -1;}

EXPORTSPEC int DECL2 daveInitAdapter(daveInterface * di) { return -1;}
EXPORTSPEC int DECL2 daveConnectPLC(daveConnection * dc) { return -1;}
EXPORTSPEC int DECL2 daveDisconnectPLC(daveConnection * dc) { return -1;}
EXPORTSPEC int DECL2 daveDisconnectAdapter(daveInterface * di) { return -1;}
EXPORTSPEC int DECL2 daveListReachablePartners(daveInterface * di, char * buf) { return -1;}
EXPORTSPEC daveInterface * DECL2 daveNewInterface(_daveOSserialType nfd, char * nname, int localMPI, int protocol, int speed) { return 0;}
EXPORTSPEC daveConnection * DECL2 daveNewConnection(daveInterface * di, int MPI,int rack, int slot) { return 0;}
EXPORTSPEC int DECL2 daveReadBytes(daveConnection * dc, int area, int DB, int start, int len, void * buffer) { return -1;}
EXPORTSPEC int DECL2 daveWriteBytes(daveConnection * dc,int area, int DB, int start, int len, void * buffer) { return -1;}
EXPORTSPEC char * DECL2 daveStrerror(int code) {
  static char r[] = "Not build with libnodave";
  return r;
}
EXPORTSPEC void DECL2 daveSetTimeout(daveInterface * di, int tmo) {}







