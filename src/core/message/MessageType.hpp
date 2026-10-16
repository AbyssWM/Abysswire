#pragma once

#include <cstdint>

namespace Abysswire {
    enum eMessageType : uint8_t {
        AW_MESSAGE_TYPE_INVALID = 0,

        /*
            Sent by the client to initiate the handshake.
            Params: str -> has to be "VAX"
        */
        AW_MESSAGE_TYPE_SUP = 1,

        /*
            Sent by the server after a HELLO.
            Params: arr(uint) -> versions supported
        */
        AW_MESSAGE_TYPE_HANDSHAKE_BEGIN = 2,

        /*
            Sent by the client to confirm a choice of a protocol version
            Params: uint -> version chosen
        */
        AW_MESSAGE_TYPE_HANDSHAKE_ACK = 3,

        /*
            Sent by the server to advertise supported protocols
            Params: arr(str) -> protocols
        */
        AW_MESSAGE_TYPE_HANDSHAKE_PROTOCOLS = 4,

        /*
            Sent by the client to bind to a specific protocol spec
            Params: uint -> seq, str -> protocol spec
        */
        AW_MESSAGE_TYPE_BIND_PROTOCOL = 10,

        /*
            Sent by the server to acknowledge the bind and return a handle
            Params: uint -> object handle ID, uint -> seq
        */
        AW_MESSAGE_TYPE_NEW_OBJECT = 11,

        /*
            Sent by the server to indicate a fatal protocol error
            Params: uint -> object handle ID, uint -> error idx, varchar -> error message
        */
        AW_MESSAGE_TYPE_FATAL_PROTOCOL_ERROR = 12,

        /*
            Sent from the client to initiate a roundtrip.
            Params: uint -> sequence
        */
        AW_MESSAGE_TYPE_ROUNDTRIP_REQUEST = 13,

        /*
            Sent from the server to finalize the roundtrip.
            Params: uint -> sequence
        */
        AW_MESSAGE_TYPE_ROUNDTRIP_DONE = 14,

        /*
            Generic protocol message. Can be either direction.
            Params: uint -> object handle ID, uint -> method ID, data...
        */
        AW_MESSAGE_TYPE_GENERIC_PROTOCOL_MESSAGE = 100,
    };

    inline const char* messageTypeToStr(eMessageType t) {
        switch (t) {
            case AW_MESSAGE_TYPE_INVALID: return "INVALID";
            case AW_MESSAGE_TYPE_SUP: return "SUP";
            case AW_MESSAGE_TYPE_HANDSHAKE_BEGIN: return "HANDSHAKE_BEGIN";
            case AW_MESSAGE_TYPE_HANDSHAKE_ACK: return "HANDSHAKE_ACK";
            case AW_MESSAGE_TYPE_HANDSHAKE_PROTOCOLS: return "HANDSHAKE_PROTOCOLS";
            case AW_MESSAGE_TYPE_BIND_PROTOCOL: return "BIND_PROTOCOL";
            case AW_MESSAGE_TYPE_NEW_OBJECT: return "NEW_OBJECT";
            case AW_MESSAGE_TYPE_FATAL_PROTOCOL_ERROR: return "AW_MESSAGE_TYPE_FATAL_PROTOCOL_ERROR";
            case AW_MESSAGE_TYPE_GENERIC_PROTOCOL_MESSAGE: return "GENERIC_PROTOCOL_MESSAGE";
            case AW_MESSAGE_TYPE_ROUNDTRIP_REQUEST: return "AW_MESSAGE_TYPE_ROUNDTRIP_REQUEST";
            case AW_MESSAGE_TYPE_ROUNDTRIP_DONE: return "AW_MESSAGE_TYPE_ROUNDTRIP_DONE";
        }
        return "ERROR";
    }
};