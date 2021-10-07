Peter Boettcher
Germany/NRW
Muelheim Ruhr
peter.boettcher@gmx.net


------------------------------------------------------------------------------------------------
Sorry, for this english!

Thanks!
My best thanks to Linus Torvalds and all the people who support the project or who
have supported it in the past.


------------------------------------------------------------------------------------------------
This is a new SYSCALL.
"The Uninterruptible Shutdown Timer". I think so.

For me, the SYSCALL makes sense and is usefull.
Maybe also for the LINUX COMMUNITY?


------------------------------------------------------------------------------------------------
Why? My Intention?

I also use my notebook outside of home.
I use Linux/Distr. as "Standard OS".
My data partition is encrypt with "LUKS".
If the DATA PARTITION is not mounted. A loss or theft would be sad but acceptable.
If the DATA PARTITION is mounted. A loss or theft would be very bad. Also in operation.

For this reason I have a uncomfortable feeling when I use the notebook outside.


------------------------------------------------------------------------------------------------
For this reason I have developed an new SYSCALL.
The UNINTERRUPTIBLE SHUTDOWN TIMER?! I think so!

SYSCALL works even if "/proc" is not mounted or other.

After a Shutdown/Halt, the DATA PARTITION is not longer mounted!


------------------------------------------------------------------------------------------------
Once activated, this "SHUTDOWN TIMER" cannot be stopped?! I think so! Not even with ROOT rights.
kill/gdb does not work.

Activate does require "ROOT" rights.
Shutdown/Halt Timer can be adjusted. Time can only set be earlier, based on the remaining time.

Tools: set.shutdown.c, set.shutdown.pas


------------------------------------------------------------------------------------------------
Adjustment of the "OS System Time" is recognized.
"SLEEP MODE" etc. is recognized.


------------------------------------------------------------------------------------------------
Use outside:

I set the time to 30/40 minutes.
Mount DATA PARTITION.
Restart before Shutdown time.

This procedure takes time. But i have a better feeling.


------------------------------------------------------------------------------------------------
I assume that the remaining time will be too short to copy data.
From "LUKS" to "NOT LUKS". Or other.


#################################################################################################
Important!
This SYSCALL works fine with:
                    x86_64/64BIT
                    x86/32BIT
                    Other not tested

The Code is compiled without errors or warnings.



------------------------------------------------------------------------------------------------
add in: syscall.tbl.

arch/x86/entry/syscalls/syscall_64.tbl
499	common	shutdown_timer		sys_shutdown_timer

arch/x86/entry/syscalls/syscall_32.tbl
499	i386	shutdown_timer		sys_shutdown_timer

arch/xtensa/kernel/syscalls/syscall.tbl
499	common	shutdown_timer		sys_shutdown_timer

arch/sh/kernel/syscalls/syscall.tbl
499	common	shutdown_timer		sys_shutdown_timer

arch/s390/kernel/syscalls/syscall.tbl
499  common shutdown_timer		sys_shutdown_timer	sys_shutdown_timer

arch/powerpc/kernel/syscalls/syscall.tbl
499	common	shutdown_timer		sys_shutdown_timer

arch/parisc/kernel/syscalls/syscall.tbl
499	common	shutdown_timer		sys_shutdown_timer

arch/microblaze/kernel/syscalls/syscall.tbl
499	common	shutdown_timer		sys_shutdown_timer

arch/m68k/kernel/syscalls/syscall.tbl
499	common	shutdown_timer		sys_shutdown_timer

arch/ia64/kernel/syscalls/syscall.tbl
499	common	shutdown_timer		sys_shutdown_timer

arch/alpha/kernel/syscalls/syscall.tbl
499	common	shutdown_timer		sys_shutdown_timer

arch/arm/tools/syscalls/syscall.tbl
557	common	shutdown_timer		sys_shutdown_timer

arch/mips/kernel/syscalls/syscall_n32.tbl
499	n32	shutdown_timer		sys_shutdown_timer

arch/mips/kernel/syscalls/syscall_n64.tbl
499	n64	shutdown_timer		sys_shutdown_timer

arch/mips/kernel/syscalls/syscall_o32.tbl
499	o32	shutdown_timer		sys_shutdown_timer

499 or other


------------------------------------------------------------------------------------------------
expample:

copy to:
shutdown_timer.c	-> kernel/time/shutdown_timer.c
stat_shutdown_timer.c	-> kernel/time/stat_shutdown_timer.c
or other?


add in:
kernel/time/Makefile
or other?


"obj-y += shutdown_timer.o"
"obj-y += stat_shutdown_timer.o"


You can remove then "printk" Messages.
