#include "LightningBolt.h"


void CLightningBolt::Update()
{
	if (mLightingTimer.StartStopWatch(0.4f))
	{
		mLighting.FlashLight();
	}
}

void CLightningBolt::Render()
{

}