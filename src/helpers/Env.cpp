#include "Env.hpp"

#include <cstdlib>
#include <string_view>

using namespace Abysswire;
using namespace Abysswire::Env;

namespace {
    bool g_traceCached = false;
    bool g_trace       = false;
}

bool Abysswire::Env::envEnabled(const std::string& env) {
    auto ret = getenv(env.c_str());
    if (!ret)
        return false;

    const std::string_view sv = ret;

    return !sv.empty() && sv != "0";
}

bool Abysswire::Env::isTrace() {
    if (!g_traceCached) {
        g_trace       = envEnabled("AW_TRACE");
        g_traceCached = true;
    }

    return g_trace;
}

void Abysswire::Env::resetTraceCache() {
    g_traceCached = false;
    g_trace       = false;
}
