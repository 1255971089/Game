#pragma once

#include"global.h"
#include"interface.h"
#include"os.h"

_XE_BEGIN

//platform-indepent implementation of the game timer
class CTimer :public ITimer
{
public:
	CTimer(bool use_performaceTimer = true):m_pretime(0)
	{
		os::Timer::InitTimer(use_performaceTimer);
	}

	virtual u32 VGetRealTime() const override
	{
		return os::Timer::GetRealTime();
	}

	virtual void VSetTime(u32 time) override
	{
		os::Timer::SetTime(time);
	}

	virtual u32 VGetTime() const override
	{
		return os::Timer::GetTime();
	}

	virtual void VStart() override
	{
		os::Timer::StartTimer();
	}

	virtual void VStop() override
	{
		os::Timer::StopTimer();
	}

	virtual void VSetSpeed(f32 speed = 1.0f) override
	{
		os::Timer::SetSpeed(speed);
	}

	virtual f32 VGetSpeed() const override
	{
		return os::Timer::GetSpeed();
	}

	virtual bool VIsStopped() const override
	{
		return os::Timer::IsStopped();
	}

	virtual void VTick() override
	{
		os::Timer::Tick();
	}

	virtual u32 VGetDeltaTime() override
	{
		u32 tmp = m_pretime;
		m_pretime = os::Timer::GetTime();
		return m_pretime - tmp;
	}
private:
	u32 m_pretime;
};

_XE_END
