ifndef link_rule_mk
link_rule_mk := 1

ifeq ($(export_type),exp)			 
  link = $(ldxx) $(explinkflags) $(domap) -o $(pwr_exe)/wb_gtk \
	$(bld_dir)/wb_gtk.o $(bld_dir)/wb_main.o $(wb_msg_eobjs) $(rt_msg_eobjs) \
	$(pwr_eobj)/rt_io_user.o $(pwr_obj)/wb_procom.o \
	-L/usr/X11R6/lib -L/usr/local/BerkeleyDB.4.0/lib \
	-L/opt/gnome/lib \
	-lpwr_wb_gtk -lpwr_wb -lpwr_wb_gtk -lpwr_wb -lpwr_rt_gtk -lpwr_rt -lpwr_ge_gtk -lpwr_ge \
	-lpwr_flow_gtk -lpwr_flow -lpwr_glow_gtk -lpwr_glow -lpwr_co_gtk -lpwr_co \
	`pkg-config --libs gtk+-2.0` \
	-lpwr_msg_dummy -lantlr -lgdk_imlib -lMrm -lXm -lXpm -lXt -lX11 -lXext -lXp\
        -lXmu -lSM -lICE\
	-lrpcsvc -lpthread -lm -ldb_cxx -lz
else
  link = $(ldxx) $(elinkflags) $(domap) -o $(pwr_exe)/wb_gtk \
	$(bld_dir)/wb_gtk.o $(bld_dir)/wb_main.o $(wb_msg_eobjs) $(rt_msg_eobjs) \
	$(pwr_eobj)/rt_io_user.o $(pwr_obj)/wb_procom.o \
	-L/usr/X11R6/lib -L/usr/local/BerkeleyDB.4.0/lib \
	-L/opt/gnome/lib \
	-lpwr_wb_gtk -lpwr_wb -lpwr_rt_gtk -lpwr_rt -lpwr_ge_gtk -lpwr_ge \
	-lpwr_flow_gtk -lpwr_flow -lpwr_glow_gtk -lpwr_glow -lpwr_co_gtk -lpwr_co \
	`pkg-config --libs gtk+-2.0` \
	-lpwr_msg_dummy -lantlr -lgdk_imlib -lMrm -lXm -lXpm -lXt -lX11 -lXext -lXp\
        -lXmu -lSM -lICE\
	-lrpcsvc -lpthread -lm -ldb_cxx -lz
endif
endif
