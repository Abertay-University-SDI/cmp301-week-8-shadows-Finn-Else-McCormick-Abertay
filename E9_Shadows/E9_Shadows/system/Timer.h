/**
* \class Timer
*
* \brief Calculates frame/delta time and FPS
*
* \author Paul Robertson
*/

// timer class
// Calculates frame/delta time and FPS.

#pragma once

#include <windows.h>

class Timer
{
public:
	Timer();

	void Frame();	///< Update delta time, called every frame

	float DeltaTime() const;
	float ElapsedTime() const;
	float FPS() const;

private:
	INT64 frequency; float ticksPerS;
	INT64 startTime, prevTime;
	float frameTime;
	float totalElapsedTime;
	float fps; int framesSinceReset; float elapsedTimeSinceReset;
};