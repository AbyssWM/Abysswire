#pragma once

#include <abyssutils/memory/SharedPtr.hpp>
#include <vector>
#include <cstdint>

namespace Abysswire {
    class IProtocolSpec;

    struct SClientObjectImplementation {
        std::string objectName = "";
        uint32_t    version    = 0;
    };

    class IProtocolClientImplementation {
      public:
        virtual ~IProtocolClientImplementation();

        virtual Abyssutils::Memory::CSharedPointer<IProtocolSpec>                            protocol()       = 0;
        virtual std::vector<Abyssutils::Memory::CSharedPointer<SClientObjectImplementation>> implementation() = 0;

      protected:
        IProtocolClientImplementation() = default;
    };
};