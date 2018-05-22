#ifndef HIGHRESOLUTIONTIMER_H
#define HIGHRESOLUTIONTIMER_H

#include <windows.h>

class CHighResolutionTimer 
{
public:
	CHighResolutionTimer();
	~CHighResolutionTimer();

	void Start();
	double Elapsed();
	bool StartStopWatch(float time); //in seconds
	void ResetTimer() { StopWatchOn = false; }

	bool StopWatchTrigger = false;
	bool StopWatchOn = false;

	bool UpdateStopWatch();

	double mTimeElapsed = 0.0f;

	double mTimeToMeasure = 0.0f;

private:
	LARGE_INTEGER m_t1, m_t2;
	bool m_started;
};

#endif