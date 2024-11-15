#pragma once
#include <Windows.h>
#include <iostream>
#include <thread>
#include <string>
#include <vector>
#include <Psapi.h>

using namespace std::chrono_literals;

// D3D9
#include <d3d9.h>
#include <d3dx9.h>

#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")

// MinHook
#include "Libs/MinHook/minhook.h"
