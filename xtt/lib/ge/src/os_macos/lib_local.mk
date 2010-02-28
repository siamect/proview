ifndef lib_local_mk
lib_local_mk := 1

vpath %.dat $(hw_source):$(os_source):$(co_source)

l_copy := $(exe_dir)/pwr_ge_setup.dat $(src_dir)/ge_colors.dat
l_clean_copy :=

clean_dat :
	@ if [ -e $(exe_dir)/pwr_ge_setup.dat ]; then $(rm) $(rmflags) $(exe_dir)/pwr_ge_setup.dat; fi
	@ if [ -e $(src_dir)/ge_colors.dat ]; then $(rm) $(rmflags) $(src_dir)/ge_colors.dat; fi

$(exe_dir)/pwr_ge_setup.dat : ../../pwr_ge_setup.dat
	@ echo "Copy $(source)"
	@ $(cp) $(cpflags) $(source) $(target)

$(src_dir)/ge_colors.dat : ge_colors.dat
	@ echo "Copy $(source)"
	@ $(cp) $(cpflags) $(source) $(target)

endif
