ifndef link_rule_mk
link_rule_mk := 1

link = $(ld) $(linkflags) $(domap) -o $(export_exe) $(export_obj) $(objects) -lfl

endif
