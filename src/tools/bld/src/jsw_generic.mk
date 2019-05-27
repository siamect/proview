#  jsw_generic.mk -- generic makefile for jsw
#
#  PROVIEW/R
#  Copyright (C) 1996 by Comator Process AB.
#
#  To build a library.
#
ifndef jsw_generic_mk
jsw_generic_mk := 1

ifndef variables_mk
 include $(pwre_kroot)/tools/bld/src/variables.mk
endif

ifndef rules_mk
 include $(pwre_kroot)/tools/bld/src/rules.mk
endif

source_dirs := $(co_source)

-include jsw_local.mk

vpath %.js $(co_source)
vpath %.ts $(co_source)
vpath %.html $(co_source)
vpath %.png $(co_source)

js_sources := $(sort \
             $(foreach file, \
               $(foreach dir, \
                 $(source_dirs), \
                 $(wildcard $(dir)/*.js) \
               ), $(notdir $(file)) \
             ) \
           )

ts_sources := $(sort \
             $(foreach file, \
               $(foreach dir, \
                 $(source_dirs), \
                 $(wildcard $(dir)/*.ts) \
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

png_sources := $(sort \
             $(foreach file, \
               $(foreach dir, \
                 $(source_dirs), \
                 $(wildcard $(dir)/*.png) \
               ), $(notdir $(file)) \
             ) \
           )

css_sources := $(sort \
             $(foreach file, \
               $(foreach dir, \
                 $(source_dirs), \
                 $(wildcard $(dir)/*.css) \
               ), $(notdir $(file)) \
             ) \
           )

export_js := $(addprefix $(web_dir)/,$(js_sources))
export_ts := $(addprefix $(web_dir)/,$(ts_sources))
export_html := $(addprefix $(web_dir)/,$(html_sources))
export_png := $(addprefix $(web_dir)/,$(png_sources))
export_css := $(addprefix $(web_dir)/,$(css_sources))
clean_js := $(patsubst %.js,clean_%.js, $(js_sources))
clean_ts := $(patsubst %.ts,clean_%.ts, $(ts_sources))
clean_html := $(patsubst %.html,clean_%.html, $(html_sources))
clean_png := $(patsubst %.png,clean_%.png, $(png_sources))
clean_css := $(patsubst %.css,clean_%.css, $(css_sources))

.PHONY : all init copy lib exe clean realclean

all : init copy lib exe | silent

init :

copy : $(export_js) $(export_ts) $(export_html) $(export_png) $(export_css) | silent

lib :

exe :

clean : $(clean_js) $(clean_ts) $(clean_html) $(clean_png) $(clean_css)

clean_copy: $(clean)

realclean : $(clean)

silent:
	@ :

$(clean_js) : clean_%.js : %.js
	@ echo "Removing js: $(web_dir)/$*.js"
	@ $(rm) $(rmflags) $(web_dir)/$*.js

$(clean_ts) : clean_%.ts : %.ts
	@ echo "Removing ts: $(web_dir)/$*.ts"
	@ $(rm) $(rmflags) $(web_dir)/$*.ts

$(clean_html) : clean_%.html : %.html
	@ echo "Removing html: $(web_dir)/$*.html"
	@ $(rm) $(rmflags) $(web_dir)/$*.html

$(clean_png) : clean_%.png : %.png
	@ echo "Removing png: $(web_dir)/$*.png"
	@ $(rm) $(rmflags) $(web_dir)/$*.png

$(clean_css) : clean_%.css : %.css
	@ echo "Removing png: $(web_dir)/$*.css"
	@ $(rm) $(rmflags) $(web_dir)/$*.css

-include $(source_dependencies)

endif
