#include "GenericProtocolMessage.hpp"
#include "../MessageType.hpp"
#include "../MessageParser.hpp"
#include "../../../helpers/Env.hpp"
#include "../../../helpers/Log.hpp"

#include <cstring>
#include <stdexcept>
#include <abysswire/core/types/MessageMagic.hpp>

using namespace Abysswire;

CGenericProtocolMessage::CGenericProtocolMessage(const std::vector<uint8_t>& data, std::vector<int>& fds, size_t offset) {
    m_type = AW_MESSAGE_TYPE_GENERIC_PROTOCOL_MESSAGE;

    try {
        if (data.at(offset + 0) != AW_MESSAGE_TYPE_GENERIC_PROTOCOL_MESSAGE)
            return;

        if (data.at(offset + 1) != AW_MESSAGE_MAGIC_TYPE_OBJECT)
            return;

        std::memcpy(&m_object, &data.at(offset + 2), sizeof(m_object));

        if (data.at(offset + 6) != AW_MESSAGE_MAGIC_TYPE_UINT)
            return;

        std::memcpy(&m_method, &data.at(offset + 7), sizeof(m_method));

        size_t i = 11;
        while (data.at(offset + i) != AW_MESSAGE_MAGIC_END) {
            switch (sc<eMessageMagic>(data.at(offset + i))) {
                case AW_MESSAGE_MAGIC_TYPE_UINT:
                case AW_MESSAGE_MAGIC_TYPE_F32:
                case AW_MESSAGE_MAGIC_TYPE_INT:
                case AW_MESSAGE_MAGIC_TYPE_OBJECT:
                case AW_MESSAGE_MAGIC_TYPE_SEQ: i += 5; break;
                case AW_MESSAGE_MAGIC_TYPE_VARCHAR: {
                    auto [a, b] = g_messageParser->parseVarInt(data, offset + i + 1);
                    i += a + b + 1;
                    break;
                }
                case AW_MESSAGE_MAGIC_TYPE_ARRAY: {
                    const auto arrType    = sc<eMessageMagic>(data.at(offset + i + 1));
                    auto [arrLen, lenLen] = g_messageParser->parseVarInt(std::span<const uint8_t>{&data[offset + i + 2], data.size() - i});
                    size_t arrMessageLen  = 2 + lenLen;

                    if (arrLen >= 10000) {
                        Debug::log(TRACE, "GenericProtocolMessage: failed demarshaling array message, array max size is 10000.");
                        return;
                    }

                    switch (arrType) {
                        case AW_MESSAGE_MAGIC_TYPE_UINT:
                        case AW_MESSAGE_MAGIC_TYPE_F32:
                        case AW_MESSAGE_MAGIC_TYPE_INT:
                        case AW_MESSAGE_MAGIC_TYPE_OBJECT:
                        case AW_MESSAGE_MAGIC_TYPE_SEQ: {
                            arrMessageLen += 4 * arrLen;
                            break;
                        }
                        case AW_MESSAGE_MAGIC_TYPE_VARCHAR: {
                            for (size_t j = 0; j < arrLen; ++j) {
                                auto [strLen, strlenLen] =
                                    g_messageParser->parseVarInt(std::span<const uint8_t>{&data[offset + i + arrMessageLen], data.size() - i - arrMessageLen});
                                arrMessageLen += strLen + strlenLen;
                            }
                            break;
                        }
                        case AW_MESSAGE_MAGIC_TYPE_FD: {
                            for (size_t j = 0; j < arrLen; ++j) {
                                if (fds.empty())
                                    return;

                                m_fds.emplace_back(fds[0]);
                                fds.erase(fds.begin(), fds.begin() + 1);
                            }

                            i += 0;
                            break;
                        }
                        default: {
                            Debug::log(TRACE, "GenericProtocolMessage: failed demarshaling array message");
                            return;
                        }
                    }

                    i += arrMessageLen;
                    break;
                }
                case AW_MESSAGE_MAGIC_TYPE_FD: {
                    if (fds.empty()) {
                        Debug::log(TRACE, "GenericProtocolMessage: AW_MESSAGE_MAGIC_TYPE_FD but fd queue is empty");
                        return;
                    }

                    m_fds.emplace_back(fds[0]);
                    fds.erase(fds.begin(), fds.begin() + 1);

                    i += 1;
                    break;
                }
                default: {
                    Debug::log(TRACE, "GenericProtocolMessage: failed demarshaling array message");
                    return;
                }
            }
        }

        m_len = i + 1;

        m_dataSpan = std::span<const uint8_t>{data.begin() + 11 + offset, m_len - 11};

        if (Env::isTrace())
            m_data = std::vector<uint8_t>{data.begin() + offset, data.begin() + offset + m_len - 1};

    } catch (std::out_of_range& e) { m_len = 0; }
}

CGenericProtocolMessage::CGenericProtocolMessage(std::vector<uint8_t>&& data, std::vector<int>&& fds) : m_fds(std::move(fds)) {
    m_data = std::move(data);
    m_type = AW_MESSAGE_TYPE_GENERIC_PROTOCOL_MESSAGE;
}

const std::vector<int>& CGenericProtocolMessage::fds() const {
    return m_fds;
}

void CGenericProtocolMessage::resolveSeq(uint32_t id) {
    m_object = id;
    if (m_data.size() >= 6)
        std::memcpy(m_data.data() + 2, &id, sizeof(uint32_t));
}
