ifndef link_rule_mk
link_rule_mk := 1

link = $(ldxx) $(linkflags) $(domap) -o $(export_exe) \
	$(export_obj) $(objects) $(rt_msg_objs) $(pwr_obj)/stdsoap2.o\
	-lpwr_statussrv \
	$(pwre_conf_libpwrrt) $(pwre_conf_lib)

endif
