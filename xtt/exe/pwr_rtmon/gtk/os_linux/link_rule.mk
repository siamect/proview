ifndef link_rule_mk
link_rule_mk := 1

link = $(ldxx) $(elinkflags) $(domap) -o $(pwr_exe)/pwr_rtmon_gtk \
	$(bld_dir)/pwr_rtmon_gtk.o \
	$(pwr_eobj)/stdsoap2.o \
	$(wb_msg_eobjs) $(rt_msg_eobjs) \
         -L/usr/X11R6/lib \
	-L/opt/gnome/lib \
	-lpwr_statussrv -lpwr_ge_gtk -lpwr_ge -lpwr_ge_gtk -lpwr_ge \
	-lpwr_flow_gtk -lpwr_flow -lpwr_glow_gtk -lpwr_glow -lpwr_rt \
	-lpwr_cow_gtk -lpwr_cow -lpwr_co -lpwr_rt -lpwr_co -lpwr_flow_gtk -lpwr_flow \
	`pkg-config --libs gtk+-2.0` \
	-lpwr_msg_dummy -lantlr \
	-lrpcsvc -lpthread -lm -ldb_cxx -lz

endif
