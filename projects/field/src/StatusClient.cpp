// StatusClient.cpp
#include <iostream>
#include <string>
#include <thread>
#include <csignal>
#include <atomic>

#ifndef _WIN32
#include <unistd.h>
#endif

#include <CommonAPI/CommonAPI.hpp>
#include <v0/commonapi/examples/StatusProxy.hpp>

using namespace v0::commonapi::examples;

// Atomic flag to stop threads safely
std::atomic<bool> keepRunning(true);

// SIGINT handler
void signalHandler(int signum) {
    std::cout << "\nKilling StatusClient ..." << std::endl;
    keepRunning = false;
}

int main() {
    CommonAPI::Runtime::setProperty("LogContext", "E01C");
    CommonAPI::Runtime::setProperty("LogApplication", "E01C");
    CommonAPI::Runtime::setProperty("LibraryBase", "Status");

    std::shared_ptr < CommonAPI::Runtime > runtime = CommonAPI::Runtime::get();

    std::string domain = "local";
    std::string instance = "commonapi.examples.Status";
    std::string connection = "Status_client";
    
    std::shared_ptr<StatusProxy<>> myProxy = runtime->buildProxy<StatusProxy>(domain, instance, connection);
    
    std::cout << "Checking availability!" << std::endl;
    while (!myProxy->isAvailable() && keepRunning)
        std::this_thread::sleep_for(std::chrono::microseconds(10));
    std::cout << "Available..." << std::endl;
    
    //SIGINT (Ctrl+C) handler declaration
    signal(SIGINT, signalHandler);
    
    // field notifier subscription
    myProxy->getStatusAttribute().getChangedEvent().subscribe([](const uint32_t& val) {
        std::cout << "Field update: " << val << std::endl;
    });

    // 2. field setter call
    CommonAPI::CallStatus callStatus;
    uint32_t newValue = 100;
    uint32_t version;
    
    std::cout << "Field setter(" << newValue << ")" << std::endl;
    myProxy->getStatusAttribute().setValue(newValue, callStatus, version, nullptr);

    if (callStatus == CommonAPI::CallStatus::SUCCESS) {
        std::cout << "Field setter successfully invoked" << std::endl;
    }

    while (keepRunning) {
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    return 0;
    
}
