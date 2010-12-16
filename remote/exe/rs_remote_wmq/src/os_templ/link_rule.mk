ifndef link_rule_mk
link_rule_mk := 1

ifeq ($(PWRE_CONF_WMQ),1)
  link = $(ldxx) $(elinkflags) $(domap) -o $(export_exe) \
	$(export_obj) $(objects) $(rt_msg_eobjs) \
        $(pwre_conf_libdir) $(pwre_conf_libpwrremote) $(pwre_conf_libpwrrt) $(pwre_conf_lib)
else
  link = echo "WMQ not installed"
endif
endif
