// StatusStubImpl.hpp
#ifndef STATUS_STUB_IMPL_HPP_
#define STATUS_STUB_IMPL_HPP_

#include <CommonAPI/CommonAPI.hpp>
#include <v0/commonapi/examples/StatusStubDefault.hpp>

class StatusStubImpl: public v0::commonapi::examples::StatusStubDefault {
public:
    StatusStubImpl();
    virtual ~StatusStubImpl();

    virtual void onRemoteStatusAttributeChanged() override;
};

#endif
