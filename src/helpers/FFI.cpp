#include "FFI.hpp"

using namespace Abysswire;

ffi_type* Abysswire::FFI::ffiTypeFrom(eMessageMagic magic) {
    switch (magic) {
        case AW_MESSAGE_MAGIC_TYPE_UINT:
        case AW_MESSAGE_MAGIC_TYPE_OBJECT:
        case AW_MESSAGE_MAGIC_TYPE_SEQ: return &ffi_type_uint32;
        case AW_MESSAGE_MAGIC_TYPE_FD:
        case AW_MESSAGE_MAGIC_TYPE_INT: return &ffi_type_sint32;
        case AW_MESSAGE_MAGIC_TYPE_F32: return &ffi_type_float;
        case AW_MESSAGE_MAGIC_TYPE_VARCHAR:
        case AW_MESSAGE_MAGIC_TYPE_ARRAY: return &ffi_type_pointer;
        default: return nullptr;
    }

    return nullptr;
}