#pragma once
#include <stdint.h>
#include <string>

class AGDeviceCallback
{
public:
	static void OnAudioVolumeIndication(int totalVolume);
};