#pragma once

#include <string>

namespace Abysswire::Env {
    bool envEnabled(const std::string& env);
    bool isTrace();
    void resetTraceCache();
}
