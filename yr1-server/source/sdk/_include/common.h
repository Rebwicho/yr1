#ifndef COMMON_H
#define COMMON_H

//#define WIN32_LEAN_AND_MEAN 
//#undef WIN32_LEAN_AND_MEAN

#include <stdio.h>

#include <thread>
#include <chrono>
#include <mutex>

#include <iomanip>
#include <ctime>
//#include <format> // not implemented till c++20

#include <string>

#include <deque>

#include <functional>

#include <iostream>

#include "singleton.h"

#define ASIO_NO_DEPRECATED
//#define ASIO_SEPARATE_COMPILATION
#define _WIN32_WINNT 0x0A00
#include <asio.hpp>

#ifdef _DEBUG
#pragma comment(lib, "asio-dbg-x86.lib")
#else
#pragma comment(lib, "asio-x86.lib")
#endif

#include <Windows.h>

typedef unsigned __int8 u8;
typedef unsigned __int16 u16;
typedef unsigned __int32 u32;

typedef unsigned long ul32;

typedef void* abyss; // yeah why not

#endif // COMMON_H
