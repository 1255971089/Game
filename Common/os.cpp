#pragma once
/*
the implemention of os.h
*/
#include"os.h"

#include<algorithm>

_XE_BEGIN
_OS_BEGIN

namespace
{
	LARGE_INTEGER highperformance_freq;
	bool highperformance_support = false;
	bool multicore = false;
}

f32 Timer::s_virtualtimer_speed = 1.0f;
s32 Timer::s_virtualtimer_stopcounter = 0;
u32 Timer::s_start_realtime = 0;
u32 Timer::s_last_virtualtime = 0;
u32 Timer::s_statictime = 0;

//initialize the real system timer
void Timer::InitTimer(bool use_performacetimer)
{
	SYSTEM_INFO sysinfo;
	GetSystemInfo(&sysinfo);
	multicore = (sysinfo.dwNumberOfProcessors > 1);
	if (use_performacetimer)
		highperformance_support = QueryPerformanceFrequency(&highperformance_freq);
	else
		highperformance_support = false;
}

//set the current virtual game time
void Timer::SetTime(u32 time)
{
	s_statictime = GetRealTime();
	s_last_virtualtime = time;
	s_start_realtime = s_statictime;
}

//return the current virtual game time
u32 Timer::GetTime()
{
	if (IsStopped())
		return s_last_virtualtime;
	return s_last_virtualtime + (s_statictime - s_start_realtime)*s_virtualtimer_speed;
}

//start the game timer
void Timer::StartTimer()
{
	++s_virtualtimer_stopcounter;
	if (!IsStopped())
		SetTime(s_last_virtualtime);
}

//stop the game timer
void Timer::StopTimer()
{
	if (!IsStopped())
		s_last_virtualtime = GetTime();
	--s_virtualtimer_stopcounter;
}

//set the speed of the virtual timer
void Timer::SetSpeed(f32 speed)
{
	SetTime(GetTime());
	s_virtualtimer_speed = std::max(0.0f,speed);
}

//get the speed of the virtual timer
f32 Timer::GetSpeed()
{
	return s_virtualtimer_speed;
}

//return if the timer is stopped
bool Timer::IsStopped()
{
	return s_virtualtimer_stopcounter<0;
}

//make the virtual timer update the time value based on the real time
void Timer::Tick()
{
	s_statictime = GetRealTime();
}

//return the current real time in metric of millisecond
u32 Timer::GetRealTime()
{
	if (highperformance_support)
	{
		DWORD_PTR affinity_mask = 0;
		if (multicore)
			affinity_mask = SetThreadAffinityMask(GetCurrentThread(), 1);
		LARGE_INTEGER tmp_counter;
		bool queried_ok = QueryPerformanceCounter(&tmp_counter);
		if (multicore)
			SetThreadAffinityMask(GetCurrentThread(), affinity_mask);
		if (queried_ok)
			return u32(tmp_counter.QuadPart * 1000 / highperformance_freq.QuadPart);
	}
	return GetTickCount();
}

//initialize the virtual timer
void Timer::InitVirtualTimer()
{
	s_statictime = GetRealTime();
	s_start_realtime = s_statictime;
}
_OS_END
_XE_END