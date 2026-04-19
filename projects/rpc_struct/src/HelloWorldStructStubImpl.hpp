// HelloWorldStructStubImpl.hpp
#ifndef HELLOWORLDSTRUCTSTUBIMPL_H_
#define HELLOWORLDSTRUCTSTUBIMPL_H_

#include <CommonAPI/CommonAPI.hpp>
#include <v0/commonapi/examples/HelloWorldStructStubDefault.hpp>

class HelloWorldStructStubImpl: public v0_1::commonapi::examples::HelloWorldStructStubDefault {

public:
    HelloWorldStructStubImpl();
    virtual ~HelloWorldStructStubImpl();

    virtual void sayHello(const std::shared_ptr<CommonAPI::ClientId> _client, v0::commonapi::examples::HelloWorldStruct::UserData _user, sayHelloReply_t _return);

};
#endif /* HELLOWORLDSTUBIMPL_H_ */
