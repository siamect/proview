include $(pwre_dir_symbols)


-include $(pwre_sroot)/tools/bld/src/$(os_name)/$(hw_name)/$(type_name)_generic.mk

ifeq ($($(type_name)_generic_mk),)
 -include $(pwre_sroot)/tools/bld/src/$(os_name)/$(type_name)_generic.mk
endif

linkflags	:= $(linkflags)/threads_enable=upcalls

ifeq ($($(type_name)_generic_mk),)
 include $(pwre_sroot)/tools/bld/src/$(type_name)_generic.mk
endif
