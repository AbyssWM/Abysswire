#include "NewObject.hpp"
#include "../MessageType.hpp"
#include "../MessageParser.hpp"
#include "../../../helpers/Env.hpp"

#include <cstring>
#include <stdexcept>
#include <abysswire/core/types/MessageMagic.hpp>

using namespace Abysswire;

CNewObjectMessage::CNewObjectMessage(const std::vector<uint8_t>& data, size_t offset) {
    m_type = AW_MESSAGE_TYPE_NEW_OBJECT;

    if (offset > data.size() || data.size() - offset < 12)
        return;

    try {
        if (data.at(offset + 0) != AW_MESSAGE_TYPE_NEW_OBJECT)
            return;

        if (data.at(offset + 1) != AW_MESSAGE_MAGIC_TYPE_UINT)
            return;

        if ((data.size() - offset - 2) < sizeof(m_seq))
            return;

        std::memcpy(&m_id, &data.at(offset + 2), sizeof(m_id));

        if (data.at(offset + 6) != AW_MESSAGE_MAGIC_TYPE_UINT)
            return;

        std::memcpy(&m_seq, &data.at(offset + 7), sizeof(m_seq));

        if (data.at(offset + 11) != AW_MESSAGE_MAGIC_END)
            return;

        m_len = 12;

        if (Env::isTrace())
            m_data = std::vector<uint8_t>{data.begin() + offset, data.begin() + offset + m_len - 1};

    } catch (std::out_of_range& e) { m_len = 0; }
}

CNewObjectMessage::CNewObjectMessage(uint32_t seq, uint32_t id) {
    m_type = AW_MESSAGE_TYPE_NEW_OBJECT;

    m_data = {
        AW_MESSAGE_TYPE_NEW_OBJECT, AW_MESSAGE_MAGIC_TYPE_UINT, 0, 0, 0, 0, AW_MESSAGE_MAGIC_TYPE_UINT, 0, 0, 0, 0, AW_MESSAGE_MAGIC_END,
    };

    std::memcpy(&m_data[2], &id, sizeof(id));
    std::memcpy(&m_data[7], &seq, sizeof(seq));
}
