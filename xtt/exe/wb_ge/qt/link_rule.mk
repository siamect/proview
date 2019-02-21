ifndef link_rule_mk
link_rule_mk := 1

# Link with local wb to avoid profibus wb methods

link = $(ldxx) $(cross_compile) \
	-L$(release_root)/wb/lib -L$(pwr_lib) -L$(pwr_elib) $(domap) -o $(pwr_exe)/wb_ge_qt \
	$(bld_dir)/wb_ge_qt.o $(wb_msg_eobjs) $(rt_msg_eobjs) $(pwr_eobj)/pwr_msg_glow.o\
	$(pwr_eobj)/rt_io_user.o $(pwr_eobj)/wb_provider.o \
	$(pwre_conf_libdir) $(pwre_conf_libpwrwbqt) $(pwre_conf_libpwrxttqt) $(pwre_conf_libpwrwb) \
	$(pwre_conf_libpwrxtt) $(pwre_conf_libpwrwbqt) $(pwre_conf_libpwrwb) \
	$(pwre_conf_libpwrxttqt) $(pwre_conf_libpwrxtt) $(pwre_conf_libqt) \
	$(pwre_conf_libpwrrt) $(pwre_conf_libwb) $(pwre_conf_lib)

endif
