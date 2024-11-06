// Timer object.
// Calculate delta/frame time and FPS.
#include "timer.h"

// Initialise timer. Check for high performance timers.
Timer::Timer()
{
	QueryPerformanceFrequency((LARGE_INTEGER*)&frequency);
	if (frequency == 0)
	{
		MessageBox(NULL, L"No support for high performance timer", L"ERROR", MB_OK);
	}

	// Find out how many times the frequency counter ticks every second.
	ticksPerS = (float)(frequency);

	QueryPerformanceCounter((LARGE_INTEGER*)&startTime);
	prevTime = startTime;

	frameTime = 0.f;
	totalElapsedTime = 0.f;
	elapsedTimeSinceReset = 0.f;
	framesSinceReset = 0;
	fps = 0.f;
}

// Once per frame calculate delta timer and update FPS calculation.
void Timer::Frame()
{
	// Query the current time.
	INT64 currentTime;
	QueryPerformanceCounter((LARGE_INTEGER*)&currentTime);
	frameTime = static_cast<float>(currentTime - prevTime) / ticksPerS;
	totalElapsedTime = static_cast<float>(currentTime - startTime) / ticksPerS;

	// Calc FPS
	++framesSinceReset;
	elapsedTimeSinceReset += frameTime;
	if (elapsedTimeSinceReset > 1.0f)
	{
		fps = static_cast<float>(framesSinceReset) / elapsedTimeSinceReset;
		framesSinceReset = 0;
		elapsedTimeSinceReset = 0.0f;
	}
	
	// Restart the timer.
	prevTime = currentTime;

	return;
}


float Timer::DeltaTime() const { return frameTime; }

float Timer::ElapsedTime() const { return totalElapsedTime; }

float Timer::FPS() const { return fps; }