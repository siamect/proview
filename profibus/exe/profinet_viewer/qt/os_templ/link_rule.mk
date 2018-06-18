ifndef link_rule_mk
link_rule_mk := 1

ifeq ($(PWRE_CONF_PNAK),"1")
link = $(ldxx) -L$(pwr_elib) $(elinkflags) $(domap) -o $(pwr_exe)/profinet_viewer_qt \
	$(bld_dir)/profinet_viewer_qt.o \
	$(bld_dir)/pn_viewer_qt.o \
	$(bld_dir)/pn_viewer_qt_moc.o \
	$(bld_dir)/pn_viewernav_qt.o \
	$(bld_dir)/pn_viewer.o $(bld_dir)/pn_viewernav.o \
	$(bld_dir)/pn_viewer_pnac.o\
	$(wb_msg_eobjs) $(rt_msg_eobjs) \
	$(pwr_eobj)/rt_io_user.o \
	$(pwre_conf_libdir) $(pwre_conf_libpwrxttqt) $(pwre_conf_libpwrxtt) $(pwre_conf_libqt) \
	$(pwre_conf_libpwrrt) $(pwre_conf_libpnak) $(pwre_conf_lib)
else
link = $(ldxx) -L$(pwr_elib) $(elinkflags) $(domap) -o $(pwr_exe)/profinet_viewer_qt \
	$(bld_dir)/profinet_viewer_qt.o \
	$(bld_dir)/pn_viewer_qt.o \
	$(bld_dir)/pn_viewernav_qt.o \
	$(bld_dir)/pn_viewer.o $(bld_dir)/pn_viewernav.o \
	$(bld_dir)/pn_viewer_pnac.o\
	$(wb_msg_eobjs) $(rt_msg_eobjs) \
	$(pwr_eobj)/rt_io_user.o \
	$(pwre_conf_libdir) $(pwre_conf_libpwrxttqt) $(pwre_conf_libpwrxtt) $(pwre_conf_libqt) \
	$(pwre_conf_libpwrrt) $(pwre_conf_libpnak) $(pwre_conf_lib)
endif

endif
