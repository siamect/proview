ifndef link_rule_mk
link_rule_mk := 1

ifeq ($(export_type),exp)			 
  link = $(ldxx) $(explinkflags) $(domap) -o $(pwr_exe)/rt_xtt_motif \
	$(bld_dir)/rt_xtt_motif.o $(bld_dir)/xtt_main.o $(rt_msg_eobjs) \
	$(pwr_eobj)/rt_io_user.o 
	$(pwre_conf_libpwrxttmotif) $(pwre_conf_libpwrxtt) $(pwre_conf_libmotif) \
	$(pwre_conf_libpwrrt) $(pwre_conf_lib)

#-L/usr/X11R6/lib\
#	-L/opt/gnome/lib \
#	-lpwr_xtt_motif -lpwr_xtt -lpwr_xtt_motif -lpwr_xtt -lpwr_ge_motif -lpwr_ge -lpwr_rt \
#	-lpwr_cow_motif -lpwr_cow -lpwr_co -lpwr_flow_motif -lpwr_flow -lpwr_glow_motif -lpwr_glow \
#	-lpwr_msg_dummy -lImlib -lMrm -lXm -lXpm -lXt -lX11 -lXext -lXp\
#        -lSM -lICE\
#	-ldb -lrpcsvc -lpthread -lm -lrt -lasound -lcrypt
else
  link = $(ldxx) $(elinkflags) $(domap) -o $(pwr_exe)/rt_xtt_motif \
	$(bld_dir)/rt_xtt_motif.o $(bld_dir)/xtt_main.o $(rt_msg_eobjs) \
	$(pwr_eobj)/rt_io_user.o \
	$(pwre_conf_libpwrxttmotif) $(pwre_conf_libpwrxtt) $(pwre_conf_libmotif) \
	$(pwre_conf_libpwrrt) $(pwre_conf_lib)

#-L/usr/X11R6/lib\
#	-L/opt/gnome/lib \
#	-lpwr_xtt_motif -lpwr_xtt -lpwr_xtt_motif -lpwr_xtt -lpwr_ge_motif -lpwr_ge -lpwr_rt \
#	-lpwr_cow_motif -lpwr_cow -lpwr_co -lpwr_flow_motif -lpwr_flow -lpwr_glow_motif -lpwr_glow \
#	-lpwr_msg_dummy -lImlib -lMrm -lXm -lXpm -lXt -lX11 -lXext -lXp\
#        -lSM -lICE\
#	-ldb -lrpcsvc -lpthread -lm -lrt -lasound -lcrypt
endif
endif
