BEGIN {
  innum = 0;
  inbody = 0;
  innumaggr = 0;
  inbodyaggr = 0;
  pos = 0;
  par = "";
  inclass = "";
}

# BaseSensor LimitHH.AlarmText -> LimitHH.Alarm
# BaseSensor LimitH.AlarmText -> LimitH.Alarm
# BaseSensor LimitL.AlarmText -> LimitL.Alarm
# BaseSensor LimitLL.AlarmText -> LimitLL.Alarm
# BaseSupSwitch AlarmText -> Alarm
# BaseCircuitBreaker AlarmText -> Alarm 
# BaseActuatorA DiffAlarmText -> DiffAlarm
# BaseActuatorA SwitchAlarmText -> SwitchAlarm
# BaseActuatorIncrDecr SwitchAlarmText -> SwitchAlarm
# BaseContactor AlarmText -> Alarm
# BaseFilter AlarmText -> Alarm
# BaseMValve SwitchAlarmText -> SwitchAlarm
# BaseManValve AlarmText -> Alarm
# BaseOverloadRelay AlarmText -> Alarm
# BaseSafetySwitch AlarmText -> Alarm
# BaseFcPPO5MotorAggr AlarmTextToOften -> AlarmToOften
# BaseFcPPO5MotorAggr AlarmTextPowOnNotOk -> AlarmPowOnNotOk
# BaseFcPPO5MotorAggr AlarmTextStartNoOk -> AlarmStartNoOk
# BaseFcPPO3MotorAggr AlarmTextToOften -> AlarmToOften
# BaseFcPPO3MotorAggr AlarmTextPowOnNotOk -> AlarmPowOnNotOk
# BaseFcPPO3MotorAggr AlarmTextStartNoOk -> AlarmStartNoOk
#
# CompModeA AlarmNotReadyM
# CompModeA AlarmNotReadyA
# CompModeA Alarm1LocalMod
# CompModeA Alarm2LocalMod
# CompModeA AlarmNotManOrd
# CompModeAM AlarmLocalMode
# CompModeD AlarmNotReadyM
# CompModeD Alarm1LocalMod
# CompModeD Alarm2LocalMod
# CompModeD AlarmNotManOrd
# CompModeD2 AlarmNotReadyM
# CompModeD2 AlarmNotReadyA
# CompModeD2 Alarm1LocalMod
# CompModeD2 Alarm2LocalMod
# CompModeD2 AlarmNotManOrd
# CompModeD2 AlarmOrderTimeOut
# BaseFcPPO5 AlarmLocalMod
# BaseFcPPO5 AlarmTripped
# BaseFcPPO5 AlarmWarning
# BaseFcPPO5 AlarmError
# BaseFcPPO5 AlarmConnection
# BaseFcPPO3 AlarmLocalMod
# BaseFcPPO3 AlarmTripped
# BaseFcPPO3 AlarmWarning
# BaseFcPPO3 AlarmError
# BaseFcPPO3 AlarmConnection
#
# BaseTempSensor Super.BaseSensor
# BaseTempSwitch Super.BaseSupSwitch
# BaseActuatorA Mode.CompModeAM
# BaseActuatorIncrDecr Mode.CompModeD2
# BaseCValve Actuator.BaseActuatorA
# BaseC3WayValve Super.BaseCValve
# BaseCDamper Actuator.BaseActuatorA
# BaseFlowSensor Super.BaseSensor
# BaseLevelSensor Super.BaseSensor
# BaseLevelSwitch Super.BaseSupSwitch
# BasePressureSwitch Super.BaseSupSwitch
# BaseMDamper Super.BaseMValve
# BaseM3WayValve Super.BaseMValve
# BaseMotor TempSwitch.BaseTempSwitch
# BaseMotor TempSensor.BaseTempSensor
# BaseElHeater TempSensor.BaseTempSensor
# BasePositSensor Super.BaseSensor
# BasePressureSensor Super.BaseSensor
# BasePropValve Mode.CompModeAM
# BaseMotorAggr Contactor.BaseContactor
# BaseMotorAggr CircuitBreaker.BaseCircuitBreaker
# BaseMotorAggr SafetySwitch.BaseSafetySwitch
# BaseMotorAggr OverloadRelay.BaseOverloadRelay
# BaseMotorAggr Mode.CompModeD
# BaseMotorAggr Motor.BaseMotor
# BaseFanAggr Super.BaseMotorAggr
# BasePumpAggr Super.BaseMotorAggr
# BaseMotorIncrDecrAggr ContactorIncr.BaseContactor
# BaseMotorIncrDecrAggr ContactorDecr.BaseContactor
# BaseMotorIncrDecrAggr CircuitBreaker.BaseCircuitBreaker
# BaseMotorIncrDecrAggr SafetySwitch.BaseSafetySwitch
# BaseMotorIncrDecrAggr OverloadRelay.BaseOverloadRelay
# BaseMotorIncrDecrAggr Mode.CompMode2D
# BaseMotorIncrDecrAggr Motor.BaseMotor
# BaseFcPPO5MotorAggr CircuitBreaker.BaseCircuitBreaker
# BaseFcPPO5MotorAggr Contactor.BaseContactor
# BaseFcPPO5MotorAggr FrequencyConverter.BaseFcPPO5
# BaseFcPPO5MotorAggr SafetySwitch.BaseSafetySwitch
# BaseFcPPO5MotorAggr Motor.BaseMotor
# BaseFcPPO5MotorAggr Mode.CompModeA
# BaseFcPPO5FanAggr Super.BaseFcPPO5MotorAggr
# BaseFcPPO5PumpAggr Super.BaseFcPPO5MotorAggr
# BaseFcPPO3MotorAggr CircuitBreaker.BaseCircuitBreaker
# BaseFcPPO3MotorAggr Contactor.BaseContactor
# BaseFcPPO3MotorAggr FrequencyConverter.BaseFcPPO5
# BaseFcPPO3MotorAggr SafetySwitch.BaseSafetySwitch
# BaseFcPPO3MotorAggr Motor.BaseMotor
# BaseFcPPO3MotorAggr Mode.CompModeA
# BaseFcPPO3FanAggr Super.BaseFcPPO3MotorAggr
# BaseFcPPO3PumpAggr Super.BaseFcPPO3MotorAggr
# BaseValveIncrDecr Actuator.BaseActuatorIncrDecr

# Ssab_SafetySwitch Super.BaseSafetySwitch
# Sinamics_G120_Tgm1 Super.BaseFcPPO3
# Inor_TempSensor_VRS Super.BaseTempSensor
# KM_CircuitBreaker_PKZ Super.BaseCircuitBreaker
# KM_Contactor_PKZ Super.BaseContactor
# Ventim_Valve_VM3001 Super.BaseManValve
# Bernard_Actuator_OA Super.BaseActuatorA
# Cewe_SafetySwitch_KSA_LBAS Super.BaseSafetySwitch
# Cewe_SafetySwitch_BAS Super.BaseSafetySwitch
# Danfoss_Converter_FC300_PPO3 Super.BaseFcPPO3
# Danfoss_Converter_FC300_PPO5 Super.BaseFcPPO5
# Eurotherm_TC3001Aggr CircuitBreaker.BaseCircuitBreaker
# Eurotherm_TC3001Aggr Contactor.BaseContactor
# Eurotherm_TC3001Aggr Heater.BaseElHeater
# Eurotherm_TC3001Aggr Mode.CompModeA
# Impac_TempSensor_IN300 Super.BaseTempSensor
# Legrand_CircuitBreaker_DX Super.BaseCircuitBreaker
# Suco_PresSwitch_27AF Super.BasePressureSwitch
# Mobrey_LevelSensor_MSP422 Super.BaseLevelSensor
# Mobrey_LevelSwitch_EC Super.BaseLevelSwitch
# Klay_PressureSensor_Series2000 Super.BasePressureSensor
# Omron_Contactor_J7KN Super.BaseContactor
# Janitza_UMG96S_1 LimitHH_Voltage.AlarmText -> LimitHH_Voltage.Alarm
# Janitza_UMG96S_1 LimitH_Voltage.AlarmText -> LimitH_Voltage.Alarm
# Janitza_UMG96S_1 LimitL_Voltage.AlarmText -> LimitL_Voltage.Alarm
# Janitza_UMG96S_1 LimitLL_Voltage.AlarmText -> LimitLL_Voltage.Alarm
# Janitza_UMG96S_1 LimitHH_Current.AlarmText -> LimitHH_Current.Alarm
# Janitza_UMG96S_1 LimitH_Current.AlarmText -> LimitH_Current.Alarm
# Janitza_UMG96S_1 LimitL_Current.AlarmText -> LimitL_Current.Alarm
# Janitza_UMG96S_1 LimitLL_Current.AlarmText -> LimitLL_Current.Alarm
# ABB_CircuitBreaker_MS116 Super.BaseCircuitBreaker
# ABB_Contactor_A Super.BaseContactor
# ABB_ACS800MotorAggr CircuitBreaker.BaseCircuitBreaker
# ABB_ACS800MotorAggr Contactor.BaseContactor
# ABB_ACS800MotorAggr SafetySwitch.BaseSafetySwitch
# ABB_ACS800MotorAggr Motor.BaseMotor
# ABB_ACS800MotorAggr Mode.CompModeA
# ABB_ACS800PumpAggr Super.ABB_ACS800MotorAggr
# ABB_ACS800FanAggr Super.ABB_ACS800MotorAggr
# ABB_Converter_ACS800_PPO3 Super.BaseFcPPO3
# ABB_Converter_ACS800_PPO5 Super.BaseFcPPO5
# ABB_Sensor_Pb_PA Value1.BaseSensor
# ABB_Sensor_Pb_PA Value2.BaseSensor
# ABB_Sensor_Pb_PA Value3.BaseSensor
# ABB_TempSensor_TF12 Super.ABB_Sensor_Pb_PA
# ABB_PressureSensor_265GS Super.ABB_Sensor_Pb_PA
# ABB_DiffPressureSensor_265DS Super.ABB_Sensor_Pb_PA
# ABB_FlowSensor_FXE4000 Super.ABB_Sensor_Pb_PA
# ABB_ACS880MotorAggr CircuitBreaker.BaseCircuitBreaker
# ABB_ACS880MotorAggr Contactor.BaseContactor
# ABB_ACS880MotorAggr SafetySwitch.BaseSafetySwitch
# ABB_ACS880MotorAggr Motor.BaseMotor
# ABB_ACS880MotorAggr Mode.CompModeA
# ABB_ACS880PumpAggr Super.ABB_ACS880MotorAggr
# ABB_ACS880FanAggr Super.ABB_ACS880MotorAggr
# ABB_ACS880EplMotorAggr Super.ABB_ACS880MotorAggr
# ABB_ACS880EplPumpAggr Super.ABB_ACS880EplMotorAggr
# ABB_ACS880EplFanAggr Super.ABB_ACS880EplMotorAggr

# ABB_ACS800_1 AlarmLocalMod
# ABB_ACS800_1 AlarmTripped
# ABB_ACS800_1 AlarmFanOld
# ABB_ACS800_1 AlarmWarning
# ABB_ACS800_1 AlarmConnection

function basesensor( prefix, attr)
{
    if ( prefix "LimitHH.AlarmText" == attr)
      return prefix "LimitHH.Alarm.DetectText";
    if ( prefix "LimitH.AlarmText" == attr)
      return prefix "LimitH.Alarm.DetectText";
    if ( prefix "LimitL.AlarmText" == attr)
      return prefix "LimitL.Alarm.DetectText";
    if ( prefix "LimitLL.AlarmText" == attr)
      return prefix "LimitLL.Alarm.DetectText";
    return "";
}

function basesupswitch( prefix, attr)
{
    if ( prefix "AlarmText" == attr)
      return prefix "Alarm.DetectText";
    return "";
}

function basecircuitbreaker( prefix, attr)
{
    if ( prefix "AlarmText" == attr)
      return prefix "Alarm.DetectText";
    return "";
}

function baseactuatora( prefix, attr)
{
    if ( prefix "SwitchAlarmText" == attr)
      return prefix "SwitchAlarm.DetectText";
    if ( prefix "DisableSwitchAlarmText" == attr)
      return prefix "DisableSwitchAlarm";
    if ( prefix "DiffAlarmText" == attr)
      return prefix "DiffAlarm.DetectText";
    if ( prefix "DisableDiffAlarmText" == attr)
      return prefix "DisableDiffAlarm";
    return compmodeam( prefix "Mode.", attr);
}

function baseactuatorincrdecr( prefix, attr)
{
    if ( prefix "SwitchAlarmText" == attr)
      return prefix "SwitchAlarm.DetectText";
    if ( prefix "DisableSwitchAlarmText" == attr)
      return prefix "DisableSwitchAlarm";
    return compmoded2( prefix "Mode.", attr);
}

function basecontactor( prefix, attr)
{
    if ( prefix "AlarmText" == attr)
      return prefix "Alarm.DetectText";
    return "";
}

function basefilter( prefix, attr)
{
    if ( prefix "AlarmText" == attr)
      return prefix "Alarm.DetectText";
    if ( prefix "DisableAlarmText" == attr)
      return prefix "DisableAlarm";
    return "";
}

function basemvalve( prefix, attr)
{
    if ( prefix "SwitchAlarmText" == attr)
      return prefix "SwitchAlarm.DetectText";
    if ( prefix "DisableSwitchAlarmText" == attr)
      return prefix "DisableSwitchAlarm";
    return "";
}

function basemanvalve( prefix, attr)
{
    if ( prefix "AlarmText" == attr)
      return prefix "Alarm.DetectText";
    if ( prefix "DisableAlarmText" == attr)
      return prefix "DisableAlarm";
    return "";
}

function baseoverloadrelay( prefix, attr)
{
  if ( prefix != "") {
    if ( prefix "AlarmText" == attr)
      return prefix "Alarm.DetectText";
    return "";
  }
  else {
    if ( "AlarmText" == attr)
      return "Alarm.DetectText";
    return "";
  }
}

function basesafetyswitch( prefix, attr)
{
    if ( prefix "AlarmText" == attr)
      return prefix "Alarm.DetectText";
    return "";
}

function basefcppo5motoraggr( prefix, attr)
{
  if ( prefix "AlarmTextToOften" == attr)
    return prefix "AlarmToOften.DetectText";
  if ( prefix "AlarmTextPowOnNotOk" == attr)
    return prefix "AlarmPowOnNotOk.DetectText";
  if ( prefix "AlarmTextStartNoOk" == attr)
    return prefix "AlarmStartNotOk.DetectText";
  ret =  basecircuitbreaker( prefix "CircuitBreaker.", attr);
  if ( ret != "")
    return ret;
  ret =  basecontactor( prefix "Contactor.", attr);
  if ( ret != "")
    return ret;
  ret =  basefcppo5( prefix "FrequencyConverter.", attr);
  if ( ret != "")
    return ret;
  ret =  basesafetyswitch( prefix "SafetySwitch.", attr);
  if ( ret != "")
    return ret;
  ret =  compmodea( prefix "Mode.", attr);
  if ( ret != "")
    return ret;
  return basemotor( prefix "Motor.", attr);
}

function basefcppo3motoraggr( prefix, attr)
{
  if ( prefix "AlarmTextToOften" == attr)
    return prefix "AlarmToOften.DetectText";
  if ( prefix "AlarmTextPowOnNotOk" == attr)
    return prefix "AlarmPowOnNotOk.DetectText";
  if ( prefix "AlarmTextStartNoOk" == attr)
    return prefix "AlarmStartNotOk.DetectText";
  ret =  basecircuitbreaker( prefix "CircuitBreaker.", attr);
  if ( ret != "")
    return ret;
  ret =  basecontactor( prefix "Contactor.", attr);
  if ( ret != "")
    return ret;
  ret =  basefcppo5( prefix "FrequencyConverter.", attr);
  if ( ret != "")
    return ret;
  ret =  basesafetyswitch( prefix "SafetySwitch.", attr);
  if ( ret != "")
    return ret;
  ret =  compmodea( prefix "Mode.", attr);
  if ( ret != "")
    return ret;
  return basemotor( prefix "Motor.", attr);
}

function compmodea( prefix, attr)
{
    if ( prefix "AlarmNotReadyM" == attr)
      return prefix "AlarmNotReadyM.DetectText";
    if ( prefix "AlarmNotReadyA" == attr)
      return prefix "AlarmNotReadyA.DetectText";
    if ( prefix "Alarm1LocalMod" == attr)
      return prefix "Alarm1LocalMod.DetectText";
    if ( prefix "Alarm2LocalMod" == attr)
      return prefix "Alarm2LocalMod.DetectText";
    if ( prefix "AlarmNotManOrd" == attr)
      return prefix "AlarmNotManOrd.DetectText";
    return "";
}

function compmodeam( prefix, attr)
{
    if ( prefix "AlarmLocalMode" == attr)
      return prefix "AlarmLocalMode.DetectText";
    return "";
}

function compmoded( prefix, attr)
{
    if ( prefix "AlarmNotReadyM" == attr)
      return prefix "AlarmNotReadyM.DetectText";
    if ( prefix "AlarmNotReadyA" == attr)
      return prefix "AlarmNotReadyA.DetectText";
    if ( prefix "Alarm1LocalMod" == attr)
      return prefix "Alarm1LocalMod.DetectText";
    if ( prefix "Alarm2LocalMod" == attr)
      return prefix "Alarm2LocalMod.DetectText";
    if ( prefix "AlarmNotManOrd" == attr)
      return prefix "AlarmNotManOrd.DetectText";
    return "";
}

function compmoded2( prefix, attr)
{
    if ( prefix "AlarmNotReadyM" == attr)
      return prefix "AlarmNotReadyM.DetectText";
    if ( prefix "AlarmNotReadyA" == attr)
      return prefix "AlarmNotReadyA.DetectText";
    if ( prefix "Alarm1LocalMod" == attr)
      return prefix "Alarm1LocalMod.DetectText";
    if ( prefix "Alarm2LocalMod" == attr)
      return prefix "Alarm2LocalMod.DetectText";
    if ( prefix "AlarmNotManOrd" == attr)
      return prefix "AlarmNotManOrd.DetectText";
    if ( prefix "AlarmOrderTimeOut" == attr)
      return prefix "AlarmOrderTimeOut.DetectText";
    return "";
}

function basefcppo5( prefix, attr)
{
    if ( prefix "AlarmLocalMod" == attr)
      return prefix "AlarmLocalMod.DetectText";
    if ( prefix "AlarmTripped" == attr)
      return prefix "AlarmTripped.DetectText";
    if ( prefix "AlarmWarning" == attr)
      return prefix "AlarmWarning.DetectText";
    if ( prefix "AlarmError" == attr)
      return prefix "AlarmError.DetectText";
    if ( prefix "AlarmConnection" == attr)
      return prefix "AlarmConnection.DetectText";
    return "";
}

function basefcppo3( prefix, attr)
{
    if ( prefix "AlarmLocalMod" == attr)
      return prefix "AlarmLocalMod.DetectText";
    if ( prefix "AlarmTripped" == attr)
      return prefix "AlarmTripped.DetectText";
    if ( prefix "AlarmWarning" == attr)
      return prefix "AlarmWarning.DetectText";
    if ( prefix "AlarmError" == attr)
      return prefix "AlarmError.DetectText";
    if ( prefix "AlarmConnection" == attr)
      return prefix "AlarmConnection.DetectText";
    return "";
}

function basetempsensor( prefix, attr)
{
  return basesensor( prefix "Super.", attr);
}

function basetempswitch( prefix, attr)
{
  return basesupswitch( prefix "Super.", attr);
}

function basecvalve( prefix, attr)
{
  return baseactuatora( prefix "Actuator.", attr);
}

function basec3wayvalve( prefix, attr)
{
  return basecvalve("Super.", attr);
}

function basecdamper( prefix, attr)
{
  return baseactuatora( prefix "Actuator.", attr);
}

function baseflowsensor( prefix, attr)
{
  return basesensor( prefix "Super.", attr);
}

function baselevelsensor( prefix, attr)
{
  return basesensor( prefix "Super.", attr);
}

function baselevelswitch( prefix, attr)
{
  return basesupswitch( prefix "Super.", attr);
}

function basepressureswitch( prefix, attr)
{
  return basesupswitch( prefix "Super.", attr);
}

function basemdamper( prefix, attr)
{
  return basemvalve( prefix "Super.", attr);
}

function basem3wayvalve( prefix, attr)
{
  return basemvalve( prefix "Super.", attr);
}

function basemotor( prefix, attr)
{
  ret =  basetempswitch( prefix "TempSwitch.", attr);
  if ( ret != "")
    return ret;
  return basetempsensor( prefix "TempSensor.", attr);
}

function baseelheater( prefix, attr)
{
  return basetempsensor( prefix "TempSensor.", attr);
}

function basepositsensor( prefix, attr)
{
  return basesensor( prefix "Super.", attr);
}

function basepressuresensor( prefix, attr)
{
  return basesensor( prefix "Super.", attr);
}

function basepropvalve( prefix, attr)
{
  return compmodeam( prefix "Mode.", attr);
}

function basemotoraggr( prefix, attr)
{
  ret =  basecontactor( prefix "Contactor.", attr);
  if ( ret != "")
    return ret;
  ret =  basecircuitbreaker( prefix "CircuitBreaker.", attr);
  if ( ret != "")
    return ret;
  ret =  basesafetyswitch( prefix "SafetySwitch.", attr);
  if ( ret != "")
    return ret;
  ret =  baseoverloadrelay( prefix "OverloadRelay.", attr);
  if ( ret != "")
    return ret;
  ret =  compmoded( prefix "Mode.", attr);
  if ( ret != "")
    return ret;
  return basemotor( prefix "Motor.", attr);
}

function basefanaggr( prefix, attr)
{
  return basemotoraggr( prefix "Super.", attr);
}

function basepumpaggr( prefix, attr)
{
  return basemotoraggr( prefix "Super.", attr);
}

function basemotorincrdecraggr( prefix, attr)
{
  ret =  basecontactor( prefix "ContactorIncr.", attr);
  if ( ret != "")
    return ret;
  ret =  basecontactor( prefix "ContactorDecr.", attr);
  if ( ret != "")
    return ret;
  ret =  basecircuitbreaker( prefix "CircuitBreaker.", attr);
  if ( ret != "")
    return ret;
  ret =  basesafetyswitch( prefix "SafetySwitch.", attr);
  if ( ret != "")
    return ret;
  ret =  baseoverloadrelay( prefix "OverloadRelay.", attr);
  if ( ret != "")
    return ret;
  ret =  compmoded2( prefix "Mode.", attr);
  if ( ret != "")
    return ret;
  return basemotor( prefix "Motor.", attr);
}

function basefcppo5fanaggr( prefix, attr)
{
  return basefcppo5motoraggr( prefix "Super.", attr);
}

function basefcppo5pumpaggr( prefix, attr)
{
  return basefcppo5motoraggr( prefix "Super.", attr);
}

function basefcppo3fanaggr( prefix, attr)
{
  return basefcppo3motoraggr( prefix "Super.", attr);
}

function basefcppo3pumpaggr( prefix, attr)
{
  return basefcppo3motoraggr( prefix "Super.", attr);
}

function basevalveincrdecr( prefix, attr)
{
  return baseactuatorincrdecr( prefix "Actuator.", attr);
}

function ssab_safetyswitch( prefix, attr)
{
  return basesafetyswitch( prefix "Super.", attr);
}

function sinamics_g120_tgm1( prefix, attr)
{
  return basefcppo3( prefix "Super.", attr);
}

function inor_tempsensor_vrs( prefix, attr)
{
  return basetempsensor( prefix "Super.", attr);
}

function km_circuitbreaker_pkz( prefix, attr)
{
  return basecircuitbreaker( prefix "Super.", attr);
}

function km_contactor_pkz( prefix, attr)
{
  return basecontactor( prefix "Super.", attr);
}

function ventim_valve_vm3001( prefix, attr)
{
  return basemanvalve( prefix "Super.", attr);
}

function bernard_actuator_oa( prefix, attr)
{
  return baseactuatora( prefix "Super.", attr);
}

function cewe_safetyswitch_ksa_lbas( prefix, attr)
{
  return basesafetyswitch( prefix "Super.", attr);
}

function cewe_safetyswitch_bas( prefix, attr)
{
  return basesafetyswitch( prefix "Super.", attr);
}

function danfoss_converter_fc300_ppo3( prefix, attr)
{
  return basefcppo3( prefix "Super.", attr);
}

function danfoss_converter_fc300_ppo5( prefix, attr)
{
  return basefcppo5( prefix "Super.", attr);
}

function eurotherm_tc3001aggr( prefix, attr)
{
  ret =  basecontactor( prefix "Contactor.", attr);
  if ( ret != "")
    return ret;
  ret =  basecircuitbreaker( prefix "CircuitBreaker.", attr);
  if ( ret != "")
    return ret;
  ret =  baseelheater( prefix "Heater.", attr);
  if ( ret != "")
    return ret;
  return  compmodea( prefix "Mode.", attr);
}

function impac_tempsensor_in300( prefix, attr)
{
  return basetempsensor( prefix "Super.", attr);
}

function legrand_circuitbreaker_dx( prefix, attr)
{
  return basecircuitbreaker( prefix "Super.", attr);
}

function suco_presswitch_27af( prefix, attr)
{
  return basepressureswitch( prefix "Super.", attr);
}

function mobrey_levelsensor_msp422( prefix, attr)
{
  return baselevelsensor( prefix "Super.", attr);
}

function mobrey_levelswitch_ec( prefix, attr)
{
  return baselevelswitch( prefix "Super.", attr);
}

function klay_pressuresensor_series2000( prefix, attr)
{
  return basepressuresensor( prefix "Super.", attr);
}

function omron_contactor_j7kn( prefix, attr)
{
  return basecontactor( prefix "Super.", attr);
}

function janitza_umg96s_1( prefix, attr)
{
    if ( prefix "LimitHH_Voltage.AlarmText" == attr)
      return prefix "LimitHH_Voltage.Alarm.DetectText";
    if ( prefix "LimitH_Voltage.AlarmText" == attr)
      return prefix "LimitH_Voltage.Alarm.DetectText";
    if ( prefix "LimitL_Voltage.AlarmText" == attr)
      return prefix "LimitL_Voltage.Alarm.DetectText";
    if ( prefix "LimitLL_Voltage.AlarmText" == attr)
      return prefix "LimitLL_Voltage.Alarm.DetectText";
    if ( prefix "LimitHH_Current.AlarmText" == attr)
      return prefix "LimitHH_Current.Alarm.DetectText";
    if ( prefix "LimitH_Current.AlarmText" == attr)
      return prefix "LimitH_Current.Alarm.DetectText";
    if ( prefix "LimitL_Current.AlarmText" == attr)
      return prefix "LimitL_Current.Alarm.DetectText";
    if ( prefix "LimitLL_Current.AlarmText" == attr)
      return prefix "LimitLL_Current.Alarm.DetectText";
    return "";
}

function abb_contactor_a( prefix, attr)
{
  return basecontactor( prefix "Super.", attr);
}

function abb_circuitbreaker_ms116( prefix, attr)
{
  return basecircuitbreaker( prefix "Super.", attr);
}

function abb_acs800_1( prefix, attr)
{
  if ( prefix "AlarmLocalMod" == attr)
    return prefix "AlarmLocalMod.DetectText";
  if ( prefix "AlarmTripped" == attr)
    return prefix "AlarmTripped.DetectText";
  if ( prefix "AlarmFanOld" == attr)
    return prefix "AlarmFanOld.DetectText";
  if ( prefix "AlarmWarning" == attr)
    return prefix "AlarmWarning.DetectText";
  if ( prefix "AlarmConnection" == attr)
    return prefix "AlarmConnection.DetectText";
  ret = "";
}

function abb_acc800( prefix, attr)
{
  if ( prefix "AlarmLocalMod" == attr)
    return prefix "AlarmLocalMod.DetectText";
  if ( prefix "AlarmWarning" == attr)
    return prefix "AlarmWarning.DetectText";
  if ( prefix "AlarmError" == attr)
    return prefix "AlarmError.DetectText";
  if ( prefix "AlarmTorqFault" == attr)
    return prefix "AlarmTorqFault.DetectText";
  if ( prefix "AlarmBrakeFault" == attr)
    return prefix "AlarmBrakeFault.DetectText";
  if ( prefix "AlarmTorqProvFault" == attr)
    return prefix "AlarmTorqProvFault.DetectText";
  ret = "";
}

function abb_acs880_1( prefix, attr)
{
  if ( prefix "AlarmLocalMod" == attr)
    return prefix "AlarmLocalMod.DetectText";
  if ( prefix "AlarmTripped" == attr)
    return prefix "AlarmTripped.DetectText";
  if ( prefix "AlarmFanOld" == attr)
    return prefix "AlarmFanOld.DetectText";
  if ( prefix "AlarmWarning" == attr)
    return prefix "AlarmWarning.DetectText";
  if ( prefix "AlarmConnection" == attr)
    return prefix "AlarmConnection.DetectText";
  ret = "";
}

function abb_acs800motoraggr( prefix, attr)
{
  if ( prefix "AlarmTextToOften" == attr)
    return prefix "AlarmToOften.DetectText";
  if ( prefix "AlarmTextPowOnNotOk" == attr)
    return prefix "AlarmPowOnNotOk.DetectText";
  if ( prefix "AlarmTextStartNoOk" == attr)
    return prefix "AlarmStartNotOk.DetectText";
  ret =  basecircuitbreaker( prefix "CircuitBreaker.", attr);
  if ( ret != "")
    return ret;
  ret =  basecontactor( prefix "Contactor.", attr);
  if ( ret != "")
    return ret;
  ret =  basesafetyswitch( prefix "SafetySwitch.", attr);
  if ( ret != "")
    return ret;
  ret =  abb_acs800_1( prefix "ACS800.", attr);
  if ( ret != "")
    return ret;
  ret =  compmodea( prefix "Mode.", attr);
  if ( ret != "")
    return ret;
  return basemotor( prefix "Motor.", attr);
}

function abb_acs800pumpaggr( prefix, attr)
{
  return abb_acs800motoraggr( prefix "Super.", attr);
}

function abb_acs800fanaggr( prefix, attr)
{
  return abb_acs800motoraggr( prefix "Super.", attr);
}

function abb_converter_acs800_ppo3( prefix, attr)
{
  return basefcppo3( prefix "Super.", attr);
}

function abb_converter_acs800_ppo5( prefix, attr)
{
  return basefcppo5( prefix "Super.", attr);
}

function abb_sensor_pb_pa( prefix, attr)
{
  ret =  basesensor( prefix "Value1.", attr);
  if ( ret != "")
    return ret;
  ret =  basesensor( prefix "Value2.", attr);
  if ( ret != "")
    return ret;
  return basesensor( prefix "Value3.", attr);
}

function abb_tempsensor_tf12( prefix, attr)
{
  return abb_sensor_pb_pa( prefix "Super.", attr);
}

function abb_pressuresensor_265gs( prefix, attr)
{
  return abb_sensor_pb_pa( prefix "Super.", attr);
}

function abb_diffpressuresensor_265ds( prefix, attr)
{
  return abb_sensor_pb_pa( prefix "Super.", attr);
}

function abb_flowsensor_fxe4000( prefix, attr)
{
  return abb_sensor_pb_pa( prefix "Super.", attr);
}

function abb_acs880motoraggr( prefix, attr)
{
  if ( prefix "AlarmTextToOften" == attr)
    return prefix "AlarmToOften.DetectText";
  if ( prefix "AlarmTextPowOnNotOk" == attr)
    return prefix "AlarmPowOnNotOk.DetectText";
  if ( prefix "AlarmTextStartNoOk" == attr)
    return prefix "AlarmStartNotOk.DetectText";
  ret =  basecircuitbreaker( prefix "CircuitBreaker.", attr);
  if ( ret != "")
    return ret;
  ret =  basecontactor( prefix "Contactor.", attr);
  if ( ret != "")
    return ret;
  ret =  basesafetyswitch( prefix "SafetySwitch.", attr);
  if ( ret != "")
    return ret;
  ret =  abb_acs880_1( prefix "ACS880.", attr);
  if ( ret != "")
    return ret;
  ret =  compmodea( prefix "Mode.", attr);
  if ( ret != "")
    return ret;
  return basemotor( prefix "Motor.", attr);
}

function abb_acs880pumpaggr( prefix, attr)
{
  return abb_acs880motoraggr( prefix "Super.", attr);
}

function abb_acs880fanaggr( prefix, attr)
{
  return abb_acs880motoraggr( prefix "Super.", attr);
}

function abb_acs880eplmotoraggr( prefix, attr)
{
  return abb_acs880motoraggr( prefix "Super.", attr);
}

function abb_acs880eplfanaggr( prefix, attr)
{
  return abb_acs880eplmotoraggr( prefix "Super.", attr);
}

function abb_acs880eplpumpaggr( prefix, attr)
{
  return abb_acs880eplmotoraggr( prefix "Super.", attr);
}

function abb_acs880_epl_1( prefix, attr)
{
  return abb_acs880_1( prefix "Super.", attr);
}


{
  if ( $3 == "BaseSensor" || 
       $3 == "BaseSupSwitch" ||
       $3 == "BaseCircuitBreaker" ||
       $3 == "BaseActuatorA" ||
       $3 == "BaseContactor" ||
       $3 == "BaseFilter" ||
       $3 == "BaseMValve" ||
       $3 == "BaseManValve" ||
       $3 == "BaseOverloadRelay" ||
       $3 == "BaseSafetySwitch" ||
       $3 == "BaseFcPPO5MotorAggr" ||
       $3 == "BaseFcPPO3MotorAggr" ||
       $3 == "CompModeA" ||
       $3 == "CompModeAM" ||
       $3 == "CompModeD" ||
       $3 == "CompModeD2" ||
       $3 == "BaseFcPPO5" ||
       $3 == "BaseFcPPO3" ||
       $3 == "BaseTempSensor" ||
       $3 == "BaseTempSwitch" ||
       $3 == "BaseActuatorIncrDecr" ||
       $3 == "BaseCValve" ||
       $3 == "BaseC3WayValve" ||
       $3 == "BaseCDamper" ||
       $3 == "BaseFlowSensor" ||
       $3 == "BaseLevelSensor" ||
       $3 == "BaseLevelSwitch" ||
       $3 == "BasePressureSwitch" ||
       $3 == "BaseMDamper" ||
       $3 == "BaseM3WayValve" ||
       $3 == "BaseMotor" ||
       $3 == "BaseElHeater" ||
       $3 == "BasePositSensor" ||
       $3 == "BasePressureSensor" ||
       $3 == "BasePropValve" ||
       $3 == "BaseMotorAggr" ||
       $3 == "BaseFanAggr" ||
       $3 == "BasePumpAggr" ||
       $3 == "BaseMotorIncrDecrAggr" ||
       $3 == "BaseFcPPO5FanAggr" ||
       $3 == "BaseFcPPO5PumpAggr" ||
       $3 == "BaseFcPPO3FanAggr" ||
       $3 == "BaseFcPPO3PumpAggr" ||
       $3 == "BaseValveIncrDecr" ||
       $3 == "Ssab_SafetySwitch" ||
       $3 == "Sinamics_G120_Tgm1" ||
       $3 == "Inor_TempSensor_VRS" ||
       $3 == "KM_CircuitBreaker_PKZ" ||
       $3 == "KM_Contactor_PKZ" ||
       $3 == "Ventim_Valve_VM3001" ||
       $3 == "Bernard_Actuator_OA" ||
       $3 == "Cewe_SafetySwitch_KSA_LBAS" ||
       $3 == "Cewe_SafetySwitch_BAS" ||
       $3 == "Danfoss_Converter_FC300_PPO3" ||
       $3 == "Danfoss_Converter_FC300_PPO5" ||
       $3 == "Eurotherm_TC3001Aggr" ||
       $3 == "Impac_TempSensor_IN300" ||
       $3 == "Legrand_CircuitBreaker_DX" ||
       $3 == "Suco_PresSwitch_27AF" ||
       $3 == "Mobrey_LevelSensor_MSP422" ||
       $3 == "Mobrey_LevelSwitch_EC" ||
       $3 == "Klay_PressureSensor_Series2000" ||
       $3 == "Omron_Contactor_J7KN" ||
       $3 == "Janitza_UMG96S_1" ||
       $3 == "ABB_CircuitBreaker_MS116" ||
       $3 == "ABB_Contactor_A" ||
       $3 == "ABB_ACS800MotorAggr" ||
       $3 == "ABB_ACS800PumpAggr" ||
       $3 == "ABB_ACS800FanAggr" ||
       $3 == "ABB_Converter_ACS800_PPO3" ||
       $3 == "ABB_Converter_ACS800_PPO5" ||
       $3 == "ABB_Sensor_Pb_PA" ||
       $3 == "ABB_TempSensor_TF12" ||
       $3 == "ABB_PressureSensor_265GS" ||
       $3 == "ABB_DiffPressureSensor_265DS" ||
       $3 == "ABB_FlowSensor_FXE4000" ||
       $3 == "ABB_ACS880MotorAggr" ||
       $3 == "ABB_ACS880PumpAggr" ||
       $3 == "ABB_ACS880FanAggr" ||
       $3 == "ABB_ACS880EplMotorAggr" ||
       $3 == "ABB_ACS880EplPumpAggr" ||
       $3 == "ABB_ACS880EplFanAggr" ||
       $3 == "ABB_ACS800_1" ||
       $3 == "ABB_ACC800" ||
       $3 == "ABB_ACS880_1" ||
       $3 == "ABB_ACS880_Epl_1") {
    innum = 1;
    inbody = 0;
    inclass = $3;
  }
  if ( innum) {
    if ( $1 == "Body" && $2 == "RtBody") {
      inbody = 1;
    }
    if ( inbody) {
      if ( $1 == "EndBody") {
        printf( "%s\n", $0);
        innum = 0;
      }
      else if ( $1 == "EndObject") {
        innum = 0;
        printf( "%s\n", $0);
      }
      else if ( $1 == "Attr") {
	if ( inclass == "BaseSensor")
	  str = basesensor("", $2);
	else if ( inclass == "BaseSupSwitch") 
	  str = basesupswitch( "", $2);
	else if ( inclass == "BaseCircuitBreaker") 
	  str = basecircuitbreaker( "", $2);
	else if ( inclass == "BaseActuatorA") 
	  str = baseactuatora( "", $2);
	else if ( inclass == "BaseContactor") 
	  str = basecontactor( "", $2);
	else if ( inclass == "BaseFilter") 
	  str = basefilter( "", $2);
	else if ( inclass == "BaseMValve") 
	  str = basemvalve( "", $2);
	else if ( inclass == "BaseManValve") 
	  str = basemanvalve( "", $2);
	else if ( inclass == "BaseOverloadRelay") 
	  str = baseoverloadrelay( "", $2);
	else if ( inclass == "BaseSafetySwitch") 
	  str = basesafetyswitch( "", $2);
	else if ( inclass == "BaseFcPPO5MotorAggr") 
	  str = basefcppo5motoraggr( "", $2);
	else if ( inclass == "BaseFcPPO3MotorAggr") 
	  str = basefcppo3motoraggr( "", $2);
	else if ( inclass == "CompModeA") 
	  str = compmodea( "", $2);
	else if ( inclass == "CompModeAM") 
	  str = compmodeam( "", $2);
	else if ( inclass == "CompModeD") 
	  str = compmoded( "", $2);
	else if ( inclass == "CompModeD2") 
	  str = compmoded2( "", $2);
	else if ( inclass == "BaseFcPPO5") 
	  str = basefcppo5( "", $2);
	else if ( inclass == "BaseFcPPO3") 
	  str = basefcppo3( "", $2);
	else if ( inclass == "BaseTempSensor") 
	  str = basetempsensor( "", $2);
	else if ( inclass == "BaseTempSwitch") 
	  str = basetempswitch( "", $2);
	else if ( inclass == "BaseActuatorIncrDecr") 
	  str = baseactuatorincrdecr( "", $2);
	else if ( inclass == "BaseCValve") 
	  str = basecvalve( "", $2);
	else if ( inclass == "BaseC3WayValve") 
	  str = basec3wayvalve( "", $2);
	else if ( inclass == "BaseCDamper") 
	  str = basecdamper( "", $2);
	else if ( inclass == "BaseFlowSensor") 
	  str = baseflowsensor( "", $2);
	else if ( inclass == "BaseLevelSensor") 
	  str = baselevelsensor( "", $2);
	else if ( inclass == "BaseLevelSwitch") 
	  str = baselevelswitch( "", $2);
	else if ( inclass == "BasePressureSwitch") 
	  str = basepressureswitch( "", $2);
	else if ( inclass == "BaseMDamper") 
	  str = basemdamper( "", $2);
	else if ( inclass == "BaseM3WayValve") 
	  str = basem3wayvalve( "", $2);
	else if ( inclass == "BaseMotor") 
	  str = basemotor( "", $2);
	else if ( inclass == "BaseElHeater") 
	  str = baseelheater( "", $2);
	else if ( inclass == "BasePositSensor") 
	  str = basepositsensor( "", $2);
	else if ( inclass == "BasePressureSensor") 
	  str = basepressuresensor( "", $2);
	else if ( inclass == "BasePropValve") 
	  str = basepropvalve( "", $2);
	else if ( inclass == "BaseMotorAggr") 
	  str = basemotoraggr( "", $2);
	else if ( inclass == "BaseFanAggr") 
	  str = basefanaggr( "", $2);
	else if ( inclass == "BasePumpAggr") 
	  str = basepumpaggr( "", $2);
	else if ( inclass == "BaseMotorIncrDecrAggr") 
	  str = basemotorincrdecraggr( "", $2);
	else if ( inclass == "BaseFcPPO5FanAggr") 
	  str = basefcppo5fanaggr( "", $2);
	else if ( inclass == "BaseFcPPO5PumpAggr") 
	  str = basefcppo5pumpaggr( "", $2);
	else if ( inclass == "BaseFcPPO3FanAggr") 
	  str = basefcppo3fanaggr( "", $2);
	else if ( inclass == "BaseFcPPO3PumpAggr") 
	  str = basefcppo3pumpaggr( "", $2);
	else if ( inclass == "BaseValveIncrDecr") 
	  str = basevalveincrdecr( "", $2);
	else if ( inclass == "Ssab_SafetySwitch") 
	  str = ssab_safetyswitch( "", $2);
	else if ( inclass == "Sinamics_G120_Tgm1") 
	  str = sinamics_g120_tgm1( "", $2);
	else if ( inclass == "Inor_TempSensor_VRS") 
	  str = inor_tempsensor_vrs( "", $2);
	else if ( inclass == "KM_CircuitBreaker_PKZ") 
	  str = km_circuitbreaker_pkz( "", $2);
	else if ( inclass == "KM_Contactor_PKZ") 
	  str = km_contactor_pkz( "", $2);
	else if ( inclass == "Ventim_Valve_VM3001") 
	  str = ventim_valve_vm3001( "", $2);
	else if ( inclass == "Bernard_Actuator_OA") 
	  str = bernard_actuator_oa( "", $2);
	else if ( inclass == "Cewe_SafetySwitch_KSA_LBAS") 
	  str = cewe_safetyswitch_ksa_lbas( "", $2);
	else if ( inclass == "Cewe_SafetySwitch_BAS") 
	  str = cewe_safetyswitch_bas( "", $2);
	else if ( inclass == "Danfoss_Converter_FC300_PPO3") 
	  str = danfoss_converter_fc300_ppo3( "", $2);
	else if ( inclass == "Danfoss_Converter_FC300_PPO5") 
	  str = danfoss_converter_fc300_ppo5( "", $2);
	else if ( inclass == "Eurotherm_TC3001Aggr") 
	  str = eurotherm_tc3001aggr( "", $2);
	else if ( inclass == "Impac_TempSensor_IN300") 
	  str = impac_tempsensor_in300( "", $2);
	else if ( inclass == "Legrand_CircuitBreaker_DX") 
	  str = legrand_circuitbreaker_dx( "", $2);
	else
	  str = "";
	if ( inclass == "Suco_PresSwitch_27AF") 
	  str = suco_presswitch_27af( "", $2);
	else if ( inclass == "Mobrey_LevelSensor_MSP422") 
	  str = mobrey_levelsensor_msp422( "", $2);
	else if ( inclass == "Mobrey_LevelSwitch_EC") 
	  str = mobrey_levelswitch_ec( "", $2);
	else if ( inclass == "Klay_PressureSensor_Series2000") 
	  str = klay_pressuresensor_series2000( "", $2);
	else if ( inclass == "Omron_Contactor_J7KN") 
	  str = omron_contactor_j7kn( "", $2);
	else if ( inclass == "Janitza_UMG96S_1") 
	  str = janitza_umg96s_1( "", $2);
	else if ( inclass == "ABB_CircuitBreaker_MS116") 
	  str = abb_circuitbreaker_ms116( "", $2);
	else if ( inclass == "ABB_Contactor_A") 
	  str = abb_contactor_a( "", $2);
	else if ( inclass == "ABB_ACS800MotorAggr") 
	  str = abb_acs800motoraggr( "", $2);
	else if ( inclass == "ABB_ACS800PumpAggr") 
	  str = abb_acs800pumpaggr( "", $2);
	else if ( inclass == "ABB_ACS800FanAggr") 
	  str = abb_acs800fanaggr( "", $2);
	else if ( inclass == "ABB_Converter_ACS800_PPO3") 
	  str = abb_converter_acs800_ppo3( "", $2);
	else if ( inclass == "ABB_Converter_ACS800_PPO5") 
	  str = abb_converter_acs800_ppo5( "", $2);
	else if ( inclass == "ABB_Sensor_Pb_PA") 
	  str = abb_sensor_pb_pa( "", $2);
	else if ( inclass == "ABB_TempSensor_TF12") 
	  str = abb_tempsensor_tf12( "", $2);
	else if ( inclass == "ABB_PressureSensor_265GS") 
	  str = abb_pressuresensor_265gs( "", $2);
	else if ( inclass == "ABB_DiffPressureSensor_265DS") 
	  str = abb_diffpressuresensor_265ds( "", $2);
	else if ( inclass == "ABB_FlowSensor_FXE4000") 
	  str = abb_flowsensor_fxe4000( "", $2);
	else if ( inclass == "ABB_ACS880MotorAggr") 
	  str = abb_acs880motoraggr( "", $2);
	else if ( inclass == "ABB_ACS880PumpAggr") 
	  str = abb_acs880pumpaggr( "", $2);
	else if ( inclass == "ABB_ACS880FanAggr") 
	  str = abb_acs880fanaggr( "", $2);
	else if ( inclass == "ABB_ACS880EplMotorAggr") 
	  str = abb_acs880eplmotoraggr( "", $2);
	else if ( inclass == "ABB_ACS880EplPumpAggr") 
	  str = abb_acs880eplpumpaggr( "", $2);
	else if ( inclass == "ABB_ACS880EplFanAggr") 
	  str = abb_acs880eplfanaggr( "", $2);
	else if ( inclass == "ABB_ACS800_1") 
	  str = abb_acs800_1( "", $2);
	else if ( inclass == "ABB_ACC800") 
	  str = abb_acc800( "", $2);
	else if ( inclass == "ABB_ACS880_1") 
	  str = abb_acs880_1( "", $2);
	else if ( inclass == "ABB_ACS880_Epl_1") 
	  str = abb_acs880_epl_1( "", $2);

	if ( str != "") {
	  printf( "Attr   %s = ", str); 
	  for ( i = 4; i <= NF; i++)
	    printf("%s ", $i);
	  printf( "\n");
	}
	else {
	  printf( "%s\n", $0);
	}
      }
      else {
        printf( "%s\n", $0);
      }
    }
    else {
      printf( "%s\n", $0);
    }
  } 
  else {
    printf( "%s\n", $0);
  }
}
