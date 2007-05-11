ifndef link_rule_mk
link_rule_mk := 1

link = $(ldxx) $(elinkflags) $(domap) -o $(pwr_exe)/rt_statusmon_gtk \
	$(bld_dir)/rt_statusmon_gtk.o \
	$(bld_dir)/statusmon_nodelist_gtk.o $(bld_dir)/statusmon_nodelistnav_gtk.o \
	$(bld_dir)/statusmon_nodelist.o $(bld_dir)/statusmon_nodelistnav.o \
	$(pwr_eobj)/stdsoap2.o \
	$(wb_msg_eobjs) $(rt_msg_eobjs) \
         -L/usr/X11R6/lib \
	-L/opt/gnome/lib \
	-lpwr_statussrv -lpwr_ge_gtk -lpwr_ge -lpwr_wb_gtk -lpwr_wb -lpwr_wb_gtk -l pwr_wb -lpwr_ge_gtk -lpwr_ge \
	-lpwr_flow_gtk -lpwr_flow -lpwr_glow_gtk -lpwr_glow -lpwr_rt_gtk -lpwr_rt \
	-lpwr_co_gtk -lpwr_co -lpwr_flow_gtk -lpwr_flow \
	`pkg-config --libs gtk+-2.0` \
	-lpwr_msg_dummy -lantlr \
	-lrpcsvc -lpthread -lm -ldb_cxx -lz

endif
