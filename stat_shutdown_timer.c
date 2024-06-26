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

/*Important: INTEL: 32Bit Linux Long=32Bit. INTEL: 64Bit Linux Long=64Bit */



#include <linux/fs.h>
#include <linux/kernel.h>
#include <linux/proc_fs.h>
#include <linux/seq_file.h>


/* proto. */
struct shutdown_timer_info_struct {
	s64 shutdown_time_sec;
	bool shutdown_flag;
};


static struct shutdown_timer_info_struct info;

extern void info_shutdown_timer(struct shutdown_timer_info_struct *info);



static s64 shutdown_time_days_modulo(void)
{

#if BITS_PER_LONG == 32
	s64 time;

	time = info.shutdown_time_sec;
	time = do_div(time, 3600 * 24 * 365);
	return(time);
#endif

#if BITS_PER_LONG == 64
	return((info.shutdown_time_sec / 3600 / 24) % 365);
#endif

}


static s64 shutdown_time_years(void)
{

#if BITS_PER_LONG == 32
	s64 time;

	time = info.shutdown_time_sec;
	do_div(time, 365 * 24 * 3600);
	return(time);
#endif

#if BITS_PER_LONG == 64
	return(info.shutdown_time_sec / 365 / 24 / 3600);
#endif

}


static s64 shutdown_time_hours_modulo(void)
{

#if BITS_PER_LONG == 32
	s64 time;
	u64 remainder;

	time = info.shutdown_time_sec;
	do_div(time, 3600);
	remainder = do_div(time, 24);
	return(remainder);
#endif

#if BITS_PER_LONG == 64
	return((info.shutdown_time_sec / 3600) % 24);
#endif

}


static s64 shutdown_time_min_modulo(void)
{

#if BITS_PER_LONG == 32
	s64 time;
	u64 remainder;

	time = info.shutdown_time_sec;
	do_div(time, 60);
	remainder = do_div(time, 60);
	return(remainder);
#endif

#if BITS_PER_LONG == 64
	return((info.shutdown_time_sec / 60) % 60);
#endif

}


static int shutdown_timer_proc_show(struct seq_file *proc_show, void *v)
{

	info_shutdown_timer(&info);

	if (info.shutdown_flag == true) {
		seq_printf(proc_show,
			"SHUTDOWN TIMER           : ACTIVE. TIME BASE 5 SEC.\n");

		seq_printf(proc_show,
			"SHUTDOWN TIMER           : %lld YEARS, %lld DAYS, %lld:%lld HOURS\n",
			shutdown_time_years(),
			shutdown_time_days_modulo(),
			shutdown_time_hours_modulo(),
			shutdown_time_min_modulo());

		seq_printf(proc_show,
			"SHUTDOWN TIMER           : %lld SEC.\n", info.shutdown_time_sec);

	}
	else {
		seq_printf(proc_show,
			"SHUTDOWN TIMER           : NOT ACTIVE\n\n");
	}

	return(0);
}


static int __init init_shutdown_timer_proc_show(void)
{
	proc_create_single("stat.shutdown.timer", 0, NULL, shutdown_timer_proc_show);
	return(0);
}
fs_initcall(init_shutdown_timer_proc_show);

