#pragma once

#include <ffi.h>
#include <abysswire/core/types/MessageMagic.hpp>

namespace Abysswire::FFI {
    ffi_type* ffiTypeFrom(eMessageMagic magic);
}
