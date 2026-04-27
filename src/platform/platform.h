#if defined(UNIX) || defined(__linux__) || defined(__unix__)
#define PLATFORM_LINUX

#elif defined(_WIN32) || defined(WIN32)
#define PLATFORM_WINDOWS

#elif defined(__APPLE__)
#define PLATFORM_APPLE

#endif