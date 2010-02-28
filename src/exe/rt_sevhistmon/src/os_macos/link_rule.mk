ifndef link_rule_mk
link_rule_mk := 1

link = $(ldxx) $(elinkflags) $(domap) -o $(export_exe) \
	$(export_obj) $(objects) $(wb_msg_objs) $(rt_msg_objs) \
	$(pwr_obj)/rt_io_user.o \
	-lpwr_rt -lpwr_co $(linkmysql)\
	-lpwr_msg_dummy \
	-lrpcsvc -lpthread -lm -ldb_cxx -lz

#-lpwr_wb -lpwr_rt -lpwr_ge -lpwr_flow -lpwr_glow -lpwr_co \
#	-L/usr/X11R6/lib -L/usr/local/BerkeleyDB.4.0/lib \
#	-L/opt/gnome/lib \
# -lantlr -lImlib -lMrm -lXm -lXpm -lXt -lX11 -lXext -lXp\
#        -lXmu -lSM -lICE\

endif
