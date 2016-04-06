#pragma once

#include "Base.h"
#include "DebugNew.h"

#include <cassert>
#include <ctime>
#include <memory>

#include <string>
#include <list>
#include <algorithm>

#ifdef _WIN32
#	include <windows.h>
#	include <crtdbg.h>
#	include <io.h>
#endif