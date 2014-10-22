package jpwr.rt;

public class Mh {
	  public static final int mh_mEventFlags_Return = 0x01;
	  public static final int mh_mEventFlags_Ack = 0x02;
	  public static final int mh_mEventFlags_Bell = 0x04;
	  public static final int mh_mEventFlags_Force = 0x08;
	  public static final int mh_mEventFlags_InfoWindow = 0x10;
	  public static final int mh_mEventFlags_Returned = 0x20;
	  public static final int mh_mEventFlags_NoObject = 0x40;
	  
	  
	  public static final  int mh_mEventStatus_NotRet = (1 << 0);
	  public static final  int mh_mEventStatus_NotAck = (1 << 1);
	  public static final  int mh_mEventStatus_Block  = (1 << 2);
	  
	  public static final int mh_eEventPrio__ = 0;   
	  public static final int mh_eEventPrio_A = 67;  
	  public static final int mh_eEventPrio_B = 66; 
	  public static final int mh_eEventPrio_C = 65;
	  public static final int mh_eEventPrio_D = 64;
	  public static final int mh_eEventPrio_  = 63;
	  
	  
	  public static final int mh_eEvent__		= 0;    
	  public static final int mh_eEvent_Ack		= 1;
	  public static final int mh_eEvent_Block	= 2;
	  public static final int mh_eEvent_Cancel	= 3;
	  public static final int mh_eEvent_CancelBlock	= 4;
	  public static final int mh_eEvent_Missing	= 5;
	  public static final int mh_eEvent_Reblock	= 6;
	  public static final int mh_eEvent_Return	= 7;
	  public static final int mh_eEvent_Unblock	= 8;
	  public static final int mh_eEvent_Info	= 32;
	  public static final int mh_eEvent_Alarm	= 64;
          public static final int mh_eEvent_MaintenanceAlarm = 128;
          public static final int mh_eEvent_SystemAlarm	= 256;
          public static final int mh_eEvent_UserAlarm1	= 512;
          public static final int mh_eEvent_UserAlarm2	= 1024;
          public static final int mh_eEvent_UserAlarm3	= 2048;
          public static final int mh_eEvent_UserAlarm4	= 4096;
          public static final int mh_eEvent_   		= 4097;
	  
	  public static final int EventType_ClearAlarmList = 66;
}
