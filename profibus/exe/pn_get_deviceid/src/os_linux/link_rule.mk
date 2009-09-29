ifndef link_rule_mk
link_rule_mk := 1

link = $(ldxx) $(elinkflags) $(domap) -o $(export_exe) \
	$(export_obj) -lpwr_co -lrt

endif
