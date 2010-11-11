ifndef link_rule_mk
link_rule_mk := 1

link = $(ldxx) -L$(pwr_elib) $(elinkflags) $(domap) -o $(pwr_exe)/profinet_viewer_gtk \
	$(bld_dir)/profinet_viewer_gtk.o \
	$(bld_dir)/pn_viewer_gtk.o $(bld_dir)/pn_viewernav_gtk.o \
	$(bld_dir)/pn_viewer.o $(bld_dir)/pn_viewernav.o $(bld_dir)/pn_viewer_pnac.o\
	$(wb_msg_eobjs) $(rt_msg_eobjs) \
	$(pwr_eobj)/rt_io_user.o -L/usr/X11R6/lib \
	-L/opt/gnome/lib \
	-lpwr_flow_gtk -lpwr_flow -lpwr_cow_gtk -lpwr_cow -lpwr_rt \
	-lpwr_co -lpwr_flow_gtk -lpwr_flow \
	`pkg-config --libs gtk+-2.0` \
	-lpwr_msg_dummy -lantlr \
	-lcrypt -lrpcsvc -lpthread -lm -ldb_cxx -lz -lpnioif

endif
