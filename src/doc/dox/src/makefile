include $(pwre_dir_symbols)

ifndef variables_mk
 include $(pwre_croot)/src/tools/bld/src/variables.mk
endif

ifndef rules_mk
 include $(pwre_croot)/src/tools/bld/src/rules.mk
endif

.SUFFIXES:

.PHONY : all init copy lib exe clean realclean

all : init copy lib exe | silent

init :

copy :

lib :

exe : $(doc_dir)/dox/index.html | silent

clean : $(clean_dox)

realclean : $(clean_html)

silent :
	@ :

$(clean_html) : clean_%.html : %.html
	@ $(rm) $(rmflags) $(doc_dir)/dox/$*.html

$(doc_dir)/dox/index.html : doxygen.dx
	@ echo "doxygen documentation $(source) $(target)"
	@ doxygen 2> /dev/null
