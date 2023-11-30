/* Copyright (c) 2021/08/28, Peter Boettcher, Germany/NRW, Muelheim Ruhr, mail:peter.boettcher@gmx.net
 * Urheber: 2021.08.28, Peter Boettcher, Germany/NRW, Muelheim Ruhr, mail:peter.boettcher@gmx.net

 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2, or (at your option)
 * any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */



 /* shutdown timer uninterruptible!
  * You can not stop this timer! I think so.
  * kill/gdb does not work
  *
  * SHUTDOWN TIME can only be set by ROOT
  *
  * I think INFO via printk is a good idea
  * You can remove the printk directives/function
  *
  * This SYSCALL works fine with:
  *                                x86_64/64BIT
  *                                x86/32BIT
  *                                Other not tested
  *
  *
  * Important: Shutdown Time = SECONDS (signed)
  *
  * 64BIT:
  *        signed 64BIT UNIX EPOCH TIME + signed 64BIT SHUTDONW_TIME. This is very, very FAR in the FUTURE.
  *
  * 32 BIT:
  *       signed 64BIT UNIX EPOCH TIME + signed 64BIT SHUTDOWN TIME. This is very, very FAR in the FUTURE.
  *
  *
  * Time is not setting, if signed UNIX EPOCH TIME + signed SHUTDOWN TIME is negative. That is longer than life on earth will exist!
  *
  *
  * You have to consider then BYTE ORDER
  *
  *
  * Important:
  *              32BIT Long=32BIT
  *              64BIT Long=64Bit
  */



#include <linux/kernel.h>
#include <linux/syscalls.h>
#include <linux/delay.h>
#include <linux/reboot.h>
#include <linux/kthread.h>
#include <linux/mutex.h>
#include <linux/uidgid.h>


#define PRINTK

/* proto. */
struct shutdown_timer_info_struct {
	s64 shutdown_time_sec;
	s64 shutdown_unix_epoch_time_sec;
	bool shutdown_flag;
};


static struct mutex shutdown_lock;
static struct task_struct *kt_shutdown;

static bool shutdown_flag = false;


/* This is the important "Counter" */
static s64 shutdown_time_sec = 0;
static s64 shutdown_unix_epoch_time_sec = 0;



/* DATA: Only over function */
void info_shutdown_timer(struct shutdown_timer_info_struct *info)
{
	info->shutdown_time_sec = shutdown_time_sec;
	info->shutdown_unix_epoch_time_sec = shutdown_unix_epoch_time_sec;
	info->shutdown_flag = shutdown_flag;
}


static s64 shutdown_time_days_modulo(void)
{

#if BITS_PER_LONG == 32
	s64 time;

	time = shutdown_time_sec;
	time = do_div(time, 3600 * 24 * 365);
	return(time);
#endif

#if BITS_PER_LONG == 64
	return((shutdown_time_sec / 3600 / 24) % 365);
#endif

}


static s64 shutdown_time_years(void)
{

#if BITS_PER_LONG == 32
	s64 time;

	time = shutdown_time_sec;
	do_div(time, 365 * 24 * 3600);
	return(time);
#endif

#if BITS_PER_LONG == 64
	return(shutdown_time_sec / 365 / 24 / 3600);
#endif

}


static s64 shutdown_time_hours_modulo(void)
{

#if BITS_PER_LONG == 32
	s64 time;
	u64 remainder;

	time = shutdown_time_sec;
	do_div(time, 3600);
	remainder = do_div(time, 24);
	return(remainder);
#endif

#if BITS_PER_LONG == 64
	return((shutdown_time_sec / 3600) % 24);
#endif

}


static s64 shutdown_time_min_modulo(void)
{

#if BITS_PER_LONG == 32
	s64 time;
	u64 remainder;

	time = shutdown_time_sec;
	do_div(time, 60);
	remainder = do_div(time, 60);
	return(remainder);
#endif

#if BITS_PER_LONG == 64
	return((shutdown_time_sec / 60) % 60);
#endif

}


/*
 * I think INFO via printk is a good idea 
 * You can remove the printk directives/functions
 */
static void printk_help_shutdown_timer(void)
{

#ifdef PRINTK
	printk("SYSCALL SHUTDOWN TIMER\n");
	printk("Overview\n");
	printk("ONLY USER ID 0");
	printk("SET SHUTDOWN TIMER                      : <0> <TIME/SEC.>\n");
	printk("SET SHUTDOWN TIMER, NEW TIME (earlier)  : <1> <TIME/SEC.>\n\n");
#endif

}


static void printk_stat_shutdown_timer(void)
{

#ifdef PRINTK

	printk("SHUTDOWN TIMER           : ACTIVE\n");
	printk("SHUTDOWN TIMER COUNTER   : %lld YEARS %lld DAYS %lld:%lld HOURS, %lld Sec.\n",
		shutdown_time_years(),
		shutdown_time_days_modulo(),
		shutdown_time_hours_modulo(),
		shutdown_time_min_modulo(),
		shutdown_time_sec);
		
	printk("SHUTDOWN UNIX EPOCH TIME : %lld Sec, %lld Sec.\n\n",
		shutdown_unix_epoch_time_sec,
		shutdown_unix_epoch_time_sec - ktime_get_real_seconds());

#endif

}


static void printk_error_shutdown_timer(void)
{

#ifdef PRINTK
	printk("SHUTDOWN TIMER           : ERROR\n\n"); 
#endif

}

static void printk_ok_shutdown_timer(void)
{

#ifdef PRINTK
	printk("SHUTDOWN TIMER           : OK\n"); 
#endif

}


/*
 * Kernel thread
 * shutdown timer. Not interruptible. I think soo.
 * You can not terminate this kernel Worker Threads
 * Kill/Debug does not work
 */
static int shutdown_timer(void *data)
{

	s64 diff_time_sec;
	s64 unix_epoch_time_sec;

	while(1) {
		/* 1 sec.*/
		ssleep(1);

		mutex_lock(&shutdown_lock);

		shutdown_time_sec -= 1;

		unix_epoch_time_sec = ktime_get_real_seconds();

		/* Shutdown/Halt? */
		if (unix_epoch_time_sec >= shutdown_unix_epoch_time_sec) {
			mutex_unlock(&shutdown_lock);
			break;
		}

		/* Shutdown/Halt? This is the important "Counter" */
		if (shutdown_time_sec <= 0) {
			mutex_unlock(&shutdown_lock);
			break;
		}

		 /* shutdown time counter. check and correct after sleep/adjusting etc.. Future OS SYSTEM TIME */
		diff_time_sec = shutdown_unix_epoch_time_sec - unix_epoch_time_sec;
		if (diff_time_sec < shutdown_time_sec) shutdown_time_sec = diff_time_sec;

		/* Check and correct shutdown time after adjusting. After set OS SYSTEM TIME in the past */
		if (diff_time_sec > shutdown_time_sec)
			shutdown_unix_epoch_time_sec = unix_epoch_time_sec + shutdown_time_sec;

		mutex_unlock(&shutdown_lock);
	}


	/* Shutdown/Halt */
	while (1) {
		kernel_power_off();
		kernel_halt();			/* if error */
		kernel_restart(" ");		/* if error */
		ssleep(3);
	}

	return(0);
}


/*
 * Init Shutdown Timer or Not
 */
static long init_shutdown_timer(s64 halt_time_sec)
{
	/* Active? */
	if (shutdown_flag == true) {
		/* IF ACTIVE: STAT INFO */
		printk_error_shutdown_timer();
		printk_stat_shutdown_timer();
		return(-1);
	}

	shutdown_flag = true;
	/* check value. Not lower 60 */
	if (halt_time_sec < 60) {
		printk_error_shutdown_timer();
		shutdown_flag = false;
		return(-1);
	}

	/* Overflow. if ktime.. + halt_time_sec <) 0 not OK! Very, Very Far in the Future */
	if ((s64) (ktime_get_real_seconds() + halt_time_sec) <= 0) {
		printk_error_shutdown_timer();
		shutdown_flag = false;
		return(-1);
	}

	/* ---Is only entered once--- */
	shutdown_time_sec = halt_time_sec;

	shutdown_unix_epoch_time_sec = ktime_get_real_seconds() + halt_time_sec;

	/* info */
	printk_ok_shutdown_timer();
	printk_stat_shutdown_timer();

	/* important */
	mutex_init(&shutdown_lock);

	/* create worker thread */
	kt_shutdown = kthread_create (shutdown_timer, NULL, "shutdown/timer");

	wake_up_process(kt_shutdown);

	return(0);
}


/* new time only earlier than than remaining time.*/
static long set_shutdown_timer(s64 halt_time_sec)
{

	/* Active? */
	if (shutdown_flag == false) {
		printk_error_shutdown_timer();
		return(-1);
	}

	/* not lower 60 sec */
	if (halt_time_sec < 60) {
		printk_error_shutdown_timer();
		return(-1);
	}

	/* new time lower than remaining time? */
	if (halt_time_sec < shutdown_time_sec) {

		mutex_lock(&shutdown_lock);

		shutdown_time_sec = halt_time_sec;
		shutdown_unix_epoch_time_sec =  ktime_get_real_seconds() + halt_time_sec;

		mutex_unlock(&shutdown_lock);

		printk_ok_shutdown_timer();
		printk_stat_shutdown_timer();

		return(0);
	}

	/* Error: New time */
	printk_error_shutdown_timer();

	return(-1);
	
}


SYSCALL_DEFINE3(shutdown_timer, long, function_number, s32, lo_halt_time_sec, s32, hi_halt_time_sec)
{

/*
 * if your USERLAND PROGRAM (SYSCALL) can handle the BYTE ORDER this is
 * not necessary (__BIG_ENDIAN)
 */
#ifdef __BIG_ENDIAN
#define LO 1
#define HI 0
#else
#define LO  0
#define HI  1
#endif

	/* Safer than pointer */
	union {
		s64 t_64;
		s32 t_32[2];
	} halt_time_sec;

	/* simple */
	/* ID check. ONLY USER ID 0 */
	if (get_current_user()->uid.val != 0) {
		printk_help_shutdown_timer();
		return(-1);
	}

	switch(function_number) {
		/* shutdown. timer init */
		case 0:
			halt_time_sec.t_32[LO] = lo_halt_time_sec;
			halt_time_sec.t_32[HI] = hi_halt_time_sec;
			return(init_shutdown_timer(halt_time_sec.t_64));

		/* shutdown. new time set. only lower than remaining time */
		case 1:
			halt_time_sec.t_32[LO] = lo_halt_time_sec;
			halt_time_sec.t_32[HI] = hi_halt_time_sec;
			return(set_shutdown_timer(halt_time_sec.t_64));

		/* if not OK */
		default:
			printk_help_shutdown_timer();
			return(-1);
	}
}
