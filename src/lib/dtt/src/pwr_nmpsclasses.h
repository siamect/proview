#ifndef pwr_nmpsclasses_h
#define pwr_nmpsclasses_h

#ifndef pwr_class_h
#include "pwr_class.h"
#endif


#ifndef pwr_cClass_CellDisp
#define pwr_cClass_CellDisp 16843104


/* Class: CellDisp
   Body:  RtBody  */


typedef struct {
  pwr_tFloat32                        **Cell1P;
  pwr_tFloat32                        *Cell1;
  pwr_tFloat32                        **Cell2P;
  pwr_tFloat32                        *Cell2;
  pwr_tFloat32                        **Cell3P;
  pwr_tFloat32                        *Cell3;
  pwr_tFloat32                        **Cell4P;
  pwr_tFloat32                        *Cell4;
  pwr_tFloat32                        **Cell5P;
  pwr_tFloat32                        *Cell5;
  pwr_tFloat32                        **Cell6P;
  pwr_tFloat32                        *Cell6;
  pwr_tFloat32                        **Cell7P;
  pwr_tFloat32                        *Cell7;
  pwr_tFloat32                        **Cell8P;
  pwr_tFloat32                        *Cell8;
  pwr_tFloat32                        **Cell9P;
  pwr_tFloat32                        *Cell9;
  pwr_tFloat32                        **Cell10P;
  pwr_tFloat32                        *Cell10;
  pwr_tFloat32                        *LinkP;
  pwr_tFloat32                        Link;
  pwr_tFloat32                        **DisplayObjectP;
  pwr_tFloat32                        *DisplayObject;
  pwr_tClassId                        DataClass;
  pwr_tInt32                          Function;
  pwr_tInt32                          Number;
  pwr_tInt32                          MaxSize;
  pwr_tInt32                          SelDirection;
  pwr_tString32                       FloatAttr[5];
  pwr_tString32                       BooleanAttr[5];
  pwr_tString32                       IntAttr[5];
  pwr_tBoolean                        Select[60];
  pwr_tBoolean                        OldSelect[60];
  pwr_tObjid                          Objid[60];
  pwr_tFloat32                        F1[60];
  pwr_tFloat32                        F2[60];
  pwr_tFloat32                        F3[60];
  pwr_tFloat32                        F4[60];
  pwr_tFloat32                        F5[60];
  pwr_tBoolean                        B1[60];
  pwr_tBoolean                        B2[60];
  pwr_tBoolean                        B3[60];
  pwr_tBoolean                        B4[60];
  pwr_tBoolean                        B5[60];
  pwr_tInt32                          I1[60];
  pwr_tInt32                          I2[60];
  pwr_tInt32                          I3[60];
  pwr_tInt32                          I4[60];
  pwr_tInt32                          I5[60];
  pwr_tInt32                          FloatAttrOffs[5];
  pwr_tInt32                          BooleanAttrOffs[5];
  pwr_tInt32                          IntAttrOffs[5];
  pwr_tObjid                          SelectObjid;
  pwr_tUInt32                         DataSize;
  pwr_tInt32                          OldLastIndex;
} pwr_sClass_CellDisp;

/* Class: CellDisp
   Body:  DevBody  */

typedef struct {
  pwr_sPlcNode                        PlcNode;
} pwr_sdClass_CellDisp;

#endif

#ifndef pwr_cClass_CellUpdate
#define pwr_cClass_CellUpdate 16843120


/* Class: CellUpdate
   Body:  RtBody  */


typedef struct {
  pwr_tFloat32                        **Cell1P;
  pwr_tFloat32                        *Cell1;
  pwr_tFloat32                        **Cell2P;
  pwr_tFloat32                        *Cell2;
  pwr_tFloat32                        **Cell3P;
  pwr_tFloat32                        *Cell3;
  pwr_tFloat32                        **Cell4P;
  pwr_tFloat32                        *Cell4;
  pwr_tFloat32                        **Cell5P;
  pwr_tFloat32                        *Cell5;
  pwr_tFloat32                        **Cell6P;
  pwr_tFloat32                        *Cell6;
  pwr_tFloat32                        **Cell7P;
  pwr_tFloat32                        *Cell7;
  pwr_tFloat32                        **Cell8P;
  pwr_tFloat32                        *Cell8;
  pwr_tFloat32                        **Cell9P;
  pwr_tFloat32                        *Cell9;
  pwr_tFloat32                        **Cell10P;
  pwr_tFloat32                        *Cell10;
  pwr_tFloat32                        *LinkP;
  pwr_tFloat32                        Link;
  pwr_tFloat32                        **DisplayObjectP;
  pwr_tFloat32                        *DisplayObject;
  pwr_tClassId                        DataClass;
  pwr_tInt32                          Function;
  pwr_tInt32                          Number;
  pwr_tInt32                          MaxSize;
  pwr_tInt32                          SelDirection;
  pwr_tString32                       FloatAttr[5];
  pwr_tString32                       BooleanAttr[5];
  pwr_tString32                       IntAttr[5];
  pwr_tBoolean                        Select[60];
  pwr_tBoolean                        OldSelect[60];
  pwr_tObjid                          Objid[60];
  pwr_tFloat32                        F1[60];
  pwr_tFloat32                        F2[60];
  pwr_tFloat32                        F3[60];
  pwr_tFloat32                        F4[60];
  pwr_tFloat32                        F5[60];
  pwr_tBoolean                        B1[60];
  pwr_tBoolean                        B2[60];
  pwr_tBoolean                        B3[60];
  pwr_tBoolean                        B4[60];
  pwr_tBoolean                        B5[60];
  pwr_tInt32                          I1[60];
  pwr_tInt32                          I2[60];
  pwr_tInt32                          I3[60];
  pwr_tInt32                          I4[60];
  pwr_tInt32                          I5[60];
  pwr_tInt32                          FloatAttrOffs[5];
  pwr_tInt32                          BooleanAttrOffs[5];
  pwr_tInt32                          IntAttrOffs[5];
  pwr_tObjid                          SelectObjid;
  pwr_tUInt32                         DataSize;
  pwr_tInt32                          OldLastIndex;
  pwr_tFloat32                        F1Old[60];
  pwr_tFloat32                        F2Old[60];
  pwr_tFloat32                        F3Old[60];
  pwr_tFloat32                        F4Old[60];
  pwr_tFloat32                        F5Old[60];
  pwr_tBoolean                        B1Old[60];
  pwr_tBoolean                        B2Old[60];
  pwr_tBoolean                        B3Old[60];
  pwr_tBoolean                        B4Old[60];
  pwr_tBoolean                        B5Old[60];
  pwr_tInt32                          I1Old[60];
  pwr_tInt32                          I2Old[60];
  pwr_tInt32                          I3Old[60];
  pwr_tInt32                          I4Old[60];
  pwr_tInt32                          I5Old[60];
} pwr_sClass_CellUpdate;

/* Class: CellUpdate
   Body:  DevBody  */

typedef struct {
  pwr_sPlcNode                        PlcNode;
} pwr_sdClass_CellUpdate;

#endif

#ifndef pwr_cClass_CLoop
#define pwr_cClass_CLoop 16842864


/* Class: CLoop
   Body:  RtBody  */


typedef struct {
  pwr_tBoolean                        *ConditionP;
  pwr_tBoolean                        Condition;
  pwr_tFloat32                        *StartIndexP;
  pwr_tFloat32                        StartIndex;
  pwr_tFloat32                        *IncrementP;
  pwr_tFloat32                        Increment;
  pwr_tFloat32                        *StopIndexP;
  pwr_tFloat32                        StopIndex;
  pwr_tFloat32                        CurrentIndex;
} pwr_sClass_CLoop;

/* Class: CLoop
   Body:  DevBody  */

typedef struct {
  pwr_sPlcNode                        PlcNode;
} pwr_sdClass_CLoop;

#endif

#ifndef pwr_cClass_ConvDef
#define pwr_cClass_ConvDef 16843064


/* Class: ConvDef
   Body:  RtBody  */


typedef struct {
  pwr_tString80                       Param[200];
} pwr_sClass_ConvDef;

#endif

#ifndef pwr_cClass_currentdata
#define pwr_cClass_currentdata 16842840


/* Class: CurrentData
   Body:  RtBody  */


typedef struct {
  pwr_tFloat32                        *DataCurrentP;
} pwr_sClass_currentdata;

/* Class: CurrentData
   Body:  DevBody  */

typedef struct {
  pwr_tObjid                          CellObject;
  pwr_sPlcNode                        PlcNode;
} pwr_sdClass_CurrentData;

#endif

#ifndef pwr_cClass_currentindex
#define pwr_cClass_currentindex 16842848


/* Class: CurrentIndex
   Body:  RtBody  */


typedef struct {
  pwr_tFloat32                        CurrentIndex;
} pwr_sClass_currentindex;

/* Class: CurrentIndex
   Body:  DevBody  */

typedef struct {
  pwr_tObjid                          CellObject;
  pwr_sPlcNode                        PlcNode;
} pwr_sdClass_CurrentIndex;

#endif

#ifndef pwr_cClass_datacopy
#define pwr_cClass_datacopy 16842824


/* Class: DataCopy
   Body:  RtBody  */


typedef struct {
  pwr_tFloat32                        **DataFromP;
  pwr_tFloat32                        *DataFrom;
  pwr_tFloat32                        **DataToP;
  pwr_tFloat32                        *DataTo;
  pwr_tBoolean                        *ConditionP;
  pwr_tBoolean                        Condition;
} pwr_sClass_datacopy;

/* Class: DataCopy
   Body:  DevBody  */

typedef struct {
  pwr_sPlcNode                        PlcNode;
} pwr_sdClass_DataCopy;

#endif

#ifndef pwr_cClass_datacnv
#define pwr_cClass_datacnv 16843056


/* Class: DataCnv
   Body:  RtBody  */


typedef struct {
  pwr_tFloat32                        **DataFromP;
  pwr_tFloat32                        *DataFrom;
  pwr_tFloat32                        **DataToP;
  pwr_tFloat32                        *DataTo;
  pwr_tBoolean                        *ConditionP;
  pwr_tBoolean                        Condition;
  pwr_tInt32                          ToConvdefType;
  pwr_tObjid                          ToConvdef;
  pwr_tString40                       ToConvdefFile;
  pwr_tInt32                          FromConvdefType;
  pwr_tObjid                          FromConvdef;
  pwr_tString40                       FromConvdefFile;
  pwr_tInt32                          ConvTableCount;
  pwr_tInt32                          ConvTable[1800];
} pwr_sClass_datacnv;

/* Class: DataCnv
   Body:  DevBody  */

typedef struct {
  pwr_sPlcNode                        PlcNode;
} pwr_sdClass_DataCnv;

#endif

#ifndef pwr_cClass_DataRcv
#define pwr_cClass_DataRcv 16843080


/* Class: DataRcv
   Body:  RtBody  */


typedef struct {
  pwr_tInt32                          *OutP;
  pwr_tInt32                          Out;
  pwr_tBoolean                        *AcceptP;
  pwr_tBoolean                        Accept;
  pwr_tBoolean                        *ResetP;
  pwr_tBoolean                        Reset;
  pwr_tBoolean                        DataReceived;
  pwr_tBoolean                        Error;
  pwr_tInt32                          Function;
  pwr_tString80                       AlarmText[5];
  pwr_tObjid                          DisplayObject;
  pwr_tObjid                          SendRemTrans;
  pwr_tObjid                          RcvRemTrans;
  pwr_tObjid                          CellObject;
  pwr_tObjid                          DataClass;
  pwr_tObjid                          DataParent;
  pwr_tInt32                          ToConvdefType;
  pwr_tObjid                          ToConvdef;
  pwr_tString40                       ToConvdefFile;
  pwr_tInt32                          FromConvdefType;
  pwr_tObjid                          FromConvdef;
  pwr_tString40                       FromConvdefFile;
  pwr_tBoolean                        OpAccept;
  pwr_tBoolean                        OpReset;
  pwr_tBoolean                        AcceptOld;
  pwr_tBoolean                        ResetOld;
  pwr_tBoolean                        SysAcceptDetected;
  pwr_tBoolean                        OpAcceptDetected;
  pwr_tBoolean                        AcceptDetected;
  pwr_tBoolean                        ResetDetected;
  pwr_tBoolean                        DataRcvDetected;
  pwr_tBoolean                        ErrorDetected;
} pwr_sClass_DataRcv;

/* Class: DataRcv
   Body:  DevBody  */

typedef struct {
  pwr_sPlcNode                        PlcNode;
} pwr_sdClass_DataRcv;

#endif

#ifndef pwr_cClass_DataRequest
#define pwr_cClass_DataRequest 16843072


/* Class: DataRequest
   Body:  RtBody  */


typedef struct {
  pwr_tInt32                          *OutP;
  pwr_tInt32                          Out;
  pwr_tBoolean                        *TriggP;
  pwr_tBoolean                        Trigg;
  pwr_tBoolean                        *AcceptP;
  pwr_tBoolean                        Accept;
  pwr_tBoolean                        *ResetP;
  pwr_tBoolean                        Reset;
  pwr_tBoolean                        DataReceived;
  pwr_tBoolean                        Error;
  pwr_tInt32                          Key;
  pwr_tString40                       KeyStr;
  pwr_tInt32                          Function;
  pwr_tFloat32                        TimeoutTime;
  pwr_tString80                       AlarmText[5];
  pwr_tUInt32                         ReturnStatus[10];
  pwr_tString80                       ReturnStatusText[10];
  pwr_tObjid                          DisplayObject;
  pwr_tObjid                          SendRemTrans;
  pwr_tObjid                          RcvRemTrans;
  pwr_tObjid                          CellObject;
  pwr_tObjid                          DataClass;
  pwr_tObjid                          DataParent;
  pwr_tInt32                          ToConvdefType;
  pwr_tObjid                          ToConvdef;
  pwr_tString40                       ToConvdefFile;
  pwr_tInt32                          FromConvdefType;
  pwr_tObjid                          FromConvdef;
  pwr_tString40                       FromConvdefFile;
  pwr_tBoolean                        OpTrigg;
  pwr_tBoolean                        OpAccept;
  pwr_tBoolean                        OpReset;
  pwr_tBoolean                        TriggOld;
  pwr_tBoolean                        AcceptOld;
  pwr_tBoolean                        ResetOld;
  pwr_tBoolean                        TriggDetected;
  pwr_tBoolean                        SysAcceptDetected;
  pwr_tBoolean                        OpAcceptDetected;
  pwr_tBoolean                        AcceptDetected;
  pwr_tBoolean                        ResetDetected;
  pwr_tBoolean                        DataRcvDetected;
  pwr_tBoolean                        ErrorDetected;
} pwr_sClass_DataRequest;

/* Class: DataRequest
   Body:  DevBody  */

typedef struct {
  pwr_sPlcNode                        PlcNode;
} pwr_sdClass_DataRequest;

#endif

#ifndef pwr_cClass_datareset
#define pwr_cClass_datareset 16842832


/* Class: DataReset
   Body:  RtBody  */


typedef struct {
  pwr_tFloat32                        **DataP;
  pwr_tFloat32                        *Data;
  pwr_tBoolean                        *ConditionP;
  pwr_tBoolean                        Condition;
} pwr_sClass_datareset;

/* Class: DataReset
   Body:  DevBody  */

typedef struct {
  pwr_sPlcNode                        PlcNode;
} pwr_sdClass_DataReset;

#endif

#ifndef pwr_cClass_DataSelect
#define pwr_cClass_DataSelect 16842872


/* Class: DataSelect
   Body:  RtBody  */


typedef struct {
  pwr_tFloat32                        *IndexP;
  pwr_tFloat32                        Index;
  pwr_tFloat32                        *MaxIndexP;
  pwr_tFloat32                        MaxIndex;
  pwr_tFloat32                        **DataIn1P;
  pwr_tFloat32                        *DataIn1;
  pwr_tFloat32                        **DataIn2P;
  pwr_tFloat32                        *DataIn2;
  pwr_tFloat32                        **DataIn3P;
  pwr_tFloat32                        *DataIn3;
  pwr_tFloat32                        **DataIn4P;
  pwr_tFloat32                        *DataIn4;
  pwr_tFloat32                        **DataIn5P;
  pwr_tFloat32                        *DataIn5;
  pwr_tFloat32                        **DataIn6P;
  pwr_tFloat32                        *DataIn6;
  pwr_tFloat32                        **DataIn7P;
  pwr_tFloat32                        *DataIn7;
  pwr_tFloat32                        **DataIn8P;
  pwr_tFloat32                        *DataIn8;
  pwr_tFloat32                        **DataIn9P;
  pwr_tFloat32                        *DataIn9;
  pwr_tFloat32                        **DataIn10P;
  pwr_tFloat32                        *DataIn10;
  pwr_tFloat32                        **DataIn11P;
  pwr_tFloat32                        *DataIn11;
  pwr_tFloat32                        **DataIn12P;
  pwr_tFloat32                        *DataIn12;
  pwr_tFloat32                        **DataIn13P;
  pwr_tFloat32                        *DataIn13;
  pwr_tFloat32                        **DataIn14P;
  pwr_tFloat32                        *DataIn14;
  pwr_tFloat32                        **DataIn15P;
  pwr_tFloat32                        *DataIn15;
  pwr_tFloat32                        **DataIn16P;
  pwr_tFloat32                        *DataIn16;
  pwr_tFloat32                        *OutDataP;
  pwr_tObjid                          OutData_ObjId;
} pwr_sClass_DataSelect;

/* Class: DataSelect
   Body:  DevBody  */

typedef struct {
  pwr_sPlcNode                        PlcNode;
} pwr_sdClass_DataSelect;

#endif

#ifndef pwr_cClass_DataSend
#define pwr_cClass_DataSend 16843088


/* Class: DataSend
   Body:  RtBody  */


typedef struct {
  pwr_tFloat32                        **DataInP;
  pwr_tFloat32                        *DataIn;
  pwr_tBoolean                        *TriggP;
  pwr_tBoolean                        Trigg;
  pwr_tBoolean                        *DummyP;
  pwr_tBoolean                        Dummy;
  pwr_tBoolean                        DataSent;
  pwr_tBoolean                        Error;
  pwr_tInt32                          Function;
  pwr_tFloat32                        TimeoutTime;
  pwr_tString80                       AlarmText[5];
  pwr_tUInt32                         ReturnStatus[10];
  pwr_tString80                       ReturnStatusText[10];
  pwr_tObjid                          SendRemTrans;
  pwr_tObjid                          RcvRemTrans;
  pwr_tInt32                          ToConvdefType;
  pwr_tObjid                          ToConvdef;
  pwr_tString40                       ToConvdefFile;
  pwr_tInt32                          FromConvdefType;
  pwr_tObjid                          FromConvdef;
  pwr_tString40                       FromConvdefFile;
  pwr_tBoolean                        TriggOld;
  pwr_tBoolean                        TriggDetected;
  pwr_tBoolean                        DataSentDetected;
  pwr_tBoolean                        ErrorDetected;
} pwr_sClass_DataSend;

/* Class: DataSend
   Body:  DevBody  */

typedef struct {
  pwr_sPlcNode                        PlcNode;
} pwr_sdClass_DataSend;

#endif

#ifndef pwr_cClass_DispLink
#define pwr_cClass_DispLink 16843112


/* Class: DispLink
   Body:  RtBody  */


typedef struct {
  pwr_tFloat32                        CellDisp;
  pwr_tObjid                          SelectObjid;
  pwr_tBoolean                        SelectExist;
  pwr_tBoolean                        MoveForward;
  pwr_tBoolean                        DoMoveForward;
  pwr_tBoolean                        MoveBackward;
  pwr_tBoolean                        DoMoveBackward;
  pwr_tBoolean                        Remove;
  pwr_tBoolean                        DoRemove;
  pwr_tBoolean                        ResetData;
  pwr_tBoolean                        SelectNext;
  pwr_tBoolean                        DoSelectNext;
  pwr_tBoolean                        SelectPrevious;
  pwr_tBoolean                        DoSelectPrevious;
  pwr_tInt32                          MaxDispNumber;
  pwr_tInt32                          SelectOrderNumber;
  pwr_tInt32                          SelectOrderOwner;
  pwr_tInt32                          SelectOrderType;
  pwr_tBoolean                        SelectOrder;
} pwr_sClass_DispLink;

/* Class: DispLink
   Body:  DevBody  */

typedef struct {
  pwr_sPlcNode                        PlcNode;
} pwr_sdClass_DispLink;

#endif

#ifndef pwr_cClass_Func
#define pwr_cClass_Func 16842920


/* Class: Func
   Body:  RtBody  */


typedef struct {
  pwr_tBoolean                        *ConditionP;
  pwr_tBoolean                        Condition;
  pwr_tFloat32                        *A1P;
  pwr_tFloat32                        A1;
  pwr_tFloat32                        *A2P;
  pwr_tFloat32                        A2;
  pwr_tFloat32                        *A3P;
  pwr_tFloat32                        A3;
  pwr_tFloat32                        *A4P;
  pwr_tFloat32                        A4;
  pwr_tFloat32                        *A5P;
  pwr_tFloat32                        A5;
  pwr_tFloat32                        *A6P;
  pwr_tFloat32                        A6;
  pwr_tFloat32                        *A7P;
  pwr_tFloat32                        A7;
  pwr_tFloat32                        *A8P;
  pwr_tFloat32                        A8;
  pwr_tBoolean                        *d1P;
  pwr_tBoolean                        d1;
  pwr_tBoolean                        *d2P;
  pwr_tBoolean                        d2;
  pwr_tBoolean                        *d3P;
  pwr_tBoolean                        d3;
  pwr_tBoolean                        *d4P;
  pwr_tBoolean                        d4;
  pwr_tBoolean                        *d5P;
  pwr_tBoolean                        d5;
  pwr_tBoolean                        *d6P;
  pwr_tBoolean                        d6;
  pwr_tBoolean                        *d7P;
  pwr_tBoolean                        d7;
  pwr_tBoolean                        *d8P;
  pwr_tBoolean                        d8;
  pwr_tFloat32                        OA1;
  pwr_tFloat32                        OA2;
  pwr_tFloat32                        OA3;
  pwr_tFloat32                        OA4;
  pwr_tFloat32                        OA5;
  pwr_tFloat32                        OA6;
  pwr_tFloat32                        OA7;
  pwr_tFloat32                        OA8;
  pwr_tBoolean                        od1;
  pwr_tBoolean                        od2;
  pwr_tBoolean                        od3;
  pwr_tBoolean                        od4;
  pwr_tBoolean                        od5;
  pwr_tBoolean                        od6;
  pwr_tBoolean                        od7;
  pwr_tBoolean                        od8;
} pwr_sClass_Func;

/* Class: Func
   Body:  DevBody  */

typedef struct {
  pwr_sPlcNode                        PlcNode;
  pwr_tObjid                          Function;
} pwr_sdClass_Func;

#endif

#ifndef pwr_cClass_FuncExtend
#define pwr_cClass_FuncExtend 16842928


/* Class: FuncExtend
   Body:  RtBody  */


typedef struct {
  pwr_tFloat32                        *A1P;
  pwr_tFloat32                        A1;
  pwr_tFloat32                        *A2P;
  pwr_tFloat32                        A2;
  pwr_tFloat32                        *A3P;
  pwr_tFloat32                        A3;
  pwr_tFloat32                        *A4P;
  pwr_tFloat32                        A4;
  pwr_tFloat32                        *A5P;
  pwr_tFloat32                        A5;
  pwr_tFloat32                        *A6P;
  pwr_tFloat32                        A6;
  pwr_tFloat32                        *A7P;
  pwr_tFloat32                        A7;
  pwr_tFloat32                        *A8P;
  pwr_tFloat32                        A8;
  pwr_tBoolean                        *d1P;
  pwr_tBoolean                        d1;
  pwr_tBoolean                        *d2P;
  pwr_tBoolean                        d2;
  pwr_tBoolean                        *d3P;
  pwr_tBoolean                        d3;
  pwr_tBoolean                        *d4P;
  pwr_tBoolean                        d4;
  pwr_tBoolean                        *d5P;
  pwr_tBoolean                        d5;
  pwr_tBoolean                        *d6P;
  pwr_tBoolean                        d6;
  pwr_tBoolean                        *d7P;
  pwr_tBoolean                        d7;
  pwr_tBoolean                        *d8P;
  pwr_tBoolean                        d8;
  pwr_tFloat32                        OA1;
  pwr_tFloat32                        OA2;
  pwr_tFloat32                        OA3;
  pwr_tFloat32                        OA4;
  pwr_tFloat32                        OA5;
  pwr_tFloat32                        OA6;
  pwr_tFloat32                        OA7;
  pwr_tFloat32                        OA8;
  pwr_tBoolean                        od1;
  pwr_tBoolean                        od2;
  pwr_tBoolean                        od3;
  pwr_tBoolean                        od4;
  pwr_tBoolean                        od5;
  pwr_tBoolean                        od6;
  pwr_tBoolean                        od7;
  pwr_tBoolean                        od8;
} pwr_sClass_FuncExtend;

/* Class: FuncExtend
   Body:  DevBody  */

typedef struct {
  pwr_tObjid                          FuncObject;
  pwr_tUInt32                         Index;
  pwr_sPlcNode                        PlcNode;
} pwr_sdClass_FuncExtend;

#endif

#ifndef pwr_cClass_FuncInput
#define pwr_cClass_FuncInput 16842936


/* Class: FuncInput
   Body:  RtBody  */


typedef struct {
  pwr_tFloat32                        A1;
  pwr_tFloat32                        A2;
  pwr_tFloat32                        A3;
  pwr_tFloat32                        A4;
  pwr_tFloat32                        A5;
  pwr_tFloat32                        A6;
  pwr_tFloat32                        A7;
  pwr_tFloat32                        A8;
  pwr_tBoolean                        d1;
  pwr_tBoolean                        d2;
  pwr_tBoolean                        d3;
  pwr_tBoolean                        d4;
  pwr_tBoolean                        d5;
  pwr_tBoolean                        d6;
  pwr_tBoolean                        d7;
  pwr_tBoolean                        d8;
} pwr_sClass_FuncInput;

/* Class: FuncInput
   Body:  DevBody  */

typedef struct {
  pwr_tUInt32                         Index;
  pwr_sPlcNode                        PlcNode;
} pwr_sdClass_FuncInput;

#endif

#ifndef pwr_cClass_FuncOutput
#define pwr_cClass_FuncOutput 16842944


/* Class: FuncOutput
   Body:  RtBody  */


typedef struct {
  pwr_tFloat32                        *OA1P;
  pwr_tFloat32                        OA1;
  pwr_tFloat32                        *OA2P;
  pwr_tFloat32                        OA2;
  pwr_tFloat32                        *OA3P;
  pwr_tFloat32                        OA3;
  pwr_tFloat32                        *OA4P;
  pwr_tFloat32                        OA4;
  pwr_tFloat32                        *OA5P;
  pwr_tFloat32                        OA5;
  pwr_tFloat32                        *OA6P;
  pwr_tFloat32                        OA6;
  pwr_tFloat32                        *OA7P;
  pwr_tFloat32                        OA7;
  pwr_tFloat32                        *OA8P;
  pwr_tFloat32                        OA8;
  pwr_tBoolean                        *od1P;
  pwr_tBoolean                        od1;
  pwr_tBoolean                        *od2P;
  pwr_tBoolean                        od2;
  pwr_tBoolean                        *od3P;
  pwr_tBoolean                        od3;
  pwr_tBoolean                        *od4P;
  pwr_tBoolean                        od4;
  pwr_tBoolean                        *od5P;
  pwr_tBoolean                        od5;
  pwr_tBoolean                        *od6P;
  pwr_tBoolean                        od6;
  pwr_tBoolean                        *od7P;
  pwr_tBoolean                        od7;
  pwr_tBoolean                        *od8P;
  pwr_tBoolean                        od8;
} pwr_sClass_FuncOutput;

/* Class: FuncOutput
   Body:  DevBody  */

typedef struct {
  pwr_tUInt32                         Index;
  pwr_sPlcNode                        PlcNode;
} pwr_sdClass_FuncOutput;

#endif

#ifndef pwr_cClass_GetDap
#define pwr_cClass_GetDap 16842904


/* Class: GetDap
   Body:  RtBody  */


typedef struct {
  pwr_tFloat32                        *OutDataP;
  pwr_tObjid                          OutData_ObjId;
} pwr_sClass_GetDap;

/* Class: GetDap
   Body:  DevBody  */

typedef struct {
  pwr_tObjid                          DataObject;
  pwr_tString80                       Parameter;
  pwr_tInt32                          DataObjectSegments;
  pwr_sPlcNode                        PlcNode;
} pwr_sdClass_GetDap;

#endif

#ifndef pwr_cClass_GetObjid
#define pwr_cClass_GetObjid 16842856


/* Class: GetObjid
   Body:  RtBody  */


typedef struct {
  pwr_tObjid                          *Out;
  pwr_tObjid                          Objid;
} pwr_sClass_GetObjid;

/* Class: GetObjid
   Body:  DevBody  */

typedef struct {
  pwr_tObjid                          Object;
  pwr_tInt32                          ObjectSegments;
  pwr_sPlcNode                        PlcNode;
} pwr_sdClass_GetObjid;

#endif

#ifndef pwr_cClass_GetObjidp
#define pwr_cClass_GetObjidp 16842912


/* Class: GetObjidp
   Body:  RtBody  */


typedef struct {
  pwr_tObjid                          ObjId;
} pwr_sClass_GetObjidp;

/* Class: GetObjidp
   Body:  DevBody  */

typedef struct {
  pwr_tObjid                          Object;
  pwr_tString80                       Parameter;
  pwr_tInt32                          ObjectSegments;
  pwr_sPlcNode                        PlcNode;
} pwr_sdClass_GetObjidp;

#endif

#ifndef pwr_cClass_NMpsMirrorConfig
#define pwr_cClass_NMpsMirrorConfig 16843008


/* Class: NMpsMirrorConfig
   Body:  RtBody  */


typedef struct {
  pwr_tFloat32                        ScanTime;
  pwr_tUInt32                         MirrorCellCount;
  pwr_tUInt32                         MirrorCellInitCount;
  pwr_tUInt32                         OrigCellCount;
  pwr_tUInt32                         RemoteOrigCellCount;
  pwr_tUInt32                         OrigCellDownCount;
  pwr_tUInt32                         DataObjectCount;
  pwr_tUInt32                         ConvertDataCount;
  pwr_tUInt32                         RemoteDataCount;
  pwr_tUInt32                         RemoteDataDownCount;
  pwr_tUInt32                         PendingRemoveDataCount;
  pwr_tUInt32                         RemoveDataCount;
  pwr_tUInt32                         CreateDataCount;
  pwr_tUInt32                         DeleteDataCount;
  pwr_tUInt32                         ReconnectDataCount;
  pwr_tUInt32                         LoopCount;
  pwr_tBoolean                        Initialize;
} pwr_sClass_NMpsMirrorConfig;

#endif

#ifndef pwr_cClass_NMpsBackupConfig
#define pwr_cClass_NMpsBackupConfig 16842984


/* Class: NMpsBackupConfig
   Body:  RtBody  */


typedef struct {
  pwr_tFloat32                        FullCycleTime;
  pwr_tFloat32                        IncrementCycleTime;
  pwr_tString80                       BackupFile;
  pwr_tBoolean                        BackupOn;
  pwr_tBoolean                        NoRead;
  pwr_tTime                           LastFullBackup;
  pwr_tTime                           LastIncrementBackup;
  pwr_tUInt32                         CellCount;
  pwr_tUInt32                         DataCount;
  pwr_tUInt32                         LoopCount;
  pwr_tUInt32                         FullLoopCount;
  pwr_tUInt32                         IncrementLoopCount;
  pwr_tBoolean                        ReloadFailure;
  pwr_tUInt32                         File1OpenCount;
  pwr_tUInt32                         File1WriteErrorCount;
  pwr_tUInt32                         File2OpenCount;
  pwr_tUInt32                         File2WriteErrorCount;
  pwr_tBoolean                        Initialize;
  pwr_tBoolean                        ForceFullBackup;
} pwr_sClass_NMpsBackupConfig;

#endif

#ifndef pwr_cClass_NMpsCell
#define pwr_cClass_NMpsCell 16842760


/* Class: NMpsCell
   Body:  RtBody  */


typedef struct {
  pwr_tInt32                          In;
  pwr_tInt32                          Out;
  pwr_tUInt16                         MaxSize;
  char                                filler_0[2];
  pwr_tUInt32                         Function;
  pwr_tObjid                          ResetObject;
  pwr_tBoolean                        *ResetObjectP;
  pwr_tBoolean                        BackupNow;
  pwr_tBoolean                        ReloadDone;
  pwr_tBoolean                        ReleaseAppl;
  pwr_tBoolean                        InitTime;
  pwr_tBoolean                        BackupNowMsg;
  pwr_tBoolean                        MirrorRestart;
  pwr_tBoolean                        CellFull;
  pwr_tUInt16                         NumberOfData;
  char                                filler_1[2];
  pwr_tUInt16                         DataSelected;
  char                                filler_2[2];
  pwr_tUInt16                         LastIndex;
  char                                filler_3[2];
  pwr_tUInt16                         SelectIndex;
  char                                filler_4[2];
  pwr_tBoolean                        FrontNew;
  pwr_tBoolean                        RearNew;
  pwr_tFloat32                        *InPointer;
  pwr_tObjid                          InObjId;
  pwr_tRefId                          InDlid;
  pwr_tUInt16                         InOpType;
  char                                filler_5[2];
  pwr_tBoolean                        InFlag;
  pwr_tUInt16                         OutFrontOpType;
  char                                filler_6[2];
  pwr_tBoolean                        OutFrontFlag;
  pwr_tUInt16                         OutRearOpType;
  char                                filler_7[2];
  pwr_tBoolean                        OutRearFlag;
  pwr_tObjid                          ExternObjId;
  pwr_tUInt16                         ExternOpType;
  char                                filler_8[2];
  pwr_tBoolean                        ExternFlag;
  pwr_tUInt16                         ExternIndex;
  char                                filler_9[2];
  pwr_tInt32                          ExternStatus;
  pwr_tFloat32                        *DataLP;
  pwr_tObjid                          DataL_ObjId;
  pwr_tBoolean                        DataL_Front;
  pwr_tBoolean                        DataL_Back;
  pwr_tRefId                          DataL_Dlid;
  pwr_tBoolean                        DataL_Select;
  pwr_tBoolean                        DataL_SelectOld;
  pwr_tFloat32                        *DataLastP;
  pwr_tObjid                          DataLast_ObjId;
  pwr_tBoolean                        DataLast_Front;
  pwr_tBoolean                        DataLast_Back;
  pwr_tRefId                          DataLast_Dlid;
  pwr_tBoolean                        DataLast_Select;
  pwr_tBoolean                        DataLast_SelectOld;
  pwr_tFloat32                        *DataCurrentP;
  pwr_tObjid                          DataCurrent_ObjId;
  pwr_tBoolean                        DataCurrent_Front;
  pwr_tBoolean                        DataCurrent_Back;
  pwr_tUInt16                         Idx;
  char                                filler_10[2];
  pwr_tFloat32                        CurrentIndex;
  pwr_tFloat32                        *Data1P;
  pwr_tObjid                          Data1_ObjId;
  pwr_tBoolean                        Data1_Front;
  pwr_tBoolean                        Data1_Back;
  pwr_tRefId                          Data1_Dlid;
  pwr_tBoolean                        Data1_Select;
  pwr_tBoolean                        Data1_SelectOld;
  pwr_tFloat32                        *Data2P;
  pwr_tObjid                          Data2_ObjId;
  pwr_tBoolean                        Data2_Front;
  pwr_tBoolean                        Data2_Back;
  pwr_tRefId                          Data2_Dlid;
  pwr_tBoolean                        Data2_Select;
  pwr_tBoolean                        Data2_SelectOld;
  pwr_tFloat32                        *Data3P;
  pwr_tObjid                          Data3_ObjId;
  pwr_tBoolean                        Data3_Front;
  pwr_tBoolean                        Data3_Back;
  pwr_tRefId                          Data3_Dlid;
  pwr_tBoolean                        Data3_Select;
  pwr_tBoolean                        Data3_SelectOld;
  pwr_tFloat32                        *Data4P;
  pwr_tObjid                          Data4_ObjId;
  pwr_tBoolean                        Data4_Front;
  pwr_tBoolean                        Data4_Back;
  pwr_tRefId                          Data4_Dlid;
  pwr_tBoolean                        Data4_Select;
  pwr_tBoolean                        Data4_SelectOld;
  pwr_tFloat32                        *Data5P;
  pwr_tObjid                          Data5_ObjId;
  pwr_tBoolean                        Data5_Front;
  pwr_tBoolean                        Data5_Back;
  pwr_tRefId                          Data5_Dlid;
  pwr_tBoolean                        Data5_Select;
  pwr_tBoolean                        Data5_SelectOld;
  pwr_tFloat32                        *Data6P;
  pwr_tObjid                          Data6_ObjId;
  pwr_tBoolean                        Data6_Front;
  pwr_tBoolean                        Data6_Back;
  pwr_tRefId                          Data6_Dlid;
  pwr_tBoolean                        Data6_Select;
  pwr_tBoolean                        Data6_SelectOld;
  pwr_tFloat32                        *Data7P;
  pwr_tObjid                          Data7_ObjId;
  pwr_tBoolean                        Data7_Front;
  pwr_tBoolean                        Data7_Back;
  pwr_tRefId                          Data7_Dlid;
  pwr_tBoolean                        Data7_Select;
  pwr_tBoolean                        Data7_SelectOld;
  pwr_tFloat32                        *Data8P;
  pwr_tObjid                          Data8_ObjId;
  pwr_tBoolean                        Data8_Front;
  pwr_tBoolean                        Data8_Back;
  pwr_tRefId                          Data8_Dlid;
  pwr_tBoolean                        Data8_Select;
  pwr_tBoolean                        Data8_SelectOld;
  pwr_tFloat32                        *Data9P;
  pwr_tObjid                          Data9_ObjId;
  pwr_tBoolean                        Data9_Front;
  pwr_tBoolean                        Data9_Back;
  pwr_tRefId                          Data9_Dlid;
  pwr_tBoolean                        Data9_Select;
  pwr_tBoolean                        Data9_SelectOld;
  pwr_tFloat32                        *Data10P;
  pwr_tObjid                          Data10_ObjId;
  pwr_tBoolean                        Data10_Front;
  pwr_tBoolean                        Data10_Back;
  pwr_tRefId                          Data10_Dlid;
  pwr_tBoolean                        Data10_Select;
  pwr_tBoolean                        Data10_SelectOld;
  pwr_tFloat32                        *Data11P;
  pwr_tObjid                          Data11_ObjId;
  pwr_tBoolean                        Data11_Front;
  pwr_tBoolean                        Data11_Back;
  pwr_tRefId                          Data11_Dlid;
  pwr_tBoolean                        Data11_Select;
  pwr_tBoolean                        Data11_SelectOld;
  pwr_tFloat32                        *Data12P;
  pwr_tObjid                          Data12_ObjId;
  pwr_tBoolean                        Data12_Front;
  pwr_tBoolean                        Data12_Back;
  pwr_tRefId                          Data12_Dlid;
  pwr_tBoolean                        Data12_Select;
  pwr_tBoolean                        Data12_SelectOld;
  pwr_tFloat32                        *Data13P;
  pwr_tObjid                          Data13_ObjId;
  pwr_tBoolean                        Data13_Front;
  pwr_tBoolean                        Data13_Back;
  pwr_tRefId                          Data13_Dlid;
  pwr_tBoolean                        Data13_Select;
  pwr_tBoolean                        Data13_SelectOld;
  pwr_tFloat32                        *Data14P;
  pwr_tObjid                          Data14_ObjId;
  pwr_tBoolean                        Data14_Front;
  pwr_tBoolean                        Data14_Back;
  pwr_tRefId                          Data14_Dlid;
  pwr_tBoolean                        Data14_Select;
  pwr_tBoolean                        Data14_SelectOld;
  pwr_tFloat32                        *Data15P;
  pwr_tObjid                          Data15_ObjId;
  pwr_tBoolean                        Data15_Front;
  pwr_tBoolean                        Data15_Back;
  pwr_tRefId                          Data15_Dlid;
  pwr_tBoolean                        Data15_Select;
  pwr_tBoolean                        Data15_SelectOld;
  pwr_tFloat32                        *Data16P;
  pwr_tObjid                          Data16_ObjId;
  pwr_tBoolean                        Data16_Front;
  pwr_tBoolean                        Data16_Back;
  pwr_tRefId                          Data16_Dlid;
  pwr_tBoolean                        Data16_Select;
  pwr_tBoolean                        Data16_SelectOld;
  pwr_tFloat32                        *Data17P;
  pwr_tObjid                          Data17_ObjId;
  pwr_tBoolean                        Data17_Front;
  pwr_tBoolean                        Data17_Back;
  pwr_tRefId                          Data17_Dlid;
  pwr_tBoolean                        Data17_Select;
  pwr_tBoolean                        Data17_SelectOld;
  pwr_tFloat32                        *Data18P;
  pwr_tObjid                          Data18_ObjId;
  pwr_tBoolean                        Data18_Front;
  pwr_tBoolean                        Data18_Back;
  pwr_tRefId                          Data18_Dlid;
  pwr_tBoolean                        Data18_Select;
  pwr_tBoolean                        Data18_SelectOld;
  pwr_tFloat32                        *Data19P;
  pwr_tObjid                          Data19_ObjId;
  pwr_tBoolean                        Data19_Front;
  pwr_tBoolean                        Data19_Back;
  pwr_tRefId                          Data19_Dlid;
  pwr_tBoolean                        Data19_Select;
  pwr_tBoolean                        Data19_SelectOld;
  pwr_tFloat32                        *Data20P;
  pwr_tObjid                          Data20_ObjId;
  pwr_tBoolean                        Data20_Front;
  pwr_tBoolean                        Data20_Back;
  pwr_tRefId                          Data20_Dlid;
  pwr_tBoolean                        Data20_Select;
  pwr_tBoolean                        Data20_SelectOld;
  pwr_tFloat32                        *Data21P;
  pwr_tObjid                          Data21_ObjId;
  pwr_tBoolean                        Data21_Front;
  pwr_tBoolean                        Data21_Back;
  pwr_tRefId                          Data21_Dlid;
  pwr_tBoolean                        Data21_Select;
  pwr_tBoolean                        Data21_SelectOld;
  pwr_tFloat32                        *Data22P;
  pwr_tObjid                          Data22_ObjId;
  pwr_tBoolean                        Data22_Front;
  pwr_tBoolean                        Data22_Back;
  pwr_tRefId                          Data22_Dlid;
  pwr_tBoolean                        Data22_Select;
  pwr_tBoolean                        Data22_SelectOld;
  pwr_tFloat32                        *Data23P;
  pwr_tObjid                          Data23_ObjId;
  pwr_tBoolean                        Data23_Front;
  pwr_tBoolean                        Data23_Back;
  pwr_tRefId                          Data23_Dlid;
  pwr_tBoolean                        Data23_Select;
  pwr_tBoolean                        Data23_SelectOld;
  pwr_tFloat32                        *Data24P;
  pwr_tObjid                          Data24_ObjId;
  pwr_tBoolean                        Data24_Front;
  pwr_tBoolean                        Data24_Back;
  pwr_tRefId                          Data24_Dlid;
  pwr_tBoolean                        Data24_Select;
  pwr_tBoolean                        Data24_SelectOld;
  pwr_tFloat32                        *Data25P;
  pwr_tObjid                          Data25_ObjId;
  pwr_tBoolean                        Data25_Front;
  pwr_tBoolean                        Data25_Back;
  pwr_tRefId                          Data25_Dlid;
  pwr_tBoolean                        Data25_Select;
  pwr_tBoolean                        Data25_SelectOld;
  pwr_tFloat32                        *Data26P;
  pwr_tObjid                          Data26_ObjId;
  pwr_tBoolean                        Data26_Front;
  pwr_tBoolean                        Data26_Back;
  pwr_tRefId                          Data26_Dlid;
  pwr_tBoolean                        Data26_Select;
  pwr_tBoolean                        Data26_SelectOld;
  pwr_tFloat32                        *Data27P;
  pwr_tObjid                          Data27_ObjId;
  pwr_tBoolean                        Data27_Front;
  pwr_tBoolean                        Data27_Back;
  pwr_tRefId                          Data27_Dlid;
  pwr_tBoolean                        Data27_Select;
  pwr_tBoolean                        Data27_SelectOld;
  pwr_tFloat32                        *Data28P;
  pwr_tObjid                          Data28_ObjId;
  pwr_tBoolean                        Data28_Front;
  pwr_tBoolean                        Data28_Back;
  pwr_tRefId                          Data28_Dlid;
  pwr_tBoolean                        Data28_Select;
  pwr_tBoolean                        Data28_SelectOld;
  pwr_tFloat32                        *Data29P;
  pwr_tObjid                          Data29_ObjId;
  pwr_tBoolean                        Data29_Front;
  pwr_tBoolean                        Data29_Back;
  pwr_tRefId                          Data29_Dlid;
  pwr_tBoolean                        Data29_Select;
  pwr_tBoolean                        Data29_SelectOld;
  pwr_tFloat32                        *Data30P;
  pwr_tObjid                          Data30_ObjId;
  pwr_tBoolean                        Data30_Front;
  pwr_tBoolean                        Data30_Back;
  pwr_tRefId                          Data30_Dlid;
  pwr_tBoolean                        Data30_Select;
  pwr_tBoolean                        Data30_SelectOld;
} pwr_sClass_NMpsCell;

/* Class: NMpsCell
   Body:  DevBody  */

typedef struct {
  pwr_sPlcNode                        PlcNode;
} pwr_sdClass_NMpsCell;

#endif

#ifndef pwr_cClass_NMpsConvConfig
#define pwr_cClass_NMpsConvConfig 16843000


/* Class: NMpsConvConfig
   Body:  RtBody  */


typedef struct {
  pwr_tObjid                          InDataClass;
  pwr_tObjid                          OutDataClass;
  pwr_tObjid                          OutDataParent;
  pwr_tUInt32                         CopyOffset;
  pwr_tUInt32                         PasteOffset;
  pwr_tUInt32                         CopySize;
  pwr_tFloat32                        UpdateTime;
} pwr_sClass_NMpsConvConfig;

#endif

#ifndef pwr_cClass_NMpsMirrorCell
#define pwr_cClass_NMpsMirrorCell 16842992


/* Class: NMpsMirrorCell
   Body:  RtBody  */


typedef struct {
  pwr_tObjid                          CellObjects[10];
  pwr_tUInt32                         NumberOfCellObj;
  pwr_tBoolean                        DataCollect;
  pwr_tObjid                          CollectCell;
  pwr_tBoolean                        DataObjConv;
  pwr_tObjid                          ConvConfig;
  pwr_tFloat32                        ReleaseTime;
  pwr_tFloat32                        UpdateTime;
  pwr_tUInt16                         MaxSize;
  char                                filler_0[2];
  pwr_tUInt32                         Function;
  pwr_tBoolean                        UpdateFlag;
  pwr_tBoolean                        InitFlag;
  pwr_tObjid                          ResetObject;
  pwr_tBoolean                        *ResetObjectP;
  pwr_tBoolean                        CellFull;
  pwr_tUInt16                         LastIndex;
  char                                filler_1[2];
  pwr_tFloat32                        *DataLastP;
  pwr_tObjid                          DataLast_ObjId;
  pwr_tRefId                          DataLast_Dlid;
  pwr_tFloat32                        *DataCurrentP;
  pwr_tObjid                          DataCurrent_ObjId;
  pwr_tUInt16                         Idx;
  char                                filler_2[2];
  pwr_tFloat32                        CurrentIndex;
  pwr_tBoolean                        BackupNow;
  pwr_tBoolean                        ReloadDone;
  pwr_tBoolean                        BackupNowMsg;
  pwr_tBoolean                        MirrorRestart;
  pwr_tFloat32                        *Data1P;
  pwr_tObjid                          Data1_ObjId;
  pwr_tRefId                          Data1_Dlid;
  pwr_tFloat32                        *Data2P;
  pwr_tObjid                          Data2_ObjId;
  pwr_tRefId                          Data2_Dlid;
  pwr_tFloat32                        *Data3P;
  pwr_tObjid                          Data3_ObjId;
  pwr_tRefId                          Data3_Dlid;
  pwr_tFloat32                        *Data4P;
  pwr_tObjid                          Data4_ObjId;
  pwr_tRefId                          Data4_Dlid;
  pwr_tFloat32                        *Data5P;
  pwr_tObjid                          Data5_ObjId;
  pwr_tRefId                          Data5_Dlid;
  pwr_tFloat32                        *Data6P;
  pwr_tObjid                          Data6_ObjId;
  pwr_tRefId                          Data6_Dlid;
  pwr_tFloat32                        *Data7P;
  pwr_tObjid                          Data7_ObjId;
  pwr_tRefId                          Data7_Dlid;
  pwr_tFloat32                        *Data8P;
  pwr_tObjid                          Data8_ObjId;
  pwr_tRefId                          Data8_Dlid;
  pwr_tFloat32                        *Data9P;
  pwr_tObjid                          Data9_ObjId;
  pwr_tRefId                          Data9_Dlid;
  pwr_tFloat32                        *Data10P;
  pwr_tObjid                          Data10_ObjId;
  pwr_tRefId                          Data10_Dlid;
  pwr_tFloat32                        *Data11P;
  pwr_tObjid                          Data11_ObjId;
  pwr_tRefId                          Data11_Dlid;
  pwr_tFloat32                        *Data12P;
  pwr_tObjid                          Data12_ObjId;
  pwr_tRefId                          Data12_Dlid;
  pwr_tFloat32                        *Data13P;
  pwr_tObjid                          Data13_ObjId;
  pwr_tRefId                          Data13_Dlid;
  pwr_tFloat32                        *Data14P;
  pwr_tObjid                          Data14_ObjId;
  pwr_tRefId                          Data14_Dlid;
  pwr_tFloat32                        *Data15P;
  pwr_tObjid                          Data15_ObjId;
  pwr_tRefId                          Data15_Dlid;
  pwr_tFloat32                        *Data16P;
  pwr_tObjid                          Data16_ObjId;
  pwr_tRefId                          Data16_Dlid;
  pwr_tFloat32                        *Data17P;
  pwr_tObjid                          Data17_ObjId;
  pwr_tRefId                          Data17_Dlid;
  pwr_tFloat32                        *Data18P;
  pwr_tObjid                          Data18_ObjId;
  pwr_tRefId                          Data18_Dlid;
  pwr_tFloat32                        *Data19P;
  pwr_tObjid                          Data19_ObjId;
  pwr_tRefId                          Data19_Dlid;
  pwr_tFloat32                        *Data20P;
  pwr_tObjid                          Data20_ObjId;
  pwr_tRefId                          Data20_Dlid;
  pwr_tFloat32                        *Data21P;
  pwr_tObjid                          Data21_ObjId;
  pwr_tRefId                          Data21_Dlid;
  pwr_tFloat32                        *Data22P;
  pwr_tObjid                          Data22_ObjId;
  pwr_tRefId                          Data22_Dlid;
  pwr_tFloat32                        *Data23P;
  pwr_tObjid                          Data23_ObjId;
  pwr_tRefId                          Data23_Dlid;
  pwr_tFloat32                        *Data24P;
  pwr_tObjid                          Data24_ObjId;
  pwr_tRefId                          Data24_Dlid;
  pwr_tFloat32                        *Data25P;
  pwr_tObjid                          Data25_ObjId;
  pwr_tRefId                          Data25_Dlid;
  pwr_tFloat32                        *Data26P;
  pwr_tObjid                          Data26_ObjId;
  pwr_tRefId                          Data26_Dlid;
  pwr_tFloat32                        *Data27P;
  pwr_tObjid                          Data27_ObjId;
  pwr_tRefId                          Data27_Dlid;
  pwr_tFloat32                        *Data28P;
  pwr_tObjid                          Data28_ObjId;
  pwr_tRefId                          Data28_Dlid;
  pwr_tFloat32                        *Data29P;
  pwr_tObjid                          Data29_ObjId;
  pwr_tRefId                          Data29_Dlid;
  pwr_tFloat32                        *Data30P;
  pwr_tObjid                          Data30_ObjId;
  pwr_tRefId                          Data30_Dlid;
  pwr_tUInt16                         TempLastIndex;
  char                                filler_3[2];
  pwr_tUInt32                         TempArea[120];
} pwr_sClass_NMpsMirrorCell;

/* Class: NMpsMirrorCell
   Body:  DevBody  */

typedef struct {
  pwr_sPlcNode                        PlcNode;
} pwr_sdClass_NMpsMirrorCell;

#endif

#ifndef pwr_cClass_NMpsOutCell
#define pwr_cClass_NMpsOutCell 16842776


/* Class: NMpsOutCell
   Body:  RtBody  */


typedef struct {
  pwr_tInt32                          In;
  pwr_tInt32                          Out;
  pwr_tUInt16                         MaxSize;
  char                                filler_0[2];
  pwr_tUInt32                         Function;
  pwr_tObjid                          ResetObject;
  pwr_tBoolean                        *ResetObjectP;
  pwr_tBoolean                        BackupNow;
  pwr_tBoolean                        ReloadDone;
  pwr_tBoolean                        ReleaseAppl;
  pwr_tBoolean                        InitTime;
  pwr_tBoolean                        BackupNowMsg;
  pwr_tBoolean                        MirrorRestart;
  pwr_tBoolean                        CellFull;
  pwr_tUInt16                         NumberOfData;
  char                                filler_1[2];
  pwr_tUInt16                         DataSelected;
  char                                filler_2[2];
  pwr_tUInt16                         LastIndex;
  char                                filler_3[2];
  pwr_tUInt16                         SelectIndex;
  char                                filler_4[2];
  pwr_tBoolean                        FrontNew;
  pwr_tBoolean                        RearNew;
  pwr_tFloat32                        *InPointer;
  pwr_tObjid                          InObjId;
  pwr_tRefId                          InDlid;
  pwr_tUInt16                         InOpType;
  char                                filler_5[2];
  pwr_tBoolean                        InFlag;
  pwr_tUInt16                         OutFrontOpType;
  char                                filler_6[2];
  pwr_tBoolean                        OutFrontFlag;
  pwr_tUInt16                         OutRearOpType;
  char                                filler_7[2];
  pwr_tBoolean                        OutRearFlag;
  pwr_tObjid                          ExternObjId;
  pwr_tUInt16                         ExternOpType;
  char                                filler_8[2];
  pwr_tBoolean                        ExternFlag;
  pwr_tUInt16                         ExternIndex;
  char                                filler_9[2];
  pwr_tInt32                          ExternStatus;
  pwr_tFloat32                        *DataLP;
  pwr_tObjid                          DataL_ObjId;
  pwr_tBoolean                        DataL_Front;
  pwr_tBoolean                        DataL_Back;
  pwr_tRefId                          DataL_Dlid;
  pwr_tBoolean                        DataL_Select;
  pwr_tBoolean                        DataL_SelectOld;
  pwr_tFloat32                        *DataLastP;
  pwr_tObjid                          DataLast_ObjId;
  pwr_tBoolean                        DataLast_Front;
  pwr_tBoolean                        DataLast_Back;
  pwr_tRefId                          DataLast_Dlid;
  pwr_tBoolean                        DataLast_Select;
  pwr_tBoolean                        DataLast_SelectOld;
  pwr_tFloat32                        *DataCurrentP;
  pwr_tObjid                          DataCurrent_ObjId;
  pwr_tBoolean                        DataCurrent_Front;
  pwr_tBoolean                        DataCurrent_Back;
  pwr_tUInt16                         Idx;
  char                                filler_10[2];
  pwr_tFloat32                        CurrentIndex;
  pwr_tFloat32                        *Data1P;
  pwr_tObjid                          Data1_ObjId;
  pwr_tBoolean                        Data1_Front;
  pwr_tBoolean                        Data1_Back;
  pwr_tRefId                          Data1_Dlid;
  pwr_tBoolean                        Data1_Select;
  pwr_tBoolean                        Data1_SelectOld;
} pwr_sClass_NMpsOutCell;

/* Class: NMpsOutCell
   Body:  DevBody  */

typedef struct {
  pwr_sPlcNode                        PlcNode;
} pwr_sdClass_NMpsOutCell;

#endif

#ifndef pwr_cClass_NMpsStoreCell
#define pwr_cClass_NMpsStoreCell 16842768


/* Class: NMpsStoreCell
   Body:  RtBody  */


typedef struct {
  pwr_tInt32                          In;
  pwr_tInt32                          Out;
  pwr_tUInt16                         MaxSize;
  char                                filler_0[2];
  pwr_tUInt32                         Function;
  pwr_tObjid                          ResetObject;
  pwr_tBoolean                        *ResetObjectP;
  pwr_tBoolean                        BackupNow;
  pwr_tBoolean                        ReloadDone;
  pwr_tBoolean                        ReleaseAppl;
  pwr_tBoolean                        InitTime;
  pwr_tBoolean                        BackupNowMsg;
  pwr_tBoolean                        MirrorRestart;
  pwr_tBoolean                        CellFull;
  pwr_tUInt16                         NumberOfData;
  char                                filler_1[2];
  pwr_tUInt16                         DataSelected;
  char                                filler_2[2];
  pwr_tUInt16                         LastIndex;
  char                                filler_3[2];
  pwr_tUInt16                         SelectIndex;
  char                                filler_4[2];
  pwr_tBoolean                        FrontNew;
  pwr_tBoolean                        RearNew;
  pwr_tFloat32                        *InPointer;
  pwr_tObjid                          InObjId;
  pwr_tRefId                          InDlid;
  pwr_tUInt16                         InOpType;
  char                                filler_5[2];
  pwr_tBoolean                        InFlag;
  pwr_tUInt16                         OutFrontOpType;
  char                                filler_6[2];
  pwr_tBoolean                        OutFrontFlag;
  pwr_tUInt16                         OutRearOpType;
  char                                filler_7[2];
  pwr_tBoolean                        OutRearFlag;
  pwr_tObjid                          ExternObjId;
  pwr_tUInt16                         ExternOpType;
  char                                filler_8[2];
  pwr_tBoolean                        ExternFlag;
  pwr_tUInt16                         ExternIndex;
  char                                filler_9[2];
  pwr_tInt32                          ExternStatus;
  pwr_tFloat32                        *DataLP;
  pwr_tObjid                          DataL_ObjId;
  pwr_tBoolean                        DataL_Front;
  pwr_tBoolean                        DataL_Back;
  pwr_tRefId                          DataL_Dlid;
  pwr_tBoolean                        DataL_Select;
  pwr_tBoolean                        DataL_SelectOld;
  pwr_tFloat32                        *DataLastP;
  pwr_tObjid                          DataLast_ObjId;
  pwr_tBoolean                        DataLast_Front;
  pwr_tBoolean                        DataLast_Back;
  pwr_tRefId                          DataLast_Dlid;
  pwr_tBoolean                        DataLast_Select;
  pwr_tBoolean                        DataLast_SelectOld;
  pwr_tFloat32                        *DataCurrentP;
  pwr_tObjid                          DataCurrent_ObjId;
  pwr_tBoolean                        DataCurrent_Front;
  pwr_tBoolean                        DataCurrent_Back;
  pwr_tUInt16                         Idx;
  char                                filler_10[2];
  pwr_tFloat32                        CurrentIndex;
  pwr_tFloat32                        *Data1P;
  pwr_tObjid                          Data1_ObjId;
  pwr_tBoolean                        Data1_Front;
  pwr_tBoolean                        Data1_Back;
  pwr_tRefId                          Data1_Dlid;
  pwr_tBoolean                        Data1_Select;
  pwr_tBoolean                        Data1_SelectOld;
  pwr_tFloat32                        *Data2P;
  pwr_tObjid                          Data2_ObjId;
  pwr_tBoolean                        Data2_Front;
  pwr_tBoolean                        Data2_Back;
  pwr_tRefId                          Data2_Dlid;
  pwr_tBoolean                        Data2_Select;
  pwr_tBoolean                        Data2_SelectOld;
  pwr_tFloat32                        *Data3P;
  pwr_tObjid                          Data3_ObjId;
  pwr_tBoolean                        Data3_Front;
  pwr_tBoolean                        Data3_Back;
  pwr_tRefId                          Data3_Dlid;
  pwr_tBoolean                        Data3_Select;
  pwr_tBoolean                        Data3_SelectOld;
  pwr_tFloat32                        *Data4P;
  pwr_tObjid                          Data4_ObjId;
  pwr_tBoolean                        Data4_Front;
  pwr_tBoolean                        Data4_Back;
  pwr_tRefId                          Data4_Dlid;
  pwr_tBoolean                        Data4_Select;
  pwr_tBoolean                        Data4_SelectOld;
  pwr_tFloat32                        *Data5P;
  pwr_tObjid                          Data5_ObjId;
  pwr_tBoolean                        Data5_Front;
  pwr_tBoolean                        Data5_Back;
  pwr_tRefId                          Data5_Dlid;
  pwr_tBoolean                        Data5_Select;
  pwr_tBoolean                        Data5_SelectOld;
  pwr_tFloat32                        *Data6P;
  pwr_tObjid                          Data6_ObjId;
  pwr_tBoolean                        Data6_Front;
  pwr_tBoolean                        Data6_Back;
  pwr_tRefId                          Data6_Dlid;
  pwr_tBoolean                        Data6_Select;
  pwr_tBoolean                        Data6_SelectOld;
  pwr_tFloat32                        *Data7P;
  pwr_tObjid                          Data7_ObjId;
  pwr_tBoolean                        Data7_Front;
  pwr_tBoolean                        Data7_Back;
  pwr_tRefId                          Data7_Dlid;
  pwr_tBoolean                        Data7_Select;
  pwr_tBoolean                        Data7_SelectOld;
  pwr_tFloat32                        *Data8P;
  pwr_tObjid                          Data8_ObjId;
  pwr_tBoolean                        Data8_Front;
  pwr_tBoolean                        Data8_Back;
  pwr_tRefId                          Data8_Dlid;
  pwr_tBoolean                        Data8_Select;
  pwr_tBoolean                        Data8_SelectOld;
  pwr_tFloat32                        *Data9P;
  pwr_tObjid                          Data9_ObjId;
  pwr_tBoolean                        Data9_Front;
  pwr_tBoolean                        Data9_Back;
  pwr_tRefId                          Data9_Dlid;
  pwr_tBoolean                        Data9_Select;
  pwr_tBoolean                        Data9_SelectOld;
  pwr_tFloat32                        *Data10P;
  pwr_tObjid                          Data10_ObjId;
  pwr_tBoolean                        Data10_Front;
  pwr_tBoolean                        Data10_Back;
  pwr_tRefId                          Data10_Dlid;
  pwr_tBoolean                        Data10_Select;
  pwr_tBoolean                        Data10_SelectOld;
  pwr_tFloat32                        *Data11P;
  pwr_tObjid                          Data11_ObjId;
  pwr_tBoolean                        Data11_Front;
  pwr_tBoolean                        Data11_Back;
  pwr_tRefId                          Data11_Dlid;
  pwr_tBoolean                        Data11_Select;
  pwr_tBoolean                        Data11_SelectOld;
  pwr_tFloat32                        *Data12P;
  pwr_tObjid                          Data12_ObjId;
  pwr_tBoolean                        Data12_Front;
  pwr_tBoolean                        Data12_Back;
  pwr_tRefId                          Data12_Dlid;
  pwr_tBoolean                        Data12_Select;
  pwr_tBoolean                        Data12_SelectOld;
  pwr_tFloat32                        *Data13P;
  pwr_tObjid                          Data13_ObjId;
  pwr_tBoolean                        Data13_Front;
  pwr_tBoolean                        Data13_Back;
  pwr_tRefId                          Data13_Dlid;
  pwr_tBoolean                        Data13_Select;
  pwr_tBoolean                        Data13_SelectOld;
  pwr_tFloat32                        *Data14P;
  pwr_tObjid                          Data14_ObjId;
  pwr_tBoolean                        Data14_Front;
  pwr_tBoolean                        Data14_Back;
  pwr_tRefId                          Data14_Dlid;
  pwr_tBoolean                        Data14_Select;
  pwr_tBoolean                        Data14_SelectOld;
  pwr_tFloat32                        *Data15P;
  pwr_tObjid                          Data15_ObjId;
  pwr_tBoolean                        Data15_Front;
  pwr_tBoolean                        Data15_Back;
  pwr_tRefId                          Data15_Dlid;
  pwr_tBoolean                        Data15_Select;
  pwr_tBoolean                        Data15_SelectOld;
  pwr_tFloat32                        *Data16P;
  pwr_tObjid                          Data16_ObjId;
  pwr_tBoolean                        Data16_Front;
  pwr_tBoolean                        Data16_Back;
  pwr_tRefId                          Data16_Dlid;
  pwr_tBoolean                        Data16_Select;
  pwr_tBoolean                        Data16_SelectOld;
  pwr_tFloat32                        *Data17P;
  pwr_tObjid                          Data17_ObjId;
  pwr_tBoolean                        Data17_Front;
  pwr_tBoolean                        Data17_Back;
  pwr_tRefId                          Data17_Dlid;
  pwr_tBoolean                        Data17_Select;
  pwr_tBoolean                        Data17_SelectOld;
  pwr_tFloat32                        *Data18P;
  pwr_tObjid                          Data18_ObjId;
  pwr_tBoolean                        Data18_Front;
  pwr_tBoolean                        Data18_Back;
  pwr_tRefId                          Data18_Dlid;
  pwr_tBoolean                        Data18_Select;
  pwr_tBoolean                        Data18_SelectOld;
  pwr_tFloat32                        *Data19P;
  pwr_tObjid                          Data19_ObjId;
  pwr_tBoolean                        Data19_Front;
  pwr_tBoolean                        Data19_Back;
  pwr_tRefId                          Data19_Dlid;
  pwr_tBoolean                        Data19_Select;
  pwr_tBoolean                        Data19_SelectOld;
  pwr_tFloat32                        *Data20P;
  pwr_tObjid                          Data20_ObjId;
  pwr_tBoolean                        Data20_Front;
  pwr_tBoolean                        Data20_Back;
  pwr_tRefId                          Data20_Dlid;
  pwr_tBoolean                        Data20_Select;
  pwr_tBoolean                        Data20_SelectOld;
  pwr_tFloat32                        *Data21P;
  pwr_tObjid                          Data21_ObjId;
  pwr_tBoolean                        Data21_Front;
  pwr_tBoolean                        Data21_Back;
  pwr_tRefId                          Data21_Dlid;
  pwr_tBoolean                        Data21_Select;
  pwr_tBoolean                        Data21_SelectOld;
  pwr_tFloat32                        *Data22P;
  pwr_tObjid                          Data22_ObjId;
  pwr_tBoolean                        Data22_Front;
  pwr_tBoolean                        Data22_Back;
  pwr_tRefId                          Data22_Dlid;
  pwr_tBoolean                        Data22_Select;
  pwr_tBoolean                        Data22_SelectOld;
  pwr_tFloat32                        *Data23P;
  pwr_tObjid                          Data23_ObjId;
  pwr_tBoolean                        Data23_Front;
  pwr_tBoolean                        Data23_Back;
  pwr_tRefId                          Data23_Dlid;
  pwr_tBoolean                        Data23_Select;
  pwr_tBoolean                        Data23_SelectOld;
  pwr_tFloat32                        *Data24P;
  pwr_tObjid                          Data24_ObjId;
  pwr_tBoolean                        Data24_Front;
  pwr_tBoolean                        Data24_Back;
  pwr_tRefId                          Data24_Dlid;
  pwr_tBoolean                        Data24_Select;
  pwr_tBoolean                        Data24_SelectOld;
  pwr_tFloat32                        *Data25P;
  pwr_tObjid                          Data25_ObjId;
  pwr_tBoolean                        Data25_Front;
  pwr_tBoolean                        Data25_Back;
  pwr_tRefId                          Data25_Dlid;
  pwr_tBoolean                        Data25_Select;
  pwr_tBoolean                        Data25_SelectOld;
  pwr_tFloat32                        *Data26P;
  pwr_tObjid                          Data26_ObjId;
  pwr_tBoolean                        Data26_Front;
  pwr_tBoolean                        Data26_Back;
  pwr_tRefId                          Data26_Dlid;
  pwr_tBoolean                        Data26_Select;
  pwr_tBoolean                        Data26_SelectOld;
  pwr_tFloat32                        *Data27P;
  pwr_tObjid                          Data27_ObjId;
  pwr_tBoolean                        Data27_Front;
  pwr_tBoolean                        Data27_Back;
  pwr_tRefId                          Data27_Dlid;
  pwr_tBoolean                        Data27_Select;
  pwr_tBoolean                        Data27_SelectOld;
  pwr_tFloat32                        *Data28P;
  pwr_tObjid                          Data28_ObjId;
  pwr_tBoolean                        Data28_Front;
  pwr_tBoolean                        Data28_Back;
  pwr_tRefId                          Data28_Dlid;
  pwr_tBoolean                        Data28_Select;
  pwr_tBoolean                        Data28_SelectOld;
  pwr_tFloat32                        *Data29P;
  pwr_tObjid                          Data29_ObjId;
  pwr_tBoolean                        Data29_Front;
  pwr_tBoolean                        Data29_Back;
  pwr_tRefId                          Data29_Dlid;
  pwr_tBoolean                        Data29_Select;
  pwr_tBoolean                        Data29_SelectOld;
  pwr_tFloat32                        *Data30P;
  pwr_tObjid                          Data30_ObjId;
  pwr_tBoolean                        Data30_Front;
  pwr_tBoolean                        Data30_Back;
  pwr_tRefId                          Data30_Dlid;
  pwr_tBoolean                        Data30_Select;
  pwr_tBoolean                        Data30_SelectOld;
} pwr_sClass_NMpsStoreCell;

/* Class: NMpsStoreCell
   Body:  DevBody  */

typedef struct {
  pwr_sPlcNode                        PlcNode;
} pwr_sdClass_NMpsStoreCell;

#endif

#ifndef pwr_cClass_NMpsTransConfig
#define pwr_cClass_NMpsTransConfig 16843096


/* Class: NMpsTransConfig
   Body:  RtBody  */


typedef struct {
  pwr_tFloat32                        CycleTime;
  pwr_tInt32                          RequestObjects;
  pwr_tInt32                          RequestSent;
  pwr_tInt32                          RequestReceived;
  pwr_tBoolean                        Initialize;
  pwr_tUInt32                         LoopCount;
} pwr_sClass_NMpsTransConfig;

#endif

#ifndef pwr_cClass_NMpsTrp
#define pwr_cClass_NMpsTrp 16842784


/* Class: NMpsTrp
   Body:  RtBody  */


typedef struct {
  pwr_tInt32                          *InP;
  pwr_tInt32                          In;
  pwr_tInt32                          *OutP;
  pwr_tInt32                          Out;
  pwr_tBoolean                        *TriggForwFrontP;
  pwr_tBoolean                        TriggForwFront;
  pwr_tBoolean                        *TriggForwBackP;
  pwr_tBoolean                        TriggForwBack;
  pwr_tBoolean                        *TriggReverseFrontP;
  pwr_tBoolean                        TriggReverseFront;
  pwr_tBoolean                        *TriggReverseBackP;
  pwr_tBoolean                        TriggReverseBack;
  pwr_tBoolean                        Status;
  pwr_tUInt32                         Function;
  pwr_tBoolean                        OldTriggForwFront;
  pwr_tBoolean                        OldTriggForwBack;
  pwr_tBoolean                        OldTriggReverseFront;
  pwr_tBoolean                        OldTriggReverseBack;
  pwr_tBoolean                        FuncAllForward;
  pwr_tBoolean                        FuncAllReverse;
} pwr_sClass_NMpsTrp;

/* Class: NMpsTrp
   Body:  DevBody  */

typedef struct {
  pwr_sPlcNode                        PlcNode;
} pwr_sdClass_NMpsTrp;

#endif

#ifndef pwr_cClass_NMpsTrpFF
#define pwr_cClass_NMpsTrpFF 16842800


/* Class: NMpsTrpFF
   Body:  RtBody  */


typedef struct {
  pwr_tInt32                          *InP;
  pwr_tInt32                          In;
  pwr_tInt32                          *OutP;
  pwr_tInt32                          Out;
  pwr_tBoolean                        *TriggForwFrontP;
  pwr_tBoolean                        TriggForwFront;
  pwr_tBoolean                        *TriggForwBackP;
  pwr_tBoolean                        TriggForwBack;
  pwr_tBoolean                        *TriggReverseFrontP;
  pwr_tBoolean                        TriggReverseFront;
  pwr_tBoolean                        *TriggReverseBackP;
  pwr_tBoolean                        TriggReverseBack;
  pwr_tBoolean                        Status;
  pwr_tUInt32                         Function;
  pwr_tBoolean                        OldTriggForwFront;
  pwr_tBoolean                        OldTriggForwBack;
  pwr_tBoolean                        OldTriggReverseFront;
  pwr_tBoolean                        OldTriggReverseBack;
  pwr_tBoolean                        FuncAllForward;
  pwr_tBoolean                        FuncAllReverse;
} pwr_sClass_NMpsTrpFF;

/* Class: NMpsTrpFF
   Body:  DevBody  */

typedef struct {
  pwr_sPlcNode                        PlcNode;
} pwr_sdClass_NMpsTrpFF;

#endif

#ifndef pwr_cClass_NMpsTrpRR
#define pwr_cClass_NMpsTrpRR 16842792


/* Class: NMpsTrpRR
   Body:  RtBody  */


typedef struct {
  pwr_tInt32                          *InP;
  pwr_tInt32                          In;
  pwr_tInt32                          *OutP;
  pwr_tInt32                          Out;
  pwr_tBoolean                        *TriggForwFrontP;
  pwr_tBoolean                        TriggForwFront;
  pwr_tBoolean                        *TriggForwBackP;
  pwr_tBoolean                        TriggForwBack;
  pwr_tBoolean                        *TriggReverseFrontP;
  pwr_tBoolean                        TriggReverseFront;
  pwr_tBoolean                        *TriggReverseBackP;
  pwr_tBoolean                        TriggReverseBack;
  pwr_tBoolean                        Status;
  pwr_tUInt32                         Function;
  pwr_tBoolean                        OldTriggForwFront;
  pwr_tBoolean                        OldTriggForwBack;
  pwr_tBoolean                        OldTriggReverseFront;
  pwr_tBoolean                        OldTriggReverseBack;
  pwr_tBoolean                        FuncAllForward;
  pwr_tBoolean                        FuncAllReverse;
} pwr_sClass_NMpsTrpRR;

/* Class: NMpsTrpRR
   Body:  DevBody  */

typedef struct {
  pwr_sPlcNode                        PlcNode;
} pwr_sdClass_NMpsTrpRR;

#endif

#ifndef pwr_cClass_RemTransRcv
#define pwr_cClass_RemTransRcv 16842976


/* Class: RemTransRcv
   Body:  RtBody  */


typedef struct {
  pwr_tFloat32                        **RemTransP;
  pwr_tFloat32                        *RemTrans;
  pwr_tBoolean                        Received;
} pwr_sClass_RemTransRcv;

/* Class: RemTransRcv
   Body:  DevBody  */

typedef struct {
  pwr_sPlcNode                        PlcNode;
} pwr_sdClass_RemTransRcv;

#endif

#ifndef pwr_cClass_RemTransSend
#define pwr_cClass_RemTransSend 16842968


/* Class: RemTransSend
   Body:  RtBody  */


typedef struct {
  pwr_tFloat32                        **RemTransP;
  pwr_tFloat32                        *RemTrans;
  pwr_tBoolean                        *SendP;
  pwr_tBoolean                        Send;
  pwr_tBoolean                        Occupied;
  pwr_tBoolean                        Buffer;
  pwr_tBoolean                        Error;
} pwr_sClass_RemTransSend;

/* Class: RemTransSend
   Body:  DevBody  */

typedef struct {
  pwr_sPlcNode                        PlcNode;
} pwr_sdClass_RemTransSend;

#endif

#ifndef pwr_cClass_RunningTime
#define pwr_cClass_RunningTime 16843128


/* Class: RunningTime
   Body:  RtBody  */


typedef struct {
  pwr_tBoolean                        *RunningP;
  pwr_tBoolean                        Running;
  pwr_tBoolean                        Start;
  pwr_tString40                       Description;
  pwr_tBoolean                        TripReset;
  pwr_tTime                           StartTime;
  pwr_tTime                           ResetTime;
  pwr_tUInt32                         TotalNOfStarts;
  pwr_tUInt32                         TotalRunHours;
  pwr_tFloat32                        TotalRunSeconds;
  pwr_tUInt32                         TotalHours;
  pwr_tFloat32                        TotalSeconds;
  pwr_tFloat32                        TotalUsage;
  pwr_tUInt32                         TripNOfStarts;
  pwr_tUInt32                         TripRunHours;
  pwr_tFloat32                        TripRunSeconds;
  pwr_tUInt32                         TripHours;
  pwr_tFloat32                        TripSeconds;
  pwr_tFloat32                        TripUsage;
  pwr_tUInt32                         OldTripNOfStarts;
  pwr_tUInt32                         OldTripRunHours;
  pwr_tFloat32                        OldTripRunSeconds;
  pwr_tUInt32                         OldTripHours;
  pwr_tFloat32                        OldTripSeconds;
  pwr_tFloat32                        OldTripUsage;
  pwr_tFloat32                        *ScanTime;
} pwr_sClass_RunningTime;

/* Class: RunningTime
   Body:  DevBody  */

typedef struct {
  pwr_sPlcNode                        PlcNode;
} pwr_sdClass_RunningTime;

#endif

#endif
