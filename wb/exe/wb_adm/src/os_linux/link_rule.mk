ifndef link_rule_mk
link_rule_mk := 1

link = $(ldxx) $(linkflags) $(domap) -o $(export_exe) \
	$(export_obj) $(objects) $(wb_msg_objs) $(rt_msg_objs) \
	$(pwr_obj)/rt_io_user.o -L/usr/X11R6/lib -L/usr/lib/mysql \
	-lpwr_wb -lpwr_rt -lpwr_ge -lpwr_flow -lpwr_glow -lpwr_co \
	-lpwr_msg_dummy -lImlib -lMrm -lXm -lXpm -lXt -lX11 -lXext -lXp\
        -lSM -lICE\
	-lrpcsvc -lpthread -lm -lrt

endif
