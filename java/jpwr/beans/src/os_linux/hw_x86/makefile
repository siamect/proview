include $(pwre_dir_symbols)

local_java_sources := \
	JopMotor.java \
	JopMotorBeanInfo.java \
	JopSpeedswitch.java \
	JopSpeedswitchBeanInfo.java \
	JopButtonset.java \
	JopButtonsetBeanInfo.java \
	JopButtonimage.java \
	JopButtonimageBeanInfo.java \
	JopDigimage.java \
	JopDigimageBeanInfo.java \
	JopImage.java \
	JopImageBeanInfo.java \
	JopFramethin.java \
	JopFramethinBeanInfo.java \
	JopFramemedium.java \
	JopFramemediumBeanInfo.java \
	JopFramethick.java \
	JopFramethickBeanInfo.java \
	JopReliefthin.java \
	JopReliefthinBeanInfo.java \
	JopReliefmedium.java \
	JopReliefmediumBeanInfo.java \
	JopReliefthick.java \
	JopReliefthickBeanInfo.java \
	JopReliefgray.java \
	JopReliefgrayBeanInfo.java \
	JopReliefroundcornersmedium.java \
	JopReliefroundcornersmediumBeanInfo.java \
	JopReliefroundcornersthick.java \
	JopReliefroundcornersthickBeanInfo.java \
	JopReliefroundcornersgray.java \
	JopReliefroundcornersgrayBeanInfo.java \
	JopFrameroundcorners.java \
	JopFrameroundcornersBeanInfo.java \
	JopArrow.java \
	JopArrowBeanInfo.java \
	JopIndround.java \
	JopIndroundBeanInfo.java \
	JopIndsquare.java \
	JopIndsquareBeanInfo.java \
	JopIndsquarelarge.java \
	JopIndsquarelargeBeanInfo.java \
	JopIndroundlarge.java \
	JopIndroundlargeBeanInfo.java \
	JopIndsoap.java \
	JopIndsoapBeanInfo.java \
	JopValve.java \
	JopValveBeanInfo.java \
	JopValvemanuel.java \
	JopValvemanuelBeanInfo.java \
	JopValvecontrol.java \
	JopValvecontrolBeanInfo.java \
	JopRoundbutton.java \
	JopRoundbuttonBeanInfo.java \
	JopSoapbutton.java \
	JopSoapbuttonBeanInfo.java \
	JopButtontoggle.java \
	JopButtontoggleBeanInfo.java \
	JopCheckvalve.java \
	JopCheckvalveBeanInfo.java \
	JopTank.java \
	JopTankBeanInfo.java \
	JopFilter.java \
	JopFilterBeanInfo.java \
	JopPressureswitch.java \
	JopPressureswitchBeanInfo.java \
	JopPressuregauge.java \
	JopPressuregaugeBeanInfo.java \
	JopReleasevalve.java \
	JopReleasevalveBeanInfo.java \
	JopReleasevalveright.java \
	JopReleasevalverightBeanInfo.java \
	JopHydraulicpump.java \
	JopHydraulicpumpBeanInfo.java \
	JopCon.java \
	JopConBeanInfo.java \
	JopCon_u.java \
	JopCon_uBeanInfo.java \
	JopCon_v.java \
	JopCon_vBeanInfo.java

#	JopTextField.java \
#	JopTextFieldBeanInfo.java \
#	JopButtoncommand.java \
#	JopHydraulicpump.java \
#	JopIndroundlarge.java \
#	JopIndsoap.java \
#	JopPump.java \
#	JopRelief.java \
#	JopRoundbutton.java \
#	JopSlider1.java \
#	JopSliderbackground1.java \
#	JopValve.java

-include $(pwre_sroot)/tools/bld/src/$(os_name)/$(hw_name)/$(type_name)_generic.mk

ifeq ($($(type_name)_generic_mk),)
 -include $(pwre_sroot)/tools/bld/src/$(os_name)/$(type_name)_generic.mk
endif

ifeq ($($(type_name)_generic_mk),)
 include $(pwre_sroot)/tools/bld/src/$(type_name)_generic.mk
endif

-include ../../special.mk
-include ../special.mk
-include special.mk
