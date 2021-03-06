#ifndef COMMON_H
#define COMMON_H

#include "../../sdk/network/asio.h"

#include <Windows.h>

#include <stdio.h>

#include <thread>
#include <chrono>

#include <string>
#include <sstream>
#include <iostream>

#include <deque>
#include <array>

#include <fstream>
#include <filesystem>

#include <dwmapi.h>
#include <windowsx.h>

#include <imgui.h>
#include <backends/imgui_impl_dx9.h>
#include <backends/imgui_impl_win32.h>

#include <d3d9.h>
#pragma comment(lib, "d3d9.lib")

#include <d3dx9.h>
#pragma comment (lib, "d3dx9.lib")

#include "singleton.h"

typedef signed __int8 s8;

typedef unsigned __int8 u8;
typedef unsigned __int16 u16;
typedef unsigned __int32 u32;

typedef unsigned long ul32;
typedef long l32;

#endif // COMMON_H
