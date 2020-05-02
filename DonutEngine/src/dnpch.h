#pragma once

#include <iostream>
#include <memory>
#include <functional>
#include <utility>
#include <algorithm>

#include <string>
#include <sstream>
#include <vector>
#include <unordered_map>
#include <unordered_set>

#include "Donut/Core/Log.h"
#include "Donut/Debug/Instrumentor.h"

#ifdef DN_PLATFORM_WINDOWS
	#include <Windows.h>
#endif