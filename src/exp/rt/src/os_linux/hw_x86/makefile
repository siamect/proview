include $(pwre_dir_symbols)

ifndef variables_mk
 -include $(pwre_sroot)/tools/bld/src/$(os_name)/$(hw_name)/variables.mk
endif
ifndef variables_mk
 include $(pwre_sroot)/tools/bld/src/$(os_name)/variables.mk
endif


ifndef rules_mk
 -include $(pwre_sroot)/tools/bld/src/$(os_name)/$(hw_name)/rules.mk
endif
ifndef rules_mk
 include $(pwre_sroot)/tools/bld/src/$(os_name)/rules.mk
endif

vpath %.c $(hw_source):$(os_source):$(co_source)
vpath %.dat $(hw_source):$(os_source):$(co_source)

source_dirs = $(hw_source) $(os_source) $(co_source)

sources := $(sort \
             $(foreach file, \
               $(foreach dir, \
                 $(source_dirs), \
                 $(wildcard $(dir)/*.c) \
               ), $(notdir $(file)) \
             ) \
           )

c_sources := $(filter %.c,$(sources))

export_obj := $(patsubst %.c,$(obj_dir)/%.o,$(c_sources))
clean_obj := $(patsubst %.c,clean_%.o,$(c_sources))

.PHONY : all init copy lib exe clean realclean\
         $(clean_c)

all : init copy lib

init :

copy : $(exe_dir)/xtt_help.dat $(exe_dir)/sv_se/pwrb_xtthelp.dat $(exe_dir)/pwr_logga.gif

lib : $(export_obj)

exe :

clean :

realclean : clean $(clean_obj) 

$(exe_dir)/xtt_help.dat : xtt_help.dat
	@ echo "Copying xtt_help.dat"
	@ $(cp) $(cpflags) $(source) $(target)
	@ co_convert -t -d $(pwr_doc)/help $(source)

$(exe_dir)/sv_se/pwrb_xtthelp.dat : ../../pwrb_xtthelp.dat_sv_se
	@ echo "Copying pwrb_xtthelp.dat sv_se"
	@ $(cp) $(cpflags) $(source) $(target)
	@ co_convert -t -d $(pwr_doc)/sv_se $(source)

$(exe_dir)/pwr_logga.gif : ../../pwr_logga.gif
	@ echo "Copying pwr_logga.gif"
	@ $(cp) $(cpflags) $(source) $(target)


$(clean_obj) : clean_%.o : %.c
	rm $(obj_dir)/$*.o


