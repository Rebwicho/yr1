#ifndef COMMON_H
#define COMMON_H

#define WIN32_LEAN_AND_MEAN 
#include <Windows.h>
#undef WIN32_LEAN_AND_MEAN

#include <stdio.h>

#include <thread>
#include <chrono>

#include <string>
#include <sstream>
#include <iostream>

#include <deque>
#include <array>

#include <filesystem>

#include "singleton.h"

typedef unsigned __int8 u8;
typedef unsigned __int16 u16;
typedef unsigned __int32 u32;

typedef unsigned long ul32;

#endif // COMMON_H
