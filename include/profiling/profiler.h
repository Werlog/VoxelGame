#pragma once

#ifdef TRACY_ENABLE
#include <tracy/Tracy.hpp>

#define PROFILER_ZONE ZoneScoped
#define PROFILER_ZONE_N(name) ZoneScopedN(name)
#define PROFILER_FRAME FrameMark
#define PROFILER_THREAD_NAME(name) tracy::SetThreadName(name)

#else

#define PROFILER_ZONE 
#define PROFILER_ZONE_N(name)
#define PROFILER_FRAME
#define PROFILER_THREAD_NAME(name)

#endif
