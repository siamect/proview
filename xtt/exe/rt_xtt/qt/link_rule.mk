ifndef link_rule_mk
link_rule_mk := 1

ifeq ($(export_type),exp)			 
  link = $(ldxx) $(explinkflags) $(domap) -o $(pwr_exe)/rt_xtt_qt \
	$(bld_dir)/rt_xtt_qt.o $(bld_dir)/rt_xtt_qt_moc.o \
	$(bld_dir)/xtt_main.o $(rt_msg_eobjs) \
	$(pwr_eobj)/rt_io_user.o \
	$(pwre_conf_libdir) $(pwre_conf_libpwrxttqt) $(pwre_conf_libpwrxtt) \
	$(pwre_conf_libpwrxttqt) $(pwre_conf_libpwrxtt) \
	$(pwre_conf_libgst) $(pwre_conf_libqt) \
	$(pwre_conf_libpwrrt) $(pwre_conf_lib)

else
#  link = $(ldxx) $(elinkflags) $(domap) -o $(pwr_exe)/rt_xtt_qt \
#	$(bld_dir)/rt_xtt_qt.o $(bld_dir)/xtt_main.o $(rt_msg_eobjs) \
#	$(pwr_eobj)/rt_io_user.o \
#	$(pwre_conf_libdir) $(pwre_conf_libpwrxttqt) $(pwre_conf_libpwrxtt) \
#	$(pwre_conf_libpwrxttqt) $(pwre_conf_libpwrxtt) \
#	$(pwre_conf_libgst) $(pwre_conf_libqt) \
#	$(pwre_conf_libpwrrt) $(pwre_conf_lib)
  link = :
endif
endif
