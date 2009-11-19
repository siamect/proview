ifndef link_rule_mk
link_rule_mk := 1

ifeq ($(export_type),exp)			 
  link = $(ldxx) $(explinkflags) $(domap) -o $(pwr_exe)/rt_xtt_gtk \
	$(bld_dir)/rt_xtt_gtk.o $(bld_dir)/xtt_main.o $(rt_msg_eobjs) \
	$(pwr_eobj)/rt_io_user.o -L/usr/X11R6/lib\
	-L/opt/gnome/lib \
	-lpwr_xtt_gtk -lpwr_xtt -lpwr_xtt_gtk -lpwr_xtt -lpwr_ge_gtk -lpwr_ge -lpwr_rt \
	-lpwr_co -lpwr_cow_gtk -lpwr_cow -lpwr_flow_gtk -lpwr_flow -lpwr_glow_gtk -lpwr_glow \
	`pkg-config --libs gtk+-2.0` \
	-lpwr_msg_dummy \
	-ldb -lrpcsvc -lpthread -lm -lrt -lcrypt -lasound
else
  link = $(ldxx) $(elinkflags) $(domap) -o $(pwr_exe)/rt_xtt_gtk \
	$(bld_dir)/rt_xtt_gtk.o $(bld_dir)/xtt_main.o $(rt_msg_eobjs) \
	$(pwr_eobj)/rt_io_user.o -L/usr/X11R6/lib\
	-L/opt/gnome/lib \
	-lpwr_xtt_gtk -lpwr_xtt -lpwr_xtt_gtk -lpwr_xtt -lpwr_ge_gtk -lpwr_ge -lpwr_rt \
	-lpwr_co -lpwr_cow_gtk -lpwr_cow -lpwr_flow_gtk -lpwr_flow -lpwr_glow_gtk -lpwr_glow \
	`pkg-config --libs gtk+-2.0` \
	-lpwr_msg_dummy \
	-ldb -lrpcsvc -lpthread -lm -lrt -lcrypt -lasound
endif
endif
