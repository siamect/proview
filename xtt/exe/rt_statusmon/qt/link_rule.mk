ifndef link_rule_mk
link_rule_mk := 1

link = $(ldxx) $(elinkflags) $(domap) -o $(pwr_exe)/rt_statusmon_qt \
	$(bld_dir)/rt_statusmon_qt.o \
	$(pwr_eobj)/stdsoap2.o \
	$(wb_msg_eobjs) $(rt_msg_eobjs) \
	$(pwre_conf_libdir) $(pwre_conf_libpwrxttqt) $(pwre_conf_libpwrxtt) $(pwre_conf_libqt) \
	$(pwre_conf_libpwrrt) $(pwre_conf_lib)

endif
