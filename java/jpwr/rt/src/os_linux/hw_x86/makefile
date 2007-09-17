include $(pwre_dir_symbols)

local_java_sources = \
	PwrtStatus.java \
	PwrtObjid.java \
	PwrtAttrRef.java \
	PwrtRefId.java \
	Pwr.java \
	Pwrs.java \
	Pwrb.java \
	CdhrBoolean.java \
	CdhrClassId.java \
	CdhrTypeId.java \
	CdhrFloat.java \
	CdhrInt.java \
	CdhrObjid.java \
	CdhrAttrRef.java \
	CdhrString.java \
	CdhrFloatArray.java \
	CdhrIntArray.java \
	Cdh.java \
	CliTable.java \
	Cli.java \
	QcomrCreateQ.java \
	QcomrGetIniEvent.java \
	Qcom.java \
	Errh.java \
	RtUtilities.java \
	RtSecurity.java \
	GdhrRefObjectInfo.java \
	GdhrGetAttributeChar.java \
	GdhrGetXttObj.java \
	CdhrObjAttr.java \
	PwrsParInfo.java \
	GdhrsAttrDef.java \
	Gdh.java \
        Sub.java \
	SubElement.java \
	GdhServer.java \
        GdhServerMonitor.java \
	HistQuery.java \
	MhrsEventId.java \
	MhrEvent.java \
	Mh.java \
	MhData.java \
	MhServer.java \
	Hist.java \
	HistServer.java


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
