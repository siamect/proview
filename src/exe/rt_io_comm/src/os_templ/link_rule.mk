ifndef link_rule_mk
link_rule_mk := 1

ifeq ($(export_type),exp)			 
  link = $(ldxx) $(explinkflags) $(domap) -o $(export_exe) \
	$(export_obj) $(objects) $(rt_msg_objs) \
	$(pwr_obj)/rt_io_user.o \
	$(pwre_conf_libdir) $(pwre_conf_libpwrrt) $(pwre_conf_lib) \
	$(pwre_conf_libpwrprofibus) $(pwre_conf_libpwrotherio)
else
  link = $(ldxx) $(elinkflags) $(domap) -o $(export_exe) \
	$(export_obj) $(objects) $(rt_msg_objs) \
	$(pwr_obj)/rt_io_user.o \
	$(pwre_conf_libdir) $(pwre_conf_libpwrrt) $(pwre_conf_lib)
endif
endif
