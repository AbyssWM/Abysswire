#include <gtest/gtest.h>

#include "core/message/messages/BindProtocol.hpp"
#include "core/message/messages/FatalProtocolError.hpp"
#include "core/message/messages/HandshakeBegin.hpp"
#include "core/message/messages/HandshakeProtocols.hpp"
#include "core/message/messages/Hello.hpp"
#include "core/message/messages/NewObject.hpp"

using namespace Abysswire;

template <typename Message>
static void checkTruncated(const Message& message) {
    for (size_t size = 0; size < message.m_data.size(); ++size) {
        const std::vector<uint8_t> truncated(message.m_data.begin(), message.m_data.begin() + size);
        EXPECT_EQ(Message(truncated, 0).m_len, 0) << "prefix length " << size;
        auto prefixed = truncated;
        prefixed.insert(prefixed.begin(), 9, 0xFF);
        EXPECT_EQ(Message(prefixed, 9).m_len, 0) << "prefix length at offset " << size;
    }
}

TEST(MessageBounds, RejectsEveryTruncatedControlMessagePrefix) {
    checkTruncated(CHelloMessage{});
    checkTruncated(CBindProtocolMessage("example", 1, 1));
    checkTruncated(CFatalErrorMessage(1u, 1u, "example"));
    checkTruncated(CHandshakeBeginMessage(std::vector<uint32_t>{1, 2}));
    checkTruncated(CHandshakeProtocolsMessage(std::vector<std::string>{"example@1"}));
    checkTruncated(CNewObjectMessage(1, 2));
}

TEST(MessageBounds, RejectsDeclaredStringPastPayload) {
    auto bind = CBindProtocolMessage("example", 1, 1).m_data;
    bind[7] = 127;
    EXPECT_EQ(CBindProtocolMessage(bind, 0).m_len, 0);

    auto fatal = CFatalErrorMessage(1u, 1u, "example").m_data;
    fatal[12] = 127;
    EXPECT_EQ(CFatalErrorMessage(fatal, 0).m_len, 0);

    auto protocols = CHandshakeProtocolsMessage(std::vector<std::string>{"example@1"}).m_data;
    protocols[4] = 127;
    EXPECT_EQ(CHandshakeProtocolsMessage(protocols, 0).m_len, 0);
}
