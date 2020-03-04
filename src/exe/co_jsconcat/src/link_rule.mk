ifndef link_rule_mk
link_rule_mk := 1

link = $(ldxx) $(linkflags) $(domap) -o $(export_exe) $(export_obj) \
	$(objects) -lpwr_co $(pwre_conf_libdir) $(pwre_conf_lib)

endif
