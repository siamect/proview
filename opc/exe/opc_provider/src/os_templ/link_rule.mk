ifndef link_rule_mk
link_rule_mk := 1

link = $(ldxx) $(elinkflags) $(domap) -o $(export_exe) \
	$(export_obj) $(pwre_conf_libpwropc) $(pwre_conf_libpwrrt) $(pwre_conf_lib)

endif
