all:
	gcc -DHW_X86 -DOS_LINUX \
	-I/proview/x3-2a/rls_dbg/os_linux/hw_x86/exp/inc -o rt_io_ssab.o \
	-c rt_io_ssab.c

	gcc -DHW_X86 -DOS_LINUX \
	-I/proview/x3-2a/rls_dbg/os_linux/hw_x86/exp/inc -o rt_rtt.o \
	-c rt_rtt.c

	gcc -DHW_X86 -DOS_LINUX \
	-I/usr/local/jdk1.2.2/include \
	-I/proview/x3-2a/rls_dbg/os_linux/hw_x86/exp/inc -o Qcom.o \
	-I/usr/local/jdk1.2.2/include/linux \
	-I/usr/local/src/posix1b \
	-c Qcom.c

	gcc -DHW_X86 -DOS_LINUX \
	-I/usr/local/jdk1.2.2/include \
	-I/proview/x3-2a/rls_dbg/os_linux/hw_x86/exp/inc -o Gdh.o \
	-I/usr/local/jdk1.2.2/include/linux \
	-I/usr/local/src/posix1b \
	-c Gdh.c

	g++ -shared -DHW_X86 -DOS_LINUX \
	-L/proview/x3-2a/rls_dbg/os_linux/hw_x86/exp/lib \
	-L/usr/X11R6/lib \
	-I/usr/local/jdk1.2.2/include \
	-I/proview/x3-2a/rls_dbg/os_linux/hw_x86/exp/inc \
	-I/usr/local/jdk1.2.2/include/linux \
	/proview/x3-2a/rls_dbg/os_linux/hw_x86/bld/lib/co/*.o \
	/proview/x3-2a/rls_dbg/os_linux/hw_x86/bld/lib/msg_dummy/*.o \
	/proview/x3-2a/rls_dbg/os_linux/hw_x86/bld/lib/rt/*.o \
	/proview/x3-2a/rls_dbg/os_linux/hw_x86/bld/lib/flow/*.o \
	/proview/x3-2a/rls_dbg/os_linux/hw_x86/bld/lib/glow/*.o \
	/proview/x3-2a/rls_dbg/os_linux/hw_x86/bld/lib/ge/*.o \
	/proview/x3-2a/rls_dbg/os_linux/hw_x86/exp/obj/rt_io_user.o \
	/proview/x3-2a/rls_dbg/os_linux/hw_x86/exp/obj/dtt_rttsys.o \
	rt_io_ssab.o rt_rtt.o Qcom.o\
	Gdh.o -o libgdh.so -lm -lpthread -lposix1b -lpwr_dtt \
	-lMrm -lXm -lXpm -lXt -lX11 -lXext -lXp\
        -lSM -lICE

test:
	gcc test.c -o test -ljavagdh

libr:
	gcc -shared -DHW_X86 -DOS_LINUX -L/proview/linux_dbg/os_linux/hw_x86/exp/lib -I/usr/local/jdk/include -I/proview/linux_dbg/os_linux/hw_x86/exp/inc -I/usr/local/jdk/include/linux /proview/linux_dbg/os_linux/hw_x86/bld/lib/co/*.o /proview/linux_dbg/os_linux/hw_x86/bld/lib/msg_dummy/*.o /proview/linux_dbg/os_linux/hw_x86/bld/lib/rt/*.o /usr/local/src/proview/bld_dbg/os_linux/hw_x86/exp/obj/rt_io_user.o ssabio.c rtt.c JavaPwrImp.c -o libjavagdh.so -lm -lpthread -lposix1b -lpwr_dtt
