(* Copyright (c) 2021/08, Peter Boettcher, Germany/NRW, Muelheim Ruhr
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
 *)

 (* FreePascal:
  * fpc -B set.shutdown.pas
  * Version_0: 1 Parameter
  * Other    : 2 Parameter, lo, hi. BYTER ORDER OK.
  *
 *)


Uses
	syscall,
	sysutils;

{$define VERSION_0}


Procedure Error_Message;
begin
	writeln('SET.SHUTDOWN, 2021/8 Peter Boettcher, Germany, Muelheim Ruhr');

{$ifdef VERSION_0}
	writeln('VERSION     : PASCAL 0, fpc');
{$else VERSION_0}
	writeln('VERSION     : PASCAL 1, fpc');
{$endif VERSION_0}

	writeln;
	writeln('SET TIME    : <-S> <Sec.>');
	writeln('SET TIME    : <-M> <Min.>');
	writeln('SET TIME    : <-H> <Hour> <Min>');
	writeln('SET TIME    : <-D> <Day>  <Hour> <Min>');
	writeln('SET TIME    : <-Y> <YEAR> <Day>  <Hour> <Min>');
	writeln;
	writeln('NEW TIME    : <-nS> <Sec.>');
	writeln('NEW TIME    : <-nM> <Min.>');
	writeln('NEW TIME    : <-nH> <Hour> <Min>');
	writeln('NEW TIME    : <-nD> <Day>  <Hour> <Min>');
	writeln('NEW TIME    : <-nY> <YEAR> <Day>  <Hour> <Min>');
	writeln(-1);
	halt(1);
end;




var
	YEAR		: int64 = 0;
	DAY		: int64 = 0;
	HOUR		: int64 = 0;
	MIN		: int64 = 0;
	SHUTDOWN_TIME	: int64 = 0;



const
	SYSCALL_NR		= 499;		//free syscall
	SYSCALL_INIT		= 0;
	SYSCALL_NEW_TIME	= 1;



//simple
begin
	If ParamCount < 1 then Error_Message;
	
	if ParamStr(1) = '-S' then begin
		if ParamCount <> 2 then Error_Message;

		if TryStrToInt64(ParamStr(2), SHUTDOWN_TIME) = False then Error_Message;
		if SHUTDOWN_TIME < 0 then Error_Message;

		writeln('SHUTDOWN TIME IN SEC. ', SHUTDOWN_TIME);

{$ifdef VERSION_0}
		writeln(do_SysCall(SYSCALL_NR, SYSCALL_INIT, SHUTDOWN_TIME));
{$else VERSION_0}
//Byte Order save? Not check.
		writeln(do_SysCall(SYSCALL_NR, SYSCALL_INIT, lo(SHUTDOWN_TIME), hi(SHUTDOWN_TIME)));
{$endif VERSION_0}

		halt(0);
	end;

	if ParamStr(1) = '-M' then begin
		if ParamCount <> 2 then Error_Message;

		if TryStrToInt64(ParamStr(2), SHUTDOWN_TIME) = False then Error_Message;

		SHUTDOWN_TIME *= 60;
		if SHUTDOWN_TIME < 0 then Error_Message;

		writeln('SHUTDOWN TIME IN SEC. ', SHUTDOWN_TIME);

{$ifdef VERSION_0}
		writeln(do_SysCall(SYSCALL_NR, SYSCALL_INIT, SHUTDOWN_TIME));
{$else VERSION_0}
		writeln(do_SysCall(SYSCALL_NR, SYSCALL_INIT, lo(SHUTDOWN_TIME), hi(SHUTDOWN_TIME)));
{$endif VERSION_0}

		halt(0);
	end;

	if ParamStr(1) = '-H' then begin
		if ParamCount <> 3 then Error_Message;

		if TryStrToInt64(ParamStr(2), HOUR) = False then Error_Message;
		if TryStrToInt64(ParamStr(3), MIN) = False then Error_Message;

		SHUTDOWN_TIME := HOUR * 3600;
		SHUTDOWN_TIME += MIN * 60;
		if SHUTDOWN_TIME < 0 then Error_Message;

		writeln('SHUTDOWN TIME IN SEC. ', SHUTDOWN_TIME);
{$ifdef VERSION_0}
		writeln(do_SysCall(SYSCALL_NR, SYSCALL_INIT, SHUTDOWN_TIME));
{$else VERSION_0}
		writeln(do_SysCall(SYSCALL_NR, SYSCALL_INIT, lo(SHUTDOWN_TIME), hi(SHUTDOWN_TIME)));
{$endif VERSION_0}
		halt(0);
	end;

	if ParamStr(1) = '-D' then begin
		if ParamCount <> 4 then Error_Message;

		if TryStrToInt64(ParamStr(2), DAY) = False then Error_Message;
		if TryStrToInt64(ParamStr(3), HOUR) = False then Error_Message;
		if TryStrToInt64(ParamStr(4), MIN) = False then Error_Message;

		SHUTDOWN_TIME := DAY * 24 * 3600;
		SHUTDOWN_TIME += HOUR * 3600;
		SHUTDOWN_TIME += MIN * 60;
		if SHUTDOWN_TIME < 0 then Error_Message;

		writeln('SHUTDOWN TIME IN SEC. ', SHUTDOWN_TIME);
{$ifdef VERSION_0}
		writeln(do_SysCall(SYSCALL_NR, SYSCALL_INIT, SHUTDOWN_TIME));
{$else VERSION_0}
		writeln(do_SysCall(SYSCALL_NR, SYSCALL_INIT, lo(SHUTDOWN_TIME), hi(SHUTDOWN_TIME)));
{$endif VERSION_0}
		halt(0);
	end;

	if ParamStr(1) = '-Y' then begin
		if ParamCount <> 5 then Error_Message;

		if TryStrToInt64(ParamStr(2), YEAR) = False then Error_Message;
		if TryStrToInt64(ParamStr(3), DAY) = False then Error_Message;
		if TryStrToInt64(ParamStr(4), HOUR) = False then Error_Message;
		if TryStrToInt64(ParamStr(5), MIN) = False then Error_Message;

		SHUTDOWN_TIME := YEAR * 365 * 24 * 3600;
		SHUTDOWN_TIME += DAY * 24 * 3600;
		SHUTDOWN_TIME += HOUR * 3600;
		SHUTDOWN_TIME += MIN * 60;
		if SHUTDOWN_TIME < 0 then Error_Message;

		writeln('SHUTDOWN TIME IN SEC. ', SHUTDOWN_TIME);

{$ifdef VERSION_0}
		writeln(do_SysCall(SYSCALL_NR, SYSCALL_INIT, SHUTDOWN_TIME));
{$else VERSION_0}
		writeln(do_SysCall(SYSCALL_NR, SYSCALL_INIT, lo(SHUTDOWN_TIME), hi(SHUTDOWN_TIME)));
{$endif VERSION_0}

		halt(0);
	end;



	if ParamStr(1) = '-nS' then begin
		if ParamCount <> 2 then Error_Message;

		if TryStrToInt64(ParamStr(2), SHUTDOWN_TIME) = False then Error_Message;
		if SHUTDOWN_TIME < 0 then Error_Message;

		writeln('SHUTDOWN TIME IN SEC. ', SHUTDOWN_TIME);
{$ifdef VERSION_0}
		writeln(do_SysCall(SYSCALL_NR, SYSCALL_NEW_TIME, SHUTDOWN_TIME));
{$else VERSION_0}
		writeln(do_SysCall(SYSCALL_NR, SYSCALL_NEW_TIME, lo(SHUTDOWN_TIME), hi(SHUTDOWN_TIME)));
{$endif VERSION_0}
		halt(0);
	end;

	if ParamStr(1) = '-nM' then begin
		if ParamCount <> 2 then Error_Message;

		if TryStrToInt64(ParamStr(2), SHUTDOWN_TIME) = False then Error_Message;

		SHUTDOWN_TIME *= 60;
		if SHUTDOWN_TIME < 0 then Error_Message;

		writeln('SHUTDOWN TIME IN SEC. ', SHUTDOWN_TIME);
{$ifdef VERSION_0}
		writeln(do_SysCall(SYSCALL_NR, SYSCALL_NEW_TIME, SHUTDOWN_TIME));
{$else VERSION_0}
		writeln(do_SysCall(SYSCALL_NR, SYSCALL_NEW_TIME, lo(SHUTDOWN_TIME), hi(SHUTDOWN_TIME)));
{$endif VERSION_0}

		halt(0);
	end;

	if ParamStr(1) = '-nH' then begin
		if ParamCount <> 3 then Error_Message;

		if TryStrToInt64(ParamStr(2), HOUR) = False then Error_Message;
		if TryStrToInt64(ParamStr(3), MIN) = False then Error_Message;

		SHUTDOWN_TIME := HOUR * 3600;
		SHUTDOWN_TIME += MIN * 60;
		if SHUTDOWN_TIME < 0 then Error_Message;

		writeln('SHUTDOWN TIME IN SEC. ', SHUTDOWN_TIME);
{$ifdef VERSION_0}
		writeln(do_SysCall(SYSCALL_NR, SYSCALL_NEW_TIME, SHUTDOWN_TIME));
{$else VERSION_0}
		writeln(do_SysCall(SYSCALL_NR, SYSCALL_NEW_TIME, lo(SHUTDOWN_TIME), hi(SHUTDOWN_TIME)));
{$endif VERSION_0}
		halt(0);
	end;

	if ParamStr(1) = '-nD' then begin
		if ParamCount <> 4 then Error_Message;

		if TryStrToInt64(ParamStr(2), DAY) = False then Error_Message;
		if TryStrToInt64(ParamStr(3), HOUR) = False then Error_Message;
		if TryStrToInt64(ParamStr(4), MIN) = False then Error_Message;

		SHUTDOWN_TIME := DAY * 24 * 3600;
		SHUTDOWN_TIME += HOUR * 3600;
		SHUTDOWN_TIME += MIN * 60;
		if SHUTDOWN_TIME < 0 then Error_Message;

		writeln('SHUTDOWN TIME IN SEC. ', SHUTDOWN_TIME);
{$ifdef VERSION_0}
		writeln(do_SysCall(SYSCALL_NR, SYSCALL_NEW_TIME, SHUTDOWN_TIME));
{$else VERSION_0}
		writeln(do_SysCall(SYSCALL_NR, SYSCALL_NEW_TIME, lo(SHUTDOWN_TIME), hi(SHUTDOWN_TIME)));
{$endif VERSION_0}
		halt(0);
	end;

	if ParamStr(1) = '-nY' then begin
		if ParamCount <> 5 then Error_Message;

		if TryStrToInt64(ParamStr(2), YEAR) = False then Error_Message;
		if TryStrToInt64(ParamStr(3), DAY) = False then Error_Message;
		if TryStrToInt64(ParamStr(4), HOUR) = False then Error_Message;
		if TryStrToInt64(ParamStr(5), MIN) = False then Error_Message;

		SHUTDOWN_TIME := YEAR * 365 * 24 * 3600;
		SHUTDOWN_TIME += DAY * 24 * 3600;
		SHUTDOWN_TIME += HOUR * 3600;
		SHUTDOWN_TIME += MIN * 60;
		if SHUTDOWN_TIME < 0 then Error_Message;

		writeln('SHUTDOWN TIME IN SEC. ', SHUTDOWN_TIME);
{$ifdef VERSION_0}
		writeln(do_SysCall(SYSCALL_NR, SYSCALL_NEW_TIME, SHUTDOWN_TIME));
{$else VERSION_0}
		writeln(do_SysCall(SYSCALL_NR, SYSCALL_NEW_TIME, lo(SHUTDOWN_TIME), hi(SHUTDOWN_TIME)));
{$endif VERSION_0}
		halt(0);
	end;

	Error_Message;

end.
