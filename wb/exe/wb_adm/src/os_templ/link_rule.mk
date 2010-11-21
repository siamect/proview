ifndef link_rule_mk
link_rule_mk := 1

link = $(ldxx) $(linkflags) $(domap) -o $(export_exe) \
	$(export_obj) $(objects) $(wb_msg_objs) $(rt_msg_objs) \
	$(pwr_obj)/rt_io_user.o \
	$(pwre_conf_libpwrwb) $(pwre_conf_libpwrxtt) $(pwre_conf_libpwrrt) $(pwre_conf_lib)

endif
