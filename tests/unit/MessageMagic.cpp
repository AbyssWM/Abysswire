#include <gtest/gtest.h>

#include "core/message/MessageMagic.hpp"

#include <array>

TEST(MessageMagic, KnownValuesMapToReadableStrings) {
    using namespace Abysswire;

    const std::array<eMessageMagic, 10> known = {
        AW_MESSAGE_MAGIC_END,         AW_MESSAGE_MAGIC_TYPE_UINT,      AW_MESSAGE_MAGIC_TYPE_INT,     AW_MESSAGE_MAGIC_TYPE_F32,
        AW_MESSAGE_MAGIC_TYPE_SEQ,    AW_MESSAGE_MAGIC_TYPE_OBJECT_ID, AW_MESSAGE_MAGIC_TYPE_VARCHAR, AW_MESSAGE_MAGIC_TYPE_ARRAY,
        AW_MESSAGE_MAGIC_TYPE_OBJECT, AW_MESSAGE_MAGIC_TYPE_FD,
    };

    for (const auto magic : known) {
        const char* str = magicToString(magic);
        ASSERT_NE(str, nullptr);
        EXPECT_STRNE(str, "ERROR");
    }
}

TEST(MessageMagic, UnknownValueReturnsError) {
    using namespace Abysswire;

    EXPECT_STREQ(magicToString(static_cast<eMessageMagic>(0xFF)), "ERROR");
}
