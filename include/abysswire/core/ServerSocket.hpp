#pragma once

#include <abyssutils/memory/SharedPtr.hpp>

namespace Abysswire {
    class IProtocolServerImplementation;
    class IObject;

    class IServerClient {
      public:
        virtual ~IServerClient();

        virtual int getPID() = 0;

      protected:
        IServerClient() = default;
    };

    class IServerSocket {
      public:
        virtual ~IServerSocket() = default;

        static Abyssutils::Memory::CSharedPointer<IServerSocket> open(const std::string& path);

        // anonymous socket, you can add and remove clients manually
        static Abyssutils::Memory::CSharedPointer<IServerSocket> open();

        /*
            Add an implementation to the socket
        */
        virtual void addImplementation(Abyssutils::Memory::CSharedPointer<IProtocolServerImplementation>&&) = 0;

        /*
            Create an object after client's request
        */
        virtual Abyssutils::Memory::CSharedPointer<IObject> createObject(Abyssutils::Memory::CSharedPointer<IServerClient> client,
                                                                        Abyssutils::Memory::CSharedPointer<IObject> reference, const std::string& object, uint32_t seq) = 0;

        /*
            Synchronously dispatch pending events. Returns false on failure.
        */
        virtual bool dispatchEvents(bool block = false) = 0;

        /*
            Extract the loop FD. FD is owned by this socket, do not close it.
        */
        virtual int extractLoopFD() = 0;

        /*
            Add a client by fd manually. Abysswire takes ownership of the fd.
        */
        virtual Abyssutils::Memory::CSharedPointer<IServerClient> addClient(int fd) = 0;

        /*
            Remove a client by fd. Do not close the fd, abysswire will manage it.
            Returns true if it removed anything.
        */
        virtual bool removeClient(int fd) = 0;

      protected:
        IServerSocket() = default;
    };
};