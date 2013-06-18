include $(pwre_dir_symbols)

extern_java_sources := \
	Cdh.java,\
	CdhrAttrRef.java,\
	CdhrBoolean.java,\
	CdhrClassId.java,\
	CdhrFloatArray.java,\
	CdhrFloat.java,\
	CdhrIntArray.java,\
	CdhrInt.java,\
	CdhrObjAttr.java,\
	CdhrObjid.java,\
	CdhrString.java,\
	CdhrTypeId.java,\
	CircBuffInfo.java,\
	Cli.java,\
	CliTable.java,\
	GdhApplIfc.java,\
	GdhrGetAttributeChar.java,\
	GdhrGetXttObj.java,\
	GdhrRefObjectInfo.java,\
	MhData.java,\
	MhrEvent.java,\
	MhrsEventId.java,\
	Pwrb.java,\
	Pwr.java,\
	Pwrs.java,\
	PwrtAttrRef.java,\
	PwrtObjid.java,\
	PwrtRefId.java,\
	PwrtStatus.java,\
	Sub.java


-include $(pwre_kroot)/tools/bld/src/$(os_name)/$(hw_name)/$(type_name)_generic.mk

ifeq ($($(type_name)_generic_mk),)
 -include $(pwre_kroot)/tools/bld/src/$(os_name)/$(type_name)_generic.mk
endif

ifeq ($($(type_name)_generic_mk),)
 include $(pwre_kroot)/tools/bld/src/$(type_name)_generic.mk
endif


#
# Copy Gdh from rt_client
#
local_objects := \
	$(app_package_dir)/$(comp_name)/Gdh.java

copy: $(objects) $(local_objects)

dirs : $(app_package_dir)/$(comp_name)$(dir_ext)
 
$(app_package_dir)/$(comp_name)$(dir_ext) :
	@ echo "Create directory " $(app_package_dir)/$(comp_name)$(dir_ext)
	@ $(mkdir) -p $(mkdirflags) $(app_package_dir)/$(comp_name)$(dir_ext)

$(app_package_dir)/$(comp_name)/Gdh.java : $(pwre_sroot)/jpwr/rt_client/src/Gdh.java
	@ $(log_h_h)
	@ $(cp) $(cpflags) $(source) $(target)

-include ../../special.mk
-include ../special.mk
-include special.mk


