ifndef link_rule_mk
link_rule_mk := 1

link = $(ldxx) $(elinkflags) $(domap) -o $(export_exe) \
	$(export_obj) $(objects) $(wb_msg_eobjs) $(rt_msg_eobjs) \
	$(pwr_eobj)/rt_io_user.o $(pwr_obj)/wb_provider.o \
	-L/usr/local/BerkeleyDB.4.0/lib \
	-lpwr_wb -lpwr_rt -lpwr_ge -lpwr_flow -lpwr_glow -lpwr_cow -lpwr_co \
	-lpwr_msg_dummy -lantlr \
	-lrpcsvc -lpthread -lm -lrt -lcrypt -ldb_cxx -lz $(linkmysql)

endif
