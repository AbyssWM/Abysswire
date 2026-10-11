#include <string_view>
#include <abysswire/core/implementation/ClientImpl.hpp>
#include <abysswire/core/implementation/Spec.hpp>
#include <abysswire/core/implementation/ServerImpl.hpp>
#include <abysswire/core/implementation/Object.hpp>
#include <abysswire/core/ServerSocket.hpp>

#include "../../helpers/Memory.hpp"

using namespace Abysswire;

IObject::~IObject() {
    if (m_onDestroy)
        m_onDestroy();
}

SP<IServerSocket> IObject::serverSock() {
    return nullptr;
}

SP<IClientSocket> IObject::clientSock() {
    return nullptr;
}

void IObject::setData(void* data) {
    m_data = data;
}

void* IObject::getData() {
    return m_data;
}

void IObject::setOnDestroy(std::function<void()>&& fn) {
    m_onDestroy = std::move(fn);
}

void IObject::error(uint32_t id, const std::string_view& message) {
    ;
}
