ifndef link_rule_mk
link_rule_mk := 1

  link = $(ldxx) $(elinkflags) $(domap) -o $(export_exe) \
	$(export_obj) \
	$(pwre_conf_libpwrrt) $(pwre_conf_lib)
#	$(export_obj) $(rt_msg_eobjs) \
#	$(pwr_eobj)/rt_io_user.o -L/usr/X11R6/lib\
#	-L/opt/gnome/lib \
#	-lpwr_xtt -lpwr_ge -lpwr_rt -lpwr_co -lpwr_flow -lpwr_glow \
#	-lpwr_msg_dummy -lImlib -lMrm -lXm -lXpm -lXt -lX11 -lXext -lXp\
#        -lSM -lICE\
#	-ldb -lrpcsvc -lpthread -lm -lrt -lasound
endif
