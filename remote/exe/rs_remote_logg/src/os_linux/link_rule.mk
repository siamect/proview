ifndef link_rule_mk
link_rule_mk := 1

link = $(ldxx) $(elinkflags) $(domap) -o $(export_exe) \
	$(export_obj) $(objects) $(rt_msg_eobjs) $(rs_msg_eobjs) \
        -lpwr_remote -lpwr_rt -lpwr_co -lpwr_msg_dummy -lpthread -lrt -lm

endif
