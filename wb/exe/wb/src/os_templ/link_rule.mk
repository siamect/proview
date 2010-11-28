ifndef link_rule_mk
link_rule_mk := 1

ifeq ($(export_type),exp)			 
  link = $(ldxx) $(explinkflags) $(domap) -o $(export_exe) \
	$(bld_dir)/wb.o \
	$(pwre_conf_libdir) $(pwre_conf_libpwrrt) $(pwre_conf_lib)

#-lpwr_co
else
  link = $(ldxx) $(elinkflags) $(domap) -o $(export_exe) \
	$(bld_dir)/wb.o \
	$(pwre_conf_libdir) $(pwre_conf_libpwrrt) $(pwre_conf_lib)

#-lpwr_co
endif
endif
