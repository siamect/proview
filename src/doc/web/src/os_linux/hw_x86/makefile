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

vpath %.gif $(hw_source):$(os_source):$(co_source)

source_dirs = $(hw_source) $(os_source) $(co_source)

gif_sources := $(sort \
             $(foreach file, \
               $(foreach dir, \
                 $(source_dirs), \
                 $(wildcard $(dir)/*.gif) \
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

js_sources := $(sort \
             $(foreach file, \
               $(foreach dir, \
                 $(source_dirs), \
                 $(wildcard $(dir)/*.js) \
               ), $(notdir $(file)) \
             ) \
           )

sv_se_html_sources := $(sort \
             $(foreach file, \
               $(foreach dir, \
                 $(pwre_sroot)/doc/web/sv_se, \
                 $(wildcard $(dir)/*.html) \
               ), $(notdir $(file)) \
             ) \
           )

export_gif := $(addprefix $(doc_dir)/,$(gif_sources))
export_html := $(addprefix $(doc_dir)/,$(html_sources))
export_js := $(addprefix $(doc_dir)/,$(js_sources))
export_html_sv_se := $(addprefix $(doc_dir)/sv_se/,$(sv_se_html_sources))
clean_pwg := $(patsubst %.pwg,clean_%.pwg,$(sources))
clean_html := $(patsubst %.html,clean_%.html,$(sources))
clean_html := $(patsubst %.js,clean_%.js,$(sources))
export_doc := $(export_gif) $(export_html) $(export_js) $(export_html_sv_se)
clean_ge := $(clean_gif) $(clean_html)
.SUFFIXES:


.PHONY : all init copy lib exe clean realclean

all : init copy lib exe

init :

copy : $(export_doc) \
       $(doc_dir)/pcss.css

lib :

exe :

clean : $(clean_dox)

realclean : $(clean_html) $(clean_js)


$(clean_html) : clean_%.html : %.html
	@ $(rm) $(rmflags) $(doc_dir)/dox/$*.html

$(clean_js) : clean_%.js : %.js
	@ $(rm) $(rmflags) $(doc_dir)/dox/$*.js

$(doc_dir)/pcss.css : ../../pcss.css
	@ echo "copy $(source) $(target)"
	@ cp $(source) $(target)

$(doc_dir)/sv_se/%.html : $(pwre_sroot)/doc/web/sv_se/%.html
	@ $(log_h_h)
	@ $(cp) $(cpflags) $(source) $(target)

$(doc_dir)/%.js : $(pwre_sroot)/doc/web/src/%.js
	@ $(log_h_h)
	@ $(cp) $(cpflags) $(source) $(target)






