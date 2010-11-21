ifndef link_rule_mk
link_rule_mk := 1

link = $(ldxx) $(linkflags) $(domap) -o $(export_exe) \
	$(export_obj) $(objects) $(rt_msg_objs)\
	$(pwr_obj)/rt_io_user.o -L$(pwr_elib) \
	$(pwre_conf_libpwrrt) $(pwre_conf_lib)

endif
