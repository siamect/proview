/* rt_plc_macro_io.h -- <short description>

   PROVIEW/R
   Copyright (C) 1996 by Comator Process AB.

   <Description>.  */

/*		 PREPROCESSOR RUTINER					    */

/**
  STODO								
  store digital output
  @aref stodo StoDo
*/
#define stodo_exec(obj,in)						\
  obj->ActualValue = in;

/**
  STODV								
  store digital value				
  @aref stodv StoDv
*/
#define stodv_exec(obj,in)						\
  obj->ActualValue = in;

/**
  STODP								
  Store into digital parameter			
  @aref stodp StoDp
*/
#define stodp_exec(ut,in)						\
  ut = in;

/**
  STOAO								
  store into analog output			
  @aref stoao StoAo
*/
#define stoao_exec(obj,in)						\
  obj->ActualValue = in;

/**
  STOAV								
  store into analog value				
  @aref stoav StoAv
*/
#define stoav_exec(obj,in)						\
  obj->ActualValue = in;

/**
  STOAP								
  Store into analog parameter			
  @aref stoap StoAp
*/
#define stoap_exec(ut,in)						\
  ut = in;

/**
  CSTOAO								
  store conditionally into analog output		
  @aref cstoao CStoAo
*/
#define cstoao_exec(obj,in,cond)					\
  if ( cond ) obj->ActualValue = in;

/**
  CSTOAV								
  store conditionally into analog value		
  @aref cstoav CStoAv
*/
#define cstoav_exec(obj,in,cond)					\
  if ( cond ) obj->ActualValue = in;

/**
  CSTOAP								
  Store conditionally into analog parameter	
  @aref cstoap CStoAp
*/
#define cstoap_exec(ut,in,cond)						\
  if ( cond ) ut = in;

/**
  SETDO								
  Set digital output if true			
  @aref setdo SetDo
*/
#define setdo_exec(obj,in)						\
  if ( in ) obj->ActualValue = true;

/**
  SETDV								
  Set digital value if true			
  @aref setdv SetDv
*/
#define setdv_exec(obj,in)						\
  if ( in ) obj->ActualValue = true;

/**
  SETDP								
  Set digital parameter if true			
  @aref setdp SetDp
*/
#define setdp_exec(ut,in)						\
  if ( in ) ut = true;

/**
  RESDO								
  Reset digital output if true			
  @aref resdo ResDo
*/
#define resdo_exec(obj,in)						\
  if ( in ) obj->ActualValue = false;

/**
  RESDV								
  Reset digital value if true			
  @aref resdv ResDv
*/
#define resdv_exec(obj,in)						\
  if ( in ) obj->ActualValue = false;

/**
  RESDP								
  Reset digital parameter if true			
  @aref resdp ResDp
*/
#define resdp_exec(ut,in)						\
  if ( in ) ut = false;

/**
  StoIp                                                           
  Store integer parameter                    
  @aref stoip StoIp
*/
#define StoIp_exec(ut,in)                                               \
  ut = in;

/**
  CStoIp                                                          
  Store conditionally integer parameter      
  @aref cstoip CStoIp
*/
#define CStoIp_exec(ut,in,cond)                                         \
  if ( cond ) ut = in;

/**
  StoAtoIp                                                           
  Store analog value into integer parameter                    
  @aref stoatoip StoAtoIp
*/
#define StoAtoIp_exec(ut,in)                                               \
  ut = in > 0 ? in + 0.5 : in - 0.5;

/**
  CStoAtoIp                                                          
  Store conditionally analog value into integer parameter      
  @aref cstoatoip CStoAtoIp
*/
#define CStoAtoIp_exec(ut,in,cond)                                         \
  if ( cond ) ut = in > 0 ? in + 0.5 : in - 0.5;

/**
  GetIpToA                                                           
  Get Integer parameter as an analog value             
  @aref getiptoa GetIpToA
*/
#define GetIpToA_exec(object,in)                                           \
  object->ActVal = in;

/**
  STODI								
  store digital input (Simulate)			
  @aref stodi StoDi
*/
#define stodi_exec(obj,in)						\
  obj->ActualValue = in;

/**
  SETDI								
  Set digital input if true (Simulate)		
  @aref setdi SetDi
*/
#define setdi_exec(obj,in)						\
  if ( in ) obj->ActualValue = true;

/**
  RESDI								
  Reset digital input if true (Simulate)		
  @aref resdi ResDi
*/
#define resdi_exec(obj,in)						\
  if ( in ) obj->ActualValue = false;

/**
  STOAI								
  store analog input (Simulate)			
  @aref stoai StoAi
*/
#define stoai_exec(obj,in)						\
  obj->ActualValue = in;

/**
  CSTOAI								
  store conditionally into analog input (Simulate) 
  @aref cstoai CStoAi
*/
#define cstoai_exec(obj,in,cond)					\
  if ( cond ) obj->ActualValue = in;

/**
  STOPI								
  store into co (Simulate) 
  @aref stopi StoPi
*/
#define stopi_exec( rawvalue, absvalue ,in)				\
  rawvalue->RawValue = in;						\
  absvalue->RawValue = in;

/**
  StoIo					
  Store integer output	
  @aref stoio StoIo
*/
#define stoio_exec(obj,in)						\
  obj->ActualValue = in;

/**
  CStoIo							
  store conditionally into integer output 
  @aref cstoio CStoIo
*/
#define cstoio_exec(obj,in,cond)					\
  if ( cond ) obj->ActualValue = in;

/**
  StoIv
  Store integer value	
  @aref stoiv StoIv
*/
#define stoiv_exec(obj,in)						\
  obj->ActualValue = in;

/**
  CStoIv
  store conditionally into integer value 
  @aref cstoiv CStoIv
*/
#define cstoiv_exec(obj,in,cond)					\
  if ( cond ) obj->ActualValue = in;

/**
  StoIi								
  store integer input (Simulate)			
  @aref stoii StoIi
*/
#define stoii_exec(obj,in)						\
  obj->ActualValue = in;

/**
  CStoIi								
  store conditionally into integer input (Simulate) 
  @aref cstoii CStoIi
*/
#define cstoii_exec(obj,in,cond)					\
  if ( cond ) obj->ActualValue = in;

/**
  AtoI
  @aref atoi AtoI
*/
#define AtoI_exec(obj,in) \
  obj->ActVal = in > 0 ? in + 0.5 : in - 0.5;

/**
  ItoA
  @aref itoa ItoA
*/
#define ItoA_exec(obj,in) \
  obj->ActVal = in;




