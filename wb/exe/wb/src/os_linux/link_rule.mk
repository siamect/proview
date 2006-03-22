ifndef link_rule_mk
link_rule_mk := 1

ifeq ($(export_type),exp)			 
  link = $(ldxx) $(explinkflags) $(domap) -o $(export_exe) \
	$(export_obj) $(objects) $(wb_msg_eobjs) $(rt_msg_eobjs) \
	$(pwr_eobj)/rt_io_user.o $(pwr_obj)/wb_provider.o \
	-L/usr/X11R6/lib -L/usr/local/BerkeleyDB.4.0/lib \
	-L/opt/gnome/lib \
	-lpwr_wb -lpwr_pb -lpwr_rt -lpwr_ge -lpwr_flow -lpwr_glow -lpwr_co \
	-lpwr_msg_dummy -lantlr -lImlib -lMrm -lXm -lXpm -lXt -lX11 -lXext -lXp\
        -lXmu -lSM -lICE\
	-lrpcsvc -lpthread -lm -ldb_cxx -lz
else
  link = $(ldxx) $(elinkflags) $(domap) -o $(export_exe) \
	$(export_obj) $(objects) $(wb_msg_eobjs) $(rt_msg_eobjs) \
	$(pwr_eobj)/rt_io_user.o $(pwr_obj)/wb_provider.o \
	-L/usr/X11R6/lib -L/usr/local/BerkeleyDB.4.0/lib \
	-L/opt/gnome/lib \
	-lpwr_wb -lpwr_rt -lpwr_ge -lpwr_flow -lpwr_glow -lpwr_co \
	-lpwr_msg_dummy -lantlr -lImlib -lMrm -lXm -lXpm -lXt -lX11 -lXext -lXp\
        -lXmu -lSM -lICE\
	-lrpcsvc -lpthread -lm -ldb_cxx -lz
endif
endif
