#include "filesystem.h"
#include <stdio.h>
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

bool core::create_file(const char* location)
{
  FILE* fp = fopen(location, "a+");
  if(fp == NULL) return false;

  fclose(fp);

  return true;
}
