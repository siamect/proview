/* rt_plc_macro_io.h -- <short description>

   PROVIEW/R
   Copyright (C) 1996 by Comator Process AB.

   <Description>.  */

/*		 PREPROCESSOR RUTINER					    */

/*_*
  STODO								
  store digital output
  @aref stodo StoDo
*/
#define stodo_exec(obj,in)						\
  obj->ActualValue = in;

/*_*
  STODV								
  store digital value				
  @aref stodv StoDv
*/
#define stodv_exec(obj,in)						\
  obj->ActualValue = in;

/*_*
  STODP								
  Store into digital parameter			
  @aref stodp StoDp
*/
#define stodp_exec(ut,in)						\
  ut = in;

/*_*
  STOAO								
  store into analog output			
  @aref stoao StoAo
*/
#define stoao_exec(obj,in)						\
  obj->ActualValue = in;

/*_*
  STOAV								
  store into analog value				
  @aref stoav StoAv
*/
#define stoav_exec(obj,in)						\
  obj->ActualValue = in;

/*_*
  STOAP								
  Store into analog parameter			
  @aref stoap StoAp
*/
#define stoap_exec(ut,in)						\
  ut = in;

/*_*
  CSTOAO								
  store conditionally into analog output		
  @aref cstoao CStoAo
*/
#define cstoao_exec(obj,in,cond)					\
  if ( cond ) obj->ActualValue = in;

/*_*
  CSTOAV								
  store conditionally into analog value		
  @aref cstoav CStoAv
*/
#define cstoav_exec(obj,in,cond)					\
  if ( cond ) obj->ActualValue = in;

/*_*
  CSTOAP								
  Store conditionally into analog parameter	
  @aref cstoap CStoAp
*/
#define cstoap_exec(ut,in,cond)						\
  if ( cond ) ut = in;

/*_*
  SETDO								
  Set digital output if true			
  @aref setdo SetDo
*/
#define setdo_exec(obj,in)						\
  if ( in ) obj->ActualValue = true;

/*_*
  SETDV								
  Set digital value if true			
  @aref setdv SetDv
*/
#define setdv_exec(obj,in)						\
  if ( in ) obj->ActualValue = true;

/*_*
  SETDP								
  Set digital parameter if true			
  @aref setdp SetDp
*/
#define setdp_exec(ut,in)						\
  if ( in ) ut = true;

/*_*
  RESDO								
  Reset digital output if true			
  @aref resdo ResDo
*/
#define resdo_exec(obj,in)						\
  if ( in ) obj->ActualValue = false;

/*_*
  RESDV								
  Reset digital value if true			
  @aref resdv ResDv
*/
#define resdv_exec(obj,in)						\
  if ( in ) obj->ActualValue = false;

/*_*
  RESDP								
  Reset digital parameter if true			
  @aref resdp ResDp
*/
#define resdp_exec(ut,in)						\
  if ( in ) ut = false;

/*_*
  StoIp                                                           
  Store integer parameter                    
  @aref stoip StoIp
*/
#define StoIp_exec(ut,in)                                               \
  ut = in;

/*_*
  CStoIp                                                          
  Store conditionally integer parameter      
  @aref cstoip CStoIp
*/
#define CStoIp_exec(ut,in,cond)                                         \
  if ( cond ) ut = in;

/*_*
  StoAtoIp                                                           
  Store analog value into integer parameter                    
  @aref stoatoip StoAtoIp
*/
#define StoAtoIp_exec(ut,in)                                               \
  ut = in > 0 ? in + 0.5 : in - 0.5;

/*_*
  CStoAtoIp                                                          
  Store conditionally analog value into integer parameter      
  @aref cstoatoip CStoAtoIp
*/
#define CStoAtoIp_exec(ut,in,cond)                                         \
  if ( cond ) ut = in > 0 ? in + 0.5 : in - 0.5;

/*_*
  GetIpToA                                                           
  Get Integer parameter as an analog value             
  @aref getiptoa GetIpToA
*/
#define GetIpToA_exec(object,in)                                           \
  object->ActVal = in;

/*_*
  STODI								
  store digital input (Simulate)			
  @aref stodi StoDi
*/
#define stodi_exec(obj,in)						\
  obj->ActualValue = in;

/*_*
  SETDI								
  Set digital input if true (Simulate)		
  @aref setdi SetDi
*/
#define setdi_exec(obj,in)						\
  if ( in ) obj->ActualValue = true;

/*_*
  RESDI								
  Reset digital input if true (Simulate)		
  @aref resdi ResDi
*/
#define resdi_exec(obj,in)						\
  if ( in ) obj->ActualValue = false;

/*_*
  STOAI								
  store analog input (Simulate)			
  @aref stoai StoAi
*/
#define stoai_exec(obj,in)						\
  obj->ActualValue = in;

/*_*
  CSTOAI								
  store conditionally into analog input (Simulate) 
  @aref cstoai CStoAi
*/
#define cstoai_exec(obj,in,cond)					\
  if ( cond ) obj->ActualValue = in;

/*_*
  STOPI								
  store into co (Simulate) 
  @aref stopi StoPi
*/
#define stopi_exec( rawvalue, absvalue ,in)				\
  rawvalue->RawValue = in;						\
  absvalue->RawValue = in;

/*_*
  StoIo					
  Store integer output	
  @aref stoio StoIo
*/
#define stoio_exec(obj,in)						\
  obj->ActualValue = in;

/*_*
  CStoIo							
  store conditionally into integer output 
  @aref cstoio CStoIo
*/
#define cstoio_exec(obj,in,cond)					\
  if ( cond ) obj->ActualValue = in;

/*_*
  StoIv
  Store integer value	
  @aref stoiv StoIv
*/
#define stoiv_exec(obj,in)						\
  obj->ActualValue = in;

/*_*
  CStoIv
  store conditionally into integer value 
  @aref cstoiv CStoIv
*/
#define cstoiv_exec(obj,in,cond)					\
  if ( cond ) obj->ActualValue = in;

/*_*
  StoIi								
  store integer input (Simulate)			
  @aref stoii StoIi
*/
#define stoii_exec(obj,in)						\
  obj->ActualValue = in;

/*_*
  CStoIi								
  store conditionally into integer input (Simulate) 
  @aref cstoii CStoIi
*/
#define cstoii_exec(obj,in,cond)					\
  if ( cond ) obj->ActualValue = in;

/*_*
  AtoI
  @aref atoi AtoI
*/
#define AtoI_exec(obj,in) \
  obj->ActVal = in > 0 ? in + 0.5 : in - 0.5;

/*_*
  ItoA
  @aref itoa ItoA
*/
#define ItoA_exec(obj,in) \
  obj->ActVal = in;

/*_*
  StoDattr
  @aref stodattr StoDattr
*/
#define StoDattr_exec(attr,in) \
  attr = in;

/*_*
  SetDattr
  @aref setdattr SetDattr
*/
#define SetDattr_exec(attr,in) \
  if ( in) attr = true;

/*_*
  ResDattr
  @aref resdattr ResDattr
*/
#define ResDattr_exec(attr,in) \
  if ( in) attr = false;

/*_*
  StoIattr
  @aref stoiattr StoIattr
*/
#define StoIattr_exec(attr,in) \
  attr = in;

/*_*
  CStoIattr
  @aref cstoiattr CStoIattr
*/
#define CStoIattr_exec(attr,in,cond) \
  if ( cond) attr = in;

/*_*
  StoAattr
  @aref stoaattr StoAattr
*/
#define StoAattr_exec(attr,in) \
  attr = in;

/*_*
  CStoAattr
  @aref cstoaattr CStoAattr
*/
#define CStoAattr_exec(attr,in,cond) \
  if ( cond) attr = in;

/*_*
  StoSattr
  @aref stosattr StoSattr
*/
#define StoSattr_exec(attr,in,size) \
  strncpy( attr, in, size); \
  attr[size-1] = 0;

/*_*
  CStoSattr
  @aref cstosattr CStoSattr
*/
#define CStoSattr_exec(attr,in,cond,size) \
  if ( cond) { \
    strncpy( attr, in, size); \
    attr[size-1] = 0; \
  }


