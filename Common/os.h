#pragma once

/*this file os.h is for communicate with the operating system:
time
printer
logger
 */

#include"global.h"

_XE_BEGIN
_OS_BEGIN

class Timer
{
public:
	//initialize the real system timer
	static void InitTimer(bool use_performaceTimer = true);

	//set the current virtual game time
	static void SetTime(u32 time);

	//return the current virtual game time
	static u32 GetTime();

	//start the game timer
	static void StartTimer();

	//stop the game timer
	static void StopTimer();

	//set the speed of the virtual timer
	static void SetSpeed(f32 speed);

	//get the speed of the virtual timer
	static f32 GetSpeed();

	//return if the timer is stopped
	static bool IsStopped();

	//make the virtual timer update the time value based on the real time
	static void Tick();

	//return the current real time in metric of millisecond
	static u32 GetRealTime();
private:
	//initialize the virtual timer
	static void InitVirtualTimer();

private:
	static f32 s_virtualtimer_speed;
	static s32 s_virtualtimer_stopcounter;
	static u32 s_start_realtime;
	static u32 s_last_virtualtime;
	static u32 s_statictime;
};

_OS_END
_XE_END
