/**
 * Title:	<p>
 * Description:	<p>
 * Copyright:	<p>
 * Company	SSAB<p>
 * @author	CS
 * @version	1.0
 */

package jpwr.rt;

public class Pwr {
  public static final int eTix_Boolean 		= 1;
  public static final int eTix_Float32 		= 2;
  public static final int eTix_Float64 		= 3;
  public static final int eTix_Char 		= 4;
  public static final int eTix_Int8 		= 5;
  public static final int eTix_Int16 		= 6;
  public static final int eTix_Int32 		= 7;
  public static final int eTix_UInt8 		= 8;
  public static final int eTix_UInt16 		= 9;
  public static final int eTix_UInt32 		= 10;
  public static final int eTix_Objid 		= 11;
  public static final int eTix_Buffer 		= 12;
  public static final int eTix_String 		= 13;
  public static final int eTix_Enum		= 14;
  public static final int eTix_Struct 		= 15;
  public static final int eTix_Mask 		= 16;
  public static final int eTix_Array 		= 17;
  public static final int eTix_Time 		= 18;
  public static final int eTix_Text 		= 19;
  public static final int eTix_AttrRef 		= 20;
  public static final int eTix_UInt64 		= 21;
  public static final int eTix_Int64 		= 22;
  public static final int eTix_ClassId 		= 23;
  public static final int eTix_TypeId 		= 24;
  public static final int eTix_VolumeId 	= 25;
  public static final int eTix_ObjectIx 	= 26;
  public static final int eTix_RefId 		= 27;
  public static final int eTix_DeltaTime 	= 28;
  public static final int eTix_Status	 	= 29;
  public static final int eTix_NetStatus 	= 30;
  public static final int eType_Unknown		= 0;
  public static final int eType_Boolean		= (1 << 16) + (1 << 15) + eTix_Boolean;
  public static final int eType_Float32		= (1 << 16) + (1 << 15) + eTix_Float32;
  public static final int eType_Float64		= (1 << 16) + (1 << 15) + eTix_Float64;
  public static final int eType_Char		= (1 << 16) + (1 << 15) + eTix_Char;
  public static final int eType_Int8		= (1 << 16) + (1 << 15) + eTix_Int8;
  public static final int eType_Int16		= (1 << 16) + (1 << 15) + eTix_Int16;
  public static final int eType_Int32		= (1 << 16) + (1 << 15) + eTix_Int32;
  public static final int eType_UInt8		= (1 << 16) + (1 << 15) + eTix_UInt8;
  public static final int eType_UInt16		= (1 << 16) + (1 << 15) + eTix_UInt16;
  public static final int eType_UInt32		= (1 << 16) + (1 << 15) + eTix_UInt32;
  public static final int eType_Objid		= (1 << 16) + (1 << 15) + eTix_Objid;
  public static final int eType_Buffer		= (1 << 16) + (1 << 15) + eTix_Buffer;
  public static final int eType_String		= (1 << 16) + (1 << 15) + eTix_String;
  public static final int eType_Enum		= (1 << 16) + (1 << 15) + eTix_Enum;
  public static final int eType_Struct		= (1 << 16) + (1 << 15) + eTix_Struct;
  public static final int eType_Mask		= (1 << 16) + (1 << 15) + eTix_Mask;
  public static final int eType_Array		= (1 << 16) + (1 << 15) + eTix_Array;
  public static final int eType_Time		= (1 << 16) + (1 << 15) + eTix_Time;
  public static final int eType_Text		= (1 << 16) + (1 << 15) + eTix_Text;
  public static final int eType_AttrRef		= (1 << 16) + (1 << 15) + eTix_AttrRef;
  public static final int eType_UInt64		= (1 << 16) + (1 << 15) + eTix_UInt64;
  public static final int eType_Int64		= (1 << 16) + (1 << 15) + eTix_Int64;
  public static final int eType_ClassId		= (1 << 16) + (1 << 15) + eTix_ClassId;
  public static final int eType_TypeId		= (1 << 16) + (1 << 15) + eTix_TypeId;
  public static final int eType_VolumeId	= (1 << 16) + (1 << 15) + eTix_VolumeId;
  public static final int eType_ObjectIx	= (1 << 16) + (1 << 15) + eTix_ObjectIx;
  public static final int eType_RefId		= (1 << 16) + (1 << 15) + eTix_RefId;
  public static final int eType_DeltaTime	= (1 << 16) + (1 << 15) + eTix_DeltaTime;
  public static final int eType_Status		= (1 << 16) + (1 << 15) + eTix_Status;
  public static final int eType_NetStatus	= (1 << 16) + (1 << 15) + eTix_NetStatus;

  public static final int mPrv_RtRead		= 1 << 0;
  public static final int mPrv_RtWrite		= 1 << 1;
  public static final int mPrv_System		= 1 << 2;
  public static final int mPrv_Maintenance	= 1 << 3;
  public static final int mPrv_Process		= 1 << 4;
  public static final int mPrv_Instrument	= 1 << 5;
  public static final int mPrv_Operator1	= 1 << 6;
  public static final int mPrv_Operator2	= 1 << 7;
  public static final int mPrv_Operator3	= 1 << 8;
  public static final int mPrv_Operator4	= 1 << 9;
  public static final int mPrv_Operator5	= 1 << 10;
  public static final int mPrv_Operator6	= 1 << 11;
  public static final int mPrv_Operator7	= 1 << 12;
  public static final int mPrv_Operator8	= 1 << 13;
  public static final int mPrv_Operator9	= 1 << 14;
  public static final int mPrv_Operator10	= 1 << 15;
  public static final int mPrv_DevRead		= 1 << 21;
  public static final int mPrv_DevPlc		= 1 << 22;
  public static final int mPrv_DevConfig	= 1 << 23;
  public static final int mPrv_DevClass		= 1 << 24;

  public static final int mAccess_AllRt		= mPrv_System |
				  		  mPrv_Maintenance |
				  		  mPrv_Process |
						  mPrv_Instrument |
						  mPrv_Operator1 |
						  mPrv_Operator2 |
						  mPrv_Operator3 |
						  mPrv_Operator4 |
						  mPrv_Operator5 |
						  mPrv_Operator6 |
						  mPrv_Operator7 |
						  mPrv_Operator8 |
						  mPrv_Operator9 |
						  mPrv_Operator10;
  public static final int mAccess_AllOperators	= mPrv_Operator1 |
						  mPrv_Operator2 |
						  mPrv_Operator3 |
						  mPrv_Operator4 |
						  mPrv_Operator5 |
						  mPrv_Operator6 |
						  mPrv_Operator7 |
						  mPrv_Operator8 |
						  mPrv_Operator9 |
						  mPrv_Operator10;
  public static final int mAccess_AllPwr	= ~0;
  
  public static final int mAdef_pointer	        = 1 << 0;		/*  1	  */
  public static final int mAdef_array	        = 1 << 1;		/*  2	  */
  public static final int mAdef_backup	        = 1 << 2;		/*  4	  */
  public static final int mAdef_changelog	= 1 << 3;		/*  8	  */
  public static final int mAdef_state	        = 1 << 4;		/*  16	  */
  public static final int mAdef_const	        = 1 << 5;		/*  32	  */
  public static final int mAdef_rtvirtual	= 1 << 6;		/*  64	  */
  public static final int mAdef_devbodyref	= 1 << 7;		/*  128	  */
  public static final int mAdef_dynamic	        = 1 << 8;		/*  256	  */
  public static final int mAdef_objidself	= 1 << 9;		/*  512	  */
  public static final int mAdef_noedit	        = 1 << 10;		/*  1024  */
  public static final int mAdef_invisible	= 1 << 11;		/*  2048  */
  public static final int mAdef_refdirect	= 1 << 12;		/*  4096  If an AttrRef should not give alue pointed to  */
  public static final int mAdef_noinvert	= 1 << 13;		/*  8192  */
  public static final int mAdef_noremove	= 1 << 14;		/*  16384 Cannot be removed, no used chkbx */
  public static final int mAdef_rtdbref	        = 1 << 15;		/*  32768 */
  public static final int mAdef_private	        = 1 << 16;		/*  65536 Private pointer, not to be displayed */


}





