#include "BindProtocol.hpp"
#include "../MessageType.hpp"
#include "../MessageParser.hpp"
#include "../../../helpers/Env.hpp"

#include <cstring>
#include <stdexcept>
#include <string_view>
#include <abysswire/core/types/MessageMagic.hpp>

using namespace Abysswire;

CBindProtocolMessage::CBindProtocolMessage(const std::vector<uint8_t>& data, size_t offset) {
    m_type = AW_MESSAGE_TYPE_BIND_PROTOCOL;

    if (offset > data.size() || data.size() - offset < 14)
        return;

    try {
        if (data.at(offset + 0) != AW_MESSAGE_TYPE_BIND_PROTOCOL)
            return;

        if (data.at(offset + 1) != AW_MESSAGE_MAGIC_TYPE_UINT)
            return;

        std::memcpy(&m_seq, &data.at(offset + 2), sizeof(m_seq));

        if (data.at(offset + 6) != AW_MESSAGE_MAGIC_TYPE_VARCHAR)
            return;

        size_t needle = 7;

        auto [strLen, varIntLen] = g_messageParser->parseVarInt(data, offset + needle);

        if (!varIntLen || strLen > data.size() - offset - needle - varIntLen)
            return;

        needle += varIntLen;

        m_protocol = std::string_view{rc<const char*>(&data.at(offset + needle)), strLen};

        needle += strLen;

        if (data.at(offset + needle) != AW_MESSAGE_MAGIC_TYPE_UINT)
            return;

        if (data.size() - offset - needle < 6)
            return;

        std::memcpy(&m_version, &data.at(offset + needle + 1), sizeof(m_version));

        if (!m_version)
            return;

        needle += 5;

        if (data.at(offset + needle) != AW_MESSAGE_MAGIC_END)
            return;

        m_len = needle + 1;

        if (Env::isTrace())
            m_data = std::vector<uint8_t>{data.begin() + offset, data.begin() + offset + m_len - 1};

    } catch (std::out_of_range& e) { m_len = 0; }
}

CBindProtocolMessage::CBindProtocolMessage(const std::string& protocol, uint32_t seq, uint32_t version) {
    m_type = AW_MESSAGE_TYPE_BIND_PROTOCOL;

    m_data = {
        AW_MESSAGE_TYPE_BIND_PROTOCOL, AW_MESSAGE_MAGIC_TYPE_UINT, 0, 0, 0, 0,
    };

    std::memcpy(&m_data[2], &seq, sizeof(seq));

    m_data.emplace_back(AW_MESSAGE_MAGIC_TYPE_VARCHAR);

    { const auto& r_ = g_messageParser->encodeVarInt(protocol.length()); m_data.insert(m_data.end(), r_.begin(), r_.end()); } // GCC 14: no append_range
    { const auto& r_ = protocol; m_data.insert(m_data.end(), r_.begin(), r_.end()); } // GCC 14: no append_range

    { const auto& r_ = std::vector<uint8_t>{AW_MESSAGE_MAGIC_TYPE_UINT, 0, 0, 0, 0}; m_data.insert(m_data.end(), r_.begin(), r_.end()); } // GCC 14: no append_range

    std::memcpy(&m_data[m_data.size() - 4], &version, sizeof(version));

    m_data.emplace_back(AW_MESSAGE_MAGIC_END);
}
