#include "MessageMagic.hpp"

const char* Abysswire::magicToString(eMessageMagic magic) {
    switch (magic) {
        case AW_MESSAGE_MAGIC_END: return "END";
        case AW_MESSAGE_MAGIC_TYPE_UINT: return "UINT";
        case AW_MESSAGE_MAGIC_TYPE_INT: return "INT";
        case AW_MESSAGE_MAGIC_TYPE_F32: return "F32";
        case AW_MESSAGE_MAGIC_TYPE_SEQ: return "SEQUENCE";
        case AW_MESSAGE_MAGIC_TYPE_OBJECT_ID: return "OBJECT_ID";
        case AW_MESSAGE_MAGIC_TYPE_VARCHAR: return "VARCHAR";
        case AW_MESSAGE_MAGIC_TYPE_ARRAY: return "ARRAY";
        case AW_MESSAGE_MAGIC_TYPE_OBJECT: return "OBJECT";
        case AW_MESSAGE_MAGIC_TYPE_FD: return "FD";
    }

    return "ERROR";
}
