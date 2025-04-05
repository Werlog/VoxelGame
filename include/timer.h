#include <stdint.h>

class Timer
{
public:

	Timer();

	double deltaTime;

	void tick();
private:
	int64_t lastTicks;
};