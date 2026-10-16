#include "HandshakeProtocols.hpp"
#include "../MessageType.hpp"
#include "../MessageParser.hpp"
#include "../../../helpers/Env.hpp"

#include <stdexcept>
#include <string_view>
#include <abysswire/core/types/MessageMagic.hpp>

using namespace Abysswire;

CHandshakeProtocolsMessage::CHandshakeProtocolsMessage(const std::vector<uint8_t>& data, size_t offset) {
    m_type = AW_MESSAGE_TYPE_HANDSHAKE_PROTOCOLS;

    try {
        if (data.at(offset + 0) != AW_MESSAGE_TYPE_HANDSHAKE_PROTOCOLS)
            return;

        if (data.at(offset + 1) != AW_MESSAGE_MAGIC_TYPE_ARRAY)
            return;

        if (data.at(offset + 2) != AW_MESSAGE_MAGIC_TYPE_VARCHAR)
            return;

        size_t needle = 3;

        auto [els, varIntLen] = g_messageParser->parseVarInt(data, offset + needle);

        needle += varIntLen;

        // max 2048 protocols per connection
        if (els >= 2048)
            return;

        m_protocols.resize(els);

        for (size_t i = 0; i < els; ++i) {
            auto [strLen, strLenLen] = g_messageParser->parseVarInt(data, offset + needle);

            m_protocols.at(i) = std::string_view{rc<const char*>(&data.at(offset + needle + strLenLen)), strLen};
            needle += strLen + strLenLen;
        }

        if (data.at(offset + needle) != AW_MESSAGE_MAGIC_END)
            return;

        m_len = needle + 1;

        if (Env::isTrace())
            m_data = std::vector<uint8_t>{data.begin() + offset, data.begin() + offset + m_len - 1};

    } catch (std::out_of_range& e) { m_len = 0; }
}

CHandshakeProtocolsMessage::CHandshakeProtocolsMessage(const std::vector<std::string>& protocols) {
    m_type = AW_MESSAGE_TYPE_HANDSHAKE_PROTOCOLS;

    m_data = {
        AW_MESSAGE_TYPE_HANDSHAKE_PROTOCOLS,
        AW_MESSAGE_MAGIC_TYPE_ARRAY,
        AW_MESSAGE_MAGIC_TYPE_VARCHAR,
    };

    { const auto& r_ = g_messageParser->encodeVarInt(protocols.size()); m_data.insert(m_data.end(), r_.begin(), r_.end()); } // GCC 14: no append_range

    for (const auto& p : protocols) {
        { const auto& r_ = g_messageParser->encodeVarInt(p.size()); m_data.insert(m_data.end(), r_.begin(), r_.end()); } // GCC 14: no append_range
        { const auto& r_ = p; m_data.insert(m_data.end(), r_.begin(), r_.end()); } // GCC 14: no append_range
    }

    m_data.emplace_back(AW_MESSAGE_MAGIC_END);
}
