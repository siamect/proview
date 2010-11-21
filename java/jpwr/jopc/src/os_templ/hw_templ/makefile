include $(pwre_dir_symbols)

local_java_sources := \
	JopcNode.java, \
	JopcRemoteconfig.java, \
	JopcRemnodeudp.java, \
	JopcRemtrans.java, \
	JopcBackup_conf.java, \
	JopcDsfastconf.java, \
	JopcDstrendconf.java, \
	JopcIohandler.java, \
	JopcMessagehandler.java, \
	JopcChanai.java, \
	JopcChanao.java, \
	JopcChandi.java, \
	JopcChando.java, \
	JopcChanii.java, \
	JopcChanio.java, \
	JopcAvGen.java, \
	JopcAv.java, \
	JopcAiGen.java, \
	JopcAi.java, \
	JopcAoGen.java, \
	JopcAo.java, \
	JopcDvGen.java, \
	JopcDv.java, \
	JopcDiGen.java, \
	JopcDi.java, \
	JopcDoGen.java, \
	JopcDo.java, \
	JopcIvGen.java, \
	JopcIv.java, \
	JopcIiGen.java, \
	JopcIi.java, \
	JopcIoGen.java, \
	JopcIo.java, \
	JopcMode.java, \
	JopcMode_sub.java, \
	JopcPidGen.java, \
	JopcPid.java, \
	JopcPlcthreadGen.java, \
	JopcPlcthread.java, \
	JopcPlcprocess.java, \
	JopcSysmonconfig.java, \
	JopcDisksup.java, \
	JopcSsab_safetyswitch.java

-include $(pwre_kroot)/tools/bld/src/$(os_name)/$(hw_name)/$(type_name)_generic.mk

ifeq ($($(type_name)_generic_mk),)
 -include $(pwre_kroot)/tools/bld/src/$(os_name)/$(type_name)_generic.mk
endif

ifeq ($($(type_name)_generic_mk),)
 include $(pwre_kroot)/tools/bld/src/$(type_name)_generic.mk
endif

-include ../../special.mk
-include ../special.mk
-include special.mk



