ifndef link_rule_mk
link_rule_mk := 1

ifeq ($(export_type),exp)			 
  link = $(ldxx) $(explinkflags) $(domap) -o $(export_exe) \
	$(export_obj) $(objects) $(rt_msg_objs) \
	$(pwr_obj)/rt_io_user.o \
	-lpwr_rt -lpwr_co -lpwr_msg_dummy -lpwr_usbio_dummy -lpwr_usb_dummy -lpthread  -lm -lpnioif
else
  link = $(ldxx) $(elinkflags) $(domap) -o $(export_exe) \
	$(export_obj) $(objects) $(rt_msg_objs) \
	$(pwr_obj)/rt_io_user.o \
	-lpwr_rt -lpwr_co -lpwr_msg_dummy -lpwr_usbio_dummy -lpwr_usb_dummy -lpthread  -lm -lpnioif
endif
endif
