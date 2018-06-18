ifndef variables_mk
 -include $(pwre_kroot)/tools/bld/src/$(os_name)/$(hw_name)/variables.mk
endif
ifndef variables_mk
 include $(pwre_kroot)/tools/bld/src/$(os_name)/variables.mk
endif

../../moc_%.cpp : ../../%.h
	@ echo "Running moc on file $(source)"
	moc $< -o $@