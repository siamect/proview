ifndef link_rule_mk
link_rule_mk := 1

link = $(ldxx) -L$(pwr_elib) $(elinkflags) $(domap) -o $(pwr_exe)/profinet_viewer_gtk \
	$(bld_dir)/profinet_viewer_gtk.o \
	$(bld_dir)/pn_viewer_gtk.o $(bld_dir)/pn_viewernav_gtk.o \
	$(bld_dir)/pn_viewer.o $(bld_dir)/pn_viewernav.o $(bld_dir)/pn_viewer_pnac.o\
	$(wb_msg_eobjs) $(rt_msg_eobjs) \
	$(pwr_eobj)/rt_io_user.o \
	$(pwre_conf_libdir) $(pwre_conf_libpwrxttgtk) $(pwre_conf_libpwrxtt) $(pwre_conf_libgtk) \
	$(pwre_conf_libpwrrt) $(pwre_conf_pnak) $(pwre_conf_lib)

endif
