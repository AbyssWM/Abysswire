#include <gtest/gtest.h>

#include "core/message/MessageType.hpp"

#include <array>

TEST(MessageType, KnownValuesMapToStrings) {
    using namespace Abysswire;

    const std::array<eMessageType, 10> known = {
        AW_MESSAGE_TYPE_INVALID,       AW_MESSAGE_TYPE_SUP,        AW_MESSAGE_TYPE_HANDSHAKE_BEGIN,      AW_MESSAGE_TYPE_HANDSHAKE_ACK,     AW_MESSAGE_TYPE_HANDSHAKE_PROTOCOLS,
        AW_MESSAGE_TYPE_BIND_PROTOCOL, AW_MESSAGE_TYPE_NEW_OBJECT, AW_MESSAGE_TYPE_FATAL_PROTOCOL_ERROR, AW_MESSAGE_TYPE_ROUNDTRIP_REQUEST, AW_MESSAGE_TYPE_ROUNDTRIP_DONE,
    };

    for (const auto type : known) {
        const char* str = messageTypeToStr(type);
        ASSERT_NE(str, nullptr);
        EXPECT_STRNE(str, "ERROR");
    }

    EXPECT_STREQ(messageTypeToStr(AW_MESSAGE_TYPE_GENERIC_PROTOCOL_MESSAGE), "GENERIC_PROTOCOL_MESSAGE");
}

TEST(MessageType, UnknownValueReturnsError) {
    using namespace Abysswire;

    EXPECT_STREQ(messageTypeToStr(static_cast<eMessageType>(0xFF)), "ERROR");
}
