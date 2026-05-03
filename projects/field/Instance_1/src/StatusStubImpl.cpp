// StatusStubImpl.cpp
#include "StatusStubImpl.hpp"
#include <iostream>

StatusStubImpl::StatusStubImpl() {
    // field initial value
    setStatusAttribute(0);
}

StatusStubImpl::~StatusStubImpl() {
}

void StatusStubImpl::onRemoteStatusAttributeChanged() {
    std::cout << "Setter invoked" << std::endl;
}
