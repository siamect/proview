ifndef link_rule_mk
link_rule_mk := 1

link = $(ldxx) $(linkflags) $(domap) -o $(export_exe) \
	$(export_obj) $(objects) $(rt_msg_objs) \
	$(pwr_obj)/rt_io_user.o -L/usr/X11R6/lib -L/usr/lib/mysql \
	-lpwr_ge -lpwr_wb -lpwr_rt -lpwr_co -lpwr_flow -lpwr_glow \
	-lpwr_msg_dummy -lantlr -lImlib -lMrm -lXm -lXpm -lXt -lX11 -lXext -lXp\
        -lXmu -lSM -lICE\
        -lrpcsvc -lpthread -ldb_cxx-4.0 -lm -lmysqlclient -lz

endif
