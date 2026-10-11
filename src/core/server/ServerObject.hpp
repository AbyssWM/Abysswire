#pragma once

#include <abysswire/core/implementation/Object.hpp>
#include <abysswire/core/implementation/Types.hpp>

#include "ServerClient.hpp"
#include "../../helpers/Memory.hpp"
#include "../wireObject/IWireObject.hpp"

namespace Abysswire {
    class CServerClient;

    class CServerObject : public IWireObject {
      public:
        CServerObject(SP<CServerClient> client);
        virtual ~CServerObject();

        virtual Abyssutils::Memory::CSharedPointer<IServerClient> client();

        virtual const std::vector<SMethod>&                      methodsOut();
        virtual const std::vector<SMethod>&                      methodsIn();
        virtual void                                             errd();
        virtual void                                             sendMessage(const IMessage&);
        virtual Abyssutils::Memory::CSharedPointer<IObject>       self();
        virtual Abyssutils::Memory::CSharedPointer<IServerSocket> serverSock();
        virtual bool                                             server();
        virtual void                                             error(uint32_t id, const std::string_view& message);

        WP<CServerClient>                                        m_client;

        void                                                     sendDestroyIfNeeded();
    };
};
