/* Copyright (c) 2021/08, Peter Boettcher, Germany/NRW, Muelheim Ruhr
 * Urheber: 2021/08, Peter Boettcher, Germany/NRW, Muelheim Ruhr

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

 /* gcc:
    gcc -o set.shutdown set.shutdown.c
 */



#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>


typedef int8_t s8;
typedef u_int8_t u8;
typedef int16_t s16;
typedef u_int16_t u16;
typedef int32_t s32;
typedef u_int32_t u32;
typedef long long s64;
typedef unsigned long long u64;

#define VERSION 0


int TryStrToInt64 (char *STRING_NUMBER, s64 *NUMBER, int ZAHLEN_SYSTEM) {

	char *FEHLER;

	if (strlen(STRING_NUMBER) == 0) return (-1);

	*NUMBER = strtoll(STRING_NUMBER, &FEHLER, ZAHLEN_SYSTEM);
	if (strlen(FEHLER) != 0) return (-1);

	if (*NUMBER == 9223372036854775807) {
		if (strncmp(STRING_NUMBER, "9223372036854775807", 19) != 0) return(-1);
		else return(0);
	}

	/* Warning gcc: -9223372036854775808 */
	if (*NUMBER + 1 == -9223372036854775807) {
		if (strncmp(STRING_NUMBER, "-9223372036854775808", 20) != 0) return(-1);
		else return(0);
	}

	return(0);
}


/* BYTE ORDER SAVE? */
/* Not check. */
u32 u64_hi(u64 register value) {
	register u32 RET_VAL;

	RET_VAL = value >> 32;
	return(RET_VAL);
}

u32 u64_lo(u64 register value) {
	register u32 RET_VAL;

	RET_VAL = (u32) value;
	return(RET_VAL);
}

u16 u32_hi(u32 register value) {
	register u16 RET_VAL;
	
	RET_VAL = value >> 16;
	return(RET_VAL);
}

u16 u32_lo(u32 register value) {
	register u16 RET_VAL;
	
	RET_VAL = (u16) value;
	return(RET_VAL);
}

u8 u16_hi(u16 register value) {
	register u8 RET_VAL;
	
	RET_VAL = value >> 8;
	return(RET_VAL);
}

u8 u16_lo(u16 register value) {
	register u8 RET_VAL;
	

	RET_VAL = (u8) value;
	return(RET_VAL);
}



long Error_Message(void)
{

	printf("SET.SHUTDOWN, 2021/8 Peter Boettcher, Germany, Muelheim Ruhr\n");
#if VERSION == 0
	printf("VERSION     : 0, C, gcc\n\n");
#endif

#if VERSION == 1
	printf("VERSION     : 1, C, gcc\n\n");
#endif

#if VERSION == 2
	printf("VERSION     : 2, C, gcc\n\n");
#endif

	printf("SET TIME    : <-S> <Sec.>\n");
	printf("SET TIME    : <-M> <Min.>\n");
	printf("SET TIME    : <-H> <Hour> <Min>\n");
	printf("SET TIME    : <-D> <Day>  <Hour><Min>\n");
	printf("SET TIME    : <-Y> <YEAR> <Day>  <Hour> <Min>\n\n");

	printf("NEW TIME    : <-nS> <Sec.>\n");
	printf("NEW TIME    : <-nM> <Min.>\n");
	printf("NEW TIME    : <-nH> <Hour> <Min> \n");
	printf("NEW TIME    : <-nD> <Day>  <Hour> <Min>\n");
	printf("NEW TIME    : <-nY> <YEAR> <Day>  <Hour> <Min>\n");
	printf("-1\n");
	exit(1);
}





#if VERSION == 0
int main(int argc, char *argv[])
{

	s64 YEAR = 0;
	s64 DAY = 0;
	s64 HOUR = 0;
	s64 MIN = 0;
	s64 SHUTDOWN_TIME = 0;
	
#define SYSCALL_NR 499		//free syscall
#define SYSCALL_INIT 0
#define SYSCALL_NEW_TIME 1


	if (argc < 2) Error_Message();

	if (strstr(argv[1], "-S")) {
		if (argc != 3) Error_Message();

		if (TryStrToInt64 (argv[2], &SHUTDOWN_TIME, 10) != 0) Error_Message();
		if (SHUTDOWN_TIME < 0) Error_Message();

		printf("SHUTDOWN TIME IN SEC. %ld\n", SHUTDOWN_TIME);
		printf("%d\n", syscall(SYSCALL_NR, SYSCALL_INIT, SHUTDOWN_TIME));
		exit(0);
	}

	if (strstr(argv[1], "-M")) {
		if (argc != 3) Error_Message();

		if (TryStrToInt64 (argv[2], &SHUTDOWN_TIME, 10) != 0) Error_Message();

		SHUTDOWN_TIME *= 60;
		if (SHUTDOWN_TIME < 0) Error_Message();

		printf("SHUTDOwn TIME IN SEC. %ld\n", SHUTDOWN_TIME);
		printf("%d\n", syscall(SYSCALL_NR, SYSCALL_INIT, SHUTDOWN_TIME));
		exit(0);
	}

	if (strstr(argv[1], "-H")) {
		if (argc != 4) Error_Message();

		if (TryStrToInt64 (argv[2], &HOUR, 10) != 0) Error_Message();
		if (TryStrToInt64 (argv[3], &MIN, 10) != 0) Error_Message();

		SHUTDOWN_TIME = HOUR * 3600;
		SHUTDOWN_TIME += MIN * 60;
		if (SHUTDOWN_TIME < 0) Error_Message();

		printf("SHUTDOWN TIME IN SEC. %ld\n", SHUTDOWN_TIME);
		printf("%d\n", syscall(SYSCALL_NR, SYSCALL_INIT, SHUTDOWN_TIME));
		exit(0);
	}


	if (strstr(argv[1], "-D")) {
		if (argc != 5) Error_Message();
		
		if (TryStrToInt64 (argv[2], &DAY, 10) != 0) Error_Message();
		if (TryStrToInt64 (argv[3], &HOUR, 10) != 0) Error_Message();
		if (TryStrToInt64 (argv[4], &MIN, 10) != 0) Error_Message();

		SHUTDOWN_TIME = DAY * 24 * 3600;
		SHUTDOWN_TIME += HOUR * 3600;
		SHUTDOWN_TIME += MIN * 60;
		if (SHUTDOWN_TIME < 0) Error_Message();

		printf("SHUTDOWN TIME IN SEC. %ld\n", SHUTDOWN_TIME);
		printf("%d\n", syscall(SYSCALL_NR, SYSCALL_INIT, SHUTDOWN_TIME));
		exit(0);
	}

	if (strstr(argv[1], "-Y")) {
		if (argc != 6) Error_Message();

		if (TryStrToInt64 (argv[2], &YEAR, 10) != 0) Error_Message();
		if (TryStrToInt64 (argv[3], &DAY, 10) != 0) Error_Message();
		if (TryStrToInt64 (argv[4], &HOUR, 10) != 0) Error_Message();
		if (TryStrToInt64 (argv[5], &MIN, 10) != 0) Error_Message();

		SHUTDOWN_TIME = YEAR * 365 * 24 * 3600;
		SHUTDOWN_TIME += DAY * 24 * 3600;
		SHUTDOWN_TIME += HOUR * 3600;
		SHUTDOWN_TIME += MIN * 60;
		if (SHUTDOWN_TIME < 0) Error_Message();

		printf("SHUTDOWN TIME IN SEC. %ld\n", SHUTDOWN_TIME);
		printf("%d\n", syscall(SYSCALL_NR, SYSCALL_INIT, SHUTDOWN_TIME));
		exit(0);
	}

	if (strstr(argv[1], "-nS")) {
		if (argc != 3) Error_Message();

		if (TryStrToInt64 (argv[2], &SHUTDOWN_TIME, 10) != 0) Error_Message();
		if (SHUTDOWN_TIME < 0) Error_Message();

		printf("SHUTDOWN TIME IN SEC. %ld\n", SHUTDOWN_TIME);
		printf("%d\n", syscall(SYSCALL_NR, SYSCALL_NEW_TIME, SHUTDOWN_TIME));
		exit(0);
	}

	if (strstr(argv[1], "-nM")) {
		if (argc != 3) Error_Message();
		if (TryStrToInt64 (argv[2], &SHUTDOWN_TIME, 10) != 0) Error_Message();
		
		SHUTDOWN_TIME *= 60;
		if (SHUTDOWN_TIME < 0) Error_Message();
		
		printf("SHUTDOWN TIME IN SEC. %ld\n", SHUTDOWN_TIME);
		printf("%d\n", syscall(SYSCALL_NR, SYSCALL_NEW_TIME, SHUTDOWN_TIME));
		exit(0);
	}

	if (strstr(argv[1], "-nH")) {
		if (argc != 4) Error_Message();
		if (TryStrToInt64 (argv[2], &HOUR, 10) != 0) Error_Message();
		if (TryStrToInt64 (argv[3], &MIN, 10) != 0) Error_Message();

		SHUTDOWN_TIME = HOUR * 3600;
		SHUTDOWN_TIME += MIN * 60;
		if (SHUTDOWN_TIME < 0) Error_Message();
		
		printf("SHUTDOWN TIME IN SEC. %ld\n", SHUTDOWN_TIME);
		printf("%d\n", syscall(SYSCALL_NR, SYSCALL_NEW_TIME, SHUTDOWN_TIME));
		exit(0);
	}

	if (strstr(argv[1], "-nD")) {
		if (argc != 5) Error_Message();
		if (TryStrToInt64 (argv[2], &DAY, 10) != 0) Error_Message();
		if (TryStrToInt64 (argv[3], &HOUR, 10) != 0) Error_Message();
		if (TryStrToInt64 (argv[4], &MIN, 10) != 0) Error_Message();

		SHUTDOWN_TIME = DAY * 24 * 3600;
		SHUTDOWN_TIME += HOUR * 3600;
		SHUTDOWN_TIME += MIN * 60;
		if (SHUTDOWN_TIME < 0) Error_Message();

		printf("SHUTDOWN TIME IN SEC. %ld\n", SHUTDOWN_TIME);
		printf("%d\n", syscall(SYSCALL_NR, SYSCALL_NEW_TIME, SHUTDOWN_TIME));
		exit(0);
	}

	if (strstr(argv[1], "-nY")) {
		if (argc != 6) Error_Message();

		if (TryStrToInt64 (argv[2], &YEAR, 10) != 0) Error_Message();
		if (TryStrToInt64 (argv[3], &DAY, 10) != 0) Error_Message();
		if (TryStrToInt64 (argv[4], &HOUR, 10) != 0) Error_Message();
		if (TryStrToInt64 (argv[5], &MIN, 10) != 0) Error_Message();

		SHUTDOWN_TIME = YEAR * 365 * 24 * 3600;
		SHUTDOWN_TIME += DAY * 24 * 3600;
		SHUTDOWN_TIME += HOUR * 3600;
		SHUTDOWN_TIME += MIN * 60;
		if (SHUTDOWN_TIME < 0) Error_Message();

		printf("SHUTDOWN TIME IN SEC. %ld\n", SHUTDOWN_TIME);
		printf("%d\n", syscall(SYSCALL_NR, SYSCALL_NEW_TIME, SHUTDOWN_TIME));
		exit(0);
	}

	Error_Message();

}
#endif



#if VERSION == 1
int main(int argc, char *argv[])
{

#define SYSCALL_NR 499		//free syscall
#define SYSCALL_INIT 0
#define SYSCALL_NEW_TIME 1

#define LO 0
#define HI 1


	s64 YEAR = 0;
	s64 DAY = 0;
	s64 HOUR = 0;
	s64 MIN = 0;
	

	union {
		s64 t_64;
		s32 t_32[2];
	} SHUTDOWN_TIME;


	if (argc < 2) Error_Message();

	if (strstr(argv[2], "-S")) {
		if (argc != 3) Error_Message();

		if (TryStrToInt64 (argv[2], &SHUTDOWN_TIME.t_64, 10) != 0) Error_Message();
		if (SHUTDOWN_TIME.t_64 < 0) Error_Message();

		printf("SHUTDOWN TIME IN SEC. %lld\n", SHUTDOWN_TIME);
		printf("%d\n", syscall(SYSCALL_NR, SYSCALL_INIT, SHUTDOWN_TIME.t_32[LO], SHUTDOWN_TIME.t_32[HI]));
		exit(0);
	}

	if (strstr(argv[2], "-M")) {
		if (argc != 3) Error_Message();

		if (TryStrToInt64 (argv[2], &SHUTDOWN_TIME.t_64, 10) != 0) Error_Message();
		SHUTDOWN_TIME.t_64 *= 60;
		if (SHUTDOWN_TIME.t_64 < 0) Error_Message();

		printf("SHUTDOwn TIME IN SEC. %lld\n", SHUTDOWN_TIME.t_64);
		printf("%d\n", syscall(SYSCALL_NR, SYSCALL_INIT, SHUTDOWN_TIME.t_32[LO], SHUTDOWN_TIME.t_32[HI]));
		exit(0);
	}

	if (strstr(argv[2], "-H")) {
		if (argc != 4) Error_Message();

		if (TryStrToInt64 (argv[2], &HOUR, 10) != 0) Error_Message();
		if (TryStrToInt64 (argv[3], &MIN, 10) != 0) Error_Message();

		SHUTDOWN_TIME.t_64 = HOUR * 3600;
		SHUTDOWN_TIME.t_64 += MIN * 60;
		if (SHUTDOWN_TIME.t_64 < 0) Error_Message();

		printf("SHUTDOWN TIME IN SEC. %lld\n", SHUTDOWN_TIME.t_64);
		printf("%d\n", syscall(SYSCALL_NR, SYSCALL_INIT, SHUTDOWN_TIME.t_32[LO], SHUTDOWN_TIME.t_32[HI]));
		exit(0);
	}


	if (strstr(argv[2], "-D")) {
		if (argc != 5) Error_Message();
		
		if (TryStrToInt64 (argv[2], &DAY, 10) != 0) Error_Message();
		if (TryStrToInt64 (argv[3], &HOUR, 10) != 0) Error_Message();
		if (TryStrToInt64 (argv[4], &MIN, 10) != 0) Error_Message();

		SHUTDOWN_TIME.t_64 = DAY * 24 * 3600;
		SHUTDOWN_TIME.t_64 += HOUR * 3600;
		SHUTDOWN_TIME.t_64 += MIN * 60;
		if (SHUTDOWN_TIME.t_64 < 0) Error_Message();

		printf("SHUTDOWN TIME IN SEC. %lld\n", SHUTDOWN_TIME.t_64);
		printf("%d\n", syscall(SYSCALL_NR, SYSCALL_INIT, SHUTDOWN_TIME.t_32[LO], SHUTDOWN_TIME.t_32[HI]));
		exit(0);
	}

	if (strstr(argv[2], "-Y")) {
		if (argc != 6) Error_Message();

		if (TryStrToInt64 (argv[2], &YEAR, 10) != 0) Error_Message();
		if (TryStrToInt64 (argv[3], &DAY, 10) != 0) Error_Message();
		if (TryStrToInt64 (argv[4], &HOUR, 10) != 0) Error_Message();
		if (TryStrToInt64 (argv[5], &MIN, 10) != 0) Error_Message();

		SHUTDOWN_TIME.t_64 = YEAR * 365 * 24 * 3600;
		SHUTDOWN_TIME.t_64 += DAY * 24 * 3600;
		SHUTDOWN_TIME.t_64 += HOUR * 3600;
		SHUTDOWN_TIME.t_64 += MIN * 60;
		if (SHUTDOWN_TIME.t_64 < 0) Error_Message();

		printf("SHUTDOWN TIME IN SEC. %lld\n", SHUTDOWN_TIME);
		printf("%d\n", syscall(SYSCALL_NR, SYSCALL_INIT, SHUTDOWN_TIME.t_32[LO], SHUTDOWN_TIME.t_32[HI]));
		exit(0);
	}

	if (strstr(argv[2], "-nS")) {
		if (argc != 3) Error_Message();
		if (TryStrToInt64 (argv[2], &SHUTDOWN_TIME.t_64, 10) != 0) Error_Message();
		if (SHUTDOWN_TIME.t_64 < 0) Error_Message();

		printf("SHUTDOWN TIME IN SEC. %lld\n", SHUTDOWN_TIME.t_64);
		printf("%d\n", syscall(SYSCALL_NR, SYSCALL_NEW_TIME, SHUTDOWN_TIME.t_32[LO], SHUTDOWN_TIME.t_32[HI]));
		exit(0);
	}

	if (strstr(argv[2], "-nM")) {
		if (argc != 3) Error_Message();
		if (TryStrToInt64 (argv[2], &SHUTDOWN_TIME.t_64, 10) != 0) Error_Message();
		
		SHUTDOWN_TIME.t_64 *= 60;
		if (SHUTDOWN_TIME.t_64 < 0) Error_Message();

		printf("SHUTDOWN TIME IN SEC. %lld\n", SHUTDOWN_TIME);
		printf("%d\n", syscall(SYSCALL_NR, SYSCALL_NEW_TIME, SHUTDOWN_TIME.t_32[LO], SHUTDOWN_TIME.t_32[HI]));
		exit(0);
	}

	if (strstr(argv[2], "-nH")) {
		if (argc != 4) Error_Message();
		if (TryStrToInt64 (argv[2], &HOUR, 10) != 0) Error_Message();
		if (TryStrToInt64 (argv[3], &MIN, 10) != 0) Error_Message();

		SHUTDOWN_TIME.t_64 = HOUR * 3600;
		SHUTDOWN_TIME.t_64 += MIN * 60;
		if (SHUTDOWN_TIME.t_64 < 0) Error_Message();

		printf("SHUTDOWN TIME IN SEC. %lld\n", SHUTDOWN_TIME.t_64);
		printf("%d\n", syscall(SYSCALL_NR, SYSCALL_NEW_TIME, SHUTDOWN_TIME.t_32[LO], SHUTDOWN_TIME.t_32[HI]));
		exit(0);
	}

	if (strstr(argv[2], "-nD")) {
		if (argc != 5) Error_Message();
		if (TryStrToInt64 (argv[2], &DAY, 10) != 0) Error_Message();
		if (TryStrToInt64 (argv[3], &HOUR, 10) != 0) Error_Message();
		if (TryStrToInt64 (argv[4], &MIN, 10) != 0) Error_Message();

		SHUTDOWN_TIME.t_64 = DAY * 24 * 3600;
		SHUTDOWN_TIME.t_64 += HOUR * 3600;
		SHUTDOWN_TIME.t_64 += MIN * 60;
		if (SHUTDOWN_TIME.t_64 < 0) Error_Message();

		printf("SHUTDOWN TIME IN SEC. %lld\n", SHUTDOWN_TIME);
		printf("%d\n", syscall(SYSCALL_NR, SYSCALL_NEW_TIME, SHUTDOWN_TIME.t_32[LO], SHUTDOWN_TIME.t_32[HI]));
		exit(0);
	}

	if (strstr(argv[2], "-nY")) {
		if (argc != 6) Error_Message();

		if (TryStrToInt64 (argv[2], &YEAR, 10) != 0) Error_Message();
		if (TryStrToInt64 (argv[3], &DAY, 10) != 0) Error_Message();
		if (TryStrToInt64 (argv[4], &HOUR, 10) != 0) Error_Message();
		if (TryStrToInt64 (argv[5], &MIN, 10) != 0) Error_Message();

		SHUTDOWN_TIME.t_64 = YEAR * 365 * 24 * 3600;
		SHUTDOWN_TIME.t_64 += DAY * 24 * 3600;
		SHUTDOWN_TIME.t_64 += HOUR * 3600;
		SHUTDOWN_TIME.t_64 += MIN * 60;
		if (SHUTDOWN_TIME.t_64 < 0) Error_Message();

		printf("SHUTDOWN TIME IN SEC. %lld\n", SHUTDOWN_TIME);
		printf("%d\n", syscall(SYSCALL_NR, SYSCALL_NEW_TIME, SHUTDOWN_TIME.t_32[LO], SHUTDOWN_TIME.t_32[HI]));
		exit(0);
	}

	Error_Message();

}
#endif


#if VERSION == 2
int main(int argc, char *argv[])
{

#define SYSCALL_NR 499		//free syscall
#define SYSCALL_INIT 0
#define SYSCALL_NEW_TIME 1

	s64 YEAR = 0;
	s64 DAY = 0;
	s64 HOUR = 0;
	s64 MIN = 0;
	s64 SHUTDOWN_TIME;



	if (argc < 2) Error_Message();


	if (strstr(argv[1], "-S")) {
		if (argc != 3) Error_Message();


		if (TryStrToInt64 (argv[2], &SHUTDOWN_TIME, 10) != 0) Error_Message();
		if (SHUTDOWN_TIME < 0) Error_Message();

		printf("SHUTDOWN TIME IN SEC. %lld\n", SHUTDOWN_TIME);
		/* BYTE ORDER RESIST */
		printf("%d\n", syscall(SYSCALL_NR, SYSCALL_INIT, u64_lo(SHUTDOWN_TIME), u64_hi(SHUTDOWN_TIME)));

		exit(0);
	}

	if (strstr(argv[1], "-M")) {
		if (argc != 3) Error_Message();

		if (TryStrToInt64 (argv[2], &SHUTDOWN_TIME, 10) != 0) Error_Message();
		SHUTDOWN_TIME *= 60;
		if (SHUTDOWN_TIME < 0) Error_Message();

		printf("SHUTDOwn TIME IN SEC. %lld\n", SHUTDOWN_TIME);
		printf("%d\n", syscall(SYSCALL_NR, SYSCALL_INIT, u64_lo(SHUTDOWN_TIME), u64_hi(SHUTDOWN_TIME)));
		exit(0);
	}

	if (strstr(argv[2], "-H")) {
		if (argc != 4) Error_Message();

		if (TryStrToInt64 (argv[2], &HOUR, 10) != 0) Error_Message();
		if (TryStrToInt64 (argv[3], &MIN, 10) != 0) Error_Message();

		SHUTDOWN_TIME = HOUR * 3600;
		SHUTDOWN_TIME += MIN * 60;
		if (SHUTDOWN_TIME < 0) Error_Message();

		printf("SHUTDOWN TIME IN SEC. %lld\n", SHUTDOWN_TIME);
		printf("%d\n", syscall(SYSCALL_NR, SYSCALL_INIT, u64_lo(SHUTDOWN_TIME), u64_hi(SHUTDOWN_TIME)));
		exit(0);
	}


	if (strstr(argv[1], "-D")) {
		if (argc != 5) Error_Message();
		
		if (TryStrToInt64 (argv[2], &DAY, 10) != 0) Error_Message();
		if (TryStrToInt64 (argv[3], &HOUR, 10) != 0) Error_Message();
		if (TryStrToInt64 (argv[4], &MIN, 10) != 0) Error_Message();

		SHUTDOWN_TIME = DAY * 24 * 3600;
		SHUTDOWN_TIME += HOUR * 3600;
		SHUTDOWN_TIME += MIN * 60;
		if (SHUTDOWN_TIME < 0) Error_Message();

		printf("SHUTDOWN TIME IN SEC. %lld\n", SHUTDOWN_TIME);
		printf("%d\n", syscall(SYSCALL_NR, SYSCALL_INIT, u64_lo(SHUTDOWN_TIME), u64_hi(SHUTDOWN_TIME)));
		exit(0);
	}

	if (strstr(argv[1], "-Y")) {
		if (argc != 6) Error_Message();

		if (TryStrToInt64 (argv[2], &YEAR, 10) != 0) Error_Message();
		if (TryStrToInt64 (argv[3], &DAY, 10) != 0) Error_Message();
		if (TryStrToInt64 (argv[4], &HOUR, 10) != 0) Error_Message();
		if (TryStrToInt64 (argv[5], &MIN, 10) != 0) Error_Message();

		SHUTDOWN_TIME = YEAR * 365 * 24 * 3600;
		SHUTDOWN_TIME += DAY * 24 * 3600;
		SHUTDOWN_TIME += HOUR * 3600;
		SHUTDOWN_TIME += MIN * 60;
		if (SHUTDOWN_TIME < 0) Error_Message();

		printf("SHUTDOWN TIME IN SEC. %lld\n", SHUTDOWN_TIME);
		printf("%d\n", syscall(SYSCALL_NR, SYSCALL_INIT, u64_lo(SHUTDOWN_TIME), u64_hi(SHUTDOWN_TIME)));
		exit(0);
	}

	if (strstr(argv[1], "-nS")) {
		if (argc != 3) Error_Message();
		if (TryStrToInt64 (argv[2], &SHUTDOWN_TIME, 10) != 0) Error_Message();
		if (SHUTDOWN_TIME < 0) Error_Message();

		printf("SHUTDOWN TIME IN SEC. %lld\n", SHUTDOWN_TIME);
		printf("%d\n", syscall(SYSCALL_NR, SYSCALL_INIT, u64_lo(SHUTDOWN_TIME), u64_hi(SHUTDOWN_TIME)));
		exit(0);
	}

	if (strstr(argv[1], "-nM")) {
		if (argc != 3) Error_Message();
		if (TryStrToInt64 (argv[2], &SHUTDOWN_TIME, 10) != 0) Error_Message();
		
		SHUTDOWN_TIME *= 60;
		if (SHUTDOWN_TIME < 0) Error_Message();

		printf("SHUTDOWN TIME IN SEC. %lld\n", SHUTDOWN_TIME);
		printf("%d\n", syscall(SYSCALL_NR, SYSCALL_NEW_TIME, u64_lo(SHUTDOWN_TIME), u64_hi(SHUTDOWN_TIME)));
		exit(0);
	}

	if (strstr(argv[1], "-nH")) {
		if (argc != 4) Error_Message();
		if (TryStrToInt64 (argv[2], &HOUR, 10) != 0) Error_Message();
		if (TryStrToInt64 (argv[3], &MIN, 10) != 0) Error_Message();

		SHUTDOWN_TIME = HOUR * 3600;
		SHUTDOWN_TIME += MIN * 60;
		if (SHUTDOWN_TIME < 0) Error_Message();

		printf("SHUTDOWN TIME IN SEC. %lld\n", SHUTDOWN_TIME);
		printf("%d\n", syscall(SYSCALL_NR, SYSCALL_NEW_TIME, u64_lo(SHUTDOWN_TIME), u64_hi(SHUTDOWN_TIME)));
		exit(0);
	}

	if (strstr(argv[1], "-nD")) {
		if (argc != 5) Error_Message();
		if (TryStrToInt64 (argv[2], &DAY, 10) != 0) Error_Message();
		if (TryStrToInt64 (argv[3], &HOUR, 10) != 0) Error_Message();
		if (TryStrToInt64 (argv[4], &MIN, 10) != 0) Error_Message();

		SHUTDOWN_TIME = DAY * 24 * 3600;
		SHUTDOWN_TIME += HOUR * 3600;
		SHUTDOWN_TIME += MIN * 60;
		if (SHUTDOWN_TIME < 0) Error_Message();

		printf("SHUTDOWN TIME IN SEC. %lld\n", SHUTDOWN_TIME);
		printf("%d\n", syscall(SYSCALL_NR, SYSCALL_NEW_TIME, u64_lo(SHUTDOWN_TIME), u64_hi(SHUTDOWN_TIME)));
		exit(0);
	}

	if (strstr(argv[1], "-nY")) {
		if (argc != 6) Error_Message();

		if (TryStrToInt64 (argv[2], &YEAR, 10) != 0) Error_Message();
		if (TryStrToInt64 (argv[3], &DAY, 10) != 0) Error_Message();
		if (TryStrToInt64 (argv[4], &HOUR, 10) != 0) Error_Message();
		if (TryStrToInt64 (argv[5], &MIN, 10) != 0) Error_Message();

		SHUTDOWN_TIME = YEAR * 365 * 24 * 3600;
		SHUTDOWN_TIME += DAY * 24 * 3600;
		SHUTDOWN_TIME += HOUR * 3600;
		SHUTDOWN_TIME += MIN * 60;
		if (SHUTDOWN_TIME < 0) Error_Message();

		printf("SHUTDOWN TIME IN SEC. %lld\n", SHUTDOWN_TIME);
		printf("%d\n", syscall(SYSCALL_NR, SYSCALL_NEW_TIME, u64_lo(SHUTDOWN_TIME), u64_hi(SHUTDOWN_TIME)));

		exit(0);
	}

	Error_Message();

}
#endif
