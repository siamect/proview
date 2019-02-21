include $(pwre_dir_symbols)

ifndef variables_mk
 include $(pwre_croot)/src/tools/bld/src/variables.mk
endif

ifndef rules_mk
 include $(pwre_croot)/src/tools/bld/src/rules.mk
endif

vpath %.gif $(co_source)

source_dirs := $(co_source)

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

pdf_sources := $(sort \
             $(foreach file, \
               $(foreach dir, \
                 $(source_dirs), \
                 $(wildcard $(dir)/*.pdf) \
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
export_html_sv_se := $(addprefix $(doc_dir)/sv_se/,$(sv_se_html_sources))
export_pdf := $(addprefix $(doc_dir)/,$(pdf_sources))
clean_pwg := $(patsubst %.pwg,clean_%.pwg,$(sources))
clean_html := $(patsubst %.html,clean_%.html,$(sources))
export_doc := $(export_gif) $(export_html) $(export_pdf) $(export_html_sv_se)
clean_ge := $(clean_gif) $(clean_html)
.SUFFIXES:

.PHONY : all init copy lib exe clean realclean

$(doc_dir)/%.pdf : %.pdf
	@ $(log_h_h)
	@ $(cp) $(cpflags) $(source) $(target)


all : init copy lib exe | silent

init :

copy : $(export_doc) | silent

lib :

exe :

clean : $(clean_dweb)

realclean : $(clean_html)

silent :
	@ :

$(clean_html) : clean_%.html : %.html
	@ $(rm) $(rmflags) $(doc_dir)/dox/$*.html
