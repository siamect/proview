ifndef link_rule_mk
link_rule_mk := 1

link = $(ldxx) $(linkflags) $(domap) -o $(export_exe) \
	$(export_obj) $(objects) $(rt_msg_objs) \
	$(pwr_obj)/rt_io_user.o $(pwr_lib)/libpwr_dtt.a\
	-lpwr_rt -lpwr_co -lpwr_msg_dummy -lrpc -lnetinet -llynx

endif
