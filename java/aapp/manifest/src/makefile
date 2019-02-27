include $(pwre_dir_symbols)

-include $(pwre_kroot)/tools/bld/src/$(type_name)_generic.mk

local_objects := \
	$(release_root)/bld/$(type_name)/$(appname)/AndroidManifest.xml \
	$(release_root)/bld/$(type_name)/$(appname)/.project \
	$(release_root)/bld/$(type_name)/$(appname)/.classpath

copy: $(local_objects)

$(release_root)/bld/$(type_name)/$(appname)/AndroidManifest.xml : AndroidManifest.xml
	@ $(log_h_h)
	@ $(cp) $(cpflags) $(source) $(target)

$(release_root)/bld/$(type_name)/$(appname)/.project : project
	@ $(log_h_h)
	@ $(cp) $(cpflags) $(source) $(target)

$(release_root)/bld/$(type_name)/$(appname)/.classpath : classpath
	@ $(log_h_h)
	@ $(cp) $(cpflags) $(source) $(target)

-include special.mk
