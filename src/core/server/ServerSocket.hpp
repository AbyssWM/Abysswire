#pragma once

#include <abysswire/core/ServerSocket.hpp>
#include <abyssutils/os/FileDescriptor.hpp>
#include "../../helpers/Memory.hpp"

#include <condition_variable>
#include <vector>
#include <thread>
#include <mutex>
#include <sys/poll.h>

namespace Abysswire {
    class CServerClient;

    class CServerSocket : public IServerSocket {
      public:
        CServerSocket();
        virtual ~CServerSocket();

        bool                                           attempt(const std::string& path);
        bool                                           attemptEmpty();

        virtual void                                   addImplementation(SP<IProtocolServerImplementation>&&);
        virtual bool                                   dispatchEvents(bool block);
        virtual int                                    extractLoopFD();
        virtual SP<IObject>                            createObject(SP<IServerClient> client, SP<IObject> reference, const std::string& object, uint32_t seq);
        virtual SP<IServerClient>                      addClient(int fd);
        virtual bool                                   removeClient(int fd);

        void                                           recheckPollFds();
        bool                                           dispatchNewConnections();
        bool                                           dispatchExistingConnections();
        bool                                           dispatchPending();
        void                                           dispatchClient(SP<CServerClient> client);
        void                                           clearEventFd();
        void                                           clearWakeupFd();
        void                                           clearFd(const Abyssutils::OS::CFileDescriptor& fd);
        size_t                                         internalFds();

        std::vector<SP<IProtocolServerImplementation>> m_impls;

        Abyssutils::OS::CFileDescriptor                 m_fd;
        Abyssutils::OS::CFileDescriptor                 m_exportFd, m_exportWriteFd;
        Abyssutils::OS::CFileDescriptor                 m_exitFd, m_exitWriteFd;
        Abyssutils::OS::CFileDescriptor                 m_wakeupFd, m_wakeupWriteFd;

        std::vector<pollfd>                            m_pollfds;
        std::vector<SP<CServerClient>>                 m_clients;

        WP<CServerSocket>                              m_self;

        bool                                           m_threadCanPoll = false;
        std::thread                                    m_pollThread;
        std::recursive_mutex                           m_pollmtx;
        std::mutex                                     m_exportPollMtx;
        std::condition_variable                        m_pollEventHandledCV;
        bool                                           m_pollEvent = false;

        bool                                           m_isEmptyListener = false;
        std::string                                    m_path;
    };
};
