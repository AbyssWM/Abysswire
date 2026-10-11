#pragma once

#include <vector>
#include <cstdint>
#include <abyssutils/os/FileDescriptor.hpp>

namespace Abysswire {
    struct SSocketRawParsedMessage {
        std::vector<uint8_t> data;
        std::vector<int>     fds;
        bool                 bad = false;
    };

    SSocketRawParsedMessage parseFromFd(const Abyssutils::OS::CFileDescriptor& fd);
};
