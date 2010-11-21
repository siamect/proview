ifndef link_rule_mk
link_rule_mk := 1

link = $(ldxx) $(elinkflags) $(domap) -o $(export_exe) \
	$(export_obj) $(objects) $(wb_msg_eobjs) $(rt_msg_eobjs) \
	$(pwr_eobj)/rt_io_user.o $(pwr_obj)/wb_provider.o \
	$(pwre_conf_libpwrwb) $(pwre_conf_libpwrxtt) $(pwre_conf_libpwrrt) $(pwre_conf_lib)

endif
