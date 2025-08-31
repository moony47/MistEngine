#pragma once

#include <algorithm>
#include <functional>
#include <memory>
#include <print>
#include <utility>

#include <array>
#include <expected>
#include <sstream>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "Mist/Core/Logger.h"

#include "Mist/Debug/Instrumentor.h"

#ifdef MIST_PLATFORM_WINDOWS
    #include <Windows.h>
#endif