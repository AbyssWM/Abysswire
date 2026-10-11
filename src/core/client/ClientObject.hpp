#pragma once

#include <abysswire/core/implementation/Object.hpp>
#include <abysswire/core/implementation/Types.hpp>

#include "ClientSocket.hpp"
#include "../../helpers/Memory.hpp"
#include "../wireObject/IWireObject.hpp"

namespace Abysswire {
    class CClientSocket;

    class CClientObject : public IWireObject {
      public:
        CClientObject(SP<CClientSocket> client);
        virtual ~CClientObject();

        virtual const std::vector<SMethod>&                      methodsOut();
        virtual const std::vector<SMethod>&                      methodsIn();
        virtual void                                             errd();
        virtual void                                             sendMessage(const IMessage&);
        virtual Abyssutils::Memory::CSharedPointer<IServerClient> client();
        virtual Abyssutils::Memory::CSharedPointer<IObject>       self();
        virtual Abyssutils::Memory::CSharedPointer<IClientSocket> clientSock();
        virtual bool                                             server();

        WP<CClientSocket>                                        m_client;
    };
};
