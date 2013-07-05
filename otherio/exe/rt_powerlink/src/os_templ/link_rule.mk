ifndef link_rule_mk
link_rule_mk := 1

  link = $(ldxx) $(explinkflags) $(domap) -o $(export_exe) \
	$(export_obj) $(pwr_eobj)/rt_io_user.o \
	$(pwre_conf_libdir) $(pwre_conf_libpwrrt) $(pwre_conf_lib) \
	-lpwr_usbio_dummy -lpwr_usb_dummy -lpwr_cifx_dummy -lpwr_nodave_dummy \
	$(pwre_conf_libpwrprofibus) $(pwre_conf_libpwrpowerlink) -lpcap
endif
