include $(pwre_dir_symbols)

local_java_sources = \
	PwrtStatus.java \
	PwrtObjid.java \
	PwrtRefId.java \
	Pwr.java \
	Pwrb.java \
	CdhrBoolean.java \
	CdhrClassId.java \
	CdhrFloat.java \
	CdhrInt.java \
	CdhrObjid.java \
	CdhrString.java \
	Cdh.java \
	CliTable.java \
	Cli.java \
	QcomrCreateQ.java \
	Qcom.java \
	Errh.java \
	RtUtilities.java \
	RtSecurity.java \
	GdhrRefObjectInfo.java \
	GdhrGetAttributeChar.java \
	GdhrGetXttObj.java \
	CdhrObjAttr.java \
	Gdh.java \
        Sub.java \
	SubElement.java \
	GdhServer.java \
        GdhServerMonitor.java \
	MhrsEventId.java \
	MhrEvent.java \
	Mh.java \
	MhData.java \
	MhServer.java


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
