ifndef lib_local_mk
lib_local_mk := 1

vpath %.dat $(hw_source):$(os_source):$(co_source)

l_copy := $(exe_dir)/pwr_ge_setup.dat $(src_dir)/ge_colors.dat
l_clean_copy := clean_dat

clean_dat :
	@ $(rm) $(rmflags) $(exe_dir)/pwr_ge_setup.dat
	@ $(rm) $(rmflags) $(src_dir)/ge_colors.dat

$(exe_dir)/pwr_ge_setup.dat : pwr_ge_setup.dat
	@ $(log_export)
	@ $(cp) $(cpflags) $(source) $(target)

$(src_dir)/ge_colors.dat : ge_colors.dat
	@ $(log_export)
	@ $(cp) $(cpflags) $(source) $(target)

endif
