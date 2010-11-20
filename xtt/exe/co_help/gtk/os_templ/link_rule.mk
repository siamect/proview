ifndef link_rule_mk
link_rule_mk := 1

  link = $(ldxx) $(elinkflags) $(domap) -o $(pwr_exe)/co_help_gtk \
	$(bld_dir)/co_help_gtk.o $(bld_dir)/help_main.o $(rt_msg_eobjs) \
	$(pwr_eobj)/rt_io_user.o \
	$(pwre_conf_libpwrxttgtk) $(pwre_conf_libpwrxtt) $(pwre_conf_libgtk) \
	$(pwre_conf_libpwrrt) $(pwre_conf_lib)

endif
