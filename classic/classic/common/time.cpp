#include "time.h"
#include <chrono>

using namespace std::chrono;

int64 GetNowMilSec()
{
	auto tp = system_clock::now();
	auto du = tp.time_since_epoch();
	return duration_cast<milliseconds>(du).count();
}
