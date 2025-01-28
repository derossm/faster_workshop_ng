//====================================================================================================
//
//====================================================================================================

#pragma once

#pragma warning(push, 1)

#pragma region StandardHeadersCpp
// C++ Headers
#include <algorithm>
#include <any>
#include <array>
#include <atomic>
#include <barrier>
#include <bit>
#include <bitset>
#include <charconv>
#include <chrono>
#include <compare>
#include <complex>
#include <concepts>
#include <condition_variable>
#include <coroutine>
#include <deque>
#include <exception>
#include <execution>
#include <expected>
#include <filesystem>
#include <format>
#include <forward_list>
#include <fstream>
#include <functional>
#include <future>
#include <generator>
#include <initializer_list>
#include <iomanip>
#include <ios>
#include <iosfwd>
#include <iostream>
#include <istream>
#include <iterator>
#include <latch>
#include <limits>
#include <list>
#include <locale>
#include <map>
#include <mdspan>
#include <memory>
#include <memory_resource>
#include <mutex>
#include <new>
#include <numbers>
#include <numeric>
#include <optional>
#include <ostream>
#include <print>
#include <queue>
#include <random>
#include <ranges>
#include <ratio>
#include <regex>
#include <scoped_allocator>
#include <semaphore>
#include <set>
#include <shared_mutex>
#include <source_location>
#include <span>
#include <spanstream>
#include <sstream>
#include <stack>
#include <stacktrace>
#include <stdexcept>
#include <stdfloat>
#include <stop_token>
#include <streambuf>
#include <string>
#include <string_view>
#include <syncstream>
#include <system_error>
#include <thread>
#include <tuple>
#include <type_traits>
#include <typeindex>
#include <typeinfo>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <valarray>
#include <variant>
#include <vector>
#include <version>
#pragma endregion

//#include <flat_map> // C++23
//#include <flat_set> // C++23

//#include <debugging> // C++26
//#include <hazard_pointer> // C++26
//#include <inplace_vector> // C++26
//#include <linalg> // C++26
//#include <rcu> // C++26
//#include <simd> // C++26
//#include <text_encoding> // C++26

#pragma region StandardHeadersC
// C Headers
#include <cassert>
#include <cctype>
#include <cerrno>
#include <cfenv>
#include <cfloat>
#include <cinttypes>
#include <climits>
#include <clocale>
#include <cmath>
#include <csetjmp>
#include <csignal>
#include <cstdarg>
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <cuchar>
#include <cwchar>
#include <cwctype>
#pragma endregion

#include <F4SE/F4SE.h>
#include <RE/Bethesda/Actor.h>
#include <RE/Bethesda/BSExtraData.h>
#include <RE/Bethesda/BGSPrimitive.h>
#include <RE/Bethesda/TESForms.h>
#include <RE/Bethesda/FormComponents.h>
#include <RE/Bethesda/TESDataHandler.h>
#include <RE/Bethesda/Workshop.h>
#include <REX/W32.h>
#include <REX/W32/OLE32.h>
#include <REX/W32/SHELL32.h>

#pragma region WindowsHeaders
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <windows.h>
#include <errhandlingapi.h>
#include <fileapi.h>
#include <handleapi.h>
#include <memoryapi.h>
#include <minwinbase.h>
#include <vadefs.h>
#include <winnt.h>
#pragma endregion

#include <spdlog/common.h>
#include <spdlog/spdlog.h>
#include <spdlog/logger.h>

#ifdef NDEBUG
#include <spdlog/sinks/basic_file_sink.h>
#else
#include <spdlog/sinks/msvc_sink.h>
#endif

#pragma warning(pop)

#ifdef min
#undef min
#endif
#ifdef max
#undef max
#endif

#ifdef near
#undef near
#endif
#ifdef far
#undef far
#endif

#ifndef DLLEXPORT
#define DLLEXPORT __declspec(dllexport)
#endif

namespace logger = F4SE::log;

using namespace std::literals;
