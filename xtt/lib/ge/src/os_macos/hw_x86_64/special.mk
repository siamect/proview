ifndef variables_mk
 -include $(pwre_kroot)/tools/bld/src/$(os_name)/$(hw_name)/variables.mk
endif
ifndef variables_mk
 include $(pwre_kroot)/tools/bld/src/$(os_name)/variables.mk
endif

export_copy_local := $(exe_dir)/pwr_ge_setup.dat $(src_dir)/ge_colors.dat

copy : $(export_includes) $(l_copy) $(export_pwsg) $(export_meth) $(export_copy_local)


$(exe_dir)/pwr_ge_setup.dat : ../../pwr_ge_setup.dat
	@ echo "Copy $(source)"
	@ $(cp) $(cpflags) $(source) $(target)

$(src_dir)/ge_colors.dat : ../../ge_colors.dat
	@ echo "Copy $(source)"
	@ $(cp) $(cpflags) $(source) $(target)
