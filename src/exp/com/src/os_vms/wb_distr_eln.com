$!
$! wb_distr_eln.com
$!
$! Copy files to eln nodes
$!
$!	p1	vms copy command
$!	p2	source file in unix format
$!	p3	target file in eln format
$!
$ call unix_to_vms "''p2'"
$ 'p1' 'unix_to_vms_name' 'p3'
$
$
$unix_to_vms: subroutine
$
$! Translate unix directory
$! /pwrp/xxx/yyy/zzz/ will be translated to pwrp:[xxx.yyy.zzz]
$! /pwrp/xxx/yyy/zzz/aaa will be translated to pwrp:[xxx.yyy.zzz]aaa
$
$ unix_name 	= p1
$ i = 0
$next_elem:
$ elem = f$element( i, "/", unix_name)
$ elem_next = f$element( i+1, "/", unix_name)
$ if elem.nes."/"
$ then
$   if i.eq.1
$   then
$     vms_name = elem + ":["
$   else
$     if i.ne.0
$     then
$       if elem_next.eqs."/"
$       then
$         vms_name = vms_name + "]"
$         vms_name = vms_name - ".]"
$         vms_name = vms_name + "]"
$         vms_name = vms_name + elem + "."
$         goto cont_elem
$       else
$         vms_name = vms_name + elem + "."
$       endif
$     endif
$   endif
$
$   i = i + 1
$   goto next_elem
$ endif  
$cont_elem:
$ vms_name = vms_name + "]"
$ vms_name = vms_name - ".]"
$
$ unix_to_vms_name == vms_name
$endsubroutine
$
