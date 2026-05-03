// HelloWorldStructStubImpl.cpp
#include "HelloWorldStructStubImpl.hpp"

HelloWorldStructStubImpl::HelloWorldStructStubImpl() {
}

HelloWorldStructStubImpl::~HelloWorldStructStubImpl() {
}

void HelloWorldStructStubImpl::sayHello(const std::shared_ptr<CommonAPI::ClientId> _client,
        v0::commonapi::examples::HelloWorldStruct::UserData _user, sayHelloReply_t _reply) {
        
    std::string name = _user.getName();
    uint32_t id = _user.getId();

    std::stringstream messageStream;

    messageStream << "Hello " << name << " (ID: " << std::to_string(id) << ") !";
    std::cout << "sayHello('" << name << ", " << id << "'): '" << messageStream.str() << "'\n";

    _reply(messageStream.str());
};
