ifndef link_rule_mk
link_rule_mk := 1

ifeq ($(export_type),exp)			 
  link = $(ldxx) $(explinkflags) $(domap) -o $(pwr_exe)/wb_gtk \
	$(bld_dir)/wb_gtk.o $(bld_dir)/wb_main.o $(wb_msg_eobjs) $(rt_msg_eobjs) \
	$(pwr_eobj)/rt_io_user.o $(pwr_obj)/wb_procom.o $(pwr_eobj)/stdsoap2.o \
	-L/usr/X11R6/lib -L/usr/local/BerkeleyDB.4.0/lib \
	-L/opt/gnome/lib \
	-lpwr_wb_gtk -lpwr_wb -lpwr_wb_gtk -lpwr_wb -lpwr_rt_gtk -lpwr_rt \
        -lpwr_ge_gtk -lpwr_ge \
	-lpwr_flow_gtk -lpwr_flow -lpwr_glow_gtk -lpwr_glow -lpwr_co_gtk -lpwr_co -lpwr_rt -lpwr_statussrv \
	`pkg-config --libs gtk+-2.0` \
	-lpwr_msg_dummy -lantlr \
	-lrpcsvc -lpthread -lm -ldb_cxx -lz $(linkmysql)
else
  link = $(ldxx) $(elinkflags) $(domap) -o $(pwr_exe)/wb_gtk \
	$(bld_dir)/wb_gtk.o $(bld_dir)/wb_main.o $(wb_msg_eobjs) $(rt_msg_eobjs) \
	$(pwr_eobj)/rt_io_user.o $(pwr_obj)/wb_procom.o $(pwr_eobj)/stdsoap2.o \
	-L/usr/X11R6/lib -L/usr/local/BerkeleyDB.4.0/lib \
	-L/opt/gnome/lib \
	-lpwr_wb_gtk -lpwr_wb -lpwr_rt_gtk -lpwr_rt -lpwr_ge_gtk -lpwr_ge \
	-lpwr_flow_gtk -lpwr_flow -lpwr_glow_gtk -lpwr_glow -lpwr_co_gtk -lpwr_co -lpwr_statussrv -lpwr_rt\
	`pkg-config --libs gtk+-2.0` \
	-lpwr_msg_dummy -lantlr \
	-lrpcsvc -lpthread -lm -ldb_cxx -lz $(linkmysql)
endif
endif
