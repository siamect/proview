include $(pwre_dir_symbols)

ifndef variables_mk
 include $(pwre_kroot)/tools/bld/src/variables.mk
endif

ifndef rules_mk
 include $(pwre_kroot)/tools/bld/src/rules.mk
endif

vpath %.py $(co_source)

source_dirs := $(co_source)

py_sources := $(sort \
             $(foreach file, \
               $(foreach dir, \
                 $(source_dirs), \
                 $(wildcard $(dir)/*.py) \
               ), $(notdir $(file)) \
             ) \
           )

export_py := $(patsubst %.py, $(exe_dir)/%.py, $(py_sources))
clean_py := $(patsubst %.py, clean_%.py, $(py_sources))


.PHONY : all init copy lib exe clean realclean\
         $(clean_py)

all : init copy | silent

init : silent

copy : $(export_py) | silent

lib : silent

exe : silent

clean :

realclean : clean $(clean_py)

silent :
	@ :

$(export_py) : $(exe_dir)/%.py : %.py
	@ echo "Exporting $< ..."
	@ $(cp) $(cpflags) $(source) $(target)

$(clean_py) : clean_%.py : %.py
	@ rm $(exe_dir)/$*.py
