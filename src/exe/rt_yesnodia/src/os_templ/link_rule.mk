ifndef link_rule_mk
link_rule_mk := 1

link = $(ldxx) $(linkflags) $(domap) -o $(export_exe) \
	$(export_obj) $(objects) -L/usr/X11R6/lib\
	-lMrm -lXm -lXpm -lXt -lX11 -lXext -lXp\
        -lSM -lICE

endif
