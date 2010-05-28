include $(pwre_dir_symbols)

ifndef variables_mk
 -include $(pwre_croot)/src/tools/bld/src/$(os_name)/$(hw_name)/variables.mk
endif
ifndef variables_mk
 include $(pwre_croot)/src/tools/bld/src/$(os_name)/variables.mk
endif


ifndef rules_mk
 -include $(pwre_croot)/src/tools/bld/src/$(os_name)/$(hw_name)/rules.mk
endif
ifndef rules_mk
 include $(pwre_croot)/src/tools/bld/src/$(os_name)/rules.mk
endif

vpath %.c $(hw_source):$(os_source):$(co_source)
vpath %.dat $(hw_source):$(os_source):$(co_source)
vpath %.gif $(hw_source):$(os_source):$(co_source)

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

all : init copy lib

init :

copy : $(exe_dir)/profibus_xtthelp.dat $(export_gif) $(export_gifdoc) $(export_wav)

lib : $(export_obj)

exe :

clean :

realclean : clean $(clean_obj) 

.SUFFIXES:

$(exe_dir)/%.wav : ../../%.wav
	@ $(log_h_h)
	@ $(cp) $(cpflags) $(source) $(target)

$(exe_dir)/profibus_xtthelp.dat : profibus_xtthelp.dat
	@ echo "Copying $(source)"
	@ $(cp) $(cpflags) $(source) $(target)
	@ $(co_convert) -t -d $(pwr_doc)/help $(source)

$(exe_dir)/%.gif : ../../%.gif
	@ echo "Copying $(source).gif exe"
	@ $(cp) $(cpflags) $(source) $(target)

$(doc_dir)/help/%.gif : ../../%.gif
	@ echo "Copying $(source).gif doc/help"
	@ $(cp) $(cpflags) $(source) $(target)


$(clean_obj) : clean_%.o : %.c
	rm $(obj_dir)/$*.o





