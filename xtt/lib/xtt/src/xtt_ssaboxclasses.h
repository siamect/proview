/*	xtt_ssaboxclasses.h */

/*	Note !!!. */
/*	These classes are copied from pwr_ssaboxclasses.h and has to be */
/*        updated when these classes are modified. */

#ifndef xtt_ssaboxclasses_h
#define xtt_ssaboxclasses_h

#ifndef pwr_class_h
#include "pwr_class.h"
#endif

#define pwr_cClass_Ssab_DI32D 4194631736UL

#define pwr_cClass_Ssab_DO32DKS 4194631744UL

#define pwr_cClass_Ssab_DO32DKS_Stall 4194631784UL

#ifndef pwr_cClass_Ssab_BaseDiCard
#define pwr_cClass_Ssab_BaseDiCard 4194631704UL

typedef struct {
  pwr_tString80                       Description pwr_dAlignLW;
  pwr_tString40                       DevName pwr_dAlignW;
  pwr_tUInt32                         ErrorCount pwr_dAlignW;
  pwr_tUInt32                         ErrorSoftLimit pwr_dAlignW;
  pwr_tUInt32                         ErrorHardLimit pwr_dAlignW;
  pwr_tUInt16                         MaxNoOfChannels pwr_dAlignW;
  pwr_tUInt32                         RegAddress pwr_dAlignW;
  pwr_tUInt32                         VectAddress pwr_dAlignW;
  pwr_tIoProcessMask                  Process pwr_dAlignW;
  pwr_tObjid                          ThreadObject pwr_dAlignW;
  pwr_tURL                            DataSheet pwr_dAlignW;
  pwr_tCardMask1_1                    ConvMask1 pwr_dAlignW;
  pwr_tCardMask2_1                    ConvMask2 pwr_dAlignW;
  pwr_tCardMask1_1                    InvMask1 pwr_dAlignW;
  pwr_tCardMask2_1                    InvMask2 pwr_dAlignW;
} pwr_sClass_Ssab_BaseDiCard;

#endif


#ifndef pwr_cClass_Ssab_BaseDoCard
#define pwr_cClass_Ssab_BaseDoCard 4194631712UL

typedef struct {
  pwr_tString80                       Description pwr_dAlignLW;
  pwr_tString40                       DevName pwr_dAlignW;
  pwr_tUInt32                         ErrorCount pwr_dAlignW;
  pwr_tUInt32                         ErrorSoftLimit pwr_dAlignW;
  pwr_tUInt32                         ErrorHardLimit pwr_dAlignW;
  pwr_tUInt16                         MaxNoOfChannels pwr_dAlignW;
  pwr_tUInt32                         RegAddress pwr_dAlignW;
  pwr_tUInt32                         VectAddress pwr_dAlignW;
  pwr_tIoProcessMask                  Process pwr_dAlignW;
  pwr_tObjid                          ThreadObject pwr_dAlignW;
  pwr_tURL                            DataSheet pwr_dAlignW;
  pwr_tCardMask1_1                    InvMask1 pwr_dAlignW;
  pwr_tCardMask2_1                    InvMask2 pwr_dAlignW;
  pwr_tCardMask1_1                    TestMask1 pwr_dAlignW;
  pwr_tCardMask2_1                    TestMask2 pwr_dAlignW;
  pwr_tCardMask1_1                    TestValue1 pwr_dAlignW;
  pwr_tCardMask2_1                    TestValue2 pwr_dAlignW;
  pwr_tCardMask1_1                    FixedOutValue1 pwr_dAlignW;
  pwr_tCardMask2_1                    FixedOutValue2 pwr_dAlignW;
} pwr_sClass_Ssab_BaseDoCard;

#endif

#endif
