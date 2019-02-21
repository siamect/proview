include $(pwre_dir_symbols)

ifndef variables_mk
 include $(pwre_croot)/src/tools/bld/src/variables.mk
endif

ifndef rules_mk
 include $(pwre_croot)/src/tools/bld/src/rules.mk
endif

vpath %.c $(co_source)
vpath %.dat $(co_source)
vpath %.gif $(co_source)

source_dirs := $(co_source)

sources := $(sort \
             $(foreach file, \
               $(foreach dir, \
                 $(source_dirs), \
                 $(wildcard $(dir)/*.c) \
               ), $(notdir $(file)) \
             ) \
           )

c_sources := $(filter %.c,$(sources))

sources := $(sort \
             $(foreach file, \
               $(foreach dir, \
                 $(source_dirs), \
                 $(wildcard $(dir)/*.gif) \
               ), $(notdir $(file)) \
             ) \
           )

gif_sources := $(filter %.gif,$(sources))

sources := $(sort \
             $(foreach file, \
               $(foreach dir, \
                 $(source_dirs), \
                 $(wildcard $(dir)/*.wav) \
               ), $(notdir $(file)) \
             ) \
           )

wav_sources := $(filter %.wav,$(sources))

export_obj := $(patsubst %.c,$(obj_dir)/%.o,$(c_sources))
clean_obj := $(patsubst %.c,clean_%.o,$(c_sources))
export_gif := $(patsubst %.gif,$(exe_dir)/%.gif,$(gif_sources))
export_gifdoc := $(patsubst %.gif,$(doc_dir)/help/%.gif,$(gif_sources))
export_wav := $(patsubst %.wav,$(exe_dir)/%.wav,$(wav_sources))

.PHONY : all init copy lib exe clean realclean\
         $(clean_c)

all : init copy lib | silent

init :

copy : $(load_dir)/pwr_volumelist.dat $(exe_dir)/sv_se/pwrb_xtthelp.dat $(exe_dir)/pwr_logga.gif $(export_gif) $(export_gifdoc) $(export_wav) | silent

lib : $(export_obj) | silent

exe :

clean :

realclean : clean $(clean_obj)

silent :
	@ :

.SUFFIXES:

$(exe_dir)/%.wav : %.wav
	@ $(log_h_h)
	@ $(cp) $(cpflags) $(source) $(target)

$(load_dir)/pwr_volumelist.dat : pwr_volumelist.dat
	@ echo "Copying pwr_volumelist.dat"
	@ $(cp) $(cpflags) $(source) $(target)

$(exe_dir)/sv_se/pwrb_xtthelp.dat : pwrb_xtthelp.dat_sv_se
	@ #echo "Copying pwrb_xtthelp.dat sv_se"
	@ #$(cp) $(cpflags) $(source) $(target)
	@ #co_convert -t -d $(pwr_doc)/sv_se $(source)

$(exe_dir)/pwr_logga.gif : pwr_logga.gif
	@ echo "Copying pwr_logga.gif"
	@ $(cp) $(cpflags) $(source) $(target)

$(exe_dir)/%.gif : %.gif
	@ echo "Copying $(source).gif exe"
	@ $(cp) $(cpflags) $(source) $(target)

$(doc_dir)/help/%.gif : %.gif
	@ echo "Copying $(source).gif doc/help"
	@ $(cp) $(cpflags) $(source) $(target)

$(clean_obj) : clean_%.o : %.c
	rm $(obj_dir)/$*.o
