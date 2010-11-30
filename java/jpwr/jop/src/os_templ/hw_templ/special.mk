# makefile  -- <short description>
#
#  PROVIEW/R
#  Copyright (C) 1996 by Comator Process AB.
#
#  <Description>.
#

vpath %.java $(hw_source):$(os_source):$(co_source)

source_dirs = $(hw_source) $(os_source) $(co_source)

jop_classes := \
	$(bld_dir)/Ge.class 

export_jop := \
        $(jop_classes)

clean_jop := $(patsubst %.class,clean_%.class,$(export_jop))

.PHONY : all init copy lib exe clean realclean\
         $(clean_jop)

all : init copy

init :

copy :

lib : $(export_jop)

exe :

clean :

realclean : clean $(clean_jop)

