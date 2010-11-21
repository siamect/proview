ifndef link_rule_mk
link_rule_mk := 1

link = $(ldxx) -L$(pwr_elib) $(elinkflags) $(domap) -o $(pwr_exe)/wb_ge_motif \
	$(bld_dir)/wb_ge_motif.o $(wb_msg_eobjs) $(rt_msg_eobjs) $(pwr_eobj)/pwr_msg_glow.o\
	$(pwr_eobj)/rt_io_user.o $(pwr_eobj)/wb_provider.o \
	$(pwre_conf_libpwrwbmotif) $(pwre_conf_libpwrxttmotif) $(pwre_conf_libpwrwb) \
	$(pwre_conf_libpwrxtt) $(pwre_conf_libpwrwbmotif) $(pwre_conf_libpwrwb) \
	$(pwre_conf_libmotif) \
	$(pwre_conf_libpwrrt) $(pwre_conf_lib)

#link = $(ldxx) -L$(pwr_elib) $(elinkflags) $(domap) -o $(pwr_exe)/wb_ge_motif \
#	$(bld_dir)/wb_ge_motif.o $(wb_msg_eobjs) $(rt_msg_eobjs) \
#	$(pwr_eobj)/rt_io_user.o $(pwr_eobj)/wb_provider.o -L/usr/X11R6/lib \
#	-L/usr/local/BerkeleyDB.4.0/lib \
#	-L/opt/gnome/lib \
#	-lpwr_ge_motif -lpwr_ge -lpwr_wb_motif -lpwr_wb -lpwr_wb_motif -lpwr_wb -lpwr_flow_motif -lpwr_flow \
#	-lpwr_glow_motif -lpwr_glow -lpwr_rt -lpwr_cow_motif -lpwr_cow -lpwr_co \
#	-lpwr_msg_dummy -lantlr -lImlib -lMrm -lXm -lXpm -lXt -lX11 -lXext -lXp\
#        -lXmu -lSM -lICE\
#	-lcrypt -lrpcsvc -lpthread -lm -ldb_cxx -lz $(linkmysql)

endif
