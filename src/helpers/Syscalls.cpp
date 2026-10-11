#include "Syscalls.hpp"

#include <poll.h>

using namespace Abysswire;

namespace {
    Abysswire::Syscalls::SHooks g_hooks;
}

int Abysswire::Syscalls::poll(pollfd* fds, nfds_t nfds, int timeout) {
    if (g_hooks.poll)
        return g_hooks.poll(fds, nfds, timeout);

    return ::poll(fds, nfds, timeout);
}

ssize_t Abysswire::Syscalls::sendmsg(int sockfd, const msghdr* msg, int flags) {
    if (g_hooks.sendmsg)
        return g_hooks.sendmsg(sockfd, msg, flags);

    return ::sendmsg(sockfd, msg, flags);
}

ssize_t Abysswire::Syscalls::recvmsg(int sockfd, msghdr* msg, int flags) {
    if (g_hooks.recvmsg)
        return g_hooks.recvmsg(sockfd, msg, flags);

    return ::recvmsg(sockfd, msg, flags);
}

void Abysswire::Syscalls::setHooks(const SHooks& hooks) {
    g_hooks = hooks;
}

void Abysswire::Syscalls::resetHooks() {
    g_hooks = {};
}
