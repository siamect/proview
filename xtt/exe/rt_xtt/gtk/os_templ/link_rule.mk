ifndef link_rule_mk
link_rule_mk := 1

ifeq ($(export_type),exp)			 
  link = $(ldxx) $(explinkflags) $(domap) -o $(pwr_exe)/rt_xtt_gtk \
	$(bld_dir)/rt_xtt_gtk.o $(bld_dir)/xtt_main.o $(rt_msg_eobjs) \
	$(pwr_eobj)/rt_io_user.o \
	$(pwre_conf_libdir) $(pwre_conf_libpwrxttgtk) $(pwre_conf_libpwrxtt) \
	$(pwre_conf_libpwrxttgtk) $(pwre_conf_libpwrxtt) \
	$(pwre_conf_libgtk) \
	$(pwre_conf_libpwrrt) $(pwre_conf_lib)

else
  link = $(ldxx) $(elinkflags) $(domap) -o $(pwr_exe)/rt_xtt_gtk \
	$(bld_dir)/rt_xtt_gtk.o $(bld_dir)/xtt_main.o $(rt_msg_eobjs) \
	$(pwr_eobj)/rt_io_user.o \
	$(pwre_conf_libdir) $(pwre_conf_libpwrxttgtk) $(pwre_conf_libpwrxtt) \
	$(pwre_conf_libpwrxttgtk) $(pwre_conf_libpwrxtt) \
	$(pwre_conf_libgtk) \
	$(pwre_conf_libpwrrt) $(pwre_conf_lib)

endif
endif
