#pragma once

#include <memory>

#include "exebox/api/IExeBoxApi.h"

namespace exebox {

inline std::unique_ptr<IExeBoxApi> g_Api = nullptr;

}
