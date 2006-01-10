ifndef link_rule_mk
link_rule_mk := 1

link = $(ld) $(elinkflags) $(domap) -o $(export_exe) \
	$(export_obj) $(objects) $(rt_msg_objs) \
	$(pwr_eobj)/rt_io_user.o -lpwr_rs\
	-lpwr_rt -lpwr_co -lpwr_msg_dummy -lrpcsvc -lpthread -lm -lrt

endif
