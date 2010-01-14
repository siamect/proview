ifndef link_rule_mk
link_rule_mk := 1

link = $(ldxx) $(linkflags) $(domap) -o $(export_exe) $(export_obj) \
	$(objects) -L/usr/X11R6/lib -L/opt/gnome/lib -lpwr_co \
	$(linkgtk) \
	-lX11 -lrt -lm

endif
