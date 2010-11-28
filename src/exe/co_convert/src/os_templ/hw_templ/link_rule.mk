ifndef link_rule_mk
link_rule_mk := 1

link = $(ldxx) $(linkflags) $(domap) -o $(export_exe) $(export_obj) \
	$(objects) $(pwre_conf_libdir) $(pwre_conf_libpwrco) $(pwre_conf_lib) $(pwre_conf_libgtk);\
	cp $(export_exe) $(eexe_dir)/

endif
