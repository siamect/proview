include $(pwre_dir_symbols)
ifndef variables_mk
 -include $(pwre_kroot)/tools/bld/src/$(os_name)/$(hw_name)/variables.mk
endif
ifndef variables_mk
 include $(pwre_kroot)/tools/bld/src/$(os_name)/variables.mk
endif


ifndef rules_mk
 -include $(pwre_kroot)/tools/bld/src/$(os_name)/$(hw_name)/rules.mk
endif
ifndef rules_mk
 include $(pwre_kroot)/tools/bld/src/$(os_name)/rules.mk
endif


vpath %.gif $(hw_source):$(os_source):$(co_source)
vpath %.png $(hw_source):$(os_source):$(co_source)

source_dirs := $(hw_source) $(os_source) $(co_source)

gif_sources := $(sort \
             $(foreach file, \
               $(foreach dir, \
                 $(source_dirs), \
                 $(wildcard $(dir)/*.gif) \
               ), $(notdir $(file)) \
             ) \
           )

png_sources := $(sort \
             $(foreach file, \
               $(foreach dir, \
                 $(source_dirs), \
                 $(wildcard $(dir)/*.png) \
               ), $(notdir $(file)) \
             ) \
           )

html_sources := $(sort \
             $(foreach file, \
               $(foreach dir, \
                 $(source_dirs), \
                 $(wildcard $(dir)/*.html) \
               ), $(notdir $(file)) \
             ) \
           )

export_gif := $(patsubst %.gif,$(doc_dir)/en_us/orm/%.gif,$(gif_sources))
export_png := $(patsubst %.png,$(doc_dir)/en_us/orm/%.png,$(png_sources))
export_html := $(patsubst %.html,$(doc_dir)/en_us/orm/%.html,$(html_sources))
clean_gif := $(patsubst %.gif,clean_%.gif,$(gif_sources))
clean_png := $(patsubst %.png,clean_%.png,$(png_sources))
clean_html := $(patsubst %.html,clean_%.html,$(html_sources))

.SUFFIXES:

$(doc_dir)/en_us/orm/%.gif : %.gif
	@ $(log_h_h)
	@ $(cp) $(cpflags) $(source) $(target)
	@ $(cp) $(cpflags) $(source) $(doc_dir)/sv_se/orm
	@ $(cp) $(cpflags) $(source) $(doc_dir)/help

$(doc_dir)/en_us/orm/%.png : %.png
	@ $(log_h_h)
	@ $(cp) $(cpflags) $(source) $(target)
	@ $(cp) $(cpflags) $(source) $(doc_dir)/sv_se/orm
	@ $(cp) $(cpflags) $(source) $(doc_dir)/help

$(doc_dir)/en_us/orm/%.html : ../../%.html
	@ $(log_h_h)
	@ $(cp) $(cpflags) $(source) $(target)
	@ $(cp) $(cpflags) $(source) $(doc_dir)/sv_se/orm
	@ $(cp) $(cpflags) $(source) $(doc_dir)/help

.PHONY : all init copy lib exe clean realclean \
         $(clean_gif) $(clean_png)

all : init copy lib exe

init :

copy : $(export_gif) $(export_png) $(export_html)

lib :

exe :


clean :

realclean : clean $(clean_gif)  $(clean_png) $(clean_html) $(clean_c)

$(clean_gif) : clean_%.gif : %.gif
	@ $(rm) $(rmflags) $(doc_dir)/orm/$*.gif

$(clean_png) : clean_%.png : %.png
	@ $(rm) $(rmflags) $(doc_dir)/orm/$*.png

$(clean_html) : clean_%.html : %.html
	@ $(rm) $(rmflags) $(doc_dir)/orm/$*.html




