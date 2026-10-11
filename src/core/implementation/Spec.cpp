#include <abysswire/core/implementation/ClientImpl.hpp>
#include <abysswire/core/implementation/Spec.hpp>
#include <abysswire/core/implementation/ServerImpl.hpp>
#include <abysswire/core/implementation/Object.hpp>
#include <abysswire/core/ServerSocket.hpp>

using namespace Abysswire;

IProtocolSpec::~IProtocolSpec()                                 = default;
IProtocolServerImplementation::~IProtocolServerImplementation() = default;
IProtocolClientImplementation::~IProtocolClientImplementation() = default;
IServerClient::~IServerClient()                                 = default;