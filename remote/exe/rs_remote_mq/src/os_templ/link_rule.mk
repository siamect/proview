ifndef link_rule_mk
link_rule_mk := 1

ifeq ($(pwre_conf_mq),1)
  link = $(ldxx) $(elinkflags) $(domap) -o $(export_exe) \
	$(export_obj) $(objects) $(rt_msg_eobjs) \
        $(pwre_conf_libdir) $(pwre_conf_libpwrremote) $(pwre_conf_libpwrrt) \
	$(pwre_conf_libmq) $(pwre_conf_lib)
else
  link = echo "Mq not installed"
endif
endif
