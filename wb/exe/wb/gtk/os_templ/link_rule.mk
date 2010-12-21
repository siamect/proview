ifndef link_rule_mk
link_rule_mk := 1

ifeq ($(export_type),exp)			 
  link = $(ldxx) $(explinkflags) $(domap) -o $(pwr_exe)/wb_gtk \
	$(bld_dir)/wb_gtk.o $(bld_dir)/wb_main.o $(wb_msg_eobjs) $(rt_msg_eobjs) \
	$(pwr_eobj)/rt_io_user.o $(pwr_obj)/wb_procom.o $(pwr_eobj)/stdsoap2.o \
	$(pwre_conf_libdir) $(pwre_conf_libpwrwbgtk) $(pwre_conf_libpwrxttgtk) $(pwre_conf_libpwrwb) \
	$(pwre_conf_libpwrxtt) $(pwre_conf_libpwrwbgtk) $(pwre_conf_libpwrwb) \
	$(pwre_conf_libpwrxttgtk) $(pwre_conf_libpwrxtt) $(pwre_conf_libgtk) \
	$(pwre_conf_libpwrrt) $(pwre_conf_libwb) $(pwre_conf_lib)

else
  link = $(ldxx) $(elinkflags) $(domap) -o $(pwr_exe)/wb_gtk \
	$(bld_dir)/wb_gtk.o $(bld_dir)/wb_main.o $(wb_msg_eobjs) $(rt_msg_eobjs) \
	$(pwr_eobj)/rt_io_user.o $(pwr_obj)/wb_procom.o $(pwr_eobj)/stdsoap2.o \
	$(pwre_conf_libdir) $(pwre_conf_libpwrwbgtk) $(pwre_conf_libpwrxttgtk) $(pwre_conf_libpwrwb) \
	$(pwre_conf_libpwrxtt) $(pwre_conf_libpwrwbgtk) $(pwre_conf_libpwrwb) \
	$(pwre_conf_libgtk) \
	$(pwre_conf_libpwrrt) $(pwre_conf_libwb) $(pwre_conf_lib)

endif
endif
