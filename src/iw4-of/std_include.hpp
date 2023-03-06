#pragma once

#pragma warning(push)
#pragma warning(disable : 4244)
#pragma warning(disable : 4458)
#pragma warning(disable : 6297)
#pragma warning(disable : 6385)
#pragma warning(disable : 6386)
#pragma warning(disable : 26451)
#pragma warning(disable : 26444)
#pragma warning(disable : 26451)
#pragma warning(disable : 26495)
#pragma warning(disable : 28020)

#define _CRT_SECURE_NO_WARNINGS

#define ZeroMemory(ptr, size) std::memset(ptr, 0, size)
#define ARRAYSIZE(a) ((sizeof(a) / sizeof(*(a))) / static_cast<size_t>(!(sizeof(a) % sizeof(*(a)))))

// min and max is required by gdi, therefore NOMINMAX won't work
#ifdef max
    #undef max
#endif

#ifdef min
    #undef min
#endif

#include <atomic>
#include <chrono>
#include <filesystem>
#include <format>
#include <fstream>
#include <functional>
#include <map>
#include <mutex>
#include <queue>
#include <regex>
#include <thread>
#include <unordered_map>
#include <utility>
#include <vector>

#include <zlib.h>

#include <tomcrypt.h>

#pragma warning(pop)

#pragma warning(disable : 4100)
#pragma warning(disable : 26812)

using namespace std::literals;
