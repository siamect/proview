include $(pwre_dir_symbols)

-include $(pwre_kroot)/tools/bld/src/$(os_name)/$(hw_name)/$(type_name)_generic.mk

ifeq ($($(type_name)_generic_mk),)
 -include $(pwre_kroot)/tools/bld/src/$(os_name)/$(type_name)_generic.mk
endif

ifeq ($($(type_name)_generic_mk),)
 include $(pwre_kroot)/tools/bld/src/$(type_name)_generic.mk
endif

local_objects := \
	$(release_root)/bld/$(type_name)/$(appname)/AndroidManifest.xml \
	$(release_root)/bld/$(type_name)/$(appname)/.project \
	$(release_root)/bld/$(type_name)/$(appname)/.classpath

copy: $(local_objects)

$(release_root)/bld/$(type_name)/$(appname)/AndroidManifest.xml : ../../AndroidManifest.xml
	@ $(log_h_h)
	@ $(cp) $(cpflags) $(source) $(target)

$(release_root)/bld/$(type_name)/$(appname)/.project : ../../project
	@ $(log_h_h)
	@ $(cp) $(cpflags) $(source) $(target)

$(release_root)/bld/$(type_name)/$(appname)/.classpath : ../../classpath
	@ $(log_h_h)
	@ $(cp) $(cpflags) $(source) $(target)

-include ../../special.mk
-include ../special.mk
-include special.mk



