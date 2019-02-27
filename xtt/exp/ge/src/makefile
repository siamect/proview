include $(pwre_dir_symbols)

ifndef variables_mk
 include $(pwre_kroot)/tools/bld/src/variables.mk
endif

ifndef rules_mk
 include $(pwre_kroot)/tools/bld/src/rules.mk
endif

vpath %.pwg $(co_source)
vpath %.pwsg $(co_source)
vpath %.pwgc $(co_source)

source_dirs := $(co_source)

pwg_c_sources := $(sort \
             $(foreach file, \
               $(foreach dir, \
                 $(source_dirs), \
                 $(wildcard $(dir)/pwr_c_*.pwg) \
               ), $(notdir $(file)) \
             ) \
           )

pwg_t_sources := $(sort \
             $(foreach file, \
               $(foreach dir, \
                 $(source_dirs), \
                 $(wildcard $(dir)/pwr_t_*.pwg) \
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

export_pwg := $(addprefix $(exe_dir)/,$(pwg_c_sources)) $(addprefix $(exe_dir)/,$(pwg_t_sources))
export_pwsg := $(addprefix $(exe_dir)/,$(pwsg_sources))
export_pwgc := $(addprefix $(exe_dir)/,$(pwgc_sources))
export_web_pwgc := $(addprefix $(web_dir)/,$(pwgc_sources))
export_html := $(addprefix $(exe_dir)/,$(html_sources))
clean_pwsg := $(patsubst %.pwg,clean_%.pwsg,$(sources))
clean_pwg := $(patsubst %.pwg,clean_%.pwg,$(sources))
clean_pwgc := $(patsubst %.pwgc,clean_%.pwgc,$(sources))
clean_html := $(patsubst %.html,clean_%.html,$(sources))
export_ge := $(export_pwg) $(export_pwsg) $(export_pwgc) $(export_html)
clean_ge := $(clean_pwg) $(clean_pwsg) $(clean_pwgc) $(clean_html)
export_web := $(export_web_pwgc)
clean_web := $(patsubst %.pwgc,clean_%.pwgc,$(sources))
.SUFFIXES:


.PHONY : all init copy lib exe clean realclean \
         $(clean_ge) $(clean_web)

all : init copy lib exe | silent

init : silent

copy : $(export_ge)\
       $(export_web)\
       $(exe_dir)/pwr_css.css \
       | silent

lib : silent

exe : silent

clean :

realclean : $(clean_ge) $(clean_web)

silent :
	@ :

$(clean_pwg) : clean_%.pwg : %.pwg
	@ $(rm) $(rmflags) $(exe_dir)/$*.pwg

$(clean_pwg) : clean_%.pwsg : %.pwsg
	@ $(rm) $(rmflags) $(exe_dir)/$*.pwsg

$(clean_pwgc) : clean_%.pwgc : %.pwgc
	@ $(rm) $(rmflags) $(exe_dir)/$*.pwgc

$(clean_html) : clean_%.html : %.html
	@ $(rm) $(rmflags) $(exe_dir)/$*.html

$(clean_web) : clean_%.pwgc : %.pwgc
	@ $(rm) $(rmflags) $(web_dir)/$*.pwgc

$(exe_dir)/pwr_css.css : pwr_css.css
	@ echo "copy $(source) $(target)"
	@ cp $(source) $(target)
