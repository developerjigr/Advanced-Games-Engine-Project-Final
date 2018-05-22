#include "highResolutionTimer.h"

CHighResolutionTimer::CHighResolutionTimer()
{
	m_started = false;
}

CHighResolutionTimer::~CHighResolutionTimer()
{
}

bool CHighResolutionTimer::StartStopWatch(float time)
{
	//If the stop watch is alredy being used
	if (StopWatchOn == true)
	{
		if (UpdateStopWatch())
			return true;
	}

	StopWatchOn = true;
	if (StopWatchTrigger == true)
	{
		StopWatchTrigger = false;
		mTimeElapsed = 0.0f;
		mTimeToMeasure = time * 100;
		Start();
	}

	return false;
}

bool CHighResolutionTimer::UpdateStopWatch()
{
	mTimeElapsed += Elapsed();

	if (mTimeElapsed >= mTimeToMeasure)
	{
		StopWatchTrigger = true;
		StopWatchOn = false;
	}

	return StopWatchTrigger;

}

void CHighResolutionTimer::Start()
{
	m_started = true;
	QueryPerformanceCounter(&m_t1);
}

double CHighResolutionTimer::Elapsed()
{
	if (!m_started) {
		return 0.0;
	}

	LARGE_INTEGER frequency;
	QueryPerformanceFrequency(&frequency);

	QueryPerformanceCounter(&m_t2);

	// Return the elapsed time in seconds
	return (double) (m_t2.QuadPart - m_t1.QuadPart) / frequency.QuadPart;
}