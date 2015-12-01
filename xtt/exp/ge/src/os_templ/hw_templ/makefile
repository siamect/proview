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

vpath %.pwg $(hw_source):$(os_source):$(co_source)
vpath %.pwsg $(hw_source):$(os_source):$(co_source)
vpath %.pwgc $(hw_source):$(os_source):$(co_source)

source_dirs = $(hw_source) $(os_source) $(co_source)

pwg_sources := $(sort \
             $(foreach file, \
               $(foreach dir, \
                 $(source_dirs), \
                 $(wildcard $(dir)/pwr_c_*.pwg) \
               ), $(notdir $(file)) \
             ) \
           )

pwsg_sources := $(sort \
             $(foreach file, \
               $(foreach dir, \
                 $(source_dirs), \
                 $(wildcard $(dir)/pwr*.pwsg) \
               ), $(notdir $(file)) \
             ) \
           )

pwgc_sources := $(sort \
             $(foreach file, \
               $(foreach dir, \
                 $(source_dirs), \
                 $(wildcard $(dir)/*.pwgc) \
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

export_pwg := $(addprefix $(exe_dir)/,$(pwg_sources))
export_pwsg := $(addprefix $(exe_dir)/,$(pwsg_sources))
export_pwgc := $(addprefix $(exe_dir)/,$(pwgc_sources))
export_html := $(addprefix $(exe_dir)/,$(html_sources))
clean_pwsg := $(patsubst %.pwg,clean_%.pwsg,$(sources))
clean_pwg := $(patsubst %.pwg,clean_%.pwg,$(sources))
clean_pwgc := $(patsubst %.pwgc,clean_%.pwgc,$(sources))
clean_html := $(patsubst %.html,clean_%.html,$(sources))
export_ge := $(export_pwg) $(export_pwsg) $(export_pwgc) $(export_html)
clean_ge := $(clean_pwg) $(clean_pwsg) $(clean_pwgc) $(clean_html)
.SUFFIXES:


.PHONY : all init copy lib exe clean realclean \
         $(clean_ge)

all : init copy lib exe

init :

copy : $(export_ge)\
       $(exe_dir)/pwr_css.css

lib :

exe :

clean :

realclean : $(clean_ge)

$(clean_pwg) : clean_%.pwg : %.pwg
	@ $(rm) $(rmflags) $(exe_dir)/$*.pwg

$(clean_pwg) : clean_%.pwsg : %.pwsg
	@ $(rm) $(rmflags) $(exe_dir)/$*.pwsg

$(clean_pwgc) : clean_%.pwgc : %.pwgc
	@ $(rm) $(rmflags) $(exe_dir)/$*.pwgc

$(clean_html) : clean_%.html : %.html
	@ $(rm) $(rmflags) $(exe_dir)/$*.html

$(exe_dir)/pwr_css.css : ../../pwr_css.css
	@ echo "copy $(source) $(target)"
	@ cp $(source) $(target)



