ifndef link_rule_mk
link_rule_mk := 1

link = $(ld) $(linkflags) $(domap) -o $(export_exe) \
	$(export_obj) $(objects) $(rt_msg_objs) \
	-lpwr_rt -lpwr_co -lpwr_msg_dummy -lpthread -lm -lrpcsvc

endif
