ifndef link_rule_mk
link_rule_mk := 1

link = $(ldxx) $(elinkflags) $(domap) -o $(export_exe) \
	$(export_obj) -lpwr_ge -lpwr_glow -lpwr_rt -lpwr_co -lpwr_flow -lpwr_msg_dummy

endif
