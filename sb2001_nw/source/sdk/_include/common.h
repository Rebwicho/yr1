#ifndef COMMON_H
#define COMMON_H

#define WIN32_LEAN_AND_MEAN 
#include <Windows.h>
#undef WIN32_LEAN_AND_MEAN

#include <stdio.h>

#include <thread>
#include <chrono>

#include <iomanip>
#include <ctime>
//#include <format> // not implemented till c++20

#include <string>

#include <deque>

#include <functional>

#include <iostream>

#include "singleton.h"

typedef unsigned __int8 u8;
typedef unsigned __int16 u16;
typedef unsigned __int32 u32;

typedef unsigned long ul32;

typedef void* abyss; // yeah why not

#endif // COMMON_H
