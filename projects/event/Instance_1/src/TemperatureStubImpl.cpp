// TemperatureStubImpl.cpp
#include "TemperatureStubImpl.hpp"
#include <iostream>

TemperatureStubImpl::TemperatureStubImpl() {
    // event initial value
    int initialData_event = 18;
    fireTemperatureEvent(initialData_event);
}

TemperatureStubImpl::~TemperatureStubImpl() {
}
