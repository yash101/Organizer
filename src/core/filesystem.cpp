#include "filesystem.h"
#ifndef _WIN32
#include <sys/stat.h>
#include <unistd.h>
#else
#include <Windows.h>
#endif

bool core::create_directory(const char* location)
{
#ifdef _WIN32
  return CreateDirectory(location, NULL) != 0;
#else
  return mkdir(location, S_IRWXU | S_IRWXG | S_IROTH) == 0;
#endif
}
