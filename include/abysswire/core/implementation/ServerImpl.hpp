#pragma once

#include <abyssutils/memory/SharedPtr.hpp>
#include <vector>
#include <cstdint>
#include <functional>

namespace Abysswire {
    class IProtocolSpec;
    class IObject;

    struct SServerObjectImplementation {
        std::string                                                     objectName = "";
        uint32_t                                                        version    = 0;
        std::function<void(Abyssutils::Memory::CSharedPointer<IObject>)> onBind;
    };

    class IProtocolServerImplementation {
      public:
        virtual ~IProtocolServerImplementation();

        virtual Abyssutils::Memory::CSharedPointer<IProtocolSpec>                            protocol()       = 0;
        virtual std::vector<Abyssutils::Memory::CSharedPointer<SServerObjectImplementation>> implementation() = 0;

      protected:
        IProtocolServerImplementation() = default;
    };
};