ifndef link_rule_mk
link_rule_mk := 1

ifeq ($(pwre_conf_mq),1)
  link = $(ldxx) $(elinkflags) $(domap) -o $(export_exe) \
	$(export_obj) $(objects) $(rt_msg_eobjs) \
        -lpwr_remote -lpwr_rt -lpwr_co -lpwr_msg_dummy -ldmq -lpthread -lrt -lm
else
  link = echo "Mq not installed"
endif
endif
