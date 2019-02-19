#pragma once

/*
the annonation will be added sooner or later
*/

#include"global.h"

_XE_BEGIN

//ITimer interface for manipulating the virtual game timer
__INTERFACE ITimer
{
	virtual void VSetTime(u32 time) = 0;

	virtual u32 VGetTime() const = 0;

	virtual u32 VGetRealTime() const = 0;

	virtual void VStop() = 0;

	virtual void VStart() = 0;

	virtual void VSetSpeed(f32 speed = 1.0f) = 0;

	virtual f32 VGetSpeed() const = 0;

	virtual bool VIsStopped() const = 0;

	virtual void VTick() = 0;

	virtual u32 VGetDeltaTime() = 0;
};

//IGame interface for manipulating the game
__INTERFACE IGame
{
	virtual bool VInit() = 0;

	virtual s32 VRun() = 0;
	
	virtual void VOnResize() = 0;

	virtual void VUpdateScene(float dt) = 0;

	virtual void VDrawScene() = 0;

	virtual LRESULT VMsgProc(HWND hwnd, u32 msg, WPARAM wparam, LPARAM lparam) = 0;
};

_XE_END
