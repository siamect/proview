ifndef link_rule_mk
link_rule_mk := 1

link = $(ldxx) -g -L$(lib_dir) $(domap) -o $(export_exe) \
	$(export_obj) $(objects) -lpwr_co \
	-llynx

endif
