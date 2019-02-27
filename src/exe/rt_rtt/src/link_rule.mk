ifndef link_rule_mk
link_rule_mk := 1

link = $(ldxx) $(linkflags) $(domap) -o $(export_exe) \
	$(export_obj) $(objects) $(rt_msg_objs) $(pwr_obj)/dtt_rttsys.o\
	$(pwr_obj)/rt_io_user.o $(pwre_conf_libdir) $(pwr_conf_libpwrdtt) -L$(pwr_elib)\
	$(pwre_conf_libpwrdtt) $(pwre_conf_libpwrrt) $(pwre_conf_lib)

endif
