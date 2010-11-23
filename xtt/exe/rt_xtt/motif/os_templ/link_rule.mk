ifndef link_rule_mk
link_rule_mk := 1

ifeq ($(export_type),exp)			 
  link = $(ldxx) $(explinkflags) $(domap) -o $(pwr_exe)/rt_xtt_motif \
	$(bld_dir)/rt_xtt_motif.o $(bld_dir)/xtt_main.o $(rt_msg_eobjs) \
	$(pwr_eobj)/rt_io_user.o \
	$(pwre_conf_libpwrxttmotif) $(pwre_conf_libpwrxtt) \
	$(pwre_conf_libpwrxttmotif) $(pwre_conf_libpwrxtt) \
	$(pwre_conf_libmotif) \
	$(pwre_conf_libpwrrt) $(pwre_conf_lib)

else
  link = $(ldxx) $(elinkflags) $(domap) -o $(pwr_exe)/rt_xtt_motif \
	$(bld_dir)/rt_xtt_motif.o $(bld_dir)/xtt_main.o $(rt_msg_eobjs) \
	$(pwr_eobj)/rt_io_user.o \
	$(pwre_conf_libpwrxttmotif) $(pwre_conf_libpwrxtt) \
	$(pwre_conf_libpwrxttmotif) $(pwre_conf_libpwrxtt) \
	$(pwre_conf_libmotif) \
	$(pwre_conf_libpwrrt) $(pwre_conf_lib)

endif
endif
