ifndef link_rule_mk
link_rule_mk := 1

ifeq ($(export_type),exp)			 
  link = $(ld) $(explinkflags) $(domap) -o $(export_exe) \
	$(export_obj) $(objects) $(rt_msg_objs) \
	$(pwr_obj)/rt_io_user.o \
	-lpwr_rt -lpwr_co -lpwr_msg_dummy -lpthread  -lm
else
  link = $(ld) $(elinkflags) $(domap) -o $(export_exe) \
	$(export_obj) $(objects) $(rt_msg_objs) \
	$(pwr_obj)/rt_io_user.o \
	-lpwr_rt -lpwr_co -lpwr_msg_dummy -lpthread  -lm
endif
endif
