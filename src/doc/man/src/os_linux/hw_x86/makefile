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


vpath %.pdf $(hw_source):$(os_source):$(co_source)

source_dirs := $(hw_source) $(os_source) $(co_source)

sources := $(sort \
             $(foreach file, \
               $(foreach dir, \
                 $(source_dirs), \
                 $(wildcard $(dir)/*.pdf) \
               ), $(notdir $(file)) \
             ) \
           )

en_us_sources := $(sort \
             $(foreach file, \
               $(foreach dir, \
                 $(pwre_sroot)/doc/man/en_us, \
                 $(wildcard $(dir)/*.pdf) \
               ), $(notdir $(file)) \
             ) \
           )

sv_se_sources := $(sort \
             $(foreach file, \
               $(foreach dir, \
                 $(pwre_sroot)/doc/man/sv_se, \
                 $(wildcard $(dir)/*.pdf) \
               ), $(notdir $(file)) \
             ) \
           )

gsources := $(sort \
             $(foreach file, \
               $(foreach dir, \
                 $(source_dirs), \
                 $(wildcard $(dir)/*.gif) \
               ), $(notdir $(file)) \
             ) \
           )

pdf_sources := $(filter %.pdf,$(sources))
pdf_sv_se_sources := $(filter %.pdf,$(sv_se_sources)) 
pdf_en_us_sources := $(filter %.pdf,$(en_us_sources)) 
gif_sources := $(filter %.gif,$(gsources))

export_pdf := $(patsubst %.pdf,$(doc_dir)/%.pdf,$(pdf_sources))
export_pdf_sv_se := $(patsubst %.pdf,$(doc_dir)/sv_se/%.pdf,$(pdf_sv_se_sources))
export_pdf_en_us := $(patsubst %.pdf,$(doc_dir)/en_us/%.pdf,$(pdf_en_us_sources))
export_gif_sv_se := $(patsubst %.gif,$(doc_dir)/sv_se/%.gif,$(gif_sources))
export_gif_en_us := $(patsubst %.gif,$(doc_dir)/en_us/%.gif,$(gif_sources))
export_gif_exe := $(patsubst %.gif,$(exe_dir)/%.gif,$(gif_sources))
clean_pdf := $(patsubst %.pdf,clean_%.pdf,$(pdf_sources))

.SUFFIXES:

$(doc_dir)/%.pdf : ../../%.pdf
	@ $(log_h_h)
	@ $(cp) $(cpflags) $(source) $(target)

$(doc_dir)/sv_se/%.pdf : $(pwre_sroot)/doc/man/sv_se/%.pdf
	@ $(log_h_h)
	@ $(cp) $(cpflags) $(source) $(target)

$(doc_dir)/en_us/%.pdf : $(pwre_sroot)/doc/man/en_us/%.pdf
	@ $(log_h_h)
	@ $(cp) $(cpflags) $(source) $(target)

$(exe_dir)/%.gif : ../../%.gif
	@ $(log_h_h)
	@ $(cp) $(cpflags) $(source) $(target)

$(doc_dir)/sv_se/%.gif : ../../%.gif
	@ $(log_h_h)
	@ $(cp) $(cpflags) $(source) $(target)

$(doc_dir)/en_us/%.gif : ../../%.gif
	@ $(log_h_h)
	@ $(cp) $(cpflags) $(source) $(target)

.PHONY : all init copy lib exe clean realclean \
         $(clean_pdf)

all : init copy lib exe

init :

copy : $(export_pdf) $(export_pdf_en_us) $(export_pdf_sv_se) \
       $(export_gif_exe) $(export_gif_en_us) $(export_gif_sv_se) \
       $(exe_dir)/en_us/wtt_help.dat $(exe_dir)/sv_se/wtt_help.dat \
       $(exe_dir)/en_us/man_dg.dat $(exe_dir)/sv_se/man_dg.dat \
       $(doc_dir)/en_us/man_dg_index.html $(doc_dir)/sv_se/man_dg_index.html \
       $(doc_dir)/en_us/man_dg.pdf $(doc_dir)/sv_se/man_dg.pdf \
       $(doc_dir)/en_us/man_dg.ps $(doc_dir)/sv_se/man_dg.ps \
       $(exe_dir)/en_us/man_geref.dat $(exe_dir)/sv_se/man_geref.dat \
       $(doc_dir)/en_us/man_geref_index.html $(doc_dir)/sv_se/man_geref_index.html \
       $(doc_dir)/en_us/man_geref.pdf $(doc_dir)/sv_se/man_geref.pdf \
       $(doc_dir)/en_us/man_geref.ps $(doc_dir)/sv_se/man_geref.ps

lib :

exe :


clean :

realclean : clean $(clean_pdf) $(clean_c)

$(clean_pdf) : clean_%.pdf : %.pdf
	@ $(rm) $(rmflags) $(doc_dir)/$*.pdf


$(exe_dir)/en_us/wtt_help.dat : $(pwre_sroot)/doc/man/en_us/wtt_help.dat
	@ $(log_h_h)
	@ $(cp) $(cpflags) $(source) $(target)


$(exe_dir)/sv_se/wtt_help.dat : $(pwre_sroot)/doc/man/sv_se/wtt_help.dat
	@ $(log_h_h)
	@ $(cp) $(cpflags) $(source) $(target)

$(exe_dir)/en_us/man_dg.dat : $(pwre_sroot)/doc/man/en_us/man_dg.dat
	@ $(log_h_h)
	@ $(cp) $(cpflags) $(source) $(target)

$(exe_dir)/sv_se/man_dg.dat : $(pwre_sroot)/doc/man/sv_se/man_dg.dat
	@ $(log_h_h)
	@ $(cp) $(cpflags) $(source) $(target)

$(doc_dir)/en_us/man_dg_index.html : $(pwre_sroot)/doc/man/en_us/man_dg.dat
	@ echo "Generating html for en_us/man_dg"
	@ co_convert -t -d $(doc_dir)/en_us $(source)

$(doc_dir)/sv_se/man_dg_index.html : $(pwre_sroot)/doc/man/sv_se/man_dg.dat
	@ echo "Generating html for sv_se/man_dg"
	@ co_convert -t -l sv_se -d $(doc_dir)/sv_se $(source)

$(doc_dir)/en_us/man_dg.pdf : $(pwre_sroot)/doc/man/en_us/man_dg.dat
	@ echo "Generating pdf for en_us/man_dg"
	@ co_convert -f -d $(doc_dir)/en_us $(source)

$(doc_dir)/sv_se/man_dg.pdf : $(pwre_sroot)/doc/man/sv_se/man_dg.dat
	@ echo "Generating pdf for sv_se/man_dg"
	@ co_convert -f -l sv_se -d $(doc_dir)/sv_se $(source)

$(doc_dir)/en_us/man_dg.ps : $(pwre_sroot)/doc/man/en_us/man_dg.dat
	@ echo "Generating postscript for en_us/man_dg"
	@ co_convert -n -d $(doc_dir)/en_us $(source)

$(exe_dir)/en_us/man_geref.dat : $(pwre_sroot)/doc/man/en_us/man_geref.dat
	@ $(log_h_h)
	@ $(cp) $(cpflags) $(source) $(target)

$(exe_dir)/sv_se/man_geref.dat : $(pwre_sroot)/doc/man/sv_se/man_geref.dat
	@ $(log_h_h)
	@ $(cp) $(cpflags) $(source) $(target)

$(doc_dir)/sv_se/man_dg.ps : $(pwre_sroot)/doc/man/sv_se/man_dg.dat
	@ echo "Generating postscript for sv_se/man_dg"
	@ co_convert -n -d $(doc_dir)/sv_se $(source)

$(doc_dir)/en_us/man_geref_index.html : $(pwre_sroot)/doc/man/en_us/man_geref.dat
	@ echo "Generating html for en_us/man_geref"
	@ co_convert -t -d $(doc_dir)/en_us $(source)

$(doc_dir)/sv_se/man_geref_index.html : $(pwre_sroot)/doc/man/sv_se/man_geref.dat
	@ echo "Generating html for sv_se/man_geref"
	@ co_convert -t -l sv_se -d $(doc_dir)/sv_se $(source)

$(doc_dir)/en_us/man_geref.pdf : $(pwre_sroot)/doc/man/en_us/man_geref.dat
	@ echo "Generating pdf for en_us/man_geref"
	@ co_convert -f -d $(doc_dir)/en_us $(source)

$(doc_dir)/sv_se/man_geref.pdf : $(pwre_sroot)/doc/man/sv_se/man_geref.dat
	@ echo "Generating pdf for sv_se/man_geref"
	@ co_convert -f -l sv_se -d $(doc_dir)/sv_se $(source)

$(doc_dir)/en_us/man_geref.ps : $(pwre_sroot)/doc/man/en_us/man_geref.dat
	@ echo "Generating postscript for en_us/man_geref"
	@ co_convert -n -d $(doc_dir)/en_us $(source)

$(doc_dir)/sv_se/man_geref.ps : $(pwre_sroot)/doc/man/sv_se/man_geref.dat
	@ echo "Generating postscript for sv_se/man_geref"
	@ co_convert -n -d $(doc_dir)/sv_se $(source)









