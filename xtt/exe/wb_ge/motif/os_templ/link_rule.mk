ifndef link_rule_mk
link_rule_mk := 1

link = $(ldxx) -L$(pwr_elib) $(elinkflags) $(domap) -o $(pwr_exe)/wb_ge_motif \
	$(bld_dir)/wb_ge_motif.o $(wb_msg_eobjs) $(rt_msg_eobjs) $(pwr_eobj)/pwr_msg_glow.o\
	$(pwr_eobj)/rt_io_user.o $(pwr_eobj)/wb_provider.o \
	$(pwre_conf_libpwrwbmotif) $(pwre_conf_libpwrxttmotif) $(pwre_conf_libpwrwb) \
	$(pwre_conf_libpwrxtt) $(pwre_conf_libpwrwbmotif) $(pwre_conf_libpwrwb) \
	$(pwre_conf_libpwrxttmotif) $(pwre_conf_libpwrxtt) $(pwre_conf_libmotif) \
	$(pwre_conf_libpwrrt) $(pwre_conf_lib)

endif
