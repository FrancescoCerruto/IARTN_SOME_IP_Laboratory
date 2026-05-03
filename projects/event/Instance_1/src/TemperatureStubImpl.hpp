// TemperatureStubImpl.hpp
#ifndef TEMPERATURE_STUB_IMPL_HPP_
#define TEMPERATURE_STUB_IMPL_HPP_

#include <CommonAPI/CommonAPI.hpp>
#include <v1/my_package/iartn/some_ip_demo/TemperatureStubDefault.hpp>

class TemperatureStubImpl: public v1::my_package::iartn::some_ip_demo::TemperatureStubDefault {
public:
    TemperatureStubImpl();
    virtual ~TemperatureStubImpl();
};

#endif
