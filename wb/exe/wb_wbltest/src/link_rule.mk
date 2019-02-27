ifndef link_rule_mk
link_rule_mk := 1

ifeq ($(export_type),exp)			 
  link = $(ldxx) $(explinkflags) $(domap) -o $(export_exe) \
	$(export_obj) $(objects) $(wb_msg_eobjs) $(rt_msg_eobjs) \
	$(pwr_eobj)/rt_io_user.o $(pwr_obj)/wb_provider.o \
	$(pwre_conf_libpwrwb) $(pwre_conf_libpwrxtt) $(pwre_conf_libpwrrt) \
	$(pwre_conf_libwb) $(pwre_conf_lib)
else
  link = $(ldxx) $(elinkflags) $(domap) -o $(export_exe) \
	$(export_obj) $(objects) $(wb_msg_eobjs) $(rt_msg_eobjs) \
	$(pwr_eobj)/rt_io_user.o $(pwr_obj)/wb_provider.o \
	$(pwre_conf_libpwrwb) $(pwre_conf_libpwrxtt) $(pwre_conf_libpwrrt) \
	$(pwre_conf_libwb) $(pwre_conf_lib)
endif
endif
