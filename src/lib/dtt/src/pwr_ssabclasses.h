#ifndef pwr_ssabclasses_h
#define pwr_ssabclasses_h

#ifndef pwr_class_h
#include "pwr_class.h"
#endif


#ifndef pwr_cClass_Buff1440
#define pwr_cClass_Buff1440 16973984


/* Class: Buff1440
   Body:  RtBody  */


typedef struct {
  pwr_tInt16                          Data[720];
} pwr_sClass_Buff1440;

#endif

#ifndef pwr_cClass_Buff256
#define pwr_cClass_Buff256 16973976


/* Class: Buff256
   Body:  RtBody  */


typedef struct {
  pwr_tInt16                          Data[128];
} pwr_sClass_Buff256;

#endif

#ifndef pwr_cClass_Buff32k
#define pwr_cClass_Buff32k 16974000


/* Class: Buff32k
   Body:  RtBody  */


typedef struct {
  pwr_tInt16                          Data[16384];
} pwr_sClass_Buff32k;

#endif

#ifndef pwr_cClass_Buff4096
#define pwr_cClass_Buff4096 16973992


/* Class: Buff4096
   Body:  RtBody  */


typedef struct {
  pwr_tInt16                          Data[2048];
} pwr_sClass_Buff4096;

#endif

#ifndef pwr_cClass_LoggConfig
#define pwr_cClass_LoggConfig 16973840


/* Class: LoggConfig
   Body:  RtBody  */


typedef struct {
  pwr_tString40                       LoggFile;
  pwr_tBoolean                        NewVersion;
  pwr_tUInt32                         Identity;
  pwr_tUInt32                         LoggCount;
  pwr_tUInt32                         FileOpenCount;
} pwr_sClass_LoggConfig;

#endif

#ifndef pwr_cClass_LocTrans
#define pwr_cClass_LocTrans 16974008


/* Class: LocTrans
   Body:  RtBody  */


typedef struct {
  pwr_tString80                       Description;
  pwr_tBoolean                        DataValid;
} pwr_sClass_LocTrans;

#endif

#ifndef pwr_cClass_MultiCast
#define pwr_cClass_MultiCast 16973864


/* Class: MultiCast
   Body:  RtBody  */


typedef struct {
  pwr_tUInt16                         Address[16];
} pwr_sClass_MultiCast;

#endif

#ifndef pwr_cClass_RemAi
#define pwr_cClass_RemAi 16973904


/* Class: RemAi
   Body:  RtBody  */


typedef struct {
  pwr_tString80                       Description;
} pwr_sClass_RemAi;

#endif

#ifndef pwr_cClass_RemAo
#define pwr_cClass_RemAo 16973912


/* Class: RemAo
   Body:  RtBody  */


typedef struct {
  pwr_tString80                       Description;
} pwr_sClass_RemAo;

#endif

#ifndef pwr_cClass_RemChan_Ai
#define pwr_cClass_RemChan_Ai 16973944


/* Class: RemChan_Ai
   Body:  RtBody  */


typedef struct {
  pwr_tString80                       Description;
  pwr_tObjid                          SigChanCon;
  pwr_tString40                       Identity;
  pwr_tFloat32                        *ActualValue;
  pwr_tUInt32                         SensorPolyType;
  pwr_tFloat32                        SensorPolyCoef0;
  pwr_tFloat32                        SensorPolyCoef1;
  pwr_tFloat32                        SensorPolyCoef2;
  pwr_tUInt32                         ConvOff;
} pwr_sClass_RemChan_Ai;

#endif

#ifndef pwr_cClass_RemChan_Ao
#define pwr_cClass_RemChan_Ao 16973952


/* Class: RemChan_Ao
   Body:  RtBody  */


typedef struct {
  pwr_tString80                       Description;
  pwr_tObjid                          SigChanCon;
  pwr_tString40                       Identity;
  pwr_tFloat32                        *ActualValue;
  pwr_tFloat32                        OutPolyCoef0;
  pwr_tFloat32                        OutPolyCoef1;
  pwr_tFloat32                        OutMaxLimit;
  pwr_tFloat32                        OutMinLimit;
  pwr_tUInt32                         ConvOff;
  pwr_tInt16                          OldValue;
  char                                filler_0[2];
  pwr_tBoolean                        PwrIsMaster;
} pwr_sClass_RemChan_Ao;

#endif

#ifndef pwr_cClass_RemChan_Co
#define pwr_cClass_RemChan_Co 16973960


/* Class: RemChan_Co
   Body:  RtBody  */


typedef struct {
  pwr_tString80                       Description;
  pwr_tObjid                          SigChanCon;
  pwr_tString40                       Identity;
  pwr_tInt32                          *ActualValue;
  pwr_tInt32                          *ExtendedValue;
  pwr_tUInt32                         NoOfBits;
  pwr_tUInt32                         ConvOff;
  pwr_tUInt32                         SyncRawValue;
  pwr_tBoolean                        CounterZeroFlag;
  pwr_tBoolean                        CounterSyncFlag;
} pwr_sClass_RemChan_Co;

#endif

#ifndef pwr_cClass_RemChan_Di
#define pwr_cClass_RemChan_Di 16973928


/* Class: RemChan_Di
   Body:  RtBody  */


typedef struct {
  pwr_tString80                       Description;
  pwr_tObjid                          SigChanCon;
  pwr_tString40                       Identity;
  pwr_tBoolean                        *ActualValue;
  pwr_tUInt32                         ConvOff;
  pwr_tUInt32                         BuffOff;
  pwr_tUInt8                          ConvMask;
  char                                filler_0[3];
} pwr_sClass_RemChan_Di;

#endif

#ifndef pwr_cClass_RemChan_Do
#define pwr_cClass_RemChan_Do 16973936


/* Class: RemChan_Do
   Body:  RtBody  */


typedef struct {
  pwr_tString80                       Description;
  pwr_tObjid                          SigChanCon;
  pwr_tString40                       Identity;
  pwr_tBoolean                        *ActualValue;
  pwr_tUInt32                         ConvOff;
  pwr_tUInt32                         BuffOff;
  pwr_tUInt8                          ConvMask;
  char                                filler_0[3];
  pwr_tBoolean                        PwrIsMaster;
  pwr_tBoolean                        OldValue;
} pwr_sClass_RemChan_Do;

#endif

#ifndef pwr_cClass_RemChan_Dv
#define pwr_cClass_RemChan_Dv 16973968


/* Class: RemChan_Dv
   Body:  RtBody  */


typedef struct {
  pwr_tString80                       Description;
  pwr_tObjid                          SigChanCon;
  pwr_tBoolean                        *ActualValue;
  pwr_tUInt32                         ConvOff;
  pwr_tUInt32                         BuffOff;
  pwr_tUInt8                          ConvMask;
  char                                filler_0[3];
  pwr_tBoolean                        PwrIsMaster;
  pwr_tBoolean                        OldValue;
} pwr_sClass_RemChan_Dv;

#endif

#ifndef pwr_cClass_RemCo
#define pwr_cClass_RemCo 16973920


/* Class: RemCo
   Body:  RtBody  */


typedef struct {
  pwr_tString80                       Description;
} pwr_sClass_RemCo;

#endif

#ifndef pwr_cClass_RemDi
#define pwr_cClass_RemDi 16973880


/* Class: RemDi
   Body:  RtBody  */


typedef struct {
  pwr_tString80                       Description;
} pwr_sClass_RemDi;

#endif

#ifndef pwr_cClass_RemDo
#define pwr_cClass_RemDo 16973888


/* Class: RemDo
   Body:  RtBody  */


typedef struct {
  pwr_tString80                       Description;
} pwr_sClass_RemDo;

#endif

#ifndef pwr_cClass_RemDv
#define pwr_cClass_RemDv 16973896


/* Class: RemDv
   Body:  RtBody  */


typedef struct {
  pwr_tString80                       Description;
} pwr_sClass_RemDv;

#endif

#ifndef pwr_cClass_RemNode
#define pwr_cClass_RemNode 16973856


/* Class: RemNode
   Body:  RtBody  */


typedef struct {
  pwr_tString80                       Description;
  pwr_tString40                       NodeName;
  pwr_tUInt16                         Address[4];
  pwr_tUInt32                         TransportType;
  pwr_tUInt32                         NumberOfRestarts;
  pwr_tFloat32                        CycleTime;
  pwr_tFloat32                        ErrTime;
  pwr_tFloat32                        IOCycleTime;
  pwr_tFloat32                        IOStallTime;
  pwr_tUInt32                         IOStallAction;
  pwr_tObjid                          IODataArea;
  pwr_tUInt32                         NumberOfTrans;
  pwr_tUInt32                         NumberOfDI;
  pwr_tUInt32                         NumberOfDO;
  pwr_tUInt32                         NumberOfAI;
  pwr_tUInt32                         NumberOfAO;
  pwr_tUInt32                         NumberOfDV;
  pwr_tUInt32                         NumberOfCo;
  pwr_tInt32                          PollDiff;
  pwr_tUInt32                         ErrTransCount;
  pwr_tBoolean                        Poll;
  pwr_tBoolean                        IOStallFlag;
} pwr_sClass_RemNode;

#endif

#ifndef pwr_cClass_RemTrans
#define pwr_cClass_RemTrans 16973872


/* Class: RemTrans
   Body:  RtBody  */


typedef struct {
  pwr_tString80                       Description;
  pwr_tString40                       TransName;
  pwr_tUInt16                         Address[4];
  pwr_tUInt32                         Direction;
  pwr_tUInt32                         DataLength;
  pwr_tUInt32                         LoggLevel;
  pwr_tUInt32                         MaxBuffers;
  pwr_tUInt32                         Buffers;
  pwr_tUInt32                         MaxLength;
  pwr_tUInt32                         LastSts;
  pwr_tTime                           TransTime;
  pwr_tUInt32                         TransCount;
  pwr_tUInt32                         BuffCount;
  pwr_tUInt32                         LostCount;
  pwr_tUInt32                         ErrCount;
  pwr_tBoolean                        DataValid;
  pwr_tString40                       StructName;
  pwr_tString80                       StructFile;
} pwr_sClass_RemTrans;

#endif

#endif
