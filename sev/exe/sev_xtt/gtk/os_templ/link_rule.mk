ifndef link_rule_mk
link_rule_mk := 1

  link = $(ldxx) $(elinkflags) $(domap) -o $(pwr_exe)/sev_xtt_gtk \
	$(bld_dir)/sev_xtt_gtk.o \
	$(bld_dir)/xtt_tbl_gtk.o $(bld_dir)/xtt_tblnav_gtk.o $(bld_dir)/xtt_tbl.o \
	$(bld_dir)/xtt_tblnav.o $(bld_dir)/xtt_tbl_command.o \
	$(rt_msg_eobjs) \
	$(pwr_eobj)/rt_io_user.o \
	$(pwre_conf_libdir) $(pwre_conf_libpwrxttgtk) $(pwre_conf_libpwrxtt) $(pwre_conf_libpwrxttgtk)  $(pwre_conf_libpwrxtt) $(pwre_conf_libgtk) \
	$(pwre_conf_libpwrrt) $(pwre_conf_lib)

endif
