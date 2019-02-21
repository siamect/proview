include $(pwre_dir_symbols)

-include $(pwre_kroot)/tools/bld/src/$(type_name)_generic.mk

dirs : $(app_package_dir)/$(comp_name)$(dir_ext)

$(app_package_dir)/$(comp_name)$(dir_ext) :
	@ echo "Create directory " $(app_package_dir)/$(comp_name)$(dir_ext)
	@ $(mkdir) -p $(mkdirflags) $(app_package_dir)/$(comp_name)$(dir_ext)

-include special.mk
