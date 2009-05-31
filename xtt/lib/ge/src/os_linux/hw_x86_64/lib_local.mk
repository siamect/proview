ifndef lib_local_mk
lib_local_mk := 1

vpath %.pwsg $(hw_source):$(os_source):$(co_source)
vpath %.dat $(hw_source):$(os_source):$(co_source)

pwsg_sources := $(sort \
             $(foreach file, \
               $(foreach dir, \
                 $(source_dirs), \
                 $(wildcard $(dir)/pwr_*.pwsg) \
               ), $(notdir $(file)) \
             ) \
           )

export_pwsg := $(addprefix $(exe_dir)/,$(pwsg_sources))
clean_pwsg := $(patsubst %.pwsg, clean_%.pwsg, $(pwsg_sources))

l_copy := $(export_pwsg) $(exe_dir)/pwr_ge_setup.dat $(src_dir)/ge_colors.dat
l_clean_copy := $(clean_pwsg) clean_dat

clean_dat :
	@ $(rm) $(rmflags) $(exe_dir)/pwr_ge_setup.dat
	@ $(rm) $(rmflags) $(src_dir)/ge_colors.dat
	
$(exe_dir)/pwr_ge_setup.dat : pwr_ge_setup.dat
	@ $(log_export)
	@ copy/nolog $(source) $(target)

$(src_dir)/ge_colors.dat : ge_colors.dat
	@ $(log_export)
	@ $(cp) $(source) $(target)

$(exe_dir)/%.pwsg : %.pwsg
	@ $(log_export)
	@ $(cp) $(source) $(target)

$(clean_pwsg) : clean_%.pwsg : %.pwsg
	@ $(rm) $(rmflags) $(exe_dir)/$(notdir $<)

endif
